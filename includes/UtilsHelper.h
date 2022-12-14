#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_INCLUDES_UTILSHELPER_H_
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_INCLUDES_UTILSHELPER_H_

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

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
};
#endif  // TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_INCLUDES_UTILSHELPER_H_
