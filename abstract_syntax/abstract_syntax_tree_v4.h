// This version of the abstract syntax adds functions. The grammar for the
// abstract syntax is:
//
// n ∈ Integer
// v ∈ Variable
// fn ∈ FunctionName
//
// ae ∈ ArithmeticExp ⟵ n | v | ae1 aop ae2
// aop ∈ BinaryArithmeticOperator ⟵ + | - | ⨯ | ÷
//
// re ∈ RelationalExp ⟵ ae1 rop ae2 | re1 lop re2 | ¬re
// rop ∈ BinaryRelationalOperator ⟵ < | <= | > | >= | =
// lop ∈ BinaryLogicalOperator ⟵ ∧ | ∨
//
// stmt ∈ Statement ⟵ assign | cond | loop | call
// block ∈ Block ⟵ stmt...
//
// assign ∈ Assignment ⟵ v := ae
// cond ∈ Conditional ⟵ if re block1 block2
// loop ∈ Loop ⟵ while re block
// call ∈ FunctionCall ⟵ v := fn(ae...)
//
// fundef ∈ FunctionDef ⟵ def fn(v...) block ae
//
// prog ∈ Program ⟵ fundef... block ae
//
// where ... means 'repeating', i.e., a block is a (possibly empty) sequence of
// statements, a function call takes a (possibly empty) sequence of arithmetic
// expressions as arguments, and a function definition has a (possibly empty)
// sequence of variables as parameters.
//
// Note that the abstract syntax assumes that all functions return a value (the
// 'ae' at the end of the function body). The concrete syntax doesn't
// necessarily need to make this same assumption (for example, the parser could
// fill in a dummy 'ae' and a fresh variable on the left-hand side of the
// abstract syntax function call for each concrete function call that doesn't
// return a value).

#ifndef ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_V4_H_
#define ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_V4_H_

#include <string>
#include <vector>

#include "utility/memory.h"

namespace cs160 {
namespace abstract_syntax {
namespace version_4 {

// Forward declarations of abstract syntax tree node types for use in the
// abstract syntax tree visitor.
class IntegerExpr;
class VariableExpr;
class AddExpr;
class SubtractExpr;
class MultiplyExpr;
class DivideExpr;
class LessThanExpr;
class LessThanEqualToExpr;
class GreaterThanExpr;
class GreaterThanEqualToExpr;
class EqualToExpr;
class LogicalAndExpr;
class LogicalOrExpr;
class LogicalNotExpr;
class Assignment;
class Conditional;
class Loop;
class FunctionCall;
class FunctionDef;
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
  virtual void VisitLessThanExpr(const LessThanExpr& exp) = 0;
  virtual void VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) = 0;
  virtual void VisitGreaterThanExpr(const GreaterThanExpr& exp) = 0;
  virtual void VisitGreaterThanEqualToExpr(
      const GreaterThanEqualToExpr& exp) = 0;
  virtual void VisitEqualToExpr(const EqualToExpr& exp) = 0;
  virtual void VisitLogicalAndExpr(const LogicalAndExpr& exp) = 0;
  virtual void VisitLogicalOrExpr(const LogicalOrExpr& exp) = 0;
  virtual void VisitLogicalNotExpr(const LogicalNotExpr& exp) = 0;
  virtual void VisitAssignment(const Assignment& assignment) = 0;
  virtual void VisitConditional(const Conditional& conditional) = 0;
  virtual void VisitLoop(const Loop& loop) = 0;
  virtual void VisitFunctionCall(const FunctionCall& call) = 0;
  virtual void VisitFunctionDef(const FunctionDef& def) = 0;
  virtual void VisitProgram(const Program& program) = 0;
};

// The definition of the abstract syntax tree abstract base class.
class AstNode {
 public:
  virtual ~AstNode() {}

  virtual void Visit(AstVisitor* visitor) const = 0;
};

// This is the abstract base class from which all arithmetic expressions will
// inherit (integers, variables, and binary arithmetic operations).
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

// An abstract arithmetic binary operator node.
class ArithmeticBinaryOperatorExpr : public ArithmeticExpr {
 public:
  ArithmeticBinaryOperatorExpr(std::unique_ptr<const ArithmeticExpr> lhs,
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
class AddExpr : public ArithmeticBinaryOperatorExpr {
 public:
  AddExpr(std::unique_ptr<const ArithmeticExpr> lhs,
          std::unique_ptr<const ArithmeticExpr> rhs)
      : ArithmeticBinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitAddExpr(*this); }
};

// A subtraction expression.
class SubtractExpr : public ArithmeticBinaryOperatorExpr {
 public:
  SubtractExpr(std::unique_ptr<const ArithmeticExpr> lhs,
               std::unique_ptr<const ArithmeticExpr> rhs)
      : ArithmeticBinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitSubtractExpr(*this); }
};

// A multiplication expression.
class MultiplyExpr : public ArithmeticBinaryOperatorExpr {
 public:
  MultiplyExpr(std::unique_ptr<const ArithmeticExpr> lhs,
               std::unique_ptr<const ArithmeticExpr> rhs)
      : ArithmeticBinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitMultiplyExpr(*this); }
};

// A division expression.
class DivideExpr : public ArithmeticBinaryOperatorExpr {
 public:
  DivideExpr(std::unique_ptr<const ArithmeticExpr> lhs,
             std::unique_ptr<const ArithmeticExpr> rhs)
      : ArithmeticBinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitDivideExpr(*this); }
};

// This is the abstract base class from which all relational expressions will
// inherit (relational and logical operations).
class RelationalExpr : public AstNode {};

// An abstract relational binary operator node (<, <=, >, >=, =).
class RelationalBinaryOperator : public RelationalExpr {
 public:
  RelationalBinaryOperator(std::unique_ptr<const ArithmeticExpr> lhs,
                           std::unique_ptr<const ArithmeticExpr> rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

  const ArithmeticExpr& lhs() const { return *lhs_; }
  const ArithmeticExpr& rhs() const { return *rhs_; }

 protected:
  // The left-hand side and right-hand side of the expression.
  const std::unique_ptr<const ArithmeticExpr> lhs_;
  const std::unique_ptr<const ArithmeticExpr> rhs_;
};

// A less-than relational expression.
class LessThanExpr : public RelationalBinaryOperator {
 public:
  LessThanExpr(std::unique_ptr<const ArithmeticExpr> lhs,
               std::unique_ptr<const ArithmeticExpr> rhs)
      : RelationalBinaryOperator(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitLessThanExpr(*this); }
};

// A less-than-or-equal-to relational expression.
class LessThanEqualToExpr : public RelationalBinaryOperator {
 public:
  LessThanEqualToExpr(std::unique_ptr<const ArithmeticExpr> lhs,
                      std::unique_ptr<const ArithmeticExpr> rhs)
      : RelationalBinaryOperator(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    visitor->VisitLessThanEqualToExpr(*this);
  }
};

// A greater-than relational expression.
class GreaterThanExpr : public RelationalBinaryOperator {
 public:
  GreaterThanExpr(std::unique_ptr<const ArithmeticExpr> lhs,
                  std::unique_ptr<const ArithmeticExpr> rhs)
      : RelationalBinaryOperator(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    visitor->VisitGreaterThanExpr(*this);
  }
};

// A greater-than-or-equal-to relational expression.
class GreaterThanEqualToExpr : public RelationalBinaryOperator {
 public:
  GreaterThanEqualToExpr(std::unique_ptr<const ArithmeticExpr> lhs,
                         std::unique_ptr<const ArithmeticExpr> rhs)
      : RelationalBinaryOperator(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    visitor->VisitGreaterThanEqualToExpr(*this);
  }
};

// An equal-to relational expression.
class EqualToExpr : public RelationalBinaryOperator {
 public:
  EqualToExpr(std::unique_ptr<const ArithmeticExpr> lhs,
              std::unique_ptr<const ArithmeticExpr> rhs)
      : RelationalBinaryOperator(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitEqualToExpr(*this); }
};

// An abstract logical binary operator node (∧, ∨).
class LogicalBinaryOperator : public RelationalExpr {
 public:
  LogicalBinaryOperator(std::unique_ptr<const RelationalExpr> lhs,
                        std::unique_ptr<const RelationalExpr> rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

  const RelationalExpr& lhs() const { return *lhs_; }
  const RelationalExpr& rhs() const { return *rhs_; }

 protected:
  // The left-hand side and right-hand side of the expression.
  const std::unique_ptr<const RelationalExpr> lhs_;
  const std::unique_ptr<const RelationalExpr> rhs_;
};

// a logical-and expression.
class LogicalAndExpr : public LogicalBinaryOperator {
 public:
  LogicalAndExpr(std::unique_ptr<const RelationalExpr> lhs,
                 std::unique_ptr<const RelationalExpr> rhs)
      : LogicalBinaryOperator(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitLogicalAndExpr(*this); }
};

// a logical-or expression.
class LogicalOrExpr : public LogicalBinaryOperator {
 public:
  LogicalOrExpr(std::unique_ptr<const RelationalExpr> lhs,
                std::unique_ptr<const RelationalExpr> rhs)
      : LogicalBinaryOperator(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitLogicalOrExpr(*this); }
};

// A logical negation expression.
class LogicalNotExpr : public RelationalExpr {
 public:
  explicit LogicalNotExpr(std::unique_ptr<const RelationalExpr> operand)
      : operand_(std::move(operand)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitLogicalNotExpr(*this); }

  const RelationalExpr& operand() const { return *operand_; }

 private:
  // The expression being negated.
  const std::unique_ptr<const RelationalExpr> operand_;
};

// A statement can be an assignment, a conditional, a loop, or a function call.
class Statement : public AstNode {
 public:
  // A block is a (possibly empty) sequence of statements.
  using Block = std::vector<std::unique_ptr<const Statement>>;
};

// An assignment: v := ae.
class Assignment : public Statement {
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

// A conditional statement: if re block1 block1
class Conditional : public Statement {
 public:
  Conditional(std::unique_ptr<const RelationalExpr> guard,
              Statement::Block true_branch, Statement::Block false_branch)
      : guard_(std::move(guard)),
        true_branch_(std::move(true_branch)),
        false_branch_(std::move(false_branch)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitConditional(*this); }

  const RelationalExpr& guard() const { return *guard_; }

  const Statement::Block& true_branch() const { return true_branch_; }
  const Statement::Block& false_branch() const { return false_branch_; }

 private:
  // The guard expression of the conditional.
  std::unique_ptr<const RelationalExpr> guard_;

  // The true and false branches of the conditional.
  Statement::Block true_branch_;
  Statement::Block false_branch_;
};

// A loop statement: while re block
class Loop : public Statement {
 public:
  Loop(std::unique_ptr<const RelationalExpr> guard, Statement::Block body)
      : guard_(std::move(guard)), body_(std::move(body)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitLoop(*this); }

  const RelationalExpr& guard() const { return *guard_; }

  const Statement::Block& body() const { return body_; }

 private:
  // The guard expression of the loop.
  std::unique_ptr<const RelationalExpr> guard_;

  // The body of the loop.
  Statement::Block body_;
};

// A function definition: def fn(v...) block ae. The 'v...' are the parameters
// of the function; the 'ae' at the end of the function body is the return value
// of the function.
class FunctionDef : public AstNode {
 public:
  // A block is a (possibly empty) sequence of function definitions.
  using Block = std::vector<std::unique_ptr<const FunctionDef>>;

  // A name is a function identifier; it is assumed to be unique.
  using Name = std::string;

  FunctionDef(const Name& function_name,
              std::vector<std::unique_ptr<const VariableExpr>> parameters,
              Statement::Block function_body,
              std::unique_ptr<const ArithmeticExpr> retval)
      : function_name_(function_name),
        parameters_(std::move(parameters)),
        function_body_(std::move(function_body)),
        retval_(std::move(retval)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitFunctionDef(*this); }

  const Name& function_name() const { return function_name_; }

  const std::vector<std::unique_ptr<const VariableExpr>>& parameters() const {
    return parameters_;
  }

  const Statement::Block& function_body() const { return function_body_; }

  const ArithmeticExpr& retval() const { return *retval_; }

 private:
  // The name of the function being defined.
  Name function_name_;

  // The parameters of the function being defined.
  std::vector<std::unique_ptr<const VariableExpr>> parameters_;

  // The body of the function being defined.
  Statement::Block function_body_;

  // The return value of the function being defined.
  std::unique_ptr<const ArithmeticExpr> retval_;
};

// A function call: v := fn(ae...)
class FunctionCall : public Statement {
 public:
  FunctionCall(std::unique_ptr<const VariableExpr> lhs,
               const FunctionDef::Name& callee_name,
               std::vector<std::unique_ptr<const ArithmeticExpr>> arguments)
      : lhs_(std::move(lhs)),
        callee_name_(callee_name),
        arguments_(std::move(arguments)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitFunctionCall(*this); }

  const VariableExpr& lhs() const { return *lhs_; }

  const FunctionDef::Name& callee_name() const { return callee_name_; }

  const std::vector<std::unique_ptr<const ArithmeticExpr>>& arguments() const {
    return arguments_;
  }

 private:
  // The left-hand side of the function call, i.e., the variable that receives
  // the call's return value.
  std::unique_ptr<const VariableExpr> lhs_;

  // The name of the function being called.
  FunctionDef::Name callee_name_;

  // The arguments to the function being called.
  std::vector<std::unique_ptr<const ArithmeticExpr>> arguments_;
};

// A program, consisting of a sequence of function definitions followed by a
// sequence of statements followed by an arithmetic expression (which is the
// final value of the program).
class Program : public AstNode {
 public:
  Program(FunctionDef::Block function_defs, Statement::Block statements,
          std::unique_ptr<const ArithmeticExpr> arithmetic_exp)
      : function_defs_(std::move(function_defs)),
        statements_(std::move(statements)),
        arithmetic_exp_(std::move(arithmetic_exp)) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitProgram(*this); }

  const FunctionDef::Block& function_defs() const { return function_defs_; }

  const Statement::Block& statements() const { return statements_; }

  const ArithmeticExpr& arithmetic_exp() const { return *arithmetic_exp_; }

 private:
  FunctionDef::Block function_defs_;
  Statement::Block statements_;
  std::unique_ptr<const ArithmeticExpr> arithmetic_exp_;
};

}  // namespace version_4
}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_V4_H_
