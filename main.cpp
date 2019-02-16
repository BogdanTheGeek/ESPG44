
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
	serial.printf("Sensors: %.2f %.2f %.2f %.2f %.2f \n\r", scanline->values[0],scanline->values[1],scanline->values[2],scanline->values[3],scanline->values[4]);
	wait(2);
	}
}
