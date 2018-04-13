#ifndef SMART_POINTER_PRIMER_SIMPLE_EXAMPLE_H_
#define SMART_POINTER_PRIMER_SIMPLE_EXAMPLE_H_

#include "utility/memory.h"

// This is just an example class.
class Item {};

// This is a class that should own a member of type Item.
class Box {
 public:
  explicit Box(std::unique_ptr<Item> m) : member_(std::move(m)) {}

 private:
  std::unique_ptr<Item> member_;
};

#endif  // SMART_POINTER_PRIMER_SIMPLE_EXAMPLE_H_
