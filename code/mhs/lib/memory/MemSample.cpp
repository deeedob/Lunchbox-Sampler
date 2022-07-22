#include "MemSample.hpp"

using namespace lbs;

MemSample::MemSample()
{
}

MemSample
&MemSample::getInstance()
{
    static MemSample *instance = new MemSample();
    return *instance;
}




