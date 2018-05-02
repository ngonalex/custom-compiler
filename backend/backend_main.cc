
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
using cs160::abstract_syntax::backend::BinaryOperatorExpr;
using cs160::abstract_syntax::backend::Assignment;
using cs160::abstract_syntax::backend::VariableExpr;
using cs160::abstract_syntax::backend::Program;
using cs160::backend::LowererVisitor;
using cs160::backend::ThreeAddressCode;
using cs160::backend::CodeGen;
using cs160::make_unique;


int main() {
  LowererVisitor lowerer_;
  auto expr = cs160::make_unique<DivideExpr>(
    make_unique<IntegerExpr>(7), make_unique<IntegerExpr>(5));

  //expr->Visit(&lowerer_);
  std::vector<std::unique_ptr<const Assignment>> assignments;
  assignments.push_back(std::move(
      make_unique<const Assignment>(make_unique<const VariableExpr>("bob"),
                                    make_unique<const IntegerExpr>(42))));
  auto ae = make_unique<const AddExpr>(
      make_unique<const SubtractExpr>(
          make_unique<const DivideExpr>(make_unique<const IntegerExpr>(12),
                                        make_unique<const IntegerExpr>(3)),
          make_unique<const IntegerExpr>(4)),
      make_unique<const MultiplyExpr>(make_unique<const IntegerExpr>(3),
                                      make_unique<const IntegerExpr>(2)));

  auto ast = make_unique<const Program>(std::move(assignments), std::move(ae));
  ast->Visit(&lowerer_);

  std::ofstream file = std::ofstream("test.s");
  CodeGen runner = CodeGen(file);
  auto test = lowerer_.GetIR();
  runner.GenerateData(lowerer_.variableset());
  runner.Generate(std::move(test));
  //std::string result = exec("gcc -c test.s && ld test.o && ./a.out");
  // CHANGE TO GENERATEEPILOGUE LATER
  return 0;
}
