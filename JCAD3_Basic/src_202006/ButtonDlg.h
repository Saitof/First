/////////////////////////////////////////////////////////////////////////////
// ラジオボタンダイアログクラス宣言
// BottunDlg.h
//---------------------------------------------------------------------------
// LastEdit: 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

///////////////////////////////////////////////////////////////////////////////
// CDispmodeDlg ダイアログ

#ifndef ___DISPMODEDLG_H
#define ___DISPMODEDLG_H

class CDispmodeDlg : public CDialog
{
// コンストラクション
public:
	CDispmodeDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_DISP_MODE };
	int m_DspMode;

// オペレーション
	void SetValue(int prm1);
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___DISPMODEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CScreenDlg ダイアログ

#ifndef ___SCREENDLG_H
#define ___SCREENDLG_H

class CScreenDlg : public CDialog
{
// コンストラクション
public:
	CScreenDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_SCREEN };
	int m_ScreenNo;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SCREENDLG_H

///////////////////////////////////////////////////////////////////////////////
// CEyesetDlg ダイアログ

#ifndef ___EYESETDLG_H
#define ___EYESETDLG_H

class CEyesetDlg : public CDialog
{
// コンストラクション
public:
	CEyesetDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_EYE_SET };
	int m_Eyeset;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___EYESETDLG_H

///////////////////////////////////////////////////////////////////////////////
// CBasesetDlg ダイアログ

#ifndef ___BASESETDLG_H
#define ___BASESETDLG_H

class CBasesetDlg : public CDialog
{
// コンストラクション
public:
	CBasesetDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_BASE_SET };
	int m_Baseset;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___BASESETDLG_H

///////////////////////////////////////////////////////////////////////////////
// CGlbDlg ダイアログ

#ifndef ___GLBDLG_H
#define ___GLBDLG_H

class CGlbDlg : public CDialog
{
// コンストラクション
public:
	CGlbDlg(int kind=0, CWnd* pParent=NULL);							// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_DISP_GLB };
	int     m_GlbNo;
	int     m_ChangeSize;
	CStatic m_Static1;
	double  m_RingSize;
	CEdit   m_Edit1;
	CStatic m_Static2;

// オペレーション
	void PermissionCheck(void);
	void SetValue(int p1, int p2, double p3);
	void GetValue(int* p1, int* p2, double* p3);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
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
// CDispTmpDlg ダイアログ

#ifndef ___DISPTMPDLG_H
#define ___DISPTMPDLG_H

class CDispTmpDlg : public CDialog
{
// コンストラクション
public:
	CDispTmpDlg(int kind=0, CWnd* pParent=NULL);						// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_DISP_TEMPLATE };
	int     m_DspTmp;
	int     m_ChangeSize;
	CStatic m_Static1;
	double  m_RingSize;
	CEdit   m_Edit1;
	CStatic m_Static2;

// オペレーション
	void PermissionCheck(void);
	void SetValue(int p1, int p2, double p3);
	void GetValue(int* p1, int* p2, double* p3);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
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
// CColorDlg ダイアログ

#ifndef ___COLORDLG_H
#define ___COLORDLG_H

class CColorDlg : public CDialog
{
// コンストラクション
public:
	CColorDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_COLOR };
	int m_Color;

// オペレーション
	void SetValue(int prm1);
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___COLORDLG_H

///////////////////////////////////////////////////////////////////////////////
// CCurveDlg ダイアログ

#ifndef ___CURVEDLG_H
#define ___CURVEDLG_H

class CCurveDlg : public CDialog
{
// コンストラクション
public:
	CCurveDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_CURVE };
	int m_Curve;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___CURVEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPrimitiveDlg ダイアログ

#ifndef ___PRIMITIVEDLG_H
#define ___PRIMITIVEDLG_H

class CPrimitiveDlg : public CDialog
{
// コンストラクション
public:
	CPrimitiveDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_PRIMITIVE };
	int m_Primitive;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___PRIMITIVEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CBooleanDlg ダイアログ

#ifndef ___BOOLEANDLG_H
#define ___BOOLEANDLG_H

class CBooleanDlg : public CDialog
{
// コンストラクション
public:
	CBooleanDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_BOOLEAN };
	int m_Boolean;
	int m_DeleteB;

// オペレーション
	void GetValue(int* prm1, int* prm2);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___BOOLEANDLG_H

///////////////////////////////////////////////////////////////////////////////
// CGemsetDlg ダイアログ

#ifndef ___GEMSETDLG_H
#define ___GEMSETDLG_H

class CGemsetDlg : public CDialog
{
// コンストラクション
public:
	CGemsetDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_GEM_SETTING };
	int m_GemSet;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___GEMSETDLG_H

///////////////////////////////////////////////////////////////////////////////
// CGemDlg ダイアログ

#ifndef ___GEMDLG_H
#define ___GEMDLG_H

class CGemDlg : public CDialog
{
// コンストラクション
public:
	CGemDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_GEM };
	int m_Gem;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___GEMDLG_H

///////////////////////////////////////////////////////////////////////////////
// CRevolveDlg ダイアログ

#ifndef ___REVOLVEDLG_H
#define ___REVOLVEDLG_H

class CRevolveDlg : public CDialog
{
// コンストラクション
public:
	CRevolveDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_REVOLVE };
	int m_Revolve;

// オペレーション
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___REVOLVEDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetFuncModeDlg ダイアログ

#ifndef ___SETFUNCMODEDLG_H
#define ___SETFUNCMODEDLG_H

class CSetFuncModeDlg : public CDialog
{
// コンストラクション
public:
	CSetFuncModeDlg(CWnd* pParent=NULL);								// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_FUNCMODE };
	int m_FuncMode;

// オペレーション
	void SetValue(int prm1);
	void GetValue(int* prm1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___SETFUNCMODEDLG_H

//==============================<EOF:ButtonDlg.h>============================