#ifndef TASK
#define TASK

#include <string>

// representation of a todo list item
class Task
{

private:
    std::string name;
    bool complete;

public:
    Task(std::string name);

    static bool cmp(const Task &a, const Task &b); 

    void toggleComplete();
    bool completed() const;
    bool div();
    std::string task();
    std::string raw();

    std::string save();
    Task parse();

};

#endif
