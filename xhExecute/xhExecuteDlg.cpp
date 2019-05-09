
// xhExecuteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xhExecute.h"
#include "xhExecuteDlg.h"
#include "cryptAlgorithm.h"
#include "statusData.h"
#include "memoryshared.h"
#include <iostream>
#include <string>
#include <Winsock2.h>
#include <iphlpapi.h>
#include<locale.h> 
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define     BUFFER_SIZE        7168
#define     MAX_TITLE          64
#define     MAX_BODY           512
#define     MAX_KEY_WORD       64  
#define		MAINTITLE			TEXT("ƻ���̵�APP�Ż�����")
typedef BOOL (* PINITIALIZEMODULE)(LPVOID strPath);
typedef void (* PUNINITIALIZEMODULE)();
PINITIALIZEMODULE pInitialModule = NULL;
PUNINITIALIZEMODULE pUninitialModule = NULL;

typedef VOID (*PXHINITCRITICALSECTION)();
typedef VOID (*PXHDELETECRITICALSECTION)();
typedef LPVOID (*PXHCREATENETWORKOBJECT)();
typedef VOID (*PXHRELEASENEWTWORKOBJECT)(LPVOID lpNetwork);

typedef int (*PXHGSA_APPLE_NETWORKVERIFIER)(IN LPVOID lpNetwork,IN LPVOID lpUdid,PMODELINFO pModInfo,PDEVICEINFO pDeviceInfo,TCHAR* lpProxy,int netType);

typedef int (*PXHLOGINAPPSTORE)(IN LPVOID lpNetwork,
								 IN LPVOID lpUserID,
								 IN LPVOID lpPassword,
								 IN LPTSTR lpwFatherAndMother,
								 IN LPTSTR lpwTeacher,
							     IN LPTSTR lpwBook,
								 IN PDEVICEINFO pDeviceInfo,
								 OUT LPVOID lpPasswordToken,
								 OUT LPVOID lpDsid,
								 TCHAR* lpProxy,
								 int netType);

typedef int (*PXHDOWNLOADAPP)(IN LPVOID lpNetwork,
							   IN LPVOID lpPasswordToken,
							   IN LPVOID lpDsid,
							   IN LPVOID lpSaleId,
							   IN LPVOID lpAppPrice,
							   BOOL bSendReport,
							   BOOL bPaidApp);
typedef int (*PXHCOMMENTAPP)(IN LPVOID lpNetwork,
							  IN LPVOID lpPasswordToken,
							  IN LPVOID lpDsid,
							  IN LPVOID lpSaleId,
							  IN int rating,
							  IN LPTSTR lpwNickname,
							  IN LPTSTR lpwTitle,
							  IN LPTSTR lpwBody);
typedef int (*PXHSEARCHKEYHOTWORD)(IN LPVOID lpNetwork,
									IN LPTSTR strwKeyword,
									IN LPVOID lpSaleId,
									IN LPVOID lpDsid,
									IN LPVOID lpPasswordToken,
									IN LPVOID lpTargetUrl);

typedef int (*PXHGETAPPEXTVRSIDVALUE)(IN LPVOID lpNetwork,
									   IN LPVOID lpPasswordToken,
									   IN LPVOID lpDsid,
									   IN LPVOID lpTargetUrl);

typedef int (*PXHBUYCONFIRMACTIVEAPP)(IN LPVOID lpNetwork,
									   IN LPVOID lpPasswordToken,
									   IN LPVOID lpSaleId,
									   IN LPVOID lpDsid,
									   IN LPVOID lpTargetUrl);

typedef  int (*PXHONLYSEARCHKEYWORD)(IN LPVOID lpNetwork,IN LPTSTR strwKeyword);

PXHINITCRITICALSECTION pInitCriticalSection = NULL;
PXHDELETECRITICALSECTION pDeleteCriticalSection = NULL;
PXHCREATENETWORKOBJECT pCreateNetworkObject = NULL;
PXHRELEASENEWTWORKOBJECT pReleaseNetworkObject = NULL;
PXHGSA_APPLE_NETWORKVERIFIER pGSA_Apple_NetworkVerifier = NULL;
PXHLOGINAPPSTORE pLoginAppStore = NULL; 
PXHDOWNLOADAPP pDownloadApp = NULL;
PXHCOMMENTAPP pCommentApp = NULL;
PXHSEARCHKEYHOTWORD pSearchKeyHotWord = NULL;
PXHGETAPPEXTVRSIDVALUE pGetAppExtVrsIdValue = NULL;
PXHBUYCONFIRMACTIVEAPP pBuyConfirmActiveApp = NULL;
PXHONLYSEARCHKEYWORD pOnlySearchKeyword = NULL;

int    g_status = 0;

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


// CxhExecuteDlg �Ի���




CxhExecuteDlg::CxhExecuteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CxhExecuteDlg::IDD, pParent)
{
	m_hPipeRead = NULL;
	m_hPipeWrite = NULL;
	m_hEvent = NULL;
}

void CxhExecuteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CxhExecuteDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CxhExecuteDlg ��Ϣ�������

BOOL CxhExecuteDlg::OnInitDialog()
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
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	int ret = 0;
	SENDINFO recvInfo = {0};
	LOGINFO logInfo = {0};
	try
	{
		if (__argc != 1)
		{
			::ExitProcess(0);
		}

		AfxMessageBox(_T("123"));

		memoryshared ms;
		ms.Open(__wargv[0]);
		if ( 1 == ms.Read((char*)&recvInfo, sizeof(SENDINFO)) )
		{
			//���¼�,֪ͨ�������ִ�н����Ѿ���ȡ������
			m_hEvent = OpenEvent(EVENT_MODIFY_STATE,FALSE,recvInfo.strEventName);
			//�����¼����ź�
			SetEvent(m_hEvent);

			if (m_hEvent)
			{
				CloseHandle(m_hEvent);
			}
			//����ģ���ȡ�ӿ�
			if (!getFunInterface())
			{
				::ExitProcess(0);
			}
			
			//��ʼִ�к��ĳ���
			DWORD dwThreadId = 0;
			HANDLE hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)InterfaceFunction,(LPVOID)&recvInfo,0,&dwThreadId);
			DWORD result = ::WaitForSingleObject(hThread,1000*300);
			if (result == WAIT_TIMEOUT)
			{
				g_status = EXECUTE_TIME_OUT;
			}
			switch(g_status)
			{
			case LOGIN_SUCCESS:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("��¼�ɹ�"));
					logInfo.result = LOGIN_SUCCESS;
				}break;
			case LOGIN_FAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("��¼ʧ��"));
					logInfo.result = LOGIN_FAILED;
				}break;
			case DOWNLOADFAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("����ʧ��"));
					logInfo.result = DOWNLOADFAILED;
				}break;
			case SEARCHFAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("����ʧ��"));
					logInfo.result = SEARCHFAILED;
				}break;
			case COMMENTFAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("����ʧ��"));
					logInfo.result = COMMENTFAILED;
				}
			case DOWNLOADSUCCESS:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�������"));
					logInfo.result = DOWNLOADSUCCESS;
				}break;
			case SEARCHSUCCESS:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�������"));
					logInfo.result = SEARCHSUCCESS;
				}break;
			case COMMENTSUCCESS:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�������"));
					logInfo.result = COMMENTSUCCESS;
				}break;
			case ACTIVEAPPFAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("����ʧ��"));
					logInfo.result = ACTIVEAPPFAILED;
				}break;
			case GSA_FAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("gsaʧ��"));
					logInfo.result = GSA_FAILED;
				}break;
			case EXECUTE_TIME_OUT:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("ִ�г�ʱ"));
					logInfo.result = EXECUTE_TIME_OUT;

				}break;
			case PASSWORD_ERROR:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�������"));
					logInfo.result = PASSWORD_ERROR;
				}break;
			case ACCOUNT_LOCKED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�˺ű���"));
					logInfo.result = ACCOUNT_LOCKED;
				}break;
			case ACCOUNT_DISABLED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�˺Ž���"));
					logInfo.result = ACCOUNT_DISABLED;
				}break;
			case ACCOUNT_FAULT:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�˺���Ϣ��ȫ"));
					logInfo.result = ACCOUNT_FAULT;
				}break;
			case ACCOUNT_STOP:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�˺�ͣ��"));
					logInfo.result = ACCOUNT_STOP;
				}break;
			case PRICE_MISMATCH:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("�۸�ƥ��"));
					logInfo.result = PRICE_MISMATCH;
				}break;
			case UNKNOWN_ERROR:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("δ֪����"));
					logInfo.result = UNKNOWN_ERROR;
				}break;
			case UNKNOWN_DEVICE:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("δ֪�豸"));
					logInfo.result = UNKNOWN_DEVICE;
				}break;
			case BUY_FAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("����ʧ��"));
					logInfo.result = BUY_FAILED;
				}break;
			case STATUS_FAILED:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("ִ��ʧ��"));
					logInfo.result = STATUS_FAILED;
				}break;
			default:
				{
					logInfo.nIndex = recvInfo.accountInfo.nIndex;
					wcscpy(logInfo.strItemLog,TEXT("ִ���쳣"));
					logInfo.result = EXECTUEFAILED;
				}
				break;
			}

			//����WM_COPYDATA��Ϣ���������֪ͨ�������
			COPYDATASTRUCT cpd;
			HWND hWnd = ::FindWindow(NULL,MAINTITLE);
			if (!hWnd)
			{
				pUninitialModule();
				ExitProcess(0);
			}

			cpd.dwData = 0;
			cpd.cbData = sizeof(logInfo);
			cpd.lpData = &logInfo;
			::SendMessage(hWnd,WM_COPYDATA,(WPARAM)this->GetSafeHwnd(),(LPARAM)&cpd);
			pUninitialModule();
			ExitProcess(0);
		}

		ExitProcess(0);
	}
	catch(exception& e)
	{
		ExitProcess(0);
		OutputDebugStringA(e.what());
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CxhExecuteDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CxhExecuteDlg::OnPaint()
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
HCURSOR CxhExecuteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CxhExecuteDlg::getFunInterface()
{
	HMODULE hIntwork = NULL;
	HMODULE hxhiTunes = NULL;
	
	hIntwork = LoadLibrary(_T("xhwork.dll"));
	if (hIntwork == NULL)
	{
		return FALSE;
	}

	hxhiTunes = LoadLibrary(_T("xhiTunes.dll"));
	if (hxhiTunes == NULL)
	{
		return FALSE;
	}

	pInitialModule = (PINITIALIZEMODULE)GetProcAddress(hxhiTunes,"InitializeModule");
	if (!pInitialModule)
	{
		return FALSE;
	}

	pUninitialModule = (PUNINITIALIZEMODULE)GetProcAddress(hxhiTunes,"UnInitializeModule");
	if (!pUninitialModule)
	{
		return FALSE;
	}

	//�����ַ���
	char srcData[] = "iTunesCore.dll";
	int dataLen = strlen(srcData);
	char key[] = "xhiTunes";
	kEncodeData(srcData,key);

	BOOL Result = pInitialModule((LPVOID)srcData);
	if (!Result)
	{
		return FALSE;
	}

	pInitCriticalSection = (PXHINITCRITICALSECTION)GetProcAddress(hIntwork,"xhInitCriticalSection");
	if (!pInitCriticalSection)
	{
		return FALSE;
	}
	pDeleteCriticalSection = (PXHDELETECRITICALSECTION)GetProcAddress(hIntwork,"xhDeleteCriticalSection");
	if (!pDeleteCriticalSection)
	{
		return FALSE;
	}

	pCreateNetworkObject = (PXHCREATENETWORKOBJECT)GetProcAddress(hIntwork,"xhCreateNetworkObject");
	if (!pCreateNetworkObject)
	{
		return FALSE;
	}
	pReleaseNetworkObject = (PXHRELEASENEWTWORKOBJECT)GetProcAddress(hIntwork,"xhReleaseNewtworkObject");
	if (!pReleaseNetworkObject)
	{
		return FALSE;
	}
	pGSA_Apple_NetworkVerifier = (PXHGSA_APPLE_NETWORKVERIFIER)GetProcAddress(hIntwork,"xhGSA_Apple_NetworkVerifier");
	if (!pGSA_Apple_NetworkVerifier)
	{
		return FALSE;
	}

	pLoginAppStore = (PXHLOGINAPPSTORE)GetProcAddress(hIntwork,"xhLoginAppStore");
	if (!pLoginAppStore)
	{
		return FALSE;
	}
	

	pDownloadApp = (PXHDOWNLOADAPP)GetProcAddress(hIntwork,"xhDownloadApp");
	if (!pDownloadApp)
	{
		return FALSE;
	}

	pCommentApp = (PXHCOMMENTAPP)GetProcAddress(hIntwork,"xhCommentApp");
	if (!pCommentApp)
	{
		return FALSE;
	}

	pSearchKeyHotWord = (PXHSEARCHKEYHOTWORD)GetProcAddress(hIntwork,"xhSearchKeyHotWord");
	if (!pSearchKeyHotWord)
	{
		return FALSE;
	}

	pGetAppExtVrsIdValue = (PXHGETAPPEXTVRSIDVALUE)GetProcAddress(hIntwork,"xhGetAppExtVrsIdValue");
	if (!pGetAppExtVrsIdValue)
	{
		return FALSE;
	}

	pBuyConfirmActiveApp = (PXHBUYCONFIRMACTIVEAPP)GetProcAddress(hIntwork,"xhBuyConfirmActiveApp");
	if (!pBuyConfirmActiveApp)
	{
		return FALSE;
	}

	pOnlySearchKeyword = (PXHONLYSEARCHKEYWORD)GetProcAddress(hIntwork,"xhOnlySearchKeyword");
	if (!pOnlySearchKeyword)
	{
		return FALSE;
	}
	return TRUE;
}

UINT WINAPI CxhExecuteDlg::InterfaceFunction(PSENDINFO pSendInfo)
{
	char szPasswordToken[MAX_PATH] = {0};

	char szDsid[64] = {0};
	char szRefferUrl[MAX_PATH] ={0};
	char szPageDetails[512] = {0};
	char szAppleId[MAX_PATH] = {0};
	char szPwd[MAX_PATH] = {0};
	char szAppSaleId[MAX_PATH] = {0};
	char szAppExtVrsId[64] = {0};
	char szRate[10] = {0};
	char szClientId[MAX_PATH] = {0};
	char szTargetUrl[MAX_PATH] = {0};
	char szAppPrice[32] = {0};
	char szUdid[MAX_PATH] = {0};
	int status = 0;

	::WideCharToMultiByte(CP_ACP,0,pSendInfo->strTargetUrl,wcslen(pSendInfo->strTargetUrl),szTargetUrl,MAX_PATH,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,pSendInfo->accountInfo.strAppleId,wcslen(pSendInfo->accountInfo.strAppleId),szAppleId,MAX_PATH,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,pSendInfo->accountInfo.strPassword,wcslen(pSendInfo->accountInfo.strPassword),szPwd,MAX_PATH,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,pSendInfo->strSaleId,wcslen(pSendInfo->strSaleId),szAppSaleId,MAX_PATH,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,pSendInfo->strRate,wcslen(pSendInfo->strRate),szRate,10,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,pSendInfo->strAppPrice,wcslen(pSendInfo->strAppPrice),szAppPrice,32,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,pSendInfo->accountInfo.deviceInfo.strUdid,wcslen(pSendInfo->accountInfo.deviceInfo.strUdid),szUdid,MAX_PATH,NULL,NULL);

	pInitCriticalSection();
	LPVOID lpNetworkObj = pCreateNetworkObject();
	if (!lpNetworkObj)
	{
		return status;
	}

	switch (pSendInfo->type)
	{
		case ONLYLOGIN:
			{
				status = pGSA_Apple_NetworkVerifier(lpNetworkObj,
													szUdid,
													&pSendInfo->modelInfo,
													&pSendInfo->accountInfo.deviceInfo,
													pSendInfo->strProxy,
													pSendInfo->netType);
				if (status != STATUS_SUCCESS)
				{
					return status;
				}
				status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
				if (status == STATUS_FAILED || status == LOGIN_FAILED)//���������������Ҫ���µ�¼һ��
				{
					status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
					
				}
			}break;
		case LOGINDOWNLOADAPP://��¼����
			{
				status = pGSA_Apple_NetworkVerifier(lpNetworkObj,
													szUdid,
													&pSendInfo->modelInfo,
													&pSendInfo->accountInfo.deviceInfo,
													pSendInfo->strProxy,
													pSendInfo->netType);
				if (status != STATUS_SUCCESS)
				{	
					return status;			
				}
				status = pLoginAppStore(lpNetworkObj,
									 szAppleId,
									 szPwd,
									 pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
				if (status == STATUS_FAILED || status == LOGIN_FAILED)//���������������Ҫ���µ�¼һ��
				{
					status = pLoginAppStore(lpNetworkObj,
									 szAppleId,
									 szPwd,
									 pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
				}
				if (status == LOGIN_SUCCESS)
				{

					//��ȡAppExtVrsId��ֵ
					status = pGetAppExtVrsIdValue(lpNetworkObj,szPasswordToken,szDsid,szTargetUrl);
					if (status == STATUS_FAILED)
					{
						status = pGetAppExtVrsIdValue(lpNetworkObj,szPasswordToken,szDsid,szTargetUrl);
					}

					if (status == STATUS_SUCCESS)
					{
						status = pDownloadApp(lpNetworkObj,szPasswordToken,szDsid,szAppSaleId,szAppPrice,FALSE,pSendInfo->bPaidApp);
						if (status == STATUS_FAILED)
						{
							status = pDownloadApp(lpNetworkObj,szPasswordToken,szDsid,szAppSaleId,szAppPrice,FALSE,pSendInfo->bPaidApp);
						}
						if (status == BUY_SUCCESS)
						{
							status = DOWNLOADSUCCESS;
						}
					}
				}
			}break;
		case COMMENTAPP://��¼��������
			{
				status = pGSA_Apple_NetworkVerifier(lpNetworkObj,
													szUdid,
													&pSendInfo->modelInfo,
													&pSendInfo->accountInfo.deviceInfo,
													pSendInfo->strProxy,
													pSendInfo->netType);
				if (status != STATUS_SUCCESS)
				{
					status = pGSA_Apple_NetworkVerifier(lpNetworkObj,
														szUdid,
														&pSendInfo->modelInfo,
														&pSendInfo->accountInfo.deviceInfo,
														pSendInfo->strProxy,
														pSendInfo->netType);
					if (status != STATUS_SUCCESS)
					{
						return status;
					}
				}
				status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
				if (status == STATUS_FAILED || status == LOGIN_FAILED)//���������������Ҫ���µ�¼һ��
				{
					status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
					
				}
				if (status == LOGIN_SUCCESS)
				{
					
					status = pSearchKeyHotWord(lpNetworkObj,
											pSendInfo->strHotKeyWord,
											szAppSaleId,
											szDsid,
											szPasswordToken,
											szTargetUrl);
					if (status == STATUS_FAILED)
					{
						status = pSearchKeyHotWord(lpNetworkObj,
											pSendInfo->strHotKeyWord,
											szAppSaleId,
											szDsid,
											szPasswordToken,
											szTargetUrl);
					}
					if (status == STATUS_SUCCESS)
					{
						status = pDownloadApp(lpNetworkObj,szPasswordToken,szDsid,szAppSaleId,szAppPrice,TRUE,pSendInfo->bPaidApp);
						if (status == STATUS_FAILED)
						{
							status = pDownloadApp(lpNetworkObj,szPasswordToken,szDsid,szAppSaleId,szAppPrice,TRUE,pSendInfo->bPaidApp);
							
						}

						if (status == BUY_SUCCESS)
						{
								if (status == BUY_SUCCESS)
								{
									status = pBuyConfirmActiveApp(lpNetworkObj,szPasswordToken,szAppSaleId,szDsid,szTargetUrl);
									if (status == STATUS_FAILED)
									{
										status = pBuyConfirmActiveApp(lpNetworkObj,szPasswordToken,szAppSaleId,szDsid,szTargetUrl);
										
									}
									if (status == STATUS_SUCCESS)
									{
										status = SEARCHSUCCESS;
									}
								}
								int rate = atoi(szRate);//�Ǽ�
								status = pCommentApp(lpNetworkObj,
													szPasswordToken,
													szDsid,
													szAppSaleId,
													rate,
													pSendInfo->strNickName,
													pSendInfo->strTitle,
													pSendInfo->strContext);
								if (status == STATUS_FAILED)
								{
									status = pCommentApp(lpNetworkObj,
													szPasswordToken,
													szDsid,
													szAppSaleId,
													rate,
													pSendInfo->strNickName,
													pSendInfo->strTitle,
													pSendInfo->strContext);
								}
								if (status == STATUS_SUCCESS)
								{
									status = COMMENTSUCCESS;
								}
							
						}
					}
				}
			}break;
		case SEARCHAPP://��¼�����ؼ�������,����ASO����
			{
				status = pGSA_Apple_NetworkVerifier(lpNetworkObj,
													szUdid,
													&pSendInfo->modelInfo,
													&pSendInfo->accountInfo.deviceInfo,
													pSendInfo->strProxy,
													pSendInfo->netType);
				if (status != STATUS_SUCCESS)
				{
					return status;
				}
				status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
				if (status == STATUS_FAILED || status == LOGIN_FAILED)//���������������Ҫ���µ�¼һ��
				{
					status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);				
				}

				if (status == LOGIN_SUCCESS)
				{
					status = pSearchKeyHotWord(lpNetworkObj,
											pSendInfo->strHotKeyWord,
											szAppSaleId,
											szDsid,
											szPasswordToken,
											szTargetUrl);
					if (status == STATUS_FAILED)
					{
						status = pSearchKeyHotWord(lpNetworkObj,
												pSendInfo->strHotKeyWord,
												szAppSaleId,
												szDsid,
												szPasswordToken,
												szTargetUrl);
					}
					
					if (status == STATUS_SUCCESS)
					{
						status = pDownloadApp(lpNetworkObj,szPasswordToken,szDsid,szAppSaleId,szAppPrice,TRUE,pSendInfo->bPaidApp);
						if (status == STATUS_FAILED)
						{
							status = pDownloadApp(lpNetworkObj,szPasswordToken,szDsid,szAppSaleId,szAppPrice,TRUE,pSendInfo->bPaidApp);
							
						}
						if (status == BUY_SUCCESS)
						{
							status = pBuyConfirmActiveApp(lpNetworkObj,szPasswordToken,szAppSaleId,szDsid,szTargetUrl);
							if (status == STATUS_FAILED)
							{
								status = pBuyConfirmActiveApp(lpNetworkObj,szPasswordToken,szAppSaleId,szDsid,szTargetUrl);
								
							}
							if (status == STATUS_SUCCESS)
							{
								status = SEARCHSUCCESS;
							}
						}
					}
				}
			}break;
		case LOGINSEARCH:
			{
				status = pGSA_Apple_NetworkVerifier(lpNetworkObj,
													szUdid,
													&pSendInfo->modelInfo,
													&pSendInfo->accountInfo.deviceInfo,
													pSendInfo->strProxy,
													pSendInfo->netType);
				if (status != STATUS_SUCCESS)
				{
					return status;
				}
				status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);
				if (status == STATUS_FAILED || status == LOGIN_FAILED)//���������������Ҫ���µ�¼һ��
				{
					status = pLoginAppStore(lpNetworkObj,
									szAppleId,
									szPwd,
									pSendInfo->strFatherAndMather,
									 pSendInfo->strTeacher,
									 pSendInfo->strBook,
									 &pSendInfo->accountInfo.deviceInfo,
									 szPasswordToken,
									 szDsid,
									 pSendInfo->strProxy,
									 pSendInfo->netType);				
				}

				if (status == LOGIN_SUCCESS)
				{
					status = pSearchKeyHotWord(lpNetworkObj,
											pSendInfo->strHotKeyWord,
											szAppSaleId,
											szDsid,
											szPasswordToken,
											szTargetUrl);
					if (status == STATUS_FAILED)
					{
						status = pSearchKeyHotWord(lpNetworkObj,
												pSendInfo->strHotKeyWord,
												szAppSaleId,
												szDsid,
												szPasswordToken,
												szTargetUrl);
					}
					if (status == STATUS_SUCCESS)
					{
						status = SEARCHSUCCESS;
					}
				}
			}break;
		case ONLYSEARCH:
			{
				status = pGSA_Apple_NetworkVerifier(lpNetworkObj,
													szUdid,
													&pSendInfo->modelInfo,
													&pSendInfo->accountInfo.deviceInfo,
													pSendInfo->strProxy,
													pSendInfo->netType);
				if (status != STATUS_SUCCESS)
				{
					return status;
				}
				//����¼����
				status = pOnlySearchKeyword(lpNetworkObj,pSendInfo->strHotKeyWord);

			}break;
		default:
			break;
	}

	if (lpNetworkObj)
	{
		pReleaseNetworkObject(lpNetworkObj);
	}

	pDeleteCriticalSection();

	g_status = status;

	return 0;

}
