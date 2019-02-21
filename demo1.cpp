//
//  demo1_main.cpp
//  
//
//  Created by yue on 12/02/2019.
//

#include "mbed.h"
#include "motors.h"
#include "demo1.h"

InterruptIn button(PC_2, PullUp);

int state = 0;

void increment_state() {
	if(state >= 2){
		state = 0;
	}
	else{
		state++;
	}
    wait(0.5);      //debounce
}



void demo1(void){
	
    //create object
    Motor *motors = new Motor();
	
	button.rise(&increment_state);
    
    //wait for 6 seconds to prepare
    //wait (3.0);
	
	
	while(1)switch (state){

    case 0:
        //pause
        wait(0.1);
    break;

	case 1:{  // pwm test	

        Serial serial(USBTX, USBRX, 9600);
        serial.printf("Serial initialised!\n\r");

    	motors->set_target_speed(9000, 9000);        //stops BB

    	motors->move_constant_speed(0.1, 0.1);
        for (int i = 0; i < 10; i++)
        {
            serial.printf("SpeedL: %.2f SpeedR: %.2f\n\r", motors->return_speed_L(), motors->return_speed_R());
            wait(0.2);
        }
        motors->move_constant_speed(0.3, 0.3);	
    	 for (int i = 0; i < 10; i++)
        {
            serial.printf("SpeedL: %.2f SpeedR: %.2f\n\r", motors->return_speed_L(), motors->return_speed_R());
            wait(0.2);
        }
        motors->move_constant_speed(0.5, 0.5);	
    	 for (int i = 0; i < 10; i++)
        {
            serial.printf("SpeedL: %.2f SpeedR: %.2f\n\r", motors->return_speed_L(), motors->return_speed_R());
            wait(0.2);
        }
        motors->move_constant_speed(0.7, 0.7);	
    	 for (int i = 0; i < 10; i++)
        {
            serial.printf("SpeedL: %.2f SpeedR: %.2f\n\r", motors->return_speed_L(), motors->return_speed_R());
            wait(0.2);
        }
        motors->move_constant_speed(0.9,0.9 );
         for (int i = 0; i < 10; i++)
        {
            serial.printf("SpeedL: %.2f SpeedR: %.2f\n\r", motors->return_speed_L(), motors->return_speed_R());
            wait(0.2);
        }
        motors->move_constant_speed(0, 0);

        state = 0;

        delete serial;
    }

	break;
	
	case 2:  // routine 

    wait (4.0);
    //start tracking the square routine

    motors->move_distance_R(480, 0.3);
    motors->move_distance_L(480, 0.3);

    while(motors->busy()){
        wait(0.1);
    }

    for (int i = 0; i < 3; i++){

        motors->turn(90, 0.3);

        while(motors->busy()){
            wait(0.1);
        }

        motors->move_distance_R(480, 0.3);
        motors->move_distance_L(480, 0.3);

        while(motors->busy()){
            wait(0.1);
        }
    }

    motors->turn(180, 0.3);

    while(motors->busy()){
        wait(0.1);
    }

    motors->move_distance_R(480, 0.3);
    motors->move_distance_L(480, 0.3);
    
    while(motors->busy()){
        wait(0.1);
    }


    for (int i = 0; i < 3; i++){

        motors->turn(-90, 0.3);

        while(motors->busy()){
            wait(0.1);
        }
        motors->move_distance_R(480, 0.3);
        motors->move_distance_L(480, 0.3);
        
        while(motors->busy()){
            wait(0.1);
        }
    } 
 
    state = 0;

	break;
    }
	
}