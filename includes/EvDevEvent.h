#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVEVENT_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVEVENT_H

#include <sys/types.h>
#include <string>
#include <map>
#include <linux/types.h>
#include "Button.h"
#include "MidiHelper.h"
#include "spdlog/spdlog.h"

using namespace std;

class EvDevEvent {
private:

public:
    EvDevEvent(__u16 type, __u16 code, __s32 value, timeval time);
    __u16 type;
    __u16 code;
    __s32 value;
    timeval time;
    static const map<__u16, string> types;
    static const map<__u16, string> codes;
    void handle_with(RtMidiOut *midi_out, int, bool, bool, bool, bool);
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVEVENT_H
