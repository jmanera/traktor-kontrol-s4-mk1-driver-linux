#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H

#include <string>
#include <rtaudio/RtAudio.h>
#include <alsa/asoundlib.h>
#include "spdlog/spdlog.h"



using namespace std;

class AlsaHelper {
private:
    static void show_control_id(snd_ctl_elem_id_t *);
    static void check(int err, const char *f);
public:
    static int set_led_value(int, int, int);
    static int bulk_led_value(int, int [], int, int);
    static int get_traktor_device();
};


#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_RTAUDIOHELPER_H
