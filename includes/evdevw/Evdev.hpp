#ifndef EVDEVW_EVDEV_HPP
#define EVDEVW_EVDEV_HPP

#include <cerrno>
#include <cstdio>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_set>
#include <string>
#include <utility>

#include <libevdev/libevdev.h>

#include "BusType.hpp"
#include "ClockId.hpp"
#include "Event/Absolute.hpp"
#include "Event/AnyEvent.hpp"
#include "Event/Key.hpp"
#include "Event/Led.hpp"
#include "Event/Repeat.hpp"
#include "Exception.hpp"
#include "InputProperty.hpp"
#include "LogPriority.hpp"
#include "ReadFlag.hpp"
#include "ReadStatus.hpp"
#include "Utility.hpp"

namespace evdevw {

  struct Repeat {
    int delay;
    int period;
  };

  struct AbsoluteInfo {
    AbsoluteInfo(struct input_absinfo absinfo)
      : value(absinfo.value),
        minimum(absinfo.minimum),
        maximum(absinfo.maximum),
        fuzz(absinfo.fuzz),
        flat(absinfo.flat),
        resolution(absinfo.resolution)
    {
    }

    struct input_absinfo to_raw() {
      struct input_absinfo absinfo = {
        .value = value,
        .minimum = minimum,
        .maximum = maximum,
        .fuzz = fuzz,
        .flat = flat,
        .resolution = resolution,
      };

      return absinfo;
    }

    int value;
    int minimum;
    int maximum;
    int fuzz;
    int flat;
    int resolution;
  };

  class Evdev : std::enable_shared_from_this<Evdev> {
  public:
    using Pointer = std::shared_ptr<Evdev>;
    using Resource = struct libevdev;
    using ResourceRawPtr = Resource*;
    using ResourcePtr = std::unique_ptr<Resource, decltype(&libevdev_free)>;

    using DeviceLogFn = std::function<void(Evdev&, LogPriority, void*, std::string, int, std::string, std::string)>;

    struct DeviceLogFnData {
      DeviceLogFnData(DeviceLogFn device_log_fn, void *data)
        : device_log_fn(device_log_fn), data(data)
      {
      }

      DeviceLogFn device_log_fn;
      void *data;
    };

    static std::shared_ptr<Evdev> create() {
      return std::shared_ptr<Evdev>(new Evdev(libevdev_new()));
    }

    static std::shared_ptr<Evdev> create_from_fd(int fd) {
      auto dev = libevdev_new();

      if (const auto err = libevdev_new_from_fd(fd, &dev))
        throw Exception(-err);

      return std::shared_ptr<Evdev>(new Evdev(dev));
    }

  public:
    Evdev(const Evdev &) = delete;
    Evdev(Evdev &&other) = delete;
    Evdev &operator=(const Evdev &) = delete;
    Evdev &operator=(Evdev &&other) = delete;

    ResourceRawPtr raw() {
      return _evdev.get();
    }

    ResourceRawPtr raw() const {
      return _evdev.get();
    }

    //////////////////////////////
    // INITIALIZATION AND SETUP //
    //////////////////////////////

    void grab() const {
      if (const auto err = libevdev_grab(raw(), LIBEVDEV_GRAB))
        throw Exception(-err);
    }

    void ungrab() const {
      if (const auto err = libevdev_grab(raw(), LIBEVDEV_UNGRAB))
        throw Exception(-err);
    }

    void set_fd(int fd) {
      auto dev = _evdev.get();

      if (const auto err = libevdev_set_fd(dev, fd))
        throw Exception(-err);

      _evdev.reset(dev);
    }

    void change_fd(int fd) const {
      if (const auto err = libevdev_set_fd(raw(), fd))
        throw Exception(-err);
    }

    int get_fd() const {
      return libevdev_get_fd(raw());
    }

    ////////////////////////////////
    // LIBRARY LOGGING FACILITIES //
    ////////////////////////////////

    void set_device_log_function(DeviceLogFn device_log_fn, LogPriority priority, void *data) {
      _device_log_fn_data.emplace(device_log_fn, data);

      libevdev_set_device_log_function(raw(), device_log_fn_helper, enum_to_raw(priority), (void*)this);
    }

    //////////////////////////////////
    // QUERYING DEVICE CAPABILITIES //
    //////////////////////////////////

    std::string get_name() const {
      return libevdev_get_name(raw());
    }

    util::OptionalString get_phys() const {
      auto s = libevdev_get_phys(raw());
      return util::raw_to_optional<std::string>(s);
    }

    util::OptionalString get_uniq() const {
      auto s = libevdev_get_uniq(raw());
      return util::raw_to_optional<std::string>(s);
    }

    int get_id_product() const {
      return libevdev_get_id_product(raw());
    }

    int get_id_vendor() const {
      return libevdev_get_id_vendor(raw());
    }

    int get_id_bustype() const {
      return libevdev_get_id_bustype(raw());
    }

    int get_id_version() const {
      return libevdev_get_id_version(raw());
    }

    int get_driver_version() const {
      return libevdev_get_driver_version(raw());
    }

    bool has_property(InputProperty property) const {
      return libevdev_has_property(raw(), enum_to_raw(property));
    }

    template <typename E>
    bool has_event_type() const {
      return libevdev_has_event_type(raw(), E::type);
    }

    template <typename Code>
    bool has_event_code(Code code) const {
      return libevdev_has_event_code(raw(), event::event_from_event_code<Code>::type::type, enum_to_raw(code));
    }

    int get_abs_minimum(event::AbsoluteCode code) const {
      return libevdev_get_abs_minimum(raw(), enum_to_raw(code));
    }

    int get_abs_maximum(event::AbsoluteCode code) const {
      return libevdev_get_abs_maximum(raw(), enum_to_raw(code));
    }

    int get_abs_fuzz(event::AbsoluteCode code) const {
      return libevdev_get_abs_fuzz(raw(), enum_to_raw(code));
    }

    int get_abs_flat(event::AbsoluteCode code) const {
      return libevdev_get_abs_flat(raw(), enum_to_raw(code));
    }

    int get_abs_resolution(event::AbsoluteCode code) const {
      return libevdev_get_abs_resolution(raw(), enum_to_raw(code));
    }

    std::optional<AbsoluteInfo> get_abs_info(event::AbsoluteCode code) {
      auto abs_info = libevdev_get_abs_info(raw(), enum_to_raw(code));

      if (!abs_info)
        return std::nullopt;

      return AbsoluteInfo(*abs_info);
    }

    template <typename Code>
    typename event::event_from_event_code<Code>::type::Value
    get_event_value(Code code) const {
      using E = typename event::event_from_event_code<Code>::type;
      return E::raw_to_value(libevdev_get_event_value(raw(), E::type, enum_to_raw(code)));
    }

    template <typename Code>
    std::optional<typename event::event_from_event_code<Code>::type::Value>
    fetch_event_value(Code code) {
      int value;
      using E = typename event::event_from_event_code<Code>::type;
      if (libevdev_fetch_event_value(raw(), E::type, enum_to_raw(code), &value))
        return E::raw_to_value(value);
      return std::nullopt;
    }

    std::optional<Repeat> get_repeat() {
      Repeat repeat;
      if (libevdev_get_repeat(raw(), &repeat.delay, &repeat.period))
        return std::nullopt;
      return repeat;
    }

    ///////////////////////////////////
    // MULTI-TOUCH RELATED FUNCTIONS //
    ///////////////////////////////////

    int get_slot_value(int slot, event::AbsoluteCode code) const {
      return libevdev_get_slot_value(raw(), slot, enum_to_raw(code));
    }

    std::optional<int> fetch_slot_value(int slot, event::AbsoluteCode code) const {
      int value;
      if (libevdev_fetch_slot_value(raw(), slot, enum_to_raw(code), &value))
        return value;
      return std::nullopt;
    }

    int get_num_slots() const {
      return libevdev_get_num_slots(raw());
    }

    int get_current_slot() const {
      return libevdev_get_current_slot(raw());
    }

    ////////////////////////////////////////////////
    // MODIFYING DEVICE APPEARANCE & CAPABILITIES //
    ////////////////////////////////////////////////

    void set_name(const std::string &name) const {
      libevdev_set_name(raw(), name.c_str());
    }

    void set_phys(const std::string &phys) const {
      libevdev_set_phys(raw(), phys.c_str());
    }

    void set_uniq(const std::string &uniq) const {
      libevdev_set_uniq(raw(), uniq.c_str());
    }

    void set_id_product(int product_id) const {
      libevdev_set_id_product(raw(), product_id);
    }

    void set_id_vendor(int vendor_id) const {
      libevdev_set_id_vendor(raw(), vendor_id);
    }

    void set_id_bustype(BusType bustype) const {
      libevdev_set_id_bustype(raw(), enum_to_raw(bustype));
    }

    void set_id_version(int version_id) const {
      libevdev_set_id_version(raw(), version_id);
    }

    void enable_property(InputProperty property) const {
      if (const auto err = libevdev_enable_property(raw(), enum_to_raw(property)))
        throw Exception(err);
    }

    template <typename E>
    void set_event_value(typename E::Code code, int value) const {
      if (const auto err = libevdev_set_event_value(raw(), E::type, enum_to_raw(code), value))
        throw Exception(err);
    }

    template <typename Code>
    void set_slot_value(int slot, Code code, int value) const {
      if (const auto err = libevdev_set_slot_value(raw(), slot, enum_to_raw(code), value))
        throw Exception(err);
    }

    void set_abs_minimum(event::AbsoluteCode code, int minimum) const {
      libevdev_set_abs_minimum(raw(), enum_to_raw(code), minimum);
    }

    void set_abs_maximum(event::AbsoluteCode code, int maximum) const {
      libevdev_set_abs_maximum(raw(), enum_to_raw(code), maximum);
    }

    void set_abs_fuzz(event::AbsoluteCode code, int fuzz) const {
      libevdev_set_abs_fuzz(raw(), enum_to_raw(code), fuzz);
    }

    void set_abs_flat(event::AbsoluteCode code, int flat) const {
      libevdev_set_abs_flat(raw(), enum_to_raw(code), flat);
    }

    void set_abs_resolution(event::AbsoluteCode code, int resolution) const {
      libevdev_set_abs_resolution(raw(), enum_to_raw(code), resolution);
    }

    void set_abs_info(event::AbsoluteCode code, AbsoluteInfo info) const {
      const auto info_raw = info.to_raw();
      libevdev_set_abs_info(raw(), enum_to_raw(code), &info_raw);
    }

    template <typename E>
    void enable_event_type() const {
      if (const auto err = libevdev_enable_event_type(raw(), enum_to_raw(E::type)))
        throw Exception(err);
    }

    template <typename E>
    void disable_event_type() const {
      if (const auto err = libevdev_disable_event_type(raw(), enum_to_raw(E::type)))
        throw Exception(err);
    }

    void enable_event_code(event::AbsoluteCode code, AbsoluteInfo info) const {
      const auto info_raw = info.to_raw();
      if (const auto err = libevdev_enable_event_code(
          raw(), event::Absolute::type, enum_to_raw(code), (void*)&info_raw))
        throw Exception(err);
    }

    void enable_event_code(event::RepeatCode code, int axis_data) const {
      if (const auto err = libevdev_enable_event_code(
          raw(), event::Repeat::type, enum_to_raw(code), (void*)&axis_data))
        throw Exception(err);
    }

    template <typename E>
    void enable_event_code(typename E::Code code) const {
      if (const auto err = libevdev_enable_event_code(raw(), enum_to_raw(code), nullptr))
        throw Exception(err);
    }

    template <typename E>
    void disable_event_code(typename E::Code code) const {
      if (const auto err = libevdev_enable_event_code(raw(), enum_to_raw(code)))
        throw Exception(err);
    }

    void kernel_set_abs_info(event::AbsoluteCode code, AbsoluteInfo info) const {
      const auto info_raw = info.to_raw();
      if (const auto err = libevdev_kernel_set_abs_info(raw(), enum_to_raw(code), &info_raw))
        throw Exception(err);
    }

    void kernel_set_led_value(event::LedCode code, event::LedValue value) const {
      if (const auto err = libevdev_kernel_set_led_value(raw(), enum_to_raw(code), (libevdev_led_value)enum_to_raw(value)))
        throw Exception(err);
    }

    template <typename... Args>
    void kernel_set_led_values(Args... args) const {
      const auto tuple = std::tuple_cat(
          std::make_tuple(raw()),
          Evdev::convert_led_code_value(args...),
          std::make_tuple(-1));
      util::call<int>(libevdev_kernel_set_led_values, tuple);
    }

    void set_clock_id(ClockId clock_id) const {
      if (const auto err = libevdev_set_clock_id(raw(), enum_to_raw(clock_id)))
        throw Exception(err);
    }

    std::optional<std::pair<ReadStatus, event::AnyEvent>> next_event(std::unordered_set<ReadFlag> flags) const {
      struct input_event raw_event;
      int raw_flags = 0;

      for (const auto &flag : flags)
        raw_flags |= enum_to_raw(flag);

      const auto ret = libevdev_next_event(raw(), raw_flags, &raw_event);

      if (ret == -EAGAIN) {
        return std::nullopt;
      } else if (ret < 0) {
        throw Exception(-ret);
      }

      const auto status = (ret == LIBEVDEV_READ_STATUS_SYNC)
          ? ReadStatus::Sync
          : ReadStatus::Success;

      switch (raw_event.type) {
        case EV_SYN:
          return std::make_pair(status, event::Synchronize(raw_event));
        case EV_KEY:
          return std::make_pair(status, event::Key(raw_event));
        case EV_REL:
          return std::make_pair(status, event::Relative(raw_event));
        case EV_ABS:
          return std::make_pair(status, event::Absolute(raw_event));
        case EV_MSC:
          return std::make_pair(status, event::Misc(raw_event));
        case EV_SW:
          return std::make_pair(status, event::Switch(raw_event));
        case EV_LED:
          return std::make_pair(status, event::Led(raw_event));
        case EV_SND:
          return std::make_pair(status, event::Sound(raw_event));
        case EV_REP:
          return std::make_pair(status, event::Repeat(raw_event));
        case EV_FF:
          return std::make_pair(status, event::ForceFeedback(raw_event));
        case EV_PWR:
          return std::make_pair(status, event::Power(raw_event));
        case EV_FF_STATUS:
          return std::make_pair(status, event::ForceFeedbackStatus(raw_event));
        default:
          throw std::runtime_error("Invalid event type!");
      }
    }

    bool has_event_pending() {
      const auto ret = libevdev_has_event_pending(raw());
      if (ret < 0)
        throw Exception(-ret);
      else
        return ret;
    }

  private:
    Evdev(ResourceRawPtr evdev)
      : _evdev(evdev, &libevdev_free)
    {
    }

    static void device_log_fn_helper(const struct libevdev *dev, enum libevdev_log_priority priority, void *data,
        const char *file, int line, const char *func, const char *format, va_list args)
    {
      auto self = static_cast<Evdev*>(data);

      if (!self->_device_log_fn_data)
        return;

      char *message_cstr;
      vasprintf(&message_cstr, format, args);
      std::string message(message_cstr);
      free(message_cstr);

      self->_device_log_fn_data->device_log_fn(*self, raw_to_enum<LogPriority>(priority),
          self->_device_log_fn_data->data, file, line, func, message);
    }

    static std::tuple<> convert_led_code_value() {
      return std::make_tuple();
    }

    template <typename... Tail>
    static auto convert_led_code_value(event::LedCode head_code, event::LedValue head_value, Tail... tail) {
      return std::tuple_cat(std::make_tuple(
          enum_to_raw(head_code),
          (libevdev_led_value)enum_to_raw(head_value)),
          Evdev::convert_led_code_value(tail...));
    }

  private:
    ResourcePtr _evdev;
    std::optional<DeviceLogFnData> _device_log_fn_data;
  };

}

#endif //EVDEVW_EVDEV_HPP
