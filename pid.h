#include "mbed.h"
#include "motors.h"
#include "sensors.h"

#define base_speed_r  0.2
#define base_speed_l  0.2
//pid basic variables
#define DT 0.1
#define MAX 0.8
#define MIN -1.2
#define KP 10
#define KD 10
#define KI 10

class PID{
private:
  
	//class pointers
	ScanLine *sensors;
	Motor *motors;
	//ticker objects
	//Ticker Ticker_PID;
  
public:

	//constructor
	PID(ScanLine *sensors_ptr, Motor *motors_ptr);

	//pid output 
	void PID_update(void);  
  
};
