#include <LoRa.h>
#include "boards.h"

int counter = 0;

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);
    Serial.begin(9600);
    Serial.println("LoRa Sender");
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    LoRa.setSpreadingFactor(8); // from 7 to 12
}

void loop()
{
    Serial.print("Sending packet: ");
    Serial.println(counter);

    // Generate a random number of "3" between 3 and 20
    int numOfThree = random(3, 21);

    // send packet
    LoRa.beginPacket(true);
    for (int i = 0; i < numOfThree; i++) {
        LoRa.print("Mensaje de prueba");
    }
    LoRa.endPacket();

#ifdef HAS_DISPLAY
    if (u8g2) {
        char buf[256];
        u8g2->clearBuffer();
        u8g2->drawStr(0, 12, "Transmitting: OK!");
        snprintf(buf, sizeof(buf), "Sending: %d", counter);
        u8g2->drawStr(0, 30, buf);
        u8g2->sendBuffer();
    }
#endif
    counter++;

    // Generate a random delay between 50ms and 300ms
    int randomDelay = random(100, 250);
    delay(randomDelay);
}
