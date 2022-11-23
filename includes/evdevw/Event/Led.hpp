#ifndef EVDEVW_LEDEVENT_HPP
#define EVDEVW_LEDEVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class LedCode {
      Numl = LED_NUML,
      Capsl = LED_CAPSL,
      Scrolll = LED_SCROLLL,
      Compose = LED_COMPOSE,
      Kana = LED_KANA,
      Sleep = LED_SLEEP,
      Suspend = LED_SUSPEND,
      Mute = LED_MUTE,
      Misc = LED_MISC,
      Mail = LED_MAIL,
      Charging = LED_CHARGING,
    };

    enum class LedValue {
      On = LIBEVDEV_LED_ON,
      Off = LIBEVDEV_LED_OFF,
    };

  }

  DECLARE_ENUM_CONVERTER(event::LedCode, uint16_t, LED_MAX);
  //DECLARE_ENUM_CONVERTER(LedValue, libevdev_led_value, (libevdev_led_value)0);
  DECLARE_ENUM_CONVERTER(event::LedValue, int, 0);
  DECLARE_EVENT_TYPE(EV_LED, Led, LedCode, LedValue);

}

#endif //EVDEVW_LEDEVENT_HPP
