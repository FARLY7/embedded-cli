# cli-embedded
A simple command-line interface for use in embedded systems.
This useful tool allows a user to remotely invoke functions on their device by specifing commands (and parameters) over a byte stream protocol.

## Features
* Remotely invoke functions on device.
* Specify fucntion parameters.
* No dynamic memory allocation.
* Backspace to remove unintentional keypresses.

## Introduction
This package contains files to implement a simple command-line interface.
The package includes cli.h, and cli.c.

## Integration details
* Integrate cli.h and cli.c files into your project.
* Include the cli.h header file in your code like below.

```c
#include "cli.h"
```

## File information
* cli.h : This header file contains the definitions of the cli user API.
* cli.c : This source file contains the implementation of the CLI.

## Supported interfaces
* Typically, UART.
* .. Any byte-stream based interface.
  
## User Guide

### Initialising the CLI
To correctly set up the CLI, the user must do four things:

1. Create a table of commands which are to be accepted by the CLI, using the cmd_t structure.
    Note: command functions must use the ```cli_status_t (*func)(int argc, char **argv)``` definition.
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
    .cmd_cnt = sizeof(cmds)
};

if((rslt = cli_init(&cli)) != CLI_OK)
{
    printf("CLI: Failed to initialise);
}
```

4. Periodically call the ```cli_process()``` function in order to process incoming commands.

