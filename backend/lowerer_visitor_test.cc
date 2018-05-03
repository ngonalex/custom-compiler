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
  auto expr = cs160::make_unique<AddExpr>((make_unique<IntegerExpr>(7)),
                                    make_unique<IntegerExpr>(5));
  expr->Visit(&lowerer_);

  // t_0 <- 7
  // t_1 <- 5
  // t_2 <- t_0 + t_1
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 + t_1\n");
}

TEST_F(LowererTest, SubtractExprIsVisited) {
  auto expr = cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 - t_1\n");
}

TEST_F(LowererTest, MultiplyExprIsVisited) {
  auto expr = cs160::make_unique<MultiplyExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  // t_0 <- 7
  // t_1 <- 5
  // t_2 <- t_0 * t_1
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 * t_1\n");
}

TEST_F(LowererTest, DivideExprIsVisited) {
  auto expr = cs160::make_unique<DivideExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  // t_0 <- 7
  // t_1 <- 5
  // t_2 <- t_0 / t_1
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 7\nt_1 <- 5\nt_2 <- t_0 / t_1\n");
}

TEST_F(LowererTest, NestedVisitationsWorkProperly) {
  auto expr = cs160::make_unique<DivideExpr>(
    cs160::make_unique<AddExpr>(make_unique<IntegerExpr>(7),
      make_unique<IntegerExpr>(5)),
    cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(2),
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
  auto expr = cs160::make_unique<Assignment>(
    cs160::make_unique<VariableExpr>("x"),
    cs160::make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);
  // t_0 <- 5
  // hello <- t_0
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nx <- t_0\n");
}

TEST_F(LowererTest, MoreComplexAssignment) {
  auto expr = cs160::make_unique<Assignment>(
    cs160::make_unique<VariableExpr>("x"),
    cs160::make_unique<AddExpr>(
      cs160::make_unique<IntegerExpr>(5),
      cs160::make_unique<IntegerExpr>(10)));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\n"
    "t_2 <- t_0 + t_1\nx <- t_2\n");
}

TEST_F(LowererTest, BasicProgramCreation) {

  Statement::Block statements;

  statements.push_back(std::move(cs160::make_unique<Assignment>(
    cs160::make_unique<VariableExpr>("x"),
    cs160::make_unique<AddExpr>(
      cs160::make_unique<IntegerExpr>(5),
      cs160::make_unique<IntegerExpr>(10)))));

  auto arithexpr = cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  auto expr = cs160::make_unique<Program>(std::move(statements),
    std::move(arithexpr));

  expr->Visit(&lowerer_);

  // t_0 <- 5
  // t_1 <- 10
  // t_2 <- t_0 + t_1
  // x <- t_2
  // t_3 <- 7
  // t_4 <- 5
  // t_5 <- t_3 - t_4

  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 5\nt_1 <- 10\nt_2 <- t_0 + t_1\n"
    "x <- t_2\nt_3 <- 7\nt_4 <- 5\nt_5 <- t_3 - t_4\n");
}

TEST_F(LowererTest, SimpleLogicalAnd) {
  auto expr = make_unique<const LogicalAndExpr>(
    cs160::make_unique<const LessThanExpr>(
        cs160::make_unique<const VariableExpr>("x"),
        cs160::make_unique<const IntegerExpr>(100)),
    cs160::make_unique<const GreaterThanExpr>(
        cs160::make_unique<const VariableExpr>("x"),
        cs160::make_unique<const IntegerExpr>(100)));
    expr->Visit(&lowerer_);

    // t_0 <- 100
    // t_1 <- x < t_0
    // t_2 <- 100
    // t_3 <- x > t_2
    // t_4 <- t_1 && t_3
    EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 100\nt_1 <- x < t_0\nt_2 <- 100\n"
      "t_3 <- x > t_2\nt_4 <- t_1 && t_3\n");
}

TEST_F(LowererTest, NestedLogicalsWithInts) {

  auto expr = cs160::make_unique<const LogicalOrExpr>(
    cs160::make_unique<const LogicalAndExpr>(
      cs160::make_unique<const LessThanExpr>(
        cs160::make_unique<const IntegerExpr>(50),
        cs160::make_unique<const IntegerExpr>(100)),
      cs160::make_unique<const GreaterThanExpr>(
        cs160::make_unique<const IntegerExpr>(50),
        cs160::make_unique<const IntegerExpr>(0))),
    cs160::make_unique<const LogicalAndExpr>(
      cs160::make_unique<const LessThanEqualToExpr>(
        cs160::make_unique<const IntegerExpr>(50),
        cs160::make_unique<const IntegerExpr>(100)),
      cs160::make_unique<const GreaterThanEqualToExpr>(
        cs160::make_unique<const IntegerExpr>(50),
        cs160::make_unique<const IntegerExpr>(0))));

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
  EXPECT_EQ(lowerer_.GetOutput(), "t_0 <- 50\nt_1 <- 100\nt_2 <- t_0 < t_1\n"
    "t_3 <- 50\nt_4 <- 0\nt_5 <- t_3 > t_4\nt_6 <- t_2 && t_5\n"
    "t_7 <- 50\nt_8 <- 100\nt_9 <- t_7 <= t_8\nt_10 <- 50\nt_11 <- 0\n"
    "t_12 <- t_10 >= t_11\nt_13 <- t_9 && t_12\nt_14 <- t_6 || t_13\n");
}

// TEST_F(LowererTest, NestedLogicalsWithVariables) {

//   auto expr = cs160::make_unique<const LogicalOrExpr>(
//     cs160::make_unique<const LogicalAndExpr>(
//       cs160::make_unique<const VariableExpr>("x"),
//         cs160::make_unique<const VariableExpr>("y")));

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
