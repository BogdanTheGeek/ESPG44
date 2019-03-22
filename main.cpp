#include "motors.h"
#include "sensors.h"

#define DEFLECTION_COEFF 	70
#define BASE_SPEED 			200
#define LINE_GAP 			20

#define ON_key	 "44GO44"
#define OFF_key  "44STOP"
#define TURN_key "44TURN"


void bluetooth_handler(void);
Serial hm10(BLETX, BLERX, 9600);

Motor *motors = new Motor();
ScanLine *sensors = new ScanLine();

enum working_state {
	Stop,
	Follow,
	Turning
};

enum working_state WORKING_STATE = Stop;

double deflection_to_speed_diff(double deflection);
void line_break_check(void);

int main(void)
{
	hm10.attach(&bluetooth_handler, Serial::RxIrq);

	float direction = 1;

	while(1)switch (WORKING_STATE){

//Stop mode
	case Stop:
		motors->target_speed_L = 0;
		motors->target_speed_R = 0;
		motors->set_speed_L(0);
		motors->set_speed_R(0);

		break;

//Follow mode
	case Follow:

		static long gap_start_dist = 0;

		if(sensors->on_line() == true){
			gap_start_dist = motors->distance_L;
		}else{
			if(motors->distance_L > gap_start_dist + LINE_GAP){
				WORKING_STATE = Turning;
			}

		}


		// if(go == true && motors->busy() == false){
		// 	motors->target_speed_R = BASE_SPEED/3;
		// 	motors->target_speed_L = BASE_SPEED/3; 
		// 	go = false;
		// 	wait(0.1);
		// }

		sensors->scan();

		double speed_diff;
		speed_diff = deflection_to_speed_diff(sensors->array_to_value_V1());

		if(speed_diff >= 0){
			direction = 1;
		}else{
			direction = -1;
		}

		motors->target_speed_R = BASE_SPEED - speed_diff;
		motors->target_speed_L = BASE_SPEED + speed_diff;

	break;

//Turning mode
	case Turning:
		sensors->scan();
		if(sensors->on_line() == false){
			motors->turn(direction, 0.25);
			while (motors->busy() == true) {wait(0.0001);}
		}else{

			WORKING_STATE = Follow;
		}
		
		break;
	}
}

double deflection_to_speed_diff(double deflection){

	double speed_diff ;
	speed_diff = deflection * DEFLECTION_COEFF;

	return	speed_diff;
}

void bluetooth_handler(void){
	char buffer[6];

	int i = 0;
	while(i < 6){
		buffer[i++] = hm10.getc();
	}

	if(strcmp(buffer, ON_key) == 0){ 
	    WORKING_STATE = Follow;
	   	return;
	}
	if(strcmp(buffer, OFF_key) == 0){ 
		WORKING_STATE = Stop;
		return;
	}
	if(strcmp(buffer, TURN_key) == 0){ 
		WORKING_STATE = Turning;
		return;
	}

	return;
}
