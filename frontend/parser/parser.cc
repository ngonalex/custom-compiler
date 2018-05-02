#include "frontend/parser/parser.h"

using namespace cs160::frontend;

std::unique_ptr<const ArithmeticExpr> Parser::ParserProgram() {
  std::vector<std::unique_ptr<const AstNode>> assignments;
  // somehow loop through until an AE
    std::unique_ptr<const Assignment> a = ParseAssignment();
    assignments.push_back(a);
    
  std::unique_ptr<const ArithmeticExpr> t = ParseAddSub();
  Expect(Token::Type::END);
  return make_unique<Program>(std::move(assignments), std::move(t));
}

<<<<<<< HEAD
std::unique_ptr<const Assignment> Parser::ParserAssignment() {
  ExpectId('int');
  std::unique_ptr<const AstNode> var;
  if (Next() == Token::Type::VAR_NAME) {
    var = mkVar(program_.back());
  } else {
    Error();
  }
  Consume();
  Expect(Token::Type::EQUAL_SIGN);
  std::unique_ptr<const AstNode> expr = ParseAddSub();
  return make_unique<Assignment>(std::move(var), std::move(expr));
}

std::unique_ptr<const ArithmeticExpr> Parser::ParseAddSub() {
  std::unique_ptr<const ArithmeticExpr> t = ParseMulDiv();
=======
std::unique_ptr<const AstNode> Parser::Eparser() {
  std::unique_ptr<const AstNode> t = ParseLine();
  Expect(Token::Type::ENDOFFILE);
  return t;
}

std::unique_ptr<const AstNode> Parser::ParseLine(){
    std::unique_ptr<const AstNode> t = ParseAddSub();
    Expect(Token::Type::END);
    return t;
}

std::unique_ptr<const AstNode> Parser::ParseAddSub() {
  std::unique_ptr<const AstNode> t = ParseMulDiv();
>>>>>>> frontend_tokenizer_variableName_v2
  Token::Type op = Next();
  while (op == Token::Type::ADD_OP || op == Token::Type::SUB_OP) {
    Consume();
    std::unique_ptr<const ArithmeticExpr> t1 = ParseMulDiv();
    t = mkNode(op, std::move(t), std::move(t1));
    op = Next();
  }
  return t;
}

std::unique_ptr<const ArithmeticExpr> Parser::ParseMulDiv() {
  std::unique_ptr<const ArithmeticExpr> t = ParseExpression();
  Token::Type op = Next();
  while (op == Token::Type::MUL_OP || op == Token::Type::DIV_OP) {
    Consume();
    std::unique_ptr<const ArithmeticExpr> t1 = ParseExpression();
    t = mkNode(op, std::move(t), std::move(t1));
    op = Next();
  }
  return t;
}

std::unique_ptr<const ArithmeticExpr> Parser::ParseExpression() {
  // Either returns an Int
  Token::Type type = Next();
  if (type == Token::Type::NUM) {
    std::unique_ptr<const ArithmeticExpr> t = mkLeaf(program_.back());
    Consume();
    return t;
  }
  // An Expression
  else if (type == Token::Type::OPEN_PAREN) {
    Consume();
    std::unique_ptr<const ArithmeticExpr> t = ParseAddSub();
    Expect(Token::Type::CLOSE_PAREN);
    return t;
  } 
  else if (type == Token::Type::VAR_NAME {
    
  }
  // A variable
  else if (type == Token::Type::IDENTIFIER) {
    std::unique_ptr<const AstNode> var = mkVar(program_.back());
    Consume();
    return var;
  }
  // Or an error
  else {
    Error();
    return NULL;
  }
}
<<<<<<< HEAD


// AST Expressions
std::unique_ptr<const ArithmeticExpr> Parser::mkNode(Token::Type op, 
  std::unique_ptr<const ArithmeticExpr> first_leaf,
  std::unique_ptr<const ArithmeticExpr> second_leaf) {
  switch(op) {
    case Token::Type::ADD_OP: {
      return make_unique<const AddExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    case Token::Type::SUB_OP: {
      return make_unique<const SubtractExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    case Token::Type::MUL_OP: {
      return make_unique<const MultiplyExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    case Token::Type::DIV_OP: {
      return make_unique<const DivideExpr>(std::move(first_leaf), std::move(second_leaf));
    }
    default: {
      Error();
      return nullptr;
    }
  }
}

std::unique_ptr<const ArithmeticExpr> Parser::mkLeaf(Token num) {
  ASSERT(num.isNumber(), "Error creating IntegerExpr");
  return make_unique<IntegerExpr>(num.numVal());
}


std::unique_ptr<const ArithmeticExpr> Parser::mkVar(Token varName) {
  ASSERT(varName.isVar(), "Error creating VariableExpr");
  return make_unique<VariableExpr>(varName.stringVal());
}
=======
>>>>>>> frontend_tokenizer_variableName_v2
