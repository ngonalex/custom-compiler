#ifndef BACKEND_LOWERER_VISITOR_H_
#define BACKEND_LOWERER_VISITOR_H_

#include <string>
#include <vector>

// #include "utility/assert.h"
#include "abstract_syntax/abstract_syntax.h"
#include "backend/ir.h"
#include "utility/memory.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::BinaryOperatorExpr;

namespace cs160 {
namespace backend {

class LowererVisitor : public AstVisitor {
 public:
  LowererVisitor() {}
  ~LowererVisitor() {}
  
  const std::string GetOutput() const;
  void VisitIntegerExpr(const IntegerExpr& exp);
  void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp) {}
  void VisitAddExpr(const AddExpr& exp);
  void VisitSubtractExpr(const SubtractExpr& exp);
  void VisitMultiplyExpr(const MultiplyExpr& exp);
  void VisitDivideExpr(const DivideExpr& exp);

  std::vector<std::unique_ptr<struct ThreeAddressCode>> GetIR() {
    return std::move(blocks_);
  }

 private:
  std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_LOWERER_VISITOR_H_
