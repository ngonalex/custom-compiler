
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

class InterpreterTest : public ::testing::Test {
 protected:
  InterpreterVisitor printer_;
};

TEST_F(InterpreterTest, IntegerExprIsVisited) {
  auto number = make_unique<IntegerExpr>(7);
  number->Visit(&printer_);

  EXPECT_EQ(printer_.opstack.top(), 7);
}

TEST_F(InterpreterTest, AddExprIsVisited) {
  auto expr = make_unique<AddExpr>((make_unique<IntegerExpr>(7)),
                                   make_unique<IntegerExpr>(5));
  expr->Visit(&printer_);

  EXPECT_EQ(printer_.opstack.top(), 12);
}

TEST_F(InterpreterTest, SubtractExprIsVisited) {
  auto expr = make_unique<SubtractExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.opstack.top(), 2);
}

TEST_F(InterpreterTest, MultiplyExprIsVisited) {
  auto expr = make_unique<MultiplyExpr>(make_unique<IntegerExpr>(7),
                                        make_unique<IntegerExpr>(5));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.opstack.top(), 35);
}

TEST_F(InterpreterTest, DivideExprIsVisited) {
  auto expr = make_unique<DivideExpr>(make_unique<IntegerExpr>(7),
                                      make_unique<IntegerExpr>(5));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.opstack.top(), 1);
}

TEST_F(InterpreterTest, NestedVisitationsWorkProperly) {
  auto expr = make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                           make_unique<IntegerExpr>(5)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(2),
                                make_unique<IntegerExpr>(1)));

  expr->Visit(&printer_);

  EXPECT_EQ(printer_.opstack.top(), 12);
}
