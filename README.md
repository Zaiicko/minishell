# Minishell  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ![42 logo little](https://github.com/user-attachments/assets/4cecf5b8-a28e-49c5-a4b1-2030e571a0b3)

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

## Introduction

**Minishell** is a Unix shell recreation project written in C, developed as the **first group project (duo)** at **42 School**
The goal is to design a minimal Bash-like shell, handling user input, executing commands, and managing the environment using low-level system calls.  

This project introduces essential topics such as:
- Process creation and synchronization
- File descriptors and redirection
- Pipes
- Signal handling
- Terminal interaction and input parsing

📚 [GNU Bash Manual (Reference)](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html)

---

## Features

### Command Execution
- Parses and executes commands using `execve`.
- Supports **absolute**, **relative**, and **PATH-based** executables.
- Internal handling of built-in commands (no child process spawned).

### Built-in Commands
- `echo [-n]`
- `cd [dir]`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### Input Parsing
- Handles `'single quotes'` and `"double quotes"`.
- Expands **environment variables** like `$USER` or `$?`.

### Redirections
- `<` standard input  
- `>` standard output (overwrite)  
- `>>` standard output (append)  
- `<<` heredoc support

### Pipes
- Fully supports pipelines (`|`) chaining multiple commands.

### Signal Handling
- `Ctrl-C`: resets prompt
- `Ctrl-D`: exits shell
- `Ctrl-\`: ignored in interactive mode

### Global Variable Policy
- Only **one global variable** allowed (to track signal values).

---

## Usage

### 1. Build the project

```bash
make
```

### 2. Launch the shell

```bash
./minishell
```

### 3. Example session

```bash
$> echo "Hello, world!"
Hello, world!

$> export MOOD=calm
$> echo "Today I feel $MOOD"
Today I feel calm

$> cat << EOF
> This is a heredoc input
> EOF
This is a heredoc input
```

---

## Team Members

- [**@SoleQz**](https://github.com/SoleQz) — execution
- [**@me**](https://github.com/Zaiicko) — parsing  

---

## Valgrind Suppression

This project uses the **readline** library, which causes known false-positive memory leaks in Valgrind.  
A suppression file (`supp.supp`) is included to ignore these.

### 🔧 Usage

```bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=./supp.supp ./minishell
```

---

## Technical Notes

- Compliant with the **42 coding norm**
- Uses the `readline` library for input and history
- Memory must be managed strictly (except `readline` leaks)
- Only **one global variable**
- Makefile includes: `all`, `clean`, `fclean`, `re`, and `bonus`

---

## Screenshot

<img width="218" alt="Capture d’écran 2024-08-02 à 02 34 22" src="https://github.com/user-attachments/assets/7d935ac1-ba16-49bc-b2db-62a40ce13e83" />

