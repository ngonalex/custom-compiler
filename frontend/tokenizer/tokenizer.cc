#include "./tokenizer.h"

using namespace cs160::tokenizer;

int extractType (const char testChar) {
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

Tokenizer(std::string program) : program_(program) {
  char prevChar;
  std::string currString;

  for (char &c : program_) {
    Type = extractType(c);
    if (Type != NONE) {
      if (Typ == prevChar) {
        currString.append(c);
      } else {
        Token newToken(Type);
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
}
