#include <mem_sd.hpp>
#include <mem_flash.hpp>

/* Arduino ... why u do this shit?! */
#undef main

int main() {
#ifdef VERBOSE
    Serial.begin(9600);
    Serial.println(":::Lunchbox Sampler:::");
#endif

    auto mf = lbs::MemFlash();
    auto ms = lbs::MemSD();

    auto list = mf.getAllFromFlash();

    mf.transferSingleToFlash("packs/Samplepack01/01.wav");

    list = mf.getAllFromFlash();


}