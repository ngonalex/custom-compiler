#include "tokenizer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
using cs160;

Tokenizer::Tokenizer(std::string program) : program(program) {}

std::vector<Token> Tokenizer::tokenize() {
  std::vector<Token> tokens;
  char prevChar;
  std::string currString;
  for (char &c : program) {
    tokenType = extractType(c);
    if (tokenType != NONE) {
      if (tokenType == prevChar)
        currString.append(c);
      else {
        Token newToken = {c, 0, tokenType};
        tokens.push_back(newToken);
        currString = "";
      }
      prevChar = c;
    }
  }
}

// NOTE: enum tokenType = { NUM, PAREN, ADDSUB_OP, MULDIV_OP, NONE };

tokenType extractType(char testChar) {
  switch (testChar) {
  case HelperIsNum(testChar): {
    return NUM;
  }
  case HelperIsParen(testChar): {
    return PAREN;
  }
  case HelperIsAddsubOp(testChar): {
    return ADDSUB_OP;
  }
  case HelperIsMuldivOp(testChar): {
    return MULDIV_OP;
  }
  default: { return NONE; }
  }
}
bool HelperIsNum(char testChar) {
  if (isdigit(testChar))
    return true;
  return false;
}
bool HelperIsParen(char testChar) {
  if (testChar == ')' || testChar == '(')
    return true;
  return false;
}
bool HelperIsAddsubOp(char testChar) {
  if (testChar == '+' || testChar == '-')
    return true;
  return false;
}
bool HelperIsMuldivOp(char testChar) {
  if (testChar == '*' || testChar == '/')
    return true;
  return false;
}
