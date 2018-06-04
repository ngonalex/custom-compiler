#ifndef UTILITY_MEMORY_H_
#define UTILITY_MEMORY_H_

#include <memory>
#include <algorithm>
#include <cctype>
#include <locale>

namespace cs160 {

#if __cplusplus < 201402L

// this is missing from the C++11 library, so we implement it here
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif

template<class R, class T>
std::unique_ptr<R> unique_cast(std::unique_ptr<T> &&p) {
  T* ptr = p.release();
  return std::unique_ptr<R>(static_cast<R*>(ptr));
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

}  // cs160
#endif  // UTILITY_MEMORY_H_
