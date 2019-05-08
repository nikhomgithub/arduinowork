#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'A','0','B'}
};

byte colPins[COLS] = {5,6,7};
byte rowPins[ROWS] = {11,10,9,8}; 

Keypad keypadA = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char inKey;//เธฃเธฑเธ�เธ�เน�เธฒ keypad
String Stringkey;//เธ�เธงเธฒเธกเธขเธฒเธง

void setup()   
{
  Serial.begin(9600);
  pinMode(12,OUTPUT);//Green LED
  pinMode(13,OUTPUT);//Relay of solinoid 
}   
                     
void loop()           
{   
    digitalWrite(12,LOW);
    digitalWrite(13,LOW);
    inKey=keypadA.getKey();             
    delay(10);
         if((inKey=='0')or(inKey=='1')or(inKey=='2')or(inKey=='3')or(inKey=='4')or(inKey=='5')or(inKey=='6')or(inKey=='7')or(inKey=='8')or(inKey=='9'))
         {
         Stringkey+=inKey;
         Serial.println(Stringkey);
         }
        else if((inKey=='A')or(inKey=='B'))
         {
              if(Stringkey=="1878")
                { 
                Stringkey="";
                Serial.println("Correct");
                digitalWrite(12,HIGH);  
                digitalWrite(13,HIGH);
                delay(7000);
                digitalWrite(12,LOW);
                digitalWrite(13,LOW);

              
                }
             else
                {
                Stringkey="";
                Serial.println("Wrong");
                  digitalWrite(13,HIGH);
                delay(200);
                digitalWrite(13,LOW);
                delay(200);
                digitalWrite(13,HIGH);
                delay(200);
                digitalWrite(13,LOW);
                    
                }
         }
   
}

