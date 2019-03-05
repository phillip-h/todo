#include "draw.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "args.hpp"
#include "config.hpp"
#include "task.hpp"
#include "todolist.hpp"
#include "typer.hpp"
#include "win.hpp"

using std::string;
using std::stringstream;

// ===================|
// internal functions |
// ===================|
std::string buildTitle(TodoList *list);

void drawTitle(TodoList* list);
void drawControls();
void drawTasks(TodoList* list, unsigned selected);
void drawDivider(string div, int ypos);

// ===================|
// internal variables |
// ===================|
Win *titleWin;
Win *taskWin;
Win *controlWin;
Win* inputWin;

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
    inHeight = std::max(inHeight, 5);

    titleWin = new Win(0, 0, COLS, titleHeight, "title", false);
    taskWin = new Win(startX, startY, width, height, "tasks");
    controlWin = new Win(inputStartX, inputStartY, COLS - inputStartX - 2, 
                         inHeight, "controls");
    inputWin = new Win(inputStartX, inputStartY, COLS - inputStartX - 2, 
                         inHeight, "new task");
}

//////////////////////
// main draw function
void Draw::draw(TodoList *list, unsigned selected)
{
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
string Draw::getInput(string str)
{
    return type(inputWin, 128, str);
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
    controlWin->clear();
    stringstream line;
    
    controlWin->move(0, 0);
    line << "[" << ((char) EXIT_KEY) << "] quit          ";
    line << "[Space] new task  ";
    line << "[Return] mark task done   ";
    line << "[Escape] cancel";
    controlWin->print(line.str());
    line.str("");

    controlWin->move(0, 1);
    line << "[" << ((char) REMOVE_KEY) << "] delete task   ";
    line << "[" << ((char) EDIT_KEY) << "] edit          ";
    line << "[" << ((char) UNDO_KEY) << "] undo                  ";
    line << "[" << ((char) DIV_KEY) << "] separate      ";
    controlWin->print(line.str());
    line.str("");

    controlWin->move(0, 2);
    line << "[" << ((char) SORT_KEY) << "] sort          ";
    line << "[" << ((char) MOVE_UP_KEY) << "] move task up  ";
    line << "[" << ((char) MOVE_DOWN_KEY) << "] move task down  ";
    controlWin->print(line.str());

    controlWin->color(BORDER_COLOR_PAIR, true);
    controlWin->draw();
}

void drawTasks(TodoList* list, unsigned selected)
{
    auto tasks = list->tasks();
    
    taskWin->clear();    

    if (tasks && tasks->size()){
        xpos = 1;
        ypos = 1;

        unsigned numTasks = height - 2;
        endTask = tasks->size();
        if (endTask > numTasks)
            endTask = numTasks;

        if (numTasks < tasks->size()){
            while (selected > (int) endTask + listOff - 2 && selected != 0)
                listOff++;
            while (selected < (int) startTask + listOff && 
                   selected != list->size() - 1)
                listOff--;
        } else{
            listOff = 0;
        }

        unsigned count = startTask + listOff;

        for (unsigned i = startTask + listOff; i < endTask + listOff &&
                                               i < tasks->size() ; i++){
            Task t = tasks->at(i);
            
            if (showNumbers){
                taskWin->inverse();
                taskWin->color(GUTTER_COLOR_PAIR);
                
                std::string number = std::to_string(count);
                if (!zeroIndexNumbers)
                    number = std::to_string(count + 1);
                while (number.size() < 2)
                    number = "0"+number;
                
                taskWin->print(number + ")", xpos, ypos);
                taskWin->colorOff(GUTTER_COLOR_PAIR);
                taskWin->inverseOff();
                xpos += 5;
            }
           
            if (i == selected){
                std::string tmp = tasks->at(i).task();
                if (tasks->at(i).div()){
                    taskWin->inverse();
                    taskWin->color(SELECT_COLOR_PAIR);
                    drawDivider(tmp.substr(5, tmp.size() - 5), ypos);
                    taskWin->colorOff(SELECT_COLOR_PAIR);
                    taskWin->inverseOff();
                    ypos++;
                    if (showNumbers){
                        xpos -= 5;
                    }
                    continue;
                }

                std::string line = tmp;
                unsigned pad = showNumbers ? 8 : 5;
                if (line.size() >= width - pad) {
                    line = line.substr(0, width - pad - 3) + "...";
                }
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
                if (tasks->at(i).div()){
                    drawDivider(tmp.substr(5, tmp.size() - 5), ypos);
                    ypos++;
                    if (showNumbers){
                        xpos -= 5;
                    }
                    continue;
                }
                
                std::string text = tmp;
                unsigned pad = showNumbers ? 8 : 5;
                if (text.size() >= width - pad) {
                    text = text.substr(0, width - pad - 3) + "...";
                }

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
                count++;
            }
        }
    }
    
    taskWin->color(BORDER_COLOR_PAIR, true);
    taskWin->draw();
}

///////////////////////////////////
// stop curses and delete windows
void Draw::stop()
{
    delete titleWin;
    delete taskWin;
    delete controlWin;
    delete inputWin;

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

    std::string stats = "[" + list->completedStr() + "]";

    for (unsigned i = title.size() + stats.size(); i < (unsigned)COLS; i++){
        title += division;
    }

    title += stats;
    return title;
}
            
void drawDivider(string div, int ypos)
{
    if (showNumbers) {
        for (unsigned i = 0; i < 3; i++) {
            div = "=" + div;
        }
    }
    while ((int) div.size() < width - 3){
        div += division;
    }
    taskWin->print(div, 1, ypos);
}

