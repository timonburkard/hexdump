/*** Includes ****************************************************************/

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "clic.h"

/*** Defines *****************************************************************/

#define BYTES_PER_LINE 16

/*** Function Declarations ***************************************************/

void       print_as_char(uint8_t byte);
clic_err_t show(clic_res_t* result);

/*** CLIC ********************************************************************/

enum {
    ARG_ID_NAME = 0,
};

static const clic_arg_t arguments[] = {
    [ARG_ID_NAME] = {
        .type        = CLIC_ARG_POSITIONAL,
        .required    = true,
        .names       = NULL,
        .value_name  = "FILE",
        .description = "File to hex dump",
    },
};

static const char* const command_names[] = {"show", NULL};

static const clic_cmd_t commands[] = {
    {
        .names       = command_names,
        .description = "Print hex dump of a file",
        .function    = show,
        .argc        = (uint8_t)(sizeof(arguments) / sizeof(arguments[0])),
        .argv        = arguments,
    },
};

/*** Main ********************************************************************/

int main(int argc, char** argv)
{
    return (int)clic_parse(
        (uint8_t)(sizeof(commands) / sizeof(commands[0])),
        commands,
        argc,
        (const char* const*)argv);
}

/*** Function Definitions ****************************************************/

void print_as_char(uint8_t byte)
{
    if (isprint(byte)) {
        printf("%c", (char)byte);
    } else {
        printf(".");
    }
}

clic_err_t show(clic_res_t* result)
{
    size_t  bytes_read;
    size_t  address = 0;
    uint8_t buffer[BYTES_PER_LINE];
    FILE*   file;
    bool    close_file = true;

    if (strcmp(result->argv[ARG_ID_NAME], "-") == 0) {
        file       = stdin;
        close_file = false;
    } else {
        file = fopen(result->argv[ARG_ID_NAME], "rb");
    }

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", result->argv[ARG_ID_NAME]);
        return CLIC_ERR_GENERAL;
    }

    do {
        bytes_read = fread(buffer, 1, BYTES_PER_LINE, file);

        if (bytes_read == 0) {
            if (ferror(file)) {
                fprintf(stderr, "Error: Could not read from file '%s'\n", result->argv[ARG_ID_NAME]);
                if (close_file) {
                    fclose(file);
                }
                return CLIC_ERR_GENERAL;
            }

            if (close_file) {
                fclose(file);
            }

            return CLIC_ERR_OK;
        }

        printf("%08llX  ", (unsigned long long)address);

        for (size_t i = 0; i < BYTES_PER_LINE; ++i) {
            if (i < bytes_read) {
                printf("%02X ", buffer[i]);
            } else {
                printf("   ");
            }
        }

        printf(" |");

        for (size_t i = 0; i < bytes_read; ++i) {
            print_as_char(buffer[i]);
        }

        printf("|\n");

        address += bytes_read;

    } while (bytes_read == BYTES_PER_LINE);

    if (close_file) {
        fclose(file);
    }

    return CLIC_ERR_OK;
}
