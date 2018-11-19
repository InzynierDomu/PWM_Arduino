#include <LiquidCrystal.h>
#include <LcdBarGraph.h>

#define NUMBER_OF_PWM 5

//LCD
const byte RS = 12;
const byte EN = 16; 
const byte D4 = 7; 
const byte D5 = 14; 
const byte D6 = 15; 
const byte D7 = 4;
//Encoder
const byte CLK = 3;
const byte DT = 2; 
const byte BUTTON = 17;
//PWM outputs Arduino Nano
const byte PWM[NUMBER_OF_PWM] = {5, 6, 9, 10, 11};

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
LcdBarGraph lbg(&lcd, 16);

unsigned long time = 0; 
int pwm[NUMBER_OF_PWM];
int current_output = 0;

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

  analog_refresh();
  
  lcd.begin(16, 2);
  start_screen();
  lcd_refresh();
}

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

void analog_refresh(){
  for(int i = 0; i < NUMBER_OF_PWM; i++)
  {
    int retval = pwm[i];
    retval = map(retval , 0, 100, 0, 255);
    analogWrite(PWM[i], retval);
  }  
}

void decrementation()
{
 if ((millis() - time) > 3 &&  pwm[current_output] > 0)
 pwm[current_output] --; 
 time = millis();
}
 
void incrementation()
{
 if ((millis() - time) > 3 &&  pwm[current_output] < 100) 
 pwm[current_output] ++ ;
 time = millis();
}

void start_screen()
{
  lcd.setCursor(0, 0);
  lcd.print(" PWM controller");
  lcd.setCursor(0, 1);
  lcd.print(" inzynier domu");
  delay(3000);
  lcd.clear();
}
