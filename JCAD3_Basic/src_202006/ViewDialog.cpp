/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewDialog.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/24 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"

/************************************/
void CJcad3GlbView::InitDialog(void)									//<<< �풓�޲�۸ޏ�����
/************************************/
{
	m_pState = NULL;
	m_pDataUse = NULL;
	m_pNumDisp = NULL;
	m_pScrScroll = NULL;
	m_pEyeChange = NULL;
	m_pOifTree = NULL;
	m_pFunction = NULL;
	m_pFunction2 = NULL;
	m_pCurveCtrl = NULL;
	m_pRendDlg = NULL, m_SRendDlg = 0;
	m_pCalcDlg1 = NULL, m_pCalcDlg2 = NULL;
	m_SCalcDlg1 = 0, m_SCalcDlg2 = 0;
	m_strCalc1 = _T("1"), m_strCalc2 = _T("1");
	m_strCalc3 = _T("1"), m_strCalc4 = _T("");
}

/***************************************/
void CJcad3GlbView::CreateDialogs(void)									//<<< �풓�޲�۸ސ���
/***************************************/
{
	m_pState = new CStateDlg(GetParentFrame());							// ��ʏ���޲�۸ލ쐬
	if(m_pState) {
		m_pState->Create();
	}
	ShowStateDlg();
	m_pDataUse = new CDataUseDlg(GetParentFrame());						// �ް��g�p���޲�۸ލ쐬
	if(m_pDataUse) {
		m_pDataUse->Create();
	}
	ShowDataUse();
	m_pNumDisp = new CNumDispDlg(GetParentFrame());						// ���l�\���޲�۸ލ쐬
	if(m_pNumDisp) {
		m_pNumDisp->Create();
	}
	SetNumDisp(m_SNumDisp);
	m_pScrScroll = new CScrollDlg(GetParentFrame());					// ��ʽ�۰��޲�۸ލ쐬
	if(m_pScrScroll) {
		m_pScrScroll->Create();
	}
	ShowScrollTool();
	m_pEyeChange = new CEyeDlg(GetParentFrame());						// ���_�ύX�޲�۸ލ쐬
	if(m_pEyeChange) {
		m_pEyeChange->Create();
	}
	ShowEyeChange();
	m_pOifTree = new COifTreeDlg(GetParentFrame());						// ���̏��°��޲�۸ލ쐬
	if(m_pOifTree) {
		m_pOifTree->Create();
	}
	ShowOifTree();
	m_pFunction = new CFunctionDlg(GetParentFrame());					// ̧ݸ��ݥ���޲�۸ލ쐬
	if(m_pFunction) {
		m_pFunction->Create();
	}
	m_pFunction2 = new CFunction2Dlg(GetParentFrame());					// ̧ݸ��ݥ���޲�۸�2�쐬
	if(m_pFunction2) {
		m_pFunction2->Create();
	}
	if(m_SFuncMode==0) {												// <̧ݸ��ݥ���޲�۸ޏ풓>
		m_SFunction = 0;
		ShowFunction();
		m_SFunction2 = 1;
		HideFunction2();
	} else if(m_SFuncMode==3) {											// <̧ݸ��ݥ���޲�۸�2�풓>
		m_SFunction = 1;
		HideFunction();
		m_SFunction2 = 0;
		ShowFunction2();
	} else {															// <�ȊO>
		m_SFunction = 1;
		HideFunction();
		m_SFunction2 = 1;
		HideFunction2();
	}
	m_pCurveCtrl = new CCurveCtrlDlg(GetParentFrame());					// �Ȑ������޲�۸ލ쐬
	if(m_pCurveCtrl) {
		m_pCurveCtrl->Create();
	}
}

/***************************************/
void CJcad3GlbView::CreateRendDlg(void)									//<<< �����ݸ��޲�۸ސ���
/***************************************/
{
	m_pRendDlg = new CRendDlg(GetParentFrame());						// �����ݸ��޲�۸ލ쐬
	if(m_pRendDlg) {
		m_pRendDlg->Create();
	}
	m_pRendDlg->ShowWindow(SW_SHOWNA);
	m_SRendDlg = 1;
}

/****************************************/
void CJcad3GlbView::CreateCalcDlg1(void)								//<<< �v�Z�@�޲�۸�1����
/****************************************/
{
	m_pCalcDlg1 = new CCalcDlg1(GetParentFrame());						// �v�Z�@�޲�۸�1�쐬
	if(m_pCalcDlg1) {
		m_pCalcDlg1->Create();
	}
	m_pCalcDlg1->ShowWindow(SW_SHOWNA);
	m_SCalcDlg1 = 1;
}

/****************************************/
void CJcad3GlbView::CreateCalcDlg2(void)								//<<< �v�Z�@�޲�۸�2����
/****************************************/
{
	m_pCalcDlg2 = new CCalcDlg2(GetParentFrame());						// �v�Z�@�޲�۸�2�쐬
	if(m_pCalcDlg2) {
		m_pCalcDlg2->Create();
	}																	// ���Ұ��ݒ�
	m_pCalcDlg2->SetParam(m_strCalc1, m_strCalc2, m_strCalc3, m_strCalc4);
	m_pCalcDlg2->ShowWindow(SW_SHOWNA);
	m_SCalcDlg2 = 1;
}

/***********************************************************************/
void CJcad3GlbView::SetCalc(CString a, CString b, CString c, CString d)	//<<< �v�Z�@�޲�۸�2�ް��ݒ�
/***********************************************************************/
{
	m_strCalc1 = a, m_strCalc2 = b, m_strCalc3 = c, m_strCalc4 = d;
}

/************************************/
void CJcad3GlbView::EndDialogs(void)									//<<< �풓�޲�۸ޏI��
/************************************/
{
	if(m_pState) {
		delete m_pState;												// ��ʏ���޲�۸ލ폜
	}
	m_pState = NULL;
	if(m_pDataUse) {
		delete m_pDataUse;												// �ް��g�p���޲�۸ލ폜
	}
	m_pDataUse = NULL;
	if(m_pNumDisp) {
		delete m_pNumDisp;												// ���l�\���޲�۸ލ폜
	}
	m_pNumDisp = NULL;
	if(m_pScrScroll) {
		delete m_pScrScroll;											// ��ʽ�۰��޲�۸ލ폜
	}
	m_pScrScroll = NULL;
	if(m_pEyeChange) {
		delete m_pEyeChange;											// ���_�ύX�޲�۸ލ폜
	}
	m_pEyeChange = NULL;
	if(m_pOifTree) {
		delete m_pOifTree;												// ���̏��°��޲�۸ލ폜
	}
	m_pOifTree = NULL;

	if(m_pFunction) {
		delete m_pFunction;												// ̧ݸ��ݥ���޲�۸ލ폜
	}
	m_pFunction = NULL;
	if(m_pFunction2) {
		delete m_pFunction2;											// ̧ݸ��ݥ���޲�۸�2�폜
	}
	m_pFunction2 = NULL;
	if(m_pCurveCtrl) {
		delete m_pCurveCtrl;											// �Ȑ������޲�۸ލ폜
	}
	m_pCurveCtrl = NULL;
}

/************************************/
void CJcad3GlbView::EndRendDlg(void)									//<<< �����ݸ��޲�۸ޏI��
/************************************/
{
	if(m_pRendDlg) {
		delete m_pRendDlg;												// �����ݸ��޲�۸ލ폜
	}
	m_pRendDlg = NULL;
	m_SRendDlg = 0;
}

/*************************************/
void CJcad3GlbView::EndCalcDlg1(void)									//<<< �v�Z�@�޲�۸�1�I��
/*************************************/
{
	if(m_pCalcDlg1) {
		delete m_pCalcDlg1;												// �v�Z�@�޲�۸�1�폜
	}
	m_pCalcDlg1 = NULL;
	m_SCalcDlg1 = 0;
}

/*************************************/
void CJcad3GlbView::EndCalcDlg2(void)									//<<< �v�Z�@�޲�۸�2�I��
/*************************************/
{
	if(m_pCalcDlg2) {
		delete m_pCalcDlg2;												// �v�Z�@�޲�۸�2�폜
	}
	m_pCalcDlg2 = NULL;
	m_SCalcDlg2 = 0;
}

/**************************************/
void CJcad3GlbView::ShowStateDlg(void)									//<<< ��ʏ�ԕ\��
/**************************************/
{
	m_SState = m_SState?0:1;
	m_pState->ShowWindow(m_SState?SW_SHOWNA:SW_HIDE);
}

/*************************************/
void CJcad3GlbView::ShowDataUse(void)									//<<< �ް��g�p���\��
/*************************************/
{
	m_SDataUse = m_SDataUse?0:1;
	m_pDataUse->ShowWindow(m_SDataUse?SW_SHOWNA:SW_HIDE);
}

/****************************************/
void CJcad3GlbView::SetNumDisp(int disp)								//<<< ���l�\���޲�۸ޕ\��
/****************************************/
{
	if(disp!=2) {
		m_SNumDisp = disp;
	}
	m_pNumDisp->SetNumDispWnd(disp);
}

/****************************************/
void CJcad3GlbView::ShowScrollTool(void)								//<<< ��ʽ�۰�°ٕ\��
/****************************************/
{
	m_SScrScroll = m_SScrScroll?0:1;
	m_pScrScroll->ShowWindow(m_SScrScroll?SW_SHOWNA:SW_HIDE);
}

/***************************************/
void CJcad3GlbView::ShowEyeChange(void)									//<<< ���_�ύX°ٕ\��
/***************************************/
{
	m_SEyeChange = m_SEyeChange?0:1;
	m_pEyeChange->ShowWindow(m_SEyeChange?SW_SHOWNA:SW_HIDE);
}

/*************************************/
void CJcad3GlbView::ShowOifTree(void)									//<<< ���̏��°ٕ\��
/*************************************/
{
	m_SOifTree = m_SOifTree?0:1;
	m_pOifTree->ShowWindow(m_SOifTree?SW_SHOWNA:SW_HIDE);
}

/*************************************/
void CJcad3GlbView::InitOifItem(void)									//<<< ���̍\������я�����
/*************************************/
{
	m_pOifTree->InitOifItem();
}

/********************************************/
void CJcad3GlbView::SetOifItem(OIFTYPE *oif)							//<<< ���̍\������ѐݒ�
/********************************************/
{
	m_pOifTree->SetOifItem(oif);
}

/********************************************/
void CJcad3GlbView::DelOifItem(OIFTYPE *oif)							//<<< ���̍\������э폜
/********************************************/
{
	m_pOifTree->DelOifItem(oif);
}

/***********************************************/
void CJcad3GlbView::UpdateOifItem(OIFTYPE *oif)							//<<< ���̍\������эX�V
/***********************************************/
{
	m_pOifTree->UpdateOifItem(oif);
}

/***********************************************/
void CJcad3GlbView::SelectOifItem(OIFTYPE *oif)							//<<< ���̍\������ёI��
/***********************************************/
{
	m_pOifTree->SelectOifItem(oif);
}

/******************************************************/
void CJcad3GlbView::UpdateDspFlg(int flg, OBJTYPE *op)					//<<< ���̍\�����\����ԍX�V
/******************************************************/
{
	m_pOifTree->UpdateDspFlg(flg, op);
}

/******************************************************/
void CJcad3GlbView::UpdateEdtFlg(int flg, OBJTYPE *op)					//<<< ���̍\�����I����ԍX�V
/******************************************************/
{
	m_pOifTree->UpdateEdtFlg(flg, op);
}

/**************************************/
void CJcad3GlbView::ShowFunction(void)									//<<< ̧ݸ��ݥ���\��
/**************************************/
{
	if(!m_SFunction) {
		m_SFunction = 1;
		m_pFunction->ShowWindow(SW_SHOWNA);
	}
}

/**************************************/
void CJcad3GlbView::HideFunction(void)									//<<< ̧ݸ��ݥ������
/**************************************/
{
	if(m_SFunction) {
		m_SFunction = 0;
		m_pFunction->ShowWindow(SW_HIDE);
	}
}

/***************************************/
void CJcad3GlbView::MessageOut(int nID)									//<<< ̧ݸ��ݥү���ޏo��
/***************************************/
{
	if(m_SFuncMode==0||m_SFuncMode==1) {								// <�ڍ�:SPACE�Ȃ�>
		m_pFunction->MessageOut(nID, m_CChar&0x4?1:0);					// ү���ޏo��
	} else if(m_SFuncMode==2) {											// <�ڍ�:SPACE����>
		if(m_CChar&0x4) {
			m_pFunction->MessageOut(nID, 1);							// ү���ޏo��
		}
	} else if(m_SFuncMode==3||m_SFuncMode==4) {							// <�Ȉ�:SPACE�Ȃ�>
		if(IsFuncDsp2()) {												// �޲�۸ޕ\���w����
			ShowFunction2();											// ̧ݸ����޲�۸�2�\��
		}
	} else if(m_SFuncMode==5) {											// <�Ȉ�:SPACE����>
		if(m_CChar&0x4) {
			if(IsFuncDsp2()) {											// �޲�۸ޕ\���w����
				ShowFunction2();										// ̧ݸ����޲�۸�2�\��
			}
		}
	}
}

/***************************************/
void CJcad3GlbView::ShowFunction2(void)									//<<< ̧ݸ��ݥ��2�\��
/***************************************/
{
	if(!m_SFunction2) {
		m_SFunction2 = 1;
		m_pFunction2->ShowWindow(SW_SHOWNA);
	}
}

/***************************************/
void CJcad3GlbView::HideFunction2(void)									//<<< ̧ݸ��ݥ��2����
/***************************************/
{
	if(m_SFunction2) {
		m_SFunction2 = 0;
		m_pFunction2->ShowWindow(SW_HIDE);
	}
}

/***************************************/
void CJcad3GlbView::ShowCurveCtrl(void)									//<<< �Ȑ������޲�۸ޕ\��
/***************************************/
{
	m_pCurveCtrl->ShowWindow(SW_SHOWNA);
}

/***************************************/
void CJcad3GlbView::HideCurveCtrl(void)									//<<< �Ȑ������޲�۸ޏ���
/***************************************/
{
	m_pCurveCtrl->ShowWindow(SW_HIDE);
}

/****************************************/
void CJcad3GlbView::ClearCurveData(void)								//<<< �Ȑ������ް��ر
/****************************************/
{
	m_pCurveCtrl->ClearData();
	m_CurveAFlg = 0;
	m_CurveRFlg = 0;
}

/**********************************************************/
void CJcad3GlbView::GetCurveData(double* agl, double* rds)				//<<< �Ȑ������ް��擾
/**********************************************************/
{
	*agl = m_CurveAngle;
	*rds = m_CurveRadius;
}

/******************************************************/
void CJcad3GlbView::SetCurveAngle(int flg, double agl)					//<<< �Ȑ�����p�x�ݒ�
/******************************************************/
{
	m_CurveAFlg = flg;
	m_CurveAngle = agl;
}

/*******************************************************/
void CJcad3GlbView::SetCurveRadius(int flg, double rds)					//<<< �Ȑ�����q�ݒ�
/*******************************************************/
{
	m_CurveRFlg = flg;
	m_CurveRadius = rds;
}

/*************************************/
void CJcad3GlbView::ChangePrint(void)									//<<< ������o�͐؂�ւ�
/*************************************/
{
	m_PrtKind = 1;
	CView::OnFilePrint();
}
