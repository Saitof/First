/////////////////////////////////////////////////////////////////////////////
// プッシュボタンダイアログクラス宣言
// PushDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/02 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ
#include "CheckButton.h"

/////////////////////////////////////////////////////////////////////////////
// CScrollDlg ダイアログ

#ifndef ___SCROLLDLG_H
#define ___SCROLLDLG_H

class CScrollDlg : public CDialog
{
// コンストラクション
public:
	CScrollDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SCR_SCROLL };

protected:
	CBitmapButton LeftXY;
	CBitmapButton RightXY;
	CBitmapButton UpXY;
	CBitmapButton DownXY;
	CBitmapButton LeftYZ;
	CBitmapButton RightYZ;
	CBitmapButton UpYZ;
	CBitmapButton DownYZ;
	CBitmapButton LeftZX;
	CBitmapButton RightZX;
	CBitmapButton UpZX;
	CBitmapButton DownZX;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnCancel(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	afx_msg void OnDestroy(void);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	int   Axis, Dir, m_nID;
};

#endif ___SCROLLDLG_H

/////////////////////////////////////////////////////////////////////////////
// CEyeDlg ダイアログ

#ifndef ___EYEDLG_H
#define ___EYEDLG_H

class CEyeDlg : public CDialog
{
// コンストラクション
public:
	CEyeDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_EYE_CHANGE };

protected:
	CBitmapButton Left;
	CBitmapButton Right;
	CBitmapButton Up;
	CBitmapButton Down;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnCancel(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	afx_msg void OnDestroy(void);
	DECLARE_MESSAGE_MAP()

private:
	CWnd*  m_pParent;
	double Theta;
	int    Dir, m_nID;
};

#endif ___EYEDLG_H

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg1 ダイアログ

#ifndef ___SETBASEDLG1_H
#define ___SETBASEDLG1_H

class CSetBaseDlg1 : public CDialog
{
// コンストラクション
public:
	CSetBaseDlg1(CWnd* pParent = NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_BASE1 };
	CCheckBox    m_Base1;
	CCheckBox    m_Base2;
	CCheckBox    m_Base3;
	CCheckBox    m_Base4;
	CCheckBox    m_Base5;
	CCheckBox    m_Base6;
	CCheckBox    m_Base7;
	CCheckBox    m_Base8;
	CCheckBox    m_Base9;
	CCheckButton m_DirSN;
	CCheckButton m_DirEWSN;
	CCheckButton m_DirEW;

protected:
	UINT m_BasePoint;
	UINT m_Direction;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetValue(int p1, int p2);
	void GetValue(int* p1, int* p2);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnBaseButton(UINT nID);
	afx_msg BOOL OnDirButton(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SETBASEDLG1_H

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg2 ダイアログ

#ifndef ___SETBASEDLG2_H
#define ___SETBASEDLG2_H

class CSetBaseDlg2 : public CDialog
{
// コンストラクション
public:
	CSetBaseDlg2(CWnd* pParent = NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_BASE2 };
	CCheckBox m_Base1;
	CCheckBox m_Base2;
	CCheckBox m_Base3;
	CCheckBox m_Base4;
	CCheckBox m_Base5;
	CCheckBox m_Base6;
	CCheckBox m_Base7;
	CCheckBox m_Base8;
	CCheckBox m_Base9;

protected:
	UINT m_BasePoint;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetValue(int p1);
	void GetValue(int* p1);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnBaseButton(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SETBASEDLG2_H

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg3 ダイアログ

#ifndef ___SETBASEDLG3_H
#define ___SETBASEDLG3_H

class CSetBaseDlg3 : public CDialog
{
// コンストラクション
public:
	CSetBaseDlg3(CWnd* pParent = NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_BASE3 };
	CCheckBox m_Base1;
	CCheckBox m_Base2;
	CCheckBox m_Base3;
	CCheckBox m_Base4;
	CCheckBox m_Base5;
	CCheckBox m_Base6;
	CCheckBox m_Base7;
	int       m_Size;

protected:
	UINT m_BasePoint1;
	UINT m_BasePoint2;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetValue(int p1, int p2, int p3);
	void GetValue(int* p1, int* p2, int* p3);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnBaseButton1(UINT nID);
	afx_msg BOOL OnBaseButton2(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SETBASEDLG3_H

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg4 ダイアログ

#ifndef ___SETBASEDLG4_H
#define ___SETBASEDLG4_H

class CSetBaseDlg4 : public CDialog
{
// コンストラクション
public:
	CSetBaseDlg4(CWnd* pParent = NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_BASE4 };
	CCheckBox m_Base1;
	CCheckBox m_Base2;
	CCheckBox m_Base3;

protected:
	UINT m_BasePoint;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetValue(int p1);
	void GetValue(int* p1);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnBaseButton(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SETBASEDLG4_H

//==============================<EOF:PushDlg.h>==============================
