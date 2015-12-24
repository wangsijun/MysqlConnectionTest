
// TestMFCDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxsock.h"

// CTestMFCDlg 对话框
class CTestMFCDlg : public CDialogEx
{
// 构造
public:
	CTestMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	int port;
public:
	CIPAddressCtrl ipAddress;
	//进行版本协商
	WORD wVersionRequested;	
	//WSAData结构
	WSADATA wsaData;
	//套接字
	SOCKET socketService;
	int err;
	BOOL InitSocket();
	static DWORD WINAPI Thread1Proc(LPVOID lpParameter); 
	HANDLE handle;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	
	
	CString content;
};
class PARAM {
public:
	SOCKET socket;
};