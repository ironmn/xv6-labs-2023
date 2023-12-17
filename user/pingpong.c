#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define P2C 0 //主进程到子进程通信的管道
#define C2P 1 //子进程到主进程通信的管道

int p[2][2]; //管道的全局变量
char msg;
int main(int argc, char ** argv) {
    
    pipe(p[P2C]);
    pipe(p[C2P]);
    if (fork() == 0) { //如果是主进程
        write(p[P2C][1], &msg, sizeof(char)); //写入单个byte
        close(p[P2C][1]); //关闭write描述符
        char read_msg;
        read(p[C2P][0], &read_msg, sizeof(char));
        printf("%d: received pong\n", getpid());
        close(p[C2P][0]);
    } 
    else {
        char read_msg;
        read(p[P2C][0], &read_msg, sizeof(char));
        printf("%d: received ping\n", getpid());
        close(p[P2C][0]);
        write(p[C2P][1], &msg, sizeof(char));
        close(p[C2P][1]);
    }
    exit(0);
}