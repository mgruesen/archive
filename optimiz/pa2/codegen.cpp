#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <sstream>

#include "../util/strops.hpp"
#include "../util/tree.hpp"

using namespace std;

int line = 0;
int c = 0;
int block = 0;
string buff;

void eval_array_read(tree::node*);
void eval_expression(tree::node*);

bool
is_numeric(const string& str)
{
  for (unsigned i = 0; i < str.length(); i++) {
    if (!isdigit(str[i]))
      return false;
  }
  return true;
}



void
eval_array_read(tree::node* n) 
{
  if (strops::is_complex_array(n->val)) {
    string index = strops::get_index_expression(n->val);
    tree::node* head = tree::makeTree(index);
    tree::printTree(head);
    cout << "let t_" << c << " = " << head->r->val << " " << head->val << 
      " " << head->l->val << endl;
    head->val = string("t_") +  to_string(c++);
    n->val = n->val.substr(0, n->val.find("[") + 1) + head->val + "]";
  } 
  cout << "let t_" << c << " = " << n->val << endl;
  n->val = string("t_") + to_string(c++);
}

void
eval_array_write(tree::node* n)
{
  if (strops::is_complex_array(n->val)) {
    string index = strops::get_index_expression(n->val);
    tree::node* head = tree::makeTree(index);
    eval_expression(head);
    n->val = n->val.substr(0, n->val.find("[") + 1) + head->val + "]";
  }
  else {
    size_t beg, end;
    string val = n->val;
    beg = val.find("[");
    beg++;
    end = val.find("]");
    string idx = strops::get_index_expression(val);
    if (!is_numeric(idx)) {
      string var = val.substr(0, beg);
      string reg = string("t_") + to_string(c++);
      cout << "let " << reg << " = " << idx << endl;
      n->val = var + reg + string("]");
    }
  } 
}

void
eval_expression(tree::node* n) 
{ 
  // Make recursive call to left leaf if it is an operator
  if (strops::is_operator(n->l->val))
    eval_expression(n->l);        
  // if (strops::is_assignment(n->val)) {
  //   if (strops::is_array(n->r->val)) {
  //     string var = n->r->val.substr(0, n->r->val.find("["));
  //     eval_array_write(n->r);
  //     tree::node* head = tree::makeTree(strops::get_index_expression(n->r->val));
  //     eval_expression(head);
  //     n->r->val = var + head->val + string("]");
  //   }
  //   cout << "let " << n->r->val << " = " << n->l->val << '\n';
  // }
  // else {
    // Check if left or right leaves are arrays
    if (strops::is_array(n->l->val))
      eval_array_read(n->l);
    if (strops::is_array(n->r->val))
      eval_array_read(n->r);
    if (NULL != n->p) {
      cout << "let t_" << c << " = " << n->r->val << " " << n->val <<
        " " << n->l->val << endl;
      n->val = string("t_") + to_string(c++);      
    }
    else {
      n->val = string(n->r->val) + string(" ") + string(n->val) + 
        string(" ") + string(n->l->val);
    } 
  // } 
}

void
eval_let(const std::string& str)
{
  string lhs, rhs;
  size_t pos = str.find(" = ");
  lhs = str.substr(0, pos);
  rhs = str.substr(pos + 3, str.length());

  //cout << "### buff: " << buff << " lhs: " << lhs << " rhs: " << rhs << "\n";

  if (strops::is_array(lhs)) {
    tree::node* head = tree::makeTree(lhs);
    eval_array_write(head);
    lhs = head->val;
  }
  if (strops::has_operator(rhs)) {
    tree::node* head = tree::makeTree(rhs);
    eval_expression(head);
    rhs = head->val;
  }
  cout << "let " << lhs << " = " << rhs << endl;
}

void
eval_if(const std::string& str)
{
  string expr = strops::get_conditional_expression(str);
  string rel = strops::get_relational(str);
  size_t pos = expr.find(rel);
  string lhs, rhs;
  lhs = expr.substr(0, pos - 2);
  rhs = expr.substr(pos + (rel.length() + 1), expr.length() - 1);
  
  
  if (strops::has_operator(lhs)) {
    tree::node* head = tree::makeTree(lhs);
    eval_expression(head);
    lhs = head->val;
  }
  else if (strops::is_array(lhs)) {
    tree::node* head = tree::makeTree(lhs);
    eval_array_read(head);
    lhs = head->val;
  }
  if (strops::has_operator(rhs)) {
    tree::node* head = tree::makeTree(rhs);
    eval_expression(head);
    rhs = head->val;
  }
  else if (strops::is_array(rhs)) {
    tree::node* head = tree::makeTree(rhs);
    eval_array_read(head);
    rhs = head->val;    
  }
  cout << "if (" << lhs << " " << rel << " " << rhs << ")";  
}

void
eval_print(const std::string& str)
{
  string var = "";
  if (strops::is_array(str)) {
    tree::node* head = tree::makeTree(str);
    eval_array_read(head);
    var = head->val;
  }
  else {
    var = str;
  }
  cout << "print " << var << endl;
}


void 
doLet(ifstream& fs) 
{    
  getline(fs, buff);
  eval_let(buff);
}


void 
doIf(ifstream& fs) 
{
  getline(fs, buff);
  size_t pos = buff.find(")");
  eval_if(buff.substr(0, pos+1));
  if (pos+1 < buff.length())
    cout << buff.substr(pos+1, buff.length()) << '\n';
  else
    cout << '\n';
}

void
doInput(ifstream& fs)
{

}

void 
doPrint(ifstream& fs)
{
  getline(fs, buff);
  eval_print(buff);
}

int main(int argc, char** argv)
{
  if (argc != 2){
    cerr << "error: invalid number of arguments\n";
    return -1;
  }  
  ifstream fs(argv[1], ios::in);
  if (!fs){
    cerr << "error: couldn't open file '" << argv[1] << "'\n";
    return -1;
  }
  long beg = fs.tellg();
  while (fs >> buff) {
    if (strops::matches(buff, "if")) {
      doIf(fs);
    }
    else if (strops::matches(buff, "let")) {  
      doLet(fs);
    }    
    else if (strops::matches(buff, "print")) {
      doPrint(fs);
    }
    else if (strops::matches(buff, "input")) {
      doInput(fs);
    }
    else {
      fs.seekg(beg, ios::beg);
      getline(fs, buff);
      cout << buff << endl;
    }
    beg = fs.tellg();
  }
  return 0;
}