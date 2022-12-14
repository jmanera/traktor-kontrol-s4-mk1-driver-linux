#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include "includes/MidiHelper.h"
#include "includes/EvDevHelper.h"
#include "includes/cxxopts.hpp"
#include "includes/UtilsHelper.h"
#include "includes/ConfigHelper.h"

using namespace std;

MidiHelper *rtmidi_helper;
EvDevHelper *evdev_helper;

static void init_application(ConfigHelper *config){

  spdlog::info("[main::init_application] Welcome to Traktor Kontrol S4 Mk1 Driver for Linux!!!");

  spdlog::info("[main::init_application] Starting helpers....");
  evdev_helper = new EvDevHelper(config);
  rtmidi_helper = new MidiHelper(config);

  spdlog::info("[main::init_application] Get MIDI information....");
  MidiHelper::show_midi_information(rtmidi_helper);

  spdlog::info("[main::init_application] Initializing EvDev device....");
  evdev_helper->check_evdev_status();

  spdlog::info("[main::init_application] Initializing leds....");
  evdev_helper->initialize_buttons_leds();

  spdlog::info("[main::init_application] Reading events from Traktor Kontrol S4 Mk1....");
  evdev_helper->read_events_from_device(rtmidi_helper->pMidiOut);
}

int main(int argc, char **argv)
{
    UtilsHelper::capture_signals();
    string config_file_uri = "../config.json";
    cxxopts::Options options("TraktorKontrolS4DriverLinux", "Driver for Native Instruments Traktor Kontrol S4 Mk1");
    options.add_options()
        ("help", "Shows this help")
        ("logLevel", "Enable Log level: debug | info (default)", cxxopts::value<std::string>())
        ("configFile", "Set PATH for config.json path (default: ../config.json)", cxxopts::value<std::string>());

    try{
      auto result = options.parse(argc, argv);
      if (result.count("help"))
      {
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
      }

      if (result.count("logLevel")){
        if(result["logLevel"].as<std::string>() == "debug"){
          spdlog::info("[main] Debug Log Level enabled");
          spdlog::set_level(spdlog::level::debug);
        }
        else if(result["logLevel"].as<std::string>() == "info"){
          spdlog::info("[main] Info Log Level enabled");
          spdlog::set_level(spdlog::level::info);
        }
      }
      else{
        spdlog::info("[main] Info Log Level enabled");
        spdlog::set_level(spdlog::level::info);
      }

      if (result.count("configFile")){
        config_file_uri = result["configFile"].as<std::string>();
      }
    }
    catch(cxxopts::exceptions::no_such_option error){
      spdlog::error("[main] {0}", error.what());
      std::cout << options.help() << std::endl;
      exit(EXIT_SUCCESS);
    }

    ConfigHelper *config = new ConfigHelper();
    if (config->init_config(config_file_uri)){
      init_application(config);
    }

    UtilsHelper::shutdown_application(EXIT_SUCCESS);
}
