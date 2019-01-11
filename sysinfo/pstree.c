/* 
 * 打印所有进程父子关系 简易版pstree
 */
#define _GNU_SOURCE
#include <limits.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include "ugid_functions.h"
#include "tlpi_hdr.h"

#define MAX_PROCESS_NUM 1024
#define MAX_LINE 1000
void printChild(int pid, int *ps, char name[][MAX_LINE], int n, int nextLevel) ;

int main(int argc, char *argv[])
{
    //保存子进程=>父进程映射关系
    int ps[MAX_PROCESS_NUM];
    //打开的目录handler
    DIR *dirp;
    //下个目录进入点
    struct dirent *dp;
    //保存临时进程文件目录
    char path[PATH_MAX]; 
    //读取/proc/PID/status中的一行，临时保存；cmd为进程名称
    char line[MAX_LINE], cmd[MAX_LINE]; 
    //进程ID=>进程名称的map
    char nameMap[MAX_PROCESS_NUM][MAX_LINE];
    //读取statu文件时，用于判断是否已经获取到进程ID，父ID和进程名称
    Boolean gotPid, gotPpid, gotName;
    //文件handler
    FILE *fp;
    char *p; 
    uid_t pid, ppid;
    int i, level;

    //初始化进程关系元素
    for (i = 0; i< MAX_PROCESS_NUM; i++) {
        ps[i] = -1;    
    }
    //读取所有的进程文件
    dirp = opendir("/proc");
    if (dirp == NULL)
        errExit("opendir");
    //循环遍历每一个进程文件夹
    for (;;) {
        errno = 0;              /* To distinguish error from end-of-directory */
        dp = readdir(dirp);
        if (dp == NULL) {
            if (errno != 0)
                errExit("readdir");
            else
                break;
        }
        if (dp->d_type != DT_DIR || !isdigit((unsigned char) dp->d_name[0]))
            continue;
        snprintf(path, PATH_MAX, "/proc/%s/status", dp->d_name);

        fp = fopen(path, "r");
        if (fp == NULL)
            continue;           /* Ignore errors: fopen() might fail if
                                   process has just terminated */
        gotPid = FALSE;
        gotPpid = FALSE;
        gotName = FALSE;
        while (!gotPid || !gotPpid || !gotName) {
            if (fgets(line, MAX_LINE, fp) == NULL)
                break; /* 判断当前行中是否包括 Pid */ if (strncmp(line, "Pid:", 4) == 0) {
                pid = strtol(line + 4, NULL, 10);
                gotPid = TRUE;
            }
            /* 判断当前行中是否包括 Ppid */
            if (strncmp(line, "PPid:", 5) == 0) {
                ppid = strtol(line + 5, NULL, 10);
                gotPpid = TRUE;
            }
            //获取进程名称
            if (strncmp(line, "Name:", 5) == 0) {
                for (p = line + 5; *p != '\0' && isspace((unsigned char) *p); )
                    p++;
                strncpy(cmd, p, MAX_LINE - 1);
                cmd[MAX_LINE -1] = '\0';        /* Ensure null-terminated */

                gotName = TRUE;
            }
        }

        fclose(fp);
        //保存父进程子进程关系
        if (pid > 1023) {
            continue;
        }
        ps[pid] = ppid;
        strncpy(nameMap[pid], cmd, MAX_LINE);
    }
    level = 0;
    //打印
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        if (ps[i]> -1) {
            printf("+|%d %s\n", ps[i], "idle");  
            printChild(ps[i], ps, nameMap, MAX_PROCESS_NUM, level+1);
            break;
        }    
    }
    printf("\n");
      
    return 0;
}
/* 递归打印 */
void printChild(int pid, int *ps, char name[][MAX_LINE], int n, int nextLevel) {
    int i;
    //找到pid所有的子节点
    for (i = pid; i < n; i++) {
        if (ps[i] == pid) {
            printf("%*.s-|%2d %s", nextLevel*4, " ",i,  name[i]);    
            printChild(i, ps, name, n, nextLevel+1);
        }  
    } 
}
