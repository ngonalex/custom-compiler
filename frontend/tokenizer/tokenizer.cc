#include "./tokenizer.h"
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
      if (tokenType == prevChar) {
        currString.append(c);
      } else {
        Token newToken = {c, 0, tokenType};
        tokens.push_back(newToken);
        currString = "";
      }
      prevChar = c;
    } else {
      Token newToken = {'', 0, NONE};
      tokens[0] = (newToken);
      break;
      // Return just one token that is null
    }
  }
  return tokens;
}

// NOTE: enum tokenType = { NUM, PAREN, ADDSUB_OP, MULDIV_OP, NONE };

tokenType extractType(char testChar) {
  switch (testChar) {
  case HelperIsNum(testChar): {
    return NUM;
  }
  case HelperIsOpenParen(testChar): {
    return OPEN_PAREN;
  }
  case HelperIsCloseParen(testChar): {
    return CLOSE_PAREN;
  }
  case HelperIsAddOp(testChar): {
    return ADD_OP;
  }
  case HelperIsSubOp(testChar): {
    return SUB_OP;
  }
  case HelperIsMulOp(testChar): {
    return MUL_OP;
  }
  case HelperIsDivOp(testChar): {
    return DIV_OP;
  }
  default: { return NONE; }
  }
}
bool HelperIsNum(char testChar) {
  if (isdigit(testChar))
    return true;
  return false;
}
bool HelperIsOpenParen(char testChar) {
  if (testChar == '(')
    return true;
  return false;
}
bool HelperIsCloseParen(char testChar) {
  if (testChar == ')')
    return true;
  reutrn false;
}
bool HelperIsAddOp(char testChar) {
  if (testChar == '+')
    return true;
  return false;
}
bool HelperIsSubOp(char testChar) {
  if (testChar == '-')
    return true;
  return false;
}
bool HelperIsMulOp(char testChar) {
  if (testChar == '*')
    return true;
  return false;
}
bool HelperIsDivOp(char testChar) {
  if (testChar == '/')
    return true;
  return false;
}
