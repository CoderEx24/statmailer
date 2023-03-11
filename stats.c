#include <stdlib.h>
#include <stdio.h>
#include "stats.h"

const char* get_mem_info()
{
    FILE *mem_file = fopen("/proc/meminfo", "r");

    if (!mem_file)
    {
        fprintf(stderr, "failed to open /proc/meminfo");
        return NULL;
    }

    const char* buf = (const char*) malloc(4 * 1024* sizeof(char));
    fread((void*) buf, sizeof(char), 4 * 1024, mem_file);

    return buf;
}

