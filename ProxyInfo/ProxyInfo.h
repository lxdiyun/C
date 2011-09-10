/************************************** 
 * ProxyInfo.h
 * struct proxy结构定义及相关操作函数
 * 
 * Create By: AD_Li
 * Create At: 07-09-24 15:50
 * E-mail:ad_li@foxmail.com 
 **************************************/
#ifndef PROXYINFO_H_
#define PROXYINFO_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

struct ProxyInfo {
	char szUrl[1024];
	time_t tExpire;
	char szFileName[256];
};

int arrayToFile(void *, int, int, int);
int fileToArray(void *, int, int, int);
char* findFile(const char *, int);
int cmpTExpire(const void *, const void *);
int cmp(struct ProxyInfo, struct ProxyInfo);

#endif /*PROXYINFO_H_*/
