#include <stack>

#include "gtest/gtest.h"

#include "abstract_syntax/abstract_syntax.h"
#include "backend/interpreter_visitor.h"
#include "utility/memory.h"


using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::BinaryOperatorExpr;
using cs160::backend::InterpreterVisitor;

using cs160::make_unique;

class InterpreterTest : public ::testing::Test {
 protected:
  InterpreterVisitor interpreter_;
};

TEST_F(InterpreterTest, IntegerExprIsVisited) {
  auto number = make_unique<IntegerExpr>(7);
  number->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 7);
}

TEST_F(InterpreterTest, AddExprIsVisited) {
  auto expr = cs160::make_unique<AddExpr>((make_unique<IntegerExpr>(7)),
                                   make_unique<IntegerExpr>(5));
  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 12);
}

TEST_F(InterpreterTest, SubtractExprIsVisited) {
  auto expr = cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));

  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 2);
}

TEST_F(InterpreterTest, MultiplyExprIsVisited) {
  auto expr = cs160::make_unique<MultiplyExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));
  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 35);
}

TEST_F(InterpreterTest, DivideExprIsVisited) {
  auto expr = cs160::make_unique<DivideExpr>(make_unique<IntegerExpr>(7),
                                      make_unique<IntegerExpr>(5));
  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 1);
}

TEST_F(InterpreterTest, NestedVisitationsWorkProperly) {
  auto expr = cs160::make_unique<DivideExpr>(
      cs160::make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                           make_unique<IntegerExpr>(5)),
      cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(2),
                                make_unique<IntegerExpr>(1)));
  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 12);
}

TEST_F(InterpreterTest, NestedVisitationsWorkProperly_2) {
  auto expr = cs160::make_unique<MultiplyExpr>(
    cs160::make_unique<DivideExpr>(
        cs160::make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                             make_unique<IntegerExpr>(5)),
        cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(2),
                                  make_unique<IntegerExpr>(1))),
    cs160::make_unique<DivideExpr>(
        cs160::make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                             make_unique<IntegerExpr>(5)),
        cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(2),
                                  make_unique<IntegerExpr>(1)))
  );
  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 144);
}


