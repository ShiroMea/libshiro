/*
 * 本软件对在使用中所产生的任何问题，不承担任何的责任。
 * 许可任何人使用本软件用于任何目的，或修改它，从新自由发布。
 * 不过以下行为被限制：
 * 1.本软件的起源不能被更改。
 * 2.修改的版本必须被标记，不能搅乱原始版本。
 * 3.本声明不能被移除，或修改。
 */

#include "shiro.h"

int color_start = 0;

/*
 * 指向二进制数出错的位置
 * 属于er_n函数的子函数
 */
static void point(int address)
{
	int i;
	for(i = 0;i < address ;i++) 
		fputc(' ',stderr);

	fputs("\e[1;31m",stderr);
	fflush(stderr);
	fprintf(stderr,"^\n");
	fputs("\e[0m",stderr);
	fflush(stderr);

	return ;
}

/*
 * 显示出出错信息
 * 属于:sro_msg,sro_exit,sro_dump函数的核心函数
 */
static void err_doit(int flag,const char *fmt,va_list ap)
{
	char buf[512] = "";
	int errno_save = errno;

	vsprintf(buf,fmt,ap);

	if (flag)
		sprintf(buf+strlen(buf),":%s",strerror(errno_save));

	strcat(buf,"\n");
	fputs(buf,stderr);
	fflush(stderr);

	return ;
}

/* 检测文件权限 */
static char permissions(const int count)
{
	switch (count%3) {
		case 0:return 'x';break;
		case 1:return 'r';break;
		case 2:return 'w';break;
		default:break;

	}
	return '-';

}

/* 检测文件的特殊权限 */
static char special_permissions(const int  count)
{
	switch (count%3) {
		case 0:return 't';break;
		case 1:
		case 2:return 's';break;
		default:break;
	}
	return '-';
}

/* 检测文件的类型 */
static char file_type(const mode_t mode)
{
	mode_t backup = mode;
	backup =  mode & S_IFMT;

	switch (backup) {
		case S_IFREG:return '-';break;
		case S_IFDIR:return 'd';break;
		case S_IFCHR:return 'c';break;
		case S_IFBLK:return 'b';break;
		case S_IFIFO:return 'p';break;
		case S_IFLNK:return 'l';break;
		case S_IFSOCK:return 's';break;
		default:break;
	}
	return '-';
}

const char *Shiro_version(void)
{
#ifdef  VERSION
	return (VERSION);
#else
	return ("(null)");
#endif
}

void sro_echo(int set)
{
	struct termios old;
	tcgetattr(STDIN_FILENO,&old);
	/* 获取当前终端的设置 */
	if (set)
		old.c_lflag |= ECHO; 
		/* 打开终端的字符回显 */
	else
		old.c_lflag &= ~ECHO;
		/* 关闭终端的字符回显 */
	tcsetattr(STDIN_FILENO,TCSANOW,&old);
	return ;
}

void sro_buff(int set)
{
	struct termios old;
	tcgetattr(STDIN_FILENO,&old);
	if (set)
		old.c_lflag |= ICANON;
		/* 打开标准输入处理 */
	else
		old.c_lflag &= ~ICANON;
		/* 关闭标准输入处理 */
	tcsetattr(STDIN_FILENO,TCSANOW,&old);
	return ;
}

const char *sro_er(long long int number)
{
	int i = 0;

	/* 每个字节等于8bit  0~63 = 64 个bit*/
	int max = sizeof(number)*8-1;

	/* 为了容纳 '\0' 必须加1 */
	static char var[sizeof(number)*8+1] = "";

	if (number != 0) {
		for (i = max;i >= 0; i--,number >>= 1) {
			if ((number & 01) == 1)
				var[i] = '1';
			else
				var[i] = '0';
		}

		/* 跳过开头的 '0' */
		for (i = 0;var[i] == '0';i++) ;
		return (var+i);
	}
	return strcpy(var,"0");
}

int sro_passwd(const char *const passwd,char ch)
{
	if (passwd == NULL) {
		errno = EINVAL;
		return -1;
	}

	int size = strlen(passwd)+1;
	char *tmp = NULL;
	int count = 0,err = 3;

	if ((tmp = (char *) calloc (size,sizeof(char))) == NULL) {
		errno = ENOMEM;
		return -1;
	}

	do {
		printf("密码:");
		fflush(stdout);

		for (count = 0; count < size; count++) {

			if ((tmp[count]=sro_getch()) == EOF) {
				errno = EIO;
				return -1;
			}

			if (tmp[count] == '\n')
				break;
			putchar(ch);
		}

		putchar('\n');
		tmp[count]='\0';

		if (!strcmp(passwd,tmp)) {
			free(tmp);
			return 0;
		} else {
			_FONT_RED; 
			printf("密码错误\n");
			_FREE;
			err--;
		}

	} while (err > 0);

	free(tmp);
	printf("错误次数到达上限\n");
	return 1;
}

char *sro_strupr(char *str)
{

	char *tmp = str;

	for (; *str != '\0';str++)
		*str = (char)toupper(*str);

	return tmp;
}

char *sro_strlwr(char *str)
{

	char *tmp = str;

	for (; *str != '\0'; str++)
		*str = (char)tolower(*str);

	return tmp;
}

long long int sro_er_n(const char *binary)
{
	if (binary == NULL) {
		errno = EINVAL;
		return -1;
	}

	int i = 0;
	long long int number = 0;

	/* 检查是否为有效的二进制数 */
	for (i = 0; binary[i] != '\0'; i++) {
		if (binary[i] == '1' || binary[i] == '0') 
			continue;
		else {
			sro_msg(0,"%s 不是有效的二进制数",binary);
			point(i);
			return -1;
		}
	}

	/* 使用位操作进行二转十 */
	for (i = 0; binary[i] != '\0'; i++,number <<= 1) {
		if (binary[i] == '1') 
			number |= 1;
		else
			number |= 0;
	}
	number >>= 1;
	return number;
}

char *sro_strrev(char *string)
{
	if (string == NULL) {
		errno = EINVAL;
		return NULL;
	}

	int i = 0;
	int china = strlen("白"); /* 计算一个汉字为多少个字节 */
	int size = strlen(string)+1;
	char *tmp = NULL,*pos = NULL;

	if ((tmp = (char *) calloc(size,sizeof(char))) == NULL) {
		errno = ENOMEM;
		return NULL;
	}

	pos = tmp;

	for (i = strlen(string)-1; (string[i] != '\0') && (i >= 0); i--,pos++) {
		if (isascii(string[i]))
			*pos = string[i];
		else {
			memcpy(pos,(string+i)-2,china);
			i -= china-1;
			pos += china-1;
		}
	}
	strcpy(string,tmp);
	free(tmp);
	return string;
}

int sro_delchar(char *str,char ch)
{
	if ((str == NULL) || (ch == '\0')) {
		errno = EINVAL;
		return -1;
	}

	int i = 0,j = 0,len = strlen(str);
	for(;str[i] != '\0';i++)
		if (str[i] != ch)
			str[j++] = str[i];
	str[j] = '\0';

	return (len-strlen(str));
}

int sro_choose(const char *msg,...)
{
	char *temp = NULL;
	char choose[20] = "";

	if ((temp = (char *) calloc (strlen(msg)+100,sizeof(char))) == NULL ) {
		errno = ENOMEM;
		return -1;
	}

	va_list ap;
	va_start(ap,msg);
	vsprintf(temp,msg,ap);
	va_end(ap);

	while (1) {
		printf("%s (y or n):",temp);

		if (fgets(choose,20,stdin) == NULL) {
			errno = EIO;
			return -1;
		}

		switch (*choose) {
			case 'y':
			case '\n':
			   	free(temp);
			   	return 0;
			case 'n':
			case '\0':
				free(temp);
				return 1;
			default:
				printf(" Please answer 'y' or 'n'\n");
		}
		memset(choose,'\0',20);
	}

	return -1;

}

int sro_fuzzy(const char *str, const char *key)
{
	if ((key == NULL) || (str == NULL)) {
		errno = EINVAL;
		return -1;
	}

	int i  = 0;

	for (; str[i] != '\0'; i++) 
		if (str[i] == *key)
			if(!strncmp(str+i,key,strlen(key))) {
				color_start = i;
				return 0;
			}
	return 1;
}

int sro_getch(void)
{
	int ch = 0;
	sro_buff(0);
	sro_echo(0);
	ch = getchar();
	sro_buff(1);
	sro_echo(1);
	return ch;
}

void sro_msg(int flag,const char *fmt,...)
{
	va_list ap;

	va_start(ap,fmt);
	err_doit(flag,fmt,ap);
	va_end(ap);

	return ;
}

void sro_exit(int flag,const char *fmt,...)
{
	va_list ap;

	va_start(ap,fmt);
	err_doit(flag,fmt,ap);
	va_end(ap);

	exit(EXIT_FAILURE);
	return ;
}

void sro_dump(int flag,const char *fmt,...)
{
	va_list ap;

	va_start(ap,fmt);
	err_doit(flag,fmt,ap);
	va_end(ap);

	abort();
	return ;
}

const char *sro_filemode(const mode_t mode)
{
	if (mode == 0) {
		errno = EINVAL;
		return NULL;
	}

	/* 
	 * rwxrwxrwx + suid + sgid + sbit + filetype = 13
	 *     9     +   1  +   1  +  1   +    1     = 13
	 */
	int count = 13;

	/* 
	 *   s   s   t
	 * rwx rwx rwx
	 *   3   6   9
	 */
	int special = 9;

	/* 从perm数组最后一个成员开始赋值 */
	int seek = 9;

	/* drwxrwxrwx = 10 (char) */
	static char perm[10] = "";

	mode_t backup = mode;
	mode_t mode_edit = 0;

	mode_edit = mode & ~S_IFMT;
	
	while (count--) {
		if (count > 3) {
			if ((mode_edit & 01) == 01)
				perm[seek] = permissions(count);
			else
				perm[seek] = '-';
		} else {

			if (mode_edit == 0)
				break;

			if ((mode_edit & 01) == 1)
				perm[special] = special_permissions(count);
			special -= 3;
		}
		mode_edit >>= 1;
		seek--;
	}

	perm[0] = file_type(backup);

	return perm;
}

const int *sro_terminal_size(int fd)
{
	if (! isatty(fd))
		return NULL;

	static int size[2] = {0};
	struct winsize win;

	if (ioctl(fd,TIOCGWINSZ,&win) == -1)
		return NULL;

	/* 行 */
	size[0] = win.ws_row;
	/* 列 */
	size[1] = win.ws_col;

	return size;
}

char *sro_progress(const char *format,unsigned int len,double perc)
{
	if (format == NULL || *format == '\0') 
		format = "#."; /* 设置默认样式 */

	int count = 0;
	double calc = 0.0;
	static char *progress = NULL;

	len = (len == 0)?sro_terminal_size(STDOUT_FILENO)[1] - 10:len;
	/* 减去10个字符是为了保留显示百分比的位置
	 * [#####.....]50.0%
	 */

	calc = (double)len / 100.0;
	calc = calc * perc;

	if (progress == NULL) {
		if ((progress = calloc(len + 1,sizeof(char))) == NULL)
			return NULL;
	} else {
		if ((progress = realloc(progress,len + 1)) == NULL)
			return NULL;
	}

	for (count = 0; len > count; count++)
		progress[count] = ((int)calc > count)?format[0]:format[1];

	progress[count] = '\0';

	return progress;
}

