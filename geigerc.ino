//
//  geigerc
//

#include <SPI.h>
#include <SoftwareSerial.h>
#include <Ethernet.h>

char      progname[] = "geigerc v0.0.6";
byte      mac[] = { 0x00, 0x15, 0xEE, 0x44, 0x01, 0x01 }; 
IPAddress default_ip(192, 168, 1, 99);

char  server_host[] = "angara.net";
int   server_port   = 80;
char  base_uri[]    = "/meteo/in?st=";
char  station[]     = "npsd";
char  password[]    = "";

// analog indicator
const int apin = 3;

// use pin-8
SoftwareSerial gser(8, 9); // RX, TX

EthernetClient client;

const unsigned long MAX_ULONG = 0xffffffff; // 4294967295
const unsigned long INTERVAL = 60000;

unsigned long t0;


void setup() {

  pinMode(apin, OUTPUT);
  analogWrite(apin, 0);
  
  Serial.begin(9600);
  // while (!Serial) { ; }  // this check is only needed on the Leonardo:
  Serial.println(progname);

  gser.begin(9600);
  t0 = millis();

  if (Ethernet.begin(mac) == 0) {
    Serial.println("dhcp failed!");
    Ethernet.begin(mac, default_ip);
  }  

  Serial.print("ip: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    if(thisByte) { Serial.print("."); }
    Serial.print(Ethernet.localIP()[thisByte], DEC);
  }
  Serial.println();
}

void send(int v) {
 if (client.connect(server_host, server_port)) {
    Serial.print("send - "); Serial.print(server_host); Serial.print(":"); Serial.print(server_port);
    
    client.print("GET "); client.print(base_uri); client.print(station);
    if(password[0]) {
      client.print("&psw="); client.print(password);
    }  
    client.print("&gcn="); client.print(v);
    client.println(" HTTP/1.0");
    client.print("Host: "); client.println(server_host);
    client.println("Connection: close"); client.println();
    delay(10);
    client.stop();
    Serial.println();
  } 
  else {
    Serial.print("failed! "); Serial.print(server_host); Serial.print(":"); Serial.println(server_port);
  }
}

int cnt = 0 ;

void loop() {

  if (gser.available()) {
    char c = gser.read();
    cnt += 1;
    Serial.write('*');
  }
  
  unsigned long t1 = millis();
  unsigned long dt;
  
  if( t1 >= t0 ) { dt = t1 - t0; }
  else { dt = MAX_ULONG - t0 + t1; }
  
  if( dt >= INTERVAL )  {
    t0 += INTERVAL; 
    int c = cnt; cnt = 0;
    Serial.print(" "); Serial.println(c);
    send(c);
  }

  analogWrite( apin, 50+cnt*10 );
}

//.

