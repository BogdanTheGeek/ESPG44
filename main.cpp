#include "pid.h"
#include "motors.h"
#include "mbed.h"
#include "sensors.h"

int main(void)
{
	Motor *pp_motors = new Motor();
	ScanLine *pp_Scanline = new ScanLine();
	pid *pp_pid = new pid(parameters); //void pid::pid (double dt, double max, double min, double kp, double kd, double ki)

	switch (pp_pid->WORKING_STATE)
	{
		//*****Stop mode
	case pp_pid->Stop:
		pp_motors->set_speed_R(0);
		pp_motors->set_speed_L(0);
		//if bluetooth signal = 1, change to Follow mode --- else stay in Stop mode
		if ()
		{
			pp_pid->WORKING_STATE = pp_pid->Follow;
		}
		else
		{
		}

		//******Follow mode
	case pp_pid->Follow:
		// apply pid control
		pp_pid->keep_calculate_PID(); //  repeat calculating pid and apply into pwm

		//if bluetooth signal = 1, change to Stop mode
		if ()
		{
			pp_pid->WORKING_STATE = pp_pid->Stop;
		}
		//when bluetooth signal = 0, check if sensors on the line
		// #1 no output via front sensors , also , front middle sensor no output--> wait for seconds to check the back sensor
		else if (pp_Scanline->array_to_value_V2() == 0 && pp_ScanLine->value[3] == 0)
		{
			wait(0.5); // wait 0.5s, if (front mid sensor & back sensor) no signal, stop wheels, and change to turning mode
			if (pp_Scanline->value[0] == 0 && pp_ScanLine->value[3] == 0)
			{
				pp_motors->set_speed_R(0);
				pp_motors->set_speed_L(0);

				pp_pid->WORKING_STATE = pp_pid->Turning;
			}
			//#2   else stay in Follow mode
			else
			{
				pp_pid->WORKING_STATE = pp_pid->Follow;
			}
		}

		//******Turning mode
	case pp_pid->Turning:
		pp_motors->turn(180, 0.2);

		pp_pid->WORKING_STATE = pp_pid->Follow;

		//check bluetooth signal to stop or not

		if ()
		{
			pp_pid->WORKING_STATE = pp_pid->Stop;
		}
	}
}