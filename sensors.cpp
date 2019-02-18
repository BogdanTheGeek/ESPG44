
#include "sensors.h"

Sensor::Sensor(PinName p){
    pin = new AnalogIn(p);   //assign the analog pin to the object
}

double Sensor::read_value(void){
    raw = pin->read();        //read the raw contents of the object pointer
    value = raw - noise;      //calculate the noise compensated value
    return value;             //return the noise compensated value
}
void Sensor::read_noise(void){
    noise = pin->read();             //read the noise value from the object pointer
}

ScanLine::ScanLine(){

    //populate the scan line array
    scan_line[0] = new Sensor(SENSOR_TAIL);

    scan_line[1] = new Sensor(SENSOR_0);
    scan_line[2] = new Sensor(SENSOR_1); 
    scan_line[3] = new Sensor(SENSOR_2);
    scan_line[4] = new Sensor(SENSOR_3);
    scan_line[5] = new Sensor(SENSOR_4);   
    
    //link the control opbects to their phisical pins from pins.h
    clock = new DigitalOut(SEN_CLOCK);
    data = new DigitalOut(SEN_DATA);
    enable = new DigitalOut(SEN_EN);
    
    //put a 1 in the shift register and shift it one position
    //this is done becouse the output clock is one position behind the shift register clock
    enable->write(0);
    data->write(1);
    wait(PROPAGTION_DELAY);
    clock->write(1);
    wait(PROPAGTION_DELAY);
    clock->write(0);
    data->write(0);
    
    n_sensor = 6;                       //this is the number of sensors in the scanline
    current_sensor = 0;                 //start at sensor 0
}

void ScanLine::next(){

    if(current_sensor < n_sensor){     //if not at the end of the scan line 
        clock->write(1);               //tick the clock
        wait(PROPAGTION_DELAY);
        clock->write(0);
        wait(PROPAGTION_DELAY);
        
        current_sensor++;               //and increment the current sensor number
    }
    else{                               //if at the end of the scanline
        for(int i = n_sensor; i < 7; i++){  
            clock->write(1);            //tick the clock the remaining number of times to get back to sensor 0
            wait(PROPAGTION_DELAY);
            clock->write(0);
            wait(PROPAGTION_DELAY);
        }

        data->write(1);                 //pop another 1 into the shift register
         wait(PROPAGTION_DELAY);

        clock->write(1);                //tick the clock
        wait(PROPAGTION_DELAY);
        clock->write(0);
        wait(PROPAGTION_DELAY);

        data->write(0);
        
        current_sensor = 0;     
    }
              
}

void ScanLine::scan(){

    this->next();

    for(int i = 0; i < n_sensor; i++){          //go through the sensors
        
        scan_line[i]->read_noise();             //read the noise
        enable->write(0);                       //turn on the led

        wait(PROPAGTION_DELAY);

        values[i] = scan_line[i]->read_value(); //read the value
        enable->write(1);                       //turn off the led

        wait(PROPAGTION_DELAY);

        this->next();                           //nove to the next led
    }
     
}

double ScanLine::array_to_value(void){

    double value;

    int max = 0;

    //find the sensor with the highest value
    for (int i = 0; i < n_sensor - 1; i++){

        if(values[i+1] > values[max+1]){
            max = i;
        }
    }
 
    if(max == 0){

        //calculate their positions
        double x0 = max/2.0 - 1;
        double x1 = (max+1)/2.0 - 1;

        //convert to linear function
        double y0 = values[max+1];
        double y1 = values[max+2];
        
        //interpolate
        double ratio = 1 - (y0 / (y0 + y1));
        value = x0 + ((x1 - x0)*ratio);

    }else if(max == 4){

        //calculate their positions
        double x0 = max/2.0 - 1;
        double x1 = (max-1)/2.0 - 1;

        //convert to linear function
        double y0 = values[max+1];
        double y1 = values[max];
        
        //interpolate
        double ratio = 1 - (y0 / (y0 + y1));
        value = x0 + ((x1 - x0)*ratio);

    }else{

        //calculate their positions
        //double x0 = (max-1)/2.0 - 1;
        double x1 = max/2.0 - 1;
        double x2 = (max+1)/2.0 - 1;

        //convert to linear function
        double y0 = values[max];
        double y1 = values[max+1];
        double y2 = values[max+2];
        
        //interpolate
        double ratio = (y1 / (y1 + y0)) - (y1 / (y1 + y2));
        value = x1 + ((x2 - x1)*ratio);

    }


    return value;
}
