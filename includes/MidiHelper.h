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
    static RtMidiIn::RtMidiCallback midi_in_callback(double, std::vector< unsigned char> *, void *);
    static RtMidiErrorCallback midi_in_error_callback(RtMidiError::Type type, const string &error_message, void *userData);
    static vector<string> explode(string& str, const char& ch);
    static int get_key_letter(unsigned char channel, unsigned char value);
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
