/*
 * MIT License
 * 
 * Copyright (c) 2019 Sean Farrelly
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * File        cli.c
 * Created by  Sean Farrelly
 * Version     1.0
 * 
 */

/*! @file cli.c
 * @brief Implementation of command-line interface.
 */
#include "cli.h"

#include <stdint.h>
#include <string.h>

static volatile uint8_t buf[MAX_BUF_SIZE];      /* CLI Rx byte-buffer */
static volatile uint8_t *buf_ptr;               /* Pointer to Rx byte-buffer */

static uint8_t cmd_buf[MAX_BUF_SIZE];  /* CLI command buffer */
static volatile uint8_t cmd_pending;

const char cli_prompt[] = ">> ";       /* CLI prompt displayed to the user */
const char cli_unrecog[] = "CMD: Command not recognised\r\n";




/*!
 * @brief This internal API prints a message to the user on the CLI.
 */
static void cli_print(cli_t *cli, const char *msg);

/*!
 * @brief This API initialises the command-line interface.
 */
cli_status_t cli_init(cli_t *cli)
{
    /* Set buffer ptr to beginning of buf */
    buf_ptr = buf;

    cmd_pending = 0;

    /* Print the CLI prompt. */
    cli_print(cli, cli_prompt);

    return CLI_OK;
}

/*!
 * @brief This API deinitialises the command-line interface.
 */
cli_status_t cli_deinit(cli_t *cli)
{
    return CLI_OK;
}


/*! @brief This API must be periodically called by the user to process and execute
 *         any commands received.
 */
cli_status_t cli_process(cli_t *cli)
{
	if (!cmd_pending)
		return CLI_IDLE;

	uint8_t argc = 0;
    char *argv[30];

    /* Get the first token (cmd name) */
    argv[argc] = strtok(cmd_buf, " ");

    /* Walk through the other tokens (parameters) */
    while((argv[argc] != NULL) && (argc < 30))
    {
        argv[++argc] = strtok(NULL, " ");
    }
    
    /* Search the command table for a matching command, using argv[0]
     * which is the command name. */
    for(size_t i = 0 ; i < cli->cmd_cnt ; i++)
    {
        if(strcmp(argv[0], cli->cmd_tbl[i].cmd) == 0)
        {
            /* Found a match, execute the associated function. */
			cli_status_t return_value = cli->cmd_tbl[i].func(argc, argv);
		    cli_print(cli, cli_prompt); /* Print the CLI prompt to the user.             */
		    cmd_pending = 0;
			return return_value;
        }
    }

    /* Command not found */
    cli_print(cli, cli_unrecog);

    cli_print(cli, cli_prompt); /* Print the CLI prompt to the user.             */

    cmd_pending = 0;
    return CLI_E_CMD_NOT_FOUND;
}

/*!
 * @brief This API should be called from the devices interrupt handler whenever a
 *        character is received over the input stream.
 */
cli_status_t cli_put(cli_t *cli, char c)
{
    switch(c)
    {
    case CMD_TERMINATOR:
        
    	if (!cmd_pending) {
			*buf_ptr = '\0';            /* Terminate the msg and reset the msg ptr.      */
			strcpy(cmd_buf, buf);       /* Copy string to command buffer for processing. */
			cmd_pending = 1;
			buf_ptr = buf;              /* Reset buf_ptr to beginning.                   */
    	}
        break;

    case '\b':
        /* Backspace. Delete character. */
        if(buf_ptr > buf)
            buf_ptr--;
        break;

    default:
        /* Normal character received, add to buffer. */
        if((buf_ptr - buf) < MAX_BUF_SIZE)
            *buf_ptr++ = c;
        else
            return CLI_E_BUF_FULL;
        break;
    }
}

/*!
 * @brief Print a message on the command-line interface.
 */
static void cli_print(cli_t *cli, const char *msg)
{
    /* Temp buffer to store text in ram first */
    char buf[50];

    strcpy(buf, msg);
    cli->println(buf);
}
