//////////////////////////////////////////////////////////////////////
// main.cpp -- Test
// Date: Mon Jul 22 22:18:38 2019   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "cppcurses.hpp"

CppCurses curses;

int
main(int argc,char **argv) {
	bool okf;

	okf = curses.open();
	assert(okf);

	Window *w = curses.get_window();

	w->printf("Hello World!\n");
	w->colour(Colour::Yellow,Colour::Black).attr_on("B");
	w->mvprintf(10,10,"[10,10] ");
	w->attr_on("R").addch('X').attr_off("R");

	w->fg(Colour::Red).addstr("(Red)").bg(Colour::Yellow).addstr("(YellowBg)");
	w->bg(Colour::Black);
	w->refresh();
	curses.readch();
	
	w->colour(Colour::Red,Colour::Blue);
	w->move(12,10).addgrstr("L--T--R");
	w->move(13,10).addgrstr("|  |  |");
	w->move(14,10).addgrstr("t--+--u");
	w->move(15,10).addgrstr("|  |  |");
	w->move(16,10).addgrstr("l--B--r");
	curses.readch();

	w->erase();
	w->attr_on("U");
	w->mvprintf(12,10,"Almost done..");
	w->attr_off("N");
	w->refresh();

	curses.readch();

	okf = curses.close();	

	return 0;
}

// End main.cpp
