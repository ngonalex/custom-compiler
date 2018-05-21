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
// y;
TEST(parser_v2_success, Assignment) {
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
  EXPECT_EQ(output, "x = 5; x");
}

// var x : int = 6;
// var y : int = 7;
// x + y;
TEST(parser_v2_success, SimpleProgram) {
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
  EXPECT_EQ(output, "x = 6; y = 7; (x + y)");
}

// var x : int = -10;
// x;
TEST(parser_v2_success, NegativeNums) {
  Token var(Token::Type::VAR_NAME);
  Token x(Token::Type::IDENTIFIER, "x");
  Token y(Token::Type::IDENTIFIER, "y");
  Token add(Token::Type::ADD_OP);
  Token field(Token::Type::FIELD);
  Token int_type(Token::Type::IDENTIFIER, "int");
  Token equals(Token::Type::EQUAL_SIGN);
  Token neg10(Token::Type::NUM, -10);
  Token seven(Token::Type::NUM, 7);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);  
  std::vector<Token> test_vector;
  // var x : int = -10;
  test_vector.push_back(var);
  test_vector.push_back(x);
  test_vector.push_back(field);
  test_vector.push_back(int_type);
  test_vector.push_back(equals);
  test_vector.push_back(neg10);
  test_vector.push_back(end);
  // x
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
  EXPECT_EQ(output, "x = -10; x");
}

TEST(parser_v2_success, RealNegation) {
  // 5 + -5;
  Token five(Token::Type::NUM, 5);
  Token plus(Token::Type::ADD_OP);
  Token minus(Token::Type::SUB_OP);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);  
  std::vector<Token> test_vector;
  test_vector.push_back(five);
  test_vector.push_back(plus);
  test_vector.push_back(minus);
  test_vector.push_back(five);
  test_vector.push_back(end);
  test_vector.push_back(endfile);
  
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();
  
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  // 
  EXPECT_EQ(output, "(5 + (0 - 5))");
}

TEST(parser_v2_success, RealNegationWithParen) {
  // 5 * -(9 - 5);
  Token five(Token::Type::NUM, 5);
  Token plus(Token::Type::ADD_OP);
  Token minus(Token::Type::SUB_OP);
  Token times(Token::Type::MUL_OP);
  Token nine(Token::Type::NUM, 9);
  Token open(Token::Type::OPEN_PAREN);
  Token close(Token::Type::CLOSE_PAREN);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);  
  std::vector<Token> test_vector;
  test_vector.push_back(five);
  test_vector.push_back(times);
  test_vector.push_back(minus);
  test_vector.push_back(open);
  test_vector.push_back(nine);
  test_vector.push_back(minus);
  test_vector.push_back(five);
  test_vector.push_back(close);
  test_vector.push_back(end);
  test_vector.push_back(endfile);
  
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();
  
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  // 
  EXPECT_EQ(output, "(5 * (0 - (9 - 5)))");
}

// x = 0 + 0;
// x;
TEST(Parser, AltAssignment) {
  Token var(Token::Type::VAR_NAME);
  Token x(Token::Type::IDENTIFIER, "x");
  Token y(Token::Type::IDENTIFIER, "y");
  Token z(Token::Type::IDENTIFIER, "z");
  Token field(Token::Type::FIELD);
  Token int_type(Token::Type::IDENTIFIER, "int");   
  Token equals(Token::Type::EQUAL_SIGN);
  Token zero(Token::Type::NUM, 0);   
  Token add(Token::Type::ADD_OP);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE); 
  std::vector<Token> test_vector;

  // x = 0 + 0;
  test_vector.push_back(x);
  test_vector.push_back(equals);
  test_vector.push_back(zero);
  test_vector.push_back(add);
  test_vector.push_back(zero);
  test_vector.push_back(end);

  // x;
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
  EXPECT_EQ(output, "x = (0 + 0); x");
}


// var x : int = 99;
// var y : int = 110101;
// var z : int = 0;
// z = x + y;
// z;
// 
// x = 10 + 10;
// x;

// VAR X : INT = SUB 99;
// SUB 99
// 99 SUB 99
TEST(Parser, Comp) {
  Token var(Token::Type::VAR_NAME);
  Token x(Token::Type::IDENTIFIER, "x");
  Token y(Token::Type::IDENTIFIER, "y");
  Token z(Token::Type::IDENTIFIER, "z");
  Token field(Token::Type::FIELD);
  Token int_type(Token::Type::IDENTIFIER, "int");   
  Token equals(Token::Type::EQUAL_SIGN);
  Token neg9(Token::Type::NUM, -99);
  Token huge(Token::Type::NUM, 110101);
  Token zero(Token::Type::NUM, 0);   
  Token add(Token::Type::ADD_OP);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE); 
  std::vector<Token> test_vector;

  // var x : int = -99;
  test_vector.push_back(var);
  test_vector.push_back(x);
  test_vector.push_back(field);
  test_vector.push_back(int_type);
  test_vector.push_back(equals);
  test_vector.push_back(neg9);
  test_vector.push_back(end);

  // var y : int = 110101;
  test_vector.push_back(var);
  test_vector.push_back(y);
  test_vector.push_back(field);
  test_vector.push_back(int_type); 
  test_vector.push_back(equals);
  test_vector.push_back(huge);
  test_vector.push_back(end);

  // var z : int = 0;
  test_vector.push_back(var);
  test_vector.push_back(z);
  test_vector.push_back(field);
  test_vector.push_back(int_type);
  test_vector.push_back(equals);
  test_vector.push_back(zero);
  test_vector.push_back(end);  

  // z = x + y;
  test_vector.push_back(z);
  test_vector.push_back(equals);
  test_vector.push_back(x);
  test_vector.push_back(add);
  test_vector.push_back(y);
  test_vector.push_back(end);  

  // z;
  test_vector.push_back(z);
  test_vector.push_back(end); 

  // EOF
  test_vector.push_back(endfile);

  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();
  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  // 
  EXPECT_EQ(output, "x = -99; y = 110101; z = 0; z = (x + y); z");
}


