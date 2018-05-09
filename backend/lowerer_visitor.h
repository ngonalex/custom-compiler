#ifndef BACKEND_LOWERER_VISITOR_H_
#define BACKEND_LOWERER_VISITOR_H_

#include <string>
#include <vector>
#include <stack>
#include <set>
#include <iostream>

#include "abstract_syntax/abstract_syntax.h"
#include "backend/ir.h"
#include "backend/helper_struct.h"
#include "utility/memory.h"
#include "utility/assert.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::Assignment;
using cs160::abstract_syntax::backend::Program;
using cs160::abstract_syntax::backend::VariableExpr;
using cs160::abstract_syntax::backend::LessThanExpr;
using cs160::abstract_syntax::backend::LessThanEqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanExpr;
using cs160::abstract_syntax::backend::GreaterThanEqualToExpr;
using cs160::abstract_syntax::backend::EqualToExpr;
using cs160::abstract_syntax::backend::LogicalAndExpr;
using cs160::abstract_syntax::backend::LogicalBinaryOperator;
using cs160::abstract_syntax::backend::LogicalNotExpr;
using cs160::abstract_syntax::backend::LogicalOrExpr;
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::FunctionCall;
using cs160::abstract_syntax::backend::FunctionDef;
using cs160::make_unique;



namespace cs160 {
namespace backend {

enum ChildType {
  INTCHILD,
  VARCHILD,
  NOCHILD
};

class LowererVisitor : public AstVisitor {
 public:
  LowererVisitor() : counter_() {}
  ~LowererVisitor() {}

  std::string GetOutput();

  // V4 (Program Updated)
  void VisitFunctionCall(const FunctionCall& call);
  void VisitFunctionDef(const FunctionDef& def);

  // V3 (Assignment + Program updated)
  void VisitLessThanExpr(const LessThanExpr& exp);
  void VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp);
  void VisitGreaterThanExpr(const GreaterThanExpr& exp);
  void VisitGreaterThanEqualToExpr(
      const GreaterThanEqualToExpr& exp);
  void VisitEqualToExpr(const EqualToExpr& exp);
  void VisitLogicalAndExpr(const LogicalAndExpr& exp);
  void VisitLogicalOrExpr(const LogicalOrExpr& exp);
  void VisitLogicalNotExpr(const LogicalNotExpr& exp);
  void VisitConditional(const Conditional& conditional);
  void VisitLoop(const Loop& loop);

  // V2
  void VisitAssignment(const Assignment& assignment);
  void VisitProgram(const Program& program);
  void VisitVariableExpr(const VariableExpr& exp);

  // V1
  void VisitIntegerExpr(const IntegerExpr& exp);
  void VisitAddExpr(const AddExpr& exp);
  void VisitSubtractExpr(const SubtractExpr& exp);
  void VisitMultiplyExpr(const MultiplyExpr& exp);
  void VisitDivideExpr(const DivideExpr& exp);

  void CreateComparisionBlock(Type type);
  void CreateLabelBlock(std::string labelname);
  void CreateJumpBlock(std::string jumpname, Type type);

  // Helpers
  std::string GetOutputArithmeticHelper(std::string output, int index,
    std::vector<std::string> printhelper);
  void BinaryOperatorHelper(Type type, Register arg1, Register arg2);
  std::string JumpLabelHelper();
  std::string ContinueLabelHelper();
  std::string LoopLabelHelper();
  // bool CheckVarFlag() {return variableflag_;}
  // void ClearVarFlag() {variableflag_ = false;}
  Register GetArgument(ChildType type);

  std::vector<std::unique_ptr<ThreeAddressCode>> GetIR() {
    return std::move(blocks_);
  }

  std::stack<std::string> variablestack() {
    return variablestack_;
  }

  std::vector<std::set<std::string>> localsets() {
    return localsets_;
  }

  std::set<std::string> globalset() {
    return globalset_;
  }

 private:
  std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks_;
  std::stack<std::string> variablestack_;
  std::vector<std::set<std::string>> localsets_;
  std::set<std::string> globalset_;
  ChildType lastchildtype_;
  struct Counter counter_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_LOWERER_VISITOR_H_
