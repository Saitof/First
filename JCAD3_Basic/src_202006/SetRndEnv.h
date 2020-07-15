/////////////////////////////////////////////////////////////////////////////
// �����_�����O���ݒ�v���p�e�B�V�[�g�N���X�錾
// SetRndEnv.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg1 �_�C�A���O

#ifndef ___RNDENVDLG1_H
#define ___RNDENVDLG1_H

class CRndEnvDlg1 : public CPropertyPage
{
// �R���X�g���N�V����
public:
	CRndEnvDlg1();														// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_RNDENV_SHT1 };
	int				m_No;
	int				m_RCl[3];
	CSpinButtonCtrl	m_Spin1;
	CSpinButtonCtrl	m_Spin4;
	CSpinButtonCtrl	m_Spin8;
	int				m_GCl[3];
	CSpinButtonCtrl	m_Spin2;
	CSpinButtonCtrl	m_Spin5;
	CSpinButtonCtrl	m_Spin9;
	int				m_BCl[3];
	CSpinButtonCtrl	m_Spin3;
	CSpinButtonCtrl	m_Spin6;
	CSpinButtonCtrl	m_Spin10;
	CStatic			m_Pic1;
	CStatic			m_Pic2;
	CStatic			m_Pic3;
    int				m_Shn;
	CSpinButtonCtrl	m_Spin7;

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg void OnPaint(void);
	virtual BOOL OnInitDialog(void);
	afx_msg void OnInitFile(void);
	afx_msg void OnSaveFile(void);
	afx_msg void OnRestoreFile(void);
	afx_msg BOOL OnColorNo(UINT nID);
	afx_msg void OnSetColor1(void);
	afx_msg void OnSetColor2(void);
	afx_msg void OnSetColor3(void);
	DECLARE_MESSAGE_MAP()
	void ColorPaint(CPaintDC *dc);

private:
	int   m_Check1;
	int   m_Check2;
	int   m_Check3;
	CRect m_SQ[3];
};

#endif ___RNDENVDLG1_H

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg2 �_�C�A���O

#ifndef ___RNDENVDLG2_H
#define ___RNDENVDLG2_H

class CRndEnvDlg2 : public CPropertyPage
{
// �R���X�g���N�V����
public:
	CRndEnvDlg2();														// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_RNDENV_SHT2 };
	int				m_RCol[3];
	CSpinButtonCtrl	m_Spin1;
	CSpinButtonCtrl	m_Spin4;
	CSpinButtonCtrl	m_Spin7;
	int				m_GCol[3];
	CSpinButtonCtrl	m_Spin2;
	CSpinButtonCtrl	m_Spin5;
	CSpinButtonCtrl	m_Spin8;
	int				m_BCol[3];
	CSpinButtonCtrl	m_Spin3;
	CSpinButtonCtrl	m_Spin6;
	CSpinButtonCtrl	m_Spin9;
	CStatic			m_Pic1;
	CStatic			m_Pic2;
	CStatic			m_Pic3;

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg void OnPaint(void);
	virtual BOOL OnInitDialog(void);
	afx_msg void OnInitFile(void);
	afx_msg void OnSaveFile(void);
	afx_msg void OnRestoreFile(void);
	afx_msg void OnSetColor1(void);
	afx_msg void OnSetColor2(void);
	afx_msg void OnSetColor3(void);
	DECLARE_MESSAGE_MAP()
	void ColorPaint(CPaintDC *dc);

private:
	int   m_Check1;
	int   m_Check2;
	int   m_Check3;
	CRect m_SQ[3];
};

#endif ___RNDENVDLG2_H

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg3 �_�C�A���O

#ifndef ___RNDENVDLG3_H
#define ___RNDENVDLG3_H

class CRndEnvDlg3 : public CPropertyPage
{
// �R���X�g���N�V����
public:
	CRndEnvDlg3();														// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_RNDENV_SHT3 };
	CString	m_FileName;
	CEdit	m_Edit1;

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnFileOpen(void);
	afx_msg void OnInitFile(void);
	afx_msg void OnSaveFile(void);
	afx_msg void OnRestoreFile(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_Path;
	CString m_File;
};

#endif ___RNDENVDLG3_H

///////////////////////////////////////////////////////////////////////////////
// CRndEnvSheet �v���p�e�B�V�[�g

#ifndef ___RNDENVSHT_H
#define ___RNDENVSHT_H

class CRndEnvSheet : public CPropertySheet
{
// �\�z
public:
	CRndEnvSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CRndEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void AddControlPages(void);

// �A�g���r���[�g
public:

	CRndEnvDlg1	m_RndEnvPage1;
	CRndEnvDlg2	m_RndEnvPage2;
	CRndEnvDlg3	m_RndEnvPage3;
// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CRndEnvSheet)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CRndEnvSheet();
	virtual BOOL OnInitDialog();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:

	//{{AFX_MSG(CRndEnvSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif ___RNDENVSHT_H

//=============================<EOF:SetRndEnv.h>=============================
