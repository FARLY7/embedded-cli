# cli-embedded

A simple command-line interface for use in embedded systems.
This useful tool allows a user to remotely invoke functions on their device by specifing commands (and parameters) over a byte stream protocol.

## Features

- Remotely invoke functions on device.
- Ability to process function parameters.
- Statically allocated memory.
- Backspace to remove unintentional keypresses.

## Introduction

This package contains files to implement a simple command-line interface.
The package includes cli.h, and cli.c.

## Integration details

- Integrate cli.h and cli.c files into your project.
- Include the cli.h header file in your code like below.

```c
#include "cli.h"
```

## File information

- cli.h : This header file contains the definitions of the cli user API.
- cli.c : This source file contains the implementation of the CLI.

## Supported interfaces

- Typically, UART.
- .. Any byte-stream based interface.

## Integration Guide

### Initialising the CLI

To correctly set up the CLI, the user must do four things:

1. Create a table of commands which are to be accepted by the CLI, using the cmd_t structure.

**Note**: Command functions must use the `cli_status_t (*func)(int argc, char **argv)` definition.

```c
cmd_t cmds[2] = {
    {
        .cmd = "help",
        .func = help_func
    },
    {
        .cmd = "echo",
        .func = echo_func
    }
};
```

2. Place the cli_put() function within the devices interrupt handler responsible for receiving 1 byte over the communication protocol.

```c
void UART_Rx_IrqHandler()
{
    char c = UART->RxData;
    cli_put(&cli, c);
}
```

3. Create an instance of the CLI handle structure, and fill in the required parameters.

```c
cli_status_t rslt = CLI_OK;

cli_t cli = {
    .println = user_uart_println,
    .cmd_tbl = cmds,
    .cmd_cnt = sizeof(cmds) / sizeof(cmd_t)
};

if((rslt = cli_init(&cli)) != CLI_OK)
{
    printf("CLI: Failed to initialise");
}
```

4. Periodically call the `cli_process()` function in order to process incoming commands.

## User Guide

To interface with the CLI, the user must open a communication stream on their chosen protocol (typically UART).
The default end-of-delimiter used by the application is '\r', however this can be changed.
The user can invoke their functions by sending:
`echo <param>\r`

- echo, the name of the command
- <param>, first parameter (if required).

## Function templates

```c
void user_uart_println(char *string)
{
    /* For example.. */
    HAL_UART_Transmit_IT(&huart, string, strlen(string));
}

cli_status_t help_func(int argc, char **argv)
{
    cli_status_t rslt = CLI_OK;

    /* Code executed when 'help' is entered */

    return rslt;
}

cli_status_t echo_func(int argc, char **argv)
{
    cli_status_t rslt = CLI_OK;

    /* Code executed when 'echo' is entered */

    return rslt;
}
```
