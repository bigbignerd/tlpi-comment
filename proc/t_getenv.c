/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2018.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 6 */

#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    char *val;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s environ-var\n", argv[0]);
    /* SUSV3 标准允许genenv的实现使用静态缓冲区返回环境变量的值（缓冲区可能会被接下来的getenv调用覆盖），
       尽管gblic没有使用这种方式（静态缓冲区）实现，安全起见还是复制到另外的地方*/
    val = getenv(argv[1]);
    printf("%s\n", (val != NULL) ? val : "No such variable");

    exit(EXIT_SUCCESS);
}
