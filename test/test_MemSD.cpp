#include <unity.h>
#include "MemSD.hpp"

void
setUp(void)
{
        //set up stuff here
}

void
tearDown(void)
{
}

void
test_MemSD_readSettings()
{
    lbs::MemSD &msd = lbs::MemSD::getInstance();
    std::vector<std::tuple<std::string, uint8_t, std::string, std::string>> *settings = msd.readSettings("SamplePack01");

    for (auto i = settings->begin(); i < settings->end(); i++) {
      std::string one = std::get<0>(*i);
      Serial.println(one.c_str());
    }

}

int
main(int argc, char **argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_MemSD_readSettings);
    UNITY_END();
}