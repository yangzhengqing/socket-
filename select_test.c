#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "errno.h"
#include "sys/select.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "sys/time.h"
#include "select_test.h"

/*测试多路复用select*/
int main(int argc,char **argv)
{
	server_select_create();

	return 0;
}

int server_select_create(void)
{
	fd_set readset;
//	fd_set writeset;
//	fd_set exceptset;
	int listenfd = 3;
	int maxfd = 0;
	int connectfd;
	int rvselect;	
	int i = 0;
	int client_length;
	struct timeval timeout;
	struct sockaddr_in client_addr;
	int fdArray[FDARRAYNUM];//用于比较出最大的文件描述符

	memset(fdArray,0,sizeof(fdArray));
	fdArray[0] = listenfd;

	/*初始化超时时间*/
	timeout.tv_sec  = TVSEC;
	timeout.tv_usec = TVUSEC;

	/*擦除文件描述符集结构体*/
	FD_ZERO(&readset);
//	FD_ZERO(listenfd,&writeset);
//	FD_ZERO(listenfd,&exceptset);

	/*将文件描述符添加至文件描述符集中*/
	FD_SET(listenfd,&readset);
//	FD_SET(listenfd,&writeset);
//	FD_SET(listenfd,&exceptset);
	while(1)
	{
		for(i = 0;i<FDARRAYNUM;i++)//找出最大的文件描述符
		{
			maxfd = maxfd > fdArray[i]?maxfd:fdArray[i]; 
		}	

		printf("maxfd = %d\n",maxfd);
		rvselect = select(maxfd+1,&readset,NULL,NULL,&timeout);//使用于非阻塞socket中同时监听多个文件描述符
		switch(rvselect)
		{
			case -1:
				printf("select error:%s\n",strerror(errno));
				return -1;

			case 0:
				printf("select wait timeout:%s\n",strerror(errno));
				break;
			default:
				if(FD_ISSET(connectfd,&readset))//判断是否是被监听中的描述符发生事件
				{
					printf("previous connectfd have data read...\n");
				}
				else
				{
					connectfd = accept(listenfd,(struct sockaddr *)&client_addr,&client_length);//接收客户端连接请求
					if(connectfd < 0)
					{
						printf("server accepted client[%s] request error:%s\n",client_addr.sin_addr.s_addr,strerror(errno));
					}
					else
					{
						printf("server accepted client[%s] request successfully.\n");
						FD_SET(connectfd,&readset);//将新的描述符添加到文件描述符集中
						for(i = 0;i<FDARRAYNUM;i++)
						{
							if(fdArray[i] == 0)
							{
								fdArray[i] = connectfd;
								break;
							}
						}
					}
				}
		}


	}



}
