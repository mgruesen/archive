#include <iostream>
#include <fstream>
#include <string>

#include "../util/strops.hpp"

using namespace std;

string buff = "";
int lbl_count = 0;
int line_no = 0;

void doEnd(){
  cout << "return 0;\n}\n";
}

void doGoto(ifstream& fs){
  getline(fs, buff);
  cout << "goto" << buff << ";\n";
}

void doInput(ifstream& fs){
  getline(fs, buff);
  cout << "scanf(\"%d\", &" << buff.substr(1, buff.length()) << ");\n";
}

void doInt(ifstream& fs){
  getline(fs, buff);
  cout << "int" << buff << ";\n";
}

void doLabel(ifstream& fs){
  getline(fs, buff);
  cout << buff.substr(1, buff.length()) << ":;\n";
}

void doLet(ifstream& fs){
  getline(fs, buff);
  cout << buff.substr(1, buff.length()) << ";\n";
}

void doList(ifstream& fs, string size){
  size_t beg, end;
  beg = size.find("[");
  end = size.find("]");
  size = size.substr(beg, end+1);
  getline(fs, buff);  
  cout << "int" << buff << size << ";\n";
}

void doPrint(ifstream& fs){
  getline(fs, buff);
  cout << "printf(\"%d\"," << buff << ");\n";
}

void doPrompt(ifstream& fs){
  getline(fs, buff);
  cout << "printf(" << buff.substr(1, buff.length()) << ");\n";
}

void doRem(ifstream& fs){
  getline(fs, buff);
  cout << "//" << buff << endl;
}

void doTitle(ifstream& fs){
  getline(fs, buff);
  cout << "//" << buff << endl;
  cout << "#include <stdio.h>\nint main(void){\n";
}

void doIf(ifstream& fs){
  getline(fs, buff);
  size_t beg = buff.find("then");
  if (beg != string::npos)
    buff.erase(beg, 5);
  cout << "if" << buff << ";\n";
}

int
main(int argc, char** argv){
  if (argc != 2){
    cerr << "error: invalid number of arguments\n";
    return -1;
  }  
  ifstream fs(argv[1], ios::in);
  if (!fs){
    cerr << "error: couldn't open file '" << argv[1] << "'\n";
    return -1;
  }
  while(fs >> buff){
    if(strops::matches(buff, "begin") || strops::matches(buff, "var"))
      continue;
    else if (strops::matches(buff, "end")){
      doEnd();
    }
    else if (strops::matches(buff, "goto")){
      doGoto(fs);
    }
    else if (strops::matches(buff, "input")){
      doInput(fs);
    }
    else if (strops::matches(buff, "int")){
      doInt(fs);
    }
    else if (strops::matches(buff, "label")){
      doLabel(fs);
    }
    else if (strops::matches(buff, "let")){
      doLet(fs);
    }
    else if (strops::contains(buff, "list[")){
      doList(fs, buff);
    }
    else if (strops::matches(buff, "print")){
      doPrint(fs);
    }
    else if (strops::matches(buff, "prompt")){
      doPrompt(fs);
    }
    else if (strops::matches(buff, "rem")){
      doRem(fs);
    }
    else if (strops::matches(buff, "title")){
      doTitle(fs);
    }
    else if (strops::matches(buff, "if")){
      doIf(fs);
    }
    else{
      string line;
      getline(fs, line);
      cout << buff << line << ";\n";
    }
  }
  fs.close();
  return 0;
}