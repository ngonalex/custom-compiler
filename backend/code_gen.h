#ifndef BACKEND_CODE_GEN_H_
#define BACKEND_CODE_GEN_H_

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <string>
#include <utility>

#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

class CodeGen {
 public:
    explicit CodeGen(std::ofstream &filename) : outfile_(filename),
      currscope_(GLOBAL) {}
    CodeGen(std::ofstream &filename, PrintFlag flag) : outfile_(filename),
      currscope_(GLOBAL), flag_(flag) {}

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
    void TestPrint(std::string label);
    // Different nodes + helpers
    void GenerateLoadInstructions(std::unique_ptr<ThreeAddressCode> tac);
    void GenerateArithmeticExpr(std::unique_ptr<ThreeAddressCode> tac,
                                OpcodeType type);
    void GenerateBinaryExprHelper(std::unique_ptr<ThreeAddressCode> tac);
    void GenerateRelationalExpr(std::unique_ptr<ThreeAddressCode> tac,
                                OpcodeType type);
    void GenerateLogicalExpr(std::unique_ptr<ThreeAddressCode> tac,
                             OpcodeType type);

    void GeneratePrintTuple();
    void GeneratePrintIntTuple();
    void GeneratePrintNestedTuple();
    void GeneratePrintUnassignedTuple();
    void GenerateDivisionByZeroCheck();
    void GenerateTupleFlagCheck();
    void GenerateIntegerFlagCheck();
    void GenerateExistenceCheck();
    void GenerateTupleSizeCheck();
    void GenerateTupleCreationSizeCheck();
    void GenerateCreateNewTuple();
    void GenerateNestedDeref();
    void GenerateBaseDeref(std::string variablename);
    void GenerateRHSDerefEpilogue(std::string arg2);
    std::string FlagHelper();

    // This function checks if the current scope is function or global
    // If it's global then it just returns the name of the variable
    // otherwise it checks the map for the function and returns the correct
    // offset inside the stacks
    std::string VariableNameHelper(std::string variablename, FlagType flag);

    std::vector<std::string> DereferenceParserHelper(std::string variablename);

 private:
  std::ofstream& outfile_;
  int printercount_;
  std::set<std::string> assignmentset_;
  Scope currscope_;
  std::map<std::string, int> symbollocations_;
  PrintFlag flag_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_CODE_GEN_H_
