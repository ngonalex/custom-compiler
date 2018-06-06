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
#include "utility/memory.h"
#include "gtest/gtest.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::backend::LowererVisitor;
using cs160::backend::ThreeAddressCode;
using cs160::backend::CodeGen;
using cs160::make_unique;

class CodeGenTest : public ::testing::Test {
 protected:
  LowererVisitor lowerer_;
};

// this piece of code is from stackoverflow
// link:
// https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

// Todo:
// 1a) ArithmeticExprs (Add/Sub/Mult/Div) + corner cases (div/zero,Overflow)
//  b) Diff combos (Int/Int, Var/Var, Var/Int etc), Nested
// 2a) RelationalExprs + LogicalExprs
//  b) Diff combos (Int/Int, Var/Var, Var/Int etc), Nested
// 3) Loops + Conditionals, Nested
// 4) Variable not assigned, Variable reassigned, Handle different Variables
// 5) Complex Program

// V1 Tests
TEST_F(CodeGenTest, PositiveIntegerExprLoadedCorrectly) {
  auto expr = make_unique<IntegerExpr>(4);

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

TEST_F(CodeGenTest, NegativeIntegerExprLoadedCorrectly) {
  auto expr = make_unique<IntegerExpr>(-4);

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

TEST_F(CodeGenTest, AddExprWithTwoPositiveIntegers) {
  auto expr = make_unique<AddExpr>(
    make_unique<IntegerExpr>(1), make_unique<IntegerExpr>(2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 3\n");
}

TEST_F(CodeGenTest, AddExprWithTwoNegativeIntegers) {
  auto expr = make_unique<AddExpr>(
    make_unique<IntegerExpr>(-1), make_unique<IntegerExpr>(-2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -3\n");
}

TEST_F(CodeGenTest, AddExprWithDifferentSignIntegersVisited) {
  auto expr = make_unique<AddExpr>(
    make_unique<IntegerExpr>(-1), make_unique<IntegerExpr>(2));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 1\n");
}

TEST_F(CodeGenTest, SubtractExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<SubtractExpr>(
    make_unique<IntegerExpr>(5), make_unique<IntegerExpr>(1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

TEST_F(CodeGenTest, SubtractExprWithTwoNegativeIntsVisited) {
  auto expr = make_unique<SubtractExpr>(
    make_unique<IntegerExpr>(-5), make_unique<IntegerExpr>(-1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -4\n");
}

TEST_F(CodeGenTest, SubtractExprWithTwoDifferentSignsVisited) {
  auto expr = make_unique<SubtractExpr>(
    make_unique<IntegerExpr>(-5), make_unique<IntegerExpr>(1));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -6\n");
}

TEST_F(CodeGenTest, MultiplyExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<MultiplyExpr>(
    make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 50\n");
}

TEST_F(CodeGenTest, MultiplyExprWithTwoNegativeIntsVisited) {
  auto expr = make_unique<MultiplyExpr>(
    make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 50\n");
}

TEST_F(CodeGenTest, MultiplyExprWithDifferentSignsVisited) {
  auto expr = make_unique<MultiplyExpr>(
    make_unique<IntegerExpr>(-10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -50\n");
}

TEST_F(CodeGenTest, DivideExprWithTwoPositiveIntsVisited) {
  auto expr = make_unique<DivideExpr>(
    make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 2\n");
}

TEST_F(CodeGenTest, DivideExprWithNegativeIntsVisited) {
  auto expr = make_unique<DivideExpr>(
    make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 2\n");
}

TEST_F(CodeGenTest, DivideExprWithDifferentSignsVisited) {
  auto expr = make_unique<DivideExpr>(
    make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: -2\n");
}

TEST_F(CodeGenTest, DivisionByZeroError) {
  auto expr = make_unique<DivideExpr>(
    make_unique<IntegerExpr>(10), make_unique<IntegerExpr>(0));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Error: Division by zero\n");
}

TEST_F(CodeGenTest, NestedArithmeticExprs) {
  auto expr = make_unique<DivideExpr>(
      make_unique<AddExpr>(make_unique<IntegerExpr>(7),
                           make_unique<IntegerExpr>(5)),
      make_unique<SubtractExpr>(make_unique<IntegerExpr>(4),
                                make_unique<IntegerExpr>(1)));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test), 1);
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "The program returned: 4\n");
}

// V2 Tests
