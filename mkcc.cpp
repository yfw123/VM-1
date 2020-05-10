#include "VM.h"
#include <fstream>
#include <sstream>
#include "assembler_functions.h"

using namespace std;

int main(int argc, char const *argv[]){
  if(argc < 2){
    cerr << "please enter a binary file name\n";
    return 1;
  }
  ifstream f(argv[1], ios::binary);
  if(!f){
    cerr << "can't open file" << '\n';
    return 1;
  }
  cout << "#include \"VM.h\"\n";
  vector<Value> vals;
  Record r;
  bool wait = false;
  cout << "int main(int argc, char** argv){\n";
  cout << "VM vm;\nstd::vector<Value>* mem = new std::vector<Value>();\nvm.attachMem(mem);\n";
  while(f.read((char*)&r, sizeof(Record))){
    wait = false;
    if(r.type == TYPE_TEXT){
      bool add = true;
      ostringstream stream;
      stream << (char)r.value;
      while(f.read((char*)&r, sizeof(Record))){
        if(r.type != TYPE_NUM)
          stream << (char)r.value;
        else {
          vals.push_back(stream.str().c_str());
          add = false;
          vals.push_back(r.value);
          if(r.value == PUT){
            wait = true;
          }
          break;
        }
      }
      if(add)vals.push_back(stream.str().c_str());
    } else {
      vals.push_back(r.value);
      if(r.value == PUT){
        wait = true;
      }
    }
    if(!wait){
      for(int c = 0; c < vals.size(); c++){
        Value v = vals[c];
        if (v.getType() == TYPE_NUM) {
          cout << "vm.run1(" << v.toString();
          if (v.getLong() == PUT) {
            cout << ", " << (vals[++c].getType()? "\"":"") << vals[c].toString() << (vals[c].getType()? "\"":"");
          }
          cout << ");\n";
        }
      }
      vals.clear();
    }
  }
  cout << "return 0;\n";
  cout << "}\n";
  f.close();
	return 0;
}
