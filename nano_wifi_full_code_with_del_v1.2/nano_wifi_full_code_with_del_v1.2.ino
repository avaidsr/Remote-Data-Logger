#include "SoftwareSerial.h"
String ssid="rohan";
String pass="avcd1234";
String ssidh="yash";
String passh="9971650138";

SoftwareSerial wifi(10, 11);

String server="avaidsr.000webhostapp.com";
//String url="add_data.php";

float vout = 0.0; //reading from 0 - 5V
float vin = 0.0; //calculated reading
float R1 = 1019000.0; // resistance of R1 (1M) 
float R2 = 97800.0;  // resistance of R2 (100K)

float vina = 0.0;
float R1a = 1000000.0; // resistance of R1 (1M) -see text!
float R2a = 100000.0;  // resistance of R2 (100K) - see text!

int value = 0; //arduino reading
float volt; //voltage value to be sent

int num;

String tcp;
String url;
String urldel;
String valdel="jaipur";



void setup() {
  pinMode(A2, INPUT);
  volt = 0.0;

  urldel = "GET /remove_data.php?valdel=";
  urldel +="jaipur";
  urldel += "&amp; HTTP/1.1\r\nHost: ";
  urldel +="avaidsr.000webhostapp.com\r\n";
  urldel += "Content-Type: application/x-www-form-urlencoded\r\n\r\n";
  
  url = "GET /add_data.php?val1=jaipur&val2=";
  url +="13.22";
  url += "&amp; HTTP/1.1\r\n";
  url += "Host: avaidsr.000webhostapp.com\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n";
 
  Serial.begin(9600); //start serial comm at 9600 baud rate  
  wifi.begin(9600); //start comm with esp8266 at 9600 baud rate

  //esp8266
  kick();
  cwmode();
  cipmux();
  connect_wifi();
}

void loop() {

  volt = volt_calculator();
  num=1;

  tcp_start();
  num++;
  delay(5000);
  tcp_start();
  
  
  //url_size();
  //data_recieve();
  //tcp_end();
  //data_recieve();
  delay(60000);
  
}

//esp8266 functions start
void kick() {
  wifi.print("AT\r\n");
  if(wifi.find("OK"))
  {
     Serial.println("AT_OK");
  }
  else {
    Serial.println("AT ERROR");
    kick();
  }
}

void cwmode() {
  wifi.println("AT+CWMODE=1");
  delay(200);
  if(wifi.find("OK")){
    Serial.println("CWMODE=1");
  }
  else {
    Serial.println("CWMODE ERROR");
    cwmode();
  }
}
void cipmux() {
  wifi.print("AT+CIPMUX=0\r\n");
  delay(200);
  if(wifi.find("OK"))
  {
     Serial.println("MUX=0");
  }
  else {
    Serial.println("cipmux ERROR");
    cipmux();
  }
}

void connect_wifi() {
  wifi.print("AT+CWJAP=\"yash\",\"9971650138\"\r\n");
  delay(300);
  if(wifi.find("OK"))
  {
     Serial.println("Connected to wifi");
  }
  else {
    Serial.println("Wifi Connect ERROR");
    connect_wifi();
  }
}

void tcp_start() {
  wifi.print("AT+CIPSTART=\"TCP\",\"avaidsr.000webhostapp.com\",80\r\n");
  delay(300);
  if(wifi.find("OK"))
  {
     Serial.println("SERVER CONNECT TCP");
     url_size();
  }
  else if(wifi.find("ERROR")) {
    Serial.println("TCP ERROR");
    tcp_start();
  }
  else {
    Serial.println("dont know");
    url_size();
    }
}

void url_size() {
  wifi.print("AT+CIPSEND=");
  switch(num) {
    case 1:wifi.println(urldel.length());
           delay(100);
           if(wifi.find(">")) {
              Serial.println("CIPSEND success");
              Serial.println(urldel.length());
              wifi.println(urldel); //space for sending url
              tcp_end();
              data_recieve();
           }
           else {
              Serial.println("CIPSEND ERROR");
              Serial.println(urldel.length());
              tcp_end();
              tcp_start();
           }
    break;
    case 2:wifi.println(url.length());
           delay(100);
           if(wifi.find(">")) {
              Serial.println("CIPSEND success");
              Serial.println(url.length());
              wifi.println(url); //space for sending url
              tcp_end();
              data_recieve();
           }
           else {
              Serial.println("CIPSEND ERROR");
              Serial.println(url.length());
              tcp_end();
              tcp_start();
           }
    break;
    default:Serial.println("no value for cipsend");
  }
  
}

void data_recieve() {
  if(wifi.find("Connected to MySQLData successfully inserted on voltage table")) {
    Serial.println("Data Recieved!!!!!!!!!!!!!!");
  }
  else if(wifi.find("Connected to MySQLdeleted")) {
    Serial.println("Data Deleted$$$$$$$$$$");
  }
  else {
    Serial.println("ERROR Data Not Recieved");
    tcp_end();
    tcp_start();
  }
}

void tcp_end() {
  wifi.println("AT+CIPCLOSE");
  //delay(100);
  if(wifi.find("OK")) {
    Serial.println("TCP connection closed");
  }
  else {
    Serial.println("TCP close ERROR");
    //tcp_end();
  }
}
//esp8266 functions end

//calculator
float volt_calculator() {

  float sum=0;
  for (int a=0;a<150;a++) //loop 150 times in 2 seconds
  {
     // read the value at analog input
   value = analogRead(A2);
   vout = value * (4.75 / 1023.0); // see text
   vin = vout / (R2/(R1+R2)); 
   vina = vout / (R2a/(R1a+R2a));
   if (vin<0.09) 
   {
    vin=0.0; //statement to quash undesired reading !
   } 
   // taking 150 samples from voltage divider with a interval of 2sec and then average the samples data collected for(int i=0;i<150;i++)

sum=sum+vina; //<original value - vina instead of vout> read the voltage from the divider circuit
delay (2);
  }

float sum1=sum/150;


   
  //Serial.println(vin);  //most accurate reading for now
  //Serial.println(vina);  //most accurate reading for now
  Serial.println(vout);
  Serial.println(sum1);
  Serial.println(" ");
  return sum1;
  //delay(5000);
}
