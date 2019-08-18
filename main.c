#include "cli.h"


cli_cmd_t cmds[2] = {
    {
        .name = "help",
        .func = help_func
    },
    {
        .name = "echo",
        .func = echo_func
    }
};

int main(void)
{
    cli_handle_t cli;

    cli_init(&cli, cmds, sizeof(cmds));


    cli_register_cmd(&cmds[0]);   
    cli_register_cmd(&cmds[1]);


    return 0;
}

void help_func()
{

}

void echo_func()
{

}