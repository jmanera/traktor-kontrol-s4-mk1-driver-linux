#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVHELPER_H
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVHELPER_H

#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <filesystem>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <typeinfo>
#include <variant>
#include <libevdev/libevdev.h>
#include <map>
#include <tuple>
#include <signal.h>
#include "spdlog/spdlog.h"
#include "evdevw.hpp"
#include "Button.h"
#include "Knob.h"
#include "Led.h"
#include "Knob.h"
#include "EvDevEvent.h"
#include "AlsaHelper.h"

using namespace std;

class EvDevHelper{
private:
    static vector<string> get_evdev_device();

    static void shutdown_application(int signal_num);

public:
    EvDevHelper(){
      traktor_device_id_ = AlsaHelper::get_traktor_device();
      if (traktor_device_id_ == -1){
        spdlog::error("[EvDevHelper:EvDevHelper] Traktor Kontrol S4 Device not found.... Bye!");
        exit(EXIT_FAILURE);
      }

    }
    static int traktor_device_id_;
    static tuple<int, struct libevdev *> get_traktor_controller_device();
    static void read_events_from_device(RtMidiOut *midi_out_port);
    static void initialize_buttons_leds();
    static void shutdown_buttons_leds();
    static void check_evdev_status();
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_EVDEVHELPER_H
