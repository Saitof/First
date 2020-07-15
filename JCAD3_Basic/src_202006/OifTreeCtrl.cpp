/////////////////////////////////////////////////////////////////////////////
// ���̏��c���[����N���X(COifTreeCtrl)��`
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
// COifTreeCtrl �׽

BEGIN_MESSAGE_MAP(COifTreeCtrl, CTreeCtrl)								// ү���ޥϯ��
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
void COifTreeCtrl::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/**********************************/
{
	CImageList *pimagelist;

	pimagelist = GetImageList(TVSIL_NORMAL);
	pimagelist->DeleteImageList();
	delete pimagelist;
}

/********************************************************************/
void COifTreeCtrl::OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT* pLResult)	//<<< TVN_ENDLABELEDIT ү���މ�������
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
			if(ptvinfo->item.hItem==m_hItemAra[i]) {					// �w�藧�̍\������
				ok = 1; break;
			}
		}
		if(ok) {
			memset(name, ' ', 256);
			memcpy(name, ptvinfo->item.pszText, strlen(ptvinfo->item.pszText));
			SetOifName(2, m_pOifAra[i], name);							// ���̍\�����ݒ�
		}
	}
	*pLResult = TRUE;
	delete[] name;
}

/*********************************************************/
void COifTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)				//<<< WM_MOUSEMOVE ү���މ�������
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
		GetClientRect(&rct);											// �ذ���۰ق̋�`����
		if(point.y<rct.top) {											// <��`�̏����ׯ��>
			SendMessage(WM_VSCROLL, SB_LINEUP, NULL);					// ��ֽ�۰�
		} else if(point.y>rct.bottom) {									// <��`�̉�����ׯ��>
			SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);					// ���ֽ�۰�
		}
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}

/*****************************************************************/
void COifTreeCtrl::OnBeginDrag(LPNMHDR pnmhdr, LRESULT* pLResult)		//<<< TVN_BEGINDRAG  ү���މ�������
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
	m_pImageList = CreateDragImage(m_hItemDrag);						// ��ׯ�ޗp�Ұ��ؽč쐬
	m_pImageList->DragShowNolock(TRUE);
	m_pImageList->SetDragCursorImage(0, CPoint(0, 0));
	m_pImageList->BeginDrag(0, CPoint(0,0));
	m_pImageList->DragMove(ptAction);
	m_pImageList->DragEnter(this, ptAction);
	SetCapture();
}

/*********************************************************/
void COifTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)				//<<< WM_LBUTTONUP ү���މ�������
/*********************************************************/
{
	OnButtonUp(nFlags);													// ϳ����݉������
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

/*********************************************************/
void COifTreeCtrl::OnRButtonUp(UINT nFlags, CPoint point)				//<<< WM_RBUTTONUP ү���މ�������
/*********************************************************/
{
	OnButtonUp(nFlags);													// ϳ����݉������
	CTreeCtrl::OnRButtonUp(nFlags, point);
}

/***********************************************************/
void COifTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)				//<<< WM_LBUTTONDOWN ү���މ�������
/***********************************************************/
{
	HTREEITEM hitem;
	UINT      flags;
	int       dsp, edt, sel, i1, i2;
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
	if((hitem=HitTest(point, &flags))!=NULL) {							// <<���т�د����Ă���ꍇ>>
		if(flags&TVHT_ONITEMSTATEICON) {								// <�����ޯ���̏ꍇ>
			dsp = GetCheck(hitem);										// �����L��(�\��/��\��)
			sel = SearchItemNo(hitem);									// ����No����
			if(View->GetActMenu()==NONE) {
				UpdateDspItem(sel, dsp);								// �\����ԍX�V
			} else {
				dsp = (dsp) ? 0 : 1;									// ������Ԃ𔽓]
				SetCheck(hitem, dsp);									// �����ݒ�
			}
		} else if(flags&TVHT_ONITEM) {									// <���т̏ꍇ>
			GetItemImage(hitem, i1, i2);								// �Ұ��No�擾
			edt = (i1%2==0) ? 1 : 0;									// �ҏW�I��L�����]
			sel = SearchItemNo(hitem);									// ����No����
			if(View->GetActMenu()==NONE) {
				UpdateEdtItem(sel, edt);								// �ҏW�Ώۏ�ԍX�V
			}
		}
	}
}

/*****************************************************/
void COifTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos,
                             CScrollBar *pScrollBar)					//<<< WM_VSCROLL ү���މ�������
/*****************************************************/
{
	RedrawWindow(NULL, NULL);
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

/***********************************************/
int COifTreeCtrl::SearchItemNo(HTREEITEM hitem)							//<<< ����No����
/***********************************************/
{
	int i=0, ok=0;

	for(i=0; i<m_iINum; i++) {
		if(hitem==m_hItemAra[i]) {										// �w�豲�ь���
			ok = 1; break;
		}
	}
	if(!ok) {															// ����NG�͐擪No
		i = 0;
	}
	return i;
}

/**************************************************/
void COifTreeCtrl::UpdateEdtItem(int sel, int edt)						//<<< �ҏW�Ώۏ�ԍX�V
/**************************************************/
{
	OBJTYPE* op;
	int      i, dsp, edt2, lvl;

	GetOifObj(m_pOifAra[sel], &op);										// �Ώۗ����߲���擾
	edt2 = (op!=NULL) ? edt : edt+2;									// �Ұ��No
	dsp = GetCheck(m_hItemAra[sel]);									// �����L��(�\��/��\��)
	if(dsp) {															// <�\�����̏ꍇ>
		SetItemImage(m_hItemAra[sel], edt2, edt2);						// �Ұ��No�ݒ�
		if(op!=NULL) {
			if(edt) {													// �ҏW�Ώېؑւ�
				OnEdtFlg(0, op);
			} else {
				OffEdtFlg(0, op);
			}
		}
	}
	lvl = m_iLvlAra[sel];												// �\������No
	for(i=sel+1; i<m_iINum; i++) {
		if(m_iLvlAra[i]<=lvl) {											// �w�����وȏ�͏I��
			break;
		}
		GetOifObj(m_pOifAra[i], &op);									// �Ώۗ����߲���擾
		edt2 = (op!=NULL) ? edt : edt+2;								// �Ұ��No
		dsp = GetCheck(m_hItemAra[i]);									// �����L��(�\��/��\��)
		if(dsp) {														// <�\�����̏ꍇ>
			SetItemImage(m_hItemAra[i], edt2, edt2);					// �Ұ��No�ݒ�
			if(op!=NULL) {
				if(edt) {												// �ҏW�Ώېؑւ�
					OnEdtFlg(0, op);
				} else {
					OffEdtFlg(0, op);
				}
			}
		}
	}
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->Display(0);													// �ĕ`��
}

/**************************************************/
void COifTreeCtrl::UpdateDspItem(int sel, int dsp)						//<<< �\����ԍX�V
/**************************************************/
{
	OBJTYPE* op;
	int      i, edt, lvl;

	GetOifObj(m_pOifAra[sel], &op);										// �Ώۗ����߲���擾
	edt = (op!=NULL) ? 0 : 2;											// �Ұ��No
	SetItemImage(m_hItemAra[sel], edt, edt);							// �Ұ��No�ݒ�
	SetCheck(m_hItemAra[sel], dsp);										// �\�������ޯ������
	if(op!=NULL) {
		OffEdtFlg(0, op);												// �ҏW�Ώ۔�I��
		if(dsp) {														// �\����Ԑؑւ�
			OnDspFlg(0, op);
		} else {
			OffDspFlg(0, op);
		}
	}
	lvl = m_iLvlAra[sel];												// �\������No
	for(i=sel+1; i<m_iINum; i++) {
		if(m_iLvlAra[i]<=lvl) {											// �w�����وȏ�͏I��
			break;
		}
		GetOifObj(m_pOifAra[i], &op);									// �Ώۗ����߲���擾
		edt = (op!=NULL) ? 0 : 2;										// �Ұ��No
		SetItemImage(m_hItemAra[i], edt, edt);							// �Ұ��No�ݒ�
		SetCheck(m_hItemAra[i], dsp);									// �\�������ޯ������
		if(op!=NULL) {
			OffEdtFlg(0, op);											// �ҏW�Ώ۔�I��
			if(dsp) {													// �\����Ԑؑւ�
				OnDspFlg(0, op);
			} else {
				OffDspFlg(0, op);
			}
		}
	}
	View->CountEdtObj();												// �ҏW�Ώۗ��̶���
	View->Display(0);													// �ĕ`��
}

/****************************************************************/
BOOL COifTreeCtrl::IsChildNodeOf(HTREEITEM hitemChild,
                                 HTREEITEM hitemSuspectedParent)		//<<< �ړ�����ړ���ʒu�֌W����
/****************************************************************/
{
	do {
		if(hitemChild==hitemSuspectedParent) {
			break;
		}
	} while((hitemChild=GetParentItem(hitemChild))!=NULL);				// �ړ������ړ�����܂ނ��m�F
	return (hitemChild!=NULL);
}

/****************************************************************/
void COifTreeCtrl::AddDataTable(int no, OIFTYPE* oif, int level,
								HTREEITEM hitem)						//<<< �ް�ð��ْǉ�
/****************************************************************/
{
	int i;

	for(i=m_iINum-1; i>no; i--) {
		m_pOifAra[i+1] = m_pOifAra[i];									// �ް�����
		m_iLvlAra[i+1] = m_iLvlAra[i];
		m_hItemAra[i+1] = m_hItemAra[i];
	}
	m_pOifAra[no+1] = oif;												// �ް�ð��ْǉ�
	m_iLvlAra[no+1] = level;
	m_hItemAra[no+1] = hitem;
	m_iINum++;
}

/*************************************************/
void COifTreeCtrl::DeleteItemOif(HTREEITEM hitem)						//<<< ���̍\�������/�ް��폜
/*************************************************/
{
	int i, j, ok=0;

	for(i=0; i<m_iINum; i++) {
		if(hitem==m_hItemAra[i]) {										// �w�豲�ь���
			ok = 1; break;
		}
	}
	if(ok) {															// <�ް�����>
		DeleteItem(hitem);												// �w�豲�э폜
		FreeOif(1, m_pOifAra[i]);										// �w�藧�̍\�����폜
		for(j=i+1; j<m_iINum; j++) {
			m_pOifAra[j-1] = m_pOifAra[j];								// �ް��O���
			m_iLvlAra[j-1] = m_iLvlAra[j];
			m_hItemAra[j-1] = m_hItemAra[j];
		}
		m_iINum--;														// ���ѐ����Z
	}
}

/******************************************************************/
HTREEITEM COifTreeCtrl::TransferItem(int flg1, int flg2, int flg3,
                                     HTREEITEM hitemDrag,
                                     HTREEITEM hitemDrop)				//<<< ���т̕��ʈړ�
/******************************************************************/
{
	TV_INSERTSTRUCT tvitem;
	HTREEITEM hNewItem, hFirstChild, hChildItem;
	OBJTYPE*  opDrag, * opDrop;
	OIFTYPE*  oif;
	CString   strName;
	int       DragNo, DropNo, no, level, edt=0, nflg=1;
	char      name[25];

	if(flg1) {															// <�ړ�����>
		if(flg2==2) {
			hitemDrop = m_hItemAra[m_iINum-1];							// �����ړ�
		}
		DragNo = SearchItemNo(hitemDrag);								// �ړ���(Drag)����No����
		GetOifObj(m_pOifAra[DragNo], &opDrag);							// �ړ��������߲���擾
		if(opDrag!=NULL) {
			OnDspFlg(0, opDrag);										// �ړ������̕\��
			OffEdtFlg(0, opDrag);										// �ړ������̔�I��
		} else {
			edt = 2;
		}
		GetOifName(m_pOifAra[DragNo], name);							// �ړ����\�����擾
		name[24] = 0x0;
		strName.Format("%s", name);
		DropNo = SearchItemNo(hitemDrop);								// �ړ���(Drop)����No����
		GetOifObj(m_pOifAra[DropNo], &opDrop);							// �ړ��旧���߲���擾
		level = GetOifLevel(m_pOifAra[DropNo]);							// �ړ���\�����َ擾
		tvitem.item.hItem = hitemDrag;
		tvitem.item.mask = TVIF_CHILDREN|TVIF_HANDLE;
		GetItem(&tvitem.item);											// ���я��擾
		if(m_iParent) {													// <�Ưč폜�̏ꍇ>
			if(m_hItemParent==NULL) {									// �e���͈ړ���\������:0
				level = 0;
			} else {
				no = SearchItemNo(m_hItemParent);						// �e����No����
				level = GetOifLevel(m_pOifAra[no])+1;					// �ړ���\�����َ擾
			}
			tvitem.hParent = m_hItemParent;								// ���̂̏���ƯĂ������
			if(hitemDrop==NULL) {
				tvitem.hInsertAfter = TVI_FIRST;						// �ړ���:NULL�͐擪�ɐڑ�
			} else {
				tvitem.hInsertAfter = hitemDrop;						// ���̂̒���ɐڑ�
			}
			m_iParent = 0;												// �̈揉����
			m_hItemParent = NULL;
		} else {
			if(flg2==1) {												// <�擪�ړ��̏ꍇ>
				tvitem.hParent = NULL;									// �e�ƯĂȂ�
				tvitem.hInsertAfter = TVI_FIRST;						// �擪�ɐڑ�
				level = 0;												// �\�����ق͕K��0
			} else if(flg2==2) {										// <�����ړ��̏ꍇ>
				tvitem.hParent = NULL;									// �e�ƯĂȂ�
				tvitem.hInsertAfter = hitemDrop;						// �����ɐڑ�
				level = 0;												// �\�����ق͕K��0
			} else if(flg3&&opDrop==NULL) {								// <�����ƯĂ̏ꍇ>
				tvitem.hParent = hitemDrop;								// �����ƯĂ������
				tvitem.hInsertAfter = TVI_FIRST;						// �����ƯĂ̒���ɐڑ�
				level++;												// �\�����ٱ���
			} else {													// <���̂̏ꍇ>
				tvitem.hParent = GetParentItem(hitemDrop);				// ���̂̏���ƯĂ������
				tvitem.hInsertAfter = hitemDrop;						// ���̂̒���ɐڑ�
			}
		}
		tvitem.item.pszText = strName.GetBuffer(strName.GetLength());	// ����
		strName.ReleaseBuffer();
		tvitem.item.cchTextMax = 24;
		tvitem.item.iImage = edt;										// ��I����Ԃ̲Ұ��No
		tvitem.item.iSelectedImage = edt;								// �I����Ԃ̲Ұ��No
		tvitem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;		// ���т̐ݒ荀��
		hNewItem = InsertItem(&tvitem);									// ���я��ǉ�
		SetCheck(hNewItem, 1);											// �\�������ޯ������
		if(flg2==1) {													// <�擪�ړ��̏ꍇ>
			CreatOif(1, NULL, name, NULL, &oif);						// ���̍\�����쐬(�擪)
			AddDataTable(-1, oif, level, hNewItem);						// �ް�ð��ْǉ�
		} else {														// <�ȊO�̏ꍇ>
			CreatOif(2, NULL, name, m_pOifAra[DropNo], &oif);			// ���̍\�����쐬(����)
			AddDataTable(DropNo, oif, level, hNewItem);					// �ް�ð��ْǉ�
		}
		SetOifObj(oif, opDrag);											// �\����񗧑��߲���ݒ�
		SetOifLevel(oif, level);										// ���̍\�����ِݒ�
		hChildItem = hNewItem;											// �ǉ�(�ړ�)����
	} else {															// <�ړ��Ȃ�>
		hChildItem = hitemDrop;											// �ړ��汲��
	}
	while((hFirstChild=GetChildItem(hitemDrag))!=NULL) {				// <�����ɱ��т�����ꍇ>
		hChildItem=TransferItem(1, 0, nflg, hFirstChild, hChildItem);	// ���т̕��ʈړ�
		DeleteItemOif(hFirstChild);										// ���̍\�������/�ް��폜
		nflg = 0;														// �ƯĂ̎��̂�,nflg=1
	}
	return hNewItem;
}

/******************************************/
void COifTreeCtrl::OnButtonUp(UINT nFlags)								//<<< ϳ����݉������
/******************************************/
{
	int flg=0;

	if(m_bDragging) {
		m_pImageList->DragLeave(this);
		m_pImageList->EndDrag();
		delete m_pImageList;
		m_pImageList = NULL;
		if(View->GetActMenu()==NONE) {
			if(nFlags&MK_CONTROL) {										// Ctrl��������,�擪��
				flg = 1;
			} else if(nFlags&MK_SHIFT) {								// Shift��������,������
				flg = 2;
			}
			if(m_hItemDrag!=NULL&&m_hItemDrop!=NULL&&
				m_hItemDrag!=m_hItemDrop&&								// <�ړ����ƈړ��悪�قȂ�&
				!IsChildNodeOf(m_hItemDrop, m_hItemDrag)) {				//  �ړ������ړ�����܂܂Ȃ�>
				TransferItem(1, flg, 1, m_hItemDrag, m_hItemDrop);		// ���т̕��ʈړ�
				DeleteItemOif(m_hItemDrag);								// ���̍\�������/�ް��폜
			} else {
				MessageBeep(0);
			}
			View->CountEdtObj();										// �ҏW�Ώۗ��̶���
			View->Display(0);											// �ĕ`��
		}
		ReleaseCapture();
		m_bDragging = FALSE;
		SelectDropTarget(NULL);
	}
}

/************************************/
void COifTreeCtrl::InitOifItem(void)									//<<< ���̍\������я�����
/************************************/
{
	m_bDragging = FALSE;
	m_pImageList = NULL;
	m_iINum = 0;
	SelectItem(NULL);													// ���є�I��
	DeleteAllItems();													// �S���э폜
}

/*********************************************/
HTREEITEM COifTreeCtrl::GetParentOif(int lvl)							//<<< �e�\�����ю擾
/*********************************************/
{
	HTREEITEM ret=NULL;
	int       i;

	for(i=0; i<m_iINum; i++) {
		if(m_iLvlAra[i]==lvl-1) {										// ���O�̏�ʱ��т�����
			ret = m_hItemAra[i];
		}
	}
	return ret;
}

/*******************************************/
void COifTreeCtrl::SetOifItem(OIFTYPE* oif)								//<<< ���̍\������ѐݒ�
/*******************************************/
{
	TV_INSERTSTRUCT tvitem;
	OBJTYPE* op;
	CString  strName;
	int      dsp, edt, lvl;
	char     name[25];

	GetOifObj(oif, &op);												// ���̍\�������߲���擾
	if(op==NULL) {														// ���̖��͕\��,�Ư�
		dsp = 1, edt = 2;
	} else {
		dsp = CheckObjPtr(1, op);										// ���̕\����Ԏ擾
		edt = CheckObjPtr(2, op);										// ���̑I����Ԏ擾
	}
	if(!dsp) {															// ��\���͕K����I��
		edt = 0;
	}
	GetOifName(oif, name);												// ���̍\�����擾
	name[24] = 0x0;
	strName.Format("%s", name);
	lvl = GetOifLevel(oif);												// ���̍\�����َ擾
	tvitem.hParent = (!lvl) ? NULL : GetParentOif(lvl);					// �e����
	tvitem.hInsertAfter = (!m_iINum) ? NULL : m_hItemAra[m_iINum-1];	// ���O����
	tvitem.item.iImage = edt;											// ��I����Ԃ̲Ұ��No
	tvitem.item.iSelectedImage = edt;									// �I����Ԃ̲Ұ��No
	tvitem.item.pszText = strName.GetBuffer(strName.GetLength());		// ����
	strName.ReleaseBuffer();
	tvitem.item.cchTextMax = 24;
	tvitem.item.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;			// ���т̐ݒ荀��
	if(m_iINum<MAX_ITEM) {
		m_hItemAra[m_iINum] = InsertItem(&tvitem);						// ���ѐݒ�
		SetCheck(m_hItemAra[m_iINum], dsp);								// �\�������ޯ������
		SelectItem(m_hItemAra[m_iINum]);								// ���ёI��
		m_pOifAra[m_iINum] = oif;
		m_iLvlAra[m_iINum] = lvl;
		m_iINum++;
	}
}

/*******************************************/
void COifTreeCtrl::DelOifItem(OIFTYPE* oif)								//<<< ���̍\������э폜
/*******************************************/
{
	HTREEITEM hitem1, hitem2;
	OBJTYPE* op;
	int      i, j, ok=0, lvl;

	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// �w�藧�̍\������
			ok = 1; break;
		}
	}
	if(ok) {															// <�ް�����>
		hitem1 = GetParentItem(m_hItemAra[i]);							// �e��������َ擾
		DeleteItem(m_hItemAra[i]);										// �w�豲�э폜
		for(j=i+1; j<m_iINum; j++) {
			m_pOifAra[j-1] = m_pOifAra[j];								// �ް��O���
			m_iLvlAra[j-1] = m_iLvlAra[j];
			m_hItemAra[j-1] = m_hItemAra[j];
		}
		m_iINum--;														// ���ѐ����Z
		while(TRUE) {
			for(i=0, ok=0; i<m_iINum; i++) {
				if(hitem1==m_hItemAra[i]) {								// �e���ь���
					ok = 1; break;
				}
			}
			if(ok) {													// <�e�ް�����>
				lvl = m_iLvlAra[i];										// �e������No
				for(j=i+1; j<m_iINum; j++) {
					if(m_iLvlAra[j]<=lvl) {								// �w�����وȏ�͏I��
						break;
					}
					GetOifObj(m_pOifAra[j], &op);						// ���̍\�������߲���擾
					if(op!=NULL) {										// �q���̂���͒��~
						ok = 0; break;
					}
				}
				if(ok) {												// <�q���̂Ȃ�>
					FreeOif(1, m_pOifAra[i]);							// ���̍\������ް��폜
					hitem2 = GetParentItem(hitem1);						// ���e��������َ擾
					DeleteItem(hitem1);									// �e���э폜
					for(j=i+1; j<m_iINum; j++) {
						m_pOifAra[j-1] = m_pOifAra[j];					// �ް��O���
						m_iLvlAra[j-1] = m_iLvlAra[j];
						m_hItemAra[j-1] = m_hItemAra[j];
					}
					m_iINum--;											// ���ѐ����Z
					if((hitem1=hitem2)==NULL) {							// ���e��(���͏I��)
						break;
					}
				}
			}
			if(!ok) {													// �I��
				break;
			}
		}
	}
}

/**********************************************/
void COifTreeCtrl::UpdateOifItem(OIFTYPE* oif)							//<<< ���̍\������эX�V
/**********************************************/
{
	OBJTYPE* op;
	CString  strName;
	char     name[25];
	int      i, ok=0, dsp, edt;

	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// �w�藧�̍\������
			ok = 1; break;
		}
	}
	if(ok) {															// <�ް�����>
		GetOifObj(oif, &op);											// ���̍\�������߲���擾
		if(op==NULL) {													// ���̖��͕\��,�Ư�
			dsp = 1, edt = 2;
		} else {
			dsp = CheckObjPtr(1, op);									// ���̕\����Ԏ擾
			edt = CheckObjPtr(2, op);									// ���̑I����Ԏ擾
		}
		if(!dsp) {														// ��\���͕K����I��
			edt = 0;
		}
		GetOifName(oif, name);											// ���̍\�����擾
		name[24] = 0x0;
		strName.Format("%s", name);
		SetItemImage(m_hItemAra[i], edt, edt);							// �Ұ��No�ݒ�
		SetCheck(m_hItemAra[i], dsp);									// �\�������ޯ������
		SetItemText(m_hItemAra[i], strName);							// �\�����ݒ�
		SelectItem(m_hItemAra[i]);										// ���ёI��
	}
}

/**********************************************/
void COifTreeCtrl::SelectOifItem(OIFTYPE* oif)							//<<< ���̍\������ёI��
/**********************************************/
{
	int i, ok=0;

	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// �w�藧�̍\������
			ok = 1; break;
		}
	}
	if(ok) {
		SelectItem(m_hItemAra[i]);										// ���ёI��
	}
}

/*****************************************************/
void COifTreeCtrl::UpdateDspFlg(int flg, OBJTYPE* op)					//<<< ���̍\�����\����ԍX�V
/*****************************************************/
{
	OIFTYPE* oif;
	int      i, ok=0;

	GetObjOif(op, &oif);												// ���̍\������߲���擾
	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// �w�藧�̍\������
			ok = 1; break;
		}
	}
	if(ok) {															// <�ް�����>
		SetCheck(m_hItemAra[i], flg);									// �\�������ޯ������
	}
}

/*****************************************************/
void COifTreeCtrl::UpdateEdtFlg(int flg, OBJTYPE* op)					//<<< ���̍\�����I����ԍX�V
/*****************************************************/
{
	OIFTYPE* oif;
	int      i, ok=0;

	GetObjOif(op, &oif);												// ���̍\������߲���擾
	for(i=0; i<m_iINum; i++) {
		if(oif==m_pOifAra[i]) {											// �w�藧�̍\������
			ok = 1; break;
		}
	}
	if(ok) {															// <�ް�����>
		SetItemImage(m_hItemAra[i], flg, flg);							// �Ұ��No�ݒ�
	}
}

/************************************/
void COifTreeCtrl::DelUnitItem(void)									//<<< �Ưı��э폜
/************************************/
{
	HTREEITEM hitem, hbitem;
	OBJTYPE* op;
	int      i, ok=0;

	if(View->GetActMenu()!=NONE) {										// �ƭ��N�����͒��~
		return;
	}
	hitem = GetSelectedItem();											// �I���ю擾
	for(i=0; i<m_iINum; i++) {
		if(hitem==m_hItemAra[i]) {										// �w�豲�ь���
			ok = 1; break;
		}
	}
	if(ok) {															// <�ް�����>
		GetOifObj(m_pOifAra[i], &op);									// ���̍\�������߲���擾
		if(op!=NULL) {													// �߲������͐e���тłȂ�
			ok = 0;
		}
	}
	if(ok) {															// <�e���т̏ꍇ>
		m_iParent = 1;
		m_hItemParent = GetParentItem(hitem);							// �e�̱���
		hbitem = (i==0) ? NULL : m_hItemAra[i-1];						// ���O�̱���
		TransferItem(0, 0, 1, hitem, hbitem);							// ���т̕��ʈړ�(�w��e�ړ���)
		DeleteItemOif(hitem);											// ���̍\�������/�ް��폜
	}
	m_iParent = 0;														// �̈揉����
	m_hItemParent = NULL;
}
