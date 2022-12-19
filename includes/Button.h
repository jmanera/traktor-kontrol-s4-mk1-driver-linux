#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H

// --------------------------
#include <string>
#include <map>
// --------------------------
#include "Led.h"
#include "AlsaHelper.h"
#include "MidiEventOut.h"
#include "MidiHelper.h"

using namespace std;

class Button
{
 public:
    Button(int code,
           string name,
           int led_code,
           int channel,
           int value);
    Button();
    int code;
    string name;
    int led_code;
    int channel;
    int value;
    bool hold{};
    static map<int, Button *> buttons_mapping;
    int handle_event(RtMidiOut *midi_out_port,
                    int controller_id,
                    bool shift_ch1,
                    bool shift_ch2,
                    bool toggle_ac,
                    bool toggle_bd);
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H
