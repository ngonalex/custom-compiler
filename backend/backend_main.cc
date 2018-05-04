
#include "backend/code_gen.h"
#include "abstract_syntax/abstract_syntax.h"
#include "backend/lowerer_visitor.h"
#include "utility/memory.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::Assignment;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::LessThanExpr;
using cs160::abstract_syntax::backend::LessThanEqualToExpr;
using cs160::abstract_syntax::backend::GreaterThanExpr;
using cs160::abstract_syntax::backend::GreaterThanEqualToExpr;
using cs160::abstract_syntax::backend::EqualToExpr;
using cs160::abstract_syntax::backend::LogicalAndExpr;
using cs160::abstract_syntax::backend::LogicalOrExpr;
using cs160::abstract_syntax::backend::LogicalNotExpr;
using cs160::abstract_syntax::backend::Statement;
using cs160::abstract_syntax::backend::Assignment;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::Program;
using cs160::backend::LowererVisitor;
using cs160::backend::ThreeAddressCode;
using cs160::backend::CodeGen;
using cs160::make_unique;

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

int main() {
  LowererVisitor lowerer_;
  Statement::Block statements;
  auto expr = cs160::make_unique<DivideExpr>(
    make_unique<IntegerExpr>(7), make_unique<IntegerExpr>(5));

  statements.push_back(std::move(
      make_unique<const Assignment>(make_unique<const VariableExpr>("bob"),
                                    make_unique<const IntegerExpr>(50))));

  Statement::Block assignmenttest;
  assignmenttest.push_back(std::move(
    make_unique<const Assignment>(make_unique<const VariableExpr>("bob"),
      make_unique<const IntegerExpr>(55))));

  statements.push_back(std::move(make_unique<const Conditional>(
      make_unique<const LogicalOrExpr>(
          make_unique<const LogicalAndExpr>(
              make_unique<const LessThanExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(100)),
              make_unique<const GreaterThanExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(0))),
          make_unique<const LogicalAndExpr>(
              make_unique<const LessThanEqualToExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(100)),
              make_unique<const GreaterThanEqualToExpr>(
                  make_unique<const VariableExpr>("bob"),
                  make_unique<const IntegerExpr>(0)))),
      std::move(assignmenttest), Statement::Block())));

  Statement::Block body;
  body.push_back(std::move(make_unique<const Assignment>(
      make_unique<const VariableExpr>("bob"),
      make_unique<const SubtractExpr>(make_unique<const VariableExpr>("bob"),
                                      make_unique<const IntegerExpr>(1)))));

  statements.push_back(std::move(make_unique<const Loop>(
     // make_unique<const LogicalNotExpr>(
          make_unique<const GreaterThanExpr>(
            make_unique<const VariableExpr>("bob"),
            make_unique<const IntegerExpr>(0)),
          std::move(body))));

  auto ae = make_unique<const AddExpr>(
      make_unique<const SubtractExpr>(
          make_unique<const DivideExpr>(make_unique<const IntegerExpr>(12),
                                        make_unique<const IntegerExpr>(3)),
          make_unique<const IntegerExpr>(4)),
      make_unique<const MultiplyExpr>(make_unique<const IntegerExpr>(3),
                                      make_unique<const IntegerExpr>(2)));

  auto ast = make_unique<const Program>(std::move(statements), std::move(ae));

  // CountVisitor counter;
  ast->Visit(&lowerer_);
  // expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.variableset());
  runner.Generate(std::move(test));
  std::string result = exec("gcc -g -c test.s && ld test.o && ./a.out");
  std::cout << result << std::endl;
  // CHANGE TO GENERATEEPILOGUE LATER
  return 0;
}
