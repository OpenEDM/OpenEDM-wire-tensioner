#include "StepperMotor.h"
#include "Arduino.h"

int StepperMotor::enable_pin = 0;

StepperMotor::StepperMotor(int stp_pin, int dir_pin)
{
    this->stp_pin = stp_pin;
    this->dir_pin = dir_pin;
}

void StepperMotor::init()
{
    pinMode(stp_pin, OUTPUT);
    pinMode(dir_pin, OUTPUT);
}

void StepperMotor::set_dir(direction_t dir)
{
    this->dir = dir;
    digitalWrite(dir_pin, dir);
}

void StepperMotor::step()
{
    digitalWrite(stp_pin, HIGH);
    digitalWrite(stp_pin, LOW);
}

void StepperMotor::steps(int number_of_steps, int delay_ms)
{
    for (int i = 0; i < number_of_steps; ++i) {
        step();
        delay(delay_ms);
    }
}

void StepperMotor::init_enable(int enable_pin)
{
    StepperMotor::enable_pin = enable_pin;
    pinMode(StepperMotor::enable_pin, OUTPUT);
}

void StepperMotor::enable()
{
    digitalWrite(StepperMotor::enable_pin, LOW);
}

void StepperMotor::disable()
{
    digitalWrite(StepperMotor::enable_pin, HIGH);
}