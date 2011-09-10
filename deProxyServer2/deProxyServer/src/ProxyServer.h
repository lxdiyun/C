/***************************************************************//**
 * @file ProxyServer.h 
 * 
 * @brief ProxyServer 类头文件
 *
 * ProxyServer 类和 main 函数的头文件
 * 
 * @author AD_Li
 *
 * @date Mon Dec 10 22:59:19 CST 2007
 *
 *******************************************************************/

#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include "connection/SocketManager.h"
#include "ThreadManager.h"
#include "HTTPPack.h"

#include <iostream>
#include <cstdio>

#include <signal.h>

using namespace std;

using namespace proxyserver::connection;

/***************************************************************//**
 * @namespace proxyserver 
 * @brief 代理服务器命名空间
 */
namespace proxyserver 
{
	/** 默认服务端口 */
	const static int GS_SERVER_PORT = 3128;

	/*******************************************************//** 
	 * @brief ProxyServer 代理服务器类
	 *
	 * 代理服务器, 在特定端口提供代理服务器服务. 
	 * \n技术特点:
	 * 	- 采用多线程技术实现.
	 * 	- 支持 GET 和 POST 等多种命令
	 * 	- 处理SIGINT 和 SIGQUIT 信号
	 * 	- 处理超时连接
	 * 
	 * @author AD_Li
	 *
	 * @date Tue Dec 11 18:15:42 CST 2007
	 */
	class ProxyServer
	{
	private:
		/** 服务端口 */
		int m_serverPort;

		/***********************************************//**
		 * @brief 处理系统信号
		 *  
		 * 包括 SIGINT 和 SIGQUIT
		 */
		void initSignalHandler();

		/***********************************************//**
		 * @brief SIGINT 信号处理函数
		 *
		 * 询问并判断是否要退出代理服务器
		 *
		 * @param signum 信号
		 */
		static void SIGINTHandler(int signum);
		
	public:
		/***********************************************//**
		 * @brief 构造函数
		 *
		 * 初始化服务端口
		 *
		 * @param serverPort 服务端口
		 */
		ProxyServer(int serverPort): m_serverPort(serverPort) {}

		 /**********************************************//**
		  * @brief 开始代理服务
		  *
		  * 在服务端口上开始代理服务.
		  *
		  */
		 void startServer();
	};
}

#endif
