/***************************************************************//**
* @file SocketManager.cpp 
*
* @brief SocketManager 类
*
* SocketManager 类函数实现源文件
*
* @author AD_Li
*
* @date Wed Dec 12 18:42:15 CST 2007
*
*******************************************************************/

#include "SocketManager.h"

#include <cstring>

using namespace std;

namespace proxyserver 
{
   namespace connection
   {
      /************************ initialize staitc memeber ********/
      const struct timeval SocketManager::MS_TIME_OUT = {8, 0};

      /***************** makeServerSocketQ ***********************/
      int SocketManager::makeServerSocketQ(int portNum,int backlog) 
      {
         struct sockaddr_in saddr;			// build out address here
         int optVal;					// socket option value

         closeSocket();					// if the socket exist
         // close it!

         m_socket = socket(PF_INET,SOCK_STREAM,0);	// get a socket

         // set socket option to reuse address
         optVal = 1;
         setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR,
                    &optVal, sizeof(optVal)
                   );

         // set the socket optrion to close immediately
         optVal = 0;
         setsockopt(m_socket, SOL_SOCKET, SO_LINGER,
                    &optVal, sizeof(optVal)
                   );

         if (m_socket == -1) 
         {
            perror("Creare socket error");
            return -1;
         }

         // build address and build it to socket
         bzero((void *)&saddr,sizeof(saddr));		// clear out struct

         saddr.sin_addr.s_addr = htonl(INADDR_ANY);	// fill in the address
         saddr.sin_port = htons(portNum);		// fill in socket port
         saddr.sin_family = AF_INET ;            	// fill in addr family
         if (bind(m_socket,(struct sockaddr *)&saddr,
                  sizeof(saddr)) != 0) 
         {
            perror("Bind socket error");
            return -1;
         }

         // arrange for incoming calls
         if (listen(m_socket,backlog) != 0) 
         {
            perror("Listen socket error");
            return -1;
         }

         return m_socket;
      } // makeServerSocketQ

      /***************** makeClientSocket **********************/
      int SocketManager::makeClientSocket(string hostName,int portNum)
      {
         struct sockaddr_in servadd;			// the number to call
         struct hostent *hp;				// used to get number

         closeSocket();					// if the socket exist 
         // close it!

         // Step 1:Get a socket
         m_socket = socket(AF_INET,SOCK_STREAM,0);
         if (m_socket == -1) 
         {
            perror("Create socket error");
            return -1;
         }

         // set connection time-out value
         setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, 
                    &MS_TIME_OUT, sizeof(MS_TIME_OUT));

         // Step 2:connect to server
         bzero(&servadd,sizeof(servadd));		// zero the address
         hp = gethostbyname(hostName.c_str());		// lookup host's ip #
         if (hp == NULL) 
         {
            perror("Get Host Name Error");
            return -1;
         }
         bcopy(hp -> h_addr, (struct sockaddr *)&servadd.sin_addr,
               hp -> h_length);
         servadd.sin_port = htons(portNum);		// fill in port number
         servadd.sin_family = AF_INET;			// fill in socket type

         if (connect(m_socket,(struct sockaddr *)&servadd,
                     sizeof(servadd)) != 0) 
         {
            perror("Connect socket error");
            return -1;
         }

         return m_socket;
      } // makeClientSocket

   }
}
