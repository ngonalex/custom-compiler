#include <utility>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#ifndef TOKENIZER_TOKENIZER_H_
#define TOKENIZER_TOKENIZER_H_

namespace cs160 {
namespace tokenizer {
enum tokenType = {NUM, PAREN, ADDSUB_OP, MULDIV_OP, NONE};

struct Token {
  std::string char *token_val;
  std::pair<int, int> token_pos;  // line number, character number
  enum type;
};

// This is a class that should own a member of type Item.
class Tokenizer {
 public:
  explicit Tokenizer(std::string program);

 private:
  std::string program;
  int pos;
  int line;

  vector<Token> Tokenize();
};

}  // namespace tokenizer
}  // namespace cs160

#endif  // TOKENIZER_TOKENIZER_H_
