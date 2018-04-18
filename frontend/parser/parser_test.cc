#include "./parser.h"
#include "../tokenizer/tokenizer.h"
#include "gtest/gtest.h"

using cs160::frontend::Token;
using cs160::frontend::Tokenizer;
using cs160::frontend::Parser;

TEST(ParserTest, CanEvaluateSimpleAdd) {
  Token firstToken(Type::NUM, 6);
  Token secondToken(Type::DIV_OP);
  Token thirdToken(Type::NUM, 3);
  std::vector<Token> test_vector;
  test_vector.push_back(firstToken);
  test_vector.push_back(secondToken);
  test_vector.push_back(thirdToken);
  
  Parser parser(test_vector);
  EXPECT_EQ(parser.start(), 2);
}