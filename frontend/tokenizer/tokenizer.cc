#include "frontend/tokenizer/tokenizer.h"
#include <sstream>

using namespace cs160::frontend;

Tokenizer::Tokenizer(std::string program) : input_program_(program) {
  char prevChar;
  Token::Type prevType = Token::NONE;
  std::string currString;

  for (char &c : input_program_) {
    if (c == ' ')
      continue;
    Token::Type type = ExtractType(c);
    if (type != Token::FAILED) {
      std::stringstream currStringStream;
      currStringStream << currString;
      currStringStream << c;

      currString = currStringStream.str();
      if (type != prevType) {
        if (type == Token::NUM) {
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
      Token newToken(Token::FAILED);
      tokens_[0] = (newToken);
      break;
      // Return just one token that is null
    }
  }
  if(tokens_[0].type() != Token::FAILED) {
    Token endToken(Token::END);
    tokens_.push_back(endToken);
  }
}

Token::Type Tokenizer::ExtractType(const char testChar) {
  if (isdigit(testChar))
    return Token::NUM;
  switch (testChar) {
  case '(': {
    return Token::OPEN_PAREN;
  }
  case ')': {
    return Token::CLOSE_PAREN;
  }
  case '+': {
    return Token::ADD_OP;
  }
  case '-': {
    return Token::SUB_OP;
  }
  case '*': {
    return Token::MUL_OP;
  }
  case '/': {
    return Token::DIV_OP;
  }
  default: { return Token::FAILED; }
  }
}
