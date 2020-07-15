/////////////////////////////////////////////////////////////////////////////
// �����_�����O���ݒ�v���p�e�B�V�[�g�N���X��`
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
// CRndEnvDlg1 �_�C�A���O

BEGIN_MESSAGE_MAP(CRndEnvDlg1, CPropertyPage)							// ү���ޥϯ��
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
    for(i=0; i<3; i++) {                                                // �װ���擾
    	View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CRndEnvDlg1::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
void CRndEnvDlg1::OnPaint(void)											//<<< WM_PAINT ү���މ�������
/*******************************/
{
	CPaintDC dc(this);													// �`��p���޲���÷��
	ColorPaint(&dc);													// �װ�߲��
}

/************************************/
BOOL CRndEnvDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
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
    m_Pic1.GetWindowRect(m_SQ[0]);										// ��`��ذݍ��W�擾
    m_Pic2.GetWindowRect(m_SQ[1]);
    m_Pic3.GetWindowRect(m_SQ[2]);
	for(i=0; i<3; i++) {
		ScreenToClient(m_SQ[i]);										// �ײ��č��W�֕ϊ�
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CRndEnvDlg1::OnInitFile(void)										//<<< �����l���݉�������
/**********************************/
{
    int i;

	View->InitRColorFile(TRUE);											// ���������ݸ޶װ̧�ٓǍ�
    for(i=0; i<3; i++) {                                                // �װ���擾
    	View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	RedrawWindow(NULL, NULL);
}

/**********************************/
void CRndEnvDlg1::OnSaveFile(void)                                      //<<< �ۑ����݉�������
/**********************************/
{
    View->SaveRndEnv1();                                                // �����ݸފ��ݒ�ۑ�����1
}

/*************************************/
void CRndEnvDlg1::OnRestoreFile(void)                                   //<<< �������݉�������
/*************************************/
{
    int i;

    View->RestoreRndEnv1();                                             // �����ݸފ��ݒ蕜������1
    for(i=0; i<3; i++) {                                                // �װ���擾
        View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
    UpdateData(FALSE);                                                  // �޲�۸ޏ���ް��\��
    RedrawWindow(NULL, NULL);
}

/*************************************/
BOOL CRndEnvDlg1::OnColorNo(UINT nID)									//<<< �װNo���݉�������
/*************************************/
{
	OBJTYPE* op;
    int      i;

	m_No = nID-IDC_RADIO1;
    for(i=0; i<3; i++) {                                                // �װ���擾
    	View->GetColorRGB(m_No, i, &m_RCl[i], &m_GCl[i], &m_BCl[i], &m_Shn);
    }
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	GetObjPtr(&op);														// �����߲���擾
	PutObjCol(op, m_No);												// ���̶װ�ݒ�
	View->Display(0);													// �ĕ`��
	RedrawWindow(NULL, NULL);
	return TRUE;
}

/***********************************/
void CRndEnvDlg1::OnSetColor1(void)										//<<< �װ�ݒ����݉�������1
/***********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		View->SetColorRGB(m_No, 0, m_RCl[0], m_GCl[0], m_BCl[0], m_Shn);// �װ���ݒ�
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg1::OnSetColor2(void)										//<<< �װ�ݒ����݉�������2
/***********************************/
{
	if(++m_Check2>4) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		View->SetColorRGB(m_No, 1, m_RCl[1], m_GCl[1], m_BCl[1], m_Shn);// �װ���ݒ�
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg1::OnSetColor3(void)										//<<< �װ�ݒ����݉�������3
/***********************************/
{
	if(++m_Check3>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		View->SetColorRGB(m_No, 2, m_RCl[2], m_GCl[2], m_BCl[2], m_Shn);// �װ���ݒ�
		RedrawWindow(NULL, NULL);
	}
}

/******************************************/
void CRndEnvDlg1::ColorPaint(CPaintDC *dc)								//<<< �װ�߲��
/******************************************/
{
	int i;

    for(i=0; i<3; i++) {
    	CBrush brushNew(RGB(m_RCl[i], m_GCl[i], m_BCl[i]));
    	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    	CPen penNew(0, 1, RGB(m_RCl[i], m_GCl[i], m_BCl[i]));
    	CPen* pPenOld = dc->SelectObject(&penNew);
    	dc->Rectangle(m_SQ[i].left, m_SQ[i].top,
                      m_SQ[i].right, m_SQ[i].bottom);					// �̈�`��
    	dc->SelectObject(pBrushOld);
    	dc->SelectObject(pPenOld);
    	brushNew.DeleteObject();
    	penNew.DeleteObject();
    }
}

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg2 �_�C�A���O

BEGIN_MESSAGE_MAP(CRndEnvDlg2, CPropertyPage)							// ү���ޥϯ��
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
    	View->GetRenderRGB(i, &m_RCol[i], &m_GCol[i], &m_BCol[i]);		// �����ݸޏ��擾
    }
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CRndEnvDlg2::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
void CRndEnvDlg2::OnPaint(void)											//<<< WM_PAINT ү���މ�������
/*******************************/
{
	CPaintDC dc(this);													// �`��p���޲���÷��
	ColorPaint(&dc);													// �װ�߲��
}

/************************************/
BOOL CRndEnvDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
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
    m_Pic1.GetWindowRect(m_SQ[0]);										// ��`��ذݍ��W�擾
    m_Pic2.GetWindowRect(m_SQ[1]);
    m_Pic3.GetWindowRect(m_SQ[2]);
    for(i=0; i<3; i++) {
        ScreenToClient(m_SQ[i]);										// �ײ��č��W�֕ϊ�
    }
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CRndEnvDlg2::OnInitFile(void)										//<<< �����l���݉�������
/**********************************/
{
    int i;

	View->InitRenderFile(1, TRUE);										// ���������ݸ�̧�ٓǍ�
    for(i=0; i<3; i++) {
    	View->GetRenderRGB(i, &m_RCol[i], &m_GCol[i], &m_BCol[i]);		// �����ݸޏ��擾
    }
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
	RedrawWindow(NULL, NULL);
}

/**********************************/
void CRndEnvDlg2::OnSaveFile(void)                                      //<<< �ۑ����݉�������
/**********************************/
{
    View->SaveRndEnv2();                                                // �����ݸފ��ݒ�ۑ�����2
}

/*************************************/
void CRndEnvDlg2::OnRestoreFile(void)                                   //<<< �������݉�������
/*************************************/
{
    int i;

    View->RestoreRndEnv2();                                             // �����ݸފ��ݒ蕜������2
    for(i=0; i<3; i++) {
        View->GetRenderRGB(i, &m_RCol[i], &m_GCol[i], &m_BCol[i]);      // �����ݸޏ��擾
    }
    UpdateData(FALSE);                                                  // �޲�۸ޏ���ް��\��
    RedrawWindow(NULL, NULL);
}

/***********************************/
void CRndEnvDlg2::OnSetColor1(void)										//<<< �װ�ݒ����݉�������1
/***********************************/
{
	if(++m_Check1>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		View->SetRenderRGB(0, m_RCol[0], m_GCol[0], m_BCol[0]);			// �����ݸޏ��ݒ�
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg2::OnSetColor2(void)										//<<< �װ�ݒ����݉�������2
/***********************************/
{
	if(++m_Check2>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		View->SetRenderRGB(1, m_RCol[1], m_GCol[1], m_BCol[1]);			// �����ݸޏ��ݒ�
		RedrawWindow(NULL, NULL);
	}
}

/***********************************/
void CRndEnvDlg2::OnSetColor3(void)										//<<< �װ�ݒ����݉�������3
/***********************************/
{
	if(++m_Check3>3) {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		View->SetRenderRGB(2, m_RCol[2], m_GCol[2], m_BCol[2]);			// �����ݸޏ��ݒ�
		RedrawWindow(NULL, NULL);
	}
}

/******************************************/
void CRndEnvDlg2::ColorPaint(CPaintDC *dc)								//<<< �װ�߲��
/******************************************/
{
	int i;

    for(i=0; i<3; i++) {
    	CBrush brushNew(RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CBrush* pBrushOld = dc->SelectObject(&brushNew);
    	CPen penNew(0, 1, RGB(m_RCol[i], m_GCol[i], m_BCol[i]));
    	CPen* pPenOld = dc->SelectObject(&penNew);
    	dc->Rectangle(m_SQ[i].left, m_SQ[i].top,
                      m_SQ[i].right, m_SQ[i].bottom);					// �̈�`��
    	dc->SelectObject(pBrushOld);
    	dc->SelectObject(pPenOld);
    	brushNew.DeleteObject();
    	penNew.DeleteObject();
    }
}

///////////////////////////////////////////////////////////////////////////////
// CRndEnvDlg3 �_�C�A���O

BEGIN_MESSAGE_MAP(CRndEnvDlg3, CPropertyPage)							// ү���ޥϯ��
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
    m_Path = View->GetMapFilePath();                                    // �����ݸ�ϯ���߽�擾
	View->GetRenderMap(&m_File);										// �����ݸ�ϯ�ߎ擾
	i = m_File.GetLength();
	j = m_File.ReverseFind('\\');
	if(i>0) {
		m_FileName = m_File.Right(i-j-1);
	}
	m_psp.dwFlags &= ~PSP_HASHELP;										// �w���v �{�^�����Ȃ����܂��B
}

/****************************************************/
void CRndEnvDlg3::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_FileName);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
}

/************************************/
BOOL CRndEnvDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	CPropertyPage::OnInitDialog();
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CRndEnvDlg3::OnFileOpen(void)										//<<< �Q�����݉�������
/**********************************/
{
	CString ext="bmp", wex, file, filter;
	DWORD   flg;
	int     i;

	if(!CheckFilePath(0, m_Path)) {										// �߽�m�F
		m_Path=GetAppPath(0);
	}
	flg = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;							// �BMP̧�ق��J���
	filter.LoadString(IDS_FILE_FILTER101);
	file = m_FileName;
	wex = file.Right(4);
	if(!wex.CompareNoCase(".bmp")) {									// <�L���Ȋg���q>
		i = file.ReverseFind('.');
		file = file.Left(i);											// �g���q���폜
	}
	CFileDlg dlg(TRUE, ext, file, flg, filter);							// �޲�۸ޕ\��
	dlg.m_ofn.lpstrInitialDir = m_Path;
	if(dlg.DoModal()==IDOK) {
		m_File = dlg.GetPathName();										// ̧�ٖ�(���߽)�擾
		m_FileName = dlg.GetFileTitle();								// ̧�ٖ��擾
		ext = dlg.GetFileExt();											// �g���q�擾
		m_FileName = m_FileName + "." + ext;
		m_Edit1.SetWindowText(m_FileName);								// ̧�ٖ��\��
		i = m_File.ReverseFind('\\');
		m_Path = m_File.Left(i+1);										// �߽�ۑ�
        View->SetMapFilePath(m_Path);                                   // �����ݸ�ϯ���߽�ݒ�
		View->SetRenderMap(m_File);										// �����ݸ�ϯ�ߐݒ�
	}
}

/**********************************/
void CRndEnvDlg3::OnInitFile(void)										//<<< �����l���݉�������
/**********************************/
{
	int i, j;

	View->InitRenderFile(2, TRUE);										// ���������ݸ�̧�ٓǍ�
	View->GetRenderMap(&m_File);										// �����ݸ�ϯ�ߎ擾
	i = m_File.GetLength();
	j = m_File.ReverseFind('\\');
	if(i>0) {
		m_FileName = m_File.Right(i-j-1);
	}
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CRndEnvDlg3::OnSaveFile(void)                                      //<<< �ۑ����݉�������
/**********************************/
{
    View->SaveRndEnv3();                                                // �����ݸފ��ݒ�ۑ�����3
}

/*************************************/
void CRndEnvDlg3::OnRestoreFile(void)                                   //<<< �������݉�������
/*************************************/
{
    int i, j;

    View->RestoreRndEnv3();                                             // �����ݸފ��ݒ蕜������3
    View->GetRenderMap(&m_File);                                        // �����ݸ�ϯ�ߎ擾
    i = m_File.GetLength();
    j = m_File.ReverseFind('\\');
    if(i>0) {
        m_FileName = m_File.Right(i-j-1);
    }
    UpdateData(FALSE);                                                  // �޲�۸ޏ���ް��\��
}

///////////////////////////////////////////////////////////////////////////////
// CRndEnvSheet �v���p�e�B�V�[�g

/*******************************************************************************/
CRndEnvSheet::CRndEnvSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
						:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
/*******************************************************************************/
{
	AddControlPages();
	// TODO :: ���̈ʒu�Ɏc��̃R���g���[���p�̃y�[�W��ǉ����Ă��������B
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
	m_psh.dwFlags &= ~PSH_HASHELP;										// �w���v �{�^�����Ȃ����܂�
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
	//����l�� DS_CONTEXT_HELP �X�^�C�����폜���܂��B
//  dwStyle= WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_VISIBLE;
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
