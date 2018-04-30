#include "frontend/tokenizer/token.h"

using namespace cs160::frontend;

bool Token::operator==(const Token &b) const {
  if (this->type_ == NUM) {
    if (this->type_ == b.type_ && this->numVal_ == b.numVal_)
      return true;
    else
      return false;
  } else if (this->type_ == IDENTIFIER || this->type_ == VAR_NAME) {
    if (this->type_ == b.type_ && this->stringVal_ == b.stringVal_)
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

bool Token::operator!=(const Token &b) const {
  if (this->type_ == NUM) {
    if (this->type_ == b.type_ && this->numVal_ == b.numVal_)
      return false;
    else
      return true;
  } else if (this->type_ == IDENTIFIER || this->type_ == VAR_NAME) {
    if (this->type_ == b.type_ && this->stringVal_ == b.stringVal_)
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

void Token::Print() {
  switch (this->type_) {
  case NUM:
    std::cout << "Type: NUM\n" << std::endl;
    std::cout << "\tValue: " << this->numVal_ << "\n" << std::endl;
    break;
  case IDENTIFIER || VAR_NAME:
    std::cout << "Type: IDENTIFIER or VAR_NAME\n" << std::endl;
    std::cout << "\tValue: " << this->stringVal_ << "\n" << std::endl;
    break;
  default:
    std::cout << "Type: " << this->type_ << "\n" << std::endl;
    break;
  }
}

bool Token::isOperator() {
  Type current_type = this->type();
  return (current_type == Type::MUL_OP || current_type == Type::DIV_OP ||
    current_type == Type::ADD_OP || current_type == Type::SUB_OP);
}
