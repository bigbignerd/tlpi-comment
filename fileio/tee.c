/* tee 命令实现 */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
    int fd, flags;

    mode_t filePerms;
    //读入的文件数量
    ssize_t numRead;
    //缓冲区大小
    char buf[BUF_SIZE];

    //是否有-a选项
    int append = 0;
    //选项
    int opt;
    //获取参数信息
    while((opt= getopt(argc, argv, "a")) != -1)
    {
        switch(opt) {
            case 'a':
                append = 1;
                break;
        }
    }
    //获取文件参数
    char *file = argv[optind];
    if (file == NULL)
    {
        errExit("缺少文件参数");
    }
    
    //读取标准输入，写入文件
    flags = O_WRONLY | O_CREAT;
    if(append == 1)
    {
        flags |= O_APPEND;
    }
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    fd = open(file, flags, filePerms);
    if (fd == -1)
        errExit("文件打开失败");

    printf("缓冲区内容:\n");
    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    {
        if(write(fd, buf, numRead) != numRead)
            errExit("无法正确的写入缓存");
        printf("%s", buf);
    }
    //关闭打开的文件
    if (close(fd) == -1)
        errExit("关闭文件失败");

    printf("ok.\n");
    return 0;
}

