/////////////////////////////////////////////////////////////////////////////
// ���l���̓_�C�A���O�N���X(CInputDlg)��`
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
// CInputDlg �_�C�A���O

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
void CInputDlg::DoDataExchange(CDataExchange* pDX)						//<<< �޲�۸ޥ�ް��̌����ƗL��������
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
	if(m_Kind==1) {														// <��د�ފԊu(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_ParamI2);
		DDV_MinMaxInt(pDX, m_ParamI2, 1, 50);
	} else if(m_Kind==2) {												// <����(3)-����è��>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.0, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.0, 1000.0);
		DDX_Text(pDX, IDC_EDIT3, m_Param3);
		DDV_MinMaxDouble(pDX, m_Param3, 0.0, 1000.0);
	} else if(m_Kind==3) {												// <����(2)-�Θg>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 1000.0);
	} else if(m_Kind==4) {												// <����(1)-�Θg>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
	} else if(m_Kind==5) {												// <����(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, -50.0, 50.0);
	} else if(m_Kind==6) {												// <�~�ʌ`��(1)-��]��>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 1.0, 360.0);
	} else if(m_Kind==7) {												// <����(2)+��-��ζ��>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT3, m_Param3);
		DDV_MinMaxDouble(pDX, m_Param3, 0.0, 100.0);
	} else if(m_Kind==8) {												// <����(1)+��-��ζ��>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 0.0, 100.0);
	} else if(m_Kind==9) {												// <�Ȑ����Ұ�(2)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 1, 5);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 1.0, 3.0);
	} else if(m_Kind==10) {												// <�|��������(1)-�|����>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 0, View->m_MaxCCtl);
	} else if(m_Kind==11) {												// <��]������(1)-��]��>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 3, View->m_MaxCCtl);
	} else if(m_Kind==12) {												// <�Θg�z�u(2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 1.0, 360.0);
		DDX_Text(pDX, IDC_EDIT2, m_ParamI2);
		DDV_MinMaxInt(pDX, m_ParamI2, 2, 100);
	} else if(m_Kind==13) {												// <�������_��(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 2, View->m_MaxCCtl);
	} else if(m_Kind==14||m_Kind==15||m_Kind==16) {						// <��~����(2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 1.0, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, 1.0, 1000.0);
	} else if(m_Kind==17||m_Kind==19) {									// <�ݸ޻���[�ڍ�:mm](2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, -100.0, 100.0);
	} else if(m_Kind==18||m_Kind==20) {									// <�ݸ޻���[�ڍ�:�x](2)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDV_MinMaxDouble(pDX, m_Param2, -360.0, 360.0);
	} else if(m_Kind==21) {												// <�߰�Ĉʒu(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, -100.0, 100.0);
	} else if(m_Kind==22) {												// <��������(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 1, View->m_MaxCCtl);
	} else if(m_Kind==23) {												// <�}������_��(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 1, View->m_MaxCCtl-2);
	} else if(m_Kind==24) {												// <��]�Ώە�����(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 2, View->m_MaxCCtl);
	} else if(m_Kind==25) {												// <���Z�ʒu(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, -100.0, 100.0);
	} else if(m_Kind==26) {												// <�`��̈�(2)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 10, 2000);
		DDX_Text(pDX, IDC_EDIT2, m_ParamI2);
		DDV_MinMaxInt(pDX, m_ParamI2, 10, 2000);
	} else if(m_Kind==27) {												// <�����\���␳�{��(1)>
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 100.0);
	} else if(m_Kind==28) {												// <��~����_��(1)>
		DDX_Text(pDX, IDC_EDIT1, m_ParamI1);
		DDV_MinMaxInt(pDX, m_ParamI1, 3, View->m_MaxCCtl);
	} else {
		DDX_Text(pDX, IDC_EDIT1, m_Param1);
		DDX_Text(pDX, IDC_EDIT2, m_Param2);
		DDX_Text(pDX, IDC_EDIT3, m_Param3);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CInputDlg ���b�Z�[�W �n���h��

/**********************************/
BOOL CInputDlg::OnInitDialog(void)										//<<< WM_INITDIALOG ү���މ�������
/**********************************/									//    �޲�۸ނ̏�����
{
	CRect r;

	CDialog::OnInitDialog();
	GetWindowRect(&r);
	Width = r.Width();
	Height = r.Height();
	SetInputDlg();
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/*********************************/
void CInputDlg::SetInputDlg(void)										//<<< �޲�۸� ڲ��Đݒ�
/*********************************/
{
	CString title, str, show, unit;
	BOOL    flg=TRUE;
	int     i, sw;

	if(m_Kind==1) {														// <��د�ފԊu(1)>
		show = "110";													// �\�����۰ِݒ�
		unit = "100";
		title.LoadString(IDS_STR14);
		str.LoadString(IDS_STR23);										// ���ږ��擾(��د�ފԊu)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR78);										// ���ږ��擾(�ڐ��Ԋu)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==2) {												// <����(3)>
		show = "111";													// �\�����۰ِݒ�
		unit = "111";
		title.LoadString(IDS_STR12);
		SetWindowPos(NULL, 0, 0, Width, Height, SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==3) {												// <����(2)>
		show = "110";													// �\�����۰ِݒ�
		unit = "110";
		title.LoadString(IDS_STR12);
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==4) {												// <����(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "100";
		title.LoadString(IDS_STR12);
		str.LoadString(IDS_STR24);										// ���ږ��擾(���a)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==5) {												// <����(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "100";
		title.LoadString(IDS_STR15);
		str.LoadString(IDS_STR25);										// ���ږ��擾(��)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==6) {												// <�~�ʌ`��(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "100";
		title.LoadString(IDS_STR56);
		str.LoadString(IDS_STR57);										// ���ږ��擾(�����̈�)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR6);										// �P�ʎ擾(�x)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// �P�ʕ\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==7) {												// <����(2)+��>
		show = "111";													// �\�����۰ِݒ�
		unit = "111";
		title.LoadString(IDS_STR12);
		str.LoadString(IDS_STR37);										// ���ږ��擾(����)
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, Height, SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==8) {												// <����(1)+��>
		show = "110";													// �\�����۰ِݒ�
		unit = "110";
		title.LoadString(IDS_STR12);
		str.LoadString(IDS_STR24);										// ���ږ��擾(���a)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR37);										// ���ږ��擾(����)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==9) {												// <�Ȑ����Ұ�(2)>
		show = "110";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR8);
		str.LoadString(IDS_STR22);										// ���ږ��擾(�Ȑ�������)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR46);										// ���ږ��擾(�Ȑ��d�݌W��)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==10) {												// <�|��������(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR47);
		str.LoadString(IDS_STR48);										// ���ږ��擾(�|��������)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==11) {												// <��]������(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR49);
		str.LoadString(IDS_STR50);										// ���ږ��擾(��]������)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==12) {												// <�Θg�z�u(2)>
		show = "110";													// �\�����۰ِݒ�
		unit = "110";
		title.LoadString(IDS_STR53);
		str.LoadString(IDS_STR54);										// ���ږ��擾(�z�u�̈�)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR55);										// ���ږ��擾(���z�u��)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR6);										// �P�ʎ擾(�x)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// �P�ʕ\��
		str.LoadString(IDS_STR13);										// �P�ʎ擾(��)
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);					// �P�ʕ\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==13) {												// <�������_��(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR29);
		str.LoadString(IDS_STR76);										// ���ږ��擾(����_��)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==14||m_Kind==15||m_Kind==16) {						// <����(2)>
		show = "110";													// �\�����۰ِݒ�
		unit = "110";
		title.LoadString(IDS_STR77);
		if(m_Kind==14) {												// ���ږ��擾(X/Y/Z)
			str.LoadString(IDS_STR62);
		} else if(m_Kind==15) {
			str.LoadString(IDS_STR63);
		} else {
			str.LoadString(IDS_STR64);
		}
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		if(m_Kind==14) {												// ���ږ��擾(Y/Z/X)
			str.LoadString(IDS_STR63);
		} else if(m_Kind==15) {
			str.LoadString(IDS_STR64);
		} else {
			str.LoadString(IDS_STR62);
		}
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==17||m_Kind==18) {									// <�ݸ޻��ލ���(2)>
		show = "110";													// �\�����۰ِݒ�
		unit = "110";
		title.LoadString(IDS_STR16);
		str.LoadString(IDS_STR26);										// ���ږ��擾(�ݸލ���)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR69);										// ���ږ��擾(�ڍ��Ԋu)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR28);										// �P�ʎ擾(��)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// �P�ʕ\��
		if(m_Kind==18) {
			str.LoadString(IDS_STR6);									// �P�ʎ擾(�x)
			GetDlgItem(IDC_STATIC5)->SetWindowText(str);				// �P�ʕ\��
		}
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==19||m_Kind==20) {									// <�ݸ޻��ޒ��a(2)>
		show = "110";													// �\�����۰ِݒ�
		unit = "110";
		title.LoadString(IDS_STR16);
		str.LoadString(IDS_STR27);										// ���ږ��擾(�ݸލ���)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR69);										// ���ږ��擾(�ڍ��Ԋu)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		if(m_Kind==20) {
			str.LoadString(IDS_STR6);									// �P�ʎ擾(�x)
			GetDlgItem(IDC_STATIC5)->SetWindowText(str);				// �P�ʕ\��
		}
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==21) {												// <�߰�Ĉʒu(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "100";
		title.LoadString(IDS_STR67);
        str.LoadString(IDS_STR68);										// ���ږ��擾(�߰�Ĉʒu)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==22) {												// <��������(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR51);
		str.LoadString(IDS_STR52);										// ���ږ��擾(��������)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==23) {												// <�}������_��(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR83);
		str.LoadString(IDS_STR84);										// ���ږ��擾(�}������_��)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==24) {												// <��]�Ώە�����(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR90);
		str.LoadString(IDS_STR91);										// ���ږ��擾(�Ώە�����)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==25) {												// <���Z�ʒu(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "100";
		title.LoadString(IDS_STR73);
		str.LoadString(IDS_STR74);										// ���ږ��擾(���Z�ʒu)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==26) {												// <�`��̈�(2)>
		show = "110";													// �\�����۰ِݒ�
		unit = "110";
		title.LoadString(IDS_STR32);
		str.LoadString(IDS_STR33);										// ���ږ��擾(�`��̈敝)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR34);										// ���ږ��擾(�`��̈捂��)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR38);										// �P�ʎ擾(pixel)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// �P�ʕ\��
		str.LoadString(IDS_STR38);										// �P�ʎ擾(pixel)
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);					// �P�ʕ\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==27) {												// <�����\���␳�{��(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "100";
		title.LoadString(IDS_STR21);
        str.LoadString(IDS_STR30);										// ���ږ��擾(�␳�{��)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR7);										// �P�ʎ擾(�{)
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);					// �P�ʕ\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else if(m_Kind==28) {												// <��~����_��(1)>
		show = "100";													// �\�����۰ِݒ�
		unit = "000";
		title.LoadString(IDS_STR29);
		str.LoadString(IDS_STR76);										// ���ږ��擾(����_��)
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		SetWindowPos(NULL, 0, 0, Width, (int)(Height*0.83), SWP_NOMOVE|SWP_NOZORDER);
	} else {
		show = "111";													// �\�����۰ِݒ�
		unit = "111";
		SetWindowPos(NULL, 0, 0, Width, Height, SWP_NOMOVE|SWP_NOZORDER);
		flg = FALSE;
	}
	for(i=0; i<3; i++) {												// ���۰ٕ\��
		sw = show[i]=='1'?1:0;
		GetDlgItem(IDC_STATIC1+i)->ShowWindow(sw?SW_SHOW:SW_HIDE);
		GetDlgItem(IDC_EDIT1+i)->ShowWindow(sw?SW_SHOW:SW_HIDE);
		sw = unit[i]=='1'?1:0;
		GetDlgItem(IDC_STATIC4+i)->ShowWindow(sw?SW_SHOW:SW_HIDE);
	}
	if(flg) {
		SetWindowText(title);											// ���ٕ\��
	}
}

/***************************************************************/
void CInputDlg::SetValue(double prm1, double prm2, double prm3)			//<<< �������Ұ��ݒ�
/***************************************************************/
{
	m_Param1 = prm1;
	m_Param2 = prm2;
	m_Param3 = prm3;
}

/************************************************************/
void CInputDlg::SetValue(int prm1, double prm2, double prm3)			//<<< ������������Ұ��ݒ�
/************************************************************/
{
	m_ParamI1 = prm1;
	m_Param2  = prm2;
	m_Param3  = prm3;
}

/***********************************************/
void CInputDlg::SetValue(double prm1, int prm2)							//<<< ������������Ұ��ݒ�
/***********************************************/
{
	m_Param1  = prm1;
	m_ParamI2 = prm2;
}

/********************************************/
void CInputDlg::SetValue(int prm1, int prm2)							//<<< �������Ұ��ݒ�
/********************************************/
{
	m_ParamI1 = prm1;
	m_ParamI2 = prm2;
}

/******************************************************************/
void CInputDlg::GetValue(double* prm1, double* prm2, double* prm3)		//<<< �������Ұ�(3)�擾
/******************************************************************/
{
	*prm1 = m_Param1;
	*prm2 = m_Param2;
	*prm3 = m_Param3;
}

/****************************************************/
void CInputDlg::GetValue(double* prm1, double* prm2)					//<<< �������Ұ�(2)�擾
/****************************************************/
{
	*prm1 = m_Param1;
	*prm2 = m_Param2;
}

/*************************************************/
void CInputDlg::GetValue(int* prm1, double* prm2)						//<<< ������������Ұ�(2)�擾
/*************************************************/
{
	*prm1 = m_ParamI1;
	*prm2 = m_Param2;
}

/*************************************************/
void CInputDlg::GetValue(double* prm1, int* prm2)						//<<< ������������Ұ�(2)�擾
/*************************************************/
{
	*prm1 = m_Param1;
	*prm2 = m_ParamI2;
}

/**********************************************/
void CInputDlg::GetValue(int* prm1, int* prm2)							//<<< �������Ұ�(2)�擾
/**********************************************/
{
	*prm1 = m_ParamI1;
	*prm2 = m_ParamI2;
}

/**************************************/
void CInputDlg::GetValue(double* prm1)									//<<< �������Ұ�(1)�擾
/**************************************/
{
	*prm1 = m_Param1;
}

/***********************************/
void CInputDlg::GetValue(int* prm1)										//<<< �������Ұ�(1)�擾
/***********************************/
{
	*prm1 = m_ParamI1;
}
