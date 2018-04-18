#ifndef BACKEND_CODE_GEN_H_
#define BACKEND_CODE_GEN_H_

#include <iostream>
#include <fstream>  
#include <string>
#include <vector>

#include "lowerer_visitor.h"

namespace cs160 {
namespace backend {


class CodeGen {

 public:
    explicit CodeGen(std::ofstream &filename) : outfile_(filename) {} 
    void Generate(std::vector<struct ThreeAddressCode*> blocks);
 private:
  std::ofstream& outfile_;
};

}  // namespace CS160
}  // namespace backend

#endif  // BACKEND_CODE_GEN_H_