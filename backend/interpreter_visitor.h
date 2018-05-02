#ifndef BACKEND_INTERPRETER_VISITOR_H_
#define BACKEND_INTERPRETER_VISITOR_H_

#include <stdio.h>
#include <map>
#include <stack>
#include <iostream>
#include <utility>
#include <string>

#include "abstract_syntax/abstract_syntax.h"
#include "backend/interpreter_stack.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
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

namespace cs160 {
namespace backend {

class InterpreterVisitor : public AstVisitor {
 public:
  InterpreterVisitor() {}
  ~InterpreterVisitor() {}

  // Should we check if the top of the stack is 1 here?
  // Assert (one item in stack)
  const int GetOutput() {
    int result = opstack_.top();
    opstack_.pop();
    return result;
  }

  const int GetVariable(std::string variable) const {
    return variablemap_.find(variable)->second;
  }

  const bool GetBoolOutput(){
    bool result = conditionalstack_.top();
    conditionalstack_.pop();
    return result;
  }

  // V3 (Assignment + Program updated) Fill 
  void VisitLessThanExpr(const LessThanExpr& exp) {
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));



    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    bool result = l < r ;
    conditionalstack_.push(result);

  }

  void VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    bool result = l <= r ;
    conditionalstack_.push(result);
  }

  void VisitGreaterThanExpr(const GreaterThanExpr& exp) {
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    bool result = l > r ;
    conditionalstack_.push(result);
  }

  void VisitGreaterThanEqualToExpr(const GreaterThanEqualToExpr& exp) {
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    bool result = l >= r ;
    conditionalstack_.push(result);
  }

  void VisitEqualToExpr(const EqualToExpr& exp) {
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    bool result = l == r ;
    conditionalstack_.push(result);
  }

  void VisitLogicalAndExpr(const LogicalAndExpr& exp) {
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    bool r = conditionalstack_.top();
    conditionalstack_.pop();
    int l = conditionalstack_.top();
    conditionalstack_.pop();
    bool result = l & r ;
    conditionalstack_.push(result);
  }

  void VisitLogicalOrExpr(const LogicalOrExpr& exp) {
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    bool r = conditionalstack_.top();
    conditionalstack_.pop();
    int l = conditionalstack_.top();
    conditionalstack_.pop();
    bool result = l | r ;
    conditionalstack_.push(result);
  }

  void VisitLogicalNotExpr(const LogicalNotExpr& exp) {
    // bool result = conditionalstack_.top();
    // conditionalstack_.pop();
    // result = !result;
    // conditionalstack_.push(result);
  }

  void VisitConditional(const Conditional& conditional) {
    conditional.guard()

  

  }
  void VisitLoop(const Loop& loop) {}

  // these should be able to change members of the visitor, thus not const
  void VisitIntegerExpr(const IntegerExpr& exp) {
    // push value to stack
    opstack_.push(exp.value());
  }

  void VisitAddExpr(const AddExpr& exp) {
    // Visit left then right (eventually reaches the base case of Int
    // and that will push two values on the stack)
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    // Pop the top two (left,right), add them, and push it back on the stack
    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    opstack_.push(l+r);
  }

  void VisitSubtractExpr(const SubtractExpr& exp) {
    // Visit left then right(eventually reaches the base case of Int)
    // and that will push the two values on the stack
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    // Pop (left,right), subtract them, and push it back onto the stack
    // int r = opstack_.top();
    // opstack_.pop();
    // int l = opstack_.top();
    // opstack_.pop();
    // opstack_.push(l-r);
    auto value = stack_.top();



  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) {
    // Visit lhs,rhs
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    // Pop top two, push result back in
    unsigned int r = opstack_.top();
    opstack_.pop();
    unsigned int l = opstack_.top();
    opstack_.pop();
    opstack_.push(l*r);
  }

  void VisitDivideExpr(const DivideExpr& exp) {
    // Visit left/right
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    // Pop top two, push result back in
    int r = opstack_.top();

    // Check if divisor is zero
    if (r == 0) {
      perror("Dividing zero");
      exit(1);
    }

    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    opstack_.push(l/r);
  }

  void VisitVariableExpr(const VariableExpr& exp) {
    variablestack_.push(exp.name());
  }

  void VisitAssignment(const Assignment& assignment) {
    // Visit left/right
    assignment.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    assignment.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    std::string assignee = variablestack_.top();
    variablestack_.pop();

    int assigner = opstack_.top();
    opstack_.top();

    variablemap_.insert(std::pair<std::string, int>(assignee, assigner));
  }

  void VisitProgram(const Program& program) {
    for (auto& assignment : program.statements()) {
      assignment->Visit(this);
    }

    program.arithmetic_exp().Visit(this);
  }

 private:
  // Not very general, this is probably a bad idea for future ASTS
  std::stack<int> opstack_;
  std::stack<std::string> variablestack_;
  std::stack<bool> conditionalstack_;
  std::map<std::string, int> variablemap_;

  std::stack<StackValue> stack_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_INTERPRETER_VISITOR_H_
