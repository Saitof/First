/////////////////////////////////////////////////////////////////////////////
// 情報表示ダイアログクラス宣言
// InfoDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2010/02/01 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

///////////////////////////////////////////////////////////////////////////////
// CStateDlg ダイアログ

#ifndef ___STATEDLG_H
#define ___STATEDLG_H

class CStateDlg : public CDialog
{
// コンストラクション
public:
	CStateDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
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

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void SetCent3D(const PNTTYPE& cent);
	void SetEye(const PNTTYPE& eye);
	void SetCentPRS(const PNTTYPE& cent);

// インプリメンテーション
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
// CDataUseDlg ダイアログ

#ifndef ___DATAUSE_H
#define ___DATAUSE_H

class CDataUseDlg : public CDialog
{
// コンストラクション
public:
	CDataUseDlg(CWnd* pParent = NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_DATA_USE };
	CString m_IntMax;
	CString m_PntMax;
	CString m_UndoMax;
	CString m_IntNum;
	CString m_PntNum;
	CString m_IntUse;
	CString m_PntUse;

// オーバーライド
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void SetData(void);

// インプリメンテーション
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
// CFunctionDlg ダイアログ

#ifndef ___FUNCTION_H
#define ___FUNCTION_H

class CFunctionDlg : public CDialog
{
// コンストラクション
public:
	CFunctionDlg(CWnd* pParent = NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
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

// オーバーライド
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void OffAllButton(void);
	void OnOffButton(int no, int flg);
	void MessageOut(int nID, int flg);

// インプリメンテーション
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
// CFunction2Dlg ダイアログ

#ifndef ___FUNCTION2_H
#define ___FUNCTION2_H

class CFunction2Dlg : public CDialog
{
// コンストラクション
public:
	CFunction2Dlg(CWnd* pParent = NULL);								// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
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

// オーバーライド
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void OffAllButton(void);
	void OnOffButton(int no, int flg);

// インプリメンテーション
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
// CCurveCtrlDlg ダイアログ

#ifndef ___CURVECTRL_H
#define ___CURVECTRL_H

class CCurveCtrlDlg : public CDialog
{
// コンストラクション
public:
	CCurveCtrlDlg(CWnd* pParent = NULL);								// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
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

// オーバーライド
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void ClearData(void);

// インプリメンテーション
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
// CProgressDlg ダイアログ

#ifndef ___PROGRESSDLG_H
#define ___PROGRESSDLG_H

class CProgressDlg : public CDialog
{
// コンストラクション
public:
	CProgressDlg(int kind=0, CWnd* pParent=NULL);						// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl m_Progress;
	CButton       m_Button;

// オペレーション
public:
	BOOL Create(void);
	void SetProgress(int per);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	int   m_Kind;
};

#endif ___PROGRESSDLG_H

///////////////////////////////////////////////////////////////////////////////
// CConfirmDlg ダイアログ

#ifndef ___CONFIRMDLG_H
#define ___CONFIRMDLG_H

class CConfirmDlg : public CDialog
{
// コンストラクション
public:
	CConfirmDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_CONFIRM };
	CButton m_Button1;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
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