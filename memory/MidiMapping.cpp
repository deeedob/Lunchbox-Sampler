#include <Arduino.h>
#include <iostream>

#include "MidiMapping.h"

bool
exists(String filepath)
{
    return true;
}

bool
validMapping(String map)
{
    if (!(map.equals("") || map.equals("")) && exists(map)) {
        return true;
    }
    return false;
}

MidiMapping::MidiMapping(String mapping[128], String mode[128])
{
    unsigned int i;

    for (i = 0; i < this->mapping->length(); i++) {
       this->mapping[i] = "none";
    }

   if(mapping->length() != 128) {
       Serial.println("MidiMapping: Error, mapping array not large enough");
       exit(-1);
   }

   for (i = 0; i < mapping->length(); i++) {
      if (!validMapping(mapping[i])) {
         Serial.println("MidiMapping: Error, invalid mapping array");
         exit(-1);
      }

      if (mapping[i].equals("") || mapping->equals("none")) {
         this->mapping[i] = "none";
      } else {
         this->mapping[i] = mapping[i];
      }
   }

   this->mode = new PlaybackMode[128];

   for (i = 0; i < mode->length(); i++) {
       if (mode[i].equals("ONESHOT")) {
            this->mode[i] = ONESHOT;
       } else if (mode[i].equals("LOOP")) {
           this->mode[i] = LOOP;
       } else if (mode[i].equals("TOGGLE")) {
           this->mode[i] = TOGGLE;
       } else {
          Serial.println("MidiMapping: Erroneous Mapping File");
          exit(-1);
       }
   }

}


