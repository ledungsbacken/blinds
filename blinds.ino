#include <Ethernet.h>
#include <ArduinoHA.h> // https://github.com/dawidchyrzynski/arduino-home-assistant
#include "./Blind.h"

#define BROKER_ADDR      IPAddress(192,168,0,15) // 192.168.0.15
#define BROKER_USERNAME  "mqtt" // replace with your credentials
#define BROKER_PASSWORD  "mqtt"

byte mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};

EthernetClient client;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);
Blind blindOne(6, "blind-1", "Blind One", 1000);

void onBlindOneCommand(HACover::CoverCommand cmd) {
    blindOne.onCommand(cmd);
}

void setup() {
    Serial.begin(9600);
    Ethernet.begin(mac);

    // Set device's details for HA
    device.setName("Arduino");
    device.setSoftwareVersion("1.0.0");

    blindOne.init(onBlindOneCommand);

    mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
}

void loop() {
    Ethernet.maintain();
    mqtt.loop();
}
