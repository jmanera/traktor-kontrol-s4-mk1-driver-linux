#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H

#include <string>
#include <map>
#include "Led.h"
#include "AlsaHelper.h"
#include "MidiEventOut.h"
#include "MidiHelper.h"

using namespace std;

class Button{
private:

public:
    Button(int, string, int, int, int);
    Button();
    int code;
    string name;
    int led_code;
    int channel;
    int value;
    bool hold;
    static map<int, Button *> buttons_mapping;
    unsigned int handle_event(RtMidiOut *, int, bool, bool, bool, bool);
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_BUTTON_H
