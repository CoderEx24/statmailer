#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stats.h"

int write_file(const char* kind, const char* tempdir, int i, const char* buf, size_t bufsize)
{
    char* filename = (char*) malloc(128 * sizeof(char));
    snprintf(filename, 128, "%s/%s%i.txt", tempdir, kind, i);

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
            const char* cpu_info = get_cpu_info();
            const char* net_info = get_net_info();
            const char* dsk_info = get_dsk_info();

            if (write_file("mem", dir_name, i, mem_info, strlen(mem_info)) < 0)
                perror("failed to write to memfile");

            if (write_file("cpu", dir_name, i, cpu_info, strlen(cpu_info)) < 0)
                perror("failed to write to cpufile");
            
            if (write_file("net", dir_name, i, net_info, strlen(net_info)) < 0)
                perror("failed to write to netfile");
            
            if (write_file("dsk", dir_name, i, dsk_info, strlen(dsk_info)) < 0)
                perror("failed to write to diskfile");

            free((void*) mem_info);
            free((void*) cpu_info);
            free((void*) net_info);
            free((void*) dsk_info);
            dsk_info = net_info = cpu_info = mem_info = NULL;

            if (i == 12 * 60)
            {
                // 12 hours have passed, send email
                
                // create archive file
                pid_t archive_process = fork();

                if (archive_process < 0)
                    perror("Failed to fork");

                else if (archive_process == 0)
                {
                    char *archive_path = (char*) malloc(512);
                    snprintf(archive_path, 512, "%s/archive.7z", dir_name);
                    char *argv[5] = { "7z", "a", archive_path, dir_name, NULL };

                    execvp("7z", argv);
                }
                else
                {
                    int ret;
                    while(wait(&ret) != archive_process);

                    if (ret != 0)
                    {
                        perror("failed to create archive");

                        i--;
                        continue;
                    }

                    i = 0;
                }

            }
            sleep(2);
        }

    }

}

