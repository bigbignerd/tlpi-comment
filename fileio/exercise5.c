/* 练习5：验证复制的文件描述符共享offset */

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd, fd2;
    int fd1_off, fd2_off;

    if (argc < 2)
        usageErr("参数错误");

    //打开参数指定的文件
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("文件打开失败");
    //复制一下文件描述符
    fd2 = dup(fd);
    if (fd2 == -1)
        errExit("复制文件描述符失败");

    //移动fd2 的offset
    fd2_off = lseek(fd2, 10, SEEK_SET);
    if (fd2_off == -1)
        errExit("fd2设置偏移失败");

    //获取fd1当前的offset
    fd1_off = lseek(fd, 0, SEEK_CUR);
    if (fd1_off == -1)
        errExit("获取fd1偏移量失败");

    if (fd1_off == fd2_off) {
        printf("fd1 offset == fd2 offset\n");
    } else {
        printf("fd1 offset != fd2 offset\n");
    }

    exit(EXIT_SUCCESS);
}

