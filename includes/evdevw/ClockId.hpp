#ifndef EVDEVW_CLOCKID_HPP
#define EVDEVW_CLOCKID_HPP

#include <time.h>

#include "Enum.hpp"

namespace evdevw {

  enum class ClockId {
    Monotonic = CLOCK_MONOTONIC,
    Realtime = CLOCK_REALTIME,
  };

  template <>
  struct convert_enum<ClockId> : public _convert_enum_impl<ClockId, int, 0> {};

}

#endif //EVDEVW_CLOCKID_HPP
