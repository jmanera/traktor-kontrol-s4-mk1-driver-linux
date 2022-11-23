#ifndef EVDEVW_READSTATUS_HPP
#define EVDEVW_READSTATUS_HPP

#include "Enum.hpp"

namespace evdevw {

  enum class ReadStatus {
    Success = LIBEVDEV_READ_STATUS_SUCCESS,
    Sync = LIBEVDEV_READ_STATUS_SYNC,
  };

  template <>
  struct convert_enum<ReadStatus> : public _convert_enum_impl<ReadStatus, int, 0> {};

}

#endif //EVDEVW_READSTATUS_HPP
