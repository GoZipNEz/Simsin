
// SimsinSuryun.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CSimsinSuryunApp:
// �� Ŭ������ ������ ���ؼ��� SimsinSuryun.cpp�� �����Ͻʽÿ�.
//

class CSimsinSuryunApp : public CWinApp
{
public:
	CSimsinSuryunApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CSimsinSuryunApp theApp;