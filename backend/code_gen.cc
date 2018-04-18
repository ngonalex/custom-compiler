#include "code_gen.h"

namespace CS160 {
namespace backend {


class CodeGen {

 public:

  CodeGen() {
     outfile_ = std::ofstream("test.s");
  }

  void CodeGen::Generate(std::vector<struct ThreeAddressCode*> blocks) {
    // outfile_ << "my text here!" << std::endl;
    // outfile_.close();
    // boiler code here 
    for(int i = 0; i < blocks.size(); ++i) {
        ThreeAddressCode* code = blocks[i];
        std::string OpCode = code->op; 
        if(OpCode == "load"){
            outfile_ << "li " + code->target + " " + code-> arg1 << std::endl;
        }else if (OpCode == "+"){
            outfile_ << "add " + code->target + " " code-> arg1 + " " + code-> arg2 <<std::endl;
        }else if (OpCode == "-"){
            outfile_ << "sub " + code->target + " " code-> arg1 + " " + code-> arg2 <<std::endl;
        }else if (OpCode == "*"){
            outfile_ << "mult " + code->target + " " code-> arg1 + " " + code-> arg2 <<std::endl;
        }else if (OpCode == "/"){
            outfile_ << "div " + code->target + " " code-> arg1 + " " + code-> arg2 <<std::endl;
        }
    }

    outfile_.close();
  }


 private:
  std::ofstream outfile_;


};

}  // namespace CS160
}  // namespace backend