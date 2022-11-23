//
// Created by aspgems on 23/11/22.
//

#include "RtMidiEvent.h"

RtMidiEvent::RtMidiEvent(unsigned int in_status_type, string in_controller_type){
    controller_type = in_controller_type;
    status_byte = in_status_type;
}

static map<int, RtMidiEvent *> midi_mapping = {
        { 16, new RtMidiEvent(0x45, "JOG")}
};

