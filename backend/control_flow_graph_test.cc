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

TEST_F(ControlFlowGraphTest, ConditionalWithNestedLogicalsWithVariablesNoElimination) {
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
  grapher_.Optimize();

  EXPECT_EQ(grapher_.GetOutput(),"t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\nbob <- t_8\n"
    "t_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\nt_12 <- y VARLOAD \n"
    "t_13 <- x VARLOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
    "t_16 <- bob VARLOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
    "t_19 <- bob VARLOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
    "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
    "je falsebranch0\njmp continue0\nMkLabel falsebranch0\njmp continue0\n"
    "MkLabel continue0\n"
    " <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n <-  FUNPROLOGUE \n"
    " <-  FUNEPILOGUE \n");
}

TEST_F(ControlFlowGraphTest, LoopWithNestedLogicalsWithVariables) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("x"),
    make_unique<AddExpr>(
      make_unique<IntegerExpr>(5),
      make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("x"),
    make_unique<AddExpr>(
      make_unique<IntegerExpr>(5),
      make_unique<IntegerExpr>(10)))));

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

  statements.push_back(std::move(make_unique<const Loop>(
      make_unique<LogicalOrExpr>(make_unique<const LogicalAndExpr>(
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
      Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));
  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));


  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);
  grapher_.CreateCFG(std::move(lowerer_.GetIR()));
  //grapher_.DebugPrint();
  //grapher_.DebugEdgeAndBlock();
  //std::string prestring = grapher_.GetOutput();
  grapher_.Optimize();
  //std::string poststring = grapher_.GetOutput();
  // int compared = prestring.compare(poststring);
  // std::cout << "--- SEE IF IT GOT REDUCED ---" << std::endl;
  // std::cout << "--- FIRST STRING ---" << std::endl;
  // std::cout << prestring << std::endl;
  // std::cout << "--- SECOND STRING ---" << std::endl;
  // std::cout << poststring << std::endl;
  // std::cout << "--- STRINGCOMPARE INT VALUE ---" << std::endl;
  // std::cout << ".compare() strings: " << compared << std::endl;
  //grapher_.Optimize();
  // t_6 <- 5
  // t_7 <- 10
  // t_8 <- t_6 + t_7
  // x <- t_8
  // t_9 <- 5
  // t_10 <- 10
  // t_11 <- t_9 - t_10
  // y <- t_11
  // t_12 <- y VARLOAD 
  // t_13 <- x VARLOAD 
  // t_14 <- t_12 + t_13
  // bob <- t_14
  // MkLabel loop0
  // t_15 <- x VARLOAD 
  // t_16 <- 100
  // t_17 <- t_15 < t_16
  // t_18 <- y VARLOAD 
  // t_19 <- x VARLOAD 
  // t_20 <- t_18 > t_19
  // t_21 <- t_17 && t_20
  // t_22 <- bob VARLOAD 
  // t_23 <- 100
  // t_24 <- t_22 <= t_23
  // t_25 <- bob VARLOAD 
  // t_26 <- 0
  // t_27 <- t_25 >= t_26
  // t_28 <- t_24 && t_27
  // t_29 <- t_21 || t_28
  // while t_29 == 0
  // je continue0
  // jmp loop0
  // MkLabel continue0
  // <-  PRINTARITH 
  // <-  FUNCTIONDEF 
  // MkLabel func
  // <-  FUNPROLOGUE 
  // <-  FUNEPILOGUE 
  EXPECT_EQ(grapher_.GetOutput(), "t_6 <- 5\nt_7 <- 10\nt_8 <- t_6 + t_7\n"
  "x <- t_8\nt_9 <- 5\nt_10 <- 10\nt_11 <- t_9 - t_10\ny <- t_11\nt_12 <- y "
  "VARLOAD \nt_13 <- x VARLOAD \nt_14 <- t_12 + t_13\nbob <- t_14\nMkLabel " 
  "loop0\nt_15 <- x VARLOAD \nt_16 <- 100\nt_17 <- t_15 < t_16\nt_18 <- y " 
  "VARLOAD \nt_19 <- x VARLOAD \nt_20 <- t_18 > t_19\nt_21 <- t_17 && t_20\n"
  "t_22 <- bob VARLOAD \nt_23 <- 100\nt_24 <- t_22 <= t_23\nt_25 <- bob " 
  "VARLOAD \nt_26 <- 0\nt_27 <- t_25 >= t_26\nt_28 <- t_24 && t_27\n"
  "t_29 <- t_21 || t_28\nwhile t_29 == 0\nje continue0\njmp loop0\n"
  "MkLabel continue0\n <-  PRINTARITH \n <-  FUNCTIONDEF \n"
  "MkLabel func\n <-  FUNPROLOGUE \n <-  FUNEPILOGUE \n");

}
