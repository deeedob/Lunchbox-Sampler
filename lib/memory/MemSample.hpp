#pragma once;
#include "MemFlash.hpp"
#include "MemSD.hpp"

namespace lbs {

    class MemSample {

    public: static MemSample &getInstance();

    public:

    private:
        lbs::MemFlash &mf = MemFlash::getInstance();
        lbs::MemSD &ms = MemSD::getInstance();
        MemSample();


    };

}