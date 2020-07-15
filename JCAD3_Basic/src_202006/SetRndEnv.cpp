/////////////////////////////////////////////////////////////////////////////
// レンダリング環境設定プロパティシートクラス定義
// SetRndEnv.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "MainFrm.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "FileDlg.h"
#include "SetRndEnv.h"
#include "DataAccess1.h"
#include "DataAccess2.h"

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg1 ダイアログ

BEGIN_MESSAGE_MAP(CRndEnvDlg1, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO18, OnColorNo)
	ON_EN_CHANGE(IDC_EDIT1, OnSetColor1)
	ON_EN_CHANGE(IDC_EDIT2, OnSetColor1)
	ON_EN_CHANGE(IDC_EDIT3, OnSetColor1)
	ON_EN_CHANGE(IDC_EDIT4, OnSetColor2)
	ON_EN_CHANGE(IDC_EDIT5, OnSetColor2)
	ON_EN_CHANGE(IDC_EDIT6, OnSetColor2)
    ON_EN_CHANGE(IDC_EDIT7, OnSetColor2)
	ON_EN_CHANGE(IDC_EDIT8, OnSetColor3)
	ON_EN_CHANGE(IDC_EDIT9, OnSetColor3)
	ON_EN_CHANGE(IDC_EDIT10, OnSetColor3)
END_MESSAGE_MAP()

/************************************************************/
CRndEnvDlg1::CRndEnvDlg1() : CPropertyPage(CRndEnvDlg1::IDD)
/************************************************************/
{
    int i;

	m_No     = 0;
	m_Check1 = 0;
	m_Check2 = 0;
	m_Check3 = 0;
    for(i=0; i<3; i++) {                                                // ｶﾗｰ情報取得
    	View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
	m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CRndEnvDlg1::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_No);
	DDX_Text(pDX, IDC_EDIT1, m_RCl[0]);
	DDV_MinMaxInt(pDX, m_RCl[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Text(pDX, IDC_EDIT2, m_GCl[0]);
	DDV_MinMaxInt(pDX, m_GCl[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Text(pDX, IDC_EDIT3, m_BCl[0]);
	DDV_MinMaxInt(pDX, m_BCl[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_PIC1, m_Pic1);
    DDX_Text(pDX, IDC_EDIT4, m_RCl[1]);
	DDV_MinMaxInt(pDX, m_RCl[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN4, m_Spin4);
	DDX_Text(pDX, IDC_EDIT5, m_GCl[1]);
	DDV_MinMaxInt(pDX, m_GCl[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN5, m_Spin5);
	DDX_Text(pDX, IDC_EDIT6, m_BCl[1]);
	DDV_MinMaxInt(pDX, m_BCl[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN6, m_Spin6);
    DDX_Text(pDX, IDC_EDIT7, m_Shn);
	DDV_MinMaxInt(pDX, m_Shn, 0, 128);
	DDX_Control(pDX, IDC_SPIN7, m_Spin7);
	DDX_Control(pDX, IDC_PIC2, m_Pic2);
    DDX_Text(pDX, IDC_EDIT8, m_RCl[2]);
	DDV_MinMaxInt(pDX, m_RCl[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN8, m_Spin8);
	DDX_Text(pDX, IDC_EDIT9, m_GCl[2]);
	DDV_MinMaxInt(pDX, m_GCl[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN9, m_Spin9);
	DDX_Text(pDX, IDC_EDIT10, m_BCl[2]);
	DDV_MinMaxInt(pDX, m_BCl[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN10, m_Spin10);
	DDX_Control(pDX, IDC_PIC3, m_Pic3);
}

/*******************************/
void CRndEnvDlg1::OnPaint(void)											//<<< WM_PAINT ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	CPaintDC dc(this);													// 描画用ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ
	ColorPaint(&dc);													// ｶﾗｰﾍﾟｲﾝﾄ
}

/************************************/
BOOL CRndEnvDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
    int i;

	CPropertyPage::OnInitDialog();
	m_Spin1.SetRange(0, 255);
	m_Spin2.SetRange(0, 255);
	m_Spin3.SetRange(0, 255);
	m_Spin4.SetRange(0, 255);
	m_Spin5.SetRange(0, 255);
	m_Spin6.SetRange(0, 255);
    m_Spin7.SetRange(0, 128);
	m_Spin8.SetRange(0, 255);
	m_Spin9.SetRange(0, 255);
	m_Spin10.SetRange(0, 255);
    m_Pic1.GetWindowRect(m_SQ[0]);										// 矩形ｽｸﾘｰﾝ座標取得
    m_Pic2.GetWindowRect(m_SQ[1]);
    m_Pic3.GetWindowRect(m_SQ[2]);
	for(i=0; i<3; i++) {
		ScreenToClient(m_SQ[i]);										// ｸﾗｲｱﾝﾄ座標へ変換
	}
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CRndEnvDlg1::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
    int i;

	View->InitRColorFile(TRUE);											// 初期ﾚﾝﾀﾞﾘﾝｸﾞｶﾗｰﾌｧｲﾙ読込
    for(i=0; i<3; i++) {                                                // ｶﾗｰ情報取得
    	View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
	RedrawWindow(NULL, NULL);
}

/**********************************/
void CRndEnvDlg1::OnSaveFile(void)                                      //<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
    View->SaveRndEnv1();                                                // ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存処理1
}

/*************************************/
void CRndEnvDlg1::OnRestoreFile(void)                                   //<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
    int i;

    View->RestoreRndEnv1();                                             // ﾚﾝﾀﾞﾘﾝｸﾞ環境設定復元処理1
    for(i=0; i<3; i++) {                                                // ｶﾗｰ情報取得
        View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
    UpdateData(FALSE);                                                  // ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
    RedrawWindow(NULL, NULL);
}

/*************************************/
BOOL CRndEnvDlg1::OnColorNo(UINT nID)									//<<< ｶﾗｰNoﾎﾞﾀﾝ押下処理
/*************************************/
{
	OBJTYPE* op;
    int      i;

	m_No = nID-IDC_RADIO1;
    for(i=0; i<3; i++) {                                                // ｶﾗｰ情報取得
    	View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
	GetObjPtr(&op);														// 立体ﾎﾟｲﾝﾀ取得
	PutObjCol(op, m_No);												// 立体ｶﾗｰ設定
	View->Display(0);													// 再描画
	RedrawWindow(NULL, NULL);
	return TRUE;
}

/***********************************/
void CRndEnvDlg1::OnSetColor1(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理1
/***********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		View->SetColorRGB(m_No, 0, m_RCl[0], m_GCl[0], m_BCl[0], m_Shn);// ｶﾗｰ情報設定
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg1::OnSetColor2(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理2
/***********************************/
{
	if(++m_Check2>4) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		View->SetColorRGB(m_No, 1, m_RCl[1], m_GCl[1], m_BCl[1], m_Shn);// ｶﾗｰ情報設定
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg1::OnSetColor3(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理3
/***********************************/
{
	if(++m_Check3>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		View->SetColorRGB(m_No, 2, m_RCl[2], m_GCl[2], m_BCl[2], m_Shn);// ｶﾗｰ情報設定
		RedrawWindow(NULL, NULL);
	}
}

/******************************************/
void CRndEnvDlg1::ColorPaint(CPaintDC *dc)								//<<< ｶﾗｰﾍﾟｲﾝﾄ
/******************************************/
{
	int i;

    for(i=0; i<3; i++) {
    	CBrush brushNew(RGB(m_RCl[i], m_GCl[i], m_BCl[i]));
    	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    	CPen penNew(0, 1, RGB(m_RCl[i], m_GCl[i], m_BCl[i]));
    	CPen* pPenOld = dc->SelectObject(&penNew);
    	dc->Rectangle(m_SQ[i].left, m_SQ[i].top,
                      m_SQ[i].right, m_SQ[i].bottom);					// 領域描画
    	dc->SelectObject(pBrushOld);
    	dc->SelectObject(pPenOld);
    	brushNew.DeleteObject();
    	penNew.DeleteObject();
    }
}

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg2 ダイアログ

BEGIN_MESSAGE_MAP(CRndEnvDlg2, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_EN_CHANGE(IDC_EDIT1, OnSetColor1)
	ON_EN_CHANGE(IDC_EDIT2, OnSetColor1)
	ON_EN_CHANGE(IDC_EDIT3, OnSetColor1)
	ON_EN_CHANGE(IDC_EDIT4, OnSetColor2)
	ON_EN_CHANGE(IDC_EDIT5, OnSetColor2)
	ON_EN_CHANGE(IDC_EDIT6, OnSetColor2)
	ON_EN_CHANGE(IDC_EDIT7, OnSetColor3)
	ON_EN_CHANGE(IDC_EDIT8, OnSetColor3)
	ON_EN_CHANGE(IDC_EDIT9, OnSetColor3)
END_MESSAGE_MAP()

/************************************************************/
CRndEnvDlg2::CRndEnvDlg2() : CPropertyPage(CRndEnvDlg2::IDD)
/************************************************************/
{
    int i;

	m_Check1 = 0;
	m_Check2 = 0;
	m_Check3 = 0;
    for(i=0; i<3; i++) {
    	View->GetRenderRGB(i, &m_RCol[i], &m_GCol[i], &m_BCol[i]);		// ﾚﾝﾀﾞﾘﾝｸﾞ情報取得
    }
	m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CRndEnvDlg2::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_RCol[0]);
	DDV_MinMaxInt(pDX, m_RCol[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Text(pDX, IDC_EDIT2, m_GCol[0]);
	DDV_MinMaxInt(pDX, m_GCol[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Text(pDX, IDC_EDIT3, m_BCol[0]);
	DDV_MinMaxInt(pDX, m_BCol[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_PIC1, m_Pic1);
	DDX_Text(pDX, IDC_EDIT4, m_RCol[1]);
	DDV_MinMaxInt(pDX, m_RCol[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN4, m_Spin4);
	DDX_Text(pDX, IDC_EDIT5, m_GCol[1]);
	DDV_MinMaxInt(pDX, m_GCol[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN5, m_Spin5);
	DDX_Text(pDX, IDC_EDIT6, m_BCol[1]);
	DDV_MinMaxInt(pDX, m_BCol[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN6, m_Spin6);
	DDX_Control(pDX, IDC_PIC2, m_Pic2);
	DDX_Text(pDX, IDC_EDIT7, m_RCol[2]);
	DDV_MinMaxInt(pDX, m_RCol[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN7, m_Spin7);
	DDX_Text(pDX, IDC_EDIT8, m_GCol[2]);
	DDV_MinMaxInt(pDX, m_GCol[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN8, m_Spin8);
	DDX_Text(pDX, IDC_EDIT9, m_BCol[2]);
	DDV_MinMaxInt(pDX, m_BCol[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN9, m_Spin9);
	DDX_Control(pDX, IDC_PIC3, m_Pic3);
}

/*******************************/
void CRndEnvDlg2::OnPaint(void)											//<<< WM_PAINT ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	CPaintDC dc(this);													// 描画用ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ
	ColorPaint(&dc);													// ｶﾗｰﾍﾟｲﾝﾄ
}

/************************************/
BOOL CRndEnvDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
    int i;

	CPropertyPage::OnInitDialog();
	m_Spin1.SetRange(0, 255);
	m_Spin2.SetRange(0, 255);
	m_Spin3.SetRange(0, 255);
	m_Spin4.SetRange(0, 255);
	m_Spin5.SetRange(0, 255);
	m_Spin6.SetRange(0, 255);
	m_Spin7.SetRange(0, 255);
	m_Spin8.SetRange(0, 255);
	m_Spin9.SetRange(0, 255);
    m_Pic1.GetWindowRect(m_SQ[0]);										// 矩形ｽｸﾘｰﾝ座標取得
    m_Pic2.GetWindowRect(m_SQ[1]);
    m_Pic3.GetWindowRect(m_SQ[2]);
    for(i=0; i<3; i++) {
        ScreenToClient(m_SQ[i]);										// ｸﾗｲｱﾝﾄ座標へ変換
    }
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CRndEnvDlg2::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
    int i;

	View->InitRenderFile(1, TRUE);										// 初期ﾚﾝﾀﾞﾘﾝｸﾞﾌｧｲﾙ読込
    for(i=0; i<3; i++) {
    	View->GetRenderRGB(i, &m_RCol[i], &m_GCol[i], &m_BCol[i]);		// ﾚﾝﾀﾞﾘﾝｸﾞ情報取得
    }
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
	RedrawWindow(NULL, NULL);
}

/**********************************/
void CRndEnvDlg2::OnSaveFile(void)                                      //<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
    View->SaveRndEnv2();                                                // ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存処理2
}

/*************************************/
void CRndEnvDlg2::OnRestoreFile(void)                                   //<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
    int i;

    View->RestoreRndEnv2();                                             // ﾚﾝﾀﾞﾘﾝｸﾞ環境設定復元処理2
    for(i=0; i<3; i++) {
        View->GetRenderRGB(i, &m_RCol[i], &m_GCol[i], &m_BCol[i]);      // ﾚﾝﾀﾞﾘﾝｸﾞ情報取得
    }
    UpdateData(FALSE);                                                  // ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
    RedrawWindow(NULL, NULL);
}

/***********************************/
void CRndEnvDlg2::OnSetColor1(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理1
/***********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		View->SetRenderRGB(0, m_RCol[0], m_GCol[0], m_BCol[0]);			// ﾚﾝﾀﾞﾘﾝｸﾞ情報設定
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg2::OnSetColor2(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理2
/***********************************/
{
	if(++m_Check2>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		View->SetRenderRGB(1, m_RCol[1], m_GCol[1], m_BCol[1]);			// ﾚﾝﾀﾞﾘﾝｸﾞ情報設定
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg2::OnSetColor3(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理3
/***********************************/
{
	if(++m_Check3>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		View->SetRenderRGB(2, m_RCol[2], m_GCol[2], m_BCol[2]);			// ﾚﾝﾀﾞﾘﾝｸﾞ情報設定
		RedrawWindow(NULL, NULL);
	}
}

/******************************************/
void CRndEnvDlg2::ColorPaint(CPaintDC *dc)								//<<< ｶﾗｰﾍﾟｲﾝﾄ
/******************************************/
{
	int i;

    for(i=0; i<3; i++) {
    	CBrush brushNew(RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    	CPen penNew(0, 1, RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CPen* pPenOld = dc->SelectObject(&penNew);
    	dc->Rectangle(m_SQ[i].left, m_SQ[i].top,
                      m_SQ[i].right, m_SQ[i].bottom);					// 領域描画
    	dc->SelectObject(pBrushOld);
    	dc->SelectObject(pPenOld);
    	brushNew.DeleteObject();
    	penNew.DeleteObject();
    }
}

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg3 ダイアログ

BEGIN_MESSAGE_MAP(CRndEnvDlg3, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_BN_CLICKED(IDC_BUTTON1, OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON2, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON4, OnRestoreFile)
END_MESSAGE_MAP()

/************************************************************/
CRndEnvDlg3::CRndEnvDlg3() : CPropertyPage(CRndEnvDlg3::IDD)
/************************************************************/
{
	int i, j;

	m_FileName = _T("");
    m_Path = View->GetMapFilePath();                                    // ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟﾊﾟｽ取得
	View->GetRenderMap(&m_File);										// ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟ取得
	i = m_File.GetLength();
	j = m_File.ReverseFind('\\');
	if(i>0) {
		m_FileName = m_File.Right(i-j-1);
	}
	m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CRndEnvDlg3::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_FileName);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
}

/************************************/
BOOL CRndEnvDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CPropertyPage::OnInitDialog();
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CRndEnvDlg3::OnFileOpen(void)										//<<< 参照ﾎﾞﾀﾝ押下処理
/**********************************/
{
	CString ext="bmp", wex, file, filter;
	DWORD   flg;
	int     i;

	if(!CheckFilePath(0, m_Path)) {										// ﾊﾟｽ確認
		m_Path=GetAppPath(0);
	}
	flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;							// ｢BMPﾌｧｲﾙを開く｣
	filter.LoadString(IDS_FILE_FILTER101);
	file = m_FileName;
	wex = file.Right(4);
	if(!wex.CompareNoCase(".bmp")) {									// <有効な拡張子>
		i = file.ReverseFind('.');
		file = file.Left(i);											// 拡張子を削除
	}
	CFileDlg dlg(TRUE, ext, file, flg, filter);							// ﾀﾞｲｱﾛｸﾞ表示
	dlg.m_ofn.lpstrInitialDir = m_Path;
	if(dlg.DoModal()==IDOK) {
		m_File = dlg.GetPathName();										// ﾌｧｲﾙ名(ﾌﾙﾊﾟｽ)取得
		m_FileName = dlg.GetFileTitle();								// ﾌｧｲﾙ名取得
		ext = dlg.GetFileExt();											// 拡張子取得
		m_FileName = m_FileName + "." + ext;
		m_Edit1.SetWindowText(m_FileName);								// ﾌｧｲﾙ名表示
		i = m_File.ReverseFind('\\');
		m_Path = m_File.Left(i+1);										// ﾊﾟｽ保存
        View->SetMapFilePath(m_Path);                                   // ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟﾊﾟｽ設定
		View->SetRenderMap(m_File);										// ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟ設定
	}
}

/**********************************/
void CRndEnvDlg3::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
	int i, j;

	View->InitRenderFile(2, TRUE);										// 初期ﾚﾝﾀﾞﾘﾝｸﾞﾌｧｲﾙ読込
	View->GetRenderMap(&m_File);										// ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟ取得
	i = m_File.GetLength();
	j = m_File.ReverseFind('\\');
	if(i>0) {
		m_FileName = m_File.Right(i-j-1);
	}
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/**********************************/
void CRndEnvDlg3::OnSaveFile(void)                                      //<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
    View->SaveRndEnv3();                                                // ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存処理3
}

/*************************************/
void CRndEnvDlg3::OnRestoreFile(void)                                   //<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
    int i, j;

    View->RestoreRndEnv3();                                             // ﾚﾝﾀﾞﾘﾝｸﾞ環境設定復元処理3
    View->GetRenderMap(&m_File);                                        // ﾚﾝﾀﾞﾘﾝｸﾞﾏｯﾌﾟ取得
    i = m_File.GetLength();
    j = m_File.ReverseFind('\\');
    if(i>0) {
        m_FileName = m_File.Right(i-j-1);
    }
    UpdateData(FALSE);                                                  // ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

///////////////////////////////////////////////////////////////////////////////
// CRndEnvSheet プロパティシート

/*******************************************************************************/
CRndEnvSheet::CRndEnvSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
/*******************************************************************************/
{
	AddControlPages();
	// TODO :: この位置に残りのコントロール用のページを追加してください。
}

/**********************************************************************************/
CRndEnvSheet::CRndEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
/**********************************************************************************/
{
	AddControlPages();
}

/*****************************/
CRndEnvSheet::~CRndEnvSheet()
/*****************************/
{
}

/************************************/
void CRndEnvSheet::AddControlPages()
/************************************/
{
	m_psh.dwFlags &= ~PSH_USEHICON;
	m_psh.dwFlags &= ~PSH_HASHELP;										// ヘルプ ボタンをなくします
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags |= PSH_USEPAGELANG;

	AddPage(&m_RndEnvPage1);
	AddPage(&m_RndEnvPage2);
	AddPage(&m_RndEnvPage3);
}

BEGIN_MESSAGE_MAP(CRndEnvSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CRndEnvSheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*********************************/
BOOL CRndEnvSheet::OnInitDialog()
/*********************************/
{
	return CPropertySheet::OnInitDialog();
}

/***************************************************************************************/
BOOL CRndEnvSheet::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
			const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
/***************************************************************************************/
{
	//既定値の DS_CONTEXT_HELP スタイルを削除します。
//  dwStyle= WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
