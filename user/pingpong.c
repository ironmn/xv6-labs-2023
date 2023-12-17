#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define P2C 0 //父到子
#define C2P 1 //子到父

int p[2][2]; //两个管道
char msg = 'a'; //初始化消息

int main(int argc, char ** argv) {
    
    pipe(p[P2C]);
    pipe(p[C2P]);

    if (fork() == 0) { // 子进程
        close(p[P2C][1]); // 关闭父到子的写端
        close(p[C2P][0]); // 关闭子到父的读端

        read(p[P2C][0], &msg, sizeof(char)); // 从父进程读取
        printf("%d: received ping\n", getpid());
        write(p[C2P][1], &msg, sizeof(char)); // 写回给父进程

        close(p[P2C][0]); // 关闭剩余的文件描述符
        close(p[C2P][1]);
    } else { // 父进程
        close(p[P2C][0]); // 关闭父到子的读端
        close(p[C2P][1]); // 关闭子到父的写端

        write(p[P2C][1], &msg, sizeof(char)); // 写给子进程
        read(p[C2P][0], &msg, sizeof(char)); // 从子进程读取

        printf("%d: received pong\n", getpid());

        close(p[P2C][1]); // 关闭剩余的文件描述符
        close(p[C2P][0]);
        
        wait(0); // 等待子进程结束
    }
    exit(0);
}