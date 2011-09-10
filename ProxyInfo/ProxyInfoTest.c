/*******************************************************************
 * ProxyInfoTest.c
 * 测试struct proxyinfo 结构和各项操作
 * 
 * Create By: AD_Li
 * Create At: 07-10-09 11:57
 * E-mail:ad_li@foxmail.com
 */

#include <assert.h>
#include "ProxyInfo.h"

#define ARRAYSIZE 5

void testArrayToFile(char *);
void testFileToArray(char *);
void testFindFile(char *);
void setup();

const int g_arraySize= ARRAYSIZE;
struct ProxyInfo g_testProxys[ARRAYSIZE];

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Usage %s outputfile\n", argv[0]);
		exit(1);
	}

	printf("Test Begin.\n");
	setup();
	testArrayToFile(argv[1]);
	testFileToArray(argv[1]);
	testFindFile(argv[1]);
	testFileToArray(argv[1]);

	return 0;
}

/*******************************************************************
 * 测试:初始化测试工作
 * 	初始化g_testProxys数组
 */
void setup() {
	printf("------------------------------------------------------------\n");
	printf("Start setup.\n");

	// 生成数组
	strcpy(g_testProxys[0].szUrl, "http://www.test.org");
	g_testProxys[0].tExpire = time(NULL) + 365 * 24 * 60 * 60;
	strcpy(g_testProxys[0].szFileName, "test0.file");

	strcpy(g_testProxys[1].szUrl, "http://www.test.net");
	g_testProxys[1].tExpire = time(NULL);
	strcpy(g_testProxys[1].szFileName, "test1.file");

	strcpy(g_testProxys[2].szUrl, "http://www.test.cn");
	g_testProxys[2].tExpire = time(NULL);
	strcpy(g_testProxys[2].szFileName, "test2.file");

	strcpy(g_testProxys[3].szUrl, "http://www.test.com");
	g_testProxys[3].tExpire = time(NULL);
	strcpy(g_testProxys[3].szFileName, "test3.file");

	strcpy(g_testProxys[4].szUrl, "http://www.test.edu");
	g_testProxys[4].tExpire = time(NULL) + 365 * 24 * 60 * 60;
	strcpy(g_testProxys[4].szFileName, "test4.file");

	printf("setup success.\n");
}// setup

/*******************************************************************
 * 测试: 测试fileToArray函数
 * 	从readFile文件读入数组并显示
 * 参数: readFile - 读入文件
 */
void testFileToArray(char *readFile) {
	printf("------------------------------------------------------------\n");
	printf("Start testFileToArray.\n");
	const int arraySize = g_arraySize;
	int arrayRealSize;
	struct ProxyInfo testProxys[arraySize];
	int read_fd;
	int i;

	// 读入
	if ((read_fd = open(readFile, O_RDONLY)) < 0) {
		fprintf(stderr,"Open %s Error:%s\n",readFile,strerror(errno));
		exit(1);
	}

	arrayRealSize = fileToArray(testProxys, read_fd, sizeof(testProxys[0]),
			arraySize);

	// 断言测试相等
	for (i = 0; i < arrayRealSize; ++i) {
		assert(!cmp(testProxys[i],g_testProxys[i]));
	}

	close(read_fd);

	printf("testFileToArray success.\n");
}// testFileToArray

/*******************************************************************
 * 测试: 测试arrayToFile函数
 * 	产生一组ProxyInfo并将其写入writeFile中
 * 参数: writeFile - 输出的文件
 */
void testArrayToFile(char* writeFile) {
	printf("------------------------------------------------------------\n");
	printf("Start testArrayToFile.\n");
	int write_fd;

	// 输出数组
	if ((write_fd = open(writeFile, O_WRONLY | O_CREAT, S_IRWXU)) == -1) {
		fprintf(stderr,"Open %s Error:%s\n",writeFile,strerror(errno));
		exit(1);
	}

	arrayToFile(g_testProxys, write_fd, sizeof(g_testProxys[0]), g_arraySize);

	close(write_fd);

	printf("testArrayToFile success.\n");
}// testArrayToFile

/*******************************************************************
 * 测试: 测试findFile函数
 * 	从readFile中读取数组并寻找各个文件
 * 参数: readFile - 读取的文件
 */
void testFindFile(char *readFile) {
	printf("------------------------------------------------------------\n");
	printf("Start testFindFile.\n");
	int read_fd, i;
	char *result;

	// 读入
	if ((read_fd = open(readFile, O_RDONLY)) < 0) {
		fprintf(stderr,"Open %s Error:%s\n",readFile,strerror(errno));
		exit(1);
	}

	// 测试查找
	for (i = 0; i < g_arraySize; ++i) {
		if ((result = findFile(g_testProxys[i].szUrl, read_fd)) != NULL)
			assert(!strcmp(g_testProxys[i].szFileName, result));

		lseek(read_fd, 0, SEEK_SET); // 重置文件读取位置
	}

	close(read_fd);

	printf("testFindFile success.\n");
}// testFindFile
