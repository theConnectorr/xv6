#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int length_child_receive;
    int length_parent_receive;
    char buffer[100];

    int fd_child_parent[2] = {0, 0};
    int fd_parent_child[2] = {0, 0};
    pipe(fd_child_parent);
    pipe(fd_parent_child);
    
    int pid_t = fork();
    // con va cha

    if (pid_t > 0) {
        // parent
        close(fd_parent_child[0]);
        write(fd_parent_child[1], "ping\n", 5); // gui cho con

        length_parent_receive = read(fd_child_parent[0], buffer, 100);
        
        printf("%d: received ", getpid());
        write(1, buffer, length_parent_receive);
        wait(0);
    } else if (pid_t == 0) {
        // child
        close(fd_parent_child[1]);
        length_child_receive = read(fd_parent_child[0], buffer, 100);
        printf("%d: received ", getpid());
        write(1, buffer, length_child_receive);
        close(fd_child_parent[0]);
        write(fd_child_parent[1], "pong\n", 5);
    } else {
        printf("something when wrong with forking\n");
    }
    exit(0);
}