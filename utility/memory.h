#ifndef UTILITY_MEMORY_H_
#define UTILITY_MEMORY_H_

#include <algorithm>
#include <cctype>
#include <locale>
#include <memory>

namespace cs160 {

#if __cplusplus < 201402L

// this is missing from the C++11 library, so we implement it here
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif

template <class R, class T>
std::unique_ptr<R> unique_cast(std::unique_ptr<T> &&p) {
  T *ptr = p.release();
  return std::unique_ptr<R>(static_cast<R *>(ptr));
}

// trim from start (in place)
static inline int ltrim(std::string &s) {
  int origCount = s.length();
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int ch) { return !std::isspace(ch); }));
  return origCount - s.length();
}

// trim from end (in place)
static inline int rtrim(std::string &s) {
  int origCount = s.length();
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
  return origCount - s.length();
}

// trim from both ends (in place)
static inline int trim(std::string &s) {
  int count = 0;
  count += ltrim(s);
  // count += rtrim(s);
  return count;
}

}  // namespace cs160
#endif  // UTILITY_MEMORY_H_
