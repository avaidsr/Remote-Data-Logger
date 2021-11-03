#include "SoftwareSerial.h"
SoftwareSerial wifi(10, 11);

//my web site, replace with yours
#define DST_IP "avaidsr.000webhostapp.com"
 
//Define the pin for the on board LED
float led = 13.2;
 
//Initialize Device
void setup()
{
wifi.begin(9600);
  
//Enable the Onboard LED to act as a Status indicator light
//pinMode(led,OUTPUT);
//Set the speed for communicating with the ESP8266 module
Serial.begin(9600);
//Send a series of flashes to show us we are in the bootup phase.
//blinkcode ();
//Reset the ESP8266
wifi.println("AT+RST");
//Wait for the WiFi module to bootup
delay(800);
//Switch the chip to "client" mode
//Serial.println("AT+CWMODE=1");
//Connect to our Wireless network NOTE: change SSID to your wireless
//network and PASSWORD to the key used to join the network.
wifi.println("AT+CWJAP=\"rohan\",\"avcd1234\"");
//Once again blink some lights because it is fun!
//blinkcode ();
}
 
void loop()
{
//Open a connection to the web server
String cmd = "AT+CIPSTART=\"TCP\",\""; //make this command: AT+CPISTART="TCP","192.168.88.35",80
cmd += DST_IP;
cmd += "\",80";
wifi.println(cmd);
 
//wait a little while for 'Linked'
delay(300);
 
//This is our HTTP GET Request change to the page and server you want to load.
cmd = "GET /add_data.php?val1=zzlon&amp;val2=14.2 HTTP/1.1\r\n";
cmd += "Host: avaidsr.000webhostapp.com\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n";
 
//The ESP8266 needs to know the size of the GET request
wifi.print("AT+CIPSEND=");
wifi.println(cmd.length());
 
//Look for the > prompt from the esp8266
if(wifi.find(">"))
{
//Send our http GET request
wifi.println(cmd);
}
else
{
//Something didn't work...
wifi.println("AT+CIPCLOSE");
}
 
//Check the returned header & web page. Looking for a keyword. I used YES12321
 
if (wifi.find("Connected to MySQLData successfully inserted on voltage table"))
{
//If the string was found we know the page is up and we turn on the LED status
//light to show the server is ONLINE
Serial.println("REcieved");
}
else
{
//If the string was not found then we can assume the server is offline therefore
//we should turn of the light.
Serial.println("not recieved");
}
//Wait a second for everything to settle.
delay(1000);
//Be great people and close our connection.
Serial.println("AT+CIPCLOSE");
 
//Introduce a delay timer before we finish and go back to the begining.
delay (5000);
}
 
//void blinkcode ()
//{
////Simply loop a few times flashing the status light (this is used during boot up)
//int i;
//for (i = 1; i <= 10; i++){
//delay (100);
//digitalWrite(led,HIGH);
//delay (100);
//digitalWrite(led,LOW);
//}
//}
