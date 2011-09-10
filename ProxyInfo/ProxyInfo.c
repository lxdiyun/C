/************************************** 
 * ProxyInfo.c
 * struct proxyinfo结构定义及相关操作
 * 
 * Create By: AD_Li
 * Create At: 07-10-08 17:41
 * E-mail: ad_li@foxmail.com 
 **************************************/
#include "ProxyInfo.h"

/*******************************************************************
 * 作用: 将数组写入文件
 * 参数: array - 写入数组, write_fd - 写入文件ID, 
 * 	size - 数组元素大小, num - 数组长度
 * 返回: -1 -> 错误, 0 -> 成功  
 */
int arrayToFile(void * array, int write_fd, int size, int num) {
	int writeNum = 0, totalNeedWrite = num * size;

	// 对数组进行排序	
	qsort((void *)array, num, sizeof(struct ProxyInfo), cmpTExpire);

	while (totalNeedWrite) {
		writeNum = write(write_fd, array, num * size);

		if (writeNum <0 && errno != EINTR) { // 非中断,发生错误
			perror("Write Error:");
			return -1;
		} else if (writeNum > 0) {
			totalNeedWrite -= writeNum;
			array += writeNum;
		}
	}

	return 0;
} // arrayToFile

/*******************************************************************
 * 作用: 从文件中读取数组
 * 参数: array - 写入数组, read_fd - 写入文件ID, 
 * 	size - 数组元素大小, num - 数组长度 (读取个数)
 * 返回: 实际读取个数, -1 -> 错误
 */
int fileToArray(void * array, int read_fd, int size, int num) {
	int readNum = 0;

	if ((readNum = read(read_fd, array, size * num)) <0) {
		perror("Read Error:");
		return -1;
	}

	readNum /= size;

	return readNum;
} // fileToArray

/*******************************************************************
 * 作用: 按szUrl查找文件
 *	一旦找到匹配的szUrl, 则将其tExpire和当前系统时间比较，
 *	如果 tExpire在系统当前时间之前，则调用删除文件szFileName，
 *	并返回NULL,否则返回szFileName
 * 参数: szUrl - 需要查找的szUrl, read_fd - 要查询的文件ID
 * 返回: 查到的的文件名(szFileName)
 * 注意: 函数中使用二分查找,故必须保证所查寻的数组的有序性
 */
char *findFile(const char* szUrl, int read_fd) {
	const int arraySize = 100;
	int arrayRealSize;
	struct ProxyInfo testProxys[arraySize], key, *result;
	time_t now;

	// 初始化
	strcpy(key.szUrl, szUrl);
	result = NULL;

	// 读取文件
	arrayRealSize = fileToArray(testProxys, read_fd, sizeof(testProxys[0]),
			arraySize);

	// 寻找文件,使用二分搜索
	result = (struct ProxyInfo *)bsearch((void *)&key, (void *)testProxys,
			arrayRealSize, sizeof(testProxys[0]), cmpTExpire);

	if (result != NULL) {// 找到文件, 比较时间
		now = time(NULL); // 获得当前时间
		if (result->tExpire <= now) {
			remove(result->szFileName);
			printf("Deleted timeout file: %s\n", result->szFileName);
			return NULL;
		} else
			return result->szFileName;
	}

	return NULL;
} // findFile

/*******************************************************************
 * 作用: 比较函数,根据szUrl比较两个ProxyInfo.用于快排qsort和二分搜索bsort
 * 参数: A,B - 待比较的参数
 * 返回: 0 -> A == B, -1 -> A < B, 1 -> A > B
 */
int cmpTExpire(const void *A, const void *B) {
	return strcmp(((struct ProxyInfo *)A)->szUrl, ((struct ProxyInfo *)B)->szUrl);
} // cmpTExpire

/*******************************************************************
 * 作用:比较函数,判断两个ProxyInfo是否完全相等
 * 参数: A,B - 待比较的参数
 * 返回: 0 -> A == B, -1 -> A < B, 1 -> A > B 
 */
int cmp(struct ProxyInfo A, struct ProxyInfo B) {
	int result;

	// 比较szFileName
	result = strcmp(A.szFileName, B.szFileName);
	if (result != 0)
		return result;

	// 比较szUrl
	result = strcmp(A.szUrl, B.szUrl);
	if (result != 0)
		return result;

	// 比较tExpire
	return A.tExpire - B.tExpire;
}
