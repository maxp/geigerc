//
//  ethergeig
//

#include <SPI.h>
#include <SoftwareSerial.h>
#include <Ethernet.h>

char      progname[] = "ethergeig v0.0.2";
byte      mac[] = { 0x00, 0x15, 0xEE, 0x44, 0x01, 0x01 }; 
IPAddress default_ip(192, 168, 1, 99);

char  server_host[] = "10.41.41.30";
char  server_port   = 8000;
char  base_uri[]    = "/meteo/inp?";
char  station[]     = "npsd";
char  secret[]      = "top_secret_pass";


// use pin-8
SoftwareSerial gser(8, 9); // RX, TX

EthernetClient client;

const unsigned long MAX_ULONG = 0xffffffff; // 4294967295
const unsigned long INTERVAL = 60000;

unsigned long t0;


char qs[100];

char* make_qs() {
  qs[0] = 'a'; qs[1] = 'b'; qs[2] = 'c'; qs[3] = 0;
  return qs;
}

void setup() {

  Serial.begin(9600);
  // while (!Serial) { ; }  // this check is only needed on the Leonardo:
  Serial.println(progname);

  gser.begin(9600);
  t0 = millis();

  if (Ethernet.begin(mac) == 0) {
    Serial.println("dhcp request failed!");
    Ethernet.begin(mac, default_ip);
    // Ethernet.begin(mac, ip, dns, gateway, subnet);
  }  

  Serial.print("ip: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    if(thisByte) { Serial.print("."); }
    Serial.print(Ethernet.localIP()[thisByte], DEC);
  }
  Serial.println();
  
  // delay(1000);
}

void http_get() {
 if (client.connect(server_host, server_port)) {
    Serial.print("connected to host: ");
    Serial.print(server_host);
    Serial.print(":");
    Serial.print(server_port);
    Serial.println();
    
    client.print("GET ");
    client.print(base_uri);
    // client.print(qs);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server_host);
    client.println("Connection: close");
    client.println();
  } 
  else {
    Serial.print("connection failed: ");
    Serial.println(server_host);
  }
  
   pinMode(aPin, OUTPUT);
   pinMode(A3, OUTPUT);
}

int cnt = 0 ;

int i = 0;

void loop() {

  if (gser.available()) {
    char c = gser.read();
    cnt += 1;
    Serial.write('*');
  }
  
  unsigned long t1 = millis();
  unsigned long dt;
  
  if( t1 < t0 ) {
    dt = MAX_ULONG-t0+t1;
  }  
  else {
    dt = t1 - t0;  
  }
  
  if( dt >= INTERVAL )  {
    t0 += INTERVAL; 
    Serial.print("\n = ");
    Serial.println(cnt);
    cnt = 0;
  }

  
  // make_qs();
  
  // http_get();
  
//  if (client.available()) {
//    char c = client.read();
//    Serial.print(c);
//  }

//  if (!client.connected()) {
//    Serial.println();
//    Serial.println("disconnecting.");
//    client.stop();

}

//.

