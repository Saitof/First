/////////////////////////////////////////////////////////////////////////////
// ラジオボタンダイアログクラス定義
// BottunDlg.cpp
//---------------------------------------------------------------------------
// LastEdit: 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "ButtonDlg.h"

///////////////////////////////////////////////////////////////////////////////
// CDispmodeDlg ダイアログ

BEGIN_MESSAGE_MAP(CDispmodeDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO6, OnCheckBtn)
END_MESSAGE_MAP()

/*******************************************************************************/
CDispmodeDlg::CDispmodeDlg(CWnd* pParent) : CDialog(CDispmodeDlg::IDD, pParent)
/*******************************************************************************/
{
	m_DspMode = 0;
}

/*****************************************************/
void CDispmodeDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_DspMode);
}

/***************************************/
BOOL CDispmodeDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/***************************************/
{
	m_DspMode = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*************************************/
void CDispmodeDlg::SetValue(int prm1)									//<<< ﾊﾟﾗﾒｰﾀ設定
/*************************************/
{
	m_DspMode = prm1;
}

/**************************************/
void CDispmodeDlg::GetValue(int* prm1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/**************************************/
{
	*prm1 = m_DspMode;
}

///////////////////////////////////////////////////////////////////////////////
// CScreenDlg ダイアログ

BEGIN_MESSAGE_MAP(CScreenDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO5, OnCheckBtn)
END_MESSAGE_MAP()

/*************************************************************************/
CScreenDlg::CScreenDlg(CWnd* pParent) : CDialog(CScreenDlg::IDD, pParent)
/*************************************************************************/
{
	m_ScreenNo = 0;
}

/***************************************************/
void CScreenDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_ScreenNo);
}

/*************************************/
BOOL CScreenDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/*************************************/
{
	m_ScreenNo = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************/
void CScreenDlg::GetValue(int* prm1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************/
{
	*prm1 = m_ScreenNo;
}

///////////////////////////////////////////////////////////////////////////////
// CEyesetDlg ダイアログ

BEGIN_MESSAGE_MAP(CEyesetDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO6, OnCheckBtn)
END_MESSAGE_MAP()

/*************************************************************************/
CEyesetDlg::CEyesetDlg(CWnd* pParent) : CDialog(CEyesetDlg::IDD, pParent)
/*************************************************************************/
{
	m_Eyeset = 0;
}

/***************************************************/
void CEyesetDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Eyeset);
}

/*************************************/
BOOL CEyesetDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/*************************************/
{
	m_Eyeset = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************/
void CEyesetDlg::GetValue(int* prm1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************/
{
	*prm1 = m_Eyeset;
}

///////////////////////////////////////////////////////////////////////////////
// CBasesetDlg ダイアログ

BEGIN_MESSAGE_MAP(CBasesetDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************/
CBasesetDlg::CBasesetDlg(CWnd* pParent) : CDialog(CBasesetDlg::IDD, pParent)
/****************************************************************************/
{
	m_Baseset = 0;
}

/****************************************************/
void CBasesetDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Baseset);
}

/**************************************/
BOOL CBasesetDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/**************************************/
{
	m_Baseset = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*************************************/
void CBasesetDlg::GetValue(int* prm1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/*************************************/
{
	*prm1 = m_Baseset;
}

///////////////////////////////////////////////////////////////////////////////
// CGlbDlg ダイアログ

BEGIN_MESSAGE_MAP(CGlbDlg, CDialog)										// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO5, OnCheckBtn)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
END_MESSAGE_MAP()

/**************************************************************************/
CGlbDlg::CGlbDlg(int kind, CWnd* pParent) : CDialog(CGlbDlg::IDD, pParent)
/**************************************************************************/
{
	m_Kind = kind;
	m_GlbNo = 0;
	m_ChangeSize = 0;
	m_RingSize = 12.0;
}

/************************************************/
void CGlbDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_GlbNo);
	DDX_Check(pDX, IDC_CHECK1, m_ChangeSize);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Text(pDX, IDC_EDIT1, m_RingSize);
	DDV_MinMaxDouble(pDX, m_RingSize, 0.1, 1000.0);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
}

/********************************/
BOOL CGlbDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/********************************/										//    ﾀﾞｲｱﾛｸﾞの初期化
{
	CString str;

	CDialog::OnInitDialog();
	if(!m_Kind) {														// <リング号数指定>
		str.LoadString(IDS_STR26);										// 項目名取得(ﾘﾝｸﾞ号数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR28);										// 単位取得(号)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 単位表示
	} else {															// <リング直径指定>
		str.LoadString(IDS_STR27);										// 項目名取得(ﾘﾝｸﾞ直径)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR5);										// 単位取得(mm)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 単位表示
	}
	PermissionCheck();													// 入力許可ﾁｪｯｸ
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
BOOL CGlbDlg::OnCheckBtn(UINT nID)										//<<< ﾎﾞﾀﾝ入力確認処理
/**********************************/
{
	m_GlbNo = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/***************************/
void CGlbDlg::OnCheck(void)												//<<< ﾎﾞﾀﾝﾁｪｯｸ
/***************************/
{
	m_ChangeSize = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;	// 釦のON/OFF状態を取得
	PermissionCheck();													// 入力許可ﾁｪｯｸ
}

/***********************************/
void CGlbDlg::PermissionCheck(void)										//<<< 入力許可ﾁｪｯｸ
/***********************************/
{
	m_Static1.EnableWindow(m_ChangeSize);
	m_Edit1.EnableWindow(m_ChangeSize);
	m_Static2.EnableWindow(m_ChangeSize);
}

/*************************************************/
void CGlbDlg::SetValue(int p1, int p2, double p3)						//<<< ﾊﾟﾗﾒｰﾀ設定
/*************************************************/
{
	m_GlbNo = p1;
	m_ChangeSize = p2;
	m_RingSize = p3;
}

/****************************************************/
void CGlbDlg::GetValue(int* p1, int* p2, double* p3)					//<<< ﾊﾟﾗﾒｰﾀ取得
/****************************************************/
{
	*p1 = m_GlbNo;
	*p2 = m_ChangeSize;
	*p3 = m_RingSize;
}

///////////////////////////////////////////////////////////////////////////////
// CDispTmpDlg ダイアログ

BEGIN_MESSAGE_MAP(CDispTmpDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO9, OnCheckBtn)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck)
END_MESSAGE_MAP()

/**************************************************************************************/
CDispTmpDlg::CDispTmpDlg(int kind, CWnd* pParent) : CDialog(CDispTmpDlg::IDD, pParent)
/**************************************************************************************/
{
	m_Kind = kind;
	m_DspTmp = 0;
	m_ChangeSize = 0;
	m_RingSize = 12.0;
}

/****************************************************/
void CDispTmpDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_DspTmp);
	DDX_Check(pDX, IDC_CHECK1, m_ChangeSize);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Text(pDX, IDC_EDIT1, m_RingSize);
	DDV_MinMaxDouble(pDX, m_RingSize, 0.1, 1000.0);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
}

/************************************/
BOOL CDispTmpDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/									//    ﾀﾞｲｱﾛｸﾞの初期化
{
	CString str;

	CDialog::OnInitDialog();
	if(!m_Kind) {														// <リング号数指定>
		str.LoadString(IDS_STR26);										// 項目名取得(ﾘﾝｸﾞ号数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR28);										// 単位取得(号)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 単位表示
	} else {															// <リング直径指定>
		str.LoadString(IDS_STR27);										// 項目名取得(ﾘﾝｸﾞ直径)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR5);										// 単位取得(mm)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 単位表示
	}
	PermissionCheck();													// 入力許可ﾁｪｯｸ
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**************************************/
BOOL CDispTmpDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/**************************************/
{
	m_DspTmp = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*******************************/
void CDispTmpDlg::OnCheck(void)											//<<< ﾎﾞﾀﾝﾁｪｯｸ
/*******************************/
{
	m_ChangeSize = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;	// 釦のON/OFF状態を取得
	PermissionCheck();													// 入力許可ﾁｪｯｸ
}

/***************************************/
void CDispTmpDlg::PermissionCheck(void)									//<<< 入力許可ﾁｪｯｸ
/***************************************/
{
	m_Static1.EnableWindow(m_ChangeSize);
	m_Edit1.EnableWindow(m_ChangeSize);
	m_Static2.EnableWindow(m_ChangeSize);
}

/*****************************************************/
void CDispTmpDlg::SetValue(int p1, int p2, double p3)					//<<< ﾊﾟﾗﾒｰﾀ設定
/*****************************************************/
{
	m_DspTmp = p1;
	m_ChangeSize = p2;
	m_RingSize = p3;
}

/********************************************************/
void CDispTmpDlg::GetValue(int* p1, int* p2, double* p3)				//<<< ﾊﾟﾗﾒｰﾀ取得
/********************************************************/
{
	*p1 = m_DspTmp;
	*p2 = m_ChangeSize;
	*p3 = m_RingSize;
}

///////////////////////////////////////////////////////////////////////////////
// CColorDlg ダイアログ

BEGIN_MESSAGE_MAP(CColorDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO18, OnCheckBtn)
END_MESSAGE_MAP()

/**********************************************************************/
CColorDlg::CColorDlg(CWnd* pParent) : CDialog(CColorDlg::IDD, pParent)
/**********************************************************************/
{
	m_Color = 0;
}

/**************************************************/
void CColorDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Color);
}

/************************************/
BOOL CColorDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/************************************/
{
	m_Color = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/**********************************/
void CColorDlg::SetValue(int prm1)										//<<< ﾊﾟﾗﾒｰﾀ設定
/**********************************/
{
	m_Color = prm1;
}

/***********************************/
void CColorDlg::GetValue(int* prm1)										//<<< ﾊﾟﾗﾒｰﾀ取得
/***********************************/
{
	*prm1 = m_Color;
}

///////////////////////////////////////////////////////////////////////////////
// CCurveDlg ダイアログ

BEGIN_MESSAGE_MAP(CCurveDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnCheckBtn)
END_MESSAGE_MAP()

/**********************************************************************/
CCurveDlg::CCurveDlg(CWnd* pParent) : CDialog(CCurveDlg::IDD, pParent)
/**********************************************************************/
{
	m_Curve = 0;
}

/**************************************************/
void CCurveDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Curve);
}

/************************************/
BOOL CCurveDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/************************************/
{
	m_Curve = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/***********************************/
void CCurveDlg::GetValue(int* prm1)										//<<< ﾊﾟﾗﾒｰﾀ取得
/***********************************/
{
	*prm1 = m_Curve;
}

///////////////////////////////////////////////////////////////////////////////
// CPrimitiveDlg ダイアログ

BEGIN_MESSAGE_MAP(CPrimitiveDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO6, OnCheckBtn)
END_MESSAGE_MAP()

/**********************************************************************************/
CPrimitiveDlg::CPrimitiveDlg(CWnd* pParent) : CDialog(CPrimitiveDlg::IDD, pParent)
/**********************************************************************************/
{
	m_Primitive = 0;
}

/******************************************************/
void CPrimitiveDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Primitive);
}

/****************************************/
BOOL CPrimitiveDlg::OnCheckBtn(UINT nID)								//<<< ﾎﾞﾀﾝ入力確認処理
/****************************************/
{
	m_Primitive = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/***************************************/
void CPrimitiveDlg::GetValue(int* prm1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/***************************************/
{
	*prm1 = m_Primitive;
}

///////////////////////////////////////////////////////////////////////////////
// CBooleanDlg ダイアログ

BEGIN_MESSAGE_MAP(CBooleanDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO3, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************/
CBooleanDlg::CBooleanDlg(CWnd* pParent) : CDialog(CBooleanDlg::IDD, pParent)
/****************************************************************************/
{
	m_Boolean = 0;
	m_DeleteB = 0;
}

/****************************************************/
void CBooleanDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Boolean);
	DDX_Check(pDX, IDC_CHECK1, m_DeleteB);
}

/**************************************/
BOOL CBooleanDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/**************************************/
{
	m_Boolean = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************************/
void CBooleanDlg::GetValue(int* prm1, int* prm2)						//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************************/
{
	*prm1 = m_Boolean;
	*prm2 = (!m_Boolean) ? m_DeleteB : 0;
}

///////////////////////////////////////////////////////////////////////////////
// CGemsetDlg ダイアログ

BEGIN_MESSAGE_MAP(CGemsetDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO3, OnCheckBtn)
END_MESSAGE_MAP()

/*************************************************************************/
CGemsetDlg::CGemsetDlg(CWnd* pParent) : CDialog(CGemsetDlg::IDD, pParent)
/*************************************************************************/
{
	m_GemSet = 0;
}

/***************************************************/
void CGemsetDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_GemSet);
}

/*************************************/
BOOL CGemsetDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/*************************************/
{
	m_GemSet = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/************************************/
void CGemsetDlg::GetValue(int* prm1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************/
{
	*prm1 = m_GemSet;
}

///////////////////////////////////////////////////////////////////////////////
// CGemDlg ダイアログ

BEGIN_MESSAGE_MAP(CGemDlg, CDialog)										// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO11, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************/
CGemDlg::CGemDlg(CWnd* pParent) : CDialog(CGemDlg::IDD, pParent)
/****************************************************************/
{
	m_Gem = 0;
}

/************************************************/
void CGemDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Gem);
}

/**********************************/
BOOL CGemDlg::OnCheckBtn(UINT nID)										//<<< ﾎﾞﾀﾝ入力確認処理
/**********************************/
{
	m_Gem = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*********************************/
void CGemDlg::GetValue(int* prm1)										//<<< ﾊﾟﾗﾒｰﾀ取得
/*********************************/
{
	*prm1 = m_Gem;
}

///////////////////////////////////////////////////////////////////////////////
// CRevolveDlg ダイアログ

BEGIN_MESSAGE_MAP(CRevolveDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************/
CRevolveDlg::CRevolveDlg(CWnd* pParent) : CDialog(CRevolveDlg::IDD, pParent)
/****************************************************************************/
{
	m_Revolve = 0;
}

/****************************************************/
void CRevolveDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Revolve);
}

/**************************************/
BOOL CRevolveDlg::OnCheckBtn(UINT nID)									//<<< ﾎﾞﾀﾝ入力確認処理
/**************************************/
{
	m_Revolve = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/*************************************/
void CRevolveDlg::GetValue(int* prm1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/*************************************/
{
	*prm1 = m_Revolve;
}

///////////////////////////////////////////////////////////////////////////////
// CSetFuncModeDlg ダイアログ

BEGIN_MESSAGE_MAP(CSetFuncModeDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO7, OnCheckBtn)
END_MESSAGE_MAP()

/****************************************************************************************/
CSetFuncModeDlg::CSetFuncModeDlg(CWnd* pParent) : CDialog(CSetFuncModeDlg::IDD, pParent)
/****************************************************************************************/
{
	m_FuncMode = 0;
}

/********************************************************/
void CSetFuncModeDlg::DoDataExchange(CDataExchange* pDX)				//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/********************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_FuncMode);
}

/******************************************/
BOOL CSetFuncModeDlg::OnCheckBtn(UINT nID)								//<<< ﾎﾞﾀﾝ入力確認処理
/******************************************/
{
	m_FuncMode = nID-IDC_RADIO1;
	CDialog::OnOK();
	return TRUE;
}

/****************************************/
void CSetFuncModeDlg::SetValue(int prm1)								//<<< ﾊﾟﾗﾒｰﾀ設定
/****************************************/
{
	m_FuncMode = prm1;
}

/*****************************************/
void CSetFuncModeDlg::GetValue(int* prm1)								//<<< ﾊﾟﾗﾒｰﾀ取得
/*****************************************/
{
	*prm1 = m_FuncMode;
}
