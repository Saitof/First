/////////////////////////////////////////////////////////////////////////////
// 立体情報ツリーダイアログクラス(COifTreeDlg)定義
// OifTreeDlg.cpp
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
#include "DataAccess2.h"
#include "OifTreeCtrl.h"

BEGIN_MESSAGE_MAP(COifTreeDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelChanged)
	ON_BN_CLICKED(IDC_BUTTON1, OnCreateUnit)
	ON_BN_CLICKED(IDC_BUTTON2, OnDeleteUnit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COifTreeDlg ダイアログ

/****************************************************************************/
COifTreeDlg::COifTreeDlg(CWnd* pParent) : CDialog(COifTreeDlg::IDD, pParent)
/****************************************************************************/
{
	m_pParent = pParent;
}

/****************************************************/
void COifTreeDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_OifTreeCtrl);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}

/////////////////////////////////////////////////////////////////////////////
// COifTreeDlg メッセージ ハンドラ

/************************************/
BOOL COifTreeDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/									//    ﾀﾞｲｱﾛｸﾞの初期化
{
	CImageList* pImageList;
	CBitmap     bitmap;
	UINT        nID;

	CDialog::OnInitDialog();
	pImageList = new CImageList();
	pImageList->Create(22, 18, ILC_MASK, 4, 2);
	for(nID=IDB_BMTREE1; nID<=IDB_BMTREE4; nID++) {						// ﾋﾞｯﾄﾏｯﾌﾟのﾛｰﾄﾞ
		bitmap.LoadBitmap(nID);
		pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
		bitmap.DeleteObject();
	}
	m_OifTreeCtrl.SetImageList(pImageList, TVSIL_NORMAL);				// ｲﾒｰｼﾞﾘｽﾄ設定
	m_Button2.EnableWindow(FALSE);										// ｢ﾕﾆｯﾄ削除｣ﾎﾞﾀﾝ無効
	return FALSE;
}

/****************************************************/
void COifTreeDlg::OnSize(UINT nType, int cx, int cy)					//<<< WM_SIZEﾒｯｾｰｼﾞ応答処理
/****************************************************/
{
	if(cx>=100&&cy>=100) {												// <ﾂﾘｰｺﾝﾄﾛｰﾙの面積あり>
		if(IsWindow(m_OifTreeCtrl.m_hWnd)) {							// <ﾂﾘｰｺﾝﾄﾛｰﾙが有効>
			m_OifTreeCtrl.MoveWindow(8, 36, cx-15, cy-44);				// ﾂﾘｰｻｲｽﾞ変更
		}
		CDialog::OnSize(nType, cx, cy);
	} else {
		if(cx<100) {
			cx = 100;
		}
		if(cy<100) {
			cy = 100;
		}
		SetWindowPos(NULL, 0, 0, cx+8, cy+8, SWP_NOMOVE|SWP_NOZORDER);
	}
}

/****************************/
void COifTreeDlg::OnOK(void)											//<<< WM_OK ﾒｯｾｰｼﾞ応答処理
/****************************/
{
}

/********************************/
void COifTreeDlg::OnCancel(void)										//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/********************************/
{
	View->HideOifTree();
	ShowWindow(SW_HIDE);
}

/**********************************************/
LRESULT COifTreeDlg::OnNcHitTest(CPoint point)							//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/**********************************************/
{
	UINT ret;

	if((ret=CDialog::OnNcHitTest(point))==HTCLIENT) {
		ret = HTCAPTION;
	}
	return ret;
}

/*********************************/
void COifTreeDlg::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "OifLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "OifTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*****************************************************************/
void COifTreeDlg::OnSelChanged(LPNMHDR pnmhdr, LRESULT *pLResult)		//<<< TVN_SELCHANGED ﾒｯｾｰｼﾞ応答処理
/*****************************************************************/
{
	NMTREEVIEW* ptview;

	ptview = (NMTREEVIEW *)pnmhdr;
	ptview->itemNew.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_OifTreeCtrl.GetItem(&ptview->itemNew);							// 選択ｱｲﾃﾑ情報取得
	if(ptview->itemNew.iImage>1&&View->GetActMenu()==NONE) {			// <選択ｱｲﾃﾑがﾕﾆｯﾄ>
		m_Button2.EnableWindow(TRUE);									// ｢ﾕﾆｯﾄ削除｣ﾎﾞﾀﾝ有効
	} else {															// <選択ｱｲﾃﾑがﾊﾟｰﾂ>
		m_Button2.EnableWindow(FALSE);									// ｢ﾕﾆｯﾄ削除｣ﾎﾞﾀﾝ無効
	}
}

/************************************/
void COifTreeDlg::OnCreateUnit(void)									//<<< ｢ﾕﾆｯﾄ追加｣ﾎﾞﾀﾝ
/************************************/
{
	OIFTYPE* oif;

	CreatOif(0, NULL, OBJ_NAME35, NULL, &oif);							// 構成情報領域確保(ﾕﾆｯﾄ)
	SetOifItem(oif);													// 立体構成情報ｱｲﾃﾑ設定
}

/************************************/
void COifTreeDlg::OnDeleteUnit(void)									//<<< ｢ﾕﾆｯﾄ削除｣ﾎﾞﾀﾝ
/************************************/
{
	m_OifTreeCtrl.DelUnitItem();										// ﾕﾆｯﾄｱｲﾃﾑ削除
}

/////////////////////////////////////////////////////////////////////////////
// COifTreeDlg オペレーション

/******************************/
BOOL COifTreeDlg::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "OifLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "OifTop", -1);
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
void COifTreeDlg::InitOifItem(void)										//<<< 立体構成情報ｱｲﾃﾑ初期化
/***********************************/
{
	m_OifTreeCtrl.InitOifItem();
}

/******************************************/
void COifTreeDlg::SetOifItem(OIFTYPE* oif)								//<<< 立体構成情報ｱｲﾃﾑ設定
/******************************************/
{
	m_OifTreeCtrl.SetOifItem(oif);
}

/******************************************/
void COifTreeDlg::DelOifItem(OIFTYPE* oif)								//<<< 立体構成情報ｱｲﾃﾑ削除
/******************************************/
{
	m_OifTreeCtrl.DelOifItem(oif);
}

/*********************************************/
void COifTreeDlg::UpdateOifItem(OIFTYPE* oif)							//<<< 立体構成情報ｱｲﾃﾑ更新
/*********************************************/
{
	m_OifTreeCtrl.UpdateOifItem(oif);
}

/*********************************************/
void COifTreeDlg::SelectOifItem(OIFTYPE* oif)							//<<< 立体構成情報ｱｲﾃﾑ選択
/*********************************************/
{
	m_OifTreeCtrl.SelectOifItem(oif);
}

/****************************************************/
void COifTreeDlg::UpdateDspFlg(int flg, OBJTYPE* op)					//<<< 立体構成情報表示状態更新
/****************************************************/
{
	m_OifTreeCtrl.UpdateDspFlg(flg, op);
}

/****************************************************/
void COifTreeDlg::UpdateEdtFlg(int flg, OBJTYPE* op)					//<<< 立体構成情報選択状態更新
/****************************************************/
{
	m_OifTreeCtrl.UpdateEdtFlg(flg, op);
}
