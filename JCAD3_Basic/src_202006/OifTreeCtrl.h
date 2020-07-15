/////////////////////////////////////////////////////////////////////////////
// 立体情報ツリー制御クラス(COifTreeCtrl)宣言
// OifTreeCtrl.h
//---------------------------------------------------------------------------
// LastEdit : 2009/04/01 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___OIFTREECTRL_H
#define ___OIFTREECTRL_H

#include "DataType.h"

#define  MAX_ITEM 3000

class COifTreeCtrl : public CTreeCtrl
{
// コンストラクション
public:
	COifTreeCtrl();

// アトリビュート
public:
	BOOL        m_bDragging;
	HTREEITEM   m_hItemDrag;
	HTREEITEM   m_hItemDrop;
	CImageList* m_pImageList;

// オーバーライド

// インプリメンテーション
public:
	virtual ~COifTreeCtrl();
	int  SearchItemNo(HTREEITEM hitem);
	void UpdateEdtItem(int sel, int edt);
	void UpdateDspItem(int sel, int dsp);
	BOOL IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent);
	void AddDataTable(int no, OIFTYPE* oif, int level, HTREEITEM hitem);
	void DeleteItemOif(HTREEITEM hitem);
	HTREEITEM TransferItem(int flg1, int flg2, int flg3, HTREEITEM hitem,
                           HTREEITEM hNewParent);
	void OnButtonUp(UINT nFlags);

protected:
	afx_msg void OnDestroy();
	afx_msg void OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT* pLResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBeginDrag(LPNMHDR pnmhdr, LRESULT* pLResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	void OnButtonUp(CPoint point);
	DECLARE_MESSAGE_MAP()

// オペレーション
public:
	void InitOifItem(void);
	HTREEITEM COifTreeCtrl::GetParentOif(int lvl);
	void SetOifItem(OIFTYPE* oif);
	void DelOifItem(OIFTYPE* oif);
	void UpdateOifItem(OIFTYPE* oif);
	void SelectOifItem(OIFTYPE* oif);
	void UpdateDspFlg(int flg, OBJTYPE* op);
	void UpdateEdtFlg(int flg, OBJTYPE* op);
	void DelUnitItem(void);

private:
	int       m_iINum;
	OIFTYPE*  m_pOifAra[MAX_ITEM*2];
	int       m_iLvlAra[MAX_ITEM*2];
	HTREEITEM m_hItemAra[MAX_ITEM*2];
	int       m_iParent;
	HTREEITEM m_hItemParent;

};

#endif ___OIFTREECTRL_H

//=========================<EOF:OifTreeCtrl.h>=========================
