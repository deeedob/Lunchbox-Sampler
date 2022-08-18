#include <mem_sd.hpp>
#include <mem_flash.hpp>

/* Arduino ... why u do this shit?! */
#undef main

int main()
{
#ifdef VERBOSE
    Serial.begin(9600);
    Serial.println(":::Lunchbox Sampler:::");
#endif

    auto mf = lbs::MemFlash();
    auto ms = lbs::MemSD();

    auto list = mf.getAllFromFlash();

    mf.eraseFlash();

    Serial.println("Files on Flash:");
    for (const auto &i: list) {
        Serial.println(i.c_str());
    }

    Serial.println("Transferring file");

    mf.transferSingleToFlash("packs/Samplepack01/01.wav");

    list = mf.getAllFromFlash();
    Serial.println("Files on Flash:");
    for (const auto &i: list) {
        Serial.println(i.c_str());
    }

}