/////////////////////////////////////////////////////////////////////////////
// 数値表示ﾀﾞｲｱﾛｸﾞｸﾗｽ(CNumDispDlg)宣言
// NumDispDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/17 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

class CNumDispDlg : public CDialog
{
// コンストラクション
public:
	CNumDispDlg(CWnd* pParent=NULL);									// 標準のｺﾝｽﾄﾗｸﾀ

// ダイアログ データ
	enum { IDD = IDD_NUMDISP_DLG };
	CString	m_Param1;
	CString	m_Param2;
	CString	m_Param3;
	CString	m_Param4;

// オーバーライド
protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVｻﾎﾟｰﾄ

// オペレーション
public:
	BOOL Create(void);
	void Init(void);
	void SetParam(double pr1, double pr2=0.0, double pr3=0.0, double pr4=0.0);
	void SetNumDispWnd(int dp);
	void SetLayout(int kd, int dp);

// インプリメンテーション
protected:
	virtual BOOL OnInitDialog(void);
	virtual void OnCancel(void);
	afx_msg void OnDestroy(void);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	int   Width, Height;
};

//============================<EOF:NumDispDlg.h>=============================