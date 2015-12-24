#pragma comment(lib,"ws2_32.lib")
//�ͻ��˳���
#include <Winsock2.h>
#include <stdio.h>


int main()
{
	//���а汾Э��
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

	//�����׽���
	SOCKET sockClient = socket(AF_INET,SOCK_DGRAM,0);

	/* ���Socket�ͻ��˳���Ҫ���ܹ����ܷ������˷��͹��������ݡ�
	   һ�ַ�ʽ�Ƿ�������ֱ�ӽ��ܺ󷵻�һ����Ϣ������һ�ַ�ʽ
	   ���ǿͻ����Լ�����һ��������Ϣ�Ķ˿ڡ�
	 */
	//������Ϣ
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//Э����  
	addrSrv.sin_family = AF_INET;
	//�˿���Ϣ������ʹ��1024���ϣ�ע���ֽ���ת��  
	addrSrv.sin_port = htons(8000);
	//���׽���  
	bind(sockClient,(SOCKADDR*)&addrSrv,sizeof(addrSrv));



	//Զ�̷�������Ϣ
	SOCKADDR_IN addrClient;
	addrClient.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//Э����  
	addrClient.sin_family = AF_INET;
	//�˿���Ϣ������ʹ��1024���ϣ�ע���ֽ���ת��  
	addrClient.sin_port = htons(6000);
	int len = sizeof(SOCKADDR);
	while(1)
	{
		char recvBuf[100];
		char sendBUf[100];
		char tempBuf[100];

		//�ͻ����߷������ݣ�

		//���뷢������
		printf_s("please input data:\r\n");
		gets(sendBUf);
		//��������
		sendto(sockClient,sendBUf,100,0,(SOCKADDR*)&addrClient,len);
		//��������
		recvfrom(sockClient,recvBuf,100,0,(SOCKADDR*)&addrClient,&len);
		//�ж϶Ի��Ƿ���ֹ
		if('#' == recvBuf[0])
		{
			sendto(sockClient,"#",strlen("#")+1,0,(SOCKADDR*)&addrClient,len);
			printf_s("chat over!");
			break;
		}
		
		//��ӡ���յ�����
		sprintf(tempBuf,"\r\n%s say: %s",inet_ntoa(addrSrv.sin_addr),recvBuf);
		printf_s("%s",tempBuf);

	}
	closesocket(sockClient);
	WSACleanup( );
	return 0;
}