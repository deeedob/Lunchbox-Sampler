#pragma once

namespace lbs {

    std::string getBasename(const std::string &filepath) {
        return filepath.substr(filepath.find_last_of("/\\") + 1);
    }
}