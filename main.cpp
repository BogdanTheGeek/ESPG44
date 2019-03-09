#include "sensors.h"

#define DELAY 0.2
#define ON_key "esp44"
#define OFF_key "44esp"


int state = 0;

void bluetooth_handler(void);
Serial hm10(BLETX, BLERX, 9600);


int main() {

	ScanLine *sensors = new ScanLine();
	Serial serial(USBTX, USBRX, 115200);
	hm10.attach(&bluetooth_handler, Serial::RxIrq);

	while(true){
		sensors->scan();

		switch(state){
		case 0:
			for (int i = 1; i < 6; ++i){
				serial.printf("%.3f  ", sensors->values[i]);
			}
			serial.printf("\n\r");
		break;
		
		case 1:
			serial.printf("Position: %.3f\n\r", sensors->array_to_value_V2());
		break;
		}

		wait(DELAY);
	}

}

void bluetooth_handler(void){
	char buffer[6];

	int i = 0;
	while(i < 5){
		buffer[i++] = hm10.getc();
	}

	if(strcmp(buffer, ON_key) == 0){ 
	    state = 1;
	   	return;
	}
	if(strcmp(buffer, OFF_key) == 0){ 
		state = 0;
		return;
	}

	return;
}