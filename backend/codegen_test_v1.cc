#include <stdio.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include "backend/code_gen.h"
#include "abstract_syntax/abstract_syntax.h"
#include "backend/lowerer_visitor.h"
#include "backend/ir.h"
#include "utility/memory.h"
#include "gtest/gtest.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::ArithmeticExpr;
using cs160::backend::LowererVisitor;
using cs160::backend::ThreeAddressCode;
using cs160::backend::CodeGen;
using cs160::backend::PrintFlag::PRINT_DEBUG;
using cs160::backend::PrintFlag::PRINT_LAST_ARITHMETIC_EXPR;
using cs160::backend::PrintFlag::PRINT_ONLY_RESULT;
using cs160::abstract_syntax::version_5::Statement;
using cs160::make_unique;

class CodeGenTestV1 : public ::testing::Test {
 public:
  // this piece of code is from stackoverflow
  // link:
  // https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
  std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    while (!feof(pipe.get())) {
      if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
        result += buffer.data();
      }
    return result;
}
 protected:
  LowererVisitor lowerer_;
};

// V1 Tests

// Simple load of the integer 4
TEST_F(CodeGenTestV1, PositiveIntegerExprLoadedCorrectly) {
  auto expr = make_unique<IntegerExpr>(4);

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

// Simple load of the integer -4
TEST_F(CodeGenTestV1, NegativeIntegerExprLoadedCorrectly) {
  auto expr = make_unique<IntegerExpr>(-4);

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

// 1 + 2
TEST_F(CodeGenTestV1, AddExprWithTwoPositiveIntegers) {
  auto expr = make_unique<AddExpr>(
      make_unique<IntegerExpr>(1), make_unique<IntegerExpr>(2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 3\n");
}

// -1 + -2
TEST_F(CodeGenTestV1, AddExprWithTwoNegativeIntegers) {
  auto expr = make_unique<AddExpr>(
      make_unique<IntegerExpr>(-1), make_unique<IntegerExpr>(-2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -3\n");
}

// -1 + 2
TEST_F(CodeGenTestV1, AddExprWithDifferentSignIntegersVisited) {
  auto expr = make_unique<AddExpr>(
      make_unique<IntegerExpr>(-1), make_unique<IntegerExpr>(2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 1\n");
}

// 5 - 1
TEST_F(CodeGenTestV1, SubtractExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<SubtractExpr>(
      make_unique<IntegerExpr>(5), make_unique<IntegerExpr>(1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

// -5 - (-1)
TEST_F(CodeGenTestV1, SubtractExprWithTwoNegativeIntsVisited) {
  auto expr = make_unique<SubtractExpr>(
      make_unique<IntegerExpr>(-5), make_unique<IntegerExpr>(-1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

// -5 - 1
TEST_F(CodeGenTestV1, SubtractExprWithTwoDifferentSignsVisited) {
  auto expr = make_unique<SubtractExpr>(
      make_unique<IntegerExpr>(-5), make_unique<IntegerExpr>(1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -6\n");
}

// 10 * 5
TEST_F(CodeGenTestV1, MultiplyExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<MultiplyExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 50\n");
}

// -10 * -5
TEST_F(CodeGenTestV1, MultiplyExprWithTwoNegativeIntsVisited) {
  auto expr = make_unique<MultiplyExpr>(
      make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 50\n");
}

// -10 * 5
TEST_F(CodeGenTestV1, MultiplyExprWithDifferentSignsVisited) {
  auto expr = make_unique<MultiplyExpr>(
      make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -50\n");
}

// 10 / 5
TEST_F(CodeGenTestV1, DivideExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 2\n");
}

// -10 / -5
TEST_F(CodeGenTestV1, DivideExprWithNegativeIntsVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 2\n");
}

// 10 / -5
TEST_F(CodeGenTestV1, DivideExprWithDifferentSignsVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -2\n");
}

// 0 / -5
TEST_F(CodeGenTestV1, DivideExprWithZeroAsDividendVisited) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(0), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 0\n");
}

// 10 / 0
TEST_F(CodeGenTestV1, DivisionByZeroError) {
  auto expr = make_unique<DivideExpr>(
      make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(0));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Error: Division by zero\n");
}

// (7+5) / (4-1)
TEST_F(CodeGenTestV1, NestedArithmeticExprs) {
  auto expr = make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                           make_unique<IntegerExpr>(5)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(4),
                                make_unique<IntegerExpr>(1)));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_LAST_ARITHMETIC_EXPR);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}