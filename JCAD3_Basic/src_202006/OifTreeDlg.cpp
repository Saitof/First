/////////////////////////////////////////////////////////////////////////////
// ���̏��c���[�_�C�A���O�N���X(COifTreeDlg)��`
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
// COifTreeDlg �_�C�A���O

/****************************************************************************/
COifTreeDlg::COifTreeDlg(CWnd* pParent) : CDialog(COifTreeDlg::IDD, pParent)
/****************************************************************************/
{
	m_pParent = pParent;
}

/****************************************************/
void COifTreeDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_OifTreeCtrl);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
}

/////////////////////////////////////////////////////////////////////////////
// COifTreeDlg ���b�Z�[�W �n���h��

/************************************/
BOOL COifTreeDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/									//    �޲�۸ނ̏�����
{
	CImageList* pImageList;
	CBitmap     bitmap;
	UINT        nID;

	CDialog::OnInitDialog();
	pImageList = new CImageList();
	pImageList->Create(22, 18, ILC_MASK, 4, 2);
	for(nID=IDB_BMTREE1; nID<=IDB_BMTREE4; nID++) {						// �ޯ�ϯ�߂�۰��
		bitmap.LoadBitmap(nID);
		pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
		bitmap.DeleteObject();
	}
	m_OifTreeCtrl.SetImageList(pImageList, TVSIL_NORMAL);				// �Ұ��ؽĐݒ�
	m_Button2.EnableWindow(FALSE);										// ��Ưč폜����ݖ���
	return FALSE;
}

/****************************************************/
void COifTreeDlg::OnSize(UINT nType, int cx, int cy)					//<<< WM_SIZEү���މ�������
/****************************************************/
{
	if(cx>=100&&cy>=100) {												// <�ذ���۰ق̖ʐς���>
		if(IsWindow(m_OifTreeCtrl.m_hWnd)) {							// <�ذ���۰ق��L��>
			m_OifTreeCtrl.MoveWindow(8, 36, cx-15, cy-44);				// �ذ���ޕύX
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
void COifTreeDlg::OnOK(void)											//<<< WM_OK ү���މ�������
/****************************/
{
}

/********************************/
void COifTreeDlg::OnCancel(void)										//<<< WM_CANCEL ү���މ�������
/********************************/
{
	View->HideOifTree();
	ShowWindow(SW_HIDE);
}

/**********************************************/
LRESULT COifTreeDlg::OnNcHitTest(CPoint point)							//<<< WM_NCHITTEST ү���މ�������
/**********************************************/
{
	UINT ret;

	if((ret=CDialog::OnNcHitTest(point))==HTCLIENT) {
		ret = HTCAPTION;
	}
	return ret;
}

/*********************************/
void COifTreeDlg::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "OifLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "OifTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*****************************************************************/
void COifTreeDlg::OnSelChanged(LPNMHDR pnmhdr, LRESULT *pLResult)		//<<< TVN_SELCHANGED ү���މ�������
/*****************************************************************/
{
	NMTREEVIEW* ptview;

	ptview = (NMTREEVIEW *)pnmhdr;
	ptview->itemNew.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	m_OifTreeCtrl.GetItem(&ptview->itemNew);							// �I���я��擾
	if(ptview->itemNew.iImage>1&&View->GetActMenu()==NONE) {			// <�I���т��Ư�>
		m_Button2.EnableWindow(TRUE);									// ��Ưč폜����ݗL��
	} else {															// <�I���т��߰�>
		m_Button2.EnableWindow(FALSE);									// ��Ưč폜����ݖ���
	}
}

/************************************/
void COifTreeDlg::OnCreateUnit(void)									//<<< ��ƯĒǉ������
/************************************/
{
	OIFTYPE* oif;

	CreatOif(0, NULL, OBJ_NAME35, NULL, &oif);							// �\�����̈�m��(�Ư�)
	SetOifItem(oif);													// ���̍\������ѐݒ�
}

/************************************/
void COifTreeDlg::OnDeleteUnit(void)									//<<< ��Ưč폜�����
/************************************/
{
	m_OifTreeCtrl.DelUnitItem();										// �Ưı��э폜
}

/////////////////////////////////////////////////////////////////////////////
// COifTreeDlg �I�y���[�V����

/******************************/
BOOL COifTreeDlg::Create(void)											//<<< �޲�۸ސ���
/******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
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
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
	return ret;
}

/***********************************/
void COifTreeDlg::InitOifItem(void)										//<<< ���̍\������я�����
/***********************************/
{
	m_OifTreeCtrl.InitOifItem();
}

/******************************************/
void COifTreeDlg::SetOifItem(OIFTYPE* oif)								//<<< ���̍\������ѐݒ�
/******************************************/
{
	m_OifTreeCtrl.SetOifItem(oif);
}

/******************************************/
void COifTreeDlg::DelOifItem(OIFTYPE* oif)								//<<< ���̍\������э폜
/******************************************/
{
	m_OifTreeCtrl.DelOifItem(oif);
}

/*********************************************/
void COifTreeDlg::UpdateOifItem(OIFTYPE* oif)							//<<< ���̍\������эX�V
/*********************************************/
{
	m_OifTreeCtrl.UpdateOifItem(oif);
}

/*********************************************/
void COifTreeDlg::SelectOifItem(OIFTYPE* oif)							//<<< ���̍\������ёI��
/*********************************************/
{
	m_OifTreeCtrl.SelectOifItem(oif);
}

/****************************************************/
void COifTreeDlg::UpdateDspFlg(int flg, OBJTYPE* op)					//<<< ���̍\�����\����ԍX�V
/****************************************************/
{
	m_OifTreeCtrl.UpdateDspFlg(flg, op);
}

/****************************************************/
void COifTreeDlg::UpdateEdtFlg(int flg, OBJTYPE* op)					//<<< ���̍\�����I����ԍX�V
/****************************************************/
{
	m_OifTreeCtrl.UpdateEdtFlg(flg, op);
}
