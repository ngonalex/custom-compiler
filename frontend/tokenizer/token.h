#ifndef TOKEN_TOKEN_H
#define TOKEN_TOKEN_H

#include "utility/assert.h"
#include "string"

namespace cs160 {
namespace frontend {

class Token {
 public:
  enum Type { NUM, OPEN_PAREN, CLOSE_PAREN, ADD_OP,
   SUB_OP, MUL_OP, DIV_OP, EQUAL_SIGN, IDENTIFIER, VAR_NAME, END, FAILED, NONE };

  // Constructor for Non-NUM Tokens
  explicit Token(Token::Type type) : type_(type), numVal_(0) {
    ASSERT(type != Token::Type::NUM, "Integer tokens need an integer val");
    ASSERT(type != Token::Type::IDENTIFIER, "Identifier tokens need a string identifierVal");
  }
  Token() { type_ = Token::Type::NONE; }

  Token(Token::Type type, std::string stringVal) : type_(type), stringVal_(stringVal) {
    ASSERT(type == Token::Type::IDENTIFIER || type == Token::Type::VAR_NAME, "Only identifier and var_name tokens have identifierVal declared");
  }

  // Constructor for NUM Tokens
  Token(Token::Type type, int val) : type_(type), numVal_(val) {
    ASSERT(type == Type::NUM, "Only integer tokens have val declared");
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

  int numVal() const {
    ASSERT(type_ == Token::Type::NUM, "Only integer tokens have value");
    return numVal_;
  }

  std::string stringVal() const {
    ASSERT(type_ == (Token::Type::IDENTIFIER || Token::Type::VAR_NAME), "Only integer tokens have value");
    return stringVal_;
  }

 private:
  // Type of token listed enum Type
  Token::Type type_;
  // Only integer has this
  int numVal_;
  // Only identifiers and variable names have this
  std::string stringVal_;
  // Character number, Line Number
  std::pair<int, int> tokenPos_;

};

class PreToken : public Token {
 public:
    explicit PreToken(std::string expression) : expression_(expression) {}
 private:
    std::string expression_;
};

} // namespace frontend
} // namespace cs160

#endif // TOKEN_TOKEN_H
