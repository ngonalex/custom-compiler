#ifndef TOKENIZER_TOKENIZER_H_
#define TOKENIZER_TOKENIZER_H_

#include "utility/assert.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

namespace cs160 {
namespace frontend {
enum Type {
  NUM,
  OPEN_PAREN,
  CLOSE_PAREN,
  ADD_OP,
  SUB_OP,
  MUL_OP,
  DIV_OP,
  FAILED,
  NONE
};
class Token {
public:
  // Constructor used for creating an empty Token
  Token() { this->type_ = Type::NONE; }
  // Constructor for Non-NUM Token
  explicit Token(Type type) : type_(type), val_(0) {
    ASSERT(type != Type::NUM, "Integer tokens need a val");
  }
  // Constructors for Integer Tokens
  Token(Type type, int val) : type_(type), val_(val) {
    ASSERT(type == Type::NUM, "Only integers have val declared");
  }

  // check if two Tokens are equal
  bool operator==(const Token &b) const {
    if (this->type_ == NUM) {
      if (this->type_ == b.type_ && this->val_ == b.val_)
        return true;
      else
        return false;
    } else {
      if (this->type_ == b.type_)
        return true;
      else
        return false;
    }
  }
  bool operator!=(const Token &b) const {
    if (this->type_ == NUM) {
      if (this->type_ == b.type_ && this->val_ == b.val_)
        return false;
      else
        return true;
    } else {
      if (this->type_ == b.type_)
        return false;
      else
        return true;
    }
  }
  // Debug function
  void print();

  void print();

  // Getter functions
  Type type() const { return type_; }
  int val() const {
    ASSERT(type_ == Type::NUM, "Only integer tokens have value");
    return val_;
  }

private:
  Type type_;
  // Default value is set to 0 for non-integer scores
  int val_;
  // TODO add token position in constructor
  std::pair<int, int> tokenPos_; // line number, character number
};

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
