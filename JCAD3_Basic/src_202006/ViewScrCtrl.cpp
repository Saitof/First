///////////////////////////////////////////////////////////////////////////// 
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewScrCtrl.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "NumFunc.h"

/*******************************************/
void MouseBind(CPoint& point, CPoint& base)								//<<< ϳ����͍��W�S��
/*******************************************/
{
	double hi;
	int    x=point.x-base.x, y=point.y-base.y;

	hi = (double)y/(double)x;											// X�ɑ΂���Y�̔���Z�o
	if(fabs(hi)<=1.0) {													// X���ɋ߂�(45�x�ȓ�)
		point.y = base.y;
	} else {															// Y���ɋ߂�(45�x�ȓ�)
		point.x = base.x;
	}
}

/********************************/
void CJcad3GlbView::ZoomIn(void)										//<<< �ްѥ��(3�ʐ})
/********************************/
{
	if(m_Fovy>200) {													// �\�������K��l(��m)�ȏ�
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3�ʐ}�\����ԕۑ�
		m_Fovy = (int)(m_Fovy*0.8+0.5);
		SetSeishaei(m_Cent3D, m_Fovy);									// ���ˉe�s��̍č\�z
		ChangeTexPixels();												// ø����߸�ٍX�V
		if(m_GridDisp) {
			SetGrid();													// ��د�ސݒ�
		}
		Display(1);														// �ĕ`��
	}
}

/*********************************/
void CJcad3GlbView::ZoomOut(void)										//<<< �ްѥ���(3�ʐ})
/*********************************/
{
	if(m_Fovy<1000000) {												// �\�������K��l(��m)�ȉ�
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3�ʐ}�\����ԕۑ�
		m_Fovy = (int)(m_Fovy/0.8+0.5);
		SetSeishaei(m_Cent3D, m_Fovy);									// ���ˉe�s��̍č\�z
		ChangeTexPixels();												// ø����߸�ٍX�V
		if(m_GridDisp) {
			SetGrid();													// ��د�ސݒ�
		}
		Display(1);														// �ĕ`��
	}
}

/**********************************/
void CJcad3GlbView::AutoZoom(void)										//<<< �����{��
/**********************************/
{
	OBJTYPE* op;
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, dsp, edt, cnt=0;
	double   x, y, z, fvy1, fvy2, fvy3, fvy4, fvy5, fvy6;

	BaseObjPtr(1, &op, &dsp, &edt);
	if(!op) {															// �\�����̖��͒��~
		return;
	}
	m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;							// 3�ʐ}�\����ԕۑ�
	GetAreaBox(1, &box);												// �S���̂�BOX�̍ő�l�擾
	if(m_DAraFlg) {														// <�\���͈͎w��̏ꍇ>
		for(i=0; i<DIM; i++) {
			if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
				box.bp[0].p[i] = m_DAra.bp[0].p[i];
			}
			if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
				box.bp[1].p[i] = m_DAra.bp[1].p[i];
			}
		}
	}
	BoxCenter(box, &cpt);												// �ޯ��������3�ʒ��S
	ScrToPnt(1, &m_Cent3D, &cpt);										// PNTTYPE => SCRTYPE
	x = box.bp[1].p[0]-box.bp[0].p[0];									// x,y,z���ގ擾
	y = box.bp[1].p[1]-box.bp[0].p[1];
	z = box.bp[1].p[2]-box.bp[0].p[2];
	fvy1 = -1.0, fvy2 = -1.0, fvy3 = -1.0;
	fvy4 = -1.0 ,fvy5 = -1.0, fvy6 = -1.0;
	if(PlSizeH[XY]>0&&PlSizeW[XY]>0) {									// ��ذݕ\������
		cnt++;
		fvy1 = x*0.5/ScrDiv2;
		fvy2 = y*0.5*PlSizeH[XY]/(ScrDiv2*PlSizeW[XY]);
	}
	if(PlSizeH[YZ]>0&&PlSizeW[YZ]>0) {
		cnt++;
		fvy3 = z*0.5/(1.0-ScrDiv2);
		fvy4 = y*0.5*PlSizeH[YZ]/((1.0-ScrDiv2)*PlSizeW[YZ]);
	}
	if(PlSizeH[ZX]>0&&PlSizeW[ZX]>0) {
		cnt++;
		fvy5 = z*0.5/(1.0-ScrDiv2);
		fvy6 = x*0.5*PlSizeH[ZX]/((1.0-ScrDiv2)*PlSizeW[ZX]);
	}
	if(cnt>0) {															// <3�ʂ̉��ꂩ���L��>
		fvy1 = (fvy1<fvy2) ? fvy2 : fvy1;
		fvy1 = (fvy1<fvy3) ? fvy3 : fvy1;
		fvy1 = (fvy1<fvy4) ? fvy4 : fvy1;
		fvy1 = (fvy1<fvy5) ? fvy5 : fvy1;
		fvy1 = (fvy1<fvy6) ? fvy6 : fvy1;
		m_Fovy = (int)(1000.0*fvy1/0.8+0.5);							// ��ʕ\�����X�V
	}
	ScrToPnt(0, &m_Cent3D, &cpt);										// SCRTYPE => PNTTYPE
	m_pState->SetCent3D(cpt);
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��̍č\�z
	ChangeTexPixels();													// ø����߸�ٍX�V
	if(m_GridDisp) {
		SetGrid();														// ��د�ސݒ�
	}
	Display(1);															// �ĕ`��
}

/***********************************/
void CJcad3GlbView::ExactZoom(void)										//<<< �����\��
/***********************************/
{
	CWnd*    m_pwnd;
	CDC*     m_pcdc;
	OBJTYPE* op;
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, dsp, edt, sh, sw, px, py;

	BaseObjPtr(1, &op, &dsp, &edt);
	if(!op) {															// �\�����̖��͒��~
		return;
	}
	if(m_FChar&0x1) {													// <[F5]:�{���␳>
		CInputDlg dlg(27);
		dlg.SetValue(m_ExactRev);										// �����l�\��
		if(dlg.DoModal()==IDOK) {										// �����޲�۸ޕ\��
			dlg.GetValue(&m_ExactRev);									// �␳�{������
		} else {														// �޲�۸޷�ݾ�
			return;
		}
	}
	m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;							// 3�ʐ}�\����ԕۑ�
	SetScreenNo(0);														// �\���͈͐ݒ�
	GetAreaBox(1, &box);												// �S���̂�BOX�̍ő�l�擾
	if(m_DAraFlg) {														// <�\���͈͎w��̏ꍇ>
		for(i=0; i<DIM; i++) {
			if(box.bp[0].p[i]>m_DAra.bp[0].p[i]) {
				box.bp[0].p[i] = m_DAra.bp[0].p[i];
			}
			if(box.bp[1].p[i]<m_DAra.bp[1].p[i]) {
				box.bp[1].p[i] = m_DAra.bp[1].p[i];
			}
		}
	}
	BoxCenter(box, &cpt);												// �ޯ��������3�ʒ��S
	ScrToPnt(1, &m_Cent3D, &cpt);										// PNTTYPE => SCRTYPE
	ScrToPnt(0, &m_Cent3D, &cpt);										// SCRTYPE => PNTTYPE
	m_pState->SetCent3D(cpt);
	sw = PlRect[XY].Width(), sh = PlRect[XY].Height();					// �����}�]��������
	m_pwnd = GetDesktopWindow();										// �޽�į�߳���޳
	m_pcdc = m_pwnd->GetDC();											// ��è�޳���޳�̸ײ���DC
	px = m_pcdc->GetDeviceCaps(LOGPIXELSX);								// X�������dpi
	py = m_pcdc->GetDeviceCaps(LOGPIXELSY);								// Y�������dpi
	m_Fovy = (int)((25.4*1000.0*sh)/(py*m_ExactRev));					// ��ʕ\�����X�V
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��̍č\�z
	ChangeTexPixels();													// ø����߸�ٍX�V
	if(m_GridDisp) {
		SetGrid();														// ��د�ސݒ�
	}
	Display(0);															// �ĕ`��
}

/**********************************************************/
void CJcad3GlbView::ZoomBox(UINT flg, const CPoint& point,
                            int plane, int ms)							//<<< ��ʎw��{��
/**********************************************************/
{
	PNTTYPE pnt;
	CPoint  cent;
	CSize   size;
	int     cx, cy, len, h;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// �����}��̏ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2&&flg&MK_LBUTTON) {											// ϳ����t�����Ȃ�����ׯ��
		if(plane!=XY&&plane!=YZ&&plane!=ZX) {
			return;
		}
		CalcScreenWorld(0, &pnt, point, plane);							// ���W�_�擾
		DrawBox(BaseCursor, pnt, plane);								// BOX�\��
		Display(0);														// �ĕ`��
	} else if(ms==3) {													// BOX�̌���
		if(plane!=XY&&plane!=YZ&&plane!=ZX) {
			return;
		}
		cent = BasePoint + point;
		cent.x = (int)((cent.x/2.0)+0.5);								// BOX�̒��S�ʒu�Z�o
		cent.y = (int)((cent.y/2.0)-0.5);								//�@ �i�l�̌ܓ��t�j
		CalcScreenWorld(0, &pnt, cent, Plane);							// ���S�_��3D�ϊ�
		m_pState->SetCent3D(pnt);
		ScrToPnt(1, &m_Cent3D, &pnt);									// PNTTYPE => SCRTYPE
		size = point - BasePoint;
		cx = abs(size.cx), cy = abs(size.cy);
		len = cy<cx ? cx : cy;
		if(len>2) {
			h = (int)(m_Fovy*len/PlSizeH[plane]/0.9+0.5);				// �\�����Z�o
			m_Fovy = (h>200&&h<1000000) ? h : m_Fovy;					// �\�������K��l���͕\�����ݒ�
		}
		SetSeishaei(m_Cent3D, m_Fovy);									// ���ˉe�s��̍č\�z
		ChangeTexPixels();												// ø����߸�ٍX�V
		if(m_GridDisp) {
			SetGrid();													// ��د�ސݒ�
		}
		DeleteBox();													// ��`�j��
		Display(1);														// �ĕ`��
	} else if(ms==0) {													// �ƭ��J�n
		m_CursorNo = (m_ActMenu!=NONE) ? 3 : m_CursorNo;				// ����No�ݒ�
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3�ʐ}�\����ԕۑ�
	} else if(ms==4||ms==9) {											// �ƭ��I��
		m_ActMenu = m_SaveMenu;											// �N�����ƭ��֖߂�
		m_SaveMenu = NONE;												// �ۑ��ƭ��ԍ��ر
		m_CursorNo = m_CursorNo_S;										// ����No����
		m_CursorNo_S = 0;												// �ۑ�����No�ر
		InitChar();														// �����͏�����
	}
}

/*********************************************************/
void CJcad3GlbView::Scroll(UINT flg, const CPoint& point,
                           int plane, int ms)							//<<< ��ʽ�۰�
/*********************************************************/
{
	PNTTYPE pnt, cpt;
	int     pln2=(plane+1)%3;
	double  dd[3];

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// �����}��̏ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2&&flg&MK_LBUTTON) {											// ϳ��̍��t�������̂ݗL��
		if(plane!=XY&&plane!=YZ&&plane!=ZX) {
			return;
		}
		m_Cent3D = SscrA;
		CalcScreenWorld(2, &pnt, point, plane);							// ���W�_�擾
		for(int i=0; i<DIM; i++) {
			dd[i] = pnt.p[i]-SpntA.p[i];								// �ŏ��̓_�ƌ��ݓ_�Ƃ̍�
		}
		if(flg&m_LimKey) {
			LimitValue(plane, dd);										// ϳ����͐����l
		}
		m_Cent3D.p[plane] = SscrA.p[plane]-(int)(dd[plane]*1000.0);
		m_Cent3D.p[pln2] = SscrA.p[pln2]-(int)(dd[pln2]*1000.0);
		ScrToPnt(0, &m_Cent3D, &cpt);									// SCRTYPE => PNTTYPE
		m_pState->SetCent3D(cpt);
		SetSeishaei(m_Cent3D, m_Fovy);									// ���ˉe�s��̍č\�z
		ChangeTexPixels();												// ø����߸�ٍX�V
		if(m_GridDisp) {
			SetGrid();													// ��د�ސݒ�
		}
		Display(1);														// �ĕ`��
	} else if(ms==1) {													// ϳ����t����
		CalcScreenWorld(2, &SpntA, point, plane);						// ��۰ٌ��_�̍��W�擾
		SscrA = m_Cent3D;												// 3�ʐ}�̎Q�Ɠ_�ۑ�
	} else if(ms==3) {													// ϳ����t���
		m_Cent3D = SscrA;
		CalcScreenWorld(2, &pnt, point, plane);							// ���W�_�擾
		for(int i=0; i<DIM; i++) {
			dd[i] = pnt.p[i]-SpntA.p[i];								// �ŏ��̓_�ƌ��ݓ_�Ƃ̍�
		}
		if(flg&m_LimKey) {
			LimitValue(plane, dd);										// ϳ����͐����l
		}
		m_Cent3D.p[plane] = SscrA.p[plane]-(int)(dd[plane]*1000.0);
		m_Cent3D.p[pln2] = SscrA.p[pln2]-(int)(dd[pln2]*1000.0);
		ScrToPnt(0, &m_Cent3D, &cpt);									// SCRTYPE => PNTTYPE
		m_pState->SetCent3D(cpt);
		SetSeishaei(m_Cent3D, m_Fovy);									// ���ˉe�s��̍č\�z
		ChangeTexPixels();												// ø����߸�ٍX�V
		if(m_GridDisp) {
			SetGrid();													// ��د�ސݒ�
		}
		Display(1);														// �ĕ`��
	} else if(ms==0) {													// �ƭ��J�n
		m_CursorNo = (m_ActMenu!=NONE) ? 2 : m_CursorNo;				// ����No�ݒ�
		m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;						// 3�ʐ}�\����ԕۑ�
	} else if(ms==4||ms==9) {											// �ƭ��I��
		m_ActMenu = m_SaveMenu;											// �N�����ƭ��֖߂�
		m_SaveMenu = NONE;												// �ۑ��ƭ��ԍ��ر
		m_CursorNo = m_CursorNo_S;										// ����No����
		m_CursorNo_S = 0;												// �ۑ�����No�ر
		InitChar();														// �����͏�����
	}
}

/*********************************************/
void CJcad3GlbView::Scroll(int axis, int dir)							//<<< ��ʽ�۰٥°�
/*********************************************/
{
	PNTTYPE cpt;

	m_Cent3D_SS = m_Cent3D, m_Fovy_SS = m_Fovy;							// 3�ʐ}�\����ԕۑ�
	m_Cent3D.p[axis] = m_Cent3D.p[axis]+(dir*m_GridSpace);				// 3�ʐ}�̒��S�_�ύX
	ScrToPnt(0, &m_Cent3D, &cpt);										// SCRTYPE => PNTTYPE
	m_pState->SetCent3D(cpt);
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��̍č\�z
	ChangeTexPixels();													// ø����߸�ٍX�V
	if(m_GridDisp) {
		SetGrid();														// ��د�ސݒ�
	}
	Display(1);															// �ĕ`��
}

/*****************************************/
void CJcad3GlbView::BackScreenState(void)								//<<< 3�ʐ}�\����Ԗ߂�
/*****************************************/
{
	m_Cent3D = m_Cent3D_SS, m_Fovy = m_Fovy_SS;							// 3�ʐ}�\����Ԗ߂�
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��̍č\�z
	ChangeTexPixels();													// ø����߸�ٍX�V
	if(m_GridDisp) {
		SetGrid();														// ��د�ސݒ�
	}
	Display(1);															// �ĕ`��
}

/***************************************************************/
void CJcad3GlbView::BasePointSet(UINT flg, const CPoint& point,
                                 int plane, int ms)						//<<< ��_/���o�^
/***************************************************************/
{
	PNTTYPE pnt, cpt;
	BOXTYPE box;
	int     i1, i2;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// <<<�����}�̏ꍇ>>>
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// <<<ϳ��ړ�>>>
		if(Sphase==2) {
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(flg&m_LimKey) {
				LimitPoint(plane, &pnt, &Sbase);						// ϳ����͐������W
			}
			i1 = plane;													// �L�����W���ݒ�
			i2 = (plane+1>2) ? 0 : plane+1;
			Spa[Si6].p[i1] = pnt.p[i1], Spa[Si6].p[i2] = pnt.p[i2];		// ���W�l�ύX
			if(Si1==0) {												// ��_�̏ꍇ
				ScrToPnt(1, &m_Bpoint, &Spa[0]);
			} else {													// ����̏ꍇ
				ScrToPnt(1, &m_Blspnt, &Spa[0]);
				ScrToPnt(1, &m_Blepnt, &Spa[1]);
			}
			SetBase();													// ��_/���ݒ�
			Tracker1.Setup(Spa, Sia, NULL, Si2, 5, 0);					// �ׯ��1�ݒ�
			Display(0);													// �ĕ`��
		}
	} else if(ms==1) {													// <<<ϳ����t����>>>
		if(Sphase==1) {
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((Si6=Tracker1.HitTracker(1, plane, pnt, Spa, Si2))!=-1) {// �ׯ��1����
				Sphase = 2, Sbase = Spa[Si6];
			}
		}
	} else if(ms==3) {													// <<<ϳ����t���>>>
		Sphase = 1;
	} else if(ms==0) {													// <<<��������>>>
		if(Sphase==0) {
			if(m_Char=='P') {											// P�������͊�_
				Si1 = 0;
			} else if(m_Char=='L') {									// L�������͊��
				Si1 = 1;
			} else {
				CBasesetDlg dlg;
				if(dlg.DoModal()==IDOK) {								// �_/���I���޲�۸ޕ\��
					dlg.GetValue(&Si1);									// ��_/���I��
				} else {
					ExitMenu(0, 0);										// �ƭ��I��
				}
			}
			if(m_ActMenu!=NONE) {										// <���~�łȂ��ꍇ>
				if(Si1==0) {											// ��_�̏ꍇ
					Si2 = 1;
					ScrToPnt(0, &m_Bpoint, &Spa[0]);
					Si3 = m_BpDisp, m_BpDisp = 1;						// ��_�\����ԕۑ�
					m_Cent3D = m_Bpoint;								// ��_��3D���S�_
				} else {												// ����̏ꍇ
					Si2 = 2;
					ScrToPnt(0, &m_Blspnt, &Spa[0]);
					ScrToPnt(0, &m_Blepnt, &Spa[1]);
					Si3 = m_BlDisp, m_BlDisp = 1;						// ����\����ԕۑ�
					box.bp[0] = Spa[0], box.bp[1] = Spa[1];				// ����n�I�_��BOX
					if(m_Blspnt.p[0]>m_Blepnt.p[0]) {
						box.bp[0].p[0] = Spa[1].p[0];
					}
					if(m_Blspnt.p[1]>m_Blepnt.p[1]) {
						box.bp[0].p[1] = Spa[1].p[1];
					}
					if(m_Blspnt.p[2]>m_Blepnt.p[2]) {
						box.bp[0].p[2] = Spa[1].p[2];
					}
					if(m_Blspnt.p[0]<m_Blepnt.p[0]) {
						box.bp[1].p[0] = Spa[1].p[0];
					}
					if(m_Blspnt.p[1]<m_Blepnt.p[1]) {
						box.bp[1].p[1] = Spa[1].p[1];
					}
					if(m_Blspnt.p[2]<m_Blepnt.p[2]) {
						box.bp[1].p[2] = Spa[1].p[2];
					}
					m_Fovy=(int)((BoxCenter(box, &cpt)*1000.0)/0.8+0.5);// �ޯ���̾���&�ő啝�擾
					ScrToPnt(1, &m_Cent3D, &cpt);
				}
				Si4 = m_BpBind, m_BpBind = 0;							// ��_�S����ԕۑ�
				Si5 = m_BlBind, m_BlBind = 0;							// ����S����ԕۑ�
				Sia[0] = 4, Sia[1] = 4;
				Tracker1.Setup(Spa, Sia, NULL, Si2, 5, 0);				// �ׯ��1�ݒ�
				Sphase = 1;
				m_pState->SetCent3D(cpt);
				SetSeishaei(m_Cent3D, m_Fovy);							// ���ˉe�s��̍č\�z
				ChangeTexPixels();										// ø����߸�ٍX�V
				if(m_GridDisp) {
					SetGrid();											// ��د�ސݒ�
				}
				Display(0);												// �ĕ`��
				m_CursorNo = 1;											// ����No�ݒ�
			}
		}
	} else if(ms==4||ms==9) {											// <<<ϳ��E�t����/�I��>>>
		if(Si1==0) {
			m_BpDisp = Si3;												// ��_/���\����Ԃ�߂�
		} else {
			m_BlDisp = Si3;
		}
		m_BpBind = Si4, m_BlBind = Si5;									// ��_/���S����Ԃ�߂�
		Tracker1.Reset(1);												// �ׯ��1����
		Display(0);														// �ĕ`��
		ExitMenu(0, 0);													// �ƭ��I��
	}
}

/**********************************/
void CJcad3GlbView::CentPars(void)										//<<< �Q�Ɠ_�w��(�����})
/**********************************/
{
	VECTYPE v1, v2={0.0, 0.0, -1.0};
	PNTTYPE ept, cpt;

	m_EyeSv = m_Eye;													// �J�n���_�ʒu
	m_CentPs = m_Cent3D;												// 3�ʐ}���S�_�𓧎��}�Q�Ɠ_
	ScrToPnt(0, &m_Eye, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	m_TruckLen = PntD(&ept, &cpt)*0.2;									// ���_���Q�Ɠ_����
	m_TruckLen = (m_TruckLen<0.1) ? 0.1 : m_TruckLen;
	VecV(&ept, &cpt, &v1);												// �����޸�َ擾
	VecN(&v1);															// <���_�޸�ق�Z���Ɠ�>
	if(fabs(v1.p[0])<EPSILON&&
       fabs(v1.p[1])<EPSILON&&
       fabs(v1.p[2])-1.0<EPSILON) {
		m_HzVec.p[0] = 0.0, m_HzVec.p[1] = -1.0, m_HzVec.p[2] = 0.0;
	} else {															// <���_�޸�ق�Z���ƈ�>
		VecO(&v1, &v2, &m_HzVec);										// �������޸�َ擾
		VecN(&m_HzVec);
	}
	VecO(&v1, &m_HzVec, &m_VtVec);										// �c�����޸�َ擾
	VecN(&m_VtVec);
	m_pState->SetCentPRS(cpt);
	SetPerspective();													// �����@�ˉe�s��̍č\�z
	SetXYZGuide();														// XYZ�޲�ސݒ�
	Display(2);															// �ĕ`��
}

/*********************************/
void CJcad3GlbView::TruckUp(void)										//<<< �ׯ������(�����})
/*********************************/
{
	PNTTYPE ept, cpt;
	VECTYPE vec;
	SCRTYPE len;
	int     i, ln, sa;

	ScrToPnt(0, &m_EyeSv, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	VecV(&ept, &cpt, &vec);												// �J�n�����޸�َ擾
	VecN(&vec);
	for(i=0; i<DIM; i++) {
		ln = (int)(vec.p[i]*m_TruckLen*1000.0);
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln>=0.5) ? 1 : 0;			// �덷�̕␳
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln<=-0.5) ? -1 : sa;
		len.p[i] = ln+sa;												// ���_�̈ړ������Z�o
	}
	for(i=0; i<DIM; i++) {
		m_Eye.p[i] = m_Eye.p[i]+len.p[i];								// ���_���ړ�
	}
	if(m_Eye.p[0]==m_Cent3D.p[0]&&
       m_Eye.p[1]==m_Cent3D.p[1]&&
       m_Eye.p[2]==m_Cent3D.p[2]) {
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] = m_Eye.p[i]+len.p[i];
		}
	}
	ScrToPnt(0, &m_Eye, &ept);
	m_pState->SetEye(ept);
	SetPerspective();													// �����@�ˉe�s��̍č\�z
	SetXYZGuide();														// XYZ�޲�ސݒ�
	Display(2);															// �ĕ`��
}

/***********************************/
void CJcad3GlbView::TruckDown(void)										//<<< �ׯ���޳�(�����})
/***********************************/
{
	PNTTYPE ept, cpt;
	VECTYPE vec;
	SCRTYPE len;
	int     i, ln, sa;

	ScrToPnt(0, &m_EyeSv, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	VecV(&ept, &cpt, &vec);												// �J�n�����޸�َ擾
	VecN(&vec);
	for(i=0; i<DIM; i++) {
		ln = (int)(vec.p[i]*m_TruckLen*1000.0);
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln>=0.5) ? 1 : 0;			// �덷�̕␳
		sa = ((vec.p[i]*m_TruckLen*1000.0)-ln<=-0.5) ? -1 : sa;
		len.p[i] = ln+sa;												// ���_�̈ړ������Z�o
	}
	for(i=0; i<DIM; i++) {
		m_Eye.p[i] = m_Eye.p[i]-len.p[i];								// ���_���ړ�
	}
	if(m_Eye.p[0]==m_Cent3D.p[0]&&
       m_Eye.p[1]==m_Cent3D.p[1]&&
       m_Eye.p[2]==m_Cent3D.p[2]) {
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] = m_Eye.p[i]-len.p[i];
		}
	}
	ScrToPnt(0, &m_Eye, &ept);
	m_pState->SetEye(ept);
	SetPerspective();													// �����@�ˉe�s��̍č\�z
	SetXYZGuide();														// XYZ�޲�ސݒ�
	Display(2);															// �ĕ`��
}

/***********************************/
void CJcad3GlbView::EyeSet(int flg)										//<<< �����}�ύX
/***********************************/
{
	PNTTYPE ept, cpt;
	VECTYPE v1, v2={0.0, 0.0, -1.0};
	double  len;

	ScrToPnt(0, &m_Eye, &ept);
	ScrToPnt(0, &m_CentPs, &cpt);
	len = PntD(&ept, &cpt);												// ���_���Q�Ɠ_����
	m_Eye = m_CentPs;													// ���_���Q�Ɠ_�ɍ��킹��
	if(flg==0) {														// <XY����(�\)>
		m_Eye.p[2] += (int)(len*1000.0);
	} else if(flg==1) {													// <YZ����(�\)>
		m_Eye.p[0] += (int)(len*1000.0);
	} else if(flg==2) {													// <ZX����(�\)>
		m_Eye.p[1] += (int)(len*1000.0);
	} else if(flg==3) {													// <XY����(��)>
		m_Eye.p[2] -= (int)(len*1000.0);
	} else if(flg==4) {													// <YZ����(��)>
		m_Eye.p[0] -= (int)(len*1000.0);
	} else {															// <ZX����(��)>
		m_Eye.p[1] -= (int)(len*1000.0);
	}
	m_EyeSv = m_Eye;													// �J�n���_�ʒu
	ScrToPnt(0, &m_Eye, &ept);
	m_TruckLen = PntD(&ept, &cpt)*0.2;									// ���_���Q�Ɠ_����X0.2
	m_TruckLen = (m_TruckLen<0.1) ? 0.1 : m_TruckLen;
	VecV(&ept, &cpt, &v1);												// �����޸�َ擾
	VecN(&v1);															// <���_�޸�ق�Z���Ɠ�>
	if(fabs(v1.p[0])<EPSILON&&
       fabs(v1.p[1])<EPSILON&&
       fabs(v1.p[2])-1.0<EPSILON) {
		m_HzVec.p[0] = 0.0, m_HzVec.p[1] = -1.0, m_HzVec.p[2] = 0.0;
	} else {															// <���_�޸�ق�Z���ƈ�>
		VecO(&v1, &v2, &m_HzVec);										// �������޸�َ擾
		VecN(&m_HzVec);
	}
	VecO(&v1, &m_HzVec, &m_VtVec);										// �c�����޸�َ擾
	VecN(&m_VtVec);
	m_pState->SetEye(ept);
	SetPerspective();													// �����@�ˉe�s��̍č\�z
	SetXYZGuide();														// XYZ�޲�ސݒ�
	Display(2);															// �ĕ`��
}

/************************************************************/
void CJcad3GlbView::EyeChange(UINT flg, const CPoint& point,
                              int plane, int ms)						//<<< ���_�ύX
/************************************************************/
{
	PNTTYPE ept;
	CPoint  npoint(point);
	double  hzth, vtth;
	int     sax, say;

	if(plane!=PRS) {
		return;
	}
	if(ms==2&&flg&MK_LBUTTON&&Sphase2==1) {								// ϳ��̍��t�������̂ݗL��
		if(flg&m_LimKey) {
			MouseBind(npoint, BasePoint);								// ��]�����S��
			m_Eye = SscrA, m_EyeSv = SscrB;
			m_VtVec = SvecA, m_HzVec = SvecB;
			sax = npoint.x - BasePoint.x;								// ��_�ƌ��ݓ_�Ƃ̍�
			say = BasePoint.y - npoint.y;
		} else {
			sax = npoint.x - Spoint.x;									// 1�_�O�ƌ��ݓ_�Ƃ̍�
			say = Spoint.y - npoint.y;
		}
		Spoint = npoint;
		hzth = -(PI*2.0*sax/PlRect[PRS].Width());						// ����������]�p�Z�o
		vtth = -(PI*2.0*say/PlRect[PRS].Height());						// ����������]�p�Z�o
		EyeMove(0, hzth);
		EyeMove(1, vtth);
		ScrToPnt(0, &m_Eye, &ept);
		m_pState->SetEye(ept);
		SetPerspective();												// �����@�ˉe�s��̍č\�z
		SetXYZGuide();													// XYZ�޲�ސݒ�
		Display(2);														// �ĕ`��
	} else if(ms==1) {													// ϳ����t����
		Spoint = point;
		SscrA = m_Eye, SscrB = m_EyeSv;
		SvecA = m_VtVec, SvecB = m_HzVec;
		Sphase2 = 1;
	} else if(ms==3) {													// ϳ����t���
		Sphase2 = 0;
	}
}

/*************************************************/
void CJcad3GlbView::EyeChange(int dir, double th)						//<<< ���_��]
/*************************************************/						// dir:0�����C1���c
{
	PNTTYPE ept;

	EyeMove(dir, th);
	ScrToPnt(0, &m_Eye, &ept);
	m_pState->SetEye(ept);
	SetPerspective();													// �����@�ˉe�s��̍č\�z
	SetXYZGuide();														// XYZ�޲�ސݒ�
	Display(2);															// �ĕ`��
}

/***********************************************/
void CJcad3GlbView::EyeMove(int dir, double th)							//<<< ���_�ړ�
/***********************************************/						// dir:0�����C1���c
{
	PNTTYPE pt1, pt2;
	VECTYPE v1;
	int     i;
	double  rot[3][3];

	if(dir==1) {
		SetRotateMatrix(m_HzVec, th, rot);								// �c������]�s����擾
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] -= m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] -= m_CentPs.p[i];
		}
		ScrToPnt(0, &m_Eye, &pt1);
		ScrToPnt(0, &m_EyeSv, &pt2);
		VecRotate(&pt1, rot);											// ���_��]
		VecRotate(&pt2, rot);											// �J�n���_��]
		ScrToPnt(1, &m_Eye, &pt1);
		ScrToPnt(1, &m_EyeSv, &pt2);
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] += m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] += m_CentPs.p[i];
		}
		ScrToPnt(0, &m_EyeSv, &pt1);
		ScrToPnt(0, &m_CentPs, &pt2);
		VecV(&pt1, &pt2, &v1);											// �����޸�َ擾
		VecN(&v1);
		VecO(&v1, &m_HzVec, &m_VtVec);									// �c�����޸�َ擾
		VecN(&m_VtVec);
	} else {
		SetRotateMatrix(m_VtVec, th, rot);								// ��������]�s��擾
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] -= m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] -= m_CentPs.p[i];
		}
		ScrToPnt(0, &m_Eye, &pt1);
		ScrToPnt(0, &m_EyeSv, &pt2);
		VecRotate(&pt1, rot);											// ���_��]
		VecRotate(&pt2, rot);											// �J�n���_��]
		ScrToPnt(1, &m_Eye, &pt1);
		ScrToPnt(1, &m_EyeSv, &pt2);
		for(i=0; i<DIM; i++) {
			m_Eye.p[i] += m_CentPs.p[i];
		}
		for(i=0; i<DIM; i++) {
			m_EyeSv.p[i] += m_CentPs.p[i];
		}
		ScrToPnt(0, &m_EyeSv, &pt1);
		ScrToPnt(0, &m_CentPs, &pt2);
		VecV(&pt1, &pt2, &v1);											// �����޸�َ擾
		VecN(&v1);
		VecO(&m_VtVec, &v1, &m_HzVec);									// �������޸�َ擾
		VecN(&m_HzVec);
	}
}

/*******************************************/
void CJcad3GlbView::SetHomePosition(int no)								//<<< ��ʏ�ԕۑ�
/*******************************************/
{
	CString str;
	int     i, j, n;

	str.Format(IDS_SAVE_SCRN, no);
	if(AfxMessageBox(str, MB_YESNO)==IDYES) {							// <�ۑ�OK>
		n = no - 1;
		m_Cent3D_S[n]    = m_Cent3D;									// ��ʕ\����ԕۑ�
		m_Fovy_S[n]      = m_Fovy;
		m_CentPs_S[n]    = m_CentPs;
		m_Eye_S[n]       = m_Eye;
		m_EyeSv_S[n]     = m_EyeSv;
		m_TruckLen_S[n]  = m_TruckLen;
		m_HzVec_S[n]     = m_HzVec;
		m_VtVec_S[n]     = m_VtVec;
		m_GridDisp_S[n]  = m_GridDisp;
		m_GridBind_S[n]  = m_GridBind;
		m_GridSpace_S[n] = m_GridSpace;
		m_GridIntvl_S[n] = m_GridIntvl;
		m_Bpoint_S[n]    = m_Bpoint;
		m_Blspnt_S[n]    = m_Blspnt;
		m_Blepnt_S[n]    = m_Blepnt;
		m_BpDisp_S[n]    = m_BpDisp;
		m_BpBind_S[n]    = m_BpBind;
		m_BlDisp_S[n]    = m_BlDisp;
		m_BlBind_S[n]    = m_BlBind;
		m_GlbDisp_S[n]   = m_GlbDisp;
		m_GlbChange_S[n] = m_GlbChange;
		m_DspTmpNo_S[n]  = m_DspTmpNo;
		for(i=0; i<8; i++) {
			for(j=0; j<3; j++) {
				m_TempPath_S[n][i][j] = m_TempPath[i][j];
			}
		}
	}
}

/***********************************************/
void CJcad3GlbView::RestoreHomePosition(int no)							//<<< ��ʏ�ԕ��A
/***********************************************/
{
	PNTTYPE p1, p2;
	VECTYPE v1, v2={0.0, 0.0, -1.0};
	int     i, j, n, hm;

	if(no>0) {															// <�ƭ�����N���̏ꍇ>
		hm = 1, n = no - 1;
	} else {															// <�V�K�N���̏ꍇ>
		hm = 0, n = 0;
	}
	if(hm||(m_ScrEnv2&0x1)) {											// <Home���A>
		m_Cent3D = m_Cent3D_S[n];										// 3�ʐ}�\����ԕ��A
		m_Fovy   = m_Fovy_S[n];
	} else if(!(m_ScrEnv1&0x1)) {										// <�V�K>
		m_Cent3D.p[0]=SCT_X, m_Cent3D.p[1]=SCT_Y, m_Cent3D.p[2]=SCT_Z;	// 3�ʐ}�\����ԏ�����
		m_Fovy   = FOVY;													
	}
	if(hm||(m_ScrEnv2&0x2)) {											// <Home���A>
		m_CentPs   = m_CentPs_S[n];										// �����}�\����ԕ��A
		m_Eye      = m_Eye_S[n];
		m_EyeSv    = m_EyeSv_S[n];
		m_TruckLen = m_TruckLen_S[n];
		m_HzVec    = m_HzVec_S[n];
		m_VtVec    = m_VtVec_S[n];
	} else if(!(m_ScrEnv1&0x2)) {										// <�V�K>
		m_CentPs.p[0]=SCT_X, m_CentPs.p[1]=SCT_Y, m_CentPs.p[2]=SCT_Z;	// �����}�\����ԏ�����
		m_Eye.p[0] = EYE_X, m_Eye.p[1] = EYE_Y, m_Eye.p[2] = EYE_Z;
		m_EyeSv.p[0]=EYE_X, m_EyeSv.p[1]=EYE_Y, m_EyeSv.p[2]=EYE_Z;
		m_TruckLen = TRLEN;
		ScrToPnt(0, &m_EyeSv, &p1);
		ScrToPnt(0, &m_CentPs, &p2);
		VecV(&p1, &p2, &v1);
		VecN(&v1);
		if(fabs(v1.p[0])<EPSILON&&
           fabs(v1.p[1])<EPSILON&&
           fabs(v1.p[2])-1.0<EPSILON) {
			m_HzVec.p[0] = 0.0, m_HzVec.p[1] = -1.0, m_HzVec.p[2] = 0.0;
		} else {
			VecO(&v1, &v2, &m_HzVec);
			VecN(&m_HzVec);
		}
		VecO(&v1, &m_HzVec, &m_VtVec);
		VecN(&m_VtVec);
	}
	if(hm||(m_ScrEnv2&0x4)) {											// <Home���A>
		m_GridDisp  = m_GridDisp_S[n];									// ��د�ޏ�ԕ��A
		m_GridBind  = m_GridBind_S[n];
		m_GridSpace = m_GridSpace_S[n];
		m_GridIntvl = m_GridIntvl_S[n];
	} else if(!(m_ScrEnv1&0x4)) {										// <�V�K>
		m_GridDisp  = 0;												// ��د�ޏ�ԏ�����
		m_GridBind  = 0;
		m_GridSpace = GRSPC;
		m_GridIntvl = GRINT;
	}
	if(hm||(m_ScrEnv2&0x10)) {											// <Home���A>
		m_BpDisp = m_BpDisp_S[n];										// ��_��ԕ��A
		m_BpBind = m_BpBind_S[n];
		m_Bpoint = m_Bpoint_S[n];
		m_BlDisp = m_BlDisp_S[n];										// �����ԕ��A
		m_BlBind = m_BlBind_S[n];
		m_Blspnt = m_Blspnt_S[n];
		m_Blepnt = m_Blepnt_S[n];
	} else if(!(m_ScrEnv1&0x10)) {										// <�V�K>
		m_BpDisp = 0;													// ��_��ԏ�����
		m_BpBind = 0;
		m_BlDisp = 0;													// �����ԏ�����
		m_BlBind = 0;
	}
	ScrToPnt(0, &m_Cent3D, &p1);
	m_pState->SetCent3D(p1);											// ��ʏ�ԕ\���ύX
	ScrToPnt(0, &m_Eye, &p1);
	m_pState->SetEye(p1);
	ScrToPnt(0, &m_CentPs, &p1);
	m_pState->SetCentPRS(p1);
	SetPerspective();													// �����@�ˉe�s��̍č\�z
	SetSeishaei(m_Cent3D, m_Fovy);										// ���ˉe�s��̍č\�z
	SetXYZGuide();														// XYZ�޲�ސݒ�
	SetGrid();															// ��د�ސݒ�
	SetBase();															// ��_/���ݒ�
	ChangeTexPixels();													// ø����߸�ٍX�V
	if(hm||(m_ScrEnv2&0x8)) {											// <Home���A>
		m_GlbDisp   = m_GlbDisp_S[n];									// GLB�\����ԕ��A
		m_GlbChange = m_GlbChange_S[n];
	} else if(!(m_ScrEnv1&0x8)) {										// <�V�K>
		m_GlbDisp   = 0;												// GLB�\����ԏ�����
		m_GlbChange = 0;
	}
	if(ReadGLB()!=TRUE) {												// GLB����
		SetSplashMsg(IDS_CANT_READ_FILE);								// ��ް�̧�ٓǍ�NG�
		DeleteGLB();													// GLB�j��
		m_GlbDisp = 0;
	}
	if(hm||(m_ScrEnv2&0x8)) {											// <Home���A>
		m_DspTmpNo = m_DspTmpNo_S[n];									// ����ڰĕ\����ԕ��A
		for(i=0; i<8; i++) {
			for(j=0; j<3; j++) {
				m_TempPath[i][j] = m_TempPath_S[n][i][j];
			}
		}
	} else if(!(m_ScrEnv1&0x8)) {										// <�V�K>
		m_DspTmpNo = 0;													// ����ڰĕ\����ԏ�����
	}
	if(ReadTEMP()!=TRUE) {												// ����ڰē���
		SetSplashMsg(IDS_CANT_READ_FILE);								// ��ް�̧�ق�Ǎ�NG�
		View->DeleteTEMP();												// ����ڰĔj��
		m_DspTmpNo = 0;
	}
	Display(0);															// �ĕ`��
}

/********************************************/
void CJcad3GlbView::GetCentPnt(PNTTYPE* pnt)							//<<< ��ʾ����擾
/********************************************/
{
	ScrToPnt(0, &m_Cent3D, pnt);										// SCRTYPE => PNTTYPE
}

/*******************************************/
void CJcad3GlbView::GetEyePnt(PNTTYPE* pnt)								//<<< ���_�擾
/*******************************************/
{
	ScrToPnt(0, &m_Eye, pnt);                                           // SCRTYPE => PNTTYPE
}

/*********************************************/
void CJcad3GlbView::GetEyeSvPnt(PNTTYPE* pnt)							//<<< ���_SV�擾
/*********************************************/
{
	ScrToPnt(0, &m_EyeSv, pnt);                                         // SCRTYPE => PNTTYPE
}

/**********************************************/
void CJcad3GlbView::GetCentPsPnt(PNTTYPE* pnt)							//<<< �Q�Ɠ_�擾
/**********************************************/
{
	ScrToPnt(0, &m_CentPs, pnt);                                        // SCRTYPE => PNTTYPE
}
