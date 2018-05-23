#ifndef UTILITY_MEMORY_H_
#define UTILITY_MEMORY_H_

#include <memory>

#if __cplusplus < 201402L

namespace cs160 {

// this is missing from the C++11 library, so we implement it here
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class R, class T>
std::unique_ptr<R> unique_cast(std::unique_ptr<T> &&p) {
  T* ptr = p.release();
  return std::unique_ptr<R>(static_cast<R*>(ptr));
}

}  // cs160

#endif
#endif  // UTILITY_MEMORY_H_
