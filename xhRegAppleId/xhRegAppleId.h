
// xhRegAppleId.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CxhRegAppleIdApp:
// �йش����ʵ�֣������ xhRegAppleId.cpp
//

class CxhRegAppleIdApp : public CWinAppEx
{
public:
	CxhRegAppleIdApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CxhRegAppleIdApp theApp;