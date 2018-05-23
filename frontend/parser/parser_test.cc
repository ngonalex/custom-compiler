#include "frontend/parser/parser.h"
#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v2.h"
#include "frontend/tokenizer/token.h"
#include "frontend/tokenizer/tokenizer.h"

#include "gtest/gtest.h"

using namespace cs160::abstract_syntax::frontend;
using cs160::frontend::Parser;
using cs160::frontend::Token;

TEST(parser_v1_success, CanParseInt) {
  // Initialize the vector
  Token ten(Token::Type::NUM, 10);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  test_vector.push_back(ten);
  test_vector.push_back(end);
  test_vector.push_back(endfile);

  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const Program> result = parser.ParseProgram();

  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "10");
}

TEST(parser_v1_success, CanParseAddition) {
  // Initialize the vector
  Token firstToken(Token::Type::NUM, 10);
  Token secondToken(Token::Type::ADD_OP);
  Token thirdToken(Token::Type::NUM, 5);
  Token fourthToken(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  test_vector.push_back(firstToken);
  test_vector.push_back(secondToken);
  test_vector.push_back(thirdToken);
  test_vector.push_back(fourthToken);
  test_vector.push_back(endfile);

  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "(10 + 5)");
}

TEST(parser_v1_success, CanParseSubtraction) {
  // Initialize the vector
  Token firstToken(Token::Type::NUM, 9);
  Token secondToken(Token::Type::SUB_OP);
  Token thirdToken(Token::Type::NUM, 3);
  Token fourthToken(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  test_vector.push_back(firstToken);
  test_vector.push_back(secondToken);
  test_vector.push_back(thirdToken);
  test_vector.push_back(fourthToken);
  test_vector.push_back(endfile);

  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "(9 - 3)");
}

TEST(parser_v1_success, CanParseMultiplication) {
  // Initialize the vector
  Token firstToken(Token::Type::NUM, 6);
  Token secondToken(Token::Type::MUL_OP);
  Token thirdToken(Token::Type::NUM, 3);
  Token fourthToken(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  test_vector.push_back(firstToken);
  test_vector.push_back(secondToken);
  test_vector.push_back(thirdToken);
  test_vector.push_back(fourthToken);
  test_vector.push_back(endfile);
  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "(6 * 3)");
}

TEST(parser_v1_success, CanParseDivision) {
  // Initialize the vector
  Token firstToken(Token::Type::NUM, 10);
  Token secondToken(Token::Type::DIV_OP);
  Token thirdToken(Token::Type::NUM, 20);
  Token fourthToken(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  test_vector.push_back(firstToken);
  test_vector.push_back(secondToken);
  test_vector.push_back(thirdToken);
  test_vector.push_back(fourthToken);
  test_vector.push_back(endfile);

  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "(10 / 20)");
}

TEST(parser_v1_success, CanDoPemdas) {
  // Initialize the vector
  Token ten(Token::Type::NUM, 10);
  Token five(Token::Type::NUM, 5);
  Token plus(Token::Type::ADD_OP);
  Token minus(Token::Type::SUB_OP);
  Token mult(Token::Type::MUL_OP);
  Token divi(Token::Type::DIV_OP);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  // 10 + 10 - 5 * 10 * 10 * 5
  // (- (+ 10 10) (* (* (* 5 10) 10) 5))
  test_vector.push_back(ten);
  test_vector.push_back(plus);
  test_vector.push_back(ten);
  test_vector.push_back(minus);
  test_vector.push_back(five);
  test_vector.push_back(mult);
  test_vector.push_back(ten);
  test_vector.push_back(mult);
  test_vector.push_back(ten);
  test_vector.push_back(mult);
  test_vector.push_back(five);
  test_vector.push_back(end);
  test_vector.push_back(endfile);
  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "((10 + 10) - (((5 * 10) * 10) * 5))");
}

TEST(parser_v1_success, CanDoParens) {
  // Initialize the vector
  Token ten(Token::Type::NUM, 10);
  Token five(Token::Type::NUM, 5);
  Token plus(Token::Type::ADD_OP);
  Token minus(Token::Type::SUB_OP);
  Token mult(Token::Type::MUL_OP);
  Token open(Token::Type::OPEN_PAREN);
  Token close(Token::Type::CLOSE_PAREN);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  // 10 + (10 - 5) * 10 * 10 * 5
  test_vector.push_back(ten);
  test_vector.push_back(plus);
  test_vector.push_back(open);
  test_vector.push_back(ten);
  test_vector.push_back(minus);
  test_vector.push_back(five);
  test_vector.push_back(close);
  test_vector.push_back(mult);
  test_vector.push_back(ten);
  test_vector.push_back(mult);
  test_vector.push_back(ten);
  test_vector.push_back(mult);
  test_vector.push_back(five);
  test_vector.push_back(end);
  test_vector.push_back(endfile);
  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "(10 + ((((10 - 5) * 10) * 10) * 5))");
}

TEST(parser_v1_success, StressTest) {
  Token seven(Token::Type::NUM, 7);
  Token ten(Token::Type::NUM, 10);
  Token nine(Token::Type::NUM, 9);
  Token three(Token::Type::NUM, 3);
  Token sixteen(Token::Type::NUM, 16);
  Token eight(Token::Type::NUM, 8);
  Token sevenseven(Token::Type::NUM, 77);
  Token two(Token::Type::NUM, 2);
  Token twelve(Token::Type::NUM, 12);
  Token one(Token::Type::NUM, 1);
  Token plus(Token::Type::ADD_OP);
  Token minus(Token::Type::SUB_OP);
  Token mult(Token::Type::MUL_OP);
  Token divi(Token::Type::DIV_OP);
  Token open(Token::Type::OPEN_PAREN);
  Token close(Token::Type::CLOSE_PAREN);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  // 7 * 10 + 9 / 3 + 16 - 8 * 2 * 3 - 77 + 12 * 1
  std::vector<Token> test_vector;
  test_vector.push_back(seven);
  test_vector.push_back(mult);
  test_vector.push_back(ten);
  test_vector.push_back(plus);
  test_vector.push_back(nine);
  test_vector.push_back(divi);
  test_vector.push_back(three);
  test_vector.push_back(plus);
  test_vector.push_back(sixteen);
  test_vector.push_back(minus);
  test_vector.push_back(eight);
  test_vector.push_back(mult);
  test_vector.push_back(two);
  test_vector.push_back(mult);
  test_vector.push_back(three);
  test_vector.push_back(minus);
  test_vector.push_back(sevenseven);
  test_vector.push_back(plus);
  test_vector.push_back(twelve);
  test_vector.push_back(mult);
  test_vector.push_back(one);
  test_vector.push_back(end);
  test_vector.push_back(endfile);
  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(
      output,
      "((((((7 * 10) + (9 / 3)) + 16) - ((8 * 2) * 3)) - 77) + (12 * 1))");
}

TEST(parser_v1_success, SingleParen) {
  Token close(Token::Type::CLOSE_PAREN);
  Token five(Token::Type::NUM, 5);
  Token open(Token::Type::OPEN_PAREN);
  Token end(Token::Type::END);
  Token endfile(Token::Type::ENDOFFILE);
  std::vector<Token> test_vector;
  test_vector.push_back(open);
  test_vector.push_back(five);
  test_vector.push_back(close);
  test_vector.push_back(end);
  test_vector.push_back(endfile);
  // Push it through the parser
  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.ParseProgram();

  // Read output
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(output, "5");
}
