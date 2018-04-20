#include "./tokenizer.h"
#include <sstream>

using namespace cs160::frontend;

Type extractType(const char testChar) {
  if (isdigit(testChar))
    return NUM;
  switch (testChar) {
  case '(': {
    return OPEN_PAREN;
  }
  case ')': {
    return CLOSE_PAREN;
  }
  case '+': {
    return ADD_OP;
  }
  case '-': {
    return SUB_OP;
  }
  case '*': {
    return MUL_OP;
  }
  case '/': {
    return DIV_OP;
  }
  default: { return FAILED; }
  }
}

void Token::print() {
  switch (this->type_) {
  case NUM:
    std::cout << "Type: NUM\n" << std::endl;
    std::cout << "\tValue: " << this->val_ "\n" << std::endl;
    break;
  case FAILED:
    std::cout << "Type: FAILED\n" << std::endl;
    break;
  default:
    std::cout << "Type: " << this->type_ << "\n" << std::endl;
    break;
  }
}

Tokenizer::Tokenizer(std::string program) : program_(program) {
  char prevChar;
  Type prevType = NONE;
  std::string currString;

  for (char &c : program_) {
    if (c == ' ')
      continue;
    Type type = extractType(c);
    if (type != FAILED) {
      std::stringstream currStringStream;
      currStringStream << currString;
      currStringStream << c;

      currString = currStringStream.str();
      if (type != prevType) {
        if (type == NUM) {
          Token newToken(type, atoi(currString.c_str()));
          tokens_.push_back(newToken);
        } else {
          Token newToken(type);
          tokens_.push_back(newToken);
        }
        currString = "";
      }
      prevChar = c;
      prevType = type;
    } else {
      Token newToken(FAILED);
      tokens_[0] = (newToken);
      break;
      // Return just one token that is null
    }
  }
}
