#include <LiquidCrystal.h>
// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int blue = 0;  // for incoming serial data
int moisture_sensor  = A0;
int moisture;
int limit = 500;
int pump = 3;
int led = 13;
int pump_time = 3000;
unsigned long time_now = 0;
unsigned long millismaxval = 4000000000;      //4,294,967,295
extern volatile unsigned long timer0_millis;

void setup()
{
  lcd.begin(16, 2);
  lcd.clear();
  
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(moisture_sensor,INPUT);
  pinMode(pump,OUTPUT);
  pinMode(led,OUTPUT);
}

void loop() 
{
  moisture = analogRead(moisture_sensor);
  Serial.print("Moisture =");
  Serial.print(moisture);
  Serial.println();
  lcd.setCursor(0,0);
  lcd.print ("Moisture =");
  lcd.setCursor(11,0);
  lcd.print (moisture);
  while(Bluetooth()==1)
  {
    Pump(1);
    blue = 0;
    break;
  }
  while(Bluetooth()==0)
  {
    if(moisture<limit)
    {
      Pump(1);
      break;  
    }
    else
    { 
      Pump(0);
      break;
    }
  }
  if(millis() >= millismaxval){
   // unsigned long millis = 0; I dont know if it works
    time_now = 0;
    noInterrupts ();
    timer0_millis = 0;
    interrupts ();
    } 
}

int Bluetooth()
{
   // send data only when you receive data:
   if(Serial.available() > 0) 
   {
    // read the incoming byte:
    blue = Serial.read();

    // say what you got:
    Serial.print("Bluetooth");
    Serial.println(blue);
     lcd.clear();
     lcd.setCursor(0,1);
     lcd.print ("BT is ");
     lcd.setCursor(5,1);
     lcd.print (blue);
    if(blue == 48)
      return 0;
    if(blue == 49)
      return 1;
  }
  else 
  return 0;
}

void Pump(int stat)
{
  if(stat == 0)
  {
    digitalWrite(pump,stat);
    digitalWrite(led,stat);
    Serial.println("Pump OFF");
  }
  if(stat == 1)
  {
    digitalWrite(pump,stat);
    digitalWrite(led,stat);
    Serial.println("Pump ON");
    
    // run LCD print code here
    lcd.clear();
    lcd.setCursor(9,1);
    lcd.print ("Pump ON");
    
    // delay(pump_time); replace with millis instead of delay  to make bluetooth work well
    if(millis() >= time_now + pump_time){
        time_now += pump_time;


  }
 
  
  /* ********************************************************************** REFERENCE ONLY *******************************
             from https://www.norwegiancreations.com/2017/09/arduino-tutorial-using-millis-instead-of-delay/
             int period = 1000;
             unsigned long time_now = 0;
 
             void setup() {
             Serial.begin(115200);
             }
 
             void loop() {
             if(millis() >= time_now + period){
             time_now += period;
             Serial.println("Hello");
              }
   
               //Run other code
  ********************************************************************** REFERENCE ONLY **********************************
  
  https://www.norwegiancreations.com/2018/10/arduino-tutorial-avoiding-the-overflow-issue-when-using-millis-and-micros/
  
  */
  }
}
