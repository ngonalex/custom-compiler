#include "gtest/gtest.h"
#include "smart_pointer_primer/simple_example.h"

// If we were using C++14, we'd #include <memory>; this is our poor-man
// substitute: if you want to use this, don't forget to add the dependency to
// "//utility" to the deps list in the corresponding BUILD file.
#include "utility/memory.h"

// These are the ways in which you can pass smart pointers around:

TEST(SmartPointerPrimerTests, PassUniquePtrByMoving) {
  auto someInstance = std::unique_ptr<Item>(new Item);

  // We have to move the unique_ptr into the member here:
  Box(std::move(someInstance));
}

TEST(SmartPointerPrimerTests, PassingUniquePtrDirectly) {
  // We don't have to move the unique_ptr if we create it inside:
  Box(std::unique_ptr<Item>(new Item));
}

TEST(SmartPointerPrimerTests, PassingUniquePtrUsingMakeUnique) {
  // we can create the unique_ptr by using make_unique as a substitute
  // constructor for the Item class
  Box(cs160::make_unique<Item>());
}
