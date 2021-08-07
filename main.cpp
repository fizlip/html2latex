#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <regex>
#include "./filterText.h"

using namespace std;

void emptyFile(char fname[100]){
  fstream file;
  file.open(fname, std::fstream::out | std::fstream::trunc);
  file.close();
}

void writeToFile(char fname[100], string line){
  fstream uidlFile(fname, std::fstream::in | std::fstream::out | std::fstream::app);

  if(uidlFile.is_open()){
    uidlFile << "\n" + line;
    uidlFile.close();
  }
  else{
    printf("Cannot open file");
  }
}

void initLatexDoc(char fname[100]){

  string init = "\\documentclass{article}"
  "\\usepackage{titlesec}\n"
  "\\usepackage{titling}\n"
  "\\usepackage[margin=1.5in]{geometry}\n"
  "\\usepackage[dvipsnames]{xcolor}\n"
  "\\usepackage[T1]{fontenc}\n"
  "\\thispagestyle{empty}\n"
  "\\titleformat{\\section}\n"
  "{\\Large\\bfseries\\color{black}}\n"
  "{}\n"
  "{0em}\n"
  "{}[\\color{black}\\titlerule]\n"
  "\\titleformat{\\subsection}\n"
  "{\\normalsize\\bfseries}\n"
  "{}\n"
  "{0em}\n"
  "{}\n"
  "\\titleformat{\\subsubsection}[runin]\n"
  "{\\small\\bfseries}\n"
  "{}\n"
  "{0em}\n"
  "{}[---]\n"
  "\\titlespacing{\\subsubsection}\n"
  "\{0em}{0.25em}{0em}\n"
  "\\title{Resume}\n"
  "\\author{Filip Zlatoidsky}\n"
  "\\begin{document}\n";

  writeToFile(fname, init);
}

string filterBuffer(string buffer){
  using namespace std::regex_constants;
  FilterText f;
  f.filterThis(buffer);

  //regex regular_expr ("(<h[1-6]>[^]*?</h[1-6]>|<p>[^]*?</p>)", ECMAScript);
  // remove unecessary tags
  string filter = "(</*(p|html|body|div).*?>|<!.*>|<head>[^]*?</head>|</li>|<script.*>[^]*?</script>|<math.*?>|</math>|<semantics.*?>|</semantics>|<mrow.*?>|</mrow>|<mstyle.*?>|</mstyle>|<msub.*?>|</msub>|<annotation.*?>[^]*?</annotation>|<a.*?>|</a>|<input.*?>|<img.*?/>|<footer.*?>[^]*?</footer>|<nav.*?>[^]*?</nav>|<sup.*?>[^]*?</sup>|<style.*?>[^]*?</style>|</abbr>|<table.*?>[^]*?</table>|<label.*?>[^]*?</label>)";

  // create sections
  string sectionsBegin = "(<h[1-6].*?>[\\n\\s]*)";
  string sectionsEnd = "([\\n\\s]*</h[1-6]>)";
  string lineBreak = "(<\\/br>|</p>)";

  // lists
  string listBegin = "(<ul.*?>)";
  string listEnd= "(</ul>)";
  string listItem ="(<li.*?>[\\n\\s]*|<li>)";

  // italics and bold
  string boldBegin = "(<b>)";
  string boldEnd= "(</b>)";
  string italicBegin = "(<em>|<i>)";
  string italicEnd = "(</em>|</i>)";
  string spanBegin = "<span.*?>";
  string spanEnd = "</span>";
  string mathBegin = "[\\n\\s]*?<mi>[\\n\\s]*?";
  string mathEnd= "[\\n\\s]*?</mi>[\\n\\s]*?";
  string quoteBegin = "<dl><dd>";
  string quoteEnd= "</dd></dl>";

  //string res = regex_replace(regex_replace(regex_replace(line, filter, ""), sectionsBegin, "\\section{"), sectionsEnd, "}"); 
  string res = "";
  f.replace(lineBreak, "\\\\\n");
  f.replace(filter, "");
  f.replaceFrom(sectionsBegin, sectionsEnd, "\\section{", "}");
  f.replaceFrom(listBegin, listEnd, "\\begin{itemize}", "\\end{itemize}");
  f.replaceFrom(boldBegin, boldEnd, "\\textbf{", "}");
  f.replaceFrom(spanBegin, spanEnd, "", "");
  f.replaceFrom(italicBegin, italicEnd, "\\emph{", "}");
  f.replaceFrom(mathBegin, mathEnd, " \\(", "\\) ");
  f.replaceFrom(quoteBegin, quoteEnd, " \\begin{displayquote}", "\\end{displayquote}");
  f.replace(listItem, "\\item");
  f.replace("^\\s*$", "");

  cout << f.getResult();
  return f.getResult();

}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

int main () {
  char fname[100];
  char url[1000];

  printf("Enter a filename: ");
  scanf("%s", fname);
  printf("Enter a URL: ");
  scanf("%s", url);

  string line = "Something went wrong!";

  emptyFile(fname);

  CURL *curl;
  CURLcode res;
  string readBuffer;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    //string line = readBuffer;
    initLatexDoc(fname);
    string line = filterBuffer(readBuffer);
    writeToFile(fname, line);
    writeToFile(fname,"\\end{document}");
    //std::cout << readBuffer << std::endl;
  }

  return 0;
}
