/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログクラス(CAboutDlg)宣言
// AboutDlg.h
//---------------------------------------------------------------------------
// LastEdit : 2010/03/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"													// ﾒｲﾝ ｼﾝﾎﾞﾙ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();														// ｸﾗｽの構築

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);					// DDX/DDVのｻﾎﾟｰﾄ

// 実装
protected:
	virtual BOOL OnInitDialog(void);
	DECLARE_MESSAGE_MAP()
};

//=============================<EOF:AboutDlg.h>==============================