#include "Model.h"

Model::Model(){}

void Model::insertLine(MyString line, int n){
  line = remTabs(line);
  lines.insert(lines.begin() + n, line);
}

void Model::appendLine(MyString line){
  line = remTabs(line);
  lines.push_back(line);
}

void Model::removeLine(int n){
  lines.erase(lines.begin() + n);
}

MyString Model::remTabs(MyString line){
  auto tab = line.find("\t");
  if (tab == -1)
    return line;
  else
    return remTabs(line.replace(tab, 1, "    "));
}