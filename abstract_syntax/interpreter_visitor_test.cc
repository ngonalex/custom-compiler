
#include <sstream>
#include <string>
#include <stack>

#include "gtest/gtest.h"

#include "abstract_syntax/abstract_syntax_tree.h"
#include "abstract_syntax/interpreter_visitor.h"
#include "utility/memory.h"


using cs160::abstract_syntax::IntegerExpr;
using cs160::abstract_syntax::AddExpr;
using cs160::abstract_syntax::SubtractExpr;
using cs160::abstract_syntax::MultiplyExpr;
using cs160::abstract_syntax::DivideExpr;
using cs160::abstract_syntax::InterpreterVisitor;

using cs160::make_unique;

class InterprerterTest : public ::testing::Test {
 protected:
  InterpreterVisitor printer_;
};

TEST_F(InterprerterTest, IntegerExprIsVisited) {
  auto number = make_unique<IntegerExpr>(7);
  number->Visit(&printer_);

  EXPECT_EQ(printer_.opstack.top(), "7");
}

TEST_F(InterprerterTest, AddExprIsVisited) {
  auto expr = make_unique<AddExpr>((make_unique<IntegerExpr>(7)),
                                   make_unique<IntegerExpr>(5));
  expr->Visit(&printer_);

  EXPECT_EQ(printer_.GetOutput(), "(+ 7 5)");
}

TEST_F(InterprerterTest, SubtractExprIsVisited) {
  auto expr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.GetOutput(), "(- 7 5)");
}

TEST_F(InterprerterTest, MultiplyExprIsVisited) {
  auto expr = make_unique<MultiplyExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.GetOutput(), "(* 7 5)");
}

TEST_F(InterprerterTest, DivideExprIsVisited) {
  auto expr = make_unique<DivideExpr>(make_unique<IntegerExpr>(7),
                                      make_unique<IntegerExpr>(5));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.GetOutput(), "(/ 7 5)");
}

TEST_F(InterprerterTest, NestedVisitationsWorkProperly) {
  auto expr = make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                           make_unique<IntegerExpr>(5)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(2),
                                make_unique<IntegerExpr>(1)));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.GetOutput(), "(/ (+ 7 5) (- 2 1))");
}
