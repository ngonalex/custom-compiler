#ifndef BACKEND_CODE_GEN_H_
#define BACKEND_CODE_GEN_H_

#include <iostream>
#include <fstream>  
#include <string>
#include <vector>

#include "lowerer_visitor.h"

namespace CS160 {
namespace backend {


class CodeGen {

    public:
    void Generate(std::vector<struct ThreeAddressCode*> blocks);
};

}  // namespace CS160
}  // namespace backend

#endif  // BACKEND_CODE_GEN_H_