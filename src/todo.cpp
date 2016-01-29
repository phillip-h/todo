/////////////////////////////////////////////
// main() function and program entry point //
/////////////////////////////////////////////
#include "args.hpp"
#include "config.hpp"
#include "core.hpp"
#include "draw.hpp"

int main(int argv, char **argc)
{
    if (parseArgs(argv, argc)){
        return EXIT_NORMAL;
    }
  
    Draw::init(); 
    Core::init();

    TodoList *todoList = Core::list(name());
    unsigned position = 0;

    while(true){
        Draw::draw(todoList, position);
        if (Core::handleInput(getch(), todoList, position)){
            break;
        }
    }

    Draw::stop();
    Core::stop();

    todoList->save();
    delete todoList;

    return EXIT_NORMAL;
}
