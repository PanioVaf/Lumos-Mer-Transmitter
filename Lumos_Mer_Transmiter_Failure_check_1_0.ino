//Mer Transmitter Copyright (C) 2013  NicolEye Team
//This program comes with ABSOLUTELY NO WARRANTY; 
//This is free software, and you are welcome to redistribute it
//under certain conditions.
//For Details Check:http://www.gnu.org/licenses/gpl-3.0.txt
//Lumos-Mer Transmiter/Failure check Software
//Version 1.0
int pwPin = 7;               //pulse pin for sonar sensor
const int light = 8;         // lighthouse light pin
int transmit = 2;            //pin for data transmission
int pulse;                   //pulse variable for calculations
int testled = 13;            // led 13 for various UI
int ledState = LOW;          //begin blink state for lighthouse light
int sonarerror = 4;          //sonar working/error testtest pin
long error =100000000;       //main transmit variable
long previousMillis = 0;     //variable used for blink state for lighthouse light
long interval = 200;         //variable used for blink state for lighthouse light (time between blinks)
int lighterror = 9;          //pin for main lighthouse error
int service = 3;             //pin for service button
int solar = A0;              //pin for solar error
int wind = 6;                //pin for wind turbine error
int battery = 12;            //pin for battery error
void setup() {
//  Serial.begin(2400);      //serial port for debuging
//  delay(500);              //delay for serial port (debuging)
pinMode(service, INPUT);
pinMode(lighterror, INPUT);  //define that lighterror pin is input
pinMode(light, OUTPUT);      //define that light pin is output
pinMode(pwPin, INPUT);       //define that pwPin pin is input
pinMode(transmit, OUTPUT);   //define that transmit pin is output
pinMode(testled, OUTPUT);    //define that testled pin is output
pinMode(sonarerror, INPUT);  //define that sonarerror pin is input
pinMode(wind, INPUT);        //define that wind pin is input
pinMode(battery, INPUT);     //define that battery pin is input
}
//starting main loop
void loop() {
error = 100000000;           //define that there is no error each time loop starts
//reading service button if on calls a function that puts the loop in hold (all the "functions" of the lighthouse) until is pressed again
if(digitalRead(service) == HIGH){
serv();}

digitalWrite(light, HIGH);   //light the lighthouse main light spontaneously to check for error
if (digitalRead(lighterror) == LOW){ //if no light error occurs goes on in the loop
digitalWrite(light, LOW);    //making sure the light is off
 
//   sonarerrorread = digitalRead(sonarerror); //(debugging code)
   if (digitalRead(sonarerror) == HIGH){ //check's if sonar isn't getting power if sonar isn't getting any, light's permanently the lighthouse
   error = error + 10000; //define the error to the error code variable
   unsigned long currentMillis = millis();  //used by blink boiler
 
  if(currentMillis - previousMillis > interval) { //lighthouse blink (boiler from the examples of arduino ide) better not run as a function (problem's during testing)
    previousMillis = currentMillis;   
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(light, ledState);
}
 }else{ 
//   runOnceSonar();                        //sanity sonar check
   pulse = pulseIn(pwPin, HIGH);          //read pulse signal from the sonar
   //delay(10); //(debugging code)
   if (pulse > 1500 && pulse < 3700){     //pulse criteria (distance criteria)
   unsigned long currentMillis = millis();//used by blink boiler
 
  if(currentMillis - previousMillis > interval) { //lighthouse blink (boiler from the examples of arduino ide) better not run as a function (problem's during testing)
    previousMillis = currentMillis;   
    if (ledState == LOW){
      ledState = HIGH;}else{
      ledState = LOW;}
    digitalWrite(light, ledState);
}                                         //end lighthouse blink boiler
   error = error + 1000;                  //if ship is detected inform error variable
   }                                      //end pulse criteria if
 }                                        //end sonar power error if
}else{ // if light error occurs
   error = error + 10000; //define the error to the error code variable
   ///delay(10); //(debugging code)
   if (digitalRead(sonarerror) == LOW){ //make sure that sonar is still operational
     pulse = pulseIn(pwPin, HIGH);      //read pulse signal from the sonar
   if (pulse > 1500 && pulse < 3700){   //pulse criteria (distance criteria)
   error = error + 1000;                //if ship is detected inform error variable and do not activate the lighthouse
     }                                  //end pulse criteria if
   }                                    //end sonar if
}                                       //end light error if

if (analogRead(solar) == 0){            //if there is no power from solar power
error = error + 10000000;               //inform error variable 
}                                       //end solar if
if (digitalRead(wind) == LOW){          //if there is no power from wind turbine sensor (virtual error)
error = error + 1000000;                //inform error variable
}                                       //end wind if
if (digitalRead(battery) == HIGH ){     //if battery is low (virtual error)
error = error + 100000;                 //inform error variable
}                                       //end wind if
  rf_send(error);                       //call apropriate function to send the error
  rf_send('#');                         //call apropriate function to send terminal tansmission
  delay(30);                            //wait very little
  digitalWrite(testled, HIGH);          //activate testled for debugging
  delay(30);                            //wait very little
  digitalWrite(testled, LOW);           //deactivate testled for debugging
  digitalWrite(light, LOW);             //make sure the lighthouse light is off (bug during tests)
//  Serial.println(pulse); //(debugging code)
//  Serial.println(sonarerror); //(debugging code)
}
void rf_send(long input){              //rf magic (boiler from kevin)
  int i;
  
  for(i=0; i<20; i++){
  digitalWrite(transmit, HIGH);
  delayMicroseconds(500);
  digitalWrite(transmit, LOW);
  delayMicroseconds(500);
}

  digitalWrite(transmit, HIGH);
  delayMicroseconds(3000);
  digitalWrite(transmit, LOW);
  delayMicroseconds(500);
  
    
  for(i=0; i<32; i++){
  if(bitRead(input,i)==1)
  digitalWrite(transmit, HIGH);
  else
  digitalWrite(transmit, LOW);
  delayMicroseconds(500);
  
  if(bitRead(input,i)==1)
  digitalWrite(transmit, LOW);
  else
  digitalWrite(transmit, HIGH);
  delayMicroseconds(500);
}//i

  
  digitalWrite(transmit, LOW);
}

void serv(){
while(digitalRead(service) == HIGH){ //service function (will continue run until service button is pushed again)
  error = 100000000;                 //make sure error variable is default
unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {  //lighthouse blink (boiler from the examples of arduino ide) better not run as a function (problem's during testing)
    previousMillis = currentMillis;  //used by blink boiler 
    if (ledState == LOW){
      ledState = HIGH;}else{
      ledState = LOW;}
    digitalWrite(light, ledState);
}
   error = error + 11111111;        //update service to the error variable
  rf_send(error);                       //call apropriate function to send the error
  rf_send('#');                         //call apropriate function to send terminal tansmission
  delay(30);                            //wait very little
  digitalWrite(testled, HIGH);          //activate testled for debugging
  delay(30);                            //wait very little
  digitalWrite(testled, LOW);           //deactivate testled for debugging
  digitalWrite(light, LOW);             //make sure the lighthouse light is off (bug during tests)
}
}

