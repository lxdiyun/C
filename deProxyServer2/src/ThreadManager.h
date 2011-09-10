/***************************************************************//**
 * @file ThreadManager.h
 *
 * @brief ThradManager 类头文件
 *
 * @author AD_Li
 *
 * @date Tue Dec 18 19:52:06 CST 2007
 *******************************************************************/

#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include "HTTPPack.h"
#include "connection/SocketManager.h"

#include <pthread.h>
#include <sys/time.h>

#include <errno.h>

#include <iostream>
#include <string>

using namespace std;

using namespace proxyserver::connection;

namespace proxyserver {
	/*******************************************************//**
	 * @brief ThreadManager 线程管理者
	 * 	
	 * 生成线程以完成你 HTTP 代理服务
	 *
	 * @author AD_Li
	 *
	 * @date Tue Dec 18 19:56:03 CST 2007
	 *
	 */
	class ThreadManager
	{
	private:
		/** 无法连接信息 */
		const static string MS_CANNT_GET_MSG;

		/***********************************************//**
		 * @brief 处理客户端 HTTP 代理请求
		 *
		 * 接受客户端请求, 将其发到服务端, 并调用 
		 * transBetSerAndCli 在两者之间进行通讯
		 *
		 * @param cliSockP 客户端套接字指针
		 *
		 */
		static void * processRequest(void *cliSockP);

		 /**********************************************//**
		  * @brief 在客户端和服务端间传递信息
		  *
		  * @param serSock 服务器端套接字
		  *
		  * @param cliSock 客户端套接字
		  *
		  */
		static void transBetSerAndCli(int serSock, int cliSock); 
	public:
		/***********************************************//**
	 	 * @brief 创建 HTTP 代理线程
	 	 * 
		 * 初始化线程并调用 proccessRequest 为客户端提供 HTTP 
		 * 代理服务
		 *
	 	 * @param cliSock 客户端套接字
	 	 *
	 	 */
		static void createProxyThread(int cliSock);
	
	};

	/***************** 以下为辅助函数 **************************/
	
	/*******************************************************//**
	 * @brief 写函数
	 *
	 * 将数据写入 file id
	 *
	 * @param fd 要写入的id
	 *
	 * @param buf 写入的数据
	 *
	 * @param count 数据长度
	 *
	 * @return 写入成功与否
	 *
	 * @retval 0 写入成功
	 *
	 * @retval -1 写入失败
	 */
	int myWrite(int fd, const void *buf, int count);

}// namespace proxyserver

#endif
