#ifndef EVDEVW_TYPE_HPP
#define EVDEVW_TYPE_HPP

namespace evdevw {

  template <typename Type>
  int type_to_raw(Type raw);

  template <typename Type>
  Type raw_to_type(int raw);

}

#endif //EVDEVW_TYPE_HPP
