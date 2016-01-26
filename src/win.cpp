#include "win.hpp"

using std::string;

/////////////////////////////////////////////////
// constructor, set position and size variables
// and create the WINDOW* and border
Win::Win(unsigned x, unsigned y, unsigned w, unsigned h, bool border)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->border = newwin(h, w, y, x); 
    
    if (border)
        win = newwin(h - 1, w - 1, y - 1, x - 1);
    else
        win = newwin(h, w, y, x);
}

//////////////////////////////////
// destructor, destroy the window
Win::~Win()
{
    delwin(border);
    delwin(win); 
}

////////////////////
// clear the window
void Win::clear()
{
    wclear(border);
    wclear(win);
}

///////////////////////////////////
// draw the current window buffer
void Win::draw()
{
    box(border, 0, 0);
    wrefresh(border);
    wrefresh(win);    
}

/////////////////////////////
// print text to the window
void Win::print(string text)
{
    wprintw(win, text.c_str());
}

//////////////////////////////////////
// print text to the window at (x, y)
void Win::print(string text, unsigned x, unsigned y)
{
    mvwprintw(win, y, x, text.c_str());
}
