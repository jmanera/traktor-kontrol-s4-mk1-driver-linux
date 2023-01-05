#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H

// --------------------------
#include <string>
#include <rtaudio/RtAudio.h>
#include <alsa/asoundlib.h>
// --------------------------
#include "Led.h"
#include "spdlog/spdlog.h"
#include "ConfigHelper.h"

using namespace std;

class AlsaHelper
{
 private:

 public:
    static int set_led_value(int card_id, int control_id, int led_value, ConfigHelper *config_helper);
    static int bulk_led_value(int card_id, int control_ids[], int led_value, int num_controls, ConfigHelper *config_helper);
    static int get_traktor_device(ConfigHelper *config_helper);

};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H
