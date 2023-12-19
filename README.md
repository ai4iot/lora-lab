# LoRa Communication Exercise
Objective:
The objective of this exercise is to establish communication between a LoRa transmitter and multiple LoRa receivers (one for each student in the class) using LoRa32-OLED boards. Several senders can be configured at the same time so that the receivers receive more messages, trying not to have more than 3 senders per 25 students due to packet collision.

Materials Required:
* LoRa32-OLED boards (1-3 transmitters, multiple receivers)
* Arduino IDE
* USB cables

## Instructions:

### Setting up Arduino IDE
* Install Arduino IDE: https://www.arduino.cc/en/software
* Open Arduino IDE and install dependencies:
  * arduino-lora: https://github.com/sandeepmistry/arduino-LoRa
  * u8g2-arduino: https://github.com/olikraus/U8g2_Arduino

### Sender
```c++
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

```

#### Setup Function:

* Initializes LoRa, sets pins, and handles initialization errors.
* Sets the spreading factor to 8, which determines the range and data rate.

#### Loop Function:

* Continuously transmits packets containing the test message "Test message"
> Note: You can modify this **test message** to whatever you want if you wish to have several emitters in the class.
* Updates the OLED display if available.
* Introduces a random delay between transmissions for variation.

#### Load script
* Open lora-sender folder in Arduino IDE
> Note: it is necessary to open the whole directory as there are .h files that are dependencies of the main script.
* Connect each board to a computer.
> Note: If using linux you may need to give sudo permissions to the port: `sudo chmod -R 777 /dev/ttyACM0 `
* Select **TTGO LoRa32-OLED** device.
* Load the script on the board and wait for it to initialize.
> Note: If you get errors, make sure that the **board has the correct permissions**, that you have **installed the libraries** and that you have opened the **folder containing the script**. 

### Receiver
```c++

#include <LoRa.h>
#include "boards.h"

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("LoRa Receiver");

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    LoRa.setSpreadingFactor(8);
}

void loop()
{
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        String recv = "";
        // read packet
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }

        Serial.println(recv);

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
#ifdef HAS_DISPLAY
        if (u8g2) {
            u8g2->clearBuffer();
            char buf[256];
            u8g2->drawStr(0, 12, "Received OK!");
            u8g2->drawStr(0, 26, recv.c_str());
            snprintf(buf, sizeof(buf), "RSSI:%i", LoRa.packetRssi());
            u8g2->drawStr(0, 40, buf);
            snprintf(buf, sizeof(buf), "SNR:%.1f", LoRa.packetSnr());
            u8g2->drawStr(0, 56, buf);
            u8g2->sendBuffer();
        }
#endif
    }
}

```

#### Setup Function:

* Initializes LoRa, sets pins, and handles initialization errors.
* Sets the spreading factor to 8, aligning it with the transmitter's settings.

#### Loop Function:

* Constantly checks for incoming packets.
* Upon receiving a packet, reads the message and retrieves RSSI and SNR.
* Updates the OLED display with the received message, RSSI, and SNR if available.

This code allows the receiver to continuously listen for incoming LoRa packets, retrieve the message, and display it along with signal strength information. The transmitter, on the other hand, sends out packets containing a test message at random intervals.

#### Load script
* Open lora-receiver folder in Arduino IDE
> Note: it is necessary to open the whole directory as there are .h files that are dependencies of the main script.
* Connect each board to a computer.
> Note: If using linux you may need to give sudo permissions to the port: `sudo chmod -R 777 /dev/ttyACM0 `
* Select **TTGO LoRa32-OLED** device.
* Load the script on the board and wait for it to initialize.
> Note: If you get errors, make sure that the **board has the correct permissions**, that you have **installed the libraries** and that you have opened the **folder containing the script**. 

## To-do
- [ ] Modify test message of the sender
- [ ] Modify the spread factor in the devices (7-12) and observe how the received messages vary.
- [ ] Try to move the receivers as far away as possible from the transmitter to see if the frames are still received correctly.