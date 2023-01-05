#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include "includes/MidiHelper.h"
#include "includes/EvDevHelper.h"
#include "includes/cxxopts.hpp"
#include "includes/UtilsHelper.h"
#include "includes/ConfigHelper.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/async.h"

using namespace std;

MidiHelper *rtmidi_helper;
EvDevHelper *evdev_helper;
ConfigHelper *config_helper;

void save_pid()
{
  ofstream pid_file;
  pid_file.open (config_helper->get_string_value("pid_file").c_str());
  auto pid = getpid();
  pid_file << pid;
  pid_file.close();
}

bool delete_pid_file()
{
  shared_ptr<spdlog::logger> logger = spdlog::get(config_helper->get_string_value("traktor_s4_logger_name"));
  if(remove(config_helper->get_string_value("pid_file").c_str()) != 0){
    logger->error( "[main::delete_pid_file] Error removing PID file");
    return false;
  }
  logger->info( "[main::delete_pid_file] PID file removed");
  return true;
}

void shutdown_application(int signum)
{
  EvDevHelper::shutdown_buttons_leds(::config_helper);
  delete_pid_file();
  exit(signum);
}

void capture_signals()
{
  signal(SIGTERM,shutdown_application);
  signal(SIGKILL,shutdown_application);
  signal(SIGSEGV,shutdown_application);
  signal(SIGABRT,shutdown_application);
  signal(SIGQUIT,shutdown_application);
  signal(SIGTSTP,shutdown_application);
  signal(SIGINT,shutdown_application);
}

static void show_main_configuration() {
  shared_ptr<spdlog::logger> logger = spdlog::get(config_helper->get_string_value("traktor_s4_logger_name"));
  logger->set_level(config_helper->log_level);

  logger->info( "[main::init_application] Traktor Kontrol S4 Mk1 Driver for Linux started");

  logger->info("[main:show_main_configuration] Current arguments:");

  if (config_helper->log_level == spdlog::level::info) {
    logger->info("[main:show_main_configuration] Log level: INFO");
  } else {
    logger->info("[main:show_main_configuration] Log level: DEBUG");
  }

  if (config_helper->log_mode == LOG_FILE) {
    logger->info("[main:show_main_configuration] Log mode: LOG IN FILE");
  }
  else{
    logger->info("[main:show_main_configuration] Log mode: LOG IN CONSOLE");
  }
}

static void init_application() {
  shared_ptr<spdlog::logger> logger = spdlog::get(config_helper->get_string_value("traktor_s4_logger_name"));

  logger->info("[main::init_application] Starting helpers....");
  evdev_helper = new EvDevHelper(config_helper);
  rtmidi_helper = new MidiHelper(config_helper);

  logger->info("[main::init_application] Get MIDI information....");
  MidiHelper::show_midi_information(rtmidi_helper, config_helper);

  logger->info("[main::init_application] Initializing EvDev device....");
  evdev_helper->check_evdev_status();

  logger->info("[main::init_application] Initializing leds....");
  evdev_helper->initialize_buttons_leds(config_helper);

  logger->info("[main::init_application] Reading events from Traktor Kontrol S4 Mk1....");
  evdev_helper->read_events_from_device(rtmidi_helper->pMidiOut);
}

int main(int argc, char **argv)
{
    capture_signals();
    config_helper = new ConfigHelper();

    cxxopts::Options options("TraktorKontrolS4DriverLinux", "Driver for Native Instruments Traktor Kontrol S4 Mk1");
    options.add_options()
        ("help", "Shows this help")
        ("logLevel", "Enable Log level: debug | info (default)", cxxopts::value<std::string>())
        ("logMode", "Enable Log model: console | logfile (default)", cxxopts::value<std::string>())
        ("configFile", "Set PATH for config.json path (default: ../config.json)", cxxopts::value<std::string>());

    try{
      auto result = options.parse(argc, argv);
      if (result.count("help"))
      {
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
      }

      string config_file_uri = "../config.json";
      if (result.count("configFile")){
        config_file_uri = result["configFile"].as<std::string>();
      }

      if (!config_helper->init_config(config_file_uri)) {
        cerr << "Error reading config file (config.json)" << endl;
        exit (EXIT_FAILURE);
      }

      save_pid();

      if (result.count("logLevel")){
        if(result["logLevel"].as<std::string>() == "debug"){
          config_helper->log_level = spdlog::level::debug;
        }
        else if(result["logLevel"].as<std::string>() == "info"){
          config_helper->log_level = spdlog::level::info;
        }
      }
      else{
        config_helper->log_level = spdlog::level::info;
      }

      spdlog::drop_all();

      if (result.count("logMode") && (result["logMode"].as<std::string>() == "console")){
        config_helper->log_mode = LOG_CONSOLE;
        auto console_logger = spdlog::stdout_color_mt(
            config_helper->get_string_value("traktor_s4_logger_name"));
      }
      else{
        config_helper->log_mode = LOG_FILE;
        auto file_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
            config_helper->get_string_value("traktor_s4_logger_name"), config_helper->get_string_value("traktor_s4_log_file"));
      }


    }
    catch(cxxopts::exceptions::no_such_option error){
      std::cout << options.help() << std::endl;
      exit(EXIT_SUCCESS);
    }

    show_main_configuration();
    init_application();

    shutdown_application(EXIT_SUCCESS);
}
