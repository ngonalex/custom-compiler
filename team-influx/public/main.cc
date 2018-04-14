#include <iostream>

#include "abstract_syntax/abstract_syntax.h"
#include "rcc/rcc.h"

int main() {
  std::cout << "RCC magic number: " << cs160::rcc::GetRCCMagicNumber()
            << std::endl;

  return 0;
}
