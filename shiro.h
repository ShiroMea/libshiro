/*
 * 本软件对在使用中所产生的任何问题，不承担任何的责任。
 * 许可任何人使用本软件用于任何目的，或修改它，从新自由发布。
 * 不过以下行为被限制：
 * 1.本软件的起源不能被更改。
 * 2.修改的版本必须被标记，不能搅乱原始版本。
 * 3.本声明不能被移除，或修改。
 */
#ifndef SHIRO_H
#define SHIRO_H

#if !(__unix__ || __linux__)
#error libshiro只能在Linux上运行
#endif

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "color.h" /* for FONT_RED,FREE */

#ifdef VERSION
/* 主版本号 */
#define MAJOR (atoi(VERSION))

/* 副版本号 */
#define MINJOR (atoi(strchr(VERSION,'.')+1))

/* 修订版本号 */
#define REVISION (atoi(strrchr(VERSION,'.')+1))
#endif  /* VERSION */

#define sro_delenter(str) (sro_delchar((str),'\n'))

#define sro_random(low,max) ((low)+rand()%((max)-(low)))

extern int color_start;

const char *Shiro_version(void);
/*
 * 返回libshiro版本
 */

void sro_buff(int set);
/*
 * 关闭/启用终端的行缓冲
 * 参数1:为0的时候关闭,为1的时候启用
 */

void sro_echo(int set);
/*
 * 关闭/启用终端回显
 * 参数1:为0的时候关闭,为1的时候启用
 */

const char *sro_er(long long int number);
/*
 * 将十进制的数转换为二进制
 * 参数1:为需要转换的十进制数
 * 返回值:返回转换后的二进制字符串,出错返回-1
 */

int sro_passwd(const char *const passwd,char ch);
/*
 * 收集密码并检查是否匹配
 * 参数1:正确的密码
 * 参数2:输入密码时候的遮盖字符
 * 返回值:密码正确返回0,密码错误返回1
 */

char *sro_strupr(char *str);
/*
 * 将字符中的字母转换为大写
 * 参数1:需要进行转换的字符串
 * 返回值:返回str字符串的首地址
 */

char *sro_strlwr(char *str);
/*
 * 将字符中的字符转换为小写
 * 参数1:需要进行转换的字符串
 * 返回值:返回str字符串的首地址
 */

long long int sro_er_n(const char *binary);
/*
 * 将二进制转换为十进制
 * 参数1:包含二进制数值的字符串(如:er函数的返回值)
 * 返回值:转换后的十进制数值,出错返回-1
 */

char *sro_strrev(char *str);
/*
 * 倒转字符串中的字符
 * 参数1:需要进行倒转的字符串
 * 返回值:返回倒转后的字符串首地址
 */

int sro_delchar(char *str,char ch);
/*
 * 从字符串中剔除指定的字符
 * 参数1:需要进行处理的字符串
 * 参数2:需要从字符串中删除的字符
 * 返回值:返回从str中剔除的字符个数
 */

int sro_choose(const char *msg,...);
/*
 * 询问用户选择
 * 参数1:询问的问题(与printf的参数相同
 * 返回值:用户输入回车或者y返回0,用户输入n返回1
 */

int sro_fuzzy(const char *str, const char *key);
/*
 * 模糊匹配
 * 参数1:需要进行匹配的字符串
 * 参数2:匹配的关键字
 * 返回值:匹配成功返回0,匹配失败返回1
 */

int sro_getch(void);
/*
 * 功能同 getch
 */

void sro_msg(int flag,const char *fmt,...);
void sro_exit(int flag,const char *fmt,...);
void sro_dump(int flag,const char *fmt,...);
/*
 * flag 控制是否显示errno,其他参数与printf相似
 * 输出一条错误信息
 * 函数        到stderr   退出方式
 * func_msg    是         return ;
 * func_exit   是         exit(EXIT_FAILURE);
 * func_dump   是         abort();
 */

const char *sro_filemode(const mode_t mode);
/*
 * 将stat结构中的st_mode成员转换为字符形式
 */
const int *sro_terminal_size(int fd);
/* 
 * 获取fd指向的终端的尺寸
 * fd   文件描述符(STDIN_FILENO,STDINOUT_FILENO)
 * 返回值:
 * sro_terminal_size(STDOUT_FILENO)[0] -> 行
 * sro_terminal_size(STDOUT_FILENO)[1] -> 列
 * 
 * 出现错误返回NULL
 */

char *sro_progress(const char *format,unsigned int len,double perc);
/*
 * 打印一个进度条
 * format  控制进度条的样式
 * len     进度条的长度,0为根据终端的尺寸
 * perc    当前的百分比
 * 返回值: 正常返回百分比,出现错误返回-1
 */

#endif /* SHIRO_H */
