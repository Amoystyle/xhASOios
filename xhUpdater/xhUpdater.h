
// xhUpdater.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CxhUpdaterApp:
// �йش����ʵ�֣������ xhUpdater.cpp
//

class CxhUpdaterApp : public CWinAppEx
{
public:
	CxhUpdaterApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CxhUpdaterApp theApp;