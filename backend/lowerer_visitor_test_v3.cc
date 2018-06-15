#include "backend/lowerer_visitor.h"

#include <vector>

#include "abstract_syntax/abstract_syntax.h"
#include "gtest/gtest.h"
#include "utility/memory.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::backend::LowererVisitor;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::Statement;


class LowererTestV3 : public ::testing::Test {
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
    return  " <-  PRINT_ARITH \n <-  FUNCTIONDEF \nMkLabel func\n <-"
    "FUN_PROLOGUE \nt_" + std::to_string(blocksize) + " <- 0\n <-"
    "  FUN_EPILOGUE \n";
  }

 protected:
  LowererVisitor lowerer_;
};

TEST_F(LowererTestV3, NestedLogicalsWithInts) {
  auto expr = make_unique<const LogicalOrExpr>(
      make_unique<const LogicalAndExpr>(
          make_unique<const LessThanExpr>(
              make_unique<const IntegerExpr>(50),
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

TEST_F(LowererTestV3, ConditionalWithNestedLogicalsWithVariables) {
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
                                   std::move(statements),
                                   std::move(arithexpr));

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
            "t_6 <- y VAR_LOAD \nt_7 <- x VAR_LOAD \nt_8 <- t_6 + t_7\n"
            "bob <- t_8\nt_9 <- x VAR_LOAD \nt_10 <- 100\n"
            "t_11 <- t_9 < t_10\nt_12 <- y VAR_LOAD \nt_13 <- x VAR_LOAD \n"
            "t_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
            "t_16 <- bob VAR_LOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
            "t_19 <- bob VAR_LOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
            "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
            "je falsebranch0\njmp continue0\nMkLabel falsebranch0\n"
            "jmp continue0\nMkLabel continue0\nt_24 <- 7\nt_25 <- 5\n"
            "t_26 <- t_24 - t_25\n <-  PRINT_ARITH \n <-  FUNCTIONDEF \n"
            "MkLabel func\n <-  FUN_PROLOGUE \nt_27 <- 0\n"
            " <-  FUN_EPILOGUE \n");
}

TEST_F(LowererTestV3, LoopWithNestedLogicalsWithVariables) {
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
      Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));
  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements),
                                   std::move(arithexpr));

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
            "t_6 <- y VAR_LOAD \nt_7 <- x VAR_LOAD \nt_8 <- t_6 + t_7\n"
            "bob <- t_8\nMkLabel loop0\nt_9 <- x VAR_LOAD \n"
            "t_10 <- 100\nt_11 <- t_9 < t_10\n"
            "t_12 <- y VAR_LOAD \nt_13 <- x VAR_LOAD \nt_14 <- t_12 > t_13\n"
            "t_15 <- t_11 && t_14\nt_16 <- bob VAR_LOAD \nt_17 <- 100\n"
            "t_18 <- t_16 <= t_17\nt_19 <- bob VAR_LOAD \nt_20 <- 0\n"
            "t_21 <- t_19 >= t_20\nt_22 <- t_18 && t_21\n"
            "t_23 <- t_15 || t_22\nwhile t_23 == 0\nje continue0\n"
            "jmp loop0\nMkLabel continue0\nt_24 <- 7\n"
            "t_25 <- 5\nt_26 <- t_24 - t_25\n <-  PRINT_ARITH \n"
            " <-  FUNCTIONDEF \nMkLabel func\n <-  FUN_PROLOGUE \n"
            "t_27 <- 0\n <-  FUN_EPILOGUE \n");
}

TEST_F(LowererTestV3, ConditionalsWithTrueBranch) {
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
      std::move(trueblock), Statement::Block())));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements),
                                   std::move(arithexpr));

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
            "t_6 <- y VAR_LOAD \nt_7 <- x VAR_LOAD \nt_8 <- t_6 + t_7\n"
            "bob <- t_8\nt_9 <- x VAR_LOAD \nt_10 <- 100\n"
            "t_11 <- t_9 < t_10\nt_12 <- y VAR_LOAD \n"
            "t_13 <- x VAR_LOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
            "t_16 <- bob VAR_LOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
            "t_19 <- bob VAR_LOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
            "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
            "je falsebranch0\nt_24 <- y VAR_LOAD \nt_25 <- x VAR_LOAD \n"
            "t_26 <- t_24 + t_25\nbob <- t_26\njmp continue0\n"
            "MkLabel falsebranch0\njmp continue0\nMkLabel continue0\n"
            "t_27 <- 7\nt_28 <- 5\nt_29 <- t_27 - t_28\n"
            " <-  PRINT_ARITH \n <-  FUNCTIONDEF \nMkLabel func\n"
            " <-  FUN_PROLOGUE \nt_30 <- 0\n <-  FUN_EPILOGUE \n");
}

TEST_F(LowererTestV3, ConditionalsWithFalseBranch) {
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
            "t_6 <- y VAR_LOAD \nt_7 <- x VAR_LOAD \nt_8 <- t_6 + t_7\n"
            "bob <- t_8\nt_9 <- x VAR_LOAD \nt_10 <- 100\n"
            "t_11 <- t_9 < t_10\nt_12 <- y VAR_LOAD \n"
            "t_13 <- x VAR_LOAD \nt_14 <- t_12 > t_13\nt_15 <- t_11 && t_14\n"
            "t_16 <- bob VAR_LOAD \nt_17 <- 100\nt_18 <- t_16 <= t_17\n"
            "t_19 <- bob VAR_LOAD \nt_20 <- 0\nt_21 <- t_19 >= t_20\n"
            "t_22 <- t_18 && t_21\nt_23 <- t_15 || t_22\nif t_23 == 0\n"
            "je falsebranch0\njmp continue0\nMkLabel falsebranch0\n"
            "t_24 <- y VAR_LOAD \nt_25 <- x VAR_LOAD \nt_26 <- t_24 + t_25\n"
            "bob <- t_26\njmp continue0\nMkLabel continue0\nt_27 <- 7\n"
            "t_28 <- 5\nt_29 <- t_27 - t_28\n <-  PRINT_ARITH \n"
            " <-  FUNCTIONDEF \nMkLabel func\n"
            " <-  FUN_PROLOGUE \nt_30 <- 0\n <-  FUN_EPILOGUE \n");
}

TEST_F(LowererTestV3, ConditionalsWithBothBranch) {
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
      std::move(trueblock), std::move(falseblock))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements),
                                   std::move(arithexpr));

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
            "t_6 <- y VAR_LOAD \nt_7 <- x VAR_LOAD \nt_8 <- t_6 + t_7\n"
            "bob <- t_8\nt_9 <- x VAR_LOAD \nt_10 <- 100\nt_11 <- t_9 < t_10\n"
            "t_12 <- y VAR_LOAD \nt_13 <- x VAR_LOAD \nt_14 <- t_12 > t_13\n"
            "t_15 <- t_11 && t_14\nt_16 <- bob VAR_LOAD \nt_17 <- 100\n"
            "t_18 <- t_16 <= t_17\nt_19 <- bob VAR_LOAD \nt_20 <- 0\n"
            "t_21 <- t_19 >= t_20\nt_22 <- t_18 && t_21\nt_23 <- t_15"
            " || t_22\nif t_23 == 0\nje falsebranch0\n"
            "t_24 <- y VAR_LOAD \nt_25 <- x VAR_LOAD \n"
            "t_26 <- t_24 + t_25\nbob <- t_26\njmp continue0\n"
            "MkLabel falsebranch0\nt_27 <- y VAR_LOAD \nt_28 <- x VAR_LOAD \n"
            "t_29 <- t_27 - t_28\nbob <- t_29\njmp continue0\n"
            "MkLabel continue0\nt_30 <- 7\nt_31 <- 5\n"
            "t_32 <- t_30 - t_31\n <-  PRINT_ARITH \n <-  FUNCTIONDEF \n"
            "MkLabel func\n <-  FUN_PROLOGUE \nt_33 <- 0\n"
            " <-  FUN_EPILOGUE \n");
}


TEST_F(LowererTestV3, SimpleLoop) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));
  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));
  loop_body.push_back(std::move(loopbody));
  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 0\nx <- t_0\nMkLabel loop0\n"
            "t_1 <- x VAR_LOAD \nt_2 <- 5\nt_3 <- t_1 < t_2\n"
            "while t_3 == 0\nje continue0\nt_4 <- x VAR_LOAD \n"
            "t_5 <- 1\nt_6 <- t_4 + t_5\nx <- t_6\njmp loop0\n"
            "MkLabel continue0\n");
}

TEST_F(LowererTestV3, EmptyLoop) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));
  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 0\nx <- t_0\nMkLabel loop0\n"
            "t_1 <- x VAR_LOAD \nt_2 <- 5\nt_3 <- t_1 < t_2\nwhile t_3 == 0\n"
            "je continue0\njmp loop0\nMkLabel continue0\n");
}

TEST_F(LowererTestV3, InfinityLoop) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));
  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));
  loop_body.push_back(std::move(loopbody));
  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 0\nx <- t_0\nMkLabel loop0\n"
            "t_1 <- x VAR_LOAD \nt_2 <- 5\nt_3 <- t_1 < t_2\nwhile t_3 == 0\n"
            "je continue0\nt_4 <- 0\nx <- t_4\njmp loop0\nMkLabel continue0\n");
}


TEST_F(LowererTestV3, NestedLoop) {
  Statement::Block loop_body;
  Statement::Block nested_loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));
  auto assign1 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(0));

  auto nestedloopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("y"),
          make_unique<IntegerExpr>(2)));
  nested_loop_body.push_back(std::move(nestedloopbody));

  auto loop = make_unique<const Loop>(
    make_unique<LessThanExpr>(
        make_unique<VariableExpr>("y"),
        make_unique<IntegerExpr>(3)),
    std::move(nested_loop_body));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));
  loop_body.push_back(std::move(loop));
  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  assign1->Visit(&lowerer_);
  ast->Visit(&lowerer_);
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 0\nx <- t_0\nt_1 <- 0\ny <- t_1\n"
            "MkLabel loop0\nt_2 <- x VAR_LOAD \nt_3 <- 5\nt_4 <- t_2 < t_3\n"
            "while t_4 == 0\nje continue0\nMkLabel loop1\nt_5 <- y VAR_LOAD \n"
            "t_6 <- 3\nt_7 <- t_5 < t_6\nwhile t_7 == 0\nje continue1\n"
            "t_8 <- y VAR_LOAD \nt_9 <- 2\nt_10 <- t_8 + t_9\n"
            "y <- t_10\njmp loop1\nMkLabel continue1\nt_11 <- x VAR_LOAD \n"
            "t_12 <- 1\nt_13 <- t_11 + t_12\nx <- t_13\n"
            "jmp loop0\nMkLabel continue0\n");
}

TEST_F(LowererTestV3, LoopWithBody) {
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
      std::move(loopbody))));

  auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                             make_unique<IntegerExpr>(5));

  FunctionDef::Block function_defs;
  auto func_def = GenerateFuncDef();
  function_defs.push_back(std::move(func_def));

  auto expr = make_unique<Program>(std::move(function_defs),
                                   std::move(statements),
                                   std::move(arithexpr));

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
            "t_6 <- y VAR_LOAD \nt_7 <- x VAR_LOAD \nt_8 <- t_6 + t_7\n"
            "bob <- t_8\nMkLabel loop0\nt_9 <- x VAR_LOAD \n"
            "t_10 <- 100\nt_11 <- t_9 < t_10\n"
            "t_12 <- y VAR_LOAD \nt_13 <- x VAR_LOAD \nt_14 <- t_12 > t_13\n"
            "t_15 <- t_11 && t_14\nt_16 <- bob VAR_LOAD \nt_17 <- 100\n"
            "t_18 <- t_16 <= t_17\nt_19 <- bob VAR_LOAD \nt_20 <- 0\n"
            "t_21 <- t_19 >= t_20\nt_22 <- t_18 && t_21\n"
            "t_23 <- t_15 || t_22\nwhile t_23 == 0\nje continue0\n"
            "t_24 <- y VAR_LOAD \nt_25 <- x VAR_LOAD \n"
            "t_26 <- t_24 - t_25\nbob <- t_26\njmp loop0\n"
            "MkLabel continue0\nt_27 <- 7\nt_28 <- 5\n"
            "t_29 <- t_27 - t_28\n <-  PRINT_ARITH \n"
            " <-  FUNCTIONDEF \nMkLabel func\n <-  FUN_PROLOGUE \n"
            "t_30 <- 0\n <-  FUN_EPILOGUE \n");
}
