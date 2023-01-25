#ifndef _VIEW_H_
#define _VIEW_H_
#include "MyString.h"
#include "Model.h"
//#include "Controller.h"
#include <iostream>
#include <fstream>
#include <ncurses.h>

class View{
	private:
	#define curLine (y + offsetLine) //A "pointer" to a current line among whole vector of lines
	#define WNDW (LINES - 2) //Size of a window which displays text
		int x, y;
		int offsetLine;
		MyString status;

		bool Up();
		bool Down();
		bool Left();
		bool Right();

		void PageUp();
		void PageDown();

		void Home();
		void End();

		void moveStart();
		void moveEnd();
		void moveToLine(int line);

		void wordEnd();
		void wordStart();
		void deleteWord();

		void BackSpace();
		void DeleteRight();
		void EnterKey();
		void TabKey();

		void statusBackSpace(int);
		void statusDelete();
		void statusAddChar(int);

		void AddChar(char);
		void deleteLine();
		void deleteLine(int);

		void copyLine();
		void cutLine();
		void copyWord();
		void insertBuffer();

	public:
		View();

		friend class Controller;

		void printLine();
		void printStatusLine();
		void updateStatus(char);
		void updateStatus(MyString);
};

#endif
