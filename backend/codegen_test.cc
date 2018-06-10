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
using cs160::backend::LowererVisitor;
using cs160::backend::ThreeAddressCode;
using cs160::backend::CodeGen;
using cs160::backend::PrintFlag::PRINT_DEBUG;
using cs160::backend::PrintFlag::PRINT_LAST_ARITHMETIC_EXPR;
using cs160::backend::PrintFlag::PRINT_ONLY_RESULT;
using cs160::abstract_syntax::version_5::Statement;
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

// Simple load of the integer 4
TEST_F(CodeGenTest, PositiveIntegerExprLoadedCorrectly) {
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
TEST_F(CodeGenTest, NegativeIntegerExprLoadedCorrectly) {
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
TEST_F(CodeGenTest, AddExprWithTwoPositiveIntegers) {
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
TEST_F(CodeGenTest, AddExprWithTwoNegativeIntegers) {
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
TEST_F(CodeGenTest, AddExprWithDifferentSignIntegersVisited) {
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
TEST_F(CodeGenTest, SubtractExprWithTwoPositiveIntsVisited) {
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
TEST_F(CodeGenTest, SubtractExprWithTwoNegativeIntsVisited) {
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
TEST_F(CodeGenTest, SubtractExprWithTwoDifferentSignsVisited) {
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
TEST_F(CodeGenTest, MultiplyExprWithTwoPositiveIntsVisited) {
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
TEST_F(CodeGenTest, MultiplyExprWithTwoNegativeIntsVisited) {
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
TEST_F(CodeGenTest, MultiplyExprWithDifferentSignsVisited) {
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
TEST_F(CodeGenTest, DivideExprWithTwoPositiveIntsVisited) {
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
TEST_F(CodeGenTest, DivideExprWithNegativeIntsVisited) {
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
TEST_F(CodeGenTest, DivideExprWithDifferentSignsVisited) {
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
TEST_F(CodeGenTest, DivideExprWithZeroAsDividendVisited) {
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
TEST_F(CodeGenTest, DivisionByZeroError) {
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
TEST_F(CodeGenTest, NestedArithmeticExprs) {
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

// V2 Tests

// x = 5
TEST_F(CodeGenTest, SimplePositiveAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 5\n");
}

// x = -5
TEST_F(CodeGenTest, SimpleNegativeAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(-5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: -5\n");
}

// x = 5 + 10
TEST_F(CodeGenTest, ArithmeticExprOnRHSAssignmentTest) {
  auto expr = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// x = 5 + 10
// y = x - 5
TEST_F(CodeGenTest, VariabletoVariableAssignmentTest) {
  Statement::Block statements;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<SubtractExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(10)))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\nVariable y is equal to: 5\n");
}

// V3 Tests

// if( 1 < 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// if( 3 < 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( z < w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( w < z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( 10 + w < 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// w = 20
// if( w < w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanExprEvalsToFalseWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// if( 1 <= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// if( 3 <= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( z <= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( w <= z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( 10 + w <= 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// w = 20
// if( w <= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LessThanEqualToExprEvalsToTrueWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
        make_unique<IntegerExpr>(5),
        make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// if( 1 > 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( 3 > 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( z > w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( w > z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( 10 + w > 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// w = 20
// if( w > w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanExprEvalsToFalseWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// if( 1 >= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithOnlyIntsThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<IntegerExpr>(1),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( 3 >= 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithOnlyIntsThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<IntegerExpr>(3),
          make_unique<IntegerExpr>(2)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( z >= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithVariablesThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 20
// if( w >= z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprWithVariablesThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("z")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( 10 + w >= 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(
    CodeGenTest, GreaterThanEqualToExprWithMoreComplexArithExprsOnBothSides) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}

// w = 20
// if( w >= w ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, GreaterThanEqualToExprEvalsToTrueWhenEqual) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;


  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<GreaterThanEqualToExpr>(
          make_unique<VariableExpr>("w"),
          make_unique<VariableExpr>("w")),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable w is equal to: 20\n"
            "Variable x is equal to: 15\n");
}


// if( -5 == -5 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToCorrectlyEvalsToTrueWithInts) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(-5),
          make_unique<IntegerExpr>(-5)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// if( 10 == -5 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToCorrectlyEvalsToFalseWithInts) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(-5)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( 10 == 15 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToCorrectlyEvalsToFalseWithIntsTwo) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(15)),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( 10 + w == 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToEvaluatesTrueCorrectlyWithMoreComplexExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 20
// if( 10 + w == 10 + z ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, EqualsToEvaluatesFalseCorrectlyWithMoreComplexExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(20))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("w")),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(10),
              make_unique<VariableExpr>("z"))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w == 10 && z <= 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToTrueWhenBothSidesAreTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<LessThanEqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( w == 20 && z < 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToFalseWhenBothSidesAreFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w == 20 && z == 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToFalseIfLeftSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w == 10 && z == -10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalAndExprEvalsToFalseIfRightSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalAndExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(-10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w >= 10 || z < 20 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToTrueWhenBothSidesAreTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<GreaterThanEqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(20))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( w == 20 || z < 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToFalseWhenBothSidesAreFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( w >= 20 && z == 10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToTrueEvenIfLeftSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<GreaterThanEqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(20)),
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( w == 10 || z < -10 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalOrExprEvalsToTrueEvenIfRightSideIsFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalOrExpr>(
          make_unique<EqualToExpr>(
              make_unique<VariableExpr>("w"),
              make_unique<IntegerExpr>(10)),
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("z"),
              make_unique<IntegerExpr>(-10))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( !(w == 10 || z < -10) ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalNotExprCorrectlyNegatesATrueExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(-10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable y is equal to: 20\n");
}

// z = 10
// w = 10
// if( !(w == 10 && z < -10) ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, LogicalNotExprCorrectlyNegatesAFalseExpr) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalAndExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(-10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n");
}

// if( !((5 < 10 && -10 > 10) || (500 == 500 || -10000 >= 10000)) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, NestedRelationalExprThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<LogicalAndExpr>(
                  make_unique<LessThanExpr>(
                      make_unique<IntegerExpr>(5),
                      make_unique<IntegerExpr>(10)),
                  make_unique<GreaterThanExpr>(
                      make_unique<IntegerExpr>(-10),
                      make_unique<IntegerExpr>(10))),
              make_unique<LogicalOrExpr>(
                  make_unique<EqualToExpr>(
                      make_unique<IntegerExpr>(500),
                      make_unique<IntegerExpr>(500)),
                  make_unique<GreaterThanEqualToExpr>(
                      make_unique<IntegerExpr>(-10000),
                      make_unique<IntegerExpr>(100000))))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable y is equal to: 20\n");
}

// if( !((5 < 10 && -10 > 10) || (-500 == 500 || -10000 >= 10000)) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTest, NestedRelationalExprThatEvalsTotrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalOrExpr>(
              make_unique<LogicalAndExpr>(
                  make_unique<LessThanExpr>(
                      make_unique<IntegerExpr>(5),
                      make_unique<IntegerExpr>(10)),
                  make_unique<GreaterThanExpr>(
                      make_unique<IntegerExpr>(-10),
                      make_unique<IntegerExpr>(10))),
              make_unique<LogicalOrExpr>(
                  make_unique<EqualToExpr>(
                      make_unique<IntegerExpr>(-500),
                      make_unique<IntegerExpr>(500)),
                  make_unique<GreaterThanEqualToExpr>(
                      make_unique<IntegerExpr>(-10000),
                      make_unique<IntegerExpr>(100000))))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 15\n");
}

// z = 10
// w = 10
// if( !(w == 10 && z < -10) ) {
//  x = 5 + 10
//  y = 5 + 10
// } else {
//  x = 10 + 10
//  y = 10 + 10
// }
TEST_F(CodeGenTest, BasicConditionalBlockThatEvalsToTrue) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalAndExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(-10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 15\n"
            "Variable y is equal to: 15\n");
}

// z = 10
// w = 10
// if( !(w == 10 && z <= -10) ) {
//  x = 5 + 10
//  y = 5 + 10
// } else {
//  x = 10 + 10
//  y = 10 + 10
// }
TEST_F(CodeGenTest, BasicConditionalBlockThatEvalsToFalse) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("w"), make_unique<IntegerExpr>(10))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LogicalNotExpr>(
          make_unique<LogicalAndExpr>(
              make_unique<EqualToExpr>(
                  make_unique<VariableExpr>("w"),
                  make_unique<IntegerExpr>(10)),
              make_unique<LessThanEqualToExpr>(
                  make_unique<VariableExpr>("z"),
                  make_unique<IntegerExpr>(10)))),
      std::move(trueblock), std::move(falseblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable w is equal to: 10\n"
            "Variable x is equal to: 20\n"
            "Variable y is equal to: 20\n");
}

// if( 15 == 15 ) {
// } else {
//   x = 5 + 10
// }
// z = 500 + 500
TEST_F(CodeGenTest, GoingToAnEmptyTrueBranchStillExitsCorrectly) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(15),
          make_unique<IntegerExpr>(15)),
      Statement::Block(), std::move(trueblock))));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(500),
          make_unique<IntegerExpr>(500)))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 1000\n");
}

// if( 10 == 15 ) {
//  x = 5 + 10
// } else {
// }
// z = 500 + 500
TEST_F(CodeGenTest, GoingToAnEmptyFalseBranchStillExitsCorrectly) {
  Statement::Block statements;
  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<EqualToExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(15)),
      std::move(trueblock), Statement::Block())));

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(500),
          make_unique<IntegerExpr>(500)))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 1000\n");
}

// z = 10
// if ( z <= 10 ) {
//  z = 5 + 10
//  if ( z <= 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z <= 15) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsTrueThenTrue) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 15\n"
            "Variable z is equal to: 20\n");
}

// z = 10
// if ( z <= 10 ) {
//  z = 5 + 10
//  if ( z < 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z <= 20) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsTrueThenFalse) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 15\n"
            "Variable z is equal to: -20\n");
}

// z = 10
// if ( z < 10 ) {
//  z = 5 + 10
//  if ( z <= 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z <= 20) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsFalseThenTrue) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 20\n"
            "Variable z is equal to: -100\n");
}

// z = 10
// if ( z < 10 ) {
//  z = 5 + 10
//  if ( z <= 15) {
//    z = 10 + 10
//  } else {
//    z = -10 + -10
//  }
// } else {
//   z = 10 + 10
//   if ( z < 20) {
//     z = -50 + -50
//   } else {
//     z = -100 + 100
//   }
// }
TEST_F(CodeGenTest, NestedConditionalThatEvalsFalseThenFalse) {
  Statement::Block statements;
  Statement::Block trueouterblock;
  Statement::Block falseouterblock;
  Statement::Block truebranchinnerblock;
  Statement::Block truebranchinnerblock2;
  Statement::Block falsebranchinnerblock;
  Statement::Block falsebranchinnerblock2;

  statements.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"), make_unique<IntegerExpr>(10))));

  truebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  truebranchinnerblock2.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-10),
          make_unique<IntegerExpr>(-10)))));

  falsebranchinnerblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(-50),
          make_unique<IntegerExpr>(-50)))));

  falsebranchinnerblock2.push_back(std::move(
      make_unique<AssignmentFromArithExp>(
          make_unique<VariableExpr>("z"),
          make_unique<AddExpr>(
              make_unique<IntegerExpr>(-100),
              make_unique<IntegerExpr>(100)))));

  trueouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(5),
          make_unique<IntegerExpr>(10)))));

  trueouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanEqualToExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(15)),
      std::move(truebranchinnerblock), std::move(truebranchinnerblock2))));

  falseouterblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("z"),
      make_unique<AddExpr>(
          make_unique<IntegerExpr>(10),
          make_unique<IntegerExpr>(10)))));

  falseouterblock.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(20)),
      std::move(falsebranchinnerblock), std::move(falsebranchinnerblock2))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("z"),
          make_unique<IntegerExpr>(10)),
      std::move(trueouterblock), std::move(falseouterblock))));

  for (auto& statement : statements) {
    statement->Visit(&lowerer_);
  }

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable z is equal to: 10\n"
            "Variable z is equal to: 20\n"
            "Variable z is equal to: 0\n");
}


// x = 0
// while(x < 5) {
//   x = x + 1
// }
TEST_F(CodeGenTest, SimpleLoopThatIsEntered) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
            "Variable x is equal to: 1\n"
            "Variable x is equal to: 2\n"
            "Variable x is equal to: 3\n"
            "Variable x is equal to: 4\n"
            "Variable x is equal to: 5\n");
}

// x = 0
// while(x < 0) {
//   x = x + 1
// }
TEST_F(CodeGenTest, SimpleLoopThatIsNeverEntered) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(0)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n");
}

// x = 0
// while (x < 5 && !((5 < 10 && -10 > 10) || (-500 == 500 || -10000 >= 10000)) {
//   x = x + 1
// }
TEST_F(CodeGenTest, LoopWithMoreComplicatedGuard) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  auto ast = make_unique<const Loop>(
      make_unique<LogicalAndExpr>(
          make_unique<LessThanExpr>(
              make_unique<VariableExpr>("x"),
              make_unique<IntegerExpr>(5)),
          make_unique<LogicalNotExpr>(
              make_unique<LogicalOrExpr>(
                  make_unique<LogicalAndExpr>(
                      make_unique<LessThanExpr>(
                          make_unique<IntegerExpr>(5),
                          make_unique<IntegerExpr>(10)),
                      make_unique<GreaterThanExpr>(
                          make_unique<IntegerExpr>(-10),
                          make_unique<IntegerExpr>(10))),
                  make_unique<LogicalOrExpr>(
                      make_unique<EqualToExpr>(
                          make_unique<IntegerExpr>(-500),
                          make_unique<IntegerExpr>(500)),
                      make_unique<GreaterThanEqualToExpr>(
                          make_unique<IntegerExpr>(-10000),
                          make_unique<IntegerExpr>(100000)))))),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
            "Variable x is equal to: 1\n"
            "Variable x is equal to: 2\n"
            "Variable x is equal to: 3\n"
            "Variable x is equal to: 4\n"
            "Variable x is equal to: 5\n");
}

// x = 0
// y = 0
// while (x < 3) {
//   while (y < 4) {
//      y = y + 2
//   }
//   x = x + 1
//   y = 0
// }
TEST_F(CodeGenTest, NestedLoop) {
  Statement::Block loop_body;
  Statement::Block nested_loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto assign1 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(0));

  auto nestedloopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("y"),
          make_unique<IntegerExpr>(2)));

  nested_loop_body.push_back(std::move(nestedloopbody));

  auto loop = make_unique<const Loop>(
    make_unique<LessThanExpr>(
        make_unique<VariableExpr>("y"),
        make_unique<IntegerExpr>(4)),
    std::move(nested_loop_body));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  auto loopbody2 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(0));

  loop_body.push_back(std::move(loop));
  loop_body.push_back(std::move(loopbody));
  loop_body.push_back(std::move(loopbody2));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(3)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  assign1->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
                    "Variable y is equal to: 0\n"
                    "Variable y is equal to: 2\n"
                    "Variable y is equal to: 4\n"
                    "Variable x is equal to: 1\n"
                    "Variable y is equal to: 0\n"
                    "Variable y is equal to: 2\n"
                    "Variable y is equal to: 4\n"
                    "Variable x is equal to: 2\n"
                    "Variable y is equal to: 0\n"
                    "Variable y is equal to: 2\n"
                    "Variable y is equal to: 4\n"
                    "Variable x is equal to: 3\n"
                    "Variable y is equal to: 0\n");
}

// x = 0
// y = 1
// while(x < 6) {
//   x = x + 1
//   if (0 < y) {
//     y = -1
//   } else {
//     y = 1
//   }
// }
TEST_F(CodeGenTest, LoopWithIfStatement) {
  Statement::Block loop_body;

  auto assign = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<IntegerExpr>(0));

  auto assign2 = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(1));

  auto loopbody = make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("x"),
      make_unique<AddExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(1)));

  loop_body.push_back(std::move(loopbody));

  Statement::Block trueblock;
  Statement::Block falseblock;

  trueblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(-1))));

  falseblock.push_back(std::move(make_unique<AssignmentFromArithExp>(
      make_unique<VariableExpr>("y"),
      make_unique<IntegerExpr>(1))));

  loop_body.push_back(std::move(make_unique<const Conditional>(
      make_unique<LessThanExpr>(
          make_unique<IntegerExpr>(0),
          make_unique<VariableExpr>("y")),
      std::move(trueblock), std::move(falseblock))));

  auto ast = make_unique<const Loop>(
      make_unique<LessThanExpr>(
          make_unique<VariableExpr>("x"),
          make_unique<IntegerExpr>(5)),
      std::move(loop_body));

  assign->Visit(&lowerer_);
  assign2->Visit(&lowerer_);
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file, PRINT_DEBUG);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.totalset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -static test.s -o run && ./run");
  EXPECT_EQ(result, "Variable x is equal to: 0\n"
            "Variable y is equal to: 1\n"
            "Variable x is equal to: 1\n"
            "Variable y is equal to: -1\n"
            "Variable x is equal to: 2\n"
            "Variable y is equal to: 1\n"
            "Variable x is equal to: 3\n"
            "Variable y is equal to: -1\n"
            "Variable x is equal to: 4\n"
            "Variable y is equal to: 1\n"
            "Variable x is equal to: 5\n"
            "Variable y is equal to: -1\n");
}

// V4 Tests