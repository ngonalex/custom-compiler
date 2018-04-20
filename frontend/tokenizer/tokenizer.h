#ifndef TOKENIZER_TOKENIZER_H_
#define TOKENIZER_TOKENIZER_H_

#include "frontend/tokenizer/token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

#include "utility/assert.h"

using namespace cs160::frontend;

namespace cs160 {
namespace frontend {

class Tokenizer {
 public:
  explicit Tokenizer(std::string program);
  std::vector<Token> Tokenize(std::string program);

  std::string program() const { return program_; }
  std::vector<Token> tokens() const { return tokens_; }

 private:
  std::string program_;
  std::vector<Token> tokens_;
  int errorPos_;
  int errorLine_;
};

} // namespace frontend
} // namespace cs160

#endif // TOKENIZER_TOKENIZER_H_
