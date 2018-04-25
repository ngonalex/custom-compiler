#ifndef UTILITY_ASSERT_H_
#define UTILITY_ASSERT_H_

#include <exception>
#include <iostream>

namespace cs160 {

#ifndef NDEBUG
#define ASSERT(condition, message)                                       \
  do {                                                                   \
    if (!(condition)) {                                                  \
      std::cerr << "Assertion `\" #condition \"` failed in " << __FILE__ \
                << " line " << __LINE__ << ": " << message << std::endl; \
      std::terminate();                                                  \
    }                                                                    \
  } while (false)
#else
#define ASSERT(condition, message) \
  do {                             \
  } while (false)
#endif  // NDEBUG

}  // namespace cs160

#endif  // UTILITY_ASSERT_H_
