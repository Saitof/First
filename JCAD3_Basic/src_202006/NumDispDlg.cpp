/////////////////////////////////////////////////////////////////////////////
// ���l�\���޲�۸޸׽(CNumDispDlg)��`
// NumDispDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"

/////////////////////////////////////////////////////////////////////////////
// CNumDispDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CNumDispDlg, CDialog)									// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/****************************************************************************/
CNumDispDlg::CNumDispDlg(CWnd* pParent) : CDialog(CNumDispDlg::IDD, pParent)
/****************************************************************************/
{
	m_pParent = pParent;												// ������
	Init();
}

/****************************************************/
void CNumDispDlg::DoDataExchange(CDataExchange* pDX)					// �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
}

/////////////////////////////////////////////////////////////////////////////
// CNumDispDlg ���b�Z�[�W �n���h��

/************************************/
BOOL CNumDispDlg::OnInitDialog(void)									// WM_INITDIALOG ү���މ�������
/************************************/									// �޲�۸ނ̏�����
{
	CRect r;

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);											// �e����޳�̒����ɕ\���w��
	GetWindowRect(&r);													// �޲�۸ނ̻��ގ擾
	Width = r.Width();													//  (��)
	Height = r.Height();												//  (����)
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/********************************/
void CNumDispDlg::OnCancel(void)										// WM_CANCEL ү���މ�������
/********************************/
{
	View->HideNumDisp();
	ShowWindow(SW_HIDE);
}

/***********************************************/
inline LRESULT CNumDispDlg::OnNcHitTest(CPoint)							// WM_NCHITTEST ү���މ�������
/***********************************************/						// ϳ����ق̈ʒu���擾
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/*********************************/
void CNumDispDlg::OnDestroy(void)										// WM_DESTROYү���މ�������
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "NumLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "NumTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CNumDispDlg �I�y���[�V����

/******************************/
BOOL CNumDispDlg::Create(void)											// ����
/******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "NumLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "NumTop", -1);
	if(left>0) {
		width = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		height = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;
		wp.showCmd = SW_HIDE;
		wp.rcNormalPosition.left = left;
		wp.rcNormalPosition.top = top;
		wp.rcNormalPosition.right = left + width;
		wp.rcNormalPosition.bottom = top + height;
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
	return ret;
}

/***********************************/
inline void CNumDispDlg::Init(void)										// ������
/***********************************/
{
	m_Param1 = _T("0.00");
	m_Param2 = _T("0.00");
	m_Param3 = _T("0.00");
	m_Param4 = _T("0.00");
}

/**************************************************/
void CNumDispDlg::SetParam(double pr1, double pr2,
						   double pr3, double pr4)						// ���Ұ��ݒ�
/**************************************************/
{
	m_Param1.Format("%.2f", pr1);
	m_Param2.Format("%.2f", pr2);
	m_Param3.Format("%.2f", pr3);
	m_Param4.Format("%.2f", pr4);
	UpdateData(FALSE);
}

/***************************************/
void CNumDispDlg::SetNumDispWnd(int dp)
/***************************************/
{
	int mn, kd=0;

	mn = View->GetActMenu();											// ���ݎ��s�����ƭ��擾
	if(mn==M_EDIT_ROTATE||mn==M_EDIT_BEND||
       mn==M_EDIT_TWIST||mn==M_MEASURE_ANGLE||
       mn==M_SURF_L_ROTATE||mn==M_SURF_TWIST) {							// <�p�x>
		kd = 2;
	} else if(mn==M_EDIT_SCALE||mn==M_JWL_GEM) {						// <����(3D)>
		kd = 3;
	} else if(mn==M_JWL_STONE) {										// <����(2D)>
		kd = 4;
	} else if(mn==M_SURF_SCALE) {										// <�{��>
		kd = 5;
	} else if(mn==M_JWL_ARRANGE) {										// <�z�u>
		kd = 6;
	} else if(mn==M_EDIT_SHEAR) {										// <����f�ړ���>
		kd = 7;
	} else if(mn==M_MEASURE_CURVE) {									// <�Ȑ�����>
		kd = 8;
	} else {															// <����>
		kd = 1;
	}
	Init();																// ���Ұ�������
	SetLayout(kd, dp);													// ڲ��Đݒ�
}

/*******************************************/
void CNumDispDlg::SetLayout(int kd, int dp)								// ڲ��Đݒ�
/*******************************************/
{
	CString title, titl1, titl2, titl3, unit1, unit2, unit3, show;
	BOOL    flg=TRUE;
	int     i;

	titl1 = _T(""), titl2 = _T(""), titl3 = _T("");
	unit1 = _T(""), unit2 = _T(""), unit3 = _T("");
	if(kd==1) {															// <����(4)>
		show = "1111111111";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR1);										// ���َ擾(����)
		titl1.LoadString(IDS_STR62);									// ���ږ�1�擾(X:)
		titl2.LoadString(IDS_STR63);									// ���ږ�2�擾(Y:)
		titl3.LoadString(IDS_STR64);									// ���ږ�3�擾(Z:)
		unit1.LoadString(IDS_STR5);										// �P��1�擾(mm)
		unit2.LoadString(IDS_STR5);										// �P��2�擾(mm)
		unit3.LoadString(IDS_STR5);										// �P��3�擾(mm)
		SetWindowPos(NULL, 0, 0, Width, Height,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==2) {													// <�p�x(1)>
		show = "0001001000";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR2);										// ���َ擾(�p�x)
		unit1.LoadString(IDS_STR6);										// �P��1�擾(�x)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==3) {													// <����(3)>
		show = "1111111110";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR3);										// ���َ擾(����)
		titl1.LoadString(IDS_STR62);									// ���ږ�1�擾(X:)
		titl2.LoadString(IDS_STR63);									// ���ږ�2�擾(Y:)
		titl3.LoadString(IDS_STR64);									// ���ږ�3�擾(Z:)
		unit1.LoadString(IDS_STR5);										// �P��1�擾(mm)
		unit2.LoadString(IDS_STR5);										// �P��2�擾(mm)
		unit3.LoadString(IDS_STR5);										// �P��3�擾(mm)
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.8),
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==4) {													// <����(2)>
		show = "1101101100";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR3);										// ���َ擾(����)
		titl1.LoadString(IDS_STR62);									// ���ږ�1�擾(X:)
		titl2.LoadString(IDS_STR63);									// ���ږ�2�擾(Y:)
		unit1.LoadString(IDS_STR5);										// �P��1�擾(mm)
		unit2.LoadString(IDS_STR5);										// �P��2�擾(mm)
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.65),
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==5) {													// <�{��(1)>
		show = "0001001000";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR4);										// ���َ擾(�{��)
		unit1.LoadString(IDS_STR7);										// �P��1�擾(�{)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==6) {													// <�Θg�z�u(3)>
		show = "1111111110";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR58);									// ���َ擾(�Θg�z�u)
		titl1.LoadString(IDS_STR59);									// ���ږ�1�擾(���a:)
		titl2.LoadString(IDS_STR60);									// ���ږ�2�擾(XY:)
		titl3.LoadString(IDS_STR61);									// ���ږ�3�擾(YZ:)
		unit1.LoadString(IDS_STR5);										// �P��1�擾(mm)
		unit2.LoadString(IDS_STR6);										// �P��2�擾(�x)
		unit3.LoadString(IDS_STR6);										// �P��3�擾(�x)
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.8),
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==7) {													// <�{��(1)>
		show = "0001001000";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR79);									// ���َ擾(����f�ړ���)
		unit1.LoadString(IDS_STR5);										// �P��1�擾(mm)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else if(kd==8) {													// <�Ȑ�����(1)>
		show = "0001001000";											// �\��������۰ق̐ݒ�
		title.LoadString(IDS_STR31);									// ���َ擾(����)
		unit1.LoadString(IDS_STR5);										// �P��1�擾(mm)
		SetWindowPos(NULL, 0, 0, Width, Height/2,
                     SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
	} else {															// <���̑�>
		flg = FALSE;
	}
	if(flg) {															// ���̑��ȊO(�L��)�̏ꍇ
		GetDlgItem(IDC_TITLE1)->SetWindowText(title);					// ���ٕ\��
		GetDlgItem(IDC_STATIC1)->SetWindowText(titl1);					// ���ږ�1�\��
		GetDlgItem(IDC_STATIC2)->SetWindowText(titl2);					// ���ږ�2�\��
		GetDlgItem(IDC_STATIC3)->SetWindowText(titl3);					// ���ږ�3�\��
		GetDlgItem(IDC_STATIC4)->SetWindowText(unit1);					// �P��1�\��
		GetDlgItem(IDC_STATIC5)->SetWindowText(unit2);					// �P��2�\��
		GetDlgItem(IDC_STATIC6)->SetWindowText(unit3);					// �P��3�\��
		for(i=0; i<6; i++) {											// ���۰ق̕\��
			GetDlgItem(IDC_STATIC1+i)->
                       ShowWindow(show[i]=='1' ? SW_SHOW : SW_HIDE);
		}
		for(i=0; i<4; i++) {											// ���۰ق̕\��
			GetDlgItem(IDC_EDIT1+i)->
                       ShowWindow(show[i+6]=='1' ? SW_SHOW : SW_HIDE);
		}
		UpdateData(FALSE);												// �ް��X�V
	}
	if(dp!=2) {
		ShowWindow(dp ? SW_SHOWNA : SW_HIDE);							// ����޳�̕\��/��\��
	}
}
