//
// Created by aspgems on 22/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVEVENT_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVEVENT_H

#include <sys/types.h>
#include <string>
#include <map>
#include <linux/types.h>
#include "Button.h"
#include "Knob.h"
#include "Slider.h"
#include "Jog.h"
#include "RtMidiHelper.h"
#include <spdlog/spdlog.h>

using namespace std;
class EvDevEvent {
private:

public:
    EvDevEvent(__u16 type, __u16 code, __s32 value, timeval time);
    __u16 type;
    __u16 code;
    __s32 value;
    timeval time;
    Button *button_dev;
    Knob *knob_dev;
    Slider *slider_dev;
    Jog *jog_dev;
    static const map<__u16, string> types;
    static const map<__u16, string> codes;
    void handle_with(RtMidiOut *midi_out);
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVEVENT_H
