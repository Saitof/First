/////////////////////////////////////////////////////////////////////////////
// レイトレーシング環境設定プロパティシートクラス定義
// SetRayEnv.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/23 by M.Fukushima
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
#include "SetRayEnv.h"
#include "DataAccess3.h"
#include "NumFunc.h"

///////////////////////////////////////////////////////////////////////////////
// CRayEnvDlg1 ダイアログ

BEGIN_MESSAGE_MAP(CRayEnvDlg1, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO19, OnColorNo)
	ON_COMMAND_EX_RANGE(IDC_RADIO20, IDC_RADIO21, OnTraceModel)
	ON_EN_CHANGE(IDC_EDIT1, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT2, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT3, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT4, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT5, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT6, OnSetColor)
    ON_EN_CHANGE(IDC_EDIT7, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT8, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT9, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT10, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT11, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT12, OnSetColor)
END_MESSAGE_MAP()

/************************************************************/
CRayEnvDlg1::CRayEnvDlg1() : CPropertyPage(CRayEnvDlg1::IDD)
/************************************************************/
{
    int i;

	m_No = 0;
    m_Check = 0;
    for(i=0; i<ATMAX; i++) {
        GetRayTraceColor(i, &Atr_Sv[i]);                                // ﾚｲ･ﾄﾚｰｽ表面属性取得
    }
    m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CRayEnvDlg1::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_No);
	DDX_Radio(pDX, IDC_RADIO20, m_Model);
	DDX_Text(pDX, IDC_EDIT1, m_RCol[0]);
	DDV_MinMaxInt(pDX, m_RCol[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN1, m_Spin1);
	DDX_Text(pDX, IDC_EDIT2, m_GCol[0]);
	DDV_MinMaxInt(pDX, m_GCol[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN2, m_Spin2);
	DDX_Text(pDX, IDC_EDIT3, m_BCol[0]);
	DDV_MinMaxInt(pDX, m_BCol[0], 0, 255);
	DDX_Control(pDX, IDC_SPIN3, m_Spin3);
	DDX_Control(pDX, IDC_PIC1, m_Pic[0]);
    DDX_Text(pDX, IDC_EDIT4, m_RCol[1]);
	DDV_MinMaxInt(pDX, m_RCol[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN4, m_Spin4);
	DDX_Text(pDX, IDC_EDIT5, m_GCol[1]);
	DDV_MinMaxInt(pDX, m_GCol[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN5, m_Spin5);
	DDX_Text(pDX, IDC_EDIT6, m_BCol[1]);
	DDV_MinMaxInt(pDX, m_BCol[1], 0, 255);
	DDX_Control(pDX, IDC_SPIN6, m_Spin6);
	DDX_Control(pDX, IDC_PIC2, m_Pic[1]);
    DDX_Text(pDX, IDC_EDIT7, m_RCol[2]);
	DDV_MinMaxInt(pDX, m_RCol[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN7, m_Spin7);
	DDX_Text(pDX, IDC_EDIT8, m_GCol[2]);
	DDV_MinMaxInt(pDX, m_GCol[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN8, m_Spin8);
	DDX_Text(pDX, IDC_EDIT9, m_BCol[2]);
	DDV_MinMaxInt(pDX, m_BCol[2], 0, 255);
	DDX_Control(pDX, IDC_SPIN9, m_Spin9);
	DDX_Control(pDX, IDC_PIC3, m_Pic[2]);
    DDX_Text(pDX, IDC_EDIT10, m_RCol[3]);
	DDV_MinMaxInt(pDX, m_RCol[3], 0, 255);
	DDX_Control(pDX, IDC_SPIN10, m_Spin10);
	DDX_Text(pDX, IDC_EDIT11, m_GCol[3]);
	DDV_MinMaxInt(pDX, m_GCol[3], 0, 255);
	DDX_Control(pDX, IDC_SPIN11, m_Spin11);
	DDX_Text(pDX, IDC_EDIT12, m_BCol[3]);
	DDV_MinMaxInt(pDX, m_BCol[3], 0, 255);
	DDX_Control(pDX, IDC_SPIN12, m_Spin12);
	DDX_Control(pDX, IDC_PIC4, m_Pic[3]);
    DDX_Check(pDX, IDC_CHECK1, m_Trace);
    DDX_Text(pDX, IDC_EDIT13, m_N);
    DDV_MinMaxDouble(pDX, m_N, 0.0, 20.0);
    DDX_Control(pDX, IDC_EDIT14, m_Edit14);
    DDX_Text(pDX, IDC_EDIT14, m_M1);
    DDV_MinMaxInt(pDX, m_M1, 0, 30);
	DDX_Control(pDX, IDC_SPIN13, m_Spin13);
    DDX_Control(pDX, IDC_EDIT15, m_Edit15);
    DDX_Text(pDX, IDC_EDIT15, m_M2);
    DDV_MinMaxDouble(pDX, m_M2, 0.0, 1.0);
    DDX_Control(pDX, IDC_EDIT16, m_Edit16);
    DDX_Text(pDX, IDC_EDIT16, m_NR);
    DDV_MinMaxDouble(pDX, m_NR, 0.0, 20.0);
    DDX_Control(pDX, IDC_EDIT17, m_Edit17);
    DDX_Text(pDX, IDC_EDIT17, m_NG);
    DDV_MinMaxDouble(pDX, m_NG, 0.0, 20.0);
    DDX_Control(pDX, IDC_EDIT18, m_Edit18);
    DDX_Text(pDX, IDC_EDIT18, m_NB);
    DDV_MinMaxDouble(pDX, m_NB, 0.0, 20.0);
    DDX_Control(pDX, IDC_STATIC1, m_Static1);
    DDX_Control(pDX, IDC_STATIC2, m_Static2);
    DDX_Control(pDX, IDC_STATIC3, m_Static3);
    DDX_Control(pDX, IDC_STATIC4, m_Static4);
    DDX_Control(pDX, IDC_STATIC5, m_Static5);
    DDX_Control(pDX, IDC_STATIC6, m_Static6);
    DDX_Control(pDX, IDC_STATIC7, m_Static7);
    DDX_Control(pDX, IDC_STATIC8, m_Static8);
    DDX_Control(pDX, IDC_STATIC9, m_Static9);
}

/*******************************/
void CRayEnvDlg1::OnPaint(void)											//<<< WM_PAINT ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	CPaintDC dc(this);													// 描画用ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ
	ColorPaint(&dc);													// ｶﾗｰﾍﾟｲﾝﾄ
}

/************************************/
BOOL CRayEnvDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
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
	m_Spin10.SetRange(0, 255);
	m_Spin11.SetRange(0, 255);
	m_Spin12.SetRange(0, 255);
	m_Spin13.SetRange(0, 30);
	for(i=0; i<4; i++) {
        m_Pic[i].GetWindowRect(m_SQ[i]);                                // 矩形ｽｸﾘｰﾝ座標取得
		ScreenToClient(m_SQ[i]);										// ｸﾗｲｱﾝﾄ座標へ変換
	}
    GetTraceColor();                                                    // ﾚｲ･ﾄﾚｰｽ表面属性取得
	return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CRayEnvDlg1::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
    FILE*   fp;
    ATTRIBT atr;
    CString path;
    int     i, i1, i2, i3, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+TCL_NAME;                                      // ﾚｲ･ﾄﾚｰｽ初期ｶﾗｰﾌｧｲﾙ名
    if(fopen_s(&fp, path, "r")==0) {                                    // ﾌｧｲﾙOPEN
        for(i=0; i<ATMAX; i++) {
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // 環境光
                er = 1; break;
            }
            atr.ka = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // 拡散反射光
                er = 1; break;
            }
            atr.kd = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // 鏡面反射係数
                er = 1; break;
            }
            atr.ks = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // 透過調整係数
                er = 1; break;
            }
            atr.ct = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%f", &i1, &f1)!=2) {
                er = 1; break;
            }
            atr.trace = i1;                                             // 映り込み有無
            atr.n     = ChangeFltToDbl(f1);                             // 視線屈折率
            if(fscanf_s(fp, "%d%d%f", &i1, &i2, &f1)!=3) {
                er = 1; break;
            }
            atr.model = i1;                                             // ﾓﾃﾞﾙ(0:Phong, 1:Cook-T)
            atr.m1    = i2;                                             // ﾊｲﾗｲﾄの広がり1(Phong)
            atr.m2    = ChangeFltToDbl(f1);                             // ﾊｲﾗｲﾄの広がり2(Cook-T)
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {              // 分光屈折率
                er = 1; break;
            }
            atr.nr = ChangeFltToDbl(f1);
            atr.ng = ChangeFltToDbl(f2);
            atr.nb = ChangeFltToDbl(f3);
            Atr_Sv[i] = atr;
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_CANT_READ_FILE);                              // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
    }
    GetTraceColor();                                                    // ﾚｲ･ﾄﾚｰｽ表面属性取得
	RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/**********************************/
void CRayEnvDlg1::OnSaveFile(void)                                      //<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
    FILE*   fp;
    CString path;
    ATTRIBT atr;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    UpdateData(TRUE);                                                   // ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
    SetTraceColor();                                                    // ﾚｲ･ﾄﾚｰｽ表面属性設定
    path = GetAppPath(0)+RT1_NAME;                                      // ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名1
    if(fopen_s(&fp, path, "w")==0) {                                    // ﾌｧｲﾙOPEN
        for(i=0; i<ATMAX; i++) {
            atr = Atr_Sv[i];
            r = GetRValue(atr.ka), g = GetGValue(atr.ka), b = GetBValue(atr.ka);
            if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
                er = 1; break;
            }
            r = GetRValue(atr.kd), g = GetGValue(atr.kd), b = GetBValue(atr.kd);
            if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
                er = 1; break;
            }
            r = GetRValue(atr.ks), g = GetGValue(atr.ks), b = GetBValue(atr.ks);
            if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
                er = 1; break;
            }
            r = GetRValue(atr.ct), g = GetGValue(atr.ct), b = GetBValue(atr.ct);
            if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
                er = 1; break;
            }
            f1 = (float)atr.n;
            if(fprintf(fp, "%d %f\n", atr.trace, f1)<0) {
                er = 1; break;
            }
            f1 = (float)atr.m2;
            if(fprintf(fp, "%d %d %f\n", atr.model, atr.m1, f1)<0) {
                er = 1; break;
            }
            f1 = (float)atr.nr, f2 = (float)atr.ng, f3 = (float)atr.nb;
            if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
                er = 1; break;
            }
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_ERR_FILEWRITE);                               // ｢ﾌｧｲﾙ保存NG｣
    }
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*************************************/
void CRayEnvDlg1::OnRestoreFile(void)                                   //<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
    FILE*   fp;
    CString path;
    ATTRIBT atr;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+RT1_NAME;                                      // ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名1
    if(fopen_s(&fp, path, "r")==0) {                                    // ﾌｧｲﾙOPEN
        for(i=0; i<ATMAX; i++) {
            if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
                er = 1; break;
            }
            atr.ka = RGB(r, g, b);
            if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
                er = 1; break;
            }
            atr.kd = RGB(r, g, b);
            if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
                er = 1; break;
            }
            atr.ks = RGB(r, g, b);
            if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
                er = 1; break;
            }
            atr.ct = RGB(r, g, b);
            if(fscanf_s(fp, "%d%f", &atr.trace, &f1)!=2) {
                er = 1; break;
            }
            atr.n = ChangeFltToDbl(f1);
            if(fscanf_s(fp, "%d%d%f", &atr.model, &atr.m1, &f1)!=3) {
                er = 1; break;
            }
            atr.m2 = ChangeFltToDbl(f1);
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
                er = 1; break;
            }
            atr.nr = ChangeFltToDbl(f1);
            atr.ng = ChangeFltToDbl(f2);
            atr.nb = ChangeFltToDbl(f3);
            Atr_Sv[i] = atr;
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
        if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
        }
    } else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ｢保存ﾌｧｲﾙなし｣
    }
    GetTraceColor();                                                    // ﾚｲ･ﾄﾚｰｽ表面属性取得
    RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*************************************/
BOOL CRayEnvDlg1::OnColorNo(UINT nID)									//<<< ｶﾗｰNoﾎﾞﾀﾝ押下処理
/*************************************/
{
    int no;

    no = m_No;                                                          // 変更前ｶﾗｰNo保存
    UpdateData(TRUE);                                                   // ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
    m_No = no;                                                          // 変更前ｶﾗｰNo
    SetTraceColor();                                                    // ﾚｲ･ﾄﾚｰｽ表面属性設定
    m_No = nID-IDC_RADIO1;                                              // 変更後ｶﾗｰNo
    GetTraceColor();                                                    // ﾚｲ･ﾄﾚｰｽ表面属性取得
	RedrawWindow(NULL, NULL);
	return TRUE;
}

/****************************************/
BOOL CRayEnvDlg1::OnTraceModel(UINT nID)                                //<<< ﾄﾚｰｽ･ﾓﾃﾞﾙﾎﾞﾀﾝ押下処理
/****************************************/
{
    m_Model = nID-IDC_RADIO20;
    CheckModel(m_Model);
    return TRUE;
}

/**********************************/
void CRayEnvDlg1::OnSetColor(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理
/**********************************/
{
	if(++m_Check>12) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		RedrawWindow(NULL, NULL);
	}
}

/******************************************/
void CRayEnvDlg1::ColorPaint(CPaintDC *dc)								//<<< ｶﾗｰﾍﾟｲﾝﾄ
/******************************************/
{
	int	i;

    for(i=0; i<4; i++) {
    	CBrush brushNew(RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    	CPen penNew(0, 1, RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CPen* pPenOld = dc->SelectObject(&penNew);
    	dc->Rectangle(m_SQ[i].left, m_SQ[i].top,
                      m_SQ[i].right, m_SQ[i].bottom);                   // 領域描画
    	dc->SelectObject(pBrushOld);
    	dc->SelectObject(pPenOld);
    	brushNew.DeleteObject();
    	penNew.DeleteObject();
    }
}

/*************************************/
void CRayEnvDlg1::GetTraceColor(void)                                   //<<< ﾚｲ･ﾄﾚｰｽ表面属性取得
/*************************************/
{
    m_RCol[0] = GetRValue(Atr_Sv[m_No].ka);
    m_GCol[0] = GetGValue(Atr_Sv[m_No].ka);
    m_BCol[0] = GetBValue(Atr_Sv[m_No].ka);
    m_RCol[1] = GetRValue(Atr_Sv[m_No].kd);
    m_GCol[1] = GetGValue(Atr_Sv[m_No].kd);
    m_BCol[1] = GetBValue(Atr_Sv[m_No].kd);
    m_RCol[2] = GetRValue(Atr_Sv[m_No].ks);
    m_GCol[2] = GetGValue(Atr_Sv[m_No].ks);
    m_BCol[2] = GetBValue(Atr_Sv[m_No].ks);
    m_RCol[3] = GetRValue(Atr_Sv[m_No].ct);
    m_GCol[3] = GetGValue(Atr_Sv[m_No].ct);
    m_BCol[3] = GetBValue(Atr_Sv[m_No].ct);
    m_Trace   = Atr_Sv[m_No].trace;
    m_N       = Atr_Sv[m_No].n;
    m_Model   = Atr_Sv[m_No].model;
    m_M1      = Atr_Sv[m_No].m1;
    m_M2      = Atr_Sv[m_No].m2;
    m_NR      = Atr_Sv[m_No].nr;
    m_NG      = Atr_Sv[m_No].ng;
    m_NB      = Atr_Sv[m_No].nb;
    CheckModel(m_Model);                                                // ﾄﾚｰｽ･ﾓﾃﾞﾙﾁｪｯｸ
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
}

/*************************************/
void CRayEnvDlg1::SetTraceColor(void)                                   //<<< ﾚｲ･ﾄﾚｰｽ表面属性設定
/*************************************/
{
    Atr_Sv[m_No].ka    = RGB(m_RCol[0], m_GCol[0], m_BCol[0]);
    Atr_Sv[m_No].kd    = RGB(m_RCol[1], m_GCol[1], m_BCol[1]);
    Atr_Sv[m_No].ks    = RGB(m_RCol[2], m_GCol[2], m_BCol[2]);
    Atr_Sv[m_No].ct    = RGB(m_RCol[3], m_GCol[3], m_BCol[3]);
    Atr_Sv[m_No].trace = m_Trace;
    Atr_Sv[m_No].n     = m_N;
    Atr_Sv[m_No].model = m_Model;
    Atr_Sv[m_No].m1    = m_M1;
    Atr_Sv[m_No].m2    = m_M2;
    Atr_Sv[m_No].nr    = m_NR;
    Atr_Sv[m_No].ng    = m_NG;
    Atr_Sv[m_No].nb    = m_NB;
}

/***************************************/
void CRayEnvDlg1::CheckModel(int model)                                 //<<< ﾄﾚｰｽ･ﾓﾃﾞﾙﾁｪｯｸ
/***************************************/
{
    m_Static1.EnableWindow(!model);
    m_Static2.EnableWindow(!model);
    m_Edit14.EnableWindow(!model);
    m_Spin13.EnableWindow(!model);
    m_Static3.EnableWindow(model);
    m_Static4.EnableWindow(model);
    m_Edit15.EnableWindow(model);
    m_Static5.EnableWindow(model);
    m_Static6.EnableWindow(model);
    m_Static7.EnableWindow(model);
    m_Static8.EnableWindow(model);
    m_Static9.EnableWindow(model);
    m_Edit16.EnableWindow(model);
    m_Edit17.EnableWindow(model);
    m_Edit18.EnableWindow(model);
}

///////////////////////////////////////////////////////////////////////////////
// CRayEnvDlg2 ダイアログ

BEGIN_MESSAGE_MAP(CRayEnvDlg2, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_COMMAND_EX_RANGE(IDC_RADIO1, IDC_RADIO8, OnLightNo)
	ON_COMMAND_EX_RANGE(IDC_RADIO9, IDC_RADIO10, OnLightType)
	ON_EN_CHANGE(IDC_EDIT1, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT2, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT3, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT4, OnSetLightPos)
	ON_EN_CHANGE(IDC_EDIT5, OnSetLightPos)
	ON_EN_CHANGE(IDC_EDIT6, OnSetLightPos)
END_MESSAGE_MAP()

/************************************************************/
CRayEnvDlg2::CRayEnvDlg2() : CPropertyPage(CRayEnvDlg2::IDD)
/************************************************************/
{
    int i;

    m_No = 0;
    m_Check1 = 0;
    m_Check2 = 0;
    GetObjCent(ObjCent);                                                // 表示立体ｾﾝﾀｰ取得
    for(i=0; i<LGMAX; i++) {
        GetRayTraceLight(i, &Lgt_Sv[i]);                                // ﾚｲ･ﾄﾚｰｽ光源取得
    }
    GetTraceLight();                                                    // ﾚｲ･ﾄﾚｰｽ光源取得
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置設定
    m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CRayEnvDlg2::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_RADIO1, m_No);
    DDX_Check(pDX, IDC_CHECK1, m_Flg);
    DDX_Radio(pDX, IDC_RADIO9, m_Type);
    DDX_Control(pDX, IDC_STATIC1, m_Static1);
    DDX_Radio(pDX, IDC_RADIO11, m_Los);
    DDX_Control(pDX, IDC_RADIO11, m_Button11);
    DDX_Control(pDX, IDC_RADIO12, m_Button12);
    DDX_Control(pDX, IDC_RADIO13, m_Button13);
    DDX_Text(pDX, IDC_EDIT1, m_Lcr);
    DDV_MinMaxInt(pDX, m_Lcr, 0, 255);
    DDX_Control(pDX, IDC_SPIN1, m_Spin1);
    DDX_Text(pDX, IDC_EDIT2, m_Lcg);
    DDV_MinMaxInt(pDX, m_Lcg, 0, 255);
    DDX_Control(pDX, IDC_SPIN2, m_Spin2);
    DDX_Text(pDX, IDC_EDIT3, m_Lcb);
    DDV_MinMaxInt(pDX, m_Lcb, 0, 255);
    DDX_Control(pDX, IDC_SPIN3, m_Spin3);
    DDX_Control(pDX, IDC_PIC1, m_Pic);
    DDX_Text(pDX, IDC_EDIT4, m_Lvx);
    DDV_MinMaxDouble(pDX, m_Lvx, -1000.0, 1000.0);
    DDX_Control(pDX, IDC_SPIN4, m_Spin4);
    DDX_Text(pDX, IDC_EDIT5, m_Lvy);
    DDV_MinMaxDouble(pDX, m_Lvy, -1000.0, 1000.0);
    DDX_Control(pDX, IDC_SPIN5, m_Spin5);
    DDX_Text(pDX, IDC_EDIT6, m_Lvz);
    DDV_MinMaxDouble(pDX, m_Lvz, 0.0, 1000.0);
    DDX_Control(pDX, IDC_SPIN6, m_Spin6);
}

/*******************************/
void CRayEnvDlg2::OnPaint(void)											//<<< WM_PAINT ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	CPaintDC dc(this);													// 描画用ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ
	ColorPaint(&dc);													// ｶﾗｰﾍﾟｲﾝﾄ
}

/************************************/
BOOL CRayEnvDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CPropertyPage::OnInitDialog();
    m_Spin1.SetRange(0, 255);
    m_Spin2.SetRange(0, 255);
    m_Spin3.SetRange(0, 255);
    m_Spin4.SetRange(-1000, 1000);
    m_Spin5.SetRange(-1000, 1000);
    m_Spin6.SetRange(0, 1000);
    m_Pic.GetWindowRect(m_SQ);                                          // 矩形ｽｸﾘｰﾝ座標取得
    ScreenToClient(m_SQ);                                               // ｸﾗｲｱﾝﾄ座標へ変換
    return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CRayEnvDlg2::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
    FILE*   fp;
    LIGHT   lgt;
    CString path;
    int     i, i1, i2, i3, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+LGT_NAME;                                      // ﾚｲ･ﾄﾚｰｽ初期光源ﾌｧｲﾙ名
    if(fopen_s(&fp, path, "r")==0) {                                    // ﾌｧｲﾙOPEN
        for(i=0; i<LGMAX; i++) {
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {
                er = 1; break;
            }
            lgt.flg  = i1;                                              // 有効ﾌﾗｸﾞ
            lgt.type = i2;                                              // 光源ﾀｲﾌﾟ
            lgt.k    = i3;                                              // 光源減衰ﾀｲﾌﾟ
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // 光源RGB
                er = 1; break;
            }
            lgt.lc = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {              // 光源座標位置
                er = 1; break;
            }
            lgt.x1 = ChangeFltToDbl(f1);
            lgt.y1 = ChangeFltToDbl(f2);
            lgt.z1 = ChangeFltToDbl(f3);
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {              // 光源視線位置
                er = 1; break;
            }
            lgt.x2 = ChangeFltToDbl(f1);
            lgt.y2 = ChangeFltToDbl(f2);
            lgt.z2 = ChangeFltToDbl(f3);
            Lgt_Sv[i] = lgt;
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_CANT_READ_FILE);                              // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
    }
    GetTraceLight();                                                    // ﾚｲ･ﾄﾚｰｽ光源取得
    CheckType();                                                        // 光源ﾀｲﾌﾟﾁｪｯｸ
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置設定
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
    View->Display(0);                                                   // 再描画
	RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/**********************************/
void CRayEnvDlg2::OnSaveFile(void)                                      //<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
    FILE*   fp;
    CString path;
    LIGHT   lgt;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    UpdateData(TRUE);                                                   // ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
    SetTraceLight();                                                    // ﾚｲ･ﾄﾚｰｽ光源設定
    path = GetAppPath(0)+RT2_NAME;                                      // ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名2
    if(fopen_s(&fp, path, "w")==0) {                                    // ﾌｧｲﾙOPEN
        for(i=0; i<LGMAX; i++) {
            lgt = Lgt_Sv[i];
            if(fprintf(fp, "%d %d %d\n", lgt.flg, lgt.type, lgt.k)<0) {
                er = 1; break;
            }
            r = GetRValue(lgt.lc), g = GetGValue(lgt.lc), b = GetBValue(lgt.lc);
            if(fprintf(fp, "%d %d %d\n", r, g, b)<0) {
                er = 1; break;
            }
            f1 = (float)lgt.x1, f2 = (float)lgt.y1, f3 = (float)lgt.z1;
            if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
                er = 1; break;
            }
            f1 = (float)lgt.x2, f2 = (float)lgt.y2, f3 = (float)lgt.z2;
            if(fprintf(fp, "%f %f %f\n", f1, f2, f3)<0) {
                er = 1; break;
            }
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_ERR_FILEWRITE);                               // ｢ﾌｧｲﾙ保存NG｣
    }
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*************************************/
void CRayEnvDlg2::OnRestoreFile(void)                                   //<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
    FILE*   fp;
    CString path;
    LIGHT   lgt;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+RT2_NAME;                                      // ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名2
    if(fopen_s(&fp, path, "r")==0) {                                    // ﾌｧｲﾙOPEN
        for(i=0; i<LGMAX; i++) {
            if(fscanf_s(fp, "%d%d%d", &lgt.flg, &lgt.type, &lgt.k)!=3) {
                er = 1; break;
            }
            if(fscanf_s(fp, "%d%d%d", &r, &g, &b)!=3) {
                er = 1; break;
            }
            lgt.lc = RGB(r, g, b);
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
                er = 1; break;
            }
            lgt.x1 = ChangeFltToDbl(f1);
            lgt.y1 = ChangeFltToDbl(f2);
            lgt.z1 = ChangeFltToDbl(f3);
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {
                er = 1; break;
            }
            lgt.x2 = ChangeFltToDbl(f1);
            lgt.y2 = ChangeFltToDbl(f2);
            lgt.z2 = ChangeFltToDbl(f3);
            Lgt_Sv[i] = lgt;
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
        if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
        }
    } else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ｢保存ﾌｧｲﾙなし｣
    }
    GetTraceLight();                                                    // ﾚｲ･ﾄﾚｰｽ光源取得
    CheckType();                                                        // 光源ﾀｲﾌﾟﾁｪｯｸ
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置設定
    UpdateData(FALSE);                                                  // ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
    View->Display(0);                                                   // 再描画
    RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*************************************/
BOOL CRayEnvDlg2::OnLightNo(UINT nID)                                   //<<< 光源Noﾎﾞﾀﾝ押下処理
/*************************************/
{
    int no;

    no = m_No;                                                          // 変更前光源No保存
    UpdateData(TRUE);                                                   // ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
    m_No = no;                                                          // 変更前光源No
    SetTraceLight();                                                    // ﾚｲ･ﾄﾚｰｽ光源設定
    m_No = nID-IDC_RADIO1;                                              // 変更後光源No
    GetTraceLight();                                                    // ﾚｲ･ﾄﾚｰｽ光源取得
    CheckType();                                                        // 光源ﾀｲﾌﾟﾁｪｯｸ
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置設定
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
    View->Display(0);                                                   // 再描画
	RedrawWindow(NULL, NULL);
	return TRUE;
}

/***************************************/
BOOL CRayEnvDlg2::OnLightType(UINT nID)                                 //<<< 光源ﾀｲﾌﾟﾎﾞﾀﾝ押下処理
/***************************************/
{
    m_Type = nID-IDC_RADIO9;
    CheckType();                                                        // 光源ﾀｲﾌﾟﾁｪｯｸ
    return TRUE;
}

/**********************************/
void CRayEnvDlg2::OnSetColor(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理
/**********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		RedrawWindow(NULL, NULL);
	}
}

/*************************************/
void CRayEnvDlg2::OnSetLightPos(void)                                   //<<< 光源位置設定ﾎﾞﾀﾝ押下処理
/*************************************/
{
	if(++m_Check2>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
        SetTraceLight();                                                // ﾚｲ･ﾄﾚｰｽ光源設定
        View->SetLightPos(m_No, Lgt_Sv, ObjCent);                       // ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置設定
        View->Display(0);                                               // 再描画
	}
}

/******************************************/
void CRayEnvDlg2::ColorPaint(CPaintDC *dc)								//<<< ｶﾗｰﾍﾟｲﾝﾄ
/******************************************/
{
    CBrush brushNew(RGB(m_Lcr, m_Lcg, m_Lcb));
   	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    CPen penNew(0, 1, RGB(m_Lcr, m_Lcg, m_Lcb));
    CPen* pPenOld = dc->SelectObject(&penNew);
    dc->Rectangle(m_SQ.left, m_SQ.top, m_SQ.right, m_SQ.bottom);        // 領域描画
    dc->SelectObject(pBrushOld);
    dc->SelectObject(pPenOld);
    brushNew.DeleteObject();
    penNew.DeleteObject();
}

/*************************************/
void CRayEnvDlg2::GetTraceLight(void)                                   //<<< ﾚｲ･ﾄﾚｰｽ光源取得
/*************************************/
{
    m_Flg  = Lgt_Sv[m_No].flg;
    m_Type = Lgt_Sv[m_No].type;
    m_Los  = Lgt_Sv[m_No].k;
    m_Lcr  = GetRValue(Lgt_Sv[m_No].lc);
    m_Lcg  = GetGValue(Lgt_Sv[m_No].lc);
    m_Lcb  = GetBValue(Lgt_Sv[m_No].lc);
    m_Lvx  = Lgt_Sv[m_No].x1;
    m_Lvy  = Lgt_Sv[m_No].y1;
    m_Lvz  = Lgt_Sv[m_No].z1;
}

/*************************************/
void CRayEnvDlg2::SetTraceLight(void)                                   //<<< ﾚｲ･ﾄﾚｰｽ光源設定
/*************************************/
{
    Lgt_Sv[m_No].flg  = m_Flg;
    Lgt_Sv[m_No].type = m_Type;
    Lgt_Sv[m_No].k    = m_Los;
    Lgt_Sv[m_No].lc   = RGB(m_Lcr, m_Lcg, m_Lcb);
    Lgt_Sv[m_No].x1   = m_Lvx;
    Lgt_Sv[m_No].y1   = m_Lvy;
    Lgt_Sv[m_No].z1   = m_Lvz;
}

/*********************************/
void CRayEnvDlg2::CheckType(void)                                       //<<< 光源ﾀｲﾌﾟﾁｪｯｸ
/*********************************/
{
    m_Static1.EnableWindow(m_Type);
    m_Button11.EnableWindow(m_Type);
    m_Button12.EnableWindow(m_Type);
    m_Button13.EnableWindow(m_Type);
}

///////////////////////////////////////////////////////////////////////////////
// CRayEnvDlg3 ダイアログ

BEGIN_MESSAGE_MAP(CRayEnvDlg3, CPropertyPage)							// ﾒｯｾｰｼﾞ･ﾏｯﾌﾟ
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnInitFile)
	ON_BN_CLICKED(IDC_BUTTON2, OnSaveFile)
	ON_BN_CLICKED(IDC_BUTTON3, OnRestoreFile)
	ON_EN_CHANGE(IDC_EDIT1, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT2, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT3, OnSetColor)
	ON_EN_CHANGE(IDC_EDIT5, OnSetStage)
	ON_EN_CHANGE(IDC_EDIT6, OnSetStage)
	ON_EN_CHANGE(IDC_EDIT7, OnSetStage)
	ON_EN_CHANGE(IDC_EDIT8, OnSetStage)
END_MESSAGE_MAP()

/************************************************************/
CRayEnvDlg3::CRayEnvDlg3() : CPropertyPage(CRayEnvDlg3::IDD)
/************************************************************/
{
    m_Check1 = 0;
    m_Check2 = 0;
    GetRayTraceEnv(&BkRGB_Sv, &Max_Lvl_Sv, &Anti_Sv, &Edge_Sv,
                   &StgFlg_Sv, StgSize_Sv);                             // ﾚｲ･ﾄﾚｰｽ環境取得
    GetObjBottom(m_Bottom);                                             // 表示立体ﾎﾞﾄﾑ取得
    GetTraceEnv();                                                      // ﾚｲ･ﾄﾚｰｽ環境取得
    DisplayStage();                                                     // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ表示
    m_psp.dwFlags &= ~PSP_HASHELP;										// ヘルプ ボタンをなくします。
}

/****************************************************/
void CRayEnvDlg3::DoDataExchange(CDataExchange* pDX)					//<<< ﾀﾞｲｱﾛｸﾞ･ﾃﾞｰﾀの交換と有効性ﾁｪｯｸ
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_Bkr);
    DDV_MinMaxInt(pDX, m_Bkr, 0, 255);
    DDX_Control(pDX, IDC_SPIN1, m_Spin1);
    DDX_Text(pDX, IDC_EDIT2, m_Bkg);
    DDV_MinMaxInt(pDX, m_Bkg, 0, 255);
    DDX_Control(pDX, IDC_SPIN2, m_Spin2);
    DDX_Text(pDX, IDC_EDIT3, m_Bkb);
    DDV_MinMaxInt(pDX, m_Bkb, 0, 255);
    DDX_Control(pDX, IDC_SPIN3, m_Spin3);
    DDX_Control(pDX, IDC_PIC1, m_Pic);
    DDX_Text(pDX, IDC_EDIT4, m_Max);
    DDV_MinMaxInt(pDX, m_Max, 1, 50);
    DDX_Control(pDX, IDC_SPIN4, m_Spin4);
    DDX_Check(pDX, IDC_CHECK1, m_Ant);
    DDX_Check(pDX, IDC_CHECK2, m_Edg);
    DDX_Check(pDX, IDC_CHECK3, m_StgF1);
    DDX_Check(pDX, IDC_CHECK4, m_StgF2);
    DDX_Check(pDX, IDC_CHECK5, m_StgF3);
    DDX_Check(pDX, IDC_CHECK6, m_StgF4);
    DDX_Text(pDX, IDC_EDIT5, m_StgX);
    DDV_MinMaxDouble(pDX, m_StgX, 0.0, 1000.0);
    DDX_Control(pDX, IDC_SPIN5, m_Spin5);
    DDX_Text(pDX, IDC_EDIT6, m_StgY);
    DDV_MinMaxDouble(pDX, m_StgY, 0.0, 1000.0);
    DDX_Control(pDX, IDC_SPIN6, m_Spin6);
    DDX_Text(pDX, IDC_EDIT7, m_StgZ);
    DDV_MinMaxDouble(pDX, m_StgZ, 0.0, 1000.0);
    DDX_Control(pDX, IDC_SPIN7, m_Spin7);
    DDX_Text(pDX, IDC_EDIT8, m_StgP);
    DDV_MinMaxDouble(pDX, m_StgP, 0.0, 1000.0);
    DDX_Control(pDX, IDC_SPIN8, m_Spin8);
}

/*******************************/
void CRayEnvDlg3::OnPaint(void)											//<<< WM_PAINT ﾒｯｾｰｼﾞ応答処理
/*******************************/
{
	CPaintDC dc(this);													// 描画用ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ
	ColorPaint(&dc);													// ｶﾗｰﾍﾟｲﾝﾄ
}

/************************************/
BOOL CRayEnvDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ﾒｯｾｰｼﾞ応答処理
/************************************/
{
	CPropertyPage::OnInitDialog();
    m_Spin1.SetRange(0, 255);
    m_Spin2.SetRange(0, 255);
    m_Spin3.SetRange(0, 255);
    m_Spin4.SetRange(1, 50);
    m_Spin5.SetRange(0, 1000);
    m_Spin6.SetRange(0, 1000);
    m_Spin7.SetRange(0, 1000);
    m_Spin8.SetRange(0, 1000);
    m_Pic.GetWindowRect(m_SQ);                                          // 矩形ｽｸﾘｰﾝ座標取得
    ScreenToClient(m_SQ);                                               // ｸﾗｲｱﾝﾄ座標へ変換
    return TRUE;	// コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
}

/**********************************/
void CRayEnvDlg3::OnInitFile(void)										//<<< 初期値ﾎﾞﾀﾝ押下処理
/**********************************/
{
    FILE*   fp;
    CString path;
    int     i1, i2, i3, i4, er=0;
    float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+TEN_NAME;                                      // ﾚｲ･ﾄﾚｰｽ初期環境ﾌｧｲﾙ名
    if(fopen_s(&fp, path, "r")==0) {                                    // ﾌｧｲﾙOPEN
        while (TRUE) {
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // 背景色設定
                er = 1; break;
            }
            BkRGB_Sv = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d%d", &i1, &i2, &i3, &i4)!=4) {
                er = 1; break;
            }
            Max_Lvl_Sv = i1;                                            // 最大探査ﾚﾍﾞﾙ
            Anti_Sv    = i2;                                            // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ
            Edge_Sv    = i3;                                            // 稜線ﾌﾗｸﾞ
            StgFlg_Sv  = i4;                                            // ｽﾃｰｼﾞ有無ﾌﾗｸﾞ
            if(fscanf_s(fp, "%f%f%f%f", &f1, &f2, &f3, &f4)!=4) {       // ｽﾃｰｼﾞｻｲｽﾞ設定
                er = 1; break;
            }
            StgSize_Sv[0] = ChangeFltToDbl(f1);
            StgSize_Sv[1] = ChangeFltToDbl(f2);
            StgSize_Sv[2] = ChangeFltToDbl(f3);
            StgSize_Sv[3] = ChangeFltToDbl(f4);
            break;
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_CANT_READ_FILE);                              // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
    }
    GetTraceEnv();                                                      // ﾚｲ･ﾄﾚｰｽ環境取得
    DisplayStage();                                                     // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ表示
	UpdateData(FALSE);													// ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
    View->Display(0);                                                   // 再描画
	RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/**********************************/
void CRayEnvDlg3::OnSaveFile(void)                                      //<<< 保存ﾎﾞﾀﾝ押下処理
/**********************************/
{
    FILE*   fp;
    CString path;
    int     i1, i2, i3, i4, er=0;
    float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    UpdateData(TRUE);                                                   // ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
    SetTraceEnv();                                                      // ﾚｲ･ﾄﾚｰｽ環境設定
    path = GetAppPath(0)+RT3_NAME;                                      // ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名3
    if(fopen_s(&fp, path, "w")==0) {                                    // ﾌｧｲﾙOPEN
        while (TRUE) {
            i1 = GetRValue(BkRGB_Sv);
            i2 = GetGValue(BkRGB_Sv);
            i3 = GetBValue(BkRGB_Sv);
            if(fprintf(fp, "%d %d %d\n", i1, i2, i3)<0) {
                er = 1; break;
            }
            i1 = Max_Lvl_Sv;
            i2 = Anti_Sv;
            i3 = Edge_Sv;
            i4 = StgFlg_Sv;
            if(fprintf(fp, "%d %d %d %d\n", i1, i2, i3, i4)<0) {
                er = 1; break;
            }
            f1 = (float)StgSize_Sv[0];
            f2 = (float)StgSize_Sv[1];
            f3 = (float)StgSize_Sv[2];
            f4 = (float)StgSize_Sv[3];
            if(fprintf(fp, "%f %f %f %f\n", f1, f2, f3, f4)<0) {
                er = 1; break;
            }
            break;
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_ERR_FILEWRITE);                               // ｢ﾌｧｲﾙ保存NG｣
    }
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/*************************************/
void CRayEnvDlg3::OnRestoreFile(void)                                   //<<< 復元ﾎﾞﾀﾝ押下処理
/*************************************/
{
    FILE*   fp;
    CString path;
    int     i1, i2, i3, i4, er=0;
    float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ﾛｹｰﾙを英語に変更
    path = GetAppPath(0)+RT3_NAME;                                      // ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名3
    if(fopen_s(&fp, path, "r")==0) {                                    // ﾌｧｲﾙOPEN
        while (TRUE) {
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {
                er = 1; break;
            }
            BkRGB_Sv = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d%d", &i1, &i2, &i3, &i4)!=4) {
                er = 1; break;
            }
            Max_Lvl_Sv = i1;
            Anti_Sv    = i2;
            Edge_Sv    = i3;
            StgFlg_Sv  = i4;
            if(fscanf_s(fp, "%f%f%f%f", &f1, &f2, &f3, &f4)!=4) {
                er = 1; break;
            }
            StgSize_Sv[0] = ChangeFltToDbl(f1);
            StgSize_Sv[1] = ChangeFltToDbl(f2);
            StgSize_Sv[2] = ChangeFltToDbl(f3);
            StgSize_Sv[3] = ChangeFltToDbl(f4);
            break;
        }
        fclose(fp);                                                     // ﾌｧｲﾙCLOSE
        if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ｢ﾃﾞｰﾀﾌｧｲﾙを読込NG｣
        }
    } else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ｢保存ﾌｧｲﾙなし｣
    }
    GetTraceEnv();                                                      // ﾚｲ･ﾄﾚｰｽ環境取得
    DisplayStage();                                                     // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ表示
    UpdateData(FALSE);                                                  // ﾀﾞｲｱﾛｸﾞ上にﾃﾞｰﾀ表示
    View->Display(0);                                                   // 再描画
    RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ﾛｹｰﾙをﾃﾞﾌｫﾙﾄに変更
}

/**********************************/
void CRayEnvDlg3::OnSetColor(void)										//<<< ｶﾗｰ設定ﾎﾞﾀﾝ押下処理
/**********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
		RedrawWindow(NULL, NULL);
	}
}

/**********************************/
void CRayEnvDlg3::OnSetStage(void)										//<<< ｽﾃｰｼﾞ設定ﾎﾞﾀﾝ押下処理
/**********************************/
{
	if(++m_Check2>4) {
		UpdateData(TRUE);												// ﾀﾞｲｱﾛｸﾞ上のﾃﾞｰﾀ取得
        DisplayStage();                                                 // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ表示
        View->Display(0);                                               // 再描画
	}
}

/******************************************/
void CRayEnvDlg3::ColorPaint(CPaintDC *dc)								//<<< ｶﾗｰﾍﾟｲﾝﾄ
/******************************************/
{
    CBrush brushNew(RGB(m_Bkr, m_Bkg, m_Bkb));
   	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    CPen penNew(0, 1, RGB(m_Bkr, m_Bkg, m_Bkb));
    CPen* pPenOld = dc->SelectObject(&penNew);
    dc->Rectangle(m_SQ.left, m_SQ.top, m_SQ.right, m_SQ.bottom);        // 領域描画
    dc->SelectObject(pBrushOld);
    dc->SelectObject(pPenOld);
    brushNew.DeleteObject();
    penNew.DeleteObject();
}

/***********************************/
void CRayEnvDlg3::GetTraceEnv(void)                                     //<<< ﾚｲ･ﾄﾚｰｽ環境取得
/***********************************/
{
    m_Bkr   = GetRValue(BkRGB_Sv);
    m_Bkg   = GetGValue(BkRGB_Sv);
    m_Bkb   = GetBValue(BkRGB_Sv);
    m_Max   = Max_Lvl_Sv;
    m_Ant   = Anti_Sv;
    m_Edg   = Edge_Sv;
    m_StgF1 = ((StgFlg_Sv&0x1)>0 ? 1 : 0);
    m_StgF2 = ((StgFlg_Sv&0x2)>0 ? 1 : 0);
    m_StgF3 = ((StgFlg_Sv&0x4)>0 ? 1 : 0);
    m_StgF4 = ((StgFlg_Sv&0x8)>0 ? 1 : 0);
    m_StgX  = StgSize_Sv[0];
    m_StgY  = StgSize_Sv[1];
    m_StgZ  = StgSize_Sv[2];
    m_StgP  = StgSize_Sv[3];
}

/***********************************/
void CRayEnvDlg3::SetTraceEnv(void)                                     //<<< ﾚｲ･ﾄﾚｰｽ環境設定
/***********************************/
{
    BkRGB_Sv   = RGB(m_Bkr, m_Bkg, m_Bkb);
    Max_Lvl_Sv = m_Max;
    Anti_Sv    = m_Ant;
    Edge_Sv    = m_Edg;
    StgFlg_Sv  = 0;
    if(m_StgF1) {
        StgFlg_Sv |= 0x1;
    }
    if(m_StgF2) {
        StgFlg_Sv |= 0x2;
    }
    if(m_StgF3) {
        StgFlg_Sv |= 0x4;
    }
    if(m_StgF4) {
        StgFlg_Sv |= 0x8;
    }
    StgSize_Sv[0] = m_StgX;
    StgSize_Sv[1] = m_StgY;
    StgSize_Sv[2] = m_StgZ;
    StgSize_Sv[3] = m_StgP;
}

/************************************/
void CRayEnvDlg3::DisplayStage(void)                                    //<<< ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ表示
/************************************/
{
    int    flg[4];
    double x, y, z, size[8][3];

    flg[0] = m_StgF1;                                                   // ｽﾃｰｼﾞ表示ﾌﾗｸﾞ設定
    flg[1] = m_StgF2;
    flg[2] = m_StgF3;
    flg[3] = m_StgF4;
    x = m_StgX/2.0;                                                     // X/2.0, Y/2.0
    y = m_StgY/2.0;
    z = m_Bottom[2]-m_StgP;                                             // 底面Z位置
    size[0][0] = m_Bottom[0]-x;
    size[0][1] = m_Bottom[1]-y;
    size[0][2] = z;
    size[1][0] = m_Bottom[0]-x;
    size[1][1] = m_Bottom[1]+y;
    size[1][2] = z;
    size[2][0] = m_Bottom[0]+x;
    size[2][1] = m_Bottom[1]-y;
    size[2][2] = z;
    size[3][0] = m_Bottom[0]+x;
    size[3][1] = m_Bottom[1]+y;
    size[3][2] = z;
    z += m_StgZ;                                                        // 背面･側面高さ
    size[4][0] = m_Bottom[0]-x;
    size[4][1] = m_Bottom[1]-y;
    size[4][2] = z;
    size[5][0] = m_Bottom[0]-x;
    size[5][1] = m_Bottom[1]+y;
    size[5][2] = z;
    size[6][0] = m_Bottom[0]+x;
    size[6][1] = m_Bottom[1]-y;
    size[6][2] = z;
    size[7][0] = m_Bottom[0]+x;
    size[7][1] = m_Bottom[1]+y;
    size[7][2] = z;
    View->SetStage(flg, size);                                          // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ設定
}

///////////////////////////////////////////////////////////////////////////////
// CRayEnvSheet プロパティシート

/*******************************************************************************/
CRayEnvSheet::CRayEnvSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
/*******************************************************************************/
{
	AddControlPages();
	// TODO :: この位置に残りのコントロール用のページを追加してください。
}

/**********************************************************************************/
CRayEnvSheet::CRayEnvSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
/**********************************************************************************/
{
	AddControlPages();
}

/*****************************/
CRayEnvSheet::~CRayEnvSheet()
/*****************************/
{
}

/************************************/
void CRayEnvSheet::AddControlPages()
/************************************/
{
	m_psh.dwFlags &= ~PSH_USEHICON;
	m_psh.dwFlags &= ~PSH_HASHELP;										// ヘルプ ボタンをなくします
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags |= PSH_USEPAGELANG;

	AddPage(&m_RayEnvPage1);
	AddPage(&m_RayEnvPage2);
	AddPage(&m_RayEnvPage3);
}

BEGIN_MESSAGE_MAP(CRayEnvSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CRayEnvSheet)
	//}}AFX_MSG_MAP
    ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************/
void CRayEnvSheet::OnDestroy(void)                                      //<<< WM_DESTROYﾒｯｾｰｼﾞ応答処理
/**********************************/
{
    View->ResetLightPos();                                              // ﾚｲ･ﾄﾚｰｼﾝｸﾞ光源位置解除
    View->DeleteStage();                                                // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ削除
    View->EndStageDisplay();                                            // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ終了処理
    View->Display(0);                                                   // 再描画
    CPropertySheet::OnDestroy();
}

/*********************************/
BOOL CRayEnvSheet::OnInitDialog()
/*********************************/
{
    BOXTYPE box;
    double  btm[3];

    GetObjBottom(btm);                                                  // 表示立体ﾎﾞﾄﾑ取得
    box.bp[0].p[0] = btm[0]-500.0;
    box.bp[0].p[1] = btm[1]-500.0;
    box.bp[0].p[2] = btm[2];
    box.bp[1].p[0] = btm[0]+500.0;
    box.bp[1].p[1] = btm[1]+500.0;
    box.bp[1].p[2] = btm[2]+1000.0;
    View->StartStageDisplay(box);                                       // ﾚｲ･ﾄﾚｰｼﾝｸﾞｽﾃｰｼﾞ初期処理
    View->Display(0);                                                   // 再描画
    return CPropertySheet::OnInitDialog();
}

/***************************************************************************************/
BOOL CRayEnvSheet::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
			const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
/***************************************************************************************/
{
	//既定値の DS_CONTEXT_HELP スタイルを削除します。
//  dwStyle= WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/****************************************/
void CRayEnvSheet::SetRayTraceData(void)                                //<<< ﾚｲ･ﾄﾚｰｽﾃﾞｰﾀ設定
/****************************************/
{
    int i;

    m_RayEnvPage1.SetTraceColor();
    for(i=0; i<ATMAX; i++) {
        SetRayTraceColor(i, m_RayEnvPage1.Atr_Sv[i]);                   // ﾚｲ･ﾄﾚｰｽ表面属性設定
    }
    m_RayEnvPage2.SetTraceLight();
    for(i=0; i<8; i++) {
        SetRayTraceLight(i, m_RayEnvPage2.Lgt_Sv[i]);                   // ﾚｲ･ﾄﾚｰｽ光源設定
    }
    m_RayEnvPage3.SetTraceEnv();
    SetRayTraceEnv(m_RayEnvPage3.BkRGB_Sv, m_RayEnvPage3.Max_Lvl_Sv,
                   m_RayEnvPage3.Anti_Sv, m_RayEnvPage3.Edge_Sv,
                   m_RayEnvPage3.StgFlg_Sv, m_RayEnvPage3.StgSize_Sv);  // ﾚｲ･ﾄﾚｰｽ環境設定
}
