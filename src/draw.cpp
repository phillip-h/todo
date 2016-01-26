#include "draw.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "args.hpp"
#include "config.hpp"
#include "task.hpp"
#include "todolist.hpp"
#include "win.hpp"

using std::string;
using std::stringstream;

// ===================|
// internal functions |
// ===================|
void drawText(string text, int x, int y);

WINDOW* createWindow(int startX, int startY, int width, int height);
void setCurrentWindow(WINDOW *curWin);

std::string buildTitle(TodoList *list);

void drawTitle(TodoList* list);
void drawControls();
void drawTasks(TodoList* list, unsigned selected);

void inverseOn();
void inverseOff();
void colorOn(int pair);
void colorOff(int pair);
void colorMarkOn(string mark);
void colorMarkOff(string mark);

// ===================|
// internal variables |
// ===================|
WINDOW *taskWindow;
WINDOW *inputWindow;
WINDOW *controlWindow;
WINDOW *taskWindowBorder;
WINDOW *inputWindowBorder;
WINDOW *controlWindowBorder;
WINDOW *currentWindow;

Win *titleWin;

int startX;
int startY;
int width;
int height;

unsigned xpos;
unsigned ypos;
unsigned startTask = 0;
unsigned endTask = 0;
unsigned int listOff = 0;

bool colors = false;

/////////////////////////////////////////
// start curses mode and create windows
void Draw::init()
{
    std::cout << "\033]0;" << "todo" << "\7" << std::flush; 
    initscr();
    raw();
    
    use_default_colors();
    if (useColors() && has_colors()){
        start_color();
        init_pair(TITLE_COLOR_PAIR, TITLE_FOREGROUND, TITLE_BACKGROUND);
        init_pair(GUTTER_COLOR_PAIR, GUTTER_FOREGROUND, GUTTER_BACKGROUND);
        init_pair(SELECT_COLOR_PAIR, SELECT_FOREGROUND, SELECT_BACKGROUND);
        init_pair(BORDER_COLOR_PAIR, BORDER_FOREGROUND, BORDER_BACKGROUND);
        init_pair(MARK_COLOR_PAIR, MARK_FOREGROUND, MARK_BACKGROUND);
        init_pair(MARK_COLOR_PAIR_DONE, MARK_FOREGROUND_DONE, 
                  MARK_BACKGROUND_DONE);
        colors = true;
    }
    Win::setColors(colors);
    
    mouseinterval(0);    
    keypad(stdscr, true);
    noecho();

    clear();
    refresh();

    curs_set(0);

    startX = listStartX;
    startY = listStartY;
    width = COLS - startX - 2;
    height = LINES * listHeight;
    taskWindowBorder = createWindow(startX, startY, width, height);
    taskWindow = createWindow(startX + 1, startY + 1, 
                              width - 2, height - 2);

    int inHeight = inputHeight * LINES;
    inHeight = std::max(inHeight, 4);
    inputWindowBorder = createWindow(inputStartX, inputStartY,
                                     COLS - inputStartX - 2, inHeight);
    inputWindow = createWindow(inputStartX + 1, inputStartY + 1, 
                               COLS - inputStartX - 4, inHeight - 2);

    controlWindowBorder = createWindow(inputStartX, inputStartY, 
                                       COLS - inputStartX - 2, inHeight);
    controlWindow = createWindow(inputStartX + 1, inputStartY + 1,
                                 COLS - inputStartX - 4, inHeight - 2);

    setCurrentWindow(taskWindow);

    titleWin = new Win(0, 0, COLS, titleHeight, "title", false);
}

//////////////////////
// main draw function
void Draw::draw(TodoList *list, unsigned selected)
{
    werase(inputWindow);
    werase(inputWindowBorder);

    drawTitle(list);
    drawControls();
    drawTasks(list, selected);
}

////////////////////////
// act on mouse events
void Draw::mouse(MEVENT event, TodoList *list,
                 unsigned &selected, bool button)
{
    int x = event.x;
    int y = event.y;
    if (wmouse_trafo(taskWindow, &y, &x, false)){
        unsigned pos = listOff + y - 1;
        if (pos < list->size()){
            if (!button){
                selected = pos;
            } else{
                list->at(pos).toggleComplete();
            }
        }
    }
}

///////////////////////////////////////
// get an input string for a new task
string Draw::getInput()
{
    echo();

    werase(controlWindow);
    werase(controlWindowBorder);

    setCurrentWindow(inputWindowBorder);
    if (colors){
        colorOn(BORDER_COLOR_PAIR);
    }
    box(inputWindowBorder, 0, 0);
    
    inverseOn();
    drawText("[New task]", 0, 0);
    inverseOff();

    if (colors){
        colorOff(BORDER_COLOR_PAIR);
    }

    setCurrentWindow(inputWindow);
    wrefresh(inputWindowBorder);
    wrefresh(inputWindow);
    curs_set(1);

    char tmp[charBufferSize];
    mvwgetnstr(inputWindow, 0, 0, tmp, charBufferSize);

    setCurrentWindow(taskWindow);
    box(inputWindowBorder, ' ', ' ');
    werase(inputWindow);
    werase(inputWindowBorder);
    curs_set(0);
    noecho();

    return std::string(tmp);
}

////////////////////
//  draw the title
void drawTitle(TodoList* list)
{
    titleWin->clear();
    titleWin->inverse();
    titleWin->color(TITLE_COLOR_PAIR);
    titleWin->print(buildTitle(list), 0, 0);
    titleWin->colorOff(TITLE_COLOR_PAIR);
    titleWin->inverseOff();
    titleWin->color(BORDER_COLOR_PAIR, true);
    titleWin->draw();
}

// draw the control panel
void drawControls()
{
    setCurrentWindow(controlWindow);
    wmove(controlWindow, 0, 0);

    stringstream line;
    
    line << "[" << ((char) EXIT_KEY) << "]";
    wprintw(controlWindow, line.str().c_str());
    wprintw(controlWindow, " quit          ");
    line.str("");

    line << "[Space]";
    wprintw(controlWindow, line.str().c_str());
    wprintw(controlWindow, " new task          ");
    line.str("");

    line << "[" << ((char) MOVE_UP_KEY) << "]";
    wprintw(controlWindow, line.str().c_str());
    wprintw(controlWindow, " move task up  ");
    line.str("");

    wmove(controlWindow, 1, 0);

    line << "[" << ((char) REMOVE_KEY) << "]";
    wprintw(controlWindow, line.str().c_str());
    wprintw(controlWindow, " delete task   ");
    line.str("");

    line << "[Return]";
    wprintw(controlWindow, line.str().c_str());
    wprintw(controlWindow, " mark task done   ");
    line.str("");

    line << "[" << ((char) MOVE_DOWN_KEY) << "]";
    wprintw(controlWindow, line.str().c_str());
    wprintw(controlWindow, " move task down  ");
    line.str("");

    setCurrentWindow(controlWindowBorder);
    if (colors){
        colorOn(BORDER_COLOR_PAIR);
    }
    box(controlWindowBorder, 0, 0);
    
    inverseOn();
    drawText("[Controls]", 0, 0);
    inverseOff();

    if (colors){
        colorOff(BORDER_COLOR_PAIR);
    }

    wrefresh(controlWindowBorder);
    wrefresh(controlWindow);
}

void drawTasks(TodoList* list, unsigned selected)
{
    auto tasks = list->tasks();

    setCurrentWindow(taskWindowBorder);
    if (colors){
        colorOn(BORDER_COLOR_PAIR);
    }
    box(taskWindowBorder, 0, 0);
    
    inverseOn();
    drawText("[Tasks]", 0, 0);
    inverseOff();

    if (colors){
        colorOff(BORDER_COLOR_PAIR);
    }
    
    setCurrentWindow(taskWindow);
    werase(taskWindow);

    if (tasks && tasks->size() != 0){
        xpos = 1;
        ypos = 1;

        unsigned numTasks = height - 2;
        endTask = tasks->size();
        if (endTask > numTasks){
            endTask = numTasks;
        }
        
        if (numTasks <= tasks->size()){
            while (selected > endTask + listOff - 2 && selected != 0){
                listOff++;
            }
            while (selected < startTask + listOff && selected != list->size() - 1){
                listOff--;
            }
        } else{
            listOff = 0;
        }

        for (unsigned i = startTask + listOff; i < endTask + listOff; i++){
            if (showNumbers){
                inverseOn();
                if (colors){
                    colorOn(GUTTER_COLOR_PAIR);
                }
                std::string number = std::to_string(i);
                if (!zeroIndexNumbers){
                    number = std::to_string(i + 1);
                }
                if (number.size() < 2){
                    number = "0"+number;
                }
                drawText(number + ")", xpos, ypos);
                if (colors){
                    colorOff(GUTTER_COLOR_PAIR);
                }
                inverseOff();
                xpos += 5;
            }
            if (i == selected){
                inverseOn();
                std::string tmp = tasks->at(i).task();
                std::string line = tmp;
                if (highlightWholeLine){
                    for (int k = tmp.size() + xpos; k < width - 2; k++){
                        line += " ";
                    }
                }
                std::string mark = line.substr(0, STRING_COMPLETE.size());
                line = line.substr(mark.size());

                colorMarkOn(mark);
                drawText(mark, xpos, ypos);
                colorMarkOff(mark);

                if (colors){
                    colorOn(SELECT_COLOR_PAIR);
                }
                drawText(line, xpos + mark.size(), ypos);
                if (colors){
                    colorOff(SELECT_COLOR_PAIR);
                }
                inverseOff();
            } else{
                std::string tmp = tasks->at(i).task();
                std::string text = tmp;
                std::string mark = text.substr(0, STRING_COMPLETE.size());
                text = text.substr(mark.size());

                colorMarkOn(mark);
                drawText(mark, xpos, ypos);
                colorMarkOff(mark);

                drawText(text, xpos + mark.size(), ypos);
            }
            ypos++;
            if (ypos > numTasks || i >= tasks->size() - 1){
                break;
            }
            if (showNumbers){
                xpos -= 5;
            }
        }
    }
    wrefresh(taskWindowBorder);
    wrefresh(taskWindow);
}

// draw text to the current window
void drawText(std::string text, int x, int y)
{
    mvwprintw(currentWindow, y, x, text.c_str());
}

// create a new window
WINDOW* createWindow(int startX, int startY, int width, int height)
{
    WINDOW *win;
    win = newwin(height, width, startY, startX);
    return win;
}

// set the current window
void setCurrentWindow(WINDOW* curWin)
{
    currentWindow = curWin;
}

// draw text in inverse
void inverseOn()
{
    wattron(currentWindow, A_STANDOUT);
}

// draw text normally
void inverseOff()
{
    wattroff(currentWindow, A_STANDOUT);
}

// turn on a color pair
void colorOn(int pair)
{
    wattron(currentWindow, COLOR_PAIR(pair));
}

// turn off a color pair
void colorOff(int pair)
{
    wattroff(currentWindow, COLOR_PAIR(pair));
}

///////////////////////////////////
// stop curses and delete windows
void Draw::stop()
{
    delwin(taskWindow);
    delwin(inputWindow);
    delwin(taskWindowBorder);
    delwin(inputWindowBorder);
    
    delete titleWin;
    
    endwin();

    curs_set(1);
}

// create the title string
std::string buildTitle(TodoList *list)
{
    std::string title = "[todo version "+vn+"]";

    unsigned cent = (COLS - list->name.size() - 2) / 2;

    for (unsigned i = title.size(); i < cent; i++){
        title += division;
    }

    title += "[";
    title += list->name;
    title += "]";

    std::string stats = "[";
    stats += std::to_string(list->completed());
    stats += "/";
    stats += std::to_string(list->size());
    stats += " -> ";
    if (list->size() == 0){
        stats += "NaN";
    } else{
        stats += std::to_string((int) ((float) list->completed() / (float) list->size() * 100));
        stats += "%";
    } 
    stats += "]";

    for (unsigned i = title.size() + stats.size(); i < (unsigned)COLS; i++){
        title += division;
    }

    title += stats;
    return title;
}

void colorMarkOn(std::string mark)
{
    if (colors){
        if (mark == STRING_COMPLETE){
            colorOn(MARK_COLOR_PAIR_DONE);
        } else{
            colorOn(MARK_COLOR_PAIR);
        }
    }
}

void colorMarkOff(std::string mark)
{
    if (colors){
        if (mark == STRING_COMPLETE){
            colorOff(MARK_COLOR_PAIR_DONE);
        } else{
            colorOff(MARK_COLOR_PAIR);
        }
    }
}
