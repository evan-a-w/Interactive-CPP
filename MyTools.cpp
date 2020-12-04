#include "MyTools.h"
#include <fstream>
#include <iterator>
#include <string>

using namespace std;

vector<string> StringTools::Split(string str, char delimiter){
    vector<string> res;
    string curr = "";
    unsigned int i = 0;
    while(i < str.length()){
        if(str[i] != delimiter){
            curr += str[i];
            if(i == str.length() - 1){
                res.push_back(curr);
            }
        }
        else{
            if(curr != "") res.push_back(curr);
            curr = "";
        }
        i++;
    }
    return res;
}

string StringTools::FileToString(string fileName){
    ifstream ifs(fileName);
    string s(istreambuf_iterator<char>{ifs}, {});
    return s;
}