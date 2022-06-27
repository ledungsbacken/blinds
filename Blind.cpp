#include "./Blind.h"
#include <ArduinoHA.h> // https://github.com/dawidchyrzynski/arduino-home-assistant
#include <Servo.h>

void Blind::onCommand(HACover::CoverCommand cmd)
{
    if (cmd == HACover::CommandOpen) {
        open();
    } else if (cmd == HACover::CommandClose) {
        close();
    } else if (cmd == HACover::CommandStop) {
        stop();
    }
}

Blind::Blind(int servoPin, const char *uniqueId, const char *name, unsigned long delayMS) : cover(uniqueId)
{
    Blind::servoPin = servoPin;
    Blind::uniqueId = uniqueId;
    Blind::name = name;
    Blind::delayMS = delayMS;
    cover.setName(name);
}

void Blind::action(ServoPosition position, HACover::CoverState intermediateState, HACover::CoverState finalState)
{
    if (isBusy())
    {
        return;
    }

    attachServo();
    servo.write(position);
    setCoverState(intermediateState);

    delay(delayMS);

    detachServo();
    setCoverPosition(finalState);
}

bool Blind::isBusy()
{
    return servo.attached();
}

void Blind::init(void (*callback)(HACover::CoverCommand cmd))
{
    cover.onCommand(callback);
}

void Blind::open()
{
    action(ServoPosition::OPEN, HACover::StateOpening, HACover::StateOpen);
}

void Blind::close()
{
    action(ServoPosition::CLOSED, HACover::StateClosing, HACover::StateClosed);
}

void Blind::stop()
{
    action(ServoPosition::STOPPED, HACover::StateStopped, HACover::StateStopped);
}

void Blind::attachServo()
{
    servo.attach(servoPin);
}

void Blind::detachServo()
{
    servo.detach();
}

void Blind::writeServo(ServoPosition position)
{
    servo.write(position);
}

void Blind::setCoverState(HACover::CoverState state)
{
    cover.setState(state);
}

void Blind::setCoverPosition(HACover::CoverState state)
{
    cover.setPosition(state);
}
