#include "Controller.h"

Controller::Controller(MyString FN) : Controller::Controller(){
      filename = FN;
      ifstream inFile(filename.data());
      if (inFile.is_open()){
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

	view.AddChar('M');
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
		/*case 'q':
			mode = 'q';
			view.updateStatus(mode);
			view.printStatusLine();
			if (getch() != 10)
			mode = 'n';
			break;*/
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
}
}

void Controller::proccesInsert(int c){
	switch(c){
		case KEY_EXIT:
		case 27:
			mode = 'n';
			break;
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
			cout << 2.4 << endl;
			view.statusAddChar(c);
			//cout << 2.3 << endl;
			//break;
	}
	view.printStatusLine();
}

void Controller::proccesCommand(int c){
	view.x = 0;
	view.y = LINES - 1;
	move(view.y, view.x);
	MyString temp;
	switch(c){
	case 'o':
		view.updateStatus("o ");
		view.printStatusLine();
		while (1){
			cout << 1 << endl;
			c = getch();
			cout << 2 << endl;
			/*if (c == KEY_EXIT or c == 27){
				mode = 'n';
				return;
			}*/
			if (c == KEY_ENTER or c == 10){
				saveNewFile(temp);
				return;
			}
			cout << 3 << endl;
			temp = statusHandleInput(c, 2);
			cout << 4 << endl;
		}

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