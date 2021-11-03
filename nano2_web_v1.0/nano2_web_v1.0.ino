//data refresh every 30 seconds

#include <UIPEthernet.h>
#include <utility/logging.h>
//pin config for enc28j60 
//with mega :-
//CS - D53
//SI - D51
//SO - D50
//SCK - D52
//VCC - 3V3
//GND - GND

//volt
//initialised variables
float vout = 0.0; //reading from 0 - 5V
float vin = 0.0; //calculated reading
float R1 = 1019000.0; // resistance of R1 (1M) 
float R2 = 97800.0;  // resistance of R2 (100K)

float vina = 0.0;
float R1a = 1000000.0; // resistance of R1 (1M) -see text!
float R2a = 100000.0;  // resistance of R2 (100K) - see text!

int value = 0; //arduino reading
float volt; //voltage value to be sent
int n;


// **** ETHERNET SETTING ****
// Ethernet MAC address - must be unique on your network - MAC Reads T4A001 in hex (unique in your network)
byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };                                       
// For the rest we use DHCP (IP address and such)

EthernetClient client;
char server[] = "avaidsr.000webhostapp.com"; // IP Adres (or name) of server to dump data to
int  interval = 5000; // Wait between dumps





void setup() {
  
  pinMode(A2, INPUT);
  volt = 0.0;
  n = 0;
  
  Serial.begin(9600); //start serial comm at 9600 baud rate
  
  Ethernet.begin(mac);

  Serial.println("Remote Voltage Calc V-1.2");
  Serial.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
  Serial.print("IP Address        : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Subnet Mask       : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("Default Gateway IP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("DNS Server IP     : ");
  Serial.println(Ethernet.dnsServerIP());
}

void loop() {

  volt = volt_calculator();
  
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) {
    Serial.println("-> Connected");
    // Make a HTTP request:
    client.print( "GET /enctest1.php?");
    client.print("val1=");
    client.print( "Location-1" );
    client.print("&");
    client.print("val2=");
    client.print( "Battery-1" );
    client.print("&");
    client.print("val3=");
    client.print( volt );
    client.print("&");
    client.print("val4=");
    client.print( n );
    client.println( " HTTP/1.1");
    client.print( "Host: " );
    client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println( "Connection: close" );
    client.println();
    client.println();
    client.stop();
  }
  else {
    // you didn't get a connection to the server:
    Serial.println("--> connection failed/n");
  }
  n+=1;
  delay(interval);
  

}

float volt_calculator() {
   // read the value at analog input
   value = analogRead(A2);
   vout = value * (4.62 / 1023.0); // see text
   vin = vout / (R2/(R1+R2)); 
   vina = vout / (R2a/(R1a+R2a));
   if (vin<0.09) 
   {
    vin=0.0; //statement to quash undesired reading !
   } 
   
  Serial.println(vin);  //most accurate reading for now
  Serial.println(vina);  //most accurate reading for now
  Serial.println(" ");
  return vina;
  //delay(5000);
}


