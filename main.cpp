#include "pid.h"

enum working_state {
	Stop,
	Follow,
	Turning
};

int main(void)
{
	enum working_state WORKING_STATE = Stop;
	
	Motor *motors = new Motor();
	ScanLine *sensors = new ScanLine();
	PID *pid = new PID(sensors, motors); 

	while(1)switch (WORKING_STATE){

			//*****Stop mode
		case Stop:
			motors->set_speed_R(0);
			motors->set_speed_L(0);
			while (motors->busy_or_not() == true) {wait(0.1);}
			//if bluetooth signal = 1, change to Follow mode --- else stay in Stop mode
			if (false){

				WORKING_STATE = Follow;
			}
			else{

			}
		break;
	
			//******Follow mode
		case Follow:
			// apply pid control
			pid->PID_update(); //  repeat calculating pid and apply into pwm

			//if bluetooth signal = 1, change to Stop mode
			if (false){
				WORKING_STATE = Stop;
			}
			//when bluetooth signal = 0, check if sensors on the line
			// #1 no output via front sensors , also , front middle sensor no output--> wait for seconds to check the back sensor
			else if (sensors->array_to_value_V2() == 0 && sensors->values[3] == 0){

				wait(0.5); // wait 0.5s, if (front mid sensor & back sensor) no signal, stop wheels, and change to turning mode
				if (sensors->values[0] == 0 && sensors->values[3] == 0){

					motors->set_speed_R(0);
					motors->set_speed_L(0);
					while (motors->busy_or_not() == true){wait(0.1);}

					WORKING_STATE = Turning;
				}
				//#2   else stay in Follow mode
				else{

					WORKING_STATE = Follow;
				}
			}
		break;

			//******Turning mode
		case Turning:
			motors->turn(180, 0.2);
			while (motors->busy_or_not() == true) {wait(0.1);}
			
			
			WORKING_STATE = Follow;

			//check bluetooth signal to stop or not
			if (false){
				WORKING_STATE = Stop;
			}
		break;
	}
}