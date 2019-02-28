#include "pid.h"
#include "motors.h"
#include "mbed.h"
#include "sensors.h"

enum working_state {
	Stop,
	Follow,
	Turning
};
enum working_state WORKING_STATE = Stop;

int main(void)
{
	Motor *pp_motors = new Motor();
	ScanLine *pp_ScanLine = new ScanLine();
	pid *pp_pid = new pid(); //void pid::pid (double dt, double max, double min, double kp, double kd, double ki)

	switch (WORKING_STATE)
	{
		//*****Stop mode
	case Stop:
		pp_motors->set_speed_R(0);
		pp_motors->set_speed_L(0);
		while (pp_motors->busy_or_not()== true) {}
			//if bluetooth signal = 1, change to Follow mode --- else stay in Stop mode
			if ()
			{
				WORKING_STATE = Follow;
			}
			else
			{
			}

		//******Follow mode
	case Follow:
		// apply pid control
		pp_pid->keep_calculate_PID(); //  repeat calculating pid and apply into pwm

		//if bluetooth signal = 1, change to Stop mode
		if ()
		{
			WORKING_STATE = Stop;
		}
		//when bluetooth signal = 0, check if sensors on the line
		// #1 no output via front sensors , also , front middle sensor no output--> wait for seconds to check the back sensor
		else if (pp_ScanLine->array_to_value_V2() == 0 && pp_ScanLine->values[3] == 0)
		{
			wait(0.5); // wait 0.5s, if (front mid sensor & back sensor) no signal, stop wheels, and change to turning mode
			if (pp_ScanLine->values[0] == 0 && pp_ScanLine->values[3] == 0)
			{
				pp_motors->set_speed_R(0);
				pp_motors->set_speed_L(0);

				WORKING_STATE = Turning;
			}
			//#2   else stay in Follow mode
			else
			{
				WORKING_STATE = Follow;
			}
		}

		//******Turning mode
	case Turning:
		pp_motors->turn(180, 0.2);
		while (pp_motors->busy_or_not() == true) {}
		
		
		WORKING_STATE = Follow;

		//check bluetooth signal to stop or not

		if ()
		{
			WORKING_STATE = Stop;
		}
	}
}