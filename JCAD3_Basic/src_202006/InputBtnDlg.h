/////////////////////////////////////////////////////////////////////////////
// 数値入力ダイアログクラス(釦対応)宣言
// InputBtnDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/17 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ
#include "DataType.h"

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg1 ダイアログ

#ifndef ___BTNDLG1_H
#define ___BTNDLG1_H

class CInputBtnDlg1 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg1(int kind=0, CWnd* pParent=NULL);						// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN1 };
	double m_Param1;
	double m_Param2;
	double m_Param3;
	double m_Param4;

protected:
	CBitmapButton Plus1;
	CBitmapButton Minus1;
	CBitmapButton Plus2;
	CBitmapButton Minus2;
	CBitmapButton Plus3;
	CBitmapButton Minus3;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetParam1(int cn, PNTTYPE sc, int ch, SCRTYPE bp, double ad);
	void SetParam2(PNTTYPE cp, double dx, double dy, double dz, double ad);
    void GetParam(double* ad);

// インプリメンテーション
protected:
	void ObjectEdit(void);
	BOOL DataCheck(void);
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	DECLARE_MESSAGE_MAP()

private:
	int		m_Kind, m_nID, m_Init, m_Copy, m_SChk;
	double	m_Xsize, m_Ysize, m_Zsize;
	PNTTYPE	m_Scale, m_Cent;
	SCRTYPE	m_Base;
};

#endif ___BTNDLG1_H

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg2 ダイアログ

#ifndef ___BTNDLG2_H
#define ___BTNDLG2_H

class CInputBtnDlg2 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg2(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN2 };
	double m_Param1;
	double m_Param2;
	double m_Param3;
	double m_Param4;

protected:
	CBitmapButton Plus1;
	CBitmapButton Minus1;
	CBitmapButton Plus2;
	CBitmapButton Minus2;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetParam(int cn, PNTTYPE sc, int ch, SCRTYPE bp,
                  PNTTYPE cp, int pl, double ad1, double ad2);
    void GetParam(double* ad1, double* ad2);

// インプリメンテーション
protected:
	void ObjectEdit(void);
	BOOL DataCheck(void);
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	DECLARE_MESSAGE_MAP()

private:
	int		m_nID, m_Init, m_Copy, m_SChk, m_Plane;
	PNTTYPE	m_Scale, m_Cent;
	SCRTYPE	m_Base;
};

#endif ___BTNDLG2_H

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg3 ダイアログ

#ifndef ___BTNDLG3_H
#define ___BTNDLG3_H

class CInputBtnDlg3 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg3(int kind=0, CWnd* pParent=NULL);						// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN3 };
	double m_Param1;
	double m_Param2;

protected:
	CBitmapButton Plus1;
	CBitmapButton Minus1;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetParam1(int fg, int fc, int vc, PNTTYPE sp, PNTTYPE ep, double ad);
	void SetParam2(int un, int vn, int uf, int vf, int sf, int bs, int* sa,
                   int sn, OBJTYPE* op, PNTTYPE* cp, VECTYPE* vp, PNTTYPE* pa,
                   int* ia, PNTTYPE* pb, PNTTYPE* pc, int* ta, double ad);
	void SetParam3(int un, int vn, int uf, int vf, int sf, int bn, int dn,
                   int bs, int* sa, int sn, OBJTYPE* op, PNTTYPE* cp, VECTYPE* vp,
                   PNTTYPE* pa, int* ia, PNTTYPE* pb, PNTTYPE* pc, int* ta, double ad);
	void SetParam4(int dr, double ln, PNTTYPE cp, double ad);
	void SetParam6(int d1, int d2, int pl, PNTTYPE sp, PNTTYPE ep, double ad);
    void GetParam(double* ad);

// インプリメンテーション
protected:
	void ObjectEdit(void);
	BOOL DataCheck(void);
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	DECLARE_MESSAGE_MAP()

private:
	int		m_Kind, m_nID, m_Init;
	int		m_Fg, m_Fc, m_Vc, m_Dr, m_Un, m_Vn, m_Uf, m_Vf;
	int		m_Sf, m_Bs, m_Sn, m_Bn, m_Dn, m_Dr1, m_Dr2, m_Pl;
	int		*m_Ia, *m_Ta, *m_Sa;
	double	m_Len, m_Ln;
	PNTTYPE	m_Spnt, m_Epnt, m_Cpnt, *m_Pa, *m_Pb, *m_Pc, *m_Cp;
	VECTYPE	*m_Vp;
	OBJTYPE	*m_Op;
};

#endif ___BTNDLG3_H

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg4 ダイアログ

#ifndef ___BTNDLG4_H
#define ___BTNDLG4_H

class CInputBtnDlg4 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg4(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN4 };
	double m_Param1;
	double m_Param2;
	double m_Param3;
	double m_Param4;
	double m_Param5;
	double m_Param6;

protected:
	CBitmapButton Plus1;
	CBitmapButton Minus1;
	CBitmapButton Plus2;
	CBitmapButton Minus2;
	CBitmapButton Plus3;
	CBitmapButton Minus3;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetParam(int pl, int cn, int lno, double len, int* mna,
                  int* mxa, PNTTYPE* ocp, PNTTYPE* mcp, VECTYPE* cvc,
                  VECTYPE* rvc, double ad1, double ad2, double ad3);
    void GetParam(double* ad1, double* ad2, double* ad3);

// インプリメンテーション
protected:
	void ObjectEdit(void);
	BOOL DataCheck(void);
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	DECLARE_MESSAGE_MAP()

private:
	int		m_nID, m_Plane, m_Cno, m_Lno, *m_Min, *m_Max;
	double	m_Len, m_Param1Sv, m_Param2Sv, m_Param3Sv;
	PNTTYPE	*m_Ocp, *m_Mcp;
	VECTYPE	*m_Cvc, *m_Rvc;
};

#endif ___BTNDLG4_H

//============================<EOF:InputBtnDlg.h>============================
