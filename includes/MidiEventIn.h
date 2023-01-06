#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_MIDIEVENTIN_H_
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_MIDIEVENTIN_H_

// --------------------------
#include <map>
#include <string>
// --------------------------

using namespace std;

class MidiEventIn
{
 public:
  MidiEventIn(string control_channel_1,
              string control_channel_2,
              string control_channel_3,
              string control_channel_4,
              string control_channel_5);

  string control_channel_1;
  string control_channel_2;
  string control_channel_3;
  string control_channel_4;
  string control_channel_5;

  static const map<char, MidiEventIn *> midi_in_mapping;
  string check_channel_value(unsigned char channel);
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_MIDIEVENTIN_H_
