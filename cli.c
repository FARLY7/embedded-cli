#include "cli.h"



uint8_t rx_byte_buf[128];
uint8_t rx_cmd_buf[128];

queue_t rx_byte_queue;
queue_t rx_cmd_queue;



cli_cmd_t *user_cmds;
size_t user_cmd_cnt;

cli_status_t cli_init(cli_handle_t *cli, cli_cmd_t *cmds, size_t cmd_cnt)
{
    queue_init(rx_byte_buf, 128, &rx_byte_queue);
    queue_init(rx_cmd_buf, 128, &rx_cmd_queue);

    cmds = _cmds;
    cmd_cnt = _cmd_cnt;
}

cli_status_t cli_register_command(cli_handle_t *cli)
{


}

void cli_put(uint8_t byte)
{
    queue_put(&rx_byte_queue, byte);

    if(byte == '\n')
    {
        uint8_t data;
        while(queue_get(cli->rx_queue, &data) == 0)
        {
            queue_put(cli->cmd_queue, &data);
            if(data == '\n')
            {
                break;
            }
        }
    }


}

void cli_rx_IrqHandler(uint8_t data);

cli_status_t cli_process(cli_handle_t *cli);

