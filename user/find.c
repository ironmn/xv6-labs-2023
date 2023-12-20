#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
fmtname(char *path)
{
  static char *buf;
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  buf = (char *)malloc(strlen(p));
  memmove(buf, p, strlen(p));
  return buf;
}

void find_file(char *path, char *file)
{
    char buf[512], *p, *temp;
    int fd;
    struct dirent de;
    struct stat st;
    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        temp = fmtname(path);
        if (!strcmp(file, temp)) {
            printf("%s\n", path);
            return;
        }
        break;
    case  T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            if(de.inum == 0) continue;
            else if (!strcmp(de.name, ".") || !strcmp(de.name, "..")) continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                continue;
            }
            find_file(buf, file);
        }
        break;
    default:
        break;
    }
    close(fd);
}


int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(2, "The input parameter is not enough\n");
        exit(1);
    }
    find_file(argv[1], argv[2]);
    exit(0);
}