// Arduino demo sketch for testing RFM12B + ENC28J60 ethernet
// Listens for RF12 messages and displays valid messages on a webpage
// Memory usage exceeds 1K, so use Atmega328 or decrease history/buffers
//
// This sketch is derived from RF12eth.pde:
// May 2010, Andras Tucsni, http://opensource.org/licenses/mit-license.php
// The EtherCard library is based on Guido Socher's driver, licensed as GPL2. 
// Based on Mods by jcw, 2010-05-20, Jeelabs.org 
// 
// Author: Glyn Hudson and Trystan lea openenergymonitor.org
// Date: 2/5/2011
//
// mods: working  John Beale - May 5-7 2011
// based on info in http://tuxgraphics.org/electronics/200905/embedded-tcp-ip-stack.shtml
// 22   162 ms   162 ms   162 ms  pepper.tuxgraphics.org [77.37.2.152]

#include <EtherCard.h>
#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

// ethernet interface mac address
// D4-28-B2-FF-E2-F6 
static byte mymac[6] = { 0xd4,0x28,0xb2,0xff,0xe2,0xf6 };
// ethernet interface ip address
static byte myip[4] = { 192,168,10,140 };
// gateway ip address
static byte gwip[4] = { 192,168,10,1 };
// remote website ip address and port
static byte hisip[4] = { 184,106,153,149 };  // address of remote server: ThingSpeak

static word hisport = 80;

// fixed RF12 settings
#define MYNODE 31            //node ID of nanode
#define freq RF12_915MHZ     //frequency
#define group 212            //network group 

//################################################################
//Data Structure to be received 
//################################################################
typedef struct {              //data structure to be received, must be same as on transmitter 
 int sec;
 int power;
 int temp;
} Payload;
Payload measurement; 
//########################################################

EtherCard eth;
MilliTimer requestTimer;
int test=1767;
unsigned long lastRead;    // millisecond timestamp of last analog input reading

static BufferFiller bufill;
static byte buf[300];   // a very small tcp/ip buffer is enough here


char tsHeader[] PROGMEM =    "GET /update?key=MySecretKey&field1=";
       
 char host[] PROGMEM =     "api.thingspeak.com";    
    
// called to fill in a request to send out data to the client
static word my_datafill_cb (byte fd) {
    BufferFiller bfill = eth.tcpOffset(buf);
    
    bfill.emit_p(PSTR("$F$D&field2=$D&field3=$D"), 
      tsHeader,measurement.sec,measurement.power,measurement.temp);
    bfill.emit_p(PSTR(" HTTP/1.1\r\n" "Host: $F\r\n" "\r\n"),host);
      
    return bfill.position();
}

// called when the client request is complete
static byte my_result_cb (byte fd, byte status, word off, word len) {
    Serial.print("<<< reply ");
    Serial.println((int) status);
    Serial.print((const char*) buf + off);
    return 0;
}

void setup () {
    Serial.begin(57600);
    Serial.println("\n[EtherServe test]");
    char ipaddr[18];
    mk_net_str(ipaddr,myip,4,'.',10);
    Serial.print("My IP: ");
    Serial.println(ipaddr);
        
    eth.spiInit();
    eth.initialize(mymac);
    eth.initIp(mymac, myip, 80);
    eth.clientSetGwIp(gwip);    // outgoing requests need a gateway
    eth.clientSetServerIp(hisip);
    
    rf12_initialize(MYNODE, freq,group);
    delay(1000);             // wait a second, just to be on safe side
    pinMode(0,INPUT);        // use pin 0 as (analog) input
    requestTimer.set(1); // send first request as soon as possible
    
    }
    
char okHeader[] PROGMEM = 
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    ;

static void homePage(BufferFiller& buf) {
   buf.emit_p(PSTR("$F\r\n"
   "<html><body>Analog Data<br>Time:$D Power:$D Temp:$D</body></html>"),
     okHeader,measurement.sec,measurement.power,measurement.temp);
}

void loop () {
    word len = eth.packetReceive(buf, sizeof buf);
    word pos = eth.packetLoop(buf, len);   // receive data from ENC28J60
 
    if (pos) {  // pos !=0 of there was a valid HTTP GET received
      bufill = eth.tcpOffset(buf);
      char* data = (char *) buf + pos;
      Serial.println(data);

       //receive buf hasn't been clobbered by reply yet
       if (strncmp("GET / ", data, 6) == 0) homePage(bufill); 
        
       eth.httpServerReply(buf,bufill.position()); // send web page data
    }
       
       
    // Receive data from RFM12
    //if (rf12_recvDone() && rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0  )  
    if (rf12_recvDone() && rf12_crc == 0 && rf12_len==sizeof(Payload) ) {
        measurement=*(Payload*) rf12_data;      //decode packet binary data into known data structure (same as Tx) http://jeelabs.org/2010/12/08/binary-packet-decoding-%E2%80%93-part-2/
        Serial.print("Data: "); Serial.println(measurement.power);        
     } else {
       if ((millis() - lastRead) > 1000) {
         lastRead = millis();
         measurement.sec = (int) (lastRead / 1000);
         measurement.power = analogRead(0); 
         measurement.temp = analogRead(1);
       }
     }
        
    
    if (eth.clientWaitingGw())
        return;
    
    if (requestTimer.poll(120000)) {
        Serial.print(">>> PUT# ");
        byte id = eth.clientTcpReq(my_result_cb, my_datafill_cb, hisport);
        Serial.println((int) id);
    }
}
