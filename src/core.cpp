#include "core.hpp"

#include <ncurses.h>
#include <string>

#include "config.hpp"
#include "draw.hpp"
#include "todolist.hpp"

using std::string;

// ===================|
// internal variables |
// ===================|
TodoList *backup_;

// ===================|
// internal functions |
// ===================|


///////////////////
// backup todolist
void backup(TodoList *list)
{
    delete backup_;
    backup_ = new TodoList(list);
}

////////////////////////////////
// restore todo list from backup
void restore(TodoList *&list)
{
    if (!backup_)
        return;
    delete list;
    list = new TodoList(backup_);
}

//////////////////////////////////////////////
// initiate things such as the mouse listener
void Core::init()
{
    if (USE_MOUSE){
        mousemask(ALL_MOUSE_EVENTS, NULL);
    }
    backup_ = nullptr;
}

///////////////////////////////////////////
// cleanup everything and prepare to exit
void Core::stop()
{
    delete backup_;
}

//////////////////////////////////
// create and load the todo list
TodoList* Core::list(string name)
{
    TodoList *list = new TodoList(name);
    list->load();
    return list;
}

//////////////////////////////////////////
// intercept keyboard input and act on it
// returns true if the program should exit
// and false if it should continue
bool Core::handleInput(int c, TodoList *&todoList, unsigned &position)
{
    std::string newTask;
    
    switch(c){
        case EXIT_KEY:
            return true;
        
        case TOGGLE_CHECK_KEY:
            if (todoList->size() != 0){
                todoList->at(position).toggleComplete();
            }
            break;
        
        case NEW_KEY:
            backup(todoList);
            newTask = Draw::getInput();
            if (!newTask.empty()){
                todoList->add(Task(newTask));
            }
            break;
       
        case EDIT_KEY:
            {
            backup(todoList);
            string base = todoList->at(position).raw();
            string editTask = Draw::getInput(base);
            if (!editTask.empty())
                todoList->at(position) = Task(editTask);
            break;
            }

        case REMOVE_KEY:
            backup(todoList);
            todoList->remove(position);
            if (todoList->size() != 0){
                if (position >= todoList->size()){
                    position = todoList->size() - 1;
                }
            }
            break;

        case UNDO_KEY:
            restore(todoList); 
            break;

        case MOVE_UP_KEY:
            if (todoList->swap(position - 1)){
                --position;
                if (autoSave){
                    todoList->save();
                }
            }
            break;

        case MOVE_DOWN_KEY:
            if(todoList->swap(position)){
                ++position;
                if (autoSave){
                    todoList->save();
                }
            }
            break;

        case UP_KEY:
            if (position != 0){
                --position;
            }
            break;
        
        case DOWN_KEY:
	        if (todoList->size() != 0){
                ++position;
                if (position >= todoList->size()){
                    position = todoList->size() - 1;
                }
	        }
	        break;

        case SORT_KEY:
            backup(todoList);
            todoList->sort();
            if (autoSave){
                todoList->save();
            }
            break;

        case KEY_RESIZE:
            Draw::stop();
            Draw::init();
            break;
    
        case KEY_MOUSE:
            MEVENT event;
            if (getmouse(&event) == OK){ 
                if (event.bstate & BUTTON1_PRESSED){
                    Draw::mouse(event, todoList, position, false);  
                } else if (event.bstate & BUTTON3_PRESSED){
                    Draw::mouse(event, todoList, position, true);  
                } else if (event.bstate & BUTTON4_PRESSED){
                    if (position != 0){
                        --position;
                    }
                }
            } else {          // ncurses is weird and returns ERR
                ++position;   // on mouse wheel down events
                if (position >= todoList->size()){
                    position = todoList->size() - 1;
                }
            }
            break;
    }
    
    return false;
}
