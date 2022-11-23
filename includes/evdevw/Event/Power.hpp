#ifndef EVDEVW_POWEREVENT_HPP
#define EVDEVW_POWEREVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class PowerCode {
      Null = 0,
    };

  }

  DECLARE_ENUM_CONVERTER(event::PowerCode, uint16_t, 0);
  DECLARE_EVENT_TYPE(EV_PWR, Power, PowerCode, int);

}

#endif //EVDEVW_POWEREVENT_HPP
