#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

#define MAX_READ 20

int main(int argc, char *argv[])
{
    char buffer[MAX_READ + 1];
    ssize_t numRead;
    /* STIN_FILENO 读取键盘输入*/
    numRead = read(STDIN_FILENO, buffer, MAX_READ);
    if (numRead == -1) {
        errExit("read");
    }
    /* 不加这个 会有乱码*/
    buffer[numRead] = '\0';
    printf("The input data is %s\n", buffer);
}

