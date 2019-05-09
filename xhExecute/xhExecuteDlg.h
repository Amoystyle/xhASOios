
// xhExecuteDlg.h : ͷ�ļ�
//
#include "accountInfo.h"
#pragma once


// CxhExecuteDlg �Ի���
class CxhExecuteDlg : public CDialog
{
// ����
public:
	CxhExecuteDlg(CWnd* pParent = NULL);	// ��׼���캯��

	enum
	{
		LOGINDOWNLOADAPP = 1,
		COMMENTAPP,
		SEARCHAPP,
		ONLYLOGIN,
		LOGINSEARCH,
		ONLYSEARCH
	};
// �Ի�������
	enum { IDD = IDD_XHEXECUTE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	HANDLE m_hPipeRead;
	HANDLE m_hPipeWrite;
	HANDLE m_hEvent;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	BOOL getFunInterface();
	static UINT WINAPI InterfaceFunction(PSENDINFO pSendInfo);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
