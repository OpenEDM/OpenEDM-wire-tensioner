#include <Arduino.h>
#include <Debounce.h>
#include <HX711.h>
#include <QuickPID.h>
#include "StepperMotor.h"

/***************************************/
/*************** BUTTONS ***************/
/***************************************/

constexpr size_t NUMBER_OF_BUTTONS  = 4;

constexpr int BUTTON_0              = 0;
constexpr int BUTTON_1              = 1;
constexpr int BUTTON_2              = 2;
constexpr int BUTTON_3              = 3;

constexpr int DEBOUNCE_DELAY_MS     = 30;

// Debounce buttons[] = {
//     Debounce(9, DEBOUNCE_DELAY_MS, true),
//     Debounce(10, DEBOUNCE_DELAY_MS, true),
//     Debounce(11, DEBOUNCE_DELAY_MS, true),
//     Debounce(12, DEBOUNCE_DELAY_MS, true),
// };

byte button_state[]                 = {0, 0, 0, 0};
byte button_state_previous[]        = {0, 0, 0, 0};

/***************************************/
/************** LOAD CELL **************/
/***************************************/

constexpr byte LOADCELL_DOUT_PIN        = D2;
constexpr byte LOADCELL_SCK_PIN         = D5;

HX711 loadcell;

constexpr long MIN_TENSION_G            = 500;
long actual_tension_g                   = 0;

constexpr long LOADCELL_SAMPLE_TIME_MS  = 10;

/***************************************/
/*************** MOTORS ****************/
/***************************************/

constexpr int PULLEY_RADIUS_MM      = 10;
constexpr int FULL_STEPS_PER_REV    = 200;
constexpr int MICROSTEPPING         = 16;

constexpr int STEPS_PER_REV         =  FULL_STEPS_PER_REV * MICROSTEPPING;
constexpr float STEPS_PER_MM        = FULL_STEPS_PER_REV * MICROSTEPPING / (2 * PI * PULLEY_RADIUS_MM);

constexpr direction_t DEFAULT_DIR   = COUNTER;

// constexpr int X_DIR_PIN             = D2;
constexpr int Y_DIR_PIN             = D3;
constexpr int Z_DIR_PIN             = D4;
// constexpr int X_STP_PIN             = D5;
constexpr int Y_STP_PIN             = D6;
constexpr int Z_STP_PIN             = A2; // D7

constexpr int SHIELD_EN_PIN         = D9; // D8

constexpr int MS1_PIN               = D12;
constexpr int MS2_PIN               = D11;

StepperMotor input_motor(Z_STP_PIN, Z_DIR_PIN);
StepperMotor output_motor(Y_STP_PIN, Y_DIR_PIN);

long output_motor_step_period_us;
long input_motor_step_period_us;

long output_motor_last_step_timestamp_us    = 0;
long input_motor_last_step_timestamp_us     = 0;

/***************************************/
/***************** PID *****************/
/***************************************/

constexpr int PID_LIMIT = 3000;

float pid_setpoint, pid_input, pid_output;

float pid_kp = 5.0f;
float pid_ki = 2.0f;
float pid_kd = 0.1f;

bool compute_ok         = false;

QuickPID PID(
    &pid_input, &pid_output, &pid_setpoint, 
    pid_kp, pid_ki, pid_kd, 
    QuickPID::pMode::pOnError, QuickPID::dMode::dOnError, 
    QuickPID::iAwMode::iAwClamp, 
    QuickPID::Action::reverse
);

/***************************************/
/*********** MAIN PARAMETERS ***********/
/***************************************/

int speed_setpoint_mmpersec = 2;
long tension_setpoint_g     = 2000;

/***************************************/
/***************************************/
/***************************************/

void calculate_motors_speed(int speed_setpoint_mmpersec) {
    int base_step_period_us = 1000000.0 / (STEPS_PER_MM * speed_setpoint_mmpersec);

    output_motor_step_period_us = base_step_period_us;
    input_motor_step_period_us = base_step_period_us;
}

/*
void process_buttons() {
    for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) {
        button_state_previous[i] = button_state[i];
    }

    for (int i = 0; i < NUMBER_OF_BUTTONS; ++i) {
        button_state[i] = buttons[i].read();
    }

    if (button_state_previous[BUTTON_3] == LOW && button_state[BUTTON_3] == HIGH) {
        enable = !enable;
    }

    fast_wire_feeding = (button_state[BUTTON_0] == HIGH);
}
*/

// https://www.aliexpress.com/item/1005003288007057.html
void set_microstepping(int microstepping) {
    pinMode(MS1_PIN, OUTPUT);
    pinMode(MS2_PIN, OUTPUT);
    
    switch (microstepping)
    {
    case 8:
        digitalWrite(MS1_PIN, LOW);
        digitalWrite(MS2_PIN, LOW);
        break;
    case 16:
        digitalWrite(MS1_PIN, HIGH);
        digitalWrite(MS2_PIN, HIGH);
        break;
    case 32:
        digitalWrite(MS1_PIN, HIGH);
        digitalWrite(MS2_PIN, LOW);
        break;
    case 64:
        digitalWrite(MS1_PIN, LOW);
        digitalWrite(MS2_PIN, HIGH);
        break;
    
    default:
        for (;;){}
        break;
    }
}

long get_tension() {
    return loadcell.get_value() * 2750 / 618858;
}

void startup() {
    constexpr int step_delay_ms = 10;
    constexpr int backwards_steps = 30;
    constexpr int taring_times = 100;

    long tension = get_tension();
    long tension_prev = tension;

    // turn the output motor backwards until the tension is no longer changing
    output_motor.set_dir(DEFAULT_DIR == COUNTER ? CLOCKWISE : COUNTER);

    do {
        tension_prev = tension;
        output_motor.steps(backwards_steps, step_delay_ms);
        tension = get_tension();
    } while (abs(tension - tension_prev) > 0);

    // tare the loadcell
    loadcell.tare(taring_times);

    // turn the output motor forwards until the tension is more than MIN_TENSION_G
    output_motor.set_dir(DEFAULT_DIR);
    
    do {
        output_motor.step();
        delay(step_delay_ms);
        tension = get_tension();
    } while (tension < MIN_TENSION_G);
}

void setup() {
    // startup delay
    delay(2000);

    Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);

    input_motor.init();
    output_motor.init();

    StepperMotor::init_enable(SHIELD_EN_PIN);
    StepperMotor::enable();

    set_microstepping(MICROSTEPPING);

    loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    // loadcell.tare();

    PID.SetSampleTimeUs(LOADCELL_SAMPLE_TIME_MS * 1000);
    PID.SetOutputLimits(-PID_LIMIT, PID_LIMIT);
    PID.SetMode(QuickPID::Control::timer);

    startup();
}

void loop() {
    // process_buttons();
    
    long now = micros();

    if (actual_tension_g > MIN_TENSION_G) {
        if (now - output_motor_last_step_timestamp_us > output_motor_step_period_us) {
            output_motor_last_step_timestamp_us = now;

            output_motor.set_dir(DEFAULT_DIR);
            output_motor.step();
        }

        if (now - input_motor_last_step_timestamp_us > input_motor_step_period_us) {
            input_motor_last_step_timestamp_us = now;

            input_motor.set_dir(DEFAULT_DIR);
            input_motor.step();
        }
    }

    if (loadcell.is_ready()) {
        actual_tension_g = get_tension();

        if (actual_tension_g > MIN_TENSION_G) {
            pid_setpoint = tension_setpoint_g;
            pid_input = actual_tension_g;

            PID.SetTunings(pid_kp, pid_ki, pid_kd);
            compute_ok = PID.Compute();

            calculate_motors_speed(speed_setpoint_mmpersec);

            output_motor_step_period_us += pid_output;
        }
    }
}
