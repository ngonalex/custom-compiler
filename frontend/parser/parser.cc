#include "frontend/parser/parser.h"

using namespace cs160::frontend;

// AST Expressions
std::unique_ptr<const AstNode> Parser::mkNode(Token::Type op, 
  std::unique_ptr<const AstNode> first_leaf,
  std::unique_ptr<const AstNode> second_leaf) {
  switch(op) {
    case Token::Type::ADD_OP: {
      return make_unique<AddExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    case Token::Type::SUB_OP: {
      return make_unique<SubtractExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    case Token::Type::MUL_OP: {
      return make_unique<MultiplyExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    case Token::Type::DIV_OP: {
      return make_unique<DivideExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    default: {
      Error();
      return nullptr;
    }
  }
}

std::unique_ptr<const AstNode> Parser::mkLeaf(Token num) {
  ASSERT(num.isNumber(), "Error creating IntegerExpr");
  return make_unique<IntegerExpr>(num.val());
}

std::unique_ptr<const AstNode> Parser::Eparser() {
  std::unique_ptr<const AstNode> t = ParseAddSub();
  Expect(Token::Type::END);
  return t;
}

std::unique_ptr<const AstNode> Parser::ParseAddSub() {
  std::unique_ptr<const AstNode> t = ParseMulDiv();
  Token::Type op = Next();
  while (op == Token::Type::ADD_OP || op == Token::Type::SUB_OP) {
    Consume();
    std::unique_ptr<const AstNode> t1 = ParseMulDiv();
    t = mkNode(op, std::move(t), std::move(t1));
    op = Next();
  }
  return t;
}

std::unique_ptr<const AstNode> Parser::ParseMulDiv() {
  std::unique_ptr<const AstNode> t = ParseExpression();
  Token::Type op = Next();
  while (op == Token::Type::MUL_OP || op == Token::Type::DIV_OP) {
    Consume();
    std::unique_ptr<const AstNode> t1 = ParseExpression();
    t = mkNode(op, std::move(t), std::move(t1));
    op = Next();
  }
  return t;
}

std::unique_ptr<const AstNode> Parser::ParseExpression() {
  // Either returns an Int
    Token::Type type = Next();
  if (type == Token::Type::NUM) {
    std::unique_ptr<const AstNode> t = mkLeaf(program_.back());
    Consume();
    return t;
  }
  // An Expression
  else if (type == Token::Type::OPEN_PAREN) {
    Consume();
    std::unique_ptr<const AstNode> t = ParseAddSub();
    Expect(Token::Type::CLOSE_PAREN);
    return t;
  }
  // Or an error
  else {
    Error();
    return NULL;
  }
}