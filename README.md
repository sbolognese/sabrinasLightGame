###############################################################################
                           SABRINA'S LIGHT GAME
STARTED ON: JAN 29, 2017
COMPLETED:  FEB  5, 2017
EST.TIME: 7HR
###############################################################################

REQUIREMENTS:
  To play, you will need the timer library.
  http://playground.arduino.cc/Code/Timer
  You will also need
    - a neopixel light strip
    - three knobs
    - a button
  You will need to adjust the input locations.

GAMEPLAY:
  STARTING SEQUENCE:
    - The neopixel strip will flash green to inform you that the game is about
      to begin.
    - The light at position 0 will flash green. This is your player character's
      starting position.
    - The strip will light up from 0 to light 13 and flash white at 15. This is
      the winning condition. You must reach this light to win.
    - The strip will reduce down to just the player light and then flash red at
      the opposite end of the strip. This is the enemy.
    - The game now begins.

HOW TO WIN:
    - The goal is to twist the three knobs, which control the RGB values of the
      player controlled light, until the player's color matches the approaching
      enemy color.
    - The color combinations are ON/OFF combinations of RGB as follows:
        -> RED    = R ON,  G OFF, B OFF
        -> GREEN  = R OFF, G ON,  B OFF
        -> BLUE   = R OFF, G OFF, B ON
        -> PURPLE = R ON,  G OFF, B ON
        -> CYAN   = R OFF, G ON,  B ON
        -> YELLOW = R ON,  G ON,  B OFF
        -> WHITE  = R ON,  G ON,  B ON
    - If a knob is left in a mid-way state (say, 50% on), it will likely result
      in a failed match.
    - When ready, the player can either wait for enemy to reach the player OR
      press the button and shoot the color at the enemy.
    - A correctly matched color will cause the light strip to flash white and 
      the player will advance one light forward, with the lights behind it fill
      -ing with the same color.
    - An incorrectly matched color will cause the light strip to flash red and
      the player will move back one space, losing a "life".
    - If a player fails to match the color when there is only one life, the game
      flashes red, then alternating brightness of reds, then plays the startup
      sequence and the game begins again.
