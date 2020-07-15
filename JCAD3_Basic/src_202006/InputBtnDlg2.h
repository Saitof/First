/////////////////////////////////////////////////////////////////////////////
// 数値入力ダイアログクラス(釦対応)宣言
// InputBtnDlg2.h
//---------------------------------------------------------------------------
// LastEdit : 2009/09/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ
#include "DataType.h"

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg5 ダイアログ

#ifndef ___BTNDLG5_H
#define ___BTNDLG5_H

class CInputBtnDlg5 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg5(int kind=0, CWnd* pParent=NULL);						// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN5 };
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
	void SetParam1(int md, int sm, int ch, PNTTYPE cp, int dv,
                   int d1, int d2, int bc, double wt, int* pn,
                   PNTTYPE* pa, PNTTYPE* pb, int* ia, int* ib,
                   double* da, double* db, double ad);
	void SetParam2(int sw, int md, int sf, int bc, double wt,
                   int an, int* pn, PNTTYPE* pa, PNTTYPE* pb,
                   PNTTYPE* pc, int* ia, int* ib, int* ic,
                   double* da, double* db, double ad);
    void GetParam(double* ad);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	afx_msg void OnAddCtrl1(void);
	afx_msg void OnAddCtrl2(void);
	afx_msg void OnCancelCtrl(void);
	afx_msg void OnRotate2D(void);
	void AddCtrlPoint(int flg);
	void ObjectEdit(void);
	BOOL DataCheck(void);
	DECLARE_MESSAGE_MAP()

private:
	int     m_Kind, m_nID, m_Sw, m_Md, m_Sm, m_Ch, m_Dv, m_D1;
    int     m_D2, m_Sf, m_Bc, m_An, m_No, *m_Pn, *m_Ia, *m_Ib, *m_Ic;
	double  *m_Da, *m_Db, m_Wt;
	PNTTYPE m_Cp, *m_Pa, *m_Pb, *m_Pc;
};

#endif ___BTNDLG5_H

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg6 ダイアログ

#ifndef ___BTNDLG6_H
#define ___BTNDLG6_H

class CInputBtnDlg6 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg6(int kind=0, CWnd* pParent=NULL);						// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN6 };
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
	void SetParam1(OBJTYPE* op1, OBJTYPE* op2, int md, int fg, int vn,
                   int vb, int xn, int* ch, int dn, int mod, int div,
                   int dr1, int dr2, int fg1, int fg2, int rvf,
                   PNTTYPE pos, PNTTYPE cpt, PNTTYPE* pa, PNTTYPE* pb,
                   PNTTYPE* pc, PNTTYPE* pd, PNTTYPE* pw, PNTTYPE* px,
                   PNTTYPE* py, int* ia, int* ib, int* ic, int* ix,
                   int* ta, double* da, double* db, double ad);
	void SetParam2(OBJTYPE* op1, OBJTYPE* op2, int md, int fg, int un,
                   int vn, int ub, int vb, double wt, int an, int xn,
                   int* ch, int mod, int dr1, int dr2, int rvf,
                   PNTTYPE pos, PNTTYPE cpt, PNTTYPE* pa, PNTTYPE* pb,
                   PNTTYPE* pc, PNTTYPE* pd, PNTTYPE* pw, PNTTYPE* px,
                   int* ia, int* ib, int* ic, int* ix, int* ta, double ad);
	void SetParam3(OBJTYPE* op1, OBJTYPE* op2, int md, int sw, int fg,
                   int un, int vn, int ub, int vb, double wt, int an,
                   int f1, int f2, int xn, int tn, int pl, int* ch,
                   int mod, int dr1, int dr2, int rvf, PNTTYPE pos,
                   PNTTYPE cpt, PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc,
                   PNTTYPE* pd, PNTTYPE* ps, PNTTYPE* pt, PNTTYPE* pw,
                   PNTTYPE* px, PNTTYPE* py, PNTTYPE* pz, int* ia, int* ib,
                   int* ic, int* is, int* iw, int* ix, int* ta, double ad);
	void SetParam4(OBJTYPE* op, int md, int vn, int vb, int pl, int* ch,
                   int mod, int dr1, int dr2, int rvf, PNTTYPE pos,
                   PNTTYPE cpt, PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc,
                   PNTTYPE* pd, PNTTYPE* py, int* ia, int* ib, int* ic,
                   int* ta, double* da, double* db, double ad);
	void SetParam5(OBJTYPE* op1, OBJTYPE* op2, int md, int fg, int un,
                   int vn, int ub, int vb, double wt, int an, int f1,
                   int f2, int tn, int xn, int* ch, int mod, int dr1,
                   int dr2, int rvf, PNTTYPE pos, PNTTYPE cpt, PNTTYPE* pt,
                   PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, PNTTYPE* pd,
                   PNTTYPE* ps, PNTTYPE* pw, PNTTYPE* px, PNTTYPE* pz,
                   int* ia, int* ib, int* iw, int* ix, int* ta, double ad);
    void GetParam(double* ad);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnOK(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg);
	afx_msg void OnTimer(UINT_PTR nIDTimer);
	void ObjectEdit(void);
	void MoveCtrlPoint(void);
	BOOL DataCheck(void);
	DECLARE_MESSAGE_MAP()

private:
	int		m_Kind, m_nID, m_Md, m_Sw, m_Fg, m_Un, m_Vn, m_Ub, m_Vb;
	int		m_An, m_F1, m_F2, m_Xn, m_Tn, m_Pl, *m_Ch;
	int		m_Dn, m_Mod, m_Div, m_Dr1, m_Dr2, m_Fg1, m_Fg2, m_Rvf;
	int		*m_Ia, *m_Ib, *m_Ic, *m_Is, *m_Iw, *m_Ix, *m_Ta;
	double  *m_Da, *m_Db, m_Wt;
	PNTTYPE m_Pos, m_Cpt;
	PNTTYPE *m_Pa, *m_Pb, *m_Pc, *m_Pd, *m_Ps, *m_Pt, *m_Pw, *m_Px, *m_Py, *m_Pz;
	OBJTYPE *m_Op1, *m_Op2;
};

#endif ___BTNDLG6_H

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg7 ダイアログ

#ifndef ___BTNDLG7_H
#define ___BTNDLG7_H

class CInputBtnDlg7 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg7(int kind=0, CWnd* pParent=NULL);						// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN7 };
	double m_MoveX;
	double m_MoveY;
	double m_MoveZ;
	double m_AddMove;
	double m_RotateX;
	double m_RotateY;
	double m_RotateZ;
	double m_AddRotate;
	double m_Scale;
	double m_AddScale;
	int    m_SDirect;
	int    m_SPoint;

protected:
	CBitmapButton Plus1;
	CBitmapButton Minus1;
	CBitmapButton Plus2;
	CBitmapButton Minus2;
	CBitmapButton Plus3;
	CBitmapButton Minus3;
	CBitmapButton Plus4;
	CBitmapButton Minus4;
	CBitmapButton Plus5;
	CBitmapButton Minus5;
	CBitmapButton Plus6;
	CBitmapButton Minus6;
	CBitmapButton Plus7;
	CBitmapButton Minus7;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetParam(double mx, double my, double mz, double rx,
                  double ry, double rz, double sc, double d1,
                  double d2, double d3, int i1, int i2, int i3, int i4);
	void SetParam2(OBJTYPE* op1, OBJTYPE* op2, int fg, int un, int vn,
                   int ub, int vb, int an, int xn, int* ch, PNTTYPE cp1,
                   PNTTYPE cp2, PNTTYPE cp3, PNTTYPE* pa, PNTTYPE* pb,
                   PNTTYPE* pc, PNTTYPE* pd, PNTTYPE* pw, PNTTYPE* px,
                   int* ia, int* ib, int* ic, int* ix, int* ta,
                   double d1, double d2, double d3, int i1, int i2);
	void GetParam(double* d1, double* d2, double* d3, int* i1, int* i2);

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
	int     m_Kind, m_nID, m_Init, m_SPointS, m_SCenter, m_RCenter;
	int		m_Fg, m_Un, m_Vn, m_Ub, m_Vb, m_An, m_Xn, *m_Ch;
	int		*m_Ia, *m_Ib, *m_Ic, *m_Ix, *m_Ta;
	PNTTYPE m_Cp1, m_Cp2, m_Cp3;
	PNTTYPE *m_Pa, *m_Pb, *m_Pc, *m_Pd, *m_Pw, *m_Px;
	OBJTYPE *m_Op1, *m_Op2;
};

#endif ___BTNDLG7_H

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg8 ダイアログ

#ifndef ___BTNDLG8_H
#define ___BTNDLG8_H

class CInputBtnDlg8 : public CDialog
{
// コンストラクション
public:
	CInputBtnDlg8(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_INPUTBTN8 };
	double m_MoveX;
	double m_MoveY;
	double m_MoveZ;
	double m_AddMove;
	double m_RotateX;
	double m_RotateY;
	double m_RotateZ;
	double m_AddRotate;
	double m_MoveR;
	double m_AddMoveR;

protected:
	CBitmapButton Plus1;
	CBitmapButton Minus1;
	CBitmapButton Plus2;
	CBitmapButton Minus2;
	CBitmapButton Plus3;
	CBitmapButton Minus3;
	CBitmapButton Plus4;
	CBitmapButton Minus4;
	CBitmapButton Plus5;
	CBitmapButton Minus5;
	CBitmapButton Plus6;
	CBitmapButton Minus6;
	CBitmapButton Plus7;
	CBitmapButton Minus7;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	void SetParam(OBJTYPE* op1, OBJTYPE* op2, int fg, int un, int vn,
                  int ub, int vb, int an, int xn, int pl, int* ch,
                  PNTTYPE cp, PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc,
                  PNTTYPE* pd, PNTTYPE* pw, PNTTYPE* px, int* ia, int* ib,
                  int* ic, int* ix, int* ta, double d1, double d2);
	void GetParam(double* d1, double* d2);

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
	int     m_nID, m_Init;
	int		m_Fg, m_Un, m_Vn, m_Ub, m_Vb, m_An, m_Xn, m_Pl, *m_Ch;
	int		*m_Ia, *m_Ib, *m_Ic, *m_Ix, *m_Ta;
	PNTTYPE m_Cp;
	PNTTYPE *m_Pa, *m_Pb, *m_Pc, *m_Pd, *m_Pw, *m_Px;
	OBJTYPE *m_Op1, *m_Op2;
};

#endif ___BTNDLG8_H

//============================<EOF:InputBtnDlg2.h>===========================
