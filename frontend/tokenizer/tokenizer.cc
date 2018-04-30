#include "frontend/tokenizer/tokenizer.h"

#include <sstream>

using namespace cs160::frontend;


Tokenizer::Tokenizer(std::string input_program) : input_program_(input_program) {
  std::string currString;
  int index = 0;
  // Looping through all characters of program
  while (index != input_program.length()) {
    Token::Type tokenType = ExtractType(input_program[index]);
    if (tokenType == Token::FAILED) {
      currString += input_program[index];
    } else if (tokenType == Token::NONE) {
      currString = "";
      continue;
    } else {
      if (!currString.empty()) {
        // Create the token of expression before delimeter
        Token expressionToken = CreateTokenFromExpression(currString);
        if (expressionToken.type() == Token::FAILED) {
          Token failedToken(Token::FAILED);
          tokens_[0] = (failedToken);
          break;
        } else {
          tokens_.push_back(expressionToken);
        }
      }
      // Create the single character (most likely operator) token
      Token operatorToken(tokenType);
      tokens_.push_back(operatorToken);
    }
    index++;
  }

  if (tokens_[0].type() != Token::FAILED) {
    Token endToken(Token::END);
    tokens_.push_back(endToken);
  }
}


Token Tokenizer::CreateTokenFromExpression(std::string tokenVal) {
  if (ValidIdentifierName(tokenVal))
    Token newToken(Token::IDENTIFIER, tokenVal);
  else if (ValidNumber(tokenVal))
    Token newToken(Token::NUM, atoi(tokenVal.c_str()));
  // This should be checked last
  if (ValidVariableName(tokenVal))
    Token newToken(Token::VAR_NAME, tokenVal);
}

bool Tokenizer::ValidNumber(const std::string variableName) {
    for (int i = 0; i < variableName.length(); i++) {
      if (!isdigit(variableName[i]))
        return false;
    }
    return true;
}

bool Tokenizer::ValidVariableName(const std::string variableName) {
  char firstC = variableName[0];
  std::string restC = variableName.substr(1);
  if ((isalpha(firstC) || firstC == '_')) {
    int i = 0;
    while (i != restC.length()) {
      if (!(isalpha(restC[i]) || isdigit(restC[i]) || restC[i] == '_'))
        return false;
      i++;
    }
  }
  return true;
}

bool Tokenizer::ValidIdentifierName(const std::string identifier) {
  if (identifier == "int")
    return true;
  else
    return false;
}

Token::Type Tokenizer::ExtractType(const char testChar) {
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
    case '=': {
      return Token::EQUAL_SIGN;
    }
    case ' ': {
      return Token::NONE;
    }
    default: { return Token::FAILED; }
  }
}
