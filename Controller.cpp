#include "Controller.h"

Controller::Controller(MyString FN) : Controller::Controller(){
      filename = FN;
      ifstream inFile(filename.data());
      if (inFile.is_open()){
			while(model.modelSize() > 0)
				model.removeLine(0);
			while(!inFile.eof()){
					string temp;
					getline(inFile, temp);  //Возможно придется добавить этот метод
					model.appendLine(temp);
			}
      }
      else {
			cerr << "Cannot open file " << filename << endl;
			model.appendLine("");
      }
}

Controller::Controller() {
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, true);

	mode = 'n';
	filename = "untitled";

	model = Model();
	view = View();
}

void Controller::MainLoop(){
	while(mode != 'q'){
		view.updateStatus(mode);
		view.printStatusLine();
		view.printLine();
		int input = getch();
		handleInput(input);
	}

	refresh();
	endwin();
}

void Controller::handleInput(int c){
	switch(c){
		case KEY_LEFT:
			view.Left();
			return;
		case KEY_RIGHT:
			view.Right();
			return;
		case KEY_UP:
			view.Up();
			return;
		case KEY_DOWN:
			view.Down();
			return;
		case KEY_PPAGE:
			view.PageUp();
			return;
		case KEY_NPAGE:
			view.PageDown();
			return;
		case KEY_EXIT:
		case 27:
			mode = 'n';
			return;
	}

	switch(mode){
		case 'n':
			proccesNavigation(c);
			break;
		case 'w':
			proccesInsert(c);
			break;
		case 'c':
			proccesCommand(c);
			break;
	}
}

void Controller::proccesNavigation(int c){
	string temp;
	back:
	switch(c){
		case 'G':
			view.Home();
			break;
		case 'g':
			if (getch() == 'g')
				view.End();
			break;
		case '^':
			view.moveStart();
			break;
		case '$':
			view.moveEnd();
			break;
		case 'w':
			view.wordEnd();
			break;
		case 'b':
			view.wordStart();
			break;
		case 'x':
			view.DeleteRight();
			break;
		case 'd':
			c = getch();
			if (c == 'i'){
				if (getch() == 'w')
					view.deleteWord();
			}
			if (c == 'd'){
				view.cutLine();
			}
			break;
		case 'y':
			c = getch();
			if (c == KEY_ENTER or c == 10){
				view.copyLine();
			}
			if (c == 'w'){
				view.copyWord();
			}
			break;
		case 'p':
			view.insertBuffer();
			break;
		case 'I':
			view.x = 0;
			move(view.y, view.x);
			c = 'i';
			goto back;
		case 'S':
			view.x = model.lines[view.y + view.offsetLine].length();
			move(view.y, view.x);
			c = 'i';
			goto back;
		case 'A':
			model.removeLine(view.y + view.offsetLine);
			view.x = 0;
			move(view.y, view.x);
			c = 'i';
			goto back;
		case 'r':
			view.updateStatus('r');
			view.printStatusLine();
			temp = char(getch());
			view.updateStatus("r" + temp);
			view.printStatusLine();
			if (view.x != 0)
				model.lines[view.y + view.offsetLine].replace(view.x-1, 1, temp);
			else {
				model.lines[view.y + view.offsetLine].insert(view.x, temp);
				view.x++;
				move(view.y, view.x);
			}
			break;
		case 'i':
			mode = 'w';
			break;
		case ':':
			mode = 'c';
			break;
		case 's':
			saveNewFile(filename);
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			int num = c - '0';
			while((c = getch()) >= '0' and c <= '9'){
				num = num * 10 + (c - '0');
			}
			if (c == 'G')
				view.moveToLine(num);
			break;
}
}

void Controller::proccesInsert(int c){
	switch(c){
		case KEY_EXIT:
		case 27:
			mode = 'n';
			return;
		case 127:
		case KEY_BACKSPACE:
			view.BackSpace();
			break;
		case KEY_DC:
			view.DeleteRight();
			break;
		case KEY_ENTER:
		case 10:
			view.EnterKey();
			break;
		case KEY_BTAB:
		case KEY_CTAB:
		case KEY_STAB:
		case KEY_CATAB:
		case 9:
			view.TabKey();
			break;
		default:
			view.AddChar(c);
			break;
	}
}



MyString Controller::statusHandleInput(int c, int border){
	switch(c){
		case 127:
		case KEY_BACKSPACE:
			view.statusBackSpace(border);
			break;
		case KEY_DC:
			view.statusDelete();
			break;
		case KEY_BTAB:
		case KEY_CTAB:
		case KEY_STAB:
		case KEY_CATAB:
		case 9:
			break;
		default:
			view.statusAddChar(c);
	}
	view.printStatusLine();
}

void Controller::proccesCommand(int c){
	view.x = 0;
	view.y = LINES - 1;
	move(view.y, view.x);
	MyString temp;
	switch(c){
	case KEY_EXIT:
	case 27:
		mode = 'n';
		return;
	case 'o':
		view.updateStatus("o ");
		view.printStatusLine();
		while (1){
			c = getch();
			if (c == KEY_EXIT or c == 27){
				mode = 'n';
				return;
			}
			if (c == KEY_ENTER or c == 10){
				saveNewFile(temp);
				return;
			}
			temp = statusHandleInput(c, 2);
			view.printStatusLine();
		}
	case 'x':
		saveNewFile(filename);
		mode = 'q';
		return;
	case 'w':
		if ((c = getch()) == KEY_ENTER or c == 10)
			saveNewFile(filename);
		return;
	}
}

void Controller::saveNewFile(MyString filename){
	ofstream fout(filename.data());
	if (fout.is_open()){
		for (int i = 0; i < model.lines.size(); i++){
			fout << model.lines[i].data(); //Надо сделать метод
			fout << "\n";
		}
		view.status = "Saved! Press any key to continue";
		view.printStatusLine();
		getch();
	}
	else
	view.status = "Error: cannot open file!";
	fout.close();
}