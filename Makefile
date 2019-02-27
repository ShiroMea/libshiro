
CC = gcc
AR = ar
CP = cp
RM = rm
CD = cd
LN = ln
MKDIR = mkdir
LDCONFIG = ldconfig

CFLAGS = -fPIC -Wall -DVERSION='"$(VERSION)"'
ARFLAGS = -crs
LNFLAGS = -sf
MKDIRFLAGS = -p

prefix = /usr/local

MAJOR = 1
VERSION = $(MAJOR).3.6
STATIC = libshiro.a

MAN=shiro.3.gz
MANPATH=$(prefix)/man/man3

SHARE = libshiro.so
SONAME = $(SHARE).$(MAJOR)
DYNAMEIC = $(SHARE).$(VERSION)

OBJ = shiro.o color.o

$(SHARE):$(OBJ)
	$(CC) $(CFLAGS) -shared -Wl,-soname=$(SONAME) -o $(DYNAMEIC) $(OBJ)
	$(AR) $(ARFLAGS) $(STATIC) $(OBJ)

shiro.o:shiro.h shiro.c
color.o:color.h color.c

.PHONY:install uninstall help clean

install:
#安装头文件
	$(CP) $(CPFLAGS) color.h $(prefix)/include/color.h
	$(CP) $(CPFLAGS) shiro.h $(prefix)/include/shiro.h
#安装库文件
	$(CP) $(CPFLAGS) $(DYNAMEIC) $(prefix)/lib/$(DYNAMEIC)
	$(CP) $(CPFLAGS) $(STATIC) $(prefix)/lib/$(STATIC)
#安装文档
	$(MKDIR) $(MKDIRFLAGS) $(MANPATH)
	$(CP) $(CPFLAGS) $(MAN) $(MANPATH)
#更新库缓冲
	$(LDCONFIG)
	$(CD) $(prefix)/lib;$(LN) $(LNFLAGS) $(DYNAMEIC) $(SHARE)
uninstall:
#删除头文件
	-$(RM) $(RMFLAGS) $(prefix)/include/color.h
	-$(RM) $(RMFLAGS) $(prefix)/include/shiro.h
#删除静态库
	-$(RM) $(RMFLAGS) $(prefix)/lib/$(STATIC)
#删除文档
	-$(RM) $(RMFLAGS) $(MANPATH)/$(MAN)
#删除动态库以及链接文件
	-$(RM) $(RMFLAGS) $(prefix)/lib/$(SHARE)
	-$(RM) $(RMFLAGS) $(prefix)/lib/$(SONAME)
	-$(RM) $(RMFLAGS) $(prefix)/lib/$(DYNAMEIC)
help:
	@echo 'install   安装'
	@echo 'uninstall 卸载'
	@echo 'help      帮助'
	@echo 'clean     清理'
clean:
	-$(RM) $(RMFLAGS) $(OBJ) $(DYNAMEIC) $(STATIC)
