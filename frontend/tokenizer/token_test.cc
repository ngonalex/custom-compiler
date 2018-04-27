#include "frontend/tokenizer/token.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

TEST(TokenTest, CanCreateTokensCorrectly) {
  Token integer_token(Token::NUM, 42);
  Token add_token(Token::ADD_OP);

  EXPECT_EQ(integer_token.type(), Token::NUM);
  EXPECT_EQ(integer_token.numVal(), 42);
  EXPECT_EQ(add_token.type(), Token::ADD_OP);
}

TEST(TokenDeathTest, CannotCreateNonIntegerTokensImproperly) {
  EXPECT_DEATH(Token add_token(Token::ADD_OP, 42), ".*");
}

TEST(TokenDeathTest, CannotCreateIntegerTokensImproperly) {
  EXPECT_DEATH(Token integer_token(Token::NUM), ".*");
}
