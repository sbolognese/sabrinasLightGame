/*
LIGHTGAME
Requires Timer library found here http://playground.arduino.cc/Code/Timer#Installation  
*/
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <Timer.h>

const int KNOB_ONE_PIN   = A0 ; 
const int KNOB_TWO_PIN   = A1 ; 
const int KNOB_THREE_PIN = A2 ; 
const int BUTTON_PIN     =  2 ;  // The number of the pushbutton pin
const int NEOPIXEL_PIN   =  6 ;  // The number of the LED pin
const int NUMPIXELS      = 20 ;  // The number of LEDs in the LED strip. 

Timer t;

int player = 0; //player start position
int enemy = NUMPIXELS-1; //enemy start position
int enemyG; //enemy green value
int enemyR; //enemy red value
int enemyB; //enemy blue value
int playerG; //player green value
int playerR; //player red value
int playerB; //player blue value
int tol = 10; //tolerance for color guessing
int enemySpeed = 500; //speed for the timer to update the approaching enemy
int winningCondition = 15; //number of successful light combinations needed to win

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin(); // This initializes the NeoPixel library.
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Initialize the pushbutton pin as an input 
                                     // and high by default.
  Serial.begin(9600); // initialize serial communications at 9600 bps:
  startUp();
  t.every(enemySpeed, updateEnemy); //set the timer to run updateEnemy every enemySpeed milliseconds
}

void loop() {
  playGame();
  t.update();
}

//starting functions for when the game starts over
void startUp(){
  //set the starting conditions for the game
  randomSeed(analogRead(13));
  player = 0;
  enemy = NUMPIXELS-1;
  flashAll(0,29,0, 0,0,0, 3);
  flashOne(0,29,0, 0,0,0, 2, player);
  for (int k = player; k<winningCondition; k++){
      setOneLight(k, 0, 29, 0);
      delay(100);
  }
  flashOne(29,29,29,0,0,0, 3, winningCondition-1);
  for (int k = winningCondition-1; k>0; k--){
      setOneLight(k, 0, 0, 0);
      delay(100);
  }
  delay(200);
  flashOne(29,0,0, 0,0,0, 3, enemy);
  setEnemy();
}
void setEnemy(){
  //set the enemy's starting values
  double include = round(random(0,2));
  enemyG = round(29*(include));
  include = round(random(0,2));
  enemyR = round(29*(include));
  include = round(random(0,2));
  enemyB = round(29*(include));
  setOneLight(enemy, enemyR, enemyG, enemyB); //set enemy in position
  if ((enemyR ==0) && (enemyG ==0) && (enemyB == 0)){
    setEnemy();
  }
  else{
  delay(100);
  }
}

//gameplay functions
void playGame(){
  //take input and update the player
  int potOneValue   = analogRead(KNOB_ONE_PIN);
  int potTwoValue   = analogRead(KNOB_TWO_PIN);
  int potThreeValue = analogRead(KNOB_THREE_PIN);
  
  playerR   = map(potOneValue,0,1023,0,29) ; 
  playerG = map(potTwoValue,0,1023,0,29) ;
  playerB  = map(potThreeValue,0,1023,0,29) ;   

  if ((playerR == 0) && (playerG == 0) && (playerB == 0)){
    playerR = 1;
  }
  setPlayerColor();
  
  static int lastButtonState = HIGH; 
  int buttonState = digitalRead(BUTTON_PIN); // read the state of the button. 
  if (buttonState != lastButtonState ) { // Check to see if the button has changed state
    lastButtonState = buttonState ; // The button changed, update the last button state.
    if( buttonState == LOW ) { // check to see if the button is currently down.
      shootColor() ; 
    }
  }
}
void updateEnemy() {
  //set the enemy's light color on the strip and then trigger a color check
  setOneLight(enemy+1,0, 0, 0);
  setOneLight(enemy, enemyR, enemyG, enemyB);
  if (enemy==player){
    checkCollision();
  }
  enemy--;
}
void enemyApproach (){
  //place the enemy one light place closer to the player
  int tempPlayer = player;
  for ( int i=player; i<enemy; i++){
  setOneLight(tempPlayer, 0,0,0);
  tempPlayer++;
  setOneLight(tempPlayer, playerR, playerG, playerB);
  delay(200);
  }
  checkCollision();
}
void shootColor(){
  //shoot the player color at the enemy and check if there is a color match
  int tempPlayer = player;
  for ( int i=player; i<enemy; i++){
    setOneLight(tempPlayer, 0,0,0);
    tempPlayer++;
    setOneLight(tempPlayer, playerR, playerG, playerB);
    delay(50);
  }
  checkCollision();
}
void setPlayer(int lifeChange){
  //update the player values and reset the game if the player is out of lives
  if ((player == 0)&&(lifeChange ==(-1))){
    flashAll(29,0,0, 50,0,0, 3);
    startUp();
  }
  else{
    player += lifeChange;
    setPlayerColor();
  }
}
void setPlayerColor(){
  //fill in the player on the strip
  for (int k = player; k>=0; k--){
      setOneLight(player-k, playerR, playerG, playerB);
  }
}
void checkCollision(){
  //check if the player's RGB values are close enough to the enemy values based on tolerance "tol"
  if ((abs(playerR-enemyR)<tol)&&(abs(playerG-enemyG)<tol)&&(abs(playerB-enemyB)<tol)){
    flashAll(29,29,29, 0,0,0, 3);
    setPlayer(1);
    if (player+1 == winningCondition){
      youWin();
      startUp();
    }
  }
  else{
    flashAll(29,0,0, 0, 0, 0, 3);
    setPlayer(-1);
  }
  enemy = NUMPIXELS-1;
  setEnemy();
}

//visual functions for setting lights
void setAllLights(uint32_t r, uint32_t g, uint32_t b){
  //set every light on the strip to one color
  uint32_t color = strip.Color(r,g,b);
  for (int i=0; i< strip.numPixels(); i++){
    strip.setPixelColor(i, color);
  }
  strip.show();
}
void setOneLight(int light, uint32_t r, uint32_t g, uint32_t b){
  //set one light on the strip to one color
  uint32_t color = strip.Color(r,g,b);
  strip.setPixelColor(light, color);
  strip.show();
}
void flashAll(uint32_t r1, uint32_t g1, uint32_t b1, uint32_t r2, uint32_t g2, uint32_t b2, int numFlashes){
  //flash every light on the strip
  for (int k=0; k< numFlashes; k++){
    setAllLights(r1, g1, b1);
    delay(300);
    setAllLights(r2, g2, b2);
    delay(200);
  }
}
void flashOne(uint32_t r1, uint32_t g1, uint32_t b1, uint32_t r2, uint32_t g2, uint32_t b2, int numFlashes, int light){
  //flash one light on the strip
  for (int k=0; k< numFlashes; k++){
    setOneLight(light, r1, g1, b1);
    delay(300);
    setOneLight(light, r2, g2, b2);
    delay(200);
  }
}
void colorWipe(int r, int g, int b){
  for (int k = 0; k< NUMPIXELS; k++){
    setOneLight(k, r, g, b);
    delay(5);
  }
  delay(300);
  for (int k = NUMPIXELS-1; k>=0; k--){
    setOneLight(k, 0, 0, 0);
    delay(5);
  }
}
void youWin(){
   colorWipe(127,0,255);//purple
   colorWipe(255,0,255);//magenta
   colorWipe(255,0,0);//red
   colorWipe(255,127,0);//orange
   colorWipe(255,255,0);//yellow
   colorWipe(0,255,0);//green
   colorWipe(0,0,255);//blue
   delay(300);
}


