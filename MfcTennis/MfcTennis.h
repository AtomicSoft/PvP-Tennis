
// MfcTennis.h : MfcTennis ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMfcTennisApp:
// �� Ŭ������ ������ ���ؼ��� MfcTennis.cpp�� �����Ͻʽÿ�.
//

class CMfcTennisApp : public CWinApp
{
public:
	CMfcTennisApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMfcTennisApp theApp;
