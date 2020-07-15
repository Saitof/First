/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)定義
// ViewDialog.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/24 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"

/************************************/
void CJcad3GlbView::InitDialog(void)									//<<< 常駐ﾀﾞｲｱﾛｸﾞ初期化
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
void CJcad3GlbView::CreateDialogs(void)									//<<< 常駐ﾀﾞｲｱﾛｸﾞ生成
/***************************************/
{
	m_pState = new CStateDlg(GetParentFrame());							// 画面状態ﾀﾞｲｱﾛｸﾞ作成
	if(m_pState) {
		m_pState->Create();
	}
	ShowStateDlg();
	m_pDataUse = new CDataUseDlg(GetParentFrame());						// ﾃﾞｰﾀ使用率ﾀﾞｲｱﾛｸﾞ作成
	if(m_pDataUse) {
		m_pDataUse->Create();
	}
	ShowDataUse();
	m_pNumDisp = new CNumDispDlg(GetParentFrame());						// 数値表示ﾀﾞｲｱﾛｸﾞ作成
	if(m_pNumDisp) {
		m_pNumDisp->Create();
	}
	SetNumDisp(m_SNumDisp);
	m_pScrScroll = new CScrollDlg(GetParentFrame());					// 画面ｽｸﾛｰﾙﾀﾞｲｱﾛｸﾞ作成
	if(m_pScrScroll) {
		m_pScrScroll->Create();
	}
	ShowScrollTool();
	m_pEyeChange = new CEyeDlg(GetParentFrame());						// 視点変更ﾀﾞｲｱﾛｸﾞ作成
	if(m_pEyeChange) {
		m_pEyeChange->Create();
	}
	ShowEyeChange();
	m_pOifTree = new COifTreeDlg(GetParentFrame());						// 立体情報ﾂｰﾙﾀﾞｲｱﾛｸﾞ作成
	if(m_pOifTree) {
		m_pOifTree->Create();
	}
	ShowOifTree();
	m_pFunction = new CFunctionDlg(GetParentFrame());					// ﾌｧﾝｸｼｮﾝ･ｷｰﾀﾞｲｱﾛｸﾞ作成
	if(m_pFunction) {
		m_pFunction->Create();
	}
	m_pFunction2 = new CFunction2Dlg(GetParentFrame());					// ﾌｧﾝｸｼｮﾝ･ｷｰﾀﾞｲｱﾛｸﾞ2作成
	if(m_pFunction2) {
		m_pFunction2->Create();
	}
	if(m_SFuncMode==0) {												// <ﾌｧﾝｸｼｮﾝ･ｷｰﾀﾞｲｱﾛｸﾞ常駐>
		m_SFunction = 0;
		ShowFunction();
		m_SFunction2 = 1;
		HideFunction2();
	} else if(m_SFuncMode==3) {											// <ﾌｧﾝｸｼｮﾝ･ｷｰﾀﾞｲｱﾛｸﾞ2常駐>
		m_SFunction = 1;
		HideFunction();
		m_SFunction2 = 0;
		ShowFunction2();
	} else {															// <以外>
		m_SFunction = 1;
		HideFunction();
		m_SFunction2 = 1;
		HideFunction2();
	}
	m_pCurveCtrl = new CCurveCtrlDlg(GetParentFrame());					// 曲線制御ﾀﾞｲｱﾛｸﾞ作成
	if(m_pCurveCtrl) {
		m_pCurveCtrl->Create();
	}
}

/***************************************/
void CJcad3GlbView::CreateRendDlg(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞ生成
/***************************************/
{
	m_pRendDlg = new CRendDlg(GetParentFrame());						// ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞ作成
	if(m_pRendDlg) {
		m_pRendDlg->Create();
	}
	m_pRendDlg->ShowWindow(SW_SHOWNA);
	m_SRendDlg = 1;
}

/****************************************/
void CJcad3GlbView::CreateCalcDlg1(void)								//<<< 計算機ﾀﾞｲｱﾛｸﾞ1生成
/****************************************/
{
	m_pCalcDlg1 = new CCalcDlg1(GetParentFrame());						// 計算機ﾀﾞｲｱﾛｸﾞ1作成
	if(m_pCalcDlg1) {
		m_pCalcDlg1->Create();
	}
	m_pCalcDlg1->ShowWindow(SW_SHOWNA);
	m_SCalcDlg1 = 1;
}

/****************************************/
void CJcad3GlbView::CreateCalcDlg2(void)								//<<< 計算機ﾀﾞｲｱﾛｸﾞ2生成
/****************************************/
{
	m_pCalcDlg2 = new CCalcDlg2(GetParentFrame());						// 計算機ﾀﾞｲｱﾛｸﾞ2作成
	if(m_pCalcDlg2) {
		m_pCalcDlg2->Create();
	}																	// ﾊﾟﾗﾒｰﾀ設定
	m_pCalcDlg2->SetParam(m_strCalc1, m_strCalc2, m_strCalc3, m_strCalc4);
	m_pCalcDlg2->ShowWindow(SW_SHOWNA);
	m_SCalcDlg2 = 1;
}

/***********************************************************************/
void CJcad3GlbView::SetCalc(CString a, CString b, CString c, CString d)	//<<< 計算機ﾀﾞｲｱﾛｸﾞ2ﾃﾞｰﾀ設定
/***********************************************************************/
{
	m_strCalc1 = a, m_strCalc2 = b, m_strCalc3 = c, m_strCalc4 = d;
}

/************************************/
void CJcad3GlbView::EndDialogs(void)									//<<< 常駐ﾀﾞｲｱﾛｸﾞ終了
/************************************/
{
	if(m_pState) {
		delete m_pState;												// 画面状態ﾀﾞｲｱﾛｸﾞ削除
	}
	m_pState = NULL;
	if(m_pDataUse) {
		delete m_pDataUse;												// ﾃﾞｰﾀ使用率ﾀﾞｲｱﾛｸﾞ削除
	}
	m_pDataUse = NULL;
	if(m_pNumDisp) {
		delete m_pNumDisp;												// 数値表示ﾀﾞｲｱﾛｸﾞ削除
	}
	m_pNumDisp = NULL;
	if(m_pScrScroll) {
		delete m_pScrScroll;											// 画面ｽｸﾛｰﾙﾀﾞｲｱﾛｸﾞ削除
	}
	m_pScrScroll = NULL;
	if(m_pEyeChange) {
		delete m_pEyeChange;											// 視点変更ﾀﾞｲｱﾛｸﾞ削除
	}
	m_pEyeChange = NULL;
	if(m_pOifTree) {
		delete m_pOifTree;												// 立体情報ﾂｰﾙﾀﾞｲｱﾛｸﾞ削除
	}
	m_pOifTree = NULL;

	if(m_pFunction) {
		delete m_pFunction;												// ﾌｧﾝｸｼｮﾝ･ｷｰﾀﾞｲｱﾛｸﾞ削除
	}
	m_pFunction = NULL;
	if(m_pFunction2) {
		delete m_pFunction2;											// ﾌｧﾝｸｼｮﾝ･ｷｰﾀﾞｲｱﾛｸﾞ2削除
	}
	m_pFunction2 = NULL;
	if(m_pCurveCtrl) {
		delete m_pCurveCtrl;											// 曲線制御ﾀﾞｲｱﾛｸﾞ削除
	}
	m_pCurveCtrl = NULL;
}

/************************************/
void CJcad3GlbView::EndRendDlg(void)									//<<< ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞ終了
/************************************/
{
	if(m_pRendDlg) {
		delete m_pRendDlg;												// ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞ削除
	}
	m_pRendDlg = NULL;
	m_SRendDlg = 0;
}

/*************************************/
void CJcad3GlbView::EndCalcDlg1(void)									//<<< 計算機ﾀﾞｲｱﾛｸﾞ1終了
/*************************************/
{
	if(m_pCalcDlg1) {
		delete m_pCalcDlg1;												// 計算機ﾀﾞｲｱﾛｸﾞ1削除
	}
	m_pCalcDlg1 = NULL;
	m_SCalcDlg1 = 0;
}

/*************************************/
void CJcad3GlbView::EndCalcDlg2(void)									//<<< 計算機ﾀﾞｲｱﾛｸﾞ2終了
/*************************************/
{
	if(m_pCalcDlg2) {
		delete m_pCalcDlg2;												// 計算機ﾀﾞｲｱﾛｸﾞ2削除
	}
	m_pCalcDlg2 = NULL;
	m_SCalcDlg2 = 0;
}

/**************************************/
void CJcad3GlbView::ShowStateDlg(void)									//<<< 画面状態表示
/**************************************/
{
	m_SState = m_SState?0:1;
	m_pState->ShowWindow(m_SState?SW_SHOWNA:SW_HIDE);
}

/*************************************/
void CJcad3GlbView::ShowDataUse(void)									//<<< ﾃﾞｰﾀ使用率表示
/*************************************/
{
	m_SDataUse = m_SDataUse?0:1;
	m_pDataUse->ShowWindow(m_SDataUse?SW_SHOWNA:SW_HIDE);
}

/****************************************/
void CJcad3GlbView::SetNumDisp(int disp)								//<<< 数値表示ﾀﾞｲｱﾛｸﾞ表示
/****************************************/
{
	if(disp!=2) {
		m_SNumDisp = disp;
	}
	m_pNumDisp->SetNumDispWnd(disp);
}

/****************************************/
void CJcad3GlbView::ShowScrollTool(void)								//<<< 画面ｽｸﾛｰﾙﾂｰﾙ表示
/****************************************/
{
	m_SScrScroll = m_SScrScroll?0:1;
	m_pScrScroll->ShowWindow(m_SScrScroll?SW_SHOWNA:SW_HIDE);
}

/***************************************/
void CJcad3GlbView::ShowEyeChange(void)									//<<< 視点変更ﾂｰﾙ表示
/***************************************/
{
	m_SEyeChange = m_SEyeChange?0:1;
	m_pEyeChange->ShowWindow(m_SEyeChange?SW_SHOWNA:SW_HIDE);
}

/*************************************/
void CJcad3GlbView::ShowOifTree(void)									//<<< 立体情報ﾂｰﾙ表示
/*************************************/
{
	m_SOifTree = m_SOifTree?0:1;
	m_pOifTree->ShowWindow(m_SOifTree?SW_SHOWNA:SW_HIDE);
}

/*************************************/
void CJcad3GlbView::InitOifItem(void)									//<<< 立体構成情報ｱｲﾃﾑ初期化
/*************************************/
{
	m_pOifTree->InitOifItem();
}

/********************************************/
void CJcad3GlbView::SetOifItem(OIFTYPE *oif)							//<<< 立体構成情報ｱｲﾃﾑ設定
/********************************************/
{
	m_pOifTree->SetOifItem(oif);
}

/********************************************/
void CJcad3GlbView::DelOifItem(OIFTYPE *oif)							//<<< 立体構成情報ｱｲﾃﾑ削除
/********************************************/
{
	m_pOifTree->DelOifItem(oif);
}

/***********************************************/
void CJcad3GlbView::UpdateOifItem(OIFTYPE *oif)							//<<< 立体構成情報ｱｲﾃﾑ更新
/***********************************************/
{
	m_pOifTree->UpdateOifItem(oif);
}

/***********************************************/
void CJcad3GlbView::SelectOifItem(OIFTYPE *oif)							//<<< 立体構成情報ｱｲﾃﾑ選択
/***********************************************/
{
	m_pOifTree->SelectOifItem(oif);
}

/******************************************************/
void CJcad3GlbView::UpdateDspFlg(int flg, OBJTYPE *op)					//<<< 立体構成情報表示状態更新
/******************************************************/
{
	m_pOifTree->UpdateDspFlg(flg, op);
}

/******************************************************/
void CJcad3GlbView::UpdateEdtFlg(int flg, OBJTYPE *op)					//<<< 立体構成情報選択状態更新
/******************************************************/
{
	m_pOifTree->UpdateEdtFlg(flg, op);
}

/**************************************/
void CJcad3GlbView::ShowFunction(void)									//<<< ﾌｧﾝｸｼｮﾝ･ｷｰ表示
/**************************************/
{
	if(!m_SFunction) {
		m_SFunction = 1;
		m_pFunction->ShowWindow(SW_SHOWNA);
	}
}

/**************************************/
void CJcad3GlbView::HideFunction(void)									//<<< ﾌｧﾝｸｼｮﾝ･ｷｰ消去
/**************************************/
{
	if(m_SFunction) {
		m_SFunction = 0;
		m_pFunction->ShowWindow(SW_HIDE);
	}
}

/***************************************/
void CJcad3GlbView::MessageOut(int nID)									//<<< ﾌｧﾝｸｼｮﾝ･ﾒｯｾｰｼﾞ出力
/***************************************/
{
	if(m_SFuncMode==0||m_SFuncMode==1) {								// <詳細:SPACEなし>
		m_pFunction->MessageOut(nID, m_CChar&0x4?1:0);					// ﾒｯｾｰｼﾞ出力
	} else if(m_SFuncMode==2) {											// <詳細:SPACEあり>
		if(m_CChar&0x4) {
			m_pFunction->MessageOut(nID, 1);							// ﾒｯｾｰｼﾞ出力
		}
	} else if(m_SFuncMode==3||m_SFuncMode==4) {							// <簡易:SPACEなし>
		if(IsFuncDsp2()) {												// ﾀﾞｲｱﾛｸﾞ表示指示中
			ShowFunction2();											// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ2表示
		}
	} else if(m_SFuncMode==5) {											// <簡易:SPACEあり>
		if(m_CChar&0x4) {
			if(IsFuncDsp2()) {											// ﾀﾞｲｱﾛｸﾞ表示指示中
				ShowFunction2();										// ﾌｧﾝｸｼｮﾝﾀﾞｲｱﾛｸﾞ2表示
			}
		}
	}
}

/***************************************/
void CJcad3GlbView::ShowFunction2(void)									//<<< ﾌｧﾝｸｼｮﾝ･ｷｰ2表示
/***************************************/
{
	if(!m_SFunction2) {
		m_SFunction2 = 1;
		m_pFunction2->ShowWindow(SW_SHOWNA);
	}
}

/***************************************/
void CJcad3GlbView::HideFunction2(void)									//<<< ﾌｧﾝｸｼｮﾝ･ｷｰ2消去
/***************************************/
{
	if(m_SFunction2) {
		m_SFunction2 = 0;
		m_pFunction2->ShowWindow(SW_HIDE);
	}
}

/***************************************/
void CJcad3GlbView::ShowCurveCtrl(void)									//<<< 曲線制御ﾀﾞｲｱﾛｸﾞ表示
/***************************************/
{
	m_pCurveCtrl->ShowWindow(SW_SHOWNA);
}

/***************************************/
void CJcad3GlbView::HideCurveCtrl(void)									//<<< 曲線制御ﾀﾞｲｱﾛｸﾞ消去
/***************************************/
{
	m_pCurveCtrl->ShowWindow(SW_HIDE);
}

/****************************************/
void CJcad3GlbView::ClearCurveData(void)								//<<< 曲線制御ﾃﾞｰﾀｸﾘｱ
/****************************************/
{
	m_pCurveCtrl->ClearData();
	m_CurveAFlg = 0;
	m_CurveRFlg = 0;
}

/**********************************************************/
void CJcad3GlbView::GetCurveData(double* agl, double* rds)				//<<< 曲線制御ﾃﾞｰﾀ取得
/**********************************************************/
{
	*agl = m_CurveAngle;
	*rds = m_CurveRadius;
}

/******************************************************/
void CJcad3GlbView::SetCurveAngle(int flg, double agl)					//<<< 曲線制御角度設定
/******************************************************/
{
	m_CurveAFlg = flg;
	m_CurveAngle = agl;
}

/*******************************************************/
void CJcad3GlbView::SetCurveRadius(int flg, double rds)					//<<< 曲線制御Ｒ設定
/*******************************************************/
{
	m_CurveRFlg = flg;
	m_CurveRadius = rds;
}

/*************************************/
void CJcad3GlbView::ChangePrint(void)									//<<< ﾌﾟﾘﾝﾀ出力切り替え
/*************************************/
{
	m_PrtKind = 1;
	CView::OnFilePrint();
}
