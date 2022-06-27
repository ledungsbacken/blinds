#include <ArduinoHA.h> // https://github.com/dawidchyrzynski/arduino-home-assistant
#include <Servo.h>

class Blind
{
protected:
    enum ServoPosition
    {
        OPEN = 0,
        CLOSED = 180,
        STOPPED = 90
    };
    ServoPosition currentServoPosition;
private:
    int servoPin;
    const char *uniqueId;
    const char *name;
    Servo servo;
    HACover cover;
    unsigned long delayMS;

    void attachServo();
    void detachServo();
    void writeServo(Blind::ServoPosition position);
    void setCoverState(HACover::CoverState state);
    void setCoverPosition(HACover::CoverState state);
public:
    Blind(int servoPin, const char *uniqueId, const char *name, unsigned long delayMS);

    void action(ServoPosition position, HACover::CoverState intermediateState, HACover::CoverState finalState);
    void open();
    void close();
    void stop();
    bool isBusy();
    void init(void (*callback)(HACover::CoverCommand cmd));
    void onCommand(HACover::CoverCommand cmd);
};
