#ifndef EVDEVW_LOGPRIORITY_HPP
#define EVDEVW_LOGPRIORITY_HPP

#include "Enum.hpp"

namespace evdevw {

  enum class LogPriority {
    Error = LIBEVDEV_LOG_ERROR,
    Info = LIBEVDEV_LOG_INFO,
    Debug = LIBEVDEV_LOG_DEBUG,
  };

  template <>
  struct convert_enum<LogPriority> : public _convert_enum_impl<LogPriority, libevdev_log_priority, (libevdev_log_priority)0> {};

}

#endif //EVDEVW_LOGPRIORITY_HPP
