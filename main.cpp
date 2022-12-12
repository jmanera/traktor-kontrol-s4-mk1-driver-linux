#include <iostream>
#include <cstdlib>
#include "includes/MidiHelper.h"
#include "includes/EvDevHelper.h"

using namespace std;

MidiHelper *rtmidi_helper;
EvDevHelper *evdev_helper;

void shutdown_application(int signum){
    spdlog::info("[main::shutdown_application] Shutting down application....");
    EvDevHelper::shutdown_buttons_leds();
    rtmidi_helper->close_input_port();
    rtmidi_helper->close_output_port();
    spdlog::info("[main::shutdown_application] Exiting application with code: {0}", signum);
    spdlog::info("[main::shutdown_application] Good bye!");
    exit(EXIT_SUCCESS);
}

void init_application(){
    spdlog::set_level(spdlog::level::info);
    spdlog::info("[main::init_application] Welcome to Traktor Kontrol S4 Mk1 Driver for Linux!!!");

    spdlog::info("[main::init_application] Starting helpers....");
    evdev_helper = new EvDevHelper();
    rtmidi_helper = new MidiHelper();

    spdlog::info("[main::init_application] Get MIDI information....");
    MidiHelper::show_midi_information(rtmidi_helper);

    spdlog::info("[main::init_application] Initializing EvDev device....");
    EvDevHelper::check_evdev_status();

    spdlog::info("[main::init_application] Initializing leds....");
    EvDevHelper::initialize_buttons_leds();

    spdlog::info("[main::init_application] Reading events from Traktor Kontrol S4 Mk1....");
    evdev_helper->read_events_from_device(rtmidi_helper->pMidiOut);
}

int main(int argc, char **argv) {
    signal(SIGTERM,shutdown_application);
    signal(SIGKILL,shutdown_application);
    signal(SIGSEGV,shutdown_application);

    init_application();

    shutdown_application(0);
}
