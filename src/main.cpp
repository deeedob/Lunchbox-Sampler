#include <Arduino.h>
#include <TeensyDebug.h>

#undef main

int main()
{
	Serial.begin(19200);
	Serial.println("Start debug session");
	
	halt_cpu();
	
	while(true) {
		Serial.println("LBS");
		delay(1000);
	}
}
