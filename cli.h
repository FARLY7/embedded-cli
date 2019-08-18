#ifndef _CLI_H_
#define _CLI_H_

#include <stdint.h>
#include <stddef.h>

#include "queue.h"


#define CLI_RX_BUF_SIZE     128U
#define CLI_TX_BUF_SIZE     128U


typedef enum
{
    CLI_OK,
    CLI_ERROR
} cli_status_t;

typedef struct 
{
    char *name;
    void *func;
} cli_cmd_t;


typedef void (*usr_uart_tx_fptr_t)(uint8_t *buf, size_t len);


typedef struct
{        
    queue_t rx_q;
    queue_t cmd_q;

    usr_uart_tx_fptr_t send;

} cli_handle_t;


void cli_rx_IrqHandler(uint8_t data);

void cli_put(uint8_t byte);


cli_status_t cli_process(cli_handle_t *cli);

cli_status_t cli_init(cli_handle_t *cli, cli_cmd_t cmds, size_t cmd_cnt);

cli_status_t cli_register_command(cli_handle_t *cli);





#endif