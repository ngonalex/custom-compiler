#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/parser/parser.h"
#include "frontend/tokenizer/token.h"
#include "frontend/tokenizer/tokenizer.h"

#include "gtest/gtest.h"

using namespace cs160::abstract_syntax::frontend;
using cs160::frontend::Parser;
using cs160::frontend::Token;

TEST(Parser, CanParseMultiplication) {
  // program vector: END, 3, MUL_OP, 6
  Token firstToken(Token::Type::NUM, 6);
  Token secondToken(Token::Type::MUL_OP);
  Token thirdToken(Token::Type::NUM, 3);
  Token fourthToken(Token::Type::END);
  std::vector<Token> test_vector;
  test_vector.push_back(firstToken);
  test_vector.push_back(secondToken);
  test_vector.push_back(thirdToken);
  test_vector.push_back(fourthToken);

  Parser parser(test_vector);
  std::unique_ptr<const AstNode> result = parser.Eparser();
  
  PrintVisitor *a = new PrintVisitor();
  result->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(output, "(* 6 3)");
}
