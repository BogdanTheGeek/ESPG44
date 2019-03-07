//this file should contain all the pin definitions and connections of the buggy

//ENCODER
#define ENCODER_LA  PC_10
#define ENCODER_LB  PC_12
#define ENCODER_RB  PA_13
#define ENCODER_RA  PA_14

//MOTOR
#define PWM_L       PA_9
#define PWM_R       PA_8
#define DIR_L       PA_12
#define DIR_R       PB_12
#define MOTOR_EN    PB_2
#define MOTOR_1WIRE PA_11

//SENSORS INPUTS
#define SENSOR_0    PA_0
#define SENSOR_1    PA_1
#define SENSOR_2    PA_4
#define SENSOR_3    PB_0
#define SENSOR_4    PC_1
#define SENSOR_TAIL PC_0

//SENSORS OUTPUTS
#define SEN_CLOCK   PC_14
#define SEN_DATA    PC_15
#define SEN_EN		PC_3

//POWER CONTROL
#define BUTTON		PC_2
