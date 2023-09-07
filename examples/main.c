#include "cli.h"

static cli_status_t help_func(int argc, char **argv);
static cli_status_t blink_func(int argc, char **argv);

cmd_t cmd_tbl[] = {{.cmd = "help", .func = help_func}, {.cmd = "blink", .func = blink_func}};

cli_t cli;

int main(void)
{
	cli.println = user_uart_println;
	cli.cmd_tbl = cmd_tbl;
	cli.cmd_cnt = sizeof(cmd_tbl) / sizeof(cmd_t);
	cli_init(&cli);

	// enable UART receive-data interrupts here, so that UART_Rx_IrqHandler() gets
	// called when data is received

	while(1) {
		cli_process(&cli);
	}

	return 0;
}

/* For example.. */
void UART_Rx_IrqHandler()
{
	char c = UART->RxData;
	cli_put(&cli, c);
}

void user_uart_println(char *string)
{
	/* For example.. */
	HAL_UART_Transmit_IT(&huart, string, strlen(string));
}

cli_status_t help_func(int argc, char **argv)
{
	cli.println("HELP function executed");
	return CLI_OK;
}

cli_status_t blink_func(int argc, char **argv)
{
	if(argc > 0) {
		if(strcmp(argv[1], "-help") == 0) {
			cli.println("BLINK help menu");
		} else {
			return CLI_E_INVALID_ARGS;
		}
	} else {
		cli.println("BLINK function executed");
	}
	return CLI_OK;
}
