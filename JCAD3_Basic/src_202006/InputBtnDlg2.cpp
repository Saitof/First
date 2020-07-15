/////////////////////////////////////////////////////////////////////////////
// ���l���̓_�C�A���O�N���X(�t�Ή�)��`
// InputBtnDlg2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
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
#include "DataAccess2.h"
#include "InputBtnDlg2.h"
#include "CCommand.h"
#include "ECommand.h"
#include "CreatCtrlPoint1.h"
#include "CreatCurvePoint.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlPoint2.h"
#include "EditCtrlTracker.h"
#include "Symmetry.h"
#include "ConvSCurve.h"
#define   TIMERID  1

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg5 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg5, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnAddCtrl1)
	ON_BN_CLICKED(IDC_BUTTON2, OnAddCtrl2)
	ON_BN_CLICKED(IDC_BUTTON3, OnCancelCtrl)
	ON_BN_CLICKED(IDC_BUTTON4, OnRotate2D)
END_MESSAGE_MAP()

/********************************************************************************************/
CInputBtnDlg5::CInputBtnDlg5(int kind, CWnd* pParent) : CDialog(CInputBtnDlg5::IDD, pParent)
/********************************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_Kind   = kind;
	m_nID    = 0;
}

/******************************************************/
void CInputBtnDlg5::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDV_MinMaxDouble(pDX, m_Param1, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDV_MinMaxDouble(pDX, m_Param2, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDV_MinMaxDouble(pDX, m_Param3, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
}

/**************************************/
BOOL CInputBtnDlg5::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn5Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn5Top", -1);
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
	if(m_Kind==1) {														// <�Ȑ�����>
		GetDlgItem(IDC_BUTTON4)->ShowWindow(SW_HIDE);					// �f�ʉ�]�t����
	} else {															// <�|����>
		str.LoadString(IDS_STR85);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg5::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	*m_Pn = m_No;														// �����㐧��_��
	CDialog::OnOK();
}

/***********************************/
void CInputBtnDlg5::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn5Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn5Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg5::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/********************************************************************/
BOOL CInputBtnDlg5::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
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
				ObjectEdit();											// �Ȗʐ�������
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/******************************************/
void CInputBtnDlg5::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
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
				ObjectEdit();											// �Ȗʐ�������
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/************************************/
void CInputBtnDlg5::OnAddCtrl1(void)									//<<< ����(�Ȑ�)���݉�������
/************************************/
{
	AddCtrlPoint(0);													// ����_�ǉ�����
}

/************************************/
void CInputBtnDlg5::OnAddCtrl2(void)									//<<< ����(�ܐ�)���݉�������
/************************************/
{
	AddCtrlPoint(1);													// ����_�ǉ�����
}

/**************************************/
void CInputBtnDlg5::OnCancelCtrl(void)									//<<< ������݉�������
/**************************************/
{
	int i;

	if(m_No>1) {														// <<����_��o�^�ς�>>
		if(m_Kind==1) {													// <�Ȑ�����>
			m_No--;														// ����_�����Z
			SetCurveMatrix1(m_Md, m_Wt, 0, m_No, m_Ia, m_Da, m_Db);		// �ʉߓ_=>����_�ϊ���د��
			CalcCurvePoint(m_Md, m_No, m_Pb, m_Pa, m_Db);				// �ʉߓ_=>����_�ϊ�
			RedoCrtCurve(m_No, 0, m_Pa, m_Ia);							// �Ȑ�����(�{)
			RedoSetCurve(m_No, 0, m_Pb);								// �Ȑ�����_����(�{)
			for(i=0; i<m_No; i++) {
				m_Ib[i] = m_Ia[i]*2;
			}
			View->Tracker1.Setup(m_Pb, m_Ib, NULL, m_No, 4, 0);			// �ׯ��1�ݒ�
			SetCurveMatrix1(m_Md, m_Wt, 0, m_No+1, m_Ia, m_Da, m_Db);	// �ʉߓ_=>����_�ϊ���د��
		} else {														// <�|���̐���>
			m_No--;														// ����_�����Z
			SetCurveMatrix1(m_Md, m_Wt, 0, m_No, m_Ib, m_Da, m_Db);		// �ʉߓ_=>����_�ϊ���د��
			CalcSweepPoint(1, m_Sw, m_Md, 0, m_An, m_Bc, m_No,
                           m_Pa, m_Ia, m_Pb, m_Ib, m_Pc, m_Ic, m_Db);	// �|���̐���_�v�Z
			RedoSetSurface(m_No, m_An, 0, m_Bc, m_Pc);					// �Ȗʐ���_����(�{)
			View->Tracker1.Setup(m_Pc, m_Ic, NULL, m_No*m_An, 5, 0);	// �ׯ��1�ݒ�
			SetCurveMatrix1(m_Md, m_Wt, 0, m_No+1, m_Ib, m_Da, m_Db);	// �ʉߓ_=>����_�ϊ���د��
		}
		m_Param1 = 0.0, m_Param2 = 0.0, m_Param3 = 0.0;					// �ړ��ʸر
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
	}
	View->Display(0);													// �ĕ`��
}

/************************************/
void CInputBtnDlg5::OnRotate2D(void)									//<<< �f�ʉ�]���݉�������
/************************************/
{
	if(m_Kind==2) {														// <�|���̂̏ꍇ>
		ChangeRotate2D(1, m_Sf, m_An, m_Pa);							// 2D�f�ʉ�]�X�V
		SetCurveMatrix1(m_Md, m_Wt, 0, m_No, m_Ib, m_Da, m_Db);			// �ʉߓ_=>����_�ϊ���د��
		CalcSweepPoint(1, m_Sw, m_Md, 0, m_An, m_Bc, m_No,
                       m_Pa, m_Ia, m_Pb, m_Ib, m_Pc, m_Ic, m_Db);		// �|���̐���_�v�Z
		RedoSetSurface(m_No, m_An, 0, m_Bc, m_Pc);						// �Ȗʐ���_����(�{)
		View->Tracker1.Setup(m_Pc, m_Ic, NULL, m_No*m_An, 5, 0);		// �ׯ��1�ݒ�
		SetCurveMatrix1(m_Md, m_Wt, 0, m_No+1, m_Ib, m_Da, m_Db);		// �ʉߓ_=>����_�ϊ���د��
		View->Display(0);												// �ĕ`��
	}
}

/*****************************************/
void CInputBtnDlg5::AddCtrlPoint(int flg)								//<<< ����_�ǉ�����
/*****************************************/
{
	PNTTYPE p, pt;
	int     i, ok=0;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		p.p[0] = m_Param1, p.p[1] = m_Param2, p.p[2] = m_Param3;		// X/Y/Z�ړ���
		if(fabs(p.p[0])>EPSILON||
           fabs(p.p[1])>EPSILON||
           fabs(p.p[2])>EPSILON) {										// <<����_�ړ��L��>>
			if(m_Kind==1) {												// <�Ȑ�����>
				for(i=0; i<DIM; i++) {
					pt.p[i] = m_Pa[m_No-1].p[i]+p.p[i];					// ���񒸓_
				}
				if(CheckSymmetryArea(&m_Ch, pt, m_Sm, m_Cp,
                                     m_Dv, m_D1, m_D2)) {				// �Ώ̗̈�����
					if((m_No+1)*m_Dv<=View->m_MaxCCtl) {				// <����_�������>
						m_Ia[m_No] = (flg) ? 1 : 0;						// Shift�ܐ��_Ӱ��
						m_Pb[m_No] = pt;								// ����_�ݒ�(�{)
						m_No++;
					} else {
						AfxMessageBox(IDS_MANY_CTRLP);					// �װү����
					}
					CalcCurvePoint(m_Md, m_No, m_Pb, m_Pa, m_Db);		// �ʉߓ_=>����_�ϊ�
					RedoCrtCurve(m_No, 0, m_Pa, m_Ia);					// �Ȑ�����(�{)
					RedoSetCurve(m_No, 0, m_Pb);						// �Ȑ�����_����(�{)
					for(i=0; i<m_No; i++) {
						m_Ib[i] = m_Ia[i]*2;
					}
					View->Tracker1.Setup(m_Pb, m_Ib, NULL, m_No, 4, 0);	// �ׯ��1�ݒ�
					SetCurveMatrix1(m_Md, m_Wt, 0, m_No+1,
                                    m_Ia, m_Da, m_Db);					// �ʉߓ_=>����_�ϊ���د��
					ok = 1;
				}
			} else {													// <�|���̐���>
				if(m_An*(2*m_Sf+m_No+1)<=View->m_MaxSCtl&&
                   2*m_Sf+m_No+1<=View->m_MaxCCtl) {					// <����_�������>
					m_Ib[m_No] = (flg) ? 1 : 0;							// Shift�ܐ��_Ӱ��
					for(i=0; i<DIM; i++) {
						m_Pb[m_No].p[i] = m_Pb[m_No-1].p[i]+p.p[i];		// ����_�ݒ�(�{)
					}
					m_No++;
				} else {
					AfxMessageBox(IDS_MANY_CTRLP);						// �װү����
				}
				CalcSweepPoint(1, m_Sw, m_Md, 0, m_An, m_Bc, m_No, m_Pa,
                               m_Ia, m_Pb, m_Ib, m_Pc, m_Ic, m_Db);		// �|���̐���_�v�Z
				RedoSetSurface(m_No, m_An, 0, m_Bc, m_Pc);				// �Ȗʐ���_����(�{)
				View->Tracker1.Setup(m_Pc, m_Ic, NULL, m_No*m_An, 5, 0);// �ׯ��1�ݒ�
				SetCurveMatrix1(m_Md, m_Wt, 0, m_No+1,
                                m_Ib, m_Da, m_Db);						// �ʉߓ_=>����_�ϊ���د��
				ok = 1;
			}
		}
		View->Display(0);												// �ĕ`��
	}
	if(ok) {															// <���튮���̏ꍇ>
		m_Param1 = 0.0, m_Param2 = 0.0, m_Param3 = 0.0;					// �ړ��ʸر
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
	}
}

/************************************/
void CInputBtnDlg5::ObjectEdit(void)									//<<< �Ȗʐ�������
/************************************/
{
	PNTTYPE p;
	int     i;
	double  sv;

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
			p.p[0] = m_Param1, p.p[1] = m_Param2, p.p[2] = m_Param3;	// X/Y/Z�ړ���
			if(fabs(p.p[0])>EPSILON||
               fabs(p.p[1])>EPSILON||
               fabs(p.p[2])>EPSILON) {									// <<����_�ړ��L��>>
				if(m_Kind==1) {											// <�Ȑ�����>
					m_Ia[m_No] = 0;										// Ӱ�ސݒ�(��)
					for(i=0; i<DIM; i++) {
						m_Pb[m_No].p[i] = m_Pa[m_No-1].p[i]+p.p[i];		// ����_�ݒ�(��)
					}
					CalcCurvePoint(m_Md, m_No+1, m_Pb, m_Pa, m_Db);		// �ʉߓ_=>����_�ϊ�
					RedoCrtCurve(m_No+1, 0, m_Pa, m_Ia);				// �Ȑ�����(��)
					RedoSetCurve(m_No+1, 0, m_Pb);						// �Ȑ�����_����(��)
					View->Tracker1.Setup(m_Pb, m_Ib, NULL, m_No, 4, 0);	// �ׯ��1�ݒ�
				} else {												// <�|���̐���>
					m_Ib[m_No] = 1;										// Ӱ�ސݒ�(��)
					for(i=0; i<DIM; i++) {
						m_Pb[m_No].p[i] = m_Pb[m_No-1].p[i]+p.p[i];		// ����_�ݒ�(��)
					}
					CalcSweepPoint(1, m_Sw, m_Md, 0, m_An, m_Bc,
                                   m_No+1, m_Pa, m_Ia, m_Pb,
                                   m_Ib, m_Pc, m_Ic, m_Db);				// �|���̐���_�v�Z
					RedoSetSurface(m_No+1, m_An, 0, m_Bc, m_Pc);		// �Ȗʐ���_����(��)
					View->Tracker1.Setup(m_Pc, m_Ic, NULL,
                                         (m_No+1)*m_An, 5, 0);			// �ׯ��1�ݒ�
				}
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
BOOL CInputBtnDlg5::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

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
	return ret;
}

/*****************************************************************/
void CInputBtnDlg5::SetParam1(int md, int sm, int ch, PNTTYPE cp,
                              int dv, int d1, int d2, int bc,
                              double wt, int* pn, PNTTYPE* pa,
                              PNTTYPE* pb, int* ia, int* ib,
                              double* da, double* db, double ad)		//<<< ���Ұ��ݒ�1
/*****************************************************************/
{
	m_Md = md, m_Sm = sm, m_Ch = ch, m_Cp = cp, m_Dv = dv, m_D1 = d1;
	m_D2 = d2, m_Bc = bc, m_Wt = wt, m_Pn = pn, m_No = *pn, m_Pa = pa;
	m_Pb = pb, m_Ia = ia, m_Ib = ib, m_Da = da, m_Db = db, m_Param4 = ad;
}

/************************************************************************/
void CInputBtnDlg5::SetParam2(int sw, int md, int sf, int bc, double wt,
                              int an, int* pn, PNTTYPE* pa, PNTTYPE* pb,
                              PNTTYPE* pc, int* ia, int* ib, int* ic,
                              double* da, double* db, double ad)		//<<< ���Ұ��ݒ�2
/************************************************************************/
{
	m_Sw = sw, m_Md = md, m_Sf = sf, m_Bc = bc, m_Wt = wt, m_An = an;
	m_Pn = pn, m_No = *pn, m_Pa = pa, m_Pb = pb, m_Pc = pc, m_Ia = ia;
	m_Ib = ib, m_Ic = ic, m_Da = da, m_Db = db, m_Param4 = ad;
}

/****************************************/
void CInputBtnDlg5::GetParam(double* ad)                                //<<< ���Ұ��擾
/****************************************/
{
	*ad = m_Param4;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg6 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg6, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/********************************************************************************************/
CInputBtnDlg6::CInputBtnDlg6(int kind, CWnd* pParent) : CDialog(CInputBtnDlg6::IDD, pParent)
/********************************************************************************************/
{
	m_Param1 = 0.0;
	m_Param2 = 0.0;
	m_Param3 = 0.0;
	m_Param4 = 0.0;
	m_Kind   = kind;
	m_nID    = 0;
}

/******************************************************/
void CInputBtnDlg6::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Param1);
	DDV_MinMaxDouble(pDX, m_Param1, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT2, m_Param2);
	DDV_MinMaxDouble(pDX, m_Param2, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_Param3);
	DDV_MinMaxDouble(pDX, m_Param3, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_Param4);
	DDV_MinMaxDouble(pDX, m_Param4, 0.001, 100.0);
}

/**************************************/
BOOL CInputBtnDlg6::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn6Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn6Top", -1);
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
	if(m_Kind==2) {														// <����_�ړ�>
		str.LoadString(IDS_STR86);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
	} else if(m_Kind==3) {												// <����_��ړ�>
		str.LoadString(IDS_STR87);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
	} else if(m_Kind==4) {												// <�f�ʐ���_�ړ�>
		str.LoadString(IDS_STR88);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
	} else if(m_Kind==5) {												// <�ް����ړ�>
		str.LoadString(IDS_STR89);										// ���َ擾
		SetWindowText(str);												// ���ٕ\��
	}
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg6::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(m_Kind==4) {														// <�f�ʐ���_�ړ��̏ꍇ>
		if(m_Pl==0) {													// �w�蕽�ʈȊO��
			m_Param3 = 0.0;												// �ړ��ʂ͸ر
		} else if(m_Pl==1) {
			m_Param1 = 0.0;
		} else {
			m_Param2 = 0.0;
		}
	}
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		MoveCtrlPoint();												// ����_�ړ�
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg6::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn6Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn6Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg6::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/********************************************************************/
BOOL CInputBtnDlg6::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
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
				ObjectEdit();											// �ȖʕҏW����
				SetTimer(TIMERID, 500, NULL);							// ��ϰ�ݒ�
			}
		}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, msg);
}

/******************************************/
void CInputBtnDlg6::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
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
				ObjectEdit();											// �ȖʕҏW����
			}
			SetTimer(TIMERID, 20, NULL);								// ��ϰ�ݒ�
		}
	}
}

/************************************/
void CInputBtnDlg6::ObjectEdit(void)									//<<< �ȖʕҏW����
/************************************/
{
	double sv;

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
		if(m_Kind==4) {													// <�f�ʐ���_�ړ��̏ꍇ>
			if(m_Pl==0) {												// �w�蕽�ʈȊO��
				m_Param3 = 0.0;											// �ړ��ʂ͸ر
			} else if(m_Pl==1) {
				m_Param1 = 0.0;
			} else {
				m_Param2 = 0.0;
			}
		}
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
		if(!DataCheck()) {												// <<�����ް��m�FOK>>
			MoveCtrlPoint();											// ����_�ړ�
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

/***************************************/
void CInputBtnDlg6::MoveCtrlPoint(void)									//<<< ����_�ړ�
/***************************************/
{
	int*    wk = new int[View->m_MaxSCtA];
	PNTTYPE p;
	int     i, tn;

	p.p[0] = m_Param1, p.p[1] = m_Param2, p.p[2] = m_Param3;			// X/Y/Z�ړ���
	*m_Ch = 1;															// ���s�ς�
	if(m_Kind==1) {														// <�Ȑ��ҏW>
		MoveSymmetryArea(m_Pos, p, m_Mod, m_Cpt, m_Div, m_Dr1,
                         m_Dr2, m_Fg1, m_Fg2, m_Rvf,
                         m_Dn, m_Vn, m_Ic, m_Py, m_Pw);					// �Ώ̗̈�ړ��ʐݒ�
		MoveECurve(m_Md, m_Db, m_Pw, m_Op1, m_Vn, m_Vb, m_Pa,
                   m_Pb, m_Ic, m_Pc, m_Ta);								// ����_�ړ�
		RedoChgCurve(m_Vn, m_Vb, m_Pc, m_Ia);							// �Ȑ�����
		TrackerECurve(m_Md, m_Da, m_Vb, m_Vn, m_Pc, m_Ia,
                      m_Ic, &tn, m_Pd, m_Ib);							// �Ȑ��ҏW�ׯ���擾
		RedoSetCurve(tn, m_Vb, m_Pd);									// �Ȑ�����_����
		for(i=0; i<tn; i++) {
			GetVtx(m_Op2, i, &m_Pd[i]);									// ���̕ҏW�㐧��_�擾
		}
		i = (m_Dn>0) ? m_Dn*2-1 : tn;									// �p���͕\�����ύX
		DispParsTracker(0, m_Fg, m_Pd, m_Ib, i, m_Px, m_Ix, m_Xn);		// �����}�ׯ���\��
	} else if(m_Kind==2) {												// <����_�ړ�>
		MoveSymmetryArea(m_Pos, p, m_Mod, m_Cpt, 0, m_Dr1, m_Dr2,
                         -1, -1, m_Rvf, 0, m_An, m_Ib, m_Pb, m_Pw);		// �Ώ̗̈�ړ��ʐݒ�
		MoveCPoint(m_Md, m_Pw, m_Op1, m_Un, m_Vn, m_Ub, m_Vb, m_Wt,
                   m_Ia, m_Ib, m_Pa, m_Pb, m_Pc, m_Ta);					// ����_�ړ�
		RedoChgSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pc, m_Ia);				// �Ȗʐ���
		CalcSurfacePoint(m_Md, 1, 0, m_Un, m_Vn, m_Ub, m_Vb, m_Wt,
                         m_Ia, m_Pc, m_Pd);								// ����_=>�ʉߓ_�ϊ�
		RedoSetSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pd);					// �Ȗʐ���_����
		for(i=0; i<m_An; i++) {
			GetVtx(m_Op2, i, &m_Pd[i]);									// ���̕ҏW�㐧��_�擾
		}
		DispParsTracker(0, m_Fg, m_Pd, m_Ic, m_An, m_Px, m_Ix, m_Xn);	// �����}�ׯ���\��
		View->ObjectShading(m_Op1);										// ����ިݸ�ؽēo�^
	} else if(m_Kind==3) {												// <����_��ړ�>
		if(m_Sw) {														// <�ʏ�Ȗ�>
			MoveSymmetryArea(m_Pos, p, m_Mod, m_Cpt, 0, m_Dr1, m_Dr2,
                             -1, -1, m_Rvf, 0, m_An, m_Ib, m_Pb, m_Py);	// �Ώ̗̈�ړ��ʐݒ�
			MoveCPoint(m_Md, m_Py, m_Op1, m_Un, m_Vn, m_Ub, m_Vb, m_Wt,
                       m_Ia, m_Ib, m_Pa, m_Pb, m_Pc, m_Ta);				// ����_�ړ�
			RedoChgSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pc, m_Ia);			// �Ȗʐ���
			CalcSurfacePoint(m_Md, 1, 0, m_Un, m_Vn, m_Ub, m_Vb, m_Wt,
                             m_Ia, m_Pc, m_Pd);							// ����_=>�ʉߓ_�ϊ�
		} else {														// <�|���Ȗ�>
			MoveSymmetryArea(m_Pos, p, m_Mod, m_Cpt, 0, m_Dr1, m_Dr2,
                             -1, -1, m_Rvf, 0, m_Tn, m_Iw, m_Pz, m_Py);	// �Ώ̗̈�ړ��ʐݒ�
			MoveSweep(m_Py, m_Pl, m_Un, m_Vn, m_Ps, m_Tn, m_Iw, m_Pt);	// �|������_�ړ�
			RedoChgSweepSurf(m_Un, m_Vn, m_Ub, m_Vb, m_Pt, m_Is);		// �|���ȖʕύX
			SetSweepCtrl(m_Pl, m_Un, m_Vn, m_Ub, m_Pt, m_Is, m_Pd, wk);	// �|���Ȗʐ���_�ϊ�
		}
		RedoSetSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pd);					// �Ȗʐ���_����
		for(i=0; i<m_An; i++) {
			GetVtx(m_Op2, i, &m_Pd[i]);									// ���̕ҏW�㐧��_�擾
		}
		TrackerCLine(m_Md, m_Sw, m_Un, m_Vn, m_Pd, m_F1, m_F2,
                     &tn, m_Pw, m_Ic);									// �_���ׯ���擾
		for(i=0; i<tn; i++) {
			if(m_Iw[i]) {
				m_Ic[i] = 4;											// �w��_�F�ύX
			}
		}
		DispParsTracker(0, m_Fg, m_Pw, m_Ic, tn, m_Px, m_Ix, m_Xn);		// �����}�ׯ���\��
		View->ObjectShading(m_Op1);										// ����ިݸ�ؽēo�^
	} else if(m_Kind==4) {												// <�f�ʐ���_�ړ�>
		MoveSymmetryArea(m_Pos, p, m_Mod, m_Cpt, 0, m_Dr1, m_Dr2, -1,
                         -1, m_Rvf, 0, m_Vn, m_Ib, m_Pa, m_Py);			// �Ώ̗̈�ړ��ʐݒ�
		MoveSCurve(m_Md, m_Db, m_Py, m_Op1, m_Vn, m_Vb, m_Pa,
                   m_Pc, m_Ib, m_Pb, m_Ta);								// �f�ʐ���_�ړ�
		RedoCrtCurve(m_Vn, m_Vb, m_Pb, m_Ia);							// �Ȑ�����
		CalcCurvePoint(m_Md, m_Vn, m_Pb, m_Pd, m_Da);					// ����_=>�ʉߓ_�ϊ�
		RedoSetCurve(m_Vn, m_Vb, m_Pd);									// �Ȑ�����_����
		for(i=0; i<m_Vn; i++) {
			m_Ic[i]=(m_Ib[i]) ? 4 : m_Ia[i]*2;							// �ׯ���F�ݒ�
		}
		View->Tracker1.Setup(m_Pd, m_Ic, NULL, m_Vn, m_Pl, 0);			// �ׯ��1�ݒ�
	} else {															// <�ް����ړ�>
		MoveSymmetryArea(m_Pos, p, m_Mod, m_Cpt, 0, m_Dr1, m_Dr2, -1,
                         -1, m_Rvf, 0, m_Tn, m_Ib, m_Pz, m_Ps);			// �Ώ̗̈�ړ��ʐݒ�
		MoveBPoint(m_Md, m_Pt, m_Ps, m_Op1, m_Un, m_Vn, m_Ub, m_Vb,
                  m_Wt, m_F2, m_Tn, m_Ia, m_Ib, m_Pa, m_Pb, m_Pc, m_Ta);// �ް����ړ�
		RedoChgSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pc, m_Ia);				// �Ȗʐ���
		CalcSurfacePoint(m_Md, 1, 0, m_Un, m_Vn, m_Ub, m_Vb, m_Wt,
                         m_Ia, m_Pc, m_Pd);								// ����_=>�ʉߓ_�ϊ�
		RedoSetSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pd);					// �Ȗʐ���_����
		for(i=0; i<m_An; i++) {
			GetVtx(m_Op2, i, &m_Pd[i]);									// ���̕ҏW�㐧��_�擾
		}
		TrackerULine(m_Md, m_Un, m_Vn, m_Pd, m_F1, m_F2, &tn,
                     m_Pw, m_Iw);										// �_���ׯ���擾
		for(i=0; i<tn; i++) {
			if(m_Ib[i]) {
				m_Iw[i] = 4;											// �w��_�F�ύX
			}
		}
		DispParsTracker(0, m_Fg, m_Pw, m_Iw, tn, m_Px, m_Ix, m_Xn);		// �����}�ׯ���\��
		View->ObjectShading(m_Op1);										// ����ިݸ�ؽēo�^
	}
	View->Display(0);													// �ĕ`��
	delete[] wk;
}

/***********************************/
BOOL CInputBtnDlg6::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

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
	return ret;
}

/***********************************************************************/
void CInputBtnDlg6::SetParam1(OBJTYPE*op1, OBJTYPE*op2, int md, int fg,
                              int vn, int vb, int xn, int* ch, int dn,
                              int mod, int div, int dr1, int dr2,
                              int fg1, int fg2, int rvf, PNTTYPE pos,
                              PNTTYPE cpt, PNTTYPE* pa, PNTTYPE* pb,
                              PNTTYPE* pc, PNTTYPE* pd, PNTTYPE* pw,
                              PNTTYPE* px, PNTTYPE* py, int* ia,
                              int* ib, int* ic, int* ix, int* ta,
                              double* da, double* db, double ad)		//<<< ���Ұ��ݒ�1
/***********************************************************************/
{
	m_Op1 = op1, m_Op2 = op2, m_Md = md, m_Fg = fg, m_Vn = vn, m_Vb = vb;
	m_Xn = xn, m_Ch = ch, m_Dn = dn, m_Mod = mod, m_Div = div;
	m_Dr1 = dr1, m_Dr2 = dr2, m_Fg1 = fg1, m_Fg2 = fg2, m_Rvf = rvf;
	m_Pos = pos, m_Cpt = cpt, m_Pa = pa, m_Pb = pb, m_Pc = pc, m_Pd = pd;
	m_Pw = pw, m_Px = px, m_Py = py, m_Ia = ia, m_Ib = ib, m_Ic = ic;
	m_Ix = ix, m_Ta = ta,	m_Da = da, m_Db = db, m_Param4 = ad;
}

/********************************************************************/
void CInputBtnDlg6::SetParam2(OBJTYPE* op1, OBJTYPE* op2, int md,
                              int fg, int un, int vn, int ub,
                              int vb, double wt, int an, int xn,
                              int* ch, int mod, int dr1, int dr2,
                              int rvf, PNTTYPE pos, PNTTYPE cpt,
                              PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc,
                              PNTTYPE* pd, PNTTYPE* pw, PNTTYPE* px,
                              int* ia, int* ib, int* ic, int* ix,
                              int* ta, double ad)						//<<< ���Ұ��ݒ�2
/********************************************************************/
{
	m_Op1 = op1, m_Op2 = op2, m_Md = md, m_Fg = fg, m_Un = un;
	m_Vn = vn, m_Ub = ub, m_Vb = vb, m_Wt = wt, m_An = an;
	m_Xn = xn, m_Ch = ch, m_Mod = mod, m_Dr1 = dr1, m_Dr2 = dr2;
	m_Rvf = rvf, m_Pos = pos, m_Cpt = cpt, m_Pa = pa, m_Pb = pb;
	m_Pc = pc, m_Pd = pd, m_Pw = pw, m_Px = px, m_Ia = ia;
	m_Ib = ib, m_Ic = ic, m_Ix = ix, m_Ta = ta, m_Param4 = ad;
}

/**********************************************************************/
void CInputBtnDlg6::SetParam3(OBJTYPE* op1, OBJTYPE* op2, int md,
                              int sw, int fg, int un, int vn, int ub,
                              int vb, double wt, int an, int f1,
                              int f2, int xn, int tn, int pl, int* ch,
                              int mod, int dr1, int dr2, int rvf,
                              PNTTYPE pos, PNTTYPE cpt, PNTTYPE* pa,
                              PNTTYPE* pb, PNTTYPE* pc, PNTTYPE* pd,
                              PNTTYPE* ps, PNTTYPE* pt, PNTTYPE* pw,
                              PNTTYPE* px, PNTTYPE* py, PNTTYPE* pz,
                              int* ia, int* ib, int* ic, int* is,
                              int* iw, int* ix, int* ta, double ad)		//<<< ���Ұ��ݒ�3
/**********************************************************************/
{
	m_Op1 = op1, m_Op2 = op2, m_Md = md, m_Sw = sw, m_Fg = fg;
	m_Un = un, m_Vn = vn, m_Ub = ub, m_Vb = vb, m_Wt = wt, m_An = an;
	m_F1 = f1, m_F2 = f2, m_Xn = xn, m_Tn = tn, m_Pl = pl, m_Ch = ch;
	m_Mod = mod, m_Dr1 = dr1, m_Dr2 = dr2, m_Rvf = rvf, m_Pos = pos;
	m_Cpt = cpt, m_Pa = pa, m_Pb = pb, m_Pc = pc, m_Pd = pd;
	m_Ps = ps, m_Pt = pt, m_Pw = pw, m_Px = px, m_Py = py, m_Pz = pz;
	m_Ia = ia, m_Ib = ib, m_Ic = ic, m_Is = is, m_Iw = iw;
	m_Ix = ix, m_Ta = ta, m_Param4 = ad;
}

/******************************************************************/
void CInputBtnDlg6::SetParam4(OBJTYPE* op, int md, int vn, int vb,
                              int pl, int* ch, int mod, int dr1,
                              int dr2, int rvf, PNTTYPE pos,
                              PNTTYPE cpt, PNTTYPE* pa, PNTTYPE* pb,
                              PNTTYPE* pc, PNTTYPE* pd, PNTTYPE* py,
                              int* ia, int* ib, int* ic, int* ta,
                              double* da, double* db, double ad)		//<<< ���Ұ��ݒ�4
/******************************************************************/
{
	m_Op1 = op, m_Md = md, m_Vn = vn, m_Vb = vb, m_Pl = pl;
	m_Ch = ch, m_Mod = mod, m_Dr1 = dr1, m_Dr2 = dr2, m_Rvf = rvf;
	m_Pos = pos, m_Cpt = cpt, m_Pa = pa, m_Pb = pb, m_Pc = pc;
	m_Pd = pd, m_Py = py, m_Ia = ia, m_Ib = ib, m_Ic = ic;
	m_Ta = ta, m_Da = da, m_Db = db, m_Param4 = ad;
}

/*********************************************************************/
void CInputBtnDlg6::SetParam5(OBJTYPE* op1, OBJTYPE* op2, int md,
                              int fg, int un, int vn, int ub, int vb,
                              double wt, int an, int f1, int f2,
                              int tn, int xn, int* ch, int mod,
                              int dr1, int dr2, int rvf, PNTTYPE pos,
                              PNTTYPE cpt, PNTTYPE* pt, PNTTYPE* pa,
                              PNTTYPE* pb, PNTTYPE* pc, PNTTYPE* pd,
                              PNTTYPE* ps, PNTTYPE* pw, PNTTYPE* px,
                              PNTTYPE* pz, int* ia, int* ib, int* iw,
                              int* ix, int* ta, double ad)				//<<< ���Ұ��ݒ�5
/*********************************************************************/
{
	m_Op1 = op1, m_Op2 = op2, m_Md = md, m_Fg = fg, m_Un = un;
	m_Vn = vn, m_Ub = ub, m_Vb = vb, m_Wt = wt, m_An = an;
	m_F1 = f1, m_F2 = f2, m_Tn = tn, m_Xn = xn, m_Ch = ch;
	m_Mod = mod, m_Dr1 = dr1, m_Dr2 = dr2, m_Rvf = rvf, m_Pos = pos;
	m_Cpt = cpt, m_Pt = pt, m_Pa = pa, m_Pb = pb, m_Pc = pc;
	m_Pd = pd, m_Ps = ps, m_Pw = pw, m_Px = px, m_Pz = pz, m_Ia = ia;
	m_Ib = ib, m_Iw = iw, m_Ix = ix, m_Ta = ta, m_Param4 = ad;
}

/****************************************/
void CInputBtnDlg6::GetParam(double* ad)                                //<<< ���Ұ��擾
/****************************************/
{
	*ad = m_Param4;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg7 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg7, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/********************************************************************************************/
CInputBtnDlg7::CInputBtnDlg7(int kind, CWnd* pParent) : CDialog(CInputBtnDlg7::IDD, pParent)
/********************************************************************************************/
{
	m_MoveX = 0.0, m_MoveY = 0.0, m_MoveZ = 0.0, m_AddMove = 0.0;
	m_RotateX = 0.0, m_RotateY = 0.0, m_RotateZ = 0.0, m_AddRotate = 0.0;
	m_Scale = 0.0, m_AddScale = 0.0;
	m_SDirect = 0, m_SPoint = 1;
	m_Kind = kind, m_nID = 0, m_Init = 0;
}

/******************************************************/
void CInputBtnDlg7::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MoveX);
	DDV_MinMaxDouble(pDX, m_MoveX, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT2, m_MoveY);
	DDV_MinMaxDouble(pDX, m_MoveY, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_MoveZ);
	DDV_MinMaxDouble(pDX, m_MoveZ, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_AddMove);
	DDV_MinMaxDouble(pDX, m_AddMove, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT5, m_RotateX);
	DDV_MinMaxDouble(pDX, m_RotateX, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT6, m_RotateY);
	DDV_MinMaxDouble(pDX, m_RotateY, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT7, m_RotateZ);
	DDV_MinMaxDouble(pDX, m_RotateZ, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT8, m_AddRotate);
	DDV_MinMaxDouble(pDX, m_AddRotate, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT9, m_Scale);
	DDV_MinMaxDouble(pDX, m_Scale, 0.001, 10.0);
	DDX_Text(pDX, IDC_EDIT10, m_AddScale);
	DDV_MinMaxDouble(pDX, m_AddScale, 0.001, 2.0);
	DDX_Radio(pDX, IDC_RADIO1, m_SDirect);
	DDX_Radio(pDX, IDC_RADIO4, m_SPoint);
}

/**************************************/
BOOL CInputBtnDlg7::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn7Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn7Top", -1);
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
	VERIFY(Plus4.AutoLoad(IDC_PLUS4, this));
	VERIFY(Minus4.AutoLoad(IDC_MINUS4, this));
	VERIFY(Plus5.AutoLoad(IDC_PLUS5, this));
	VERIFY(Minus5.AutoLoad(IDC_MINUS5, this));
	VERIFY(Plus6.AutoLoad(IDC_PLUS6, this));
	VERIFY(Minus6.AutoLoad(IDC_MINUS6, this));
	VERIFY(Plus7.AutoLoad(IDC_PLUS7, this));
	VERIFY(Minus7.AutoLoad(IDC_MINUS7, this));
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg7::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	PNTTYPE cp;
	int     i, ch=0;
	double  sx, sy, sz;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(fabs(m_MoveX)>EPSILON||fabs(m_MoveY)>EPSILON||fabs(m_MoveZ)>EPSILON||
           fabs(m_RotateX)>EPSILON||fabs(m_RotateY)>EPSILON||fabs(m_RotateZ)>EPSILON||
           fabs(m_Scale-1.0)>EPSILON||m_Init!=0) {
			if(m_SDirect==0) {											// <ػ��ޕ���:�S��>
				sx = m_Scale, sy = m_Scale, sz = m_Scale;
			} else if(m_SDirect==1) {									// <ػ��ޕ���:����>
				sx = m_Scale, sy = m_Scale, sz = 1.0;
			} else {													// <ػ��ޕ���:����>
				sx = 1.0, sy = 1.0, sz = m_Scale;
			}
			ch = (m_SPoint!=m_SPointS) ? 1 : ch;						// ��_�ύX����
			if(m_Kind==1) {												// <��߰������>
				ObjectSpace(m_Init, m_RCenter, m_SCenter, m_SPoint,
                            ch, m_MoveX, m_MoveY, m_MoveZ, m_RotateX,
                            m_RotateY, m_RotateZ, sx, sy, sz);			// ��߰�
			} else if(m_Kind==2) {										// <��������>
				ObjectEarth(m_Init, m_RCenter, m_SCenter, m_SPoint,
                            ch, m_MoveX, m_MoveY, m_MoveZ, m_RotateX,
                            m_RotateY, m_RotateZ, sx, sy, sz);			// ���
			} else {													// <CPS�����>
				*m_Ch = 1;												// ���s�ς�
				cp = (m_SPoint==0)?m_Cp1:((m_SPoint==1)?m_Cp2:m_Cp3);
				CtrlPointSpace(m_Pa, m_Pw, m_Ib, m_An, m_Cp2, cp, 0, 0,
                               m_MoveX, m_MoveY, m_MoveZ, m_RotateX,
                               m_RotateY, m_RotateZ, sx, sy, sz, 0.0);	// ����_��߰�(CPS)
				MoveCPoint(0, m_Pw, m_Op1, m_Un, m_Vn, m_Ub, m_Vb, 2.0,
                           m_Ia, m_Ib, m_Pa, m_Pb, m_Pc, m_Ta);			// ����_�ړ�
				RedoChgSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pc, m_Ia);		// �Ȗʐ����ʉߓ_�ϊ�
				for(i=0; i<m_An; i++) {
					m_Pd[i] = m_Pc[i];
				}
				RedoSetSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pd);			// �Ȗʐ���_����
				for(i=0; i<m_An; i++) {
					GetVtx(m_Op2, i, &m_Pd[i]);							// ���̕ҏW�㐧��_�擾
				}
				DispParsTracker(0, m_Fg, m_Pd, m_Ic, m_An,
                                m_Px, m_Ix, m_Xn);						// �����}�ׯ���\��
				View->ObjectShading(m_Op1);								// ����ިݸ�ؽēo�^
			}
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg7::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn7Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn7Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg7::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/******************************************/
void CInputBtnDlg7::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
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
BOOL CInputBtnDlg7::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖt��ID�擾
			if(id>=IDC_PLUS1&&id<=IDC_MINUS7) {							// ϳ����ޯ�ϯ�ߖt��
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
void CInputBtnDlg7::ObjectEdit(void)									//<<< ���̕ҏW����
/************************************/
{
	PNTTYPE cp;
	int     i, ch=0;
	double  sv, sx, sy, sz;

	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z�O�ۑ�
			sv = m_MoveX;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			sv = m_MoveY;
		} else if(m_nID==IDC_PLUS3||m_nID==IDC_MINUS3) {
			sv = m_MoveZ;
		} else if(m_nID==IDC_PLUS4||m_nID==IDC_MINUS4) {
			sv = m_RotateX;
		} else if(m_nID==IDC_PLUS5||m_nID==IDC_MINUS5) {
			sv = m_RotateY;
		} else if(m_nID==IDC_PLUS6||m_nID==IDC_MINUS6) {
			sv = m_RotateZ;
		} else {
			sv = m_Scale;
		}
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z/���Z
			m_MoveX = (m_nID==IDC_PLUS1)?m_MoveX+m_AddMove:m_MoveX-m_AddMove;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			m_MoveY = (m_nID==IDC_PLUS2)?m_MoveY+m_AddMove:m_MoveY-m_AddMove;
		} else if(m_nID==IDC_PLUS3||m_nID==IDC_MINUS3) {
			m_MoveZ = (m_nID==IDC_PLUS3)?m_MoveZ+m_AddMove:m_MoveZ-m_AddMove;
		} else if(m_nID==IDC_PLUS4||m_nID==IDC_MINUS4) {
			m_RotateX = (m_nID==IDC_PLUS4)?m_RotateX+m_AddRotate:m_RotateX-m_AddRotate;
		} else if(m_nID==IDC_PLUS5||m_nID==IDC_MINUS5) {
			m_RotateY = (m_nID==IDC_PLUS5)?m_RotateY+m_AddRotate:m_RotateY-m_AddRotate;
		} else if(m_nID==IDC_PLUS6||m_nID==IDC_MINUS6) {
			m_RotateZ = (m_nID==IDC_PLUS6)?m_RotateZ+m_AddRotate:m_RotateZ-m_AddRotate;
		} else {
			m_Scale = (m_nID==IDC_PLUS7)?m_Scale+m_AddScale:m_Scale-m_AddScale;
		}
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
		if(!DataCheck()) {												// <<�����ް��m�FOK>>
			if(fabs(m_MoveX)>EPSILON||fabs(m_MoveY)>EPSILON||fabs(m_MoveZ)>EPSILON||
		       fabs(m_RotateX)>EPSILON||fabs(m_RotateY)>EPSILON||fabs(m_RotateZ)>EPSILON||
			   fabs(m_Scale-1.0)>EPSILON||m_Init!=0) {
				if(m_SDirect==0) {										// <ػ��ޕ���:�S��>
					sx = m_Scale, sy = m_Scale, sz = m_Scale;
				} else if(m_SDirect==1) {								// <ػ��ޕ���:����>
					sx = m_Scale, sy = m_Scale, sz = 1.0;
				} else {												// <ػ��ޕ���:����>
					sx = 1.0, sy = 1.0, sz = m_Scale;
				}
				ch = (m_SPoint!=m_SPointS) ? 1 : ch;					// ��_�ύX����
				m_SPointS = m_SPoint;
				if(m_Kind==1) {											// <��߰������>
					ObjectSpace(m_Init, m_RCenter, m_SCenter, m_SPoint,
                                ch, m_MoveX, m_MoveY, m_MoveZ, m_RotateX,
                                m_RotateY, m_RotateZ, sx, sy, sz);		// ��߰�
				} else if(m_Kind==2) {									// <��������>
					ObjectEarth(m_Init, m_RCenter, m_SCenter, m_SPoint,
                                ch, m_MoveX, m_MoveY, m_MoveZ, m_RotateX,
                                m_RotateY, m_RotateZ, sx, sy, sz);		// ���
				} else {												// <CPS�����>
					*m_Ch = 1;											// ���s�ς�
					cp=(m_SPoint==0)?m_Cp1:((m_SPoint==1)?m_Cp2:m_Cp3);
					CtrlPointSpace(m_Pa, m_Pw, m_Ib, m_An, m_Cp2, cp,
                                   0, 0, m_MoveX, m_MoveY, m_MoveZ,
                                   m_RotateX, m_RotateY, m_RotateZ,
                                   sx, sy, sz, 0.0);					// ����_��߰�(CPS)
					MoveCPoint(0, m_Pw, m_Op1, m_Un, m_Vn, m_Ub, m_Vb,
                               2.0, m_Ia, m_Ib, m_Pa, m_Pb, m_Pc, m_Ta);// ����_�ړ�
					RedoChgSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pc, m_Ia);	// �Ȗʐ����ʉߓ_�ϊ�
					for(i=0; i<m_An; i++) {
						m_Pd[i] = m_Pc[i];
					}
					RedoSetSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pd);		// �Ȗʐ���_����
					for(i=0; i<m_An; i++) {
						GetVtx(m_Op2, i, &m_Pd[i]);						// ���̕ҏW�㐧��_�擾
					}
					DispParsTracker(0, m_Fg, m_Pd, m_Ic, m_An,
                                    m_Px, m_Ix, m_Xn);					// �����}�ׯ���\��
					View->ObjectShading(m_Op1);							// ����ިݸ�ؽēo�^
				}
				m_Init = 1;												// �����I��
			}
			View->Display(0);											// �ĕ`��
		} else {														// <<�����ް��m�FNG>>
			UpdateData(TRUE);											// �װү���ޗp
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {
				m_MoveX = sv;
			} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
				m_MoveY = sv;
			} else if(m_nID==IDC_PLUS3||m_nID==IDC_MINUS3) {
				m_MoveZ = sv;
			} else if(m_nID==IDC_PLUS4||m_nID==IDC_MINUS4) {
				m_RotateX = sv;
			} else if(m_nID==IDC_PLUS5||m_nID==IDC_MINUS5) {
				m_RotateY = sv;
			} else if(m_nID==IDC_PLUS6||m_nID==IDC_MINUS6) {
				m_RotateZ = sv;
			} else {
				m_Scale = sv;
			}
			UpdateData(FALSE);											// �޲�۸ޏ���ް�����
		}
	}
}

/***********************************/
BOOL CInputBtnDlg7::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

	if(m_MoveX<-1000.0||m_MoveX>1000.0) {
		ret = TRUE;
	}
	if(m_MoveY<-1000.0||m_MoveY>1000.0) {
		ret = TRUE;
	}
	if(m_MoveZ<-1000.0||m_MoveZ>1000.0) {
		ret = TRUE;
	}
	if(m_AddMove<0.001||m_AddMove>100.0) {
		ret = TRUE;
	}
	if(m_RotateX<-360.0||m_RotateX>360.0) {
		ret = TRUE;
	}
	if(m_RotateY<-360.0||m_RotateY>360.0) {
		ret = TRUE;
	}
	if(m_RotateZ<-360.0||m_RotateZ>360.0) {
		ret = TRUE;
	}
	if(m_AddRotate<0.001||m_AddRotate>100.0) {
		ret = TRUE;
	}
	if(m_Scale<0.001||m_Scale>10.0) {
		ret = TRUE;
	}
	if(m_AddScale<0.001||m_AddScale>2.0) {
		ret = TRUE;
	}
	return ret;
}

/***********************************************************************/
void CInputBtnDlg7::SetParam(double mx, double my, double mz,
                             double rx, double ry, double rz,
                             double sc, double d1, double d2,
                             double d3, int i1, int i2, int i3, int i4)	//<<< ���Ұ��ݒ�
/***********************************************************************/
{
	m_MoveX     = mx, m_MoveY    = my, m_MoveZ   = mz, m_RotateX = rx;
	m_RotateY   = ry, m_RotateZ  = rz, m_Scale   = sc, m_AddMove = d1;
	m_AddRotate = d2, m_AddScale = d3, m_SDirect = i1, m_SPoint  = i2;
	m_SCenter   = i3, m_RCenter  = i4, m_SPointS = m_SPoint;
}

/************************************************************************/
void CInputBtnDlg7::SetParam2(OBJTYPE* op1, OBJTYPE* op2, int fg,
                              int un, int vn, int ub, int vb, int an,
                              int xn, int* ch, PNTTYPE cp1, PNTTYPE cp2,
                              PNTTYPE cp3, PNTTYPE* pa, PNTTYPE* pb,
                              PNTTYPE* pc, PNTTYPE* pd, PNTTYPE* pw,
                              PNTTYPE* px, int* ia, int* ib, int* ic,
                              int* ix, int* ta, double d1, double d2,
                              double d3, int i1, int i2)				//<<< ���Ұ��ݒ�2
/************************************************************************/
{
	m_Op1 = op1, m_Op2 = op2, m_Fg = fg, m_Un = un, m_Vn = vn;
	m_Ub = ub, m_Vb = vb, m_An = an, m_Xn = xn, m_Ch = ch;
	m_Cp1 = cp1, m_Cp2 = cp2, m_Cp3 = cp3, m_Pa = pa, m_Pb = pb;
	m_Pc = pc, m_Pd = pd, m_Pw = pw, m_Px = px, m_Ia = ia, m_Ib = ib;
	m_Ic = ic, m_Ix = ix, m_Ta = ta, m_AddMove = d1, m_AddRotate = d2;
	m_AddScale = d3, m_SDirect = i1, m_SPoint = i2;
	m_MoveX = 0.0, m_MoveY = 0.0, m_MoveZ = 0.0;
	m_RotateX = 0.0, m_RotateY = 0.0, m_RotateZ = 0.0, m_Scale = 1.0;
}

/****************************************************************/
void CInputBtnDlg7::GetParam(double* d1, double* d2, double* d3,
                             int* i1, int* i2)							//<<< ���Ұ��擾
/****************************************************************/
{
	*d1 = m_AddMove, *d2 = m_AddRotate, *d3 = m_AddScale;
	*i1 = m_SDirect, *i2 = m_SPoint;
}

///////////////////////////////////////////////////////////////////////////////
// CInputBtnDlg8 �_�C�A���O

BEGIN_MESSAGE_MAP(CInputBtnDlg8, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/**********************************************************************************/
CInputBtnDlg8::CInputBtnDlg8(CWnd* pParent) : CDialog(CInputBtnDlg8::IDD, pParent)
/**********************************************************************************/
{
	m_MoveX = 0.0, m_MoveY = 0.0, m_MoveZ = 0.0, m_AddMove = 0.0;
	m_RotateX = 0.0, m_RotateY = 0.0, m_RotateZ = 0.0, m_AddRotate = 0.0;
	m_MoveR = 0.0, m_AddMoveR = 0.0;
	m_nID = 0, m_Init = 0;
}

/******************************************************/
void CInputBtnDlg8::DoDataExchange(CDataExchange* pDX)					//<<< �޲�۸ޥ�ް��̌����ƗL��������
/******************************************************/
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_MoveX);
	DDV_MinMaxDouble(pDX, m_MoveX, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT2, m_MoveY);
	DDV_MinMaxDouble(pDX, m_MoveY, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT3, m_MoveZ);
	DDV_MinMaxDouble(pDX, m_MoveZ, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT4, m_AddMove);
	DDV_MinMaxDouble(pDX, m_AddMove, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT5, m_RotateX);
	DDV_MinMaxDouble(pDX, m_RotateX, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT6, m_RotateY);
	DDV_MinMaxDouble(pDX, m_RotateY, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT7, m_RotateZ);
	DDV_MinMaxDouble(pDX, m_RotateZ, -360.0, 360.0);
	DDX_Text(pDX, IDC_EDIT8, m_AddRotate);
	DDV_MinMaxDouble(pDX, m_AddRotate, 0.001, 100.0);
	DDX_Text(pDX, IDC_EDIT9, m_MoveR);
	DDV_MinMaxDouble(pDX, m_MoveR, -1000.0, 1000.0);
	DDX_Text(pDX, IDC_EDIT10, m_AddMoveR);
	DDV_MinMaxDouble(pDX, m_AddMoveR, 0.001, 100.0);
}

/**************************************/
BOOL CInputBtnDlg8::OnInitDialog(void)									//<<< WM_INITDIALOG ү���މ�������
/**************************************/
{
	CString str;
	int     left, top, width, height;

	CDialog::OnInitDialog();
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	left = theApp.GetProfileInt("LAYOUT", "Btn8Left", -1);
	top = theApp.GetProfileInt("LAYOUT", "Btn8Top", -1);
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
	VERIFY(Plus4.AutoLoad(IDC_PLUS4, this));
	VERIFY(Minus4.AutoLoad(IDC_MINUS4, this));
	VERIFY(Plus5.AutoLoad(IDC_PLUS5, this));
	VERIFY(Minus5.AutoLoad(IDC_MINUS5, this));
	VERIFY(Plus6.AutoLoad(IDC_PLUS6, this));
	VERIFY(Minus6.AutoLoad(IDC_MINUS6, this));
	VERIFY(Plus7.AutoLoad(IDC_PLUS7, this));
	VERIFY(Minus7.AutoLoad(IDC_MINUS7, this));
	return TRUE;	// �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
}

/******************************/
void CInputBtnDlg8::OnOK(void)											//<<< OK���݉�������
/******************************/
{
	int i;

	UpdateData(TRUE);													// �޲�۸ޏ���ް��擾
	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(fabs(m_MoveX)>EPSILON||fabs(m_MoveY)>EPSILON||fabs(m_MoveZ)>EPSILON||
           fabs(m_RotateX)>EPSILON||fabs(m_RotateY)>EPSILON||fabs(m_RotateZ)>EPSILON||
           fabs(m_MoveR)>EPSILON||m_Init!=0) {
			*m_Ch = 1;													// ���s�ς�
			CtrlPointSpace(m_Pa, m_Pw, m_Ib, m_An, m_Cp, m_Cp,
                           1, m_Pl, m_MoveX, m_MoveY, m_MoveZ,
                           m_RotateX, m_RotateY, m_RotateZ,
                           1.0, 1.0, 1.0, m_MoveR);						// ����_��߰�(CPS)
			MoveCPoint(0, m_Pw, m_Op1, m_Un, m_Vn, m_Ub, m_Vb, 2.0,
                       m_Ia, m_Ib, m_Pa, m_Pb, m_Pc, m_Ta);				// ����_�ړ�
			RedoChgSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pc, m_Ia);			// �Ȗʐ����ʉߓ_�ϊ�
			for(i=0; i<m_An; i++) {
				m_Pd[i] = m_Pc[i];
			}
			RedoSetSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pd);				// �Ȗʐ���_����
			for(i=0; i<m_An; i++) {
				GetVtx(m_Op2, i, &m_Pd[i]);								// ���̕ҏW�㐧��_�擾
			}
			DispParsTracker(0, m_Fg, m_Pd, m_Ic, m_An,
                            m_Px, m_Ix, m_Xn);							// �����}�ׯ���\��
			View->ObjectShading(m_Op1);									// ����ިݸ�ؽēo�^
		}
		CDialog::OnOK();
	}
}

/***********************************/
void CInputBtnDlg8::OnDestroy(void)										//<<< WM_DESTROY ү���މ�������
/***********************************/
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);											// ����޳�\����Ԏ擾
	theApp.WriteProfileInt("LAYOUT", "Btn8Left", wp.rcNormalPosition.left);
	theApp.WriteProfileInt("LAYOUT", "Btn8Top", wp.rcNormalPosition.top);
	CDialog::OnDestroy();
}

/******************************************/
LRESULT CInputBtnDlg8::OnNcHitTest(CPoint)								//<<< WM_NCHITTEST ү���މ�������
/******************************************/
{
	return HTCAPTION;													// �ײ��ė̈����ׯ�ނ��ĳ���޳��
}																		// �ړ����邽��

/******************************************/
void CInputBtnDlg8::OnTimer(UINT_PTR nIDTimer)								//<<< WM_TIMER ү���މ�������
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
BOOL CInputBtnDlg8::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT msg)	//<<< WM_SETCURSOR ү���މ�������
/********************************************************************/
{
	if(nHitTest==HTCLIENT&&msg==WM_LBUTTONDOWN) {						// �ޯ�ϯ�ߖt���ϳ����t����
		POINT point;
		GetCursorPos(&point);											// ϳ��̽�ذݍ��W�擾
		ScreenToClient(&point);
		CWnd* pWnd = ChildWindowFromPoint(point);						// ϳ��ʒu���܂ޖt�擾
		if(pWnd!=NULL) {
			int id = pWnd->GetDlgCtrlID();								// ϳ��ʒu���܂ޖt��ID�擾
			if(id>=IDC_PLUS1&&id<=IDC_MINUS7) {							// ϳ����ޯ�ϯ�ߖt��
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
void CInputBtnDlg8::ObjectEdit(void)									//<<< ���̕ҏW����
/************************************/
{
	int    i;
	double sv;

	if(!DataCheck()) {													// <<�����ް��m�FOK>>
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z�O�ۑ�
			sv = m_MoveX;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			sv = m_MoveY;
		} else if(m_nID==IDC_PLUS3||m_nID==IDC_MINUS3) {
			sv = m_MoveZ;
		} else if(m_nID==IDC_PLUS4||m_nID==IDC_MINUS4) {
			sv = m_RotateX;
		} else if(m_nID==IDC_PLUS5||m_nID==IDC_MINUS5) {
			sv = m_RotateY;
		} else if(m_nID==IDC_PLUS6||m_nID==IDC_MINUS6) {
			sv = m_RotateZ;
		} else {
			sv = m_MoveR;
		}
		if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {						// ���Z/���Z
			m_MoveX = (m_nID==IDC_PLUS1)?m_MoveX+m_AddMove:m_MoveX-m_AddMove;
		} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
			m_MoveY = (m_nID==IDC_PLUS2)?m_MoveY+m_AddMove:m_MoveY-m_AddMove;
		} else if(m_nID==IDC_PLUS3||m_nID==IDC_MINUS3) {
			m_MoveZ = (m_nID==IDC_PLUS3)?m_MoveZ+m_AddMove:m_MoveZ-m_AddMove;
		} else if(m_nID==IDC_PLUS4||m_nID==IDC_MINUS4) {
			m_RotateX = (m_nID==IDC_PLUS4)?m_RotateX+m_AddRotate:m_RotateX-m_AddRotate;
		} else if(m_nID==IDC_PLUS5||m_nID==IDC_MINUS5) {
			m_RotateY = (m_nID==IDC_PLUS5)?m_RotateY+m_AddRotate:m_RotateY-m_AddRotate;
		} else if(m_nID==IDC_PLUS6||m_nID==IDC_MINUS6) {
			m_RotateZ = (m_nID==IDC_PLUS6)?m_RotateZ+m_AddRotate:m_RotateZ-m_AddRotate;
		} else {
			m_MoveR = (m_nID==IDC_PLUS7)?m_MoveR+m_AddMoveR:m_MoveR-m_AddMoveR;
		}
		UpdateData(FALSE);												// �޲�۸ޏ���ް��\��
		if(!DataCheck()) {												// <<�����ް��m�FOK>>
			if(fabs(m_MoveX)>EPSILON||fabs(m_MoveY)>EPSILON||fabs(m_MoveZ)>EPSILON||
               fabs(m_RotateX)>EPSILON||fabs(m_RotateY)>EPSILON||fabs(m_RotateZ)>EPSILON||
               fabs(m_MoveR)>EPSILON||m_Init!=0) {
				*m_Ch = 1;												// ���s�ς�
				CtrlPointSpace(m_Pa, m_Pw, m_Ib, m_An, m_Cp, m_Cp,
                               1, m_Pl, m_MoveX, m_MoveY, m_MoveZ,
                               m_RotateX, m_RotateY, m_RotateZ,
                               1.0, 1.0, 1.0, m_MoveR);					// ����_��߰�(CPS)
				MoveCPoint(0, m_Pw, m_Op1, m_Un, m_Vn, m_Ub, m_Vb, 2.0,
                           m_Ia, m_Ib, m_Pa, m_Pb, m_Pc, m_Ta);			// ����_�ړ�
				RedoChgSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pc, m_Ia);		// �Ȗʐ����ʉߓ_�ϊ�
				for(i=0; i<m_An; i++) {
					m_Pd[i] = m_Pc[i];
				}
				RedoSetSurface(m_Un, m_Vn, m_Ub, m_Vb, m_Pd);			// �Ȗʐ���_����
				for(i=0; i<m_An; i++) {
					GetVtx(m_Op2, i, &m_Pd[i]);							// ���̕ҏW�㐧��_�擾
				}
				DispParsTracker(0, m_Fg, m_Pd, m_Ic, m_An,
                                m_Px, m_Ix, m_Xn);						// �����}�ׯ���\��
				View->ObjectShading(m_Op1);								// ����ިݸ�ؽēo�^
				m_Init = 1;												// �����I��
			}
			View->Display(0);											// �ĕ`��
		} else {														// <<�����ް��m�FNG>>
			UpdateData(TRUE);											// �װү���ޗp
			if(m_nID==IDC_PLUS1||m_nID==IDC_MINUS1) {
				m_MoveX = sv;
			} else if(m_nID==IDC_PLUS2||m_nID==IDC_MINUS2) {
				m_MoveY = sv;
			} else if(m_nID==IDC_PLUS3||m_nID==IDC_MINUS3) {
				m_MoveZ = sv;
			} else if(m_nID==IDC_PLUS4||m_nID==IDC_MINUS4) {
				m_RotateX = sv;
			} else if(m_nID==IDC_PLUS5||m_nID==IDC_MINUS5) {
				m_RotateY = sv;
			} else if(m_nID==IDC_PLUS6||m_nID==IDC_MINUS6) {
				m_RotateZ = sv;
			} else {
				m_MoveR = sv;
			}
			UpdateData(FALSE);											// �޲�۸ޏ���ް�����
		}
	}
}

/***********************************/
BOOL CInputBtnDlg8::DataCheck(void)										//<<< �����ް��m�F
/***********************************/
{
	BOOL ret = FALSE;

	if(m_MoveX<-1000.0||m_MoveX>1000.0) {
		ret = TRUE;
	}
	if(m_MoveY<-1000.0||m_MoveY>1000.0) {
		ret = TRUE;
	}
	if(m_MoveZ<-1000.0||m_MoveZ>1000.0) {
		ret = TRUE;
	}
	if(m_AddMove<0.001||m_AddMove>100.0) {
		ret = TRUE;
	}
	if(m_RotateX<-360.0||m_RotateX>360.0) {
		ret = TRUE;
	}
	if(m_RotateY<-360.0||m_RotateY>360.0) {
		ret = TRUE;
	}
	if(m_RotateZ<-360.0||m_RotateZ>360.0) {
		ret = TRUE;
	}
	if(m_AddRotate<0.001||m_AddRotate>100.0) {
		ret = TRUE;
	}
	if(m_MoveR<-1000.0||m_MoveR>1000.0) {
		ret = TRUE;
	}
	if(m_AddMoveR<0.001||m_AddMoveR>100.0) {
		ret = TRUE;
	}
	return ret;
}

/********************************************************************/
void CInputBtnDlg8::SetParam(OBJTYPE* op1, OBJTYPE* op2, int fg,
                             int un, int vn, int ub, int vb, int an,
                             int xn, int pl, int* ch, PNTTYPE cp,
                             PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc,
                             PNTTYPE* pd, PNTTYPE* pw, PNTTYPE* px,
                             int* ia, int* ib, int* ic, int* ix,
                             int* ta, double d1, double d2)				//<<< ���Ұ��ݒ�
/********************************************************************/
{
	m_Op1 = op1, m_Op2 = op2, m_Fg = fg, m_Un = un, m_Vn = vn;
	m_Ub = ub, m_Vb = vb, m_An = an, m_Xn = xn, m_Pl = pl, m_Ch = ch;
	m_Cp = cp, m_Pa = pa, m_Pb = pb, m_Pc = pc, m_Pd = pd, m_Pw = pw;
	m_Px = px, m_Ia = ia, m_Ib = ib, m_Ic = ic, m_Ix = ix, m_Ta = ta;
	m_AddMove = d1, m_AddRotate = d2, m_AddMoveR = d1;
	m_MoveX = 0.0, m_MoveY = 0.0, m_MoveZ = 0.0;
	m_RotateX = 0.0, m_RotateY = 0.0, m_RotateZ = 0.0, m_MoveR = 0.0;
}

/****************************************************/
void CInputBtnDlg8::GetParam(double* d1, double* d2)					//<<< ���Ұ��擾
/****************************************************/
{
	*d1 = m_AddMove, *d2 = m_AddRotate;
}
