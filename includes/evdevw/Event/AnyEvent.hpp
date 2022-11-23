#ifndef EVDEVW_ANYEVENT_HPP
#define EVDEVW_ANYEVENT_HPP

#include <variant>

#include "Absolute.hpp"
#include "ForceFeedback.hpp"
#include "ForceFeedbackStatus.hpp"
#include "Key.hpp"
#include "Led.hpp"
#include "Misc.hpp"
#include "Power.hpp"
#include "Relative.hpp"
#include "Repeat.hpp"
#include "Sound.hpp"
#include "Switch.hpp"
#include "Synchronize.hpp"

namespace evdevw::event {

  using AnyEvent = std::variant<
    Absolute,
    ForceFeedback,
    ForceFeedbackStatus,
    Key,
    Led,
    Misc,
    Power,
    Relative,
    Repeat,
    Sound,
    Switch,
    Synchronize>;

}

#endif //EVDEVW_ANYEVENT_HPP
