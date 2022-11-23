#ifndef EVDEVW_FORCEFEEDBACKEVENT_HPP
#define EVDEVW_FORCEFEEDBACKEVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class ForceFeedbackCode {
      Rumble = FF_RUMBLE,
      Periodic = FF_PERIODIC,
      Constant = FF_CONSTANT,
      Spring = FF_SPRING,
      Friction = FF_FRICTION,
      Damper = FF_DAMPER,
      Inertia = FF_INERTIA,
      Ramp = FF_RAMP,
      EffectMin = FF_EFFECT_MIN,
      EffectMax = FF_EFFECT_MAX,
      Square = FF_SQUARE,
      Triangle = FF_TRIANGLE,
      Sine = FF_SINE,
      SawUp = FF_SAW_UP,
      SawDown = FF_SAW_DOWN,
      Custom = FF_CUSTOM,
      WaveformMin = FF_WAVEFORM_MIN,
      WaveformMax = FF_WAVEFORM_MAX,
      Gain = FF_GAIN,
      Autocenter = FF_AUTOCENTER,
      MaxEffects = FF_MAX_EFFECTS,
    };

  }

  DECLARE_ENUM_CONVERTER(event::ForceFeedbackCode, uint16_t, FF_MAX);
  DECLARE_EVENT_TYPE(EV_FF, ForceFeedback, ForceFeedbackCode, int);

}

#endif //EVDEVW_FORCEFEEDBACKEVENT_HPP
