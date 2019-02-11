//this is the header file for the motors class
#include "mbed.h"
#include "pins.h"

#define BB_COEF 0.01;   //bang bang coeddiciant

#define X1              //select encoder precision

#ifdef X1
#define PPR 64.0                   //this is the number of pulses per revolution in X1
#endif

#ifdef X2
#define PPR 128.0                   //this is the number of pulses per revolution in X2
#endif

#ifdef X4
#define PPR 256.0                   //this is the number of pulses per revolution in X4
#endif

#define WHEEL_DIA 0.01              //in meters to calculate speed in m/s
#define PI 3.14159265359            //this is pi
#define CHECK_SPEED_INTERVAL 100    //this is the update interval for the speed measurement ISR

class Motor{

    private:

    //the following pointers are used to interface with mbed libraries
    PwmOut *motor_R, *motor_L;
    DigitalOut *motor_EN, *dir_R, *dir_L;
    InterruptIn *encoder_RA, *encoder_RB, *encoder_LA, *encoder_LB;

    //this in and ISR to measure speed
    Ticker *check_speed;

    //store the encoder counts and directions for encoders 
    int encoder_count_R, encoder_count_L;
    bool direction_R, direction_L;
    
    //store the current speeds and the target speeds
    double speed_L, speed_R, target_speed_L, target_speed_R;

    //store distance traveled
    long distance_L, distance_R;
    
    
    public:    
    
    Motor(void);            //initialisation function
    
    void speed_ISR();      //speed calculation ISR
    void set_speed_R(double speed);
    void set_speed_L(double speed);
    void update_speed_PID();    //PID
    void update_speed_BB();     //bang bang
    void set_target_speed(double new_target_speed_R, double new_target_speed_L);    //set the target speed for the PID
    void move_constant_speed(double new_speed_R, double new_speed_L);     //move with no encoder feedback, arguments 0-1.0 sign gives direction
   
    //interrupt handlers for up to X4 encoder readout

    void encoder_rise_handler_RA();
    void encoder_rise_handler_RB();
    void encoder_fall_handler_RA();
    void encoder_fall_handler_RB();

    void encoder_rise_handler_LA();
    void encoder_rise_handler_LB();
    void encoder_fall_handler_LA();
    void encoder_fall_handler_LB();


};