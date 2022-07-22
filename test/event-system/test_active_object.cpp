#include <unity.h>
#include <active_object/worker.hpp>

using namespace lbs;

#undef main

int main() {
    Serial.begin(9600);
    Serial.println("Begin program");
    Worker<std::function<void()>> w(500000);
    while(true) {
        w.send([](){
            Serial.println("Hello");
        });
        threads.delay(500);
    }
    return 0;
}
