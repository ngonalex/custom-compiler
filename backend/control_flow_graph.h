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
  FALSE_BLOCK, END_BLOCK, NO_TYPE, ERROR_TYPE
};

enum EdgeType {
  CONDITIONAL_TRUE, CONDITIONAL_FALSE,
  CONDITIONAL_TRUE_RETURN, CONDITIONAL_FALSE_RETURN,
  LOOP_TRUE, LOOP_FALSE, LOOP_RETURN,
  TYPELESS_EDGE  
};
struct Edge {
  std::pair<int,int> edge_pair = std::make_pair(0,0);
  EdgeType edge_type;
  Edge(std::pair<int,int> edge_input,
    EdgeType edge_type_input) : 
    edge_pair(edge_input) { edge_type = edge_type_input; }
};

// Individual nodes in the CGF
// leftnode_ will be true branches
// rightnode_ will be false branches
class ControlFlowGraphNode {
 public:
  ControlFlowGraphNode();
  ControlFlowGraphNode(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  std::vector<std::unique_ptr<struct ThreeAddressCode>> GetLocalBlock() {
   //return std::move(localblock_);
    std::vector<std::unique_ptr<struct ThreeAddressCode>> local_block_copy;
    for (auto& iter: localblock_) {
      auto block = make_unique<struct ThreeAddressCode>();
      block->target = iter->target;
      block->op = iter->op;
      block->arg1 = iter->arg1;
      block->arg2 = iter->arg2;
      local_block_copy.push_back(std::move(block));
    }
    return std::move(local_block_copy);

  }
  ControlFlowGraphNode( ControlFlowGraphNode &copy);
  int GetCreationOrder() {
    return creation_order;
  }
  BlockType GetBlockType() {
    return blocktype_;
  }
  ControlFlowGraphNode& operator=(ControlFlowGraphNode &copy);
  ControlFlowGraphNode operator=(ControlFlowGraphNode copy);
  void SetLocalBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
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
  std::vector<Edge> edges_;
};

} // namespace backend
} // namespace cs160

#endif // CONTROL_FLOW_GRAPH_H_