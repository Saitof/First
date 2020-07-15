/////////////////////////////////////////////////////////////////////////////
// ���C�g���[�V���O���ݒ�v���p�e�B�V�[�g�N���X�錾
// SetRayEnv.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

///////////////////////////////////////////////////////////////////////////////
// CRayEnvDlg1 �_�C�A���O

#ifndef ___RAYENVDLG1_H
#define ___RAYENVDLG1_H

class CRayEnvDlg1 : public CPropertyPage
{
// �R���X�g���N�V����
public:
	CRayEnvDlg1();														// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_RAYENV_SHT1 };
    int             m_No;
    int             m_Model;
    int             m_RCol[4];
    CSpinButtonCtrl m_Spin1;
    CSpinButtonCtrl m_Spin4;
    CSpinButtonCtrl m_Spin7;
    CSpinButtonCtrl m_Spin10;
    int             m_GCol[4];
    CSpinButtonCtrl m_Spin2;
    CSpinButtonCtrl m_Spin5;
    CSpinButtonCtrl m_Spin8;
    CSpinButtonCtrl m_Spin11;
    int             m_BCol[4];
    CSpinButtonCtrl m_Spin3;
    CSpinButtonCtrl m_Spin6;
    CSpinButtonCtrl m_Spin9;
    CSpinButtonCtrl m_Spin12;
    CStatic         m_Pic[4];
    int             m_Trace;
    double          m_N;
    CStatic         m_Static1;
    CStatic         m_Static2;
    int             m_M1;
    CSpinButtonCtrl m_Spin13;
    CEdit           m_Edit14;
    CStatic         m_Static3;
    CStatic         m_Static4;
    double          m_M2;
    CEdit           m_Edit15;
    CStatic         m_Static5;
    CStatic         m_Static6;
    CStatic         m_Static7;
    CStatic         m_Static8;
    CStatic         m_Static9;
    double          m_NR;
    CEdit           m_Edit16;
    double          m_NG;
    CEdit           m_Edit17;
    double          m_NB;
    CEdit           m_Edit18;

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
    afx_msg BOOL OnTraceModel(UINT nID);
    afx_msg void OnSetColor(void);
    DECLARE_MESSAGE_MAP()
    void ColorPaint(CPaintDC *dc);
    void GetTraceColor(void);
    void CheckModel(int model);
public:
    void SetTraceColor(void);

private:
    int   m_Check;
    CRect m_SQ[4];

public:
    ATTRIBT Atr_Sv[ATMAX];                                              // �\�ʑ����z��
};

#endif ___RAYENVDLG1_H

///////////////////////////////////////////////////////////////////////////////
// CRayEnvDlg2 �_�C�A���O

#ifndef ___RAYENVDLG2_H
#define ___RAYENVDLG2_H

class CRayEnvDlg2 : public CPropertyPage
{
// �R���X�g���N�V����
public:
    CRayEnvDlg2();                                                      // �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
    enum { IDD = IDD_RAYENV_SHT2 };
    int             m_No;
    int             m_Flg;
    int             m_Type;
    CStatic         m_Static1;
    int             m_Los;
    CButton         m_Button11;
    CButton         m_Button12;
    CButton         m_Button13;
    int             m_Lcr;
    CSpinButtonCtrl m_Spin1;
    int             m_Lcg;
    CSpinButtonCtrl m_Spin2;
    int             m_Lcb;
    CSpinButtonCtrl m_Spin3;
    CStatic         m_Pic;
    double          m_Lvx;
    CSpinButtonCtrl m_Spin4;
    double          m_Lvy;
    CSpinButtonCtrl m_Spin5;
    double          m_Lvz;
    CSpinButtonCtrl m_Spin6;

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
    afx_msg BOOL OnLightNo(UINT nID);
    afx_msg BOOL OnLightType(UINT nID);
    afx_msg void OnSetColor(void);
    afx_msg void OnSetLightPos(void);
	DECLARE_MESSAGE_MAP()
	void ColorPaint(CPaintDC *dc);
    void GetTraceLight(void);
    void CheckType(void);
public:
    void SetTraceLight(void);

private:
	int   m_Check1;
	int   m_Check2;
	CRect m_SQ;

public:
    double  ObjCent[3];                                                 // ���̒��S
    LIGHT   Lgt_Sv[LGMAX];                                              // �������z��
};

#endif ___RAYENVDLG2_H

///////////////////////////////////////////////////////////////////////////////
// CRayEnvDlg3 �_�C�A���O

#ifndef ___RAYENVDLG3_H
#define ___RAYENVDLG3_H

class CRayEnvDlg3 : public CPropertyPage
{
// �R���X�g���N�V����
public:
    CRayEnvDlg3();                                                      // �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
    enum { IDD = IDD_RAYENV_SHT3 };
    int             m_Bkr;
    CSpinButtonCtrl m_Spin1;
    int             m_Bkg;
    CSpinButtonCtrl m_Spin2;
    int             m_Bkb;
    CSpinButtonCtrl m_Spin3;
    CStatic         m_Pic;
    int             m_Max;
    CSpinButtonCtrl m_Spin4;
    int             m_Ant;
    int             m_Edg;
    int             m_StgF1;
    int             m_StgF2;
    int             m_StgF3;
    int             m_StgF4;
    double          m_StgX;
    CSpinButtonCtrl m_Spin5;
    double          m_StgY;
    CSpinButtonCtrl m_Spin6;
    double          m_StgZ;
    CSpinButtonCtrl m_Spin7;
    double          m_StgP;
    CSpinButtonCtrl m_Spin8;
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
    afx_msg void OnSetColor(void);
    afx_msg void OnSetStage(void);
	DECLARE_MESSAGE_MAP()
	void ColorPaint(CPaintDC *dc);
    void GetTraceEnv(void);
    void DisplayStage(void);
public:
    void SetTraceEnv(void);

private:
	int   m_Check1;
	int   m_Check2;
	CRect m_SQ;

public:
    double   m_Bottom[3];
    COLORREF BkRGB_Sv;                                                  // �w�i�F
    int      Max_Lvl_Sv;                                                // �ő�T������
    int      Anti_Sv;                                                   // �����ر�ݸ�
    int      Edge_Sv;                                                   // �Ő��׸�
    int      StgFlg_Sv;                                                 // �ð�ޗL���׸�
    double   StgSize_Sv[4];                                             // �ð�޻���
};

#endif ___RAYENVDLG3_H

///////////////////////////////////////////////////////////////////////////////
// CRayEnvSheet �v���p�e�B�V�[�g

#ifndef ___RAYENVSHT_H
#define ___RAYENVSHT_H

class CRayEnvSheet : public CPropertySheet
{
// �\�z
public:
	CRayEnvSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CRayEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void AddControlPages(void);

// �A�g���r���[�g
public:
	CRayEnvDlg1	m_RayEnvPage1;
	CRayEnvDlg2	m_RayEnvPage2;
	CRayEnvDlg3	m_RayEnvPage3;

// �I�y���[�V����
public:
    void SetRayTraceData(void);

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CRayEnvSheet)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CRayEnvSheet();
	virtual BOOL OnInitDialog();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:

	//{{AFX_MSG(CRayEnvSheet)
	//}}AFX_MSG
    afx_msg void OnDestroy(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___RAYENVSHT_H

//=============================<EOF:SetRayEnv.h>=============================
