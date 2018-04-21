#include "abstract_syntax/abstract_syntax.h"
#include "frontend/parser/parser.h"
#include "frontend/tokenizer/tokenizer.h"
#include "frontend/tokenizer/token.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;
using cs160::abstract_syntax::frontend::AstVisitor;
using cs160::abstract_syntax::frontend::IntegerExpr;
using cs160::abstract_syntax::frontend::AddExpr;
using cs160::abstract_syntax::frontend::SubtractExpr;
using cs160::abstract_syntax::frontend::MultiplyExpr;
using cs160::abstract_syntax::frontend::DivideExpr;
using cs160::abstract_syntax::frontend::BinaryOperatorExpr;
using cs160::frontend::InterpreterVisitor;
using cs160::make_unique;

TEST(ParserTest, CanEvaluateSimpleAdd) {
  // program vector: END, 3, MUL_OP, 6
  Token firstToken(Type::NUM, 6);
  Token secondToken(Type::MUL_OP);
  Token thirdToken(Type::NUM, 3);
  Token fourthToken(Type::END);
  std::vector<Token> test_vector;
  test_vector.push_back(fourthToken);
  test_vector.push_back(thirdToken);
  test_vector.push_back(secondToken);
  test_vector.push_back(firstToken);

  Parser parser(test_vector);
  auto result = parser.Eparser();
  
  EXPECT_EQ(VisitMultiplyExpr(result), "(* 3 6)");

}