#include "gtest/gtest.h"
#include "./tokenizer.h"

TEST(Lexer, tokenize) {
  std::vector<Token> tokens;
  Token newToken = {3, 0, NUM};
  tokens.push_back(newToken);
  EXPECT_EQ(cs160::tokenizer::tokenize("3"), newToken);
}
