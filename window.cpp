//////////////////////////////////////////////////////////////////////
// window.cpp -- C++ NCurses Class Library -- Window
// Date: Mon Jul 22 22:01:51 2019   (C) ve3wwg@gmail.com
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "cppcurses.hpp"

#include <curses.h>
#include <map>

static const std::map<char,attr_t> attr_map({
	{ 'N',	A_NORMAL },		// Normal display (no highlight)
	{ 'S',	A_STANDOUT },		// Best highlighting mode of the terminal.
	{ 'U',	A_UNDERLINE },		// Underlining
	{ 'R',	A_REVERSE },		// Reverse video
	{ 'b',	A_BLINK },		// Blinking
	{ 'D',	A_DIM },		// Half bright
	{ 'B',	A_BOLD },		// Extra bright or bold
	{ 'A',	A_ALTCHARSET },		// Alternate character set
});

//////////////////////////////////////////////////////////////////////
// Convert attribute string to curses attributes
//////////////////////////////////////////////////////////////////////

static attr_t
to_attrs(const char *attrstr) {
	attr_t attrs = 0;
	char ch;

	if ( !attrstr || !*attrstr )
		return A_NORMAL;

	while ( (ch = *attrstr++) != 0 ) {
		auto it = attr_map.find(ch);
		if ( it != attr_map.end() )
			attrs |= it->second;
	}
	return attrs;
}

static inline void
curs_wattr_on(void *win,attr_t a) {
	wattr_on((WINDOW*)win,a,nullptr);
}

static inline void
curs_wattr_off(void *win,attr_t a) {
	wattr_off((WINDOW*)win,a,nullptr);
}

static inline void
curs_wattr_set(void *win,attr_t a) {
	wattr_set((WINDOW*)win,a,0,nullptr);
}

static inline void
curs_waddch(void *win,int ch) {
	waddch((WINDOW*)win,ch);
}

static inline void
curs_waddstr(void *win,const char *str) {
	waddstr((WINDOW*)win,str);
}

#undef attr_on
#undef wattr_on
#undef attr_off
#undef wattr_off
#undef attr_set
#undef wattr_set
#undef addch
#undef waddch
#undef addstr
#undef waddstr

Window::Window(CppCurses *main,void *win) : win(win) {
	mainf = true;	

	cbreak();		// Disable line buffering
	noecho();
	keypad(stdscr,TRUE);	// Recognize keys
}

Window::~Window() {
	if ( mainf ) {
		main->fini();
		mainf = false;
	} else	{
		delwin((WINDOW*)win);
		win = nullptr;
	}
}

Window&
Window::addch(int ch) {
	curs_waddch(win,ch);
	return *this;
}

Window&
Window::addstr(const char *str) {
	curs_waddstr(win,str);
	return *this;
}

Window&
Window::addstr(const std::string& str) {
	curs_waddstr(win,str.c_str());
	return *this;
}

size_t
Window::printf(const char *format,...) {
	va_list ap;
	size_t n;

	va_start(ap,format);
	n = vwprintw((WINDOW*)win,format,ap);
	va_end(ap);
	return n;
}

size_t
Window::mvprintf(int y,int x,const char *format,...) {
	va_list ap;
	size_t n;

	wmove((WINDOW*)win,y,x);
	va_start(ap,format);
	n = vwprintw((WINDOW*)win,format,ap);
	va_end(ap);
	return n;
}

Window&
Window::refresh() {

	assert(win);
	wrefresh((WINDOW*)win);
	return *this;
}

Window&
Window::move(int y,int x) {
	wmove((WINDOW *)win,y,x);
	return *this;
}

Window&
Window::erase() {
	werase((WINDOW*)win);
	return *this;
}

Window&
Window::clear() {
	wclear((WINDOW*)win);
	return *this;
}

Window&
Window::attr_on(const char *attrs) {
	attr_t a = to_attrs(attrs);

	curs_wattr_on(win,a);
	return *this;
}

Window&
Window::attr_off(const char *attrs) {
	attr_t a = to_attrs(attrs);

	curs_wattr_off(win,a);
	return *this;
}

Window&
Window::attr_set(const char *attrs) {
	attr_t a = to_attrs(attrs);

	curs_wattr_set(win,a);
	return *this;
}

// End window.cpp
