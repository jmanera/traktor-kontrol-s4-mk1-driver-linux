//
// Created by aspgems on 23/11/22.
//

#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIEVENT_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIEVENT_H

#include <string>
#include <map>

using namespace std;

class MidiEvent {
public:
    MidiEvent(unsigned char status_type,
              unsigned char channel_byte,
              string controller_type,
              string name,
              unsigned char tgl_off_shf_on_status_byte,
              unsigned char tgl_off_shf_on_channel_byte,
              unsigned char tgl_on_shf_off_status_byte,
              unsigned char tgl_on_shf_off_channel_byte,
              unsigned char tgl_on_shf_on_status_byte,
              unsigned char tgl_on_shf_on_channel_byte);
    string controller_type;
    string name;
    unsigned char status_byte;
    unsigned char channel_byte;
    unsigned char tgl_off_shf_on_status_byte;
    unsigned char tgl_off_shf_on_channel_byte;
    unsigned char tgl_on_shf_off_status_byte;
    unsigned char tgl_on_shf_off_channel_byte;
    unsigned char tgl_on_shf_on_status_byte;
    unsigned char tgl_on_shf_on_channel_byte;
    static map<int, MidiEvent *> midi_mapping;
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTMIDIEVENT_H
