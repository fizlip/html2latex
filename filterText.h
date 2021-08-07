#include <stdio.h>
#include <string>
#include <regex>

using namespace std;
using namespace std::regex_constants;

/*
 * This class contains operations that make it easier to search and replace
 * large strings useing regular expressions
*/
class FilterText{
  private:
    string str;
  public:
    void replace(string segment, string with);
    void replaceFrom(string startSeg, string endSeg, string startWith, string endWith);
    void filterThis(string str);
    string getResult();
}; 

/*
 * Initialises the text to be filtered by replacing different special characters
 * that are resereved into theri escaped format.
*/
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

/*
 * Will replace the 'segment' with the 'with' string
 * Example:
 * assume that str = 'abc'
 * replace('[bc]', 'a') returns str = 'aaa'
*/
void FilterText::replace(string segment, string with){
  regex r (segment, ECMAScript);
  string res = ""; 
  for(char c : regex_replace(str, r, with))
    res += c;
  str = res;
}

/*
 * Replaces a pair of segments in a string.
 * Exmaple:
 * assume str = <p>abc</p>
 * replaceFrom('<p>', '</p>', '', '') returns str = 'abc'
*/

void FilterText::replaceFrom(string startSeg, string endSeg, string startWith, string endWith){

  regex rs (startSeg, ECMAScript);
  regex re (endSeg, ECMAScript);
  string res = ""; 
  for(char c : regex_replace(regex_replace(str, rs, startWith), re, endWith))
    res += c;
  str = res;

}

/*
 * returns the replaced string
*/

string FilterText::getResult(){
  return this->str;
}
