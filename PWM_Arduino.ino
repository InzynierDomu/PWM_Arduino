/**
 * @file PWM_Arduino.ino
 * @brief PWM Arduino controller - Main
 * @author by Szymon Markiewicz
 * @details http://www.inzynierdomu.pl/  
 * @date 11-2018
 */

#include <LiquidCrystal.h>
#include <LcdBarGraph.h>
#include <Servo.h>

#define NUMBER_OF_PWM 5   ///< Number of PWM outputs for Arduino Nano without PB3, PD3
//#define SERVO   ///< Flag for servo version compile

//LCD
const byte RS = 12;   ///< register select LCD pin
const byte EN = 16;   ///< enebale LCD pin
const byte D4 = 7;    ///< Data 4 LCD pin
const byte D5 = 14;   ///< Data 5 LCD pin
const byte D6 = 15;   ///< Data 6 LCD pin
const byte D7 = 4;    ///< Data 7 LCD pin
//Encoder
const byte CLK = 3;   ///< CLK signal encoder pin
const byte DT = 2;    ///< DT signal encoder pin
const byte BUTTON = 17;   ///< Butlin encoder button pin
//PWM outputs Arduino Nano    
const byte PWM[NUMBER_OF_PWM] = {5, 6, 9, 10, 11};    ///< Arduino Nano PWM output pins 
#ifdef SERVO
  Servo servoPWM[NUMBER_OF_PWM];  ///< Arduino Nano PWM output pins for servos
#endif

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);  ///< LCD 
LcdBarGraph lbg(&lcd, 16);                  ///< LCD for bar graph

unsigned long time = 0;   ///< Time for comparison CLK - DT encoder
int pwm[NUMBER_OF_PWM];   ///< PWMs values of duty cycle
int current_output = 0;   ///< Current controlled PWM

/**
 * @brief preparation to work, startup
 */
void setup() {
  pinMode(DT,INPUT); 
  pinMode(CLK,INPUT); 
  pinMode(BUTTON,INPUT_PULLUP); 

  attachInterrupt(digitalPinToInterrupt(DT), decrementation, LOW); 
  attachInterrupt(digitalPinToInterrupt(CLK), incrementation, LOW); 

  for(int i = 0; i < NUMBER_OF_PWM; i++)
  {
    pwm[i]=0;
  }

  #ifdef SERVO
    for(int i = 0; i < NUMBER_OF_PWM; i++)
    {
      servoPWM[i].attach(PWM[i]);
    }
  #endif 

  analog_refresh();
  
  lcd.begin(16, 2);
  startup_screen();
  lcd_refresh();
}

/**
 * @brief main loop
 */
void loop() {
  if (digitalRead(BUTTON)==0) 
  {
      current_output++;
      if (current_output == NUMBER_OF_PWM)
      current_output = 0;
      delay(300);
  }
  lcd_refresh();
  analog_refresh();
}

/**
 * @brief refresh LCD 
 */
void lcd_refresh(){
  lcd.setCursor(0, 0);
  lbg.drawValue(pwm[current_output],100);
  lcd.setCursor(0, 1);
  String output = "PWM=";
  output += pwm[current_output];
  output += "% Out=";
  output += current_output;
  output += "  ";
  lcd.print(output);
}

/**
 * @brief refresh PWM outputs value of duty cycle
 */
void analog_refresh(){
  for(int i = 0; i < NUMBER_OF_PWM; i++)
  {
    int retval = pwm[i];
    #ifdef SERVO
      retval = map(retval, 0, 100, 0, 180);
      servoPWM[i].write(retval);  
    #else /*SERVO*/
      retval = map(retval , 0, 100, 0, 255);
      analogWrite(PWM[i], retval);
    #endif 
  }  
}

/**
 * @brief decrementation interrupt from encoder   
 */
void decrementation()
{
 if ((millis() - time) > 3 &&  pwm[current_output] > 0)
 pwm[current_output] --; 
 time = millis();
}

/**
 * @brief incrementation interrupt from encoder   
 */
void incrementation()
{
 if ((millis() - time) > 3 &&  pwm[current_output] < 100) 
 pwm[current_output] ++ ;
 time = millis();
}

/**
 * @brief startup screen 
 */
void startup_screen()
{
  lcd.setCursor(0, 0);
  lcd.print(" PWM controller");
  lcd.setCursor(0, 1);
  lcd.print(" inzynier domu");
  delay(3000);
  lcd.clear();
}
