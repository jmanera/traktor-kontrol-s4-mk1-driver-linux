#ifndef EVDEVW_UINPUT_HPP
#define EVDEVW_UINPUT_HPP

#include <libevdev/libevdev-uinput.h>

#include "Evdev.hpp"
#include "Exception.hpp"
#include "Utility.hpp"

namespace evdevw {

  class UInput {
  public:
    using Resource = struct libevdev_uinput;
    using ResourceRawPtr = Resource*;

    UInput(const Evdev& evdev, int uinput_fd = LIBEVDEV_UINPUT_OPEN_MANAGED)
      : _uinput(nullptr, &libevdev_uinput_destroy)
    {
      Resource *uinput_raw;
      if (const auto err = libevdev_uinput_create_from_device(evdev.raw(), uinput_fd, &uinput_raw))
        throw Exception(-err);
      _uinput.reset(uinput_raw);
    }

    ResourceRawPtr raw() {
      return _uinput.get();
    }

    ResourceRawPtr raw() const {
      return _uinput.get();
    }

    int get_fd() {
      return libevdev_uinput_get_fd(raw());
    }

    util::OptionalString get_syspath() {
      return util::raw_to_optional<std::string>(libevdev_uinput_get_syspath(raw()));
    }

    util::OptionalString get_devnode() {
      return util::raw_to_optional<std::string>(libevdev_uinput_get_devnode(raw()));
    }

    void write_event(event::AnyEvent event_any) {
      std::visit(util::overloaded {
        [this](const auto &event) {
          _write_event(event);
        }
      }, event_any);
    }

  private:
    std::unique_ptr<struct libevdev_uinput, decltype(&libevdev_uinput_destroy)> _uinput;

    template <typename E>
    void _write_event(const E &event) {
      if (const auto err = libevdev_uinput_write_event(raw(), E::type, event.get_raw_code(), event.get_raw_value()))
        throw Exception(-err);
    }
  };

}

#endif //EVDEVW_UINPUT_HPP
