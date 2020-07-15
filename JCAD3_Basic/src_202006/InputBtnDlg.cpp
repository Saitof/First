/////////////////////////////////////////////////////////////////////////////
// ���l���̓_�C�A���O�N���X(�t�Ή�)��`
// InputBtnDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/22 by M.Fukushima
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
#include "InputBtnDlg.h"
#include "CCommand.h"
#include "ECommand.h"
#include "ECmdUnit.h"
#include "EditCtrlPoint1.h"
#define   TIMERID  1

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg1 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg1, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/********************************************************************************************/
CInputBtnDlg1::CInputBtnDlg1(int kind, CWnd* pParent) : CDialog(CInputBtnDlg1::IDD, pParent)
/********************************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_Kind   = kind;
	m_nID    = 0;
	m_Init   = 0;
}

/******************************************************/
void CInputBtnDlg1::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	if(m_Kind==1) {														// <�ړ�>
		DDV_MinMaxDouble(pDX, m_Param1, -1000.0, 1000.0);
		DDV_MinMaxDouble(pDX, m_Param2, -1000.0, 1000.0);
		DDV_MinMaxDouble(pDX, m_Param3, -1000.0, 1000.0);
		DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
	} else if(m_Kind==2) {												// <���ޕύX1>
		DDV_MinMaxDouble(pDX, m_Param1, 0.0, 1500.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.0, 1500.0);
		DDV_MinMaxDouble(pDX, m_Param3, 0.0, 1500.0);
		DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
	} else {															// <���ޕύX2>
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param3, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param4, 0.001, 2.0);
	}
}

/**************************************/
BOOL CInputBtnDlg1::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn1Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn1Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// �t���ޯ�ϯ�߂����ѕt����
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	VERIFY(Plus2.AutoLoad(IDC_PLUS2, this));
	VERIFY(Minus2.AutoLoad(IDC_MINUS2, this));
	VERIFY(Plus3.AutoLoad(IDC_PLUS3, this));
	VERIFY(Minus3.AutoLoad(IDC_MINUS3, this));
	if(m_Kind==2) {														// <���ޕύX1>
		str.LoadString(IDS_STR9);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
		str.LoadString(IDS_STR39);										// ���ږ��擾
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR40);
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);
	} else if(m_Kind==3) {												// <���ޕύX2>
		str.LoadString(IDS_STR9);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
		str.LoadString(IDS_STR43);										// ���ږ��擾
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR40);
		GetDlgItem(IDC_STATIC5)->SetWindowText(str);
		str.LoadString(IDS_STR7);										// �P�ʖ��擾
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// �P�ʖ��\��
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
		GetDlgItem(IDC_STATIC6)->SetWindowText(str);
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg1::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	double dx, dy, dz;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(m_Kind==1) {													// <�ړ�>
			if(fabs(m_Param1)>EPSILON||
               fabs(m_Param2)>EPSILON||
               fabs(m_Param3)>EPSILON||m_Init!=0) {
				ObjectMove(m_Init, m_Copy, m_Scale, m_Base, m_Param1,
                           m_Param2, m_Param3);							// ���̕��s�ړ�
				EndMove(m_Copy, m_Scale, m_SChk, m_Base, m_Param1,
                        m_Param2, m_Param3);							// �I������
			}
		} else if(m_Kind==2||m_Kind==3) {								// <���ޕύX>
			if(m_Kind==2) {												// ����(mm)�w��̏ꍇ
				dx = (m_Xsize==0||m_Param1==0) ? 1.0 : m_Param1/m_Xsize;// X�{��
				dy = (m_Ysize==0||m_Param2==0) ? 1.0 : m_Param2/m_Ysize;// Y�{��
				dz = (m_Zsize==0||m_Param3==0) ? 1.0 : m_Param3/m_Zsize;// Z�{��
			} else {													// �{���w��̏ꍇ
				dx = m_Param1;											// X�{��
				dy = m_Param2;											// Y�{��
				dz = m_Param3;											// Z�{��
			}
			if(fabs(dx-1.0)>EPSILON||
               fabs(dy-1.0)>EPSILON||
               fabs(dz-1.0)>EPSILON||m_Init!=0) {
				ObjectScale(m_Init, m_Cent, dx, dy, dz);				// ���̊g��/�k��
			}
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg1::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn1Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn1Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg1::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/******************************************/
void CInputBtnDlg1::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
/******************************************/
{
	KillTimer(TIMERID);													// ��ϰ����
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// �����݉������Ȃ��
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ϳ����ޯ�ϯ�ߖt��ɂ��鎞
				ObjectEdit();											// ���̕ҏW����
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg1::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖt��ID�擾
			if(id>=IDC_PLUS1&&id<=IDC_MINUS3) {							// ϳ����ޯ�ϯ�ߖt��
				m_nID = id;
				UpdateData(TRUE);										// �޲�۸ޏ���ް��擾
				ObjectEdit();											// ���̕ҏW����
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg1::ObjectEdit(void)									//<<< ���̕ҏW����
/************************************/
{
	double sv, dx, dy, dz;

	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z�O�ۑ�
			sv = m_Param1;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			sv = m_Param2;
		} else {
			sv = m_Param3;
		}
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z/���Z
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param4;
			} else {
				m_Param1 -= m_Param4;
			}
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			if(m_nID==IDC_PLUS2) {
				m_Param2 += m_Param4;
			} else {
				m_Param2 -= m_Param4;
			}
		} else {
			if(m_nID==IDC_PLUS3) {
				m_Param3 += m_Param4;
			} else {
				m_Param3 -= m_Param4;
			}
		}
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
		if(!DataCheck()) {												// <<�����ް��m�FOK>>
			if(m_Kind==1) {												// <�ړ�>
				if(fabs(m_Param1)>EPSILON||
                   fabs(m_Param2)>EPSILON||
                   fabs(m_Param3)>EPSILON||m_Init!=0) {
					ObjectMove(m_Init, m_Copy, m_Scale, m_Base,
                               m_Param1, m_Param2, m_Param3);			// ���̕��s�ړ�
					m_Init = 1;											// �����I��
				}
			} else if(m_Kind==2||m_Kind==3) {							// <���ޕύX>
				if(m_Kind==2) {											// ����(mm)�w��̏ꍇ
					dx = (m_Xsize==0||m_Param1==0) ? 1.0 : m_Param1/m_Xsize;
					dy = (m_Ysize==0||m_Param2==0) ? 1.0 : m_Param2/m_Ysize;
					dz = (m_Zsize==0||m_Param3==0) ? 1.0 : m_Param3/m_Zsize;
				} else {												// �{���w��̏ꍇ
					dx = m_Param1;										// X�{��
					dy = m_Param2;										// Y�{��
					dz = m_Param3;										// Z�{��
				}
				if(fabs(dx-1.0)>EPSILON||
                   fabs(dy-1.0)>EPSILON||
                   fabs(dz-1.0)>EPSILON||m_Init!=0) {
					ObjectScale(m_Init, m_Cent, dx, dy, dz);			// ���̊g��/�k��
					m_Init = 1;											// �����I��
				}
			}
			View->Display(0);											// �ĕ`��
		} else {														// <<�����ް��m�FNG>>
			UpdateData(TRUE);											// �װү���ޗp
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1)	{
				m_Param1 = sv;
			} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
				m_Param2 = sv;
			} else {
				m_Param3 = sv;
			}
			UpdateData(FALSE);											// �޲�۸ޏ���ް�����
		}
	}
}

/***********************************/
BOOL CInputBtnDlg1::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Kind==1) {
		if(m_Param1<-1000.0||m_Param1>1000.0) {
			ret = TRUE;
		}
		if(m_Param2<-1000.0||m_Param2>1000.0) {
			ret = TRUE;
		}
		if(m_Param3<-1000.0||m_Param3>1000.0) {
			ret = TRUE;
		}
		if(m_Param4<0.001||m_Param4>100.0) {
			ret = TRUE;
		}
	} else if(m_Kind==2) {
		if(m_Param1<0.0||m_Param1>1500.0) {
			ret = TRUE;
		}
		if(m_Param2<0.0||m_Param2>1500.0) {
			ret = TRUE;
		}
		if(m_Param3<0.0||m_Param3>1500.0) {
			ret = TRUE;
		}
		if(m_Param4<0.001||m_Param4>100.0) {
			ret = TRUE;
		}
	} else if(m_Kind==3) {
		if(m_Param1<0.001||m_Param1>10.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>10.0) {
			ret = TRUE;
		}
		if(m_Param3<0.001||m_Param3>10.0) {
			ret = TRUE;
		}
		if(m_Param4<0.001||m_Param4>2.0) {
			ret = TRUE;
		}
	}
	return ret;
}

/*********************************************************/
void CInputBtnDlg1::SetParam1(int cn, PNTTYPE sc, int ch,
                              SCRTYPE bp, double ad)                    //<<< ���Ұ��ݒ�1
/*********************************************************/
{
	m_Copy = cn, m_Scale  = sc, m_SChk = ch;
	m_Base = bp, m_Param4 = ad;
}

/**************************************************************/
void CInputBtnDlg1::SetParam2(PNTTYPE cp, double dx,
							  double dy, double dz, double ad)			//<<< ���Ұ��ݒ�2
/**************************************************************/
{
	m_Cent  = cp, m_Xsize  = dx, m_Ysize = dy;
	m_Zsize = dz, m_Param4 = ad;
	if(m_Kind==2) {
		m_Param1 = dx,  m_Param2 = dy,  m_Param3 = dz;
	} else {
		m_Param1 = 1.0, m_Param2 = 1.0, m_Param3 = 1.0;
	}
}

/****************************************/
void CInputBtnDlg1::GetParam(double* ad)                                //<<< ���Ұ��擾
/****************************************/
{
	*ad = m_Param4;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg2 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg2, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************/
CInputBtnDlg2::CInputBtnDlg2(CWnd* pParent) : CDialog(CInputBtnDlg2::IDD, pParent)
/**********************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_nID    = 0;
	m_Init   = 0;
}

/******************************************************/
void CInputBtnDlg2::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDV_MinMaxDouble(pDX, m_Param1, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDV_MinMaxDouble(pDX, m_Param2, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDV_MinMaxDouble(pDX, m_Param3, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
}

/**************************************/
BOOL CInputBtnDlg2::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	int left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn2Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn2Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// �t���ޯ�ϯ�߂����ѕt����
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	VERIFY(Plus2.AutoLoad(IDC_PLUS2, this));
	VERIFY(Minus2.AutoLoad(IDC_MINUS2, this));
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg2::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(fabs(m_Param1)>EPSILON||m_Init!=0) {
			ObjectRotate(m_Init, m_Copy, m_Scale, m_Base, m_Cent,
                         m_Plane, m_Param1, m_Param2);					// ���̉�]�ړ�
			EndRotate(m_Copy, m_Scale, m_SChk, m_Base, m_Cent,
                      m_Plane, m_Param1, m_Param2);						// �I������
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg2::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn2Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn2Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg2::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/******************************************/
void CInputBtnDlg2::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
/******************************************/
{
	KillTimer(TIMERID);													// ��ϰ����
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// �����݉������Ȃ��
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ϳ����ޯ�ϯ�ߖt��ɂ��鎞
				ObjectEdit();											// ���̕ҏW����
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg2::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖt��ID�擾
			if(id==IDC_PLUS1||id==IDC_MINUS1||
               id==IDC_PLUS2||id==IDC_MINUS2) {							// ϳ����ޯ�ϯ�ߖt��
				m_nID = id;
				UpdateData(TRUE);										// �޲�۸ޏ���ް��擾
				ObjectEdit();											// ���̕ҏW����
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg2::ObjectEdit(void)									//<<< ���̕ҏW����
/************************************/
{
	double sv;

	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		sv = (m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) ? m_Param1:m_Param2;	// ���Z�O�ۑ�
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z/���Z
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param3;
			} else {
				m_Param1 -= m_Param3;
			}
		} else {
			if(m_nID==IDC_PLUS2) {
				m_Param2 += m_Param4;
			} else {
				m_Param2 -= m_Param4;
			}
		}
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
		if(!DataCheck()) {												// <<�����ް��m�FOK>>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				ObjectRotate(m_Init, m_Copy, m_Scale, m_Base, m_Cent,
                             m_Plane, m_Param1, m_Param2);				// ���̉�]�ړ�
				m_Init = 1;												// �����I��
			}
			View->Display(0);											// �ĕ`��
		} else {														// <<�����ް��m�FNG>>
			UpdateData(TRUE);											// �װү���ޗp
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {
				m_Param1 = sv;
			} else {
				m_Param2 = sv;
			}
			UpdateData(FALSE);											// �޲�۸ޏ���ް�����
		}
	}
}

/***********************************/
BOOL CInputBtnDlg2::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Param1<-360.0||m_Param1>360.0)	{
		ret = TRUE;
	}
	if(m_Param2<-1000.0||m_Param2>1000.0) {
		ret = TRUE;
	}
	if(m_Param3<0.001||m_Param3>100.0) {
		ret = TRUE;
	}
	if(m_Param4<0.001||m_Param4>100.0) {
		ret = TRUE;
	}
	return ret;
}

/************************************************************/
void CInputBtnDlg2::SetParam(int cn, PNTTYPE sc, int ch,
                             SCRTYPE bp, PNTTYPE cp, int pl,
                             double ad1, double ad2)					//<<< ���Ұ��ݒ�
/************************************************************/
{
	m_Copy = cn, m_Scale = sc, m_SChk   = ch,  m_Base   = bp;
	m_Cent = cp, m_Plane = pl, m_Param3 = ad1, m_Param4 = ad2;
}

/******************************************************/
void CInputBtnDlg2::GetParam(double* ad1, double* ad2)                  //<<< ���Ұ��擾
/******************************************************/
{
	*ad1 = m_Param3, *ad2 = m_Param4;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg3 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg3, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/********************************************************************************************/
CInputBtnDlg3::CInputBtnDlg3(int kind, CWnd* pParent) : CDialog(CInputBtnDlg3::IDD, pParent)
/********************************************************************************************/
{
	if(kind==3) {
		m_Param1 = 1.0;
	} else if(kind==6) {
		m_Param1 = 0.0;
	} else {
		m_Param1 = 0.0;
	}
	m_Param2 = 0.0;
	m_Kind   = kind;
	m_nID    = 0;
	m_Init   = 0;
}

/******************************************************/
void CInputBtnDlg3::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	if(m_Kind==3) {														// <�g��/�k��>
		DDV_MinMaxDouble(pDX, m_Param1, 0.001, 10.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 2.0);
	} else if(m_Kind==6) {												// <����f>
		DDV_MinMaxDouble(pDX, m_Param1, -100.0, 100.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 10.0);
	} else {															// <�~���Ȃ���Ђ˂襉�]>
		DDV_MinMaxDouble(pDX, m_Param1, -1080.0, 1080.0);
		DDV_MinMaxDouble(pDX, m_Param2, 0.001, 100.0);
	}
}

/**************************************/
BOOL CInputBtnDlg3::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn3Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn3Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// �t���ޯ�ϯ�߂����ѕt����
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	if(m_Kind==2||m_Kind==4) {											// <�Ђ˂�(����_��/����)>
		str.LoadString(IDS_STR10);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
		str.LoadString(IDS_STR41);										// ���ږ��擾
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR42);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
	} else if(m_Kind==3) {												// <�g��/�k��(����_��)>
		str.LoadString(IDS_STR11);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
		str.LoadString(IDS_STR43);										// ���ږ��擾
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR44);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.LoadString(IDS_STR7);										// �P�ʖ��擾(�{)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// �P�ʖ��\��
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	} else if(m_Kind==5) {												// <��](����_��)>
		str.LoadString(IDS_STR80);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
		str.LoadString(IDS_STR81);										// ���ږ��擾
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR82);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
	} else if(m_Kind==6) {												// <����f(����)>
		str.LoadString(IDS_STR92);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
		str.LoadString(IDS_STR93);										// ���ږ��擾
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ���ږ��\��
		str.LoadString(IDS_STR75);
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.LoadString(IDS_STR5);										// �P�ʖ��擾(mm)
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);					// �P�ʖ��\��
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg3::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	double dd;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(m_Kind==1) {													// <�~���Ȃ�(����)>
			dd = m_Param1;												// �Ȃ��p�x
			if(fabs(dd)>EPSILON||m_Init!=0) {
				if(!m_Fg) {												// ���l�ϊ�
					dd = -dd;
				}
				ObjectBend(m_Init, m_Fc, m_Vc, dd, m_Spnt, m_Epnt);		// ���̉~���Ȃ�
			}
		} else if(m_Kind==2||m_Kind==5) {								// <�Ђ˂�/��](����_��)>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				TwistCLine(m_Cp, m_Vp, m_Param1, m_Bs, m_Sa, m_Sn,
                           m_Op, m_Un, m_Vn, m_Uf, m_Sf, m_Pa,
                           m_Pb, m_Pc, m_Ta);							// ����_�Ђ˂�
				RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc, m_Ia);		// �Ȗʐ���
				RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);			// �Ȗʐ���_����
				View->ObjectShading(m_Op);								// ����ިݸ�ؽēo�^
			}
		} else if(m_Kind==3) {											// <�g��/�k��(����_��)>
			if(fabs(m_Param1-1.0)>EPSILON||m_Init!=0) {
				ScaleCLine(m_Cp, m_Vp, m_Bn, m_Dn, m_Param1, m_Bs,
                           m_Sa, m_Sn, m_Op, m_Un, m_Vn,
                           m_Uf, m_Sf, m_Pa, m_Pb, m_Pc, m_Ta);			// ����_�g��/�k��
				RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc, m_Ia);		// �Ȗʐ���
				RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);			// �Ȗʐ���_����
				View->ObjectShading(m_Op);								// ����ިݸ�ؽēo�^
			}
		} else if(m_Kind==4) {											// <�Ђ˂�(����)>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				ObjectTwist(m_Init, m_Dr, m_Param1, m_Ln, m_Cpnt);		// ���̂Ђ˂�
			}
		} else if(m_Kind==6) {											// <����f(����)>
			if(fabs(m_Param1)>EPSILON||m_Init!=0) {
				dd = m_Param1/m_Len;									// ����f�X��
				ObjectShear(m_Init, m_Dr1, m_Dr2, dd, m_Spnt);			// ����f
			}
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg3::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn3Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn3Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg3::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/******************************************/
void CInputBtnDlg3::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
/******************************************/
{
	KillTimer(TIMERID);													// ��ϰ����
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// �����݉������Ȃ��
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ϳ����ޯ�ϯ�ߖt��ɂ��鎞
				ObjectEdit();											// ���̕ҏW����
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg3::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖt��ID�擾
			if(id==IDC_PLUS1||id==IDC_MINUS1) {							// ϳ����ޯ�ϯ�ߖt��
				m_nID = id;
				UpdateData(TRUE);										// �޲�۸ޏ���ް��擾
				ObjectEdit();											// ���̕ҏW����
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg3::ObjectEdit(void)									//<<< ���̕ҏW����
/************************************/
{
	PNTTYPE p1[2], p2[2];
	int     i1[2];
	double  sv, dd;

	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		sv = m_Param1;													// ���Z�O�ۑ�
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z/���Z
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param2;
			} else {
				m_Param1 -= m_Param2;
			}
		}
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
		if(!DataCheck()) {												// <<�����ް��m�FOK>>
			if(m_Kind==1) {												// <�~���Ȃ�(����)>
				dd = m_Param1;											// �Ȃ��p�x
				if(fabs(dd)>EPSILON||m_Init!=0) {
					if(!m_Fg) {											// ���l�ϊ�
						dd = -dd;
					}
					ObjectBend(m_Init, m_Fc, m_Vc, dd, m_Spnt, m_Epnt);	// ���̉~���Ȃ�
					m_Init = 1;											// �����I��
				}
			} else if(m_Kind==2||m_Kind==5) {							// <�Ђ˂�/��](����_��)>
				if(fabs(m_Param1)>EPSILON||m_Init!=0) {
					TwistCLine(m_Cp, m_Vp, m_Param1, m_Bs, m_Sa,
                               m_Sn, m_Op, m_Un, m_Vn, m_Uf,
                               m_Sf, m_Pa, m_Pb, m_Pc, m_Ta);			// ����_�Ђ˂�
					RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf,m_Pc,m_Ia);	// �Ȗʐ���
					RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);		// �Ȗʐ���_����
					View->ObjectShading(m_Op);							// ����ިݸ�ؽēo�^
					m_Init = 1;											// �����I��
				}
			} else if(m_Kind==3) {										// <�g��/�k��(����_��)>
				if(fabs(m_Param1-1.0)>EPSILON||m_Init!=0) {
					ScaleCLine(m_Cp, m_Vp, m_Bn, m_Dn, m_Param1,
                               m_Bs, m_Sa, m_Sn, m_Op, m_Un, m_Vn,
                               m_Uf, m_Sf, m_Pa, m_Pb, m_Pc, m_Ta);		// ����_�g��/�k��
					RedoChgSurface(m_Un, m_Vn, m_Uf, m_Vf,m_Pc,m_Ia);	// �Ȗʐ���
					RedoSetSurface(m_Un, m_Vn, m_Uf, m_Vf, m_Pc);		// �Ȗʐ���_����
					View->ObjectShading(m_Op);							// ����ިݸ�ؽēo�^
					m_Init = 1;											// �����I��
				}
			} else if(m_Kind==4) {										// <�Ђ˂�(����)>
				if(fabs(m_Param1)>EPSILON||m_Init!=0) {
					ObjectTwist(m_Init, m_Dr, m_Param1, m_Ln, m_Cpnt);	// ���̂Ђ˂�
					m_Init = 1;											// �����I��
				}
			} else if(m_Kind==6) {										// <����f(����)>
				if(fabs(m_Param1)>EPSILON||m_Init!=0) {
					dd = m_Param1/m_Len;								// ����f�X��
					ObjectShear(m_Init, m_Dr1, m_Dr2, dd, m_Spnt);		// ����f
					m_Init = 1;											// �����I��
					p1[0] = m_Spnt, p2[0] = m_Epnt;						// ����f��(ײ�)�����ݒ�
					i1[0] = 0;
					p2[0].p[m_Dr2] += m_Param1;							// ����f�������ʒu�ύX
					View->Line.Setup(p1, p2, i1, 1, m_Pl);				// ����f��(ײ�)�ݒ�
				}
			}
			View->Display(0);											// �ĕ`��
		} else {														// <<�����ް��m�FNG>>
			UpdateData(TRUE);											// �װү���ޗp
			m_Param1 = sv;
			UpdateData(FALSE);											// �޲�۸ޏ���ް�����
		}
	}
}

/***********************************/
BOOL CInputBtnDlg3::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Kind==3) {														// <�g��/�k��>
		if(m_Param1<0.001||m_Param1>10.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>2.0) {
			ret = TRUE;
		}
	} else if(m_Kind==6) {												// <����f>
		if(m_Param1<-100.0||m_Param1>100.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>10.0) {
			ret = TRUE;
		}
	} else {															// <�~���Ȃ���Ђ˂襉�]>
		if(m_Param1<-1080.0||m_Param1>1080.0) {
			ret = TRUE;
		}
		if(m_Param2<0.001||m_Param2>100.0) {
			ret = TRUE;
		}
	}
	return ret;
}

/****************************************************************/
void CInputBtnDlg3::SetParam1(int fg, int fc, int vc,
                              PNTTYPE sp, PNTTYPE ep, double ad)		//<<< ���Ұ��ݒ�1
/****************************************************************/
{
	m_Fg = fg, m_Fc = fc, m_Vc = vc;
	m_Spnt = sp, m_Epnt = ep, m_Param2 = ad;
}

/********************************************************************/
void CInputBtnDlg3::SetParam2(int un, int vn, int uf, int vf,
                              int sf, int bs, int* sa, int sn,
                              OBJTYPE* op, PNTTYPE* cp, VECTYPE* vp,
                              PNTTYPE* pa, int* ia, PNTTYPE* pb,
                              PNTTYPE* pc, int* ta, double ad)			//<<< ���Ұ��ݒ�2
/********************************************************************/
{
	m_Un = un, m_Vn = vn, m_Uf = uf, m_Vf = vf, m_Sf = sf, m_Bs = bs;
	m_Sa = sa, m_Sn = sn, m_Op = op, m_Cp = cp, m_Vp = vp, m_Pa = pa;
	m_Ia = ia, m_Pb = pb, m_Pc = pc, m_Ta = ta, m_Param2 = ad;
}

/**********************************************************************/
void CInputBtnDlg3::SetParam3(int un, int vn, int uf, int vf, int sf,
                              int bn, int dn, int bs, int* sa, int sn,
                              OBJTYPE* op, PNTTYPE* cp, VECTYPE* vp,
                              PNTTYPE* pa, int* ia, PNTTYPE* pb,
                              PNTTYPE* pc, int* ta, double ad)			//<<< ���Ұ��ݒ�3
/**********************************************************************/
{
	m_Un = un, m_Vn = vn, m_Uf = uf, m_Vf = vf, m_Sf = sf, m_Bn = bn;
	m_Dn = dn, m_Bs = bs, m_Sa = sa, m_Sn = sn, m_Op = op, m_Cp = cp;
	m_Vp = vp, m_Pa = pa, m_Ia = ia, m_Pb = pb, m_Pc = pc, m_Ta = ta;
	m_Param2 = ad;
}

/***********************************************************************/
void CInputBtnDlg3::SetParam4(int dr, double ln, PNTTYPE cp, double ad)	//<<< ���Ұ��ݒ�4
/***********************************************************************/
{
	m_Dr = dr, m_Ln = ln, m_Cpnt = cp, m_Param2 = ad;
}

/****************************************************************/
void CInputBtnDlg3::SetParam6(int d1, int d2, int pl,
                              PNTTYPE sp, PNTTYPE ep, double ad)		//<<< ���Ұ��ݒ�6
/****************************************************************/
{
	m_Dr1 = d1, m_Dr2 = d2, m_Pl = pl, m_Spnt = sp, m_Epnt = ep;
	m_Len = ep.p[d1]-sp.p[d1], m_Param2 = ad;
}

/****************************************/
void CInputBtnDlg3::GetParam(double* ad)                                //<<< ���Ұ��擾
/****************************************/
{
	*ad = m_Param2;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg4 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg4, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************/
CInputBtnDlg4::CInputBtnDlg4(CWnd* pParent) : CDialog(CInputBtnDlg4::IDD, pParent)
/**********************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_Param5 = 0.0;
	m_Param6 = 0.0;
	m_nID    = 0;
}

/******************************************************/
void CInputBtnDlg4::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDV_MinMaxDouble(pDX, m_Param1, 0.001, 1000.0);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDV_MinMaxDouble(pDX, m_Param2, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDV_MinMaxDouble(pDX, m_Param3, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT5, m_Param5);
	DDV_MinMaxDouble(pDX, m_Param5, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT6, m_Param6);
	DDV_MinMaxDouble(pDX, m_Param6, 0.001, 100.0);
}

/**************************************/
BOOL CInputBtnDlg4::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn4Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn4Top", -1);
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
	VERIFY(Plus1.AutoLoad(IDC_PLUS1, this));							// �t���ޯ�ϯ�߂����ѕt����
	VERIFY(Minus1.AutoLoad(IDC_MINUS1, this));
	VERIFY(Plus2.AutoLoad(IDC_PLUS2, this));
	VERIFY(Minus2.AutoLoad(IDC_MINUS2, this));
	VERIFY(Plus3.AutoLoad(IDC_PLUS3, this));
	VERIFY(Minus3.AutoLoad(IDC_MINUS3, this));
	if(m_Plane==0) {
		str.Format(IDS_STR65, "XY");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
		str.Format(IDS_STR65, "YZ");
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);
		str.Format(IDS_STR66, "XY");
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.Format(IDS_STR66, "YZ");
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	} else if(m_Plane==1) {
		str.Format(IDS_STR65, "YZ");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
		str.Format(IDS_STR65, "ZX");
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);
		str.Format(IDS_STR66, "YZ");
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.Format(IDS_STR66, "ZX");
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	} else {
		str.Format(IDS_STR65, "ZX");
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);
		str.Format(IDS_STR65, "XY");
		GetDlgItem(IDC_STATIC2)->SetWindowText(str);
		str.Format(IDS_STR66, "ZX");
		GetDlgItem(IDC_STATIC3)->SetWindowText(str);
		str.Format(IDS_STR66, "XY");
		GetDlgItem(IDC_STATIC4)->SetWindowText(str);
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg4::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	double mv;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(fabs(m_Param1-m_Param1Sv)>EPSILON) {							// <���a�����L>
			mv = m_Param1-m_Len;										// �ړ���
			ArrangeMove(mv, m_Lno, m_Cno, m_Min, m_Max, m_Ocp,
                        m_Mcp, m_Cvc);									// �ړ�����
		}
		if(fabs(m_Param3-m_Param3Sv)>EPSILON) {							// <������]�L>
			ArrangeRotate2(m_Param3, m_Lno, m_Cno, m_Min, m_Max,
                           m_Mcp, m_Rvc);								// ������]����
		}
		if(fabs(m_Param2-m_Param2Sv)>EPSILON) {							// <������]�L>
			ArrangeRotate1(m_Param2, m_Lno, m_Cno, m_Min, m_Max, m_Mcp);// ������]����
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg4::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn4Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn4Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg4::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/******************************************/
void CInputBtnDlg4::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
/******************************************/
{
	KillTimer(TIMERID);													// ��ϰ����
	CDialog::OnTimer(nIDTimer);
	if(GetKeyState(VK_LBUTTON)<0) {										// �����݉������Ȃ��
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			if(m_nID==pWnd->GetDlgCtrlID()) {							// ϳ����ޯ�ϯ�ߖt��ɂ��鎞
				ObjectEdit();											// ���̕ҏW����
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/********************************************************************/
BOOL CInputBtnDlg4::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖt��ID�擾
			if(id>=IDC_PLUS1&&id<=IDC_MINUS3) {							// ϳ����ޯ�ϯ�ߖt��
				m_nID = id;
				UpdateData(TRUE);										// �޲�۸ޏ���ް��擾
				ObjectEdit();											// ���̕ҏW����
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/************************************/
void CInputBtnDlg4::ObjectEdit(void)									//<<< ���̕ҏW����
/************************************/
{
	double sv, mv;

	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z�O�ۑ�
			sv = m_Param1;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			sv = m_Param2;
		} else {
			sv = m_Param3;
		}
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z/���Z
			if(m_nID==IDC_PLUS1) {
				m_Param1 += m_Param4;
			} else {
				m_Param1 -= m_Param4;
			}
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			if(m_nID==IDC_PLUS2) {
				m_Param2 += m_Param5;
			} else {
				m_Param2 -= m_Param5;
			}
		} else {
			if(m_nID==IDC_PLUS3) {
				m_Param3 += m_Param6;
			} else {
				m_Param3 -= m_Param6;
			}
		}
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
		if(!DataCheck()) {												// <<�����ް��m�FOK>>
			if(fabs(m_Param1-m_Param1Sv)>EPSILON) {						// <���a�����L>
				mv = m_Param1-m_Len;									// �ړ���
				ArrangeMove(mv, m_Lno, m_Cno, m_Min, m_Max,
                            m_Ocp, m_Mcp, m_Cvc);						// XY�ړ�����
				m_Param1Sv = m_Param1;
			}
			if(fabs(m_Param3-m_Param3Sv)>EPSILON) {						// <������]�L>
				ArrangeRotate2(m_Param3, m_Lno, m_Cno, m_Min,
                               m_Max, m_Mcp, m_Rvc);					// ������]����
				m_Param3Sv = m_Param3;
			}
			if(fabs(m_Param2-m_Param2Sv)>EPSILON) {						// <������]�L>
				ArrangeRotate1(m_Param2, m_Lno, m_Cno, m_Min,
                               m_Max, m_Mcp);							// ������]����
				m_Param2Sv = m_Param2;
			}
			View->Display(0);											// �ĕ`��
		} else {														// <<�����ް��m�FNG>>
			UpdateData(TRUE);											// �װү���ޗp
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {
				m_Param1 = sv;
			} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
				m_Param2 = sv;
			} else {
				m_Param3 = sv;
			}
			UpdateData(FALSE);											// �޲�۸ޏ���ް�����
		}
	}
}

/***********************************/
BOOL CInputBtnDlg4::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

	if(m_Param1<0.001||m_Param1>1000.0) {
		ret = TRUE;
	}
	if(m_Param2<-360.0||m_Param2>360.0) {
		ret = TRUE;
	}
	if(m_Param3<-360.0||m_Param3>360.0) {
		ret = TRUE;
	}
	if(m_Param4<0.001||m_Param4>100.0) {
		ret = TRUE;
	}
	if(m_Param5<0.001||m_Param5>100.0) {
		ret = TRUE;
	}
	if(m_Param6<0.001||m_Param6>100.0) {
		ret = TRUE;
	}
	return ret;
}

/**********************************************************************/
void CInputBtnDlg4::SetParam(int pl, int cn, int lno, double len,
                             int* mna, int* mxa, PNTTYPE* ocp,
                             PNTTYPE* mcp, VECTYPE* cvc, VECTYPE* rvc,
                             double ad1, double ad2, double ad3)		//<<< ���Ұ��ݒ�
/**********************************************************************/
{
	m_Plane = pl, m_Cno = cn,  m_Lno = lno, m_Len = len, m_Min = mna;
	m_Max = mxa,  m_Ocp = ocp, m_Mcp = mcp, m_Cvc = cvc, m_Rvc = rvc;
	m_Param4 = ad1, m_Param5 = ad2, m_Param6 = ad3, m_Param1 = m_Len;
	m_Param1Sv = m_Param1, m_Param2Sv = m_Param2, m_Param3Sv = m_Param3;
}

/*******************************************************************/
void CInputBtnDlg4::GetParam(double* ad1, double* ad2, double* ad3)     //<<< ���Ұ��擾
/*******************************************************************/
{
	*ad1 = m_Param4, *ad2 = m_Param5, *ad3 = m_Param6;
}
