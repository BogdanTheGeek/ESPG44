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
    
    this->update_speed_PID();
}

void Motor::update_speed_PID(){
    
    static double pwm_L, pwm_R;

    //initialise target value of sensors
    static double last_error_L, last_error_R;
    static double I_value_L, I_value_R;
    //error term
    double error_L = target_speed_L - speed_L;
    double error_R = target_speed_R - speed_R;


    //Integral term
    I_value_L += error_L * CHECK_SPEED_INTERVAL;
    I_value_R += error_R * CHECK_SPEED_INTERVAL;

    //final pid output
    double PID_out_L = KP * error_L + KD * (error_L - last_error_L)/CHECK_SPEED_INTERVAL + KI * I_value_L;
    double PID_out_R = KP * error_R + KD * (error_R - last_error_R)/CHECK_SPEED_INTERVAL + KI * I_value_R;

    //add limitation of range
    if (PID_out_L > MAX){
    PID_out_L = MAX;
    }
    else if(PID_out_L < MIN){
    PID_out_L = MIN;
    }
    if (PID_out_R > MAX){
    PID_out_R = MAX;
    }
    else if(PID_out_R < MIN){
    PID_out_R = MIN;
    }

    //store error for next time use
    last_error_L = error_L;
    last_error_R = error_R;

    pwm_L += PID_out_L;
    pwm_R += PID_out_R;


    this->set_speed_L(pwm_L);
    this->set_speed_R(pwm_R);
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


void Motor::move_distance_R(long distance, double speed){

    long start_distance = distance_R;
    end_distance_R = start_distance + distance;

    if(distance > 0){
        direction_R = true;
        this->set_speed_R(speed);
    }
    else {
        direction_R = false;
        this->set_speed_R(-speed);
    }

    
    this->check_distance_R();

}

void Motor::move_distance_L(long distance, double speed){

    long start_distance = distance_L;
    end_distance_L = start_distance + distance;

    if(distance > 0){
        direction_L = true;
        this->set_speed_L(speed);

    }
    else {
        direction_L = false;
        this->set_speed_L(-speed);
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
            this->set_speed_R(0);
        }
    }else{
        if(distance_R > end_distance_R){
            busy_R = true;
            check_reached_distance_R.attach(callback(this, &Motor::check_distance_R), CHECK_DISTANCE_INTERVAL);
        }else{
            busy_R = false;
            this->set_speed_R(0);
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
            this->set_speed_L(0);
        }
    }else{
        if(distance_L > end_distance_L){
            busy_L = true;
            check_reached_distance_L.attach(callback(this, &Motor::check_distance_L), CHECK_DISTANCE_INTERVAL);          
        }else{
            busy_L = false;
            this->set_speed_L(0);
        }
    }
}

void Motor::turn(double degrees, double speed){

    long distance = (degrees/360.0)*(PI*WHEEL_AXEL_LENGTH);

    if (degrees >= 0){
        distance = distance - 20;
    }else{
        distance = distance + 20;
    }

    turning = true;

    this->move_distance_R(-distance, speed);
    this->move_distance_L(distance, speed);    
    
}

bool Motor::busy(void){
    if(  ((busy_L == false) && (busy_R == false)) ||  ((speed_L == 0) && (speed_R == 0))  ){

        if((busy_L == false) && (busy_R == false) && (turning == true)){
            turning = false;
        }
        return false;
    }
    else{
        return true;
    }
}

double Motor::return_speed_R(void){
    return speed_R;
}

double Motor::return_speed_L(void){
    return speed_L;
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
