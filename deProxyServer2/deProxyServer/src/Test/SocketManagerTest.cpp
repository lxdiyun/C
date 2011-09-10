/***************************************************************//**
 * @file SocketManagerTest.cpp
 * 
 * @brief SocketManager 类示例和测试
 *
 * SocketManager 类的测试文件以及测试函数
 *
 * @author AD_Li
 *
 * @date Wed Dec 12 19:02:54 CST 2007
 *
 *******************************************************************/

#include <assert.h>

#include "../connection/SocketManager.h"

using namespace proxyserver::connection;

/** 服务端口  */
const static int GS_SERVER_PORT = 6688;
/** 默认包大小  */
const static int GS_PACK_LEN = 256;

/***************************************************************//**
 * @brief SocketManager 类测试与使用示例
 *
 * 建立一服务器端和一客户端互相传递信息,并校验信息
 */
int main(int argc, char *argv[]) 
{
	const string testStr = "Test String.";			// test string
	int procID;

	if ((procID = fork()) > 0)
	{// server	
		SocketManager serSockMan;			// server socket manager

		int serSock = serSockMan.			// server socket
			makeServerSocket(GS_SERVER_PORT);

		int fd = accept(serSock,NULL,NULL);		// file ID
		if (fd == -1)
			perror("Server Accept Failure");

		write(fd,testStr.c_str(),			// write the message
			testStr.length() * sizeof(testStr[0])
		);
		close(fd);
	}
	else if (procID == 0)
	{// client
		sleep(1);					// wait for the server

		SocketManager cliSockMan;			// client socket manager
		
		int cliSock = cliSockMan.			// client socket
			makeClientSocket("localhost",
				GS_SERVER_PORT);

		char read_buf[GS_PACK_LEN];			// read buffer

		// read the message
		int readBytes = read(cliSock,read_buf,GS_PACK_LEN);	
		read_buf[readBytes] = '\0';

		string cmpStr(read_buf);

		// compare message
		assert(testStr.compare(cmpStr) == 0);
	}
	else 
	{
		perror("Fork Error");
	}

	return 0;
}
