#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  
// Set the LCD I2C address
// 0x3F or 0x27

#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 3; 
char keys[ROWS][COLS] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'A','0','B'}
};

byte colPins[COLS] = {5,6,7};
byte rowPins[ROWS] = {11,10,9,8}; 

Keypad keypadA = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char inKey;
String Stringkey,Stringkey1,Stringkey2;

long mode,time1,time2,inputStatus;
//String Stringkey;//


void setup()   /*----( SETUP: RUNS ONCE )----*/
{/*
  pinMode(5,INPUT);pinMode(6,INPUT);pinMode(7,INPUT);
  pinMode(8,INPUT);pinMode(9,INPUT);pinMode(10,INPUT);
  pinMode(11,INPUT);
  digitalWrite(5,LOW);digitalWrite(6,LOW);digitalWrite(7,LOW);
  digitalWrite(8,LOW);digitalWrite(9,LOW);digitalWrite(10,LOW);
  digitalWrite(11,LOW);*/
  
  pinMode(3,OUTPUT);//Motor
  pinMode(4,OUTPUT);//Gun
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Setting Up");
  delay(1000); 
  lcd.clear();lcd.print("??");
}

void machineStart()
{
         if(mode==1)//Motor
            {
             lcd.clear();lcd.print("Mode 1 :");
             lcd.setCursor(0,1);lcd.print(">> Test");delay(1000);
             lcd.clear();lcd.setCursor(0,1);lcd.print("..Test Motor");
             digitalWrite(4,HIGH);//Motor
             delay(time1*1000);
             lcd.clear();lcd.setCursor(0,1);lcd.print("..Test Gun");
             digitalWrite(3,LOW);//Gun
             delay(time2*1000);
             digitalWrite(3,LOW);digitalWrite(4,LOW);
             lcd.clear();lcd.print("Finish !!");delay(2000);
             inputStatus=4;
            }
            
            if(mode==2)//Motor and Gun
            {
             lcd.clear();lcd.print("Mode 2 :");
             lcd.setCursor(0,1);lcd.print(">> Gun & Motor");delay(1000);
             lcd.clear();lcd.setCursor(0,1);lcd.print("..Motor");
             digitalWrite(4,HIGH);//motor
             delay(time1*1000);
             lcd.clear();lcd.setCursor(0,1);lcd.print("..Gun");
             digitalWrite(3,HIGH);//Gun
             delay(time2*1000);
             digitalWrite(3,LOW);digitalWrite(4,LOW);
             lcd.clear();lcd.print("Finish !!");delay(2000);
             inputStatus=4;
            }
            
            if(mode==3)//Gun
            {
             lcd.clear();lcd.print("Mode 3 :");
             lcd.setCursor(0,1);lcd.print(">> Gun");delay(1000);
             lcd.clear();lcd.setCursor(0,1);lcd.print("..Gun");
             digitalWrite(3,HIGH);//Gun
             delay(time2*1000);
             digitalWrite(3,LOW);digitalWrite(4,LOW);
             lcd.clear();lcd.print("Finish !!");delay(2000);
             inputStatus=4;
            }
            
}

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{     
 
    digitalWrite(3,LOW);digitalWrite(4,LOW);
    inKey=keypadA.getKey();             
    delay(2);
    
    if(inKey=='A')//Reset Parameter
    {
        time1=0;time2=0;mode=0;inputStatus==0;
        lcd.clear();lcd.print("Set Mode ??");
              
        Stringkey="";
        while(inputStatus==0)//Mode==============
           {
             inKey=keypadA.getKey();delay(2);
             if(inKey=='A')
             {lcd.clear();lcd.print("Reset");delay(500);
              lcd.clear();lcd.print("??"); time1=0;time2=0;mode=0;inputStatus=0;Stringkey="";break;}
             if(inKey=='B'){mode=Stringkey.toInt();inputStatus=1;
                           lcd.clear();lcd.print("Mode = ");
                           lcd.setCursor(7,0);lcd.print(mode);delay(1000);
                           lcd.clear();lcd.setCursor(0,0);lcd.print("Time 1 = ??");                    
                           break;}
             if((inKey=='0')or(inKey=='1')or(inKey=='2')or(inKey=='3')or(inKey=='4')or(inKey=='5')or(inKey=='6')or(inKey=='7')or(inKey=='8')or(inKey=='9'))
             {Stringkey+=inKey;lcd.clear();lcd.setCursor(0,0);lcd.print("Mode = ");lcd.setCursor(0,1);lcd.print(Stringkey);}
           }
        
        Stringkey="";  
        while(inputStatus==1)//Time1============
           {
             inKey=keypadA.getKey();delay(2);
             if(inKey=='A')
             {lcd.clear();lcd.print("Reset");delay(500);
              lcd.clear();lcd.print("??");time1=0;time2=0;mode=0;inputStatus=0;Stringkey="";break;}
             if(inKey=='B'){time1=Stringkey.toInt();inputStatus=2;
                            lcd.clear();lcd.print("Time 1 =");
                            lcd.setCursor(9,0);lcd.print(time1);delay(1000);
                            lcd.clear();lcd.setCursor(0,0);lcd.print("Time 2 = ??");
                            break;}
             if((inKey=='0')or(inKey=='1')or(inKey=='2')or(inKey=='3')or(inKey=='4')or(inKey=='5')or(inKey=='6')or(inKey=='7')or(inKey=='8')or(inKey=='9'))
             {Stringkey+=inKey;
              lcd.clear();lcd.setCursor(0,0);lcd.print("Time 1 = ");lcd.setCursor(0,1);lcd.print(Stringkey);}
           }
        
        Stringkey="";  
        while(inputStatus==2)//Time2================
           {
             inKey=keypadA.getKey();delay(2);
             if(inKey=='A')
             {lcd.clear();lcd.print("Reset");delay(500);
              lcd.clear();lcd.print("??"); time1=0;time2=0;mode=0;inputStatus=0;Stringkey="";break;}
             if(inKey=='B'){time2=Stringkey.toInt();inputStatus=3;
                            lcd.clear();lcd.print("Time 2 =");
                            lcd.setCursor(9,0);lcd.print(time2);delay(1000);
                            break;}
             if((inKey=='0')or(inKey=='1')or(inKey=='2')or(inKey=='3')or(inKey=='4')or(inKey=='5')or(inKey=='6')or(inKey=='7')or(inKey=='8')or(inKey=='9'))
             {Stringkey+=inKey;
              lcd.clear();lcd.setCursor(0,0);lcd.print("Time 2 = ");lcd.setCursor(0,1);lcd.print(Stringkey);}
           }
           
       while(inputStatus==3)//Machine================
           {
            if((mode<1)||(mode>3))
            { lcd.clear();lcd.print("Reset");delay(500);
              lcd.clear();lcd.print("??"); time1=0;time2=0;mode=0;inputStatus=0;Stringkey="";break;
            }
            machineStart();           
            lcd.clear();lcd.print("Continue ??");
            while(inputStatus=4)
                {
                  inKey=keypadA.getKey();             
                  delay(2);
      
                  if(inKey=='A'){lcd.clear();lcd.print("Reset");delay(500);lcd.clear();lcd.print("??"); time1=0;time2=0;mode=0;inputStatus=0;Stringkey="";break;}
                  if(inKey=='B'){lcd.clear();lcd.print("..Again");delay(2000);lcd.clear();inputStatus=3;break;}
                }
          
          }
       
    }
}

