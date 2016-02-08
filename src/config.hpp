#ifndef CONFIG
#define CONFIG

#include <ncurses.h>
#include <string>

// version number
const std::string vn = "1.0_dev";

// the default name of a todo list, used if one isn't provided at runtime
const std::string DEFAULT_NAME = "todo";

// exit codes
const int EXIT_NORMAL = 0;

const bool USE_MOUSE = true;

// color settings
const bool tryColors = true;

const int TITLE_FOREGROUND = COLOR_BLUE;
const int TITLE_BACKGROUND = -1;
const int TITLE_COLOR_PAIR = 1;

const int GUTTER_FOREGROUND = COLOR_GREEN;
const int GUTTER_BACKGROUND = -1;
const int GUTTER_COLOR_PAIR = 2;

const int SELECT_FOREGROUND = COLOR_CYAN;
const int SELECT_BACKGROUND = -1;
const int SELECT_COLOR_PAIR = 3;

const int BORDER_FOREGROUND = COLOR_BLUE;
const int BORDER_BACKGROUND = -1;
const int BORDER_COLOR_PAIR = 4;

const int MARK_FOREGROUND = COLOR_RED;
const int MARK_BACKGROUND = -1;
const int MARK_COLOR_PAIR = 5;

const int MARK_FOREGROUND_DONE = COLOR_GREEN;
const int MARK_BACKGROUND_DONE = -1;
const int MARK_COLOR_PAIR_DONE = 6;

// various settings for how the todo list is displayed
const bool showNumbers = true;
const bool zeroIndexNumbers = true;
const bool highlightWholeLine = true;
const std::string STRING_NOT_COMPLETE = "[ ]";
const std::string STRING_COMPLETE = "[x]";

// string used as a divider in various places
const std::string division = "=";

// char used to designate a divider
const char DIV_CODE = 0x07;

// settings for the size and location of the various windows
// the width of windows will be set using their startX to ensure
// they are centered.
const int listStartX = 2;
const int listStartY = 6;
const float listHeight = 0.75f;

const int inputStartX = 2;
const int inputStartY = 2;
const float inputHeight = 0.1f;

const int titleHeight = 1;

// size of the char buffer used for creating a new task
const unsigned charBufferSize = 200;

// keybinds
const int EXIT_KEY = 'q';
const int DOWN_KEY = KEY_DOWN;
const int UP_KEY = KEY_UP;
const int TOGGLE_CHECK_KEY = '\n';
const int NEW_KEY = ' ';
const int REMOVE_KEY = 'd';
const int MOVE_UP_KEY = 'k';
const int MOVE_DOWN_KEY = 'j';
const int SORT_KEY = 's';
const int EDIT_KEY = 'e';
const int UNDO_KEY = 'u';
const int DIV_KEY = 'z';

// settings related to saving
const std::string saveDirectory = ".todo";
const std::string extension = ".todo";
const bool autoSave = true;

#endif
