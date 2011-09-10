/***************************************************************//**
* @file SocketManager.h 
* 
* @brief SocketManager 类头文件
*
* SocketManager 的定义以及依赖的库声明. 
*
* @author AD_Li
*
* @date Tue Dec 11 20:50:39 CST 2007
* 
*******************************************************************/

#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <iostream>
#include <string>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

namespace proxyserver 
{
   /***************************************************************//**
   * @namespace connection
   * @brief 命名空间 - 连接
                                                                     */	
   namespace connection 
   {
      /*******************************************************//**
      * @brief SocketManager socket 管理者
      *
      * 提供函数建立 server socket 或 client socket 
      *
                                                                */
      class SocketManager
      {
         private:
            /** socket ID */
            int m_socket;

            /** 默认监听队列长度 */
            const static int MS_BACK_LOG = 1;

            /** 连接超时时间 */
            const static struct timeval MS_TIME_OUT;

         public:
            /***********************************************//**
            * @brief 构造函数
            * 
            * 初始化各项变量.
            *
                                                              */
            SocketManager():m_socket(0){};

            /***********************************************//**
            * @brief 析构函数
            * 
            * 关闭 socket
                                                              */
            ~SocketManager() { closeSocket(); }

            /***********************************************//**
            * @brief 生成 server socket
            *
            * 利用系统函数生成 server socket
            *
            * @param portNum 服务端口号
            *
            * @param backlog 监听队列长度
            *
            * @return server socket
            *
            * @retval -1 生成 server socket 失败
            *
                                                              */
            int makeServerSocketQ(int portNum,int backlog);

            /***********************************************//**
            * @brief 生成 server socket
            *
            * 使用默认监听队列生成 server socket
            *
            * @param portNum 服务端口号
            *
            * @return server socket
            *
            * @retval -1 生成 server socket 失败
                                                              */
            int makeServerSocket(int portNum)
            {
               return makeServerSocketQ(portNum,MS_BACK_LOG);
            }

            /**********************************************//**
            * @brief 生成 client socket
            *
            * 利用系统函数生成 client socket
            *
            * @param hostName 主机名
            *
            * @param portNum 主机服务端口
            *
            * @return client socket
            *
            * @retval -1 生成 client socket 失败
            *
                                                             */
            int makeClientSocket(string hostName,int portNum);

            /*********************************************//**
            * @brief 获取 socket ID
            *
            * 返回 socket ID
            *
            * @return socket ID
            *
                                                            */
            int getSocket() { return m_socket; }

            /********************************************//**
            * @brief 关闭 socket
                                                           */
            void closeSocket() 
            {
               if (m_socket > 0)				// if the socket exist close it.
               {
                  if(close(m_socket) != 0)
                     perror("Close socket failure");
               }
            }
      };

      /*******************************************************//**
      * @example SocketManagerTest.cpp
      * 
      * @brief SocketManager 类示例和测试
      *
      * SocketManager 类的测试文件以及测试函数
      *
                                                                */
   }
}

#endif
