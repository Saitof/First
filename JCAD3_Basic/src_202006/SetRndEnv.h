/////////////////////////////////////////////////////////////////////////////
// レンダリング環境設定プロパティシートクラス宣言
// SetRndEnv.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg1 ダイアログ

#ifndef ___RNDENVDLG1_H
#define ___RNDENVDLG1_H

class CRndEnvDlg1 : public CPropertyPage
{
// コンストラクション
public:
	CRndEnvDlg1();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
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

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
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
// CRndEnvDlg2 ダイアログ

#ifndef ___RNDENVDLG2_H
#define ___RNDENVDLG2_H

class CRndEnvDlg2 : public CPropertyPage
{
// コンストラクション
public:
	CRndEnvDlg2();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
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

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
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
// CRndEnvDlg3 ダイアログ

#ifndef ___RNDENVDLG3_H
#define ___RNDENVDLG3_H

class CRndEnvDlg3 : public CPropertyPage
{
// コンストラクション
public:
	CRndEnvDlg3();														// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_RNDENV_SHT3 };
	CString	m_FileName;
	CEdit	m_Edit1;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// インプリメンテーション
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
// CRndEnvSheet プロパティシート

#ifndef ___RNDENVSHT_H
#define ___RNDENVSHT_H

class CRndEnvSheet : public CPropertySheet
{
// 構築
public:
	CRndEnvSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CRndEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

protected:
	void AddControlPages(void);

// アトリビュート
public:

	CRndEnvDlg1	m_RndEnvPage1;
	CRndEnvDlg2	m_RndEnvPage2;
	CRndEnvDlg3	m_RndEnvPage3;
// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CRndEnvSheet)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CRndEnvSheet();
	virtual BOOL OnInitDialog();

	// 生成されたメッセージ マップ関数
protected:

	//{{AFX_MSG(CRndEnvSheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif ___RNDENVSHT_H

//=============================<EOF:SetRndEnv.h>=============================
