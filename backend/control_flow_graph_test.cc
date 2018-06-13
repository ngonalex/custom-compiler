#include "backend/control_flow_graph.h"
#include "backend/lowerer_visitor.h"

#include <vector>

#include "abstract_syntax/abstract_syntax.h"
#include "utility/memory.h"
#include "gtest/gtest.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::LessThanExpr;
using cs160::abstract_syntax::backend::LessThanEqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanExpr;
using cs160::abstract_syntax::backend::GreaterThanEqualToExpr;
using cs160::abstract_syntax::backend::EqualToExpr;
using cs160::abstract_syntax::backend::LogicalAndExpr;
using cs160::abstract_syntax::backend::LogicalBinaryOperator;
using cs160::abstract_syntax::backend::LogicalNotExpr;
using cs160::abstract_syntax::backend::LogicalOrExpr;
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::abstract_syntax::backend::AssignmentFromNewTuple;
using cs160::abstract_syntax::backend::Dereference;
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::Statement;
using cs160::backend::LowererVisitor;
using cs160::backend::ControlFlowGraph;
using cs160::backend::ControlFlowGraphNode;
using cs160::make_unique;

class ControlFlowGraphTest : public ::testing::Test {
 protected:
  LowererVisitor lowerer_;
  ControlFlowGraph grapher_;
};

TEST_F(ControlFlowGraphTest, ConditionalWithNestedLogicalsWithVariables) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("x"),
    make_unique<AddExpr>(
      make_unique<IntegerExpr>(5),
      make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("y"),
    make_unique<SubtractExpr>(
      make_unique<IntegerExpr>(5),
      make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("bob"),
    make_unique<AddExpr>(
      make_unique<VariableExpr>("y"),
      make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Conditional>(
    make_unique<LogicalOrExpr>(
    make_unique<const LogicalAndExpr>(
      make_unique<const LessThanExpr>(
        make_unique<const VariableExpr>("x"),
        make_unique<const IntegerExpr>(100)),
      make_unique<const GreaterThanExpr>(
        make_unique<const VariableExpr>("y"),
        make_unique<const VariableExpr>("x"))),
    make_unique<const LogicalAndExpr>(
      make_unique<const LessThanEqualToExpr>(
        make_unique<const VariableExpr>("bob"),
        make_unique<const IntegerExpr>(100)),
      make_unique<const GreaterThanEqualToExpr>(
        make_unique<const VariableExpr>("bob"),
        make_unique<const IntegerExpr>(0)))),
        Statement::Block(), Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  auto expr = make_unique<Program>(std::move(statements),
    std::move(arithexpr));

  expr->Visit(&lowerer_);
  grapher_(std::move(lowerer_.GetIR()));


}