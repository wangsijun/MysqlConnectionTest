
// TestMFCDlg.cpp : ʵ���ļ�
//
#pragma comment(lib,"ws2_32.lib")
#include <Winsock.h>  
#include "stdafx.h"
#include "TestMFC.h"
#include "TestMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFCDlg �Ի���



CTestMFCDlg::CTestMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestMFCDlg::IDD, pParent)
	, port(0)
	, content(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, port);
	DDV_MinMaxInt(pDX, port, 0, 65535);
	DDX_Control(pDX, IDC_IPADDRESS1, ipAddress);
	DDX_Text(pDX, IDC_EDIT3, content);
}

BEGIN_MESSAGE_MAP(CTestMFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestMFCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestMFCDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTestMFCDlg ��Ϣ�������

BOOL CTestMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestMFCDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	InitSocket();
	handle = CreateThread(NULL,0,Thread1Proc,this,0,NULL);
}

BOOL CTestMFCDlg::InitSocket(){
	//��ʼ���汾Э��
	wVersionRequested = MAKEWORD( 1, 1 );  
	err = WSAStartup( wVersionRequested, &wsaData );
	//����ֵ���������0����˵����ʼ��ʧ��
	if(err!=0){
		GetDlgItem(IDC_STATUS)->SetWindowText("��ʼ��WSAʧ��:"+GetLastError());
	}

	if ( LOBYTE( wsaData.wVersion ) != 1 ||  
		HIBYTE( wsaData.wVersion ) != 1 )   
	{  
		WSACleanup( ); 
		return FALSE;
	}  
	//�����׽���
	socketService = socket(AF_INET,SOCK_DGRAM,0);
	//��д��ַ��Ϣ
	SOCKADDR_IN addrSrv;
	//����IP��ַ  
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//Э����  
	addrSrv.sin_family = AF_INET;
	//�˿���Ϣ������ʹ��1024���ϣ�ע���ֽ���ת��  
	addrSrv.sin_port = htons(6000);
	//���׽���  
	bind(socketService,(SOCKADDR*)&addrSrv,sizeof(addrSrv));
	return TRUE;
}
DWORD WINAPI CTestMFCDlg::Thread1Proc( LPVOID lpParameter){

	CTestMFCDlg *p = (CTestMFCDlg *)lpParameter;
	SOCKET socketService = p->socketService;
	

	//ͨ�ŵ�ַ
	SOCKADDR_IN addClient;
	int len = sizeof(SOCKADDR);
	CListBox *pCtr1 = (CListBox *)p->GetDlgItem(IDC_LIST1);

	while(1)
	{
		char recvBuf[100];
		char sendBuf[100];
		char tempBuf[100];
		//��������
		recvfrom(socketService,recvBuf,99,0,(SOCKADDR*)&addClient,&len);

		//�ж϶Ի��Ƿ�Ӧ�ñ���ֹ
		if('#' == recvBuf[0])
		{
			sendto(socketService,"#",strlen("#")+1,0,(SOCKADDR*)&addClient,len);
			//printf_s("chat end!\n");
			pCtr1->AddString("�رշ���");
			break;
		}
		//��ӡ���յ�����

		sprintf(tempBuf,"%s says: %s",inet_ntoa(addClient.sin_addr),recvBuf);
		pCtr1->AddString((CString)tempBuf);
		//printf_s("%s\n",tempBuf);

		//��������
		//printf_s("please input data:\n");
		//gets(sendBuf);  
		//sendto(socketService,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addClient,len);  

	}
	return TRUE;
}
void CTestMFCDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	closesocket(socketService);
	WSACleanup( );
}


void CTestMFCDlg::OnBnClickedButton4()
{
	unsigned char *pIP;   
	DWORD dwIP;    
	ipAddress.GetAddress(dwIP); 
	pIP = (unsigned char*)&dwIP;    
	CString strIP;
	strIP.Format("%u.%u.%u.%u",*(pIP+3), *(pIP+2), *(pIP+1), *pIP);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListBox *pCtr2 = (CListBox *)GetDlgItem(IDC_LIST2);
	//ͨ�ŵ�ַ
	SOCKADDR_IN addClient;
	//����IP��ַ  
	addClient.sin_addr.S_un.S_addr = inet_addr(strIP);  
	//Э����  
	addClient.sin_family = AF_INET;
	//�˿���Ϣ������ʹ��1024���ϣ�ע���ֽ���ת��  
	addClient.sin_port = htons(8000);
	//�ͻ��˵�ַ����
	int len =sizeof(SOCKADDR);
	const size_t newsize =100; 
	char sendBuf[newsize];
	char tempBuf[newsize];
	UpdateData(TRUE);
	strcpy_s(sendBuf,content); 
	sendto(socketService,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addClient,len);
	//sprintf(tempBuf,"send: %s",sendBuf);
	pCtr2->AddString(content);
}


void CTestMFCDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CListBox *pCtr1 = (CListBox *)GetDlgItem(IDC_LIST1);
	pCtr1->AddString("haha");
}
