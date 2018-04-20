#ifndef TOKEN_TOKEN_H
#define TOKEN_TOKEN_H

#include "utility/assert.h"

namespace cs160 {
namespace frontend {

class Token {
 public:
  enum Type { NUM, OPEN_PAREN, CLOSE_PAREN, ADD_OP,
   SUB_OP, MUL_OP, DIV_OP, END, FAILED, NONE };
  // Constructor for Non-NUM Tokens
  explicit Token(Type type) : type_(type), val_(0) {
    ASSERT(type != Type::NUM, "Integer tokens need a val");
  }
  Token() { type_ = Type::NONE; }

  // Constructor for NUM Tokens
  Token(Type type, int val) : type_(type), val_(val) {
    ASSERT(type == Type::NUM, "Only integers have val declared");
  }

  // check if two Tokens are equal
  bool operator==(const Token &b) const {
    return (*this == b);
    /*
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
    }*/
  }

  // Ben's Suggestion
  bool operator!=(const Token &b) const {
    return !(*this == b);
  }

  void Print() {
    switch (this->type_) {
    case NUM:
      std::cout << "Type: NUM\n" << std::endl;
      std::cout << "\tValue: " << this->val_ << "\n" << std::endl;
      break;
    default:
      std::cout << "Type: " << this->type_ << "\n" << std::endl;
      break;
    }
  }

  // Getter functions
  Type type() const { return type_; }

  int val() const {
    ASSERT(type_ == Type::NUM, "Only integer tokens have value");
    return val_;
  }

 private:
  // Types of token listed in the Token class
  Type type_;
  // Only integer has value
  int val_;
  std::pair<int, int> tokenPos_;

};

} // namespace frontend
} // namespace cs160

#endif // TOKEN_TOKEN_H
