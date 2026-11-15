#include <SPI.h>
#include "printf.h"
#include "RF24.h"

#define CE_PIN 4
#define CSN_PIN 5
#define LED 2

RF24 radio(CE_PIN, CSN_PIN);
const uint64_t ADDRESS = 0xF0F0F0F0E1LL;
unsigned long ledOffAt = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(1000);

  if (!radio.begin() || !radio.isChipConnected()) {
    Serial.println(F("radio hardware is not responding!!"));
    digitalWrite(LED, LOW);
    while (1) {}
  }
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(76);
  radio.setAutoAck(true);
  radio.openReadingPipe(1, ADDRESS);
  radio.startListening();

  printf_begin();
  radio.printPrettyDetails();   // compare with the transmitter's output
  digitalWrite(LED, HIGH);
}

void loop() {
  if (radio.available()) {
    uint16_t data[4];
    radio.read(&data, sizeof(data));
    Serial.print(F("receivedData: "));
    for (int i = 0; i < 4; i++) { Serial.print(data[i]); Serial.print(' '); }
    Serial.println();
    digitalWrite(LED, HIGH);
    ledOffAt = millis() + 100;
  }
  if (millis() > ledOffAt) digitalWrite(LED, LOW);
}
