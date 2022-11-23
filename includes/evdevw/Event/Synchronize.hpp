#ifndef EVDEVW_SYNCHRONIZEEVENT_HPP
#define EVDEVW_SYNCHRONIZEEVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class SynchronizeCode {
      Report = SYN_REPORT,
      Config = SYN_CONFIG,
      MtReport = SYN_MT_REPORT,
      Dropped = SYN_DROPPED,
    };

  }

  DECLARE_ENUM_CONVERTER(event::SynchronizeCode, uint16_t, SYN_MAX);
  DECLARE_EVENT_TYPE(EV_SYN, Synchronize, SynchronizeCode, int);

}

#endif //EVDEVW_SYNCHRONIZEEVENT_HPP
