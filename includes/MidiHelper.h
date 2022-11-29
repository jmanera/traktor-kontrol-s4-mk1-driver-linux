#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <rtmidi/RtMidi.h>
#include "spdlog/spdlog.h"
#include "Button.h"
#include "Slider.h"

using namespace std;

class MidiHelper{
private:
    static void midi_in_callback(double, std::vector< unsigned char > *, void *);
public:
    MidiHelper();
    RtMidiOut *pMidiOut = 0;
    RtMidiIn *pMidiIn = 0;
    bool close_input_port();
    bool close_output_port();
    static void show_midi_information(MidiHelper *);
};
#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
