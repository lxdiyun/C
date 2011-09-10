/***************************************************************//**
 * @file HTTPPack.h
 *
 * @brief HTTPPack 类头文件
 *
 * HTTPPack 类头文件, 由字符串构造 HTTP 包, 检测出 HOST CMD 等属性
 *
 * @author AD_Li
 *
 * @date Sun Dec 16 15:07:55 CST 2007
 *
 *******************************************************************/

#ifndef HTTP_PACK_H
#define HTTP_PACK_H

#include <iostream>
#include <string>

#include "string.h"

using namespace std;

namespace proxyserver 
{
	/*******************************************************//**
	 * @brief HTTPPack HTTP 包
	 *
	 * 分析字符串, 获取HTTP 包中命令, 主机名, 协议各项信息
	 *
	 * @author AD_Li
	 *
	 * @date Sun Dec 16 15:16:47 CST 2007
	 */
	class HTTPPack
	{
	private:
		/** 默认 HTTP 服务端口 */
		const static int GS_DEFAULT_HTTP_PORT = 80;

		/** HTTP 包 */
		string m_pack;

		/** 包中主机名 */
		string m_hostName;

		/** 主机端口 */
		int m_hostPort;

		/** 包是否有效 */
		bool m_valid;
	public:
		/***********************************************//**
		 * @brief 构造函数
		 *
		 * 由字符串分析 HTTP 包
		 *
		 * @param pack 字符串形式的 HTTP 包
		 */
		HTTPPack(const char *pack);

		/*********************** inline ********************/

		/***********************************************//**
		 * @brief 获取 HTTP 包
		 *
		 * @return HTTP 包 
		 */
		string getPack()
		{
			return m_pack;
		}

		/***********************************************//**
		 * @brief 获取主机名
		 *
		 * @return HTTP 包中主机名
		 * 
		 */
		string getHostName() 
		{
			return m_hostName;
		}

		/***********************************************//** 
		 * @brief 包是否为有效 HTTP 包
		 *
		 * @return 是否为有效包
		 *
		 */
		bool isValid()
		{
			return m_valid;
		}

		/***********************************************//**
		 * @brief 获取主机端口
		 *
		 * @return 主机端口
		 */
		int getHostPort() 
		{
			return m_hostPort;
		}
	};
	
	/*******************************************************//**
	 * @example HTTPPackTest.cpp
	 *
	 * @brief HTTPPack 类示例和测试
	 *
	 * HTTPPack 类测试文件以及测试函数
	 *
	 */
}

#endif
