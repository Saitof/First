/////////////////////////////////////////////////////////////////////////////
// 複数項目設定ダイアログクラス定義
// InputsDlg.cpp
//---------------------------------------------------------------------------
// LastEdit: 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "FileDlg.h"
#include "InputsDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "TrueTypeFont.h"
#define   TIMERID    1

///////////////////////////////////////////////////////////////////////////////
// CPrtParamDlg ダイアログ

BEGIN_MESSAGE_MAP(CPrtParamDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnPrintMode)
	ON_COMMAND_EX_RANGE(IDC_RADIO6, IDC_RADIO7, OnPrintOut)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, OnFileSave)
END_MESSAGE_MAP()

/*******************************************************************************/
CPrtParamDlg::CPrtParamDlg(CWnd* pParent) : CDialog(CPrtParamDlg::IDD, pParent)
/*******************************************************************************/
{
	m_PMode  = 0;
	m_PColor = 0;
	m_POut   = 0;
	m_POri   = 0;
	m_PPos   = 0;
	m_PSize  = 0;
	m_Check1 = FALSE;
	m_Check2 = FALSE;
	m_sPicPath = _T("");
	m_sPicFile = View->GetDocument()->GetTitle();
	int i = m_sPicFile.ReverseFind('.');
	if(i!=-1) {
		m_sPicFile = m_sPicFile.Left(i);
	}
	m_sPicFile = m_sPicFile + ".jpg";
}

/*****************************************************/
void CPrtParamDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_PMode);
	DDX_Radio(pDX, IDC_RADIO3, m_PColor);
	DDX_Radio(pDX, IDC_RADIO6, m_POut);
	DDX_Radio(pDX, IDC_RADIO8, m_POri);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_RADIO8, m_RButton1);
	DDX_Control(pDX, IDC_RADIO9, m_RButton2);
	DDX_Radio(pDX, IDC_RADIO10, m_PPos);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_RADIO10, m_RButton3);
	DDX_Control(pDX, IDC_RADIO11, m_RButton4);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_CHECK1, m_CCheck1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK2, m_CCheck2);
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_SPIN1, m_Spin);
	DDX_Text(pDX, IDC_EDIT1, m_PSize);
	DDV_MinMaxUInt(pDX, m_PSize, 10, 300);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC4, m_Static4);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
}

/*************************************/
BOOL CPrtParamDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/*************************************/
{
	CDialog::OnInitDialog();
	m_Spin.SetRange(10, 300);
	m_Static1.EnableWindow(!m_POut);
	m_RButton1.EnableWindow(!m_POut);
	m_RButton2.EnableWindow(!m_POut);
	m_Static2.EnableWindow(!m_POut);
	m_RButton3.EnableWindow(!m_POut);
	m_RButton4.EnableWindow(!m_POut);
	m_CCheck1.EnableWindow((!m_PMode)&&(!m_POut));
	m_CCheck2.EnableWindow((!m_POut)&&(!m_Check1));
	m_Static3.EnableWindow((!m_POut)&&m_Check2);
	m_Spin.EnableWindow((!m_POut)&&m_Check2);
	m_Edit1.EnableWindow((!m_POut)&&m_Check2);
	m_Static4.EnableWindow((!m_POut)&&m_Check2);
	m_Button1.EnableWindow(m_POut);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/****************************************/
BOOL CPrtParamDlg::OnPrintMode(UINT nID)								//<<< 出力範囲ﾎﾞﾀﾝ押下処理
/****************************************/
{
	m_PMode = nID-IDC_RADIO1;
	if(m_PMode) {
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);				// 釦OFF
		m_Check1 = FALSE;
	}
	m_CCheck1.EnableWindow((!m_PMode)&&(!m_POut));
	m_CCheck2.EnableWindow((!m_POut)&&(!m_Check1));
	return TRUE;
}

/***************************************/
BOOL CPrtParamDlg::OnPrintOut(UINT nID)									//<<< 出力先ﾎﾞﾀﾝ押下処理
/***************************************/
{
	m_POut = nID-IDC_RADIO6;
	m_Static1.EnableWindow(!m_POut);
	m_RButton1.EnableWindow(!m_POut);
	m_RButton2.EnableWindow(!m_POut);
	m_Static2.EnableWindow(!m_POut);
	m_RButton3.EnableWindow(!m_POut);
	m_RButton4.EnableWindow(!m_POut);
	if(m_POut) {
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);				// 釦OFF
		m_Check1 = FALSE;
	}
	m_CCheck1.EnableWindow((!m_PMode)&&(!m_POut));
	if(m_POut) {
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);				// 釦OFF
		m_Check2 = FALSE;
	}
	m_CCheck2.EnableWindow((!m_POut)&&(!m_Check1));
	m_Static3.EnableWindow((!m_POut)&&m_Check2);
	m_Spin.EnableWindow((!m_POut)&&m_Check2);
	m_Edit1.EnableWindow((!m_POut)&&m_Check2);
	m_Static4.EnableWindow((!m_POut)&&m_Check2);
	m_Button1.EnableWindow(m_POut);
	return TRUE;
}

/*********************************/
void CPrtParamDlg::OnCheck1(void)										//<<< ﾎﾞﾀﾝ1ﾁｪｯｸ
/*********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// 釦のON/OFF状態を取得
	if(m_Check1) {
		((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);				// 釦OFF
		m_Check2 = FALSE;
	}
	m_CCheck2.EnableWindow((!m_POut)&&(!m_Check1));
	m_Static3.EnableWindow((!m_POut)&&m_Check2);
	m_Spin.EnableWindow((!m_POut)&&m_Check2);
	m_Edit1.EnableWindow((!m_POut)&&m_Check2);
	m_Static4.EnableWindow((!m_POut)&&m_Check2);
}

/*********************************/
void CPrtParamDlg::OnCheck2(void)										//<<< ﾎﾞﾀﾝ2ﾁｪｯｸ
/*********************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// 釦のON/OFF状態を取得
	m_Static3.EnableWindow((!m_POut)&&m_Check2);
	m_Spin.EnableWindow((!m_POut)&&m_Check2);
	m_Edit1.EnableWindow((!m_POut)&&m_Check2);
	m_Static4.EnableWindow((!m_POut)&&m_Check2);
}

/***********************************/
void CPrtParamDlg::OnFileSave(void)										//<<< ﾌｧｲﾙ保存ﾎﾞﾀﾝ処理
/***********************************/
{
	CString ext="jpg", wex, file, filter;
	DWORD   flg;
	int     i;

	if(!CheckFilePath(0, m_sPicPath)) {									// ﾊﾟｽ確認
		m_sPicPath = GetAppPath(0);
	}
	flg = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;							// ｢JPEG/BMPﾌｧｲﾙを保存｣
	filter.LoadString(IDS_FILE_FILTER102);
	file = m_sPicFile;
	wex = file.Right(4);
	if(!wex.CompareNoCase(".jpg")||!wex.CompareNoCase(".bmp")) {		// <有効な拡張子>
		i = file.ReverseFind('.');
		file = file.Left(i);											// 拡張子を削除
	}
	CFileDlg dlg(FALSE, ext, file, flg, filter);						// ﾀﾞｲｱﾛｸﾞ表示
	dlg.m_ofn.lpstrInitialDir = m_sPicPath;
	if(dlg.DoModal()==IDOK) {
		m_sPicPath = dlg.GetPathName();									// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		m_sPicFile = dlg.GetFileTitle();								// ﾌｧｲﾙ名取得
		ext = dlg.GetFileExt();											// ﾌｧｲﾙの拡張子取得
		m_sPicFile = m_sPicFile + "." + ext;
		i = m_sPicPath.ReverseFind('\\');
		m_sPicPath = m_sPicPath.Left(i+1);								// ﾊﾟｽ保存
	}
}

/***********************************************************/
void CPrtParamDlg::SetValue(int p1, int p2, int p3, int p4,
                            int p5, int p6, CString p7)					//<<< ﾊﾟﾗﾒｰﾀ設定
/***********************************************************/
{
	m_PMode = p1, m_PColor = p2, m_POut = p3, m_POri = p4;
	m_PPos  = p5, m_PSize  = p6, m_sPicPath = p7;
}

/***************************************************************/
void CPrtParamDlg::GetValue(int* p1, int* p2, int* p3, int* p4,
                            int* p5, int* p6, CString* p7,
                            int* p8, int* p9, CString* p10)				//<<< ﾊﾟﾗﾒｰﾀ取得
/***************************************************************/
{
	*p1 = m_PMode,  *p2 = m_PColor, *p3 = m_POut, *p4 = m_POri;
	*p5 = m_PPos,   *p6 = m_PSize,  *p7 = m_sPicPath;
	*p8 = m_Check1, *p9 = m_Check2, *p10 = m_sPicPath+m_sPicFile;
}

///////////////////////////////////////////////////////////////////////////////
// CPrtParam2Dlg ダイアログ

BEGIN_MESSAGE_MAP(CPrtParam2Dlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO2, OnPrintOut)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, OnFileSave)
END_MESSAGE_MAP()

/**********************************************************************************/
CPrtParam2Dlg::CPrtParam2Dlg(CWnd* pParent) : CDialog(CPrtParam2Dlg::IDD, pParent)
/**********************************************************************************/
{
	m_POut  = 0;
	m_POri  = 0;
	m_PPos  = 0;
	m_PSize = 0;
	m_Check1   = FALSE;
	m_sPicPath = _T("");
	m_sPicFile = View->GetDocument()->GetTitle();
	int i = m_sPicFile.ReverseFind('.');
	if(i!=-1) {
		m_sPicFile = m_sPicFile.Left(i);
	}
	m_sPicFile = m_sPicFile + ".jpg";
}

/******************************************************/
void CPrtParam2Dlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_POut);
	DDX_Radio(pDX, IDC_RADIO3, m_POri);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_RADIO3, m_RButton1);
	DDX_Control(pDX, IDC_RADIO4, m_RButton2);
	DDX_Radio(pDX, IDC_RADIO5, m_PPos);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_RADIO5, m_RButton3);
	DDX_Control(pDX, IDC_RADIO6, m_RButton4);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_CHECK1, m_CCheck1);
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_SPIN1, m_Spin);
	DDX_Text(pDX, IDC_EDIT1, m_PSize);
	DDV_MinMaxUInt(pDX, m_PSize, 30, 250);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC4, m_Static4);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
}

/**************************************/
BOOL CPrtParam2Dlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**************************************/
{
	CDialog::OnInitDialog();
	m_Spin.SetRange(30, 250);
	m_Static1.EnableWindow(!m_POut);
	m_RButton1.EnableWindow(!m_POut);
	m_RButton2.EnableWindow(!m_POut);
	m_Static2.EnableWindow(!m_POut);
	m_RButton3.EnableWindow(!m_POut);
	m_RButton4.EnableWindow(!m_POut);
	m_CCheck1.EnableWindow(!m_POut);
	m_Static3.EnableWindow((!m_POut)&&m_Check1);
	m_Spin.EnableWindow((!m_POut)&&m_Check1);
	m_Edit1.EnableWindow((!m_POut)&&m_Check1);
	m_Static4.EnableWindow((!m_POut)&&m_Check1);
	m_Button1.EnableWindow(m_POut);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/****************************************/
BOOL CPrtParam2Dlg::OnPrintOut(UINT nID)								//<<< 出力先ﾎﾞﾀﾝ押下処理
/****************************************/
{
	m_POut = nID-IDC_RADIO1;
	m_Static1.EnableWindow(!m_POut);
	m_RButton1.EnableWindow(!m_POut);
	m_RButton2.EnableWindow(!m_POut);
	m_Static2.EnableWindow(!m_POut);
	m_RButton3.EnableWindow(!m_POut);
	m_RButton4.EnableWindow(!m_POut);
	if(m_POut) {
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(0);				// 釦OFF
		m_Check1 = FALSE;
	}
	m_CCheck1.EnableWindow(!m_POut);
	m_Static3.EnableWindow((!m_POut)&&m_Check1);
	m_Spin.EnableWindow((!m_POut)&&m_Check1);
	m_Edit1.EnableWindow((!m_POut)&&m_Check1);
	m_Static4.EnableWindow((!m_POut)&&m_Check1);
	m_Button1.EnableWindow(m_POut);
	return TRUE;
}

/**********************************/
void CPrtParam2Dlg::OnCheck1(void)										//<<< ﾎﾞﾀﾝ1ﾁｪｯｸ
/**********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// 釦のON/OFF状態を取得
	m_Static3.EnableWindow((!m_POut)&&m_Check1);
	m_Spin.EnableWindow((!m_POut)&&m_Check1);
	m_Edit1.EnableWindow((!m_POut)&&m_Check1);
	m_Static4.EnableWindow((!m_POut)&&m_Check1);
}

/************************************/
void CPrtParam2Dlg::OnFileSave(void)									//<<< ﾌｧｲﾙ保存ﾎﾞﾀﾝ処理
/************************************/
{
	CString ext="jpg", wex, file, filter;
	DWORD   flg;
	int     i;

	if(!CheckFilePath(0, m_sPicPath)) {									// ﾊﾟｽ確認
		m_sPicPath = GetAppPath(0);
	}
	flg = OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY;							// ｢JPEG/BMPﾌｧｲﾙを保存｣
	filter.LoadString(IDS_FILE_FILTER102);
	file = m_sPicFile;
	wex = file.Right(4);
	if(!wex.CompareNoCase(".jpg")||!wex.CompareNoCase(".bmp")) {		// <有効な拡張子>
		i = file.ReverseFind('.');
		file = file.Left(i);											// 拡張子を削除
	}
	CFileDlg dlg(FALSE, ext, file, flg, filter);						// ﾀﾞｲｱﾛｸﾞ表示
	dlg.m_ofn.lpstrInitialDir = m_sPicPath;
	if(dlg.DoModal()==IDOK) {
		m_sPicPath = dlg.GetPathName();									// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		m_sPicFile = dlg.GetFileTitle();								// ﾌｧｲﾙ名取得
		ext = dlg.GetFileExt();											// ﾌｧｲﾙの拡張子取得
		m_sPicFile = m_sPicFile + "." + ext;
		i = m_sPicPath.ReverseFind('\\');
		m_sPicPath = m_sPicPath.Left(i+1);								// ﾊﾟｽ保存
	}
}

/************************************************************************/
void CPrtParam2Dlg::SetValue(int p1, int p2, int p3, int p4, CString p5)//<<< ﾊﾟﾗﾒｰﾀ設定
/************************************************************************/
{
	m_POut = p1, m_POri = p2, m_PPos = p3, m_PSize = p4, m_sPicPath = p5;
}

/****************************************************************/
void CPrtParam2Dlg::GetValue(int* p1, int* p2, int* p3, int* p4,
                             CString* p5, int* p6, CString* p7)			//<<< ﾊﾟﾗﾒｰﾀ取得
/****************************************************************/
{
	*p1 = m_POut, *p2 = m_POri, *p3 = m_PPos, *p4 = m_PSize;
	*p5 = m_sPicPath, *p6 = m_Check1, *p7 = m_sPicPath+m_sPicFile;
}

///////////////////////////////////////////////////////////////////////////////
// CCopyInfoDlg ダイアログ

BEGIN_MESSAGE_MAP(CCopyInfoDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/*******************************************************************************/
CCopyInfoDlg::CCopyInfoDlg(CWnd* pParent) : CDialog(CCopyInfoDlg::IDD, pParent)
/*******************************************************************************/
{
	m_CopyNum = 0;
	m_XScale  = 0.0;
	m_YScale  = 0.0;
	m_ZScale  = 0.0;
	m_SynFlg  = 0;
	m_XPoint  = 0;
	m_YPoint  = 0;
	m_ZPoint  = 0;
}

/*****************************************************/
void CCopyInfoDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_CopyNum);
	DDV_MinMaxInt(pDX, m_CopyNum, 1, 500);
	DDX_Text(pDX, IDC_EDIT2, m_XScale);
	DDV_MinMaxDouble(pDX, m_XScale, 0.001, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_YScale);
	DDV_MinMaxDouble(pDX, m_YScale, 0.001, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_ZScale);
	DDV_MinMaxDouble(pDX, m_ZScale, 0.001, 1000.0);
	DDX_Radio(pDX, IDC_RADIO1, m_SynFlg);
	DDX_Radio(pDX, IDC_RADIO4, m_XPoint);
	DDX_Radio(pDX, IDC_RADIO7, m_YPoint);
	DDX_Radio(pDX, IDC_RADIO10, m_ZPoint);
}

/********************************************************************/
void CCopyInfoDlg::SetValue(int p1, double p2, double p3, double p4,
                            int p5, int p6, int p7, int p8)				//<<< ﾊﾟﾗﾒｰﾀ設定
/********************************************************************/
{
	m_CopyNum = p1, m_XScale = p2, m_YScale = p3, m_ZScale = p4;
	m_SynFlg  = p5, m_XPoint = p6, m_YPoint = p7, m_ZPoint = p8;
}

/************************************************************************/
void CCopyInfoDlg::GetValue(int* p1, double* p2, double* p3, double* p4,
                            int* p5, int* p6, int* p7, int* p8)			//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************************************************/
{
	*p1 = m_CopyNum, *p2 = m_XScale, *p3 = m_YScale, *p4 = m_ZScale;
	*p5 = m_SynFlg,  *p6 = m_XPoint, *p7 = m_YPoint, *p8 = m_ZPoint;
}

///////////////////////////////////////////////////////////////////////////////
// CSpiralDlg ダイアログ

BEGIN_MESSAGE_MAP(CSpiralDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/*************************************************************************/
CSpiralDlg::CSpiralDlg(CWnd* pParent) : CDialog(CSpiralDlg::IDD, pParent)
/*************************************************************************/
{
	m_Diameter = 0.0;
	m_Height   = 0.0;
	m_CtrlNum  = 0;
	m_SprlNum  = 0;
}

/***************************************************/
void CSpiralDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Diameter);
	DDV_MinMaxDouble(pDX, m_Diameter, 1.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT2, m_Height);
	DDV_MinMaxDouble(pDX, m_Height, 1.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_CtrlNum);
	DDV_MinMaxInt(pDX, m_CtrlNum, 1, View->m_MaxCCtl);
	DDX_Text(pDX, IDC_EDIT4, m_SprlNum);
	DDV_MinMaxInt(pDX, m_SprlNum, 1, View->m_MaxCCtl);
}

/***************************************************************/
void CSpiralDlg::SetValue(double p1, double p2, int p3, int p4)			//<<< ﾊﾟﾗﾒｰﾀ取得
/***************************************************************/
{
	m_Diameter = p1, m_Height = p2, m_CtrlNum = p3, m_SprlNum = p4;
}

/*******************************************************************/
void CSpiralDlg::GetValue(double* p1, double* p2, int* p3, int* p4)		//<<< ﾊﾟﾗﾒｰﾀ取得
/*******************************************************************/
{
	*p1 = m_Diameter, *p2 = m_Height, *p3 = m_CtrlNum, *p4 = m_SprlNum;
}

///////////////////////////////////////////////////////////////////////////////
// CTensionDlg ダイアログ

BEGIN_MESSAGE_MAP(CTensionDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/****************************************************************************/
CTensionDlg::CTensionDlg(CWnd* pParent) : CDialog(CTensionDlg::IDD, pParent)
/****************************************************************************/
{
	m_Tension = 0;
	m_Check1  = 0;
}

/****************************************************/
void CTensionDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Tension);
	DDV_MinMaxInt(pDX, m_Tension, 0, 3);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
}

/**********************************/
void CTensionDlg::SetValue(int p1)										//<<< ﾊﾟﾗﾒｰﾀ設定
/**********************************/
{
	m_Tension = p1;
}

/********************************************/
void CTensionDlg::GetValue(int* p1, int* p2)							//<<< ﾊﾟﾗﾒｰﾀ取得
/********************************************/
{
	*p1 = m_Tension, *p2 = m_Check1;
}

///////////////////////////////////////////////////////////////////////////////
// CSetTmpDlg ダイアログ

BEGIN_MESSAGE_MAP(CSetTmpDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO8, OnCheckBtn1)
	ON_COMMAND_EX_RANGE(IDC_RADIO9, IDC_RADIO11, OnCheckBtn2)
	ON_BN_CLICKED(IDC_BUTTON1, OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON2, OnFileClear)
END_MESSAGE_MAP()

/*************************************************************************/
CSetTmpDlg::CSetTmpDlg(CWnd* pParent) : CDialog(CSetTmpDlg::IDD, pParent)
/*************************************************************************/
{
	int i, j;

	m_Knd = 0;
	m_Pln = 0;
	for(i=0; i<8; i++) {
		for(j=0; j<3; j++) {
			m_sTmpName[i][j] = _T("");
		}
	}
	m_sTmpPath = _T("");
	for(i=0; i<8; i++) {
		for(j=0; j<3; j++) {
			m_sTmpFile[i][j] = _T("");
		}
	}
}

/***************************************************/
void CSetTmpDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Knd);
	DDX_Radio(pDX, IDC_RADIO9, m_Pln);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
	DDX_Text(pDX, IDC_EDIT1, m_sTmpName[0][0]);
	DDX_Text(pDX, IDC_EDIT2, m_sTmpName[0][1]);
	DDX_Text(pDX, IDC_EDIT3, m_sTmpName[0][2]);
	DDX_Text(pDX, IDC_EDIT4, m_sTmpName[1][0]);
	DDX_Text(pDX, IDC_EDIT5, m_sTmpName[1][1]);
	DDX_Text(pDX, IDC_EDIT6, m_sTmpName[1][2]);
	DDX_Text(pDX, IDC_EDIT7, m_sTmpName[2][0]);
	DDX_Text(pDX, IDC_EDIT8, m_sTmpName[2][1]);
	DDX_Text(pDX, IDC_EDIT9, m_sTmpName[2][2]);
	DDX_Text(pDX, IDC_EDIT10, m_sTmpName[3][0]);
	DDX_Text(pDX, IDC_EDIT11, m_sTmpName[3][1]);
	DDX_Text(pDX, IDC_EDIT12, m_sTmpName[3][2]);
	DDX_Text(pDX, IDC_EDIT13, m_sTmpName[4][0]);
	DDX_Text(pDX, IDC_EDIT14, m_sTmpName[4][1]);
	DDX_Text(pDX, IDC_EDIT15, m_sTmpName[4][2]);
	DDX_Text(pDX, IDC_EDIT16, m_sTmpName[5][0]);
	DDX_Text(pDX, IDC_EDIT17, m_sTmpName[5][1]);
	DDX_Text(pDX, IDC_EDIT18, m_sTmpName[5][2]);
	DDX_Text(pDX, IDC_EDIT19, m_sTmpName[6][0]);
	DDX_Text(pDX, IDC_EDIT20, m_sTmpName[6][1]);
	DDX_Text(pDX, IDC_EDIT21, m_sTmpName[6][2]);
	DDX_Text(pDX, IDC_EDIT22, m_sTmpName[7][0]);
	DDX_Text(pDX, IDC_EDIT23, m_sTmpName[7][1]);
	DDX_Text(pDX, IDC_EDIT24, m_sTmpName[7][2]);
	DDX_Control(pDX, IDC_EDIT1, m_Edit[0][0]);
	DDX_Control(pDX, IDC_EDIT2, m_Edit[0][1]);
	DDX_Control(pDX, IDC_EDIT3, m_Edit[0][2]);
	DDX_Control(pDX, IDC_EDIT4, m_Edit[1][0]);
	DDX_Control(pDX, IDC_EDIT5, m_Edit[1][1]);
	DDX_Control(pDX, IDC_EDIT6, m_Edit[1][2]);
	DDX_Control(pDX, IDC_EDIT7, m_Edit[2][0]);
	DDX_Control(pDX, IDC_EDIT8, m_Edit[2][1]);
	DDX_Control(pDX, IDC_EDIT9, m_Edit[2][2]);
	DDX_Control(pDX, IDC_EDIT10, m_Edit[3][0]);
	DDX_Control(pDX, IDC_EDIT11, m_Edit[3][1]);
	DDX_Control(pDX, IDC_EDIT12, m_Edit[3][2]);
	DDX_Control(pDX, IDC_EDIT13, m_Edit[4][0]);
	DDX_Control(pDX, IDC_EDIT14, m_Edit[4][1]);
	DDX_Control(pDX, IDC_EDIT15, m_Edit[4][2]);
	DDX_Control(pDX, IDC_EDIT16, m_Edit[5][0]);
	DDX_Control(pDX, IDC_EDIT17, m_Edit[5][1]);
	DDX_Control(pDX, IDC_EDIT18, m_Edit[5][2]);
	DDX_Control(pDX, IDC_EDIT19, m_Edit[6][0]);
	DDX_Control(pDX, IDC_EDIT20, m_Edit[6][1]);
	DDX_Control(pDX, IDC_EDIT21, m_Edit[6][2]);
	DDX_Control(pDX, IDC_EDIT22, m_Edit[7][0]);
	DDX_Control(pDX, IDC_EDIT23, m_Edit[7][1]);
	DDX_Control(pDX, IDC_EDIT24, m_Edit[7][2]);
}

/**************************************/
BOOL CSetTmpDlg::OnCheckBtn1(UINT nID)									//<<< 設定ﾃﾝﾌﾟﾚｰﾄ入力確認処理
/**************************************/
{
	m_Knd = nID - IDC_RADIO1;
	return TRUE;
}

/**************************************/
BOOL CSetTmpDlg::OnCheckBtn2(UINT nID)									//<<< 表示平面入力確認処理
/**************************************/
{
	m_Pln = nID - IDC_RADIO9;
	return TRUE;
}

/*********************************/
void CSetTmpDlg::OnFileOpen(void)										//<<< 参照ﾎﾞﾀﾝ押下処理
/*********************************/
{
	CString ext="dxf", wex, file, filter;
	DWORD   flg;
	int     i;

	if(!CheckFilePath(0, m_sTmpPath)) {									// ﾊﾟｽ確認
		m_sTmpPath = GetAppPath(0);
	}
	flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;							// ｢ﾌｧｲﾙを開く｣
	filter.LoadString(IDS_FILE_FILTER105);
	file = m_sTmpName[m_Knd][m_Pln];
	wex = file.Right(4);
	if(!wex.CompareNoCase(".dxf")||!wex.CompareNoCase(".glt")) {		// <有効な拡張子>
		i = file.ReverseFind('.');
		file = file.Left(i);											// 拡張子を削除
	}
	CFileDlg dlg(TRUE, ext, file, flg, filter);							// ﾀﾞｲｱﾛｸﾞ表示
	dlg.m_ofn.lpstrInitialDir = m_sTmpPath;
	if(dlg.DoModal()==IDOK) {
		m_sTmpFile[m_Knd][m_Pln] = dlg.GetPathName();					// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		m_sTmpName[m_Knd][m_Pln] = dlg.GetFileTitle();					// ﾌｧｲﾙ名取得
		ext = dlg.GetFileExt();											// 拡張子取得
		m_sTmpName[m_Knd][m_Pln] = m_sTmpName[m_Knd][m_Pln]+"."+ext;
		m_Edit[m_Knd][m_Pln].SetWindowText(m_sTmpName[m_Knd][m_Pln]);	// ﾌｧｲﾙ名表示
		i = m_sTmpFile[m_Knd][m_Pln].ReverseFind('\\');
		m_sTmpPath = m_sTmpFile[m_Knd][m_Pln].Left(i+1);				// ﾊﾟｽ保存
	}
}

/**********************************/
void CSetTmpDlg::OnFileClear(void)										//<<< 消去ﾎﾞﾀﾝ押下処理
/**********************************/
{
	m_sTmpFile[m_Knd][m_Pln] = _T("");									// ﾌｧｲﾙ名消去
	m_sTmpName[m_Knd][m_Pln] = _T("");
	m_Edit[m_Knd][m_Pln].SetWindowText(m_sTmpName[m_Knd][m_Pln]);		// ﾌｧｲﾙ名表示
}

/**************************************/
void CSetTmpDlg::SetValue1(CString p1)									//<<< ﾊﾟﾗﾒｰﾀ1設定
/**************************************/
{
	m_sTmpPath = p1;
}

/**********************************************************************/
void CSetTmpDlg::SetValue2(int no, CString p1, CString p2, CString p3)	//<<< ﾊﾟﾗﾒｰﾀ2設定
/**********************************************************************/
{
	int i, j, k;

	m_sTmpFile[no][0] = p1;
	m_sTmpFile[no][1] = p2;
	m_sTmpFile[no][2] = p3;
	for(i=0; i<3; i++) {
		j = m_sTmpFile[no][i].GetLength();
		k = m_sTmpFile[no][i].ReverseFind('\\');
		if(j>0) {
			m_sTmpName[no][i] = m_sTmpFile[no][i].Right(j-k-1);
		}
	}
}

/***************************************/
void CSetTmpDlg::GetValue1(CString* p1)									//<<< ﾊﾟﾗﾒｰﾀ1取得
/***************************************/
{
	*p1 = m_sTmpPath;
}

/****************************************************/
void CSetTmpDlg::GetValue2(int no, CString* p1,
                           CString* p2, CString* p3)					//<<< ﾊﾟﾗﾒｰﾀ2取得
/****************************************************/
{
	*p1 = m_sTmpFile[no][0];
	*p2 = m_sTmpFile[no][1];
	*p3 = m_sTmpFile[no][2];
}

///////////////////////////////////////////////////////////////////////////////
// CCaptureDlg ダイアログ

BEGIN_MESSAGE_MAP(CCaptureDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_COMMAND_EX_RANGE(IDC_RADIO8, IDC_RADIO9, OnPictureKind)
END_MESSAGE_MAP()

/****************************************************************************/
CCaptureDlg::CCaptureDlg(CWnd* pParent) : CDialog(CCaptureDlg::IDD, pParent)
/****************************************************************************/
{
	m_CPlane = 3;
	m_CPos = 0;
	m_PKind = 0;
	m_PWidth = 100;
	m_PHeight = 100;
	m_JComp = 75;
}

/****************************************************/
void CCaptureDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_CPlane);
	DDX_Radio(pDX, IDC_RADIO6, m_CPos);
	DDX_Radio(pDX, IDC_RADIO8, m_PKind);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Text(pDX, IDC_EDIT1, m_PWidth);
	DDV_MinMaxUInt(pDX, m_PWidth, 1, 100);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Text(pDX, IDC_EDIT2, m_PHeight);
	DDV_MinMaxUInt(pDX, m_PHeight, 1, 100);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Text(pDX, IDC_EDIT3, m_JComp);
	DDV_MinMaxUInt(pDX, m_JComp, 1, 100);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
}

/************************************/
BOOL CCaptureDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CDialog::OnInitDialog();
	m_Spin1.SetRange(1, 100);
	m_Spin2.SetRange(1, 100);
	m_Spin3.SetRange(1, 100);
	m_Static1.EnableWindow(!m_PKind);
	m_Spin3.EnableWindow(!m_PKind);
	m_Edit3.EnableWindow(!m_PKind);
	m_Static2.EnableWindow(!m_PKind);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*****************************************/
BOOL CCaptureDlg::OnPictureKind(UINT nID)								//<<< 画像種類ﾎﾞﾀﾝ押下処理
/*****************************************/
{
	m_PKind = nID-IDC_RADIO8;
	m_Static1.EnableWindow(!m_PKind);
	m_Spin3.EnableWindow(!m_PKind);
	m_Edit3.EnableWindow(!m_PKind);
	m_Static2.EnableWindow(!m_PKind);
	return TRUE;
}

/**************************************************/
void CCaptureDlg::SetValue(int p1, int p2, int p3,
                           int p4, int p5, int p6)						//<<< ﾊﾟﾗﾒｰﾀ設定
/**************************************************/
{
	m_CPlane = p1, m_CPos = p2, m_PKind = p3;
	m_PWidth = p4, m_PHeight = p5, m_JComp = p6;
}

/*****************************************************/
void CCaptureDlg::GetValue(int* p1, int* p2, int* p3,
                           int* p4, int* p5, int* p6)					//<<< ﾊﾟﾗﾒｰﾀ取得
/*****************************************************/
{
	*p1 = m_CPlane, *p2 = m_CPos, *p3 = m_PKind;
	*p4 = m_PWidth, *p5 = m_PHeight, *p6 = m_JComp;
}

///////////////////////////////////////////////////////////////////////////////
// CSetTextDlg ダイアログ

BEGIN_MESSAGE_MAP(CSetTextDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/****************************************************************************/
CSetTextDlg::CSetTextDlg(CWnd* pParent) : CDialog(CSetTextDlg::IDD, pParent)
/****************************************************************************/
{
	m_FontName = _T("");
	m_Text     = _T("");
	m_Height   = 0.0;
	m_Depth    = 0.0;
	m_Intval   = 0.0;
	m_Check1   = FALSE;
	m_FontNo   = 0;
	GetFontInfo();
}

/****************************************************/
void CSetTextDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Combo1);
	DDX_CBString(pDX, IDC_COMBO1, m_FontName);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
	DDX_Text(pDX, IDC_EDIT2, m_Height);
	DDV_MinMaxDouble(pDX, m_Height, 1.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_Depth);
	DDV_MinMaxDouble(pDX, m_Depth, 0.5, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_Intval);
	DDV_MinMaxDouble(pDX, m_Intval, 0.0, 1000.0);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
}

/************************************/
BOOL CSetTextDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CDialog::OnInitDialog();
	for(int i=0; i<m_FontNum; i++) {
		m_Combo1.AddString(m_FNameAra[i]);
	}
	m_FontName = m_FNameAra[m_FontNo];
	m_Combo1.SetWindowText(m_FontName);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/****************************/
void CSetTextDlg::OnOK(void)											//<<< OKﾎﾞﾀﾝ押下処理
/****************************/
{
	char*   path = new char[_MAX_PATH];
	CString fname;
	int     i;

	CDialog::OnOK();
	if(m_Text.GetLength()>0&&
       m_Height>=1.0&&m_Height<=100.0&&
       m_Depth>=0.5&&m_Depth<=50.0&&
       m_Intval>=0.0&&m_Intval<=10.0) {
		for(i=0; i<m_FontNum; i++) {
			fname.Format("%s", m_FNameAra[i]);
			if(m_FontName==fname) {
				m_FontNo = i; break;
			}
		}
		if(i<m_FontNum) {
			memset(path, NULL, _MAX_PATH);
			memcpy(path, m_FontPath, m_FontLeng);
			memcpy(&path[m_FontLeng], m_FFileAra[i], strlen(m_FFileAra[i]));
			CreatTrueTypeFont(path, m_Text.GetBuffer(m_Text.GetLength()),
                              m_Depth, m_Height, m_Intval, m_Check1);
			m_Text.ReleaseBuffer();
		}
	}
	delete[] path;
}

/***********************************/
void CSetTextDlg::GetFontInfo(void)										//<<< Font情報取得
/***********************************/
{
	char*  file = new char[128];
	char*  path = new char[_MAX_PATH];
	char*  name = new char[1024];
	WIN32_FIND_DATA wf;
	HANDLE fhdl;
	int    len;

	m_FontNum = 0;
	memset(m_FontPath, NULL, _MAX_PATH);
	m_FontLeng = (int)GetWindowsDirectory(m_FontPath, _MAX_PATH);
	memcpy(&m_FontPath[m_FontLeng], "\\Fonts\\*.tt?", 12);
	m_FontLeng += 7;
	if((fhdl=FindFirstFile(m_FontPath, &wf))!=INVALID_HANDLE_VALUE) {
		while(TRUE) {
			memset(file, NULL, 128);
			strcpy_s(file, 128, wf.cFileName);
			len = strlen(file)-3;
			if(memcmp(&file[len], "ttf", 3)==0||
               memcmp(&file[len], "TTF", 3)==0||
               memcmp(&file[len], "ttc", 3)==0||
               memcmp(&file[len], "TTC", 3)==0) {
				memset(path, NULL, _MAX_PATH);
				memcpy(path, m_FontPath, m_FontLeng);
				memcpy(&path[m_FontLeng], file, strlen(file));
				memset(name, NULL, 1024);
				GetFontFileName(path, name);							// Fontﾌｧｲﾙ名取得
				if(strlen(name)>0) {
					memset(m_FNameAra[m_FontNum], NULL, 128);
					memcpy(m_FNameAra[m_FontNum], name, strlen(name));
					memset(m_FFileAra[m_FontNum], NULL, 128);
					memcpy(m_FFileAra[m_FontNum], file, strlen(file));
					if(++m_FontNum>=512) {
						break;
					}
				}
			}
			if(!FindNextFile(fhdl, &wf)) {
				break;
			}
		}
		FindClose(fhdl);
	}
	delete[] file;
	delete[] path;
	delete[] name;
}

/***********************************************************/
void CSetTextDlg::SetValue(int p1, int p2,
                           double p3, double p4, double p5)				//<<< ﾊﾟﾗﾒｰﾀ設定
/***********************************************************/
{
	m_FontNo = p1, m_Check1 = p2, m_Height = p3;
	m_Depth  = p4, m_Intval = p5;
}

/**************************************************************/
void CSetTextDlg::GetValue(int* p1, int* p2,
                           double* p3, double* p4, double* p5)			//<<< ﾊﾟﾗﾒｰﾀ取得
/**************************************************************/
{
	*p1 = m_FontNo, *p2 = m_Check1, *p3 = m_Height;
	*p4 = m_Depth,  *p5 = m_Intval;
}

///////////////////////////////////////////////////////////////////////////////
// CSetTexDlg ダイアログ

BEGIN_MESSAGE_MAP(CSetTexDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO3, OnCheckBtn1)
	ON_BN_CLICKED(IDC_BUTTON1, OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON2, OnFileClear)
END_MESSAGE_MAP()

/*************************************************************************/
CSetTexDlg::CSetTexDlg(CWnd* pParent) : CDialog(CSetTexDlg::IDD, pParent)
/*************************************************************************/
{
	int i, j, k;

	m_nID   = 0;
	m_Plane = 0;
	m_sTexPath = View->GetTexPath();									// ﾃｸｽﾁｬﾊﾟｽ取得
	for(i=2; i>=0; i--) {
		View->GetTexInfo(i, &m_sTexFile[i], &m_Move1, &m_Move2,
                         &m_Rotate, &m_Scale);							// ﾃｸｽﾁｬ表示状態取得
		j = m_sTexFile[i].GetLength();
		k = m_sTexFile[i].ReverseFind('\\');
		m_sTexName[i] = (j>0) ? m_sTexFile[i].Right(j-k-1) : _T("");
	}
	m_Rotate = m_Rotate*90;
}

/***************************************************/
void CSetTexDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_Plane);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_BUTTON2, m_Button2);
	DDX_Text(pDX, IDC_EDIT1, m_sTexName[0]);
	DDX_Text(pDX, IDC_EDIT2, m_sTexName[1]);
	DDX_Text(pDX, IDC_EDIT3, m_sTexName[2]);
	DDX_Text(pDX, IDC_EDIT4, m_Move1);
	DDX_Text(pDX, IDC_EDIT5, m_Move2);
	DDX_Text(pDX, IDC_EDIT6, m_Rotate);
	DDX_Text(pDX, IDC_EDIT7, m_Scale);
}

/***********************************/
BOOL CSetTexDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	int left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "STexLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "STexTop", -1);
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
	VERIFY(Left.AutoLoad(IDC_LEFT, this));								// 釦とﾋﾞｯﾄﾏｯﾌﾟを結び付ける
	VERIFY(Right.AutoLoad(IDC_RIGHT, this));
	VERIFY(Down.AutoLoad(IDC_DOWN, this));
	VERIFY(Up.AutoLoad(IDC_UP, this));
	VERIFY(Left2.AutoLoad(IDC_BUTTON3, this));
	VERIFY(Right2.AutoLoad(IDC_BUTTON4, this));
	VERIFY(Up2.AutoLoad(IDC_BUTTON5, this));
	VERIFY(Down2.AutoLoad(IDC_BUTTON6, this));
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/********************************/
void CSetTexDlg::OnDestroy(void)										//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "STexLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "STexTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/***************************************/
LRESULT CSetTexDlg::OnNcHitTest(CPoint)									//<<< WM_NCHITTEST ﾒｯｾｰｼﾞ応答処理
/***************************************/
{
	return HTCAPTION;													// ｸﾗｲｱﾝﾄ領域をﾄﾞﾗｯｸﾞしてｳｨﾝﾄﾞｳを
}																		// 移動するため

/***************************************/
void CSetTexDlg::OnTimer(UINT_PTR nIDTimer)									//<<< WM_TIMER ﾒｯｾｰｼﾞ応答処理
/***************************************/
{
	CString name;

	KillTimer(TIMERID);													// ﾀｲﾏｰ解除
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// 左ﾎﾞﾀﾝ押下中ならば
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上にある時
				View->ChangeTexInfo(m_Flg, m_Plane, m_Mode);			// ﾃｸｽﾁｬ表示状態変更
				View->GetTexInfo(m_Plane, &name, &m_Move1, &m_Move2,
                                 &m_Rotate, &m_Scale);					// ﾃｸｽﾁｬ表示状態取得
				m_Rotate = m_Rotate*90;
				UpdateData(FALSE);										// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
			}
			SetTimer(TIMERID, 20, NULL);								// ﾀｲﾏｰ設定
		}
	}
}

/*****************************************************************/
BOOL CSetTexDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)		//<<< WM_SETCURSOR ﾒｯｾｰｼﾞ応答処理
/*****************************************************************/
{
	CString name;

	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// ﾋﾞｯﾄﾏｯﾌﾟ釦上でﾏｳｽ左釦押下
		POINT point;
		GetCursorPos(&point);											// ﾏｳｽのｽｸﾘｰﾝ座標取得
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ﾏｳｽ位置を含む釦取得
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ﾏｳｽ位置を含む釦ID取得
			if(id>=IDC_LEFT&&id<=IDC_UP) {								// <ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上(移動)>
				m_nID  = id;
				m_Mode = (id - IDC_LEFT) / 2;							// 方向
				m_Flg  = (id - IDC_LEFT) % 2;							// 正負
				View->ChangeTexInfo(m_Flg, m_Plane, m_Mode);			// ﾃｸｽﾁｬ表示状態変更
				View->GetTexInfo(m_Plane, &name, &m_Move1, &m_Move2,
                                 &m_Rotate, &m_Scale);					// ﾃｸｽﾁｬ表示状態取得
				m_Rotate = m_Rotate*90;
				UpdateData(FALSE);										// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			} else if(id==IDC_BUTTON3||id==IDC_BUTTON4) {				// <ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上(回転)>
				m_nID  = id;
				m_Mode = 2;												// 回転
				m_Flg  = id - IDC_BUTTON3;								// 正負
				View->ChangeTexInfo(m_Flg, m_Plane, m_Mode);			// ﾃｸｽﾁｬ表示状態変更
				View->GetTexInfo(m_Plane, &name, &m_Move1, &m_Move2,
                                 &m_Rotate, &m_Scale);					// ﾃｸｽﾁｬ表示状態取得
				m_Rotate = m_Rotate*90;
				UpdateData(FALSE);										// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			} else if(id==IDC_BUTTON5||id==IDC_BUTTON6) {				// <ﾏｳｽがﾋﾞｯﾄﾏｯﾌﾟ釦上(ｻｲｽﾞ変更)>
				m_nID  = id;
				m_Mode = 3;												// サイズ変更
				m_Flg  = IDC_BUTTON6 - id;								// 正負
				View->ChangeTexInfo(m_Flg, m_Plane, m_Mode);			// ﾃｸｽﾁｬ表示状態変更
				View->GetTexInfo(m_Plane, &name, &m_Move1, &m_Move2,
                                 &m_Rotate, &m_Scale);					// ﾃｸｽﾁｬ表示状態取得
				m_Rotate = m_Rotate*90;
				UpdateData(FALSE);										// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
				SetTimer(TIMERID, 500, NULL);							// ﾀｲﾏｰ設定
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/**************************************/
BOOL CSetTexDlg::OnCheckBtn1(UINT nID)									//<<< 表示平面入力確認処理
/**************************************/
{
	CString name;

	m_Plane = nID - IDC_RADIO1;
	View->GetTexInfo(m_Plane, &name, &m_Move1, &m_Move2,
                     &m_Rotate, &m_Scale);								// ﾃｸｽﾁｬ表示状態取得
	m_Rotate = m_Rotate*90;
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
	return TRUE;
}

/*********************************/
void CSetTexDlg::OnFileOpen(void)										//<<< 参照ﾎﾞﾀﾝ押下処理
/*********************************/
{
	CString ext="jpg", wex, file, filter;
	DWORD   flg;
	int     i, j, kind;

	if(!CheckFilePath(0, m_sTexPath)) {									// ﾊﾟｽ確認
		m_sTexPath = GetAppPath(0);
	}
	flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;							// ｢ﾌｧｲﾙを開く｣
	filter.LoadString(IDS_FILE_FILTER103);
	file = m_sTexName[m_Plane];
	wex = file.Right(4);
	if(!wex.CompareNoCase(".jpg")||!wex.CompareNoCase("jpeg")||
       !wex.CompareNoCase(".bmp")) {									// <有効な拡張子>
		i = file.ReverseFind('.');
		file = file.Left(i);											// 拡張子を削除
	}
	CFileDlg dlg(TRUE, ext, file, flg, filter);							// ﾀﾞｲｱﾛｸﾞ表示
	dlg.m_ofn.lpstrInitialDir = m_sTexPath;
	if(dlg.DoModal()==IDOK) {
		m_sTexFile[m_Plane] = dlg.GetPathName();						// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		i = m_sTexFile[m_Plane].ReverseFind('\\');
		m_sTexPath = m_sTexFile[m_Plane].Left(i+1);						// ﾊﾟｽ保存
		View->SetTexPath(m_sTexPath);									// ﾃｸｽﾁｬﾊﾟｽ設定
		ext = dlg.GetFileExt();											// 拡張子取得
		kind = (!ext.CompareNoCase("bmp")) ? 0 : 1;						// ﾌｧｲﾙ種類(0:bmp,1:jpg)
		View->SetTexFile(1, kind, m_Plane, m_sTexFile[m_Plane]);		// ﾃｸｽﾁｬﾌｧｲﾙ名設定
		View->GetTexInfo(m_Plane, &m_sTexFile[m_Plane], &m_Move1,
                         &m_Move2,&m_Rotate, &m_Scale);					// ﾃｸｽﾁｬ表示状態取得
		m_Rotate = m_Rotate*90;
		i = m_sTexFile[m_Plane].GetLength();
		j = m_sTexFile[m_Plane].ReverseFind('\\');
		m_sTexName[m_Plane] = (i>0) ? m_sTexFile[m_Plane].Right(i-j-1) :
                                      _T("");
		UpdateData(FALSE);												// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
	}
}

/**********************************/
void CSetTexDlg::OnFileClear(void)										//<<< 消去ﾎﾞﾀﾝ押下処理
/**********************************/
{
	CString name;

	m_sTexFile[m_Plane] = _T("");										// ﾌｧｲﾙ名消去
	m_sTexName[m_Plane] = _T("");
	View->SetTexFile(0, 0, m_Plane, m_sTexFile[m_Plane]);				// ﾃｸｽﾁｬﾌｧｲﾙ名消去
	View->GetTexInfo(m_Plane, &name, &m_Move1, &m_Move2,
                     &m_Rotate, &m_Scale);								// ﾃｸｽﾁｬ表示状態取得
	m_Rotate = m_Rotate*90;
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

///////////////////////////////////////////////////////////////////////////////
// CProxyDlg ダイアログ

BEGIN_MESSAGE_MAP(CProxyDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
END_MESSAGE_MAP()

/**********************************************************************/
CProxyDlg::CProxyDlg(CWnd* pParent) : CDialog(CProxyDlg::IDD, pParent)
/**********************************************************************/
{
	m_Check1 = FALSE;
	m_Check2 = FALSE;
	m_ProxyAdress = _T("");
	m_ProxyPort = 8080;
}

/**********************************/
BOOL CProxyDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	CDialog::OnInitDialog();
	m_CCheck.EnableWindow(!m_Check1);
	m_Static1.EnableWindow((!m_Check1)&&m_Check2);
	m_Edit1.EnableWindow((!m_Check1)&&m_Check2);
	m_Static2.EnableWindow((!m_Check1)&&m_Check2);
	m_Edit2.EnableWindow((!m_Check1)&&m_Check2);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**************************************************/
void CProxyDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK2, m_CCheck);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Text(pDX, IDC_EDIT1, m_ProxyAdress);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Text(pDX, IDC_EDIT2, m_ProxyPort);
	DDV_MinMaxInt(pDX, m_ProxyPort, 0, 99999999);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
}

/******************************/
void CProxyDlg::OnCheck1(void)											//<<< ﾎﾞﾀﾝﾁｪｯｸ1
/******************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// 釦のON/OFF状態を取得
	m_CCheck.EnableWindow(!m_Check1);
	m_Static1.EnableWindow((!m_Check1)&&m_Check2);
	m_Edit1.EnableWindow((!m_Check1)&&m_Check2);
	m_Static2.EnableWindow((!m_Check1)&&m_Check2);
	m_Edit2.EnableWindow((!m_Check1)&&m_Check2);
}

/******************************/
void CProxyDlg::OnCheck2(void)											//<<< ﾎﾞﾀﾝﾁｪｯｸ2
/******************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// 釦のON/OFF状態を取得
	m_Static1.EnableWindow((!m_Check1)&&m_Check2);
	m_Edit1.EnableWindow((!m_Check1)&&m_Check2);
	m_Static2.EnableWindow((!m_Check1)&&m_Check2);
	m_Edit2.EnableWindow((!m_Check1)&&m_Check2);
}

/******************************************************************/
void CProxyDlg::GetValue(BOOL* p1, BOOL* p2, CString* p3, int* p4)		//<<< ﾊﾟﾗﾒｰﾀ取得
/******************************************************************/
{
	*p1 = m_Check1, *p2 = m_Check2;
	*p3 = m_ProxyAdress, *p4 = m_ProxyPort;
}

///////////////////////////////////////////////////////////////////////////////
// CPasswdDlg ダイアログ

BEGIN_MESSAGE_MAP(CPasswdDlg, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
END_MESSAGE_MAP()

/***********************************************************************/
CPasswdDlg::CPasswdDlg(CWnd* pParent):CDialog(CPasswdDlg::IDD, pParent)
/***********************************************************************/
{
	m_WPwdData1 = _T("");
	m_WPwdData2 = _T("");
	m_Check1 = FALSE;
	m_RPwdData1 = _T("");
	m_RPwdData2 = _T("");
}

/***************************************************/
void CPasswdDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/***************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_WPwdData1);
	DDX_Text(pDX, IDC_EDIT2, m_WPwdData2);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Text(pDX, IDC_EDIT3, m_RPwdData1);
	DDX_Text(pDX, IDC_EDIT4, m_RPwdData2);
}

/***********************************/
BOOL CPasswdDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/***********************************/
{
	CDialog::OnInitDialog();
	m_Static1.EnableWindow(!m_Check1);
	m_Static2.EnableWindow(!m_Check1);
	m_Edit3.EnableWindow(!m_Check1);
	m_Edit4.EnableWindow(!m_Check1);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/***************************/
void CPasswdDlg::OnOK(void)												//<<< OKﾎﾞﾀﾝ押下処理
/***************************/
{
	BOOL ok=TRUE;

	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	if(m_WPwdData1!=m_WPwdData2) {
		AfxMessageBox(IDS_ERR_INPUT);									// ﾊﾟｽﾜｰﾄﾞが異なるはｴﾗｰ
		ok = FALSE;
	}
	if(m_WPwdData1.GetLength()>10) {
		AfxMessageBox(IDS_ERR_INPUT);									// 10桁より多いはｴﾗｰ
		ok = FALSE;
	}
	if(!m_Check1) {														// <書込ﾊﾟｽﾜｰﾄﾞ!=ﾊﾟｽﾜｰﾄﾞ読込>
		if(m_RPwdData1!=m_RPwdData2) {
			AfxMessageBox(IDS_ERR_INPUT);								// ﾊﾟｽﾜｰﾄﾞが異なるはｴﾗｰ
			ok = FALSE;
		}
		if(m_RPwdData1.GetLength()>10) {
			AfxMessageBox(IDS_ERR_INPUT);								// 10桁より多いはｴﾗｰ
			ok = FALSE;
		}
	}
	if(ok) {															// <入力ﾃﾞｰﾀ確認OK>
		CDialog::OnOK();
	}
}

/*******************************/
void CPasswdDlg::OnCheck1(void)											//<<< ﾎﾞﾀﾝ1ﾁｪｯｸ
/*******************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// 釦のON/OFF状態を取得
	m_Static1.EnableWindow(!m_Check1);
	m_Static2.EnableWindow(!m_Check1);
	m_Edit3.EnableWindow(!m_Check1);
	m_Edit4.EnableWindow(!m_Check1);
}

/*********************************************************/
void CPasswdDlg::SetValue(CString p1, CString p2, int p3)				//<<< ﾊﾟﾗﾒｰﾀ設定
/*********************************************************/
{
	m_WPwdData1 = p1;
	m_WPwdData2 = p1;
	m_RPwdData1 = p2;
	m_RPwdData2 = p2;
	m_Check1 = (BOOL)p3;
}

/************************************************************/
void CPasswdDlg::GetValue(CString* p1, CString* p2, int* p3)			//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************************************/
{
	*p1 = m_WPwdData1;
	*p2 = (m_Check1) ? m_WPwdData1 : m_RPwdData1;
	*p3 = (int)m_Check1;
}

///////////////////////////////////////////////////////////////////////////////
// CPasswdDlg2 ダイアログ

BEGIN_MESSAGE_MAP(CPasswdDlg2, CDialog)									// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/**************************************************************************/
CPasswdDlg2::CPasswdDlg2(CWnd* pParent):CDialog(CPasswdDlg2::IDD, pParent)
/**************************************************************************/
{
	m_WPwdData = _T("");
}

/****************************************************/
void CPasswdDlg2::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_WPwdData);
}

/****************************/
void CPasswdDlg2::OnOK(void)											//<<< OKﾎﾞﾀﾝ押下処理
/****************************/
{
	BOOL ok=TRUE;

	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	if(m_WPwdData.GetLength()>10) {
		AfxMessageBox(IDS_ERR_INPUT);									// 10桁より多いはｴﾗｰ
		ok = FALSE;
	}
	if(ok) {															// <入力ﾃﾞｰﾀ確認OK>
		CDialog::OnOK();
	}
}

/***************************************/
void CPasswdDlg2::GetValue(CString* p1)									//<<< ﾊﾟﾗﾒｰﾀ取得
/***************************************/
{
	*p1 = m_WPwdData;
}

///////////////////////////////////////////////////////////////////////////////
// CCurveDiviDlg ダイアログ

BEGIN_MESSAGE_MAP(CCurveDiviDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/**********************************************************************************/
CCurveDiviDlg::CCurveDiviDlg(CWnd* pParent) : CDialog(CCurveDiviDlg::IDD, pParent)
/**********************************************************************************/
{
	m_CurveDiv = 0;
	m_Polygon  = 0;
	m_FileBUp  = 0;
}

/******************************************************/
void CCurveDiviDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_CurveDiv);
	DDX_Radio(pDX, IDC_RADIO5, m_Polygon);
	DDX_Radio(pDX, IDC_RADIO7, m_FileBUp);
}

/****************************************************/
void CCurveDiviDlg::SetValue(int p1, int p2, int p3)					//<<< ﾊﾟﾗﾒｰﾀ設定
/****************************************************/
{
	m_CurveDiv = p1, m_Polygon = p2, m_FileBUp = p3;
}

/*******************************************************/
void CCurveDiviDlg::GetValue(int* p1, int* p2, int* p3)					//<<< ﾊﾟﾗﾒｰﾀ取得
/*******************************************************/
{
	*p1 = m_CurveDiv, *p2 = m_Polygon, *p3 = m_FileBUp;
}

///////////////////////////////////////////////////////////////////////////////
// CSetProngDlg ダイアログ

BEGIN_MESSAGE_MAP(CSetProngDlg, CDialog)								// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/*******************************************************************************/
CSetProngDlg::CSetProngDlg(CWnd* pParent) : CDialog(CSetProngDlg::IDD, pParent)
/*******************************************************************************/
{
	m_ProngN = 0;
	m_ProngL = 1.0;
	m_ProngD = 1.0;
	m_ProngR = 0.0;
}

/****************************************************/
void CSetProngDlg::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ProngN);
	DDV_MinMaxInt(pDX, m_ProngN, 0, 16);
	DDX_Text(pDX, IDC_EDIT2, m_ProngL);
	DDV_MinMaxDouble(pDX, m_ProngL, 0.001, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_ProngD);
	DDV_MinMaxDouble(pDX, m_ProngD, 0.001, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_ProngR);
	DDV_MinMaxDouble(pDX, m_ProngR, -1000.0, 1000.0);
}

/********************************************************************/
void CSetProngDlg::SetValue(int p1, double p2, double p3, double p4)	//<<< ﾊﾟﾗﾒｰﾀ設定
/********************************************************************/
{
	m_ProngN = p1, m_ProngL = p2, m_ProngD = p3, m_ProngR = p4;
}

/************************************************************************/
void CSetProngDlg::GetValue(int* p1, double* p2, double* p3, double* p4)//<<< ﾊﾟﾗﾒｰﾀ取得
/************************************************************************/
{
	*p1 = m_ProngN, *p2 = m_ProngL, *p3 = m_ProngD, *p4 = m_ProngR;
}

///////////////////////////////////////////////////////////////////////////////
// CLimitTrackerDlg ダイアログ

BEGIN_MESSAGE_MAP(CLimitTrackerDlg, CDialog)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
END_MESSAGE_MAP()

/*******************************************************************************************/
CLimitTrackerDlg::CLimitTrackerDlg(CWnd* pParent) : CDialog(CLimitTrackerDlg::IDD, pParent)
/*******************************************************************************************/
{
	m_XBase = 0.0;
	m_YBase = 0.0;
	m_ZBase = 0.0;
	m_XCheck1 = TRUE;
	m_XCheck2 = TRUE;
	m_YCheck1 = TRUE;
	m_YCheck2 = TRUE;
	m_ZCheck1 = TRUE;
	m_ZCheck2 = TRUE;
}

/*********************************************************/
void CLimitTrackerDlg::DoDataExchange(CDataExchange* pDX)				//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/*********************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_XBase);
	DDV_MinMaxDouble(pDX, m_XBase, -100000.0, 100000.0);
	DDX_Text(pDX, IDC_EDIT2, m_YBase);
	DDV_MinMaxDouble(pDX, m_YBase, -100000.0, 100000.0);
	DDX_Text(pDX, IDC_EDIT3, m_ZBase);
	DDV_MinMaxDouble(pDX, m_ZBase, -100000.0, 100000.0);
	DDX_Check(pDX, IDC_CHECK1, m_XCheck1);
	DDX_Check(pDX, IDC_CHECK2, m_XCheck2);
	DDX_Check(pDX, IDC_CHECK3, m_YCheck1);
	DDX_Check(pDX, IDC_CHECK4, m_YCheck2);
	DDX_Check(pDX, IDC_CHECK5, m_ZCheck1);
	DDX_Check(pDX, IDC_CHECK6, m_ZCheck2);
}

/************************************************************************/
void CLimitTrackerDlg::SetValue(double p1, double p2, double p3, int p4,
                                int p5, int p6, int p7, int p8, int p9)	//<<< ﾊﾟﾗﾒｰﾀ設定
/************************************************************************/
{
	m_XBase = p1, m_YBase = p2, m_ZBase = p3;
	m_XCheck1 = p4, m_XCheck2 = p5, m_YCheck1 = p6;
	m_YCheck2 = p7, m_ZCheck1 = p8, m_ZCheck2 = p9;
}

/*******************************************************************/
void CLimitTrackerDlg::GetValue(double* p1, double* p2, double* p3,
                                int* p4, int* p5, int* p6,
                                int* p7, int* p8, int* p9)				//<<< ﾊﾟﾗﾒｰﾀ取得
/*******************************************************************/
{
	*p1 = m_XBase, *p2 = m_YBase, *p3 = m_ZBase;
	*p4 = m_XCheck1, *p5 = m_XCheck2, *p6 = m_YCheck1;
	*p7 = m_YCheck2, *p8 = m_ZCheck1, *p9 = m_ZCheck2;
}
