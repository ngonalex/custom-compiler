#ifndef ABSTRACT_SYNTAX_PRINT_VISITOR_V2_H_
#define ABSTRACT_SYNTAX_PRINT_VISITOR_V2_H_

#include <sstream>
#include <string>

#include "abstract_syntax/abstract_syntax_tree_v2.h"

namespace cs160 {
namespace abstract_syntax {
<<<<<<< HEAD
namespace version_2 {
=======
namespace version_1 {
>>>>>>> master

class PrintVisitor : public AstVisitor {
 public:
  PrintVisitor() {}
  ~PrintVisitor() {}

  const std::string GetOutput() const { return output_.str(); }

  void VisitIntegerExpr(const IntegerExpr& exp) override {
    output_ << exp.value();
  }
<<<<<<< HEAD
  
  void VisitVariableExpr(const VariableExpr& exp) override { 
    output_ << exp.name(); 
  }

  void VisitAddExpr(const AddExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " + ";
=======

  void VisitAddExpr(const AddExpr& exp) override {
    output_ << "(+ ";
    exp.lhs().Visit(this);
    output_ << " ";
>>>>>>> master
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitSubtractExpr(const SubtractExpr& exp) override {
<<<<<<< HEAD
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " - ";
=======
    output_ << "(- ";
    exp.lhs().Visit(this);
    output_ << " ";
>>>>>>> master
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) override {
<<<<<<< HEAD
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " * ";
=======
    output_ << "(* ";
    exp.lhs().Visit(this);
    output_ << " ";
>>>>>>> master
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitDivideExpr(const DivideExpr& exp) override {
<<<<<<< HEAD
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " / ";
=======
    output_ << "(/ ";
    exp.lhs().Visit(this);
    output_ << " ";
>>>>>>> master
    exp.rhs().Visit(this);
    output_ << ")";
  }

<<<<<<< HEAD
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

=======
>>>>>>> master
 private:
  std::stringstream output_;
};

<<<<<<< HEAD
}  // namespace version_2
}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_PRINT_VISITOR_V2_H_
=======
}  // namespace version_1
}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_PRINT_VISITOR_V1_H_
>>>>>>> master
