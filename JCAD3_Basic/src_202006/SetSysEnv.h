/////////////////////////////////////////////////////////////////////////////
// システム環境設定プロパティシートクラス宣言
// SetSysEnv.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/17 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg1 ダイアログ

#ifndef ___SYSENVDLG1_H
#define ___SYSENVDLG1_H

class CSysEnvDlg1 : public CPropertyPage
{
// コンストラクション
public:
	CSysEnvDlg1();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SYSENV_SHT1 };
	int	m_Key1;
	int	m_Key2;
	int	m_Key3;
	int	m_Key4;
	int	m_Key5;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg void OnInitData(void);
	afx_msg void OnSaveData(void);
	afx_msg void OnRestoreData(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___SYSENVDLG1_H

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg2 ダイアログ

#ifndef ___SYSENVDLG2_H
#define ___SYSENVDLG2_H

class CSysEnvDlg2 : public CPropertyPage
{
// コンストラクション
public:
	CSysEnvDlg2();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SYSENV_SHT2 };
	int	m_Init1;
	int	m_Init2;
	int	m_Init3;
	int	m_Init4;
	int	m_Init5;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg void OnInitData(void);
	afx_msg void OnSaveData(void);
	afx_msg void OnRestoreData(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___SYSENVDLG2_H

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg3 ダイアログ

#ifndef ___SYSENVDLG3_H
#define ___SYSENVDLG3_H

class CSysEnvDlg3 : public CPropertyPage
{
// コンストラクション
public:
	CSysEnvDlg3();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SYSENV_SHT3 };
	int	m_Btn1;
	int	m_Btn2;
	int	m_Btn3;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg void OnInitData(void);
	afx_msg void OnSaveData(void);
	afx_msg void OnRestoreData(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___SYSENVDLG3_H

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg4 ダイアログ

#ifndef ___SYSENVDLG4_H
#define ___SYSENVDLG4_H

class CSysEnvDlg4 : public CPropertyPage
{
// コンストラクション
public:
	CSysEnvDlg4();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SYSENV_SHT4 };
	int m_CRepeat;
	double m_CWeight;
	int m_Attr;
	int m_Sect;
	int	m_Shape;
	int	m_Shade;
	int m_SaveMax;
	CEdit m_Edit4;
	CString m_SavePath;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg void OnInitData(void);
	afx_msg void OnSaveData(void);
	afx_msg void OnRestoreData(void);
	afx_msg void OnSavePathBtn(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___SYSENVDLG4_H

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg5 ダイアログ

#ifndef ___SYSENVDLG5_H
#define ___SYSENVDLG5_H

class CSysEnvDlg5 : public CPropertyPage
{
// コンストラクション
public:
	CSysEnvDlg5();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SYSENV_SHT5 };
	int				m_Kind;
	int				m_RCol;
	CSpinButtonCtrl	m_Spin1;
	int				m_GCol;
	CSpinButtonCtrl	m_Spin2;
	int				m_BCol;
	CSpinButtonCtrl	m_Spin3;
	CStatic			m_Pic1;
	CStatic			m_Pic2;
	CStatic			m_Pic3;
	CStatic			m_Pic4;
	CStatic			m_Pic5;
	CStatic			m_Pic6;
	CStatic			m_Pic7;
	CStatic			m_Pic8;
	CStatic			m_Pic9;
	CStatic			m_Pic10;
	CStatic			m_Pic11;
	CStatic			m_Pic12;
	CStatic			m_Pic13;
	CStatic			m_Pic14;
	CStatic			m_Pic15;
	CStatic			m_Pic16;
	CStatic			m_Pic17;
	CStatic			m_Pic18;
	CStatic			m_Pic19;
	CStatic			m_Pic20;
	CStatic			m_Pic21;
	CStatic			m_Pic22;
	CStatic			m_Pic23;
	CStatic			m_Pic24;
	CStatic			m_Pic25;
	CStatic			m_Pic26;
	CStatic			m_Pic27;
	CStatic			m_Pic28;
	CStatic			m_Pic29;
	CStatic			m_Pic30;
	CStatic			m_Pic31;
	CStatic			m_Pic32;
	CStatic			m_Pic33;
	CStatic			m_Pic34;
	CStatic			m_Pic35;
	CStatic			m_Pic36;
	CStatic			m_Pic37;
	CStatic			m_Pic38;
	CStatic			m_Pic39;
	CStatic			m_Pic40;
	CStatic			m_Pic41;
	CStatic			m_Pic42;
	CStatic			m_Pic43;
	CStatic			m_Pic44;
	CStatic			m_Pic45;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg void OnPaint(void);
	virtual BOOL OnInitDialog(void);
	afx_msg void OnInitFile1(void);
	afx_msg void OnInitFile2(void);
	afx_msg void OnSaveData(void);
	afx_msg void OnRestoreData(void);
	afx_msg BOOL OnColorKind(UINT nID);
	afx_msg void OnSetColor(void);
	DECLARE_MESSAGE_MAP()
	void ColorPaint(CPaintDC *dc);

private:
	int   m_Check;
	CRect m_SQ[45];

public:
	BOOL     m_Update;
	COLORREF m_Color[45];
};

#endif ___SYSENVDLG5_H

///////////////////////////////////////////////////////////////////////////////
// CSysEnvDlg6 ダイアログ

#ifndef ___SYSENVDLG6_H
#define ___SYSENVDLG6_H

class CSysEnvDlg6 : public CPropertyPage
{
// コンストラクション
public:
	CSysEnvDlg6();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_SYSENV_SHT6 };
	int	m_Num1;
	int	m_Num2;
	int	m_Num3;
	int	m_Num4;
	int	m_Num5;
	int	m_Num6;
	int	m_Num7;
	int	m_Num8;
	int	m_Num9;
	int	m_Num10;
	int	m_Num11;
	int	m_Num12;
	int	m_Num13;
	int	m_Num14;
	int	m_Num15;
	int	m_Num16;
	int	m_Num17;
	int	m_Num18;
	int	m_Num19;
	int	m_Num20;
	int	m_Num21;
	int	m_Num22;
	CString m_Num23;
	CString m_Num24;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

	DECLARE_MESSAGE_MAP()
};

#endif ___SYSENVDLG6_H

///////////////////////////////////////////////////////////////////////////////
// CSysEnvSheet プロパティシート

#ifndef ___SYSENVSHT_H
#define ___SYSENVSHT_H

class CSysEnvSheet : public CPropertySheet
{
// 構築
public:
	CSysEnvSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSysEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void AddControlPages(void);

// アトリビュート
public:

	CSysEnvDlg1	m_SysEnvPage1;
	CSysEnvDlg2	m_SysEnvPage2;
	CSysEnvDlg3	m_SysEnvPage3;
	CSysEnvDlg4	m_SysEnvPage4;
	CSysEnvDlg5	m_SysEnvPage5;
	CSysEnvDlg6	m_SysEnvPage6;

// オペレーション
public:
	void     SetValue(int p1, int p2, double p3, int p4, int p5, int p6, int p7, int p8,
                      CString p9, int p10, int p11, int p12, int p13, int p14, int p15,
                      int p16, int p17, int p18, int p19, int p20, int p21, int p22);
	void     GetValue(int* p1, int* p2, double* p3, int* p4, int* p5, int* p6, int* p7, int* p8,
                      CString* p9, int* p10, int* p11, int* p12, int* p13, int* p14, int* p15,
                      int* p16, int* p17, int* p18, int* p19, int* p20, int* p21, int* p22);
	BOOL     GetCUpdate(void);
	COLORREF GetColor(int no);

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSysEnvSheet)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSysEnvSheet();
	virtual BOOL OnInitDialog();

	// 生成されたメッセージ マップ関数
protected:

	//{{AFX_MSG(CSysEnvSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif ___SYSENVSHT_H

//=============================<EOF:SetSysEnv.h>=============================