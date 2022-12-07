//
// Created by aspgems on 21/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_KNOB_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_KNOB_H

#include <string>
#include <map>
#include <chrono>
#include "MidiEventOut.h"
#include "MidiHelper.h"

using namespace std;

class Knob {
private:

    int get_value_gain_rot();
    int get_value_rot();
    int get_value_browser_rot();
public:
    Knob(int code, string name, int value);
    Knob();
    int code;
    string name;
    int value;
    int counter;
    int prev_control_value;
    int updated;
    static map<int, Knob *> knob_mapping;
    unsigned int handle_event(RtMidiOut *, bool, bool, bool, bool);

};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_KNOB_H
