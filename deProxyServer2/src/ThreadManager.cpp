/*******************************************************************
* @file ThreaManager.cpp
*
* @brief ThreadManager 类源文件
*
* @author AD_Li
*
* @date Tue Dec 18 21:36:36 CST 2007
*
*******************************************************************/

#include "ThreadManager.h"

namespace proxyserver {
   /************************ initailize static member **********/
   const string ThreadManager::MS_CANNT_GET_MSG 
      = "Can't connect to server";

   extern FILE *debugFile;


   /************************ createProxyThread *****************/
   void ThreadManager::createProxyThread(int cliSocket)
   {
      pthread_t proxyThread;				// thread
      pthread_attr_t detachedAttr;			// thread attribute
      int *cliSocketP;				// client socket pointer

      // initalize the thread attribute to be detached
      pthread_attr_init(&detachedAttr);
      if(pthread_attr_setdetachstate(&detachedAttr, 
                                     PTHREAD_CREATE_DETACHED) != 0)
      {
         perror("Set Thread Attribute Failed");
         return;
      }

      // copy the clisocket
      cliSocketP = new int;
      *cliSocketP = cliSocket;

      // create proxy thread
      if(pthread_create(&proxyThread, &detachedAttr, 
                        processRequest, cliSocketP) != 0)
      {
         perror("Create Thread Error");
         return;
      }
   }

   /************************ processRequest *******************/
   void * ThreadManager::processRequest(void *cliSocketP)
   {
      // copy the client socket
      int cliSock = *static_cast<int *>(cliSocketP);
      delete static_cast<int *>(cliSocketP);

      char pack[BUFSIZ];				// package to be the cache between client 
      // and  server(here server is mean the server 
      // which provide the resources that client 
      // ask for, below is the same meaning)
      int bytesRead;
      SocketManager serSockMan;			// server socket manager
      int serSock;					// server socket

      if ((bytesRead = read(cliSock, pack, BUFSIZ))) 
      {						
         // Get the HTTP command and arguments
         if (bytesRead < 0 && errno != EINTR)
         {
            perror("Read Client Error");
            close(cliSock);
            return NULL;
         }

         // set end to the pack
         pack[bytesRead] = '\0';

         // make up the HTTP package
         HTTPPack firstHttpPack(pack);

         // Is the package valid?
         if (!firstHttpPack.isValid())
         {
            close(cliSock);
            return NULL;
         }

         // connect to server
         serSock = serSockMan.
            makeClientSocket(firstHttpPack.getHostName().c_str(), 
                             firstHttpPack.getHostPort());
         if (serSock <= 0 && errno != EINTR) 
         {
            perror("Connect Server Error");
            myWrite(cliSock, 
                    MS_CANNT_GET_MSG.c_str(),
                    MS_CANNT_GET_MSG.length());
            close(cliSock);
            return NULL;
         }

         // send the request to server
         fputs(firstHttpPack.getPack().c_str(), debugFile);
         if(myWrite(serSock, 
                    firstHttpPack.getPack().c_str(), 
                    firstHttpPack.getPack().length() * sizeof(char)) == -1) 
         {
            perror("Write to Server Failure");
            close(cliSock);
            return NULL;
         }

         transBetSerAndCli(serSock,cliSock);

      }

      close(cliSock);
      return NULL;
   } // processRequest

   /************************ tansBetSerAndCli *****************/
   void ThreadManager::transBetSerAndCli(int serSock, int cliSock)
   {
      int iolen;					// read length
      char buf [BUFSIZ];				// read buf
      fd_set rdfdset;					// read file set
      struct timeval timeout;				// time out value
      int selVal;					// select value

      // wait for client and server for 1 seconds
      timeout.tv_sec = 1;				
      timeout.tv_usec = 0;

      while (1)
      {
         // Select for readability on either of our two sockets 
         FD_ZERO(&rdfdset);
         FD_SET(cliSock, &rdfdset);
         FD_SET(serSock, &rdfdset);
         if ((selVal = select(FD_SETSIZE, &rdfdset, NULL, NULL, &timeout)) < 0) 
         {
            perror("Select Failed");
            break;
         }

         if(selVal == 0)
            break;

         // is the client sending data? 
         if (FD_ISSET(cliSock, &rdfdset))
         {
            // zero length means the client disconnected 
            if ((iolen = read(cliSock, buf, sizeof(buf))) <= 0)
               break; 			

            // copy to host -- blocking semantics
            fputs(buf, debugFile);
            myWrite(serSock, buf, iolen); 	 
         }

         // is the host sending data?
         if (FD_ISSET(serSock, &rdfdset))
         {
            // zero length means the host disconnected
            if ((iolen = read(serSock, buf, sizeof(buf))) <= 0)
               break; 			 

            // copy to client -- blocking semantics
            myWrite(cliSock, buf, iolen); 	 
         }
      }
   } // tranBetSerAndCli

   int myWrite(int fd, const void *buf, int count)
   {
      int bytesWrite;					// bytes write
      char *ptr = (char *)buf;			// write buffer pointer

      while((bytesWrite = write(fd,ptr,count))) 
      {
         // a critcal error happen
         if((bytesWrite == -1) && (errno != EINTR))
            break;
         // all bytes has been writed
         else if(bytesWrite == count)
            break;	
         // part of the bytes has been writed, contiue to write
         else if(bytesWrite > 0) 
         {
            ptr += bytesWrite;
            count -= bytesWrite;
         }
      }

      if(bytesWrite == -1)
         // write failure
         return -1;						
      else
         // write success
         return 0;				
   } // myWrite

} // namespace proxyserver
