#include <unity.h>
#include <Arduino.h>
#include <SD.h>
#include <SerialFlash.h>
#include <iostream>
#include <map>
#include "MemSample.hpp"

namespace test {

    void
    setUp(void) {
        //clean up stuff here
    }

    void
    tearDown(void) {
        //set up stuff here
    }

    void test_MemS_getIntFromNote() {
        TEST_ASSERT_EQUAL(0, MemSample::MidiMapping::getIntFromNote("C0"));
    }

}

int
main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_MemS_getIntFromNote);
    UNITY_END();
}