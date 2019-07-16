#include <SPI.h>
#include <Ethernet.h>
#include <PubNub.h>

byte mac[] = {0x00, 0x0E, 0xEF, 0x00, 0x00, 0x32};

const int publedPin = 13;
const int analogIn = A0;

char pubkey[] = "pub-c-961a8cb6-eda7-4fb5-b6a3-d0f5b43db81e";
char subkey[] = "sub-c-759c3ac2-a3da-11e9-a74d-4e0ad457cfa9";
char channel[] = "iotchannel";


void setup() {
  // put your setup code here, to run once:
  pinMode(publedPin, OUTPUT);
  digitalWrite (publedPin , LOW);
  Serial.begin(9600) ;
  Serial.println("Serial Setup");
  while (!Ethernet.begin(mac)) {
    Serial.println("Ethernet setup error!");
    delay(1000);
  }
  Serial.println("Ethernet set up success");
  PubNub.begin(pubkey, subkey);
  Serial.println ("Pubnub set up");
}

void flash ( int ledPin) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Ethernet.maintain();

  EthernetClient *client;

  char msg [64] = "{\"eon\":{\"sensor\":";
  sprintf(msg + strlen(msg), "%d", analogRead(analogIn));
  strcat(msg, "}}");

  Serial.print("Publishing message: ");
  Serial.println(msg);
  client = PubNub.publish(channel, msg);
  if (!client) {
    Serial.println("Publishing error");
  } else {
    flash(publedPin);
    client ->stop();
  }

  delay (500);
}
