#ifndef EVDEVW_READFLAG_HPP
#define EVDEVW_READFLAG_HPP

#include <cstdint>

#include "Enum.hpp"

namespace evdevw {

  enum class ReadFlag {
    Sync = LIBEVDEV_READ_FLAG_SYNC,
    Normal = LIBEVDEV_READ_FLAG_NORMAL,
    ForceSync = LIBEVDEV_READ_FLAG_FORCE_SYNC,
    Blocking = LIBEVDEV_READ_FLAG_BLOCKING,
  };

  template <>
  struct convert_enum<ReadFlag> : public _convert_enum_impl<ReadFlag, int, 0> {};

}

#endif //EVDEVW_READFLAG_HPP
