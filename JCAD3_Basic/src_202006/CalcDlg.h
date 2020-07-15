/////////////////////////////////////////////////////////////////////////////
// 計算機ダイアログクラス宣言
// CalcDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

///////////////////////////////////////////////////////////////////////////////
// CCalcDlg1 ダイアログ

#ifndef ___CALCDLG1_H
#define ___CALCDLG1_H

class CCalcDlg1 : public CDialog
{
// コンストラクション
public:
	CCalcDlg1(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_CALC_DLG1 };
	CString m_strNumber;
	CEdit	m_Edit1;
	CString m_strType;
	CEdit	m_Edit2;

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
	afx_msg void OnDestroy(void);
	afx_msg void OnInputKey1(void);
	afx_msg void OnInputKey2(void);
	afx_msg void OnInputKey3(void);
	afx_msg void OnInputKey4(void);
	afx_msg void OnInputKey5(void);
	afx_msg void OnInputKey6(void);
	afx_msg void OnInputKey7(void);
	afx_msg void OnInputKey8(void);
	afx_msg void OnInputKey9(void);
	afx_msg void OnInputKey0(void);
	afx_msg void OnInputKeyDot(void);
	afx_msg void OnInputKeyPai(void);
	afx_msg void OnInputKeyAddition(void);
	afx_msg void OnInputKeySubtract(void);
	afx_msg void OnInputKeyMultiply(void);
	afx_msg void OnInputKeyDivision(void);
	afx_msg void OnInputKeyEqual(void);
	afx_msg void OnInputKeyC(void);
	afx_msg void OnInputKeyCE(void);
	afx_msg void OnInputKeySign(void);
	afx_msg void OnInputKeyCopy(void);
	afx_msg void OnInputKeyOval(void);
	DECLARE_MESSAGE_MAP()
	void InputKeyData(CString strKey);
    void CheckInputData(void);
	void ChangeKeyData(void);
	void CalcKeyData(void);

private:
	CString m_strSign;
	double  m_Number, m_Value;
	int     m_Type, m_Type2, m_Sign, m_Count, m_StartFlg;
	CWnd*   m_pParent;
};

#endif ___CALCDLG1_H

///////////////////////////////////////////////////////////////////////////////
// CCalcDlg2 ダイアログ

#ifndef ___CALCDLG2_H
#define ___CALCDLG2_H

class CCalcDlg2 : public CDialog
{
// コンストラクション
public:
	CCalcDlg2(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_CALC_DLG2 };
	CString m_strNumber1;
	CEdit	m_Edit1;
	CString m_strNumber2;
	CEdit	m_Edit2;
	CString m_strNumber3;
	CEdit	m_Edit3;
	CString m_strNumber4;
	CEdit	m_Edit4;
	CEdit	m_Edit5;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void SetParam(CString a, CString b, CString c, CString d);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg void OnPasteData1(void);
	afx_msg void OnCopyData1(void);
	afx_msg void OnPasteData2(void);
	afx_msg void OnCopyData2(void);
	afx_msg void OnHelpProc(void);
	afx_msg void OnPasteData3(void);
	afx_msg void OnCopyData3(void);
	afx_msg void OnInputCls(void);
	afx_msg void OnCopyData4(void);
	afx_msg void OnExeProc(void);
	afx_msg void OnChangeCalc(void);
	DECLARE_MESSAGE_MAP()
    void CheckInputData(void);
	double CalcCircumference(double a, double b);

private:
	double m_Number1, m_Number2, m_Number3;
	int    m_Size, m_Width, m_Height;
	CWnd*  m_pParent;
};

#endif ___CALCDLG2_H

//=============================<EOF:CalcDlg.h>============================
