/*
 * 注意：该头文件只能在Linux系统的终端中使用 
 * 本软件对在使用中所产生的任何问题，不承担任何的责任。 
 * 许可任何人使用本软件用于任何目的，或修改它，从新自由发布。 
 * 不过以下行为被限制： 
 * 1.本软件的起源不能被更改。 
 * 2.修改的版本必须被标记，不能搅乱原始版本。 
 * 3.本声明不能被移除，或修改。 
 */
#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>

#if !(__linux__ || __unix__)
#error 仅使用于linux/unix
#endif

/* 光标控制 */
#define _CUR          (printf("\e[?25h"))       /* 显示光标 */
#define _NO_CUR       (printf("\e[?25l"))       /* 隐藏光标 */
#define _MOVEUP(x)    (printf("\e[%dA",x))      /* 光标上移x行 */
#define _MOVEDOWN(x)  (printf("\e[%dB",x))      /* 光标下移x行 */
#define _MOVELEFT(x)  (printf("\e[%dD",x))      /* 光标左移x个字符 */
#define _MOVERIGHT(x) (printf("\e[%dC",x))      /* 光标右移x个字符 */
#define _MOVETO(x,y)  (printf("\e[%d;%dH",x,y)) /* 将光标移动至x行y列处 */

/* 特殊属性 */
#define _HIGHT      (printf("\e[1m"))   /* 高亮 */
#define _ITALIC     (printf("\e[3m"))   /* 斜体 (不保证有效果) */
#define _UNDERLINE  (printf("\e[4m"))   /* 下划线 */
#define _FLASH      (printf("\e[5m"))   /* 闪烁 (不保证有效果) */
#define _REVERSE    (printf("\e[7m"))   /* 反色显示 */
#define _HIDE_FONT  (printf("\e[8m"))   /* 字体不可见 (不保证有效果) */

typedef enum {
	/* 特殊属性 */
	HIGHT     = 1,
	ITALIC    = 3,
	UNDERLINE = 4,
	FLASH     = 5,
	C_NONE    = 6, /* 无效的颜色 */
	REVERSE   = 7,
	HIDE_FONT = 8,

	/* 字体颜色 */
	BLACK   = 30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	PURPLE,
	CYAN,
	WHITE,

	/* 背景色 */
	B_BLACK = 40,
	B_RED,
	B_GREEN,
	B_YELLOW,
	B_BLUE,
	B_PURPLE,
	B_CYAN,
	B_WHITE
} COLOR_NUMBER;

/* 为 sro_setcolor 和 sro_color 提供掩码 */
typedef enum {
	/* 字体颜色 */
	BLACK_F  = 1,              /* 黑色 */
	RED_F    = BLACK_F * 2,    /* 红色 */
	GREEN_F  = RED_F * 2,      /* 绿色 */
	YELLOW_F = GREEN_F * 2,    /* 黄色 */
	BLUE_F   = YELLOW_F * 2,   /* 蓝色 */
	PURPLE_F = BLUE_F * 2,     /* 紫色 */
	CYAN_F   = PURPLE_F * 2,   /* 青色 */
	WHITE_F  = CYAN_F * 2,     /* 白色 */

	/* 字体背景颜色 */
	BLACK_B  = WHITE_F * 2,   
	RED_B    = BLACK_B * 2,    
	GREEN_B  = RED_B * 2,    
	YELLOW_B = GREEN_B * 2, 
	BLUE_B   = YELLOW_B * 2,  
	PURPLE_B = BLUE_B * 2,  
	CYAN_B   = PURPLE_B * 2,  
	WHITE_B  = CYAN_B * 2,   

	/* 属性设置 */
	HIGHT_S     = WHITE_B * 2,     /* 设置高亮 */
	ITALIC_S    = HIGHT_S * 2,     /* 设置斜体 (不保证有效果) */
	UNDERLINE_S = ITALIC_S * 2,    /* 设置下划线 */
	FLASH_S     = UNDERLINE_S * 2, /* 设置闪烁 (不保证有效果) */
	REVERSE_S   = FLASH_S * 2,     /* 反转颜色 */
	HIDE_FONT_S = REVERSE_S * 2    /* 字体隐藏 (不保证有效果) */
} SEED;


/*
 * 在某些Linux的控制台中有的可能没有效果
 * 关于采取的优先级:
 * 掩码越小优先级越高(针对同一系列的设置)
 */

void sro_setcolor(SEED seed);

char *sro_color(const char *str,int start,int size,SEED color);
/*
 * 设置颜色
 * str  	需要设置颜色的字符串
 * start	从第几个字符上色
 * size 	上色的长度为几个字符
 * color	需要设置颜色的种子
 * 返回值:返回处理过的str,失败返回NULL,并设置errno
 * 警告:该函数仅适用于Linux/unix
 *   	由于函数使用了calloc函数使用结束后请调用free函数释放
 */

int sro_printf(const char *format,...);
int sro_fprintf(FILE *stream,const char *format,...);
int sro_dprintf(int fd,const char *format,...);
int sro_sprintf(char *str,const char *format,...);
int sro_snprintf(char *str,size_t size,const char *format,...);
int sro_fputs(const char *str,FILE *stream);
int sro_puts(const char *str);
/*
 * 设置嵌入式颜色
 * #      F    0-8
 * ^      ^    ^
 * 标志 类型  颜色
 *
 * F  字体
 * B  字体背景
 * S  特殊
 *
 * -----------------------------------------------
 * |                 字体&背景色                 |
 * -----------------------------------------------
 * |黑色 红色  绿色  黄色  蓝色  紫色  青色  白色|
 * | 1    2     3     4     5     6     7     8  |
 * -----------------------------------------------
 *
 * -----------------------------------------------
 * |                   特殊属性                  |
 * -----------------------------------------------
 * |  高亮  斜体  下划线  闪烁   反色   隐藏字体 |
 * |   1     2      3      4      5        6     |
 * -----------------------------------------------
 *
 * 例子:
 * sro_printf("#F2%s#F0\n","Shiro");
 */

/* 黑色 */
#define _FONT_BLACK (printf("\e[1;30m"))  /* 设置字体颜色为黑色 */
#define _LOW_BLACK  (printf("\e[30m"))    /* 低亮的黑色 */
#define _BLACK      (printf("\e[1;40m"))  /* 设置字体背景色为黑色 */

/* 红色 */
#define _FONT_RED (printf("\e[1;31m"))  /* 设置字体颜色为红色 */
#define _LOW_RED  (printf("\e[31m"))    /* 低亮的红色 */
#define _RED      (printf("\e[1;41m"))  /* 设置字体背景色为红色 */

/* 绿色 */
#define _FONT_GREEN (printf("\e[1;32m"))  /* 设置字体颜色为绿色 */
#define _LOW_GREEN  (printf("\e[32m"))    /* 低亮的绿色 */
#define _GREEN      (printf("\e[1;42m"))  /* 设置字体背景色为绿色 */

/* 黄色 */
#define _FONT_YELLOW (printf("\e[1;33m"))  /* 设置字体颜色为黄色 */
#define _LOW_YELLOW  (printf("\e[33m"))    /* 低亮的黄色 */
#define _YELLOW      (printf("\e[1;43m"))  /* 设置字体背景色为黄色 */

/* 蓝色 */
#define _FONT_BLUE (printf("\e[1;34m"))  /* 设置字体颜色为蓝色 */
#define _LOW_BLUE  (printf("\e[34m"))    /* 低亮的蓝色 */
#define _BLUE      (printf("\e[1;44m"))  /* 设置字体背景色为蓝色 */

/* 紫色 */
#define _FONT_PURPLE (printf("\e[1;35m"))  /* 设置字体颜色为紫色 */
#define _LOW_PURPLE  (printf("\e[35m"))    /* 低亮的紫色 */
#define _PURPLE      (printf("\e[1;45m"))  /* 设置字体背景色为紫色 */

/* 青色 */
#define _FONT_CYAN  (printf("\e[1;36m"))  /* 设置字体颜色为青色 */
#define _LOW_CYAN   (printf("\e[36m"))    /* 低亮的青色 */
#define _CYAN       (printf("\e[1;46m"))  /* 设置字体背景色为青色 */

/* 白色 */
#define _FONT_WHITE (printf("\e[1;37m"))  /* 设置字体颜色为白色 */
#define _LOW_WHITE  (printf("\e[37m"))    /* 低亮的白色 */
#define _WHITE      (printf("\e[1;47m"))  /* 设置字体背景色为白色 */

#define _FREE (printf("\e[0m"))   /* 释放掉上一次设置的颜色 */

/* 清屏 */
#define _CLEAR (system("clear"))  /* 调用clear命令清屏 */
#define _CLS   (printf("\f"))     /* 使用字符控制串清屏 */

#define _COLOR(a,b,c) (printf("\e[%d;%d;%dm",a,b,c)) /* 自定义配色 */

#endif /* COLOR_H */
