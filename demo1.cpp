//
//  demo1_main.cpp
//  
//
//  Created by yue on 12/02/2019.
//

#include "mbed.h"
#include "motors.h"
#include "demo1.h"

#define ROUTINE_DELAYSx 2

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

	case 1:  // pwm test	
	motors->set_target_speed(9000, 9000);        //stops BB

	motors->move_constant_speed(0.1, 0.1);
    wait(2.0);
    motors->move_constant_speed(0.3, 0.3);	
	wait(2.0);
    motors->move_constant_speed(0.5, 0.5);	
	wait(2.0);
    motors->move_constant_speed(0.7, 0.7);	
	wait(2.0);
    motors->move_constant_speed(0.9,0.9 );
    wait(2.0);
    motors->move_constant_speed(0, 0);

    state = 0;

	break;
	
	case 2:  // routine 

    wait (4.0);
    //start tracking the square routine

    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);

    while(motors->busy()){
        wait(0.1);
    }
    
#if defined(ROUTINE_DELAYS)
    wait(ROUTINE_DELAYS);
#endif

    for (int i = 0; i < 3; i++)
     {
        motors->turn(90, 0.25);

        while(motors->busy()){
            wait(0.1);
        }

#if defined(ROUTINE_DELAYS)
        wait(ROUTINE_DELAYS);
#endif
        motors->move_distance_R(500, 0.3);
        motors->move_distance_L(500, 0.3);

        while(motors->busy()){
            wait(0.1);
        }

#if defined(ROUTINE_DELAYS)
        wait(ROUTINE_DELAYS);
#endif
     } 

    motors->turn(180, 0.3);

    while(motors->busy()){
        wait(0.1);
    }

#if defined(ROUTINE_DELAYS)
    wait(ROUTINE_DELAYS);
#endif

    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);
    
    while(motors->busy()){
        wait(0.1);
    }

    #if defined(ROUTINE_DELAYS)
    wait(ROUTINE_DELAYS);
#endif
    
     for (int i = 0; i < 3; i++)
     {
        motors->turn(-90, 0.25);

    while(motors->busy()){
        wait(0.1);
    }
#if defined(ROUTINE_DELAYS)
        wait(ROUTINE_DELAYS);
#endif
        motors->move_distance_R(500, 0.3);
        motors->move_distance_L(500, 0.3);
        
        while(motors->busy()){
            wait(0.1);
        }
#if defined(ROUTINE_DELAYS)
        wait(ROUTINE_DELAYS);
#endif
     } 

   
    state = 0;

	break;
	}
}