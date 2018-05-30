#include "backend/control_flow_graph.h"

namespace cs160 {
namespace backend {

ControlFlowGraph::ControlFlowGraph
(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  CreateCFG(std::move(input));
}

std::unique_ptr<ControlFlowGraphNode> RecursiveCreate(std::vector<std::unique_ptr<ControlFlowGraphNode>> graph_set) {
  if (graph_set.empty()) {
    return NULL;
  } else if (graph_set.size() == 1) {
    return std::move(graph_set[0]);
  } else {
    std::vector<std::unique_ptr<ControlFlowGraphNode>> sub_vector_1;
    std::vector<std::unique_ptr<ControlFlowGraphNode>> sub_vector_2;
    for(const auto &iter: graph_set) {
      std::unique_ptr<ControlFlowGraphNode> copy_node_1;
      std::unique_ptr<ControlFlowGraphNode> copy_node_2;
      copy_node_1->SetBlockType(iter->GetBlockType());
      copy_node_1->SetCreationOrder(iter->GetCreationOrder());
      copy_node_1->SetLocalBlock(std::move(iter->GetLocalBlock()));
      copy_node_1->SetLeftNode(std::move(iter->GetLeftNode()));
      copy_node_1->SetRightNode(std::move(iter->GetRightNode()));
      copy_node_2->SetBlockType(iter->GetBlockType());
      copy_node_2->SetCreationOrder(iter->GetCreationOrder());
      copy_node_2->SetLocalBlock(std::move(iter->GetLocalBlock()));
      copy_node_2->SetLeftNode(std::move(iter->GetLeftNode()));
      copy_node_2->SetRightNode(std::move(iter->GetRightNode()));
      sub_vector_1.push_back(std::move(copy_node_1));
      sub_vector_2.push_back(std::move(copy_node_2));
    }
    if (graph_set.front()->GetBlockType() == CONDITIONAL_BLOCK) {
      sub_vector_1.erase(sub_vector_1.begin());
      sub_vector_2.erase(sub_vector_2.begin(),sub_vector_2.begin()+2);
      graph_set.front()->SetLeftNode(RecursiveCreate(std::move(sub_vector_1)));
      graph_set.front()->SetRightNode(RecursiveCreate(std::move(sub_vector_2)));
      return graph_set.front();
    } else if (graph_set.front()->GetBlockType() == LOOP_BLOCK) {
      sub_vector_1.erase(sub_vector_1.begin());
      sub_vector_2.erase(sub_vector_2.begin(),sub_vector_2.begin()+2);
      graph_set.front()->SetLeftNode(RecursiveCreate(std::move(sub_vector_1)));
      graph_set.front()->SetRightNode(RecursiveCreate(std::move(sub_vector_2)));
      return graph_set.front();
    }
    return std::move(graph_set[0]);
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
    } else if (iter->op.opcode() == FUNCALL) {
      //std::vector<std::unique_ptr<struct ThreeAddressCode>> copy_block = CopyBlock(new_block);
      //ControlFlowGraphNode function_block(std::move(new_block));
      auto function_block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      function_block->SetCreationOrder(creation_order);
      function_block->SetBlockType(FUNCTION_BLOCK);
      //new_block.clear();
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_vector.push_back(std::move(function_block));
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
  std::unique_ptr<ControlFlowGraphNode> root = RecursiveCreate(std::move(cfg_vector));
  root_ = std::move(root);
}

ControlFlowGraphNode::ControlFlowGraphNode() {
  creation_order = 0;
  leftnode_ = NULL;
  rightnode_ = NULL;
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
  leftnode_ = NULL;
  rightnode_ = NULL;
  blocktype_ = NO_TYPE;
}
std::unique_ptr<ControlFlowGraphNode> ControlFlowGraphNode::GetLeftNode() const {
  auto leftcopy = make_unique<class ControlFlowGraphNode>();
  leftcopy->SetBlockType(leftnode_->GetBlockType()); 
  leftcopy->SetCreationOrder(leftnode_->GetCreationOrder());
  leftcopy->SetLocalBlock(leftnode_->GetLocalBlock());
  leftcopy->SetLeftNode(leftnode_->GetLeftNode());
  leftcopy->SetRightNode(leftnode_->GetRightNode());
  return std::move(leftcopy); 
}

std::unique_ptr<ControlFlowGraphNode> ControlFlowGraphNode::GetRightNode() const {
  auto rightcopy = make_unique<class ControlFlowGraphNode>();
  rightcopy->SetBlockType(rightnode_->GetBlockType()); 
  rightcopy->SetCreationOrder(rightnode_->GetCreationOrder());
  rightcopy->SetLocalBlock(rightnode_->GetLocalBlock());
  rightcopy->SetLeftNode(rightnode_->GetLeftNode());
  rightcopy->SetRightNode(rightnode_->GetRightNode());
  return std::move(rightcopy); 
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
  leftnode_ = copy.GetLeftNode();
  rightnode_ = copy.GetRightNode();
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
  leftnode_ = copy.GetLeftNode();
  rightnode_ = copy.GetRightNode();
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

void ControlFlowGraphNode::SetLeftNode(std::unique_ptr<ControlFlowGraphNode> input) {
  leftnode_ = std::move(input);
}

void ControlFlowGraphNode::SetRightNode(std::unique_ptr<ControlFlowGraphNode> input) {
  rightnode_ = std::move(input);
}


} //backend
} //cs160
