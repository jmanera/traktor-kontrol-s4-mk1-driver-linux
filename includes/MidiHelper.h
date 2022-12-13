#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <rtmidi/RtMidi.h>
#include <sstream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "Button.h"
#include "Slider.h"
#include "Jog.h"
#include "Knob.h"
#include "Led.h"
#include "EvDevHelper.h"

using namespace std;

class MidiHelper{
private:
    static RtMidiIn::RtMidiCallback midi_in_callback(double deltatime, std::vector< unsigned char> *message, void *user_data);
    static RtMidiErrorCallback midi_in_error_callback(RtMidiError::Type type, const string &error_message, void *user_data);
    static vector<string> explode(string& string_to_explode, const char& separator);
 public:
    MidiHelper();
    RtMidiOut *pMidiOut = 0;
    RtMidiIn *pMidiIn = 0;
    bool close_input_port();
    bool close_output_port();
    static void show_midi_information(MidiHelper *rtmidi_helper);
    int traktor_device_id;


};
#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIHELPER_H
