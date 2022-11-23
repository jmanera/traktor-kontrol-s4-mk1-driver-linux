#ifndef EVDEVW_UTILITY_HPP
#define EVDEVW_UTILITY_HPP

#include <optional>
#include <string>

namespace evdevw::util {

  using OptionalString = std::optional<std::string>;

  template <typename Ret, typename Raw>
  std::optional<Ret> raw_to_optional(Raw raw) {
    return raw ? std::optional<Ret>(raw) : std::nullopt;
  }

  template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
  template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

  // implementation _impls, users never invoke these directly
  namespace _impl
  {

    template <typename Ret, typename F, typename Tuple, bool Done, int Total, int... N>
    struct call_impl
    {
      static Ret call(F f, Tuple && t)
      {
        return call_impl<Ret, F, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t));
      }
    };

    template <typename Ret, typename F, typename Tuple, int Total, int... N>
    struct call_impl<Ret, F, Tuple, true, Total, N...>
    {
      static Ret call(F f, Tuple && t)
      {
        return f(std::get<N>(std::forward<Tuple>(t))...);
      }
    };
  }

  // user invokes this
  template <typename Ret, typename F, typename Tuple>
  Ret call(F f, Tuple && t)
  {
    typedef typename std::decay<Tuple>::type ttype;
    return _impl::call_impl<Ret, F, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(f, std::forward<Tuple>(t));
  }

}

#endif //EVDEVW_UTILITY_HPP
