#ifndef BACKEND_CODE_GEN_H_
#define BACKEND_CODE_GEN_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

class CodeGen {
 public:
    explicit CodeGen(std::ofstream &filename) : outfile_(filename) {}
    void Generate(std::vector<struct ThreeAddressCode*> blocks);
    void GenerateEpilogue();
    void ClearRegister(std::string reg);
    void GenerateBoiler();
 private:
  std::ofstream& outfile_;
  std::map<std::string, int> memorymap_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_CODE_GEN_H_
