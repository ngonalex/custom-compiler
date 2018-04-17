#include "./tokenizer.h"
#include "gtest/gtest.h"
using cs160::tokenizer::Token;
using cs160::tokenizer::Tokenizer;

TEST(TokenTest, CanCreateTokensCorrectly) {
  Token integer_token(Token::Type::kInteger, 42);
  Token add_token(Token::Type::kAdd);

  EXPECT_EQ(integer_token.type(), Token::Type::kInteger);
  EXPECT_EQ(integer_token.value(), 42);
  EXPECT_EQ(add_token.type(), Token::Type::kAdd);
}

TEST(TokenDeathTest, CannotCreateNonIntegerTokensImproperly) {
  EXPECT_DEATH(Token add_token(Token::Type::kAdd, 42), ".*");
}

TEST(TokenDeathTest, CannotCreateIntegerTokensImproperly) {
  EXPECT_DEATH(Token integer_token(Token::Type::kInteger), ".*");
}
/*
TEST(Tokenizer, tokenize) {
  std::vector<Token> tokens;
  Token newToken = {3, 0, NUM};
  tokens.push_back(newToken);
  EXPECT_EQ(cs160::tokenizer::tokenize("3"), newToken);
}
check invalid syntax if that's true
*/
