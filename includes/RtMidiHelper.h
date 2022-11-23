//
// Created by aspgems on 17/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <rtmidi/RtMidi.h>
#include <spdlog/spdlog.h>

using namespace std;

class RtMidiHelper{
private:
    static void midi_in_callback(double, std::vector< unsigned char > *, void *);
public:
    RtMidiHelper();
    RtMidiOut *pMidiOut = 0;
    RtMidiIn *pMidiIn = 0;
    bool close_input_port();
    bool close_output_port();
    static void show_midi_information(RtMidiHelper *);
};
#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
