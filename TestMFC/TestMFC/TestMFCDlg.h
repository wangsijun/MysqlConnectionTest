
// TestMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxsock.h"

// CTestMFCDlg �Ի���
class CTestMFCDlg : public CDialogEx
{
// ����
public:
	CTestMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	int port;
public:
	CIPAddressCtrl ipAddress;
	//���а汾Э��
	WORD wVersionRequested;	
	//WSAData�ṹ
	WSADATA wsaData;
	//�׽���
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