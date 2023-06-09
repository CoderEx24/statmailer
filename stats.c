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

    fclose(mem_file);
    return buf;
}

const char* get_cpu_info()
{
    FILE *cpu_file = fopen("/proc/stat", "r");

    if (!cpu_file)
    {
        fprintf(stderr, "failed to open /proc/stat");
        return NULL;
    }

    const char* buf = (const char*) malloc(20 * 1024 * sizeof(char));
    fread((void*) buf, sizeof(char), 20 * 1024, cpu_file);

    fclose(cpu_file);
    return buf;
}

const char* get_net_info()
{
    FILE *net_file = fopen("/proc/net/sockstat", "r");

    if (!net_file)
    {
        fprintf(stderr, "failed to open /proc/net/sockstat");
        return NULL;
    }

    const char* buf = (const char*) malloc(4 * 1024 * sizeof(char));
    fread((void*) buf, sizeof(char), 4 * 1024, net_file);

    fclose(net_file);
    return buf;
}

const char* get_dsk_info()
{
    FILE *dsk_file = fopen("/proc/diskstats", "r");

    if (!dsk_file)
    {
        fprintf(stderr, "failed to open /proc/diskstats");
        return NULL;
    }

    const char* buf = (const char*) malloc(6 * 1024 * sizeof(char));
    fread((void*) buf, sizeof(char), 6 * 1024, dsk_file);

    fclose(dsk_file);
    return buf;
}

