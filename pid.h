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

class pid {
private:

  
//class pointers
  ScanLine *p_ScanLine;
  Motor  *p_Motor  ;
//ticker objects
  Ticker Ticker_pid;
  
public:
      
   

  //constructor
  pid (void);

  //pid output 
  void pid_calculation(void);
  void keep_calculate_PID(void);
  
  
};
