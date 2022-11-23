#ifndef EVDEVW_EVENT_HPP
#define EVDEVW_EVENT_HPP

#include <libevdev/libevdev.h>

#include "../Enum.hpp"
#include "../Utility.hpp"

#define DECLARE_EVENT_TYPE(_raw_type, _type, _code_type, _value_type)   \
  namespace event {                                                     \
    struct _type : public Event<_raw_type, _code_type, _value_type> {   \
      _type(_code_type code, _value_type value)                         \
          : Event<_raw_type, _code_type, _value_type>(code, value)      \
      {                                                                 \
      }                                                                 \
      _type(struct input_event event)                                   \
          : Event<_raw_type, _code_type, _value_type>(event)            \
      {                                                                 \
      }                                                                 \
    };                                                                  \
    template <>                                                         \
    struct event_from_event_code<_code_type> {                          \
      using type = _type;                                               \
    };                                                                  \
  }

namespace evdevw::event {

  template <typename Code>
  struct event_from_event_code {
  };

  template <uint16_t _type, typename _Code, typename _Value>
  struct Event {
  public:
    using Code = _Code;
    using Value = _Value;

    static const int type = _type;

  public:
    static Value raw_to_value(int raw) {
      if constexpr (std::is_enum<Value>::value)
        return raw_to_enum<Value>(raw);
      else
        return raw;
    }

    static int value_to_raw(Value value) {
      if constexpr (std::is_enum<Value>::value)
        return enum_to_raw<Value>(value);
      else
        return value;
    }
    Event(Code code, Value value)
      : _code(code), _value(value)
    {
    }

    Event(struct input_event event)
        : _code(raw_to_enum<Code>(event.code)), _value(raw_to_value(event.value))
    {
      if (event.type != type)
        throw std::runtime_error(
            std::string("Raw type mismatch for event type ") +
            typeid(decltype(*this)).name() + ": got " + std::to_string(event.type) +
            ", expected " + std::to_string(type));
    }

    Event(uint16_t raw_code, Value value)
      : _code(raw_to_enum<Code>(raw_code)), _value(raw_to_value(value))
    {
    }

    bool operator==(const Event &other) const {
      return is_code(other._code);
    }

    bool is_code(Code code) const {
      return code == _code;
    }

    uint16_t get_raw_code() const {
      return enum_to_raw<Code>(_code);
    }

    int get_raw_value() const {
      return value_to_raw(_value);
    }

    Code get_code() const {
      return _code;
    }

    Value get_value() const {
      return _value;
    }

    std::string get_type_name() const {
      return libevdev_event_type_get_name(type);
    }
    std::string get_code_name() const {
      return libevdev_event_code_get_name(type, enum_to_raw(_code));
    }

  private:
    Code _code;
    Value _value;
  };

  template <typename E>
  std::string get_type_name() {
    return libevdev_event_type_get_name(E::type);
  }

  template <typename E>
  std::string get_code_name(typename E::Code code) {
    return libevdev_event_code_get_name(E::type, enum_to_raw(code));
  }

}

#endif //EVDEVW_EVENT_HPP
