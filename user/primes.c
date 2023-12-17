#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int prev[2];
int now[2];

void dfs(int fd) {
    int p[2];
    int num;
    pipe(p);
    read(fd, &num, sizeof(int));
    printf("prime %d\n", num);
    int tmp = -1;
    while(1) {
        int n = read(fd, &tmp, sizeof(int));
        if (n <= 0) {break;}
        if (tmp % num != 0) {
            write(p[1], &tmp, sizeof(int));
        }
    }
    
    if (tmp == -1) {
        close(p[1]);
        close(p[0]);
        close(fd);
        return;
    }
    if (fork() == 0) { //子进程
        close(p[1]);
        close(fd);
        dfs(p[0]);
        close(p[0]);
    } else {
        close(p[1]);
        close(p[0]);
        close(fd);
        wait(0);
    }
}

int main(int argc, char **argv) {
    pipe(prev);
    for (int i = 2; i <= 35; i++) {
        write(prev[1], &i, sizeof(int));
    }
    close(prev[1]);
    dfs(prev[0]);
    close(prev[0]);

    exit(0);
}