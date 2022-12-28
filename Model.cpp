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

int Model::wordEnd(int l, int idx){
	string table[] = {" ", "!", "^", "&", "*", "(", ")", "-", "=", "+", "[", "{", "]", "}", "\\", "|", ",", "<", ".", ">", "/", "?", "`", "~", ";", ":", "\'", "\""};

	size_t res = -1;
	int i = 0;
	for (; i < 28; i++){
		size_t tmp = lines[l].find(table[i], idx);
		if (tmp != -1){
			if (tmp != 0 and lines[l][tmp - 1] == lines[l][tmp] and idx < lineSize(l)){
				i = 0;
				idx++;
				continue;
			}
			/*if ((lines[l][tmp] < 'A' or lines[l][tmp] > 'Z') and (lines[l][tmp] < 'a' or lines[l][tmp] > 'z') and (lines[l][tmp] < '0' and lines[l][tmp] > '9')){
				i = 0;
				idx++;
				continue;
			}*/
			res = min(res, tmp);
		}
	}

	return res;
}