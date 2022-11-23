//
// Created by aspgems on 23/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIEVENT_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIEVENT_H

#include <string>
#include <map>

using namespace std;

class RtMidiEvent {
public:
    RtMidiEvent(unsigned int status_type, string controller_type);
    string controller_type;
    unsigned int status_byte;
    static map<int, RtMidiEvent *> midi_mapping;
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIEVENT_H
