
#include "pid.h"



void pid::pid ()

{
  //initialise pointers
  Scanline *p_ScanLine = new ScanLine(void);
  Motor *p_Motor = new Motor(void);
  
  p_Motor->keep_calculate_PID();    //ticker function , keep calculating PID
 
}



void pid::pid_calculation ()      //calculate and return PID value
{
  //initialise target value of sensors
  static double set_point = 0.0 ;
  static double LAST_ERROR = 0.0;
  static double I_VALUE = 0.0;
  //error term
  double error = set_point + (p_ScanLine->array_to_value_V2(void));

  //Proportional term
  double P_out = KP * error ;

//Derivative term
  double derivative = (error - LAST_ERROR)/ DT ;
  double D_out = KD * derivative ;

//Integral term
  I_VALUE += error * DT ;
  double I_out = KI * I_VALUE ;

//final pid output
double PID_out = P_out + D_out + I_out ;

//add limitation of range
if (PID_out > MAX) {PID_out = MAX ;}
else if (PID_out < MIN) {PID_out = MIN ;}

//store error for next time use
LAST_ERROR = error ;

//reture PID value , apply into pwm speed
double left_speed = base_speed_l + p_pid->pid_calculation();
double right_speed = base_speed_r - p_pid->pid_calculation();

p_Motor->set_target_speed(left_speed, right_speed);
}

//PID  Ticker keep calculating 
void pid::keep_calculate_PID (void)
{
  Ticker_pid.attach (callback(this,&pid::pid_calculation), 0.2) ;
}


//checking and change working state with responding to bluetooth





