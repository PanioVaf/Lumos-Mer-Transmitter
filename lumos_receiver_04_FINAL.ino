//Lumos Receiver Copyright (C) 2013  NicolEye Team
//This program comes with ABSOLUTELY NO WARRANTY; 
//This is free software, and you are welcome to redistribute it
//under certain conditions.
//For Details Check:http://www.gnu.org/licenses/gpl-3.0.txt
//Lumos Receiver Software
//Version 0.4 "creepy doll"
int i, good, k;                 //receiver stuff 0
long data_in;                   //receiver stuff 1
long data_filtered = 101010101; //filtered receiver value(9 digits)
int ledlight = 4;               //light & sensor errors
int ledgood = 2;                //all seems good
int ledship = 5;                //For when seeing a ship
int ledsol = 6;                 //For Solar Panel errors
int ledwind = 7;                //for Wind turbine errors
int ledserv = 9;                //For service mode
int ledbatt = 8;                //For battery errors
int buzz = 12;                  //The buzzer Obviously
int error = 0;                  //used to blink the buzzer
int ship = 0;                   //used to blink for ship
int service = 1;                //Used for the service blink
long threshold = 60000;         //time counter  to milisecond to trigger apocalypse mode
int presence = 1;               //presence is used for the apocalypse mode trigger
int absence = 0;                //absence is used for the apocalypse mode trigger
int apoc = 0;                   //used for the apocalypse function
int periode = 2000;             //another fade variable
int displace = 500;             //Yet another fade variable
int ledState = LOW;             //ledState used to set the BUZZER
long previousMillis = 0;        //will store last time BUZZER was updated
long interval = 300;            //interval at which to blink (milliseconds)
unsigned long deathMillis;      //used for apocalypse trigger
unsigned long deathMillis2;      //used for apocalypse trigger as well 
//***Begin Setup
void setup()
{
Serial.begin(115200);   //Initialize serial
  //***Begin Setting Pin Modes
pinMode(3, INPUT);	//receiver pin
pinMode(ledlight,OUTPUT);
pinMode(ledgood,OUTPUT);
pinMode(ledship,OUTPUT);
pinMode(buzz,OUTPUT);
pinMode(ledsol,OUTPUT);
pinMode(ledwind,OUTPUT);
pinMode(ledbatt,OUTPUT);
pinMode(ledserv,OUTPUT);
//***End Setting Pin Modes
//***Begin Visual Calibration
digitalWrite(buzz,HIGH);
delay(500);
digitalWrite(ledgood,HIGH);
delay(500);
digitalWrite(ledgood,LOW);
digitalWrite(ledserv,HIGH);
delay(500);
digitalWrite(ledserv,LOW);
digitalWrite(ledship,HIGH);
delay(500);
digitalWrite(ledship,LOW);
digitalWrite(ledbatt,HIGH);
delay(500);
digitalWrite(ledbatt,LOW);
digitalWrite(ledwind,HIGH);
delay(500);
digitalWrite(ledwind,LOW);
digitalWrite(ledsol,HIGH);
delay(500);
digitalWrite(ledsol,LOW);
digitalWrite(ledlight,HIGH);
delay(500);
digitalWrite(ledlight,LOW);
digitalWrite(buzz,LOW);
digitalWrite(ledgood,HIGH);
//***End Visual Calibration 
}
//***End Setup
void loop() {
/*BEGIN LOOP*/
//***Begin Receiver Boiler-Plate
for(i=0; i<100; i++){
      delayMicroseconds(20);
      good=1;
      if(digitalRead(3)==LOW){
      good=0;
      i=100;
      }
    }//for
    if(good==1){
    detachInterrupt(1);
    while(1){
      if(digitalRead(3)==LOW){
        delayMicroseconds(850);

        for(i=0; i<32; i++){
          if(digitalRead(3)==HIGH)
          bitWrite(data_in, i, 1);
          else
          bitWrite(data_in, i, 0);
          delayMicroseconds(1000);
        }//for
      if(data_in=='#')
        Serial.println("");
        else
        Serial.print(long(data_in)); 
        //Serial.println(data_in, DEC);
       break;//secondtwhile
      }//low kickoff
    }//second while
    }//good check
//***End Receiver Boiler-Plate
//----------------------------
//***Begin Buzzer code
if (error == 1)
{
  unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis > interval) { 
    previousMillis = currentMillis;   
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(buzz, ledState);
  }
}
//***End Buzzer code
//---------------------------
//***Begin ship code
if (ship == 1)
{
  if (error == 0)
  {
  unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis > interval) { 
    previousMillis = currentMillis;   
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(ledship, ledState);
          }
  }
  else
    {
     if (ledState == HIGH)
     {digitalWrite(ledship,LOW);}
     else {digitalWrite(ledship,HIGH);}
    }
}
//***End Ship code
//---------------------------
//***Begin Service code
if (service == 1)
{
  unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis > interval) { 
    previousMillis = currentMillis;   
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(ledserv, ledState);
    presence = 1;  
}
}
//***End Service Code
//---------------------------
//***Begin Filter
if (data_in == 100000000 || data_in == 100001000 || data_in == 100010000 || 
    data_in == 110000000 || data_in == 101000000 || data_in == 100100000 || 
    data_in == 111111111 || data_in == 111000000 || data_in == 110100000 || 
    data_in == 110010000 || data_in == 101100000 || data_in == 101010000 || 
    data_in == 100110000 || data_in == 111100000 || data_in == 111010000 ||
    data_in == 101110000 || data_in == 110010000 || data_in == 100011000 || 
    data_in == 110001000 || data_in == 101001000 || data_in == 100100000 || 
    data_in == 111001000 || data_in == 110101000 || data_in == 110011000 || 
    data_in == 101101000 || data_in == 101011000 || data_in == 100111000 || 
    data_in == 111101000 || data_in == 111011000 || data_in == 101111000 || 
    data_in == 111111000 || data_in == 100101000 || data_in == 111110000)
{
data_filtered = data_in;
presence = 1;
absence = 0;
deathMillis = 0;
}
else
{
if (data_filtered != 101010101)
{presence = 0 ;}
}
//***End Filter
//---------------------------
//***Begin Case Hell
switch(data_filtered)
{
case 100000000://All seems good
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,HIGH);
digitalWrite(ledship,LOW);
digitalWrite(buzz,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 0;
ship = 0;
service = 0;
break;
case 100001000://Found a ship
digitalWrite(ledgood,HIGH);
digitalWrite(ledlight,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(buzz,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 0;
ship = 1;
service = 0;
break;
case 100010000://Light &/Or Sensor error
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 110000000://Solar Error
digitalWrite(ledsol,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledlight,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 101000000://Wind Error
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 100100000://Battery Error
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 111111111://Service Code
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(buzz,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
error = 0;
ship = 0;
service = 1;
apoc = 0;
break;
case 111000000://Solar + Wind Error
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
service = 0;
break;
case 110100000://Solar + Battery Error
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 110010000://Solar + Light Error
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 101100000://Wind + Battery Error
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 101010000://Wind + Light Error
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 100110000://Battery + Light Error
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 111100000://Solar + Wind + Battery Error
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 111010000://Solar + Wind + Light Error
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 101110000://Wind + Battery + Light Error
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledship,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 111110000://Solar + Light + Battery + Wind Error
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 0;
service = 0;
break;
case 100011000://Light &/Or Sensor error + Ship
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 110001000://Solar Error + Ship
digitalWrite(ledsol,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledlight,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 101001000://Wind Error + Ship
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 100101000://Battery Error + Ship
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 111001000://Solar + Wind Error + Ship
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 110101000://Solar + Battery Error + Ship
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 110011000://Solar + Light Error + Ship
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 101101000://Wind + Battery Error + Ship
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 101011000://Wind + Light Error + Ship
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 100111000://Battery + Light Error + Ship
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,LOW);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 111101000://Solar + Wind + Battery Error + Ship
digitalWrite(ledlight,LOW);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 111011000://Solar + Wind + Light Error + Ship
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,LOW);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 101111000://Wind + Battery + Light Error + Ship
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,LOW);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
case 111111000://Solar + Light + Battery + Wind Error + Ship
digitalWrite(ledlight,HIGH);
digitalWrite(ledgood,LOW);
digitalWrite(ledsol,HIGH);
digitalWrite(ledwind,HIGH);
digitalWrite(ledbatt,HIGH);
digitalWrite(ledserv,LOW);
error = 1;
ship = 1;
service = 0;
break;
}//***End Case Hell 
//-------------------------
//***Begin Apocalypse Code 

while (apoc == 1 || apoc == 2 || apoc == 3 || apoc == 4)
{
  {
      if (apoc == 4)
      {
        unsigned long currentMillis = millis(); 
        if(currentMillis - previousMillis > interval) { 
          previousMillis = currentMillis;   
          if (ledState == LOW)
            ledState = HIGH;
          else
            {ledState = LOW;
            apoc = 1;}
          digitalWrite(ledbatt, ledState);
          absence = 0;//put here to maintain apocalypse mode
        }
      }
      if (apoc == 3)
      {
        unsigned long currentMillis = millis(); 
        if(currentMillis - previousMillis > interval) { 
          previousMillis = currentMillis;   
          if (ledState == LOW)
            ledState = HIGH;
          else
            {ledState = LOW;
            apoc++;}
          digitalWrite(ledwind, ledState);
          absence = 0;//put here to maintain apocalypse mode
        }
      }
    if (apoc == 2)
      {
        unsigned long currentMillis = millis(); 
        if(currentMillis - previousMillis > interval) { 
          previousMillis = currentMillis;   
          if (ledState == LOW)
            ledState = HIGH;
          else
            {ledState = LOW;
            apoc++;}
          digitalWrite(ledsol, ledState);
          absence = 0;//put here to maintain apocalypse mode
        }
      }
    if (apoc == 1)
      {
        digitalWrite(ledgood,LOW);
        digitalWrite(ledship,LOW);
        unsigned long currentMillis = millis(); 
        if(currentMillis - previousMillis > interval) { 
          previousMillis = currentMillis;   
          if (ledState == LOW)
            ledState = HIGH;
          else
            {ledState = LOW;
            apoc++;}
          digitalWrite(ledlight, ledState);
          absence = 0;//put here to maintain apocalypse mode
        }
      }
  }
digitalWrite(buzz,HIGH);
}
//***End Apocalypse Code
//----------------------------
//***Begin Apocalypse Check
if (presence ==1)
{
  absence = 0;
  deathMillis2 = millis(); 
}

if (presence == 0)
  {
  	if (absence == 0)
  	{deathMillis = millis();
        	if(deathMillis - deathMillis2 > threshold)	
        	{
        	apoc = 1;
        	}
        }
  //absence = 1;
  }
//***End Apocalypse Check
Serial.println(presence);
/***END OF LOOP***/}
