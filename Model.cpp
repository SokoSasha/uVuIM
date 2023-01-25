#include "Model.h"

Model::Model(){
	buffer = "\0";
}

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

int Model::lineSize(int line){
	return lines[line].size();
}

void Model::Insert(int line, int idx, int count, char chr){
	lines[line].insert(idx, count, chr);
}

int Model::wordEnd(int line, int idx){
	size_t res = -1;
	int i = 0;
	for (; i < 28; i++){
		MyString chr(1, table[i]);
		size_t tmp = lines[line].find(chr.data(), idx);
		if (tmp != -1){
			/*if (tmp != 0 and lines[line][tmp - 1] == lines[line][tmp] and idx < lineSize(line)){
				i = 0;
				idx++;
				continue;
			}*/
			res = (res == -1) ? tmp : min(res, tmp);
		}
	}

	return res;
}

int Model::wordStart(int line, int idx){
	int tmp = idx;
	while(1){
		tmp--;
		if (tmp == -1)
			return -1;

		int i = 0;
		int res = - 1;
		for (; i < 28; i++){
			if (tmp == lineSize(line) - 1)
				break;

			if (lines[line][tmp] == table[i]/* and lines[line][tmp + 1] != lines[line][tmp] and tmp != idx - 1*/)
				res = (res == -1) ? tmp : min(res, tmp);
		}
		if (res != -1 or (res == -1 and tmp == 0))
			return res + 1;
	}
}

void Model::deleteWord(int line, int start, int end){
	lines[line].erase(start, end - start + 1);
}

void Model::copyToBuf(MyString line){
	buffer = line;
}

void Model::copyToBuf(int line){
	buffer = lines[line];
}

void Model::insertBuf(int line, int idx){
	lines[line].insert(idx, buffer.data(), buffer.size());
}