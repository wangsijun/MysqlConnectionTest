
// TestADODlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestADO.h"
#include "TestADODlg.h"
#include "afxdialogex.h"
#include "ADOConn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestADODlg 对话框



CTestADODlg::CTestADODlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CTestADODlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestADODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, dataList);
}

BEGIN_MESSAGE_MAP(CTestADODlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestADODlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestADODlg 消息处理程序

BOOL CTestADODlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestADODlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestADODlg::OnPaint()
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
HCURSOR CTestADODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestADODlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_STATIC)->SetWindowText("未链接");
	ADOConn conn;
	conn.OnInitADOConn();
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pRecordset = conn.GetRecordSet("SELECT * FROM AT_User WITH(NOLOCK);");
	try
	{
		m_pRecordset->MoveFirst();
		while (m_pRecordset->adoEOF == VARIANT_FALSE){
			//char* sName = (char*)(_bstr_t)(m_pRecordset->Fields->GetItem(_variant_t("Photo"))->Value);
			//char* TheValue1 = (char*)(_bstr_t)m_pRecordset->Fields->GetItem("Photo")->Value;

			//取纪录字段值方式之一 			_variant_t vValue = m_pRecordset->GetCollect("Photo"); //VARIANT数据类型 //得到字段BIG_NAME的值 			if (vValue.vt != VT_NULL)				AfxMessageBox((char*)_bstr_t(vValue));				//dataList.AddString((char*)_bstr_t(vValue));

			//dataList.AddString(TheValue1);
			conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error &e){
		CString str = (char*)e.Description();      
		::MessageBox(NULL, str + "\n又出毛病了。", "提示", MB_OK | MB_ICONWARNING);
	}
}