#ifndef EVDEVW_INPUTPROPERTY_HPP
#define EVDEVW_INPUTPROPERTY_HPP

#include "Enum.hpp"

namespace evdevw {

  enum class InputProperty {
    Pointer = INPUT_PROP_POINTER,
    Direct = INPUT_PROP_DIRECT,
    Buttonpad = INPUT_PROP_BUTTONPAD,
    SemiMt = INPUT_PROP_SEMI_MT,
    Topbuttonpad = INPUT_PROP_TOPBUTTONPAD,
    PointingStick = INPUT_PROP_POINTING_STICK,
    Accelerometer = INPUT_PROP_ACCELEROMETER,
  };

  template <>
  struct convert_enum<InputProperty> : public _convert_enum_impl<InputProperty, int, 0> {};

  inline std::string input_property_get_name(InputProperty input_property) {
    return libevdev_property_get_name(enum_to_raw(input_property));
  }

  inline std::optional <InputProperty> input_property_from_name(const std::string &name) {
    auto raw_input_property = libevdev_property_from_name(name.c_str());
    if (raw_input_property < 0)
      return std::nullopt;
    return raw_to_enum<InputProperty>(raw_input_property);
  }

}

#endif //EVDEVW_INPUTPROPERTY_HPP
