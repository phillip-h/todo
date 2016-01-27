#include "typer.hpp"

#include <ncurses.h>

#include "config.hpp"

using std::string;

string type(Win *win, unsigned size, string base)
{
    char* buf = new char[size];
    for (unsigned i = 0; i < size; i++)
        buf[i] = i < base.size() ? base.at(i) : 0;
    unsigned pos = base.size();
   
    curs_set(1);

    int in;
    bool typing = true;
    while(typing){

        win->clear();
        win->print(string(buf));
        win->color(BORDER_COLOR_PAIR, true);       
        win->draw();
        
        switch(in = getch()){
            case 27: // ESCAPE
                curs_set(0);
                return base;
            case '\n':
                typing = false;
                break;
            case KEY_BACKSPACE:
                if (pos)
                    pos--;
                buf[pos] = 0;
                break;
            default:
                if (in > 127)
                    break;
                buf[pos] = in;
                if (pos < size)
                    pos++;
                break;
        }
    }
    
    curs_set(0);

    string ret(buf);
    delete[] buf;
    return ret;
}
