#pragma once
#ifndef MHS_MIDIMAPPING_H
#define MHS_MIDIMAPPING_H

enum PlaybackMode { ONESHOT, LOOP, TOGGLE };

namespace Mapping {
    static String mode0[128] = {"ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT",
                         "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT", "ONESHOT"};
};

class MidiMapping {

public: static MidiMapping *getInstance();
private: static MidiMapping *instance;

public:
    MidiMapping(String mapping[128], String mode[128] = Mapping::mode0);

private:
    String *mapping;
    PlaybackMode *mode;

};


#endif //MHS_MIDIMAPPING_H
