#pragma once

#include <iostream>

namespace lbs {
    inline std::string getBasename(std::string filepath) {
        return filepath.substr(filepath.find_last_of("/\\") + 1);
    }
}