/*************拷贝文件程序cpoyfile.c*******************
 * Created By: AD_LI
 *
 * Created At:2008-09-17 16:09
 *
 * E-mail:ad_li@foxmail.com
 ******************************************************/
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1024

void copy(int from_fd,int to_fd);

int main(int argc,char **argv) {
	int from_fd,to_fd;
	struct stat statbuf;
	
	if(argc != 3) {
		fprintf(stderr,"Usage:%s fromfile tofile\n\a",argv[0]);
		exit(1);
	}

	// 打开源文件
	if((from_fd = open(argv[1],O_RDONLY)) == -1) {
		fprintf(stderr,"Open %s Error:%s\n",argv[1],strerror(errno));
		exit(-1);
	}

	// 获取原文件属性
	if((fstat(from_fd,&statbuf)) == -1) {
		fprintf(stderr,"Get %s Information Error:%s\n",
			argv[1],strerror(errno));
		exit(1);
	}

	// 创建目的文件
	if((to_fd = open(argv[2],O_WRONLY | O_CREAT,statbuf.st_mode)) == -1) {
		fprintf(stderr,"Open %s Error:%s\n",argv[2],strerror(errno));
		exit(1);
	}
	// 拷贝文件
	copy(from_fd,to_fd);
	
	return 0;
}

// 拷贝文件的方法
void copy(int from_fd,int to_fd) {
	int bytes_read,bytes_write;
	char buffer[BUFFER_SIZE];
	char *ptr;

	// 拷贝文件代码
	while((bytes_read = read(from_fd,buffer,BUFFER_SIZE))) {
		//一个致命错误发生了
		if((bytes_read == -1) && (errno != EINTR)) 
			break;
		else if(bytes_read > 0) {
			ptr = buffer;
			while((bytes_write = write(to_fd,ptr,bytes_read))) {
				// 一个致命错误发生了
				if((bytes_write == -1) && (errno != EINTR))
					break;
				// 写完了所有读的字节
				else if(bytes_write == bytes_read)
					break;
				// 只写了一部分,继续写
				else if(bytes_write > 0) {
					ptr += bytes_write;
					bytes_read -= bytes_write;
				}
			}

			// 写得时候发生致命错误
			if(bytes_write == -1)
				break;
		}
	}
}
