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

int Model::lastLineSize(){
  return lines[lines.size() - 1].size();
}

int Model::modelSize(){
  return lines.size();
}

int Model::lineSize(int l){
  return lines[l].size();
}

void Model::Insert(int l, int idx, int count, char chr){
  lines[l].insert(idx, count, chr);
}