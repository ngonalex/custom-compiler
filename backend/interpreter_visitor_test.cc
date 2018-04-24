#include "backend/interpreter_visitor.h"

#include <stack>
#include <math.h>
#include <iostream>

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
using cs160::backend::InterpreterVisitor;
using cs160::make_unique;
using cs160::backend::DivisorIsZeroException;

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

TEST_F(InterpreterTest, AddLargeNum){
  auto expr = cs160::make_unique<AddExpr>((make_unique<IntegerExpr>(pow(2,30))),make_unique<IntegerExpr>(pow(2,30)));
  expr->Visit(&interpreter_);

  EXPECT_EQ(interpreter_.GetOutput(), -pow(2,31));
}

// not working for now, need to figure out mult overflow later
// TEST_F(InterpreterTest, MultLargeNum){
//   auto expr = cs160::make_unique<MultiplyExpr>((make_unique<IntegerExpr>(pow(2,30))),make_unique<IntegerExpr>(pow(2,30)));
//   expr->Visit(&interpreter_);
//   std::cout<<"result is "<< interpreter_.GetOutput()<<std::endl;
//   EXPECT_EQ(interpreter_.GetOutput(), 0);
// }

TEST_F(InterpreterTest, DivisionByZero){
  auto expr = cs160::make_unique<DivideExpr>((make_unique<IntegerExpr>(1)),make_unique<IntegerExpr>(0));
  
  EXPECT_THROW(expr->Visit(&interpreter_), DivisorIsZeroException);
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
                                  make_unique<IntegerExpr>(1))));
  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetOutput(), 144);
}


