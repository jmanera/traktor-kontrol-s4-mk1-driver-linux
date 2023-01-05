#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_CONFIGHELPER_H_
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_CONFIGHELPER_H_

// --------------------------
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "spdlog/logger-inl.h"

// --------------------------
#define LOG_FILE 1
#define LOG_CONSOLE 2

using json = nlohmann::json;

using namespace std;

class ConfigHelper
{
 private:
  json data;
  string config_file_name;
  json parse_configuration_file();

 public:
  bool init_config(const string& file_name);
  string get_string_value(string key);
  int get_int_value(const string& key);
  static int get_int_value_from_file(const string& key, const string file_name);
  static string get_string_value_from_file(const string& key, const string file_name);
  int log_mode;
  spdlog::level::level_enum log_level;
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_CONFIGHELPER_H_
