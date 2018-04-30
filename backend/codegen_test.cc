
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
using cs160::abstract_syntax::backend::BinaryOperatorExpr;
using cs160::backend::LowererVisitor;
using cs160::backend::ThreeAddressCode;
using cs160::backend::CodeGen;
using cs160::make_unique;

class CodeGenTest : public ::testing::Test {
 protected:
  LowererVisitor lowerer_;
};

// this piece of code is from stackoverflow 
// link: https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
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

TEST_F(CodeGenTest, IntegerExprIsVisited) {
  auto expr = cs160::make_unique<DivideExpr>(
    make_unique<IntegerExpr>(21), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.Generate(std::move(test));
  std::string result = exec("gcc -c test.s && ld test.o && ./a.out");
  EXPECT_EQ(result, "The result is equal to: 4\n");
}

TEST_F(CodeGenTest, PrintTest) {
  auto expr = cs160::make_unique<SubtractExpr>(
    make_unique<IntegerExpr>(21), make_unique<IntegerExpr>(5));

  expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.Generate(std::move(test));
  std::string result = exec("gcc -c test.s && ld test.o && ./a.out");
  EXPECT_EQ(result, "The result is equal to: 16\n");

}

