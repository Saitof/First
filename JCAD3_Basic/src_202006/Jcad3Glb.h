/////////////////////////////////////////////////////////////////////////////
// �A�v���P�[�V�������C���N���X(CJcad3GlbApp)�錾
// Jcad3Glb.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"													// ���C�� �V���{��
// ************************ �ǉ� ************************
#include "Imagekit7.h"
#define  _IK7FILE_
#define  _IK7PRINT_
#define  _IK7VECT_
#include "Imagekit7Def.h"
// ******************************************************

/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbApp:
// ���̃N���X�̎����ɂ��ẮAJcad3Glb.cpp ���Q�Ƃ��Ă��������B
//

class CJcad3GlbApp : public CWinApp
{
public:
	CJcad3GlbApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

CString GetAppPath(int mode);
BOOL    CheckFilePath(int flg, CString path);
extern  CJcad3GlbApp theApp;											// CJcad3GlbApp��޼ު��

//=============================<EOF:Jcad3Glb.h>=============================