/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewMenu.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "ButtonDlg.h"
#include "InputsDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "NumFunc.h"
#include "LCheck.h"

/****************************************************/
int CJcad3GlbView::HitTestPlane(const CPoint& point)					//<<< ���ʈʒu�m�F
/****************************************************/
{
	if(PlRect[XY].PtInRect(point)) {
		return XY;
	}
	if(PlRect[YZ].PtInRect(point)) {
		return YZ;
	}
	if(PlRect[ZX].PtInRect(point)) {
		return ZX;
	}
	if(PlRect[PRS].PtInRect(point)) {
		return PRS;
	}
	return -1;
}

/***************************************************************/
void CJcad3GlbView::GetViewport(int plane,
                                int* x, int* y, int* w, int* h)			//<<< �ޭ��߰Ď擾
/***************************************************************/
{
	*x = PlRect[plane].left;
	*y = ViewSize.cy-PlRect[plane].bottom-1;
	*w = PlRect[plane].Width();
	*h = PlRect[plane].Height();
}

/*******************************************************************/
BOOL CJcad3GlbView::CalcScreenWorld(int bind, PNTTYPE* pnt,
                                    const CPoint& point, int plane)		//<<< Screen->World���W�ϊ�
/*******************************************************************/
{
	CPoint  cp;
	CSize   dist;
	PNTTYPE spt, bp1, bp2, bp3;
	BOOL    ret=TRUE;
	int     grid, bpoint, bline, id;
	double  ln0, ln1, ln2, ln3;

	if(bind==1) {														// <��د�ލS��OFF�w�莞>
		grid = m_GridBind, m_GridBind = 0;								// �S����ԕۑ����S��
	} else if(bind==2) {												// <��د�ލS��ON�w�莞>
		grid = m_GridBind, bpoint = m_BpBind, bline = m_BlBind;			// �S����ԕۑ�
		m_GridBind = 1, m_BpBind = 0, m_BlBind = 0;						// ��د�ޕ��ړ�
	}
	if(plane==XY) {
		cp = CPoint((PlRect[XY].left+PlRect[XY].right)/2,
					(PlRect[XY].top+PlRect[XY].bottom)/2);
		dist = point - cp;												// XY���ʒ��S���w��_�̋���
		pnt->p[0] = (m_Cent3D.p[0]+(dist.cy*m_Fovy/PlSize))/1000.0;
		pnt->p[1] = (m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize))/1000.0;
		pnt->p[2] = m_Cent3D.p[2]/1000.0;
		spt = *pnt;
		if(m_GridBind) {												// ��د�ލS��
			id = m_Cent3D.p[0]+(dist.cy*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[0] = (id*m_GridSpace)/1000.0;
			id = m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[1] = (id*m_GridSpace)/1000.0;
			if(m_BpBind) {												// ��_�S��
				ScrToPnt(0, &m_Bpoint, &bp1);							// ��_�ʒu
				bp1.p[2] = m_Cent3D.p[2]/1000.0;
			}
			if(m_BlBind) {												// ����S��
				ScrToPnt(0, &m_Blspnt, &bp2);							// ����n�_�ʒu
				bp2.p[2] = m_Cent3D.p[2]/1000.0;
				ScrToPnt(0, &m_Blepnt, &bp3);							// ����I�_�ʒu
				bp3.p[2] = m_Cent3D.p[2]/1000.0;
			}
		}
	} else if(plane==YZ) {
		cp = CPoint((PlRect[YZ].left+PlRect[YZ].right)/2,
					(PlRect[YZ].top+PlRect[YZ].bottom)/2);
		dist = point - cp;												// YZ���ʒ��S���w��_�̋���
		pnt->p[1] = (m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize))/1000.0;
		pnt->p[2] = (m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize))/1000.0;
		pnt->p[0] = m_Cent3D.p[0]/1000.0;
		spt = *pnt;
		if(m_GridBind) {												// ��د�ލS��
			id = m_Cent3D.p[1]+(dist.cx*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[1] = (id*m_GridSpace)/1000.0;
			id = m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[2] = (id*m_GridSpace)/1000.0;
			if(m_BpBind) {												// ��_�S��
				ScrToPnt(0, &m_Bpoint, &bp1);							// ��_�ʒu
				bp1.p[0] = m_Cent3D.p[0]/1000.0;
			}
			if(m_BlBind) {												// ����S��
				ScrToPnt(0, &m_Blspnt, &bp2);							// ����n�_�ʒu
				bp2.p[0] = m_Cent3D.p[0]/1000.0;
				ScrToPnt(0, &m_Blepnt, &bp3);							// ����I�_�ʒu
				bp3.p[0] = m_Cent3D.p[0]/1000.0;
			}
		}
	} else if(plane==ZX) {
		cp = CPoint((PlRect[ZX].left+PlRect[ZX].right)/2,
					(PlRect[ZX].top+PlRect[ZX].bottom)/2);
		dist = point - cp;												// ZX���ʒ��S���w��_�̋���
		pnt->p[2] = (m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize))/1000.0;
		pnt->p[0] = (m_Cent3D.p[0]-(dist.cx*m_Fovy/PlSize))/1000.0;
		pnt->p[1] = m_Cent3D.p[1]/1000.0;
		spt = *pnt;
		if(m_GridBind) {												// ��د�ލS��
			id = m_Cent3D.p[2]-(dist.cy*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[2] = (id*m_GridSpace)/1000.0;
			id = m_Cent3D.p[0]-(dist.cx*m_Fovy/PlSize);
			id = (int)((double)id/m_GridSpace+(id<0?-0.5:0.5));
			pnt->p[0] = (id*m_GridSpace)/1000.0;
			if(m_BpBind) {												// ��_�S��
				ScrToPnt(0, &m_Bpoint, &bp1);							// ��_�ʒu
				bp1.p[1] = m_Cent3D.p[1]/1000.0;
			}
			if(m_BlBind) {												// ����S��
				ScrToPnt(0, &m_Blspnt, &bp2);							// ����n�_�ʒu
				bp2.p[1] = m_Cent3D.p[1]/1000.0;
				ScrToPnt(0, &m_Blepnt, &bp3);							// ����I�_�ʒu
				bp3.p[1] = m_Cent3D.p[1]/1000.0;
			}
		}
	} else {
		ret = FALSE;
	}
	if(ret&&m_GridBind) {												// �����د�ލS����
		if(m_BpBind||m_BlBind) {										// ��_/���S���L
			ln0 = PntD(&spt, pnt);										// �د��_-��د�ޓ_����
			ln1 = (m_BpBind) ? PntD(&spt, &bp1) : DBL_MAX;				// �د��_-��_����
			ln2 = (m_BlBind) ? PntD(&spt, &bp2) : DBL_MAX;				// �د��_-�������
			ln3 = (m_BlBind) ? PntD(&spt, &bp3) : DBL_MAX;
			if(ln0<ln1) {												// ln0-ln3�̍ŏ�����
				id = 0;
			} else {
				id = 1, ln0 = ln1;
			}
			if(ln0>ln2) {
				id = 2, ln0 = ln2;
			}
			if(ln0>ln3) {
				id = 3;
			}
			if(id==1) {													// �ŏ��̍��W��ݒ�
				*pnt = bp1;
			} else if(id==2) {
				*pnt = bp2;
			} else if(id==3) {
				*pnt = bp3;
			}
		}
	}
	if(bind==1) {														// <��د�ލS��OFF�w�莞>
		m_GridBind = grid;												// �S����ԕ���
	} else if(bind==2) {												// <��د�ލS��ON�w�莞>
		m_GridBind = grid, m_BpBind = bpoint, m_BlBind = bline;			// �S����ԕ���
	}
	return ret;
}

/******************************************************************/
BOOL CJcad3GlbView::CalcWorldScreen(CPoint* point,
                                    const PNTTYPE& pnt, int plane)		//<<< World->Screen���W�ϊ�
/******************************************************************/
{
	CPoint cp;
	CSize  dist;
	BOOL   ret=TRUE;

	if(plane==XY) {
		cp = CPoint((PlRect[XY].left+PlRect[XY].right)/2,
					(PlRect[XY].top+PlRect[XY].bottom)/2);
		dist.cy = ((int)(pnt.p[0]*1000.0)-m_Cent3D.p[0])*PlSize/m_Fovy;	// 3D�ϊ�
		dist.cx = ((int)(pnt.p[1]*1000.0)-m_Cent3D.p[1])*PlSize/m_Fovy;
		*point = dist+cp;
	} else if(plane==YZ) {
		cp = CPoint((PlRect[YZ].left+PlRect[YZ].right)/2,
					(PlRect[YZ].top+PlRect[YZ].bottom)/2);
		dist.cx = ((int)(pnt.p[1]*1000.0)-m_Cent3D.p[1])*PlSize/m_Fovy;	// 3D�ϊ�
		dist.cy = (m_Cent3D.p[2]-(int)(pnt.p[2]*1000.0))*PlSize/m_Fovy;
		*point = dist+cp;
	} else if(plane==ZX) {
		cp = CPoint((PlRect[ZX].left+PlRect[ZX].right)/2,
					(PlRect[ZX].top+PlRect[ZX].bottom)/2);
		dist.cy = (m_Cent3D.p[2]-(int)(pnt.p[2]*1000.0))*PlSize/m_Fovy;	// 3D�ϊ�
		dist.cx = (m_Cent3D.p[0]-(int)(pnt.p[0]*1000.0))*PlSize/m_Fovy;
		*point = dist+cp;
	} else {
		ret = FALSE;
	}
	return ret;
}

/*****************************************************/
void CJcad3GlbView::LimitValue(int plane, double* dd)					//<<< ϳ����͐����l
/*****************************************************/
{
	int    i1, i2;
	double d1, d2;

	i1 = plane;															// �w�蕽�ʗL�����W��
	i2 = (plane+1>2) ? 0 : plane+1;
	d1 = dd[i1]/dd[i2];													// ���������̍��W����
	d2 = dd[i2]/dd[i1];													// �΂���䗦
	if(fabs(d1)>1.732) {												// <i1���ɋ߂�(30�x�ȓ�)>
		dd[i2] = 0.0;
	} else if(fabs(d2)>1.732) {											// <i2���ɋ߂�(30�x�ȓ�)>
		dd[i1] = 0.0;
	} else {															// <i1,i2���̒���>
		if(fabs(dd[i1])>fabs(dd[i2])) {									// �傫�����̍��W���̗p
			dd[i2] = (dd[i2]>0) ? fabs(dd[i1]) : -fabs(dd[i1]);
		} else {
			dd[i1] = (dd[i1]>0) ? fabs(dd[i2]) : -fabs(dd[i2]);
		}
	}
}

/*******************************************************************/
BOOL CJcad3GlbView::LimitPoint(int plane, PNTTYPE* pa, PNTTYPE* pb)		//<<< ϳ����͐������W
/*******************************************************************/
{
	int    i, i1, i2;
	double dd[DIM], d1, d2;
	BOOL   ret=FALSE;

	i1 = plane;															// �w�蕽�ʗL�����W��
	i2 = (plane+1>2) ? 0 : plane+1;
	for(i=0; i<DIM; i++) {
		dd[i] = pa->p[i]-pb->p[i];										// �O�_�Ƃ̌덷�Z�o
	}
	d1 = dd[i1]/dd[i2];													// ���������̍��W����
	d2 = dd[i2]/dd[i1];													// �΂���䗦
	if(fabs(d1)>1.732) {												// <i1���ɋ߂�(30�x�ȓ�)>
		dd[i2] = 0.0;
	} else if(fabs(d2)>1.732) {											// <i2���ɋ߂�(30�x�ȓ�)>
		dd[i1] = 0.0;
	} else {															// <i1,i2���̒���>
		if(fabs(dd[i1])>fabs(dd[i2])) {									// �傫�����̍��W���̗p
			dd[i2] = (dd[i2]>0) ? fabs(dd[i1]) : -fabs(dd[i1]);
		} else {
			dd[i1] = (dd[i1]>0) ? fabs(dd[i2]) : -fabs(dd[i2]);
		}
		ret = TRUE;
	}
	for(i=0; i<DIM; i++) {
		pa->p[i] = dd[i]+pb->p[i];										// ����_����
	}
	return ret;
}

/******************************************************/
void CJcad3GlbView::LimitAngle(int plane, PNTTYPE* pa,
                               PNTTYPE* pb, double th)					//<<< ϳ��p�x�w�萧��
/******************************************************/
{
	int    i, i1, i2;
	double dd[DIM], d1, d2;

	i1 = plane;															// �w�蕽�ʗL�����W��
	i2 = (plane+1>2) ? 0 : plane+1;
	for(i=0; i<DIM; i++) {
		dd[i] = pa->p[i]-pb->p[i];										// �O�_�Ƃ̌덷�Z�o
	}
	d1 = cos(th);														// d1�������̔䗦
	d2 = sin(th);														// d2�������̔䗦
	if(fabs(dd[i1])>fabs(dd[i2])) {										// ϳ��ړ�������̗p
		if(fabs(d1)>EPSILON) {
			dd[i2] = dd[i1]*d2/d1;
		} else {
			dd[i1] = dd[i2]*d1/d2;
		}
	} else {
		if(fabs(d2)>EPSILON) {
			dd[i1] = dd[i2]*d1/d2;
		} else {
			dd[i2] = dd[i1]*d2/d1;
		}
	}
	for(i=0; i<DIM; i++) {
		pa->p[i] = dd[i]+pb->p[i];										// ����_����
	}
}

/************************************/
int CJcad3GlbView::CountEdtObj(void)									//<<< �ҏW�Ώۗ��̶���
/************************************/
{
	OBJTYPE* op;
	int      dsp, edt, cnt1=0, cnt2=0;

	BaseObjPtr(1, &op, &dsp, &edt);										// �擪���̂��߲��(�\���Ώ�)
	while(op) {															// �w�藧�̂����݂���
		cnt1++;
		NextObjPtr(1, &op, &dsp, &edt);
	}
	m_DspCount = cnt1;
	BaseObjPtr(2, &op, &dsp, &edt);										// �擪���̂��߲��(�ҏW�Ώ�)
	while(op) {															// �w�藧�̂����݂���
		cnt2++;
		NextObjPtr(2, &op, &dsp, &edt);
	}
	m_EdtCount = cnt2;
	return cnt2;
}

/****************************************/
void CJcad3GlbView::CheckEdtObjPrm(void)								//<<< �ҏW�Ώۗ��̐������Ұ�����
/****************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, cnt=0, cno, mx;
	double   wt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �擪���̂��߲��(�ҏW�Ώ�)
	while(op) {															// �w�藧�̂����݂���
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		cno = GetCmdNo(cmp);											// �����No�擾
		if(cno==CRTCURVE||cno==CHGCURVE) {								// <�Ȑ�����/�ύX>
			mx = GetParaInt(cmp, 2);									// �Ȑ����J�Ԑ�
			wt = (double)GetParaInt(cmp, 3)/1000.0;						// �Ȑ��d�݌W��
			cnt++; break;
		}
		if(cno==CRTSURF||cno==CHGSURF||									// <�Ȗʐ���/�ҏW>
		   cno==CRTSWEEP||cno==CHGSWEEP) {								// <�|���Ȗʐ���/�ҏW>
			mx = GetParaInt(cmp, 4);									// �Ȑ����J�Ԑ�
			wt = (double)GetParaInt(cmp, 5)/1000.0;						// �Ȑ��d�݌W��
			cnt++; break;
		}
		NextObjPtr(2, &op, &dsp, &edt);
	}
	if(cnt==0) {														// <�ΏەҏW���̂Ȃ�>
		mx = m_CurveRepeat;												// �Ȑ����J�Ԑ��擾
		wt = m_CurveWeight;												// �Ȑ��d�݌W���擾
	}
	MainWnd->SetCrtPrm(mx, wt);											// �������Ұ��ݒ�
}

/*****************************************/
void CJcad3GlbView::InitActMenu(int mode)								//<<< �ƭ���������
/*****************************************/
{
	if(m_ActMenu==M_JWL_GEM) {											// <��ζ�Ă̏ꍇ>
		if(m_Char=='R') {												// R������:׳���
			m_GemNo = 0;
		} else if(m_Char=='O') {										// O������:�����
			m_GemNo = 1;
		} else if(m_Char=='M') {										// M������:ϰ����
			m_GemNo = 2;
		} else if(m_Char=='P') {										// P������:�߱�����
			m_GemNo = 3;
		} else if(m_Char=='H') {										// H������:ʰļ����
			m_GemNo = 4;
		} else if(m_Char=='S') {										// S������:�ݸ��
			m_GemNo = 5;
		} else if(m_Char=='E') {										// E������:������
			m_GemNo = 6;
		} else if(m_Char=='B') {										// B������:�޹ޯ�
			m_GemNo = 7;
		} else if(m_Char=='T') {										// T������:ð�߰��
			m_GemNo = 8;
		} else if(m_Char=='U') {										// U������:۰��
			m_GemNo = 9;
		} else if(m_Char=='C') {										// C������:��޼��
			m_GemNo = 10;
		} else {
			CGemDlg dlg;
			if(dlg.DoModal()==IDOK) {									// �����޲�۸ޕ\��
				dlg.GetValue(&m_GemNo);									// ��Ď�ސݒ�
			} else {
				m_ActMenu = NONE;										// ��ݾق��ƭ����~
				return;
			}
		}
	}
	if(m_ActMenu!=M_SURF_CPS&&m_ActMenu!=M_SURF_P_MOVE&&
       m_ActMenu!=M_SURF_L_MOVE&&m_ActMenu!=M_SURF_L_ROTATE&&
       m_ActMenu!=M_SURF_TWIST&&m_ActMenu!=M_SURF_SCALE&&
       m_ActMenu!=M_SURF_S_MOVE&&m_ActMenu!=M_SURF_B_MOVE&&
       m_ActMenu!=M_SURF_P_ADJUST&&m_ActMenu!=M_SURF_S_ADJUST&&
       m_ActMenu!=M_EDIT_CURVE) {										// ���͗L���ƭ��ȊO��
		m_SurfTension = 0;												// ���͌W��������
	}
	m_InitMenu = mode;
	MenuCall(m_ActMenu, 0, 0, 0, 0);
	m_Char = 0;
}

/*****************************************/
void CJcad3GlbView::ExitActMenu(int menu)								//<<< �ƭ��㏈��
/*****************************************/
{
	int flg=9;

	if(m_SaveMenu!=NONE&&m_ActMenu!=menu) {								// �ۑ��ƭ����I���̏ꍇ
		m_ActMenu = m_SaveMenu;											// �ۑ��ƭ����N���ƭ���
		m_SaveMenu = NONE;
		m_CursorNo_S = 0;
	}
	if(m_ActMenu==M_MEASURE_SCALE||m_ActMenu==M_MEASURE_GRAD) {			// ��K/���x��
		if(m_ActMenu!=menu) {											// ���ƭ��ɂ��I��
			flg = 8;
		}
	}
	MenuCall(m_ActMenu, 0, 0, 0, flg);
}

/********************************************************************/
void CJcad3GlbView::MenuCall(int menu, UINT nflg,
                             const CPoint& point, int plane, int ev)	//<<< �ƭ��Ăяo��
/********************************************************************/
{
	if(GetSelectNo()==1) {
		if(menu!=NONE&&
           menu!=M_SELECT_OBJ&&menu!=M_SCROLL&&
           menu!=M_ZOOM_BOX&&menu!=M_CRT_SETSECTION&&
           menu!=M_EDIT_LINES&&menu!=M_EDIT_LINE&&
           menu!=M_SURF_P_ADJUST&&menu!=M_SURF_S_ADJUST&&
           menu!=M_EDIT_PASTEOBJ) {
			ResetSelNo();
		}
	}
	if(menu==M_SURF_CPS) {
		MouseCPointSpace(nflg, point, plane, ev);						// �ȕ� CPS
	} else if(menu==M_SURF_P_MOVE) {
		MousePointMove(nflg, point, plane, ev);							// �ȕ� ����_�ړ�
	} else if(menu==M_SURF_L_MOVE) {
		MouseLineMove(nflg, point, plane, ev);							// �ȕ� ����_��ړ�
	} else if(menu==M_SURF_L_INST) {
		MouseLineInsert(nflg, point, plane, ev);						// �ȕ� ����_��}��
	} else if(menu==M_SURF_L_ROTATE) {
		MouseLineRotate(nflg, point, plane, ev);						// �ȕ� ����_���]
	} else if(menu==M_SURF_TWIST) {
		MouseLineTwist(nflg, point, plane, ev);							// �ȕ� �Ђ˂�
	} else if(menu==M_SURF_SCALE) {
		MouseLineScale(nflg, point, plane, ev);							// �ȕ� �g��/�k��
	} else if(menu==M_SURF_S_MOVE) {
		MouseSPointMove(nflg, point, plane, ev);						// �ȕ� �f�ʐ���_�ړ�
	} else if(menu==M_SURF_B_MOVE) {
		MouseBPointMove(nflg, point, plane, ev);						// �ȕ� �ް����ړ�
	} else if(menu==M_SURF_P_ADJUST) {
		MouseLineAdjust(nflg, point, plane, ev);						// �ȕ� ����_�Ȑ����킹
	} else if(menu==M_SURF_S_ADJUST) {
		MouseObjectAdjust(nflg, point, plane, ev);						// �ȕ� ����_���̍��킹
	} else if(menu==M_SURF_TENSION) {
		MouseTension(nflg, point, plane, ev);							// �ȕ� ���͌W��
	} else if(menu==M_CRT_CURVE) {
		MouseCurve(nflg, point, plane, ev);								// ���� �Ȑ�����
	} else if(menu==M_EDIT_CURVE) {
		MouseECurve(nflg, point, plane, ev);							// ���� �Ȑ��ҏW
	} else if(menu==M_CRT_EXTRCURVE) {
		MouseExtrCurve(nflg, point, plane, ev);							// ���� �Ȑ����o
	} else if(menu==M_CRT_SWEEP) {
		MouseSweep(nflg, point, plane, ev);								// ���� �|����
	} else if(menu==M_CRT_REVOLVE) {
		MouseRevolve(nflg, point, plane, ev);							// ���� ��]��
	} else if(menu==M_CRT_PYRAMID) {
		MousePyramid(nflg, point, plane, ev);							// ���� ��
	} else if(menu==M_CRT_SETSECTION) {
		MouseSetSection(nflg, point, plane, ev);						// ���� �Ȑ��f�ʊ��t
	} else if(menu==M_EDIT_SPACE) {
		MouseSpace(nflg, point, plane, ev);								// ���� ��߰�
	} else if(menu==M_EDIT_MOVE) {
		MouseMove(nflg, point, plane, ev);								// ���� �ړ�
	} else if(menu==M_EDIT_ROTATE) {
		MouseRotate(nflg, point, plane, ev);							// ���� ��]
	} else if(menu==M_EDIT_MIRROR) {
		MouseMirror(nflg, point, plane, ev);							// ���� ����
	} else if(menu==M_EDIT_SCALE) {
		MouseScale(nflg, point, plane, ev);								// ���� ���ޕύX
	} else if(menu==M_EDIT_RESIZE) {
		MouseResize3(nflg, point, plane, ev);							// ���� �����w��ػ���2
	} else if(menu==M_EDIT_RESIZE2) {
		MouseResize4(nflg, point, plane, ev);							// ���� �Ȑ��w��ػ���2
	} else if(menu==M_EDIT_BEND) {
		MouseBend(nflg, point, plane, ev);								// ���� �~���Ȃ�
	} else if(menu==M_EDIT_TWIST) {
		MouseTwist(nflg, point, plane, ev);								// ���� ���̂Ђ˂�
	} else if(menu==M_EDIT_SHEAR) {
		MouseShear(nflg, point, plane, ev);								// ���� ����f
	} else if(menu==M_EDIT_LINES) {
		MouseLinesEdit(nflg, point, plane, ev);							// ���� �Ȑ��w��ړ�
	} else if(menu==M_EDIT_LINE) {
		MouseLineEdit(nflg, point, plane, ev);							// ���� ����w��ړ�
	} else if(menu==M_EDIT_CIRCLE) {
		MouseCircleEdit(nflg, point, plane, ev);						// ���� ��~�w��ړ�
	} else if(menu==M_EDIT_PCUT) {
		MousePCut(nflg, point, plane, ev);								// ���� ���ʕ���
	} else if(menu==M_EDIT_PASTEOBJ) {
		MousePasteObject(nflg, point, plane, ev);						// �ҏW �߰��
	} else if(menu==M_SELECT_OBJ) {
		ObjChoose(nflg, point, plane, ev);								// �ҏW ���̑I��
	} else if(menu==M_ZOOM_BOX) {
		ZoomBox(nflg, point, plane, ev);								// �w��{��(BOX�\��)
	} else if(menu==M_SCROLL) {
		Scroll(nflg, point, plane, ev);									// ��ʽ�۰�
	} else if(menu==M_BASE_SET) {
		BasePointSet(nflg, point, plane, ev);							// ��_�o�^
	} else if(menu==M_ALLSHADING) {
		MenuAllShad(nflg, point, plane, ev);							// ����ިݸ�
	} else if(menu==M_RENDERING) {
		MenuRender(nflg, point, plane, ev);								// �����ݸ�
	} else if(menu==M_JWL_STONE) {
		MouseJwlStone(nflg, point, plane, ev);							// �Θg����
	} else if(menu==M_JWL_ARRANGE) {
		MouseJwlArrange(nflg, point, plane, ev);						// �Θg�����z�u
	} else if(menu==M_JWL_GEM) {
		if(m_GemNo==0||m_GemNo==5||m_GemNo==9) {
			MouseGemSquare(nflg, point, plane, ev);						// ��ζ��1(׳��ތn)
		} else if(m_GemNo==1||m_GemNo==2||m_GemNo==3||
                  m_GemNo==4||m_GemNo==6||m_GemNo==7||m_GemNo==10) {
			MouseGemRect(nflg, point, plane, ev);						// ��ζ��2(����ٌn)
		} else if(m_GemNo==8) {
			MouseGemRect2(nflg, point, plane, ev);						// ��ζ��3(ð�߰��)
		}
	} else if(menu==M_JWL_EARTH) {
		MouseEarth(nflg, point, plane, ev);								// ���
	} else if(menu==M_MEASURE_DIST) {
		MouseMeasureDistance(nflg, point, plane, ev);					// ��������
	} else if(menu==M_MEASURE_ANGLE) {
		MouseMeasureAngle(nflg, point, plane, ev);						// �p�x����
	} else if(menu==M_MEASURE_CURVE) {
		MouseMeasureCurve(nflg, point, plane, ev);						// �Ȑ���������
	} else if(menu==M_MEASURE_SCALE) {
		MouseMeasureScale(nflg, point, plane, ev);						// ��K
	} else if(menu==M_MEASURE_GRAD) {
		MouseMeasureProt(nflg, point, plane, ev);						// ���x��
	} else {
		EyeChange(nflg, point, plane, ev);								// �����}���_�ύX
		ObjSelect(nflg, point, plane, ev);								// ���̑I��
	}
}

/****************************************/
void CJcad3GlbView::InitializeMenu(void)								//<<< �ƭ���������
/****************************************/
{
	if(m_SFuncMode<3) {													// <�ڍ�>
		m_pFunction->MessageOut(-1, 0);									// ̧ݸ���ү���ޏ�����
		if(m_SFuncMode>0) {												// <�ڍ�[�풓�ȊO]>
			HideFunction();												// ̧ݸ����޲�۸ޏ���
		}
	} else if(m_SFuncMode==4||m_SFuncMode==5) {							// <�Ȉ�[�풓�ȊO]>
		HideFunction2();												// ̧ݸ����޲�۸�2����
	}
}

/********************************************/
void CJcad3GlbView::ExitMenu(int f1, int f2)							//<<< �ƭ��I������
/********************************************/
{
	if(m_ActMenu!=M_SURF_TENSION) {										// ���͌W��������
		m_SurfTension = 0;
	}
	m_ActMenu = NONE;													// �ƭ��ԍ��ر
	Sphase = 0;															// ̪��޸ر
	m_CursorNo = 0;														// ����No�ر
	if(f1&&!(m_BtnEnv&0x1)) {											// �������ݸر
		m_CopyBtn = 0;
	}
	m_CopyInp = 0;
	m_CopyNum = 0;
	if(f2&&!(m_BtnEnv&0x2)) {											// ���l���ݸر
		m_NumBtn = 0;
	}
	m_NumInp = 0;
	InitChar();															// �����͏�����
	CheckEdtObjPrm();													// �ҏW�Ώۗ��̐������Ұ�����
}

/*************************************/
void CJcad3GlbView::InitTRLimit(void)									//<<< �ׯ���͈͌��菉����
/*************************************/
{
	m_XLMin = 1.0-DBL_MAX, m_XLMax = DBL_MAX;							// X ����Ȃ�
	m_YLMin = 1.0-DBL_MAX, m_YLMax = DBL_MAX;							// Y ����Ȃ�
	m_ZLMin = 1.0-DBL_MAX, m_ZLMax = DBL_MAX;							// Z ����Ȃ�
}

/*******************************************/
BOOL CJcad3GlbView::SetTRLimit(OBJTYPE* op)								//<<< �ׯ���͈͌���ݒ�
/*******************************************/
{
	PNTTYPE cpt;
	BOXTYPE box;

	if(op!=NULL) {														// <OP�w�肠��>
		GetObjBox(op, &box);											// ����BOX�擾
	} else {															// <OP�w��Ȃ�>
		GetAreaBox(2, &box);											// �I�𗧑�BOX�擾
	}
	BoxCenter(box, &cpt);												// BOX�����擾
	m_XLmtR = (double)((int)(cpt.p[0]*10000))/10000.0;					// �ҏW���̂̒��S
	m_YLmtR = (double)((int)(cpt.p[1]*10000))/10000.0;
	m_ZLmtR = (double)((int)(cpt.p[2]*10000))/10000.0;
	CLimitTrackerDlg dlg;
	dlg.SetValue(m_XLmtR, m_YLmtR, m_ZLmtR, m_XLmt1,
                 m_XLmt2, m_YLmt1, m_YLmt2, m_ZLmt1, m_ZLmt2);
	if(dlg.DoModal()==IDOK) {											// �޲�۸ޕ\��
		dlg.GetValue(&m_XLmtR, &m_YLmtR, &m_ZLmtR, &m_XLmt1,
                     &m_XLmt2, &m_YLmt1, &m_YLmt2, &m_ZLmt1, &m_ZLmt2);
		m_XLMin = (m_XLmt1) ? 1.0-DBL_MAX : m_XLmtR-0.0001;				// X �ŏ�������
		m_XLMax = (m_XLmt2) ? DBL_MAX : m_XLmtR+0.0001;					// X �ő呤����
		m_YLMin = (m_YLmt1) ? 1.0-DBL_MAX : m_YLmtR-0.0001;				// Y �ŏ�������
		m_YLMax = (m_YLmt2) ? DBL_MAX : m_YLmtR+0.0001;					// Y �ő呤����
		m_ZLMin = (m_ZLmt1) ? 1.0-DBL_MAX : m_ZLmtR-0.0001;				// Z �ŏ�������
		m_ZLMax = (m_ZLmt2) ? DBL_MAX : m_ZLmtR+0.0001;					// Z �ő呤����
		return TRUE;
	} else {
		InitTRLimit();													// �ׯ���͈͌��菉����
		return FALSE;
	}
}

/******************************************************************/
void CJcad3GlbView::GetTRLimit(double* x1, double* x2, double* y1,
                               double* y2, double* z1, double* z2)		//<<< �ׯ���͈͌���擾
/******************************************************************/
{
	*x1 = m_XLMin, *x2 = m_XLMax, *y1 = m_YLMin;
	*y2 = m_YLMax, *z1 = m_ZLMin, *z2 = m_ZLMax;
}
