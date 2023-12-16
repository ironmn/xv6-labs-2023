#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char **argv) {
    char msg[32] = "No argument!\n";
    if (argc < 2) {
        write(1, msg, sizeof(msg));
        exit(0);
    }
    int second = atoi(argv[1]);
    sleep(second);
    exit(0);
}