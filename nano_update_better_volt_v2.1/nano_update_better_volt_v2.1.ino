#define aref_voltage 3.3         // we tie 3.3V to ARef and measure it with a multimeter!
#include "SoftwareSerial.h"
String ssid="rohan";
String pass="avcd1234";
//ssidc="cosmic";
//passc="9910005330";
//ssidh="yash";
//passh="9971650138";
SoftwareSerial wifi(10, 11);  //rx=d10  tx=d11

String server="avaidr.000webhostapp.com";
//String url="add_data.php";

float vout = 0.0; //reading from 0 - 5V
float vin = 0.0; //calculated reading

float R1 = 1010000.0; // resistance of R1 (1M) -see text!
float R2 = 98500.0;  // resistance of R2 (100K) - see tex

float vina = 0.0;
float R1a = 1000000.0; // resistance of R1 (1M) -see text!
float R2a = 100000.0;  // resistance of R2 (100K) - see text!

int value = 0; //arduino reading
float volt; //voltage value to be sent

float voltage;
float sum;

int Reading;        // the analog reading from the sensor

int num;
float volte=13.11;

String tcp;
String url;

void setup() {
  analogReference(EXTERNAL);
  pinMode(A2, INPUT);
  volt = 0.0;
  num = 0;
 
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
  url = "GET /update_data.php?valup=";
  url +=volte;
  url += "&valid=1&amp; HTTP/1.1\r\n";
  url += "Host: avaidsr.000webhostapp.com\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n";

  tcp_start();
  url_size();
  //data_recieve();
  tcp_end();
  data_recieve();
  delay(2000);
  
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
  }
  else if(wifi.find("ERROR")) {
    Serial.println("TCP ERROR");
    tcp_start();
  }
  else {
    Serial.println("dont know");
  }
}

void url_size() {
  wifi.print("AT+CIPSEND=");
  wifi.println(url.length());
  delay(100);
  if(wifi.find(">")) {
     Serial.println("CIPSEND success");
     Serial.println(url.length());
     wifi.println(url); //space for sending url
  }
  else {
    Serial.println("CIPSEND ERROR");
    Serial.println(url.length());
    tcp_end();
    //tcp_start();
  }
}

void data_recieve() {
  if(wifi.find("Connected to MySQLSuccess! record updated / deleted")) {
    Serial.println("Data Recieved!!!!!!!!!!!!!!");
  }
  else {
    Serial.println("ERROR Data Not Recieved");
   // tcp_end();
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

  sum=0;
  for(int j=0;j<=200;j++){
 
  Reading = analogRead(A2);  
 
//  Serial.print("Temp reading = ");
//  Serial.print(Reading);     // the raw analog reading
// 
//  // converting that reading to voltage, which is based off the reference voltage
   voltage = Reading * aref_voltage;
  voltage /= 1024.0;
  sum=sum+voltage;
   
  delay(2);}
  sum=sum/201;
//  float vina = voltage / (R2a/(R1a+R2a));
//  float vin = voltage / (R2/(R1+R2));
float vina = sum*11;
float vin = sum*11.25;
 
  // print out the voltage
  Serial.print(" - ");
  Serial.print(voltage); Serial.print(" volts");
  Serial.print(" - ");
  Serial.print(vina); Serial.print(" volts");
  Serial.print(" - ");
  Serial.print(vin); Serial.println(" volts");
 
//  // now print out the temperature
//  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
//                                               //to degrees ((volatge - 500mV) times 100)
//  Serial.print(temperatureC); Serial.println(" degrees C");
// 
//  // now convert to Fahrenheight
//  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
//  Serial.print(temperatureF); Serial.println(" degrees F");
  return vin;
  delay(1000);
  //delay(5000);
}
