#ifndef BACKEND_IR_H_
#define BACKEND_IR_H_

#include <string>
#include "utility/assert.h"

namespace cs160 {
namespace backend {

// Somewhat unwieldy and long is there a better way to do this?
enum Type {
  INTLOAD,
  VARLOAD,
  VARASSIGNLOAD,
  FUNARGLOAD,
  FUNRETLOAD,
  ADD,
  SUB,
  MULT,
  DIV,
  LESSTHAN,
  LESSTHANEQ,
  GREATERTHAN,
  GREATERTHANEQ,
  EQUAL,
  LOGAND,
  LOGOR,
  LOGNOT,
  LOOP,
  CONDITIONAL,
  JUMP,
  JEQUAL,
  JNOTEQUAL,
  JGREATER,
  JGREATEREQ,
  JLESS,
  JLESSEQ,
  LABEL,
  FUNCALL,
  FUNRETEP,
  FUNDEF,
  FUNPROLOGUE,
  FUNEPILOGUE,
  PRINTARITH,
  NOTYPE,
  LHSDEREFERENCE,
  RHSINTDEFERENCE,
  RHSTUPLEDEREFENCE,
  NEWTUPLE,
  VARCHILDTUPLE,  // change this later
};

enum OperandType { OPREGISTER, INT };

enum TargetType { TARGETREGISTER, TARGETLABEL };

enum RegisterType { VIRTUALREG, VARIABLEREG, NOREG };

// Maybe unneeded
enum Scope {
  GLOBAL,
  FUNCTION
};

enum VariableType {
  LEFTHAND,
  RIGHTHAND,
};

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
  Register() : name_(""), type_(NOREG) {}
  std::string name() const { return name_; }
  RegisterType type() const { return type_; }

 private:
  std::string name_;
  RegisterType type_;
};

// For ThreeAddressCodes, arg1/arg2 can be a Register or an Int
// Look at this later, this probably can be designed much better
class Operand {
 public:
  explicit Operand(Register reg) : reg_(reg), value_(0), optype_(OPREGISTER) {
    // ASSERT (Registers can't have values )
  }
  explicit Operand(int value) : reg_(Register()), value_(value), optype_(INT) {
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
  explicit Opcode(Type type) : opcode_(type) {}

  void ChangeOpCode(Type type) { opcode_ = type; }
  Type opcode() const { return opcode_; }

  bool operator!=(const Opcode& a) const {
    return !(this->opcode() == a.opcode());
  }

 private:
  Type opcode_;
};

class Target {
 public:
  explicit Target(Register reg)
      : reg_(reg), label_(Label("")), type_(TARGETREGISTER) {}
  explicit Target(Label label)
      : reg_(Register()), label_(label), type_(TARGETLABEL) {}

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

  struct ThreeAddressCode* next;
  struct ThreeAddressCode* prev;
  ThreeAddressCode()
      : target(Register()), op(NOTYPE), arg1(Operand(0)), arg2(Operand(0)) {}
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_IR_H_
