/*/
Created by Lukas Kramer

Github: https://github.com/lukas-kramer07/BME-GateSensor


Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

ESP Mail code adapted from https://github.com/mobizt/ESP-Mail-Client
/*/




//==================================================================================================================================================
// Import the necessary libraries
#include <ESP8266WiFi.h>  // WiFi Library
#include <ESPAsyncTCP.h>  // Libraries for async Webserver
#include <ESPAsyncWebSrv.h>
#include "html_code.h"    //html code
#include "network.h"     //Password and SSid for local network
#include <ESP_Mail_Client.h> //ESP Mail library

//==================================================================================================================================================
//Webserver
AsyncWebServer server(80);  //Webserver hosted on Port 80 (HTTP) 
WiFiClient client;

//Password and SSid for local network
String ssid = SSid;
String password = PASSWORD;

//Password and SSid for hotSpot
const char* ssid_HotSpot = "<SSid>";   // Enter SSID here
const char* password_HotSpot = "<password>"; // Enter Password here

//==================================================================================================================================================
//PINs and distance config for ultrasonic sensor
#define pingPin D6
#define echoPin D5
const int MaxDistance = 100; //Set the max distance in cm the Node reads for the gate to be closed

//==================================================================================================================================================
int reset=0; //reset for millis() timer
String status;
//==================================================================================================================================================
//E-MAIL
/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "smtp.gmail.com"

/** The smtp port e.g.
 * 25  or esp_mail_smtp_port_25
 * 465 or esp_mail_smtp_port_465
 * 587 or esp_mail_smtp_port_587
 */
#define SMTP_PORT esp_mail_smtp_port_587

/* The log in credentials */
#define AUTHOR_EMAIL ""
#define AUTHOR_PASSWORD ""

/* Recipient email address */
#define RECIPIENT_EMAIL ""

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

int timeToMail = 1200000; //time to send a mail, when door is open (20min)
//==================================================================================================================================================

void initialization(){
    pinMode(pingPin, OUTPUT);
    pinMode(echoPin, INPUT);

    Serial.begin(9600);
    WiFi.begin(ssid, password);                     
    while (WiFi.status() != WL_CONNECTED) { //waiting for WIFI connection
      delay(1000);
      Serial.println("...");
    }

    Serial.println("creating HotSpot");
    WiFi.softAP(ssid_HotSpot, password_HotSpot); // Startet den HotSpot mit der SSID und dem Passwort
    Serial.println(WiFi.localIP());
}

void setup()
{
    initialization();
    
    
    
    // Website load route, html code and BME values are sent
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, values_onload);
    });
    server.on("/Status", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", Status().c_str());
    });
    server.on("/Timer", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", HH_MM_SS((millis()-reset)/1000).c_str());
    });
    /*/server.on("/Reset", HTTP_GET, [](AsyncWebServerRequest *request){
        Reset();
    });/*/

    //starting the Webserver
    server.begin();
}

void loop(){
  delay(300);
  if(millis()-reset > timeToMail && Status() == "open"){
    SendMAil();
    delay(timeToMail);
  }
  StatusCheck();
}

void Reset(){
    reset = millis();
}

String values_onload(const String& var){
  if(var == "Status"){
    return Status();
  }
  else if(var == "Timer"){
    return HH_MM_SS((millis()-reset)/1000);
  }
  return String();
}


String Status(){
    if(distanceCM() > 100){
        return "open";
    }
    else{
        return "closed";
    }
}
void StatusCheck(){
  String statusTemp;
    if(distanceCM() > 100){
        statusTemp = "open";
    }
    else{
        statusTemp = "closed";
    }
    if(status != statusTemp && status !=""){
      Serial.println("Reset");
      Reset();
    }
  status = statusTemp;
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
String HH_MM_SS(long sec_num) {
    String h;
    String m;
    String s;

    long hours = floor(sec_num / 3600);
    long minutes = floor((sec_num - (hours * 3600)) / 60);
    long seconds = sec_num - (hours * 3600) - (minutes * 60);

    if (hours < 10) {
        h = "0" + String(hours);
    } else {
        h = String(hours);
    }

    if (minutes < 10) {
        m = "0" + String(minutes);
    } else {
        m = String(minutes);
    }

    if (seconds < 10) {
        s = "0" + String(seconds);
    } else {
        s = String(seconds);
    }

    if (s == "") {
        s = "00";
    }

    return h + ':' + m + ':' + s;
}


//==================================================================================================================================================
//E-MAIL
void smtpCallback(SMTP_Status status)
{
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success())
  {
    // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}
void SendMAil(){
  Serial.println("SendMAil");
  /*  Set the network reconnection option */
  MailClient.networkReconnect(true);
  /** Enable the debug via Serial port
   * 0 for no debugging
   * 1 for basic level debugging
   *
   * Debug port can be changed via ESP_MAIL_DEFAULT_DEBUG_PORT in ESP_Mail_FS.h
   */
  smtp.debug(1);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the Session_Config for user defined session credentials */
  Session_Config config;

  /* Set the session config */
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  /* The full message sending logs can now save to file */
  /* Since v3.0.4, the sent logs stored in smtp.sendingResult will store only the latest message logs */
  // config.sentLogs.filename = "/path/to/log/file";
  // config.sentLogs.storage_type = esp_mail_file_storage_type_flash;

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("ESP Mail");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Status Gate: open");
  message.addRecipient(F("Admin"), RECIPIENT_EMAIL);
  long time_open = (millis()-reset)/1000/60;
  String htmlMsg = "<h1>Your Gate has been open for a prolonged period of time: "+String(time_open)+"min</h1><p>The message was sent via ESP device.</p>";
  if(time_open >= 60){
    long hours = floor(time_open / 60);
    long minutes = (time_open - (hours * 60));
    htmlMsg = "<h1>Your Gate has been open for a prolonged period of time: "+String(hours)+"h "+String(minutes)+"min</h1><p>The message was sent via ESP device.</p>";
  }
  message.html.content = htmlMsg;

  /** The html text message character set e.g.
   * us-ascii
   * utf-8
   * utf-7
   * The default value is utf-8
   */
  message.html.charSet = F("us-ascii");

  /** The content transfer encoding e.g.
   * enc_7bit or "7bit" (not encoded)
   * enc_qp or "quoted-printable" (encoded)
   * enc_base64 or "base64" (encoded)
   * enc_binary or "binary" (not encoded)
   * enc_8bit or "8bit" (not encoded)
   * The default value is "7bit"
   */
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /** The message priority
   * esp_mail_smtp_priority_high or 1
   * esp_mail_smtp_priority_normal or 3
   * esp_mail_smtp_priority_low or 5
   * The default value is esp_mail_smtp_priority_low
   */
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;

  /** The Delivery Status Notifications e.g.
   * esp_mail_smtp_notify_never
   * esp_mail_smtp_notify_success
   * esp_mail_smtp_notify_failure
   * esp_mail_smtp_notify_delay
   * The default value is esp_mail_smtp_notify_never
   */
  // message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;

  /* Set the custom message header */
  message.addHeader(F("Message-ID: ESP-Message"));

  /* Set the TCP response read timeout in seconds */
  // smtp.setTCPTimeout(10);

  /* Connect to the server */
  if (!smtp.connect(&config))
  {
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (smtp.isAuthenticated())
    Serial.println("\nSuccessfully logged in.");
  else
    Serial.println("\nConnected with no Auth.");

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

  // to clear sending result log
  // smtp.sendingResult.clear();

  ESP_MAIL_PRINTF("Free Heap: %d\n", MailClient.getFreeHeap());
}

