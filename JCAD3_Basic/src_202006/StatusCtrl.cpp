/////////////////////////////////////////////////////////////////////////////
// �X�e�[�^�X�o�[�N���X(CStatusCtrl)��`
// StatusCtrl.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CStatusCtrl

BEGIN_MESSAGE_MAP(CStatusCtrl, CStatusBar)
END_MESSAGE_MAP()

/******************************/
CStatusCtrl::CStatusCtrl(void)											// �׽�̍\�z
/******************************/
{
	m_sMess = "Ready";
}

/////////////////////////////////////////////////////////////////////////////
// CStatusCtrl ���b�Z�[�W �n���h��

/**************************************/
void CStatusCtrl::MessageOut(UINT nID)									// ү���ސݒ�
/**************************************/
{
	m_sMess.LoadString(nID);											// ү���ޕ�����Ǎ�
}

/**********************************/
void CStatusCtrl::MessageOut(void)										// ү���ޕ\��
/**********************************/
{
	POINT point;

	GetCursorPos(&point);
	if(MainWnd->GetActiveView()==MainWnd->WindowFromPoint(point)) {		// ϳ���View��ɂ���ꍇ
		GetStatusBarCtrl().SetText(m_sMess, 0, 0);						// ү���ޕ\��
	}
}
