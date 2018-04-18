#include "./tokenizer.h"
#include "gtest/gtest.h"
using namespace cs160::tokenizer;

/*
for reference:::

enum Type {
  NUM, OPEN_PAREN, CLOSE_PAREN, ADD_OP, SUB_OP, MUL_OP, DIV_OP, NONE
};
*/
TEST(TokenTest, CanCreateTokensCorrectly) {
  Token integer_token(NUM, 42);
  Token add_token(ADD_OP);

  EXPECT_EQ(integer_token.type(), Token::Type::kInteger);
  EXPECT_EQ(integer_token.Val(), 42);
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
  Tokenizer lexer("3+3");
  std::vector<Token> tokens;
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(ADD_OP));
  tokens.push_back(Token(NUM, 3));
  for(Token a : lexer.tokens_){
    EXPECT_EQ(a, tokens);
  }
}
check invalid syntax if that's true
*/
