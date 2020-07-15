/////////////////////////////////////////////////////////////////////////////
// レンダリング(RayTrace)ダイアログクラス宣言
// RendDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CRendDlg ダイアログ

#ifndef ___RENDDLG_H
#define ___RENDDLG_H

class CRendDlg : public CDialog
{
// コンストラクション
public:
	CRendDlg(CWnd* pParent=NULL);										// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_REND_DLG };
	CStatic m_Pic1;
	CButton m_Button1;
	CButton m_Button2;
	CButton m_Button3;
	CButton m_Button4;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void DDBtoDIB(CDC* pDC, int out, int check, int size);
	void OutputPrinter(CDC* pDC, int pos);
	void OutputFile(CString path);
	void FreeDIB(void);
	void CheckBtn(void);
	void GetRendSize(int* x, int* y);
	int  CheckStpSW(void);
	void SetRendColor(int x, int y, int r, int g, int b);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnPaint(void);
	virtual void OnCancel(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDestroy(void);
	afx_msg void OnRunTrace(void);
	afx_msg void OnStpTrace(void);
	afx_msg void OnPrtTrace(void);
	afx_msg void OnChangeSize(void);
	DECLARE_MESSAGE_MAP()

private:
	CClientDC* m_pDlgDC;
	CDC     m_RndDC;
	CBitmap m_RndBit;
	int     m_SRndDC, m_SRndBit;
	HANDLE  m_hDIB;
	DWORD   PalSize;
	CWinThread* m_pThread;
	CWnd*   m_pParent;
	CRect   m_Rt1, m_Rt2, m_Rt2S, m_RtD;
	int     XSize, YSize, XSizeS, YSizeS, XSizeP, YSizeP, Line;
	int     InitSW, RunSW, StpSW, PaintSW;
};

#endif ___RENDDLG_H

//==============================<EOF:RendDlg.h>==============================
