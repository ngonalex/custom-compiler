#include "backend/control_flow_graph.h"

namespace cs160 {
namespace backend {

// This just exists for the control_flow_graph_test.cc
ControlFlowGraph::ControlFlowGraph() {}

ControlFlowGraph::ControlFlowGraph
(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  CreateCFG(std::move(input));
}

// Use Recursion to find all the edges in the graph
ControlFlowGraphNode* RecursiveCreate(
    std::vector<ControlFlowGraphNode*> &graph_set,
    std::vector<Edge> &edge_graph) {
  if (graph_set.empty()) {
    // If this occurs then there's a bigger problem
    std::vector<std::unique_ptr<struct ThreeAddressCode>> empty;
    auto error_node = make_unique<class ControlFlowGraphNode>(std::move(empty));
    error_node->SetBlockType(ERROR_TYPE);
    std::cerr <<
      "Error block created, edge creation failed. Creating Error Node"
      << std::endl;
    return error_node.get();
  } else if (graph_set.size() == 1) {
    ControlFlowGraphNode* temp = graph_set[0];
    graph_set.erase(graph_set.begin());
    return temp;
  } else {
    if (graph_set.front()->GetBlockType() == CONDITIONAL_BLOCK) {
      // Should look like this:
      //      Condition
      //      |     |
      //    True   False
      //      \     /
      //        END
      // temp0 = Condition
      // temp1 = True
      // temp2 = False
      // temp3 = END
      // ControlFlowGraphNode* temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode* temp0 = graph_set.front();
      graph_set.erase(graph_set.begin());
      ControlFlowGraphNode* next_true_node = graph_set.front();
      ControlFlowGraphNode* temp1 = RecursiveCreate(graph_set, edge_graph);
      ControlFlowGraphNode* temp2 = RecursiveCreate(graph_set, edge_graph);
      ControlFlowGraphNode* temp3 = RecursiveCreate(graph_set, edge_graph);
      int node0 = temp0->GetCreationOrder();
      int next_true = next_true_node->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int next_false = node1 + 1;
      int node2 = temp2->GetCreationOrder();
      int node3 = temp3->GetCreationOrder();
      // Has to handle the possibility of 6 different nodes
      // If next_true == node1 and next_false == node2
      // Then that means there isn't a nested case
      Edge edge1(std::make_pair(node0, next_true), CONDITIONAL_TRUE);
      Edge edge2(std::make_pair(node0, next_false), CONDITIONAL_FALSE);
      Edge edge3(std::make_pair(node1, node3), CONDITIONAL_TRUE_RETURN);
      Edge edge4(std::make_pair(node2, node3), CONDITIONAL_FALSE_RETURN);
      edge_graph.push_back(edge1);
      edge_graph.push_back(edge2);
      edge_graph.push_back(edge3);
      edge_graph.push_back(edge4);
      return temp3;
    } else if (graph_set.front()->GetBlockType() == LOOP_BLOCK) {
      // Should look like this:
      //      Loop
      //      /\  \
      //     True  \
      //           False
      // temp0 = Loop
      // temp1 = True
      // temp2 = False
      // ControlFlowGraphNode* temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode* temp0 = graph_set.front();
      graph_set.erase(graph_set.begin());
      // Needed for nested statements
      ControlFlowGraphNode* next_node = graph_set.front();
      ControlFlowGraphNode* temp1 = RecursiveCreate(graph_set, edge_graph);
      ControlFlowGraphNode* temp2 = RecursiveCreate(graph_set, edge_graph);
      int node0 = temp0->GetCreationOrder();
      int next = next_node->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int node2 = temp2->GetCreationOrder();
      // Next is there to handle nested cases
      // If next == node1 then there isn't a nested case
      Edge edge1(std::make_pair(node0, next), LOOP_TRUE);
      Edge edge2(std::make_pair(node1, node0), LOOP_RETURN);
      Edge edge3(std::make_pair(node0, node2), LOOP_FALSE);
      edge_graph.push_back(edge1);
      edge_graph.push_back(edge2);
      edge_graph.push_back(edge3);
      return temp2;
    } else {
      // It is a type that we don't need to branch on
      ControlFlowGraphNode* temp = graph_set[0];
      graph_set.erase(graph_set.begin());
      return temp;
    }
  }
}

// Iterates through the IR
// Creates blocks depending on what kind of op type it sees
void ControlFlowGraph::CreateCFG(std::vector<std::unique_ptr<
struct ThreeAddressCode>> input) {
  // Has to branch on:
  // IF/WHILE/FUNCTION
  // Each one has a different branching pattern
  // std::vector<std::unique_ptr<ControlFlowGraphNode>> cfg_vector;
  cfg_nodes_.clear();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
  int creation_order = 0;
  for (auto &iter : input) {
    // Move IR to the block
    OpcodeType op_type = iter->op.opcode();
    bool is_end = (iter == input.back());
    new_block.push_back(std::move(iter));
    // Create a block if neccessary
    if (op_type == CONDITIONAL) {
      auto conditional_block =
      make_unique<class ControlFlowGraphNode>(std::move(new_block));
      conditional_block->SetCreationOrder(creation_order);
      conditional_block->SetBlockType(CONDITIONAL_BLOCK);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(conditional_block));
      ++creation_order;
    } else if (op_type == LOOP) {
      auto loop_block =
      make_unique<class ControlFlowGraphNode>(std::move(new_block));
      loop_block->SetCreationOrder(creation_order);
      loop_block->SetBlockType(LOOP_BLOCK);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(loop_block));
      ++creation_order;
    } else if (op_type == JUMP) {
      auto block =
      make_unique<class ControlFlowGraphNode>(std::move(new_block));
      block->SetCreationOrder(creation_order);
      block->SetBlockType(NO_BLOCK_TYPE);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(block));
      ++creation_order;
    } else if (is_end) {
      // Reached the end of the IR
      // Make sure to create a block with the leftovers
      auto block =
      make_unique<class ControlFlowGraphNode>(std::move(new_block));
      block->SetCreationOrder(creation_order);
      block->SetBlockType(END_BLOCK);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(block));
      // Theres no reason to actually increment
      ++creation_order;
    } else {
      // No need to make a block
    }
  }
  // Use CFGN to recursively create edge vector
  std::vector<ControlFlowGraphNode*> cfg_pointers;
  for (auto &iter : cfg_nodes_) {
    cfg_pointers.push_back(iter.get());
  }
  std::vector<Edge> edge_vector;
  RecursiveCreate(cfg_pointers, edge_vector);
  edges_ = edge_vector;
}

// Apply a local optimization
ControlFlowGraphNode* MarkSweep(std::vector<std::string> &live_set,
    ControlFlowGraphNode* apply_sweep) {
  std::vector<std::unique_ptr<struct ThreeAddressCode>> optimize_block =
  std::move(apply_sweep->GetLocalUniqueBlock());
  // Create a local virtual register list
  // Does not need to be passed along because virtual registers
  // are used immediately when they are made
  std::vector<std::string> virtual_register_list;
  // Reverse iterate through the IR
  for (std::vector<std::unique_ptr<struct ThreeAddressCode>>::
  reverse_iterator iter = optimize_block.rbegin(); iter !=
  optimize_block.rend(); ++iter) {
    // Target is the LHS check
    // Opcode is the RHS check
    // This variable is so we prevent segfaults via short circuiting
    // I probably could've just checked if iter was NULL
    // But admittedly I forgot
    bool deletionperformed = false;
    // Check LHS for Variable
    if (iter->get()->target.reg().type() == VARIABLE_REG) {
      // Check if the variable is not in the liveset
      if (std::find(live_set.begin(), live_set.end()
      , iter->get()->target.reg().name()) == live_set.end()) {
        // Check if variable isn't being used on the RHS
        if (iter->get()->target.reg().name() != iter->get()->arg1.reg().name()
        || iter->get()->target.reg().name()
        != iter->get()->arg2.reg().name()) {
          // Remove the IR because it is not alive
          // Set the Unique_ptr to NULL
          // Codegen will thus ignore it
          iter->reset();
          deletionperformed = true;
        }
      } else {
        // Remove it from the live set
        live_set.erase(std::remove(live_set.begin(), live_set.end()
        , iter->get()->target.reg().name()), live_set.end());
      }
    }
    // Check LHS for Virtual Register
    if (!deletionperformed &&
    iter->get()->target.reg().type() == VIRTUAL_REG) {
      // Check if the variable is not in the liveset
      if (std::find(virtual_register_list.begin(), virtual_register_list.end()
      , iter->get()->target.reg().name()) == virtual_register_list.end()) {
        // Check if variable isn't being used on the RHS
        if (iter->get()->target.reg().name() != iter->get()->arg1.reg().name()
        || iter->get()->target.reg().name()
        != iter->get()->arg2.reg().name()) {
          // Remove the IR because it is not alive
          // Set the Unique_ptr to NULL
          // Codegen will thus ignore it
          iter->reset();
          deletionperformed = true;
        }
      } else {
        // Remove it from the live set
        virtual_register_list.erase(std::remove(
          virtual_register_list.begin(), virtual_register_list.end()
          , iter->get()->target.reg().name()), virtual_register_list.end());
      }
    }
    // Check RHS for Variable
    if (!deletionperformed && iter->get()->arg1.reg().type() == VARIABLE_REG) {
      // Check if the variable is not in the liveset
      if (std::find(live_set.begin(), live_set.end()
      , iter->get()->target.reg().name()) == live_set.end()) {
        // If not it's ok to add to the list
        live_set.push_back(iter->get()->arg1.reg().name());
      }
    }
    // Check RHS for Variable
    if (!deletionperformed && iter->get()->arg2.reg().type() == VARIABLE_REG) {
      // Check if the variable is not in the liveset
      if (std::find(live_set.begin(), live_set.end()
      , iter->get()->target.reg().name()) == live_set.end()) {
        // If not it's ok to add to the list
        live_set.push_back(iter->get()->arg2.reg().name());
      }
    }
    // Check RHS for Virtual Register
    if (!deletionperformed && iter->get()->arg1.reg().type() == VIRTUAL_REG) {
      // Check if the variable is not in the registerset
      if (std::find(virtual_register_list.begin(), virtual_register_list.end()
      , iter->get()->target.reg().name()) == virtual_register_list.end()) {
        // If not it's ok to add to the list
        virtual_register_list.push_back(iter->get()->arg1.reg().name());
      }
    }
    // Check RHS for Virtual Register
    if (!deletionperformed && iter->get()->arg2.reg().type() == VIRTUAL_REG) {
      // Check if the variable is not in the registerset
      if (std::find(virtual_register_list.begin(), virtual_register_list.end()
      , iter->get()->target.reg().name()) == virtual_register_list.end()) {
        // If not it's ok to add to the list
        virtual_register_list.push_back(iter->get()->arg2.reg().name());
      }
    }
    // This looks sort of weird but because of how the IR is designed
    // Function Definitions is always at the bottom of the code
    // Because the optimization begins from the last node
    // And the local operations reverse traverses the node
    // Function Defs will always be seen before other CFG related blocks
    // Their variables are local to the Function Def so we clear the set
    // This prevents situations where things don't get optimized out
    if (!deletionperformed && iter->get()->op.opcode() == FUN_DEF) {
      live_set.clear();
      virtual_register_list.clear();
    }
  }
  apply_sweep->SetLocalBlock(std::move(optimize_block));
  return apply_sweep;
}

// String Vector Merge helper function
std::vector<std::string> MergeVector(std::vector<std::string> vector1
, std::vector<std::string> vector2) {
  std::vector<std::string> return_vector = vector1;
  for (auto iter : vector2) {
    std::vector<std::string>::iterator find_value =
    std::find(vector2.begin(), vector2.end(), iter);
    if (find_value != vector2.end()) {
      return_vector.push_back(iter);
    }
  }
  return return_vector;
}

// Starts at the End of the Graph and reverse traverse upwards
// This is to find how the live_set should be passed along
// Very strange recursion is needed because of the difficulty of going backwards
std::pair<std::vector<std::string>, ControlFlowGraphNode*> RecursiveFindPath(
    std::vector<ControlFlowGraphNode*> passed_cfgn_vec,
    std::vector<Edge> edges, std::vector<std::string> live_set,
    ControlFlowGraphNode* optimize_node, std::vector<int> &visited) {
  // Optimize the local block
  int node_number = optimize_node->GetCreationOrder();
  optimize_node = MarkSweep(live_set, optimize_node);
  visited.push_back(optimize_node->GetCreationOrder());
  EdgeType edge1 = TYPELESS_EDGE;
  EdgeType edge2 = TYPELESS_EDGE;
  int block1 = -1;
  int block2 = -1;
  // Look for edges that lead to the node
  for (auto iter : edges) {
    if (iter.edge_pair.second == node_number) {
      if (edge1 == TYPELESS_EDGE) {
        edge1 = iter.edge_type;
        block1 = iter.edge_pair.first;
      } else if (edge2 == TYPELESS_EDGE) {
        edge2 = iter.edge_type;
        block2 = iter.edge_pair.first;
      } else {
        std::cerr << "More than 2 edges detected" << std::endl;
      }
    }
  }
  // Find the node itself
  ControlFlowGraphNode* node1;
  ControlFlowGraphNode* node2;
  for (auto tac_iter : passed_cfgn_vec) {
    if (tac_iter->GetCreationOrder() == block1) {
      node1 = tac_iter;
    } else if (tac_iter->GetCreationOrder() == block2) {
      node2 = tac_iter;
    }
  }
  if (edge1 == CONDITIONAL_TRUE_RETURN || edge1 == CONDITIONAL_FALSE_RETURN) {
    std::vector<std::string> live_set_copy = live_set;
    std::vector<std::string> str_return1, str_return2, merged_return;
    // Find the True + False Nodes
    std::pair<std::vector<std::string>, ControlFlowGraphNode*>  return1
    = RecursiveFindPath(passed_cfgn_vec, edges, live_set, node1, visited);
    std::pair<std::vector<std::string>, ControlFlowGraphNode*>  return2
    = RecursiveFindPath(passed_cfgn_vec
    , edges, live_set_copy, node2, visited);
    str_return1 = return1.first;
    str_return2 = return2.first;
    // Has to be merged here
    merged_return = MergeVector(str_return1, str_return2);
    // Using a merged string vector, find the guard
    std::pair<std::vector<std::string>, ControlFlowGraphNode*>  return3
    = RecursiveFindPath(passed_cfgn_vec, edges
    , merged_return, return1.second, visited);
    return return3;
  } else if (edge1 == CONDITIONAL_TRUE || edge1 == CONDITIONAL_FALSE) {
    // True + False branches returns their
    // values so the original node can handle it
    return std::make_pair(live_set, node1);
  } else if (edge1 == LOOP_TRUE) {
    if (!(std::find(visited.begin(), visited.end()
    , node1->GetCreationOrder()) != visited.end())) {
      RecursiveFindPath(passed_cfgn_vec, edges, live_set, node1, visited);
    }
    // If there is something that branches off of this
    if (block2 != -1) {
      // Check if the node hasn't been visited yet
      // Prevents infinite recursive loops
      if (!(std::find(visited.begin(), visited.end()
      , node2->GetCreationOrder()) != visited.end())) {
        RecursiveFindPath(passed_cfgn_vec, edges, live_set, node2, visited);
      }
    }
    return std::make_pair(live_set, node1);
  } else if (edge1 == LOOP_FALSE) {
    RecursiveFindPath(passed_cfgn_vec, edges, live_set, node1, visited);
    return std::make_pair(live_set, node1);
  } else if (edge1 == LOOP_RETURN) {
    RecursiveFindPath(passed_cfgn_vec, edges, live_set, node1, visited);
    return std::make_pair(live_set, node1);
  } else {
    // If there are no edges that go to the node
    // then that means this is the first node
    // Nothing really has to be done
    std::vector<std::string> empty;
    ControlFlowGraphNode* null;
    return std::make_pair(empty, null);
  }
}

// Helper function to pass everything needed
std::vector<std::unique_ptr<ControlFlowGraphNode>> OptimizeHelp(
    std::vector<std::unique_ptr<ControlFlowGraphNode>> cfg_node,
    std::vector<Edge> edges) {
  std::vector<ControlFlowGraphNode*> cfg_pointer;
  for (auto &iter : cfg_node) {
    cfg_pointer.push_back(iter.get());
  }
  std::vector<std::string> live_set;
  ControlFlowGraphNode* end = cfg_node.back().get();
  std::vector<int> visited_set;
  // if (edges.empty()){
  //   edges.push_back(Edge(std::make_pair(0,0),TYPELESS_EDGE));
  // }
  // We don't need the return value of RecursiveFindPath
  // Only thing that is important is modifying the local_block of each node
  RecursiveFindPath(cfg_pointer, edges, live_set, end, visited_set);
  cfg_pointer.clear();
  return cfg_node;
}

// Calls on a bunch of other functions
void ControlFlowGraph::Optimize() {
  cfg_nodes_ = std::move(OptimizeHelp(std::move(cfg_nodes_), edges_));
}

// Useful to see the IR within the nodes
void ControlFlowGraph::DebugPrint() {
  for (auto &iter : cfg_nodes_) {
    std::cout << "--- NEW BLOCK ---" << std::endl;
    std::cout << "Creation Order: "<< iter->GetCreationOrder() <<
      " Block Type: "<< iter->GetBlockType() << std::endl;
    iter->DebugNode();
  }
  std::cout << "EDGES: " << std::endl;
  for (auto iter1 : edges_) {
    std::cout << "From: " << iter1.edge_pair.first << " To: " <<
    iter1.edge_pair.second << " Edge Type: "
    << iter1.edge_type << std::endl;
  }
}

// Useful when you don't want to see the IR withing the nodes
void ControlFlowGraph::DebugEdgeAndBlock() {
  for (auto &iter : cfg_nodes_) {
    std::cout << "--- NEW BLOCK ---" << std::endl;
    std::cout << "Creation Order: "<< iter->GetCreationOrder() <<
      " Block Type: "<< iter->GetBlockType() << std::endl;
  }
  std::cout << "EDGES: " << std::endl;
  for (auto iter1 : edges_) {
    std::cout << "From: " << iter1.edge_pair.first << " To: "
      << iter1.edge_pair.second << " Edge Type: "
      << iter1.edge_type << std::endl;
  }
}

// Iterates through the Node vector
// Slowly accumulates the IR and then returns it
std::vector<std::unique_ptr<struct ThreeAddressCode>>
ControlFlowGraph::MakeThreeAddressCode() {
  int creation_check = 0;
  std::vector<std::unique_ptr<struct ThreeAddressCode>> return_three_address;
  for (auto &iter : cfg_nodes_) {
    if (iter->GetCreationOrder() == creation_check) {
      for (auto &three_iter : iter->GetLocalUniqueBlock()) {
        return_three_address.push_back(std::move(three_iter));
      }
    }
  }
  return std::move(return_three_address);
}

// Iterate through the vector and print out each basic block
// Shamelessly borrowed from lowerer_visitor.cc
// Slightly repurposed for the sake of CFGS
std::string OutputHelper(
    std::vector<ThreeAddressCode*> return_three_address ) {
  std::string output = "";
  std::vector<std::string> printhelper = {"INTLOAD", "VARLOAD"
    , "VAR_ASSIGN_LOAD", "FUNARGLOAD", "FUNRETLOAD", "+", "-"
    , "*", "/", "<", "<=", ">", ">=", "==", "&&", "||", "¬",
    "while", "if", "jmp", "je", "jne", "jg", "jge",
    "jl", "jle", "MkLabel", "FUNCTIONCALL", "FUNRETURNEPILOGUE",
    "FUNCTIONDEF", "FUNPROLOGUE", "FUNEPILOGUE", "PRINTARITH",
    "NOTYPE", "LHSDEREFERENCE", "RHSDEREFERENCE", "NEWTUPLE",
    "VARCHILDTUPLE"};

  for (unsigned int i = 0; i < return_three_address.size(); ++i) {
    // If it's a just a int (Register without a name then access it's value)
    // Otherwise access its name
    if (return_three_address[i] == NULL) {
      continue;
    }
    OpcodeType opcodetype = return_three_address[i]->op.opcode();
    switch (opcodetype) {
     case INT_LOAD:
        output = output + return_three_address[i]->target.reg().name()
        + " <- " + std::to_string(return_three_address[i]->arg1.value());
        break;
      case VAR_ASSIGN_LOAD:
        output = output + return_three_address[i]->target.reg().name()
        + " <- " + return_three_address[i]->arg1.reg().name();
        break;
      case FUN_ARG_LOAD:
        output = output + return_three_address[i]->target.reg().name()
        + " <- " +std::to_string(return_three_address[i]->arg1.value());
        break;
      case LOG_NOT:
        output = output + return_three_address[i]->target.reg().name()
        + " <- " + printhelper[LOG_NOT] +
        return_three_address[i]->arg1.reg().name();
        break;
      case LOOP:
        output = output + printhelper[LOOP] + " " +
                 return_three_address[i]->arg2.reg().name() + " == 0";
        break;
      case CONDITIONAL:
        output = output + printhelper[CONDITIONAL] + " " +
                 return_three_address[i]->arg2.reg().name() + " == 0";
        break;
      // Abstract jumps out
      case JUMP:
        output = output + printhelper[JUMP] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JMP_EQUAL:
        output = output + printhelper[JMP_EQUAL] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JMP_NOT_EQUAL:
        output = output + printhelper[JMP_NOT_EQUAL] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JMP_GREATER_THAN:
        output = output + printhelper[JMP_GREATER_THAN] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JMP_GREATER_THAN_EQ:
        output = output + printhelper[JMP_GREATER_THAN_EQ] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JMP_LESS_THAN:
        output = output + printhelper[JMP_LESS_THAN] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JMP_LESS_THAN_EQ:
        output = output + printhelper[JMP_LESS_THAN_EQ] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case LABEL:
        output = output + printhelper[LABEL] + " " +
                 return_three_address[i]->target.label().name();
        break;
      default:
        output = output + return_three_address[i]->target.reg().name()
        + " <- " + return_three_address[i]->arg1.reg().name();

        output = output + " " + printhelper[
        return_three_address[i]->op.opcode()] + " " +
        return_three_address[i]->arg2.reg().name();
        break;
    }
    output = output + "\n";
  }

  return output;
}

// Once again iterates through all the nodes to create IR
// Uses direct pointers because there is no need to transfer ownership
std::string ControlFlowGraph::GetOutput() {
  int creation_check = 0;
  std::vector<ThreeAddressCode*> return_three_address;
  for (auto &iter : cfg_nodes_) {
      for (auto &three_iter : iter->GetLocalBlock()) {
        return_three_address.push_back(three_iter);
      }
    }

  return OutputHelper(return_three_address);
}

// Default Constructor
ControlFlowGraphNode::ControlFlowGraphNode() {
  creation_order = 0;
  blocktype_ = NO_BLOCK_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode(
    std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  localblock_ = std::move(input);
  creation_order = 0;
  blocktype_ = NO_BLOCK_TYPE;
}

// Originally the idea was to use objects
// Pointers turned out to be easier to manage
// Not Used
ControlFlowGraphNode::ControlFlowGraphNode(ControlFlowGraphNode &copy) {
  localblock_.clear();
  // Because it is a vector of unique pointers
  // Each element has to be manually copied
  for (const auto& iter : copy.GetLocalBlock()) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    block->arg1 = iter->arg1;
    block->arg2 = iter->arg2;
    localblock_.push_back(std::move(block));
  }
  // localblock_ = std::move(copy.GetLocalBlock());
  creation_order = copy.GetCreationOrder();
  blocktype_ = copy.GetBlockType();
}

// Transfers ownership of the IR
void ControlFlowGraphNode::SetLocalBlock(
    std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  localblock_ = std::move(input);
}

// Debug function for the nodes
void ControlFlowGraphNode::DebugNode() {
  for (auto &iter : localblock_) {
    std::cout << "Target name: " << iter->target.reg().name()
      << " Arg1 name: " << iter->arg1.reg().name() <<
      " Arg2 name: " << iter->arg2.reg().name() << std::endl;
    std::cout << "Target type: " << iter->target.type()
      << " Arg1 value: " << iter->arg1.value() << " Arg2 value: "
      << iter->arg2.value() << std::endl;
    std::cout << "Op code type: " << iter->op.opcode() << std::endl;
  }
}

}  // namespace backend
}  // namespace cs160
