#include "backend/interpreter_visitor.h"

#include <math.h>
#include <iostream>
#include <stack>
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
using cs160::backend::InterpreterVisitor;
using cs160::make_unique;
using cs160::abstract_syntax::backend::Statement;
using cs160::abstract_syntax::backend::Assignment;

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

TEST_F(InterpreterTest, AddLargeNum) {
  auto expr = cs160::make_unique<AddExpr>(
    (make_unique<IntegerExpr>(pow(2, 30))),
    make_unique<IntegerExpr>(pow(2, 30)));
  expr->Visit(&interpreter_);

  EXPECT_EQ(interpreter_.GetOutput(), -pow(2, 31));
}

// not working for now, need to figure out mult overflow later
// TEST_F(InterpreterTest, MultLargeNum){
//   auto expr = cs160::make_unique<MultiplyExpr>((
//   make_unique<IntegerExpr>(pow(2,30))),make_unique<IntegerExpr>(pow(2,30)));
//   expr->Visit(&interpreter_);
//   std::cout<<"result is "<< interpreter_.GetOutput()<<std::endl;
//   EXPECT_EQ(interpreter_.GetOutput(), 0);
// }

TEST_F(InterpreterTest, DivisionByZero) {
  auto expr = cs160::make_unique<DivideExpr>(
    (make_unique<IntegerExpr>(1)), make_unique<IntegerExpr>(0));

  EXPECT_EXIT(expr->Visit(&interpreter_),
    ::testing::ExitedWithCode(1), "Dividing zero");
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

TEST_F(InterpreterTest, LessThanExprIsVisited) {
  auto expr = cs160::make_unique<const LessThanExpr>(
                  make_unique<const IntegerExpr>(50),
                  make_unique<const VariableExpr>('x'));
  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetBoolOutput(), true);
}

TEST_F(InterpreterTest, NestedLogicals) {

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

  expr->Visit(&interpreter_);
  EXPECT_EQ(interpreter_.GetBoolOutput(), true);
}

TEST_F(InterpreterTest, SimpleAssignmentTest) {
  auto expr = cs160::make_unique<Assignment>(
    cs160::make_unique<VariableExpr>("x"),
    cs160::make_unique<IntegerExpr>(5));

  expr->Visit(&interpreter_);

  EXPECT_EQ(interpreter_.GetVariable("x"), 5);
}

TEST_F(InterpreterTest, MoreComplexAssignment) {
  auto expr = cs160::make_unique<Assignment>(
    cs160::make_unique<VariableExpr>("x"),
    cs160::make_unique<AddExpr>(
      cs160::make_unique<IntegerExpr>(5),
      cs160::make_unique<IntegerExpr>(10)));

  expr->Visit(&interpreter_);

  EXPECT_EQ(interpreter_.GetVariable("x"), 15);
}

TEST_F(InterpreterTest, SanityCheckProg) {
  
  Statement::Block statements;

  auto state1 = cs160::make_unique<Assignment>(
    cs160::make_unique<VariableExpr>("x"),
    cs160::make_unique<AddExpr>(
      cs160::make_unique<IntegerExpr>(5),
      cs160::make_unique<IntegerExpr>(10)));

  statements.push_back(std::move(state1));

  auto arithexpr = cs160::make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
    make_unique<IntegerExpr>(5));

  auto expr = cs160::make_unique<Program>(std::move(statements),
    std::move(arithexpr));

  expr->Visit(&interpreter_);



  // Idk how this works lol
  // for (auto& statement : statements) {
    //std::string lol = dynamic_cast<Assignment*>(statement)->lhs().name();
    EXPECT_EQ(interpreter_.GetVariable("x"), 15);
  // }

  EXPECT_EQ(interpreter_.GetOutput(), 2);
}





