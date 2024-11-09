# LCD_Arduino_Game
Fun game made for Arduino and LCD 16x2.

The goal of the game is to collect randomly appearing objects (apples) by riding into them with a controllable vehicle. The car is moved by the input buttons - one for forward movement and two for clockwise/counter-clockwise rotation (note that the display loops - going through the right "wall" leads to appearing next to the left one.) With each collected object (gained point) the player has less time before the next one disappears - if it does, the game is over and the score and highscore information (saved in the EEPROM memory) is displayed. These values can be zeroed by pressing the movement button. To start the game again, the user has to reset the board via board reset button.    

All the graphic objects are custom made and converted into characters showable by the 16x2 LCD display (connected to Arduino via I2C converter).  

Additionaly, player actions have sound effects, achieved by various tones of the buzzer, operated with the use of PWM signal from one of the pins.  

Here you can see short demo presentation of the game (sadly, the display is slightly corrupted in certain places): https://youtu.be/NS5YVRj8-JI

![scheme](https://github.com/user-attachments/assets/0a8b2675-7a2c-4806-b0e1-110ab4e81a3e)

The project was made for fun and it is no longer developed by me, however it could be somewhat improved, e.g. by allowing to manually customize the difficulty level or by soldering the pieces together onto some circuit board. 
