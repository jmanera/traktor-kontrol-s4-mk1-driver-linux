#include <iostream>
#include <cstdlib>
#include "includes/MidiHelper.h"
#include "includes/EvDevHelper.h"
#include "includes/cxxopts.hpp"
#include "includes/UtilsHelper.h"

using namespace std;

MidiHelper *rtmidi_helper;
EvDevHelper *evdev_helper;

static void init_application(){

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

int main(int argc, char **argv)
{
    UtilsHelper::capture_signals();
    cxxopts::Options options("TraktorKontrolS4DriverLinux", "Driver for Native Instruments Traktor Kontrol S4 Mk1");
    options.add_options()
        ("help", "Shows this help")
        ("loglevel", "Enable Log level: debug | info (default)", cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    if (result.count("loglevel")){
      if(result["loglevel"].as<std::string>() == "debug"){
        spdlog::info("[main] Debug Log Level enabled");
        spdlog::set_level(spdlog::level::debug);
      }
      else if(result["loglevel"].as<std::string>() == "info"){
        spdlog::info("[main] Info Log Level enabled");
        spdlog::set_level(spdlog::level::info);
      }
    }
    else{
      spdlog::info("[main] Info Log Level enabled");
      spdlog::set_level(spdlog::level::info);
    }

    if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(EXIT_SUCCESS);
    }

    init_application();

    UtilsHelper::shutdown_application(EXIT_SUCCESS);
}
