#include <UIPEthernet.h>
#include <utility/logging.h>





//#include <UIPEthernet.h> // Used for Ethernet

// **** ETHERNET SETTING ****
// Arduino Uno pins: 10 = CS, 11 = MOSI, 12 = MISO, 13 = SCK
// Ethernet MAC address - must be unique on your network - MAC Reads T4A001 in hex (unique in your network)
byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };                                       
// For the rest we use DHCP (IP address and such)

EthernetClient client;
char server[] = "avaidsr.000webhostapp.com"; // IP Adres (or name) of server to dump data to
int  interval = 1000; // Wait between dumps

void setup() {

  Serial.begin(9600);
  Ethernet.begin(mac);

  Serial.println("Tweaking4All.com - Temperature Drone - v2.0");
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
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("-> Connected");
    // Make a HTTP request:
    client.print( "POST /public_html/enctest1.php HTTP/1.1");
    client.print("Battery=");
    client.print( "Battery_1" );
    client.print("&&");
    client.print("volatege=");
    client.print( "12.67" );
    client.println( " HTTP/1.1");
    client.print( "Host: " );
    client.println(server);
    client.println( "Connection: close" );
    client.println();
    client.println();
    client.stop();
  }
  else {
    // you didn't get a connection to the server:
    Serial.println("--> connection failed/n");
  }

  delay(interval);
}

