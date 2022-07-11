#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

Servo myservo;

const byte ROWS = 4;
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13,12,11,10}; 
byte colPins[COLS] = {9,8,7,6}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

SoftwareSerial sim800l(2, 3);
int otp;
String otpstring ="";
int pos=0;
void setup()
{
 // pinMode(5,LOW);
  myservo.attach(5); 
  sim800l.begin(4800);   
  lcd.init(); 
  lcd.backlight();
  sim800l.println("AT"); 
  delay(500);
  sim800l.println("AT+CSQ"); 
  delay(1000);
  myservo.write(0);
  

}

void loop()
{
  
  myservo.write(0); 
  lcd.setCursor(0,0);
  lcd.print("Parcel Box Lock");
  lcd.setCursor(0,1);
  lcd.print("Press * for OTP:");
  if(customKeypad.getKey()=='*')
  {
  otp = random(1000,9999); 
  otpstring=String(otp);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("OTP Sent to");
  lcd.setCursor(0,1);
  lcd.print("Customer: ");
  SendSMS();  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter OTP :");
  getotp();
  }

}

void getotp(){
  
  String y="";
  int a=y.length();
  while (a<4)
 {
  char customKey = customKeypad.getKey();
  if (customKey){
    lcd.setCursor(0,1);
    y=y+customKey;
    lcd.print(y);
    a=y.length();
  }
}
  
   if (otpstring==y)
   {
 //   myservo.write(pos);
    lcd.setCursor(0,0);
    lcd.print("Access Granted");
    lcd.setCursor(0,1);
    lcd.print("Box Opening");
//    delay(1000);
//     for (pos = 0; pos <= 160; pos += 1) { 
    myservo.write(140);             
//    delay(15);                      
//  }
    delay(5000);
//   for (pos = 160; pos >= 0; pos -= 1) { 
    myservo.write(0);             
//    delay(15);                   
//  }
    }
else
   {
    lcd.setCursor(0,0);
    lcd.print("Access Failed");
    lcd.setCursor(0,1);
    lcd.print("Try Again !!!");
    delay(3000);}
    
  
}
void SendSMS()
{
  Serial.println("Sending SMS...");              
  sim800l.print("AT+CMGF=1\r");                   
  delay(100);  
  sim800l.print ("AT+CSMP=17,167,0,0\r");
  delay(500);
  sim800l.print("AT+CMGS=\"+918709901594\"\r");  
  delay(500);
  sim800l.print("Your OTP is "+otpstring+"  to unlock box");      
  delay(500);
  sim800l.print((char)26);
  delay(500);
   sim800l.println();
  Serial.println("Text Sent.");
  delay(500);
}
