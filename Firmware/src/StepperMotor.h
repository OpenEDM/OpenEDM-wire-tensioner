#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

typedef enum {
    COUNTER = 0,
    CLOCKWISE
} direction_t;

typedef enum {
    X_AXIS = 0,
    Y_AXIS,
    Z_AXIS
} axis_t;

class StepperMotor {
public:
    StepperMotor(int stp_pin, int dir_pin);

    void init();
    void set_dir(direction_t dir);
    void step();
    void steps(int number_of_steps, int delay_ms);

    static void init_enable(int enable_pin);
    static void enable();
    static void disable();
    
private:
    direction_t dir = COUNTER;
    int stp_pin;
    int dir_pin;
    static int enable_pin;
};

#endif /* STEPPER_MOTOR_H */
