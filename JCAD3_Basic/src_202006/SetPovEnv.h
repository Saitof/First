/////////////////////////////////////////////////////////////////////////////
// POV-Rayデータ出力環境設定プロパティシートクラス宣言
// SetPovEnv.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/24 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg1 ダイアログ

#ifndef ___POVENVDLG1_H
#define ___POVENVDLG1_H

class CPovEnvDlg1 : public CPropertyPage
{
// コンストラクション
public:
	CPovEnvDlg1();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_POVENV_SHT1 };
	CString m_OutName;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	DECLARE_MESSAGE_MAP()
};

#endif ___POVENVDLG1_H

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg2 ダイアログ

#ifndef ___POVENVDLG2_H
#define ___POVENVDLG2_H

class CPovEnvDlg2 : public CPropertyPage
{
// コンストラクション
public:
	CPovEnvDlg2();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_POVENV_SHT2 };
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
	CEdit   m_Edit13;
	CEdit   m_Edit14;
	CStatic m_Static1;
	CStatic m_Static2;
	CStatic m_Static3;
	CStatic m_Static4;
	CStatic m_Static5;
	CStatic m_Static6;
	CStatic m_Static7;
	CStatic m_Static8;
	CStatic m_Static9;
	CStatic m_Static10;
	CStatic m_Static11;
	CStatic m_Static12;
	CStatic m_Static13;
	CStatic m_Static14;
	CStatic m_Static15;
	CStatic m_Static16;
	CStatic m_Static17;
	CStatic m_Static18;
	CStatic m_Static19;
	CStatic m_Static20;
	CStatic m_Static21;
	CStatic m_Static22;
	CStatic m_Static23;
	CStatic m_Static24;
	CString m_InName;
	CString m_OutPath;
	BOOL    m_bSmooth;
	double  m_Angle;
	BOOL    m_bTranslate;
	double  m_XTranslate;
	double  m_YTranslate;
	double  m_ZTranslate;
	BOOL    m_bRotate;
	double  m_XRotate;
	double  m_YRotate;
	double  m_ZRotate;
	BOOL    m_bScale;
	double  m_XScale;
	double  m_YScale;
	double  m_ZScale;
	BOOL    m_bAnima;
	int     m_ZAnima;
	int     m_YAnima;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
public:
	void PermissionCheck(void);

protected:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnInitFile(void);
	afx_msg void OnSaveFile(void);
	afx_msg void OnRestoreFile(void);
	afx_msg void OnInfileBtn(void);
	afx_msg void OnOutpathBtn(void);
	afx_msg void OnCheck1(void);
	afx_msg void OnCheck2(void);
	afx_msg void OnCheck3(void);
	afx_msg void OnCheck4(void);
	afx_msg void OnCheck5(void);
	DECLARE_MESSAGE_MAP()

public:
	CString m_sFile;
	CString	m_sPath;
};

#endif ___POVENVDLG2_H

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg3 ダイアログ

#ifndef ___POVENVDLG3_H
#define ___POVENVDLG3_H

class CPovEnvDlg3 : public CPropertyPage
{
// コンストラクション
public:
	CPovEnvDlg3();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_POVENV_SHT3 };
	CEdit   m_Edit1;
	CEdit   m_Edit2;
	CString m_DefName1;
	CString m_DefName2;
	int     m_Target;
	int     m_Collect;
	int     m_Reflection;
	int     m_Refraction;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
protected:
	afx_msg void OnInitFile(void);
	afx_msg void OnSaveFile(void);
	afx_msg void OnRestoreFile(void);
	afx_msg void OnInfileBtn1(void);
	afx_msg void OnInfileBtn2(void);
	DECLARE_MESSAGE_MAP()

public:
	CString m_sDefFile1, m_sDefFile2;
	CString m_sDefPath;
};

#endif ___POVENVDLG3_H

///////////////////////////////////////////////////////////////////////////////
// CPovEnvDlg4 ダイアログ

#ifndef ___POVENVDLG4_H
#define ___POVENVDLG4_H

class CPovEnvDlg4 : public CPropertyPage
{
// コンストラクション
public:
	CPovEnvDlg4();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_POVENV_SHT4 };
	int       m_No;
	int       m_Shape;
	CStatic   m_Static1;
	CStatic   m_Static2;
	CStatic   m_Static3;
	CStatic   m_Static4;
	CComboBox m_Combo1;
	CComboBox m_Combo2;
	CComboBox m_Combo3;
	CComboBox m_Combo4;
	CComboBox m_Combo5;
	CString   m_Texture1;
	CString   m_Texture2;
	CString   m_Texture3;
	CString   m_Texture4;
	CString   m_Texture5;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
public:
	void ChangeComboList(void);
	void SetCombo1(void);
	void SetCombo2(void);
	void SetCombo3(void);
	void SetCombo4(void);
	void PermissionCheck(void);

protected:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnFileOpen(void);
	afx_msg void OnInitFile(void);
	afx_msg void OnSaveFile(void);
	afx_msg void OnRestoreFile(void);
	afx_msg BOOL OnColorNo(UINT nID);
	afx_msg BOOL OnShapeNo(UINT nID);
	afx_msg void OnSelChange1(void);
	afx_msg void OnSelChange2(void);
	afx_msg void OnSelChange3(void);
	afx_msg void OnSelChange4(void);
	afx_msg void OnSelChange5(void);
	DECLARE_MESSAGE_MAP()

public:
	int     m_PovShape[POVCMAX];									// ｵﾌﾞｼﾞｪｸﾄ形状
	int     m_PovTex1[POVCMAX];										// ﾃｸｽﾁｬNo1
	int     m_PovTex2[POVCMAX];										// ﾃｸｽﾁｬNo2
	int     m_PovTex3[POVCMAX];										// ﾃｸｽﾁｬNo3
	int     m_PovTex4[POVCMAX];										// ﾃｸｽﾁｬNo4
	int     m_PovTex5[POVCMAX];										// ﾃｸｽﾁｬNo5
	CString m_sDefFile1, m_sDefFile2;
	char    m_Tex1_Flg[16], m_Tex2_Flg[128];
	char    m_Tex1, m_Tex2;
	CString m_DefName1[128], m_DefName2[128];
	int     m_DefCnt1, m_DefCnt2;
};

#endif ___POVENVDLG4_H

///////////////////////////////////////////////////////////////////////////////
// CPovEnvSheet プロパティシート

#ifndef ___POVENVSHT_H
#define ___POVENVSHT_H

class CPovEnvSheet : public CPropertySheet
{
// 構築
public:
	CPovEnvSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPovEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void AddControlPages(void);

// アトリビュート
public:

	CPovEnvDlg1 m_PovEnvPage1;
	CPovEnvDlg2 m_PovEnvPage2;
	CPovEnvDlg3 m_PovEnvPage3;
	CPovEnvDlg4 m_PovEnvPage4;

// オペレーション
public:
	void SetPovRayData(void);
	void GetValue(CString* p1);

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CPovEnvSheet)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CPovEnvSheet();
	virtual BOOL OnInitDialog();

	// 生成されたメッセージ マップ関数
protected:

	//{{AFX_MSG(CPovEnvSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif ___POVENVSHT_H

//=============================<EOF:SetPovEnv.h>=============================