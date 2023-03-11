#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "stats.h"

int write_mem_file(const char* tempdir, int i, const char* buf, size_t bufsize)
{
    char* filename = (char*) malloc(128 * sizeof(char));
    snprintf(filename, 128, "%s/mem%i.txt", tempdir, i);

    FILE *mem_file = fopen((const char*) filename, "w");

    if (!mem_file)
        return -1;

    unsigned long bytes = fwrite(buf, sizeof(char), bufsize, mem_file);

    if (bytes < 0)
        return -1;

    free(filename);
    fclose(mem_file);

    return bytes;
}

int main()
{
    pid_t child;

    child = fork();

    if (child < 0)
        exit(-1);
    else if (child > 0)
        exit(0);
    else
    {
        
        sleep(4);

        char template[] = "/tmp/statmailer.XXXXXX";
        char *dir_name = mkdtemp(template);
        
        if (!dir_name)
        {
            perror("failed to create tmpdir");
            exit(-1);
        }

        for (int i = 0;; i ++)
        {
            const char* mem_info = get_mem_info();

            if (write_mem_file(dir_name, i, mem_info, 4 * 1024) < 0)
                perror("failed to write to memfile");

            free((void*) mem_info);
            mem_info = NULL;

            sleep(60);
        }

    }

}

