#include "stdio.h"
#include "string.h"
#include "opt_test.h"
#include "sys/socket.h"
#include "errno.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdlib.h"

void socketInit(char **strI,char **strP);

int main(int argc,char **argv)
{
	char *strI;
	char *strP;

	argumentParse(argc,argv,&strI,&strP);
	printf("socketInit before strI = %s strP = %s\n",strI,strP);
        socketInit(&strI,&strP);
}

void socketInit(char **strI,char **strP)
{
	int sockfd = -1;
	int listenfd = -1;

	int connectNum = 5; 
       	int opt =  1;
	struct sockaddr_in server_addr;

	/*创建套接字*/
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	printf("sockFd value:%d\n",sockfd);
	if(sockfd == -1)
	{
		printf("create socket error:%s\n",strerror(errno));
		exit(0);
	}

	/*将本地ipv4地址和端口填入文件描述符表中*/
        server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(**strP);
	printf("server_addr.sin_port value :%d\n",server_addr.sin_port);
	printf("socketInit strI = %s\n",*strI);

	if(*strI != NULL)
	{
		if(inet_aton(*strI,&server_addr.sin_addr) < 0) //将点分十进制ipv4地址转换为网络字节序地址
		{
			printf("ipv4 tranform error:%s\n",strerror(errno));
			exit(0);
		
		}
		else
		{
			printf("server_addr.sin_addr.s_addr:%d\n",server_addr.sin_addr.s_addr);
		}
	
	}

	else
	{
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//指定绑定任意ipv4地址	
	}


	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&opt,sizeof(opt)) < 0)//设置端口复用，防止被占用
	{
		printf("set port reuse error:%s\n",strerror(errno));
		exit(0);
	}

	if(bind(sockfd,(const struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
	{
		printf("bind ip port error:%s\n",strerror(errno));
		exit(0);
	}

	/*监听socket，等待客户端连接触发并指定服务器可排队连接的最大数*/		
        listenfd  = listen(sockfd,connectNum);
	if(listenfd < 0)
	{
		printf("listen server socket error:%s\n",strerror(errno));
		exit(0);
	}

}
