
// xhExecute.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CxhExecuteApp:
// �йش����ʵ�֣������ xhExecute.cpp
//

class CxhExecuteApp : public CWinAppEx
{
public:
	CxhExecuteApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CxhExecuteApp theApp;