#ifndef BACKEND_EXCEPTIONS_H_
#define BACKEND_EXCEPTIONS_H_

#include <iostream>
#include <exception>

namespace cs160 {
namespace backend {

class DivisorIsZeroException : public std::exception {
 public:
  const char * PrintTrace() const throw() {
    return "Attempted to divide by zero!\n";
  }
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_EXCEPTIONS_H_
