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

// TO DO: Write tests for variables as AEs
class LowererTest : public ::testing::Test {
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

  std::string GenerateFuncDefOutPut(int blocksize) {
    return  " <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n <-"
    "FUNPROLOGUE \nt_" + std::to_string(blocksize) + " <- 0\n <-"
    "  FUNEPILOGUE \n";
  }

 protected:
  LowererVisitor lowerer_;
};

TEST_F(LowererTest, IntegerExprIsVisited) {
  auto number = make_unique<IntegerExpr>(7);
  number->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\n");
} 

TEST_F(LowererTest, IntegerExprIsVisited_0) {
  auto number = make_unique<IntegerExpr>(0);
  number->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 0\n");
}

TEST_F(LowererTest, AddExprIsVisited) {
  auto expr = make_unique<AddExpr>((make_unique<IntegerExpr>(7)),
                                   make_unique<IntegerExpr>(5));
  expr->Visit(&lowerer_);

  // t_0 <- 7
  // t_1 <- 5
  // t_2 <- t_0 + t_1
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 + t_1\n");
}

TEST_F(LowererTest, SubtractExprIsVisited) {
  auto expr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 - t_1\n");
}

TEST_F(LowererTest, MultiplyExprIsVisited) {
  auto expr = make_unique<MultiplyExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  // t_0 <- 7
  // t_1 <- 5
  // t_2 <- t_0 * t_1
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 * t_1\n");
}

TEST_F(LowererTest, DivideExprIsVisited) {
  auto expr = make_unique<DivideExpr>(make_unique<IntegerExpr>(7),
                                      make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  // t_0 <- 7
  // t_1 <- 5
  // t_2 <- t_0 / t_1
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 / t_1\n");
}

TEST_F(LowererTest, NestedIntVisitationsWorkProperly) {
  auto expr = make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                           make_unique<IntegerExpr>(5)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(2),
                                make_unique<IntegerExpr>(1)));

  expr->Visit(&lowerer_);

  // t_0 <- 7
  // t_1 <- 5
  // t_2 <- t_0 + t_1
  // t_3 <- 2
  // t_4 <- 1
  // t_5 <- t_3 - t_4
  // t_6 <- t_2 / t_5
  EXPECT_EQ(lowerer_.GetOutput(),
            "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 + t_1\n"
            "t_3 <- 2\nt_4 <- 1\nt_5 <- t_3 - t_4\nt_6 <- t_2 / t_5\n");
}

TEST_F(LowererTest, SimpleAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("x"),
    make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);
  // t_0 <- 5
  // hello <- t_0
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nx <- t_0\n");
}

TEST_F(LowererTest, DoubleIntAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("x"),
    make_unique<AddExpr>(
      make_unique<IntegerExpr>(5),
      make_unique<IntegerExpr>(10)));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  EXPECT_EQ(lowerer_.GetOutput(),
            "t_0 <- 5\nt_1 <- 10\n"
            "t_2 <- t_0 + t_1\nx <- t_2\n");
}

TEST_F(LowererTest, FunctionDefTest) {
  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();

  // empty fact_body
  Statement::Block fact_body;

  // return value
  auto foo_retval = make_unique<const AddExpr>(
    make_unique<const IntegerExpr>(1),
    make_unique<const IntegerExpr>(0));

  auto foo_def = make_unique<const FunctionDef>("func", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));

  foo_def->Visit(&lowerer_);
  EXPECT_EQ(lowerer_.GetOutput(), " <-  FUNCTIONDEF \nMkLabel func\n"
  " <-  FUNPROLOGUE \nt_0 <- 1\nt_1 <- 0\n"
  "t_2 <- t_0 + t_1\n <-  FUNEPILOGUE \n");
}


TEST_F(LowererTest, FunctionCallTest) {
  Statement::Block statements;

  // bob = 10
  // int returnval = fact(bob)
  // func fact (int bob) {
  //    returnval = 1
  //    return retunval + 0
  // }
  statements.push_back(make_unique<const AssignmentFromArithExp>(
    make_unique<const VariableExpr>("bob"),
    make_unique<const IntegerExpr>(10)));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();
  arguments.push_back(std::move(make_unique<const VariableExpr>("bob")));

  // call function fact
  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "fact",
      std::move(arguments))));

  // getting the return value
  auto ae = make_unique<const VariableExpr>("foo_retval");

  auto foo_retval = make_unique<const AddExpr>(
    make_unique<const VariableExpr>("foo_retval"),
    make_unique<const IntegerExpr>(0));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("bob")));

  Statement::Block fact_body;

  fact_body.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("foo_retval"),
    make_unique<IntegerExpr>(1))));

  // fact_body.push_back(std::move(make_unique<const AddExpr>(
  //  make_unique<const VariableExpr>("foo_reval"),
  //  make_unique<const VariableExpr>("bob"))));

  auto foo_def = make_unique<const FunctionDef>("fact", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto ast = make_unique<const Program>(std::move(function_defs),
    std::move(statements), std::move(ae));


  ast->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 10\nbob <- t_0\n"
    "t_1 <- bob VARLOAD \n <-  FUNCTIONCALL \n"
    "foo_retval <- FUNRETLOAD FUNRETLOAD \n <-  FUNRETURNEPILOGUE \n"
    "t_2 <- foo_retval VARLOAD \n <-  PRINTARITH \n <-  FUNCTIONDEF \n"
    "MkLabel fact\n <-  FUNPROLOGUE \nbob <- 0\nt_3 <- 1\nfoo_retval <- t_3\n"
    "t_4 <- foo_retval VARLOAD \nt_5 <- 0\nt_6 <- t_4 + t_5\n"
    " <-  FUNEPILOGUE \n");
}

TEST_F(LowererTest, UnassignedVariable) {
  auto expr = make_unique<const LogicalAndExpr>(
      make_unique<const LessThanExpr>(make_unique<const VariableExpr>("x"),
                                      make_unique<const IntegerExpr>(100)),
      make_unique<const GreaterThanExpr>(make_unique<const VariableExpr>("x"),
                                         make_unique<const IntegerExpr>(100)));

  // t_0 <- 100
  // t_1 <- x < t_0
  // t_2 <- 100
  // t_3 <- x > t_2
  // t_4 <- t_1 && t_3
  EXPECT_EXIT(expr->Visit(&lowerer_), ::testing::ExitedWithCode(1),
              "Variable x not assigned");
}

TEST_F(LowererTest, NestedLogicalsWithInts) {
  auto expr = make_unique<const LogicalOrExpr>(
      make_unique<const LogicalAndExpr>(
          make_unique<const LessThanExpr>(make_unique<const IntegerExpr>(50),
                                          make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanExpr>(
              make_unique<const IntegerExpr>(50),
              make_unique<const IntegerExpr>(0))),
      make_unique<const LogicalAndExpr>(
          make_unique<const LessThanEqualToExpr>(
              make_unique<const IntegerExpr>(50),
              make_unique<const IntegerExpr>(100)),
          make_unique<const GreaterThanEqualToExpr>(
              make_unique<const IntegerExpr>(50),
              make_unique<const IntegerExpr>(0))));

  expr->Visit(&lowerer_);
  // t_0 <- 50
  // t_1 <- 100
  // t_2 <- t_0 < t_1
  // t_3 <- 50
  // t_4 <- 0
  // t_5 <- t_3 > t_4
  // t_6 <- t_2 && t_5
  // t_7 <- 50
  // t_8 <- 100
  // t_9 <- t_7 <= t_8
  // t_10 <- 50
  // t_11 <- 0
  // t_12 <- t_10 >= t_11
  // t_13 <- t_9 && t_12
  // t_14 <- t_6 || t_13
  EXPECT_EQ(lowerer_.GetOutput(),
            "t_0 <- 50\nt_1 <- 100\nt_2 <- t_0 < t_1\n"
            "t_3 <- 50\nt_4 <- 0\nt_5 <- t_3 > t_4\nt_6 <- t_2 && t_5\n"
            "t_7 <- 50\nt_8 <- 100\nt_9 <- t_7 <= t_8\nt_10 <- 50\nt_11 <- 0\n"
            "t_12 <- t_10 >= t_11\nt_13 <- t_9 && t_12\nt_14 <- t_6 || t_13\n");
}



TEST_F(LowererTest, VariabletoVariableAssignmentTest) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("x"),
    make_unique<AddExpr>(
      make_unique<IntegerExpr>(5),
      make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("y"),
    make_unique<SubtractExpr>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(10)))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 10
  // t_4 <- x - t_3
  // y <- t_4
  // t_5 <- 7
  // t_6 <- 5
  // t_7 <- t_5 - t_6

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- x VARLOAD \nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- 7\nt_7 <- 5\nt_8 <- t_6 - t_7\n <-  PRINTARITH \n"
    " <-  FUNCTIONDEF \nMkLabel func\n <-  FUNPROLOGUE \nt_9 <- 0\n"
    " <-  FUNEPILOGUE \n");
}

TEST_F(LowererTest, ConditionalWithNestedLogicalsWithVariables) {
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
      Statement::Block(), Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 5
  // t_4 <- 10
  // t_5 <- t_3 - t_4
  // y <- t_5
  // t_6 <- y + x
  // bob <- t_6
  // t_7 <- 100
  // t_8 <- x < t_7
  // t_9 <- y > x
  // t_10 <- t_8 && t_9
  // t_11 <- 100
  // t_12 <- bob <= t_11
  // t_13 <- 0
  // t_14 <- bob >= t_13
  // t_15 <- t_12 && t_14
  // t_16 <- t_10 || t_15
  // if t_16 == 0
  // je falsebranch0
  // jmp continue0
  // MkLabel falsebranch0
  // jmp continue0
  // MkLabel continue0
  // t_17 <- 7
  // t_18 <- 5
  // t_19 <- t_17 - t_18
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
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

TEST_F(LowererTest, LoopWithNestedLogicalsWithVariables) {
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

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 5
  // t_4 <- 10
  // t_5 <- t_3 - t_4
  // y <- t_5
  // t_6 <- y + x
  // bob <- t_6
  // MkLabel loop0
  // t_7 <- 100
  // t_8 <- x < t_7
  // t_9 <- y > x
  // t_10 <- t_8 && t_9
  // t_11 <- 100
  // t_12 <- bob <= t_11
  // t_13 <- 0
  // t_14 <- bob >= t_13
  // t_15 <- t_12 && t_14
  // t_16 <- t_10 || t_15
  // while t_16 == 0
  // je continue0
  // jmp loop0
  // MkLabel continue0
  // t_17 <- 7
  // t_18 <- 5
  // t_19 <- t_17 - t_18
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\nbob <- t_8\n"
    "MkLabel loop0\nt_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\n"
    "t_12 <- y VARLOAD \nt_13 <- x VARLOAD \nt_14 <- t_12 > t_13\n"
    "t_15 <- t_11 && t_14\nt_16 <- bob VARLOAD \nt_17 <- 100\n"
    "t_18 <- t_16 <= t_17\nt_19 <- bob VARLOAD \nt_20 <- 0\n"
    "t_21 <- t_19 >= t_20\nt_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\n"
    "while t_23 == 0\nje continue0\njmp loop0\nMkLabel continue0\nt_24 <- 7\n"
    "t_25 <- 5\nt_26 <- t_24 - t_25\n <-  PRINTARITH \n <-  FUNCTIONDEF \n"
    "MkLabel func\n <-  FUNPROLOGUE \nt_27 <- 0\n <-  FUNEPILOGUE \n");
}

TEST_F(LowererTest, ConditionalsWithTrueBranch) {
  Statement::Block statements;
  Statement::Block trueblock;

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

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(
      make_unique<VariableExpr>("y"),
      make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Conditional>(
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
      std::move(trueblock), Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 5
  // t_4 <- 10
  // t_5 <- t_3 - t_4
  // y <- t_5
  // t_6 <- y + x
  // bob <- t_6
  // t_7 <- 100
  // t_8 <- x < t_7
  // t_9 <- y > x
  // t_10 <- t_8 && t_9
  // t_11 <- 100
  // t_12 <- bob <= t_11
  // t_13 <- 0
  // t_14 <- bob >= t_13
  // t_15 <- t_12 && t_14
  // t_16 <- t_10 || t_15
  // if t_16 == 0
  // je falsebranch0
  // t_17 <- y + x
  // bob <- t_17
  // jmp continue0
  // MkLabel falsebranch0
  // jmp continue0
  // MkLabel continue0
  // t_18 <- 7
  // t_19 <- 5
  // t_20 <- t_18 - t_19
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\nbob <- t_8\n"
    "t_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\nt_12 <- y VARLOAD \n"
    "t_13 <- x VARLOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
    "t_16 <- bob VARLOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
    "t_19 <- bob VARLOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
    "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
    "je falsebranch0\nt_24 <- y VARLOAD \nt_25 <- x VARLOAD \n"
    "t_26 <- t_24 + t_25\nbob <- t_26\njmp continue0\nMkLabel falsebranch0\n"
    "jmp continue0\nMkLabel continue0\nt_27 <- 7\nt_28 <- 5\n"
    "t_29 <- t_27 - t_28\n <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n"
    " <-  FUNPROLOGUE \nt_30 <- 0\n <-  FUNEPILOGUE \n");
}

TEST_F(LowererTest, ConditionalsWithFalseBranch) {
  Statement::Block statements;
  Statement::Block falseblock;

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

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(
      make_unique<VariableExpr>("y"),
      make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Conditional>(
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
      Statement::Block(), std::move(falseblock))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 5
  // t_4 <- 10
  // t_5 <- t_3 - t_4
  // y <- t_5
  // t_6 <- y + x
  // bob <- t_6
  // t_7 <- 100
  // t_8 <- x < t_7
  // t_9 <- y > x
  // t_10 <- t_8 && t_9
  // t_11 <- 100
  // t_12 <- bob <= t_11
  // t_13 <- 0
  // t_14 <- bob >= t_13
  // t_15 <- t_12 && t_14
  // t_16 <- t_10 || t_15
  // if t_16 == 0
  // je falsebranch0
  // jmp continue0
  // MkLabel falsebranch0
  // t_17 <- y + x
  // bob <- t_17
  // jmp continue0
  // MkLabel continue0
  // t_18 <- 7
  // t_19 <- 5
  // t_20 <- t_18 - t_19
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\nbob <- t_8\n"
    "t_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\nt_12 <- y VARLOAD \n"
    "t_13 <- x VARLOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
    "t_16 <- bob VARLOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
    "t_19 <- bob VARLOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
    "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
    "je falsebranch0\njmp continue0\nMkLabel falsebranch0\n"
    "t_24 <- y VARLOAD \nt_25 <- x VARLOAD \nt_26 <- t_24 + t_25\n"
    "bob <- t_26\njmp continue0\nMkLabel continue0\nt_27 <- 7\nt_28 <- 5\n"
    "t_29 <- t_27 - t_28\n <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n"
    " <-  FUNPROLOGUE \nt_30 <- 0\n <-  FUNEPILOGUE \n");
}

TEST_F(LowererTest, ConditionalsWithBothBranch) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

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

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("bob"),
      make_unique<AddExpr>(
      make_unique<VariableExpr>("y"),
      make_unique<VariableExpr>("x")))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("bob"),
      make_unique<SubtractExpr>(
      make_unique<VariableExpr>("y"),
      make_unique<VariableExpr>("x")))));

  statements.push_back(std::move(make_unique<const Conditional>(
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
      std::move(trueblock), std::move(falseblock))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 5
  // t_4 <- 10
  // t_5 <- t_3 - t_4
  // y <- t_5
  // t_6 <- y + x
  // bob <- t_6
  // t_7 <- 100
  // t_8 <- x < t_7
  // t_9 <- y > x
  // t_10 <- t_8 && t_9
  // t_11 <- 100
  // t_12 <- bob <= t_11
  // t_13 <- 0
  // t_14 <- bob >= t_13
  // t_15 <- t_12 && t_14
  // t_16 <- t_10 || t_15
  // if t_16 == 0
  // je falsebranch0
  // t_17 <- y + x
  // bob <- t_17
  // jmp continue0
  // MkLabel falsebranch0
  // t_18 <- y - x
  // bob <- t_18
  // jmp continue0
  // MkLabel continue0
  // t_19 <- 7
  // t_20 <- 5
  // t_21 <- t_19 - t_20
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\nbob <- t_8\n"
    "t_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\nt_12 <- y VARLOAD \n"
    "t_13 <- x VARLOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
    "t_16 <- bob VARLOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
    "t_19 <- bob VARLOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
    "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
    "je falsebranch0\nt_24 <- y VARLOAD \nt_25 <- x VARLOAD \n"
    "t_26 <- t_24 + t_25\nbob <- t_26\njmp continue0\nMkLabel falsebranch0\n"
    "t_27 <- y VARLOAD \nt_28 <- x VARLOAD \nt_29 <- t_27 - t_28\n"
    "bob <- t_29\njmp continue0\nMkLabel continue0\nt_30 <- 7\nt_31 <- 5\n"
    "t_32 <- t_30 - t_31\n <-  PRINTARITH \n <-  FUNCTIONDEF \nMkLabel func\n"
    " <-  FUNPROLOGUE \nt_33 <- 0\n <-  FUNEPILOGUE \n");
}

TEST_F(LowererTest, LoopWithBody) {
  Statement::Block statements;
  Statement::Block loopbody;

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

  loopbody.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("bob"),
      make_unique<SubtractExpr>(
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
      std::move(loopbody))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
    std::move(statements), std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 5
  // t_4 <- 10
  // t_5 <- t_3 - t_4
  // y <- t_5
  // t_6 <- y + x
  // bob <- t_6
  // MkLabel loop0
  // t_7 <- 100
  // t_8 <- x < t_7
  // t_9 <- y > x
  // t_10 <- t_8 && t_9
  // t_11 <- 100
  // t_12 <- bob <= t_11
  // t_13 <- 0
  // t_14 <- bob >= t_13
  // t_15 <- t_12 && t_14
  // t_16 <- t_10 || t_15
  // while t_16 == 0
  // je continue0
  // t_17 <- y - x
  // bob <- t_17
  // jmp loop0
  // MkLabel continue0
  // t_18 <- 7
  // t_19 <- 5
  // t_20 <- t_18 - t_19
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
    "t_6 <- y VARLOAD \nt_7 <- x VARLOAD \nt_8 <- t_6 + t_7\nbob <- t_8\n"
    "MkLabel loop0\nt_9 <- x VARLOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\n"
    "t_12 <- y VARLOAD \nt_13 <- x VARLOAD \nt_14 <- t_12 > t_13\n"
    "t_15 <- t_11 && t_14\nt_16 <- bob VARLOAD \nt_17 <- 100\n"
    "t_18 <- t_16 <= t_17\nt_19 <- bob VARLOAD \nt_20 <- 0\n"
    "t_21 <- t_19 >= t_20\nt_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\n"
    "while t_23 == 0\nje continue0\nt_24 <- y VARLOAD \nt_25 <- x VARLOAD \n"
    "t_26 <- t_24 - t_25\nbob <- t_26\njmp loop0\nMkLabel continue0\n"
    "t_27 <- 7\nt_28 <- 5\nt_29 <- t_27 - t_28\n <-  PRINTARITH \n"
    " <-  FUNCTIONDEF \nMkLabel func\n <-  FUNPROLOGUE \nt_30 <- 0\n"
    " <-  FUNEPILOGUE \n");
}

TEST_F(LowererTest, SimpleTupleTest) {
  auto ast = make_unique<AssignmentFromNewTuple>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(3));
  ast->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "bob <-  VARCHILDTUPLE \nt_0 <- 3\nbob <- t_0 NEWTUPLE \n");
}


TEST_F(LowererTest, AccessTupleTest) {
  auto ast = make_unique<AssignmentFromNewTuple>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(3));
  
  // x->1 = 2
  auto def = make_unique<Dereference>(make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1));
  auto access = make_unique<AssignmentFromArithExp>(std::move(def), make_unique<IntegerExpr>(2));
  ast->Visit(&lowerer_);
  access->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "bob <-  VARCHILDTUPLE \nt_0 <- 3\nbob <- t_0 NEWTUPLE \nt_1 <- 1\nbob->t_1 <- bob LHSDEREFERENCE Parent\nt_2 <- 2\nbob->t_1 <- t_2\n");
}

// To do: Nested Branches + Nested Loops + LogicalNot
