#ifndef TODOLIST
#define TODOLIST

#include <string>
#include <vector>

class Task;

// representation of a todo list
class TodoList{
private:
     std::vector<Task> *taskList;
 
public:
    std::string name;

    static std::string getSaveDir();

    TodoList(std::string name);
    TodoList(TodoList *base);
    ~TodoList();

    bool exists();
    void save();
    void load();

    std::vector<Task>* tasks();
    
    unsigned size();
    unsigned completed();
    
    Task& at(int position);
    
    void add(Task task);
    void remove(unsigned position);

    bool swap(unsigned pos);

    void sort();

    std::string getPath();

};

#endif
