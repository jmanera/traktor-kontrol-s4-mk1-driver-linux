#ifndef TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_CONFIGHELPER_H_
#define TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_CONFIGHELPER_H_

// --------------------------
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
// --------------------------

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
};

#endif //TRAKTOR_KONTROL_S4_MK1_DRIVER_LINUX_CLASSES_CONFIGHELPER_H_
