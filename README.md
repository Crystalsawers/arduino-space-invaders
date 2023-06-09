# Embedded Systems ID620 2023

## Arduino Project 2: Space Invaders Game

Project Timeframe: 31 May - 20 June 2023

## Main Components used:

- Arduino Uno board
- LCD 128x64 screen (specifically the ST7920 chipset/controller)
- Piezo Buzzer, for sound effects
- Breadboard
- Jumper Wires
- Joystick 
- Push buttons

## Hardware Prototype
![si_hardwareprototype](https://github.com/Crystalsawers/arduino-space-invaders/assets/87682183/f754e27b-aadc-4359-a287-7e3af817df15)

## Circuit Diagram

![image](https://github.com/Crystalsawers/arduino-space-invaders/assets/87682183/bb1e8051-74e3-4d9c-94ca-51bce28cc8ce)


## Narrative

I wanted to create some kind of game in an Arduino form with some type of LCD screen. The goal was to make it as entertaining and engaging as possible with adding some sound, a joystick to move the spaceship, and buttons to fire the missile to hit the alien. The score represented in my game is how many aliens you shoot before the aliens get to the bottom of the screen and/or hit the spaceship. There is an unlimited amount of missiles, and at the end of the game it shows how many aliens you shoot in the score and whether you win or you lose.

## References

- https://www.instructables.com/LCD-Invaders-a-Space-Invaders-Like-Game-on-16x2-LC/ 
- ChatGPT for the Space invaders idea
- https://www.arduino.cc/reference/en/libraries/u8g2/ 
- https://github.com/rmorenojr/SpaceInvaders_U8g2/blob/master/SpaceInvaders_U8g2.ino

Setup reference: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#st7920-128x64

With the help of ChatGPT I prompted this to customise my aliens using bytes:

- what would be the byte code for at least one space invaders alien, the most commonly used one?

and it's response came up with this:

![image](https://github.com/Crystalsawers/arduino-space-invaders/assets/87682183/45495ba9-c850-421d-b874-47861e6d471f)
