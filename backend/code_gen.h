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
    void Generate(std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks);
    void GenerateEpilogue();
    void ClearRegister(std::string reg);
    void GenerateBoiler();

    // REMOVE THIS LATER
    void GenerateDumbTest();
    // Have used this "helper twice now think about making it apart of a class later"
    std::vector<std::string> printhelper = {"load", "+", "-", "*", "/"};
 private:
  std::ofstream& outfile_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_CODE_GEN_H_
