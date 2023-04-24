#include <Wire.h>
#include <Keypad.h>
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

const byte numRows = 4;        
const byte numCols = 4;   
int Button = 0 ;   

char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char keypressed;                 
char code[] = {'1','2','3','4'}; 
char check1[sizeof(code)];  
char check2[sizeof(code)];  
short a = 0, i = 0, s = 0, j = 0; 

byte rowPins[numRows] = {2, 3, 4, 5}; 
byte colPins[numCols] = {6, 7, 8, 9}; 

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
Servo myservo;

int LDR_Val=0;    
#define sensor A0   
#define LIGHT 13

void setup()
{
  lcd.init();                      
  lcd.backlight();
  lcd.begin (16, 2);
  lcd.setCursor(0, 0);
  lcd.print("LONG PRESS * TO");
  lcd.setCursor(0 , 1);

  lcd.print("ENTER THE CODE");      
  pinMode(Button, INPUT);
  myservo.attach(11);
  myservo.write(0);
  
  Serial.begin(9600);
  pinMode(LIGHT, OUTPUT);                
 
 

}
void loop()
{

  keypressed = myKeypad.getKey();              
  if (keypressed == '*') {                    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("*ENTER THE CODE*");            
    ReadCode();                         
    if (a == sizeof(code))        
      OpenDoor();                   
    else {
      
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("CODE"); 
      lcd.setCursor(6, 0);
      lcd.print("INCORRECT");
      lcd.setCursor(15, 1);
      lcd.print(" ");
      lcd.setCursor(4, 1);
      lcd.print("ALERT !!!");
      

    }
  
  }
    LDR_Val = analogRead(sensor);  
    Serial.print("LDR Output Value: ");
    Serial.println(LDR_Val);   
    if(LDR_Val < 100) {    
        Serial.println(" High intensity LAMP OFF ");
        digitalWrite(LIGHT,LOW); 
        }
    else  {
       Serial.println("LOW Intensity LAMP ON");
        digitalWrite(LIGHT,HIGH);
         
        }
    delay(1000);   
    
}

void ReadCode() {                
  i = 0;                    
  a = 0;
  j = 0;

  while (keypressed != 'A') {                                  
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {     
      lcd.setCursor(j, 1);                                 
      lcd.print("*");
      j++;
      if (keypressed == code[i] && i < sizeof(code)) {      
        a++;
        i++;
      }
      else
        a--;                                               
    }
  }
  keypressed = NO_KEY;

}

void OpenDoor() {            
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Access Granted");
  lcd.setCursor(4, 1);
  lcd.print("WELCOME!!");
  myservo.write(90);
  delay(3000);
  //TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Temp = 38 C");
  lcd.setCursor(1,1);
  lcd.print("Humidity = 64 %");
  delay(3000);
  myservo.write(0);
}
