#include <Ethernet.h>
#include <ArduinoHA.h> // https://github.com/dawidchyrzynski/arduino-home-assistant
#include <Servo.h>

#define SERVO_PIN        6
#define BROKER_ADDR      IPAddress(192,168,0,15) // 192.168.0.15
#define BROKER_USERNAME  "mqtt" // replace with your credentials
#define BROKER_PASSWORD  "mqtt"

byte mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};
enum ServoPosition {
    OPEN = 0,
    CLOSED = 180,
    STOPPED = 90
};

EthernetClient client;
Servo servo;
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);
HACover cover("blind-1"); // Unique ID of the "cover".

void onCoverCommand(HACover::CoverCommand cmd) {
    if (servo.attached()) {
        return;
    } else {
        servo.attach(SERVO_PIN);
    }
    if (cmd == HACover::CommandOpen) {
        Serial.println("Command: Open");
        servo.write(ServoPosition::OPEN);
        cover.setState(HACover::StateOpening);
        endServo(HACover::StateOpen);
    } else if (cmd == HACover::CommandClose) {
        Serial.println("Command: Close");
        servo.write(ServoPosition::CLOSED);
        cover.setState(HACover::StateClosing);
        endServo(HACover::StateClosed);
    } else if (cmd == HACover::CommandStop) {
        Serial.println("Command: Stop");
        servo.write(ServoPosition::STOPPED);
        cover.setState(HACover::StateStopped);
        endServo(HACover::StateStopped);
    }
}

void endServo(HACover::CoverState state)
{
    delay(1000);
    servo.detach();
    cover.setPosition(state);
    Serial.println(servo.read());
}

void setup() {
    Serial.begin(9600);
    Ethernet.begin(mac);

    // Set device's details for HA
    device.setName("Arduino");
    device.setSoftwareVersion("1.0.0");

    cover.onCommand(onCoverCommand);
    cover.setName("Blind One");
    // cover.setRetain(true); // optional

    mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
}

void loop() {
    Ethernet.maintain();
    mqtt.loop();
}
