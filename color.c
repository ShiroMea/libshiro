/*
 * 注意：该头文件只能在Linux系统的终端中使用 
 * 本软件对在使用中所产生的任何问题，不承担任何的责任。 
 * 许可任何人使用本软件用于任何目的，或修改它，从新自由发布。 
 * 不过以下行为被限制： 
 * 1.本软件的起源不能被更改。 
 * 2.修改的版本必须被标记，不能搅乱原始版本。 
 * 3.本声明不能被移除，或修改。 
 */

#include "color.h"
#include "shiro.h"

/* 为set_color 提供字体颜色 */
static COLOR_NUMBER retc_font(SEED seed)
{
	if (seed == 0)
		return 0;
	int i = 0;
	while (i < 8) {
		if ((seed >> i) & 1)
			switch (i) {
				case 0: 
					return BLACK;
				case 1: 
					return RED;
				case 2: 
					return GREEN;
				case 3: 
					return YELLOW;
				case 4: 
					return BLUE;
				case 5: 
					return PURPLE;
				case 6: 
					return CYAN;
				case 7: 
					return WHITE;
				default:
					break;
			}
		i++;
	}
	return C_NONE;
}

/* 为 set_color 提供字体背景色 */
static COLOR_NUMBER retc_back(SEED seed)
{
	if (seed == 0)
		return 0;
	int i = 7;
	while (i < 16) {
		if ((seed >> i) & 1)
			switch (i) {
				case 8: 
					return B_BLACK;
				case 9: 
					return B_RED;
				case 10:
					return B_GREEN;
				case 11:
					return B_YELLOW;
				case 12:
					return B_BLUE;
				case 13:
					return B_PURPLE;
				case 14:
					return B_CYAN;
				case 15:
					return B_WHITE;
				default:
					break;
			}
		i++;
	}
	return C_NONE;
}

/* 为 set_color 提供特殊属性 */
static COLOR_NUMBER retc_special(SEED seed)
{
	if (seed == 0)
		return 0;
	int i = 15;
	while (i < 22) {
		if ((seed >> i) & 1)
			switch (i) {
				case 16:
					return HIGHT;
				case 17:
					return ITALIC;
				case 18:
					return UNDERLINE;
				case 19:
					return FLASH;
				case 20:
					return REVERSE;
				case 21:
					return HIDE_FONT;
				default:
					break;
			}
		i++;
	}
	return C_NONE;
	/*注意,此处返回0的话会导致字体颜色无效*/
}

/* 合成终端颜色控制字符 */
static char *set_color(SEED seed)
{
	if (seed == 0)
		return NULL;
	static char buffer[12] = "";
	COLOR_NUMBER opt = retc_special(seed);
	/* 特殊选项 */
	COLOR_NUMBER font = retc_font(seed);
	/* 字体颜色 */
	COLOR_NUMBER back = retc_back(seed);
	/* 字体背景色 */
	sprintf(buffer,"\e[%d;%d;%dm",opt,font,back);
	return (buffer);
}

/* 为 change_format 提供字体颜色控制字符串 */
static const char *set_font(char code)
{
	switch (code) {
		case '0':
			return "\e[0m";
		case '1':
			return (set_color(BLACK_F));
		case '2':
			return (set_color(RED_F));
		case '3':
			return (set_color(GREEN_F));
		case '4':
			return (set_color(YELLOW_F));
		case '5':
			return (set_color(BLUE_F));
		case '6':
			return (set_color(PURPLE_F));
		case '7':
			return (set_color(CYAN_F));
		case '8':
			return (set_color(WHITE_F));
		default:
			break;
	}
	return NULL;
}

/* 为 change_format 提供字符背景颜色控制字符串 */
static const char *set_background(char code)
{
	switch (code) {
		case '0':
			return "\e[0m";
		case '1':
			return (set_color(BLACK_B));
		case '2':
			return (set_color(RED_B));
		case '3':
			return (set_color(GREEN_B));
		case '4':
			return (set_color(YELLOW_B));
		case '5':
			return (set_color(BLUE_B));
		case '6':
			return (set_color(PURPLE_B));
		case '7':
			return (set_color(CYAN_B));
		case '8':
			return (set_color(WHITE_B));
		default:
			break;
	}
	return NULL;
}

/* 为 change_format 提供特殊属性控制字符串 */
static const char *set_special(char code)
{
	switch (code) {
		case '0':
			return "\e[0m";
		case '1':
			return (set_color(HIGHT_S));
		case '2':
			return (set_color(ITALIC_S));
		case '3':
			return (set_color(UNDERLINE_S));
		case '4':
			return (set_color(FLASH_S));
		case '5':
			return (set_color(REVERSE_S));
		case '6':
			return (set_color(HIDE_FONT_S));
		default:
			break;
	}
	return NULL;
}

static int check_variable(int count,char code)
{
	int invalid = 1;
	int i = 0;

	while (i <= count) {

		if ((int)(code - '0') == i) {
			invalid = 0;
			break;
		}
		i++;
	}

	return invalid;
}

/* 展开 sro_printf,sro_sprintf... 系列函数的format */
static char *change_format(const char *format)
{
	if (format == NULL)
		return NULL;

	char *temp = NULL;
	char *seek = NULL;
	const char *f_seek = NULL;

	int skip = 0;

	if ((temp = (char *)calloc(sizeof(char),strlen(format)+512)) == NULL) {
		fprintf(stderr,"%s:%s\n",__func__,strerror(errno));
		return NULL;
	}

	seek = temp;
	f_seek = format;
	
	while (*f_seek != '\0') {

		if (*f_seek == '#') {
			switch (*(f_seek+1)) {
				case 'F':

					if (check_variable(8,*(f_seek+2))) {
						*seek = *f_seek;
						break;
					}

					skip = strlen(set_font(*(f_seek+2)));
					strncpy(seek,set_font(*(f_seek+2)),skip);
					f_seek += 2;
					seek += skip-1;
					break;
				case 'B':

					if (check_variable(8,*(f_seek+2))) {
						*seek = *f_seek;
						break;
					}

					skip = strlen(set_background(*(f_seek+2)));
					strncpy(seek,set_background(*(f_seek+2)),skip);
					f_seek += 2;
					seek += skip-1;
					break;
				case 'S':
					
					if (check_variable(6,*(f_seek+2))) {
						*seek = *f_seek;
						break;
					}

					skip = strlen(set_special(*(f_seek+2)));
					strncpy(seek,set_special(*(f_seek+2)),skip);
					f_seek += 2;
					seek += skip-1;
					break;
				default:
					*seek = *f_seek;
					break;
			}

		} else 
			*seek = *f_seek;

		seek++;
		f_seek++;
	}

	return temp;
}

void sro_setcolor(SEED seed)
{
	if (seed == 0) {
		printf("\e[0m");
		return ;
	}

	printf("%s",set_color(seed));
	return ;
}

char *sro_color(const char *string,int start,int size,SEED seed)
{
	if ((string == NULL) || (start < 0) || (size < 0)) {
		errno = EINVAL;
		return NULL;
	}

	char *buf = NULL;
	char *con = NULL;
	int len = 0;
	int i = 0;

	if ((con = set_color(seed)) == NULL) {
		sro_msg(0,"E:读取种子失败(%s)",__func__);
		return NULL;
	}

	len = strlen(con); //计算颜色控制字符的长度

	if ((buf = (char *)calloc(strlen(string)+len+10,sizeof(char))) == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	while (start--)
		buf[i++] = *(string++);
	strcpy(buf+i,con);
	i += len;
	while (size--)
		buf[i++] = *(string++);
	strcpy(buf+i,"\e[0m");
	i += strlen("\e[0m");
	while (*string)
		buf[i++] = *(string++);
	buf[i] = '\0';
	return buf;
}

int sro_printf(const char *format,...)
{
	if (format == NULL)
		return -1;

	va_list ap;
	int retc = 0;
	char *temp = NULL;

	if ((temp = change_format(format)) == NULL)
		return -1;

	va_start(ap,format);
	retc = vprintf(temp,ap);
	fflush(stdout);
	va_end(ap);

	free(temp);

	return retc;
}

int sro_fprintf(FILE *stream,const char *format,...)
{
	if (stream == NULL || format == NULL)
		return -1;

	va_list ap;
	int retc = 0;
	char *temp = NULL;

	if ((temp = change_format(format)) == NULL)
		return -1;

	va_start(ap,format);
	retc = vfprintf(stream,temp,ap);
	fflush(stream);
	va_end(ap);

	free(temp);
	
	return retc;
}

int sro_dprintf(int fd,const char *format,...)
{
	if (format == NULL)
		return -1;

	va_list ap;
	int retc = 0;
	char *temp = NULL;

	if ((temp = change_format(format)) == NULL)
		return -1;

	va_start(ap,format);
	retc = vdprintf(fd,temp,ap);
	va_end(ap);

	free(temp);

	return retc;
}

int sro_sprintf(char *str,const char *format,...)
{
	if (str == NULL || format == NULL)
		return -1;

	va_list ap;
	int retc = 0;
	char *temp = NULL;

	if ((temp = change_format(format)) == NULL)
		return -1;

	va_start(ap,format);
	retc = vsprintf(str,temp,ap);
	va_end(ap);

	free(temp);

	return retc;
}

int sro_snprintf(char *str,size_t size,const char *format,...)
{
	if (str == NULL || size > 0 || format == NULL)
		return -1;

	va_list ap;
	int retc = 0;
	char *temp = NULL;

	if ((temp = change_format(format)) == NULL)
		return -1;

	va_start(ap,format);
	retc = vsnprintf(str,size,temp,ap);
	va_end(ap);

	free(temp);

	return retc;
}

int sro_fputs(const char *str,FILE *stream)
{
	if (str == NULL || stream == NULL)
		return -1;

	int retc = 0;
	char *temp = NULL;

	if ((temp = change_format(str)) == NULL)
		return -1;

	retc = fputs(temp,stream);
	fflush(stream);

	free(temp);

	return retc;
}

int sro_puts(const char *str)
{
	if (str == NULL)
		return -1;

	int retc = 0;
	char *temp = NULL;

	if ((temp = change_format(str)) == NULL)
		return -1;

	retc = puts(temp);
	fflush(stdout);

	free(temp);

	return retc;
}
