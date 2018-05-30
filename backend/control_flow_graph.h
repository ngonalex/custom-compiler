#ifndef BACKEND_CONTROL_FLOW_GRAPH_H_
#define BACKEND_CONTROL_FLOW_GRAPH_H_

#include <vector>
#include <utility>

#include "utility/memory.h"
#include "backend/lowerer_visitor.h"
#include "backend/ir.h"

using cs160::make_unique;

namespace cs160 {
namespace backend {

// A CFG can be composed of different block types
enum BlockType {
  CONDITIONAL_BLOCK, LOOP_BLOCK,
  FUNCTION_BLOCK, TRUE_BLOCK,
  FALSE_BLOCK, END_BLOCK, NO_TYPE
};

// Individual nodes in the CGF
// leftnode_ will be true branches
// rightnode_ will be false branches
class ControlFlowGraphNode {
 public:
  ControlFlowGraphNode();
  ControlFlowGraphNode(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  std::vector<std::unique_ptr<struct ThreeAddressCode>> GetLocalBlock() const {
   //return std::move(localblock_);
    std::vector<std::unique_ptr<struct ThreeAddressCode>> local_block_copy;
    for (const auto& iter: localblock_) {
      auto block = make_unique<struct ThreeAddressCode>();
      block->target = iter->target;
      block->op = iter->op;
      local_block_copy.push_back(std::move(block));
    }
    return std::move(local_block_copy);

  }
  ControlFlowGraphNode(const ControlFlowGraphNode &copy);
  std::unique_ptr<ControlFlowGraphNode> GetLeftNode() const;
  std::unique_ptr<ControlFlowGraphNode> GetRightNode() const;
  int GetCreationOrder() const {
    return creation_order;
  }
  BlockType GetBlockType() const {
    return blocktype_;
  }
  ControlFlowGraphNode& operator=(ControlFlowGraphNode &copy);
  void SetLocalBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  void SetLeftNode(std::unique_ptr<ControlFlowGraphNode>);
  void SetRightNode(std::unique_ptr<ControlFlowGraphNode>);
  void SetCreationOrder(int order) {
    creation_order = order;
  }
  void SetBlockType(BlockType settype) {
    blocktype_ = settype;
  }

 private:
  BlockType blocktype_;
  int creation_order;
  std::vector<std::unique_ptr<struct ThreeAddressCode>> localblock_;
};

class ControlFlowGraph {
 public:
  ControlFlowGraph(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  std::vector<std::unique_ptr<ControlFlowGraphNode>> GetRoot() {
   return std::move(cfg_nodes_);
  }
  void CreateCFG(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  void Optimize();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> MakeThreeAddressCode();

 private:
  //std::unique_ptr<ControlFlowGraphNode> root_;
  std::vector<std::unique_ptr<ControlFlowGraphNode>> cfg_nodes_;
  std::vector<std::pair<int,int>> edges_;
};

} // namespace backend
} // namespace cs160

#endif // CONTROL_FLOW_GRAPH_H_