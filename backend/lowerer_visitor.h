#ifndef BACKEND_LOWERER_VISITOR_H_
#define BACKEND_LOWERER_VISITOR_H_

#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <utility>

#include "abstract_syntax/abstract_syntax.h"
#include "backend/helper_struct.h"
#include "backend/ir.h"
#include "utility/assert.h"
#include "utility/memory.h"

using cs160::make_unique;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::Assignment;
using cs160::abstract_syntax::backend::Program;
using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::VariableExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::EqualToExpr;
using cs160::abstract_syntax::backend::FunctionCall;
using cs160::abstract_syntax::backend::FunctionDef;
using cs160::abstract_syntax::backend::GreaterThanEqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanExpr;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::LessThanEqualToExpr;
using cs160::abstract_syntax::backend::LessThanExpr;
using cs160::abstract_syntax::backend::LogicalAndExpr;
using cs160::abstract_syntax::backend::LogicalBinaryOperator;
using cs160::abstract_syntax::backend::LogicalNotExpr;
using cs160::abstract_syntax::backend::LogicalOrExpr;
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::FunctionCall;
using cs160::abstract_syntax::backend::FunctionDef;
using cs160::abstract_syntax::backend::Dereference;
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::abstract_syntax::backend::AssignmentFromNewTuple;

namespace cs160 {
namespace backend {

enum ChildType {
  INTCHILD,
  VARCHILD,
  DEREFCHILD,
  BINOPCHILD,
  NOCHILD
};

class LowererVisitor : public AstVisitor {
 public:
  LowererVisitor() : counter_(), currvariabletype_(RIGHT_HAND_VAR),
    currdereferencetype_(RHS_DEREFERENCE) {}

  std::string GetOutput();

  // V5
  void VisitDereference(const Dereference& exp);
  void VisitAssignmentFromArithExp(const AssignmentFromArithExp& assignment);
  void VisitAssignmentFromNewTuple(const AssignmentFromNewTuple& assignment);

  // V4 (Program Updated)
  void VisitFunctionCall(const FunctionCall& call);
  void VisitFunctionDef(const FunctionDef& def);

  // V3 (Assignment + Program updated)
  void VisitLessThanExpr(const LessThanExpr& exp);
  void VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp);
  void VisitGreaterThanExpr(const GreaterThanExpr& exp);
  void VisitGreaterThanEqualToExpr(const GreaterThanEqualToExpr& exp);
  void VisitEqualToExpr(const EqualToExpr& exp);
  void VisitLogicalAndExpr(const LogicalAndExpr& exp);
  void VisitLogicalOrExpr(const LogicalOrExpr& exp);
  void VisitLogicalNotExpr(const LogicalNotExpr& exp);
  void VisitConditional(const Conditional& conditional);
  void VisitLoop(const Loop& loop);

  // V2
  // void VisitAssignment(const Assignment& assignment);
  void VisitProgram(const Program& program);
  void VisitVariableExpr(const VariableExpr& exp);

  // V1
  void VisitIntegerExpr(const IntegerExpr& exp);
  void VisitAddExpr(const AddExpr& exp);
  void VisitSubtractExpr(const SubtractExpr& exp);
  void VisitMultiplyExpr(const MultiplyExpr& exp);
  void VisitDivideExpr(const DivideExpr& exp);

  // Abstracted out opcode creators
  void CreateFunctionDefPrologue(std::string name);
  void CreateFunctionDefEpilogue(std::string name);
  void CreateFunctionDefReturnBlock();
  void CreateFunctionDefSignal(std::string name);
  void CreateFunctionCallBlock(std::string funname);
  void CreateFunctionCallReturnEpilogue(int numofregs);
  void CreateLoadBlock(OpcodeType type, Operand arg1);
  void CreateComparisionBlock(OpcodeType type);
  void CreateLabelBlock(std::string labelname);
  void CreateJumpBlock(std::string jumpname, OpcodeType type);
  void CreateDereference(std::string basevariable, std::string targetvariable,
    int indexofchild);
  void CreateTupleAssignment(std::string target, Operand operand);
  void CreateArithmeticAssignment(std::string target, Operand operand);

  // Helpers
  std::string GetOutputArithmeticHelper(std::string output, int index,
                                        std::vector<std::string> printhelper);
  void BinaryOperatorHelper(OpcodeType type, Register arg1, Register arg2);
  std::string JumpLabelHelper();
  std::string ContinueLabelHelper();
  std::string LoopLabelHelper();
  Register GetArgument(ChildType type);

  // Two functions that help us identify unassigned variables
  std::set<std::string> GetSetDifference(std::set<std::string> set1,
    std::set<std::string> set2);
  std::set<std::string> GetSetIntersection(std::set<std::string> set1,
    std::set<std::string> set2);

  std::vector<std::unique_ptr<ThreeAddressCode>> GetIR() {
    return std::move(blocks_);
  }

  std::stack<std::string> variablestack() { return variablestack_; }
  std::vector<std::set<std::string>> localsets() { return localsets_; }
  std::set<std::string> globalset() { return globalset_; }
  std::set<std::string> totalset() { return totalset_; }

 private:
  std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks_;
  std::stack<std::string> variablestack_;
  std::vector<std::set<std::string>> localsets_;
  std::set<std::string> totalset_;
  std::set<std::string> globalset_;
  std::map<std::string, int> functioncheck_;
  ChildType lastchildtype_;
  struct Counter counter_;
  VariableType currvariabletype_;
  OpcodeType currdereferencetype_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_LOWERER_VISITOR_H_
