/////////////////////////////////////////////////////////////////////////////
// 計算機ダイアログクラス定義
// CalcDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "CalcDlg.h"

///////////////////////////////////////////////////////////////////////////////
// CCalcDlg1 ダイアログ

BEGIN_MESSAGE_MAP(CCalcDlg1, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnInputKey1)
	ON_BN_CLICKED(IDC_BUTTON2, OnInputKey2)
	ON_BN_CLICKED(IDC_BUTTON3, OnInputKey3)
	ON_BN_CLICKED(IDC_BUTTON4, OnInputKey4)
	ON_BN_CLICKED(IDC_BUTTON5, OnInputKey5)
	ON_BN_CLICKED(IDC_BUTTON6, OnInputKey6)
	ON_BN_CLICKED(IDC_BUTTON7, OnInputKey7)
	ON_BN_CLICKED(IDC_BUTTON8, OnInputKey8)
	ON_BN_CLICKED(IDC_BUTTON9, OnInputKey9)
	ON_BN_CLICKED(IDC_BUTTON10, OnInputKey0)
	ON_BN_CLICKED(IDC_BUTTON11, OnInputKeyDot)
	ON_BN_CLICKED(IDC_BUTTON12, OnInputKeyPai)
	ON_BN_CLICKED(IDC_BUTTON13, OnInputKeyAddition)
	ON_BN_CLICKED(IDC_BUTTON14, OnInputKeySubtract)
	ON_BN_CLICKED(IDC_BUTTON15, OnInputKeyMultiply)
	ON_BN_CLICKED(IDC_BUTTON16, OnInputKeyDivision)
	ON_BN_CLICKED(IDC_BUTTON17, OnInputKeyEqual)
	ON_BN_CLICKED(IDC_BUTTON18, OnInputKeyC)
	ON_BN_CLICKED(IDC_BUTTON19, OnInputKeyCE)
	ON_BN_CLICKED(IDC_BUTTON20, OnInputKeySign)
	ON_BN_CLICKED(IDC_BUTTON21, OnInputKeyCopy)
	ON_BN_CLICKED(IDC_BUTTON22, OnInputKeyOval)
END_MESSAGE_MAP()

/**********************************************************************/
CCalcDlg1::CCalcDlg1(CWnd* pParent) : CDialog(CCalcDlg1::IDD, pParent)
/**********************************************************************/
{
	m_pParent = pParent;
	m_strNumber = _T("0"), m_strType = _T(""), m_strSign = _T("");
	m_Number = 0.0, m_Value = 0.0;
	m_Type = 0, m_Type2 = 0, m_Sign = 1, m_Count = 0, m_StartFlg = 0;
}

/**************************************************/
void CCalcDlg1::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strNumber);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT2, m_strType);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
}

/**********************************/
BOOL CCalcDlg1::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************/
void CCalcDlg1::OnCancel(void)											//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/******************************/
{
	DestroyWindow();
	View->EndCalcDlg1();												// ﾀﾞｲｱﾛｸﾞ終了
}

/*******************************/
void CCalcDlg1::OnDestroy(void)											//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "CalcLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CalcTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/****************************/
BOOL CCalcDlg1::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/****************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "CalcLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "CalcTop", -1);
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
	return ret;
}

/*********************************/
void CCalcDlg1::OnInputKey1(void)										//<<< Key:[1]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("1"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey2(void)										//<<< Key:[2]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("2"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey3(void)										//<<< Key:[3]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("3"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey4(void)										//<<< Key:[4]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("4"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey5(void)										//<<< Key:[5]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("5"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey6(void)										//<<< Key:[6]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("6"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey7(void)										//<<< Key:[7]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("7"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey8(void)										//<<< Key:[8]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("8"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey9(void)										//<<< Key:[9]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("9"));												// Keyﾃﾞｰﾀ入力処理
}

/*********************************/
void CCalcDlg1::OnInputKey0(void)										//<<< Key:[0]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	InputKeyData(_T("0"));												// Keyﾃﾞｰﾀ入力処理
}

/***********************************/
void CCalcDlg1::OnInputKeyDot(void)										//<<< Key:[.]ﾎﾞﾀﾝ押下処理
/***********************************/
{
	InputKeyData(_T("."));												// Keyﾃﾞｰﾀ入力処理
}

/***********************************/
void CCalcDlg1::OnInputKeyPai(void)										//<<< Key:[Pai]ﾎﾞﾀﾝ押下処理
/***********************************/
{
	m_strNumber = _T("3.1415926535");									// 12桁まで入力
	m_Count = 12;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// 入力値表示
}

/****************************************/
void CCalcDlg1::OnInputKeyAddition(void)								//<<< Key:[+]ﾎﾞﾀﾝ押下処理
/****************************************/
{
	CheckInputData();													// 直接入力ﾁｪｯｸ処理
	ChangeKeyData();													// Keyﾃﾞｰﾀ数値変換処理
	if(m_StartFlg>0&&m_Count>0) {										// <2回目以降の入力有>
		CalcKeyData();													// Keyﾃﾞｰﾀ数値計算処理
	}
	if(m_Type==1) {														// <既に[+]の場合>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("+k"), m_Type2 = 1;							// [+]は[+k]へ変更
			m_Number = m_Value;											// その時点の結果を入力値へ
		} else {
			m_strType = _T("+"), m_Type2 = 0;							// [+k]は[+]へ変更
		}
	} else {															// <新規に[+]の場合>
		m_strType = _T("+"), m_Type = 1, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/****************************************/
void CCalcDlg1::OnInputKeySubtract(void)								//<<< Key:[-]ﾎﾞﾀﾝ押下処理
/****************************************/
{
	CheckInputData();													// 直接入力ﾁｪｯｸ処理
	ChangeKeyData();													// Keyﾃﾞｰﾀ数値変換処理
	if(m_StartFlg>0&&m_Count>0) {										// <2回目以降の入力有>
		CalcKeyData();													// Keyﾃﾞｰﾀ数値計算処理
	}
	if(m_Type==2) {														// <既に[-]の場合>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("-k"), m_Type2 = 1;							// [-]は[-k]へ変更
			m_Number = m_Value;											// その時点の結果を入力値へ
		} else {
			m_strType = _T("-"), m_Type2 = 0;							// [-k]は[-]へ変更
		}
	} else {															// <新規に[-]の場合>
		m_strType = _T("-"), m_Type = 2, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/****************************************/
void CCalcDlg1::OnInputKeyMultiply(void)								//<<< Key:[x]ﾎﾞﾀﾝ押下処理
/****************************************/
{
	CheckInputData();													// 直接入力ﾁｪｯｸ処理
	ChangeKeyData();													// Keyﾃﾞｰﾀ数値変換処理
	if(m_StartFlg>0&&m_Count>0) {										// <2回目以降の入力有>
		CalcKeyData();													// Keyﾃﾞｰﾀ数値計算処理
	}
	if(m_Type==3) {														// <既に[x]の場合>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("xk"), m_Type2 = 1;							// [x]は[xk]へ変更
			m_Number = m_Value;											// その時点の結果を入力値へ
		} else {
			m_strType = _T("x"), m_Type2 = 0;							// [xk]は[x]へ変更
		}
	} else {															// <新規に[x]の場合>
		m_strType = _T("x"), m_Type = 3, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/****************************************/
void CCalcDlg1::OnInputKeyDivision(void)								//<<< Key:[/]ﾎﾞﾀﾝ押下処理
/****************************************/
{
	CheckInputData();													// 直接入力ﾁｪｯｸ処理
	ChangeKeyData();													// Keyﾃﾞｰﾀ数値変換処理
	if(m_StartFlg>0&&m_Count>0) {										// <2回目以降の入力有>
		CalcKeyData();													// Keyﾃﾞｰﾀ数値計算処理
	}
	if(m_Type==4) {														// <既に[/]の場合>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("/k"), m_Type2 = 1;							// [/]は[/k]へ変更
			m_Number = m_Value;											// その時点の結果を入力値へ
		} else {
			m_strType = _T("/"), m_Type2 = 0;							// [/k]は[/]へ変更
		}
	} else {															// <新規に[/]の場合>
		m_strType = _T("/"), m_Type = 4, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/*************************************/
void CCalcDlg1::OnInputKeyEqual(void)									//<<< Key:[=]ﾎﾞﾀﾝ押下処理
/*************************************/
{
	if(m_Type2==1) {													// <繰返し有の場合>
		CalcKeyData();													// Keyﾃﾞｰﾀ数値計算処理
	} else {															// <繰返し無の場合>
		CheckInputData();												// 直接入力ﾁｪｯｸ処理
		ChangeKeyData();												// Keyﾃﾞｰﾀ数値変換処理
		if(m_StartFlg>0&&m_Count>0) {									// <2回目以降の入力有>
			CalcKeyData();												// Keyﾃﾞｰﾀ数値計算処理
		}
		m_strType = _T(""), m_Type = 0, m_Type2 = 0;
		m_Edit2.SetWindowText(m_strType);
	}
	m_StartFlg = 1, m_Count = 0;
}

/*********************************/
void CCalcDlg1::OnInputKeyC(void)										//<<< Key:[C]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	m_strNumber = _T("0"), m_strType = _T(""), m_strSign = _T("");		// 初期化
	m_Number = 0.0, m_Value = 0.0;
	m_Type = 0, m_Type2 = 0, m_Sign = 1, m_Count = 0, m_StartFlg = 0;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// 初期値表示
	m_Edit2.SetWindowText(m_strType);
}

/**********************************/
void CCalcDlg1::OnInputKeyCE(void)										//<<< Key:[CE]ﾎﾞﾀﾝ押下処理
/**********************************/
{
	m_strNumber = _T("0"), m_strSign = _T("");							// 初期化
	m_Sign = 1, m_Count = 0;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// 初期値表示
}

/************************************/
void CCalcDlg1::OnInputKeySign(void)									//<<< Key:[+/-]ﾎﾞﾀﾝ押下処理
/************************************/
{
	m_strSign = (m_Sign==1) ? _T("-") : _T("");							// 符号を入替え
	m_Sign = (m_Sign==1) ? -1 : 1;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// 入力値表示
}

/************************************/
void CCalcDlg1::OnInputKeyCopy(void)									//<<< Key:[Copy]ﾎﾞﾀﾝ押下処理
/************************************/
{
	m_Edit1.SetSel(0, -1);
	m_Edit1.Copy();														// ｺﾋﾟｰ
}

/************************************/
void CCalcDlg1::OnInputKeyOval(void)									//<<< Key:[Oval]ﾎﾞﾀﾝ押下処理
/************************************/
{
	DestroyWindow();
	View->EndCalcDlg1();												// ﾀﾞｲｱﾛｸﾞ終了
	if(!View->IsCalcDlg2Show()) {
		View->CreateCalcDlg2();											// 計算機ﾀﾞｲｱﾛｸﾞ2生成
	}
}

/********************************************/
void CCalcDlg1::InputKeyData(CString strKey)							//<<< Keyﾃﾞｰﾀ入力処理
/********************************************/
{
	if(m_Count==0) {
		m_strNumber = _T("");											// 初期はﾊﾞｯﾌｧｸﾘｱ
		m_strSign = _T(""), m_Sign = 1;
	}
	if(m_Count<12) {
		m_strNumber += strKey;											// 12桁まで入力
		m_Count++;
		m_Edit1.SetWindowText(m_strSign+m_strNumber);					// 入力値表示
	} else {
		AfxMessageBox(IDS_ERR_INPUT);									// 12桁超はｴﾗｰ
	}
}

/************************************/
void CCalcDlg1::CheckInputData(void)									//<<< 直接入力ﾁｪｯｸ処理
/************************************/
{
	CString str;
	int     len;

	m_Edit1.GetWindowText(str);											// 入力値取得
	len = str.GetLength();												// 入力文字数取得
	m_strNumber = str.SpanIncluding(_T("0123456789."));					// 数値のみ抽出
	m_Count = m_strNumber.GetLength();									// 有効文字数
	if(m_Count!=len||m_Count>12) {
		AfxMessageBox(IDS_ERR_INPUT);									// 12桁超/数値以外はｴﾗｰ
	}
	if(m_Count>12) {
		m_strNumber = m_strNumber.Left(12);								// 12桁まで入力
		m_Count = 12;
	}
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// 入力値表示
}

/***********************************/
void CCalcDlg1::ChangeKeyData(void)										//<<< Keyﾃﾞｰﾀ数値変換処理
/***********************************/
{
	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	if(m_StartFlg==0) {													// <<1回目の変換>>
		if(m_Count==0) {
			m_Value = 0.0;												// 未入力は0
		} else {
			m_Value = atof(m_strSign+m_strNumber);						// 入力文字列を数値変換
		}
	} else {															// <<2回目以降の変換>>
		if(m_Count>0) {
			m_Number = atof(m_strSign+m_strNumber);						// 入力文字列を数値変換
		}
	}
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*********************************/
void CCalcDlg1::CalcKeyData(void)										//<<< Keyﾃﾞｰﾀ数値計算処理
/*********************************/
{
	CString str;
	int     len;

	if(m_Type==1) {														// <[+]の場合>
		m_Value = m_Value + m_Number;
	} else if(m_Type==2) {												// <[-]の場合>
		m_Value = m_Value - m_Number;
	} else if(m_Type==3) {												// <[x]の場合>
		m_Value = m_Value * m_Number;
	} else if(m_Type==4) {												// <[/]の場合>
		m_Value = (m_Number!=0) ? m_Value / m_Number : 0.0;
	}
	m_Sign = (m_Value<0.0) ? -1 : 1;
	m_strSign = (m_Value<0.0) ? _T("-") : _T("");
	m_strNumber.Format("%0.12f", fabs(m_Value));
	while(TRUE) {
		str = m_strNumber;
		len = m_strNumber.GetLength();									// 文字数(数値)
		if(str.Right(1)==_T("0")) {										// <末尾が0の場合>
			m_strNumber = str.Left(len-1);								// 末尾をｶｯﾄ
		} else {
			break;
		}
	}
	str = m_strNumber;
	len = m_strNumber.GetLength();										// 文字数(数値)
	if(str.Right(1)==_T(".")) {											// <末尾が.の場合>
		m_strNumber = str.Left(len-1);									// 末尾をｶｯﾄ
	}
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// 結果表示
	if(fabs(m_Value)>999999999999) {
		AfxMessageBox(IDS_ERR_OVERFLOW);								// ｵｰﾊﾞｰﾌﾛｰ
	}
}

///////////////////////////////////////////////////////////////////////////////
// CCalcDlg2 ダイアログ

BEGIN_MESSAGE_MAP(CCalcDlg2, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnPasteData1)
	ON_BN_CLICKED(IDC_BUTTON2, OnCopyData1)
	ON_BN_CLICKED(IDC_BUTTON3, OnPasteData2)
	ON_BN_CLICKED(IDC_BUTTON4, OnCopyData2)
	ON_BN_CLICKED(IDC_BUTTON5, OnHelpProc)
	ON_BN_CLICKED(IDC_BUTTON6, OnPasteData3)
	ON_BN_CLICKED(IDC_BUTTON7, OnCopyData3)
	ON_BN_CLICKED(IDC_BUTTON8, OnInputCls)
	ON_BN_CLICKED(IDC_BUTTON9, OnCopyData4)
	ON_BN_CLICKED(IDC_BUTTON10, OnExeProc)
	ON_BN_CLICKED(IDC_BUTTON11, OnChangeCalc)
END_MESSAGE_MAP()

/**********************************************************************/
CCalcDlg2::CCalcDlg2(CWnd* pParent) : CDialog(CCalcDlg2::IDD, pParent)
/**********************************************************************/
{
	m_pParent = pParent;
	m_strNumber1 = _T("1"), m_strNumber2 = _T("1");
	m_strNumber3 = _T("1"), m_strNumber4 = _T("");
	m_Number1 = 1.0, m_Number2 = 1.0, m_Number3 = 1.0;
	m_Size = 0, m_Width = 0, m_Height = 0;
}

/**************************************************/
void CCalcDlg2::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strNumber1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT2, m_strNumber2);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Text(pDX, IDC_EDIT3, m_strNumber3);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Text(pDX, IDC_EDIT4, m_strNumber4);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_EDIT5, m_Edit5);
}

/**********************************/
BOOL CCalcDlg2::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**********************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/******************************/
void CCalcDlg2::OnCancel(void)											//<<< WM_CANCEL ﾒｯｾｰｼﾞ応答処理
/******************************/
{
	DestroyWindow();
	View->EndCalcDlg2();												// ﾀﾞｲｱﾛｸﾞ終了
}

/*******************************/
void CCalcDlg2::OnDestroy(void)											//<<< WM_DESTROY ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	theApp.WriteProfileInt("LAYOUT", "CalcLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CalcTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/****************************/
BOOL CCalcDlg2::Create(void)											//<<< ﾀﾞｲｱﾛｸﾞ生成
/****************************/
{
	CRect r;
	int   left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ｳｨﾝﾄﾞｳ表示状態取得
	left = theApp.GetProfileInt("LAYOUT", "CalcLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "CalcTop", -1);
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
	GetWindowRect(&r);
	m_Width = r.Width();
	m_Height = r.Height();												// ｻｲｽﾞ変更
	SetWindowPos(NULL, 0, 0, m_Width, (int)(m_Height*0.373), SWP_NOMOVE|SWP_NOZORDER);
	return ret;
}

/**********************************/
void CCalcDlg2::OnPasteData1(void)										//<<< [Paste1]ﾎﾞﾀﾝ押下処理
/**********************************/
{
	m_Edit1.SetSel(0, -1);
	m_Edit1.Paste();													// ﾍﾟｰｽﾄ
}

/*********************************/
void CCalcDlg2::OnCopyData1(void)										//<<< [Copy1]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	m_Edit1.SetSel(0, -1);
	m_Edit1.Copy();														// ｺﾋﾟｰ
}

/**********************************/
void CCalcDlg2::OnPasteData2(void)										//<<< [Paste2]ﾎﾞﾀﾝ押下処理
/**********************************/
{
	m_Edit2.SetSel(0, -1);
	m_Edit2.Paste();													// ﾍﾟｰｽﾄ
}

/*********************************/
void CCalcDlg2::OnCopyData2(void)										//<<< [Copy2]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	m_Edit2.SetSel(0, -1);
	m_Edit2.Copy();														// ｺﾋﾟｰ
}

/********************************/
void CCalcDlg2::OnHelpProc(void)										//<<< [Help]ﾎﾞﾀﾝ押下処理
/********************************/
{
	CString str;
	int     hei;

	hei = (!m_Size) ? m_Height : (int)(m_Height*0.373);					// 縦ｻｲｽﾞ
	m_Size = (!m_Size) ? 1 : 0;
	SetWindowPos(NULL, 0, 0, m_Width, hei, SWP_NOMOVE|SWP_NOZORDER);	// ｻｲｽﾞ変更
	if(m_Size) {
		str = "それぞれの項目は、次の意味で使用しています\n";
		str += "a,b:オーバルの径\nd:周りに並べる円(メレダイヤ)の直径\n";
		str += "n:並べる円の個数\n(右図参照)\n";
		str += "求められる値はn(個数)のみです\n";
		str += "a,b,dの値は必ず入力してください\n";
		str += "また、求められた値は近似値でしかありません\n";
		str += "参考程度として、ご使用ください\n\n";
		str += "計算の実行は[EXE]ボタンを押してください\n";
		str += "また、[Copy]ボタンはクリップボードを使用したコピーを、";
		str += "[Paste]ボタンはクリップボードを使用した値入力をできます\n";
		SetDlgItemText(IDC_EDIT5, str);									// 説明表示
	}
}

/**********************************/
void CCalcDlg2::OnPasteData3(void)										//<<< [Paste3]ﾎﾞﾀﾝ押下処理
/**********************************/
{
	m_Edit3.SetSel(0, -1);
	m_Edit3.Paste();													// ﾍﾟｰｽﾄ
}

/*********************************/
void CCalcDlg2::OnCopyData3(void)										//<<< [Copy3]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	m_Edit3.SetSel(0, -1);
	m_Edit3.Copy();														// ｺﾋﾟｰ
}

/********************************/
void CCalcDlg2::OnInputCls(void)										//<<< [Cls]ﾎﾞﾀﾝ押下処理
/********************************/
{
	m_strNumber1 = _T("1"), m_strNumber2 = _T("1");						// 初期化
	m_strNumber3 = _T("1"), m_strNumber4 = _T("");
	m_Number1 = 1.0, m_Number2 = 1.0, m_Number3 = 1.0;
	m_Edit1.SetWindowText(m_strNumber1);								// 初期値表示
	m_Edit2.SetWindowText(m_strNumber2);
	m_Edit3.SetWindowText(m_strNumber3);
	m_Edit4.SetWindowText(m_strNumber4);
}

/*********************************/
void CCalcDlg2::OnCopyData4(void)										//<<< [Copy4]ﾎﾞﾀﾝ押下処理
/*********************************/
{
	m_Edit4.SetSel(0, -1);
	m_Edit4.Copy();														// ｺﾋﾟｰ
}

/*******************************/
void CCalcDlg2::OnExeProc(void)											//<<< [EXE]ﾎﾞﾀﾝ押下処理
/*******************************/
{
	int    num;
	double len;

	CheckInputData();													// 直接入力ﾁｪｯｸ処理
	len = CalcCircumference((m_Number1+m_Number3)/2.0,
                            (m_Number2+m_Number3)/2.0);					// 楕円円周取得
    num = (int)(len/m_Number3);											// 円周に配置できる個数
	m_strNumber4.Format("%d", num);
	m_Edit4.SetWindowText(m_strNumber4);								// 個数表示
}

/**********************************/
void CCalcDlg2::OnChangeCalc(void)										//<<< [Calc]ﾎﾞﾀﾝ押下処理
/**********************************/
{
	UpdateData(TRUE);													// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
	View->SetCalc(m_strNumber1, m_strNumber2,
                  m_strNumber3, m_strNumber4);							// ﾃﾞｰﾀ保存
	DestroyWindow();
	View->EndCalcDlg2();												// ﾀﾞｲｱﾛｸﾞ終了
	if(!View->IsCalcDlg1Show()) {
		View->CreateCalcDlg1();											// 計算機ﾀﾞｲｱﾛｸﾞ1生成
	}
}

/************************************/
void CCalcDlg2::CheckInputData(void)									//<<< 直接入力ﾁｪｯｸ処理
/************************************/
{
	CString str;
	int     ln1, ln2;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
	m_Edit1.GetWindowText(str);											// 入力値1取得
	ln1 = str.GetLength();												// 入力文字数取得
	m_strNumber1 = str.SpanIncluding(_T("-0123456789."));				// 数値のみ抽出
	ln2 = m_strNumber1.GetLength();										// 有効文字数
	if(ln1!=ln2) {
		AfxMessageBox(IDS_ERR_INPUT);									// 数値以外はｴﾗｰ
	}
	m_Edit1.SetWindowText(m_strNumber1);								// 入力値1表示
	m_Number1 = atof(m_strNumber1);										// 入力文字列1を数値変換
	m_Edit2.GetWindowText(str);											// 入力値2取得
	ln1 = str.GetLength();												// 入力文字数取得
	m_strNumber2 = str.SpanIncluding(_T("-0123456789."));				// 数値のみ抽出
	ln2 = m_strNumber2.GetLength();										// 有効文字数
	if(ln1!=ln2) {
		AfxMessageBox(IDS_ERR_INPUT);									// 数値以外はｴﾗｰ
	}
	m_Edit2.SetWindowText(m_strNumber2);								// 入力値2表示
	m_Number2 = atof(m_strNumber2);										// 入力文字列2を数値変換
	m_Edit3.GetWindowText(str);											// 入力値3取得
	ln1 = str.GetLength();												// 入力文字数取得
	m_strNumber3 = str.SpanIncluding(_T("-0123456789."));				// 数値のみ抽出
	ln2 = m_strNumber3.GetLength();										// 有効文字数
	if(ln1!=ln2) {
		AfxMessageBox(IDS_ERR_INPUT);									// 数値以外はｴﾗｰ
	}
	m_Edit3.SetWindowText(m_strNumber3);								// 入力値3表示
	m_Number3 = atof(m_strNumber3);										// 入力文字列3を数値変換
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*******************************************************/
double CCalcDlg2::CalcCircumference(double a, double b)					//<<< 楕円円周取得
/*******************************************************/
{
	int    i;
	double th1, th2, xlen, ylen, len=0.0;

	for(i=0; i<18; i++) {
		th1 = i*PI*5.0/180.0;
		th2 = (i+1)*PI*5.0/180.0;
		xlen = a*(cos(th2)-cos(th1));
		ylen = b*(sin(th2)-sin(th1));
		len += sqrt(xlen*xlen+ylen*ylen);
	}
	len = len*4.0;
	return len;
}

/********************************************************************/
void CCalcDlg2::SetParam(CString a, CString b, CString c, CString d)	//<<< ﾊﾟﾗﾒｰﾀ設定
/********************************************************************/
{
	m_strNumber1 = a, m_strNumber2 = b;
	m_strNumber3 = c, m_strNumber4 = d;
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ復元
}
