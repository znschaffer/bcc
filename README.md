# Brainf*ck Compiler (bcc)

This repository contains a simple **Brainf*ck** compiler written in C. The compiler reads a Brainf*ck source file, executes it, and outputs the result.

## Features

- **Dynamic Memory Management:** The memory used for data and instructions can dynamically resize based on the program's needs.
- **Jump List Building:** Efficiently handles `[` and `]` loops by precomputing the jump positions.
- **Debug Mode:** Provides debug output to help trace the execution (if compiled with `DEBUG` mode).

## Compilation

To compile the program, use the following command:

```bash
gcc -o bcc bcc.c
```

## Debug Mode
If you want to enable debug mode, compile with the DEBUG flag:

```bash
gcc -o bcc -DDEBUG bcc.c
```

In debug mode, the program will print each instruction and its corresponding state (instruction pointer, data pointer, and current data value).

## Usage
To run the Brainf*ck compiler, provide the path to a .bf file as a command-line argument:

```bash
./bcc filename.bf
```

## Future Improvements
Add better error handling and reporting for invalid Brainf*ck syntax.
Improve memory management efficiency.
Expand the debug output to include more detailed trace information.
