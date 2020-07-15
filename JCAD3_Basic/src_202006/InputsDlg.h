/////////////////////////////////////////////////////////////////////////////
// 複数項目設定ダイアログクラス宣言
// InputsDlg.h
//---------------------------------------------------------------------------
// LastEdit: 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

///////////////////////////////////////////////////////////////////////////////
// CPrtParamDlg ダイアログ

#ifndef ___PRTPARAMDLG_H
#define ___PRTPARAMDLG_H

class CPrtParamDlg : public CDialog
{
// コンストラクション
public:
	CPrtParamDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_PRTPARAM };
	int     m_PMode;
	int     m_PColor;
	int     m_POut;
	int     m_POri;
	CStatic m_Static1;
	CButton m_RButton1;
	CButton m_RButton2;
	int     m_PPos;
	CStatic m_Static2;
	CButton m_RButton3;
	CButton m_RButton4;
	CButton m_CCheck1;
	BOOL    m_Check1;
	CButton m_CCheck2;
	BOOL    m_Check2;
	CStatic m_Static3;
	CEdit   m_Edit1;
	int     m_PSize;
	CSpinButtonCtrl m_Spin;
	CStatic m_Static4;
	CButton m_Button1;

// オペレーション
	void SetValue(int p1, int p2, int p3, int p4, int p5, int p6, CString p7);
	void GetValue(int* p1, int* p2, int* p3, int* p4, int* p5, int* p6,
                  CString* p7, int* p8, int* p9, CString* p10);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnPrintMode(UINT nID);
	afx_msg BOOL OnPrintOut(UINT nID);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	afx_msg void OnFileSave(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sPicFile, m_sPicPath;
};

#endif ___PRTPARAMDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPrtParam2Dlg ダイアログ

#ifndef ___PRTPARAM2DLG_H
#define ___PRTPARAM2DLG_H

class CPrtParam2Dlg : public CDialog
{
// コンストラクション
public:
	CPrtParam2Dlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_PRTPARAM2 };
	int     m_POut;
	int     m_POri;
	CStatic m_Static1;
	CButton m_RButton1;
	CButton m_RButton2;
	int     m_PPos;
	CStatic m_Static2;
	CButton m_RButton3;
	CButton m_RButton4;
	CButton m_CCheck1;
	BOOL    m_Check1;
	CStatic m_Static3;
	CEdit   m_Edit1;
	int     m_PSize;
	CSpinButtonCtrl m_Spin;
	CStatic m_Static4;
	CButton m_Button1;

// オペレーション
	void SetValue(int p1, int p2, int p3, int p4, CString p5);
	void GetValue(int* p1, int* p2, int* p3, int* p4,
                  CString* p5, int* p6, CString* p7);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnPrintOut(UINT nID);
	afx_msg void OnCheck1(void);
	afx_msg void OnFileSave(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sPicFile, m_sPicPath;
};

#endif ___PRTPARAM2DLG_H

///////////////////////////////////////////////////////////////////////////////
// CCopyInfoDlg ダイアログ

#ifndef ___COPYINFODLG_H
#define ___COPYINFODLG_H

class CCopyInfoDlg : public CDialog
{
// コンストラクション
public:
	CCopyInfoDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_COPY_INFO };
	int    m_CopyNum;
	double m_XScale;
	double m_YScale;
	double m_ZScale;
	int    m_SynFlg;
	int    m_XPoint;
	int    m_YPoint;
	int    m_ZPoint;

// オペレーション
	void SetValue(int p1, double p2, double p3, double p4,
                  int p5, int p6, int p7, int p8);
	void GetValue(int* p1, double* p2, double* p3, double* p4,
                  int* p5, int* p6, int* p7, int* p8);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___COPYINFODLG_H

///////////////////////////////////////////////////////////////////////////////
// CSpiralDlg ダイアログ

#ifndef ___SPIRALDLG_H
#define ___SPIRALDLG_H

class CSpiralDlg : public CDialog
{
// コンストラクション
public:
	CSpiralDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SPIRAL };
	double m_Diameter;
	double m_Height;
	int    m_CtrlNum;
	int    m_SprlNum;

// オペレーション
	void SetValue(double p1, double p2, int p3, int p4);
	void GetValue(double* p1, double* p2, int* p3, int* p4);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___SPIRALDLG_H

///////////////////////////////////////////////////////////////////////////////
// CTensionDlg ダイアログ

#ifndef ___TENSIONDLG_H
#define ___TENSIONDLG_H

class CTensionDlg : public CDialog
{
// コンストラクション
public:
	CTensionDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_TENSION };
	int m_Tension;
	int m_Check1;

// オペレーション
	void SetValue(int p1);
	void GetValue(int* p1, int* p2);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___TENSIONDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetTmpDlg ダイアログ

#ifndef ___SETTMPDLG_H
#define ___SETTMPDLG_H

class CSetTmpDlg : public CDialog
{
// コンストラクション
public:
	CSetTmpDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_TEMPLATE };
	int     m_Knd;
	int     m_Pln;
	CButton m_Button1;
	CButton m_Button2;
	CString m_sTmpName[8][3];
	CEdit   m_Edit[8][3];

// オペレーション
	void SetValue1(CString p1);
	void SetValue2(int no, CString p1, CString p2, CString p3);
	void GetValue1(CString* p1);
	void GetValue2(int no, CString* p1, CString* p2, CString* p3);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn1(UINT nID);
	afx_msg BOOL OnCheckBtn2(UINT nID);
	afx_msg void OnFileOpen(void);
	afx_msg void OnFileClear(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sTmpPath, m_sTmpFile[8][3];
};

#endif ___SETTMPDLG_H

///////////////////////////////////////////////////////////////////////////////
// CCaptureDlg ダイアログ

#ifndef ___CAPTUREDLG_H
#define ___CAPTUREDLG_H

class CCaptureDlg : public CDialog
{
// コンストラクション
public:
	CCaptureDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_CAPTURE };
	int     m_CPlane;
	int     m_CPos;
	int     m_PKind;
	int     m_PWidth;
	CSpinButtonCtrl m_Spin1;
	int     m_PHeight;
	CSpinButtonCtrl m_Spin2;
	CStatic m_Static1;
	CEdit   m_Edit3;
	int     m_JComp;
	CSpinButtonCtrl m_Spin3;
	CStatic m_Static2;

// オペレーション
	void SetValue(int p1, int p2, int p3, int p4, int p5, int p6);
	void GetValue(int* p1, int* p2, int* p3, int* p4, int* p5, int* p6);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg BOOL OnPictureKind(UINT nID);
	DECLARE_MESSAGE_MAP()
};

#endif ___CAPTUREDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetTextDlg ダイアログ

#ifndef ___SETTEXTDLG_H
#define ___SETTEXTDLG_H

class CSetTextDlg : public CDialog
{
// コンストラクション
public:
	CSetTextDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_TEXT };
	CComboBox m_Combo1;
	CString m_FontName;
	CString m_Text;
	double  m_Height;
	double  m_Depth;
	double  m_Intval;
	BOOL    m_Check1;

// オペレーション
	void GetFontInfo(void);
	void SetValue(int p1, int p2, double p3, double p4, double p5);
	void GetValue(int* p1, int* p2, double* p3, double* p4, double* p5);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	DECLARE_MESSAGE_MAP()

private:
	int  m_FontNo, m_FontLeng, m_FontNum;
	char m_FontPath[_MAX_PATH], m_FNameAra[512][128], m_FFileAra[512][128];
};

#endif ___SETTEXTDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetTexDlg ダイアログ

#ifndef ___SETTEXDLG_H
#define ___SETTEXDLG_H

class CSetTexDlg : public CDialog
{
// コンストラクション
public:
	CSetTexDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_TEXTURE };
	int     m_Plane;
	CButton	m_Button1;
	CButton	m_Button2;
	CString	m_sTexName[3];
	double  m_Move1, m_Move2;
	int     m_Rotate;
	float   m_Scale;

protected:
	CBitmapButton Left;
	CBitmapButton Right;
	CBitmapButton Down;
	CBitmapButton Up;
	CBitmapButton Left2;
	CBitmapButton Right2;
	CBitmapButton Up2;
	CBitmapButton Down2;

// オペレーション

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg BOOL OnCheckBtn1(UINT nID);
	afx_msg BOOL OnCheckBtn2(UINT nID);
	virtual BOOL OnInitDialog(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	afx_msg void OnFileOpen(void);
	afx_msg void OnFileClear(void);
	DECLARE_MESSAGE_MAP()

private:
	CString m_sTexPath, m_sTexFile[3];
	int     m_nID, m_Mode, m_Flg;
};

#endif ___SETTEXDLG_H

///////////////////////////////////////////////////////////////////////////////
// CProxyDlg ダイアログ

#ifndef ___PROXYDLG_H
#define ___PROXYDLG_H

class CProxyDlg : public CDialog
{
// コンストラクション
public:
	CProxyDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_PROXY_DLG };
	BOOL    m_Check1;
	BOOL    m_Check2;
	CButton m_CCheck;
	CStatic m_Static1;
	CEdit   m_Edit1;
	CString m_ProxyAdress;
	CStatic m_Static2;
	CEdit   m_Edit2;
	int     m_ProxyPort;

// オペレーション
	void GetValue(BOOL* p1, BOOL* p2, CString* p3, int* p4);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___PROXYDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPasswdDlg ダイアログ

#ifndef ___PASSWDDLG_H
#define ___PASSWDDLG_H

class CPasswdDlg : public CDialog
{
// コンストラクション
public:
	CPasswdDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_PASSWD_DLG };
	CString m_WPwdData1;
	CString m_WPwdData2;
	BOOL    m_Check1;
	CStatic m_Static1;
	CStatic m_Static2;
	CEdit   m_Edit3;
	CEdit   m_Edit4;
	CString m_RPwdData1;
	CString m_RPwdData2;

// オペレーション
	void SetValue(CString p1, CString p2, int p3);
	void GetValue(CString* p1, CString* p2, int* p3);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnCheck1(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___PASSWDDLG_H

///////////////////////////////////////////////////////////////////////////////
// CPasswdDlg2 ダイアログ

#ifndef ___PASSWDDLG2_H
#define ___PASSWDDLG2_H

class CPasswdDlg2 : public CDialog
{
// コンストラクション
public:
	CPasswdDlg2(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_PASSWD_DLG2 };
	CString m_WPwdData;

// オペレーション
	void GetValue(CString* p1);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual void OnOK(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___PASSWDDLG2_H

///////////////////////////////////////////////////////////////////////////////
// CCurveDiviDlg ダイアログ

#ifndef ___CURVEDIVIDLG_H
#define ___CURVEDIVIDLG_H

class CCurveDiviDlg : public CDialog
{
// コンストラクション
public:
	CCurveDiviDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_CURVE_DIVI };
	int m_CurveDiv;
	int m_Polygon;
	int m_FileBUp;

// オペレーション
	void SetValue(int p1, int p2, int p3);
	void GetValue(int* p1, int* p2, int* p3);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___CURVEDIVIDLG_H

///////////////////////////////////////////////////////////////////////////////
// CSetProngDlg ダイアログ

#ifndef ___SETPRONGDLG_H
#define ___SETPRONGDLG_H

class CSetProngDlg : public CDialog
{
// コンストラクション
public:
	CSetProngDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SET_PRONG };
	int    m_ProngN;
	double m_ProngL;
	double m_ProngD;
	double m_ProngR;

// オペレーション
	void SetValue(int p1, double p2, double p3, double p4);
	void GetValue(int* p1, double* p2, double* p3, double* p4);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	DECLARE_MESSAGE_MAP()

};

#endif ___SETPRONGDLG_H

///////////////////////////////////////////////////////////////////////////////
// CLimitTrackerDlg ダイアログ

#ifndef ___LIMITTRACKERDLG_H
#define ___LIMITTRACKERDLG_H

class CLimitTrackerDlg : public CDialog
{
// コンストラクション
public:
	CLimitTrackerDlg(CWnd* pParent=NULL);								// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_LIMIT_TRACKER };
	double m_XBase;
	double m_YBase;
	double m_ZBase;
	BOOL   m_XCheck1;
	BOOL   m_XCheck2;
	BOOL   m_YCheck1;
	BOOL   m_YCheck2;
	BOOL   m_ZCheck1;
	BOOL   m_ZCheck2;

// オペレーション
	void SetValue(double p1, double p2, double p3,
                  int p4, int p5, int p6, int p7, int p8, int p9);
	void GetValue(double* p1, double* p2, double* p3,
                  int* p4, int* p5, int* p6, int* p7, int* p8, int* p9);

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	DECLARE_MESSAGE_MAP()
};

#endif ___LIMITTRACKERDLG_H

//=============================<EOF:InputsDlg.h>=============================