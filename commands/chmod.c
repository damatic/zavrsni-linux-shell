#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/limits.h>

#define BUFFER_LENGTH 1024

void print_error(const char *this, const char *filename)
{ // in case if command fails for some reason
    // this will be name of the command
    fprintf(stderr, "%s: cannot change permission mode for '%s'\n"
                    "ERROR: %s\n",
            this, filename, strerror(errno));

    exit(EXIT_FAILURE);
}

void print_usage(const char *this)
{ // if syntax of command is not correct
    fprintf(stderr, "SYNTAX ERROR: \n"
                    "USAGE %s [mode] [filename]\n",
            this);

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    char mode_buffer[BUFFER_LENGTH];
    char buf[PATH_MAX];
    int mode_octal;

    if (argv[1] == NULL || argv[2] == NULL)
    {
        print_usage(argv[0]);
    }

    strcpy(mode_buffer, argv[1]);
    strcpy(buf, argv[2]);

    if ((mode_octal = strtol(mode_buffer, 0, 8)) == 0)
    { // convert to octal
        print_error(argv[0], argv[2]);
    }

    if (chmod(buf, mode_octal) == -1)
    {
        print_error(argv[0], argv[2]);
    }

    return 0;
}
