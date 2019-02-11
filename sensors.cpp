
#include "sensors.h"

Sensor::Sensor(PinName p){
    pin = new AnalogIn(p);   //assign the analog pin to the object
}

double Sensor::read_value(void){
    raw = *pin;               //read the raw contents of the object pointer
    value = raw - noise;      //calculate the noise compensated value
    return value;             //return the noise compensated value
}
void Sensor::read_noise(void){
    noise = *pin;             //read the noise value from the object pointer
}

ScanLine::ScanLine(){

    //populate the scan line array
    scan_line[0] = new Sensor(SENSOR_0);
    scan_line[1] = new Sensor(SENSOR_1); 
    scan_line[2] = new Sensor(SENSOR_2);
    scan_line[3] = new Sensor(SENSOR_3);
    scan_line[4] = new Sensor(SENSOR_4);   
    
    //link the control opbects to their phisical pins from pins.h
    clock = new DigitalOut(SEN_CLOCK);
    data = new DigitalOut(SEN_DATA);
    enable = new DigitalOut(SEN_EN);
    
    //put a 1 in the shift register and shift it one position
    *enable = 0;
    *data = 1;
    *clock = 1;
    *clock = 0;
    *data = 0;
    
    n_sensor = 5;       //this is the number of sensors in the scanline
    current_sensor = 0; //start at sensor 0
}

void ScanLine::next(){
    if(current_sensor != n_sensor){     //if not at the end of the scan line 
        *clock = 1;                     //tick the clock
        *clock = 0;
        
        current_sensor++;               //and increment the current sensor number
    }
    else{                               //if at the end of the scanline
        for(int i = n_sensor; i < 8; i++){  
            *clock = 1;                 //tick the clock the remaining number of times to get back to sensor 0
            *clock = 0;
        }

        *data = 1;              //pop another 1 into the shift register
        *clock = 1;
        *clock = 0;
        *data = 0;
        
        current_sensor = 0;     
    }
              
}

void ScanLine::scan(){
    
    for(int i = 0; i < n_sensor+1; i++){        //go through the sensors
        
        scan_line[i]->read_noise();             //read the noise
        *enable = 1;                            //turn on the led
        values[i] = scan_line[i]->read_value(); //read the value
        *enable = 0;                            //turn off the led
        this->next();                           //nove to the next led
    }
    
    
}