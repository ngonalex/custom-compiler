#ifndef TOKENIZER_TOKENIZER_H_
#define TOKENIZER_TOKENIZER_H_

#include "frontend/tokenizer/token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

#include "utility/assert.h"

using namespace cs160::frontend;

namespace cs160 {
namespace frontend {

class Tokenizer {
 public:
  explicit Tokenizer(std::string program) : input_program_(program) {
    char prevChar;
    Token::Type prevType = Token::NONE;
    std::string currString;

    for (char &c : input_program_) {
      if (c == ' ')
        continue;
      Token::Type type = ExtractType(c);
      if (type != Token::FAILED) {
        std::stringstream currStringStream;
        currStringStream << currString;
        currStringStream << c;

        currString = currStringStream.str();
        if (type != prevType) {
          if (type == Token::NUM) {
            Token newToken(type, atoi(currString.c_str()));
            tokens_.push_back(newToken);
          } else {
            Token newToken(type);
            tokens_.push_back(newToken);
          }
          currString = "";
        }
        prevChar = c;
        prevType = type;
      } else {
        Token newToken(Token::FAILED);
        tokens_[0] = (newToken);
        break;
        // Return just one token that is null
      }
    }
  }

  Token::Type ExtractType(const char testChar) {
    if (isdigit(testChar))
      return Token::NUM;
    switch (testChar) {
    case '(': {
      return Token::OPEN_PAREN;
    }
    case ')': {
      return Token::CLOSE_PAREN;
    }
    case '+': {
      return Token::ADD_OP;
    }
    case '-': {
      return Token::SUB_OP;
    }
    case '*': {
      return Token::MUL_OP;
    }
    case '/': {
      return Token::DIV_OP;
    }
    default: { return Token::FAILED; }
    }
  }

  std::string program() const { return input_program_; }
  std::vector<Token> tokens() const { return tokens_; }

 private:
  // Input to tokenizer is a file containing a program
  std::string input_program_;
  // Tokenizer takes the program and results in a list of Tokens
  std::vector<Token> tokens_;
  // Character that error occured (offset by # of characters)
  int errorPos_;
  // Line number that error occured (offset by # of \n)
  int errorLine_;
};

} // namespace frontend
} // namespace cs160

#endif // TOKENIZER_TOKENIZER_H_
