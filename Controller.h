#ifndef _CONTROL_H_
#define _CONTROL_H_
#include "View.h"
#include <iostream>
#include <ncurses.h>

using namespace std;

class Controller {
	public:
		Controller();
		Controller(MyString);

		void MainLoop();

		inline static Model model;
		View view;

	private:
		char mode;
		MyString filename, nullStr = "";

		void handleInput(int);
		void proccesNavigation(int);
		void proccesInsert(int);
		void proccesCommand(int);
		MyString statusHandleInput(int, int);
		void saveNewFile(MyString);

};

#endif
