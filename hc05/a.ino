#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // CONNECT BT RX PIN TO ARDUINO 11 PIN | CONNECT BT TX PIN TO ARDUINO 10 PIN
//ต้องมี library ด้านบนนะ
//RX ไปที่ pin 11
//TX ไปที่ pin 10


//98d3:32:1158c7  ของ เบอร์ 1
//98d3:32:115705  ของ ที่ตู้

//Bluetooth address will show as this way: NAP: UAP: LAP(Hexadecimal)
//Example:
//Module Bluetooth address: 12: 34: 56: ab: cd: ef

//AT+ADDR? 
//+ADDR:1234:56:abcdef

//AT+ PSWD? 
//+PSWD:1234

//AT+PSWD=1111 
//OK
void setup() 
{
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH); 
  Serial.begin(9600);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);  // HC-05 default speed in AT command more
}

void loop()
{

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
    BTSerial.write(Serial.read());
}


