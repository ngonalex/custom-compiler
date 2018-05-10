#ifndef BACKEND_CODE_GEN_H_
#define BACKEND_CODE_GEN_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <set>

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

    // Printing functions
    void GeneratePrinter();
    void GeneratePrintHeader();
    void GenerateAssignment(std::string);
    void GenerateResult();
    void GenerateData(std::set<std::string>);
    // Different nodes + helpers
    void GenerateLoadInstructions(std::unique_ptr<ThreeAddressCode> tac);
    void GenerateArithmeticExpr(std::unique_ptr<ThreeAddressCode> tac,
      Type type);
    void GenerateBinaryExprHelper(std::unique_ptr<ThreeAddressCode> tac);
    void GenerateRelationalExpr(std::unique_ptr<ThreeAddressCode> tac,
      Type type);
    void GenerateLogicalExpr(std::unique_ptr<ThreeAddressCode> tac,
      Type type);

 private:
  std::ofstream& outfile_;
  int printercount_;
  std::set<std::string> assignmentset_;
  Scope currscope_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_CODE_GEN_H_
