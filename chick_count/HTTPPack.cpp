/***************************************************************//**
 * @file HTTPPack.cpp
 *
 * @brief HTTPPack class definition
 *
 * @author AD_Li
 *
 * @date Sun Dec 16 16:24:44 CST 2007
 *******************************************************************/

#include "HTTPPack.h"

namespace proxyserver
{
	/************************ HTTPPack *************************/
	HTTPPack::HTTPPack(const char *pack)
		:m_pack(pack)
	{
		char hostNameBuf[BUFSIZ];
		char cmdBuf[BUFSIZ];
		
		// initialize the package to be invalid
		m_valid = false;

		// initialize the host port to be HTTP server default port
		m_hostPort = GS_DEFAULT_HTTP_PORT;

		if (sscanf(m_pack.c_str(), "%s http://%s", 
			cmdBuf, hostNameBuf) != 2)
		{
			fprintf(stderr,
				"Invalid HTTP Package: Bad Format\n");

			return;
		}
		
		// scan host name		
		int 	scanCount = 0,
			totalLen = strlen(hostNameBuf);
		scanCount = 0;
		while (hostNameBuf[scanCount] != '/' 
			&& scanCount <= totalLen)
		{	
			// special HTTP server port
			if (hostNameBuf[scanCount] == ':')
                        {
				if (sscanf(hostNameBuf + scanCount + 1,
					"%d", &m_hostPort) != 1)
				{
					fprintf(stderr,
						"Invalid HTTP Package: Bad Server Port\n");

					return;
				}
				else
				{
					break;
				}
                        }

			++scanCount;
		}

		// get the host name
		if(scanCount > totalLen)
		{
			fprintf(stderr,
				"Invalid HTTP Package: Can't Get the Host\n");

			return;
		}
		else 
		{
			hostNameBuf[scanCount] = '\0';
			m_hostName = hostNameBuf;
		}
		
		m_valid = true;

	} // HTTPPack
}
