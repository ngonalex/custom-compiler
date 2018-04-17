#include "./tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
using cs160::tokenizer::Tokenizer;
using cs160::tokenizer::Token;

std::vector<Token> tokenize() {
  std::vector<Token> tokens;
  char prevChar;
  std::string currString;

  for (char &c : program) {
    Type = extractType(c);
    if (Type != NONE) {
      if (Typ == prevChar) {
        currString.append(c);
      } else {
        Token newToken(Type);
        tokens.push_back(newToken);
        currString = "";
      }
      prevChar = c;
    }
    else {
      Token newToken(NONE);
      tokens[0] = (newToken);
      break;
      // Return just one token that is null
    }
  }

  return tokens;
}

Type extractType(char testChar) {
  switch (testChar) {
    case isdigit(testChar): { return NUM; }
    case testChar == '(': { return OPEN_PAREN; }
    case testChar == ')': { return CLOSE_PAREN; }
    case testChar == '+': { return ADD_OP; }
    case testChar == '-': { return SUB_OP; }
    case testChar == '*': { return MUL_OP; }
    case testChar == '/': { return DIV_OP; }
    default: { return NONE; }
  }
}
