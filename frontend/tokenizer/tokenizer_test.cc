#include "frontend/tokenizer/tokenizer.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

TEST(Tokenizer, BasicAddTokenizer) {
  Tokenizer lexer("3 + 3");
  std::vector<Token> tokens;
  tokens.push_back(Token(Token::NUM, 3));
  tokens.push_back(Token(Token::ADD_OP));
  tokens.push_back(Token(Token::NUM, 3));
  tokens.push_back(Token(Token::END));

  EXPECT_EQ(tokens, lexer.tokens());
}

TEST(Tokenizer, ComplexExpression) {
  Tokenizer lexer("(3+3) * 4 / 2 - 1 =");
  std::vector<Token> tokens;
  tokens.push_back(Token(Token::OPEN_PAREN));
  tokens.push_back(Token(Token::NUM, 3));
  tokens.push_back(Token(Token::ADD_OP));
  tokens.push_back(Token(Token::NUM, 3));
  tokens.push_back(Token(Token::CLOSE_PAREN));
  tokens.push_back(Token(Token::MUL_OP));
  tokens.push_back(Token(Token::NUM, 4));
  tokens.push_back(Token(Token::DIV_OP));
  tokens.push_back(Token(Token::NUM, 2));
  tokens.push_back(Token(Token::SUB_OP));
  tokens.push_back(Token(Token::NUM, 1));
  tokens.push_back(Token(Token::EQUAL_SIGN));
  tokens.push_back(Token(Token::END));

  EXPECT_EQ(tokens, lexer.tokens());
}

TEST(Tokenizer, WeirdSpacing) {
  Tokenizer lexer("  (  3  +  3  )   *  4/2   -1 ");
  std::vector<Token> tokens;
  tokens.push_back(Token(Token::OPEN_PAREN));
  tokens.push_back(Token(Token::NUM, 3));
  tokens.push_back(Token(Token::ADD_OP));
  tokens.push_back(Token(Token::NUM, 3));
  tokens.push_back(Token(Token::CLOSE_PAREN));
  tokens.push_back(Token(Token::MUL_OP));
  tokens.push_back(Token(Token::NUM, 4));
  tokens.push_back(Token(Token::DIV_OP));
  tokens.push_back(Token(Token::NUM, 2));
  tokens.push_back(Token(Token::SUB_OP));
  tokens.push_back(Token(Token::NUM, 1));
  tokens.push_back(Token(Token::END));

  EXPECT_EQ(tokens, lexer.tokens());
}

TEST(Tokenizer, SingleLetterVariableName) {
  Tokenizer lexer("5 + x");
  std::vector<Token> tokens;
  tokens.push_back(Token(Token::NUM, 5));
  tokens.push_back(Token(Token::ADD_OP));
  tokens.push_back(Token(Token::VAR_NAME, "x"));

  EXPECT_EQ(tokens, lexer.tokens());
}

TEST(Tokenizer, MuliLetterVariableName) {
  Tokenizer lexer("5 + victor");
  std::vector<Token> tokens;
  tokens.push_back(Token(Token::NUM, 5));
  tokens.push_back(Token(Token::ADD_OP));
  tokens.push_back(Token(Token::VAR_NAME, "victor"));

  EXPECT_EQ(tokens, lexer.tokens());
}
