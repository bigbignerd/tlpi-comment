/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 5-1 */

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);
    /* 目的是：判断一个文件是否存在，不存在则创建并打开 */
    fd = open(argv[1], O_WRONLY);       /* Open 1: check if file exists */
    if (fd != -1) {                     /* Open succeeded */
        printf("[PID %ld] File \"%s\" already exists\n",
                (long) getpid(), argv[1]);
        close(fd);
    } else {
        /* ENOENT:文件或目录不存在 */
        if (errno != ENOENT) {          /* Failed for unexpected reason */
            errExit("open");
        } else {
            printf("[PID %ld] File \"%s\" doesn't exist yet\n",
                    (long) getpid(), argv[1]);
            /*
             * 这里就是想验证一下两个进程同时进行文件是否存在以及不存在则创建一个文件的操作时 这种方式存在的操作原子性问题 
             * A进程 判断文件不存在进入else，然后sleep五秒
             * B进程 判断文件不存在进入else，不sleep直接创建并打开
             * A进程 sleep 5秒后继续执行创建文件并打开
             * 这样的话操作就出现问题了，所以这时候如果使用O_CREAT 再加上O_EXCL flag就好了，文件存在时会返回-1
             */
            if (argc > 2) {             /* Delay between check and create */
                sleep(5);               /* Suspend execution for 5 seconds */
                printf("[PID %ld] Done sleeping\n", (long) getpid());
            }
            fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
            if (fd == -1)
                errExit("open");

            printf("[PID %ld] Created file \"%s\" exclusively\n",
                    (long) getpid(), argv[1]);          /* MAY NOT BE TRUE! */
        }
    }

    exit(EXIT_SUCCESS);
}
