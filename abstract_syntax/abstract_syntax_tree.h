#ifndef ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_H_
#define ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_H_

#include <memory>
#include <stack>

namespace cs160 {
namespace abstract_syntax {

// Forward declarations of abstract syntax tree node types for use in the
// abstract syntax tree visitor.
class IntegerExpr;
class AddExpr;
class SubtractExpr;
class MultiplyExpr;
class DivideExpr;
class BinaryOperatorExpr;

// The visitor abstract base class for visiting abstract syntax trees.
class AstVisitor {
 public:
  virtual ~AstVisitor() {}

  //Not very general, this is probably a bad idea for future ASTS
  std::stack<int> opstack;

  // these should be able to change members of the visitor, thus not const
  virtual void VisitIntegerExpr(const IntegerExpr& exp) = 0;
  virtual void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp) = 0;
  virtual void VisitAddExpr(const AddExpr& exp) = 0;
  virtual void VisitSubtractExpr(const SubtractExpr& exp) = 0;
  virtual void VisitMultiplyExpr(const MultiplyExpr& exp) = 0;
  virtual void VisitDivideExpr(const DivideExpr& exp) = 0;
};

// The definition of the abstract syntax tree abstract base class.
class AstNode {
 public:
  virtual ~AstNode() {}

  virtual void Visit(AstVisitor* visitor) const = 0;
};

// An abstract binary expression node.
class BinaryOperatorExpr : public AstNode {
 public:
  BinaryOperatorExpr(std::unique_ptr<const AstNode> lhs,
                     std::unique_ptr<const AstNode> rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    visitor->VisitBinaryOperatorExpr(*this);
  }

  const AstNode& lhs() const { return *lhs_; }
  const AstNode& rhs() const { return *rhs_; }

 protected:
  // The left-hand side and right-hand side of the expression.
  const std::unique_ptr<const AstNode> lhs_;
  const std::unique_ptr<const AstNode> rhs_;
};

// An integer constant expression.
class IntegerExpr : public AstNode {
 public:
  explicit IntegerExpr(int value) : value_(value) {}

  void Visit(AstVisitor* visitor) const { visitor->VisitIntegerExpr(*this); }

  int value() const { return value_; }

 private:
  // The value of the integer constant.
  int value_;
};

// An addition expression.
class AddExpr : public BinaryOperatorExpr {
 public:
  AddExpr(std::unique_ptr<const AstNode> lhs,
          std::unique_ptr<const AstNode> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    BinaryOperatorExpr::Visit(visitor);
    visitor->VisitAddExpr(*const_cast<AddExpr*>(this));
  }
};

// A subtraction expression.
class SubtractExpr : public BinaryOperatorExpr {
 public:
  SubtractExpr(std::unique_ptr<const AstNode> lhs,
               std::unique_ptr<const AstNode> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    visitor->VisitSubtractExpr(*const_cast<SubtractExpr*>(this));
  }
};

// A multiplication expression.
class MultiplyExpr : public BinaryOperatorExpr {
 public:
  MultiplyExpr(std::unique_ptr<const AstNode> lhs,
               std::unique_ptr<const AstNode> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    visitor->VisitMultiplyExpr(*const_cast<MultiplyExpr*>(this));
  }
};

// A division expression.
class DivideExpr : public BinaryOperatorExpr {
 public:
  DivideExpr(std::unique_ptr<const AstNode> lhs,
             std::unique_ptr<const AstNode> rhs)
      : BinaryOperatorExpr(std::move(lhs), std::move(rhs)) {}

  void Visit(AstVisitor* visitor) const {
    visitor->VisitDivideExpr(*const_cast<DivideExpr*>(this));
  }
};

}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_H_
