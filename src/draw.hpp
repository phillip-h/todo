#ifndef DRAW
#define DRAW

#include <ncurses.h>
#include <string>
#include <vector>

class Todo;
class TodoList;

namespace Draw 
{

void init();
void stop();

void draw(TodoList *list, unsigned selected);
std::string getInput(std::string str = "");

void mouse(MEVENT event, TodoList *list, unsigned &selected, bool button);

}
#endif
