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

  explicit Tokenizer(std::string program);

  Token::Type ExtractType(const char testChar);

  /*
    1) start with lowercase or uppercase letter or underscore
    2) can have lowercase, uppercase, underscore, or numbers
  */
  bool ValidVariableName(const std::string variableName);

  bool ValidIdentifierName(const std::string variableName);

  bool ValidNumber(const std::string variableName);

  // potential token value -> Token Object
  Token CreateTokenFromExpression(std::string tokenVal);

  // Getter function
  std::string program() const { return input_program_; }

  // Getter function
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
