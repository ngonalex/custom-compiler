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

class CodeGenTestV3 : public ::testing::Test {
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

// V3 Tests

// if( 1 < 2 ) {
//  x = 5 + 10
// } else {
//  y = 10 + 10
// }
TEST_F(CodeGenTestV3, LessThanExprWithOnlyIntsThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, LessThanExprWithOnlyIntsThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, LessThanExprWithVariablesThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, LessThanExprWithVariablesThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, LessThanExprWithMoreComplexArithExprsOnBothSides) {
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
TEST_F(CodeGenTestV3, LessThanExprEvalsToFalseWhenEqual) {
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
TEST_F(CodeGenTestV3, LessThanEqualToExprWithOnlyIntsThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, LessThanEqualToExprWithOnlyIntsThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, LessThanEqualToExprWithVariablesThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, LessThanEqualToExprWithVariablesThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, LessThanEqualToExprWithMoreComplexArithExprsOnBothSides) {
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
TEST_F(CodeGenTestV3, LessThanEqualToExprEvalsToTrueWhenEqual) {
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
TEST_F(CodeGenTestV3, GreaterThanExprWithOnlyIntsThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, GreaterThanExprWithOnlyIntsThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, GreaterThanExprWithVariablesThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, GreaterThanExprWithVariablesThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, GreaterThanExprWithMoreComplexArithExprsOnBothSides) {
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
TEST_F(CodeGenTestV3, GreaterThanExprEvalsToFalseWhenEqual) {
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
TEST_F(CodeGenTestV3, GreaterThanEqualToExprWithOnlyIntsThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, GreaterThanEqualToExprWithOnlyIntsThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, GreaterThanEqualToExprWithVariablesThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, GreaterThanEqualToExprWithVariablesThatEvalsToTrue) {
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
    CodeGenTestV3, GreaterThanEqualToExprWithMoreComplexArithExprsOnBothSides) {
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
TEST_F(CodeGenTestV3, GreaterThanEqualToExprEvalsToTrueWhenEqual) {
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
TEST_F(CodeGenTestV3, EqualsToCorrectlyEvalsToTrueWithInts) {
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
TEST_F(CodeGenTestV3, EqualsToCorrectlyEvalsToFalseWithInts) {
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
TEST_F(CodeGenTestV3, EqualsToCorrectlyEvalsToFalseWithIntsTwo) {
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
TEST_F(CodeGenTestV3, EqualsToEvaluatesTrueCorrectlyWithMoreComplexExpr) {
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
TEST_F(CodeGenTestV3, EqualsToEvaluatesFalseCorrectlyWithMoreComplexExpr) {
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
TEST_F(CodeGenTestV3, LogicalAndExprEvalsToTrueWhenBothSidesAreTrue) {
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
TEST_F(CodeGenTestV3, LogicalAndExprEvalsToFalseWhenBothSidesAreFalse) {
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
TEST_F(CodeGenTestV3, LogicalAndExprEvalsToFalseIfLeftSideIsFalse) {
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
TEST_F(CodeGenTestV3, LogicalAndExprEvalsToFalseIfRightSideIsFalse) {
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
TEST_F(CodeGenTestV3, LogicalOrExprEvalsToTrueWhenBothSidesAreTrue) {
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
TEST_F(CodeGenTestV3, LogicalOrExprEvalsToFalseWhenBothSidesAreFalse) {
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
TEST_F(CodeGenTestV3, LogicalOrExprEvalsToTrueEvenIfLeftSideIsFalse) {
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
TEST_F(CodeGenTestV3, LogicalOrExprEvalsToTrueEvenIfRightSideIsFalse) {
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
TEST_F(CodeGenTestV3, LogicalNotExprCorrectlyNegatesATrueExpr) {
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
TEST_F(CodeGenTestV3, LogicalNotExprCorrectlyNegatesAFalseExpr) {
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
TEST_F(CodeGenTestV3, NestedRelationalExprThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, NestedRelationalExprThatEvalsTotrue) {
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
TEST_F(CodeGenTestV3, BasicConditionalBlockThatEvalsToTrue) {
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
TEST_F(CodeGenTestV3, BasicConditionalBlockThatEvalsToFalse) {
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
TEST_F(CodeGenTestV3, GoingToAnEmptyTrueBranchStillExitsCorrectly) {
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
TEST_F(CodeGenTestV3, GoingToAnEmptyFalseBranchStillExitsCorrectly) {
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
TEST_F(CodeGenTestV3, NestedConditionalThatEvalsTrueThenTrue) {
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
TEST_F(CodeGenTestV3, NestedConditionalThatEvalsTrueThenFalse) {
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
TEST_F(CodeGenTestV3, NestedConditionalThatEvalsFalseThenTrue) {
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
TEST_F(CodeGenTestV3, NestedConditionalThatEvalsFalseThenFalse) {
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
TEST_F(CodeGenTestV3, SimpleLoopThatIsEntered) {
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
TEST_F(CodeGenTestV3, SimpleLoopThatIsNeverEntered) {
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
TEST_F(CodeGenTestV3, LoopWithMoreComplicatedGuard) {
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
TEST_F(CodeGenTestV3, NestedLoop) {
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
TEST_F(CodeGenTestV3, LoopWithIfStatement) {
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
