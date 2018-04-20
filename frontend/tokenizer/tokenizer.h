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
    Type prevType = NONE;
    std::string currString;

    for (char &c : input_program_) {
      if (c == ' ')
        continue;
      Type type = ExtractType(c);
      if (type != FAILED) {
        std::stringstream currStringStream;
        currStringStream << currString;
        currStringStream << c;

        currString = currStringStream.str();
        if (type != prevType) {
          if (type == NUM) {
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
        Token newToken(FAILED);
        tokens_[0] = (newToken);
        break;
        // Return just one token that is null
      }
    }
  }
  
  Type ExtractType(const char testChar) {
    if (isdigit(testChar))
      return NUM;
    switch (testChar) {
    case '(': {
      return OPEN_PAREN;
    }
    case ')': {
      return CLOSE_PAREN;
    }
    case '+': {
      return ADD_OP;
    }
    case '-': {
      return SUB_OP;
    }
    case '*': {
      return MUL_OP;
    }
    case '/': {
      return DIV_OP;
    }
    default: { return FAILED; }
    }
  }

  std::string program() const { return input_program_; }
  std::vector<Token> tokens() const { return tokens_; }

 private:
  // Input to tokenizer is a file containing a program
  std::string input_program_;
  // Tokenizer takes the program and results in a list of Tokens
  std::vector<Token> tokens_;
  int errorPos_;
  int errorLine_;
};

} // namespace frontend
} // namespace cs160

#endif // TOKENIZER_TOKENIZER_H_
