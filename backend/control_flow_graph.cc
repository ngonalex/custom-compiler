#include "backend/control_flow_graph.h"

namespace cs160 {
namespace backend {

ControlFlowGraph::ControlFlowGraph
(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  CreateCFG(std::move(input));
}
std::pair<int,int> PairMakeHelp(const int a, const int b) {
  int front = a;
  int back = b;
  return std::make_pair(front,back);
}
ControlFlowGraphNode* RecursiveCreate(std::vector<ControlFlowGraphNode*> graph_set, 
  std::vector<std::pair<int,int>> edge_graph) {
  if (graph_set.empty()) {
    return NULL;
  } else if (graph_set.size() == 1) {
    ControlFlowGraphNode * temp = graph_set[0];
      graph_set.erase(graph_set.begin());
  } else {
    if (graph_set.front()->GetBlockType() == CONDITIONAL_BLOCK) {
      ControlFlowGraphNode* temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode* temp1 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode* temp2 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode* temp3 = RecursiveCreate(graph_set,edge_graph);
      int node0 = temp0->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int node2 = temp2->GetCreationOrder();
      int node3 = temp3->GetCreationOrder();
      edge_graph.push_back(std::make_pair(node0,node1));
      edge_graph.push_back(std::make_pair(node0,node2));
      edge_graph.push_back(std::make_pair(node1,node3));
      edge_graph.push_back(std::make_pair(node2,node3));
      return temp3;
    } else if (graph_set.front()->GetBlockType() == LOOP_BLOCK) { 
      ControlFlowGraphNode* temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode* temp1 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode* temp2 = RecursiveCreate(graph_set,edge_graph);
      int node0 = temp0->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int node2 = temp2->GetCreationOrder();
      edge_graph.push_back(std::make_pair(node0,node1));
      edge_graph.push_back(std::make_pair(node1,node0));
      edge_graph.push_back(std::make_pair(node0,node2));
      return temp2;
    } else { // NO TYPE
      ControlFlowGraphNode * temp = graph_set[0];
      graph_set.erase(graph_set.begin());
      return std::move(temp); 
    }
  }
}
std::vector<std::unique_ptr<struct ThreeAddressCode>> CopyBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>> copy) {
  std::vector<std::unique_ptr<struct ThreeAddressCode>> copy_object;
  for (const auto& iter: copy) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    copy_object.push_back(std::move(block));
  }
  return std::move(copy_object);
}
void ControlFlowGraph::CreateCFG(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  // Has to branch on:
  // IF/WHILE/FUNCTION
  // Each one has a different branching pattern
  std::vector<std::unique_ptr<ControlFlowGraphNode>> cfg_vector;
  //ControlFlowGraphNode new_node,true_node, false_node; 
  std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
  int creation_order = 0;
  for (const auto &iter: input) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    new_block.push_back(std::move(block));
    if (iter->op.opcode() == CONDITIONAL) {
      //std::vector<std::unique_ptr<struct ThreeAddressCode>> copy_block = CopyBlock(new_block);
      //ControlFlowGraphNode conditional_block(std::move(new_block));
      auto conditional_block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      conditional_block->SetCreationOrder(creation_order);
      conditional_block->SetBlockType(CONDITIONAL_BLOCK);
      //new_block.clear();
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_vector.push_back(std::move(conditional_block));
      ++creation_order;
    } else if (iter->op.opcode() == LOOP) {
      //std::vector<std::unique_ptr<struct ThreeAddressCode>> copy_block = CopyBlock(new_block);
      //ControlFlowGraphNode loop_block(std::move(new_block));
      auto loop_block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      loop_block->SetCreationOrder(creation_order);
      loop_block->SetBlockType(LOOP_BLOCK);
      //new_block.clear();
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_vector.push_back(std::move(loop_block));
      ++creation_order;
    } else if (iter->op.opcode() == JUMP) {
      //std::vector<std::unique_ptr<struct ThreeAddressCode>> copy_block = CopyBlock(new_block);
      //ControlFlowGraphNode block(std::move(new_block));
      auto block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      block->SetCreationOrder(creation_order);
      block->SetBlockType(NO_TYPE);
      //new_block.clear();
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_vector.push_back(std::move(block));
      ++creation_order;
    } else if (iter == input.back()) {
      //std::vector<std::unique_ptr<struct ThreeAddressCode>> copy_block = CopyBlock(new_block);
      auto block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      //ControlFlowGraphNode block(std::move(new_block));
      block->SetCreationOrder(creation_order);
      block->SetBlockType(END_BLOCK);
      //new_block.clear();
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_vector.push_back(std::move(block));
      ++creation_order;
    } else {
      //No need to make a block
    }

  } 
  std::vector<std::pair<int,int>> edge_vector;
  RecursiveCreate(std::move(cfg_vector),edge_vector);
  edges_ = edge_vector;
}

ControlFlowGraphNode::ControlFlowGraphNode() {
  creation_order = 0;
  blocktype_ = NO_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  for (const auto& iter: input) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    localblock_.push_back(std::move(block));
  }
  creation_order = 0;
  blocktype_ = NO_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode(const ControlFlowGraphNode &copy) {
  //std::vector<std::unique_ptr<ThreeAddressCode>> block_copy;
  localblock_.clear();
  // Because it is a vector of unique pointers
  // Each element has to be manually copied
  for (const auto& iter: copy.GetLocalBlock()) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    localblock_.push_back(std::move(block));
  }
  //localblock_ = block_copy;
  creation_order = copy.GetCreationOrder();
  blocktype_ = copy.GetBlockType();
}

ControlFlowGraphNode& ControlFlowGraphNode::operator=(ControlFlowGraphNode &copy) {
  //localblock_ = copy.GetLocalBlock();
  localblock_.clear();
  for (const auto& iter: copy.GetLocalBlock()) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    localblock_.push_back(std::move(block));
  }
  creation_order = copy.GetCreationOrder();
  blocktype_ = copy.GetBlockType();
  return *this;
}

void ControlFlowGraphNode::SetLocalBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  for (const auto& iter: input) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    localblock_.push_back(std::move(block));
  }
}

} //backend
} //cs160
