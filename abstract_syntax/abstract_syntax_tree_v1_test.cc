#include "abstract_syntax/abstract_syntax_tree_v1.h"
#include "gtest/gtest.h"
#include "utility/memory.h"

using cs160::abstract_syntax::version_1::AstVisitor;
using cs160::abstract_syntax::version_1::AddExpr;
using cs160::abstract_syntax::version_1::SubtractExpr;
using cs160::abstract_syntax::version_1::MultiplyExpr;
using cs160::abstract_syntax::version_1::DivideExpr;
using cs160::abstract_syntax::version_1::IntegerExpr;

using cs160::make_unique;

namespace {

// Counts the number of AST nodes.
class CountVisitor : public AstVisitor {
 public:
  void VisitIntegerExpr(const IntegerExpr& exp) override { count_++; }

  void VisitAddExpr(const AddExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitSubtractExpr(const SubtractExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  void VisitDivideExpr(const DivideExpr& exp) override {
    exp.lhs().Visit(this);
    exp.rhs().Visit(this);
    count_++;
  }

  int count() { return count_; }

 private:
  int count_ = 0;
};

}  // namespace

TEST(AstTestV1, CanCreateAst) {
  auto ast = make_unique<const AddExpr>(
      make_unique<const SubtractExpr>(
          make_unique<const DivideExpr>(make_unique<const IntegerExpr>(12),
                                        make_unique<const IntegerExpr>(3)),
          make_unique<const IntegerExpr>(4)),
      make_unique<const MultiplyExpr>(make_unique<const IntegerExpr>(3),
                                      make_unique<const IntegerExpr>(2)));

  CountVisitor counter;
  ast->Visit(&counter);

  EXPECT_EQ(counter.count(), 9);
}
