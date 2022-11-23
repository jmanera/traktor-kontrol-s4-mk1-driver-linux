#ifndef EVDEVW_MISCEVENT_HPP
#define EVDEVW_MISCEVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class MiscCode {
      Serial = MSC_SERIAL,
      Pulseled = MSC_PULSELED,
      Gesture = MSC_GESTURE,
      Raw = MSC_RAW,
      Scan = MSC_SCAN,
      Timestamp = MSC_TIMESTAMP,
    };

  }

  DECLARE_ENUM_CONVERTER(event::MiscCode, uint16_t, MSC_MAX);
  DECLARE_EVENT_TYPE(EV_MSC, Misc, MiscCode, int);

}

#endif //EVDEVW_MISCEVENT_HPP
