#ifndef WIN_
#define WIN_

#include <ncurses.h>
#include <string>

class Win
{
private:
    static bool colors;
    
    WINDOW *win;
    WINDOW *border;
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;

public:
    Win(unsigned x, unsigned y, unsigned w, unsigned h, bool border = 1);
    ~Win(); 

    static void setColors(bool colors);

    void clear();
    void draw();

    void print(std::string text);
    void print(std::string text, unsigned x, unsigned y);

    void inverse(bool border = 0);
    void inverseOff(bool border = 0);
    void color(int pair, bool border = 0);
    void colorOff(int pair, bool border = 0);
};

#endif
