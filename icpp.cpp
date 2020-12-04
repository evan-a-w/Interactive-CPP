#define mFileToString StringTools::FileToString
#define mSplit StringTools::Split
#include "MyTools.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void writeToFile(string);
void process(string &str, string &prevCurr, string& curr, unsigned int &prevPos, unsigned int &currPos, int &level);

int main(){
    cout << "Code is automatically in the main function except if \"def\" is the first word (followed by function definition). Undo will undo the last command but no more than that. Clear will reset everything." << endl;
    cout << "If a variable name is written without a trailing semi-colon (;), cout will be applied to the variable but the underlying program will not be changed." << endl;
    cout << "Type \"cp\" or \"copy\" at any time to copy the generated file into the working directory." << endl;
    unsigned int currPos;
    int currLevel = 1;
    bool putFile = true;
    bool funcMode = false;
    int prevLevel;
    int nonFuncPos;
    unsigned int prevPos;
    unsigned int funcPos;
    bool save = false;
    string prevCurr;
    string curr = "#include <iostream>\n"
                  "#include <vector>\n"
                  "#include <string>\n"
                  "using namespace std;\n"
                  "int main(){\n"
                  "\t\n"
                  "\treturn 0;\n"
                  "}\n";
    writeToFile(curr);
    currPos = curr.find("main(){") + 8;
    while(true){
        bool noSemi = false;
        bool proc = true;
        string str;
        getline(cin, str);
        if(currLevel == 0 && !funcMode) currLevel = 1;
        if(mSplit(str, ' ')[0] == "#include"){
            curr = str + "\n" + curr;
            prevPos = currPos;
            currPos += str.length() + 1;
            goto skip;
        }
        if(mSplit(str, ' ')[0] == "def"){
            str.replace(0,4,"");
            funcMode = true;
            funcPos = nonFuncPos = curr.find("int main(){");
            currLevel = 0;
            putFile = false;
            proc = false;
            process(str, prevCurr, curr, prevPos, funcPos, currLevel);
            currLevel++;
        }
        else if(str[str.length() - 1] == '{'){
            proc = false;
            process(str, prevCurr, curr, prevPos, funcMode ? funcPos : currPos, currLevel);
            putFile = false;
            currLevel++;
        }
        else if(str[0] == '}'){
            if(funcMode){
                if(currLevel == 0 || currLevel == 1){
                    putFile = true;
                    funcMode = false;
                    proc = false;
                    currLevel--;
                    process(str, prevCurr, curr, prevPos, funcPos, currLevel);
                    currPos += (funcPos - nonFuncPos);
                }
                else{
                    currLevel--;
                }
                
            }
            else{
                if(currLevel == 2){
                    currLevel--;
                    putFile = true;
                }
                else{
                    currLevel--;
                }
            }
        }
        else if(str == "undo"){
            putFile = true;
            writeToFile(prevCurr);
            curr = prevCurr;
            currPos = prevPos;
            proc = false;
        }
        else if(str == "clear" || str == "reset"){
            proc = false;
            int currLevel = 1;
            bool putFile = true;
            bool funcMode = false;
            curr = "#include <iostream>\n"
                    "#include <vector>\n"
                    "#include <string>\n"
                    "using namespace std;\n"
                    "int main(){\n"
                    "\t\n"
                    "\treturn 0;\n"
                    "}\n";
            currPos = curr.find("main(){") + 8;
        }
        else if(str == "exit") goto end;
        else if(str == "cp" || str == "copy"){
            save = true;
            proc = false;
            noSemi = true;
        }
        else if(str[str.length() - 1] != ';' && !funcMode){
            proc = false;
            noSemi = true;
            str = "cout << " + str + " << endl;";
            process(str, prevCurr, curr, prevPos, currPos, currLevel);
            writeToFile(curr);
            system("g++ -O0 program.cpp -o program");
            system("program.exe");
            system("del program.exe");
            writeToFile(prevCurr);
            curr = prevCurr;
            currPos = prevPos;
        }
        if(proc) process(str, prevCurr, curr, prevPos, funcMode ? funcPos : currPos, currLevel);
        skip:
        if(putFile && !noSemi){
            writeToFile(curr);
            system("g++ -O0 program.cpp -o program");
            system("program.exe");
            system("del program.exe");
        }
    }
    end:
    if(!save) system("del program.cpp");
    return 0;
}

void writeToFile(string write){
    ofstream myFile("program.cpp");
    myFile.write(write.c_str(), write.length());
    myFile.close();
}

void process(string &str, string &prevCurr, string& curr, unsigned int &prevPos, unsigned int &Pos, int &currLevel){
    for(int i = 0; i < currLevel; i++){
        str = "\t" + str;
    }
    prevCurr = curr;
    prevPos = Pos;
    curr.insert(Pos, str + "\n");
    Pos += str.length() + 1;
}