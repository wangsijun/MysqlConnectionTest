
// TestMFCDlg.cpp : 实现文件
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


// CTestMFCDlg 对话框



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


// CTestMFCDlg 消息处理程序

BOOL CTestMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestMFCDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	InitSocket();
	handle = CreateThread(NULL,0,Thread1Proc,this,0,NULL);
}

BOOL CTestMFCDlg::InitSocket(){
	//初始化版本协商
	wVersionRequested = MAKEWORD( 1, 1 );  
	err = WSAStartup( wVersionRequested, &wsaData );
	//返回值如果不等于0，就说明初始化失败
	if(err!=0){
		GetDlgItem(IDC_STATUS)->SetWindowText("初始化WSA失败:"+GetLastError());
	}

	if ( LOBYTE( wsaData.wVersion ) != 1 ||  
		HIBYTE( wsaData.wVersion ) != 1 )   
	{  
		WSACleanup( ); 
		return FALSE;
	}  
	//创建套接字
	socketService = socket(AF_INET,SOCK_DGRAM,0);
	//填写地址信息
	SOCKADDR_IN addrSrv;
	//本机IP地址  
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//协议族  
	addrSrv.sin_family = AF_INET;
	//端口信息，必须使用1024以上，注意字节序转换  
	addrSrv.sin_port = htons(6000);
	//绑定套接字  
	bind(socketService,(SOCKADDR*)&addrSrv,sizeof(addrSrv));
	return TRUE;
}
DWORD WINAPI CTestMFCDlg::Thread1Proc( LPVOID lpParameter){

	CTestMFCDlg *p = (CTestMFCDlg *)lpParameter;
	SOCKET socketService = p->socketService;
	

	//通信地址
	SOCKADDR_IN addClient;
	int len = sizeof(SOCKADDR);
	CListBox *pCtr1 = (CListBox *)p->GetDlgItem(IDC_LIST1);

	while(1)
	{
		char recvBuf[100];
		char sendBuf[100];
		char tempBuf[100];
		//接收数据
		recvfrom(socketService,recvBuf,99,0,(SOCKADDR*)&addClient,&len);

		//判断对话是否应该被终止
		if('#' == recvBuf[0])
		{
			sendto(socketService,"#",strlen("#")+1,0,(SOCKADDR*)&addClient,len);
			//printf_s("chat end!\n");
			pCtr1->AddString("关闭服务");
			break;
		}
		//打印接收的数据

		sprintf(tempBuf,"%s says: %s",inet_ntoa(addClient.sin_addr),recvBuf);
		pCtr1->AddString((CString)tempBuf);
		//printf_s("%s\n",tempBuf);

		//发送数据
		//printf_s("please input data:\n");
		//gets(sendBuf);  
		//sendto(socketService,sendBuf,strlen(sendBuf)+1,0,(SOCKADDR*)&addClient,len);  

	}
	return TRUE;
}
void CTestMFCDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CListBox *pCtr2 = (CListBox *)GetDlgItem(IDC_LIST2);
	//通信地址
	SOCKADDR_IN addClient;
	//本机IP地址  
	addClient.sin_addr.S_un.S_addr = inet_addr(strIP);  
	//协议族  
	addClient.sin_family = AF_INET;
	//端口信息，必须使用1024以上，注意字节序转换  
	addClient.sin_port = htons(8000);
	//客户端地址长度
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
	// TODO: 在此添加控件通知处理程序代码
	CListBox *pCtr1 = (CListBox *)GetDlgItem(IDC_LIST1);
	pCtr1->AddString("haha");
}
