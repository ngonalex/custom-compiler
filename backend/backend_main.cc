
#include "backend/code_gen.h"
#include "abstract_syntax/abstract_syntax.h"
#include "backend/lowerer_visitor.h"
#include "utility/memory.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::ArithmeticExpr;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
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
using cs160::abstract_syntax::backend::AssignmentFromArithExp;
using cs160::abstract_syntax::backend::AssignmentFromNewTuple;
using cs160::abstract_syntax::backend::Dereference;
using cs160::abstract_syntax::backend::Conditional;
using cs160::abstract_syntax::backend::Loop;
using cs160::abstract_syntax::backend::Program;
using cs160::abstract_syntax::backend::FunctionCall;
using cs160::abstract_syntax::backend::FunctionDef;
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
  auto expr = make_unique<DivideExpr>(
    make_unique<IntegerExpr>(7), make_unique<IntegerExpr>(5));

  statements.push_back(std::move(
      make_unique<const AssignmentFromArithExp>(
        make_unique<const VariableExpr>("bob"),
        make_unique<const IntegerExpr>(5))));

  statements.push_back(std::move(make_unique<const AssignmentFromNewTuple>(
      make_unique<const VariableExpr>("x"),
      make_unique<const IntegerExpr>(3))));

  statements.push_back(std::move(make_unique<const AssignmentFromNewTuple>(
      make_unique<const Dereference>(
        make_unique<const VariableExpr>("x"),
        make_unique<const IntegerExpr>(1)),
      make_unique<const IntegerExpr>(10))));

  statements.push_back(std::move(make_unique<const AssignmentFromArithExp>(
      make_unique<const Dereference>(
        make_unique<const Dereference>(
          make_unique<const VariableExpr>("x"),
          make_unique<const IntegerExpr>(1)),
        make_unique<const IntegerExpr>(2)),
      make_unique<const IntegerExpr>(100))));

  statements.push_back(std::move(make_unique<const AssignmentFromArithExp>(
    make_unique<const Dereference>(
        make_unique<const VariableExpr>("x"),
        make_unique<const IntegerExpr>(2)),
    make_unique<const Dereference>(
      make_unique<const Dereference>(
        make_unique<const VariableExpr>("x"),
        make_unique<const IntegerExpr>(1)),
      make_unique<const IntegerExpr>(2)))));

  statements.push_back(std::move(make_unique<const AssignmentFromArithExp>(
    make_unique<const VariableExpr>("y"),
    make_unique<const Dereference>(
      make_unique<const VariableExpr>("x"),
      make_unique<const IntegerExpr>(2)))));

  auto arguments = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  arguments.push_back(std::move(make_unique<const VariableExpr>("bob")));

  statements.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("foo_retval"), "fact",
      std::move(arguments))));

  auto ae = make_unique<const VariableExpr>("foo_retval");
  auto foo_retval = make_unique<const AddExpr>(
    make_unique<const VariableExpr>("foo_retval"),
    make_unique<const IntegerExpr>(0));

  auto foo_params = std::vector<std::unique_ptr<const VariableExpr>>();
  foo_params.push_back(std::move(make_unique<const VariableExpr>("bob")));

  Statement::Block fact_body;
  Statement::Block true_fact;
  Statement::Block false_fact;

  auto arguments1 = std::vector<std::unique_ptr<const ArithmeticExpr>>();

  arguments1.push_back(std::move(make_unique<const SubtractExpr>(
    make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1))));

  false_fact.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("foo_retval"),
    make_unique<IntegerExpr>(1))));

  true_fact.push_back(std::move(make_unique<const FunctionCall>(
      make_unique<const VariableExpr>("bobMONEY"), "fact",
      std::move(arguments1))));

  true_fact.push_back(std::move(make_unique<AssignmentFromArithExp>(
    make_unique<VariableExpr>("foo_retval"),
    make_unique<MultiplyExpr>(
      make_unique<VariableExpr>("bobMONEY"),
      make_unique<VariableExpr>("bob")))));

  fact_body.push_back(std::move(make_unique<Conditional>(
    make_unique<GreaterThanExpr>(
      make_unique<VariableExpr>("bob"), make_unique<IntegerExpr>(1)),
      std::move(true_fact), std::move(false_fact))));

  auto foo_def = make_unique<const FunctionDef>("fact", std::move(foo_params),
                                                std::move(fact_body),
                                                std::move(foo_retval));

  FunctionDef::Block function_defs;
  function_defs.push_back(std::move(foo_def));

  auto ast = make_unique<const Program>(std::move(function_defs),
    std::move(statements), std::move(ae));

  // CountVisitor counter;
  ast->Visit(&lowerer_);
  // expr->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.globalset());
  runner.Generate(std::move(test));
  system("gcc -g -static test.s -o run && ./run");
  // CHANGE TO GENERATEEPILOGUE LATER
  return 0;
}
