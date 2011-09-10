/***************************************************************//**
* @file ProxyServer.cpp 
*
* @brief ProxyServer 类
*
* PrxoyServer 类和 main 函数的源文件
*
* @author AD_Li
*
* @date Mon Dec 10 22:56:10 CST 2007
*
*******************************************************************/

#include "ProxyServer.h" 

#include <cstdlib>


/***************************************************************//**
* @brief 程序入口, main() 函数
* 生成 ProxyServer 实例, 提供代理服务
                                                                  */
int main(int argc, char *argv[]) {
   using namespace proxyserver;

   int serverPort = GS_SERVER_PORT;

   // get the host port
   if(argc == 2)
   {
      if((serverPort = atoi(argv[1])) <= 0)
      {
         printf("Usage: %s portNum\n",argv[0]);
         return 0;
      }
   }

   ProxyServer proxyServer(serverPort);

   proxyServer.startServer();				// start proxy server

   return 0;
}

namespace proxyserver {

/** File ouput for debug */
FILE *debugFile;

   /***************** starServer ******************************/
   void ProxyServer::startServer() 
   {
      int serSock, cliSock; 				// server socket
      SocketManager serSockMan; 			// server socket manager

      // initialize to handle singnals
      initSignalHandler();

      printf("******** Start Http Proxy Server on Port %d ********\n",
             m_serverPort);

      // create server socket
      serSock = serSockMan.
         makeServerSocket(m_serverPort); 			
      if(serSock == -1)

      {
         fprintf(stderr,"Setup Server Socket Failed\n");
         exit(1);
      }

      debugFile = fopen("./%s_debug_file", "wb");


      // handle client connection
      while (true) 
      {
         cliSock = accept(serSock, NULL, NULL);

         if (cliSock != -1) 
         {
            ThreadManager::createProxyThread(cliSock);
         }
         else
         {
            perror("Connect Client Error");
         }
      }
   } // starServer

   /************************ InitSignalHandler ***********************/
   void ProxyServer::initSignalHandler()
   {
      // set SIGINT signal handle
      signal(SIGINT, SIGINTHandler);
      // ignore SIGQUIT signal
      signal(SIGQUIT, SIG_IGN);
   } // InitSignalHandler

   /************************ SIGINTHandler ***************************/
   void ProxyServer::SIGINTHandler(int signum)
   {
      char input; 

      cout << "Stop the Proxy Server?[y/n]";

      input = tolower(getchar());

      if (input == 'y')
      {
         cout << "*********  Proxy Server Stop ********" << endl;

         // close debug file
         fclose(debugFile);

         exit(0);
      }
   } // SIGINTHandler

} // namespce proxyserver

