/////////////////////////////////////////////////////////////////////////////
// ���\���_�C�A���O�N���X��`
// InfoDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "MainFrm.h"
#include "DataAccess1.h"

///////////////////////////////////////////////////////////////////////////////
// CStateDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CStateDlg, CDialog)									// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************/
CStateDlg::CStateDlg(CWnd* pParent) : CDialog(CStateDlg::IDD, pParent)
/**********************************************************************/
{
	m_pParent  = pParent;
	m_Cent3DX  = 0.0;
	m_Cent3DY  = 0.0;
	m_Cent3DZ  = 0.0;
	m_EyeX     = 0.0;
	m_EyeY     = 0.0;
	m_EyeZ     = 0.0;
	m_CentPRSX = 0.0;
	m_CentPRSY = 0.0;
	m_CentPRSZ = 0.0;
}

/**************************************************/
void CStateDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Cent3DX);
	DDX_Text(pDX, IDC_EDIT2, m_Cent3DY);
	DDX_Text(pDX, IDC_EDIT3, m_Cent3DZ);
	DDX_Text(pDX, IDC_EDIT4, m_EyeX);
	DDX_Text(pDX, IDC_EDIT5, m_EyeY);
	DDX_Text(pDX, IDC_EDIT6, m_EyeZ);
	DDX_Text(pDX, IDC_EDIT7, m_CentPRSX);
	DDX_Text(pDX, IDC_EDIT8, m_CentPRSY);
	DDX_Text(pDX, IDC_EDIT9, m_CentPRSZ);
}

/***************************************************/
inline LRESULT CStateDlg::OnNcHitTest(CPoint point)						//<<< WM_NCHITTEST ү���މ�������
/***************************************************/
{
	LRESULT hit;

	if((hit=CDialog::OnNcHitTest(point))!=HTCLIENT) {
		return hit;
	} else {
		return HTCAPTION;												// �ײ��ė̈����ׯ�ނ��ĳ���޳��
	}																	// �ړ����邽��
}

/**********************************/
BOOL CStateDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ү���މ�������
/**********************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CStateDlg::OnCancel(void)											//<<< WM_CANCEL ү���މ�������
/******************************/
{
	View->HideStateDlg();
	ShowWindow(SW_HIDE);
}

/*******************************/
void CStateDlg::OnDestroy(void)											//<<< WM_DESTROY ү���މ�������
/*******************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "StaLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "StaTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/****************************/
BOOL CStateDlg::Create(void)											//<<< �޲�۸ސ���
/****************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "StaLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "StaTop", -1);
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

/**********************************************/
void CStateDlg::SetCent3D(const PNTTYPE& cent)							//<<< 3�ʐ}���S�_�ݒ�
/**********************************************/
{
	m_Cent3DX = cent.p[0];
	m_Cent3DY = cent.p[1];
	m_Cent3DZ = cent.p[2];
	UpdateData(FALSE);
}

/******************************************/
void CStateDlg::SetEye(const PNTTYPE& eye)								//<<< �����}���_�ݒ�
/******************************************/
{
	m_EyeX = eye.p[0];
	m_EyeY = eye.p[1];
	m_EyeZ = eye.p[2];
	UpdateData(FALSE);
}

/***********************************************/
void CStateDlg::SetCentPRS(const PNTTYPE& cent)							//<<< �����}�Q�Ɠ_�ݒ�
/***********************************************/
{
	m_CentPRSX = cent.p[0];
	m_CentPRSY = cent.p[1];
	m_CentPRSZ = cent.p[2];
	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// CDataUseDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CDataUseDlg, CDialog)									// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/****************************************************************************/
CDataUseDlg::CDataUseDlg(CWnd* pParent) : CDialog(CDataUseDlg::IDD, pParent)
/****************************************************************************/
{
	m_pParent = pParent;
	m_IntMax  = _T("");
	m_PntMax  = _T("");
	m_UndoMax = _T("");
	m_IntNum  = _T("");
	m_PntNum  = _T("");
	m_IntUse  = _T("");
	m_PntUse  = _T("");
}

/****************************************************/
void CDataUseDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_IntMax);
	DDX_Text(pDX, IDC_EDIT2, m_PntMax);
	DDX_Text(pDX, IDC_EDIT3, m_UndoMax);
	DDX_Text(pDX, IDC_EDIT4, m_IntNum);
	DDX_Text(pDX, IDC_EDIT5, m_PntNum);
	DDX_Text(pDX, IDC_EDIT6, m_IntUse);
	DDX_Text(pDX, IDC_EDIT7, m_PntUse);
}

/***********************************************/
inline LRESULT CDataUseDlg::OnNcHitTest(CPoint)							//<<< WM_NCHITTEST ү���މ�������
/***********************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/************************************/
BOOL CDataUseDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	SetData();
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/********************************/
void CDataUseDlg::OnCancel(void)										//<<< WM_CANCEL ү���މ�������
/********************************/
{
	View->HideDataUse();
	ShowWindow(SW_HIDE);
}

/*********************************/
void CDataUseDlg::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "DatLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "DatTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************/
BOOL CDataUseDlg::Create(void)											//<<< �޲�۸ސ���
/******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "DatLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "DatTop", -1);
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

/*******************************/
void CDataUseDlg::SetData(void)											//<<< �ް��g�p���ݒ�
/*******************************/
{
	m_IntMax.Format("%d", GetIntMax());
	m_PntMax.Format("%d", GetPntMax());
	m_UndoMax.Format("%d", GetUndoMax());
	m_IntNum.Format("%d", GetIntUseNum());
	m_PntNum.Format("%d", GetPntUseNum());
	m_IntUse.Format("%.2f %%", GetIntUse());
	m_PntUse.Format("%.2f %%", GetPntUse());
	UpdateData(FALSE);
}

///////////////////////////////////////////////////////////////////////////////
// CFunctionDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CFunctionDlg, CDialog)								// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonClick)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
END_MESSAGE_MAP()

/*******************************************************************************/
CFunctionDlg::CFunctionDlg(CWnd* pParent) : CDialog(CFunctionDlg::IDD, pParent)
/*******************************************************************************/
{
	m_pParent = pParent;
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = 0;
	m_Check10 = 0;
	m_Check11 = 0;
	m_Check12 = 0;
	m_Lock    = 0;
}

/*****************************************************/
void CFunctionDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Control(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_EDIT5, m_Edit5);
	DDX_Control(pDX, IDC_EDIT6, m_Edit6);
	DDX_Control(pDX, IDC_EDIT7, m_Edit7);
	DDX_Control(pDX, IDC_EDIT8, m_Edit8);
	DDX_Control(pDX, IDC_EDIT9, m_Edit9);
	DDX_Control(pDX, IDC_EDIT10, m_Edit10);
	DDX_Control(pDX, IDC_EDIT11, m_Edit11);
	DDX_Control(pDX, IDC_EDIT12, m_Edit12);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Check(pDX, IDC_CHECK5, m_Check5);
	DDX_Check(pDX, IDC_CHECK6, m_Check6);
	DDX_Check(pDX, IDC_CHECK7, m_Check7);
	DDX_Check(pDX, IDC_CHECK8, m_Check8);
	DDX_Check(pDX, IDC_CHECK9, m_Check9);
	DDX_Check(pDX, IDC_CHECK10, m_Check10);
	DDX_Check(pDX, IDC_CHECK11, m_Check11);
	DDX_Check(pDX, IDC_CHECK12, m_Check12);
}

/************************************************/
inline LRESULT CFunctionDlg::OnNcHitTest(CPoint)						//<<< WM_NCHITTEST ү���މ�������
/************************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/*************************************/
BOOL CFunctionDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/*************************************/
{
	int func[8];

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	View->GetFChar(func);												// ̧ݸ��ݷ��擾
	m_Check1  = (func[0]) ? 1 : 0;										// �t��ON/OFF��ݒ�
	m_Check2  = (func[1]) ? 1 : 0;
	m_Check3  = (func[2]) ? 1 : 0;
	m_Check4  = (func[3]) ? 1 : 0;
	m_Check5  = (func[4]) ? 1 : 0;
	m_Check6  = (func[5]) ? 1 : 0;
	m_Check7  = (func[6]) ? 1 : 0;
	m_Check8  = (func[7]) ? 1 : 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*********************************/
void CFunctionDlg::OnCancel(void)										//<<< WM_CANCEL ү���މ�������
/*********************************/
{
	MessageOut(-1, 0);													// ̧ݸ���ү���ޏ�����
	View->HideFunction();												// �޲�۸ޏ���(��\��)
}

/**********************************/
void CFunctionDlg::OnDestroy(void)										//<<< WM_DESTROYү���މ�������
/**********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "FncLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "FncTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**************************************/
void CFunctionDlg::OnButtonClick(void)									//<<< ��ƭ��N��]����ݏ���
/**************************************/
{
	if(m_nID>0) {
		MainWnd->ExecuteMenu((UINT)m_nID);								// ̧ݸ��ݑI���ƭ��N��
	}
}

/*********************************/
void CFunctionDlg::OnCheck1(void)										//<<< ��������1
/*********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check1, 0x1);										// [F5]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck2(void)										//<<< ��������2
/*********************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check2, 0x2);										// [F6]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck3(void)										//<<< ��������3
/*********************************/
{
	m_Check3 = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check3, 0x4);										// [F7]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck4(void)										//<<< ��������4
/*********************************/
{
	m_Check4 = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check4, 0x8);										// [F8]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck5(void)										//<<< ��������5
/*********************************/
{
	m_Check5 = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check5, 0x10);										// [F9]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck6(void)										//<<< ��������6
/*********************************/
{
	m_Check6 = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check6, 0x20);										// [F10]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck7(void)										//<<< ��������7
/*********************************/
{
	m_Check7 = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check7, 0x40);										// [F11]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck8(void)										//<<< ��������8
/*********************************/
{
	m_Check8 = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check8, 0x80);										// [F12]���̍X�V
}

/*********************************/
void CFunctionDlg::OnCheck9(void)										//<<< COP��������
/*********************************/
{
	m_Check9 = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCopyBtn(m_Check9);											// [COP]���݂̍X�V
}

/**********************************/
void CFunctionDlg::OnCheck10(void)										//<<< NUM��������
/**********************************/
{
	m_Check10 = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetNumBtn(m_Check10);											// [NUM]���݂̍X�V
}

/**********************************/
void CFunctionDlg::OnCheck11(void)										//<<< Shift��������
/**********************************/
{
	m_Check11 = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCChar(m_Check11, 0x1);										// [Shift]���̍X�V
}

/**********************************/
void CFunctionDlg::OnCheck12(void)										//<<< Shift��������
/**********************************/
{
	m_Check12 = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCChar(m_Check12, 0x2);										// [Ctrl]���̍X�V
}

/*******************************/
BOOL CFunctionDlg::Create(void)											//<<< �޲�۸ސ���
/*******************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "FncLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "FncTop", -1);
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

/*************************************/
void CFunctionDlg::OffAllButton(void)									//<<< �S�tOFF
/*************************************/
{
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/***********************************************/
void CFunctionDlg::OnOffButton(int no, int flg)							//<<< �w��tON/OFF
/***********************************************/
{
	if(no==0) {
		m_Check1 = flg;
	} else if(no==1) {
		m_Check2 = flg;
	} else if(no==2) {
		m_Check3 = flg;
	} else if(no==3) {
		m_Check4 = flg;
	} else if(no==4) {
		m_Check5 = flg;
	} else if(no==5) {
		m_Check6 = flg;
	} else if(no==6) {
		m_Check7 = flg;
	} else if(no==7) {
		m_Check8 = flg;
	} else if(no==8) {
		m_Check9 = flg;
	} else if(no==9) {
		m_Check10 = flg;
	} else if(no==10) {
		m_Check11 = flg;
	} else if(no==11) {
		m_Check12 = flg;
	}
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/***********************************************/
void CFunctionDlg::MessageOut(int nID, int flg)							//<<< ү���ޏo��
/***********************************************/
{
	CString strMess, strBuff[13], strCKey, strNKey;
	int     i, pos, kenv;

	if(!View->IsFuncDsp()) {											// �޲�۸ޕ\�����͒��~
		return;
	}
	if(flg==0&&m_Lock==1&&nID>0) {										// ۯ���Ԃ͔�ۯ�ү���ޕ\����
		return;
	}
	m_Lock = flg;														// ۯ���Ԃ��X�V
	View->ShowFunction();												// ̧ݸ����޲�۸ޕ\��
	m_nID = 0;															// �ƭ�ID������
	for(i=0; i<13; i++) {
		strBuff[i] = _T("");											// ̧ݸ������ُ�����
	}
	if(nID>=10200&&nID<12000) {											// <JCAD3�ŗL���ƭ�>
		strMess.LoadString(nID+5000);									// ؿ������ү���ގ擾
		for(i=0; i<13; i++) {
			pos = strMess.Find("|");									// ��ؕ���(|)
			strBuff[i] = strMess.Left(pos);
			strMess = strMess.Mid(pos+1);
		}
		if(strBuff[0]=="A") {											// <��ɢ�ƭ��N������L��>
			m_Button1.EnableWindow(TRUE);								// ��ƭ��N������ݗL��
			m_nID = nID;												// �N���ƭ�ID�ۑ�
		} else if(strBuff[0]=="B") {									// <�I�𗧑̗L�Ţ�ƭ��N������L��>
			if(View->GetEdtObjNum()>0) {
				m_Button1.EnableWindow(TRUE);							// ��ƭ��N������ݗL��
				m_nID = nID;											// �N���ƭ�ID�ۑ�
			} else {
				m_Button1.EnableWindow(FALSE);							// ��ƭ��N������ݖ���
			}
		} else if(strBuff[0]=="C"&&nID==ID_EDIT_PASTEOBJ) {				// <�߰�ĥ��޼ު�Ă̏ꍇ>
			if(MainWnd->GetCopyCmdFlg()) {								// ��߰���̗L��
				m_Button1.EnableWindow(TRUE);							// ��ƭ��N������ݗL��
				m_nID = nID;											// �N���ƭ�ID�ۑ�
			} else {
				m_Button1.EnableWindow(FALSE);							// ��ƭ��N������ݖ���
			}
		} else {
			m_Button1.EnableWindow(FALSE);								// ��ƭ��N������ݖ���
		}
		kenv = View->GetKeyEnv();										// ���ݒ���擾
		if(kenv&0x10) {													// ���l���Ķ��=Shift
			strCKey = "[Ctrl]";
			strNKey = "[Shift]";
		} else {														// ���l���Ķ��=Ctrl
			strCKey = "[Shift]";
			strNKey = "[Ctrl]";
		}
		if(strBuff[11]!=_T("")) {
			strBuff[11] += strNKey;
		}
		if(strBuff[12]!=_T("")) {
			strBuff[12] += strCKey;
		}
	} else if(nID>0) {													// <Windows���ʂ��ƭ�>
		m_Button1.EnableWindow(FALSE);									// ��ƭ��N������ݖ���
		strMess.LoadString(nID);										// ؿ������ү���ގ擾
		pos = strMess.Find("\n");										// ��ؕ���(\n)
		strBuff[2] = strMess.Left(pos);
		strBuff[1] = strMess.Mid(pos+1);
	} else {															// <�ƭ��Ȃ�(���)>
		m_Button1.EnableWindow(FALSE);									// ��ƭ��N������ݖ���
	}
	m_Edit1.SetWindowText(strBuff[3]);									// ̧ݸ��ݖ�(F5)
	m_Edit2.SetWindowText(strBuff[4]);									//     �V   (F6)
	m_Edit3.SetWindowText(strBuff[5]);									//     �V   (F7)
	m_Edit4.SetWindowText(strBuff[6]);									//     �V   (F8)
	m_Edit5.SetWindowText(strBuff[7]);									//     �V   (F9)
	m_Edit6.SetWindowText(strBuff[8]);									//     �V   (F10)
	m_Edit7.SetWindowText(strBuff[9]);									//     �V   (F11)
	m_Edit8.SetWindowText(strBuff[10]);									//     �V   (F12)
	m_Edit9.SetWindowText(strBuff[12]);									//     �V   (COP)
	m_Edit10.SetWindowText(strBuff[11]);								//     �V   (NUM)
	m_Edit11.SetWindowText(strBuff[1]);									// ���j���[��
	m_Edit12.SetWindowText(strBuff[2]);									// �����T�v
}

///////////////////////////////////////////////////////////////////////////////
// CFunction2Dlg �_�C�A���O

BEGIN_MESSAGE_MAP(CFunction2Dlg, CDialog)								// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
END_MESSAGE_MAP()

/**********************************************************************************/
CFunction2Dlg::CFunction2Dlg(CWnd* pParent) : CDialog(CFunction2Dlg::IDD, pParent)
/**********************************************************************************/
{
	m_pParent = pParent;
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = 0;
	m_Check10 = 0;
	m_Check11 = 0;
	m_Check12 = 0;
}

/******************************************************/
void CFunction2Dlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Check(pDX, IDC_CHECK5, m_Check5);
	DDX_Check(pDX, IDC_CHECK6, m_Check6);
	DDX_Check(pDX, IDC_CHECK7, m_Check7);
	DDX_Check(pDX, IDC_CHECK8, m_Check8);
	DDX_Check(pDX, IDC_CHECK9, m_Check9);
	DDX_Check(pDX, IDC_CHECK10, m_Check10);
	DDX_Check(pDX, IDC_CHECK11, m_Check11);
	DDX_Check(pDX, IDC_CHECK12, m_Check12);
}

/*************************************************/
inline LRESULT CFunction2Dlg::OnNcHitTest(CPoint)						//<<< WM_NCHITTEST ү���މ�������
/*************************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/**************************************/
BOOL CFunction2Dlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	int func[8];

	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	View->GetFChar(func);												// ̧ݸ��ݷ��擾
	m_Check1  = (func[0]) ? 1 : 0;										// �t��ON/OFF��ݒ�
	m_Check2  = (func[1]) ? 1 : 0;
	m_Check3  = (func[2]) ? 1 : 0;
	m_Check4  = (func[3]) ? 1 : 0;
	m_Check5  = (func[4]) ? 1 : 0;
	m_Check6  = (func[5]) ? 1 : 0;
	m_Check7  = (func[6]) ? 1 : 0;
	m_Check8  = (func[7]) ? 1 : 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CFunction2Dlg::OnCancel(void)										//<<< WM_CANCEL ү���މ�������
/**********************************/
{
	View->HideFunction2();												// �޲�۸ޏ���(��\��)
}

/***********************************/
void CFunction2Dlg::OnDestroy(void)										//<<< WM_DESTROYү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Fnc2Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Fnc2Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**********************************/
void CFunction2Dlg::OnCheck1(void)										//<<< ��������1
/**********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check1, 0x1);										// [F5]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck2(void)										//<<< ��������2
/**********************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check2, 0x2);										// [F6]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck3(void)										//<<< ��������3
/**********************************/
{
	m_Check3 = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check3, 0x4);										// [F7]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck4(void)										//<<< ��������4
/**********************************/
{
	m_Check4 = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check4, 0x8);										// [F8]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck5(void)										//<<< ��������5
/**********************************/
{
	m_Check5 = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check5, 0x10);										// [F9]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck6(void)										//<<< ��������6
/**********************************/
{
	m_Check6 = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check6, 0x20);										// [F10]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck7(void)										//<<< ��������7
/**********************************/
{
	m_Check7 = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check7, 0x40);										// [F11]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck8(void)										//<<< ��������8
/**********************************/
{
	m_Check8 = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetFChar(m_Check8, 0x80);										// [F12]���̍X�V
}

/**********************************/
void CFunction2Dlg::OnCheck9(void)										//<<< COP��������
/**********************************/
{
	m_Check9 = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCopyBtn(m_Check9);											// [COP]���݂̍X�V
}

/***********************************/
void CFunction2Dlg::OnCheck10(void)										//<<< NUM��������
/***********************************/
{
	m_Check10 = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetNumBtn(m_Check10);											// [NUM]���݂̍X�V
}

/***********************************/
void CFunction2Dlg::OnCheck11(void)										//<<< Shift��������
/***********************************/
{
	m_Check11 = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCChar(m_Check11, 0x1);										// [Shift]���̍X�V
}

/***********************************/
void CFunction2Dlg::OnCheck12(void)										//<<< Shift��������
/***********************************/
{
	m_Check12 = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCChar(m_Check12, 0x2);										// [Ctrl]���̍X�V
}

/********************************/
BOOL CFunction2Dlg::Create(void)										//<<< �޲�۸ސ���
/********************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Fnc2Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Fnc2Top", -1);
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

/**************************************/
void CFunction2Dlg::OffAllButton(void)									//<<< �S�tOFF
/**************************************/
{
	m_Check1  = 0;
	m_Check2  = 0;
	m_Check3  = 0;
	m_Check4  = 0;
	m_Check5  = 0;
	m_Check6  = 0;
	m_Check7  = 0;
	m_Check8  = 0;
	m_Check9  = View->GetCopyBtn();
	m_Check10 = View->GetNumBtn();
	m_Check11 = View->GetCChar()&0x1;
	m_Check12 = View->GetCChar()&0x2;
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/************************************************/
void CFunction2Dlg::OnOffButton(int no, int flg)						//<<< �w��tON/OFF
/************************************************/
{
	if(no==0) {
		m_Check1 = flg;
	} else if(no==1) {
		m_Check2 = flg;
	} else if(no==2) {
		m_Check3 = flg;
	} else if(no==3) {
		m_Check4 = flg;
	} else if(no==4) {
		m_Check5 = flg;
	} else if(no==5) {
		m_Check6 = flg;
	} else if(no==6) {
		m_Check7 = flg;
	} else if(no==7) {
		m_Check8 = flg;
	} else if(no==8) {
		m_Check9 = flg;
	} else if(no==9) {
		m_Check10 = flg;
	} else if(no==10) {
		m_Check11 = flg;
	} else if(no==11) {
		m_Check12 = flg;
	}
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

///////////////////////////////////////////////////////////////////////////////
// CCurveCtrlDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CCurveCtrlDlg, CDialog)								// ү���ޥϯ��
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_EN_CHANGE(IDC_EDIT1, OnSetAngle)
	ON_EN_CHANGE(IDC_EDIT2, OnSetRadius)
END_MESSAGE_MAP()

/**********************************************************************************/
CCurveCtrlDlg::CCurveCtrlDlg(CWnd* pParent) : CDialog(CCurveCtrlDlg::IDD, pParent)
/**********************************************************************************/
{
	m_pParent = pParent;
	m_Check1  = 0;
	m_Check2  = 0;
	View->GetCurveData(&m_Angle, &m_Radius);							// �Ȑ�����p�x�ݒ�
}

/******************************************************/
void CCurveCtrlDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_STATIC2, m_Static2);
	DDX_Control(pDX, IDC_STATIC3, m_Static3);
	DDX_Control(pDX, IDC_STATIC4, m_Static4);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Text(pDX, IDC_EDIT1, m_Angle);
	DDV_MinMaxDouble(pDX, m_Angle, -359.0, 359.0);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Text(pDX, IDC_EDIT2, m_Radius);
	DDV_MinMaxDouble(pDX, m_Radius, 0.0, 100.0);
}

/*************************************************/
inline LRESULT CCurveCtrlDlg::OnNcHitTest(CPoint)						//<<< WM_NCHITTEST ү���މ�������
/*************************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/**************************************/
BOOL CCurveCtrlDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CDialog::OnInitDialog();
	CenterWindow(m_pParent);
	PermissionCheck1();													// ���͋�������1
	PermissionCheck2();													// ���͋�������2
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/**********************************/
void CCurveCtrlDlg::OnCancel(void)										//<<< WM_CANCEL ү���މ�������
/**********************************/
{
	ShowWindow(SW_HIDE);
}

/***********************************/
void CCurveCtrlDlg::OnDestroy(void)										//<<< WM_DESTROYү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "CtlLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CtlTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/**********************************/
void CCurveCtrlDlg::OnCheck1(void)										//<<< ��������1
/**********************************/
{
	m_Check1 = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCurveAngle(m_Check1, m_Angle);								// �Ȑ�����p�x�ݒ�
	PermissionCheck1();													// ���͋�������1
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/**********************************/
void CCurveCtrlDlg::OnCheck2(void)										//<<< ��������2
/**********************************/
{
	m_Check2 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()==1;		// �t��ON/OFF��Ԃ��擾
	View->SetCurveRadius(m_Check2, m_Radius);							// �Ȑ�����q�ݒ�
	PermissionCheck2();													// ���͋�������2
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

/************************************/
void CCurveCtrlDlg::OnSetAngle(void)									//<<< �p�x�ݒ�
/************************************/
{
	CString str;

	m_Edit1.GetWindowText(str);
	if(str!="-") {
		UpdateData(TRUE);												// �޲�۸ޏ���ް��擾
		View->SetCurveAngle(m_Check1, m_Angle);							// �Ȑ�����p�x�ݒ�
	}
}

/*************************************/
void CCurveCtrlDlg::OnSetRadius(void)									//<<< ���a�ݒ�
/*************************************/
{
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	View->SetCurveRadius(m_Check2, m_Radius);							// �Ȑ�����q�ݒ�
}

/******************************************/
void CCurveCtrlDlg::PermissionCheck1(void)								//<<< ���͋�������1
/******************************************/
{
	m_Static1.EnableWindow(m_Check1);
	m_Static2.EnableWindow(m_Check1);
	m_Edit1.EnableWindow(m_Check1);
}

/******************************************/
void CCurveCtrlDlg::PermissionCheck2(void)								//<<< ���͋�������2
/******************************************/
{
	m_Static3.EnableWindow(m_Check2);
	m_Static4.EnableWindow(m_Check2);
	m_Edit2.EnableWindow(m_Check2);
}

/********************************/
BOOL CCurveCtrlDlg::Create(void)										//<<< �޲�۸ސ���
/********************************/
{
	int left, top, width, height;

	BOOL ret = CDialog::Create(IDD, m_pParent);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "CtlLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "CtlTop", -1);
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
void CCurveCtrlDlg::ClearData(void)										//<<< �ް��ر
/***********************************/
{
	m_Check1 = 0;
	m_Check2 = 0;
	PermissionCheck1();													// ���͋�������1
	PermissionCheck2();													// ���͋�������2
	UpdateData(FALSE);													// �޲�۸ޏ���ް��\��
}

///////////////////////////////////////////////////////////////////////////////
// CProgressDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)								// ү���ޥϯ��
END_MESSAGE_MAP()

/*****************************************************************************************/
CProgressDlg::CProgressDlg(int kind, CWnd* pParent) : CDialog(CProgressDlg::IDD, pParent)
/*****************************************************************************************/
{
	m_pParent = pParent;
	m_Kind    = kind;
}

/*****************************************************/
void CProgressDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/*****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_CHECK1, m_Button);
}

/*************************************/
BOOL CProgressDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/*************************************/
{
	CString title;

	CDialog::OnInitDialog();
	SetProgress(0);
	if(m_Kind!=0) {
		if(m_Kind==1) {													// ���َ擾
			title.LoadString(IDS_PROGRESS1);
		} else if(m_Kind==2) {
			title.LoadString(IDS_PROGRESS2);
		} else if(m_Kind==3) {
			title.LoadString(IDS_PROGRESS3);
		} else if(m_Kind==4) {
			title.LoadString(IDS_PROGRESS4);
		} else if(m_Kind==5) {
			title.LoadString(IDS_PROGRESS5);
		} else {
			title.LoadString(IDS_PROGRESS6);
		}
		SetWindowText(title);											// ���ٕ\��
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*******************************/
BOOL CProgressDlg::Create(void)											//<<< �޲�۸ސ���
/*******************************/
{
	return CDialog::Create(IDD, m_pParent);
}

/***************************************/
void CProgressDlg::SetProgress(int per)									//<<< �i�����\��
/***************************************/
{
	m_Progress.SetPos(per);												// ��۸�ڽ�ް���۰ٌ��݈ʒu�ݒ�
}

///////////////////////////////////////////////////////////////////////////////
// CConfirmDlg �_�C�A���O

BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)									// ү���ޥϯ��
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonClick)
END_MESSAGE_MAP()

/****************************************************************************/
CConfirmDlg::CConfirmDlg(CWnd* pParent) : CDialog(CConfirmDlg::IDD, pParent)
/****************************************************************************/
{
	m_EndMode = 0;
}

/****************************************************/
void CConfirmDlg::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/****************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Button1);
}

/************************************/
BOOL CConfirmDlg::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/************************************/
{
	int left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "CofimLeft", -1);
	top = theApp.GetProfileInt("LAYOUT", "CofimTop", -1);
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
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*********************************/
void CConfirmDlg::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/*********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "CofimLeft", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "CofimTop", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/*************************************/
void CConfirmDlg::OnButtonClick(void)									//<<< ��I��[�ۑ��L]����ݏ���
/*************************************/
{
	m_EndMode = 1;														// �ۑ��L�I��
	CDialog::OnOK();
}
