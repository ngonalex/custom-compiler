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
  return make_unique<IntegerExpr>(num.numVal());
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

std::unique_ptr<const AstNode> Parser::mkVar(Token varName) {
  ASSERT(varName.isVar(), "Error creating VariableExpr");
  return make_unique<VariableExpr>(varName.stringVal());
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
  // An assignment
  else if (type == Token::Type::IDENTIFIER) {
    // Could possibly have
    // int x = 5
    // int y = x <-- NOT IMPLEMENTED RIGHT NOW
    // int z;
    // if (type.stringVal() != "int") {
    //   Error();
    // }
    Consume();
    Expect(Token::Type::VAR_NAME);
    std::unique_ptr<const AstNode> var = mkVar(program_.back());
    Consume();
    Expect(Token::Type::EQUAL_SIGN);
    Consume();
    std::unique_ptr<const AstNode> expr = ParseAddSub();
    return make_unique<Assignment>(std::move(var), std::move(expr));
  }
  // Or an error
  else {
    Error();
    return NULL;
  }
}