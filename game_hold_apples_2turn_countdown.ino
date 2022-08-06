#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

//Pins definition

#define BACKLIGHT_PIN 3 //one way

const int LED_PIN = 13; //another
const int MOVEMENT_PIN = 3;
const int ROTATION_PIN = 2;
const int RIGHT_ROTATION_PIN = 4;
const int buzzerPin = 5;

//Variables for vehicle coordinates
int car_x = 0;
int car_y = 0;
int last_car_x = car_x;
int last_car_y = car_y;

//Variables for vehicle orientation
enum direction{LEFT,UP,RIGHT,DOWN};
direction rotate = LEFT;
direction last_rotate = rotate;

//Defining LCD display object
LiquidCrystal_I2C  lcd(0x27,16,2);

//Defining custom characters
byte Character[8] =
{
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

byte Car[8] =
{
0b00000,
0b00000,
0b10010,
0b11111,
0b11111,
0b10010,
0b00000,
0b00000
};

byte Car_right[8] =
{
0b00000,
0b00000,
0b01001,
0b11111,
0b11111,
0b01001,
0b00000,
0b00000
};

byte Car_down[8] =
{
0b00000,
0b11111,
0b01110,
0b01110,
0b11111,
0b01110,
0b00100,
0b00000
};

byte Car_up[8] =
{
0b00000,
0b00100,
0b01110,
0b11111,
0b01110,
0b01110,
0b11111,
0b00000
};

byte empty[8] =
{
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000,
0b00000
};

byte apple[8] =
{
0b00000,
0b00000,
0b00100,
0b01110,
0b01110,
0b00100,
0b00000,
0b00000
};

//Random placement of first apple

int apple_x = random(16);
int apple_y = random(2);

//Initializing of the counter 
int apple_counter = 0;
int apple_max_lasting_time = 25; //Increase this value to make the game progress faster and become harder

//Additional variables
int points = 0;
bool temp_lock = true;


void setup()
{
  //LCD initialization and parameters
  lcd.init();  
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.createChar(0, Character);
  lcd.createChar(1, Car);
  lcd.createChar(2, empty);
  lcd.createChar(4, Car_right);
  lcd.createChar(5, Car_down);
  lcd.createChar(3, Car_up);
  lcd.createChar(8, apple);
  lcd.home();

  //Pins setup
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOVEMENT_PIN, INPUT_PULLUP);
  pinMode(ROTATION_PIN, INPUT_PULLUP);
  pinMode(RIGHT_ROTATION_PIN, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  delay(300);

  //Initialization of car position
  rotate = LEFT;
  car_x = 0;
  car_y = 0;
  lcd.write(byte(1));  
  delay(2000);
  
  //Uncomment the line below only for first execution, memory initialization
  //EEPROM.write(0,0);
  
  tone(buzzerPin, 500, 500);
  
  randomSeed(analogRead(0));
  apple_x = random(16);
  apple_y = random(2);
  
}  


//Function used to summarize the game
void game_summary(int points)
{
   lcd.clear();
   tone(buzzerPin, 1000, 170);
   lcd.print("Score: " + String(points));   
   lcd.setCursor(0,1);
   int temp = EEPROM.read(0);
   if(points > temp)
   {
     EEPROM.update(0,points);
     lcd.print("NEW HIGHSCORE!");  
   }
   else
   {
     lcd.print("Highscore: " + String(temp));  
   }
}

//Enabling this function allows to reset the highscore WARNING, EEPROM MEMORY HAS LIMITED USE!
void highscore_reset()
{
   lcd.clear();
   lcd.home(); 
   EEPROM.update(0,0);
   int temp = EEPROM.read(0);   
   lcd.print("Score: " + String(temp)); 
   lcd.setCursor(0,1);
   lcd.print("Highscore: " + String(temp));       
}

void loop()
{ 
  delay(250);
  if(apple_counter == 0) //If next apple is about to show up
  { 
    if(temp_lock == false)// Check if the previous one was collected
    {
      game_summary(points); //If not, summarize the game
      while(1)//Endless waiting for reset, can be interrupted by resetting the highscore
      {
        delay(1000);
        if(digitalRead(MOVEMENT_PIN) == LOW)
        {
          highscore_reset();   
        }    
      }
    }

    //Proceed if the player collected the previous apple

    //Randomize the next position
    apple_x = random(16);
    apple_y = random(2);

    //Randomize again if it is the same position as that of the car
    if(apple_x == last_car_x && apple_y == last_car_y)
    {
       apple_x = random(16);
       apple_y = random(2);
    }
    temp_lock = false;

    //Speed up the game, decrease time for the next apple to show up
    apple_counter = apple_max_lasting_time;
    --apple_max_lasting_time;
    
    if(apple_max_lasting_time == 0)//Probably this section will never do anything, provided just for basic error handling
    {
      ++apple_max_lasting_time;
    }  

    //Drawing the next apple
    lcd.setCursor(apple_x, apple_y);
    lcd.write(byte(8));  
  }
  
  //Simulation of passing time
  --apple_counter;

  
  if(digitalRead(MOVEMENT_PIN) == LOW) //If car is moving
  {
    tone(buzzerPin, 1200, 150);
    lcd.setCursor(last_car_x, last_car_y);
    lcd.write(byte(2)); //erase the previous position
    //lcd.setCursor(car_x, car_y);//move to the next one
    switch(rotate){
      case LEFT:
        ++car_x;
        if(car_x == 16){car_x = 0;} //out of bounds handling
        if(car_x == -1){car_x = 15;}
        lcd.setCursor(car_x, car_y);//move to the next one                
        lcd.write(byte(1));
        break;
      case UP:
        ++car_y;
        if(car_y == 2){car_y = 0;}
        if(car_y == -1){car_y = 1;}
        lcd.setCursor(car_x, car_y);//move to the next one        
        lcd.write(byte(5));
        break;
      case DOWN:
        --car_y;
        if(car_y == 2){car_y = 0;}
        if(car_y == -1){car_y = 1;}
        lcd.setCursor(car_x, car_y);//move to the next one         
        lcd.write(byte(3));
        break;
      case RIGHT:
        --car_x;
        if(car_x == 16){car_x = 0;}
        if(car_x == -1){car_x = 15;}
        lcd.setCursor(car_x, car_y);//move to the next one   
        lcd.write(byte(4));
        break;
      default:
        break;
    }
    last_car_y = car_y;
    last_car_x = car_x;
    if(apple_x == last_car_x && apple_y == last_car_y)
    {
      ++points;
      tone(buzzerPin, 900, 120);
      temp_lock = true;
    }       
  }
  else if(digitalRead(ROTATION_PIN) == LOW || digitalRead(RIGHT_ROTATION_PIN) == LOW)//If car is rotating
  {
    tone(buzzerPin, 800, 150);
    if(digitalRead(ROTATION_PIN) == LOW) //left
    {
      if(rotate == DOWN){rotate = LEFT;}
      else{rotate = rotate + 1;}
    }
    else if(digitalRead(RIGHT_ROTATION_PIN) == LOW) //right
    {
      if(rotate == LEFT){rotate = DOWN;}
      else{rotate = rotate - 1;}      
    }
    lcd.setCursor(last_car_x, last_car_y);//stay in place but rotate
    switch(rotate){
      case LEFT:     
        lcd.write(byte(1));
        break;
      case UP:
        lcd.write(byte(5));
        break;
      case DOWN:
        lcd.write(byte(3));
        break;
      case RIGHT:
        lcd.write(byte(4));
        break;
      default:
        break;
    }      
  } 
}
