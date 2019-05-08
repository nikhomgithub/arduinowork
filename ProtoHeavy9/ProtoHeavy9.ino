
//Arduino Code นี้ รับข้อมูล จากแอนดรอย์ และส่งกลับทันที่ ไปยัง Android แนะนำให้ใช้ App Bluettooth Terminal HC-05 ในการทดสอบ
//ว่า HC-05 หรือ HC-06 นั้นยังใช้งานได้ทั้งฝั่งรับและส่งข้อมูลหรือไม่
//เวลา ส่ง กลับข้อความจะเพิ่ม A ไปอีกตัวเพื่อให้สังเกตเห็นได้ง่ายๆ เช่น รับมา 111 ส่งกลับ 111A จะเห็นใน App ข้างต้นชัดเจน
//แนะนำให้ใช้ HC-05 เพราะคุณภาพดีกว่า 06 มาก ในการเชื่อมต่อ และ เวลา ต่อเข้า RX ต้องต่อ pull ground ลดแรงดันไฟให้เหลือ ครึ่ง โดยต่อร่วมกัน 1k 2 ตัว หัวท้าย หัวคือต่อเข้า TX arduino และ Gnd arduino

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//ในบางกรณี จาก 0x3F เป็น 0x27
//pincode ใน HC-06 คือ 1234

#include "DHT.h"
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
int SolidStateRelay1=24;
int SolidStateRelay2=26;


int sendCount=0;

int Htarget=0;
int Vtarget=0;
int H=0;
int V=0;
int Hcount=0;
int Vcount=0;
bool HcurrentSensorRead,HpreviousSensorRead,VcurrentSensorRead,VpreviousSensorRead;
bool UPstatus,LRstatus;
int BackwardSensor=25,ForwardSensor=27;
int LRStaticSensor=29,LRDynamicSensor=31;
int DropSensor=33;
int UPDynamicSensor=35,UPStaticSensor=37;

//ict coinbox
int money,change,price;
long timecoin1,timecoin2;
bool previouscoinpulse,currentcoinpulse;
int cbhpin=23;
int coinsignal=22;
int FBtarget,OBtarget;
int c5baht,c1baht;

int Freeze=26;
int FreezeFan=28;

int C5BahtSensor=45;
int C1BahtSensor=47;
int MoneyBoxSensor=49;
int count5Baht;
int count1Baht;
int count5BahtTarget;
int count1BahtTarget;

bool cur5BahtRead;
bool cur1BahtRead;
bool pre5BahtRead;
bool pre1BahtRead;


int Motor5Baht1=46;
int Motor5Baht2=48;

int Motor1Baht1=50;
int Motor1Baht2=52;

int MotorMoneyBox1=42;
int MotorMoneyBox2=44;

//42 HIGH,44 LOW = moneyBox keep money
//42 LOW, 44 HIGH = moneyBox return money
//46 HIGH,48 LOW = 5 Baht
//50 HIGH,52 LOW = 1 Baht

String order="",itemString="",priceString="",c5bahtString="",c1bahtString="";
String itemXString="",priceXString="",c5bahtXString="",c1bahtXString="";
String IT="IT",PR="PR",FB="FB",OB="OB";

String result003,result004,result005;
long longtime=0;
long diflongtime=0;
String standard="OD000IT000PR000FB000OB000";
String workStatus="OD000IT000PR000FB000OB000";
String message="";
int btint=0;

void setup()   //
{
  Serial.begin(9600);
  /*
  lcd.begin(16,2);
  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Select");
  lcd.setCursor(0,1);
  lcd.print("Review");  
  */
  //delay(1000);
 
  
  //lcd.clear();
  pinMode(BackwardSensor,INPUT);//BackwardSensor
  pinMode(ForwardSensor,INPUT);//ForwardSensor
  pinMode(LRStaticSensor,INPUT);//LRStaticSensor
  pinMode(LRDynamicSensor,INPUT);//LRDynamicSensor
  pinMode(DropSensor,INPUT);//DropSensor
  pinMode(UPDynamicSensor,INPUT);//UPDynamicSensor
  pinMode(UPStaticSensor,INPUT);//UPStaticSensor
  //ict coinbox
  pinMode(coinsignal,INPUT);//signal
  pinMode(cbhpin,OUTPUT);//On,Off
  
  pinMode(MoneyBoxSensor,INPUT);
  pinMode(Freeze,OUTPUT);
  pinMode(FreezeFan,OUTPUT);

  dht.begin();
  pinMode(SolidStateRelay1,OUTPUT);
  pinMode(SolidStateRelay2,OUTPUT);
  digitalWrite(SolidStateRelay1,LOW);
  digitalWrite(SolidStateRelay2,LOW);
  
  pinMode(C5BahtSensor,INPUT);
  pinMode(C1BahtSensor,INPUT);
  pinMode(MoneyBoxSensor,INPUT);

  pinMode(Motor5Baht1,OUTPUT);
  pinMode(Motor5Baht2,OUTPUT);

  pinMode(Motor1Baht1,OUTPUT);
  pinMode(Motor1Baht2,OUTPUT);

  pinMode(MotorMoneyBox1,OUTPUT);
  pinMode(MotorMoneyBox2,OUTPUT); 
}
//=========================================
//===================================
//To convert intr which is received via Serial to String
//Input int
//Output String
//
//
String convertToString(int i)
  {
    switch(i){

      case 48:return "0";break;
      case 49:return "1";break;
      case 50:return "2";break;
      case 51:return "3";break;
      case 52:return "4";break;
      case 53:return "5";break;
      case 54:return "6";break;
      case 55:return "7";break;
      case 56:return "8";break;
      case 57:return "9";break;
      case 64:return "@";break;
      case 65:return "A";break;
      case 66:return "B";break;
      case 67:return "C";break;
      case 68:return "D";break;
      case 69:return "E";break;
      case 70:return "F";break;
      case 71:return "G";break;
      case 72:return "H";break;
      case 73:return "I";break;
      case 74:return "J";break;
      case 75:return "K";break;
      case 76:return "L";break;
      case 77:return "M";break;
      case 78:return "N";break;
      case 79:return "O";break;
      case 80:return "P";break;
      case 81:return "Q";break;
      case 82:return "R";break;
      case 83:return "S";break;
      case 84:return "T";break;
      case 85:return "U";break;
      case 86:return "V";break;
      case 87:return "W";break;
      case 88:return "X";break;
      case 89:return "Y";break;
      case 90:return "Z";break;
     
      default: return"";break;
   }
}
//=============================================
//=============================================
//===================================
//config Hatarget,Vtarget to move to slot
void selection(int item){
  switch (item){
    case 1:Htarget=6;Vtarget=4;
      break;
    case 2:Htarget=5;Vtarget=4;
      break;
    case 3:Htarget=4;Vtarget=4;
      break;
    case 4:Htarget=3;Vtarget=4;
      break;
    case 5:Htarget=2;Vtarget=4;
      break;
    case 6:Htarget=1;Vtarget=4;
      break;
  //=================
    case 7:Htarget=6;Vtarget=3;
      break;
    case 8:Htarget=5;Vtarget=3;
      break;
    case 9:Htarget=4;Vtarget=3;
      break;
    case 10:Htarget=3;Vtarget=3;
      break;
    case 11:Htarget=2;Vtarget=3;
      break;
    case 12:Htarget=1;Vtarget=3;
      break;
  //=================    
    case 13:Htarget=6;Vtarget=2;
      break;
    case 14:Htarget=5;Vtarget=2;
      break;
    case 15:Htarget=4;Vtarget=2;
      break;
    case 16:Htarget=3;Vtarget=2;
      break;
    case 17:Htarget=2;Vtarget=2;
      break;
    case 18:Htarget=1;Vtarget=2;
      break;
  //==============
    case 19:Htarget=6;Vtarget=1;
      break;
    case 20:Htarget=5;Vtarget=1;
      break;
    case 21:Htarget=4;Vtarget=1;
      break;
    case 22:Htarget=3;Vtarget=1;
      break;
  //==============
  }
}
//==============================
//
//clear all var
void clearCode(){
      order="";
      itemXString=""; itemString="";
      priceXString=""; priceString="";
      c5bahtXString=""; c5bahtString="";
      c1bahtXString=""; c1bahtString="";
}
//===============================

//=================================
String convertTo3Digit(String mString){
  String newString;  
  if(mString.length()==0){newString="000";}
  else if(mString.length()==1){newString="00"+mString;}
  else if(mString.length()==2){newString="0"+mString;}
  else if(mString.length()==3){newString=mString;}
  else{newString="000";}
  return newString;  
}
//=================================
//================================
//
//give 5b change
void change5BahtX(){
  count5Baht=0;
  count5BahtTarget=FBtarget;

  pre5BahtRead=0;
  cur5BahtRead=0;
  
  Motor5BahtStart();
 
  while(count5Baht<count5BahtTarget){
    cur5BahtRead=digitalRead(C5BahtSensor);
    delay(1);
    if(cur5BahtRead==1&&pre5BahtRead==0){count5Baht=count5Baht+1;}
    pre5BahtRead=cur5BahtRead;
  }
  Motor5BahtStop();
}
//===============================
//
//give 1b change
void change1BahtX(){
  count1Baht=0;
  count1BahtTarget=OBtarget;

  pre1BahtRead=0;
  cur1BahtRead=0;
  
  Motor1BahtStart();
 
  while(count1Baht<count1BahtTarget){
    cur1BahtRead=digitalRead(C1BahtSensor);
    delay(1);
    if(cur1BahtRead==1&&pre1BahtRead==0){count1Baht=count1Baht+1;}
    pre1BahtRead=cur1BahtRead;
  }
  Motor1BahtStop();
}
//==========================================
//
//get money from customer
//
String ICTcoinboxX()
{
  //Serial.println("Start Coin Box");
  digitalWrite(cbhpin,HIGH);//on coinbox 
  delay(100);
  previouscoinpulse=1;
  currentcoinpulse=1; 
  money=0;
  price=priceString.toInt();
  //Serial.print("Price = ");
  //Serial.println(price);
  change=0;
  timecoin1=millis();timecoin2=millis();   
  
  while((money<price)&&((timecoin2-timecoin1)<10000))
        {         
          currentcoinpulse=digitalRead(coinsignal);
          if (currentcoinpulse==0&&previouscoinpulse==1)
                    {   money=money+1;             
                        timecoin1=millis();      
                    }
          timecoin2=millis();  
          previouscoinpulse=currentcoinpulse;     
        }
  timecoin1=millis();timecoin2=millis();
  while((money<(price+10))&&((timecoin2-timecoin1)<200))//
        {         
          currentcoinpulse=digitalRead(coinsignal);
          if (currentcoinpulse==0&&previouscoinpulse==1)
                    {   money=money+1;             
                        timecoin1=millis();      
                    }
          timecoin2=millis();  
          previouscoinpulse=currentcoinpulse;     
        }
  
  //Serial.println("Stop Coin Box");
  //digitalWrite(cbhpin,LOW);//off coinbox
  //Serial.print(" End money =");
  //Serial.println(money);

  change=money-price;
  c1baht=c1bahtString.toInt();
  c5baht=c5bahtString.toInt();
  OBtarget = 0;  
  FBtarget = 0;
  
  if(change>0){
    OBtarget = change%5;  
    FBtarget = change/5;
    if(c5baht<FBtarget){OBtarget=FBtarget*5+OBtarget;FBtarget=0;}
    //Serial.print(" 5 Baht Coin = ");Serial.println(FBtarget);
    //Serial.print(" 1 Baht Coin = ");Serial.println(OBtarget);    
  }
  
  if(money>=price){  
      if ((c1baht>=OBtarget)&&(c5baht>=FBtarget)){
          //Serial.println("Thank you, pleas wait"); 003
 //
 //
 //this line is keep money,change,get object to customer
 //
 //Serial to android ====================================== "Job Finish Thank you"          
          keepMoney();
          change1BahtX();change5BahtX(); 
          takeActionX();
          

          c5baht=c5baht-FBtarget;c5bahtString=String(c5baht);c5bahtString=convertTo3Digit(c5bahtString);
          c1baht=c1baht-OBtarget;c1bahtString=String(c1baht);c1bahtString=convertTo3Digit(c1bahtString);
          result003="OD003";
          result003=result003+IT+itemString+PR+priceString+FB+c5bahtString+OB+c1bahtString;
          clearCode();
          return result003;
          //  
          }
 //Serial to android ====================================== "No Chage" 
      else{
        //Serial.println("Sorry No Change") 004;
        returnMoney();
        result004="OD004";
        result004=result004+IT+itemString+PR+priceString+FB+c5bahtString+OB+c1bahtString;
        clearCode();
        return result004;
        }
  }
//Serial to android ========================================"Time Up"
  if(money<price){
    //Serial.println("Sorry Time is Up") 005;
    returnMoney();
    result005="OD005";
    result005=result005+IT+itemString+PR+priceString+FB+c5bahtString+OB+c1bahtString;
    clearCode();
    return  result005;
    }
 }  
//=================================================================
//=================================================================



//================================= 
//==================================
//=================================
void Motor5BahtStart(){digitalWrite(Motor5Baht1,HIGH);digitalWrite(Motor5Baht2,LOW);delay(3);}
void Motor5BahtStop(){digitalWrite(Motor5Baht1,LOW);digitalWrite(Motor5Baht2,LOW);}
void Motor1BahtStart(){digitalWrite(Motor1Baht1,HIGH);digitalWrite(Motor1Baht2,LOW);delay(3);}
void Motor1BahtStop(){digitalWrite(Motor1Baht1,LOW);digitalWrite(Motor1Baht2,LOW);}


//==========================
//
//motor drive to change5 baht
//
void change5Baht(int target){
  //delay(2000);
  count5Baht=0;
  count5BahtTarget=target;

  pre5BahtRead=0;
  cur5BahtRead=0;
  
  Motor5BahtStart();
 
  while(count5Baht<count5BahtTarget){
    cur5BahtRead=digitalRead(C5BahtSensor);
    delay(1);
    if(cur5BahtRead==1&&pre5BahtRead==0){count5Baht=count5Baht+1;}
    pre5BahtRead=cur5BahtRead;
  }
  Motor5BahtStop();
}
//====================
//
//motor drive to change 1 baht
//
void change1Baht(int target){
  delay(2000);
  count1Baht=0;
  count1BahtTarget=target;

  pre1BahtRead=0;
  cur1BahtRead=0;
  
  Motor1BahtStart();
 
  while(count1Baht<count1BahtTarget){
    cur1BahtRead=digitalRead(C1BahtSensor);
    delay(1);
    if(cur1BahtRead==1&&pre1BahtRead==0){count1Baht=count1Baht+1;}
    pre1BahtRead=cur1BahtRead;
  }
  Motor1BahtStop();
}
//==================================
//
//To keep Money in Box 
void keepMoney(){
  digitalWrite(MotorMoneyBox1,HIGH);digitalWrite(MotorMoneyBox2,LOW);delay(1000);
  while(digitalRead(MoneyBoxSensor)==0){}delay(75);digitalWrite(MotorMoneyBox1,LOW);digitalWrite(MotorMoneyBox2,LOW);
}
//================================
//
//To return Money in customer
void returnMoney(){
  digitalWrite(MotorMoneyBox2,HIGH);digitalWrite(MotorMoneyBox1,LOW);delay(1000);
  while(digitalRead(MoneyBoxSensor)==0){}delay(70);digitalWrite(MotorMoneyBox1,LOW);digitalWrite(MotorMoneyBox2,LOW);
}
//================================
//
//To motor action
void startSpin(int spinRateBegin,int spinRateEnd,int spinPin,int zeroPin,int delayTime)//normal spinRateBegin=0,spinRateEnd=250
    { for (int i=spinRateBegin;i<=spinRateEnd;i=i+10){analogWrite(spinPin,i);analogWrite(zeroPin,0);delay(delayTime);}  }

void slowSpin (int spinRateBegin,int spinRateEnd,int spinPin,int zeroPin,int delayTime)//normal spinRateBegin=250,spinRateEnd=0
    { for(int i=spinRateBegin;i>=spinRateEnd;i=i-10){analogWrite(spinPin,i);analogWrite(zeroPin,0);delay(delayTime);} }

void keepSpin(int spinRate,int spinPin,int zeroPin,int delayTime)//normal spinRate=250
    {analogWrite(spinPin,spinRate);analogWrite(zeroPin,0);delay(delayTime);}

void stopSpin(int spinPin,int zeroPin,int delayTime)
    {analogWrite(spinPin,0);analogWrite(zeroPin,0);delay(delayTime);}
//=====================================
//to check before start machine
void errorCheck(){
  if(digitalRead(BackwardSensor)!=1){/* lcd.clear();lcd.setCursor(0,0);lcd.print("BackwardSensor!!");*/while(1){} }
  if(digitalRead(UPStaticSensor)!=1){/* lcd.clear();lcd.setCursor(0,0);lcd.print("UPStaticSensor!!");*/while(1){} }
  if(digitalRead(LRStaticSensor)!=1){/* lcd.clear();lcd.setCursor(0,0);lcd.print("LRStaticSensor!!");*/while(1){} }
  if(digitalRead(DropSensor)!=1){ /*lcd.clear();lcd.setCursor(0,0);lcd.print("DropSensor!!");*/while(1){} }
  if(digitalRead(MoneyBoxSensor)!=1){/* lcd.clear();lcd.setCursor(0,0);lcd.print("MoneyBoxSensor!!");*/while(1){} }
}
//======================================
//MoveDown Pin 4=1,Pin 5=0
//MoveUp Pin 5=1,Pin 4 =0
//MoveLeft Pin 3=1,Pin 2 =0
//MoveRight Pin 2=1,Pin3 =0
//MoveForward Pin 11=1 Pin 10=0
//MoveBackward Pin 10=1,Pin 11=0
//MoveHold Pin 8=1,Pin 9=0
//MoveDrop Pin 9=1, Pin 8=0,

//======================================
// Do not use any more !!!! move to item with sensor control
void ToItemWithSensor(){
  startSpin(0,250,5,4,1);
  startSpin(0,120,3,2,1);
  UPstatus=0;
  LRstatus=0;
  delay(300);
  while( digitalRead(UPDynamicSensor)==0 || digitalRead(LRDynamicSensor)==0 ){
    delay(5);
    if(digitalRead(UPDynamicSensor)==1&&UPstatus==0){slowSpin(250,0,5,4,1);UPstatus=1; } 
    if(digitalRead(LRDynamicSensor)==1&&LRstatus==0){slowSpin(120,0,3,2,1);LRstatus=1; }
  }
  stopSpin(5,4,1); 
  stopSpin(3,2,1); 
}
//=====================================
//move forward with sensor control
void moveForward(){
  if(digitalRead(BackwardSensor)==1){startSpin(0,250,11,10,1);}
  while(digitalRead(ForwardSensor)==0){keepSpin(250,11,10,1);}
  if(digitalRead(ForwardSensor)==1){slowSpin(250,0,11,10,1);stopSpin(11,10,1);}
}
//=====================================
//move backward with sensor control
void moveBackward(){
  if(digitalRead(ForwardSensor)==1){startSpin(0,250,10,11,1);}
  while(digitalRead(BackwardSensor)==0){keepSpin(250,10,11,1);}
  if(digitalRead(BackwardSensor)==1){slowSpin(250,0,10,11,1);stopSpin(10,11,1);}
}
//=====================================
//move drop with sensor control
void moveDrop(){
  if(digitalRead(DropSensor)==0){startSpin(0,250,9,8,1);}
  while(digitalRead(DropSensor)==0){keepSpin(250,9,8,1);}
  keepSpin(250,9,8,1500);
  slowSpin(250,0,9,8,1);
  stopSpin(9,8,1);
}
//=====================================
//keep holding object 
void moveHold(){
  startSpin(0,250,8,9,1);
  delay(2800);
  slowSpin(250,150,8,9,1);
  delay(1000);
}
//======================================
//move a little bit up without sensor check
void moveAbitUp(){
 startSpin(0,150,5,4,1);
 delay(850);
 slowSpin(150,0,5,4,1);
}
//=======================================
//move 2 direction with sensor to original location and drop object to customer
void ToBWithSensor(){
  startSpin(0,200,4,5,1);
  startSpin(0,120,2,3,1);
  UPstatus=0; //as switch
  LRstatus=0; //as switch 
  
  delay(300);
  while( digitalRead(UPStaticSensor)==0 || digitalRead(LRStaticSensor)==0 ){
    delay(1);
    if(digitalRead(UPStaticSensor)==1&&UPstatus==0){slowSpin(200,0,4,5,1);UPstatus=1;} 
    if(digitalRead(LRStaticSensor)==1&&LRstatus==0){slowSpin(120,0,2,3,1);LRstatus=1;}
  }
  moveForward();
  moveDrop();
  moveBackward();
}
//=========================================
//move 2 direction with sensor to pick up object at target slot and grab object and move down to origital location give object to customer
void ToSelectionItem(){
  H=Htarget;
  V=Vtarget;
  Hcount=0;
  Vcount=0;

  UPstatus=0;
  LRstatus=0;
  
  HcurrentSensorRead=0;
  HpreviousSensorRead=0;

  VcurrentSensorRead=0;
  VpreviousSensorRead=0;
  
  startSpin(0,250,5,4,1);
  startSpin(0,120,3,2,1);
  delay(300);

  while(Vcount<V || Hcount<H){
      delay(5);
      VcurrentSensorRead=digitalRead(UPDynamicSensor);
      HcurrentSensorRead=digitalRead(LRDynamicSensor);
    
      if(VcurrentSensorRead==1 && VpreviousSensorRead==0){Vcount=Vcount+1;}
      if(HcurrentSensorRead==1 && HpreviousSensorRead==0){Hcount=Hcount+1;}

      if(Vcount==V && UPstatus==0){slowSpin(250,0,5,4,1);UPstatus=1; } 
      if(Hcount==H && LRstatus==0){slowSpin(120,0,3,2,1);LRstatus=1; }

      VpreviousSensorRead=VcurrentSensorRead;
      HpreviousSensorRead=HcurrentSensorRead;
  }
  moveForward(); 
  moveHold();
  moveAbitUp();
  moveBackward();
  ToBWithSensor();
}
//=================================
//to check temperature and humidity and turn on/off fridge
//Input nothing
//Output action of fridge
//

void fridgecontrol()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(t);
  Serial.print("   Humidity = ");
  Serial.println(h);
  
  //if (t>14){digitalWrite(8,LOW);}
  //if (t<12){digitalWrite(8,HIGH);}
  delay(4000);
}
//================================
//To receive Serial message from Android and to Pick Up Product
// Input itemString
// Output action of machine as following "selection" and "ToSelectionItem"
//

void takeActionX(){
  //Serial.print("Item = ");
  //Serial.println(itemString);
  if(itemString=="001"){/*Serial.println("To 01");*/selection(1);ToSelectionItem();} 
  if(itemString=="002"){/*Serial.println("To 02");*/selection(2);ToSelectionItem();}
  if(itemString=="003"){/*Serial.println("To 03");*/selection(3);ToSelectionItem();}
  if(itemString=="004"){/*Serial.println("To 04");*/selection(4);ToSelectionItem();} 
  if(itemString=="005"){/*Serial.println("To 05");*/selection(5);ToSelectionItem();}  
  if(itemString=="006"){/*Serial.println("To 06");*/selection(6);ToSelectionItem();}
  if(itemString=="007"){/*Serial.println("To 07");*/selection(7);ToSelectionItem();}
  if(itemString=="008"){/*Serial.println("To 08");*/selection(8);ToSelectionItem();}
  if(itemString=="009"){/*Serial.println("To 09");*/selection(9);ToSelectionItem();}  
  if(itemString=="010"){/*Serial.println("To 10");*/selection(10);ToSelectionItem();}  
  if(itemString=="011"){/*Serial.println("To 11");*/selection(11);ToSelectionItem();} 
  if(itemString=="012"){/*Serial.println("To 12");*/selection(12);ToSelectionItem();}
  if(itemString=="013"){/*Serial.println("To 13");*/selection(13);ToSelectionItem();}
  if(itemString=="014"){/*Serial.println("To 14");*/selection(14);ToSelectionItem();}
  if(itemString=="015"){/*Serial.println("To 15");*/selection(15);ToSelectionItem();}
  if(itemString=="016"){/*Serial.println("To 16");*/selection(16);ToSelectionItem();}
  if(itemString=="017"){/*Serial.println("To 17");*/selection(17);ToSelectionItem();}
  if(itemString=="018"){/*Serial.println("To 18");*/selection(18);ToSelectionItem();}
  if(itemString=="019"){/*Serial.println("To 19");*/selection(19);ToSelectionItem();}
  if(itemString=="020"){/*Serial.println("To 20");*/selection(20);ToSelectionItem();}
  if(itemString=="021"){/*Serial.println("To 21");*/selection(21);ToSelectionItem();}
  if(itemString=="022"){/*Serial.println("To 22");*/selection(22);ToSelectionItem();}  
}
//=======================================
//To take action for maintenancd/test purpose
// Input message(code) which is 3 digit of string and put it to Action as followng
// Output action of machine as following
// moveAbitUp, moveDrop, moveBackward, moveForward, keepMoney, returnMoney,change1Baht,change5Baht,fridgecontrol,ToBWithSensor,selection and To SelectionItem


//For maintenance, short action according to code
//
void takeAction(){
  if(message.length()==3){
    if(message=="001"){/*Serial.println("MoveAbitUp");*/moveAbitUp();}
    if(message=="002"){/*Serial.println("MoveDrop");*/moveDrop();}
    if(message=="003"){/*Serial.println("MoveBackward");*/moveBackward();}
    if(message=="004"){/*Serial.println("MoveForward");*/moveForward();}

    //if(message=="020"){Serial.println("ICT coinbox");ICTcoinbox();}
    if(message=="021"){/*Serial.println("Keep Money");*/keepMoney();}
    if(message=="022"){/*Serial.println("Return Money");*/returnMoney();}
    if(message=="023"){/*Serial.println("Change 1 Baht");*/change1Baht(3);}
    if(message=="024"){/*Serial.println("Change 5 Baht");*/change5Baht(1);}
  
    if(message=="030"){/*Serial.println("Fridge Control");*/fridgecontrol();}
  
    if(message=="100"){/*Serial.println("ToBWithSensor");*/ToBWithSensor();}
  
    if(message=="101"){/*Serial.println("To 01");*/selection(1);ToSelectionItem();} 
    if(message=="102"){/*Serial.println("To 02");*/selection(2);ToSelectionItem();}
    if(message=="103"){/*Serial.println("To 03");*/selection(3);ToSelectionItem();}
    if(message=="104"){/*Serial.println("To 04");*/selection(4);ToSelectionItem();} 
    if(message=="105"){/*Serial.println("To 05");*/selection(5);ToSelectionItem();}  
    if(message=="106"){/*Serial.println("To 06");*/selection(6);ToSelectionItem();}
    if(message=="107"){/*Serial.println("To 07");*/selection(7);ToSelectionItem();}
    if(message=="108"){/*Serial.println("To 08");*/selection(8);ToSelectionItem();}
    if(message=="109"){/*Serial.println("To 09");*/selection(9);ToSelectionItem();}  
    if(message=="110"){/*Serial.println("To 10");*/selection(10);ToSelectionItem();}  
    if(message=="111"){/*Serial.println("To 11");*/selection(11);ToSelectionItem();} 
    if(message=="112"){/*Serial.println("To 12");*/selection(12);ToSelectionItem();}
    if(message=="113"){/*Serial.println("To 13");*/selection(13);ToSelectionItem();}
    if(message=="114"){/*Serial.println("To 14");*/selection(14);ToSelectionItem();}
    if(message=="115"){/*Serial.println("To 15");*/selection(15);ToSelectionItem();}
    if(message=="116"){/*Serial.println("To 16");*/selection(16);ToSelectionItem();}
    if(message=="117"){/*Serial.println("To 17");*/selection(17);ToSelectionItem();}
    if(message=="118"){/*Serial.println("To 18");*/selection(18);ToSelectionItem();}
    if(message=="119"){/*Serial.println("To 19");*/selection(19);ToSelectionItem();}
    if(message=="120"){/*Serial.println("To 20");*/selection(20);ToSelectionItem();}
    if(message=="121"){/*Serial.println("To 21");*/selection(21);ToSelectionItem();}
    if(message=="122"){/*Serial.println("To 22");*/selection(22);ToSelectionItem();} 
  }
}
//===============================================
//
// get Serial from Android
//
void deCode(){
if(Serial.available()>0)
  {
  delay(2);  //delay 2 millisecond for corret data transfer
  btint=Serial.read();
  message=message+convertToString(btint);
      if(Serial.available()<=0)
    {
        Serial.print(message);
        //takeAction();
        Serial.flush();
        message="";
    }
  }
   //sendCount=sendCount+1;
}  
//==================================
String ICTcoinboxTest()
{ 
  String resultTest=convertTo3Digit(String(random(3,4)));
  delay(2000);
  if(resultTest=="003"){resultTest="OD003";resultTest=resultTest+IT+itemString+PR+priceString+FB+c5bahtString+OB+c1bahtString;return resultTest;}
  else if(resultTest="004"){resultTest="OD004";resultTest=resultTest+IT+itemString+PR+priceString+FB+c5bahtString+OB+c1bahtString;return resultTest;}
  else {resultTest="OD005";resultTest=resultTest+IT+itemString+PR+priceString+FB+c5bahtString+OB+c1bahtString;return resultTest;}
}
//=============================================
void decodeTest(){

  diflongtime=millis()-longtime;
  if((diflongtime>=1000)&&(standard==message)&&(order="001")){standard=ICTcoinboxTest();order="";Serial.flush();message="";longtime=millis();workStatus=standard;Serial.print(standard);}  
  
  if(Serial.available()>0)
  {delay(2);btint=Serial.read();message=message+convertToString(btint);
      if(Serial.available()<=0)
      {
         if((message.length()==25)&&(message.indexOf("OD")==0)&&(message.indexOf("IT")==5)&&(message.indexOf("PR")==10)&&(message.indexOf("FB")==15)&&(message.indexOf("OB")==20))
              {
                         order=message.substring(2,5);itemString=message.substring(7,10);priceString=message.substring(12,15);c5bahtString=message.substring(17,20);c1bahtString=message.substring(22);
                         if(order=="001")
                          { 
                              if(message!=standard){standard=message;Serial.flush();message=""; longtime=millis();workStatus=="OD000IT000PR000FB000OB000";Serial.print(workStatus);}     
                              else {Serial.print(standard);}
                          }
                          else if((order=="003")||(order=="004")||(order=="005"))
                          {Serial.print(standard);Serial.flush();message=""; longtime=millis();}

                         else {Serial.print(workStatus);Serial.flush();message="";longtime=millis();}                           
              }
         else{Serial.print(workStatus);Serial.flush();message="";longtime=millis();}
      }
  }
}
//

void loop()   
{
  //fridgecontrol();
  decodeTest();
}
//==========================================

