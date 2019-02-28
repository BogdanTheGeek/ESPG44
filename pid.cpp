#include "pid.h"


PID::PID(ScanLine *sensors_ptr, Motor *motors_ptr){

  //initialise pointers
  sensors = sensors_ptr;
  motors = motors_ptr;

  //Ticker_PID.attach(callback(this, &PID::PID_update), 0.1);
 
}


void PID::PID_update(){      //calculate and return PID value

  double left_speed, right_speed;

  //initialise target value of sensors
  static double set_point = 0.0 ;
  static double LAST_ERROR = 0.0;
  static double I_VALUE = 0.0;
  //error term
  double error = set_point + sensors->array_to_value_V2();

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
  if (PID_out > MAX){
    PID_out = MAX;
  }
  else if(PID_out < MIN){
    PID_out = MIN;
  }

  //store error for next time use
  LAST_ERROR = error ;

  //reture PID value , apply into pwm speed
  left_speed = base_speed_l + PID_out;  
  right_speed = base_speed_r - PID_out;

  motors->set_target_speed(left_speed, right_speed);

}
