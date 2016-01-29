#ifndef CORE
#define CORE

#include <ncurses.h>
#include <string>

#include "task.hpp"
#include "todolist.hpp"

namespace Core
{

void init();
void stop();

bool handleInput(int c, TodoList *&todoList, unsigned &position);

TodoList* list(std::string name);

}
#endif
