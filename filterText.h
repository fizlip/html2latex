#include <stdio.h>
#include <string>
#include <regex>

using namespace std;
using namespace std::regex_constants;

class FilterText{
  private:
    string str;
  public:
    void replace(string segment, string with);
    void replaceFrom(string startSeg, string endSeg, string startWith, string endWith);
    void filterThis(string str);
    string getResult();
}; 

void FilterText::filterThis(string str){
  regex r ("\\\\[^\\[\\]]", ECMAScript);
  regex math ("\\^", ECMAScript);
  regex d ("\\$", ECMAScript);
  regex u ("\\_", ECMAScript);
  regex h ("#", ECMAScript);
  string res = "";
  for(char c : regex_replace(regex_replace(regex_replace(regex_replace(regex_replace(str, r, "\\textbackslash "), math, "\\^"), d, "\\$"), u, "\\_"), h, "\\#"))
    res += c;

  this->str = res;
}

void FilterText::replace(string segment, string with){
  regex r (segment, ECMAScript);
  string res = ""; 
  for(char c : regex_replace(str, r, with))
    res += c;
  str = res;
}

void FilterText::replaceFrom(string startSeg, string endSeg, string startWith, string endWith){

  regex rs (startSeg, ECMAScript);
  regex re (endSeg, ECMAScript);
  string res = ""; 
  for(char c : regex_replace(regex_replace(str, rs, startWith), re, endWith))
    res += c;
  str = res;

}

string FilterText::getResult(){
  return this->str;
}
