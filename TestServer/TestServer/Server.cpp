#pragma comment(lib,"ws2_32.lib")
//服务器程序
#include <Winsock2.h>
#include <stdio.h>

int main()
{
	//进行版本协商
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )                             
		return -1;
	
	
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
         HIBYTE( wsaData.wVersion ) != 1 ) 
	{

		WSACleanup( );
		return -1; 
	}

	SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(addrSrv));

	char recvBuf[100];
	char sendBuf[100];
	char tempBuf[100];

	SOCKADDR_IN addClient;
	int len = sizeof(SOCKADDR);
	while(1)
	{
		//接收数据
		recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addClient,&len);
		//判断对话是否应该被终止
		if('#' == recvBuf[0])
		{
			sendto(sockSrv,"#",strlen("#")+1,0,(SOCKADDR*)&addClient,len);
			printf_s("chat end!\n");
			break;
		}

		//打印接收的数据
		sprintf(tempBuf,"%s says: %s",inet_ntoa(addClient.sin_addr),recvBuf);
		printf_s("%s\n",tempBuf);

		//发送数据
		printf_s("please input data:\n");
		gets(sendBuf);
		sendto(sockSrv,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addClient,len);

	}

	closesocket(sockSrv);
	WSACleanup( );
	return 0;
}
