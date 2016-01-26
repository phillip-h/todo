#include "win.hpp"

using std::string;

/////////////////////////////////////////////////
// constructor, set position and size variables
// and create the WINDOW*
Win::Win(unsigned x, unsigned y, unsigned width, unsigned height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    win = newwin(height, width, y, x);
}

//////////////////////////////////
// destructor, destroy the window
Win::~Win()
{
   delwin(win); 
}

////////////////////
// clear the window
void Win::clear()
{
    wclear(win);
}

///////////////////////////////////
// draw the current window buffer
void Win::draw()
{
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
void Win::print(unsigned x, unsigned y, string text)
{
    mvwprintw(win, y, x, text.c_str());
}
