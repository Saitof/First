/////////////////////////////////////////////////////////////////////////////
// �o�[�W�������_�C�A���O�N���X(CAboutDlg)�錾
// AboutDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2010/03/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();														// �׽�̍\�z

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV�̻�߰�

// ����
protected:
	virtual BOOL OnInitDialog(void);
	DECLARE_MESSAGE_MAP()
};

//=============================<EOF:AboutDlg.h>==============================