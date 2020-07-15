/////////////////////////////////////////////////////////////////////////////
// 立体情報ツリー制御クラス(COifTreeCtrl)定義
// OifTreeCtrl.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/01 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "OifTreeCtrl.h"
#include "DataAccess2.h"
#include "Jcad3GlbView.h"

///////////////////////////////////////////////////////////////////////////////
// COifTreeCtrl ｸﾗｽ

BEGIN_MESSAGE_MAP(COifTreeCtrl, CTreeCtrl)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/********************************/
COifTreeCtrl::COifTreeCtrl(void)
/********************************/
{
	m_bDragging = FALSE;
	m_pImageList = NULL;
	m_iINum = 0;
	m_iParent = 0;
	m_hItemParent = NULL;
}

/*********************************/
COifTreeCtrl::~COifTreeCtrl(void)
/*********************************/
{
}

/**********************************/
void COifTreeCtrl::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	CImageList *pimagelist;

	pimagelist = GetImageList(TVSIL_NORMAL);
	pimagelist->DeleteImageList();
	delete pimagelist;
}

/********************************************************************/
void COifTreeCtrl::OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT* pLResult)	//<<< TVN_ENDLABELEDIT ﾒｯｾｰｼﾞ応答処理
/********************************************************************/
{
	char*        name = new char[256];
	TV_DISPINFO* ptvinfo;
	int          i, ok=0;

	ptvinfo = (TV_DISPINFO *)pnmhdr;
	if(ptvinfo->item.pszText!=NULL) {
		ptvinfo->item.mask = TVIF_TEXT;
		ptvinfo->item.cchTextMax = 24;
		SetItem(&ptvinfo->item);
		for(i=0; i<m_iINum; i++) {
			if(ptvinfo->item.hItem==m_hItemAra[i]) {					// 指定立体構成検索
				ok = 1; break;
			}
		}
		if(ok) {
			memset(name, ' ', 256);
			memcpy(name, ptvinfo->item.pszText, strlen(ptvinfo->item.pszText));
			SetOifName(2, m_pOifAra[i], name);							// 立体構成名設定
		}
	}
	*pLResult = TRUE;
	delete[] name;
}

/*********************************************************/
void COifTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)				//<<< WM_MOUSEMOVE ﾒｯｾｰｼﾞ応答処理
/*********************************************************/
{
	HTREEITEM hitem;
	UINT      flags;
	RECT      rct;

	if(m_bDragging) {
		m_pImageList->DragMove(point);
		if((hitem=HitTest(point, &flags))!=NULL) {
			m_pImageList->DragLeave(this);
			SelectDropTarget(hitem);
			m_hItemDrop = hitem;
			m_pImageList->DragEnter(this, point);
		}
		GetClientRect(&rct);											// ﾂﾘｰｺﾝﾄﾛｰﾙの矩形ｻｲｽﾞ
		if(point.y<rct.top) {											// <矩形の上へﾄﾞﾗｯｸﾞ>
			SendMessage(WM_VSCROLL, SB_LINEUP, NULL);					// 上へｽｸﾛｰﾙ
		} else if(point.y>rct.bottom) {									// <矩形の下へﾄﾞﾗｯｸﾞ>
			SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);					// 下へｽｸﾛｰﾙ
		}
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}

/*****************************************************************/
void COifTreeCtrl::OnBeginDrag(LPNMHDR pnmhdr, LRESULT* pLResult)		//<<< TVN_BEGINDRAG  ﾒｯｾｰｼﾞ応答処理
/*****************************************************************/		//    TVN_BEGINRDRAG
{
	NMTREEVIEW* ptview;
	CPoint      ptAction;

	ptview = (NMTREEVIEW *)pnmhdr;
	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	m_bDragging = TRUE;
	m_hItemDrag = ptview->itemNew.hItem;
	m_hItemDrop = NULL;
	m_pImageList = CreateDragImage(m_hItemDrag);						// ﾄﾞﾗｯｸﾞ用ｲﾒｰｼﾞﾘｽﾄ作成
	m_pImageList->DragShowNolock(TRUE);
	m_pImageList->SetDragCursorImage(0, CPoint(0, 0));
	m_pImageList->BeginDrag(0, CPoint(0,0));
	m_pImageList->DragMove(ptAction);
	m_pImageList->DragEnter(this, ptAction);
	SetCapture();
}

/*********************************************************/
void COifTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)				//<<< WM_LBUTTONUP ﾒｯｾｰｼﾞ応答処理
/*********************************************************/
{
	OnButtonUp(nFlags);													// ﾏｳｽﾎﾞﾀﾝ解放処理
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

/*********************************************************/
void COifTreeCtrl::OnRButtonUp(UINT nFlags, CPoint point)				//<<< WM_RBUTTONUP ﾒｯｾｰｼﾞ応答処理
/*********************************************************/
{
	OnButtonUp(nFlags);													// ﾏｳｽﾎﾞﾀﾝ解放処理
	CTreeCtrl::OnRButtonUp(nFlags, point);
}

/***********************************************************/
void COifTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)				//<<< WM_LBUTTONDOWN ﾒｯｾｰｼﾞ応答処理
/***********************************************************/
{
	HTREEITEM hitem;
	UINT      flags;
	int       dsp, edt, sel, i1, i2;
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
	if((hitem=HitTest(point, &flags))!=NULL) {							// <<ｱｲﾃﾑをｸﾘｯｸしている場合>>
		if(flags&TVHT_ONITEMSTATEICON) {								// <ﾁｪｯｸﾎﾞｯｸｽの場合>
			dsp = GetCheck(hitem);										// ﾁｪｯｸ有無(表示/非表示)
			sel = SearchItemNo(hitem);									// ｱｲﾃﾑNo検索
			if(View->GetActMenu()==NONE) {
				UpdateDspItem(sel, dsp);								// 表示状態更新
			} else {
				dsp = (dsp) ? 0 : 1;									// ﾁｪｯｸ状態を反転
				SetCheck(hitem, dsp);									// ﾁｪｯｸ設定
			}
		} else if(flags&TVHT_ONITEM) {									// <ｱｲﾃﾑの場合>
			GetItemImage(hitem, i1, i2);								// ｲﾒｰｼﾞNo取得
			edt = (i1%2==0) ? 1 : 0;									// 編集選択有無反転
			sel = SearchItemNo(hitem);									// ｱｲﾃﾑNo検索
			if(View->GetActMenu()==NONE) {
				UpdateEdtItem(sel, edt);								// 編集対象状態更新
			}
		}
	}
}

/*****************************************************/
void COifTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos,
                             CScrollBar *pScrollBar)					//<<< WM_VSCROLL ﾒｯｾｰｼﾞ応答処理
/*****************************************************/
{
	RedrawWindow(NULL, NULL);
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

/***********************************************/
int COifTreeCtrl::SearchItemNo(HTREEITEM hitem)							//<<< ｱｲﾃﾑNo検索
/***********************************************/
{
	int i=0, ok=0;

	for(i=0; i<m_iINum; i++) {
		if(hitem==m_hItemAra[i]) {										// 指定ｱｲﾃﾑ検索
			ok = 1; break;
		}
	}
	if(!ok) {															// 検索NGは先頭No
		i = 0;
	}
	return i;
}

/**************************************************/
void COifTreeCtrl::UpdateEdtItem(int sel, int edt)						//<<< 編集対象状態更新
/**************************************************/
{
	OBJTYPE* op;
	int      i, dsp, edt2, lvl;

	GetOifObj(m_pOifAra[sel], &op);										// 対象立体ﾎﾟｲﾝﾀ取得
	edt2 = (op!=NULL) ? edt : edt+2;									// ｲﾒｰｼﾞNo
	dsp = GetCheck(m_hItemAra[sel]);									// ﾁｪｯｸ有無(表示/非表示)
	if(dsp) {															// <表示中の場合>
		SetItemImage(m_hItemAra[sel], edt2, edt2);						// ｲﾒｰｼﾞNo設定
		if(op!=NULL) {
			if(edt) {													// 編集対象切替え
				OnEdtFlg(0, op);
			} else {
				OffEdtFlg(0, op);
			}
		}
	}
	lvl = m_iLvlAra[sel];												// 構成ﾚﾍﾞﾙNo
	for(i=sel+1; i<m_iINum; i++) {
		if(m_iLvlAra[i]<=lvl) {											// 指定ﾚﾍﾞﾙ以上は終了
			break;
		}
		GetOifObj(m_pOifAra[i], &op);									// 対象立体ﾎﾟｲﾝﾀ取得
		edt2 = (op!=NULL) ? edt : edt+2;								// ｲﾒｰｼﾞNo
		dsp = GetCheck(m_hItemAra[i]);									// ﾁｪｯｸ有無(表示/非表示)
		if(dsp) {														// <表示中の場合>
			SetItemImage(m_hItemAra[i], edt2, edt2);					// ｲﾒｰｼﾞNo設定
			if(op!=NULL) {
				if(edt) {												// 編集対象切替え
					OnEdtFlg(0, op);
				} else {
					OffEdtFlg(0, op);
				}
			}
		}
	}
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->Display(0);													// 再描画
}

/**************************************************/
void COifTreeCtrl::UpdateDspItem(int sel, int dsp)						//<<< 表示状態更新
/**************************************************/
{
	OBJTYPE* op;
	int      i, edt, lvl;

	GetOifObj(m_pOifAra[sel], &op);										// 対象立体ﾎﾟｲﾝﾀ取得
	edt = (op!=NULL) ? 0 : 2;											// ｲﾒｰｼﾞNo
	SetItemImage(m_hItemAra[sel], edt, edt);							// ｲﾒｰｼﾞNo設定
	SetCheck(m_hItemAra[sel], dsp);										// 表示ﾁｪｯｸﾎﾞｯｸｽﾁｪｯｸ
	if(op!=NULL) {
		OffEdtFlg(0, op);												// 編集対象非選択
		if(dsp) {														// 表示状態切替え
			OnDspFlg(0, op);
		} else {
			OffDspFlg(0, op);
		}
	}
	lvl = m_iLvlAra[sel];												// 構成ﾚﾍﾞﾙNo
	for(i=sel+1; i<m_iINum; i++) {
		if(m_iLvlAra[i]<=lvl) {											// 指定ﾚﾍﾞﾙ以上は終了
			break;
		}
		GetOifObj(m_pOifAra[i], &op);									// 対象立体ﾎﾟｲﾝﾀ取得
		edt = (op!=NULL) ? 0 : 2;										// ｲﾒｰｼﾞNo
		SetItemImage(m_hItemAra[i], edt, edt);							// ｲﾒｰｼﾞNo設定
		SetCheck(m_hItemAra[i], dsp);									// 表示ﾁｪｯｸﾎﾞｯｸｽﾁｪｯｸ
		if(op!=NULL) {
			OffEdtFlg(0, op);											// 編集対象非選択
			if(dsp) {													// 表示状態切替え
				OnDspFlg(0, op);
			} else {
				OffDspFlg(0, op);
			}
		}
	}
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->Display(0);													// 再描画
}

/****************************************************************/
BOOL COifTreeCtrl::IsChildNodeOf(HTREEITEM hitemChild,
                                 HTREEITEM hitemSuspectedParent)		//<<< 移動元･移動先位置関係ﾁｪｯｸ
/****************************************************************/
{
	do {
		if(hitemChild==hitemSuspectedParent) {
			break;
		}
	} while((hitemChild=GetParentItem(hitemChild))!=NULL);				// 移動元が移動先を含むか確認
	return (hitemChild!=NULL);
}

/****************************************************************/
void COifTreeCtrl::AddDataTable(int no, OIFTYPE* oif, int level,
								HTREEITEM hitem)						//<<< ﾃﾞｰﾀﾃｰﾌﾞﾙ追加
/****************************************************************/
{
	int i;

	for(i=m_iINum-1; i>no; i--) {
		m_pOifAra[i+1] = m_pOifAra[i];									// ﾃﾞｰﾀ後ｼﾌﾄ
		m_iLvlAra[i+1] = m_iLvlAra[i];
		m_hItemAra[i+1] = m_hItemAra[i];
	}
	m_pOifAra[no+1] = oif;												// ﾃﾞｰﾀﾃｰﾌﾞﾙ追加
	m_iLvlAra[no+1] = level;
	m_hItemAra[no+1] = hitem;
	m_iINum++;
}

/*************************************************/
void COifTreeCtrl::DeleteItemOif(HTREEITEM hitem)						//<<< 立体構成情報ｱｲﾃﾑ/ﾃﾞｰﾀ削除
/*************************************************/
{
	int i, j, ok=0;

	for(i=0; i<m_iINum; i++) {
		if(hitem==m_hItemAra[i]) {										// 指定ｱｲﾃﾑ検索
			ok = 1; break;
		}
	}
	if(ok) {															// <ﾃﾞｰﾀあり>
		DeleteItem(hitem);												// 指定ｱｲﾃﾑ削除
		FreeOif(1, m_pOifAra[i]);										// 指定立体構成情報削除
		for(j=i+1; j<m_iINum; j++) {
			m_pOifAra[j-1] = m_pOifAra[j];								// ﾃﾞｰﾀ前ｼﾌﾄ
			m_iLvlAra[j-1] = m_iLvlAra[j];
			m_hItemAra[j-1] = m_hItemAra[j];
		}
		m_iINum--;														// ｱｲﾃﾑ数減算
	}
}

/******************************************************************/
HTREEITEM COifTreeCtrl::TransferItem(int flg1, int flg2, int flg3,
                                     HTREEITEM hitemDrag,
                                     HTREEITEM hitemDrop)				//<<< ｱｲﾃﾑの複写移動
/******************************************************************/
{
	TV_INSERTSTRUCT tvitem;
	HTREEITEM hNewItem, hFirstChild, hChildItem;
	OBJTYPE*  opDrag, * opDrop;
	OIFTYPE*  oif;
	CString   strName;
	int       DragNo, DropNo, no, level, edt=0, nflg=1;
	char      name[25];

	if(flg1) {															// <移動あり>
		if(flg2==2) {
			hitemDrop = m_hItemAra[m_iINum-1];							// 末尾移動
		}
		DragNo = SearchItemNo(hitemDrag);								// 移動元(Drag)ｱｲﾃﾑNo検索
		GetOifObj(m_pOifAra[DragNo], &opDrag);							// 移動元立体ﾎﾟｲﾝﾀ取得
		if(opDrag!=NULL) {
			OnDspFlg(0, opDrag);										// 移動元立体表示
			OffEdtFlg(0, opDrag);										// 移動元立体非選択
		} else {
			edt = 2;
		}
		GetOifName(m_pOifAra[DragNo], name);							// 移動元構成名取得
		name[24] = 0x0;
		strName.Format("%s", name);
		DropNo = SearchItemNo(hitemDrop);								// 移動先(Drop)ｱｲﾃﾑNo検索
		GetOifObj(m_pOifAra[DropNo], &opDrop);							// 移動先立体ﾎﾟｲﾝﾀ取得
		level = GetOifLevel(m_pOifAra[DropNo]);							// 移動先構成ﾚﾍﾞﾙ取得
		tvitem.item.hItem = hitemDrag;
		tvitem.item.mask = TVIF_CHILDREN|TVIF_HANDLE;
		GetItem(&tvitem.item);											// ｱｲﾃﾑ情報取得
		if(m_iParent) {													// <ﾕﾆｯﾄ削除の場合>
			if(m_hItemParent==NULL) {									// 親無は移動先構成ﾚﾍﾞﾙ:0
				level = 0;
			} else {
				no = SearchItemNo(m_hItemParent);						// 親ｱｲﾃﾑNo検索
				level = GetOifLevel(m_pOifAra[no])+1;					// 移動先構成ﾚﾍﾞﾙ取得
			}
			tvitem.hParent = m_hItemParent;								// 立体の上位ﾕﾆｯﾄのﾊﾝﾄﾞﾙ
			if(hitemDrop==NULL) {
				tvitem.hInsertAfter = TVI_FIRST;						// 移動先:NULLは先頭に接続
			} else {
				tvitem.hInsertAfter = hitemDrop;						// 立体の直後に接続
			}
			m_iParent = 0;												// 領域初期化
			m_hItemParent = NULL;
		} else {
			if(flg2==1) {												// <先頭移動の場合>
				tvitem.hParent = NULL;									// 親ﾕﾆｯﾄなし
				tvitem.hInsertAfter = TVI_FIRST;						// 先頭に接続
				level = 0;												// 構成ﾚﾍﾞﾙは必ず0
			} else if(flg2==2) {										// <末尾移動の場合>
				tvitem.hParent = NULL;									// 親ﾕﾆｯﾄなし
				tvitem.hInsertAfter = hitemDrop;						// 末尾に接続
				level = 0;												// 構成ﾚﾍﾞﾙは必ず0
			} else if(flg3&&opDrop==NULL) {								// <中間ﾕﾆｯﾄの場合>
				tvitem.hParent = hitemDrop;								// 中間ﾕﾆｯﾄのﾊﾝﾄﾞﾙ
				tvitem.hInsertAfter = TVI_FIRST;						// 中間ﾕﾆｯﾄの直後に接続
				level++;												// 構成ﾚﾍﾞﾙｱｯﾌﾟ
			} else {													// <立体の場合>
				tvitem.hParent = GetParentItem(hitemDrop);				// 立体の上位ﾕﾆｯﾄのﾊﾝﾄﾞﾙ
				tvitem.hInsertAfter = hitemDrop;						// 立体の直後に接続
			}
		}
		tvitem.item.pszText = strName.GetBuffer(strName.GetLength());	// 名称
		strName.ReleaseBuffer();
		tvitem.item.cchTextMax = 24;
		tvitem.item.iImage = edt;										// 非選択状態のｲﾒｰｼﾞNo
		tvitem.item.iSelectedImage = edt;								// 選択状態のｲﾒｰｼﾞNo
		tvitem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;		// ｱｲﾃﾑの設定項目
		hNewItem = InsertItem(&tvitem);									// ｱｲﾃﾑ情報追加
		SetCheck(hNewItem, 1);											// 表示ﾁｪｯｸﾎﾞｯｸｽﾁｪｯｸ
		if(flg2==1) {													// <先頭移動の場合>
			CreatOif(1, NULL, name, NULL, &oif);						// 立体構成情報作成(先頭)
			AddDataTable(-1, oif, level, hNewItem);						// ﾃﾞｰﾀﾃｰﾌﾞﾙ追加
		} else {														// <以外の場合>
			CreatOif(2, NULL, name, m_pOifAra[DropNo], &oif);			// 立体構成情報作成(中間)
			AddDataTable(DropNo, oif, level, hNewItem);					// ﾃﾞｰﾀﾃｰﾌﾞﾙ追加
		}
		SetOifObj(oif, opDrag);											// 構成情報立体ﾎﾟｲﾝﾀ設定
		SetOifLevel(oif, level);										// 立体構成ﾚﾍﾞﾙ設定
		hChildItem = hNewItem;											// 追加(移動)ｱｲﾃﾑ
	} else {															// <移動なし>
		hChildItem = hitemDrop;											// 移動先ｱｲﾃﾑ
	}
	while((hFirstChild=GetChildItem(hitemDrag))!=NULL) {				// <直下にｱｲﾃﾑがある場合>
		hChildItem=TransferItem(1, 0, nflg, hFirstChild, hChildItem);	// ｱｲﾃﾑの複写移動
		DeleteItemOif(hFirstChild);										// 立体構成情報ｱｲﾃﾑ/ﾃﾞｰﾀ削除
		nflg = 0;														// ﾕﾆｯﾄの次のみ,nflg=1
	}
	return hNewItem;
}

/******************************************/
void COifTreeCtrl::OnButtonUp(UINT nFlags)								//<<< ﾏｳｽﾎﾞﾀﾝ解放処理
/******************************************/
{
	int flg=0;

	if(m_bDragging) {
		m_pImageList->DragLeave(this);
		m_pImageList->EndDrag();
		delete m_pImageList;
		m_pImageList = NULL;
		if(View->GetActMenu()==NONE) {
			if(nFlags&MK_CONTROL) {										// Ctrlｷｰ押下時,先頭へ
				flg = 1;
			} else if(nFlags&MK_SHIFT) {								// Shiftｷｰ押下時,末尾へ
				flg = 2;
			}
			if(m_hItemDrag!=NULL&&m_hItemDrop!=NULL&&
				m_hItemDrag!=m_hItemDrop&&								// <移動元と移動先が異なる&
				!IsChildNodeOf(m_hItemDrop, m_hItemDrag)) {				//  移動元が移動先を含まない>
				TransferItem(1, flg, 1, m_hItemDrag, m_hItemDrop);		// ｱｲﾃﾑの複写移動
				DeleteItemOif(m_hItemDrag);								// 立体構成情報ｱｲﾃﾑ/ﾃﾞｰﾀ削除
			} else {
				MessageBeep(0);
			}
			View->CountEdtObj();										// 編集対象立体ｶｳﾝﾄ
			View->Display(0);											// 再描画
		}
		ReleaseCapture();
		m_bDragging = FALSE;
		SelectDropTarget(NULL);
	}
}

/************************************/
void COifTreeCtrl::InitOifItem(void)									//<<< 立体構成情報ｱｲﾃﾑ初期化
/************************************/
{
	m_bDragging = FALSE;
	m_pImageList = NULL;
	m_iINum = 0;
	SelectItem(NULL);													// ｱｲﾃﾑ非選択
	DeleteAllItems();													// 全ｱｲﾃﾑ削除
}

/*********************************************/
HTREEITEM COifTreeCtrl::GetParentOif(int lvl)							//<<< 親構成ｱｲﾃﾑ取得
/*********************************************/
{
	HTREEITEM ret=NULL;
	int       i;

	for(i=0; i<m_iINum; i++) {
		if(m_iLvlAra[i]==lvl-1) {										// 直前の上位ｱｲﾃﾑを検索
			ret = m_hItemAra[i];
		}
	}
	return ret;
}

/*******************************************/
void COifTreeCtrl::SetOifItem(OIFTYPE* oif)								//<<< 立体構成情報ｱｲﾃﾑ設定
/*******************************************/
{
	TV_INSERTSTRUCT tvitem;
	OBJTYPE* op;
	CString  strName;
	int      dsp, edt, lvl;
	char     name[25];

	GetOifObj(oif, &op);												// 立体構成立体ﾎﾟｲﾝﾀ取得
	if(op==NULL) {														// 立体無は表示,ﾕﾆｯﾄ
		dsp = 1, edt = 2;
	} else {
		dsp = CheckObjPtr(1, op);										// 立体表示状態取得
		edt = CheckObjPtr(2, op);										// 立体選択状態取得
	}
	if(!dsp) {															// 非表示は必ず非選択
		edt = 0;
	}
	GetOifName(oif, name);												// 立体構成名取得
	name[24] = 0x0;
	strName.Format("%s", name);
	lvl = GetOifLevel(oif);												// 立体構成ﾚﾍﾞﾙ取得
	tvitem.hParent = (!lvl) ? NULL : GetParentOif(lvl);					// 親ｱｲﾃﾑ
	tvitem.hInsertAfter = (!m_iINum) ? NULL : m_hItemAra[m_iINum-1];	// 直前ｱｲﾃﾑ
	tvitem.item.iImage = edt;											// 非選択状態のｲﾒｰｼﾞNo
	tvitem.item.iSelectedImage = edt;									// 選択状態のｲﾒｰｼﾞNo
	tvitem.item.pszText = strName.GetBuffer(strName.GetLength());		// 名称
	strName.ReleaseBuffer();
	tvitem.item.cchTextMax = 24;
	tvitem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;			// ｱｲﾃﾑの設定項目
	if(m_iINum<MAX_ITEM) {
		m_hItemAra[m_iINum] = InsertItem(&tvitem);						// ｱｲﾃﾑ設定
		SetCheck(m_hItemAra[m_iINum], dsp);								// 表示ﾁｪｯｸﾎﾞｯｸｽﾁｪｯｸ
		SelectItem(m_hItemAra[m_iINum]);								// ｱｲﾃﾑ選択
		m_pOifAra[m_iINum] = oif;
		m_iLvlAra[m_iINum] = lvl;
		m_iINum++;
	}
}

/*******************************************/
void COifTreeCtrl::DelOifItem(OIFTYPE* oif)								//<<< 立体構成情報ｱｲﾃﾑ削除
/*******************************************/
{
	HTREEITEM hitem1, hitem2;
	OBJTYPE* op;
	int      i, j, ok=0, lvl;

	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// 指定立体構成検索
			ok = 1; break;
		}
	}
	if(ok) {															// <ﾃﾞｰﾀあり>
		hitem1 = GetParentItem(m_hItemAra[i]);							// 親ｱｲﾃﾑﾊﾝﾄﾞﾙ取得
		DeleteItem(m_hItemAra[i]);										// 指定ｱｲﾃﾑ削除
		for(j=i+1; j<m_iINum; j++) {
			m_pOifAra[j-1] = m_pOifAra[j];								// ﾃﾞｰﾀ前ｼﾌﾄ
			m_iLvlAra[j-1] = m_iLvlAra[j];
			m_hItemAra[j-1] = m_hItemAra[j];
		}
		m_iINum--;														// ｱｲﾃﾑ数減算
		while(TRUE) {
			for(i=0, ok=0; i<m_iINum; i++) {
				if(hitem1==m_hItemAra[i]) {								// 親ｱｲﾃﾑ検索
					ok = 1; break;
				}
			}
			if(ok) {													// <親ﾃﾞｰﾀあり>
				lvl = m_iLvlAra[i];										// 親のﾚﾍﾞﾙNo
				for(j=i+1; j<m_iINum; j++) {
					if(m_iLvlAra[j]<=lvl) {								// 指定ﾚﾍﾞﾙ以上は終了
						break;
					}
					GetOifObj(m_pOifAra[j], &op);						// 立体構成立体ﾎﾟｲﾝﾀ取得
					if(op!=NULL) {										// 子立体ありは中止
						ok = 0; break;
					}
				}
				if(ok) {												// <子立体なし>
					FreeOif(1, m_pOifAra[i]);							// 立体構成情報ﾃﾞｰﾀ削除
					hitem2 = GetParentItem(hitem1);						// 次親ｱｲﾃﾑﾊﾝﾄﾞﾙ取得
					DeleteItem(hitem1);									// 親ｱｲﾃﾑ削除
					for(j=i+1; j<m_iINum; j++) {
						m_pOifAra[j-1] = m_pOifAra[j];					// ﾃﾞｰﾀ前ｼﾌﾄ
						m_iLvlAra[j-1] = m_iLvlAra[j];
						m_hItemAra[j-1] = m_hItemAra[j];
					}
					m_iINum--;											// ｱｲﾃﾑ数減算
					if((hitem1=hitem2)==NULL) {							// 次親へ(無は終了)
						break;
					}
				}
			}
			if(!ok) {													// 終了
				break;
			}
		}
	}
}

/**********************************************/
void COifTreeCtrl::UpdateOifItem(OIFTYPE* oif)							//<<< 立体構成情報ｱｲﾃﾑ更新
/**********************************************/
{
	OBJTYPE* op;
	CString  strName;
	char     name[25];
	int      i, ok=0, dsp, edt;

	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// 指定立体構成検索
			ok = 1; break;
		}
	}
	if(ok) {															// <ﾃﾞｰﾀあり>
		GetOifObj(oif, &op);											// 立体構成立体ﾎﾟｲﾝﾀ取得
		if(op==NULL) {													// 立体無は表示,ﾕﾆｯﾄ
			dsp = 1, edt = 2;
		} else {
			dsp = CheckObjPtr(1, op);									// 立体表示状態取得
			edt = CheckObjPtr(2, op);									// 立体選択状態取得
		}
		if(!dsp) {														// 非表示は必ず非選択
			edt = 0;
		}
		GetOifName(oif, name);											// 立体構成名取得
		name[24] = 0x0;
		strName.Format("%s", name);
		SetItemImage(m_hItemAra[i], edt, edt);							// ｲﾒｰｼﾞNo設定
		SetCheck(m_hItemAra[i], dsp);									// 表示ﾁｪｯｸﾎﾞｯｸｽﾁｪｯｸ
		SetItemText(m_hItemAra[i], strName);							// 構成名設定
		SelectItem(m_hItemAra[i]);										// ｱｲﾃﾑ選択
	}
}

/**********************************************/
void COifTreeCtrl::SelectOifItem(OIFTYPE* oif)							//<<< 立体構成情報ｱｲﾃﾑ選択
/**********************************************/
{
	int i, ok=0;

	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// 指定立体構成検索
			ok = 1; break;
		}
	}
	if(ok) {
		SelectItem(m_hItemAra[i]);										// ｱｲﾃﾑ選択
	}
}

/*****************************************************/
void COifTreeCtrl::UpdateDspFlg(int flg, OBJTYPE* op)					//<<< 立体構成情報表示状態更新
/*****************************************************/
{
	OIFTYPE* oif;
	int      i, ok=0;

	GetObjOif(op, &oif);												// 立体構成情報ﾎﾟｲﾝﾀ取得
	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// 指定立体構成検索
			ok = 1; break;
		}
	}
	if(ok) {															// <ﾃﾞｰﾀあり>
		SetCheck(m_hItemAra[i], flg);									// 表示ﾁｪｯｸﾎﾞｯｸｽﾁｪｯｸ
	}
}

/*****************************************************/
void COifTreeCtrl::UpdateEdtFlg(int flg, OBJTYPE* op)					//<<< 立体構成情報選択状態更新
/*****************************************************/
{
	OIFTYPE* oif;
	int      i, ok=0;

	GetObjOif(op, &oif);												// 立体構成情報ﾎﾟｲﾝﾀ取得
	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// 指定立体構成検索
			ok = 1; break;
		}
	}
	if(ok) {															// <ﾃﾞｰﾀあり>
		SetItemImage(m_hItemAra[i], flg, flg);							// ｲﾒｰｼﾞNo設定
	}
}

/************************************/
void COifTreeCtrl::DelUnitItem(void)									//<<< ﾕﾆｯﾄｱｲﾃﾑ削除
/************************************/
{
	HTREEITEM hitem, hbitem;
	OBJTYPE* op;
	int      i, ok=0;

	if(View->GetActMenu()!=NONE) {										// ﾒﾆｭｰ起動中は中止
		return;
	}
	hitem = GetSelectedItem();											// 選択ｱｲﾃﾑ取得
	for(i=0; i<m_iINum; i++) {
		if(hitem==m_hItemAra[i]) {										// 指定ｱｲﾃﾑ検索
			ok = 1; break;
		}
	}
	if(ok) {															// <ﾃﾞｰﾀあり>
		GetOifObj(m_pOifAra[i], &op);									// 立体構成立体ﾎﾟｲﾝﾀ取得
		if(op!=NULL) {													// ﾎﾟｲﾝﾀありは親ｱｲﾃﾑでない
			ok = 0;
		}
	}
	if(ok) {															// <親ｱｲﾃﾑの場合>
		m_iParent = 1;
		m_hItemParent = GetParentItem(hitem);							// 親のｱｲﾃﾑ
		hbitem = (i==0) ? NULL : m_hItemAra[i-1];						// 直前のｱｲﾃﾑ
		TransferItem(0, 0, 1, hitem, hbitem);							// ｱｲﾃﾑの複写移動(指定親移動無)
		DeleteItemOif(hitem);											// 立体構成情報ｱｲﾃﾑ/ﾃﾞｰﾀ削除
	}
	m_iParent = 0;														// 領域初期化
	m_hItemParent = NULL;
}
