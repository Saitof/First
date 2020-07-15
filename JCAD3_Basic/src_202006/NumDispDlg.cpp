/////////////////////////////////////////////////////////////////////////////
// 数値表示ﾀﾞｲｱﾛｸﾞｸﾗｽ(CNumDispDlg)定義
// NumDispDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"

/////////////////////////////////////////////////////////////////////////////
// CNumDispDlg ダイアログ

BEGIN_MESSAGE_MAP(CNumDispDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/****************************************************************************/
CNumDispDlg::CNumDispDlg(CWnd* pParent) : CDialog(CNumDispDlg::IDD, pParent)
/****************************************************************************/
{
	m_pParent = pParent;												// 初期化
	Init();
}

/****************************************************/
void CNumDispDlg::DoDataExchange(CDataExchange* pDX)					// ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
}

/////////////////////////////////////////////////////////////////////////////
// CNumDispDlg メッセージ ハンドラ

/************************************/
BOOL CNumDispDlg::OnInitDialog(void)									// WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/									// ﾀﾞｲｱﾛｸﾞの初期化
{
	CRect r;

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);											// 親ｳｨﾝﾄﾞｳの中央に表示指定
	GetWindowRect(&r);													// ﾀﾞｲｱﾛｸﾞのｻｲｽﾞ取得
	Width = r.Width();													//  (幅)
	Height = r.Height();												//  (高さ)
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/********************************/
void CNumDispDlg::OnCancel(void)										// WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/********************************/
{
	View->HideNumDisp();
	ShowWindow(SW_HIDE);
}

/***********************************************/
inline LRESULT CNumDispDlg::OnNcHitTest(CPoint)							// WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/***********************************************/						// ﾏｳｽｶｰｿﾙの位置を取得
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/*********************************/
void CNumDispDlg::OnDestroy(void)										// WM_DESTROYﾒｯｾｰｼﾞ応答処理
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "NumLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "NumTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CNumDispDlg オペレーション

/******************************/
BOOL CNumDispDlg::Create(void)											// 生成
/******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "NumLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "NumTop", -1);
	if(left>0) {
		width = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		height = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		wp.showCmd = SW_HIDE;
		wp.rcNormalPosition.left = left;
		wp.rcNormalPosition.top = top;
		wp.rcNormalPosition.right = left + width;
		wp.rcNormalPosition.bottom = top + height;
		SetWindowPlacement(&wp);										// ｳｨﾝﾄﾞｳ表示状態設定
	}
	return ret;
}

/***********************************/
inline void CNumDispDlg::Init(void)										// 初期化
/***********************************/
{
	m_Param1 = _T("0.00");
	m_Param2 = _T("0.00");
	m_Param3 = _T("0.00");
	m_Param4 = _T("0.00");
}

/**************************************************/
void CNumDispDlg::SetParam(double pr1, double pr2,
						   double pr3, double pr4)						// ﾊﾟﾗﾒｰﾀ設定
/**************************************************/
{
	m_Param1.Format("%.2f", pr1);
	m_Param2.Format("%.2f", pr2);
	m_Param3.Format("%.2f", pr3);
	m_Param4.Format("%.2f", pr4);
	UpdateData(FALSE);
}

/***************************************/
void CNumDispDlg::SetNumDispWnd(int dp)
/***************************************/
{
	int mn, kd=0;

	mn = View->GetActMenu();											// 現在実行中のﾒﾆｭｰ取得
	if(mn==M_EDIT_ROTATE||mn==M_EDIT_BEND||
       mn==M_EDIT_TWIST||mn==M_MEASURE_ANGLE||
       mn==M_SURF_L_ROTATE||mn==M_SURF_TWIST) {							// <角度>
		kd = 2;
	} else if(mn==M_EDIT_SCALE||mn==M_JWL_GEM) {						// <ｻｲｽﾞ(3D)>
		kd = 3;
	} else if(mn==M_JWL_STONE) {										// <ｻｲｽﾞ(2D)>
		kd = 4;
	} else if(mn==M_SURF_SCALE) {										// <倍率>
		kd = 5;
	} else if(mn==M_JWL_ARRANGE) {										// <配置>
		kd = 6;
	} else if(mn==M_EDIT_SHEAR) {										// <せん断移動量>
		kd = 7;
	} else if(mn==M_MEASURE_CURVE) {									// <曲線長さ>
		kd = 8;
	} else {															// <距離>
		kd = 1;
	}
	Init();																// ﾊﾟﾗﾒｰﾀ初期化
	SetLayout(kd, dp);													// ﾚｲｱｳﾄ設定
}

/*******************************************/
void CNumDispDlg::SetLayout(int kd, int dp)								// ﾚｲｱｳﾄ設定
/*******************************************/
{
	CString title, titl1, titl2, titl3, unit1, unit2, unit3, show;
	BOOL    flg=TRUE;
	int     i;

	titl1 = _T(""), titl2 = _T(""), titl3 = _T("");
	unit1 = _T(""), unit2 = _T(""), unit3 = _T("");
	if(kd==1) {															// <距離(4)>
		show = "1111111111";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR1);										// ﾀｲﾄﾙ取得(距離)
		titl1.LoadString(IDS_STR62);									// 項目名1取得(X:)
		titl2.LoadString(IDS_STR63);									// 項目名2取得(Y:)
		titl3.LoadString(IDS_STR64);									// 項目名3取得(Z:)
		unit1.LoadString(IDS_STR5);										// 単位1取得(mm)
		unit2.LoadString(IDS_STR5);										// 単位2取得(mm)
		unit3.LoadString(IDS_STR5);										// 単位3取得(mm)
		SetWindowPos(NULL, 0, 0, Width, Height,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==2) {													// <角度(1)>
		show = "0001001000";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR2);										// ﾀｲﾄﾙ取得(角度)
		unit1.LoadString(IDS_STR6);										// 単位1取得(度)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==3) {													// <ｻｲｽﾞ(3)>
		show = "1111111110";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR3);										// ﾀｲﾄﾙ取得(ｻｲｽﾞ)
		titl1.LoadString(IDS_STR62);									// 項目名1取得(X:)
		titl2.LoadString(IDS_STR63);									// 項目名2取得(Y:)
		titl3.LoadString(IDS_STR64);									// 項目名3取得(Z:)
		unit1.LoadString(IDS_STR5);										// 単位1取得(mm)
		unit2.LoadString(IDS_STR5);										// 単位2取得(mm)
		unit3.LoadString(IDS_STR5);										// 単位3取得(mm)
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.8),
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==4) {													// <ｻｲｽﾞ(2)>
		show = "1101101100";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR3);										// ﾀｲﾄﾙ取得(ｻｲｽﾞ)
		titl1.LoadString(IDS_STR62);									// 項目名1取得(X:)
		titl2.LoadString(IDS_STR63);									// 項目名2取得(Y:)
		unit1.LoadString(IDS_STR5);										// 単位1取得(mm)
		unit2.LoadString(IDS_STR5);										// 単位2取得(mm)
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.65),
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==5) {													// <倍率(1)>
		show = "0001001000";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR4);										// ﾀｲﾄﾙ取得(倍率)
		unit1.LoadString(IDS_STR7);										// 単位1取得(倍)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==6) {													// <石枠配置(3)>
		show = "1111111110";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR58);									// ﾀｲﾄﾙ取得(石枠配置)
		titl1.LoadString(IDS_STR59);									// 項目名1取得(半径:)
		titl2.LoadString(IDS_STR60);									// 項目名2取得(XY:)
		titl3.LoadString(IDS_STR61);									// 項目名3取得(YZ:)
		unit1.LoadString(IDS_STR5);										// 単位1取得(mm)
		unit2.LoadString(IDS_STR6);										// 単位2取得(度)
		unit3.LoadString(IDS_STR6);										// 単位3取得(度)
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.8),
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==7) {													// <倍率(1)>
		show = "0001001000";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR79);									// ﾀｲﾄﾙ取得(せん断移動量)
		unit1.LoadString(IDS_STR5);										// 単位1取得(mm)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==8) {													// <曲線長さ(1)>
		show = "0001001000";											// 表示するｺﾝﾄﾛｰﾙの設定
		title.LoadString(IDS_STR31);									// ﾀｲﾄﾙ取得(長さ)
		unit1.LoadString(IDS_STR5);										// 単位1取得(mm)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else {															// <その他>
		flg = FALSE;
	}
	if(flg) {															// その他以外(有効)の場合
		GetDlgItem(IDC_TITLE1)->SetWindowText(title);					// ﾀｲﾄﾙ表示
		GetDlgItem(IDC_STATIC1)->SetWindowText(titl1);					// 項目名1表示
		GetDlgItem(IDC_STATIC2)->SetWindowText(titl2);					// 項目名2表示
		GetDlgItem(IDC_STATIC3)->SetWindowText(titl3);					// 項目名3表示
		GetDlgItem(IDC_STATIC4)->SetWindowText(unit1);					// 単位1表示
		GetDlgItem(IDC_STATIC5)->SetWindowText(unit2);					// 単位2表示
		GetDlgItem(IDC_STATIC6)->SetWindowText(unit3);					// 単位3表示
		for(i=0; i<6; i++) {											// ｺﾝﾄﾛｰﾙの表示
			GetDlgItem(IDC_STATIC1+i)->
                       ShowWindow(show[i]=='1' ? SW_SHOW : SW_HIDE);
		}
		for(i=0; i<4; i++) {											// ｺﾝﾄﾛｰﾙの表示
			GetDlgItem(IDC_EDIT1+i)->
                       ShowWindow(show[i+6]=='1' ? SW_SHOW : SW_HIDE);
		}
		UpdateData(FALSE);												// ﾃﾞｰﾀ更新
	}
	if(dp!=2) {
		ShowWindow(dp ? SW_SHOWNA : SW_HIDE);							// ｳｨﾝﾄﾞｳの表示/非表示
	}
}
