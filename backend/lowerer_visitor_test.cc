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
using cs160::abstract_syntax::backend::BinaryOperatorExpr;
using cs160::backend::LowererVisitor;
using cs160::make_unique;

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

TEST_F(LowererTest, SanityCheckProg) {
  auto assignment = cs160::make_unique<Assignment>(
    cs160::make_unique<VariableExpr>("x"),
    cs160::make_unique<AddExpr>(
      cs160::make_unique<IntegerExpr>(5),
      cs160::make_unique<IntegerExpr>(10)));

  std::vector<std::unique_ptr<const Assignment>> assignmentvector;
  assignmentvector.push_back(std::move(assignment));

  auto arithexpr = cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  auto expr = cs160::make_unique<Program>(std::move(assignmentvector),
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


