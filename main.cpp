
#include "demo1.h"
#include "sensors.h"

int main() {

	//demo1();

	
	Serial serial(USBTX, USBRX);
	serial.printf("Serial started!\n\r");

	ScanLine *scanline = new ScanLine();
	serial.printf("Sensors initialised!\n\r");


	while(1){
	serial.printf("Scan!\n\r");
	scanline->scan();
	serial.printf("Sensors: %.2f %.2f %.2f %.2f %.2f \n\r", scanline->values[1],scanline->values[2],scanline->values[3],scanline->values[4],scanline->values[5]);
	serial.printf("position: %.3f \n\r", scanline->array_to_value());
	wait(2);
	}
}
