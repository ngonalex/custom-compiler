#ifndef TOKEN_TOKEN_H
#define TOKEN_TOKEN_H

#include "utility/assert.h"

namespace cs160 {
namespace frontend {

class Token {
 public:
  enum Type { NUM, OPEN_PAREN, CLOSE_PAREN, ADD_OP,
   SUB_OP, MUL_OP, DIV_OP, EQUAL_SIGN, VAR_NAME, END, FAILED, NONE };
  // Constructor for Non-NUM Tokens
  explicit Token(Token::Type type) : type_(type), val_(0) {
    ASSERT(type != Token::Type::NUM, "Integer tokens need a val");
  }
  Token() { type_ = Token::Type::NONE; }

  // Constructor for NUM Tokens
  Token(Token::Type type, int val) : type_(type), val_(val) {
    ASSERT(type == Type::NUM, "Only integers have val declared");
  }

  // check if two Tokens are equal
  bool operator==(const Token &b) const;

  bool operator!=(const Token &b) const;

  void Print();

  // Helper functions for the parser
  bool isOperator();

  bool isNumber () { return (this->type() == Type::NUM); };

  // Getter functions
  Token::Type type() const { return type_; }

  int val() const {
    ASSERT(type_ == Token::Type::NUM, "Only integer tokens have value");
    return val_;
  }

 private:
  // Types of token listed in the Token class
  Token::Type type_;
  // Only integer has value
  int val_;
  // Character number, Line Number
  std::pair<int, int> tokenPos_;

};

} // namespace frontend
} // namespace cs160

#endif // TOKEN_TOKEN_H
