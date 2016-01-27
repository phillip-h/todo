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
        win = newwin(h - 2, w - 2, y + 1, x + 1);
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
    werase(border);
    werase(win);
}

///////////////////////////////////
// draw the current window buffer
void Win::draw()
{
    box(border, 0, 0);
    inverse(true);
    mvwprintw(border, 0, 0, string("[" + name + "]").c_str());
    inverseOff(true);
    wrefresh(border);
    wrefresh(win);    
}

/////////////////////////////////////////
// move window print position to (x, y)
void Win::move(unsigned x, unsigned y)
{
    wmove(win, y, x);
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

//////////////////////////
// turn on inverse mode
void Win::inverse(bool border)
{
    if (border)
        wattron(this->border, A_STANDOUT);
    else
        wattron(this->win, A_STANDOUT);
}

//////////////////////////
// turn off inverse mode
void Win::inverseOff(bool border)
{
    if (border)
        wattroff(this->border, A_STANDOUT);
    else
        wattroff(this->win, A_STANDOUT);   
}

///////////////////////
// turn on color pair
void Win::color(int pair, bool border)
{
    if (border)
        wattron(this->border, COLOR_PAIR(pair));
    else
        wattron(this->win, COLOR_PAIR(pair));
}

////////////////////////
// turn off color pair
void Win::colorOff(int pair, bool border)
{
    if (border)
        wattroff(this->border, COLOR_PAIR(pair));
    else
        wattroff(this->win, COLOR_PAIR(pair));
}

////////////////////////////////////////////
// transform mouse click x, y to window
// coordinates
bool Win::mouse(int &x, int &y)
{
   return wmouse_trafo(win, &y, &x, false); 
}
