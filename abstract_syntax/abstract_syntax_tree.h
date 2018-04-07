#ifndef ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_H_
#define ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_H_

namespace cs160 {
namespace abstract_syntax {

// Forward declarations of abstract syntax tree node types for use in the
// abstract syntax tree visitor.
class IntegerExpr;
class AddExpr;
class SubtractExpr;
class MultiplyExpr;
class DivideExpr;

// The visitor abstract base class for visiting abstract syntax trees.
class AstVisitor {
 public:
  virtual ~AstVisitor() = 0;
  virtual void VisitIntegerExpr(const IntegerExpr& exp) const = 0;
  virtual void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp) const = 0;
  virtual void VisitAddExpr(const AddExpr& exp) const = 0;
  virtual void VisitSubtractExpr(const SubtractExpr& exp) const = 0;
  virtual void VisitMultiplyExpr(const MultiplyExpr& exp) const = 0;
  virtual void VisitDivideExpr(const DivideExpr& exp) const = 0;
};

// The definition of the abstract syntax tree abstract base class.
class AstNode {
 public:
  virtual ~AstNode() = 0;
  virtual void Visit(const AstVisitor* const visitor) const = 0;
};

// An abstract binary expression node.
class BinaryOperatorExpr : public AstNode {
 public:
  explicit BinaryOperatorExpr(const AstNode& lhs, const AstNode& rhs)
    : lhs_(lhs), rhs_(rhs) {}

  void Visit(const AstVisitor* const visitor) const {
    visitor->VisitBinaryOperatorExpr(*this);
  }

  const AstNode& lhs() const { return lhs_; }
  const AstNode& rhs() const { return rhs_; }

 protected:
  // The left-hand side and right-hand side of the expression.
  const AstNode& lhs_;
  const AstNode& rhs_;
};

// An integer constant expression.
class IntegerExpr : public AstNode {
 public:
  explicit IntegerExpr(int value)
    : value_(value) {}

  void Visit(const AstVisitor* const visitor) const {
    visitor->VisitIntegerExpr(*this);
  }

  int value() const { return value_; }

 private:
  // The value of the integer constant.
  int value_;
};

// An addition expression.
class AddExpr : public BinaryOperatorExpr {
 public:
  AddExpr(const AstNode& lhs, const AstNode& rhs)
    : BinaryOperatorExpr(lhs, rhs) {}

  void Visit(const AstVisitor* const visitor) const {
    visitor->VisitAddExpr(*this);
  }
};

// A subtraction expression.
class SubtractExpr : public BinaryOperatorExpr {
 public:
  SubtractExpr(const AstNode& lhs, const AstNode& rhs)
    : BinaryOperatorExpr(lhs, rhs) {}

  void Visit(const AstVisitor* const visitor) const {
    visitor->VisitSubtractExpr(*this);
  }
};

// A multiplication expression.
class MultiplyExpr : public BinaryOperatorExpr {
 public:
  MultiplyExpr(const AstNode& lhs, const AstNode& rhs)
    : BinaryOperatorExpr(lhs, rhs) {}

  void Visit(const AstVisitor* const visitor) const {
    visitor->VisitMultiplyExpr(*this);
  }
};

// A division expression.
class DivideExpr : public BinaryOperatorExpr {
 public:
  DivideExpr(const AstNode& lhs, const AstNode& rhs)
    : BinaryOperatorExpr(lhs, rhs) {}

  void Visit(const AstVisitor* const visitor) const {
    visitor->VisitDivideExpr(*this);
  }
};

}  // namespace abstract_syntax
}  // namespace cs160

#endif  // ABSTRACT_SYNTAX_ABSTRACT_SYNTAX_TREE_H_
