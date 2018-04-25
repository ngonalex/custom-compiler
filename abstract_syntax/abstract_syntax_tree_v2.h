// This version of the abstract syntax has sequences of assignments, where for
// each assignment the left-hand side is a variable and the right-hand side is
// an arithmetic expression (which may contain variables itself). The grammar
// for the abstract syntax is:
//
// n ∈ Integer
// v ∈ Variable
//
// ae ∈ ArithmeticExp ⟵ n | v | ae1 op ae2
// op ∈ BinaryOperator ⟵ + | - | ⨯ | ÷
//
// assign ∈ Assignment ⟵ v := ae
//
// prog ∈ Program ⟵ assign... ae
//
// where ... means 'repeating', i.e., a program is a (possibly empty)
// sequence of assignments followed by an arithmetic expression.

#ifndef ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_V2_H_
#define ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_V2_H_

#include <string>
#include <vector>

#include "utility/memory.h"

namespace cs160 {
namespace abstract_syntax {
namespace version_2 {

// Forward declarations of abstract syntax tree node types for use in the
// abstract syntax tree visitor.
class IntegerExpr;
class VariableExpr;
class AddExpr;
class SubtractExpr;
class MultiplyExpr;
class DivideExpr;
class Assignment;
class Program;

// The visitor abstract base class for visiting abstract syntax trees.
class AstVisitor {
 public:
  virtual ~AstVisitor() {}

  // These should be able to change members of the visitor, thus not const
  virtual void VisitIntegerExpr(const IntegerExpr& exp) = 0;
  virtual void VisitVariableExpr(const VariableExpr& exp) = 0;
  virtual void VisitAddExpr(const AddExpr& exp) = 0;
  virtual void VisitSubtractExpr(const SubtractExpr& exp) = 0;
  virtual void VisitMultiplyExpr(const MultiplyExpr& exp) = 0;
  virtual void VisitDivideExpr(const DivideExpr& exp) = 0;
  virtual void VisitAssignment(const Assignment& assignment) = 0;
  virtual void VisitProgram(const Program& program) = 0;
};

// The definition of the abstract syntax tree abstract base class.
class AstNode {
 public:
  virtual ~AstNode() {}

  virtual void Visit(AstVisitor* visitor) const = 0;
};

// This is the abstract base class from which all arithmetic expressions will
// inherit (integers, variables, and binary operations).
class ArithmeticExpr : public AstNode {};

// An integer constant expression.
class IntegerExpr : public ArithmeticExpr {
 public:
  explicit IntegerExpr(int value) : value_(value) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitIntegerExpr(*this); }

  int value() const { return value_; }

 private:
  // The value of the integer constant.
  int value_;
};

// A program variable expression.
class VariableExpr : public ArithmeticExpr {
 public:
  explicit VariableExpr(const std::string& name) : name_(name) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitVariableExpr(*this); }

  const std::string& name() const { return name_; }

 private:
  // The name of the variable.
  std::string name_;
};

// An abstract binary operator node.
class BinaryOperatorExpr : public ArithmeticExpr {
 public:
  BinaryOperatorExpr(std::unique_ptr<const ArithmeticExpr> lhs,
                     std::unique_ptr<const ArithmeticExpr> rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

  const ArithmeticExpr& lhs() const { return *lhs_; }
  const ArithmeticExpr& rhs() const { return *rhs_; }

 protected:
  // The left-hand side and right-hand side of the expression.
  const std::unique_ptr<const ArithmeticExpr> lhs_;
  const std::unique_ptr<const ArithmeticExpr> rhs_;
};

// An addition expression.
class AddExpr : public BinaryOperatorExpr {
 public:
  AddExpr(std::unique_ptr<const ArithmeticExpr> lhs,
          std::unique_ptr<const ArithmeticExpr> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitAddExpr(*this); }
};

// A subtraction expression.
class SubtractExpr : public BinaryOperatorExpr {
 public:
  SubtractExpr(std::unique_ptr<const ArithmeticExpr> lhs,
               std::unique_ptr<const ArithmeticExpr> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitSubtractExpr(*this); }
};

// A multiplication expression.
class MultiplyExpr : public BinaryOperatorExpr {
 public:
  MultiplyExpr(std::unique_ptr<const ArithmeticExpr> lhs,
               std::unique_ptr<const ArithmeticExpr> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitMultiplyExpr(*this); }
};

// A division expression.
class DivideExpr : public BinaryOperatorExpr {
 public:
  DivideExpr(std::unique_ptr<const ArithmeticExpr> lhs,
             std::unique_ptr<const ArithmeticExpr> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitDivideExpr(*this); }
};

// An assignment v := ae.
class Assignment : public AstNode {
 public:
  Assignment(std::unique_ptr<const VariableExpr> lhs,
             std::unique_ptr<const ArithmeticExpr> rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitAssignment(*this); }

  const VariableExpr& lhs() const { return *lhs_; }
  const ArithmeticExpr& rhs() const { return *rhs_; }

 private:
  // The left-hand side and right-hand side of the assignment.
  const std::unique_ptr<const VariableExpr> lhs_;
  const std::unique_ptr<const ArithmeticExpr> rhs_;
};

// A program, consisting of a (possibly empty) sequence of assignments followed
// by an arithmetic expression.
class Program : public AstNode {
 public:
  Program(std::vector<std::unique_ptr<const Assignment>> assignments,
          std::unique_ptr<const ArithmeticExpr> arithmetic_exp)
      : assignments_(std::move(assignments)),
        arithmetic_exp_(std::move(arithmetic_exp)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitProgram(*this); }

  const std::vector<std::unique_ptr<const Assignment>>& assignments() const {
    return assignments_;
  }

  const ArithmeticExpr& arithmetic_exp() const { return *arithmetic_exp_; }

 private:
  std::vector<std::unique_ptr<const Assignment>> assignments_;
  std::unique_ptr<const ArithmeticExpr> arithmetic_exp_;
};

}  // namespace version_2
}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_V2_H_
