#pragma comment(lib,"ws2_32.lib")
//客户端程序
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

	//创建套接字
	SOCKET sockClient = socket(AF_INET,SOCK_DGRAM,0);

	/* 这个Socket客户端程序，要向能够接受服务器端发送过来的数据。
	   一种方式是服务器端直接接受后返回一段信息，还有一种方式
	   就是客户端自己监听一个接受消息的端口。
	 */
	//本地信息
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//协议族  
	addrSrv.sin_family = AF_INET;
	//端口信息，必须使用1024以上，注意字节序转换  
	addrSrv.sin_port = htons(8000);
	//绑定套接字  
	bind(sockClient,(SOCKADDR*)&addrSrv,sizeof(addrSrv));



	//远程服务器信息
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//协议族  
	addrClient.sin_family = AF_INET;
	//端口信息，必须使用1024以上，注意字节序转换  
	addrClient.sin_port = htons(6000);
	int len = sizeof(SOCKADDR);
	while(1)
	{
		char recvBuf[100];
		char sendBUf[100];
		char tempBuf[100];

		//客户端线发送数据：

		//输入发送数据
		printf_s("please input data:\r\n");
		gets(sendBUf);
		//发送数据
		sendto(sockClient,sendBUf,100,0,(SOCKADDR*)&addrClient,len);
		//接收数据
		recvfrom(sockClient,recvBuf,100,0,(SOCKADDR*)&addrClient,&len);
		//判断对话是否终止
		if('#' == recvBuf[0])
		{
			sendto(sockClient,"#",strlen("#")+1,0,(SOCKADDR*)&addrClient,len);
			printf_s("chat over!");
			break;
		}
		
		//打印接收的数据
		sprintf(tempBuf,"\r\n%s say: %s",inet_ntoa(addrSrv.sin_addr),recvBuf);
		printf_s("%s",tempBuf);

	}
	closesocket(sockClient);
	WSACleanup( );
	return 0;
}