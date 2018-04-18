#include "./tokenizer.h"
#include "gtest/gtest.h"
using namespace cs160::frontend;

/*
for reference:::

enum Type {
  NUM, OPEN_PAREN, CLOSE_PAREN, ADD_OP, SUB_OP, MUL_OP, DIV_OP, NONE
};
*/
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

// TEST(Tokenizer, basicAddTokenizer) {
//   Tokenizer lexer("3+3");
//   std::vector<Token> tokens;
//   tokens.push_back(Token(NUM, 3));
//   tokens.push_back(Token(ADD_OP));
//   tokens.push_back(Token(NUM, 3));
//   int i = 0;
//   for(Token a : lexer.tokens()){
//     std::cout << a.type() << std::endl;
//     if(a.type() == NUM)
//       std::cout << a.val() << std::endl;
//     EXPECT_EQ(a, tokens[i]);
//     i++;
//   }
// }
