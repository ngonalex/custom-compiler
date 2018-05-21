#include "abstract_syntax/abstract_syntax_tree_v2.h"
#include "utility/memory.h"

#include "gtest/gtest.h"

using cs160::abstract_syntax::version_2::AstVisitor;
using cs160::abstract_syntax::version_2::AddExpr;
using cs160::abstract_syntax::version_2::SubtractExpr;
using cs160::abstract_syntax::version_2::MultiplyExpr;
using cs160::abstract_syntax::version_2::DivideExpr;
using cs160::abstract_syntax::version_2::IntegerExpr;
using cs160::abstract_syntax::version_2::VariableExpr;
using cs160::abstract_syntax::version_2::Assignment;
using cs160::abstract_syntax::version_2::Program;

using cs160::make_unique;

namespace {

// Counts the number of AST nodes.
class CountVisitor : public AstVisitor {
 public:
  void VisitIntegerExpr(const IntegerExpr& exp) override { count_++; }

  void VisitVariableExpr(const VariableExpr& exp) override { count_++; }

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

  void VisitAssignment(const Assignment& assignment) override {
    assignment.rhs().Visit(this);
    count_++;
  }

  void VisitProgram(const Program& program) override {
    for (auto& assignment : program.assignments()) {
      assignment->Visit(this);
    }
    program.arithmetic_exp().Visit(this);
  }

  int count() { return count_; }

 private:
  int count_ = 0;
};

}  // namespace

TEST(AstTestV2, CanCreateAst) {
  std::vector<std::unique_ptr<const Assignment>> assignments;
  assignments.push_back(std::move(
      make_unique<const Assignment>(make_unique<const VariableExpr>("bob"),
                                    make_unique<const IntegerExpr>(42))));

  auto ae = make_unique<const AddExpr>(
      make_unique<const SubtractExpr>(
          make_unique<const DivideExpr>(make_unique<const IntegerExpr>(12),
                                        make_unique<const IntegerExpr>(3)),
          make_unique<const IntegerExpr>(4)),
      make_unique<const MultiplyExpr>(make_unique<const IntegerExpr>(3),
                                      make_unique<const IntegerExpr>(2)));

  auto ast = make_unique<const Program>(std::move(assignments), std::move(ae));

  CountVisitor counter;
  ast->Visit(&counter);

  EXPECT_EQ(counter.count(), 11);
}
