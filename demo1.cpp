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

	case 1:  // pwm test	
	
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
    
    //set 'travel distance' and 'wheel speed (Right and Left)' ,
    //distance in mm, speed scale 0-1
    
    //*****  clockwise  *****
    //step 1    O==O ---------> 500mm,  ^stop!^
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);
   
    //step 2    O==O ------>     //turning right for 90 degrees, with a speed of 0.3 (scale 0-1)
    //                      |
    //                      |
    //                      |
    //                      |
    //                      v
    //                      O
    //                      ||
    //                      O
    motors->turn(90.0, 0.3);
     
    //step 3    O==O ---------> 500mm,  ^stop!^
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);  
    
    //step 4    O==O ------>     //turning right for 1 degrees, with a speed of 0.3 (scale 0-1)
    //                      |
    //                      |
    //                      |
    //                      |
    //                      v
    //                      O
    //                      ||
    //                      O
    motors->turn(90.0, 0.3);   
    
    //step 5    O==O ---------> 500mm,  ^stop!^
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);   
    
    //step 6    O==O ------>     //turning right for 90 degrees, with a speed of 0.3 (scale 0-1)
    //                      |
    //                      |
    //                      |
    //                      |
    //                      v
    //                      O
    //                      ||
    //                      O
    motors->turn(90.0, 0.3);   
    
    //step 7    O==O ---------> 500mm,  ^stop!^  ,  now the buggy is back to the origin
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);   
    
    //******   anti-clockwise   ******
    //step 1   turning around at the origin
    motors->turn(90.0, 0.3);    
    
    //step 2   O==O ---------> 500mm,  ^stop!^
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);    
    
    //step 3    O==O <------^    //turning left for 90 degrees, with a speed of 0.3 (scale 0-1)
    //                      |
    //                      |
    //                      |
    //                      |
    //
    //                      O
    //                      ||
    //                      O
    motors->turn(-90.0, 0.3);    
    
    //step 4   O==O ---------> 500mm,  ^stop!^
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);   
    
    //step 5    O==O <------^    //turning left for 90 degrees, with a speed of 0.3 (scale 0-1)
    //                      |
    //                      |
    //                      |
    //                      |
    //
    //                      O
    //                      ||
    //                      O
    motors->turn(-90.0, 0.3);  
    
    //step 6   O==O ---------> 500mm,  ^stop!^
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);   
    
    //step 7    O==O <------^    //turning left for 90 degrees, with a speed of 0.3 (scale 0-1)
    //                      |
    //                      |
    //                      |
    //                      |
    //
    //                      O
    //                      ||
    //                      O
    motors->turn(-90.0, 0.3);    
    
    //step 6   O==O ---------> 500mm,  ^stop!^
    motors->move_distance_R(500, 0.3);
    motors->move_distance_L(500, 0.3);  
	
    state = 0;

	break;
	}
}