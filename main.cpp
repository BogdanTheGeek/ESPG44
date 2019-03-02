#include "motors.h"
#include "sensors.h"

#define DEFLECTION_COEFF 100

enum working_state {
	Stop,
	Follow,
	Turning
};

double deflection_to_speed_diff(double deflection);

int main(void)
{
	enum working_state WORKING_STATE = Stop;
	
	Motor *motors = new Motor();
	ScanLine *sensors = new ScanLine();

	while(1)switch (WORKING_STATE){

			//*****Stop mode
	case Stop:
		motors->target_speed_L = 0;
		motors->target_speed_L = 0;

		while (motors->busy() == true) {wait(0.1);}

		break;

		//******Follow mode
	case Follow:
		if(sensors->on_line() == true){
			sensors->scan();

			double speed_diff;
			speed_diff = deflection_to_speed_diff(sensors->array_to_value_V2());

			motors->target_speed_R -= speed_diff;
			motors->target_speed_L += speed_diff; 
		}else{
			//check if it is a gap or the end of the line
			wait(0.2);
			if(sensors->on_line() == false){
				WORKING_STATE = Turning;
				break;
			}
			//optional rear sensor check
			//
		}

		break;

		//******Turning mode
	case Turning:

		motors->turn(180, 0.2);
		while (motors->busy() == true) {wait(0.1);}
		
		WORKING_STATE = Follow;

		break;
	}
}

double deflection_to_speed_diff(double deflection){

	double speed_diff = deflection * DEFLECTION_COEFF;

	return	speed_diff;
}