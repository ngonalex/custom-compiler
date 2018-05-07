#include "abstract_syntax/abstract_syntax.h"
#include "frontend/parser/parser.h"
#include "frontend/tokenizer/token.h"
#include "frontend/tokenizer/tokenizer.h"
#include "abstract_syntax/print_visitor_v2.h"

#include "gtest/gtest.h"

using namespace cs160::abstract_syntax::frontend;
using cs160::frontend::Parser;
using cs160::frontend::Token;

// var x : int = 5;
// x;
TEST(Parser, Assignment) {
  Token var(Token::Type::VAR_NAME);
  Token x(Token::Type::IDENTIFIER, "x");
  Token field(Token::Type::FIELD);
  Token int_type(Token::Type::IDENTIFIER, "int");
  Token equals(Token::Type::EQUAL_SIGN);
  Token five(Token::Type::NUM, 5);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);  
  std::vector<Token> test_vector;
  test_vector.push_back(var);
  test_vector.push_back(x);
  test_vector.push_back(field);
  test_vector.push_back(int_type);
  test_vector.push_back(equals);
  test_vector.push_back(five);
  test_vector.push_back(end);
  test_vector.push_back(x);
  test_vector.push_back(end);
  test_vector.push_back(endfile);
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();
  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  // 
  EXPECT_EQ(output, "(= 5); x");
}

// var x : int = 6;
// var y : int = 7;
// x + y;
TEST(Parser, SimpleProgram) {
  Token var(Token::Type::VAR_NAME);
  Token x(Token::Type::IDENTIFIER, "x");
  Token y(Token::Type::IDENTIFIER, "y");
  Token add(Token::Type::ADD_OP);
  Token field(Token::Type::FIELD);
  Token int_type(Token::Type::IDENTIFIER, "int");
  Token equals(Token::Type::EQUAL_SIGN);
  Token six(Token::Type::NUM, 6);
  Token seven(Token::Type::NUM, 7);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);  
  std::vector<Token> test_vector;
  // var x : int = 6;
  test_vector.push_back(var);
  test_vector.push_back(x);
  test_vector.push_back(field);
  test_vector.push_back(int_type);
  test_vector.push_back(equals);
  test_vector.push_back(six);
  test_vector.push_back(end);
  // var y : int = 7;
  test_vector.push_back(var);
  test_vector.push_back(y);
  test_vector.push_back(field);
  test_vector.push_back(int_type);
  test_vector.push_back(equals);
  test_vector.push_back(seven);
  test_vector.push_back(end);
  // x + y
  test_vector.push_back(x);
  test_vector.push_back(add);
  test_vector.push_back(y);
  test_vector.push_back(end);
  
  test_vector.push_back(endfile);
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();
  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  // 
  EXPECT_EQ(output, "(= 6); (= 7); (+ x y)");
}