#ifndef BACKEND_INTERPRETER_VISITOR_H_
#define BACKEND_INTERPRETER_VISITOR_H_

#include <stack>
#include <iostream>

#include "abstract_syntax/abstract_syntax.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::BinaryOperatorExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::Assignment;
using cs160::abstract_syntax::backend::Program;
using cs160::abstract_syntax::backend::VariableExpr;

namespace cs160 {
namespace backend {

class InterpreterVisitor : public AstVisitor {
 public:
  InterpreterVisitor() {}
  ~InterpreterVisitor() {}

  // Should we check if the top of the stack is 1 here?
  // Assert (one item in stack)
  const int GetOutput() const { return opstack_.top(); }

  // Fill me in
  void VisitAssignment(const Assignment& assignment){};
  void VisitProgram(const Program& program) {};
  void VisitVariableExpr(const VariableExpr& exp) {};

  // these should be able to change members of the visitor, thus not const
  void VisitIntegerExpr(const IntegerExpr& exp) {
    // push value to stack
    opstack_.push(exp.value());
  }

  void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp) {
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
    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    opstack_.push(l-r);
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) {
    // Visit lhs,rhs
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    // Pop top two, push result back in
    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    std::cout<<"l is: "<<l<<std::endl;
    std::cout<<"r is: "<<r<<std::endl;
    int mul = r*l;
    std::cout<<"printing before pushing"<<mul<<std::endl;
    opstack_.push(l*r);
  }

  void VisitDivideExpr(const DivideExpr& exp) {
    // Visit left/right
    exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
    exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

    // Pop top two, push result back in
    // Should we check for division by zero here?
    int r = opstack_.top();
    opstack_.pop();
    int l = opstack_.top();
    opstack_.pop();
    opstack_.push(l/r);
  }

 private:
  // Not very general, this is probably a bad idea for future ASTS
  std::stack<int> opstack_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_INTERPRETER_VISITOR_H_