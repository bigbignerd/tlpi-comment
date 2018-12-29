/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 6-4 */

#define _GNU_SOURCE     /* Get various declarations from <stdlib.h> */
#include <stdlib.h>
#include "tlpi_hdr.h"

/* 全局变量：在c program 运行时的开始会初始化环境变量信息 */
extern char **environ;

int
main(int argc, char *argv[])
{
    int j;
    char **ep;
    /* clearenv 无法clear setenv设置的环境变量 */
    clearenv();         /* Erase entire environment */

    for (j = 1; j < argc; j++)
        /* putenv修改环境变量的方式，environ中并不会复制一份，而是直接定义个指针指向这个string，所以一旦不小心修改了string，则         * 其他使用这个环境变量的地方都会受到影响 
         */
        if (putenv(argv[j]) != 0)
            errExit("putenv: %s", argv[j]);
    /* setenv 不会有putenv的问题，他会复制一份，变量已经存在不会覆盖 */
    if (setenv("GREET", "Hello world", 0) == -1)
        errExit("setenv");

    unsetenv("BYE");

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    exit(EXIT_SUCCESS);
}
