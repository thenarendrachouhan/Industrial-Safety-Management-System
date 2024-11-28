#define BLYNK_TEMPLATE_ID "TMPL38DN5hymx"
#define BLYNK_TEMPLATE_NAME "Major Project"
#define BLYNK_AUTH_TOKEN "wZU6pTPEsCpEnK8XoX2K7rbQE8FYW424"
/*************************************************************

  You can use this sketch as a debug tool that prints all incoming values
  sent by a widget connected to a Virtual Pin 1 in the Blynk App.

  App dashboard setup:
    Slider widget (0...100) on V1
 *************************************************************/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WiFiClient.h>

// Data wire is conntec to the Arduino digital pin 4
#define ONE_WIRE_BUS 4
#define Sensor 2
#define Buzzer 18
#define DO_PIN 14
#define RelayPin1 26  
#define RelayPin2 25

#define VPIN_BUTTON_1    V26
#define VPIN_BUTTON_2    V25
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

/* Fill-in information from Blynk Device Info here */


/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

int toggleState_1 = 0; //Define integer to remember the toggle state for relay 1
int toggleState_2 = 0; //Define integer to remember the toggle state for relay 2

int wifiFlag = 0;


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Redmiii";
char pass[] = "nnnnnnnn";

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
// Declaring a global variabl for sensor data
double sensorVal;
String fireState;
String smokeState; 

// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer; 


void myTimer() 
{
  // This function describes what will happen with each timer tick
  // e.g. writing sensor value to datastream V5
  Blynk.virtualWrite(V4, sensorVal);
  
    
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  sensors.begin();
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimer);
  

}

void loop()
{
  sensors.requestTemperatures(); 
  
  Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  delay(1000);
  sensorVal = sensors.getTempCByIndex(0); 
  


  bool value = digitalRead(Sensor);
  Serial.println(value);

  if (value == 0) {
    digitalWrite(Buzzer, HIGH);
    Serial.println("Fire Detected");
    Blynk.virtualWrite(V2, "Fire Detected");
  } else {
    digitalWrite(Buzzer, LOW);
    Serial.println("Fire not Detected");
    Blynk.virtualWrite(V2, "No Fire");
    

  }
  

  int gasState = digitalRead(DO_PIN);

  if (gasState == HIGH) {
    Serial.println("The gas is NOT present");
    Blynk.virtualWrite(V33, "Safe Air");
  }else {
    Serial.println("The gas is present");
    Blynk.virtualWrite(V33, "Toxins Detected");
  }

  // Runs all Blynk stuff
  Blynk.run(); 
  
  // runs BlynkTimer
  timer.run();
}

void relayOnOff(int relay){

    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(RelayPin1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(RelayPin1, HIGH); // turn off relay 1
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
              }
             delay(100);
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(RelayPin2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(RelayPin2, HIGH); // turn off relay 2
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
              }
             delay(100);
      break;
      default : break;      
      }  
}

void with_internet(){
    //Manual Switch Control
    if (digitalRead(RelayPin1) == LOW){
      delay(200);
      relayOnOff(1); 
      Blynk.virtualWrite(VPIN_BUTTON_1, toggleState_1);   // Update Button Widget  
    }
    else if (digitalRead(RelayPin2) == LOW){
      delay(200);
      relayOnOff(2);      
      Blynk.virtualWrite(VPIN_BUTTON_2, toggleState_2);   // Update Button Widget
    }
    
}


BLYNK_CONNECTED() {
  // Request the latest state from the server
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
 
}

// When App button is pushed - switch the state

BLYNK_WRITE(VPIN_BUTTON_1) {
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, toggleState_1);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, toggleState_2);
}



