
// xhRegAppleIdDlg.h : ͷ�ļ�
//

#pragma once


// CxhRegAppleIdDlg �Ի���
class CxhRegAppleIdDlg : public CDialog
{
// ����
public:
	CxhRegAppleIdDlg(CWnd* pParent = NULL);	// ��׼���캯��
	static UINT WINAPI  registerIdThread(LPVOID lPtr);
	void InitAndLoadModule();
	void UnInitAndUnLoadModule();

// �Ի�������
	enum { IDD = IDD_XHREGAPPLEID_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnRegisterStart();
};
