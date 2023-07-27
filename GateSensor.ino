// Import the necessary libraries
#include <ESP8266WiFi.h>  // WiFi Library
#include <ESPAsyncTCP.h>  // Libraries for async Webserver
#include <ESPAsyncWebSrv.h>
#include "html_code_1.h"    //html code
#include "Network_1.h"     //Password and SSid for local network
AsyncWebServer server(80);  //Webserver hosted on Port 80 (HTTP) 
WiFiClient client;

//Password and SSid for local network
String ssid = SSId;
String passwort = PASSWORD;

//Password and SSid for hotSpot
const char* ssid_HotSpot = "";   // Enter SSID here
const char* password_HotSpot = ""; // Enter Password here

//PINs and distance config for ultrasonic sensor
#define pingPin D5
#define echoPin D6
const int MaxDistance = 100; //Set the max distance in cm the Node reads for the gate to be closed

int Reset; //Reset for millis() timer


void initialization(){
    Serial.begin(9600);
    Serial.println("creating HotSpot");
    WiFi.softAP(ssid_HotSpot, password_HotSpot); // Startet den HotSpot mit der SSID und dem Passwort
    Serial.println(WiFi.localIP());
}

void setup()
{
    initialization();
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);
    
}
void loop()
{
    Serial.println(HH_MM_SS(millis()/1000));
    delay(100);
    Serial.println("Distanz: " + String(distanceCM()) + "cm");
}

String Status(){
    if(distanceCM() > 100){
        return "open";
    }
    else{
        return "closed";
    }
}
long distanceCM(){
    long duration,cm;

    // Trigger the ping
    
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
    duration = pulseIn(echoPin, HIGH);

    cm = duration/34/2;
    return cm;
}
String HH_MM_SS(long sec_num){
    String h;
    String m;
    String s;

    long hours   = floor(sec_num / 3600);
    long minutes = floor((sec_num - (hours * 3600)) / 60);
    long seconds = sec_num - (hours * 3600) - (minutes * 60);

    if (hours   < 10) {
        h = "0"+ String(hours);
        }
    else{
        h = String(hours);
    }
    if (minutes < 10) {
        m = "0"+ String(minutes);
        }
    else{
        m = String(minutes);
    }
    if (seconds < 10) {
        String second = "0"+ String(seconds);
        }
    else{
        s = String(seconds);
    }
    if (s == ""){
        s = "00";
    }
    return h + ':' + m + ':' + s;
}