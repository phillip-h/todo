///////////////////////////////////
// parse command line arguments  //
// and act on them if necessary  //
///////////////////////////////////
#include "args.hpp"

#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <vector>

#include "config.hpp"
#include "core.hpp"
#include "task.hpp"
#include "todolist.hpp"

using std::cin;
using std::cout;
using std::string;
using std::vector;

// ===================|
// internal functions |
// ===================|
unsigned getTermWidth();

vector<string> listFiles(string path);

void printLists();
void printTodo();
void printUsage();
void printVersion();

void removeList();
void renameList();

// ===================|
// internal variables |
// ===================|
string todoName = DEFAULT_NAME;

bool colors_ = tryColors; 
string ANSI_RESET = "\u001B[0m";
string ANSI_RED = "\u001B[1;31m";
string ANSI_GREEN = "\u001B[1;32m";
string ANSI_BLUE = "\u001B[1;34m";
string ANSI_WHITE = "\u001B[1;37m";

//////////////////////////////////////////////
// parse and act on program arguments
// returns true if the program should exit, 
// false if it should continue
bool parseArgs(int argv, char **argc)
{
    if (argv <= 1){
        return false;
    }

    for (int i = 1; i < argv; i++){
        if (string(argc[i]) == "-h" || 
            string(argc[i]) == "-help" || 
            string(argc[i]) == "--help"){           
            
            printUsage();
            return true;
        } 
        
        else if (string(argc[i]) == "-version" ||
                 string(argc[i]) == "--version"){
            
            printVersion();
            return true;
        }

        else if (string(argc[i]) == "-r" ||
                 string(argc[i]) == "--rename"){
            
            renameList();
            return true;
        }

        else if (string(argc[i]) == "-d" ||
                 string(argc[i]) == "--delete"){
            
            removeList();
            return true;
        }

        else if (string(argc[i]) == "-l" ||
                 string(argc[i]) == "--list"){
            
            printLists();
            return true;
        }

        else if (string(argc[i]) == "-p" ||
                 string(argc[i]) == "--print"){
            
            printTodo();
            return true;
        }

        else if(string(argc[i]) == "-nc" ||
                string(argc[i]) == "--no-color"){
            ANSI_WHITE = ANSI_BLUE = ANSI_GREEN 
            = ANSI_RED = ANSI_RESET = "";
            colors_ = false;
        }

        else if (i != 1){
            cout << ANSI_RED;
            cout << "unknown argument \"" << argc[i] << "\"\n";
            cout << ANSI_RESET;
            cout << "run ";
            cout << ANSI_BLUE;
            cout << "todo --help "; 
            cout << ANSI_RESET; 
            cout << "for more information\n";
            return true;
        } else {        
            todoName = string(argc[1]);
        }
    }
    
    return false; 
}

/////////////////////////////////////////
// return the todolist name if provided,
// or the default name if it was not 
string name()
{
    return todoName;
}

////////////////////////////////////
// return if colors should be used
bool useColors()
{
    return colors_;
}

// ===================|
// internal functions |
// ===================|

//////////////////////////////////
// get the width of the terminal
unsigned getTermWidth()
{
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    return w.ws_col;
}
///////////////////////////////////////////
// return a vector of all todo files in 
// a directory ommits files starting with 
// "." to exclude "." and ".."
vector<string> listFiles(string dir)
{
    auto list = std::vector<std::string>();

    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL){
        cout << ANSI_RED << "Could not find directory \"";
        cout << dir << "\" !\n";
        cout << ANSI_RESET;
    }

    while ((dirp = readdir(dp)) != NULL){
        if (dirp->d_name[0] != '.'){
            string name(dirp->d_name);
            if (name.size() > extension.size()){
                name = name.substr(0, name.size() - extension.size());
            }
            if (!name.empty()){
                list.push_back(name);
            }
        }
    }
    closedir(dp);
    
    return list;
}

/////////////////////////
// print all to-do lists
void printLists()
{
    auto list = listFiles(TodoList::getSaveDir());

    cout << ANSI_BLUE << "to-do lists:\n" << ANSI_RESET;
    cout << "\n";

    unsigned count = 0;
    const unsigned width = getTermWidth();
    
    for (unsigned i = 0; i < list.size(); i++){
        if (count + list.at(i).length() + 10 >= width){
            count = 0;
            cout << "\n";
        }
        cout << ANSI_GREEN << list.at(i) << "  ";
        count += list.at(i).length() + 2;
    }

    cout << ANSI_RESET << "\n";
}

/////////////////////////////////////
// print the to-do list to std::cout
void printTodo()
{
    TodoList* list = Core::list(name());
    if (!list->exists()){
        cout << ANSI_RED << "To-do list '" << list->name;
        cout << "' does not exist!\n" << ANSI_RESET;
        return;
    }

    auto tasks = list->tasks();

    cout << ANSI_BLUE << "to-do -- " << list->name;
    cout << ANSI_RESET << "\n";
    for (unsigned i = 0; i < tasks->size(); i++){
        string taskStr = tasks->at(i).save();
        taskStr = taskStr.substr(0, taskStr.size() - 2);
        cout << i << ". " << taskStr<< "\n";
    }
}

/////////////////////////////////
// print program usage and exit
void printUsage()
{
    cout << ANSI_BLUE << "todo version " << vn << ANSI_RESET << "\n";
    cout << "\n";
    cout << ANSI_BLUE << "syntax" << ANSI_RESET << "\n";
    cout << ANSI_WHITE << "todo [list name] [arguments]";
    cout << ANSI_RESET << "\n";
    cout << "If no name is provided, '" << DEFAULT_NAME;
    cout << "' will be used.\n"; 
    cout << "\n";
    cout << ANSI_BLUE << "arguments" << ANSI_RESET << "\n";
    cout << "-h, -help, --help   -> prints usage and exits\n";
    cout << "-version, --version -> prints version information and exits\n";
    cout << "\n";
    cout << "-d, --delete    -> delete the to-do list\n";
    cout << "-l, --list      -> list all to-do lists and exit\n";
    cout << "-nc, --no-color -> disable colored output\n";
    cout << "-p, --print     -> print the to-do list to the console";
    cout << " and exit\n";
    cout << "-r, --rename    -> rename the to-do list\n";
    cout << "\n";
}

//////////////////////////////////////
// print program version information 
// and exit
void printVersion()
{
    cout << ANSI_BLUE << "todo version " << vn << ANSI_RESET << "\n";
    cout << ANSI_BLUE << "(c) phillip-h 2015" << ANSI_RESET << "\n";
}

////////////////////////////
// remove a to-do list file
void removeList()
{
    TodoList *list = Core::list(name());
    if (!list->exists()){
        cout << ANSI_RED << "Todo List '" << list->name;
        cout << "' does not exist!\n" << ANSI_RESET;
        return;
    }

    string choice;
    cout << "Remove list? [Y/n] > ";
    cin >> choice;
    if (choice != "Y"){
        cout << "Not removing.\n";
        return;
    }

    if (remove(list->getPath().c_str()) != 0){
        cout << ANSI_RED << "Failed to remove list!\n" << ANSI_RESET;
        return;
    }
    cout << "List removed.\n";
}

///////////////////////
// rename a to-do list
void renameList()
{
    TodoList *list = Core::list(name());
    if (!list->exists()){
        cout << ANSI_RED << "Todo List '" << list->name;
        cout << "' does not exist!\n" << ANSI_RESET;
        return;
    }
    
    string newName;
    cout << "Enter new list name, or an empty line to cancel > ";
    getline(cin, newName);
    if (newName.empty()){
        cout << "Canceling rename.\n";
        return;
    }
    
    if (remove(list->getPath().c_str()) != 0){
        cout << ANSI_RED << "Failed to remove list!\n" << ANSI_RESET;
        return;
    }
    
    string oldName = list->name;
    list->name = newName;
    if (list->exists()){
        cout << ANSI_RED << "List already exists!\n" << ANSI_RESET;
        list->name = oldName;
    } else{
       cout << "List renamed.\n"; 
    }
    
    list->save();    
}
