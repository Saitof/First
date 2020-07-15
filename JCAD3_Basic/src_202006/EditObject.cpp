/////////////////////////////////////////////////////////////////////////////
// ���̕ҏW���암
// EditObject.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/08 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputsDlg.h"
#include "DataAccess2.h"
#include "NumFunc.h"

/*************************************************************/
BOOL InputCopyNum(int* cn, PNTTYPE* sc, int* sf, SCRTYPE* bp)           //<<< ���ʏ�����
/*************************************************************/
{
    CCopyInfoDlg dlg;
    dlg.SetValue(*cn, sc->p[0], sc->p[1], sc->p[2],
                 *sf, bp->p[0], bp->p[1], bp->p[2]);                    // �����l�\��
    if(dlg.DoModal()==IDOK) {                                           // ���ʏ������޲�۸ޕ\��
        dlg.GetValue(cn, &sc->p[0], &sc->p[1], &sc->p[2],
                     sf, &bp->p[0], &bp->p[1], &bp->p[2]);              // ���ʏ��擾
        return TRUE;
    } else {
        return FALSE;
    }
}

/**********************************************************/
void TrackerScale1(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa)				//<<< ػ��ޗp�ׯ���擾1
/**********************************************************/
{
	pa[0] = cent;														// �ׯ�����W�ݒ�
	pa[1].p[0]=box.bp[0].p[0], pa[1].p[1]=box.bp[0].p[1], pa[1].p[2]=cent.p[2];
	pa[2].p[0]=box.bp[1].p[0], pa[2].p[1]=box.bp[0].p[1], pa[2].p[2]=cent.p[2];
	pa[3].p[0]=box.bp[1].p[0], pa[3].p[1]=box.bp[1].p[1], pa[3].p[2]=cent.p[2];
	pa[4].p[0]=box.bp[0].p[0], pa[4].p[1]=box.bp[1].p[1], pa[4].p[2]=cent.p[2];
	pa[5].p[0]=cent.p[0], pa[5].p[1]=box.bp[0].p[1], pa[5].p[2]=box.bp[0].p[2];
	pa[6].p[0]=cent.p[0], pa[6].p[1]=box.bp[1].p[1], pa[6].p[2]=box.bp[0].p[2];
	pa[7].p[0]=cent.p[0], pa[7].p[1]=box.bp[1].p[1], pa[7].p[2]=box.bp[1].p[2];
	pa[8].p[0]=cent.p[0], pa[8].p[1]=box.bp[0].p[1], pa[8].p[2]=box.bp[1].p[2];
	pa[9].p[0]=box.bp[0].p[0], pa[9].p[1]=cent.p[1], pa[9].p[2]=box.bp[0].p[2];
	pa[10].p[0]=box.bp[0].p[0], pa[10].p[1]=cent.p[1], pa[10].p[2]=box.bp[1].p[2];
	pa[11].p[0]=box.bp[1].p[0],	pa[11].p[1]=cent.p[1], pa[11].p[2]=box.bp[1].p[2];
	pa[12].p[0]=box.bp[1].p[0], pa[12].p[1]=cent.p[1], pa[12].p[2]=box.bp[0].p[2];
}

/*****************************************************/
void TrackerScale2(int plane, BOXTYPE bx, PNTTYPE bs,
				   PNTTYPE* pa, int* op, int* pn)						//<<< ػ��ޗp�ׯ���擾2
/*****************************************************/
{
	int i, j, a, b, c;

	a = plane;															// ���W���ݒ�
    b = (a+1>2) ? 0 : a+1;
    c = (b+1>2) ? 0 : b+1;
	pa[0] = bs, op[0] = 0;												// �ׯ�����W�ݒ�
	pa[1].p[a]=bx.bp[0].p[a], pa[1].p[b]=bx.bp[0].p[b], pa[1].p[c]=bs.p[c];
	pa[2].p[a]=bx.bp[1].p[a], pa[2].p[b]=bx.bp[0].p[b], pa[2].p[c]=bs.p[c];
	pa[3].p[a]=bx.bp[1].p[a], pa[3].p[b]=bx.bp[1].p[b], pa[3].p[c]=bs.p[c];
	pa[4].p[a]=bx.bp[0].p[a], pa[4].p[b]=bx.bp[1].p[b], pa[4].p[c]=bs.p[c];
	for(i=1, j=1; i<5; i++) {											// ��_�Ɠ���_�͍폜
		if(fabs(bs.p[a]-pa[i].p[a])>EPSILON||
           fabs(bs.p[b]-pa[i].p[b])>EPSILON) {
			if(fabs(bs.p[a]-pa[i].p[a])<=EPSILON) {
				op[j] = 6;
			} else if(fabs(bs.p[b]-pa[i].p[b])<=EPSILON) {
				op[j] = 5;
			} else {
				op[j] = 7;
			}
			pa[j++] = pa[i];
		}
	}
	*pn = j;
}

/*****************************************************************/
void TrackerBend(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa, int* op)		//<<< �~���Ȃ��p�ׯ���擾
/*****************************************************************/
{																		// �ׯ�����W�ݒ�
	pa[0].p[0]=box.bp[0].p[0], pa[0].p[1]=box.bp[0].p[1], pa[0].p[2]=cent.p[2];
	pa[1].p[0]=box.bp[1].p[0], pa[1].p[1]=box.bp[0].p[1], pa[1].p[2]=cent.p[2];
	pa[2].p[0]=box.bp[1].p[0], pa[2].p[1]=box.bp[1].p[1], pa[2].p[2]=cent.p[2];
	pa[3].p[0]=box.bp[0].p[0], pa[3].p[1]=box.bp[1].p[1], pa[3].p[2]=cent.p[2];
	pa[4].p[0]=cent.p[0], pa[4].p[1]=box.bp[0].p[1], pa[4].p[2]=box.bp[0].p[2];
	pa[5].p[0]=cent.p[0], pa[5].p[1]=box.bp[1].p[1], pa[5].p[2]=box.bp[0].p[2];
	pa[6].p[0]=cent.p[0], pa[6].p[1]=box.bp[1].p[1], pa[6].p[2]=box.bp[1].p[2];
	pa[7].p[0]=cent.p[0], pa[7].p[1]=box.bp[0].p[1], pa[7].p[2]=box.bp[1].p[2];
	pa[8].p[0]=box.bp[0].p[0], pa[8].p[1]=cent.p[1], pa[8].p[2]=box.bp[0].p[2];
	pa[9].p[0]=box.bp[0].p[0], pa[9].p[1]=cent.p[1], pa[9].p[2]=box.bp[1].p[2];
	pa[10].p[0]=box.bp[1].p[0], pa[10].p[1]=cent.p[1], pa[10].p[2]=box.bp[1].p[2];
	pa[11].p[0]=box.bp[1].p[0], pa[11].p[1]=cent.p[1], pa[11].p[2]=box.bp[0].p[2];
	op[0] = 9, op[1] = 12, op[2] = 10, op[3] = 11;						// �ׯ����߼�ݐݒ�
	op[4] = 5, op[5] = 5, op[6] = 6, op[7] = 6;

}

/*******************************************/
void TrackerBox3D(BOXTYPE box, PNTTYPE* pa)								//<<< �ޯ��3D�p�ׯ���擾
/*******************************************/
{																		// �ׯ�����W�ݒ�
	pa[0].p[0]=box.bp[0].p[0], pa[0].p[1]=box.bp[0].p[1], pa[0].p[2]=box.bp[0].p[2];
	pa[1].p[0]=box.bp[1].p[0], pa[1].p[1]=box.bp[1].p[1], pa[1].p[2]=box.bp[0].p[2];
	pa[2].p[0]=box.bp[0].p[0], pa[2].p[1]=box.bp[1].p[1], pa[2].p[2]=box.bp[1].p[2];
	pa[3].p[0]=box.bp[1].p[0], pa[3].p[1]=box.bp[0].p[1], pa[3].p[2]=box.bp[1].p[2];
}

/***************************************************************/
void TrackerBox2D(int plane, BOXTYPE box, PNTTYPE* pa, int* op)			//<<< �ޯ��2D�p�ׯ���擾
/***************************************************************/
{
	int i, j, k;

	i = plane;															// ���W���ݒ�
    j = (i+1>2) ? 0 : i+1;
    k = (j+1>2) ? 0 : j+1;												// �ׯ�����W�ݒ�
	pa[0].p[i] = box.bp[0].p[i], pa[0].p[j] = box.bp[0].p[j];
	pa[1].p[i] = box.bp[1].p[i], pa[1].p[j] = box.bp[0].p[j];
	pa[2].p[i] = box.bp[1].p[i], pa[2].p[j] = box.bp[1].p[j];
	pa[3].p[i] = box.bp[0].p[i], pa[3].p[j] = box.bp[1].p[j];
	for(i=0; i<4; i++) {
		pa[i].p[k] = (box.bp[0].p[k]+box.bp[1].p[k])/2.0;
	}
	op[0] = 9, op[1] = 12, op[2] = 10, op[3] = 11;						// �ׯ����߼�ݐݒ�
}

/**********************************************************/
void TrackerBoxCrs(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa)				//<<< �ޯ���\���p�ׯ���擾
/**********************************************************/
{
	pa[0] = cent, pa[1] = cent, pa[2] = cent;
	pa[3] = cent, pa[4] = cent, pa[5] = cent;
	pa[0].p[0] = box.bp[0].p[0], pa[1].p[0] = box.bp[1].p[0];
	pa[2].p[1] = box.bp[0].p[1], pa[3].p[1] = box.bp[1].p[1];
	pa[4].p[2] = box.bp[0].p[2], pa[5].p[2] = box.bp[1].p[2];
}

/***********************************************************/
void TrackerBoxCrs2(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa)				//<<< �ޯ���\���p�ׯ���擾2
/***********************************************************/
{
	double ln;

	ln = PntD(&box.bp[0], &cent);										// BOX�̒��S�_����̍Œ�����
	pa[0] = cent, pa[1] = cent, pa[2] = cent, pa[3] = cent;
	pa[4] = cent, pa[5] = cent, pa[6] = cent, pa[7] = cent;
	pa[1].p[0] = cent.p[0]-ln, pa[2].p[0] = cent.p[0]+ln;
	pa[3].p[1] = cent.p[1]-ln, pa[4].p[1] = cent.p[1]+ln;
	pa[5].p[2] = cent.p[2]-ln, pa[6].p[2] = cent.p[2]+ln;
	pa[7].p[0] -= ln*cos(PI/4.0);
	pa[7].p[1] -= ln*cos(PI/4.0);
	pa[7].p[2] -= ln*cos(PI/4.0);
}

/********************************************************/
static BOOL CalcCross(int i1, int i2, int i3,
					  PNTTYPE* pa, int pn, PNTTYPE* crs)				//<<< ��_�v�Z
/********************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE  pt, p1, p2, pt1, pt2;
	int      i, j, k, ct1=0, ct2=0, ret=FALSE;
	double   pr, mi, mi1=DBL_MAX, mi2=-DBL_MAX;

	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			pb[i].p[j] = pa[i].p[j]+EPSILON*3.0;						// �v�Z��̂���,�[�����Z
		}
	}
	for(i=0; i<pn; i++) {
		j = (i==0) ? pn-1 : i-1;
		if(pb[i].p[i1]<pb[j].p[i1]) {									// i1���W:����p1�Ƃ���
			p1 = pb[i], p2 = pb[j];
		} else {
			p1 = pb[j], p2 = pb[i];
		}
		if(p1.p[i1]<=0&&p2.p[i1]>=0&&fabs(p1.p[i1]-p2.p[i1])>EPSILON) {	// <���������_��ʂ�>
			for(k=0; k<DIM; k++) {
				pt.p[k] = p2.p[k]-p1.p[k];								// �����Ԃ̍�
			}
			pr = fabs(p1.p[i1]/pt.p[i1]);								// i1���W�̌��_�܂ł̔䗦
			mi = p1.p[i2]+pt.p[i2]*pr;									// i1���_��i2���W
			pt.p[i1] = 0.0;
			pt.p[i2] = p1.p[i2]+pt.p[i2]*pr;
			pt.p[i3] = p1.p[i3]+pt.p[i3]*pr;
			if(mi>=0) {													// <�����̏ꍇ>
				ct1++;													// ������������
				if(mi<mi1) {											// ���_�ɍł��߂�
					pt1 = pt, mi1 = mi;
				}
			} else {													// <�����̏ꍇ>
				ct2++;													// ������������
				if(mi>mi2) {											// ���_�ɍł��߂�
					pt2 = pt, mi2 = mi;
				}
			}
		}
	}
	if(ct1%2==1&&ct2%2==1) {											// <���_���ʓ��Ɋ܂܂��>
		for(k=0; k<DIM; k++) {
			pt.p[k] = pt2.p[k]-pt1.p[k];								// �����Ԃ̍�
		}
		pr = fabs(pt1.p[i2]/pt.p[i2]);									// i2���W�̌��_�܂ł̔䗦
		crs->p[i1] = 0.0;
		crs->p[i2] = 0.0;
		crs->p[i3] = pt1.p[i3]+pt.p[i3]*pr-EPSILON*3.0;
		ret = TRUE;
	}
	delete[] pb;
	return ret;
}

/***********************************************************************/
static BOOL CrossPoint(int plane, OBJTYPE* op, PNTTYPE pnt, double* mx,
					   PNTTYPE* crs, VECTYPE* vec, double* th)			//<<< ��_�Z�o
/***********************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE  pt, mip, map;
	VECTYPE  nvc;
	int      i, j, i1, i2, i3, flp, flg, fst=-1, cnt, ret=FALSE;
	double   ln, mxd;

	mxd = *mx;
	i1 = plane;															// ���W���ݒ�
    i2 = (i1+1>2) ? 0 : i1+1;
    i3 = (i2+1>2) ? 0 : i2+1;
	flp = GetFlpNum1(op);												// ��ٰ�ߐ��擾
	for(i=0; i<flp; i++) {
		GetFlpVtx1(op, i, &pt, &flg);									// ��ٰ�ߒ��_�擾
		if(fst<0) {														// �ʏ�񏉊��ݒ�
			mip = pt, map = pt, fst = i;
		}
		mip.p[i1] = (mip.p[i1]>pt.p[i1]) ? pt.p[i1] : mip.p[i1];		// �ŏ��l�擾
		mip.p[i2] = (mip.p[i2]>pt.p[i2]) ? pt.p[i2] : mip.p[i2];
		map.p[i1] = (map.p[i1]<pt.p[i1]) ? pt.p[i1] : map.p[i1];		// �ő�l�擾
		map.p[i2] = (map.p[i2]<pt.p[i2]) ? pt.p[i2] : map.p[i2];
		map.p[i3] = (map.p[i3]<pt.p[i3]) ? pt.p[i3] : map.p[i3];
		if(flg) {														// <<��ٰ�ߏI��>>
			if(pnt.p[i1]>=mip.p[i1]&&pnt.p[i2]>=mip.p[i2]&&				// <�L���Ȍ����̉\���L>
               pnt.p[i1]<=map.p[i1]&&pnt.p[i2]<=map.p[i2]&&mxd<=map.p[i3]) {
				for(j=fst, cnt=0; j<=i; j++) {
					GetFlpVtx1(op, j, &pt, &flg);						// ��ٰ�ߒ��_�擾
					pt.p[i1] -= pnt.p[i1], pt.p[i2] -= pnt.p[i2];		// pnt�_�����_
					pa[cnt++] = pt;
				}
				if(CalcCross(i1, i2, i3, pa, cnt, crs)) {				// ��_�v�Z
					if(crs->p[i3]>mxd) {								// i3���W����
						crs->p[i1] = pnt.p[i1], crs->p[i2] = pnt.p[i2];	// pnt�_�ɖ߂�
						mxd = crs->p[i3];								// i3�ő�l�X�V
						CalcNVec(pa, cnt, &nvc);						// �@���޸�َ擾
						vec->p[i1] = -nvc.p[i2];
						vec->p[i2] = nvc.p[i1];
						vec->p[i3] = 0.0;
						VecN(vec);										// �@���̒����޸��
						ln = sqrt(nvc.p[i1]*nvc.p[i1]+nvc.p[i2]*nvc.p[i2]);
						ln = (nvc.p[i3]<EPSILON) ? 0.0 : atan2(ln, nvc.p[i3]);
						ln = (ln<-10||ln>10) ? 0.0 : ln;
						*th = 180.0*ln/PI;								// ��]�p�x
						ret = TRUE;
					}
				}
			}
			fst = -1;
		}
	}
	*mx = mxd;
	delete[] pa;
	return ret;
}

/***********************************************************/
BOOL GetObjCrossPnt(int plane, PNTTYPE pnt,
					PNTTYPE* crs, VECTYPE* vec, double* th)				//<<< ���̌�_�擾
/***********************************************************/
{
	OBJTYPE* op;
	BOXTYPE  box;
	int      i1, i2, i3, ret=FALSE;
	double   mxd=1.0-DBL_MAX;

	i1 = plane;															// ���W���ݒ�
    i2 = (i1+1>2) ? 0 : i1+1;
    i3 = (i2+1>2) ? 0 : i2+1;
	SelBaseObjPtr(99, &op);												// �I�𗧑̐擪�߲���擾
	if(op==NULL) {														// �I�𗧑̖��͌�_�Ȃ�
		return FALSE;
	}
	while(op!=NULL) {
		GetObjBox(op, &box);											// ����BOX���ގ擾
		if(pnt.p[i1]>=box.bp[0].p[i1]&&pnt.p[i2]>=box.bp[0].p[i2]&&
           pnt.p[i1]<=box.bp[1].p[i1]&&pnt.p[i2]<=box.bp[1].p[i2]&&
           mxd<=box.bp[1].p[i3]) {										// <�L���Ȍ����̉\���L>
			if(CrossPoint(plane, op, pnt, &mxd, crs, vec, th)) {		// ��_�Z�o
				ret = TRUE;
			}
		}
		SelNextObjPtr(99, &op);											// ���I�𗧑�
	}
	return ret;
}
