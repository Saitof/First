/////////////////////////////////////////////////////////////////////////////
// 数値入力ダイアログクラス(釦対応)定義
// InputBtnDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "InputBtnDlg.h"
#include "CCommand.h"
#include "ECommand.h"
#include "ECmdUnit.h"
#include "EditCtrlPoint1.h"
#define   TIMERID  1

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg1 ダイアログ

BEGIN_MESSAGE_MAP(CInputBtnDlg1, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/********************************************************************************************/
CInputBtnDlg1::CInputBtnDlg1(int kind, CWnd* pParent) : CDialog(CInputBtnDlg1::IDD, pParent)
/********************************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_Kind   = kind;
	m_nID    = 0;
	m_Init   = 0;
}

/******************************************************/
void CInputBtnDlg1::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	if(m_Kind==1) {														// <移動>
		DDV_MinMaxDouble(pDX, m_Param1, -1000.0, 1000.0);
		DDV_MinMaxDouble(pDX, m_Param2, -1000.0, 1000.0);
		DDV_MinMaxDouble(pDX, m_Param3, -1000.0, 1000.0);
		DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
	} else if(m_Kind==2) {												// <ｻｲｽﾞ変更1>
		DDV_MinMaxDouble(pDX, m_Param1, 0.0, 1500.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.0, 1500.0);
		DDV_MinMaxDouble(pDX, m_Param3, 0.0, 1500.0);
		DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
	} else {															// <ｻｲｽﾞ変更2>
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param3, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param4, 0.001, 2.0);
	}
}

/**************************************/
BOOL CInputBtnDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "Btn1Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn1Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// 釦とﾋﾞｯﾄﾏｯﾌﾟを結び付ける
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	VERIFY(Plus2.AutoLoad(IDC_PLUS2, this));
	VERIFY(Minus2.AutoLoad(IDC_MINUS2, this));
	VERIFY(Plus3.AutoLoad(IDC_PLUS3, this));
	VERIFY(Minus3.AutoLoad(IDC_MINUS3, this));
	if(m_Kind==2) {														// <ｻｲｽﾞ変更1>
		str.LoadString(IDS_STR9);										// ﾀｲﾄﾙ取得
		SetWindowText(str);												// ﾀｲﾄﾙ表示
		str.LoadString(IDS_STR39);										// 項目名取得
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR40);
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);
	} else if(m_Kind==3) {												// <ｻｲｽﾞ変更2>
		str.LoadString(IDS_STR9);										// ﾀｲﾄﾙ取得
		SetWindowText(str);												// ﾀｲﾄﾙ表示
		str.LoadString(IDS_STR43);										// 項目名取得
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR40);
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);
		str.LoadString(IDS_STR7);										// 単位名取得
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 単位名表示
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
		GetDlgItem(IDC_STATIC6)->SetWindowText(str);
	}
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************/
void CInputBtnDlg1::OnOK(void)											//<<< OKﾎﾞﾀﾝ押下処理
/******************************/
{
	double dx, dy, dz;

	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		if(m_Kind==1) {													// <移動>
			if(fabs(m_Param1)>EPSILON||
               fabs(m_Param2)>EPSILON||
               fabs(m_Param3)>EPSILON||m_Init!=0) {
				ObjectMove(m_Init, m_Copy, m_Scale, m_Base, m_Param1,
                           m_Param2, m_Param3);							// 立体平行移動
				EndMove(m_Copy, m_Scale, m_SChk, m_Base, m_Param1,
                        m_Param2, m_Param3);							// 終了処理
			}
		} else if(m_Kind==2||m_Kind==3) {								// <ｻｲｽﾞ変更>
			if(m_Kind==2) {												// ｻｲｽﾞ(mm)指定の場合
				dx = (m_Xsize==0||m_Param1==0) ? 1.0 : m_Param1/m_Xsize;// X倍率
				dy = (m_Ysize==0||m_Param2==0) ? 1.0 : m_Param2/m_Ysize;// Y倍率
				dz = (m_Zsize==0||m_Param3==0) ? 1.0 : m_Param3/m_Zsize;// Z倍率
			} else {													// 倍率指定の場合
				dx = m_Param1;											// X倍率
				dy = m_Param2;											// Y倍率
				dz = m_Param3;											// Z倍率
			}
			if(fabs(dx-1.0)>EPSILON||
               fabs(dy-1.0)>EPSILON||
               fabs(dz-1.0)>EPSILON||m_Init!=0) {
				ObjectScale(m_Init, m_Cent, dx, dy, dz);				// 立体拡大/縮小
			}
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg1::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "Btn1Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn1Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg1::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/******************************************/
void CInputBtnDlg1::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	KillTimer(TIMERID);													// ﾀｲﾏｰ解除
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// 左ﾎﾞﾀﾝ押下中ならば
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上にある時
				ObjectEdit();											// 立体編集処理
			}
			SetTimer(TIMERID, 20, NULL);								// ﾀｲﾏｰ設定
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg1::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ﾒｯｾｰｼﾞ応答処理
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// ﾋﾞｯﾄﾏｯﾌﾟ釦上でﾏｳｽ左釦押下
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ﾏｳｽ位置を含む釦のID取得
			if(id>=IDC_PLUS1&&id<=IDC_MINUS3) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				m_nID = id;
				UpdateData(TRUE);										// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
				ObjectEdit();											// 立体編集処理
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg1::ObjectEdit(void)									//<<< 立体編集処理
/************************************/
{
	double sv, dx, dy, dz;

	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// 加算前保存
			sv = m_Param1;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			sv = m_Param2;
		} else {
			sv = m_Param3;
		}
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// 加算/減算
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param4;
			} else {
				m_Param1 -= m_Param4;
			}
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			if(m_nID==IDC_PLUS2) {
				m_Param2 += m_Param4;
			} else {
				m_Param2 -= m_Param4;
			}
		} else {
			if(m_nID==IDC_PLUS3) {
				m_Param3 += m_Param4;
			} else {
				m_Param3 -= m_Param4;
			}
		}
		UpdateData(FALSE);												// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
		if(!DataCheck()) {												// <<入力ﾃﾞｰﾀ確認OK>>
			if(m_Kind==1) {												// <移動>
				if(fabs(m_Param1)>EPSILON||
                   fabs(m_Param2)>EPSILON||
                   fabs(m_Param3)>EPSILON||m_Init!=0) {
					ObjectMove(m_Init, m_Copy, m_Scale, m_Base,
                               m_Param1, m_Param2, m_Param3);			// 立体平行移動
					m_Init = 1;											// 初期終了
				}
			} else if(m_Kind==2||m_Kind==3) {							// <ｻｲｽﾞ変更>
				if(m_Kind==2) {											// ｻｲｽﾞ(mm)指定の場合
					dx = (m_Xsize==0||m_Param1==0) ? 1.0 : m_Param1/m_Xsize;
					dy = (m_Ysize==0||m_Param2==0) ? 1.0 : m_Param2/m_Ysize;
					dz = (m_Zsize==0||m_Param3==0) ? 1.0 : m_Param3/m_Zsize;
				} else {												// 倍率指定の場合
					dx = m_Param1;										// X倍率
					dy = m_Param2;										// Y倍率
					dz = m_Param3;										// Z倍率
				}
				if(fabs(dx-1.0)>EPSILON||
                   fabs(dy-1.0)>EPSILON||
                   fabs(dz-1.0)>EPSILON||m_Init!=0) {
					ObjectScale(m_Init, m_Cent, dx, dy, dz);			// 立体拡大/縮小
					m_Init = 1;											// 初期終了
				}
			}
			View->Display(0);											// 再描画
		} else {														// <<入力ﾃﾞｰﾀ確認NG>>
			UpdateData(TRUE);											// ｴﾗｰﾒｯｾｰｼﾞ用
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1)	{
				m_Param1 = sv;
			} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
				m_Param2 = sv;
			} else {
				m_Param3 = sv;
			}
			UpdateData(FALSE);											// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ復元
		}
	}
}

/***********************************/
BOOL CInputBtnDlg1::DataCheck(void)										//<<< 入力ﾃﾞｰﾀ確認
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Kind==1) {
		if(m_Param1<-1000.0||m_Param1>1000.0) {
			ret = TRUE;
		}
		if(m_Param2<-1000.0||m_Param2>1000.0) {
			ret = TRUE;
		}
		if(m_Param3<-1000.0||m_Param3>1000.0) {
			ret = TRUE;
		}
		if(m_Param4<0.001||m_Param4>100.0) {
			ret = TRUE;
		}
	} else if(m_Kind==2) {
		if(m_Param1<0.0||m_Param1>1500.0) {
			ret = TRUE;
		}
		if(m_Param2<0.0||m_Param2>1500.0) {
			ret = TRUE;
		}
		if(m_Param3<0.0||m_Param3>1500.0) {
			ret = TRUE;
		}
		if(m_Param4<0.001||m_Param4>100.0) {
			ret = TRUE;
		}
	} else if(m_Kind==3) {
		if(m_Param1<0.001||m_Param1>10.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>10.0) {
			ret = TRUE;
		}
		if(m_Param3<0.001||m_Param3>10.0) {
			ret = TRUE;
		}
		if(m_Param4<0.001||m_Param4>2.0) {
			ret = TRUE;
		}
	}
	return ret;
}

/*********************************************************/
void CInputBtnDlg1::SetParam1(int cn, PNTTYPE sc, int ch,
                              SCRTYPE bp, double ad)                    //<<< ﾊﾟﾗﾒｰﾀ設定1
/*********************************************************/
{
	m_Copy = cn, m_Scale  = sc, m_SChk = ch;
	m_Base = bp, m_Param4 = ad;
}

/**************************************************************/
void CInputBtnDlg1::SetParam2(PNTTYPE cp, double dx,
							  double dy, double dz, double ad)			//<<< ﾊﾟﾗﾒｰﾀ設定2
/**************************************************************/
{
	m_Cent  = cp, m_Xsize  = dx, m_Ysize = dy;
	m_Zsize = dz, m_Param4 = ad;
	if(m_Kind==2) {
		m_Param1 = dx,  m_Param2 = dy,  m_Param3 = dz;
	} else {
		m_Param1 = 1.0, m_Param2 = 1.0, m_Param3 = 1.0;
	}
}

/****************************************/
void CInputBtnDlg1::GetParam(double* ad)                                //<<< ﾊﾟﾗﾒｰﾀ取得
/****************************************/
{
	*ad = m_Param4;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg2 ダイアログ

BEGIN_MESSAGE_MAP(CInputBtnDlg2, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************/
CInputBtnDlg2::CInputBtnDlg2(CWnd* pParent) : CDialog(CInputBtnDlg2::IDD, pParent)
/**********************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_nID    = 0;
	m_Init   = 0;
}

/******************************************************/
void CInputBtnDlg2::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDV_MinMaxDouble(pDX, m_Param1, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDV_MinMaxDouble(pDX, m_Param2, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDV_MinMaxDouble(pDX, m_Param3, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
}

/**************************************/
BOOL CInputBtnDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**************************************/
{
	int left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "Btn2Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn2Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// 釦とﾋﾞｯﾄﾏｯﾌﾟを結び付ける
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	VERIFY(Plus2.AutoLoad(IDC_PLUS2, this));
	VERIFY(Minus2.AutoLoad(IDC_MINUS2, this));
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************/
void CInputBtnDlg2::OnOK(void)											//<<< OKﾎﾞﾀﾝ押下処理
/******************************/
{
	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		if(fabs(m_Param1)>EPSILON||m_Init!=0) {
			ObjectRotate(m_Init, m_Copy, m_Scale, m_Base, m_Cent,
                         m_Plane, m_Param1, m_Param2);					// 立体回転移動
			EndRotate(m_Copy, m_Scale, m_SChk, m_Base, m_Cent,
                      m_Plane, m_Param1, m_Param2);						// 終了処理
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg2::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "Btn2Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn2Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg2::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/******************************************/
void CInputBtnDlg2::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	KillTimer(TIMERID);													// ﾀｲﾏｰ解除
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// 左ﾎﾞﾀﾝ押下中ならば
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上にある時
				ObjectEdit();											// 立体編集処理
			}
			SetTimer(TIMERID, 20, NULL);								// ﾀｲﾏｰ設定
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg2::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ﾒｯｾｰｼﾞ応答処理
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// ﾋﾞｯﾄﾏｯﾌﾟ釦上でﾏｳｽ左釦押下
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ﾏｳｽ位置を含む釦のID取得
			if(id==IDC_PLUS1||id==IDC_MINUS1||
               id==IDC_PLUS2||id==IDC_MINUS2) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				m_nID = id;
				UpdateData(TRUE);										// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
				ObjectEdit();											// 立体編集処理
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg2::ObjectEdit(void)									//<<< 立体編集処理
/************************************/
{
	double sv;

	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		sv = (m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) ? m_Param1:m_Param2;	// 加算前保存
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// 加算/減算
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param3;
			} else {
				m_Param1 -= m_Param3;
			}
		} else {
			if(m_nID==IDC_PLUS2) {
				m_Param2 += m_Param4;
			} else {
				m_Param2 -= m_Param4;
			}
		}
		UpdateData(FALSE);												// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
		if(!DataCheck()) {												// <<入力ﾃﾞｰﾀ確認OK>>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				ObjectRotate(m_Init, m_Copy, m_Scale, m_Base, m_Cent,
                             m_Plane, m_Param1, m_Param2);				// 立体回転移動
				m_Init = 1;												// 初期終了
			}
			View->Display(0);											// 再描画
		} else {														// <<入力ﾃﾞｰﾀ確認NG>>
			UpdateData(TRUE);											// ｴﾗｰﾒｯｾｰｼﾞ用
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {
				m_Param1 = sv;
			} else {
				m_Param2 = sv;
			}
			UpdateData(FALSE);											// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ復元
		}
	}
}

/***********************************/
BOOL CInputBtnDlg2::DataCheck(void)										//<<< 入力ﾃﾞｰﾀ確認
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Param1<-360.0||m_Param1>360.0)	{
		ret = TRUE;
	}
	if(m_Param2<-1000.0||m_Param2>1000.0) {
		ret = TRUE;
	}
	if(m_Param3<0.001||m_Param3>100.0) {
		ret = TRUE;
	}
	if(m_Param4<0.001||m_Param4>100.0) {
		ret = TRUE;
	}
	return ret;
}

/************************************************************/
void CInputBtnDlg2::SetParam(int cn, PNTTYPE sc, int ch,
                             SCRTYPE bp, PNTTYPE cp, int pl,
                             double ad1, double ad2)					//<<< ﾊﾟﾗﾒｰﾀ設定
/************************************************************/
{
	m_Copy = cn, m_Scale = sc, m_SChk   = ch,  m_Base   = bp;
	m_Cent = cp, m_Plane = pl, m_Param3 = ad1, m_Param4 = ad2;
}

/******************************************************/
void CInputBtnDlg2::GetParam(double* ad1, double* ad2)                  //<<< ﾊﾟﾗﾒｰﾀ取得
/******************************************************/
{
	*ad1 = m_Param3, *ad2 = m_Param4;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg3 ダイアログ

BEGIN_MESSAGE_MAP(CInputBtnDlg3, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/********************************************************************************************/
CInputBtnDlg3::CInputBtnDlg3(int kind, CWnd* pParent) : CDialog(CInputBtnDlg3::IDD, pParent)
/********************************************************************************************/
{
	if(kind==3) {
		m_Param1 = 1.0;
	} else if(kind==6) {
		m_Param1 = 0.0;
	} else {
		m_Param1 = 0.0;
	}
	m_Param2 = 0.0;
	m_Kind   = kind;
	m_nID    = 0;
	m_Init   = 0;
}

/******************************************************/
void CInputBtnDlg3::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	if(m_Kind==3) {														// <拡大/縮小>
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 2.0);
	} else if(m_Kind==6) {												// <せん断>
		DDV_MinMaxDouble(pDX, m_Param1, -100.0, 100.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 10.0);
	} else {															// <円筒曲げ･ひねり･回転>
		DDV_MinMaxDouble(pDX, m_Param1, -1080.0, 1080.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 100.0);
	}
}

/**************************************/
BOOL CInputBtnDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "Btn3Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn3Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// 釦とﾋﾞｯﾄﾏｯﾌﾟを結び付ける
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	if(m_Kind==2||m_Kind==4) {											// <ひねり(制御点列/立体)>
		str.LoadString(IDS_STR10);										// ﾀｲﾄﾙ取得
		SetWindowText(str);												// ﾀｲﾄﾙ表示
		str.LoadString(IDS_STR41);										// 項目名取得
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR42);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
	} else if(m_Kind==3) {												// <拡大/縮小(制御点列)>
		str.LoadString(IDS_STR11);										// ﾀｲﾄﾙ取得
		SetWindowText(str);												// ﾀｲﾄﾙ表示
		str.LoadString(IDS_STR43);										// 項目名取得
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR44);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.LoadString(IDS_STR7);										// 単位名取得(倍)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 単位名表示
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	} else if(m_Kind==5) {												// <回転(制御点列)>
		str.LoadString(IDS_STR80);										// ﾀｲﾄﾙ取得
		SetWindowText(str);												// ﾀｲﾄﾙ表示
		str.LoadString(IDS_STR81);										// 項目名取得
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR82);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
	} else if(m_Kind==6) {												// <せん断(立体)>
		str.LoadString(IDS_STR92);										// ﾀｲﾄﾙ取得
		SetWindowText(str);												// ﾀｲﾄﾙ表示
		str.LoadString(IDS_STR93);										// 項目名取得
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR75);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.LoadString(IDS_STR5);										// 単位名取得(mm)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 単位名表示
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	}
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************/
void CInputBtnDlg3::OnOK(void)											//<<< OKﾎﾞﾀﾝ押下処理
/******************************/
{
	double dd;

	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		if(m_Kind==1) {													// <円筒曲げ(立体)>
			dd = m_Param1;												// 曲げ角度
			if(fabs(dd)>EPSILON||m_Init!=0) {
				if(!m_Fg) {												// 数値変換
					dd = -dd;
				}
				ObjectBend(m_Init, m_Fc, m_Vc, dd, m_Spnt, m_Epnt);		// 立体円筒曲げ
			}
		} else if(m_Kind==2||m_Kind==5) {								// <ひねり/回転(制御点列)>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				TwistCLine(m_Cp, m_Vp, m_Param1, m_Bs, m_Sa, m_Sn,
                           m_Op, m_Un, m_Vn, m_Uf, m_Sf, m_Pa,
                           m_Pb, m_Pc, m_Ta);							// 制御点ひねり
				RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc, m_Ia);		// 曲面生成
				RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);			// 曲面制御点生成
				View->ObjectShading(m_Op);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			}
		} else if(m_Kind==3) {											// <拡大/縮小(制御点列)>
			if(fabs(m_Param1-1.0)>EPSILON||m_Init!=0) {
				ScaleCLine(m_Cp, m_Vp, m_Bn, m_Dn, m_Param1, m_Bs,
                           m_Sa, m_Sn, m_Op, m_Un, m_Vn,
                           m_Uf, m_Sf, m_Pa, m_Pb, m_Pc, m_Ta);			// 制御点拡大/縮小
				RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc, m_Ia);		// 曲面生成
				RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);			// 曲面制御点生成
				View->ObjectShading(m_Op);								// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
			}
		} else if(m_Kind==4) {											// <ひねり(立体)>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				ObjectTwist(m_Init, m_Dr, m_Param1, m_Ln, m_Cpnt);		// 立体ひねり
			}
		} else if(m_Kind==6) {											// <せん断(立体)>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				dd = m_Param1/m_Len;									// せん断傾き
				ObjectShear(m_Init, m_Dr1, m_Dr2, dd, m_Spnt);			// せん断
			}
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg3::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "Btn3Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn3Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg3::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/******************************************/
void CInputBtnDlg3::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	KillTimer(TIMERID);													// ﾀｲﾏｰ解除
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// 左ﾎﾞﾀﾝ押下中ならば
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上にある時
				ObjectEdit();											// 立体編集処理
			}
			SetTimer(TIMERID, 20, NULL);								// ﾀｲﾏｰ設定
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg3::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ﾒｯｾｰｼﾞ応答処理
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// ﾋﾞｯﾄﾏｯﾌﾟ釦上でﾏｳｽ左釦押下
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ﾏｳｽ位置を含む釦のID取得
			if(id==IDC_PLUS1||id==IDC_MINUS1) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				m_nID = id;
				UpdateData(TRUE);										// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
				ObjectEdit();											// 立体編集処理
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg3::ObjectEdit(void)									//<<< 立体編集処理
/************************************/
{
	PNTTYPE p1[2], p2[2];
	int     i1[2];
	double  sv, dd;

	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		sv = m_Param1;													// 加算前保存
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// 加算/減算
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param2;
			} else {
				m_Param1 -= m_Param2;
			}
		}
		UpdateData(FALSE);												// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
		if(!DataCheck()) {												// <<入力ﾃﾞｰﾀ確認OK>>
			if(m_Kind==1) {												// <円筒曲げ(立体)>
				dd = m_Param1;											// 曲げ角度
				if(fabs(dd)>EPSILON||m_Init!=0) {
					if(!m_Fg) {											// 数値変換
						dd = -dd;
					}
					ObjectBend(m_Init, m_Fc, m_Vc, dd, m_Spnt, m_Epnt);	// 立体円筒曲げ
					m_Init = 1;											// 初期終了
				}
			} else if(m_Kind==2||m_Kind==5) {							// <ひねり/回転(制御点列)>
				if(fabs(m_Param1)>EPSILON||m_Init!=0) {
					TwistCLine(m_Cp, m_Vp, m_Param1, m_Bs, m_Sa,
                               m_Sn, m_Op, m_Un, m_Vn, m_Uf,
                               m_Sf, m_Pa, m_Pb, m_Pc, m_Ta);			// 制御点ひねり
					RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf,m_Pc,m_Ia);	// 曲面生成
					RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);		// 曲面制御点生成
					View->ObjectShading(m_Op);							// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
					m_Init = 1;											// 初期終了
				}
			} else if(m_Kind==3) {										// <拡大/縮小(制御点列)>
				if(fabs(m_Param1-1.0)>EPSILON||m_Init!=0) {
					ScaleCLine(m_Cp, m_Vp, m_Bn, m_Dn, m_Param1,
                               m_Bs, m_Sa, m_Sn, m_Op, m_Un, m_Vn,
                               m_Uf, m_Sf, m_Pa, m_Pb, m_Pc, m_Ta);		// 制御点拡大/縮小
					RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf,m_Pc,m_Ia);	// 曲面生成
					RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);		// 曲面制御点生成
					View->ObjectShading(m_Op);							// ｼｪｰﾃﾞｨﾝｸﾞﾘｽﾄ登録
					m_Init = 1;											// 初期終了
				}
			} else if(m_Kind==4) {										// <ひねり(立体)>
				if(fabs(m_Param1)>EPSILON||m_Init!=0) {
					ObjectTwist(m_Init, m_Dr, m_Param1, m_Ln, m_Cpnt);	// 立体ひねり
					m_Init = 1;											// 初期終了
				}
			} else if(m_Kind==6) {										// <せん断(立体)>
				if(fabs(m_Param1)>EPSILON||m_Init!=0) {
					dd = m_Param1/m_Len;								// せん断傾き
					ObjectShear(m_Init, m_Dr1, m_Dr2, dd, m_Spnt);		// せん断
					m_Init = 1;											// 初期終了
					p1[0] = m_Spnt, p2[0] = m_Epnt;						// せん断線(ﾗｲﾝ)初期設定
					i1[0] = 0;
					p2[0].p[m_Dr2] += m_Param1;							// せん断線水平位置変更
					View->Line.Setup(p1, p2, i1, 1, m_Pl);				// せん断線(ﾗｲﾝ)設定
				}
			}
			View->Display(0);											// 再描画
		} else {														// <<入力ﾃﾞｰﾀ確認NG>>
			UpdateData(TRUE);											// ｴﾗｰﾒｯｾｰｼﾞ用
			m_Param1 = sv;
			UpdateData(FALSE);											// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ復元
		}
	}
}

/***********************************/
BOOL CInputBtnDlg3::DataCheck(void)										//<<< 入力ﾃﾞｰﾀ確認
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Kind==3) {														// <拡大/縮小>
		if(m_Param1<0.001||m_Param1>10.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>2.0) {
			ret = TRUE;
		}
	} else if(m_Kind==6) {												// <せん断>
		if(m_Param1<-100.0||m_Param1>100.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>10.0) {
			ret = TRUE;
		}
	} else {															// <円筒曲げ･ひねり･回転>
		if(m_Param1<-1080.0||m_Param1>1080.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>100.0) {
			ret = TRUE;
		}
	}
	return ret;
}

/****************************************************************/
void CInputBtnDlg3::SetParam1(int fg, int fc, int vc,
                              PNTTYPE sp, PNTTYPE ep, double ad)		//<<< ﾊﾟﾗﾒｰﾀ設定1
/****************************************************************/
{
	m_Fg = fg, m_Fc = fc, m_Vc = vc;
	m_Spnt = sp, m_Epnt = ep, m_Param2 = ad;
}

/********************************************************************/
void CInputBtnDlg3::SetParam2(int un, int vn, int uf, int vf,
                              int sf, int bs, int* sa, int sn,
                              OBJTYPE* op, PNTTYPE* cp, VECTYPE* vp,
                              PNTTYPE* pa, int* ia, PNTTYPE* pb,
                              PNTTYPE* pc, int* ta, double ad)			//<<< ﾊﾟﾗﾒｰﾀ設定2
/********************************************************************/
{
	m_Un = un, m_Vn = vn, m_Uf = uf, m_Vf = vf, m_Sf = sf, m_Bs = bs;
	m_Sa = sa, m_Sn = sn, m_Op = op, m_Cp = cp, m_Vp = vp, m_Pa = pa;
	m_Ia = ia, m_Pb = pb, m_Pc = pc, m_Ta = ta, m_Param2 = ad;
}

/**********************************************************************/
void CInputBtnDlg3::SetParam3(int un, int vn, int uf, int vf, int sf,
                              int bn, int dn, int bs, int* sa, int sn,
                              OBJTYPE* op, PNTTYPE* cp, VECTYPE* vp,
                              PNTTYPE* pa, int* ia, PNTTYPE* pb,
                              PNTTYPE* pc, int* ta, double ad)			//<<< ﾊﾟﾗﾒｰﾀ設定3
/**********************************************************************/
{
	m_Un = un, m_Vn = vn, m_Uf = uf, m_Vf = vf, m_Sf = sf, m_Bn = bn;
	m_Dn = dn, m_Bs = bs, m_Sa = sa, m_Sn = sn, m_Op = op, m_Cp = cp;
	m_Vp = vp, m_Pa = pa, m_Ia = ia, m_Pb = pb, m_Pc = pc, m_Ta = ta;
	m_Param2 = ad;
}

/***********************************************************************/
void CInputBtnDlg3::SetParam4(int dr, double ln, PNTTYPE cp, double ad)	//<<< ﾊﾟﾗﾒｰﾀ設定4
/***********************************************************************/
{
	m_Dr = dr, m_Ln = ln, m_Cpnt = cp, m_Param2 = ad;
}

/****************************************************************/
void CInputBtnDlg3::SetParam6(int d1, int d2, int pl,
                              PNTTYPE sp, PNTTYPE ep, double ad)		//<<< ﾊﾟﾗﾒｰﾀ設定6
/****************************************************************/
{
	m_Dr1 = d1, m_Dr2 = d2, m_Pl = pl, m_Spnt = sp, m_Epnt = ep;
	m_Len = ep.p[d1]-sp.p[d1], m_Param2 = ad;
}

/****************************************/
void CInputBtnDlg3::GetParam(double* ad)                                //<<< ﾊﾟﾗﾒｰﾀ取得
/****************************************/
{
	*ad = m_Param2;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg4 ダイアログ

BEGIN_MESSAGE_MAP(CInputBtnDlg4, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************/
CInputBtnDlg4::CInputBtnDlg4(CWnd* pParent) : CDialog(CInputBtnDlg4::IDD, pParent)
/**********************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_Param5 = 0.0;
	m_Param6 = 0.0;
	m_nID    = 0;
}

/******************************************************/
void CInputBtnDlg4::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDV_MinMaxDouble(pDX, m_Param2, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDV_MinMaxDouble(pDX, m_Param3, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT5, m_Param5);
	DDV_MinMaxDouble(pDX, m_Param5, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT6, m_Param6);
	DDV_MinMaxDouble(pDX, m_Param6, 0.001, 100.0);
}

/**************************************/
BOOL CInputBtnDlg4::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "Btn4Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn4Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// 釦とﾋﾞｯﾄﾏｯﾌﾟを結び付ける
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	VERIFY(Plus2.AutoLoad(IDC_PLUS2, this));
	VERIFY(Minus2.AutoLoad(IDC_MINUS2, this));
	VERIFY(Plus3.AutoLoad(IDC_PLUS3, this));
	VERIFY(Minus3.AutoLoad(IDC_MINUS3, this));
	if(m_Plane==0) {
		str.Format(IDS_STR65, "XY");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
		str.Format(IDS_STR65, "YZ");
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);
		str.Format(IDS_STR66, "XY");
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.Format(IDS_STR66, "YZ");
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	} else if(m_Plane==1) {
		str.Format(IDS_STR65, "YZ");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
		str.Format(IDS_STR65, "ZX");
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);
		str.Format(IDS_STR66, "YZ");
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.Format(IDS_STR66, "ZX");
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	} else {
		str.Format(IDS_STR65, "ZX");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
		str.Format(IDS_STR65, "XY");
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);
		str.Format(IDS_STR66, "ZX");
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.Format(IDS_STR66, "XY");
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	}
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************/
void CInputBtnDlg4::OnOK(void)											//<<< OKﾎﾞﾀﾝ押下処理
/******************************/
{
	double mv;

	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		if(fabs(m_Param1-m_Param1Sv)>EPSILON) {							// <半径調整有>
			mv = m_Param1-m_Len;										// 移動量
			ArrangeMove(mv, m_Lno, m_Cno, m_Min, m_Max, m_Ocp,
                        m_Mcp, m_Cvc);									// 移動処理
		}
		if(fabs(m_Param3-m_Param3Sv)>EPSILON) {							// <垂直回転有>
			ArrangeRotate2(m_Param3, m_Lno, m_Cno, m_Min, m_Max,
                           m_Mcp, m_Rvc);								// 垂直回転処理
		}
		if(fabs(m_Param2-m_Param2Sv)>EPSILON) {							// <水平回転有>
			ArrangeRotate1(m_Param2, m_Lno, m_Cno, m_Min, m_Max, m_Mcp);// 水平回転処理
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg4::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "Btn4Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn4Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg4::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/******************************************/
void CInputBtnDlg4::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ﾒｯｾｰｼﾞ応答処理
/******************************************/
{
	KillTimer(TIMERID);													// ﾀｲﾏｰ解除
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// 左ﾎﾞﾀﾝ押下中ならば
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上にある時
				ObjectEdit();											// 立体編集処理
			}
			SetTimer(TIMERID, 20, NULL);								// ﾀｲﾏｰ設定
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg4::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ﾒｯｾｰｼﾞ応答処理
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// ﾋﾞｯﾄﾏｯﾌﾟ釦上でﾏｳｽ左釦押下
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ﾏｳｽ位置を含む釦のID取得
			if(id>=IDC_PLUS1&&id<=IDC_MINUS3) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				m_nID = id;
				UpdateData(TRUE);										// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
				ObjectEdit();											// 立体編集処理
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg4::ObjectEdit(void)									//<<< 立体編集処理
/************************************/
{
	double sv, mv;

	if(!DataCheck()) {													// <<入力ﾃﾞｰﾀ確認OK>>
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// 加算前保存
			sv = m_Param1;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			sv = m_Param2;
		} else {
			sv = m_Param3;
		}
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// 加算/減算
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param4;
			} else {
				m_Param1 -= m_Param4;
			}
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			if(m_nID==IDC_PLUS2) {
				m_Param2 += m_Param5;
			} else {
				m_Param2 -= m_Param5;
			}
		} else {
			if(m_nID==IDC_PLUS3) {
				m_Param3 += m_Param6;
			} else {
				m_Param3 -= m_Param6;
			}
		}
		UpdateData(FALSE);												// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
		if(!DataCheck()) {												// <<入力ﾃﾞｰﾀ確認OK>>
			if(fabs(m_Param1-m_Param1Sv)>EPSILON) {						// <半径調整有>
				mv = m_Param1-m_Len;									// 移動量
				ArrangeMove(mv, m_Lno, m_Cno, m_Min, m_Max,
                            m_Ocp, m_Mcp, m_Cvc);						// XY移動処理
				m_Param1Sv = m_Param1;
			}
			if(fabs(m_Param3-m_Param3Sv)>EPSILON) {						// <垂直回転有>
				ArrangeRotate2(m_Param3, m_Lno, m_Cno, m_Min,
                               m_Max, m_Mcp, m_Rvc);					// 垂直回転処理
				m_Param3Sv = m_Param3;
			}
			if(fabs(m_Param2-m_Param2Sv)>EPSILON) {						// <水平回転有>
				ArrangeRotate1(m_Param2, m_Lno, m_Cno, m_Min,
                               m_Max, m_Mcp);							// 水平回転処理
				m_Param2Sv = m_Param2;
			}
			View->Display(0);											// 再描画
		} else {														// <<入力ﾃﾞｰﾀ確認NG>>
			UpdateData(TRUE);											// ｴﾗｰﾒｯｾｰｼﾞ用
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {
				m_Param1 = sv;
			} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
				m_Param2 = sv;
			} else {
				m_Param3 = sv;
			}
			UpdateData(FALSE);											// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ復元
		}
	}
}

/***********************************/
BOOL CInputBtnDlg4::DataCheck(void)										//<<< 入力ﾃﾞｰﾀ確認
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Param1<0.001||m_Param1>1000.0) {
		ret = TRUE;
	}
	if(m_Param2<-360.0||m_Param2>360.0) {
		ret = TRUE;
	}
	if(m_Param3<-360.0||m_Param3>360.0) {
		ret = TRUE;
	}
	if(m_Param4<0.001||m_Param4>100.0) {
		ret = TRUE;
	}
	if(m_Param5<0.001||m_Param5>100.0) {
		ret = TRUE;
	}
	if(m_Param6<0.001||m_Param6>100.0) {
		ret = TRUE;
	}
	return ret;
}

/**********************************************************************/
void CInputBtnDlg4::SetParam(int pl, int cn, int lno, double len,
                             int* mna, int* mxa, PNTTYPE* ocp,
                             PNTTYPE* mcp, VECTYPE* cvc, VECTYPE* rvc,
                             double ad1, double ad2, double ad3)		//<<< ﾊﾟﾗﾒｰﾀ設定
/**********************************************************************/
{
	m_Plane = pl, m_Cno = cn,  m_Lno = lno, m_Len = len, m_Min = mna;
	m_Max = mxa,  m_Ocp = ocp, m_Mcp = mcp, m_Cvc = cvc, m_Rvc = rvc;
	m_Param4 = ad1, m_Param5 = ad2, m_Param6 = ad3, m_Param1 = m_Len;
	m_Param1Sv = m_Param1, m_Param2Sv = m_Param2, m_Param3Sv = m_Param3;
}

/*******************************************************************/
void CInputBtnDlg4::GetParam(double* ad1, double* ad2, double* ad3)     //<<< ﾊﾟﾗﾒｰﾀ取得
/*******************************************************************/
{
	*ad1 = m_Param4, *ad2 = m_Param5, *ad3 = m_Param6;
}
