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

// ===================|
// internal variables |
// ===================|
WINDOW *inputWindow;
WINDOW *inputWindowBorder;
WINDOW *currentWindow;

Win *titleWin;
Win *taskWin;
Win *controlWin;

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

    int inHeight = inputHeight * LINES;
    inHeight = std::max(inHeight, 4);
    inputWindowBorder = createWindow(inputStartX, inputStartY,
                                     COLS - inputStartX - 2, inHeight);
    inputWindow = createWindow(inputStartX + 1, inputStartY + 1, 
                               COLS - inputStartX - 4, inHeight - 2);


    titleWin = new Win(0, 0, COLS, titleHeight, "title", false);
    taskWin = new Win(startX, startY, width, height, "tasks");
    controlWin = new Win(inputStartX, inputStartY, COLS - inputStartX - 2, 
                         inHeight, "controls");
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
    if (taskWin->mouse(x, y)){
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
    controlWin->color(BORDER_COLOR_PAIR, true);
    controlWin->box();
    controlWin->draw(true);

    controlWin->clear();
    controlWin->move(0, 0);

    stringstream line;
    
    line << "[" << ((char) EXIT_KEY) << "] quit          ";
    controlWin->print(line.str());
    line.str("");

    line << "[Space] new task          ";
    controlWin->print(line.str());
    line.str("");

    line << "[" << ((char) MOVE_UP_KEY) << "] move task up  ";
    controlWin->print(line.str());
    line.str("");

    controlWin->move(0, 1);

    line << "[" << ((char) REMOVE_KEY) << "] delete task   ";
    controlWin->print(line.str());
    line.str("");

    line << "[Return] mark task done   ";
    controlWin->print(line.str());
    line.str("");

    line << "[" << ((char) MOVE_DOWN_KEY) << "] move task down  ";
    controlWin->print(line.str());
    line.str("");

    controlWin->draw();
}

void drawTasks(TodoList* list, unsigned selected)
{
    auto tasks = list->tasks();
    
    taskWin->color(BORDER_COLOR_PAIR, true);
    taskWin->box();
    taskWin->draw(true);
    taskWin->clear();    

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
            while (selected < startTask + listOff && 
                   selected != list->size() - 1){
                listOff--;
            }
        } else{
            listOff = 0;
        }

        for (unsigned i = startTask + listOff; i < endTask + listOff; i++){
            Task t = tasks->at(i);
            
            if (showNumbers){
                taskWin->inverse();
                taskWin->color(GUTTER_COLOR_PAIR);
                
                std::string number = std::to_string(i);
                if (!zeroIndexNumbers)
                    number = std::to_string(i + 1);
                if (number.size() < 2)
                    number = "0"+number;
                
                taskWin->print(number + ")", xpos, ypos);
                taskWin->colorOff(GUTTER_COLOR_PAIR);
                taskWin->inverseOff();
                xpos += 5;
            }
            
            if (i == selected){
                std::string tmp = tasks->at(i).task();
                std::string line = tmp;
                if (highlightWholeLine){
                    for (int k = tmp.size() + xpos; k < width - 2; k++){
                        line += " ";
                    }
                }
                
                std::string mark = line.substr(0, STRING_COMPLETE.size());
                line = line.substr(mark.size());
                
                taskWin->inverse();

                taskWin->color(!t.completed() ? MARK_COLOR_PAIR
                                              : MARK_COLOR_PAIR_DONE); 
                taskWin->print(mark, xpos, ypos);
                taskWin->colorOff(!t.completed() ? MARK_COLOR_PAIR
                                                 : MARK_COLOR_PAIR_DONE);

                taskWin->color(SELECT_COLOR_PAIR);
                taskWin->print(line, xpos + mark.size(), ypos);
                taskWin->colorOff(SELECT_COLOR_PAIR);
                
                taskWin->inverseOff();
            } else{
                std::string tmp = tasks->at(i).task();
                std::string text = tmp;
                std::string mark = text.substr(0, STRING_COMPLETE.size());
                text = text.substr(mark.size());

                taskWin->color(!t.completed() ? MARK_COLOR_PAIR
                                              : MARK_COLOR_PAIR_DONE); 
                taskWin->print(mark, xpos, ypos);
                taskWin->colorOff(!t.completed() ? MARK_COLOR_PAIR
                                                 : MARK_COLOR_PAIR_DONE);

                taskWin->print(text, xpos + mark.size(), ypos);
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
    
    taskWin->draw();
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
    delwin(inputWindow);
    delwin(inputWindowBorder);
    
    delete titleWin;
    delete taskWin;
    delete controlWin;

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
