/////////////////////////////////////////////////////////////////////////////
// ���W�I�{�^���_�C�A���O�N���X�錾
// BottunDlg.h
//---------------------------------------------------------------------------
// LastEdit: 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

///////////////////////////////////////////////////////////////////////////////
// CDispmodeDlg �_�C�A���O

#ifndef ___DISPMODEDLG_H
#define ___DISPMODEDLG_H

class CDispmodeDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CDispmodeDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DISP_MODE };
	int m_DspMode;

// �I�y���[�V����
	void SetValue(int prm1);
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___DISPMODEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CScreenDlg �_�C�A���O

#ifndef ___SCREENDLG_H
#define ___SCREENDLG_H

class CScreenDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CScreenDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_SCREEN };
	int m_ScreenNo;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SCREENDLG_H

///////////////////////////////////////////////////////////////////////////////
// CEyesetDlg �_�C�A���O

#ifndef ___EYESETDLG_H
#define ___EYESETDLG_H

class CEyesetDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CEyesetDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_EYE_SET };
	int m_Eyeset;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___EYESETDLG_H

///////////////////////////////////////////////////////////////////////////////
// CBasesetDlg �_�C�A���O

#ifndef ___BASESETDLG_H
#define ___BASESETDLG_H

class CBasesetDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CBasesetDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_BASE_SET };
	int m_Baseset;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___BASESETDLG_H

///////////////////////////////////////////////////////////////////////////////
// CGlbDlg �_�C�A���O

#ifndef ___GLBDLG_H
#define ___GLBDLG_H

class CGlbDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CGlbDlg(int kind=0, CWnd* pParent=NULL);							// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DISP_GLB };
	int     m_GlbNo;
	int     m_ChangeSize;
	CStatic m_Static1;
	double  m_RingSize;
	CEdit   m_Edit1;
	CStatic m_Static2;

// �I�y���[�V����
	void PermissionCheck(void);
	void SetValue(int p1, int p2, double p3);
	void GetValue(int* p1, int* p2, double* p3);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnCheckBtn(UINT nID);
	afx_msg void OnCheck(void);
	DECLARE_MESSAGE_MAP()

private:
	int m_Kind;
};

#endif ___GLBDLG_H

///////////////////////////////////////////////////////////////////////////////
// CDispTmpDlg �_�C�A���O

#ifndef ___DISPTMPDLG_H
#define ___DISPTMPDLG_H

class CDispTmpDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CDispTmpDlg(int kind=0, CWnd* pParent=NULL);						// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DISP_TEMPLATE };
	int     m_DspTmp;
	int     m_ChangeSize;
	CStatic m_Static1;
	double  m_RingSize;
	CEdit   m_Edit1;
	CStatic m_Static2;

// �I�y���[�V����
	void PermissionCheck(void);
	void SetValue(int p1, int p2, double p3);
	void GetValue(int* p1, int* p2, double* p3);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnCheckBtn(UINT nID);
	afx_msg void OnCheck(void);
	DECLARE_MESSAGE_MAP()

private:
	int m_Kind;
};

#endif ___DISPTMPDLG_H

///////////////////////////////////////////////////////////////////////////////
// CColorDlg �_�C�A���O

#ifndef ___COLORDLG_H
#define ___COLORDLG_H

class CColorDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CColorDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_COLOR };
	int m_Color;

// �I�y���[�V����
	void SetValue(int prm1);
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___COLORDLG_H

///////////////////////////////////////////////////////////////////////////////
// CCurveDlg �_�C�A���O

#ifndef ___CURVEDLG_H
#define ___CURVEDLG_H

class CCurveDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CCurveDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CURVE };
	int m_Curve;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___CURVEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPrimitiveDlg �_�C�A���O

#ifndef ___PRIMITIVEDLG_H
#define ___PRIMITIVEDLG_H

class CPrimitiveDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CPrimitiveDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PRIMITIVE };
	int m_Primitive;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___PRIMITIVEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CBooleanDlg �_�C�A���O

#ifndef ___BOOLEANDLG_H
#define ___BOOLEANDLG_H

class CBooleanDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CBooleanDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_BOOLEAN };
	int m_Boolean;
	int m_DeleteB;

// �I�y���[�V����
	void GetValue(int* prm1, int* prm2);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___BOOLEANDLG_H

///////////////////////////////////////////////////////////////////////////////
// CGemsetDlg �_�C�A���O

#ifndef ___GEMSETDLG_H
#define ___GEMSETDLG_H

class CGemsetDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CGemsetDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GEM_SETTING };
	int m_GemSet;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___GEMSETDLG_H

///////////////////////////////////////////////////////////////////////////////
// CGemDlg �_�C�A���O

#ifndef ___GEMDLG_H
#define ___GEMDLG_H

class CGemDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CGemDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GEM };
	int m_Gem;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___GEMDLG_H

///////////////////////////////////////////////////////////////////////////////
// CRevolveDlg �_�C�A���O

#ifndef ___REVOLVEDLG_H
#define ___REVOLVEDLG_H

class CRevolveDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CRevolveDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_REVOLVE };
	int m_Revolve;

// �I�y���[�V����
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___REVOLVEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetFuncModeDlg �_�C�A���O

#ifndef ___SETFUNCMODEDLG_H
#define ___SETFUNCMODEDLG_H

class CSetFuncModeDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CSetFuncModeDlg(CWnd* pParent=NULL);								// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SET_FUNCMODE };
	int m_FuncMode;

// �I�y���[�V����
	void SetValue(int prm1);
	void GetValue(int* prm1);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SETFUNCMODEDLG_H

//==============================<EOF:ButtonDlg.h>============================