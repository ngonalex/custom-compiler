#include <utility>
#ifndef TOKEN_H_
#define TOKEN_H_

namespace cs160 {
namespace tokenizer {
enum tokenType = { NUM, PAREN, ADDSUB_OP, MULDIV_OP, NONE };

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

} // namespace tokenizer
} // namespace cs160

#endif // TOKEN_H_
