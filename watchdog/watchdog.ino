
#include <avr/wdt.h>//watch dog timer functions
#include <EEPROM.h>//library for using EEPROM
#define   WDTO_2S   7
int led=13;//pin 13 has LED
volatile int wSetting=1; // variable to store WDT setting, make it valatile since it will be use in ISR

void setup() {
  wdt_disable();//Datasheet recommend disabling WDT right away in case of low probability event
  pinMode(led,OUTPUT); //set up the LED pin to OUTPUT
  pinMode(2,INPUT_PULLUP);//setup pin 2 for input since we will be using it with out button
  getSettings();//start serial setting menu to choose WDT setting

  if(wSetting==1) setWDT(0b01000000);//set for intterrupt
  else if(wSetting==2) setWDT(0b00001000);//set for reset
  else setWDT(0b01001000);//set for interrupt and then reset
  
}

void getSettings(){
  Serial.begin(9600);
  Serial.println("We just start up....");
  byte b;//variable to store interrupt / reset tracker
  if(EEPROM.get(10,b)){//check if interrupt occured before reset
    Serial.println("...and an interrupt occured...");
    b=0;//reset variable
    EEPROM.put(10,b);
  }
  Serial.println("Select the WDI setting: : 1-> interrupt, 2-> reset, 3->interrupt and reset");
  //wait for user to input setting selection
  while(!Serial.available()){}

  wSetting=Serial.parseInt();//get entry and ensure it is valid

  if(wSetting<1 || wSetting>3){//if you put something else(not 1,2,3) let it be = 1
    wSetting=1;
  }
  Serial.print("WDT setting is");//to confirm what your setting
  Serial.print(wSetting);
  Serial.println();
  Serial.end();//do not want to mix serail comm and interrupt together
  
}

//this function setups and starts watchdog timer
void setWDT(byte sWDT){
  WDTCSR |= 0b00001000;
  WDTCSR = sWDT | WDTO_2S;//set WDT based user setting for 2 second interval
  wdt_reset();
}

  
//This is the interrupt service routine for WDT. It is called when WDT times out and is in interrupt mode
//This ISR must be in your Arduino sketch or else the WDT will not work correctly

ISR(WDT_vect){
  digitalWrite(led,HIGH);
  if(wSetting==3){
    byte b=1;
    EEPROM.put(10,b);
  }
}
  
void loop() {
  if(!digitalRead(2)){
    digitalWrite(led,LOW);
    wdt_reset();
  }

}
