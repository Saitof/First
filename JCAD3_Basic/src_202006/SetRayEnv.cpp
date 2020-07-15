/////////////////////////////////////////////////////////////////////////////
// ���C�g���[�V���O���ݒ�v���p�e�B�V�[�g�N���X��`
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
// CRayEnvDlg1 �_�C�A���O

BEGIN_MESSAGE_MAP(CRayEnvDlg1, CPropertyPage)							// ү���ޥϯ��
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
        GetRayTraceColor(i, &Atr_Sv[i]);                                // ڲ��ڰ��\�ʑ����擾
    }
    m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CRayEnvDlg1::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
void CRayEnvDlg1::OnPaint(void)											//<<< WM_PAINT ү���މ�������
/*******************************/
{
	CPaintDC dc(this);													// �`��p���޲���÷��
	ColorPaint(&dc);													// �װ�߲��
}

/************************************/
BOOL CRayEnvDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
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
        m_Pic[i].GetWindowRect(m_SQ[i]);                                // ��`��ذݍ��W�擾
		ScreenToClient(m_SQ[i]);										// �ײ��č��W�֕ϊ�
	}
    GetTraceColor();                                                    // ڲ��ڰ��\�ʑ����擾
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CRayEnvDlg1::OnInitFile(void)										//<<< �����l���݉�������
/**********************************/
{
    FILE*   fp;
    ATTRIBT atr;
    CString path;
    int     i, i1, i2, i3, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+TCL_NAME;                                      // ڲ��ڰ������װ̧�ٖ�
    if(fopen_s(&fp, path, "r")==0) {                                    // ̧��OPEN
        for(i=0; i<ATMAX; i++) {
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // ����
                er = 1; break;
            }
            atr.ka = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // �g�U���ˌ�
                er = 1; break;
            }
            atr.kd = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // ���ʔ��ˌW��
                er = 1; break;
            }
            atr.ks = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // ���ߒ����W��
                er = 1; break;
            }
            atr.ct = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%f", &i1, &f1)!=2) {
                er = 1; break;
            }
            atr.trace = i1;                                             // �f�荞�ݗL��
            atr.n     = ChangeFltToDbl(f1);                             // �������ܗ�
            if(fscanf_s(fp, "%d%d%f", &i1, &i2, &f1)!=3) {
                er = 1; break;
            }
            atr.model = i1;                                             // ����(0:Phong, 1:Cook-T)
            atr.m1    = i2;                                             // ʲײĂ̍L����1(Phong)
            atr.m2    = ChangeFltToDbl(f1);                             // ʲײĂ̍L����2(Cook-T)
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {              // �������ܗ�
                er = 1; break;
            }
            atr.nr = ChangeFltToDbl(f1);
            atr.ng = ChangeFltToDbl(f2);
            atr.nb = ChangeFltToDbl(f3);
            Atr_Sv[i] = atr;
        }
        fclose(fp);                                                     // ̧��CLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_CANT_READ_FILE);                              // ��ް�̧�ق�Ǎ�NG�
    }
    GetTraceColor();                                                    // ڲ��ڰ��\�ʑ����擾
	RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/**********************************/
void CRayEnvDlg1::OnSaveFile(void)                                      //<<< �ۑ����݉�������
/**********************************/
{
    FILE*   fp;
    CString path;
    ATTRIBT atr;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    UpdateData(TRUE);                                                   // �޲�۸ޏ���ް��擾
    SetTraceColor();                                                    // ڲ��ڰ��\�ʑ����ݒ�
    path = GetAppPath(0)+RT1_NAME;                                      // ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�1
    if(fopen_s(&fp, path, "w")==0) {                                    // ̧��OPEN
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
        fclose(fp);                                                     // ̧��CLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_ERR_FILEWRITE);                               // �̧�ٕۑ�NG�
    }
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*************************************/
void CRayEnvDlg1::OnRestoreFile(void)                                   //<<< �������݉�������
/*************************************/
{
    FILE*   fp;
    CString path;
    ATTRIBT atr;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+RT1_NAME;                                      // ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�1
    if(fopen_s(&fp, path, "r")==0) {                                    // ̧��OPEN
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
        fclose(fp);                                                     // ̧��CLOSE
        if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ��ް�̧�ق�Ǎ�NG�
        }
    } else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ��ۑ�̧�قȂ��
    }
    GetTraceColor();                                                    // ڲ��ڰ��\�ʑ����擾
    RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*************************************/
BOOL CRayEnvDlg1::OnColorNo(UINT nID)									//<<< �װNo���݉�������
/*************************************/
{
    int no;

    no = m_No;                                                          // �ύX�O�װNo�ۑ�
    UpdateData(TRUE);                                                   // �޲�۸ޏ���ް��擾
    m_No = no;                                                          // �ύX�O�װNo
    SetTraceColor();                                                    // ڲ��ڰ��\�ʑ����ݒ�
    m_No = nID-IDC_RADIO1;                                              // �ύX��װNo
    GetTraceColor();                                                    // ڲ��ڰ��\�ʑ����擾
	RedrawWindow(NULL, NULL);
	return TRUE;
}

/****************************************/
BOOL CRayEnvDlg1::OnTraceModel(UINT nID)                                //<<< �ڰ���������݉�������
/****************************************/
{
    m_Model = nID-IDC_RADIO20;
    CheckModel(m_Model);
    return TRUE;
}

/**********************************/
void CRayEnvDlg1::OnSetColor(void)										//<<< �װ�ݒ����݉�������
/**********************************/
{
	if(++m_Check>12) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		RedrawWindow(NULL, NULL);
	}
}

/******************************************/
void CRayEnvDlg1::ColorPaint(CPaintDC *dc)								//<<< �װ�߲��
/******************************************/
{
	int	i;

    for(i=0; i<4; i++) {
    	CBrush brushNew(RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    	CPen penNew(0, 1, RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CPen* pPenOld = dc->SelectObject(&penNew);
    	dc->Rectangle(m_SQ[i].left, m_SQ[i].top,
                      m_SQ[i].right, m_SQ[i].bottom);                   // �̈�`��
    	dc->SelectObject(pBrushOld);
    	dc->SelectObject(pPenOld);
    	brushNew.DeleteObject();
    	penNew.DeleteObject();
    }
}

/*************************************/
void CRayEnvDlg1::GetTraceColor(void)                                   //<<< ڲ��ڰ��\�ʑ����擾
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
    CheckModel(m_Model);                                                // �ڰ����������
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/*************************************/
void CRayEnvDlg1::SetTraceColor(void)                                   //<<< ڲ��ڰ��\�ʑ����ݒ�
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
void CRayEnvDlg1::CheckModel(int model)                                 //<<< �ڰ����������
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
// CRayEnvDlg2 �_�C�A���O

BEGIN_MESSAGE_MAP(CRayEnvDlg2, CPropertyPage)							// ү���ޥϯ��
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
    GetObjCent(ObjCent);                                                // �\�����̾����擾
    for(i=0; i<LGMAX; i++) {
        GetRayTraceLight(i, &Lgt_Sv[i]);                                // ڲ��ڰ������擾
    }
    GetTraceLight();                                                    // ڲ��ڰ������擾
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ڲ��ڰ�ݸތ����ʒu�ݒ�
    m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CRayEnvDlg2::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
void CRayEnvDlg2::OnPaint(void)											//<<< WM_PAINT ү���މ�������
/*******************************/
{
	CPaintDC dc(this);													// �`��p���޲���÷��
	ColorPaint(&dc);													// �װ�߲��
}

/************************************/
BOOL CRayEnvDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	CPropertyPage::OnInitDialog();
    m_Spin1.SetRange(0, 255);
    m_Spin2.SetRange(0, 255);
    m_Spin3.SetRange(0, 255);
    m_Spin4.SetRange(-1000, 1000);
    m_Spin5.SetRange(-1000, 1000);
    m_Spin6.SetRange(0, 1000);
    m_Pic.GetWindowRect(m_SQ);                                          // ��`��ذݍ��W�擾
    ScreenToClient(m_SQ);                                               // �ײ��č��W�֕ϊ�
    return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CRayEnvDlg2::OnInitFile(void)										//<<< �����l���݉�������
/**********************************/
{
    FILE*   fp;
    LIGHT   lgt;
    CString path;
    int     i, i1, i2, i3, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+LGT_NAME;                                      // ڲ��ڰ���������̧�ٖ�
    if(fopen_s(&fp, path, "r")==0) {                                    // ̧��OPEN
        for(i=0; i<LGMAX; i++) {
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {
                er = 1; break;
            }
            lgt.flg  = i1;                                              // �L���׸�
            lgt.type = i2;                                              // ��������
            lgt.k    = i3;                                              // ������������
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // ����RGB
                er = 1; break;
            }
            lgt.lc = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {              // �������W�ʒu
                er = 1; break;
            }
            lgt.x1 = ChangeFltToDbl(f1);
            lgt.y1 = ChangeFltToDbl(f2);
            lgt.z1 = ChangeFltToDbl(f3);
            if(fscanf_s(fp, "%f%f%f", &f1, &f2, &f3)!=3) {              // ���������ʒu
                er = 1; break;
            }
            lgt.x2 = ChangeFltToDbl(f1);
            lgt.y2 = ChangeFltToDbl(f2);
            lgt.z2 = ChangeFltToDbl(f3);
            Lgt_Sv[i] = lgt;
        }
        fclose(fp);                                                     // ̧��CLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_CANT_READ_FILE);                              // ��ް�̧�ق�Ǎ�NG�
    }
    GetTraceLight();                                                    // ڲ��ڰ������擾
    CheckType();                                                        // ������������
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ڲ��ڰ�ݸތ����ʒu�ݒ�
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
    View->Display(0);                                                   // �ĕ`��
	RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/**********************************/
void CRayEnvDlg2::OnSaveFile(void)                                      //<<< �ۑ����݉�������
/**********************************/
{
    FILE*   fp;
    CString path;
    LIGHT   lgt;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    UpdateData(TRUE);                                                   // �޲�۸ޏ���ް��擾
    SetTraceLight();                                                    // ڲ��ڰ������ݒ�
    path = GetAppPath(0)+RT2_NAME;                                      // ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�2
    if(fopen_s(&fp, path, "w")==0) {                                    // ̧��OPEN
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
        fclose(fp);                                                     // ̧��CLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_ERR_FILEWRITE);                               // �̧�ٕۑ�NG�
    }
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*************************************/
void CRayEnvDlg2::OnRestoreFile(void)                                   //<<< �������݉�������
/*************************************/
{
    FILE*   fp;
    CString path;
    LIGHT   lgt;
    int     i, r, g, b, er=0;
    float   f1, f2, f3;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+RT2_NAME;                                      // ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�2
    if(fopen_s(&fp, path, "r")==0) {                                    // ̧��OPEN
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
        fclose(fp);                                                     // ̧��CLOSE
        if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ��ް�̧�ق�Ǎ�NG�
        }
    } else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ��ۑ�̧�قȂ��
    }
    GetTraceLight();                                                    // ڲ��ڰ������擾
    CheckType();                                                        // ������������
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ڲ��ڰ�ݸތ����ʒu�ݒ�
    UpdateData(FALSE);                                                  // �޲�۸ޏ���ް��\��
    View->Display(0);                                                   // �ĕ`��
    RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*************************************/
BOOL CRayEnvDlg2::OnLightNo(UINT nID)                                   //<<< ����No���݉�������
/*************************************/
{
    int no;

    no = m_No;                                                          // �ύX�O����No�ۑ�
    UpdateData(TRUE);                                                   // �޲�۸ޏ���ް��擾
    m_No = no;                                                          // �ύX�O����No
    SetTraceLight();                                                    // ڲ��ڰ������ݒ�
    m_No = nID-IDC_RADIO1;                                              // �ύX�����No
    GetTraceLight();                                                    // ڲ��ڰ������擾
    CheckType();                                                        // ������������
    View->SetLightPos(m_No, Lgt_Sv, ObjCent);                           // ڲ��ڰ�ݸތ����ʒu�ݒ�
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
    View->Display(0);                                                   // �ĕ`��
	RedrawWindow(NULL, NULL);
	return TRUE;
}

/***************************************/
BOOL CRayEnvDlg2::OnLightType(UINT nID)                                 //<<< �����������݉�������
/***************************************/
{
    m_Type = nID-IDC_RADIO9;
    CheckType();                                                        // ������������
    return TRUE;
}

/**********************************/
void CRayEnvDlg2::OnSetColor(void)										//<<< �װ�ݒ����݉�������
/**********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		RedrawWindow(NULL, NULL);
	}
}

/*************************************/
void CRayEnvDlg2::OnSetLightPos(void)                                   //<<< �����ʒu�ݒ����݉�������
/*************************************/
{
	if(++m_Check2>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
        SetTraceLight();                                                // ڲ��ڰ������ݒ�
        View->SetLightPos(m_No, Lgt_Sv, ObjCent);                       // ڲ��ڰ�ݸތ����ʒu�ݒ�
        View->Display(0);                                               // �ĕ`��
	}
}

/******************************************/
void CRayEnvDlg2::ColorPaint(CPaintDC *dc)								//<<< �װ�߲��
/******************************************/
{
    CBrush brushNew(RGB(m_Lcr, m_Lcg, m_Lcb));
   	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    CPen penNew(0, 1, RGB(m_Lcr, m_Lcg, m_Lcb));
    CPen* pPenOld = dc->SelectObject(&penNew);
    dc->Rectangle(m_SQ.left, m_SQ.top, m_SQ.right, m_SQ.bottom);        // �̈�`��
    dc->SelectObject(pBrushOld);
    dc->SelectObject(pPenOld);
    brushNew.DeleteObject();
    penNew.DeleteObject();
}

/*************************************/
void CRayEnvDlg2::GetTraceLight(void)                                   //<<< ڲ��ڰ������擾
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
void CRayEnvDlg2::SetTraceLight(void)                                   //<<< ڲ��ڰ������ݒ�
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
void CRayEnvDlg2::CheckType(void)                                       //<<< ������������
/*********************************/
{
    m_Static1.EnableWindow(m_Type);
    m_Button11.EnableWindow(m_Type);
    m_Button12.EnableWindow(m_Type);
    m_Button13.EnableWindow(m_Type);
}

///////////////////////////////////////////////////////////////////////////////
// CRayEnvDlg3 �_�C�A���O

BEGIN_MESSAGE_MAP(CRayEnvDlg3, CPropertyPage)							// ү���ޥϯ��
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
                   &StgFlg_Sv, StgSize_Sv);                             // ڲ��ڰ����擾
    GetObjBottom(m_Bottom);                                             // �\���������ю擾
    GetTraceEnv();                                                      // ڲ��ڰ����擾
    DisplayStage();                                                     // ڲ��ڰ�ݸ޽ð�ޕ\��
    m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CRayEnvDlg3::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
void CRayEnvDlg3::OnPaint(void)											//<<< WM_PAINT ү���މ�������
/*******************************/
{
	CPaintDC dc(this);													// �`��p���޲���÷��
	ColorPaint(&dc);													// �װ�߲��
}

/************************************/
BOOL CRayEnvDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
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
    m_Pic.GetWindowRect(m_SQ);                                          // ��`��ذݍ��W�擾
    ScreenToClient(m_SQ);                                               // �ײ��č��W�֕ϊ�
    return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CRayEnvDlg3::OnInitFile(void)										//<<< �����l���݉�������
/**********************************/
{
    FILE*   fp;
    CString path;
    int     i1, i2, i3, i4, er=0;
    float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+TEN_NAME;                                      // ڲ��ڰ�������̧�ٖ�
    if(fopen_s(&fp, path, "r")==0) {                                    // ̧��OPEN
        while (TRUE) {
            if(fscanf_s(fp, "%d%d%d", &i1, &i2, &i3)!=3) {              // �w�i�F�ݒ�
                er = 1; break;
            }
            BkRGB_Sv = RGB(i1, i2, i3);
            if(fscanf_s(fp, "%d%d%d%d", &i1, &i2, &i3, &i4)!=4) {
                er = 1; break;
            }
            Max_Lvl_Sv = i1;                                            // �ő�T������
            Anti_Sv    = i2;                                            // �����ر�ݸ�
            Edge_Sv    = i3;                                            // �Ő��׸�
            StgFlg_Sv  = i4;                                            // �ð�ޗL���׸�
            if(fscanf_s(fp, "%f%f%f%f", &f1, &f2, &f3, &f4)!=4) {       // �ð�޻��ސݒ�
                er = 1; break;
            }
            StgSize_Sv[0] = ChangeFltToDbl(f1);
            StgSize_Sv[1] = ChangeFltToDbl(f2);
            StgSize_Sv[2] = ChangeFltToDbl(f3);
            StgSize_Sv[3] = ChangeFltToDbl(f4);
            break;
        }
        fclose(fp);                                                     // ̧��CLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_CANT_READ_FILE);                              // ��ް�̧�ق�Ǎ�NG�
    }
    GetTraceEnv();                                                      // ڲ��ڰ����擾
    DisplayStage();                                                     // ڲ��ڰ�ݸ޽ð�ޕ\��
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
    View->Display(0);                                                   // �ĕ`��
	RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/**********************************/
void CRayEnvDlg3::OnSaveFile(void)                                      //<<< �ۑ����݉�������
/**********************************/
{
    FILE*   fp;
    CString path;
    int     i1, i2, i3, i4, er=0;
    float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    UpdateData(TRUE);                                                   // �޲�۸ޏ���ް��擾
    SetTraceEnv();                                                      // ڲ��ڰ����ݒ�
    path = GetAppPath(0)+RT3_NAME;                                      // ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�3
    if(fopen_s(&fp, path, "w")==0) {                                    // ̧��OPEN
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
        fclose(fp);                                                     // ̧��CLOSE
    } else {
        er = 1;
    }
    if(er) {
        AfxMessageBox(IDS_ERR_FILEWRITE);                               // �̧�ٕۑ�NG�
    }
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*************************************/
void CRayEnvDlg3::OnRestoreFile(void)                                   //<<< �������݉�������
/*************************************/
{
    FILE*   fp;
    CString path;
    int     i1, i2, i3, i4, er=0;
    float   f1, f2, f3, f4;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
    path = GetAppPath(0)+RT3_NAME;                                      // ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�3
    if(fopen_s(&fp, path, "r")==0) {                                    // ̧��OPEN
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
        fclose(fp);                                                     // ̧��CLOSE
        if(er) {
            AfxMessageBox(IDS_CANT_READ_FILE);                          // ��ް�̧�ق�Ǎ�NG�
        }
    } else {
        AfxMessageBox(IDS_NON_BACKUP);                                  // ��ۑ�̧�قȂ��
    }
    GetTraceEnv();                                                      // ڲ��ڰ����擾
    DisplayStage();                                                     // ڲ��ڰ�ݸ޽ð�ޕ\��
    UpdateData(FALSE);                                                  // �޲�۸ޏ���ް��\��
    View->Display(0);                                                   // �ĕ`��
    RedrawWindow(NULL, NULL);
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/**********************************/
void CRayEnvDlg3::OnSetColor(void)										//<<< �װ�ݒ����݉�������
/**********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		RedrawWindow(NULL, NULL);
	}
}

/**********************************/
void CRayEnvDlg3::OnSetStage(void)										//<<< �ð�ސݒ����݉�������
/**********************************/
{
	if(++m_Check2>4) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
        DisplayStage();                                                 // ڲ��ڰ�ݸ޽ð�ޕ\��
        View->Display(0);                                               // �ĕ`��
	}
}

/******************************************/
void CRayEnvDlg3::ColorPaint(CPaintDC *dc)								//<<< �װ�߲��
/******************************************/
{
    CBrush brushNew(RGB(m_Bkr, m_Bkg, m_Bkb));
   	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    CPen penNew(0, 1, RGB(m_Bkr, m_Bkg, m_Bkb));
    CPen* pPenOld = dc->SelectObject(&penNew);
    dc->Rectangle(m_SQ.left, m_SQ.top, m_SQ.right, m_SQ.bottom);        // �̈�`��
    dc->SelectObject(pBrushOld);
    dc->SelectObject(pPenOld);
    brushNew.DeleteObject();
    penNew.DeleteObject();
}

/***********************************/
void CRayEnvDlg3::GetTraceEnv(void)                                     //<<< ڲ��ڰ����擾
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
void CRayEnvDlg3::SetTraceEnv(void)                                     //<<< ڲ��ڰ����ݒ�
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
void CRayEnvDlg3::DisplayStage(void)                                    //<<< ڲ��ڰ�ݸ޽ð�ޕ\��
/************************************/
{
    int    flg[4];
    double x, y, z, size[8][3];

    flg[0] = m_StgF1;                                                   // �ð�ޕ\���׸ސݒ�
    flg[1] = m_StgF2;
    flg[2] = m_StgF3;
    flg[3] = m_StgF4;
    x = m_StgX/2.0;                                                     // X/2.0, Y/2.0
    y = m_StgY/2.0;
    z = m_Bottom[2]-m_StgP;                                             // ���Z�ʒu
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
    z += m_StgZ;                                                        // �w�ʥ���ʍ���
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
    View->SetStage(flg, size);                                          // ڲ��ڰ�ݸ޽ð�ސݒ�
}

///////////////////////////////////////////////////////////////////////////////
// CRayEnvSheet �v���p�e�B�V�[�g

/*******************************************************************************/
CRayEnvSheet::CRayEnvSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
/*******************************************************************************/
{
	AddControlPages();
	// TODO :: ���̈ʒu�Ɏc��̃R���g���[���p�̃y�[�W��ǉ����Ă��������B
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
	m_psh.dwFlags &= ~PSH_HASHELP;										// �w���v �{�^�����Ȃ����܂�
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
void CRayEnvSheet::OnDestroy(void)                                      //<<< WM_DESTROYү���މ�������
/**********************************/
{
    View->ResetLightPos();                                              // ڲ��ڰ�ݸތ����ʒu����
    View->DeleteStage();                                                // ڲ��ڰ�ݸ޽ð�ލ폜
    View->EndStageDisplay();                                            // ڲ��ڰ�ݸ޽ð�ޏI������
    View->Display(0);                                                   // �ĕ`��
    CPropertySheet::OnDestroy();
}

/*********************************/
BOOL CRayEnvSheet::OnInitDialog()
/*********************************/
{
    BOXTYPE box;
    double  btm[3];

    GetObjBottom(btm);                                                  // �\���������ю擾
    box.bp[0].p[0] = btm[0]-500.0;
    box.bp[0].p[1] = btm[1]-500.0;
    box.bp[0].p[2] = btm[2];
    box.bp[1].p[0] = btm[0]+500.0;
    box.bp[1].p[1] = btm[1]+500.0;
    box.bp[1].p[2] = btm[2]+1000.0;
    View->StartStageDisplay(box);                                       // ڲ��ڰ�ݸ޽ð�ޏ�������
    View->Display(0);                                                   // �ĕ`��
    return CPropertySheet::OnInitDialog();
}

/***************************************************************************************/
BOOL CRayEnvSheet::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
			const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
/***************************************************************************************/
{
	//����l�� DS_CONTEXT_HELP �X�^�C�����폜���܂��B
//  dwStyle= WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/****************************************/
void CRayEnvSheet::SetRayTraceData(void)                                //<<< ڲ��ڰ��ް��ݒ�
/****************************************/
{
    int i;

    m_RayEnvPage1.SetTraceColor();
    for(i=0; i<ATMAX; i++) {
        SetRayTraceColor(i, m_RayEnvPage1.Atr_Sv[i]);                   // ڲ��ڰ��\�ʑ����ݒ�
    }
    m_RayEnvPage2.SetTraceLight();
    for(i=0; i<8; i++) {
        SetRayTraceLight(i, m_RayEnvPage2.Lgt_Sv[i]);                   // ڲ��ڰ������ݒ�
    }
    m_RayEnvPage3.SetTraceEnv();
    SetRayTraceEnv(m_RayEnvPage3.BkRGB_Sv, m_RayEnvPage3.Max_Lvl_Sv,
                   m_RayEnvPage3.Anti_Sv, m_RayEnvPage3.Edge_Sv,
                   m_RayEnvPage3.StgFlg_Sv, m_RayEnvPage3.StgSize_Sv);  // ڲ��ڰ����ݒ�
}
