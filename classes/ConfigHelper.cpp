#include "ConfigHelper.h"

#include <utility>
#include "spdlog/spdlog.h"

bool ConfigHelper::init_config(const string& file_name)
{
  ifstream f(file_name.c_str());
  if (f.good()){
    spdlog::info("[ConfigHelper::init_config] Configuration file found!");
    this->config_file_name = file_name;
    this->data = parse_configuration_file();
    if (this->data == NULL){
      return false;
    }
    return true;
  }
  else {
    spdlog::error("[ConfigHelper::init_config] Configuration file not found");
    return false;
  }
}

string ConfigHelper::get_string_value(string key)
{
  return (string)data[std::move(key)];
}

int ConfigHelper::get_int_value(const string& key)
{
  return (int)data.at(key);
}

json ConfigHelper::parse_configuration_file()
{
  try{
    std::ifstream f(this->config_file_name);
    return json::parse(f);
  }
  catch(nlohmann::json_abi_v3_11_2::detail::parse_error &error){
    spdlog::error("[ConfigHelper::parse_configuration_file] Error parsing config file");
    return NULL;
  }

}

