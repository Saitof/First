/////////////////////////////////////////////////////////////////////////////
// プッシュボタンダイアログクラス定義
// PushDlg.cpp
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
#define   TIMERID    1

/////////////////////////////////////////////////////////////////////////////
// CScrollDlg ダイアログ

BEGIN_MESSAGE_MAP(CScrollDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/*************************************************************************/
CScrollDlg::CScrollDlg(CWnd* pParent) : CDialog(CScrollDlg::IDD, pParent)
/*************************************************************************/
{
	m_pParent = pParent;
	Axis      = 0;														// ﾒﾝﾊﾞ変数の初期化
	Dir       = 0;
	m_nID     = 0;
}

/***************************************************/
void CScrollDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
}

/***********************************/
BOOL CScrollDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	CDialog::OnInitDialog();
	VERIFY(LeftXY.AutoLoad(IDC_LEFTXY, this));							// 釦とﾋﾞｯﾄﾏｯﾌﾟを結び付ける
	VERIFY(RightXY.AutoLoad(IDC_RIGHTXY, this));
	VERIFY(UpXY.AutoLoad(IDC_UPXY, this));
	VERIFY(DownXY.AutoLoad(IDC_DOWNXY, this));
	VERIFY(LeftYZ.AutoLoad(IDC_LEFTYZ, this));
	VERIFY(RightYZ.AutoLoad(IDC_RIGHTYZ, this));
	VERIFY(UpYZ.AutoLoad(IDC_UPYZ, this));
	VERIFY(DownYZ.AutoLoad(IDC_DOWNYZ, this));
	VERIFY(LeftZX.AutoLoad(IDC_LEFTZX, this));
	VERIFY(RightZX.AutoLoad(IDC_RIGHTZX, this));
	VERIFY(UpZX.AutoLoad(IDC_UPZX, this));
	VERIFY(DownZX.AutoLoad(IDC_DOWNZX, this));
	CenterWindow(m_pParent);											// ﾀﾞｲｱﾛｸﾞを親ｳｨﾝﾄﾞｳの中心に表示
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*******************************/
void CScrollDlg::OnCancel(void)											//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	View->HideScrollTool();
	ShowWindow(SW_HIDE);
}

/***************************************/
LRESULT CScrollDlg::OnNcHitTest(CPoint)									//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/***************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/***************************************/
void CScrollDlg::OnTimer(UINT_PTR nIDTimer)									//<<< WM_TIMER ﾒｯｾｰｼﾞ応答処理
/***************************************/
{
	KillTimer(TIMERID);													// ﾀｲﾏｰ解除
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// 左ﾎﾞﾀﾝ押下中
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				View->Scroll(Axis, Dir);								// 画面ｽｸﾛｰﾙ
			}
			SetTimer(TIMERID, 20, NULL);								// ﾀｲﾏｰ設定
		}
	}
}

/*****************************************************************/
BOOL CScrollDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)		//<<< WM_SETCURSOR ﾒｯｾｰｼﾞ応答処理
/*****************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// ﾋﾞｯﾄﾏｯﾌﾟ釦上でﾏｳｽ左釦押下
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ﾏｳｽ位置を含む釦ID取得
			if(id>=IDC_UPXY&&id<=IDC_UPZX) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				m_nID = id;
				id -= IDC_UPXY;
				Dir = (id % 2) ? 1 : -1;								// 正負の方向を算出
				Axis = id / 4;											// X,Y,Z軸の内,何れかを算出
				View->Scroll(Axis, Dir);								// 画面ｽｸﾛｰﾙ
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/********************************/
void CScrollDlg::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "ScrLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "ScrTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*****************************/
BOOL CScrollDlg::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/*****************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "ScrLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "ScrTop", -1);
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

/////////////////////////////////////////////////////////////////////////////
// CEyeDlg ダイアログ

BEGIN_MESSAGE_MAP(CEyeDlg, CDialog)										// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/****************************************************************/
CEyeDlg::CEyeDlg(CWnd* pParent) : CDialog(CEyeDlg::IDD, pParent)
/****************************************************************/
{
	m_pParent = pParent;
	Theta     = 0.0;													// ﾒﾝﾊﾞ変数の初期化
	Dir       = 0;
	m_nID     = 0;
}

/************************************************/
void CEyeDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/************************************************/
{
	CDialog::DoDataExchange(pDX);
}

/********************************/
BOOL CEyeDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/********************************/
{
	CDialog::OnInitDialog();
	VERIFY(Left.AutoLoad(IDC_LEFT, this));								// 釦とﾋﾞｯﾄﾏｯﾌﾟを結び付ける
	VERIFY(Right.AutoLoad(IDC_RIGHT, this));
	VERIFY(Down.AutoLoad(IDC_DOWN, this));
	VERIFY(Up.AutoLoad(IDC_UP, this));
	CenterWindow(m_pParent);											// ﾀﾞｲｱﾛｸﾞを親ｳｨﾝﾄﾞｳの中心に表示
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/****************************/
void CEyeDlg::OnCancel(void)											//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/****************************/
{
	View->HideEyeChange();
	ShowWindow(SW_HIDE);
}

/************************************/
LRESULT CEyeDlg::OnNcHitTest(CPoint)									//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/************************************/
void CEyeDlg::OnTimer(UINT_PTR nIDTimer)									//<<< WM_TIMER ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	KillTimer(TIMERID);													// ﾀｲﾏｰ解除
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// 左ﾎﾞﾀﾝ押下中
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				View->EyeChange(Dir, Theta);							// 視点変更
			}
			SetTimer(TIMERID, 20, NULL);								// ﾀｲﾏｰ設定
		}
	}
}

/**************************************************************/
BOOL CEyeDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)			//<<< WM_SETCURSOR ﾒｯｾｰｼﾞ応答処理
/**************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// ﾋﾞｯﾄﾏｯﾌﾟ釦上でﾏｳｽ左釦押下
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ﾏｳｽ位置を含む釦ID取得
			if(id>=IDC_LEFT&&id<=IDC_UP) {								// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上
				m_nID = id;
				Dir = (id - IDC_LEFT) / 2;								// 方向を算出
				Theta = ((id-IDC_LEFT)%2?1:-1)*10.0*PI/180.0;			// 角度を算出
				View->EyeChange(Dir, Theta);							// 視点変更
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/*****************************/
void CEyeDlg::OnDestroy(void)											//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/*****************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "EyeLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "EyeTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**************************/
BOOL CEyeDlg::Create(void)												//<<< ﾀﾞｲｱﾛｸﾞ生成
/**************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "EyeLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "EyeTop", -1);
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

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg1 ダイアログ

BEGIN_MESSAGE_MAP(CSetBaseDlg1, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO9, OnBaseButton)
	ON_COMMAND_EX_RANGE(IDC_BUTTON1, IDC_BUTTON3, OnDirButton)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg1::CSetBaseDlg1(CWnd* pParent) : CDialog(CSetBaseDlg1::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint = 0;
	m_Direction = 0;
}

/*****************************************************/
void CSetBaseDlg1::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
	DDX_Control(pDX, IDC_RADIO4, m_Base4);
	DDX_Control(pDX, IDC_RADIO5, m_Base5);
	DDX_Control(pDX, IDC_RADIO6, m_Base6);
	DDX_Control(pDX, IDC_RADIO7, m_Base7);
	DDX_Control(pDX, IDC_RADIO8, m_Base8);
	DDX_Control(pDX, IDC_RADIO9, m_Base9);
	DDX_Control(pDX, IDC_BUTTON1, m_DirSN);
	DDX_Control(pDX, IDC_BUTTON2, m_DirEWSN);
	DDX_Control(pDX, IDC_BUTTON3, m_DirEW);
}

/*************************************/
BOOL CSetBaseDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/*************************************/
{
	CDialog::OnInitDialog();
	m_DirSN.LoadBitmap("SN");
	m_DirEWSN.LoadBitmap("EWSN");
	m_DirEW.LoadBitmap("EW");
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	m_Base4.LoadBitmap();
	m_Base5.LoadBitmap();
	m_Base6.LoadBitmap();
	m_Base7.LoadBitmap();
	m_Base8.LoadBitmap();
	m_Base9.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint))->SetCheckBtn(1);
	((CCheckButton*)GetDlgItem(IDC_BUTTON1+m_Direction))->SetCheckBtn(1);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*****************************************/
BOOL CSetBaseDlg1::OnBaseButton(UINT nID)								//<<< 基準点釦押下
/*****************************************/
{
	m_BasePoint = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO9; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/****************************************/
BOOL CSetBaseDlg1::OnDirButton(UINT nID)								//<<< 方向釦押下
/****************************************/
{
	m_Direction = nID-IDC_BUTTON1;
	for(UINT id=IDC_BUTTON1; id<=IDC_BUTTON3; id++) {
		((CCheckButton*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/*******************************************/
void CSetBaseDlg1::SetValue(int p1, int p2)								//<<< ﾊﾟﾗﾒｰﾀ設定
/*******************************************/
{
	m_BasePoint = p1;
	m_Direction = p2;
}

/*********************************************/
void CSetBaseDlg1::GetValue(int* p1, int* p2)							//<<< ﾊﾟﾗﾒｰﾀ取得
/*********************************************/
{
	*p1 = m_BasePoint;
	*p2 = m_Direction;
}

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg2 ダイアログ

BEGIN_MESSAGE_MAP(CSetBaseDlg2, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO9, OnBaseButton)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg2::CSetBaseDlg2(CWnd* pParent) : CDialog(CSetBaseDlg2::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint = 0;
}

/*****************************************************/
void CSetBaseDlg2::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
	DDX_Control(pDX, IDC_RADIO4, m_Base4);
	DDX_Control(pDX, IDC_RADIO5, m_Base5);
	DDX_Control(pDX, IDC_RADIO6, m_Base6);
	DDX_Control(pDX, IDC_RADIO7, m_Base7);
	DDX_Control(pDX, IDC_RADIO8, m_Base8);
	DDX_Control(pDX, IDC_RADIO9, m_Base9);
}

/*************************************/
BOOL CSetBaseDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/*************************************/
{
	CDialog::OnInitDialog();
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	m_Base4.LoadBitmap();
	m_Base5.LoadBitmap();
	m_Base6.LoadBitmap();
	m_Base7.LoadBitmap();
	m_Base8.LoadBitmap();
	m_Base9.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint))->SetCheckBtn(1);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*****************************************/
BOOL CSetBaseDlg2::OnBaseButton(UINT nID)								//<<< 基準点釦押下
/*****************************************/
{
	m_BasePoint = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO9; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/***********************************/
void CSetBaseDlg2::SetValue(int p1)										//<<< ﾊﾟﾗﾒｰﾀ設定
/***********************************/
{
	m_BasePoint = p1;
}

/************************************/
void CSetBaseDlg2::GetValue(int* p1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************/
{
	*p1 = m_BasePoint;
}

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg3 ダイアログ

BEGIN_MESSAGE_MAP(CSetBaseDlg3, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO4, OnBaseButton1)
	ON_COMMAND_EX_RANGE(IDC_RADIO5, IDC_RADIO7, OnBaseButton2)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg3::CSetBaseDlg3(CWnd* pParent) : CDialog(CSetBaseDlg3::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint1 = 0;
	m_BasePoint2 = 0;
	m_Size       = 0;
}

/*****************************************************/
void CSetBaseDlg3::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
	DDX_Control(pDX, IDC_RADIO4, m_Base4);
	DDX_Control(pDX, IDC_RADIO5, m_Base5);
	DDX_Control(pDX, IDC_RADIO6, m_Base6);
	DDX_Control(pDX, IDC_RADIO7, m_Base7);
	DDX_Radio(pDX, IDC_RADIO8, m_Size);
}

/*************************************/
BOOL CSetBaseDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/*************************************/
{
	CDialog::OnInitDialog();
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	m_Base4.LoadBitmap();
	m_Base5.LoadBitmap();
	m_Base6.LoadBitmap();
	m_Base7.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint1))->SetCheckBtn(1);
	((CCheckBox*)GetDlgItem(IDC_RADIO5+m_BasePoint2))->SetCheckBtn(1);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************************/
BOOL CSetBaseDlg3::OnBaseButton1(UINT nID)								//<<< 基準点釦1押下
/******************************************/
{
	m_BasePoint1 = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO4; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/******************************************/
BOOL CSetBaseDlg3::OnBaseButton2(UINT nID)								//<<< 基準点釦2押下
/******************************************/
{
	m_BasePoint2 = nID-IDC_RADIO5;
	for(UINT id=IDC_RADIO5; id<=IDC_RADIO7; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/***************************************************/
void CSetBaseDlg3::SetValue(int p1, int p2, int p3)						//<<< ﾊﾟﾗﾒｰﾀ設定
/***************************************************/
{
	m_BasePoint1 = p1;
	m_BasePoint2 = p2;
	m_Size       = p3;
}

/******************************************************/
void CSetBaseDlg3::GetValue(int* p1, int* p2, int* p3)					//<<< ﾊﾟﾗﾒｰﾀ取得
/******************************************************/
{
	*p1 = m_BasePoint1;
	*p2 = m_BasePoint2;
	*p3 = m_Size;
}

/////////////////////////////////////////////////////////////////////////////
// CSetBaseDlg4 ダイアログ

BEGIN_MESSAGE_MAP(CSetBaseDlg4, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO3, OnBaseButton)
END_MESSAGE_MAP()

/*******************************************************************************/
CSetBaseDlg4::CSetBaseDlg4(CWnd* pParent) : CDialog(CSetBaseDlg4::IDD, pParent)
/*******************************************************************************/
{
	m_BasePoint = 0;
}

/*****************************************************/
void CSetBaseDlg4::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_Base1);
	DDX_Control(pDX, IDC_RADIO2, m_Base2);
	DDX_Control(pDX, IDC_RADIO3, m_Base3);
}

/*************************************/
BOOL CSetBaseDlg4::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/*************************************/
{
	CDialog::OnInitDialog();
	m_Base1.LoadBitmap();
	m_Base2.LoadBitmap();
	m_Base3.LoadBitmap();
	((CCheckBox*)GetDlgItem(IDC_RADIO1+m_BasePoint))->SetCheckBtn(1);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*****************************************/
BOOL CSetBaseDlg4::OnBaseButton(UINT nID)								//<<< 基準点釦押下
/*****************************************/
{
	m_BasePoint = nID-IDC_RADIO1;
	for(UINT id=IDC_RADIO1; id<=IDC_RADIO3; id++) {
		((CCheckBox*)GetDlgItem(id))->SetCheckBtn(id==nID?1:0);
	}
	return TRUE;
}

/***********************************/
void CSetBaseDlg4::SetValue(int p1)										//<<< ﾊﾟﾗﾒｰﾀ設定
/***********************************/
{
	m_BasePoint = p1;
}

/************************************/
void CSetBaseDlg4::GetValue(int* p1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************/
{
	*p1 = m_BasePoint;
}
