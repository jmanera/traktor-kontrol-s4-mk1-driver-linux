#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_JOG_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_JOG_H

// --------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
// --------------------------
#include "MidiEventOut.h"
#include "MidiHelper.h"
#include "UtilsHelper.h"

using namespace std;

class Jog
{
 public:
    Jog(int code,
        string name,
        int value);
    Jog();
    int code;
    string name;
    int value;
    int counter;
    int prev_control_value;
    int sensitivity;
    int updated;
    unsigned int handle_event(RtMidiOut *midi_out_port,
                              bool shift_ch1,
                              bool shift_ch2,
                              bool toggle_ac,
                              bool toggle_bd);
    static map<int, Jog *> jog_mapping;
    int get_value_jog();
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_JOG_H
