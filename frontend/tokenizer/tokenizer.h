#ifndef TOKENIZER_TOKENIZER_H_
#define TOKENIZER_TOKENIZER_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>


namespace cs160 {
namespace tokenizer {
class Token {
 public:
  enum Type = {
    NUM, OPEN_PAREN, CLOSE_PAREN, ADD_OP, SUB_OP, MUL_OP, DIV_OP, NONE
  };
  // Constructor for Non-NUM Tokens
  explicit Token(Type type) : type_(type), val_(0) {
    ASSERT(type != Type::NUM, "Integer tokens need a val");
  }
  // Constructors for Integer Tokens
  Token(Type type, int val) : type_(type), val_(val) {
    ASSERT(type == Type::NUM, "Only integers have val declared");
  }

  // Getter functions
  Type getType() const { return type_; };
  int getVal() const {
    ASSERT(type_ == Type::NUM, "Only integer tokens have value");
    return val_;
  };

 private:
  Type type_;
  // Default value is set to 0 for non-integer scores
  int val_;
  // TODO add token position in constructor
  std::pair<int, int> tokenPos_;  // line number, character number
}

class Tokenizer {
 public:
  explicit Tokenizer(std::string program) : program_(program);
  std::vector<Token> Tokenize();

 private:
  std::string program_;
  int pos_;
  int line_;
};

}  // namespace tokenizer
}  // namespace cs160

#endif // TOKENIZER_TOKENIZER_H_
