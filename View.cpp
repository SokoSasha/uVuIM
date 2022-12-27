#include "View.h"
#include "Controller.h"

#define model Controller::model

View::View(){
	x = 0;
	y = 0;
	offsetLine = 0;
	status = "Navigation mode";
	model.appendLine("");
}

void View::Up(){
	if (y > 0)
		y--;
	else if (y == 0 && offsetLine > 0)
		offsetLine--;
	else
		beep();
	if (x > model.lines[curLine].length())
		x = model.lines[curLine].length();
	move(y, x);
}

void View::Down(){
	if (y < LINES - 3 && curLine < model.lines.size() - 1)
		y++;
	else if (y == LINES - 3 && model.lines.size() - 1 > curLine)
		offsetLine++;
	else
		beep();
	if (x > model.lines[curLine].length())
			x = model.lines[curLine].length();
	move(y, x);
}

void View::Left(){
	if (x > 0)
		x--;
	else if (x == 0){
		if (y > 0){
			y--;
			x = model.lines[curLine].length();
		}
		else if (y==0 && offsetLine > 0){
			offsetLine--;
			x = model.lines[curLine].length();
		}
		else
			beep();
	}
	move(y, x);
}

void View::Right(){
	if (x < model.lines[curLine].length()){
		x++;
	}
	else if (x == model.lines[curLine].length() && curLine < model.lines.size() - 1){
		if (y < LINES - 2)
			y++;
		else
			offsetLine++;
		x = 0;
	}
	else
		beep();
	move(y, x);
}

void View::BackSpace(){
	if (x == 0 && y != 0){
		x = model.lines[curLine - 1].length();
		model.lines[curLine - 1] += model.lines[curLine];
		deleteLine();
		if (offsetLine > 0)
			offsetLine--;
		else
			Up();
	}
	else if (x > 0 && y >= 0)
		model.lines[curLine].erase(--x, 1);
	else
		beep();
}

void View::DeleteRight(){
	if(x == model.lines[curLine].length() and curLine != model.lines.size() - 1){
		model.lines[curLine] += model.lines[curLine + 1];
		deleteLine(curLine + 1);
	}
	else if (x != model.lines[curLine].length())
		model.lines[curLine].erase(x, 1);
	else
		beep();
}

void View::TabKey(){
	model.lines[curLine].insert(x, 4, ' ');
		x += 4;
}

void View::EnterKey(){
	if (x < model.lines[curLine].length()){
		model.insertLine(model.lines[curLine].substr(x, model.lines[curLine].length() - x), curLine + 1);
		model.lines[curLine].erase(x, model.lines[curLine].length() - x);
	}
	else{
		model.insertLine("", curLine + 1);
	}
	x = 0;
	Down();
}

void View::deleteLine(){
	model.removeLine(curLine);
}

void View::deleteLine(int cord){
	model.removeLine(cord);
}

void View::AddChar(char c){
	if (x == COLS - 1){ //Надо будет исправить случай с 1 длинной строкой
		x = 0;
		y++;
		model.appendLine("");
	}
	model.lines[curLine].insert(x, 1, c);
	x++;
}

void View::printLine(){
	for (int i = 0; i < LINES - 2 && i + offsetLine < model.lines.size(); i++){
		if (i + offsetLine > model.lines.size()){
			move(i, 0);
			clrtoeol();
		}
		else
			mvprintw(i, 0, "%s\n", model.lines[i + offsetLine].data());
		clrtoeol();
	}
	move(y, x);
}

void View::printStatusLine(){
	attron(A_REVERSE);
	mvprintw(LINES - 1, 0, "%s", status.data());
	clrtoeol();
	attroff(A_REVERSE);
}

void View::updateStatus(char mode){
	switch(mode){
		case 'n':
			status = "Navigation mode";
			break;
		case 'w':
			status = "Write mode";
			break;
		case 'c':
			status = "Command mode";
			break;
		case 'q':
			status = "To quit press Enter";
			break;
		case 'r':
			status = mode;
	}

	auto cols = status.find("\tCOL");
	if (cols != -1)
		status.erase(cols, status.size());

	status += "\tCOL: " + to_string(x) + "\tROW: " + to_string(curLine); //Надо добавить этот метод
}

void View::updateStatus(MyString extra){
	status.clear();
	status = extra.data();
}

void View::statusBackSpace(int bolder){
	if (x >= bolder)
		status.erase(x-1, 1);
	else beep();
}

void View::statusDelete(){
	if (x < status.length())
		status.erase(x, 1);
	else beep();
}

void View::statusAddChar(int c){
	cout << 2.31 << endl;
	status.append(1, (char)c);
	x++;
	move(y, x);
}