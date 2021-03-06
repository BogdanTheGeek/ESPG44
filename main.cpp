#include "motors.h"
#include "sensors.h"

#define DEFLECTION_COEFF 	200
#define BASE_SPEED 			335
#define LINE_GAP 			20

#define ON_key	 "44GO44"
#define OFF_key  "44STOP"
#define TURN_key "44TURN"


void bluetooth_handler(void);
Serial hm10(BLETX, BLERX, 9600);

void start(void);
InterruptIn button(BUTTON, PullUp);

Motor *motors = new Motor();
ScanLine *sensors = new ScanLine();

enum working_state {
	Stop,
	Follow,
	Turning
};

enum working_state WORKING_STATE = Stop;

double last_array_value;

double deflection_to_speed_diff(double deflection);

bool turn = false;

int main(void)
{
	hm10.attach(&bluetooth_handler, Serial::RxIrq);
	button.rise(&start);

	while(1)switch (WORKING_STATE){

//Stop mode
	case Stop:
		//hm10.printf("stop");
		motors->distance_L = 0;

		motors->target_speed_L = 0;
		motors->target_speed_R = 0;
		motors->set_speed_L(0);
		motors->set_speed_R(0);

		if (turn == true){
			turn = false;
			wait(0.3);
			motors->turn(100, 0.3);
			wait(0.4);
			WORKING_STATE = Turning;
		}

		break;

//Follow mode
	case Follow:

		sensors->scan();

		double speed_diff, array_value;

		array_value = sensors->array_to_value_V2();

		static long gap_start_dist = 0;

		if(sensors->on_line() == true){
			gap_start_dist = motors->distance_L;
			last_array_value = array_value;
		}else{
			if(motors->distance_L > (gap_start_dist + LINE_GAP)){
				
				if (last_array_value < 0.3 && last_array_value > -0.3){
					WORKING_STATE = Stop;
				}
			}

			if (last_array_value >= 0){
				array_value = 1.0;
			}else{
				array_value = -1.0;
			}
			
		}

		speed_diff = deflection_to_speed_diff(array_value);
		
		double START_SPEED;

		if (motors->distance_L < 80){
			START_SPEED = BASE_SPEED*0.25;
		}else if (motors->distance_L < 160){
			START_SPEED = BASE_SPEED*0.6;
		}else{
			START_SPEED = BASE_SPEED;
		}

		motors->target_speed_R = START_SPEED - speed_diff;
		motors->target_speed_L = START_SPEED + speed_diff;

	break;

//Turning mode
	case Turning:
	//hm10.printf("turn");
		sensors->scan();
		if(sensors->on_line() == false){
			motors->turn(1, 0.35);
			while (motors->busy() == true) {wait(0.0001);}

		}else{
			motors->distance_L = 0;
			WORKING_STATE = Follow;
			//hm10.printf("follow");
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
		WORKING_STATE = Stop;
		turn = true;
		return;
	}

	return;
}

void start(void){

	WORKING_STATE = Follow;
}
