
// xhRegAppleIdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xhRegAppleId.h"
#include "xhRegAppleIdDlg.h"
#include "accountInfo.h"
#include "DataProvider.h"
#include "cryptAlgorithm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


typedef BOOL (* PINITIALIZEMODULE)(LPVOID strPath);
typedef void (* PUNINITIALIZEMODULE)();
typedef LPVOID (*PXHCREATENETWORKOBJECT)();
typedef VOID (*PXHRELEASENEWTWORKOBJECT)(LPVOID lpNetwork);
typedef int (* PXHREGISTERAPPLEID)(IN LPVOID lpNetwork,TAGREGISTERINFO& tagRegisterInfo);

PINITIALIZEMODULE				pInitialModule = NULL;
PUNINITIALIZEMODULE				pUninitialModule = NULL;
PXHCREATENETWORKOBJECT			pCreateNetworkObject = NULL;
PXHRELEASENEWTWORKOBJECT		pReleaseNetworkObject = NULL;
PXHREGISTERAPPLEID				pRegisterAppleId = NULL; 

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CxhRegAppleIdDlg �Ի���




CxhRegAppleIdDlg::CxhRegAppleIdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CxhRegAppleIdDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CxhRegAppleIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CxhRegAppleIdDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_REGISTER_START, &CxhRegAppleIdDlg::OnBnClickedBtnRegisterStart)
END_MESSAGE_MAP()


// CxhRegAppleIdDlg ��Ϣ�������

BOOL CxhRegAppleIdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CxhRegAppleIdDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CxhRegAppleIdDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CxhRegAppleIdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CxhRegAppleIdDlg::InitAndLoadModule()
{
	HMODULE hIntwork = NULL;
	HMODULE hxhiTunes = NULL;

	//�����ַ���
	char srcData[] = "iTunesCore.dll";
	int dataLen = strlen(srcData);
	char key[] = "xhiTunes";
	kEncodeData(srcData,key);
	
	hIntwork = LoadLibrary(_T("xhwork.dll"));
	if (hIntwork == NULL)
	{
		return;
	}

	hxhiTunes = LoadLibrary(_T("xhiTunes.dll"));
	if (hxhiTunes == NULL)
	{
		return;
	}

	pInitialModule = (PINITIALIZEMODULE)GetProcAddress(hxhiTunes,"InitializeModule");
	if (!pInitialModule)
	{
		return;
	}

	pUninitialModule = (PUNINITIALIZEMODULE)GetProcAddress(hxhiTunes,"UnInitializeModule");
	if (!pUninitialModule)
	{
		return;
	}
	pCreateNetworkObject = (PXHCREATENETWORKOBJECT)GetProcAddress(hIntwork,"xhCreateNetworkObject");
	if (!pCreateNetworkObject)
	{
		return;
	}
	pReleaseNetworkObject = (PXHRELEASENEWTWORKOBJECT)GetProcAddress(hIntwork,"xhReleaseNewtworkObject");
	if (!pReleaseNetworkObject)
	{
		return;
	}
	pRegisterAppleId = (PXHREGISTERAPPLEID)GetProcAddress(hIntwork,"xhRegisterAppleId");
	if (!pRegisterAppleId)
	{
		return;
	}

	BOOL Result = pInitialModule((LPVOID)srcData);
	if (!Result)
	{
		return;
	}

}

void CxhRegAppleIdDlg::UnInitAndUnLoadModule()
{
	pUninitialModule();
}

UINT WINAPI  CxhRegAppleIdDlg::registerIdThread(LPVOID lPtr)
{
	CxhRegAppleIdDlg* pThis = (CxhRegAppleIdDlg*)lPtr;
	_ASSERTE( pThis != NULL );

	int					status = 0;
	TAGREGISTERINFO		tagRegInfo;
	ACCOUNTINFO			appleIdInfo = {0};
	CDataProvider		registerData;
	
	//��ʼ������ģ��
	pThis->InitAndLoadModule();

	//����ע����Ϣ
	registerData.CreateUserData();

	wcscpy(appleIdInfo.strAppleId,registerData.GetEmailAddressW().c_str());
	wcscpy(appleIdInfo.strPassword,registerData.GetPasswordW().c_str());
	tagRegInfo.accountInfo.push_back(appleIdInfo);

	//ж��ģ��
	pThis->UnInitAndUnLoadModule();
	return 0;
}



void CxhRegAppleIdDlg::OnBnClickedBtnRegisterStart()
{
	// TODO: Add your control notification handler code here
	//�����߳�
	DWORD dwThreadId = 0;
	HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)registerIdThread,this,0,&dwThreadId);

}
