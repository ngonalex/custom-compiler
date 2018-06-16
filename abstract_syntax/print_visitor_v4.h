#ifndef ABSTRACT_SYNTAX_PRINT_VISITOR_V4_H_
#define ABSTRACT_SYNTAX_PRINT_VISITOR_V4_H_

#include <sstream>
#include <string>

#include "abstract_syntax/abstract_syntax_tree_v4.h"

namespace cs160 {
namespace abstract_syntax {
namespace version_4 {

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

  void VisitLessThanExpr(const LessThanExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " < ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " <= ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitGreaterThanExpr(const GreaterThanExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " > ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitGreaterThanEqualToExpr(const GreaterThanEqualToExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " >= ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitEqualToExpr(const EqualToExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " == ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitLogicalAndExpr(const LogicalAndExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " && ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitLogicalOrExpr(const LogicalOrExpr& exp) override {
    output_ << "(";
    exp.lhs().Visit(this);
    output_ << " || ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitLogicalNotExpr(const LogicalNotExpr& exp) override {
    output_ << "! (";
    exp.operand().Visit(this);
    output_ << ")";
  }

  void VisitAssignment(const Assignment& assignment) override {
    assignment.lhs().Visit(this);
    output_ << " = ";
    assignment.rhs().Visit(this);
  }

  // Might be incorrect
  void VisitConditional(const Conditional& conditional) override {
    output_ << "if ";
    conditional.guard().Visit(this);
    output_ << " {";
    for (auto& statement : conditional.true_branch()) {
      statement->Visit(this);
    }
    output_ << "} else {";
    for (auto& statement : conditional.false_branch()) {
      statement->Visit(this);
    }
    output_ << "}";
  }

  // Might be incorrect
  void VisitLoop(const Loop& loop) override {
    output_ << "while ";
    loop.guard().Visit(this);
    output_ << " {";
    for (auto& statement : loop.body()) {
      statement->Visit(this);
    }
    output_ << "}";
  }

  void VisitFunctionDef(const FunctionDef& def) override {
    output_ << "func";
    output_ << def.function_name();
    output_ << "(";
    for (auto& params : def.parameters()) {
      params->Visit(this);
      output_ << " ";
    }
    output_ << ")";
    output_ << "{";
    for (auto& statement : def.function_body()) {
      statement->Visit(this);
    }
    output_ << "}";
  }

  void VisitFunctionCall(const FunctionCall& call) override {
    call.lhs().Visit(this);
    output_ << " = ";
    call.callee_name().Visit(this);
    for (auto& args : call.arguments()) {
      args->Visit(this);
    }
  }


  // Might be incorrect
  void VisitProgram(const Program& program) override {
    for (auto& functions : program.function_defs()) {
      functions->Visit(this);
      output_ << "; ";
    }
    for (auto& states : program.statements()) {
      states->Visit(this);
      output_ << "; ";
    }
    program.arithmetic_exp().Visit(this);
  }

 private:
  std::stringstream output_;
};

}  // namespace version_4
}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_PRINT_VISITOR_V4_H_
