#ifndef BACKEND_IR_H_
#define BACKEND_IR_H_

#include <string>

namespace cs160 {
namespace backend {

enum Type {
  LOAD,
  ADD,
  SUB,
  MULT,
  DIV,
  NONE
};

enum OperandType {
  REGISTER,
  INT
};

class Register {
 public:
  explicit Register(std::string name) : name_(name) {}

  std::string name() const {return name_;}

 private:
  std::string name_;
};

// For ThreeAddressCodes, arg1/arg2 can be a Register or an Int
// Look at this later, this probably can be designed much better
class Operand {
 public:
  explicit Operand(Register reg) : reg_(reg), 
    value_(0), optype_(REGISTER) {
    // ASSERT (Registers can't have values )
  }
  explicit Operand(int value) : reg_(Register("")), 
    value_(value), optype_(INT) {
    // ASSERT (only ints can have values)
  }

  Register reg() const {return reg_;}
  int value() const {return value_;}
  OperandType optype() const {return optype_;}

 private:
  Register reg_;
  int value_;
  OperandType optype_;
};

class Opcode {
 public:
  explicit Opcode(Type type) : opcode_(type) {}

  Type opcode() const {return opcode_;}

  bool operator !=(const Opcode &a) const {
    return !(this->opcode() == a.opcode());
  }

 private:
  Type opcode_;
};


// Structure to hold a 3Address, Basically 1 block
// Right now all of them are strings change to classes later
struct ThreeAddressCode {
  Register target;
  Opcode op;
  Operand arg1;
  Operand arg2;

  struct ThreeAddressCode* next;
  struct ThreeAddressCode* prev;
  ThreeAddressCode() : target(Register("")), op(NONE),
    arg1(Operand(0)), arg2(Operand(0)) {}
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_IR_H_

