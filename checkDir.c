#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>

static int get_file_size_time(const char *file_path,const char *filename) {
	struct stat statbuf;
	if(stat(file_path,&statbuf) == -1) {
		printf("Get stat on %s Error:%s\n",
			filename,strerror(errno));
		return -1;
	}
	//获得目录
	if(S_ISDIR(statbuf.st_mode)) {
		printf("[%s]\n",filename);
		return 1;
	}
	//获得文件
	if(S_ISREG(statbuf.st_mode)) {
		printf("%s\tsize:%ld bytes\tmodified at %s",
			filename,statbuf.st_size,
			ctime(&statbuf.st_mtime));
	}
	return 0;
}

int main(int argc,char **argv) {
	DIR *dirp;
	struct dirent *direntp;
	int stats;

	if(argc != 2) {
		printf("Usage:%s filname\n\a",argv[0]);
		exit(1);
	}
	
	//获得文件或错误
	if((stats = get_file_size_time(argv[1],argv[1]) == 0) 
		|| (stats == -1)) exit(1);
	//获得目录
	if((dirp = opendir(argv[1])) == NULL) {
		printf("Open Directory %s Error:%s\n",argv[1],
			strerror(errno));
		exit(1);
	}
	printf("Open Dir %s success.\n",argv[1]);
	while((direntp = readdir(dirp)) != NULL) {
		int path_length = strlen(argv[1])
			+ strlen(direntp->d_name) + 2;
		char *file_path = (char*)malloc(path_length * sizeof(char));
		if(file_path == NULL) {
			fprintf(stderr,"Not enough memory.");
			exit(1);
		}
		strcpy(file_path,argv[1]);
		if(file_path[strlen(file_path) - 1] != '/')
			strcat(file_path,"/");
		strcat(file_path,direntp->d_name);
		if(get_file_size_time(file_path,direntp->d_name) == -1) {
			free(file_path);
			break;
		}
		free(file_path);
	}
	closedir(dirp);
	
	return 0;
}
