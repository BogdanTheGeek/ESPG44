#include "motors.h"



void Motor::speed_ISR(){
    const double speed_calc_optim = (60.0/PPR)/((double)CHECK_SPEED_INTERVAL);          //calculate speed in rpm
    const double distance_calc_optim = (1.0/PPR)*(PI*WHEEL_DIA);                        //calculate distance in mm
    
    speed_R = encoder_count_R * speed_calc_optim;
    speed_L = encoder_count_L * speed_calc_optim; 

    distance_R += encoder_count_R * distance_calc_optim;
    distance_L += encoder_count_L * distance_calc_optim;

#if defined(SERIAL_DEBUG)
    serial->printf("Rs:%.2f Ls:%.2f Rd=%d Ld=%d %d %d %d\n\r", speed_R, speed_L, distance_R, distance_L, busy_L, busy_R, turning);
#endif

    encoder_count_R = 0;
    encoder_count_L = 0;

    this->update_speed_BB();
    
}


void Motor::update_speed_PID(){
    
    //this is for PID
}

void Motor::update_speed_BB(){

    static double pwm_R, pwm_L;

    if(target_speed_R == 0){

        this->set_speed_R(0);
        pwm_R = 0;

    }else if(target_speed_R == 9000){
        target_speed_R = 9000;
    }else{
        if (speed_R < target_speed_R){
            pwm_R += BB_COEF;
            this->set_speed_R(pwm_R);
        }
        else if(speed_R > target_speed_R){
            pwm_R -= BB_COEF;
            this->set_speed_R(pwm_R);
        }
    }

    if(target_speed_L == 0){

        this->set_speed_L(0);
        pwm_L = 0;

    }else if(target_speed_L == 9000){
        target_speed_L = 9000;
    }else{
        if (speed_L < target_speed_L){
            pwm_L += BB_COEF;
            this->set_speed_L(pwm_L);
        }
        else if(speed_L > target_speed_L){
            pwm_L -= BB_COEF;
            this->set_speed_L(pwm_L);
        }
    }

}
void Motor::set_speed_R(double speed){

    if(speed >= 0){        //set the direction of the motors depending on the sign of the speed
        dir_R->write(1);
   		motor_R->write(1.0 - speed);
    }
    else if(speed < 0){
        dir_R->write(0);
    	motor_R->write(1.0 + speed);
    }
}
void Motor::set_speed_L(double speed){

    if(speed >= 0){        //set the direction of the motors depending on the sign of the speed
        dir_L->write(1);
    	motor_L->write(1.0 - speed);
    }
    else if(speed < 0){
        dir_L->write(0);
    	motor_L->write(1.0 + speed);
    }
}

void Motor::move_constant_speed(double new_speed_L, double new_speed_R){        //move with no encoder feedback, arguments 0-1.0 sign gives direction
    this->set_speed_R(new_speed_R);
    this->set_speed_L(new_speed_L);
}


void Motor::set_target_speed(double new_target_speed_L, double new_target_speed_R){
   
   target_speed_R = new_target_speed_R;
   target_speed_L = new_target_speed_L;
   
}

void Motor::move_distance_R(long distance, double speed){

    long start_distance = distance_R;
    end_distance_R = start_distance + distance;

    if(distance > 0){
        direction_R = true;
        //this->set_speed_R(speed);
        target_speed_R = speed * 300;
    }
    else {
        direction_R = false;
        //this->set_speed_R(-speed);
        target_speed_R = -speed * 300;
    }

    
    this->check_distance_R();

}

void Motor::move_distance_L(long distance, double speed){

    long start_distance = distance_L;
    end_distance_L = start_distance + distance;

    if(distance > 0){
        direction_L = true;
        //this->set_speed_L(speed);
        end_distance_L -= 20;
        target_speed_L = speed * 300;

    }
    else {
        direction_L = false;
        //this->set_speed_L(-speed);
        end_distance_L += 20;
        target_speed_L = -speed * 300;
    }

    
    this->check_distance_L();
    
}

void Motor::check_distance_R(){

    if(direction_R == true){
        if(distance_R < end_distance_R){
            busy_R = true;
            check_reached_distance_R.attach(callback(this, &Motor::check_distance_R), CHECK_DISTANCE_INTERVAL);    
        }else{
            busy_R = false;
            target_speed_R = 0; 
        }
    }else{
        if(distance_R > end_distance_R){
            busy_R = true;
            check_reached_distance_R.attach(callback(this, &Motor::check_distance_R), CHECK_DISTANCE_INTERVAL);
        }else{
            busy_R = false;
            target_speed_R = 0;
        }
    }
}
void Motor::check_distance_L(){

    if(direction_L == true){
        if(distance_L < end_distance_L){
            busy_L = true;
            check_reached_distance_L.attach(callback(this, &Motor::check_distance_L), CHECK_DISTANCE_INTERVAL);
        }else{
            busy_L = false;
            target_speed_L = 0;
            this->set_speed_L(0);
        }
    }else{
        if(distance_L > end_distance_L){
            busy_L = true;
            check_reached_distance_L.attach(callback(this, &Motor::check_distance_L), CHECK_DISTANCE_INTERVAL);          
        }else{
            busy_L = false;
            target_speed_L = 0;
            this->set_speed_L(0);
        }
    }
}

void Motor::turn(double degrees, double speed){

    long distance = (degrees/360.0)*(PI*WHEEL_AXEL_LENGTH);

    if (degrees >= 0){
        distance = distance - 10;
    }else{
        distance = distance + 10;
    }

    turning = true;

    this->move_distance_R(-distance, speed);
    this->move_distance_L(distance, speed);    
    
}

bool Motor::busy(void){
    if((busy_L == true) || (busy_R == true) || (turning == true) || (speed_L != 0) || (speed_R != 0)){

        if((busy_L == false) && (busy_R == false) && (turning == true)){
            turning = false;
        }
        return true;
    }
    else{
        return false;
    }
}

// this is the encoder logic
void Motor::encoder_rise_handler_RA(){
    if(encoder_RB->read() == 1){
        encoder_count_R--;
    }
    else{
        encoder_count_R++;
    }
}
void Motor::encoder_rise_handler_RB(){
    if(encoder_RA->read() == 1){
        encoder_count_R++;
    }
    else{
        encoder_count_R--;
    }
}
void Motor::encoder_rise_handler_LA(){
    if(encoder_LB->read() == 1){
        encoder_count_L--;
    }
    else{
        encoder_count_L++;
    }
}
void Motor::encoder_rise_handler_LB(){
    if(encoder_LA->read() == 1){
        encoder_count_L++;
    }
    else{
        encoder_count_L--;
    }
}
void Motor::encoder_fall_handler_RA(){
    if(encoder_RB->read() == 1){
        encoder_count_R++;
    }
    else{
        encoder_count_R--;
    }
}
void Motor::encoder_fall_handler_RB(){
    if(encoder_RA->read() == 1){
        encoder_count_R--;
    }
    else{
        encoder_count_R++;
    }
}
void Motor::encoder_fall_handler_LA(){
    if(encoder_LB->read() == 1){
        encoder_count_L++;
    }
    else{
        encoder_count_L--;
    }
}
void Motor::encoder_fall_handler_LB(){
    if(encoder_LA->read() == 1){
        encoder_count_L--;
    }
    else{
        encoder_count_L++;
    }
}


Motor::Motor(void){

#if defined(SERIAL_DEBUG)
    serial = new Serial(USBTX, USBRX, 9600);
#endif
    //create objects form the pointers and assign their pins from pin.h

    motor_R = new PwmOut(PWM_R);    
    motor_L = new PwmOut(PWM_L);
    dir_R = new DigitalOut(DIR_R);
    dir_L = new DigitalOut(DIR_L);
    motor_EN = new DigitalOut(MOTOR_EN);

    motor_R->period(0.020);      //set the periods of the PWM signal
    motor_L->period(0.020);

    motor_R->write(1.0);     
    motor_L->write(1.0);

    wait(0.5);      //used to avoid jerking when started

    motor_EN->write(1);

    //make the initial values of the distance 0
    distance_R = 0;
    distance_L = 0;

	busy_R = false;
	busy_L = false;
    turning = false;	

    target_speed_R = 0;
    target_speed_L = 0;
    
    encoder_RA = new InterruptIn(ENCODER_RA);
    encoder_RB = new InterruptIn(ENCODER_RB);
    encoder_LA = new InterruptIn(ENCODER_LA);
    encoder_LB = new InterruptIn(ENCODER_LB);


    //select the necessery interrupts for X1, X2 or X4 encoder sampleing
#if defined(X1) || defined(X2) || defined(X4)
    encoder_RA->rise(callback(this, &Motor::encoder_rise_handler_RA));
    encoder_LA->rise(callback(this, &Motor::encoder_rise_handler_LA));
#endif

#if defined(X2) || defined(X4)
    encoder_RA->fall(callback(this, &Motor::encoder_fall_handler_RA));
    encoder_LA->fall(callback(this, &Motor::encoder_fall_handler_LA));
#endif

#if defined(X4)
    encoder_RB->rise(callback(this, &Motor::encoder_rise_handler_RB));
    encoder_RB->fall(callback(this, &Motor::encoder_fall_handler_RB));
    encoder_LB->rise(callback(this, &Motor::encoder_rise_handler_LB));
    encoder_LB->fall(callback(this, &Motor::encoder_fall_handler_LB));
#endif
    
#if defined(SERIAL_DEBUG)
    serial->printf("Motors and encoders initialised!\n\r");
#endif
    //start the speed measuring ISR
    check_speed.attach(callback(this, &Motor::speed_ISR), CHECK_SPEED_INTERVAL);

#if defined(SERIAL_DEBUG)
    serial->printf("check_speed attached!\n\r");
#endif

}
