#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H

#include <string>
#include <rtaudio/RtAudio.h>
#include <alsa/asoundlib.h>
#include "Led.h"
#include "spdlog/spdlog.h"

using namespace std;

class AlsaHelper
{
 private:
    static void show_control_id(snd_ctl_elem_id_t *id);
    static void check(int error, const char *function);

 public:
    static int set_led_value(int card_id, int control_id, int led_value);
    static int bulk_led_value(int card_id, int control_ids[], int led_value, int num_controls);
    static int get_traktor_device();

};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H
