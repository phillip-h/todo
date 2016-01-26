#ifndef WIN_
#define WIN_

#include <ncurses.h>
#include <string>

class Win
{
private:
    WINDOW *win;
    WINDOW *border;
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;

public:
    Win(unsigned x, unsigned y, unsigned w, unsigned h, bool border = 1);
    ~Win(); 

    void clear();
    void draw();

    void print(std::string text);
    void print(std::string text, unsigned x, unsigned y);
};

#endif
