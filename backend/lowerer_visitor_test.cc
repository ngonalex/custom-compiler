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
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::Statement;
using cs160::backend::LowererVisitor;
using cs160::make_unique;


// TO DO: Write tests for variables as AEs
class LowererTest : public ::testing::Test {
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
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 + t_1\n"
    "t_3 <- 2\nt_4 <- 1\nt_5 <- t_3 - t_4\nt_6 <- t_2 / t_5\n");
}

TEST_F(LowererTest, SimpleAssignmentTest) {
  auto expr = make_unique<Assignment>(
    make_unique<VariableExpr>("x"),
    make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);
  // t_0 <- 5
  // hello <- t_0
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nx <- t_0\n");
}

TEST_F(LowererTest, DoubleIntAssignmentTest) {
  auto expr = make_unique<Assignment>(
    make_unique<VariableExpr>("x"),
    make_unique<AddExpr>(
      make_unique<IntegerExpr>(5),
      make_unique<IntegerExpr>(10)));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\n"
    "t_2 <- t_0 + t_1\nx <- t_2\n");
}

TEST_F(LowererTest, FunctionDefTest){

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  Statement::Block fact_body;

  auto foo_retval = make_unique<const AddExpr>(
    make_unique<const IntegerExpr>(1),
    make_unique<const IntegerExpr>(0));

  auto foo_def = make_unique<const FunctionDef>("func", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));

  foo_def->Visit(&lowerer_);
  
  EXPECT_EQ(lowerer_.GetOutput(), " <-  def \nMkLabel func\n <-  FUNPROLOGUE \nt_0 <- 1\nt_1 <- 0\nt_2 <- t_0 + t_1\n <-  FUNEPILOGUE \n");
}

// TEST_F(LowererTest, BasicProgramCreation) {
//   Statement::Block statements;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   auto expr = make_unique<Program>(std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 7
//   // t_4 <- 5
//   // t_5 <- t_3 - t_4

//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 7\nt_4 <- 5\nt_5 <- t_3 - t_4\n");
// }

// TEST_F(LowererTest, UnassignedVariable) {
//   auto expr = make_unique<const LogicalAndExpr>(
//     make_unique<const LessThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)),
//     make_unique<const GreaterThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)));

//     // t_0 <- 100
//     // t_1 <- x < t_0
//     // t_2 <- 100
//     // t_3 <- x > t_2
//     // t_4 <- t_1 && t_3
//     EXPECT_EXIT(expr->Visit(&lowerer_), ::testing::ExitedWithCode(1),
//       "Variable x not assigned");
// }

// TEST_F(LowererTest, NestedLogicalsWithInts) {
//   auto expr = make_unique<const LogicalOrExpr>(
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanExpr>(
//         make_unique<const IntegerExpr>(50),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanExpr>(
//         make_unique<const IntegerExpr>(50),
//         make_unique<const IntegerExpr>(0))),
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanEqualToExpr>(
//         make_unique<const IntegerExpr>(50),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanEqualToExpr>(
//         make_unique<const IntegerExpr>(50),
//         make_unique<const IntegerExpr>(0))));

//   expr->Visit(&lowerer_);
//   // t_0 <- 50
//   // t_1 <- 100
//   // t_2 <- t_0 < t_1
//   // t_3 <- 50
//   // t_4 <- 0
//   // t_5 <- t_3 > t_4
//   // t_6 <- t_2 && t_5
//   // t_7 <- 50
//   // t_8 <- 100
//   // t_9 <- t_7 <= t_8
//   // t_10 <- 50
//   // t_11 <- 0
//   // t_12 <- t_10 >= t_11
//   // t_13 <- t_9 && t_12
//   // t_14 <- t_6 || t_13
//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 50\nt_1 <- 100\nt_2 <- t_0 < t_1\n"
//     "t_3 <- 50\nt_4 <- 0\nt_5 <- t_3 > t_4\nt_6 <- t_2 && t_5\n"
//     "t_7 <- 50\nt_8 <- 100\nt_9 <- t_7 <= t_8\nt_10 <- 50\nt_11 <- 0\n"
//     "t_12 <- t_10 >= t_11\nt_13 <- t_9 && t_12\nt_14 <- t_6 || t_13\n");
// }

// TEST_F(LowererTest, VariabletoVariableAssignmentTest) {
//   Statement::Block statements;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("y"),
//     make_unique<SubtractExpr>(
//       make_unique<VariableExpr>("x"),
//       make_unique<IntegerExpr>(10)))));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   auto expr = make_unique<Program>(std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 10
//   // t_4 <- x - t_3
//   // y <- t_4
//   // t_5 <- 7
//   // t_6 <- 5
//   // t_7 <- t_5 - t_6

//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 10\nt_4 <- x - t_3\ny <- t_4\nt_5 <- 7\nt_6 <- 5\n"
//     "t_7 <- t_5 - t_6\n");
// }

// TEST_F(LowererTest, ConditionalWithNestedLogicalsWithVariables) {
//   Statement::Block statements;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("y"),
//     make_unique<SubtractExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//     make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Conditional>(
//     make_unique<LogicalOrExpr>(
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanExpr>(
//         make_unique<const VariableExpr>("y"),
//         make_unique<const VariableExpr>("x"))),
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(0)))),
//         Statement::Block(), Statement::Block())));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   auto expr = make_unique<Program>(std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 5
//   // t_4 <- 10
//   // t_5 <- t_3 - t_4
//   // y <- t_5
//   // t_6 <- y + x
//   // bob <- t_6
//   // t_7 <- 100
//   // t_8 <- x < t_7
//   // t_9 <- y > x
//   // t_10 <- t_8 && t_9
//   // t_11 <- 100
//   // t_12 <- bob <= t_11
//   // t_13 <- 0
//   // t_14 <- bob >= t_13
//   // t_15 <- t_12 && t_14
//   // t_16 <- t_10 || t_15
//   // if t_16 == 0
//   // je falsebranch0
//   // jmp continue0
//   // MkLabel falsebranch0
//   // jmp continue0
//   // MkLabel continue0
//   // t_17 <- 7
//   // t_18 <- 5
//   // t_19 <- t_17 - t_18
//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
//     "t_6 <- y + x\nbob <- t_6\nt_7 <- 100\nt_8 <- x < t_7\nt_9 <- y > x\n"
//     "t_10 <- t_8 && t_9\nt_11 <- 100\nt_12 <- bob <= t_11\n"
//     "t_13 <- 0\nt_14 <- bob >= t_13\nt_15 <- t_12 && t_14\n"
//     "t_16 <- t_10 || t_15\nif t_16 == 0\nje falsebranch0\njmp continue0\n"
//     "MkLabel falsebranch0\njmp continue0\nMkLabel continue0\nt_17 <- 7\n"
//     "t_18 <- 5\nt_19 <- t_17 - t_18\n");
// }

// TEST_F(LowererTest, LoopWithNestedLogicalsWithVariables) {
//   Statement::Block statements;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("y"),
//     make_unique<SubtractExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//     make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Loop>(
//     make_unique<LogicalOrExpr>(
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanExpr>(
//         make_unique<const VariableExpr>("y"),
//         make_unique<const VariableExpr>("x"))),
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(0)))),
//         Statement::Block())));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   auto expr = make_unique<Program>(std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 5
//   // t_4 <- 10
//   // t_5 <- t_3 - t_4
//   // y <- t_5
//   // t_6 <- y + x
//   // bob <- t_6
//   // MkLabel loop0
//   // t_7 <- 100
//   // t_8 <- x < t_7
//   // t_9 <- y > x
//   // t_10 <- t_8 && t_9
//   // t_11 <- 100
//   // t_12 <- bob <= t_11
//   // t_13 <- 0
//   // t_14 <- bob >= t_13
//   // t_15 <- t_12 && t_14
//   // t_16 <- t_10 || t_15
//   // while t_16 == 0
//   // je continue0
//   // jmp loop0
//   // MkLabel continue0
//   // t_17 <- 7
//   // t_18 <- 5
//   // t_19 <- t_17 - t_18
//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
//     "t_6 <- y + x\nbob <- t_6\nMkLabel loop0\nt_7 <- 100\nt_8 <- x < t_7\n"
//     "t_9 <- y > x\nt_10 <- t_8 && t_9\nt_11 <- 100\nt_12 <- bob <= t_11\n"
//     "t_13 <- 0\nt_14 <- bob >= t_13\nt_15 <- t_12 && t_14\n"
//     "t_16 <- t_10 || t_15\nwhile t_16 == 0\nje continue0\njmp loop0\n"
//     "MkLabel continue0\nt_17 <- 7\nt_18 <- 5\nt_19 <- t_17 - t_18\n");
// }

// TEST_F(LowererTest, ConditionalsWithTrueBranch) {
//   Statement::Block statements;
//   Statement::Block trueblock;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("y"),
//     make_unique<SubtractExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//     make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   trueblock.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Conditional>(
//     make_unique<LogicalOrExpr>(
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanExpr>(
//         make_unique<const VariableExpr>("y"),
//         make_unique<const VariableExpr>("x"))),
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(0)))),
//         std::move(trueblock), Statement::Block())));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   auto expr = make_unique<Program>(std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 5
//   // t_4 <- 10
//   // t_5 <- t_3 - t_4
//   // y <- t_5
//   // t_6 <- y + x
//   // bob <- t_6
//   // t_7 <- 100
//   // t_8 <- x < t_7
//   // t_9 <- y > x
//   // t_10 <- t_8 && t_9
//   // t_11 <- 100
//   // t_12 <- bob <= t_11
//   // t_13 <- 0
//   // t_14 <- bob >= t_13
//   // t_15 <- t_12 && t_14
//   // t_16 <- t_10 || t_15
//   // if t_16 == 0
//   // je falsebranch0
//   // t_17 <- y + x
//   // bob <- t_17
//   // jmp continue0
//   // MkLabel falsebranch0
//   // jmp continue0
//   // MkLabel continue0
//   // t_18 <- 7
//   // t_19 <- 5
//   // t_20 <- t_18 - t_19
//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
//     "t_6 <- y + x\nbob <- t_6\nt_7 <- 100\nt_8 <- x < t_7\nt_9 <- y > x\n"
//     "t_10 <- t_8 && t_9\nt_11 <- 100\nt_12 <- bob <= t_11\n"
//     "t_13 <- 0\nt_14 <- bob >= t_13\nt_15 <- t_12 && t_14\n"
//     "t_16 <- t_10 || t_15\nif t_16 == 0\nje falsebranch0\nt_17 <- y + x\n"
//     "bob <- t_17\njmp continue0\nMkLabel falsebranch0\njmp continue0\n"
//     "MkLabel continue0\nt_18 <- 7\nt_19 <- 5\nt_20 <- t_18 - t_19\n");
// }

// TEST_F(LowererTest, ConditionalsWithFalseBranch) {
//   Statement::Block statements;
//   Statement::Block falseblock;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("y"),
//     make_unique<SubtractExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//     make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   falseblock.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Conditional>(
//     make_unique<LogicalOrExpr>(
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanExpr>(
//         make_unique<const VariableExpr>("y"),
//         make_unique<const VariableExpr>("x"))),
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(0)))),
//         Statement::Block(), std::move(falseblock))));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   auto expr = make_unique<Program>(std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 5
//   // t_4 <- 10
//   // t_5 <- t_3 - t_4
//   // y <- t_5
//   // t_6 <- y + x
//   // bob <- t_6
//   // t_7 <- 100
//   // t_8 <- x < t_7
//   // t_9 <- y > x
//   // t_10 <- t_8 && t_9
//   // t_11 <- 100
//   // t_12 <- bob <= t_11
//   // t_13 <- 0
//   // t_14 <- bob >= t_13
//   // t_15 <- t_12 && t_14
//   // t_16 <- t_10 || t_15
//   // if t_16 == 0
//   // je falsebranch0
//   // jmp continue0
//   // MkLabel falsebranch0
//   // t_17 <- y + x
//   // bob <- t_17
//   // jmp continue0
//   // MkLabel continue0
//   // t_18 <- 7
//   // t_19 <- 5
//   // t_20 <- t_18 - t_19
//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
//     "t_6 <- y + x\nbob <- t_6\nt_7 <- 100\nt_8 <- x < t_7\nt_9 <- y > x\n"
//     "t_10 <- t_8 && t_9\nt_11 <- 100\nt_12 <- bob <= t_11\n"
//     "t_13 <- 0\nt_14 <- bob >= t_13\nt_15 <- t_12 && t_14\n"
//     "t_16 <- t_10 || t_15\nif t_16 == 0\nje falsebranch0\njmp continue0\n"
//     "MkLabel falsebranch0\nt_17 <- y + x\nbob <- t_17\njmp continue0\n"
//     "MkLabel continue0\nt_18 <- 7\nt_19 <- 5\nt_20 <- t_18 - t_19\n");
// }

// TEST_F(LowererTest, ConditionalsWithBothBranch) {
//   Statement::Block statements;
//   Statement::Block trueblock;
//   Statement::Block falseblock;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("y"),
//     make_unique<SubtractExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//     make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   trueblock.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//       make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   falseblock.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//       make_unique<SubtractExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Conditional>(
//     make_unique<LogicalOrExpr>(
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanExpr>(
//         make_unique<const VariableExpr>("y"),
//         make_unique<const VariableExpr>("x"))),
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(0)))),
//         std::move(trueblock), std::move(falseblock))));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));

//   auto expr = make_unique<Program>(std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 5
//   // t_4 <- 10
//   // t_5 <- t_3 - t_4
//   // y <- t_5
//   // t_6 <- y + x
//   // bob <- t_6
//   // t_7 <- 100
//   // t_8 <- x < t_7
//   // t_9 <- y > x
//   // t_10 <- t_8 && t_9
//   // t_11 <- 100
//   // t_12 <- bob <= t_11
//   // t_13 <- 0
//   // t_14 <- bob >= t_13
//   // t_15 <- t_12 && t_14
//   // t_16 <- t_10 || t_15
//   // if t_16 == 0
//   // je falsebranch0
//   // t_17 <- y + x
//   // bob <- t_17
//   // jmp continue0
//   // MkLabel falsebranch0
//   // t_18 <- y - x
//   // bob <- t_18
//   // jmp continue0
//   // MkLabel continue0
//   // t_19 <- 7
//   // t_20 <- 5
//   // t_21 <- t_19 - t_20
//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
//     "t_6 <- y + x\nbob <- t_6\nt_7 <- 100\nt_8 <- x < t_7\nt_9 <- y > x\n"
//     "t_10 <- t_8 && t_9\nt_11 <- 100\nt_12 <- bob <= t_11\n"
//     "t_13 <- 0\nt_14 <- bob >= t_13\nt_15 <- t_12 && t_14\n"
//     "t_16 <- t_10 || t_15\nif t_16 == 0\nje falsebranch0\nt_17 <- y + x\n"
//     "bob <- t_17\njmp continue0\nMkLabel falsebranch0\nt_18 <- y - x\n"
//     "bob <- t_18\njmp continue0\nMkLabel continue0\nt_19 <- 7\nt_20 <- 5\n"
//     "t_21 <- t_19 - t_20\n");
// }

// TEST_F(LowererTest, LoopWithBody) {
//   Statement::Block statements;
//   Statement::Block loopbody;

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("x"),
//     make_unique<AddExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("y"),
//     make_unique<SubtractExpr>(
//       make_unique<IntegerExpr>(5),
//       make_unique<IntegerExpr>(10)))));

//   statements.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//     make_unique<AddExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   loopbody.push_back(std::move(make_unique<Assignment>(
//     make_unique<VariableExpr>("bob"),
//       make_unique<SubtractExpr>(
//       make_unique<VariableExpr>("y"),
//       make_unique<VariableExpr>("x")))));

//   statements.push_back(std::move(make_unique<const Loop>(
//     make_unique<LogicalOrExpr>(
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanExpr>(
//         make_unique<const VariableExpr>("x"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanExpr>(
//         make_unique<const VariableExpr>("y"),
//         make_unique<const VariableExpr>("x"))),
//     make_unique<const LogicalAndExpr>(
//       make_unique<const LessThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(100)),
//       make_unique<const GreaterThanEqualToExpr>(
//         make_unique<const VariableExpr>("bob"),
//         make_unique<const IntegerExpr>(0)))),
//         std::move(loopbody))));

//   auto arithexpr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
//     make_unique<IntegerExpr>(5));



//   auto expr = make_unique<Program>(Statement::Block(), std::move(statements),
//     std::move(arithexpr));

//   expr->Visit(&lowerer_);

//   // t_0 <- 5
//   // t_1 <- 10
//   // t_2 <- t_0 + t_1
//   // x <- t_2
//   // t_3 <- 5
//   // t_4 <- 10
//   // t_5 <- t_3 - t_4
//   // y <- t_5
//   // t_6 <- y + x
//   // bob <- t_6
//   // MkLabel loop0
//   // t_7 <- 100
//   // t_8 <- x < t_7
//   // t_9 <- y > x
//   // t_10 <- t_8 && t_9
//   // t_11 <- 100
//   // t_12 <- bob <= t_11
//   // t_13 <- 0
//   // t_14 <- bob >= t_13
//   // t_15 <- t_12 && t_14
//   // t_16 <- t_10 || t_15
//   // while t_16 == 0
//   // je continue0
//   // t_17 <- y - x
//   // bob <- t_17
//   // jmp loop0
//   // MkLabel continue0
//   // t_18 <- 7
//   // t_19 <- 5
//   // t_20 <- t_18 - t_19
//   EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
//     "x <- t_2\nt_3 <- 5\nt_4 <- 10\nt_5 <- t_3 - t_4\ny <- t_5\n"
//     "t_6 <- y + x\nbob <- t_6\nMkLabel loop0\nt_7 <- 100\nt_8 <- x < t_7\n"
//     "t_9 <- y > x\nt_10 <- t_8 && t_9\nt_11 <- 100\nt_12 <- bob <= t_11\n"
//     "t_13 <- 0\nt_14 <- bob >= t_13\nt_15 <- t_12 && t_14\n"
//     "t_16 <- t_10 || t_15\nwhile t_16 == 0\nje continue0\nt_17 <- y - x\n"
//     "bob <- t_17\njmp loop0\nMkLabel continue0\nt_18 <- 7\nt_19 <- 5\n"
//     "t_20 <- t_18 - t_19\n");
// }

  // TEST_F(LowererTest, EasyFunction) {
  //   auto expr = make_unique<DivideExpr>(
  //   make_unique<IntegerExpr>(7), make_unique<IntegerExpr>(5));

  //   auto foo_def = make_unique<const FunctionDef>("fib");
  // }

// To do: Nested Branches + Nested Loops + LogicalNot
