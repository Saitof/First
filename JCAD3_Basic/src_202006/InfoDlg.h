/////////////////////////////////////////////////////////////////////////////
// ���\���_�C�A���O�N���X�錾
// InfoDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2010/02/01 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// Ҳ� �����

///////////////////////////////////////////////////////////////////////////////
// CStateDlg �_�C�A���O

#ifndef ___STATEDLG_H
#define ___STATEDLG_H

class CStateDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CStateDlg(CWnd* pParent=NULL);										// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_STATUS_DLG };
	double m_Cent3DX;
	double m_Cent3DY;
	double m_Cent3DZ;
	double m_EyeX;
	double m_EyeY;
	double m_EyeZ;
	double m_CentPRSX;
	double m_CentPRSY;
	double m_CentPRSZ;

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �I�y���[�V����
public:
	BOOL Create(void);
	void SetCent3D(const PNTTYPE& cent);
	void SetEye(const PNTTYPE& eye);
	void SetCentPRS(const PNTTYPE& cent);

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()
  
private:
	CWnd* m_pParent;
};

#endif ___STATEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CDataUseDlg �_�C�A���O

#ifndef ___DATAUSE_H
#define ___DATAUSE_H

class CDataUseDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CDataUseDlg(CWnd* pParent = NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DATA_USE };
	CString m_IntMax;
	CString m_PntMax;
	CString m_UndoMax;
	CString m_IntNum;
	CString m_PntNum;
	CString m_IntUse;
	CString m_PntUse;

// �I�[�o�[���C�h
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �I�y���[�V����
public:
	BOOL Create(void);
	void SetData(void);

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
};

#endif ___DATAUSE_H

///////////////////////////////////////////////////////////////////////////////
// CFunctionDlg �_�C�A���O

#ifndef ___FUNCTION_H
#define ___FUNCTION_H

class CFunctionDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CFunctionDlg(CWnd* pParent = NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FUNCTION_BTN };
	CButton m_Button1;
	CEdit   m_Edit1;
	CEdit   m_Edit2;
	CEdit   m_Edit3;
	CEdit   m_Edit4;
	CEdit   m_Edit5;
	CEdit   m_Edit6;
	CEdit   m_Edit7;
	CEdit   m_Edit8;
	CEdit   m_Edit9;
	CEdit   m_Edit10;
	CEdit   m_Edit11;
	CEdit   m_Edit12;
	int     m_Check1;
	int     m_Check2;
	int     m_Check3;
	int     m_Check4;
	int     m_Check5;
	int     m_Check6;
	int     m_Check7;
	int     m_Check8;
	int     m_Check9;
	int     m_Check10;
	int     m_Check11;
	int     m_Check12;

// �I�[�o�[���C�h
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �I�y���[�V����
public:
	BOOL Create(void);
	void OffAllButton(void);
	void OnOffButton(int no, int flg);
	void MessageOut(int nID, int flg);

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnButtonClick(void);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	afx_msg void OnCheck3(void);
	afx_msg void OnCheck4(void);
	afx_msg void OnCheck5(void);
	afx_msg void OnCheck6(void);
	afx_msg void OnCheck7(void);
	afx_msg void OnCheck8(void);
	afx_msg void OnCheck9(void);
	afx_msg void OnCheck10(void);
	afx_msg void OnCheck11(void);
	afx_msg void OnCheck12(void);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	int   m_nID;
	int   m_Lock;
};

#endif ___FUNCTION_H

///////////////////////////////////////////////////////////////////////////////
// CFunction2Dlg �_�C�A���O

#ifndef ___FUNCTION2_H
#define ___FUNCTION2_H

class CFunction2Dlg : public CDialog
{
// �R���X�g���N�V����
public:
	CFunction2Dlg(CWnd* pParent = NULL);								// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FUNCTION_BTN2 };
	int m_Check1;
	int m_Check2;
	int m_Check3;
	int m_Check4;
	int m_Check5;
	int m_Check6;
	int m_Check7;
	int m_Check8;
	int m_Check9;
	int m_Check10;
	int m_Check11;
	int m_Check12;

// �I�[�o�[���C�h
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �I�y���[�V����
public:
	BOOL Create(void);
	void OffAllButton(void);
	void OnOffButton(int no, int flg);

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	afx_msg void OnCheck3(void);
	afx_msg void OnCheck4(void);
	afx_msg void OnCheck5(void);
	afx_msg void OnCheck6(void);
	afx_msg void OnCheck7(void);
	afx_msg void OnCheck8(void);
	afx_msg void OnCheck9(void);
	afx_msg void OnCheck10(void);
	afx_msg void OnCheck11(void);
	afx_msg void OnCheck12(void);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
};

#endif ___FUNCTION2_H

///////////////////////////////////////////////////////////////////////////////
// CCurveCtrlDlg �_�C�A���O

#ifndef ___CURVECTRL_H
#define ___CURVECTRL_H

class CCurveCtrlDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CCurveCtrlDlg(CWnd* pParent = NULL);								// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CURVE_CTRL };
	CStatic m_Static1;
	CStatic m_Static2;
	CStatic m_Static3;
	CStatic m_Static4;
	CEdit   m_Edit1;
	CEdit   m_Edit2;
	int     m_Check1;
	int     m_Check2;
	double  m_Angle;
	double  m_Radius;

// �I�[�o�[���C�h
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �I�y���[�V����
public:
	BOOL Create(void);
	void ClearData(void);

// �C���v�������e�[�V����
public:
	void PermissionCheck1(void);
	void PermissionCheck2(void);

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	afx_msg void OnSetAngle(void);
	afx_msg void OnSetRadius(void);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
};

#endif ___CURVECTRL_H

///////////////////////////////////////////////////////////////////////////////
// CProgressDlg �_�C�A���O

#ifndef ___PROGRESSDLG_H
#define ___PROGRESSDLG_H

class CProgressDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CProgressDlg(int kind=0, CWnd* pParent=NULL);						// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl m_Progress;
	CButton       m_Button;

// �I�y���[�V����
public:
	BOOL Create(void);
	void SetProgress(int per);

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	int   m_Kind;
};

#endif ___PROGRESSDLG_H

///////////////////////////////////////////////////////////////////////////////
// CConfirmDlg �_�C�A���O

#ifndef ___CONFIRMDLG_H
#define ___CONFIRMDLG_H

class CConfirmDlg : public CDialog
{
// �R���X�g���N�V����
public:
	CConfirmDlg(CWnd* pParent=NULL);									// �W���̺ݽ�׸�

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CONFIRM };
	CButton m_Button1;

// �I�[�o�[���C�h
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDV��߰�

// �C���v�������e�[�V����
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnDestroy(void);
	afx_msg void OnButtonClick(void);
	DECLARE_MESSAGE_MAP()

public:
	int m_EndMode;
};

#endif ___CONFIRMDLG_H

//==============================<EOF:InfoDlg.h>==============================