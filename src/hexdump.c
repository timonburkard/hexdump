/*** Includes ****************************************************************/

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clic.h"

/*** Defines *****************************************************************/

#define BYTES_PER_LINE 16

/*** Function Declarations ***************************************************/

static bool parse_size_value(const char* text, size_t* value);
void        print_as_char(uint8_t byte);
clic_err_t  show(clic_res_t* result);

/*** CLIC ********************************************************************/

enum {
    ARG_ID_NAME = 0,
    ARG_ID_OFFSET,
    ARG_ID_LENGTH,
    ARG_ID_WIDTH,
};

static const char* const option_offset_names[] = {"--offset", "-o", NULL};
static const char* const option_length_names[] = {"--length", "-l", NULL};
static const char* const option_width_names[]  = {"--width", "-w", NULL};

static const clic_arg_t arguments[] = {
    [ARG_ID_NAME] = {
        .type        = CLIC_ARG_POSITIONAL,
        .required    = true,
        .names       = NULL,
        .value_name  = "FILE",
        .description = "File to hex dump or '-' for stdin",
    },
    [ARG_ID_OFFSET] = {
        .type        = CLIC_ARG_WITH_VALUE,
        .required    = false,
        .names       = option_offset_names,
        .value_name  = "BYTES",
        .description = "Byte offset to start reading from",
    },
    [ARG_ID_LENGTH] = {
        .type        = CLIC_ARG_WITH_VALUE,
        .required    = false,
        .names       = option_length_names,
        .value_name  = "BYTES",
        .description = "Maximum number of bytes to print",
    },
    [ARG_ID_WIDTH] = {
        .type        = CLIC_ARG_WITH_VALUE,
        .required    = false,
        .names       = option_width_names,
        .value_name  = "BYTES",
        .description = "Number of bytes per line",
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

static bool parse_size_value(const char* text, size_t* value)
{
    char*              end    = NULL;
    unsigned long long parsed = 0;

    errno  = 0;
    parsed = strtoull(text, &end, 0);

    if ((errno != 0) || (end == text) || (*end != '\0') || (parsed > SIZE_MAX)) {
        return false;
    }

    *value = (size_t)parsed;
    return true;
}

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
    const char* file_name = result->argv[ARG_ID_NAME];
    size_t      offset    = 0;
    size_t      length    = SIZE_MAX;
    size_t      width     = BYTES_PER_LINE;
    size_t      bytes_read;
    size_t      bytes_printed = 0;
    uint8_t*    buffer;
    FILE*       file;
    bool        close_file = true;

    if (result->argv[ARG_ID_OFFSET] != NULL) {
        if (!parse_size_value(result->argv[ARG_ID_OFFSET], &offset)) {
            fprintf(stderr, "Error: invalid offset '%s'\n", result->argv[ARG_ID_OFFSET]);
            return CLIC_ERR_ARG;
        }
    }

    if (result->argv[ARG_ID_LENGTH] != NULL) {
        if (!parse_size_value(result->argv[ARG_ID_LENGTH], &length)) {
            fprintf(stderr, "Error: invalid length '%s'\n", result->argv[ARG_ID_LENGTH]);
            return CLIC_ERR_ARG;
        }
    }

    if (result->argv[ARG_ID_WIDTH] != NULL) {
        if (!parse_size_value(result->argv[ARG_ID_WIDTH], &width) || (width == 0)) {
            fprintf(stderr, "Error: invalid width '%s'\n", result->argv[ARG_ID_WIDTH]);
            return CLIC_ERR_ARG;
        }
    }

    if (length == 0) {
        return CLIC_ERR_OK;
    }

    buffer = malloc(width * sizeof(*buffer));
    if (buffer == NULL) {
        fprintf(stderr, "Error: could not allocate a buffer of %llu bytes\n", (unsigned long long)width);
        return CLIC_ERR_GENERAL;
    }

    if (strcmp(file_name, "-") == 0) {
        file       = stdin;
        close_file = false;
    } else {
        file = fopen(file_name, "rb");
    }

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file '%s'\n", file_name);
        free(buffer);
        return CLIC_ERR_GENERAL;
    }

    if (offset > 0) {
        if (close_file) {
            if (offset > (size_t)LONG_MAX) {
                fprintf(stderr, "Error: offset too large\n");
                fclose(file);
                free(buffer);
                return CLIC_ERR_GENERAL;
            }

            if (fseek(file, (long)offset, SEEK_SET) != 0) {
                fprintf(stderr, "Error: Could not seek in file '%s'\n", file_name);
                fclose(file);
                free(buffer);
                return CLIC_ERR_GENERAL;
            }
        } else {
            for (size_t i = 0; i < offset; ++i) {
                int c = fgetc(file);
                if (c == EOF) {
                    fprintf(stderr, "Error: offset exceeds input size\n");
                    free(buffer);
                    return CLIC_ERR_GENERAL;
                }
            }
        }
    }

    while (bytes_printed < length) {
        size_t chunk_size      = width;
        size_t total_remaining = 0;

        if (length != SIZE_MAX) {
            total_remaining = length - bytes_printed;
            if (total_remaining < chunk_size) {
                chunk_size = total_remaining;
            }
        }

        bytes_read = fread(buffer, 1, chunk_size, file);

        if ((bytes_read == 0) && ferror(file)) {
            fprintf(stderr, "Error: Could not read from file '%s'\n", file_name);
            if (close_file) {
                fclose(file);
            }
            free(buffer);
            return CLIC_ERR_GENERAL;
        }

        if (bytes_read == 0) {
            break;
        }

        printf("%08llX  ", (unsigned long long)(offset + bytes_printed));

        for (size_t i = 0; i < width; ++i) {
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

        bytes_printed += bytes_read;

        if (bytes_read < chunk_size) {
            break;
        }
    }

    if (close_file) {
        fclose(file);
    }

    free(buffer);
    return CLIC_ERR_OK;
}
