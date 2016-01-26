#ifndef WIN_
#define WIN_

#include <ncurses.h>
#include <string>

class Win
{
private:
    WINDOW *win;
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;

public:
    Win(unsigned x, unsigned y, unsigned width, unsigned height);
    ~Win(); 

    void clear();
    void draw();

    void print(std::string text);
    void print(unsigned x, unsigned y, std::string text);
};

#endif
