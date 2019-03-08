#include "sensors.h"

#define DELAY 0.2

int state = 0;

int main() {

	ScanLine *sensors = new ScanLine();
	Serial serial(USBTX, USBRX, 115200);

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
