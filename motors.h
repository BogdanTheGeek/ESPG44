//this is the header file for the motors class

#include "mbed.h"
#include "pins.h"


#define BB_COEF 0.02;   //bang bang coeddiciant

#define SERIAL_DEBUGx

#define X1              //select encoder precision

#ifdef X1
#define PPR 256.0                  //this is the number of pulses per revolution in X1
#endif

#ifdef X2
#define PPR 512.0                   //this is the number of pulses per revolution in X2
#endif

#ifdef X4
#define PPR 1024.0                   //this is the number of pulses per revolution in X4
#endif

#define WHEEL_DIA 80             //wheel diameter in mm
#define WHEEL_AXEL_LENGTH 166    //distance between centers of the wheels in mm
#define PI 3.14159265359            //this is pi
#define CHECK_SPEED_INTERVAL 0.04    //this is the update interval for the speed measurement ISR
#define CHECK_DISTANCE_INTERVAL 0.02    //this is the update interval for the speed measurement ISR


class Motor{

    private:

    //the following pointers are used to interface with mbed libraries
    PwmOut *motor_R, *motor_L;
    DigitalOut *motor_EN, *dir_R, *dir_L;
    InterruptIn *encoder_RA, *encoder_RB, *encoder_LA, *encoder_LB;

    //this in and ISR to measure speed
    Ticker check_speed;

    //these are timeout interupts and end distance values used to move the buggy a set distance
    Timeout check_reached_distance_R, check_reached_distance_L;
    long end_distance_R, end_distance_L;
    bool direction_R, direction_L;

    //store the encoder counts and directions for encoders 
    int encoder_count_R, encoder_count_L;
    
    //store the current speeds and the target speeds
    double speed_L, speed_R, target_speed_L, target_speed_R;

    //Serial object pointer used for debuging
    

    //indicates that a command is already running
    bool busy_L, busy_R;

    public:  

    //store distance traveled
    long distance_L, distance_R;  
    Serial *serial;
    
    Motor(void);            //initialisation function
    
    void speed_ISR();      //speed calculation ISR
    void set_speed_R(double speed);
    void set_speed_L(double speed);
    void update_speed_PID();    //PID
    void update_speed_BB();     //bang bang
    void set_target_speed(double new_target_speed_L, double new_target_speed_R);    //set the target speed for the PID
    void move_constant_speed(double new_speed_L, double new_speed_R);     //move with no encoder feedback, arguments 0-1.0 sign gives direction
    void move_distance_R(long distance, double speed);            //move the right wheel a certain distance
    void move_distance_L(long distance, double speed);            //move the left wheel a certain distance
    void check_distance_R();
    void check_distance_L();

    void turn(double degrees, double speed);                        //turn the buggy a number of degrees (positive is right, negative is left)


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
