#include <Arduino.h>
#include <define_t40.hpp>

/* Arduino ... why u do this shit?! */
#undef main

int main() {
    Serial.begin(9600);
    Serial.println(":::Beginning Multiplexer Test:::");

    while(true) {

    }

    return 0;
}