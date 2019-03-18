#include "motors.h"
#include "sensors.h"

#define DEFLECTION_COEFF 	70
#define BASE_SPEED 			70

#define ON_key "44GO"
#define OFF_key "44STOP"
#define TURN_key "44TURN"


void bluetooth_handler(void);
Serial hm10(BLETX, BLERX, 9600);

enum working_state {
	Stop,
	Follow,
	Turning
};

enum working_state WORKING_STATE = Stop;

double deflection_to_speed_diff(double deflection);

void run() {
	
	WORKING_STATE = Follow;
    wait(3);      //debounce
}

int main(void)
{
	Motor *motors = new Motor();
	ScanLine *sensors = new ScanLine();

	hm10.attach(&bluetooth_handler, Serial::RxIrq);

	InterruptIn button(BUTTON, PullUp);
	button.rise(&run);

	bool go = true;

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
		if(/*sensors->on_line() ==*/ true){

			if(go == true){
				motors->target_speed_R = BASE_SPEED;
				motors->target_speed_L = BASE_SPEED; 
				go = false;
				wait(0.1);
			}

			sensors->scan();

			double speed_diff;
			speed_diff = deflection_to_speed_diff(sensors->array_to_value_V1());

			motors->target_speed_R = BASE_SPEED - speed_diff;
			motors->target_speed_L = BASE_SPEED + speed_diff; 
		}else{
			//check if it is a gap or the end of the line
			wait(0.2);
			if(sensors->on_line() == false){
				WORKING_STATE = Stop;
			}
			//optional rear sensor check
			//
		}

		break;

//Turning mode
	case Turning:

		motors->turn(180, 0.2);
		while (motors->busy() == true) {wait(0.1);}
		
		WORKING_STATE = Follow;

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
	while(i < 5){
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