#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc,char *args[]){
    
    if(2>argc){
        printf("you must input a domain name!\n ");
        return 0;
    }
    
    const char * name = args[1];
    
    struct in_addr in;
    struct sockaddr_in local_addr;
    
    struct hostent * ht = gethostbyname(name);
    memcpy(&local_addr.sin_addr.s_addr,ht->h_addr,4);
    in.s_addr = local_addr.sin_addr.s_addr;
    
    printf("///////////////////////////////////////////////////\n");
    printf("Domain Name: %s\n",ht->h_name);
    printf("IP address: %s\n",inet_ntoa(in));

    printf("///////////////////////////////////////////////////\n");
        return 0;
}
