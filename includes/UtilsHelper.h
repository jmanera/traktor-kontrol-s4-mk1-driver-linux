#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_INCLUDES_UTILSHELPER_H_
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_INCLUDES_UTILSHELPER_H_

// --------------------------
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
// --------------------------
#include "EvDevHelper.h"
#include "MidiHelper.h"

using namespace std;

class UtilsHelper
{
 public:
  static vector<string> explode(string& string_to_explode,
                                const char& separator);
  static void capture_signals();
  static void shutdown_application(int signum);
  static vector<unsigned char> create_message(bool shift1, bool shift2, bool toggle_ac,
                                       bool toggle_bd, MidiEventOut* midi_event,
                                       unsigned char value);
  static bool show_beat_loop_display(unsigned char channel, unsigned char status, unsigned char value, int traktor_device_id, ConfigHelper *config);
  static bool show_vumeters_leds(unsigned char value, int traktor_device_id, string control_id);
  static bool show_static_leds(unsigned char value, int traktor_device_id, string control_id);
};
#endif  // TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_INCLUDES_UTILSHELPER_H_
