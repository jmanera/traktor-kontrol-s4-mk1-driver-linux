#include <iostream>

#include "ConfigHelper.h"

bool ConfigHelper::init_config(const string& file_name)
{
  ifstream f(file_name.c_str());
  if (f.good()){
    this->config_file_name = file_name;
    this->data = parse_configuration_file();
    if (this->data != NULL){
      return true;
    }
  }
  return false;
}

string ConfigHelper::get_string_value(string key)
{
  return (string)data[std::move(key)];
}

int ConfigHelper::get_int_value(const string& key)
{
  try{
    return (int)data.at(key);
  }
  catch(...){
    cerr << key << endl;
  }
  return -1;
}

int ConfigHelper::get_int_value_from_file(const string& key, const string file_name)
{
  ifstream f(file_name.c_str());
  if (f.good()){
    json data = json::parse(file_name);
    if (data != NULL){
      return (int)data.at(key);
    }
    return -1;
  }
  else {
    std::cerr << "[ConfigHelper::init_config] Configuration file not found" << endl;
    return -1;
  }

}

string ConfigHelper::get_string_value_from_file(const string& key, const string file_name)
{
  ifstream f(file_name.c_str());
  if (f.good()){
    json data = json::parse(file_name);
    if (data != NULL){
      return (string)data.at(key);
    }
    return nullptr;
  }
  else {
    std::cerr << "[ConfigHelper::get_string_value_from_file] Configuration file not found" << endl;
    return nullptr;
  }

}

json ConfigHelper::parse_configuration_file()
{
  try{
    std::ifstream f(this->config_file_name);
    return json::parse(f);
  }
  catch(nlohmann::json_abi_v3_11_2::detail::parse_error &error){
    return nullptr;
  }
}

