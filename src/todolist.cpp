#include "todolist.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "config.hpp"
#include "task.hpp"

using std::string;
using std::vector;

TodoList::TodoList(string name)
{
    this->name = name;
    taskList = new vector<Task>();
}

TodoList::TodoList(TodoList *base)
{
    this->name = base->name;
    taskList = new vector<Task>(*base->taskList);
}

TodoList::~TodoList()
{
    delete taskList;
}

//////////////////////////////////
// write the todo list to a file
void TodoList::save()
{
    mkdir(getSaveDir().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::ofstream file(getPath());

    if (!file.good()){
        std::cout << "failed to write to-do list!\n";
        return;
    }

    for (unsigned i = 0; i < size(); i++){
        file << at(i).save() << "\n";
    }

    file.close();
}

///////////////////////////////////
// try to read the todo list from
// a file
void TodoList::load()
{
    if (!exists()){
        return;
    }

    std::ifstream file(getPath());
    string line;
    
    while(std::getline(file, line)){
        add(Task(line).parse());
    }
}

////////////////////////////////////////
// return true if the todo list exists
// on the disk
bool TodoList::exists()
{
  struct stat buffer;
  return (stat (getPath().c_str(), &buffer) == 0);
}

/////////////////////////////////
// return a vector of all the 
// tasks in the list
vector<Task>* TodoList::tasks()
{
    return taskList;
}

///////////////////////////////////
// return a reference to the task 
// at the specific index
Task& TodoList::at(int position)
{
    return taskList->at(position);
}

//////////////////////////////////
// return the number of tasks in 
// the todo list
unsigned TodoList::size()
{
    return taskList->size();
}

///////////////////////////////////
// return the number of completed
// tasks in the todo list
unsigned TodoList::completed()
{
    unsigned count = 0;
    for (unsigned i = 0; i < size(); i++){
        if (at(i).completed()){
            count++;
        }
    }

    return count;
}

////////////////////////////////////
// add a new task to the todo list
void TodoList::add(Task task)
{
    taskList->push_back(task);
    
    if (autoSave){
        save();
    }
}

/////////////////////////////////////
// remove a task from the todo list
void TodoList::remove(unsigned position)
{
    if (taskList->size() > position){
        taskList->erase(taskList->begin() + position);
    }
    if (autoSave){
        save();
    }
}

///////////////////////////////////////////////
// swap the element at pos and the element at
// pos + 1, if possible
// returns true if the swap occured, and false
// if it did not
bool TodoList::swap(unsigned pos)
{
    if (pos >= size() - 1){
        return false; 
    }
    Task temp = taskList->at(pos);
    taskList->at(pos) = taskList->at(pos + 1);
    taskList->at(pos + 1) = temp;
    return true;
}

/////////////////////////////
// return the save directory
string TodoList::getSaveDir()
{
    return string(getenv("HOME"))+"/"+saveDirectory;
}

///////////////////////////
// sort the todo list
void TodoList::sort()
{
    std::sort(taskList->begin(), taskList->end(), Task::cmp);
}

///////////////////////////////////////////
// return the save path for the todo list
string TodoList::getPath()
{
    return string(getenv("HOME"))+"/"+saveDirectory+"/"+name+extension;
}
