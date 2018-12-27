/* 练习题2：打开文件，lseek到开始，写入，数据会写在哪里? 答案是从0开始覆盖 */

#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;
    ssize_t numberWrite;

    if (argc < 3) 
        usageErr("用法错误");   
    //打开文件
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
    if (fd == -1) 
        errExit("打开文件失败");

    //offset 指到开始位置
    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("设置偏移位置失败");
    
    //写点数据
    numberWrite = write(fd, &argv[2][0], strlen(&argv[2][0]));
    if (numberWrite == -1)
        errExit("写入数据错误");
       
    printf("写入：%s", (char *)&argv[2][0]);

    exit(EXIT_SUCCESS);
}
