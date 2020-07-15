/////////////////////////////////////////////////////////////////////////////
// 情報表示ダイアログクラス定義
// InfoDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "MainFrm.h"
#include "DataAccess1.h"

///////////////////////////////////////////////////////////////////////////////
// CStateDlg ダイアログ

BEGIN_MESSAGE_MAP(CStateDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************/
CStateDlg::CStateDlg(CWnd* pParent) : CDialog(CStateDlg::IDD, pParent)
/**********************************************************************/
{
	m_pParent  = pParent;
	m_Cent3DX  = 0.0;
	m_Cent3DY  = 0.0;
	m_Cent3DZ  = 0.0;
	m_EyeX     = 0.0;
	m_EyeY     = 0.0;
	m_EyeZ     = 0.0;
	m_CentPRSX = 0.0;
	m_CentPRSY = 0.0;
	m_CentPRSZ = 0.0;
}

/**************************************************/
void CStateDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Cent3DX);
	DDX_Text(pDX, IDC_EDIT2, m_Cent3DY);
	DDX_Text(pDX, IDC_EDIT3, m_Cent3DZ);
	DDX_Text(pDX, IDC_EDIT4, m_EyeX);
	DDX_Text(pDX, IDC_EDIT5, m_EyeY);
	DDX_Text(pDX, IDC_EDIT6, m_EyeZ);
	DDX_Text(pDX, IDC_EDIT7, m_CentPRSX);
	DDX_Text(pDX, IDC_EDIT8, m_CentPRSY);
	DDX_Text(pDX, IDC_EDIT9, m_CentPRSZ);
}

/***************************************************/
inline LRESULT CStateDlg::OnNcHitTest(CPoint point)						//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/***************************************************/
{
	LRESULT hit;

	if((hit=CDialog::OnNcHitTest(point))!=HTCLIENT) {
		return hit;
	} else {
		return HTCAPTION;												// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
	}																	// 移動するため
}

/**********************************/
BOOL CStateDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************/
void CStateDlg::OnCancel(void)											//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/******************************/
{
	View->HideStateDlg();
	ShowWindow(SW_HIDE);
}

/*******************************/
void CStateDlg::OnDestroy(void)											//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "StaLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "StaTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/****************************/
BOOL CStateDlg::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/****************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "StaLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "StaTop", -1);
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

/**********************************************/
void CStateDlg::SetCent3D(const PNTTYPE& cent)							//<<< 3面図中心点設定
/**********************************************/
{
	m_Cent3DX = cent.p[0];
	m_Cent3DY = cent.p[1];
	m_Cent3DZ = cent.p[2];
	UpdateData(FALSE);
}

/******************************************/
void CStateDlg::SetEye(const PNTTYPE& eye)								//<<< 透視図視点設定
/******************************************/
{
	m_EyeX = eye.p[0];
	m_EyeY = eye.p[1];
	m_EyeZ = eye.p[2];
	UpdateData(FALSE);
}

/***********************************************/
void CStateDlg::SetCentPRS(const PNTTYPE& cent)							//<<< 透視図参照点設定
/***********************************************/
{
	m_CentPRSX = cent.p[0];
	m_CentPRSY = cent.p[1];
	m_CentPRSZ = cent.p[2];
	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// CDataUseDlg ダイアログ

BEGIN_MESSAGE_MAP(CDataUseDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/****************************************************************************/
CDataUseDlg::CDataUseDlg(CWnd* pParent) : CDialog(CDataUseDlg::IDD, pParent)
/****************************************************************************/
{
	m_pParent = pParent;
	m_IntMax  = _T("");
	m_PntMax  = _T("");
	m_UndoMax = _T("");
	m_IntNum  = _T("");
	m_PntNum  = _T("");
	m_IntUse  = _T("");
	m_PntUse  = _T("");
}

/****************************************************/
void CDataUseDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_IntMax);
	DDX_Text(pDX, IDC_EDIT2, m_PntMax);
	DDX_Text(pDX, IDC_EDIT3, m_UndoMax);
	DDX_Text(pDX, IDC_EDIT4, m_IntNum);
	DDX_Text(pDX, IDC_EDIT5, m_PntNum);
	DDX_Text(pDX, IDC_EDIT6, m_IntUse);
	DDX_Text(pDX, IDC_EDIT7, m_PntUse);
}

/***********************************************/
inline LRESULT CDataUseDlg::OnNcHitTest(CPoint)							//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/***********************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/************************************/
BOOL CDataUseDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	SetData();
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/********************************/
void CDataUseDlg::OnCancel(void)										//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/********************************/
{
	View->HideDataUse();
	ShowWindow(SW_HIDE);
}

/*********************************/
void CDataUseDlg::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "DatLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "DatTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************/
BOOL CDataUseDlg::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "DatLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "DatTop", -1);
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

/*******************************/
void CDataUseDlg::SetData(void)											//<<< ﾃﾞｰﾀ使用率設定
/*******************************/
{
	m_IntMax.Format("%d", GetIntMax());
	m_PntMax.Format("%d", GetPntMax());
	m_UndoMax.Format("%d", GetUndoMax());
	m_IntNum.Format("%d", GetIntUseNum());
	m_PntNum.Format("%d", GetPntUseNum());
	m_IntUse.Format("%.2f %%", GetIntUse());
	m_PntUse.Format("%.2f %%", GetPntUse());
	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// CFunctionDlg ダイアログ

BEGIN_MESSAGE_MAP(CFunctionDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonClick)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
END_MESSAGE_MAP()

/*******************************************************************************/
CFunctionDlg::CFunctionDlg(CWnd* pParent) : CDialog(CFunctionDlg::IDD, pParent)
/*******************************************************************************/
{
	m_pParent = pParent;
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = 0;
	m_Check10 = 0;
	m_Check11 = 0;
	m_Check12 = 0;
	m_Lock    = 0;
}

/*****************************************************/
void CFunctionDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_EDIT5, m_Edit5);
	DDX_Control(pDX, IDC_EDIT6, m_Edit6);
	DDX_Control(pDX, IDC_EDIT7, m_Edit7);
	DDX_Control(pDX, IDC_EDIT8, m_Edit8);
	DDX_Control(pDX, IDC_EDIT9, m_Edit9);
	DDX_Control(pDX, IDC_EDIT10, m_Edit10);
	DDX_Control(pDX, IDC_EDIT11, m_Edit11);
	DDX_Control(pDX, IDC_EDIT12, m_Edit12);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Check(pDX, IDC_CHECK5, m_Check5);
	DDX_Check(pDX, IDC_CHECK6, m_Check6);
	DDX_Check(pDX, IDC_CHECK7, m_Check7);
	DDX_Check(pDX, IDC_CHECK8, m_Check8);
	DDX_Check(pDX, IDC_CHECK9, m_Check9);
	DDX_Check(pDX, IDC_CHECK10, m_Check10);
	DDX_Check(pDX, IDC_CHECK11, m_Check11);
	DDX_Check(pDX, IDC_CHECK12, m_Check12);
}

/************************************************/
inline LRESULT CFunctionDlg::OnNcHitTest(CPoint)						//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/************************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/*************************************/
BOOL CFunctionDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/*************************************/
{
	int func[8];

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	View->GetFChar(func);												// ﾌｧﾝｸｼｮﾝｷｰ取得
	m_Check1  = (func[0]) ? 1 : 0;										// 釦のON/OFFを設定
	m_Check2  = (func[1]) ? 1 : 0;
	m_Check3  = (func[2]) ? 1 : 0;
	m_Check4  = (func[3]) ? 1 : 0;
	m_Check5  = (func[4]) ? 1 : 0;
	m_Check6  = (func[5]) ? 1 : 0;
	m_Check7  = (func[6]) ? 1 : 0;
	m_Check8  = (func[7]) ? 1 : 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*********************************/
void CFunctionDlg::OnCancel(void)										//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/*********************************/
{
	MessageOut(-1, 0);													// ﾌｧﾝｸｼｮﾝﾒｯｾｰｼﾞ初期化
	View->HideFunction();												// ﾀﾞｲｱﾛｸﾞ消去(非表示)
}

/**********************************/
void CFunctionDlg::OnDestroy(void)										//<<< WM_DESTROYﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "FncLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "FncTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**************************************/
void CFunctionDlg::OnButtonClick(void)									//<<< ｢ﾒﾆｭｰ起動]｣ﾎﾞﾀﾝ処理
/**************************************/
{
	if(m_nID>0) {
		MainWnd->ExecuteMenu((UINT)m_nID);								// ﾌｧﾝｸｼｮﾝ選択ﾒﾆｭｰ起動
	}
}

/*********************************/
void CFunctionDlg::OnCheck1(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ1
/*********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check1, 0x1);										// [F5]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck2(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ2
/*********************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check2, 0x2);										// [F6]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck3(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ3
/*********************************/
{
	m_Check3 = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check3, 0x4);										// [F7]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck4(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ4
/*********************************/
{
	m_Check4 = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check4, 0x8);										// [F8]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck5(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ5
/*********************************/
{
	m_Check5 = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check5, 0x10);										// [F9]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck6(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ6
/*********************************/
{
	m_Check6 = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check6, 0x20);										// [F10]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck7(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ7
/*********************************/
{
	m_Check7 = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check7, 0x40);										// [F11]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck8(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ8
/*********************************/
{
	m_Check8 = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check8, 0x80);										// [F12]ｷｰの更新
}

/*********************************/
void CFunctionDlg::OnCheck9(void)										//<<< COPﾎﾞﾀﾝﾁｪｯｸ
/*********************************/
{
	m_Check9 = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCopyBtn(m_Check9);											// [COP]ﾎﾞﾀﾝの更新
}

/**********************************/
void CFunctionDlg::OnCheck10(void)										//<<< NUMﾎﾞﾀﾝﾁｪｯｸ
/**********************************/
{
	m_Check10 = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetNumBtn(m_Check10);											// [NUM]ﾎﾞﾀﾝの更新
}

/**********************************/
void CFunctionDlg::OnCheck11(void)										//<<< Shiftﾎﾞﾀﾝﾁｪｯｸ
/**********************************/
{
	m_Check11 = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCChar(m_Check11, 0x1);										// [Shift]ｷｰの更新
}

/**********************************/
void CFunctionDlg::OnCheck12(void)										//<<< Shiftﾎﾞﾀﾝﾁｪｯｸ
/**********************************/
{
	m_Check12 = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCChar(m_Check12, 0x2);										// [Ctrl]ｷｰの更新
}

/*******************************/
BOOL CFunctionDlg::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/*******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "FncLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "FncTop", -1);
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

/*************************************/
void CFunctionDlg::OffAllButton(void)									//<<< 全釦OFF
/*************************************/
{
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/***********************************************/
void CFunctionDlg::OnOffButton(int no, int flg)							//<<< 指定釦ON/OFF
/***********************************************/
{
	if(no==0) {
		m_Check1 = flg;
	} else if(no==1) {
		m_Check2 = flg;
	} else if(no==2) {
		m_Check3 = flg;
	} else if(no==3) {
		m_Check4 = flg;
	} else if(no==4) {
		m_Check5 = flg;
	} else if(no==5) {
		m_Check6 = flg;
	} else if(no==6) {
		m_Check7 = flg;
	} else if(no==7) {
		m_Check8 = flg;
	} else if(no==8) {
		m_Check9 = flg;
	} else if(no==9) {
		m_Check10 = flg;
	} else if(no==10) {
		m_Check11 = flg;
	} else if(no==11) {
		m_Check12 = flg;
	}
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/***********************************************/
void CFunctionDlg::MessageOut(int nID, int flg)							//<<< ﾒｯｾｰｼﾞ出力
/***********************************************/
{
	CString strMess, strBuff[13], strCKey, strNKey;
	int     i, pos, kenv;

	if(!View->IsFuncDsp()) {											// ﾀﾞｲｱﾛｸﾞ表示無は中止
		return;
	}
	if(flg==0&&m_Lock==1&&nID>0) {										// ﾛｯｸ状態は非ﾛｯｸﾒｯｾｰｼﾞ表示無
		return;
	}
	m_Lock = flg;														// ﾛｯｸ状態を更新
	View->ShowFunction();												// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ表示
	m_nID = 0;															// ﾒﾆｭｰID初期化
	for(i=0; i<13; i++) {
		strBuff[i] = _T("");											// ﾌｧﾝｸｼｮﾝﾀｲﾄﾙ初期化
	}
	if(nID>=10200&&nID<12000) {											// <JCAD3固有のﾒﾆｭｰ>
		strMess.LoadString(nID+5000);									// ﾘｿｰｽからﾒｯｾｰｼﾞ取得
		for(i=0; i<13; i++) {
			pos = strMess.Find("|");									// 区切文字(|)
			strBuff[i] = strMess.Left(pos);
			strMess = strMess.Mid(pos+1);
		}
		if(strBuff[0]=="A") {											// <常に｢ﾒﾆｭｰ起動｣が有効>
			m_Button1.EnableWindow(TRUE);								// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ有効
			m_nID = nID;												// 起動ﾒﾆｭｰID保存
		} else if(strBuff[0]=="B") {									// <選択立体有で｢ﾒﾆｭｰ起動｣が有効>
			if(View->GetEdtObjNum()>0) {
				m_Button1.EnableWindow(TRUE);							// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ有効
				m_nID = nID;											// 起動ﾒﾆｭｰID保存
			} else {
				m_Button1.EnableWindow(FALSE);							// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ無効
			}
		} else if(strBuff[0]=="C"&&nID==ID_EDIT_PASTEOBJ) {				// <ﾍﾟｰｽﾄ･ｵﾌﾞｼﾞｪｸﾄの場合>
			if(MainWnd->GetCopyCmdFlg()) {								// ｺﾋﾟｰ立体有で
				m_Button1.EnableWindow(TRUE);							// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ有効
				m_nID = nID;											// 起動ﾒﾆｭｰID保存
			} else {
				m_Button1.EnableWindow(FALSE);							// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ無効
			}
		} else {
			m_Button1.EnableWindow(FALSE);								// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ無効
		}
		kenv = View->GetKeyEnv();										// 環境設定情報取得
		if(kenv&0x10) {													// 数値ｼｮｰﾄｶｯﾄ=Shift
			strCKey = "[Ctrl]";
			strNKey = "[Shift]";
		} else {														// 数値ｼｮｰﾄｶｯﾄ=Ctrl
			strCKey = "[Shift]";
			strNKey = "[Ctrl]";
		}
		if(strBuff[11]!=_T("")) {
			strBuff[11] += strNKey;
		}
		if(strBuff[12]!=_T("")) {
			strBuff[12] += strCKey;
		}
	} else if(nID>0) {													// <Windows共通のﾒﾆｭｰ>
		m_Button1.EnableWindow(FALSE);									// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ無効
		strMess.LoadString(nID);										// ﾘｿｰｽからﾒｯｾｰｼﾞ取得
		pos = strMess.Find("\n");										// 区切文字(\n)
		strBuff[2] = strMess.Left(pos);
		strBuff[1] = strMess.Mid(pos+1);
	} else {															// <ﾒﾆｭｰなし(取消)>
		m_Button1.EnableWindow(FALSE);									// ｢ﾒﾆｭｰ起動｣ﾎﾞﾀﾝ無効
	}
	m_Edit1.SetWindowText(strBuff[3]);									// ﾌｧﾝｸｼｮﾝ名(F5)
	m_Edit2.SetWindowText(strBuff[4]);									//     〃   (F6)
	m_Edit3.SetWindowText(strBuff[5]);									//     〃   (F7)
	m_Edit4.SetWindowText(strBuff[6]);									//     〃   (F8)
	m_Edit5.SetWindowText(strBuff[7]);									//     〃   (F9)
	m_Edit6.SetWindowText(strBuff[8]);									//     〃   (F10)
	m_Edit7.SetWindowText(strBuff[9]);									//     〃   (F11)
	m_Edit8.SetWindowText(strBuff[10]);									//     〃   (F12)
	m_Edit9.SetWindowText(strBuff[12]);									//     〃   (COP)
	m_Edit10.SetWindowText(strBuff[11]);								//     〃   (NUM)
	m_Edit11.SetWindowText(strBuff[1]);									// メニュー名
	m_Edit12.SetWindowText(strBuff[2]);									// 処理概要
}

///////////////////////////////////////////////////////////////////////////////
// CFunction2Dlg ダイアログ

BEGIN_MESSAGE_MAP(CFunction2Dlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
END_MESSAGE_MAP()

/**********************************************************************************/
CFunction2Dlg::CFunction2Dlg(CWnd* pParent) : CDialog(CFunction2Dlg::IDD, pParent)
/**********************************************************************************/
{
	m_pParent = pParent;
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = 0;
	m_Check10 = 0;
	m_Check11 = 0;
	m_Check12 = 0;
}

/******************************************************/
void CFunction2Dlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Check(pDX, IDC_CHECK5, m_Check5);
	DDX_Check(pDX, IDC_CHECK6, m_Check6);
	DDX_Check(pDX, IDC_CHECK7, m_Check7);
	DDX_Check(pDX, IDC_CHECK8, m_Check8);
	DDX_Check(pDX, IDC_CHECK9, m_Check9);
	DDX_Check(pDX, IDC_CHECK10, m_Check10);
	DDX_Check(pDX, IDC_CHECK11, m_Check11);
	DDX_Check(pDX, IDC_CHECK12, m_Check12);
}

/*************************************************/
inline LRESULT CFunction2Dlg::OnNcHitTest(CPoint)						//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/*************************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/**************************************/
BOOL CFunction2Dlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**************************************/
{
	int func[8];

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	View->GetFChar(func);												// ﾌｧﾝｸｼｮﾝｷｰ取得
	m_Check1  = (func[0]) ? 1 : 0;										// 釦のON/OFFを設定
	m_Check2  = (func[1]) ? 1 : 0;
	m_Check3  = (func[2]) ? 1 : 0;
	m_Check4  = (func[3]) ? 1 : 0;
	m_Check5  = (func[4]) ? 1 : 0;
	m_Check6  = (func[5]) ? 1 : 0;
	m_Check7  = (func[6]) ? 1 : 0;
	m_Check8  = (func[7]) ? 1 : 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CFunction2Dlg::OnCancel(void)										//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	View->HideFunction2();												// ﾀﾞｲｱﾛｸﾞ消去(非表示)
}

/***********************************/
void CFunction2Dlg::OnDestroy(void)										//<<< WM_DESTROYﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "Fnc2Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Fnc2Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**********************************/
void CFunction2Dlg::OnCheck1(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ1
/**********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check1, 0x1);										// [F5]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck2(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ2
/**********************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check2, 0x2);										// [F6]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck3(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ3
/**********************************/
{
	m_Check3 = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check3, 0x4);										// [F7]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck4(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ4
/**********************************/
{
	m_Check4 = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check4, 0x8);										// [F8]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck5(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ5
/**********************************/
{
	m_Check5 = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check5, 0x10);										// [F9]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck6(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ6
/**********************************/
{
	m_Check6 = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check6, 0x20);										// [F10]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck7(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ7
/**********************************/
{
	m_Check7 = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check7, 0x40);										// [F11]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck8(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ8
/**********************************/
{
	m_Check8 = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetFChar(m_Check8, 0x80);										// [F12]ｷｰの更新
}

/**********************************/
void CFunction2Dlg::OnCheck9(void)										//<<< COPﾎﾞﾀﾝﾁｪｯｸ
/**********************************/
{
	m_Check9 = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCopyBtn(m_Check9);											// [COP]ﾎﾞﾀﾝの更新
}

/***********************************/
void CFunction2Dlg::OnCheck10(void)										//<<< NUMﾎﾞﾀﾝﾁｪｯｸ
/***********************************/
{
	m_Check10 = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetNumBtn(m_Check10);											// [NUM]ﾎﾞﾀﾝの更新
}

/***********************************/
void CFunction2Dlg::OnCheck11(void)										//<<< Shiftﾎﾞﾀﾝﾁｪｯｸ
/***********************************/
{
	m_Check11 = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCChar(m_Check11, 0x1);										// [Shift]ｷｰの更新
}

/***********************************/
void CFunction2Dlg::OnCheck12(void)										//<<< Shiftﾎﾞﾀﾝﾁｪｯｸ
/***********************************/
{
	m_Check12 = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCChar(m_Check12, 0x2);										// [Ctrl]ｷｰの更新
}

/********************************/
BOOL CFunction2Dlg::Create(void)										//<<< ﾀﾞｲｱﾛｸﾞ生成
/********************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "Fnc2Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Fnc2Top", -1);
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

/**************************************/
void CFunction2Dlg::OffAllButton(void)									//<<< 全釦OFF
/**************************************/
{
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/************************************************/
void CFunction2Dlg::OnOffButton(int no, int flg)						//<<< 指定釦ON/OFF
/************************************************/
{
	if(no==0) {
		m_Check1 = flg;
	} else if(no==1) {
		m_Check2 = flg;
	} else if(no==2) {
		m_Check3 = flg;
	} else if(no==3) {
		m_Check4 = flg;
	} else if(no==4) {
		m_Check5 = flg;
	} else if(no==5) {
		m_Check6 = flg;
	} else if(no==6) {
		m_Check7 = flg;
	} else if(no==7) {
		m_Check8 = flg;
	} else if(no==8) {
		m_Check9 = flg;
	} else if(no==9) {
		m_Check10 = flg;
	} else if(no==10) {
		m_Check11 = flg;
	} else if(no==11) {
		m_Check12 = flg;
	}
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

///////////////////////////////////////////////////////////////////////////////
// CCurveCtrlDlg ダイアログ

BEGIN_MESSAGE_MAP(CCurveCtrlDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_EN_CHANGE(IDC_EDIT1, OnSetAngle)
	ON_EN_CHANGE(IDC_EDIT2, OnSetRadius)
END_MESSAGE_MAP()

/**********************************************************************************/
CCurveCtrlDlg::CCurveCtrlDlg(CWnd* pParent) : CDialog(CCurveCtrlDlg::IDD, pParent)
/**********************************************************************************/
{
	m_pParent = pParent;
	m_Check1  = 0;
	m_Check2  = 0;
	View->GetCurveData(&m_Angle, &m_Radius);							// 曲線制御角度設定
}

/******************************************************/
void CCurveCtrlDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_STATIC4, m_Static4);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Text(pDX, IDC_EDIT1, m_Angle);
	DDV_MinMaxDouble(pDX, m_Angle, -359.0, 359.0);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Text(pDX, IDC_EDIT2, m_Radius);
	DDV_MinMaxDouble(pDX, m_Radius, 0.0, 100.0);
}

/*************************************************/
inline LRESULT CCurveCtrlDlg::OnNcHitTest(CPoint)						//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/*************************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/**************************************/
BOOL CCurveCtrlDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**************************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	PermissionCheck1();													// 入力許可ﾁｪｯｸ1
	PermissionCheck2();													// 入力許可ﾁｪｯｸ2
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CCurveCtrlDlg::OnCancel(void)										//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	ShowWindow(SW_HIDE);
}

/***********************************/
void CCurveCtrlDlg::OnDestroy(void)										//<<< WM_DESTROYﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "CtlLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CtlTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**********************************/
void CCurveCtrlDlg::OnCheck1(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ1
/**********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCurveAngle(m_Check1, m_Angle);								// 曲線制御角度設定
	PermissionCheck1();													// 入力許可ﾁｪｯｸ1
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/**********************************/
void CCurveCtrlDlg::OnCheck2(void)										//<<< ﾎﾞﾀﾝﾁｪｯｸ2
/**********************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// 釦のON/OFF状態を取得
	View->SetCurveRadius(m_Check2, m_Radius);							// 曲線制御Ｒ設定
	PermissionCheck2();													// 入力許可ﾁｪｯｸ2
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/************************************/
void CCurveCtrlDlg::OnSetAngle(void)									//<<< 角度設定
/************************************/
{
	CString str;

	m_Edit1.GetWindowText(str);
	if(str!="-") {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		View->SetCurveAngle(m_Check1, m_Angle);							// 曲線制御角度設定
	}
}

/*************************************/
void CCurveCtrlDlg::OnSetRadius(void)									//<<< 半径設定
/*************************************/
{
	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	View->SetCurveRadius(m_Check2, m_Radius);							// 曲線制御Ｒ設定
}

/******************************************/
void CCurveCtrlDlg::PermissionCheck1(void)								//<<< 入力許可ﾁｪｯｸ1
/******************************************/
{
	m_Static1.EnableWindow(m_Check1);
	m_Static2.EnableWindow(m_Check1);
	m_Edit1.EnableWindow(m_Check1);
}

/******************************************/
void CCurveCtrlDlg::PermissionCheck2(void)								//<<< 入力許可ﾁｪｯｸ2
/******************************************/
{
	m_Static3.EnableWindow(m_Check2);
	m_Static4.EnableWindow(m_Check2);
	m_Edit2.EnableWindow(m_Check2);
}

/********************************/
BOOL CCurveCtrlDlg::Create(void)										//<<< ﾀﾞｲｱﾛｸﾞ生成
/********************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "CtlLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "CtlTop", -1);
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
void CCurveCtrlDlg::ClearData(void)										//<<< ﾃﾞｰﾀｸﾘｱ
/***********************************/
{
	m_Check1 = 0;
	m_Check2 = 0;
	PermissionCheck1();													// 入力許可ﾁｪｯｸ1
	PermissionCheck2();													// 入力許可ﾁｪｯｸ2
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

///////////////////////////////////////////////////////////////////////////////
// CProgressDlg ダイアログ

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/*****************************************************************************************/
CProgressDlg::CProgressDlg(int kind, CWnd* pParent) : CDialog(CProgressDlg::IDD, pParent)
/*****************************************************************************************/
{
	m_pParent = pParent;
	m_Kind    = kind;
}

/*****************************************************/
void CProgressDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_CHECK1, m_Button);
}

/*************************************/
BOOL CProgressDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/*************************************/
{
	CString title;

	CDialog::OnInitDialog();
	SetProgress(0);
	if(m_Kind!=0) {
		if(m_Kind==1) {													// ﾀｲﾄﾙ取得
			title.LoadString(IDS_PROGRESS1);
		} else if(m_Kind==2) {
			title.LoadString(IDS_PROGRESS2);
		} else if(m_Kind==3) {
			title.LoadString(IDS_PROGRESS3);
		} else if(m_Kind==4) {
			title.LoadString(IDS_PROGRESS4);
		} else if(m_Kind==5) {
			title.LoadString(IDS_PROGRESS5);
		} else {
			title.LoadString(IDS_PROGRESS6);
		}
		SetWindowText(title);											// ﾀｲﾄﾙ表示
	}
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*******************************/
BOOL CProgressDlg::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/*******************************/
{
	return CDialog::Create(IDD, m_pParent);
}

/***************************************/
void CProgressDlg::SetProgress(int per)									//<<< 進捗率表示
/***************************************/
{
	m_Progress.SetPos(per);												// ﾌﾟﾛｸﾞﾚｽﾊﾞｰｺﾝﾄﾛｰﾙ現在位置設定
}

///////////////////////////////////////////////////////////////////////////////
// CConfirmDlg ダイアログ

BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonClick)
END_MESSAGE_MAP()

/****************************************************************************/
CConfirmDlg::CConfirmDlg(CWnd* pParent) : CDialog(CConfirmDlg::IDD, pParent)
/****************************************************************************/
{
	m_EndMode = 0;
}

/****************************************************/
void CConfirmDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
}

/************************************/
BOOL CConfirmDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	int left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "CofimLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "CofimTop", -1);
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
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*********************************/
void CConfirmDlg::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "CofimLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CofimTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*************************************/
void CConfirmDlg::OnButtonClick(void)									//<<< ｢終了[保存有]｣ﾎﾞﾀﾝ処理
/*************************************/
{
	m_EndMode = 1;														// 保存有終了
	CDialog::OnOK();
}
