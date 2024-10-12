#include "../kernel/types.h"
#include "user.h" 

int main(void)
{
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);

    if (fork() == 0)
    {
        // Child process
        close(p1[1]);  
        close(p2[0]);  
        char buf[1];

        if (read(p1[0], buf, 1) != 1)
        {
            printf("child: read error\n");
            exit(1);
        }
        else
        {
            int child_pid = getpid();
            printf("%d: received ping\n", child_pid);  
        }

        buf[0] = 'a'; 
        write(p2[1], buf, 1);

        close(p1[0]);  
        close(p2[1]);  
    }
    else
    {
        // Parent process
        close(p1[0]);  
        close(p2[1]);  
        char buf[1] = {'a'};

        write(p1[1], buf, 1);

        if (read(p2[0], buf, 1) != 1)
        {
            printf("parent: read error\n");
            exit(1);
        }
        else
        {
            int parent_pid = getpid();
            printf("%d: received pong\n", parent_pid);  
        }

        close(p1[1]); 
        close(p2[0]);
    }

    exit(0);
}