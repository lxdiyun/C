#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc,char **argv) {
	struct sockaddr_in addr;
	struct hostent *host;
	char **alias;

	if(argc < 2) {
		fprintf(stderr,"Usage:%s hostname|ip..\n\a",argv[0]);
		exit(1);
	}

	++argv;
	for(; *argv != NULL; ++argv) {
		// 假设输入为IP
		if(inet_aton(*argv,&addr.sin_addr) != 0) {
			if((host = gethostbyaddr((char *)&addr.sin_addr,
				4,AF_INET)) == NULL) {
				fprintf(stderr,"No address information of IP %s\n\n",*argv);
				continue;
			}
			printf("Address information of IP %s\n",*argv);
		} else if((host = gethostbyname(*argv)) != NULL) {
		// 假设输入为IP失败,假设输入为域名
			printf("Address information of host %s\n",*argv);
		} else {
		// 假设失败
			fprintf(stderr,"No address information of %s\n\n",*argv);
			continue;
		}

		printf("Offical host name %s\n",host -> h_name);
		printf("Name aliases:");
		for(alias = host -> h_aliases; *alias != NULL; ++alias)
			printf("%s,",*alias);
		printf("\nIP address:");
		for(alias  = host -> h_addr_list; *alias != NULL; ++alias)
			printf("%s,",inet_ntoa(*(struct in_addr *)(*alias)));
		printf("\n\n");
	}
	exit(0);
}
