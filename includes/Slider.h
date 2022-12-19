#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_SLIDER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_SLIDER_H

// --------------------------
#include <string>
#include <map>
// --------------------------
#include "MidiEventOut.h"
#include "MidiHelper.h"

using namespace std;

class Slider
{
 public:
    Slider(int code,
           string name,
           int value);
    Slider();
    int code, value;
    string name;
    static map<int, Slider *> sliders_mapping;
    int handle_event(RtMidiOut *midi_out_port,
                     bool shift_ch1,
                     bool shift_ch2,
                     bool toggle_ac,
                     bool toggle_bd);
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_SLIDER_H
