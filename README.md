# todo [![Build Status](https://travis-ci.org/phillip-h/todo.svg?branch=master)](https://travis-ci.org/phillip-h/todo)
A program for CLI todo lists, written using `ncurses`!

![todo running in gnome-terminal](images/todo.png)

todo running in gnome-terminal

# installation

Clone and run `make` to build. Run `todo` to launch the program. Requires `ncurses`. On debian-like systems, such as ubuntu, ncurses can be installed through `sudo apt-get install libncurses-dev`.

You can then run `make install` to copy the executable to `/usr/local/bin`.

If you wish to uninstall the application, run `make remove`

This program supports bash autocompletion through `completion/todoc`.
This file will be copied to `/etc/bash_completion.d` when `make
install` is run, and can be removed with `make remove`.

# usage

The up and down arrow keys can be used to navigate through the list.

Use the space key to start creating a new task. Once the space key
has been pressed, a input window will appear, enter your text and
press return to finish. Hitting return without entering text
will cancel adding a new task, as will the escape key.

A Task can be edited by selecting it and hitting "E". This will display
the input box with the current task text prefilled. You can then modify it
as you see fit, and save it with the return key. If the escape key is
hit or you save an empty task, the task will not be edited.

The return key will toggle task completion, and the "D" key will
remove the selected task from the list.

The "K" and "J" keys will move the selected task up and down in the
list, respectivly.

The "Q" key will exit the program.

If your terminal supports it, the mouse can also be used to interact
with the program. Left click will highlight the task under the
cursor, and right click will toggle the task under the cursor. The
mouse wheel can be used to scroll up and down in the list. Note that
mouse scrolling, particulary down scrolling, is a bit finicky and may
not work with your terminal.

!! Experimental - pressing 'S' will sort the todo list by name and
alphabetical order. There is not currently a way to undo the sort 

Todo lists are saved to `~/.todo`. By default, the list will be saved
any time it is edited. If you wish to disable this for some reason,
simply change `autoSave` to `false` in `config.hpp` and recompile.

# syntax

`todo [list name] [arguments]`

If no list name is provided, the default one will be used. This is "todo" unless you changed it.

# arguments

* -h, -help, --help -> prints usage and exits

* -version, --version -> prints version information and exits



* -d, --delete   -> delete the todo list

* -l, --list     -> list all todo lists and exit

* -n, --no-color -> disable colored output

* -p, --print    -> print the todo list to the console and exit

* -r, --rename   -> rename the todo list

# configuration

Don't like how something works or looks? You can probably change it in `config.hpp`. Just change and rebuild
