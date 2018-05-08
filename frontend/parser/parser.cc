#include "frontend/parser/parser.h"
#include "frontend/parser/parse_result.h"

using namespace cs160::frontend;

// Program is just a series of Assignments folowed be an Expr
std::unique_ptr<const Program> Parser::ParseProgram() {
  std::vector<std::unique_ptr<const Assignment>> assignments;
  while (true) {
    std::unique_ptr<const Assignment> a = ParseAssignment();
    if (a == NULL) {
      break;
    } else {
      assignments.push_back(std::move(a));
    }
  }
  std::unique_ptr<const ArithmeticExpr> expression = Eparser();
  Expect(Token::Type::ENDOFFILE);
  return (make_unique<const Program>(std::move(assignments), std::move(expression)));
}

// Assignment should follow the structure of
// VAR_NAME, ID, FIELD, ID, EQUALS, EXPR
// ID, EQUALS, EXPR
std::unique_ptr<const Assignment> Parser::ParseAssignment() {
  if (Next() == Token::Type::VAR_NAME) {
    Consume();
    std::unique_ptr<const VariableExpr> var = ParseVariable(program_.back());
    Expect(Token::Type::FIELD);
    ExpectID("int");
    Expect(Token::Type::EQUAL_SIGN);
    std::unique_ptr<const ArithmeticExpr> expr = Eparser();
    return make_unique<Assignment>(std::move(var), std::move(expr));
  } 
  if (Next() == Token::Type::IDENTIFIER) {
    if (DoubleNext() != Token::Type::EQUAL_SIGN) {
      // printf("Not double next\n");
      return NULL;
    }
    std::unique_ptr<const VariableExpr> var = ParseVariable(program_.back());
    Expect(Token::Type::EQUAL_SIGN);
    std::unique_ptr<const ArithmeticExpr> expr = Eparser();
  }
  else {
    // printf("Error Here\n");
    return NULL;
  }
}

std::unique_ptr<const VariableExpr> Parser::ParseVariable(Token curr) {
  if (curr.type() == Token::Type::IDENTIFIER && curr.idVal() != "int") {
    Consume();
    return make_unique<VariableExpr>(curr.idVal());
  } 
  Error();
  return nullptr;
}

std::unique_ptr<const ArithmeticExpr> Parser::Eparser() {
  std::unique_ptr<const ArithmeticExpr> t = ParseAddSub();
  Expect(Token::Type::END);
  return t;
}

std::unique_ptr<const ArithmeticExpr> Parser::ParseAddSub() {
  std::unique_ptr<const ArithmeticExpr> t = ParseMulDiv();
  Token::Type op = Next();
  while (op == Token::Type::ADD_OP || op == Token::Type::SUB_OP) {
    Consume();
    std::unique_ptr<const ArithmeticExpr> t1 = ParseMulDiv();
    t = MakeArithmeticExpr(op, std::move(t), std::move(t1));
    op = Next();
  }
  return t;
}

std::unique_ptr<const ArithmeticExpr> Parser::ParseMulDiv() {
  std::unique_ptr<const ArithmeticExpr> t = ParseExpression();
  // auto r1 = ParseExpression()
  // if (!r1.success) return Result::failure("asdasd", tok)
  
  Token::Type op = Next();
  while (op == Token::Type::MUL_OP || op == Token::Type::DIV_OP) {
    Consume();
    std::unique_ptr<const ArithmeticExpr> t1 = ParseExpression();
    // auto r2 = ParseExpr()
    // t = MakeArightExpr(op, std::move(r1.result), std::move(r2.result));
    t = MakeArithmeticExpr(op, std::move(t), std::move(t1));
    op = Next();
  }
  return t;
}

std::unique_ptr<const ArithmeticExpr> Parser::ParseExpression() {
  // Either returns an Int
  Token::Type type = Next();
  if (type == Token::Type::NUM) {
    std::unique_ptr<const ArithmeticExpr> t = MakeInteger(program_.back());
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
  // A variable
  else if (type == Token::Type::IDENTIFIER) {
    std::unique_ptr<const ArithmeticExpr> var = ParseVariable(program_.back());
    return var;
  } 
  // 0 - (expr)
  else if (type == Token::Type::SUB_OP) {
    Consume();
    std::unique_ptr<const ArithmeticExpr> t = ParseExpression();
    std::unique_ptr<const ArithmeticExpr> zero = MakeInteger(Token(Token::Type::NUM, 0));
    return make_unique<const SubtractExpr>(std::move(zero), std::move(t));
  }
  // Or an error
  else {
    Error();
    return NULL;
  }
}

// AST Expressions
std::unique_ptr<const ArithmeticExpr> Parser::MakeArithmeticExpr(Token::Type op, 
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

std::unique_ptr<const ArithmeticExpr> Parser::MakeInteger(Token num) {
  ASSERT(num.isNumber(), "Error creating IntegerExpr");
  return make_unique<IntegerExpr>(num.val());
}
