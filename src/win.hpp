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
    
    std::string name;

public:
    Win(unsigned x, unsigned y, unsigned w, unsigned h, 
        std::string name, bool border = 1);
    ~Win(); 

    static void setColors(bool colors);

    void clear();
    void draw();

    void move(unsigned x, unsigned y);

    void print(std::string text);
    void print(std::string text, unsigned x, unsigned y);

    void inverse(bool border = 0);
    void inverseOff(bool border = 0);
    void color(int pair, bool border = 0);
    void colorOff(int pair, bool border = 0);

    bool mouse(int &x, int &y);

    WINDOW* getWin();
};

#endif
