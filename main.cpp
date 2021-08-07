#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <regex>
#include "./filterText.h"

using namespace std;

/*
 * Creates an empty file with the given filename
 * @input fname - the filename to be created
*/

void emptyFile(char fname[100]){
  fstream file;
  file.open(fname, std::fstream::out | std::fstream::trunc);
  file.close();
}

/*
 * Appends the string 'line' to the end of a file with the 'fname' filename.
 * If no file exists this function will create it.
 * @input fname - the filename to be created
 * @input line - the filename to be created
*/
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

/*
 * Initializes a LaTeX document with a.5 inch margin and underlines on every section.
 * @input fname - the name of the LaTeC file 
*/
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

/*
 * Filters the buffer string to extract any undecessary tags and tranlate any 
 * other tags into valid LaTeX.
 * @input buffer- the text thats going to be translated to LaTeX 
 * @output string- the translated LaTeX text 
*/

string filterBuffer(string buffer){
  using namespace std::regex_constants;
  FilterText f;
  f.filterThis(buffer);

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

  // Extract text from html
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

/*
 * Used by cURL
*/

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  ((std::string*)userp)->append((char*)contents, size * nmemb);
  return size * nmemb;
}

/**
 * Will prompt user for a filename and URL. Initialize a LaTeX doc. and then fetch 
 * the HTML doc. from the given URL. filterBuffer will translate the HTML into 
 * LateX the this string is then written to the given filename.
*/

int main () {
  char fname[100];
  char url[1000];

  // Prompt user
  printf("Enter a filename: ");
  scanf("%s", fname);
  printf("Enter a URL: ");
  scanf("%s", url);

  // Init file
  emptyFile(fname);

  // Start fetch
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

    // Init LaTeX
    initLatexDoc(fname);
    // Filter HTML
    string line = filterBuffer(readBuffer);

    writeToFile(fname, line);
    writeToFile(fname,"\\end{document}");
  }

  return 0;
}
