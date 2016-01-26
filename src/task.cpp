////////////////////////////////////////
// representation of a todo list item //
////////////////////////////////////////
#include "task.hpp"

#include "config.hpp"

using std::string;

Task::Task(string name)
{
    this->name = name;
    complete = false;
}

///////////////////////////
// toggle task completion
void Task::toggleComplete()
{
    complete ^= true;
}

///////////////////////////////////////
// get a formatted string of this task
string Task::task()
{
    string value = "";
    
    if (complete){
        value += STRING_COMPLETE;
    } else{
        value += STRING_NOT_COMPLETE;
    }
    
    value += " " + name;
    
    return value;
}

//////////////////////////////////////
// get a string of this task to write
// to a file
string Task::save()
{
    string value = "";
    
    value += name;
    value += ":";
    
    value += std::to_string(complete);
    
    return value;
}

////////////////////////////////////////
// parse the task from the saved string
Task Task::parse()
{
    for (unsigned i = 0; i < name.size() - 1; i++){
        if (name.substr(i, 1) == ":"){
            if (name.substr(i + 1, 1) == "1"){
                complete = true;
            }
            
            name = name.substr(0, i);
            return *this;
        }
    }
    
    return *this;
}

////////////////////////////////
// return if the task has been 
// completed or not
bool Task::completed() const
{
    return complete;
}

///////////////////////////////////
// compare a task to another task
bool Task::cmp(const Task &a, const Task &b)
{
    if (a.completed() != b.completed()){
        return a.completed() < b.completed();
    }
    return a.name.compare(b.name) < 0;
}
