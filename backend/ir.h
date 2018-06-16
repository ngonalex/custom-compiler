#ifndef BACKEND_IR_H_
#define BACKEND_IR_H_

#include <string>
#include "utility/assert.h"

namespace cs160 {
namespace backend {

enum OpcodeType {
  INT_LOAD,
  VAR_LOAD,
  VAR_ASSIGN_LOAD,
  FUN_ARG_LOAD,
  FUN_RET_LOAD,
  ADD,
  SUB,
  MULT,
  DIV,
  LESS_THAN,
  LESS_THAN_EQ,
  GREATER_THAN,
  GREATER_THAN_EQ,
  EQUAL,
  LOG_AND,
  LOG_OR,
  LOG_NOT,
  LOOP,
  CONDITIONAL,
  JUMP,
  JMP_EQUAL,
  JMP_NOT_EQUAL,
  JMP_GREATER_THAN,
  JMP_GREATER_THAN_EQ,
  JMP_LESS_THAN,
  JMP_LESS_THAN_EQ,
  LABEL,
  FUN_CALL,
  FUN_RET_EP,
  FUN_DEF,
  FUN_PROLOGUE,
  FUN_EPILOGUE,
  PRINT_ARITH,
  NO_TYPE,
  LHS_DEREFERENCE,
  RHS_DEREFERENCE,
  NEW_TUPLE,
  VAR_CHILD_TUPLE
};

enum PrintFlag {
  PRINT_ONLY_RESULT,
  PRINT_LAST_ARITHMETIC_EXPR,
  PRINT_DEBUG,
  PRINT_PROGRAM
};

enum OperandType { OP_REGISTER, OP_INT };

enum TargetType { TARGET_REGISTER, TARGET_LABEL };

enum RegisterType { VIRTUAL_REG, VARIABLE_REG, DEREF_REG, NO_REG };

enum Scope { GLOBAL, FUNCTION };

enum FlagType { TYPE_FLAG, EXISTENCE_FLAG, SIZE_FLAG, OBJECT_FLAG, NO_FLAG };

enum VariableType { LEFT_HAND_VAR, RIGHT_HAND_VAR };

class Label {
 public:
  explicit Label(std::string labelname) : name_(labelname) {}
  std::string name() const { return name_; }

 private:
  std::string name_;
};

class Register {
 public:
  Register(std::string name, RegisterType type) : name_(name), type_(type) {}
  Register() : name_(""), type_(NO_REG) {}
  std::string name() const { return name_; }
  RegisterType type() const { return type_; }
  void ChangeRegisterName(std::string newname) { name_ = newname; }

 private:
  std::string name_;
  RegisterType type_;
};

// For ThreeAddressCodes, arg1/arg2 can be a Register or an Int
// Look at this later, this probably can be designed much better
class Operand {
 public:
  explicit Operand(Register reg) : reg_(reg), value_(0), optype_(OP_REGISTER) {
    // ASSERT (Registers can't have values )
  }
  explicit Operand(int value)
      : reg_(Register()), value_(value), optype_(OP_INT) {
    // ASSERT (only ints can have values)
  }
  // explicit Operand(VariableOperand var) : reg_(Register("")),
  //   value_(0), varname_(var), optype_(VARIABLE) {
  //   // ASSERT (only ints can have values)
  // }

  Register reg() const { return reg_; }
  int value() const { return value_; }
  // VariableOperand varname() const {return varname_;}
  OperandType optype() const { return optype_; }

 private:
  Register reg_;
  int value_;
  // VariableOperand varname_;
  OperandType optype_;
};

class Opcode {
 public:
  explicit Opcode(OpcodeType type) : opcode_(type) {}

  void ChangeOpCode(OpcodeType type) { opcode_ = type; }
  OpcodeType opcode() const { return opcode_; }

  bool operator!=(const Opcode& a) const {
    return !(this->opcode() == a.opcode());
  }

 private:
  OpcodeType opcode_;
};

class Target {
 public:
  explicit Target(Register reg)
      : reg_(reg), label_(Label("")), type_(TARGET_REGISTER) {}
  explicit Target(Label label)
      : reg_(Register()), label_(label), type_(TARGET_LABEL) {}

  Register reg() const { return reg_; }
  Label label() const { return label_; }
  TargetType type() const { return type_; }

 private:
  Register reg_;
  Label label_;
  TargetType type_;
};

// Structure to hold a 3Address, Basically 1 block
// Right now all of them are strings change to classes later
struct ThreeAddressCode {
  Target target;
  Opcode op;
  Operand arg1;
  Operand arg2;

  ThreeAddressCode()
      : target(Register()), op(NO_TYPE), arg1(Operand(0)), arg2(Operand(0)) {}
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_IR_H_
