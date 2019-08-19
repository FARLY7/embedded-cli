#include "cli.h"


static void help_func(int argc, char **argv);
static void blink_func(int argc, char **argv);

cmd_t cmd_tbl[] = {
    {
        .cmd = "help",
        .func = help_func
    },
    {
        .cmd = "blink",
        .func = blink_func
    }
};

cli_t cli;

int main(void)
{
    cli.println = user_uart_println;
    cli.cmd_tbl = cmd_tbl;
    cli.cmd_cnt = sizeof(cmd_tbl);
    cli_init(&cli);

    while(1)
    {
        cli_process(&cli);
    }

    return 0;
}


void user_uart_println(char *string)
{
    printf(string);
}

void help_func(int argc, char **argv)
{
    cli.println("HELP function executed");
}

void blink_func(int argc, char **argv)
{
    if(argc > 0)
    {
        if(strcmp(argv[1], "-help") == 0)
        {
            cli.println("BLINK help menu");
        }
    }
    else
    {
        cli.println("BLINK function executed");
    }
}