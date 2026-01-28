# File Manager (C++17 + wxWidgets)

**Author:** Shuyun Zheng  
**Date:** January 2026  

---

## Overview

This project is a simple file manager implemented in **C++17** using the
**wxWidgets** graphical framework.  
It allows users to browse directories and perform basic file operations
through a graphical user interface.

The purpose of this project is to gain experience with:
- Object-oriented programming in C++
- Using the C++17 `std::filesystem` library
- Developing GUI applications with wxWidgets
- Working in a Unix/Linux-style development environment

---

## Functionality

### Directory Browsing

- Displays files and directories in the current folder
- Shows the following file properties:
  - Name
  - Type (File or Directory)
  - Size
  - Last modified time
- The current directory path is shown in a text bar
  - The path bar is editable and can be used to change directories
- Supports navigation by:
  - Double-clicking directories
  - Typing a path directly into the path bar
- Includes a parent directory entry (`..`)
- Status bar displays the current path and operation messages

---

### File Operations

All operations are accessible through the menu bar and keyboard shortcuts.

- **Open**
  - Opens files using the system’s default application
  - If a directory is selected, navigates into that directory

- **Create Directory**
  - Prompts the user to enter a new directory name

- **Rename**
  - Prompts the user for a new name
  - Requests confirmation if the destination already exists

- **Delete**
  - Prompts the user for confirmation before deleting files or directories

- **Copy / Cut / Paste**
  - Implements a virtual clipboard (not the system clipboard)
  - Supports both files and directories
  - Directory copy is recursive
  - Prompts before overwriting existing files
  - Clears the clipboard after a successful paste

- **Refresh**
  - Reloads the contents of the current directory

- **Exit**
  - Closes the application

## Build Instructions
This program was developed and tested on a Unix-like environment with wxWidgets installed.

### Requirements
- C++17 compatible compiler (clang++ or g++)
- wxWidgets (version 3.2 or newer recommended)
- wx-config available in your PATH

### Compile
- From the project directory, run:
  make
  This will compile all source files and produce the executable.
- If wxWidgets is not found, make sure wx-config is installed and accessible:
  wx-config --version

### Testing Environment
- Tested on macOS
- wxWidgets version 3.3
- Compiled using clang++ with -std=c++17

---

### Known Limitations
- No support for multiple selection operations
- No drag-and-drop support
- Limited error recovery for certain filesystem permission errors
