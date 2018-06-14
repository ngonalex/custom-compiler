#include "backend/control_flow_graph.h"
#include "backend/lowerer_visitor.h"

#include <vector>

#include "abstract_syntax/abstract_syntax.h"
#include "gtest/gtest.h"
#include "utility/memory.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::ArithmeticExpr;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::EqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanEqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanExpr;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::LessThanEqualToExpr;
using cs160::abstract_syntax::backend::LessThanExpr;
using cs160::abstract_syntax::backend::LogicalAndExpr;
using cs160::abstract_syntax::backend::LogicalBinaryOperator;
using cs160::abstract_syntax::backend::LogicalNotExpr;
using cs160::abstract_syntax::backend::LogicalOrExpr;
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::abstract_syntax::backend::AssignmentFromNewTuple;
using cs160::abstract_syntax::backend::Dereference;
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::Statement;
using cs160::abstract_syntax::backend::FunctionCall;
using cs160::abstract_syntax::backend::FunctionDef;
using cs160::backend::LowererVisitor;
using cs160::backend::ControlFlowGraph;
using cs160::backend::ControlFlowGraphNode;
using cs160::make_unique;

class ControlFlowGraphTest : public ::testing::Test {
  public:
  std::unique_ptr<const FunctionDef> GenerateFuncDef() {
    // empty params
    auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  // empty fact_body
    Statement::Block fact_body;

  // return value
    auto foo_retval = make_unique<const IntegerExpr>(0);

    auto foo_def = make_unique<const FunctionDef>("func", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));
    return foo_def;
  }
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

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);
  
  grapher_.CreateCFG(std::move(lowerer_.GetIR()));
  grapher_.DebugPrint();
  grapher_.Optimize();
  grapher_.MakeThreeAddressCode();

  EXPECT_EQ(grapher_.GetOutput(),"t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\nbob <- t_8\n"
    "t_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\nt_12 <- y VARLOAD \n"
    "t_13 <- x VARLOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
    "t_16 <- bob VARLOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
    "t_19 <- bob VARLOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
    "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
    "je falsebranch0\njmp continue0\nMkLabel falsebranch0\njmp continue0\n"
    "MkLabel continue0\nt_24 <- 7\nt_25 <- 5\nt_26 <- t_24 - t_25\n"
    " <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n <-  FUNPROLOGUE \n"
    "t_27 <- 0\n <-  FUNEPILOGUE \n");
}