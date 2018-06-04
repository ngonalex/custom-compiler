#ifndef ABSTRACT_SYNTAX_PRINT_VISITOR_V2_H_
#define ABSTRACT_SYNTAX_PRINT_VISITOR_V2_H_

#include <sstream>
#include <string>

#include "abstract_syntax/abstract_syntax_tree_v2.h"

namespace cs160 {
namespace abstract_syntax {
namespace version_2 {

class PrintVisitor : public AstVisitor {
 public:
  PrintVisitor() {}
  ~PrintVisitor() {}

  const std::string GetOutput() const { return output_.str(); }

  void VisitIntegerExpr(const IntegerExpr& exp) override {
    output_ << exp.value();
  }

  void VisitVariableExpr(const VariableExpr& exp) override {
    output_ << exp.name();
  }

  void VisitAddExpr(const AddExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " + ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitSubtractExpr(const SubtractExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " - ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " * ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitDivideExpr(const DivideExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " / ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitAssignment(const Assignment& assignment) override {
    assignment.lhs().Visit(this);
    output_ << " = ";
    assignment.rhs().Visit(this);
  }

  void VisitProgram(const Program& program) override {
    for (auto& assignment : program.assignments()) {
      assignment->Visit(this);
      output_ << "; ";
    }
    program.arithmetic_exp().Visit(this);
  }

 private:
  std::stringstream output_;
};

}  // namespace version_2
}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_PRINT_VISITOR_V2_H_
