#include "abstract_syntax/abstract_syntax_tree_v4.h"
#include "gtest/gtest.h"
#include "utility/memory.h"

using cs160::abstract_syntax::version_4::AddExpr;
using cs160::abstract_syntax::version_4::ArithmeticExpr;
using cs160::abstract_syntax::version_4::Assignment;
using cs160::abstract_syntax::version_4::AstVisitor;
using cs160::abstract_syntax::version_4::Conditional;
using cs160::abstract_syntax::version_4::DivideExpr;
using cs160::abstract_syntax::version_4::EqualToExpr;
using cs160::abstract_syntax::version_4::FunctionCall;
using cs160::abstract_syntax::version_4::FunctionDef;
using cs160::abstract_syntax::version_4::GreaterThanEqualToExpr;
using cs160::abstract_syntax::version_4::GreaterThanExpr;
using cs160::abstract_syntax::version_4::IntegerExpr;
using cs160::abstract_syntax::version_4::LessThanEqualToExpr;
using cs160::abstract_syntax::version_4::LessThanExpr;
using cs160::abstract_syntax::version_4::LogicalAndExpr;
using cs160::abstract_syntax::version_4::LogicalNotExpr;
using cs160::abstract_syntax::version_4::LogicalOrExpr;
using cs160::abstract_syntax::version_4::Loop;
using cs160::abstract_syntax::version_4::MultiplyExpr;
using cs160::abstract_syntax::version_4::Program;
using cs160::abstract_syntax::version_4::Statement;
using cs160::abstract_syntax::version_4::SubtractExpr;
using cs160::abstract_syntax::version_4::VariableExpr;

using cs160::make_unique;

namespace {

// Counts the number of AST nodes.
class CountVisitor : public AstVisitor {
 public:
  void VisitIntegerExpr(const IntegerExpr& exp) override { count_++; }

  void VisitVariableExpr(const VariableExpr& exp) override { count_++; }

  void VisitAddExpr(const AddExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitSubtractExpr(const SubtractExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitDivideExpr(const DivideExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitLessThanExpr(const LessThanExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitGreaterThanExpr(const GreaterThanExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitGreaterThanEqualToExpr(const GreaterThanEqualToExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitEqualToExpr(const EqualToExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitLogicalAndExpr(const LogicalAndExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitLogicalOrExpr(const LogicalOrExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitLogicalNotExpr(const LogicalNotExpr& exp) override {
    exp.operand().Visit(this);
    count_++;
  }

  void VisitAssignment(const Assignment& assignment) override {
    assignment.rhs().Visit(this);
    count_++;
  }

  void VisitConditional(const Conditional& conditional) override {
    conditional.guard().Visit(this);
    for (auto& statement : conditional.true_branch()) {
      statement->Visit(this);
    }
    for (auto& statement : conditional.false_branch()) {
      statement->Visit(this);
    }
    count_++;
  }

  void VisitLoop(const Loop& loop) override {
    loop.guard().Visit(this);
    for (auto& statement : loop.body()) {
      statement->Visit(this);
    }
    count_++;
  }

  void VisitFunctionCall(const FunctionCall& call) override {
    call.lhs().Visit(this);
    for (auto& arg : call.arguments()) {
      arg->Visit(this);
    }
    count_++;
  }

  void VisitFunctionDef(const FunctionDef& def) override {
    for (auto& param : def.parameters()) {
      param->Visit(this);
    }
    for (auto& statement : def.function_body()) {
      statement->Visit(this);
    }
    def.retval().Visit(this);
    count_++;
  }

  void VisitProgram(const Program& program) override {
    for (auto& def : program.function_defs()) {
      def->Visit(this);
    }
    for (auto& statement : program.statements()) {
      statement->Visit(this);
    }
    program.arithmetic_exp().Visit(this);
  }

  int count() { return count_; }

 private:
  int count_ = 0;
};

}  // namespace

TEST(AstTestV4, CanCreateAst) {
  Statement::Block foo_statements;

  foo_statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<const LogicalOrExpr>(
          make_unique<const LogicalAndExpr>(
              make_unique<const LessThanExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(100)),
              make_unique<const GreaterThanExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(0))),
          make_unique<const LogicalAndExpr>(
              make_unique<const LessThanEqualToExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(100)),
              make_unique<const GreaterThanEqualToExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(0)))),
      Statement::Block(), Statement::Block())));

  Statement::Block loop_body;
  loop_body.push_back(std::move(make_unique<const Assignment>(
      make_unique<const VariableExpr>("bob"),
      make_unique<const SubtractExpr>(make_unique<const VariableExpr>("bob"),
                                      make_unique<const IntegerExpr>(1)))));

  foo_statements.push_back(std::move(make_unique<const Loop>(
      make_unique<const LogicalNotExpr>(
          make_unique<const EqualToExpr>(make_unique<const VariableExpr>("bob"),
                                         make_unique<const IntegerExpr>(0))),
      std::move(loop_body))));

  auto foo_retval = make_unique<const AddExpr>(
      make_unique<const SubtractExpr>(
          make_unique<const DivideExpr>(make_unique<const IntegerExpr>(12),
                                        make_unique<const IntegerExpr>(3)),
          make_unique<const IntegerExpr>(4)),
      make_unique<const MultiplyExpr>(make_unique<const IntegerExpr>(3),
                                      make_unique<const IntegerExpr>(2)));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("bob")));

  auto foo_def = make_unique<const FunctionDef>("foo", std::move(foo_params),
                                                std::move(foo_statements),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const IntegerExpr>(42)));

  Statement::Block statements;
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "foo",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto ast = make_unique<const Program>(std::move(function_defs),
                                        std::move(statements), std::move(ae));

  CountVisitor counter;
  ast->Visit(&counter);

  EXPECT_EQ(counter.count(), 40);
}
