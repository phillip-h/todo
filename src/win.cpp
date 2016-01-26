#include "win.hpp"

using std::string;

bool Win::colors;

/////////////////////////////////////////////////
// constructor, set position and size variables
// and create the WINDOW* and border
Win::Win(unsigned x, unsigned y, unsigned w, unsigned h, 
         string name, bool border)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    this->name = name;

    this->border = newwin(h, w, y, x); 
    
    if (border)
        win = newwin(h - 1, w - 1, y + 1, x + 1);
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

////////////////////////////////////////
// set if colors should be used or not
void Win::setColors(bool colors)
{
    Win::colors = colors;
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
    mvwprintw(border, 0, 0, string("[" + name + "]").c_str());
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

void Win::inverse(bool border)
{
    if (border)
        wattron(this->border, A_STANDOUT);
    else
        wattron(this->win, A_STANDOUT);
}

void Win::inverseOff(bool border)
{
    if (border)
        wattroff(this->border, A_STANDOUT);
    else
        wattroff(this->win, A_STANDOUT);   
}

void Win::color(int pair, bool border)
{
    if (border)
        wattron(this->border, COLOR_PAIR(pair));
    else
        wattron(this->win, COLOR_PAIR(pair));
}

void Win::colorOff(int pair, bool border)
{
    if (border)
        wattroff(this->border, COLOR_PAIR(pair));
    else
        wattroff(this->win, COLOR_PAIR(pair));
}
