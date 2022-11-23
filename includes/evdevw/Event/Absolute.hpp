#ifndef EVDEVW_ABSOLUTEEVENT_HPP
#define EVDEVW_ABSOLUTEEVENT_HPP

#include <cstdint>

#include "Event.hpp"

namespace evdevw {

  namespace event {

    enum class AbsoluteCode {
      X = ABS_X,
      Y = ABS_Y,
      Z = ABS_Z,
      Rx = ABS_RX,
      Ry = ABS_RY,
      Rz = ABS_RZ,
      Throttle = ABS_THROTTLE,
      Rudder = ABS_RUDDER,
      Wheel = ABS_WHEEL,
      Gas = ABS_GAS,
      Brake = ABS_BRAKE,
      Hat0x = ABS_HAT0X,
      Hat0y = ABS_HAT0Y,
      Hat1x = ABS_HAT1X,
      Hat1y = ABS_HAT1Y,
      Hat2x = ABS_HAT2X,
      Hat2y = ABS_HAT2Y,
      Hat3x = ABS_HAT3X,
      Hat3y = ABS_HAT3Y,
      Pressure = ABS_PRESSURE,
      Distance = ABS_DISTANCE,
      TiltX = ABS_TILT_X,
      TiltY = ABS_TILT_Y,
      ToolWidth = ABS_TOOL_WIDTH,
      Volume = ABS_VOLUME,
      Misc = ABS_MISC,
      MtSlot = ABS_MT_SLOT,
      MtTouchMajor = ABS_MT_TOUCH_MAJOR,
      MtTouchMinor = ABS_MT_TOUCH_MINOR,
      MtWidthMajor = ABS_MT_WIDTH_MAJOR,
      MtWidthMinor = ABS_MT_WIDTH_MINOR,
      MtOrientation = ABS_MT_ORIENTATION,
      MtPositionX = ABS_MT_POSITION_X,
      MtPositionY = ABS_MT_POSITION_Y,
      MtToolType = ABS_MT_TOOL_TYPE,
      MtBlobId = ABS_MT_BLOB_ID,
      MtTrackingId = ABS_MT_TRACKING_ID,
      MtPressure = ABS_MT_PRESSURE,
      MtDistance = ABS_MT_DISTANCE,
      MtToolX = ABS_MT_TOOL_X,
      MtToolY = ABS_MT_TOOL_Y,
    };

  }

  DECLARE_ENUM_CONVERTER(event::AbsoluteCode, uint16_t, ABS_MAX);
  DECLARE_EVENT_TYPE(EV_ABS, Absolute, AbsoluteCode, int);

}

#endif //EVDEVW_ABSOLUTEEVENT_HPP
