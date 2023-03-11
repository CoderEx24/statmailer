#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

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
        system("sleep 10");
        printf("I am the child\n");
    }


}

