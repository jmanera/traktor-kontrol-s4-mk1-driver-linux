#ifndef EVDEVW_EXCEPTION_HPP
#define EVDEVW_EXCEPTION_HPP

#include <stdexcept>

namespace evdevw {

  class Exception : public std::exception {
  public:
    Exception(int error)
        : _error(error)
    {
    }

    int get_error() const {
      return _error;
    }

  private:
    int _error;
  };

}

#endif //EVDEVW_EXCEPTION_HPP
