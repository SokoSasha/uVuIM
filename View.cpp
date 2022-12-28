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
	if (x > model.lineSize(curLine))
		x = model.lineSize(curLine);
	move(y, x);
}

void View::Down(){
	if (y < WNDW - 1 && curLine < model.modelSize() - 1)
		y++;
	else if (y == WNDW - 1 && model.modelSize() - 1 > curLine)
		offsetLine++;
	else
		beep();
	if (x > model.lineSize(curLine))
		x = model.lineSize(curLine);
	move(y, x);
}

void View::Left(){
	if (x > 0)
		x--;
	else if (x == 0){
		if (y > 0){
			y--;
			x = model.lineSize(curLine);
		}
		else if (y==0 && offsetLine > 0){
			offsetLine--;
			x = model.lineSize(curLine);
		}
		else
			beep();
	}
	move(y, x);
}

void View::Right(){
	if (x < model.lineSize(curLine)){
		x++;
	}
	else if (x == model.lineSize(curLine) && curLine < model.modelSize() - 1){
		if (y < WNDW)
			y++;
		else
			offsetLine++;
		x = 0;
	}
	else
		beep();
	move(y, x);
}

void View::PageUp(){
	if (offsetLine > WNDW) offsetLine -= WNDW;
	else if (offsetLine != 0)
		offsetLine = 0;
	else beep();
	if (x > model.lineSize(curLine))
		x = model.lineSize(curLine);
}

void View::PageDown(){
	if (offsetLine + WNDW * 2 + 2 <= model.modelSize()) offsetLine += WNDW;
	else if (offsetLine + WNDW < model.modelSize())
		offsetLine = model.modelSize() - WNDW;
	else
		beep();
	if (x > model.lineSize(curLine))
		x = model.lineSize(curLine);
}

void View::Home(){
	offsetLine = 0;
	x = 0;
	y = 0;
}

void View::End(){
	offsetLine = model.modelSize() - WNDW;
	x = model.lastLineSize();
	if (model.modelSize() > WNDW){
		offsetLine = model.modelSize() - WNDW;
		y = WNDW - 1;
	}
	else
		y = model.modelSize();
}

void View::moveStart(){
	x = 0;
}

void View::moveEnd(){
	x = model.lineSize(curLine);
}

void View::wordEnd(){ //Почему-то застревает после 1 использования, хотя сама программа не виснет
	int pos;
	while(1){
		pos = model.wordEnd(curLine, x);
		if (x == model.lineSize(curLine) or pos == model.lineSize(curLine)){
			Right();
			continue;
		}
		break;
	}
	if (pos == -1){
		x = model.lineSize(curLine) ? model.lineSize(curLine) : x;
		return;
	}
	if (pos == x){
		while(1){
			pos = model.wordEnd(curLine, x + 1);
			if (x == model.lineSize(curLine) or pos == model.lineSize(curLine)){
				Right();
				continue;
			}
			break;
		}
		if (pos == -1){
			x = model.lineSize(curLine) ? model.lineSize(curLine) : x;
			return;
		}
	}
	x = pos;
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
	if(x == model.lineSize(curLine) and curLine != model.modelSize() - 1){
		model.lines[curLine] += model.lines[curLine + 1];
		deleteLine(curLine + 1);
	}
	else if (x != model.lineSize(curLine))
		model.lines[curLine].erase(x, 1);
	else
		beep();
}

void View::TabKey(){
	model.Insert(curLine, x, 4, ' ');
		x += 4;
}

void View::EnterKey(){
	if (x < model.lineSize(curLine)){
		model.insertLine(model.lines[curLine].substr(x, model.lineSize(curLine) - x), curLine + 1);
		model.lines[curLine].erase(x, model.lineSize(curLine) - x);
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
	model.Insert(curLine, x, 1, c);
	x++;
}

void View::printLine(){
	for (int i = 0; i < WNDW && i + offsetLine < model.modelSize(); i++){
		if (i + offsetLine > model.modelSize()){
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

	status += "\tCOL: " + to_string(x) + "\tROW: " + to_string(curLine) +
		"\tTotal: " + to_string(model.modelSize()) + "\toffsetLine: " + to_string(offsetLine); //Надо добавить этот метод
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