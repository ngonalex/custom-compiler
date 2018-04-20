#include "frontend/tokenizer/token.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

TEST(TokenTest, CanCreateTokensCorrectly) {
  Token integer_token(NUM, 42);
  Token add_token(ADD_OP);

  EXPECT_EQ(integer_token.type(), NUM);
  EXPECT_EQ(integer_token.val(), 42);
  EXPECT_EQ(add_token.type(), ADD_OP);
}

TEST(TokenDeathTest, CannotCreateNonIntegerTokensImproperly) {
  EXPECT_DEATH(Token add_token(ADD_OP, 42), ".*");
}

TEST(TokenDeathTest, CannotCreateIntegerTokensImproperly) {
  EXPECT_DEATH(Token integer_token(NUM), ".*");
}
