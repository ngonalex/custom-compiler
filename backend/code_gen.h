#ifndef BACKEND_CODE_GEN_H_
#define BACKEND_CODE_GEN_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <set>
#include <sstream>

#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

class CodeGen {
 public:
    explicit CodeGen(std::ofstream &filename) : outfile_(filename),
      currscope_(GLOBAL) {}
    void Generate(std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks);
    void GenerateEpilogue();
    void ClearRegister(std::string reg);
    void GenerateBoiler();

    // Printing functions
    void GeneratePrinter();
    void GeneratePrintCall(std::string label);
    void GeneratePrintAssignment(std::string);
    void GeneratePrintFunctionResult();
    void GeneratePrintResult();
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

    void GenerateTupleFlagCheck();
    void GenerateIntegerFlagCheck();
    void GenerateExistenceCheck();
    void GenerateTupleSizeCheck();
    void GenerateUnboxer();
    void GenerateBoxer();
    void GenerateCreateNewTuple();

    // This function checks if the current scope is function or global
    // If it's global then it just returns the name of the variable
    // otherwise it checks the map for the function and returns the correct
    // offset inside the stacks
    std::string VariableNameHelper(std::string variablename);

    std::vector<std::string> DereferenceParserHelper(std::string variablename);

 private:
  std::ofstream& outfile_;
  int printercount_;
  std::set<std::string> assignmentset_;
  Scope currscope_;
  std::map<std::string, int> symbollocations_;
  std::map<std::string, int> symboltypes_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_CODE_GEN_H_
