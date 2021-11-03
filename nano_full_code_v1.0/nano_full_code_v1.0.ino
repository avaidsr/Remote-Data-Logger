#include "SoftwareSerial.h"
String ssid="yash";
String pass="9971650138";
SoftwareSerial wifi(10, 11);

String server="avaidr.000webhostapp.com";
String url="add_data.php";

void setup() {
  // put your setup code here, to run once:
  wifi.begin(9600);
  Serial.begin(9600);


  wifi.print("AT\r\n");
  if(wifi.find("OK"))
  {
     Serial.println("OK");
  }
  else {
    Serial.println("ERROR");
  }

  wifi.print("AT+CWJAP=\"rohan\",\"avcd1234\"\r\n");
  delay(4000);
  if(wifi.find("OK"))
  {
     Serial.println("Connected");
  }
  else {
    Serial.println("ERROR");
  }

   delay(2000);
  wifi.print("AT+CIPMUX=0\r\n");
  if(wifi.find("OK"))
  {
     Serial.println("MUX=0");
  }
  else {
    Serial.println("ERROR");
  }
 
}

void loop() {
  // put your main code here, to run repeatedly:


  // delay(2000);
  wifi.print("AT+CIPSTART=\"TCP\",\"avaidsr.000webhostapp.com\",80\r\n");
  delay(100);
  if(wifi.find("OK"))
  {
     Serial.println("SERVER CONNECT");
     wifi.println("AT+CIPSEND=
       wifi.println("GET /add_data.php?val1=zzz&val2=14 HTTP/1.1");
       wifi.println("Host: avaidsr.000webhostapp.com");
       wifi.println("Content-Type: application/x-www-form-urlencoded");
       wifi.println(" ");
       Serial.println("sent");
  }
  else {
    Serial.println("ERROR");
  }




  //delay(1000);
   wifi.print("AT+CIPCLOSE\r\n");
  delay(2000);

//  delay(2000);
//wifi.print("AT+CWQAP\r\n");
//  if(wifi.find("OK"))
//  {
//     Serial.println("Disconnected");
//  }
//  else {
//    Serial.println("ERROR");
//  }

 
  Serial.println("  ");

//delay(2000);
}
