#include <iostream>
#include "rcc/rcc.h"

int main() {
  std::cout
    << "RCC magic number: "
    << cs160::rcc::GetRCCMagicNumber()
    << std::endl;

  return 0;
}
