#ifndef EVDEVW_REPEATEVENT_HPP
#define EVDEVW_REPEATEVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class RepeatCode {
      Delay = REP_DELAY,
      Period = REP_PERIOD,
    };

  }

  DECLARE_ENUM_CONVERTER(event::RepeatCode, uint16_t, REP_MAX);
  DECLARE_EVENT_TYPE(EV_REP, Repeat, RepeatCode, int);

}

#endif //EVDEVW_REPEATEVENT_HPP
