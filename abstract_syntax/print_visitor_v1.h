#ifndef ABSTRACT_SYNTAX_PRINT_VISITOR_V1_H_
#define ABSTRACT_SYNTAX_PRINT_VISITOR_V1_H_

#include <sstream>
#include <string>

#include "abstract_syntax/abstract_syntax_tree_v1.h"

namespace cs160 {
namespace abstract_syntax {
namespace version_1 {

class PrintVisitor : public AstVisitor {
 public:
  PrintVisitor() {}
  ~PrintVisitor() {}

  const std::string GetOutput() const { return output_.str(); }

  void VisitIntegerExpr(const IntegerExpr& exp) { output_ << exp.value(); }

  void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp) {}

  void VisitAddExpr(const AddExpr& exp) {
    output_ << "(+ ";
    exp.lhs().Visit(this);
    output_ << " ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitSubtractExpr(const SubtractExpr& exp) {
    output_ << "(- ";
    exp.lhs().Visit(this);
    output_ << " ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) {
    output_ << "(* ";
    exp.lhs().Visit(this);
    output_ << " ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

  void VisitDivideExpr(const DivideExpr& exp) {
    output_ << "(/ ";
    exp.lhs().Visit(this);
    output_ << " ";
    exp.rhs().Visit(this);
    output_ << ")";
  }

 private:
  std::stringstream output_;
};

}  // namespace version_1
}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_PRINT_VISITOR_V1_H_
