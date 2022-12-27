#ifndef _MODEL_H_
#define _MODEL_H_
#include "MyString.h"
#include <iostream>
#include <vector>
#include <ncurses.h>
using namespace std;

class Model{
	public:
		Model();

		void insertLine(MyString, int);
		void appendLine(MyString);
		void removeLine(int);
		MyString remTabs(MyString);

		friend class View;
		friend class Controller;

	private:
		vector<MyString> lines;
};

#endif
