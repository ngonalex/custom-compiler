#include "frontend/tokenizer/tokenizer.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

TEST(Tokenizer, basicAddTokenizer) {
  Tokenizer lexer("3 + 3");
  std::vector<Token> tokens;
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(ADD_OP));
  tokens.push_back(Token(NUM, 3));
  int i = 0;
  for (Token a : lexer.tokens()) {
    EXPECT_EQ(a, tokens[i]);
    i++;
  }
}

TEST(Tokenizer, complexExpression) {
  Tokenizer lexer("(3+3) * 4 / 2 - 1");
  std::vector<Token> tokens;
  tokens.push_back(Token(OPEN_PAREN));
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(ADD_OP));
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(CLOSE_PAREN));
  tokens.push_back(Token(MUL_OP));
  tokens.push_back(Token(NUM, 4));
  tokens.push_back(Token(DIV_OP));
  tokens.push_back(Token(NUM, 2));
  tokens.push_back(Token(SUB_OP));
  tokens.push_back(Token(NUM, 1));

  int i = 0;
  for (Token a : lexer.tokens()) {
    EXPECT_EQ(a, tokens[i]);
    i++;
  }
}

TEST(Tokenizer, WeirdSpacing) {
  Tokenizer lexer("  (  3  +  3  )   *  4/2   -1 ");
  std::vector<Token> tokens;
  tokens.push_back(Token(OPEN_PAREN));
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(ADD_OP));
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(CLOSE_PAREN));
  tokens.push_back(Token(MUL_OP));
  tokens.push_back(Token(NUM, 4));
  tokens.push_back(Token(DIV_OP));
  tokens.push_back(Token(NUM, 2));
  tokens.push_back(Token(SUB_OP));
  tokens.push_back(Token(NUM, 1));

  int i = 0;
  for (Token a : lexer.tokens()) {
    EXPECT_EQ(a, tokens[i]);
    i++;
  }
}

TEST(Tokenizer, UnexpectedToken) {
  Tokenizer lexer("5 + x");
  std::vector<Token> tokens;
  tokens.push_back(Token(FAILED));

  EXPECT_EQ(lexer.tokens()[0],tokens[0]);
}
