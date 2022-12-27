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
	#define curLine y + offsetLine
		int x, y;
		int offsetLine;
		MyString status;

		void Up();
		void Down();
		void Left();
		void Right();

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

	public:
		View();

		friend class Controller;

		void printLine();
		void printStatusLine();
		void updateStatus(char);
		void updateStatus(MyString);
};

#endif
