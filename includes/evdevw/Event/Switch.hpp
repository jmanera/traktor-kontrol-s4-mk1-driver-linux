#ifndef EVDEVW_SWITCHEVENT_HPP
#define EVDEVW_SWITCHEVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class SwitchCode {
      Lid = SW_LID,
      TabletMode = SW_TABLET_MODE,
      HeadphoneInsert = SW_HEADPHONE_INSERT,
      RfkillAll = SW_RFKILL_ALL,
      Radio = SW_RADIO,
      MicrophoneInsert = SW_MICROPHONE_INSERT,
      Dock = SW_DOCK,
      LineoutInsert = SW_LINEOUT_INSERT,
      JackPhysicalInsert = SW_JACK_PHYSICAL_INSERT,
      VideooutInsert = SW_VIDEOOUT_INSERT,
      CameraLensCover = SW_CAMERA_LENS_COVER,
      KeypadSlide = SW_KEYPAD_SLIDE,
      FrontProximity = SW_FRONT_PROXIMITY,
      RotateLock = SW_ROTATE_LOCK,
      LineinInsert = SW_LINEIN_INSERT,
      MuteDevice = SW_MUTE_DEVICE,
      PenInserted = SW_PEN_INSERTED,
    };

  }

  DECLARE_ENUM_CONVERTER(event::SwitchCode, uint16_t, SW_MAX);
  DECLARE_EVENT_TYPE(EV_SW, Switch, SwitchCode, int);

}

#endif //EVDEVW_SWITCHEVENT_HPP
