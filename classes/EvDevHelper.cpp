//
// Created by aspgems on 17/11/22.
//


#include <iostream>
#include "../includes/EvDevHelper.h"

using namespace std;
vector<string> EvDevHelper::get_evdev_device(){
    string path = "/dev/input";
    vector<string> dev_input_files;
    for (const auto & entry : filesystem::directory_iterator(path)){
        string uri = entry.path();
        if (uri.find("event") != string::npos)
            dev_input_files.push_back(uri);
    }

    return dev_input_files;
}

tuple<int, struct libevdev *> EvDevHelper::get_traktor_controller_device(){
    vector<string> uris = get_evdev_device();
    struct libevdev *dev = NULL;
    for (const string & file : uris){
        spdlog::info("Trying to open {0} file...", file);
        const int fd = open(file.c_str(), O_RDONLY | O_NONBLOCK);
        spdlog::info("FD obtained: {0}", fd);
        try{
            //auto evdev = evdevw::Evdev::create_from_fd(fd);
            int evdev = libevdev_new_from_fd(fd, &dev);
            if (evdev < 0) {
                spdlog::error("[ERROR-0001] Failed to init libevdev ({0})", strerror(-evdev));
                exit(EXIT_FAILURE);
            }
            if ((libevdev_get_id_vendor(dev) == 0x17cc) && (libevdev_get_id_product(dev) == 0xbaff)){
                spdlog::info("Found Traktor S4 Device: {0}", libevdev_get_name(dev));
                return make_tuple(evdev, dev);
            }
        }
        catch (const evdevw::Exception &e) {
            spdlog::error("[ERROR-0002] Error Reading: {0} Error: {1}", file, strerror(e.get_error()));
        }
    }
    return make_tuple(-1, dev);
}

void EvDevHelper::read_events_from_device(RtMidiOut *pMidiOut) {

    struct libevdev *dev = NULL;
    Button *buttons = new Button();
    int rc = 1;
    tie(rc, dev) = get_traktor_controller_device();
    if (rc < 0) {
        spdlog::error("[ERROR-0003] Failed to init libevdev (%s)", strerror(-rc));
        exit(EXIT_FAILURE);
    }
    spdlog::info("Input device name: {0}", libevdev_get_name(dev));
    spdlog::info("Input device ID: bus {0} vendor {1} product {2}",
           libevdev_get_id_bustype(dev),
           libevdev_get_id_vendor(dev),
           libevdev_get_id_product(dev));
    do {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc >= 0){
            /*if (Button::buttons_mapping.find(ev.code) != Button::buttons_mapping.end()){
                unsigned int value = ev.value;
                if (value == 1){
                    value = 31;
                }
                else{
                    value = 10;
                }
                Button *btn_pressed = Button::buttons_mapping[ev.code];
                spdlog::info("KEY: {0} Value: {1} ", btn_pressed->name, value);
                if (btn_pressed->led_code >= 1){
                    string cmd = "amixer -c TraktorKontrolS cset numid=" + to_string(btn_pressed->led_code) + " " + to_string(value) + " > /dev/null";
                    system(cmd.c_str());
                }

                EvDevEvent *evdev_event = new EvDevEvent(ev.type, ev.code, ev.value, ev.time);
                evdev_e
                evdev_event->handle_event();

            }*/
            EvDevEvent *evdev_event = new EvDevEvent(ev.type, ev.code, ev.value, ev.time);
            evdev_event->handle_with(pMidiOut);
            /*spdlog::info("Event: TypeName: {0} - CodeName: {1} - Type: {2} - Code: {3} - Value: {4} - Time: {5}",
                   libevdev_event_type_get_name(ev.type),
                   libevdev_event_code_get_name(ev.type, ev.code),
                   ev.type,
                   ev.code,
                   ev.value,
                   ev.time.tv_sec);*/

        }
    } while (rc == 1 || rc == 0 || rc == -EAGAIN);
}

void EvDevHelper::initialize_buttons_leds(){
    spdlog::info("Initializing controller leds....");

    map<int, Led *>::iterator it;
    for (it = Led::leds_mapping.begin(); it !=Led::leds_mapping.end(); it++)
    {
        Led *led = it->second;
        if (led->by_default == true) {
            string cmd = "amixer -c TraktorKontrolS cset numid=" + to_string(led->code) + " " + to_string(Led::MIDDLE) +
                         " > /dev/null";
            system(cmd.c_str());
        }
    }
    spdlog::info("SUCCESS");
}

void EvDevHelper::shutdown_buttons_leds(){
    spdlog::info("Shutting down controller leds....");

    map<int, Led *>::iterator it;
    for (it = Led::leds_mapping.begin(); it != Led::leds_mapping.end(); it++)
    {
        Led *led = it->second;
        string cmd = "amixer -c TraktorKontrolS cset numid=" + to_string(led->code) + " " + to_string(Led::OFF) + " > /dev/null";
        system(cmd.c_str());
    }
    spdlog::info("SUCCESS");
}

