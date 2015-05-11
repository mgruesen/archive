#ifndef TREE_HPP
#define TREE_HPP

#include <string>

namespace tree
{
  class node
  {
  public:
    node():val(""),p(NULL),l(NULL),r(NULL) {}
    node(std::string str):val(str),p(NULL),l(NULL),r(NULL) {}
    ~node() {
      delete p;
      delete l;
      delete r;
    }
    
    std::string val;
    node* p;
    node* l;
    node* r;
  };

  void
  printTree(node* h);

  node*
  makeTree(const std::string&);

  int
  printThreeOpForm(node*, int);

}

#endif