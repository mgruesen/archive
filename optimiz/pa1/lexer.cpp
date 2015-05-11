#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>

#include "../util/strops.hpp"

using namespace std;

char delim[] = {'<', '>', '!', '='};
int delim_count = 4;

string buff = "";
int lbl_count = 0;
stack<string> stk;

void doWhile(ifstream& fs){
  cout << "goto L_" << lbl_count++ << endl;
  cout << "label L_" << lbl_count++ << endl;
  string lbl, end;            
  buff.clear();
  getline(fs, buff);
  string condit = buff;
  lbl = "label L_" + to_string(lbl_count - 2);
  end = "if" + condit + " then goto L_" + to_string(lbl_count - 1);
  stk.push(end);
  stk.push(lbl);
}

void doEndLoop(){
  cout << stk.top() << endl;
  stk.pop();
  cout << stk.top() << endl;
  stk.pop();
}

void doFor(ifstream& fs){
  string var;
  fs >> var;      
  string lb, ub, delta = "1";
  fs >> buff >> lb >> buff >> ub;
  cout << "let " << var << " = " << lb << endl;
  cout << "label L_" << lbl_count << endl;      
  buff.clear();
  getline(fs,buff);
  if(strops::contains(buff, "by"))
    delta = buff.substr(buff.find("by") + 3, buff.length());        
  string incr, condit;
  incr = "if (" + var + " <= " + ub + ") then goto L_" + to_string(lbl_count++);
  stk.push(incr);
  condit = "let " + var + " = " + var + " + " + delta;
  stk.push(condit);
}

void doIf(ifstream& fs){
  string lhs, rhs, rel, lbl = to_string(lbl_count++);  
  getline(fs, buff);
  stringstream ss(buff);
  size_t pos;
  for(int i = 0; i < delim_count; i++){
    pos = buff.find(delim[i]);
    if(pos != string::npos) break;
  }
  if (pos == string::npos)
    cerr << "unable to find relational in string '" << buff << "'\n";
  getline(ss, lhs, buff[pos]);
  ss.unget();
  getline(ss, rel, ' ');
  getline(ss, rhs, ')');
  lhs = lhs.substr(2, lhs.length() - 1);
  rel = strops::negate_relational(rel);
  cout << "if (" << lhs << " " << rel << " " << rhs << ") then goto L_" << lbl << endl;
  ss >> buff; // then
  buff.clear();
  getline(ss,buff);
  if (buff != "") cout << buff << endl;
  int len = fs.tellg();
  buff.clear();
  fs >> buff;  
  if (strops::matches(buff, "if"))
    doIf(fs);
  else 
    fs.seekg(len, ios_base::beg);
  cout << "label L_" << lbl << endl;
}

int
main(int argc, char** argv){  
  if(argc != 2){
    cerr << "error: invalid number of arguments\n";
    return -1;
  }

  ifstream fs(argv[1], ios::in);
  
  if(!fs){
    cerr << "error: couldn't open file '" << argv[1] << "'\n";
    return -1;
  }
  while(fs >> buff)
  {
    if(strops::matches(buff, "while"))
    {
      doWhile(fs);
    }
    else if(strops::matches(buff, "endwhile")){
      doEndLoop();
    }
    else if(strops::matches(buff, "for")){
      doFor(fs);
    }
    else if (strops::matches(buff, "endfor")){
      doEndLoop();
    }
    else if (strops::matches(buff, "if")){      
      doIf(fs);
    }
    else{
      string line;
      getline(fs, line);
      cout << buff << line << endl;
    }    
  }
  fs.close();
  return 0;
}