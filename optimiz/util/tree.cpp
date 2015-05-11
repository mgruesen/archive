#include "tree.hpp"
#include "strops.hpp"

#include <iostream>

void
tree::printTree(node* h)
{
  while (h != NULL) {
    if (h->r != NULL)
      std::cout << h->r->val << " ";
    std::cout << h->val << " ";
    h = h->l;
  }
  std::cout << "\n";
}

tree::node*
tree::makeTree(const std::string& b)
{
  std::stringstream ss(b);
  std::string buff;
  ss >> buff;
  if (strops::is_array(buff)) {
    std::string temp("");
    while (buff.find("]") == std::string::npos) {
      ss >> temp;
      buff += " " + temp;
    }
  }
  node* n = new node(buff);
  node* curr;
  node* head;
  ss >> buff;
  head = new node(buff);
  head->r = n;
  n->p = head;
  curr = head;
  while (ss >> buff) {
    if (strops::is_variable(buff)) {
      n = new node(buff);
      if (curr->r == NULL)
        curr->r = n;
      else
        curr->l = n;
      n->p = curr;
    }
    else if (strops::is_array(buff)) {
      std::string temp;
      while (buff.find("]") == std::string::npos) {
        ss >> temp;
        buff += " " + temp;
      }
      n = new node(buff);
      if (curr->r == NULL)
        curr->r = n;
      else
        curr->l = n;
      n->p = curr;
    }
    else if (strops::is_operator(buff)) {
      n = new node(buff);
      if (curr->l == NULL) {
        curr->l = n;
        n->p = curr;
      }
      else {
        n->r = curr->l;
        n->p = curr;
        curr->l = n;
      }
      curr = curr->l;
    }
  }
  return head;
}