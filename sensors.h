// this is the header file for the sensors class
#include "mbed.h"
#include "pins.h"

#define PROPAGTION_DELAY 0.005

//this class represents and individual sensor
class Sensor{
    private:
    double raw, noise;
    AnalogIn *pin;

    public:
    double value;
    
    Sensor(PinName p);          //initialization function requires an analog input pin
    
    double read_value(void);    //returns a noise compensated reading
    void read_noise(void);      //sets the noise level
};

//thhis class represents the line following sensors at the front of the buggy
class ScanLine{

    private:
    Sensor *scan_line[6];               //array containing the scan line sensors at the front of the buggy
    DigitalOut *clock, *data, *enable;  //control pins for the shift register sensor LEDs
    int current_sensor, n_sensor;       //variables to keep track of the curent sensor LED and the number of sensors in the scan line

    void next(void);    //simple function for incrementing the clock of the shift register

    public:

    double values[6];    //array storing the values of the sensors in the scan line

    ScanLine(void);     //initialization function

    void scan(void);    //scan the whole line of sensors 

    double array_to_value_V1(void);    //convert the array to one directional value between -1 and 1 using interpolation
    double array_to_value_V2(void);    //convert the array to one directional value between -1 and 1 using weighted average
    
};