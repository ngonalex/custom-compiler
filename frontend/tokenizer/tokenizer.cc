#include "./tokenizer.h"
#include <sstream>

using namespace cs160::frontend;

Type extractType (const char testChar) {
  if (isdigit(testChar))
    return NUM;
  switch (testChar) {
    case '(': { return OPEN_PAREN; }
    case ')': { return CLOSE_PAREN; }
    case '+': { return ADD_OP; }
    case '-': { return SUB_OP; }
    case '*': { return MUL_OP; }
    case '/': { return DIV_OP; }
    default: { return NONE; }
  }
}

Tokenizer::Tokenizer(std::string program) : program_(program) {
  char prevChar;
  std::string currString;

  for (char &c : program_) {
    Type type = extractType(c);
    if (type != NONE) {
      if (type == prevChar) {

        std::stringstream currStringStream;
        currStringStream << currString;
        currStringStream << c;

        currString = currStringStream.str();
      } else {
        Token newToken(type);
        tokens_.push_back(newToken);
        currString = "";
      }
      prevChar = c;
    }
    else {
      Token newToken(NONE);
      tokens_[0] = (newToken);
      break;
      // Return just one token that is null
    }
  }
};
