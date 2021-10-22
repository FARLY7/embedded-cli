#include "cli.h"


static cli_status_t help_func(int argc, char **argv);
static cli_status_t blink_func(int argc, char **argv);

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
    cli.cmd_cnt = sizeof(cmd_tbl)/sizeof(cmd_t);
    cli_init(&cli);

    while(1)
    {
        cli_process(&cli);
    }

    return 0;
}


cli_status_t user_uart_println(char *string)
{
    printf(string);
    return SLI_OK;
}

cli_status_t help_func(int argc, char **argv)
{
    cli.println("HELP function executed");
    return CLI_OK;
}

cli_status_t blink_func(int argc, char **argv)
{
    if(argc > 0)
    {
        if(strcmp(argv[1], "-help") == 0)
        {
            cli.println("BLINK help menu");
        }
        else
        {
            return CLI_E_INVALID_ARGS;
        }
    }
    else
    {
        cli.println("BLINK function executed");
    }
    return CLI_OK;
}