/////////////////////////////////////////////////////////////////////////////
// 数値入力ダイアログクラス(CInputDlg)定義
// InputDlg.cpp
//---------------------------------------------------------------------------
// LastEdit: 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3GlbView.h"
#include "InputDlg.h"

BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg ダイアログ

/********************************************************************************/
CInputDlg::CInputDlg(int kind, CWnd* pParent) : CDialog(CInputDlg::IDD, pParent)
/********************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_ParamI1 = 0;
	m_ParamI2 = 0;
	m_Kind = kind;
}

/**************************************************/
void CInputDlg::DoDataExchange(CDataExchange* pDX)						//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	if(m_Kind==1) {														// <ｸﾞﾘｯﾄﾞ間隔(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_ParamI2);
		DDV_MinMaxInt(pDX, m_ParamI2, 1, 50);
	} else if(m_Kind==2) {												// <ｻｲｽﾞ(3)-ﾌﾟﾘﾐﾃｨﾌﾞ>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.0, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.0, 1000.0);
		DDX_Text(pDX, IDC_EDIT3, m_Param3);
		DDV_MinMaxDouble(pDX, m_Param3, 0.0, 1000.0);
	} else if(m_Kind==3) {												// <ｻｲｽﾞ(2)-石枠>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 1000.0);
	} else if(m_Kind==4) {												// <ｻｲｽﾞ(1)-石枠>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
	} else if(m_Kind==5) {												// <厚み(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, -50.0, 50.0);
	} else if(m_Kind==6) {												// <円弧形状(1)-回転体>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 1.0, 360.0);
	} else if(m_Kind==7) {												// <ｻｲｽﾞ(2)+高-宝石ｶｯﾄ>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT3, m_Param3);
		DDV_MinMaxDouble(pDX, m_Param3, 0.0, 100.0);
	} else if(m_Kind==8) {												// <ｻｲｽﾞ(1)+高-宝石ｶｯﾄ>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.0, 100.0);
	} else if(m_Kind==9) {												// <曲線ﾊﾟﾗﾒｰﾀ(2)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 1, 5);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 1.0, 3.0);
	} else if(m_Kind==10) {												// <掃引分割数(1)-掃引体>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 0, View->m_MaxCCtl);
	} else if(m_Kind==11) {												// <回転分割数(1)-回転体>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 3, View->m_MaxCCtl);
	} else if(m_Kind==12) {												// <石枠配置(2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 1.0, 360.0);
		DDX_Text(pDX, IDC_EDIT2, m_ParamI2);
		DDV_MinMaxInt(pDX, m_ParamI2, 2, 100);
	} else if(m_Kind==13) {												// <基準線制御点数(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 2, View->m_MaxCCtl);
	} else if(m_Kind==14||m_Kind==15||m_Kind==16) {						// <基準円ｻｲｽﾞ(2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 1.0, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 1.0, 1000.0);
	} else if(m_Kind==17||m_Kind==19) {									// <ﾘﾝｸﾞｻｲｽﾞ[接合:mm](2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, -100.0, 100.0);
	} else if(m_Kind==18||m_Kind==20) {									// <ﾘﾝｸﾞｻｲｽﾞ[接合:度](2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, -360.0, 360.0);
	} else if(m_Kind==21) {												// <ﾍﾟｰｽﾄ位置(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, -100.0, 100.0);
	} else if(m_Kind==22) {												// <錐分割数(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 1, View->m_MaxCCtl);
	} else if(m_Kind==23) {												// <挿入制御点数(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 1, View->m_MaxCCtl-2);
	} else if(m_Kind==24) {												// <回転対象分割数(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 2, View->m_MaxCCtl);
	} else if(m_Kind==25) {												// <基準面Z位置(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, -100.0, 100.0);
	} else if(m_Kind==26) {												// <描画領域(2)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 10, 2000);
		DDX_Text(pDX, IDC_EDIT2, m_ParamI2);
		DDV_MinMaxInt(pDX, m_ParamI2, 10, 2000);
	} else if(m_Kind==27) {												// <実寸表示補正倍率(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 100.0);
	} else if(m_Kind==28) {												// <基準円制御点数(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 3, View->m_MaxCCtl);
	} else {
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDX_Text(pDX, IDC_EDIT3, m_Param3);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInputDlg メッセージ ハンドラ

/**********************************/
BOOL CInputDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/**********************************/									//    ﾀﾞｲｱﾛｸﾞの初期化
{
	CRect r;

	CDialog::OnInitDialog();
	GetWindowRect(&r);
	Width = r.Width();
	Height = r.Height();
	SetInputDlg();
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/*********************************/
void CInputDlg::SetInputDlg(void)										//<<< ﾀﾞｲｱﾛｸﾞ ﾚｲｱｳﾄ設定
/*********************************/
{
	CString title, str, show, unit;
	BOOL    flg=TRUE;
	int     i, sw;

	if(m_Kind==1) {														// <ｸﾞﾘｯﾄﾞ間隔(1)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "100";
		title.LoadString(IDS_STR14);
		str.LoadString(IDS_STR23);										// 項目名取得(ｸﾞﾘｯﾄﾞ間隔)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR78);										// 項目名取得(目盛間隔)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==2) {												// <ｻｲｽﾞ(3)>
		show = "111";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "111";
		title.LoadString(IDS_STR12);
		SetWindowPos(NULL, 0, 0, Width, Height, SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==3) {												// <ｻｲｽﾞ(2)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "110";
		title.LoadString(IDS_STR12);
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==4) {												// <ｻｲｽﾞ(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "100";
		title.LoadString(IDS_STR12);
		str.LoadString(IDS_STR24);										// 項目名取得(直径)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==5) {												// <厚み(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "100";
		title.LoadString(IDS_STR15);
		str.LoadString(IDS_STR25);										// 項目名取得(幅)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==6) {												// <円弧形状(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "100";
		title.LoadString(IDS_STR56);
		str.LoadString(IDS_STR57);										// 項目名取得(生成領域)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR6);										// 単位取得(度)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// 単位表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==7) {												// <ｻｲｽﾞ(2)+高>
		show = "111";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "111";
		title.LoadString(IDS_STR12);
		str.LoadString(IDS_STR37);										// 項目名取得(高さ)
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, Height, SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==8) {												// <ｻｲｽﾞ(1)+高>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "110";
		title.LoadString(IDS_STR12);
		str.LoadString(IDS_STR24);										// 項目名取得(直径)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR37);										// 項目名取得(高さ)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==9) {												// <曲線ﾊﾟﾗﾒｰﾀ(2)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR8);
		str.LoadString(IDS_STR22);										// 項目名取得(曲線分割数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR46);										// 項目名取得(曲線重み係数)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==10) {												// <掃引分割数(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR47);
		str.LoadString(IDS_STR48);										// 項目名取得(掃引分割数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==11) {												// <回転分割数(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR49);
		str.LoadString(IDS_STR50);										// 項目名取得(回転分割数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==12) {												// <石枠配置(2)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "110";
		title.LoadString(IDS_STR53);
		str.LoadString(IDS_STR54);										// 項目名取得(配置領域)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR55);										// 項目名取得(総配置数)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR6);										// 単位取得(度)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// 単位表示
		str.LoadString(IDS_STR13);										// 単位取得(個)
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);					// 単位表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==13) {												// <基準線制御点数(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR29);
		str.LoadString(IDS_STR76);										// 項目名取得(制御点数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==14||m_Kind==15||m_Kind==16) {						// <ｻｲｽﾞ(2)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "110";
		title.LoadString(IDS_STR77);
		if(m_Kind==14) {												// 項目名取得(X/Y/Z)
			str.LoadString(IDS_STR62);
		} else if(m_Kind==15) {
			str.LoadString(IDS_STR63);
		} else {
			str.LoadString(IDS_STR64);
		}
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		if(m_Kind==14) {												// 項目名取得(Y/Z/X)
			str.LoadString(IDS_STR63);
		} else if(m_Kind==15) {
			str.LoadString(IDS_STR64);
		} else {
			str.LoadString(IDS_STR62);
		}
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==17||m_Kind==18) {									// <ﾘﾝｸﾞｻｲｽﾞ号数(2)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "110";
		title.LoadString(IDS_STR16);
		str.LoadString(IDS_STR26);										// 項目名取得(ﾘﾝｸﾞ号数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR69);										// 項目名取得(接合間隔)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR28);										// 単位取得(号)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// 単位表示
		if(m_Kind==18) {
			str.LoadString(IDS_STR6);									// 単位取得(度)
			GetDlgItem(IDC_STATIC5)->SetWindowText(str);				// 単位表示
		}
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==19||m_Kind==20) {									// <ﾘﾝｸﾞｻｲｽﾞ直径(2)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "110";
		title.LoadString(IDS_STR16);
		str.LoadString(IDS_STR27);										// 項目名取得(ﾘﾝｸﾞ号数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR69);										// 項目名取得(接合間隔)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		if(m_Kind==20) {
			str.LoadString(IDS_STR6);									// 単位取得(度)
			GetDlgItem(IDC_STATIC5)->SetWindowText(str);				// 単位表示
		}
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==21) {												// <ﾍﾟｰｽﾄ位置(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "100";
		title.LoadString(IDS_STR67);
        str.LoadString(IDS_STR68);										// 項目名取得(ﾍﾟｰｽﾄ位置)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==22) {												// <錐分割数(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR51);
		str.LoadString(IDS_STR52);										// 項目名取得(錐分割数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==23) {												// <挿入制御点数(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR83);
		str.LoadString(IDS_STR84);										// 項目名取得(挿入制御点数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==24) {												// <回転対象分割数(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR90);
		str.LoadString(IDS_STR91);										// 項目名取得(対象分割数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==25) {												// <基準面Z位置(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "100";
		title.LoadString(IDS_STR73);
		str.LoadString(IDS_STR74);										// 項目名取得(基準面Z位置)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==26) {												// <描画領域(2)>
		show = "110";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "110";
		title.LoadString(IDS_STR32);
		str.LoadString(IDS_STR33);										// 項目名取得(描画領域幅)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR34);										// 項目名取得(描画領域高さ)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR38);										// 単位取得(pixel)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// 単位表示
		str.LoadString(IDS_STR38);										// 単位取得(pixel)
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);					// 単位表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==27) {												// <実寸表示補正倍率(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "100";
		title.LoadString(IDS_STR21);
        str.LoadString(IDS_STR30);										// 項目名取得(補正倍率)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		str.LoadString(IDS_STR7);										// 単位取得(倍)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// 単位表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==28) {												// <基準円制御点数(1)>
		show = "100";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "000";
		title.LoadString(IDS_STR29);
		str.LoadString(IDS_STR76);										// 項目名取得(制御点数)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// 項目名表示
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else {
		show = "111";													// 表示ｺﾝﾄﾛｰﾙ設定
		unit = "111";
		SetWindowPos(NULL, 0, 0, Width, Height, SWP_NOMOVE|SWP_NOZORDER);
		flg = FALSE;
	}
	for(i=0; i<3; i++) {												// ｺﾝﾄﾛｰﾙ表示
		sw = show[i]=='1'?1:0;
		GetDlgItem(IDC_STATIC1+i)->ShowWindow(sw?SW_SHOW:SW_HIDE);
		GetDlgItem(IDC_EDIT1+i)->ShowWindow(sw?SW_SHOW:SW_HIDE);
		sw = unit[i]=='1'?1:0;
		GetDlgItem(IDC_STATIC4+i)->ShowWindow(sw?SW_SHOW:SW_HIDE);
	}
	if(flg) {
		SetWindowText(title);											// ﾀｲﾄﾙ表示
	}
}

/***************************************************************/
void CInputDlg::SetValue(double prm1, double prm2, double prm3)			//<<< 小数ﾊﾟﾗﾒｰﾀ設定
/***************************************************************/
{
	m_Param1 = prm1;
	m_Param2 = prm2;
	m_Param3 = prm3;
}

/************************************************************/
void CInputDlg::SetValue(int prm1, double prm2, double prm3)			//<<< 整数･小数ﾊﾟﾗﾒｰﾀ設定
/************************************************************/
{
	m_ParamI1 = prm1;
	m_Param2  = prm2;
	m_Param3  = prm3;
}

/***********************************************/
void CInputDlg::SetValue(double prm1, int prm2)							//<<< 小数･整数ﾊﾟﾗﾒｰﾀ設定
/***********************************************/
{
	m_Param1  = prm1;
	m_ParamI2 = prm2;
}

/********************************************/
void CInputDlg::SetValue(int prm1, int prm2)							//<<< 整数ﾊﾟﾗﾒｰﾀ設定
/********************************************/
{
	m_ParamI1 = prm1;
	m_ParamI2 = prm2;
}

/******************************************************************/
void CInputDlg::GetValue(double* prm1, double* prm2, double* prm3)		//<<< 小数ﾊﾟﾗﾒｰﾀ(3)取得
/******************************************************************/
{
	*prm1 = m_Param1;
	*prm2 = m_Param2;
	*prm3 = m_Param3;
}

/****************************************************/
void CInputDlg::GetValue(double* prm1, double* prm2)					//<<< 小数ﾊﾟﾗﾒｰﾀ(2)取得
/****************************************************/
{
	*prm1 = m_Param1;
	*prm2 = m_Param2;
}

/*************************************************/
void CInputDlg::GetValue(int* prm1, double* prm2)						//<<< 整数･小数ﾊﾟﾗﾒｰﾀ(2)取得
/*************************************************/
{
	*prm1 = m_ParamI1;
	*prm2 = m_Param2;
}

/*************************************************/
void CInputDlg::GetValue(double* prm1, int* prm2)						//<<< 整数･小数ﾊﾟﾗﾒｰﾀ(2)取得
/*************************************************/
{
	*prm1 = m_Param1;
	*prm2 = m_ParamI2;
}

/**********************************************/
void CInputDlg::GetValue(int* prm1, int* prm2)							//<<< 整数ﾊﾟﾗﾒｰﾀ(2)取得
/**********************************************/
{
	*prm1 = m_ParamI1;
	*prm2 = m_ParamI2;
}

/**************************************/
void CInputDlg::GetValue(double* prm1)									//<<< 小数ﾊﾟﾗﾒｰﾀ(1)取得
/**************************************/
{
	*prm1 = m_Param1;
}

/***********************************/
void CInputDlg::GetValue(int* prm1)										//<<< 整数ﾊﾟﾗﾒｰﾀ(1)取得
/***********************************/
{
	*prm1 = m_ParamI1;
}
