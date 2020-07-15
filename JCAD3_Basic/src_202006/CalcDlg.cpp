/////////////////////////////////////////////////////////////////////////////
// �v�Z�@�_�C�A���O�N���X��`
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
// CCalcDlg1 �_�C�A���O

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
void CCalcDlg1::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strNumber);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT2, m_strType);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
}

/**********************************/
BOOL CCalcDlg1::OnInitDialog(void)										//<<< WM_INITDIALOG ү���މ�������
/**********************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CCalcDlg1::OnCancel(void)											//<<< WM_CANCEL ү���މ�������
/******************************/
{
	DestroyWindow();
	View->EndCalcDlg1();												// �޲�۸ޏI��
}

/*******************************/
void CCalcDlg1::OnDestroy(void)											//<<< WM_DESTROY ү���މ�������
/*******************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "CalcLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CalcTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/****************************/
BOOL CCalcDlg1::Create(void)											//<<< �޲�۸ސ���
/****************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
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
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
	return ret;
}

/*********************************/
void CCalcDlg1::OnInputKey1(void)										//<<< Key:[1]���݉�������
/*********************************/
{
	InputKeyData(_T("1"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey2(void)										//<<< Key:[2]���݉�������
/*********************************/
{
	InputKeyData(_T("2"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey3(void)										//<<< Key:[3]���݉�������
/*********************************/
{
	InputKeyData(_T("3"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey4(void)										//<<< Key:[4]���݉�������
/*********************************/
{
	InputKeyData(_T("4"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey5(void)										//<<< Key:[5]���݉�������
/*********************************/
{
	InputKeyData(_T("5"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey6(void)										//<<< Key:[6]���݉�������
/*********************************/
{
	InputKeyData(_T("6"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey7(void)										//<<< Key:[7]���݉�������
/*********************************/
{
	InputKeyData(_T("7"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey8(void)										//<<< Key:[8]���݉�������
/*********************************/
{
	InputKeyData(_T("8"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey9(void)										//<<< Key:[9]���݉�������
/*********************************/
{
	InputKeyData(_T("9"));												// Key�ް����͏���
}

/*********************************/
void CCalcDlg1::OnInputKey0(void)										//<<< Key:[0]���݉�������
/*********************************/
{
	InputKeyData(_T("0"));												// Key�ް����͏���
}

/***********************************/
void CCalcDlg1::OnInputKeyDot(void)										//<<< Key:[.]���݉�������
/***********************************/
{
	InputKeyData(_T("."));												// Key�ް����͏���
}

/***********************************/
void CCalcDlg1::OnInputKeyPai(void)										//<<< Key:[Pai]���݉�������
/***********************************/
{
	m_strNumber = _T("3.1415926535");									// 12���܂œ���
	m_Count = 12;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// ���͒l�\��
}

/****************************************/
void CCalcDlg1::OnInputKeyAddition(void)								//<<< Key:[+]���݉�������
/****************************************/
{
	CheckInputData();													// ���ړ�����������
	ChangeKeyData();													// Key�ް����l�ϊ�����
	if(m_StartFlg>0&&m_Count>0) {										// <2��ڈȍ~�̓��͗L>
		CalcKeyData();													// Key�ް����l�v�Z����
	}
	if(m_Type==1) {														// <����[+]�̏ꍇ>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("+k"), m_Type2 = 1;							// [+]��[+k]�֕ύX
			m_Number = m_Value;											// ���̎��_�̌��ʂ���͒l��
		} else {
			m_strType = _T("+"), m_Type2 = 0;							// [+k]��[+]�֕ύX
		}
	} else {															// <�V�K��[+]�̏ꍇ>
		m_strType = _T("+"), m_Type = 1, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/****************************************/
void CCalcDlg1::OnInputKeySubtract(void)								//<<< Key:[-]���݉�������
/****************************************/
{
	CheckInputData();													// ���ړ�����������
	ChangeKeyData();													// Key�ް����l�ϊ�����
	if(m_StartFlg>0&&m_Count>0) {										// <2��ڈȍ~�̓��͗L>
		CalcKeyData();													// Key�ް����l�v�Z����
	}
	if(m_Type==2) {														// <����[-]�̏ꍇ>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("-k"), m_Type2 = 1;							// [-]��[-k]�֕ύX
			m_Number = m_Value;											// ���̎��_�̌��ʂ���͒l��
		} else {
			m_strType = _T("-"), m_Type2 = 0;							// [-k]��[-]�֕ύX
		}
	} else {															// <�V�K��[-]�̏ꍇ>
		m_strType = _T("-"), m_Type = 2, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/****************************************/
void CCalcDlg1::OnInputKeyMultiply(void)								//<<< Key:[x]���݉�������
/****************************************/
{
	CheckInputData();													// ���ړ�����������
	ChangeKeyData();													// Key�ް����l�ϊ�����
	if(m_StartFlg>0&&m_Count>0) {										// <2��ڈȍ~�̓��͗L>
		CalcKeyData();													// Key�ް����l�v�Z����
	}
	if(m_Type==3) {														// <����[x]�̏ꍇ>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("xk"), m_Type2 = 1;							// [x]��[xk]�֕ύX
			m_Number = m_Value;											// ���̎��_�̌��ʂ���͒l��
		} else {
			m_strType = _T("x"), m_Type2 = 0;							// [xk]��[x]�֕ύX
		}
	} else {															// <�V�K��[x]�̏ꍇ>
		m_strType = _T("x"), m_Type = 3, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/****************************************/
void CCalcDlg1::OnInputKeyDivision(void)								//<<< Key:[/]���݉�������
/****************************************/
{
	CheckInputData();													// ���ړ�����������
	ChangeKeyData();													// Key�ް����l�ϊ�����
	if(m_StartFlg>0&&m_Count>0) {										// <2��ڈȍ~�̓��͗L>
		CalcKeyData();													// Key�ް����l�v�Z����
	}
	if(m_Type==4) {														// <����[/]�̏ꍇ>
		if(m_Type2==0&&m_Count==0) {
			m_strType = _T("/k"), m_Type2 = 1;							// [/]��[/k]�֕ύX
			m_Number = m_Value;											// ���̎��_�̌��ʂ���͒l��
		} else {
			m_strType = _T("/"), m_Type2 = 0;							// [/k]��[/]�֕ύX
		}
	} else {															// <�V�K��[/]�̏ꍇ>
		m_strType = _T("/"), m_Type = 4, m_Type2 = 0;
	}
	m_Edit2.SetWindowText(m_strType);
	m_StartFlg = 1, m_Count = 0;
}

/*************************************/
void CCalcDlg1::OnInputKeyEqual(void)									//<<< Key:[=]���݉�������
/*************************************/
{
	if(m_Type2==1) {													// <�J�Ԃ��L�̏ꍇ>
		CalcKeyData();													// Key�ް����l�v�Z����
	} else {															// <�J�Ԃ����̏ꍇ>
		CheckInputData();												// ���ړ�����������
		ChangeKeyData();												// Key�ް����l�ϊ�����
		if(m_StartFlg>0&&m_Count>0) {									// <2��ڈȍ~�̓��͗L>
			CalcKeyData();												// Key�ް����l�v�Z����
		}
		m_strType = _T(""), m_Type = 0, m_Type2 = 0;
		m_Edit2.SetWindowText(m_strType);
	}
	m_StartFlg = 1, m_Count = 0;
}

/*********************************/
void CCalcDlg1::OnInputKeyC(void)										//<<< Key:[C]���݉�������
/*********************************/
{
	m_strNumber = _T("0"), m_strType = _T(""), m_strSign = _T("");		// ������
	m_Number = 0.0, m_Value = 0.0;
	m_Type = 0, m_Type2 = 0, m_Sign = 1, m_Count = 0, m_StartFlg = 0;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// �����l�\��
	m_Edit2.SetWindowText(m_strType);
}

/**********************************/
void CCalcDlg1::OnInputKeyCE(void)										//<<< Key:[CE]���݉�������
/**********************************/
{
	m_strNumber = _T("0"), m_strSign = _T("");							// ������
	m_Sign = 1, m_Count = 0;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// �����l�\��
}

/************************************/
void CCalcDlg1::OnInputKeySign(void)									//<<< Key:[+/-]���݉�������
/************************************/
{
	m_strSign = (m_Sign==1) ? _T("-") : _T("");							// ��������ւ�
	m_Sign = (m_Sign==1) ? -1 : 1;
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// ���͒l�\��
}

/************************************/
void CCalcDlg1::OnInputKeyCopy(void)									//<<< Key:[Copy]���݉�������
/************************************/
{
	m_Edit1.SetSel(0, -1);
	m_Edit1.Copy();														// ��߰
}

/************************************/
void CCalcDlg1::OnInputKeyOval(void)									//<<< Key:[Oval]���݉�������
/************************************/
{
	DestroyWindow();
	View->EndCalcDlg1();												// �޲�۸ޏI��
	if(!View->IsCalcDlg2Show()) {
		View->CreateCalcDlg2();											// �v�Z�@�޲�۸�2����
	}
}

/********************************************/
void CCalcDlg1::InputKeyData(CString strKey)							//<<< Key�ް����͏���
/********************************************/
{
	if(m_Count==0) {
		m_strNumber = _T("");											// �������ޯ̧�ر
		m_strSign = _T(""), m_Sign = 1;
	}
	if(m_Count<12) {
		m_strNumber += strKey;											// 12���܂œ���
		m_Count++;
		m_Edit1.SetWindowText(m_strSign+m_strNumber);					// ���͒l�\��
	} else {
		AfxMessageBox(IDS_ERR_INPUT);									// 12�����ʹװ
	}
}

/************************************/
void CCalcDlg1::CheckInputData(void)									//<<< ���ړ�����������
/************************************/
{
	CString str;
	int     len;

	m_Edit1.GetWindowText(str);											// ���͒l�擾
	len = str.GetLength();												// ���͕������擾
	m_strNumber = str.SpanIncluding(_T("0123456789."));					// ���l�̂ݒ��o
	m_Count = m_strNumber.GetLength();									// �L��������
	if(m_Count!=len||m_Count>12) {
		AfxMessageBox(IDS_ERR_INPUT);									// 12����/���l�ȊO�ʹװ
	}
	if(m_Count>12) {
		m_strNumber = m_strNumber.Left(12);								// 12���܂œ���
		m_Count = 12;
	}
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// ���͒l�\��
}

/***********************************/
void CCalcDlg1::ChangeKeyData(void)										//<<< Key�ް����l�ϊ�����
/***********************************/
{
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	if(m_StartFlg==0) {													// <<1��ڂ̕ϊ�>>
		if(m_Count==0) {
			m_Value = 0.0;												// �����͂�0
		} else {
			m_Value = atof(m_strSign+m_strNumber);						// ���͕�����𐔒l�ϊ�
		}
	} else {															// <<2��ڈȍ~�̕ϊ�>>
		if(m_Count>0) {
			m_Number = atof(m_strSign+m_strNumber);						// ���͕�����𐔒l�ϊ�
		}
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*********************************/
void CCalcDlg1::CalcKeyData(void)										//<<< Key�ް����l�v�Z����
/*********************************/
{
	CString str;
	int     len;

	if(m_Type==1) {														// <[+]�̏ꍇ>
		m_Value = m_Value + m_Number;
	} else if(m_Type==2) {												// <[-]�̏ꍇ>
		m_Value = m_Value - m_Number;
	} else if(m_Type==3) {												// <[x]�̏ꍇ>
		m_Value = m_Value * m_Number;
	} else if(m_Type==4) {												// <[/]�̏ꍇ>
		m_Value = (m_Number!=0) ? m_Value / m_Number : 0.0;
	}
	m_Sign = (m_Value<0.0) ? -1 : 1;
	m_strSign = (m_Value<0.0) ? _T("-") : _T("");
	m_strNumber.Format("%0.12f", fabs(m_Value));
	while(TRUE) {
		str = m_strNumber;
		len = m_strNumber.GetLength();									// ������(���l)
		if(str.Right(1)==_T("0")) {										// <������0�̏ꍇ>
			m_strNumber = str.Left(len-1);								// �������
		} else {
			break;
		}
	}
	str = m_strNumber;
	len = m_strNumber.GetLength();										// ������(���l)
	if(str.Right(1)==_T(".")) {											// <������.�̏ꍇ>
		m_strNumber = str.Left(len-1);									// �������
	}
	m_Edit1.SetWindowText(m_strSign+m_strNumber);						// ���ʕ\��
	if(fabs(m_Value)>999999999999) {
		AfxMessageBox(IDS_ERR_OVERFLOW);								// ���ް�۰
	}
}

///////////////////////////////////////////////////////////////////////////////
// CCalcDlg2 �_�C�A���O

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
void CCalcDlg2::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
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
BOOL CCalcDlg2::OnInitDialog(void)										//<<< WM_INITDIALOG ү���މ�������
/**********************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CCalcDlg2::OnCancel(void)											//<<< WM_CANCEL ү���މ�������
/******************************/
{
	DestroyWindow();
	View->EndCalcDlg2();												// �޲�۸ޏI��
}

/*******************************/
void CCalcDlg2::OnDestroy(void)											//<<< WM_DESTROY ү���މ�������
/*******************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "CalcLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CalcTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/****************************/
BOOL CCalcDlg2::Create(void)											//<<< �޲�۸ސ���
/****************************/
{
	CRect r;
	int   left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
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
		SetWindowPlacement(&wp);										// ����޳�\����Ԑݒ�
	}
	GetWindowRect(&r);
	m_Width = r.Width();
	m_Height = r.Height();												// ���ޕύX
	SetWindowPos(NULL, 0, 0, m_Width, (int)(m_Height*0.373), SWP_NOMOVE|SWP_NOZORDER);
	return ret;
}

/**********************************/
void CCalcDlg2::OnPasteData1(void)										//<<< [Paste1]���݉�������
/**********************************/
{
	m_Edit1.SetSel(0, -1);
	m_Edit1.Paste();													// �߰��
}

/*********************************/
void CCalcDlg2::OnCopyData1(void)										//<<< [Copy1]���݉�������
/*********************************/
{
	m_Edit1.SetSel(0, -1);
	m_Edit1.Copy();														// ��߰
}

/**********************************/
void CCalcDlg2::OnPasteData2(void)										//<<< [Paste2]���݉�������
/**********************************/
{
	m_Edit2.SetSel(0, -1);
	m_Edit2.Paste();													// �߰��
}

/*********************************/
void CCalcDlg2::OnCopyData2(void)										//<<< [Copy2]���݉�������
/*********************************/
{
	m_Edit2.SetSel(0, -1);
	m_Edit2.Copy();														// ��߰
}

/********************************/
void CCalcDlg2::OnHelpProc(void)										//<<< [Help]���݉�������
/********************************/
{
	CString str;
	int     hei;

	hei = (!m_Size) ? m_Height : (int)(m_Height*0.373);					// �c����
	m_Size = (!m_Size) ? 1 : 0;
	SetWindowPos(NULL, 0, 0, m_Width, hei, SWP_NOMOVE|SWP_NOZORDER);	// ���ޕύX
	if(m_Size) {
		str = "���ꂼ��̍��ڂ́A���̈Ӗ��Ŏg�p���Ă��܂�\n";
		str += "a,b:�I�[�o���̌a\nd:����ɕ��ׂ�~(�����_�C��)�̒��a\n";
		str += "n:���ׂ�~�̌�\n(�E�}�Q��)\n";
		str += "���߂���l��n(��)�݂̂ł�\n";
		str += "a,b,d�̒l�͕K�����͂��Ă�������\n";
		str += "�܂��A���߂�ꂽ�l�͋ߎ��l�ł�������܂���\n";
		str += "�Q�l���x�Ƃ��āA���g�p��������\n\n";
		str += "�v�Z�̎��s��[EXE]�{�^���������Ă�������\n";
		str += "�܂��A[Copy]�{�^���̓N���b�v�{�[�h���g�p�����R�s�[���A";
		str += "[Paste]�{�^���̓N���b�v�{�[�h���g�p�����l���͂��ł��܂�\n";
		SetDlgItemText(IDC_EDIT5, str);									// �����\��
	}
}

/**********************************/
void CCalcDlg2::OnPasteData3(void)										//<<< [Paste3]���݉�������
/**********************************/
{
	m_Edit3.SetSel(0, -1);
	m_Edit3.Paste();													// �߰��
}

/*********************************/
void CCalcDlg2::OnCopyData3(void)										//<<< [Copy3]���݉�������
/*********************************/
{
	m_Edit3.SetSel(0, -1);
	m_Edit3.Copy();														// ��߰
}

/********************************/
void CCalcDlg2::OnInputCls(void)										//<<< [Cls]���݉�������
/********************************/
{
	m_strNumber1 = _T("1"), m_strNumber2 = _T("1");						// ������
	m_strNumber3 = _T("1"), m_strNumber4 = _T("");
	m_Number1 = 1.0, m_Number2 = 1.0, m_Number3 = 1.0;
	m_Edit1.SetWindowText(m_strNumber1);								// �����l�\��
	m_Edit2.SetWindowText(m_strNumber2);
	m_Edit3.SetWindowText(m_strNumber3);
	m_Edit4.SetWindowText(m_strNumber4);
}

/*********************************/
void CCalcDlg2::OnCopyData4(void)										//<<< [Copy4]���݉�������
/*********************************/
{
	m_Edit4.SetSel(0, -1);
	m_Edit4.Copy();														// ��߰
}

/*******************************/
void CCalcDlg2::OnExeProc(void)											//<<< [EXE]���݉�������
/*******************************/
{
	int    num;
	double len;

	CheckInputData();													// ���ړ�����������
	len = CalcCircumference((m_Number1+m_Number3)/2.0,
                            (m_Number2+m_Number3)/2.0);					// �ȉ~�~���擾
    num = (int)(len/m_Number3);											// �~���ɔz�u�ł����
	m_strNumber4.Format("%d", num);
	m_Edit4.SetWindowText(m_strNumber4);								// ���\��
}

/**********************************/
void CCalcDlg2::OnChangeCalc(void)										//<<< [Calc]���݉�������
/**********************************/
{
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	View->SetCalc(m_strNumber1, m_strNumber2,
                  m_strNumber3, m_strNumber4);							// �ް��ۑ�
	DestroyWindow();
	View->EndCalcDlg2();												// �޲�۸ޏI��
	if(!View->IsCalcDlg1Show()) {
		View->CreateCalcDlg1();											// �v�Z�@�޲�۸�1����
	}
}

/************************************/
void CCalcDlg2::CheckInputData(void)									//<<< ���ړ�����������
/************************************/
{
	CString str;
	int     ln1, ln2;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	m_Edit1.GetWindowText(str);											// ���͒l1�擾
	ln1 = str.GetLength();												// ���͕������擾
	m_strNumber1 = str.SpanIncluding(_T("-0123456789."));				// ���l�̂ݒ��o
	ln2 = m_strNumber1.GetLength();										// �L��������
	if(ln1!=ln2) {
		AfxMessageBox(IDS_ERR_INPUT);									// ���l�ȊO�ʹװ
	}
	m_Edit1.SetWindowText(m_strNumber1);								// ���͒l1�\��
	m_Number1 = atof(m_strNumber1);										// ���͕�����1�𐔒l�ϊ�
	m_Edit2.GetWindowText(str);											// ���͒l2�擾
	ln1 = str.GetLength();												// ���͕������擾
	m_strNumber2 = str.SpanIncluding(_T("-0123456789."));				// ���l�̂ݒ��o
	ln2 = m_strNumber2.GetLength();										// �L��������
	if(ln1!=ln2) {
		AfxMessageBox(IDS_ERR_INPUT);									// ���l�ȊO�ʹװ
	}
	m_Edit2.SetWindowText(m_strNumber2);								// ���͒l2�\��
	m_Number2 = atof(m_strNumber2);										// ���͕�����2�𐔒l�ϊ�
	m_Edit3.GetWindowText(str);											// ���͒l3�擾
	ln1 = str.GetLength();												// ���͕������擾
	m_strNumber3 = str.SpanIncluding(_T("-0123456789."));				// ���l�̂ݒ��o
	ln2 = m_strNumber3.GetLength();										// �L��������
	if(ln1!=ln2) {
		AfxMessageBox(IDS_ERR_INPUT);									// ���l�ȊO�ʹװ
	}
	m_Edit3.SetWindowText(m_strNumber3);								// ���͒l3�\��
	m_Number3 = atof(m_strNumber3);										// ���͕�����3�𐔒l�ϊ�
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/*******************************************************/
double CCalcDlg2::CalcCircumference(double a, double b)					//<<< �ȉ~�~���擾
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
void CCalcDlg2::SetParam(CString a, CString b, CString c, CString d)	//<<< ���Ұ��ݒ�
/********************************************************************/
{
	m_strNumber1 = a, m_strNumber2 = b;
	m_strNumber3 = c, m_strNumber4 = d;
	UpdateData(FALSE);													// �޲�۸ޏ���ް�����
}
