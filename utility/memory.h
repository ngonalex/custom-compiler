
#ifndef UTILITY_MEMORY_H_
#define UTILITY_MEMORY_H_

namespace cs160 {

// this is missing from the C++11 library, so we implement it here
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}  // cs160

#endif  // UTILITY_MEMORY_H_
