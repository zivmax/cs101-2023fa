#ifndef HUFFMAN_CALCULATOR_HPP
#define HUFFMAN_CALCULATOR_HPP

#include <tuple>
#include <vector>
#include<memory>

struct Node;
using NodePtr = std::shared_ptr<Node>;

// size_t heap_size;
struct Node{
    std::pair<size_t, size_t> main_data;
    // decide whether the node is secondary product
    bool is_secondary = false;
    int temp_sum = 0; 
    Node(std::pair<size_t, size_t>data, int _temp_sum):main_data(data),temp_sum(_temp_sum){}
};


size_t get_previous(size_t index){
  return (index -1)/2;
}

void insert_heap(NodePtr current, std::vector<NodePtr>& v){
  v.push_back(current);
  size_t index = v.size()-1;
  while(index > 0 && v[index]->main_data.first < v[get_previous(index)]->main_data.first){
    std::swap(v[index], v[get_previous(index)]);
    index = get_previous(index);
  }
}

int get_node_temp_sum(NodePtr node){
  if(node->is_secondary){
    return node->temp_sum;
  }else{
    return node->main_data.first;
  }
}

void prelocate_down(size_t current, size_t n, std::vector<NodePtr>& v){

      size_t min_index = current;
      size_t right_index = 2 + 2 * current;
      if(right_index < n){
        if(v[right_index]->main_data.first < v[min_index]->main_data.first ){
            min_index = right_index;
        }
      }

      size_t left_index = 1 + 2 * current;
      if(left_index < n){
        if(v[left_index]->main_data.first < v[min_index]->main_data.first && left_index < n ){
          min_index = left_index;
        }
      }

      // one of the child is smaller than current
      if(min_index != current){
        std::swap(v[current], v[min_index]);
        prelocate_down(min_index, n, v);
      }
    
}

NodePtr pop_front(std::vector<NodePtr>& v){
    auto res = v[0];
    v[0] = v.back();
    v.pop_back();
    size_t n = v.size();
    if(n){
      // pass n to save time
      prelocate_down(0, n, v);
    }
    return res;
}
void deal_with_mutiple_node(std::vector<NodePtr>&v, size_t& sum, NodePtr current_node){

    size_t combined_first = current_node->main_data.first * 2;
    size_t combined_second = current_node->main_data.second / 2;

    auto combined_pair = std::make_pair(combined_first, combined_second);
    auto combined_node = std::make_shared<Node>(combined_pair, 0);
    insert_heap(combined_node, v);

    sum += combined_first * combined_second;

    // the node can combine perfectly
    if(current_node->main_data.second % 2 == 1){
      auto spared_pair = std::make_pair(current_node->main_data.first, 1);
      auto spared_node = std::make_shared<Node>(spared_pair, 0);
      insert_heap(spared_node, v);
    }
}
void get_sum(std::vector<NodePtr>&v, size_t& sum){
  while(v[0]){
    NodePtr front = pop_front(v);

    if(front->main_data.second != 1){
      deal_with_mutiple_node(v, sum, front);
    }
    else{
      
      if(!v.size()){
        return;
      }
      // combine with other node
      NodePtr second_node = pop_front(v);

      // secondary nodes sum needs to combine with more sub sum
      auto secondary_temp_sum = get_node_temp_sum(front) + get_node_temp_sum(second_node);
      sum += secondary_temp_sum;
      auto new_pair = std::make_pair(front->main_data.first + second_node->main_data.first, 1);
      auto combined_node = std::make_shared<Node>(new_pair, secondary_temp_sum);
      combined_node->is_secondary = true;

      insert_heap(combined_node, v);
      // still have to insert what's left of second_node
      size_t& count_num = second_node->main_data.second;
      if(count_num != 1){
        insert_heap(second_node, v);
        --count_num;
      }
    }
  }
}

size_t get_huffman_length(const std::vector<std::pair<size_t, size_t>> &data) {
  // TODO: Implement this.
  std::vector<NodePtr> v;
  // construct a heap 
  for(size_t i=0;i<data.size();i++){
    NodePtr current_node = std::make_shared<Node>(data[i], 0);
    insert_heap(current_node, v);
  }

  size_t sum = 0;
  get_sum(v, sum);

  return sum;
}
#endif // HUFFMAN_CALCULATOR_HPP