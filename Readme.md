# File Manager (wxWidgets + C++17)

## Author
Shuyun Zheng  
Jan 2026

---

## Overview

This project implements a **file manager** using **C++17** and **wxWidgets**.  
It allows users to browse directories and perform basic file operations such as opening, copying, moving, renaming, and deleting files through a graphical interface.

The design focuses on:
- Clear separation between **UI logic** and **file system operations**
- Error handling
- A user-friendly interface following typical desktop file manager behavior

---

## Features

### Directory Browsing
- Display files and directories in the current folder
- Shows file properties:
  - Name
  - Type (File / Directory)
  - Size
  - Last modified time
- Supports navigation by user input
- Supports navigating to the parent directory (`..`)
- Refresh current directory list
- Status bar

### File Operations
All operations are accessible through menu items and keyboard shortcuts:

- **Open**
  - Opens files using the system’s default application
  - Navigates into directories when selected
- **Create Directory**
  - Prompts the user for a new directory name
- **Rename**
  - Prompts for a new name
  - Supports overwrite confirmation
- **Delete**
  - Prompts the user before deleting files or directories
- **Copy / Cut / Paste**
  - Implements a virtual clipboard
  - Supports recursive copy for directories
  - Prompts before overwriting existing files
- **Refresh**
  - Reloads the current directory listing
- **Exit**
  - Closes the application safely

## Build Instructions

This program was developed and tested on a Unix-like environment with wxWidgets installed.

### Requirements

C++17 compatible compiler (clang++ or g++)

wxWidgets (version 3.2 or newer recommended)

wx-config available in your PATH

### Compile

- From the project directory, run:

make

This will compile all source files and produce the executable.

- If wxWidgets is not found, make sure wx-config is installed and accessible:

wx-config --version

---

## Architecture

The project is structured into two main components:

### 1. `MainFrame`
Responsible for:
- Creating and managing the GUI
- Handling user input and events
- Displaying directory contents
- Coordinating file operations

Key responsibilities:
- Path bar handling
- File list display
- Menu command handling
- Clipboard state management

### 2. `FileOp`
A utility class that encapsulates all filesystem operations using `std::filesystem`.

Responsibilities:
- File and directory existence checks
- Creation, deletion, renaming
- Recursive copy and move operations
- Centralized error handling via `std::error_code`

This separation keeps UI logic independent from filesystem logic.

---

## Technologies Used

- **Language:** C++17
- **GUI Library:** wxWidgets
- **Filesystem API:** `std::filesystem`
- **Build Tool:** Make / clang++

---

## Build & Run

### Requirements
- wxWidgets (built with C++17 support)
- clang++ or g++
- Unix-like environment (Linux, macOS, or WSL)

### Build
```bash
make
