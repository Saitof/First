/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW�g���b�J�[���암
// EditCtrlTracker.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/23 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "NumFunc.h"

/*******************************************************/
void GetMoveValue(int plane, PNTTYPE bas, PNTTYPE* val)					//<<< �ׯ���ړ��ʎ擾
/*******************************************************/
{
	int i1, i2, i3;

	i1 = plane;															// �X�V�Ώۍ��W
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	val->p[i1] = val->p[i1]-bas.p[i1];									// �ړ��ʎZ�o
	val->p[i2] = val->p[i2]-bas.p[i2];
	val->p[i3] = 0.0;
}

/*******************************************************************/
void SetParsTracker(int ps, int* md, int fg, int* pk, PNTTYPE* pa,
                    int* ia, int an, PNTTYPE* pb, int* ib, int* bn)		//<<< �����}�ׯ���ݒ�
/*******************************************************************/
{
	int* cl = new int[View->m_MaxWCtl];
	int  i, j, ct=0;

	if(*md) {															// <<�����}�I��L>>
		if(!fg) {														// <�Ȗʂ̏ꍇ>
			for(i=0; i<an; i++) {
				if(pk[i]) {												// �w���ς݂𒊏o
					pb[ct] = pa[i], ib[ct] = i, cl[ct++] = 4;
				}
			}
		} else {														// <�Ȑ��̏ꍇ>
			j = (an+1)/2;
			for(i=0; i<j; i++) {
				if(pk[i]) {												// �w���ς݂𒊏o
					pb[ct] = pa[i*2], ib[ct] = i*2, cl[ct++] = 4;
				}
			}
		}
		if(ct>0) {
			View->Tracker1.Setup(pa, ia, NULL, an, 3, ps);				// �ׯ��1�ݒ�
			View->Tracker2.Setup(pb, cl, NULL, ct, 5, ps);				// �ׯ��2�ݒ�
		} else {
			View->Tracker1.Setup(pa, ia, NULL, an, 4, ps);				// �ׯ��1�ݒ�
			View->Tracker2.Reset(0);									// �ׯ��2����
			*md = 0;
		}
	} else {															// <<�����}�I��>>
		View->Tracker1.Setup(pa, ia, NULL, an, 4, ps);					// �ׯ��1�ݒ�
		View->Tracker2.Reset(0);										// �ׯ��2����
	}
	*bn = ct;
	delete[] cl;
}

/**********************************************************/
void DispParsTracker(int ps, int md, PNTTYPE* pa, int* ia,
                     int an, PNTTYPE* pb, int* ib, int bn)				//<<< �����}�ׯ���\��
/**********************************************************/
{
	int* cl = new int[View->m_MaxSCtA];
	int  i;

	if(md) {															// <�����}�I��L>
		View->Tracker1.Setup(pa, ia, NULL, an, 3, ps);					// �ׯ��1�ݒ�
		for(i=0; i<bn; i++) {
			pb[i] = pa[ib[i]], cl[i] = 4;								// �ŐV���ް��ɍX�V
		}
		View->Tracker2.Setup(pb, cl, NULL, bn, 5, ps);					// �ׯ��2�ݒ�
	} else {															// <�����}�I��>
		View->Tracker1.Setup(pa, ia, NULL, an, 4, ps);					// �ׯ��1�ݒ�
		View->Tracker2.Reset(0);										// �ׯ��2����
	}
	delete[] cl;
}

/**********************************************************/
void HitCtrlTracker1(UINT fg, const CPoint& po, int pl,
                     int ms, PNTTYPE pt, PNTTYPE* p1,
                     int n1, PNTTYPE* p2, int* i2, int n2,
                     int* f1, int* f2, int* pn, int* pa)				//<<< �S�ׯ������1
/**********************************************************/
{
	int ht;

	*pn = 0;
	if(n2==0||pl==3) {
		if(pl==3) {														// <�����}�I��>
			if((ht=View->Tracker1.HitTracker3D(0, po, p1, n1))!=-1) {	// �ׯ��1����
				*f1 = 1, *pn = 1, pa[0] = ht;							// �����}�I��ݒ�
			} else {
				View->EyeChange(fg, po, pl, ms);						// ���_�ύX
				*f2 = 1;												// ���_�ύXӰ��
			}
		} else {														// <3�ʐ}�I��>
			*pn = View->Tracker1.HitTrackerS(0, pl, pt, p1, n1, pa);	// �ׯ��1����
			if(*pn>1) {
				*f1 = 1;
			}
		}
	} else {
		if((ht=View->Tracker2.HitTracker(0, pl, pt, p2, n2))!=-1) {		// �ׯ��2����
			*pn = 1, pa[0] = i2[ht];									// �I���ׯ���ԍ�
		}
	}
}

/*************************************************************/
void HitCtrlTracker2(UINT fg, const CPoint& po, int pl,
                     int ms, PNTTYPE pt, PNTTYPE* p1, int n1,
                     int m1, int* f2, int* pn, int* pa)					//<<< �S�ׯ������2
/*************************************************************/
{
	int ht;

	*pn = 0;
	if(pl==3) {															// <�����}�I��>
		if((ht=View->Tracker1.HitTracker3D(0, po, p1, n1))!=-1) {		// �O�����ׯ��1����
			*pn = 1, pa[0] = ht;
		} else {
			View->EyeChange(fg, po, pl, ms);							// ���_�ύX
			*f2 = 1;													// ���_�ύXӰ��
		}
	} else {															// <3�ʐ}�I��>
		if((ht=View->Tracker1.HitTracker(0, pl, pt, &p1[n1], m1))!=-1) {// �㔼���ׯ��1����
			*pn = 1, pa[0] = ht+n1;
		} else {
			*pn = View->Tracker1.HitTrackerS(0, pl, pt, p1, n1, pa);	// �O�����ׯ��1����
		}
	}
}

/********************************************************/
void DragCtrlTracker1(int pl, BOXTYPE bx, PNTTYPE* p1,
                      int n1, PNTTYPE* p2, int* i2,
                      int n2, int* f1, int* pn, int* pa)				//<<< �ׯ����ׯ��1
/********************************************************/
{
	BOXTYPE box;
	int     i, j, k, cnt=0;
	double  xmin, xmax, ymin, ymax, zmin, zmax;

	View->GetTRLimit(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);			// �ׯ���͈͌���擾
	for(i=0; i<DIM; i++) {
		if(bx.bp[0].p[i]<bx.bp[1].p[i]) {								// �ޯ�����ވʒu�␳
			box.bp[0].p[i] = bx.bp[0].p[i], box.bp[1].p[i] = bx.bp[1].p[i];
		} else {
			box.bp[0].p[i] = bx.bp[1].p[i], box.bp[1].p[i] = bx.bp[0].p[i];
		}
	}
	if(n2==0) {
		for(i=0; i<n1; i++) {
			if(xmin<p1[i].p[0]&&p1[i].p[0]<xmax&&
               ymin<p1[i].p[1]&&p1[i].p[1]<ymax&&
               zmin<p1[i].p[2]&&p1[i].p[2]<zmax) {						// <����͈͓�>
				if(BoxCheck(pl, box, p1[i])) {							// BOX���͑I��
					pa[cnt++] = i;
				}
			}
		}
		for(i=0; i<cnt; i++) {
			for(j=i+1; j<cnt; j++) {									// ���꒸�_�����
				if(fabs(p1[pa[i]].p[0]-p1[pa[j]].p[0])<EPSILON&&
                   fabs(p1[pa[i]].p[1]-p1[pa[j]].p[1])<EPSILON&&
                   fabs(p1[pa[i]].p[2]-p1[pa[j]].p[2])<EPSILON) {
					for(k=j; k<cnt-1; k++) {
						pa[k] = pa[k+1];
					}
					cnt--, j--;
				}
			}
		}
	} else {
		for(i=0; i<n2; i++) {
			if(xmin<p2[i].p[0]&&p2[i].p[0]<xmax&&
               ymin<p2[i].p[1]&&p2[i].p[1]<ymax&&
               zmin<p2[i].p[2]&&p2[i].p[2]<zmax) {						// <����͈͓�>
				if(BoxCheck(pl, box, p2[i])) {							// BOX���͑I��
					pa[cnt++] = i;
				}
			}
		}
		for(i=0; i<cnt; i++) {
			for(j=i+1; j<cnt; j++) {									// ���꒸�_�����
				if(fabs(p2[pa[i]].p[0]-p2[pa[j]].p[0])<EPSILON&&
                   fabs(p2[pa[i]].p[1]-p2[pa[j]].p[1])<EPSILON&&
                   fabs(p2[pa[i]].p[2]-p2[pa[j]].p[2])<EPSILON) {
					for(k=j; k<cnt-1; k++) {
						pa[k] = pa[k+1];
					}
					cnt--, j--;
				}
			}
		}
		for(i=0; i<cnt; i++) {
			pa[i] = i2[pa[i]];
		}
	}
	if(cnt>1) {
		*f1 = 1;
	}
	*pn = cnt;
}

/******************************************************/
void DragCtrlTracker2(int pl, BOXTYPE bx, PNTTYPE* p1,
                      int n1, int* pn, int* pa)							//<<< �ׯ����ׯ��2
/******************************************************/
{
	BOXTYPE box;
	int     i, j, k, cnt=0;
	double  xmin, xmax, ymin, ymax, zmin, zmax;

	View->GetTRLimit(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);			// �ׯ���͈͌���擾
	for(i=0; i<DIM; i++) {
		if(bx.bp[0].p[i]<bx.bp[1].p[i]) {								// �ޯ�����ވʒu�␳
			box.bp[0].p[i] = bx.bp[0].p[i], box.bp[1].p[i] = bx.bp[1].p[i];
		} else {
			box.bp[0].p[i] = bx.bp[1].p[i], box.bp[1].p[i] = bx.bp[0].p[i];
		}
	}
	for(i=0; i<n1; i++) {
		if(xmin<p1[i].p[0]&&p1[i].p[0]<xmax&&
           ymin<p1[i].p[1]&&p1[i].p[1]<ymax&&
           zmin<p1[i].p[2]&&p1[i].p[2]<zmax) {							// <����͈͓�>
			if(BoxCheck(pl, box, p1[i])) {								// BOX���͑I��
				pa[cnt++] = i;
			}
		}
	}
	for(i=0; i<cnt; i++) {
		for(j=i+1; j<cnt; j++) {										// ���꒸�_�����
			if(fabs(p1[pa[i]].p[0]-p1[pa[j]].p[0])<EPSILON&&
               fabs(p1[pa[i]].p[1]-p1[pa[j]].p[1])<EPSILON&&
               fabs(p1[pa[i]].p[2]-p1[pa[j]].p[2])<EPSILON) {
				for(k=j; k<cnt-1; k++) {
					pa[k] = pa[k+1];
				}
				cnt--, j--;
			}
		}
	}
	*pn = cnt;
}

/********************************************************/
void SetLineNo1(int un, int vn, int f1, int f2, int* ib)				//<<< ײݔԍ��ݒ�1
/********************************************************/
{
	int* la = new int[View->m_MaxSCtA];
	int  i, j, cnt, chk, wu1, wu2, wv1, wv2;

	wv1 = (f1%2==1) ? 1 : 0;											// �L���͈͐ݒ�
	wv2 = (f1>=2) ? 1 : 0;
	wu1 = (f2%2==1) ? 1 : 0;
	wu2 = (f2>=2) ? 1 : 0;
	for(i=wu1, cnt=0; i<un-wu2; i++) {									// �w��U�f�ʌ���
		for(j=wv1, chk=0; j<vn-wv2; j++) {
			if(ib[j*un+i]==1) {
				chk = 1; break;
			}
		}
		if(chk==1) {
			la[cnt++] = i;
		}
	}
	View->SetLineNo(0, cnt, la);										// ײݔԍ��ݒ�
	for(i=wv1, cnt=0; i<vn-wv2; i++) {									// �w��V�f�ʌ���
		for(j=wu1, chk=0; j<un-wu2; j++) {
			if(ib[i*un+j]==1) {
				chk = 1; break;
			}
		}
		if(chk==1) {
			la[cnt++] = i;
		}
	}
	View->SetLineNo(1, cnt, la);										// ײݔԍ��ݒ�
	delete[] la;
}

/********************************************************/
void SetLineNo2(int vn, int tn, int f1, int f2, int* iw)				//<<< ײݔԍ��ݒ�2
/********************************************************/
{
	int* la = new int[View->m_MaxSCtA];
	int  i, cnt, wuv, wua, wva;

	wva = (f1%2==1) ? 1 : 0;											// U/V���E�ƕ␳���擾
	wua = (f2%2==1) ? 1 : 0;
	wuv = (f1%2==1) ? vn-1 : vn;
	wuv = (f1>=2) ? wuv-1 : wuv;
	for(i=0, cnt=0; i<wuv; i++) {
		if(iw[i]==1) {													// Vײ݊m�F
			la[cnt++] = i+wva;
		}
	}
	View->SetLineNo(1, cnt, la);										// ײݔԍ��ݒ�
	for(i=wuv, cnt=0; i<tn; i++) {
		if(iw[i]==1) {													// Uײ݊m�F
			la[cnt++] = i-wuv+wua;
		}
	}
	View->SetLineNo(0, cnt, la);										// ײݔԍ��ݒ�
	delete[] la;
}

/*****************************************************/
void SetLineNo3(int tn, int fg, int* ib, OBJTYPE* op)					//<<< ײݔԍ��ݒ�3
/*****************************************************/
{
	int* la = new int[View->m_MaxSCtA];
	int  i, cnt=0;

	for(i=0; i<tn; i++) {
		if(ib[i]==1) {													// V�n�_�����+1
			la[cnt++] = (fg%2) ? i+1 : i;
		}
	}
	View->SetLineNo(0, cnt, la);										// ײݔԍ��ݒ�
	View->SetDispList(op);												// �ި���ڲؽēo�^
	delete[] la;
}

/*********************************************************/
void PickCPoint(PNTTYPE pnt, int ix, int pn, PNTTYPE* pb,
                int ub, int vb, int un, int vn,
                int uf, int vf, int fg, int* ia, int* ib)				//<<< �Ώې���_�I��
/*********************************************************/
{
	int i, ch, md=0;

	md = ((uf==1||uf==3)&&ix>=0&&ix<un) ? 1 : 0;						// �Y���ʒu�m�F
	md = ((uf==2||uf==3)&&ix>=un*(vn-1)&&ix<un*(vn-1)+un) ? 2 : md;
	md = ((vf==1||vf==3)&&ix%un==0) ? 3 : md;
	md = ((vf==2||vf==3)&&(ix+1)%un==0) ? 4 : md;
	if(fg) {															// <�Ȑ�Ӱ�޼��>
		ch = 0;
		ia[ix] = (ia[ix]+1>3) ? 0 : ia[ix]+1;
		if((!ub)&&(ix%un==0||ix%un==un-1)) {							// <U�J�̏ꍇ>
			ia[ix] = (ia[ix]%2==0) ? ia[ix]+1 : ia[ix];					// U���[�͐ܐ�
			ch++;
		}
		if((!vb)&&(ix<un||ix>=un*(vn-1))) {								// <V�J�̏ꍇ>
			ia[ix] = (ia[ix]==0) ? 2 : ia[ix];							// V���[�͐ܐ�
			ch++;
		}
		ia[ix] = (ch==2) ? 3 : ia[ix];									// U/V�J�̎l��
	}
	ib[ix] = (ib[ix]) ? 0 : 1;											// �I���󋵔��]
	if(md==0) {															// �����_�ʒu�ȊO�I��
		return;
	}
	for(i=0; i<pn; i++) {												// <<�w��_������W>>
		if(fabs(pnt.p[0]-pb[i].p[0])<EPSILON&&
           fabs(pnt.p[1]-pb[i].p[1])<EPSILON&&
           fabs(pnt.p[2]-pb[i].p[2])<EPSILON&&i!=ix) {
			if(md==1&&(i<0||i>=un)) {									// �ΏۊO�ް�
				continue;
			}
			if(md==2&&(i<un*(vn-1)||i>=un*(vn-1)+un)) {
				continue;
			}
			if(md==3&&i%un!=0) {
				continue;
			}
			if(md==4&&(i+1)%un!=0) {
				continue;
			}
			if(fg) {													// <�Ȑ�Ӱ�޼��>
				ch = 0;
				ia[i] = (ia[i]+1>3) ? 0 : ia[i]+1;
				if((!ub)&&(i%un==0||i%un==un-1)) {						// <U�J�̏ꍇ>
					ia[i] = (ia[i]%2==0) ? ia[i]+1 : ia[i];				// U���[�͐ܐ�
					ch++;
				}
				if((!vb)&&(i<un||i>=un*(vn-1))) {						// <V�J�̏ꍇ>
					ia[i] = (ia[i]==0) ? 2 : ia[i];						// V���[�͐ܐ�
					ch++;
				}
				ia[i] = (ch==2) ? 3 : ia[i];							// U/V�J�̎l��
			}
			ib[i] = (ib[i]) ? 0 : 1;									// �I���󋵔��]
		}
	}
}

/**************************************************/
void CheckCLine(int ub, int vb, int un, int vn,
                PNTTYPE* pa, int* uflg, int* vflg)						//<<< ����_��m�F
/**************************************************/
{
	int i, j, chk;

	*uflg = 0, chk = 0;
	if(ub) {															// <<U�n�_�m�F>>
		for(i=1; i<un; i++) {
			for(j=0; j<DIM; j++) {
				if(fabs(pa[0].p[j]-pa[i].p[j])>EPSILON) {				// U�n�_����_?
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// ����_+1
		*uflg += 1;
	}
	chk = 0;
	if(ub) {															// <<U�I�_�m�F>>
		for(i=1; i<un; i++) {
			for(j=0; j<DIM; j++) {										// U�I�_����_?
				if(fabs(pa[un*(vn-1)].p[j]-pa[un*(vn-1)+i].p[j])>EPSILON) {
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// ����_+2
		*uflg += 2;
	}
	*vflg = 0, chk = 0;
	if(vb) {															// <<V�n�_�m�F>>
		for(i=1; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				if(fabs(pa[0].p[j]-pa[i*un].p[j])>EPSILON) {			// V�n�_����_?
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// ����_+1
		*vflg += 1;
	}
	chk = 0;
	if(vb) {															// <<V�I�_�m�F>>
		for(i=1; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				if(fabs(pa[un-1].p[j]-pa[un-1+i*un].p[j])>EPSILON) {	// V�I�_����_?
					chk = 1; break;
				}
			}
			if(chk) {
				break;
			}
		}
	} else {
		chk = 1;
	}
	if(!chk) {															// ����_+2
		*vflg += 2;
	}
}

/****************************************************************/
void TrackerCLine(int md, int sw, int un, int vn, PNTTYPE* pa,
                  int uf, int vf, int *wn, PNTTYPE* pw, int* iw)		//<<< ����_���ׯ���擾
/****************************************************************/
{
	int i, j, uu, vv, us, ue, vs, ve, uv, pc=0;

	us = 0, vs = 0, uu = un, ue = un, vv = vn, ve = vn;					// �ϐ�������
	if(uf%2==1) {														// <U�n�_����̏ꍇ>
		vs++, vv--;
	}
	if(uf>=2) {															// <U�I�_����̏ꍇ>
		ve--, vv--;
	}
	if(vf%2==1) {														// <V�n�_����̏ꍇ>
		us++, uu--;
	}
	if(vf>=2) {															// <V�I�_����̏ꍇ>
		ue--, uu--;
	}
	uv = us+(uu-1)/2;
	for(i=vs; i<ve; i++) {												// <<V�����ׯ��>>
		if(sw) {														// �ʏ�Ȗʂ͒��ԓ_
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[i*un+uv].p[j]+pa[i*un+uv+1].p[j])/2.0;
			}
		} else {														// �|���Ȗʂ͊J�n�f��
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = pa[i*un+us].p[j];
			}
		}
		iw[pc++] = 0;
	}
	uv = vs+(vv-1)/2;
	for(i=us; i<ue; i++) {												// <<U�����ׯ��>>
		if(md) {														// �ʉߓ_�͒f�ʓ_��
			pw[pc] = pa[i+uv*un];
		} else {														// ����_�͒f�ʓ_����
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[i+uv*un].p[j]+pa[i+(uv+1)*un].p[j])/2.0;
			}
		}
		iw[pc++] = 2;
	}
	*wn = pc;
}

/****************************************************************/
static void CheckCPoint(PNTTYPE pnt, int pn, PNTTYPE* pp,
                        int ub, int vb, int un, int vn, int uvf,
                        int flg, int* ia, int* ib, int* iw)				//<<< �Ώې���_�m�F
/****************************************************************/
{
	int i, f1, f2;

	for(i=0; i<pn; i++) {												// <<�w��_�Ɠ�����W>>
		if(iw[i]==0) {													// <�������̏ꍇ>
			if(fabs(pnt.p[0]-pp[i].p[0])<EPSILON&&
               fabs(pnt.p[1]-pp[i].p[1])<EPSILON&&
               fabs(pnt.p[2]-pp[i].p[2])<EPSILON) {
				f1 = ia[i]%2, f2 = ia[i]/2;								// �Ȑ�Ӱ�ސݒ�
				if(!uvf) {
					if(flg) {											// �Ȑ�Ӱ�ޔ��]
						f1 = (f1) ? 0 : 1;
					}
				} else {
					if(flg) {											// �Ȑ�Ӱ�ޔ��]
						f2 = (f2) ? 0 : 1;
					}
				}
				f1 = ((!ub)&&(i%un==0||i%un==un-1)) ? 1 : f1;			// U�J��U���[�ܐ�
				f2 = ((!vb)&&(i<un||i>=un*(vn-1))) ? 1 : f2;			// V�J��V���[�ܐ�
				ia[i] = f1+(f2*2);										// �Ȑ�Ӱ��
				ib[i] = (ib[i]) ? 0 : 1;								// �I���󋵔��]
				iw[i] = 1;												// ������
			}
		}
	}
}

/****************************************************************/
void PickCLine(int ix, int ub, int vb, int un, int vn, int uflg,
               int vflg, int flg, PNTTYPE* pa, int* ia, int* ib)		//<<< �Ώې���_��I��
/****************************************************************/
{
	int*    iw = new int[View->m_MaxSCtA];
	PNTTYPE pnt;
	int     i, pn, uu, vv, ust, ued, vst, ved;

	ust = 0, vst = 0, uu = un, ued = un, vv = vn, ved = vn;				// �ϐ�������
	if(uflg%2==1) {														// <U�n�_����̏ꍇ>
		vst++, vv--;
	}
	if(uflg>=2) {														// <U�I�_����̏ꍇ>
		ved--, vv--;
	}
	if(vflg%2==1) {														// <V�n�_����̏ꍇ>
		ust++, uu--;
	}
	if(vflg>=2) {														// <V�I�_����̏ꍇ>
		ued--, uu--;
	}
	pn = un*vn;															// ������_��
	for(i=0; i<pn; i++) {
		iw[i] = 0;														// ���s���׸ޏ�����
	}
	if(ix<vv) {															// <<V�����w��>>
		if(uflg%2==1&&ix==0) {											// <U�n�_����&�[�w��>
			for(i=ust; i<ued; i++) {
				pnt = pa[i];											// �Ώې���_�m�F
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 1, flg, ia, ib, iw);
			}
		}
		if(uflg>=2&&ix==vv-1) {											// <U�I�_����&�[�w��>
			for(i=ust; i<ued; i++) {
				pnt = pa[i+(vn-1)*un];									// �Ώې���_�m�F
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 1, flg, ia, ib, iw);
			}
		}
		if(uflg%2==1) {													// U�n�_����+1
			ix++;
		}
		for(i=ust; i<ued; i++) {
			pnt = pa[i+ix*un];											// �Ώې���_�m�F
			CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 1, flg, ia, ib, iw);
		}
	} else {															// <<U�����w��>>
		ix = ix-vv;
		if(vflg%2==1&&ix==0) {											// <V�n�_����&�[�w��>
			for(i=vst; i<ved; i++) {
				pnt = pa[i*un];											// �Ώې���_�m�F
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 0, flg, ia, ib, iw);
			}
		}
		if(vflg>=2&&ix==uu-1) {											// <V�I�_����&�[�w��>
			for(i=vst; i<ved; i++) {
				pnt = pa[(i+1)*un-1];									// �Ώې���_�m�F
				CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 0, flg, ia, ib, iw);
			}
		}
		if(vflg%2==1) {													// V�n�_����+1
			ix++;
		}
		for(i=vst; i<ved; i++) {
			pnt = pa[ix+i*un];											// �Ώې���_�m�F
			CheckCPoint(pnt, pn, pa, ub, vb, un, vn, 0, flg, ia, ib, iw);
		}
	}
	delete[] iw;
}

/*******************************************************************/
void TrackerCInsert(int md, int ub, int vb, int un, int vn, int cn,
                    PNTTYPE* pa, int* wn, PNTTYPE* pw, int* iw)			//<<< ����_���ׯ���擾
/*******************************************************************/
{
	int i, j, k, l, mx, pc=0;

	k = (vn-1)/2, l = (un-1)/2;
	for(i=0; i<un-1; i++) {												// <<U�����ׯ��>>
		if(md) {														// �ʏ�Ȗʂ̂ݒf�ʒu���p
			pw[pc] = pa[k*un+i], iw[pc++] = 2;
		}
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[k*un+i].p[j]+pa[k*un+i+1].p[j])/2.0;
		}
		iw[pc++] = 2;
	}
	if(md) {															// �ʏ�Ȗʂ̂ݒf�ʒu���p
		pw[pc] = pa[(k+1)*un-1], iw[pc++] = 2;
	}
	if(ub) {															// <U�̏ꍇ>
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[(k+1)*un-1].p[j]+pa[k*un].p[j])/2.0;
		}
		iw[pc++] = 2;
	}
	for(i=0; i<vn-1; i++) {												// <<V�����ׯ��>>
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[l+i*un].p[j]+pa[l+(i+1)*un].p[j])/2.0;
		}
		iw[pc++] = 0;
	}
	if(vb) {															// <V�̏ꍇ>
		for(j=0; j<DIM; j++) {
			pw[pc].p[j] = (pa[l+(vn-1)*un].p[j]+pa[l].p[j])/2.0;
		}
		iw[pc++] = 0;
	}
	if(md) {															// �ʏ�Ȗʂͺ�߰�p�ׯ��
		for(i=0; i<un; i++) {											// <<U�����ׯ��>>
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[k*un+i].p[j]*3.0+pa[(k+1)*un+i].p[j])/4.0;
			}
			iw[pc++] = 3;
		}
	}
	if(cn>-1) {															// <���ʒf�ʎw�莞>
		mx = 2*un+vn;													// �ő�}���ʒuNo
		if(!ub) {														// U���J�͌��Z
			mx--;
		}
		if(!vb) {														// V���J�͌��Z
			mx--;
		}
		iw[mx+cn] = 5;													// �w��f�ʶװ�ύX
	}
	*wn = pc;
}

/********************************************************************/
void TrackerULine(int md, int un, int vn, PNTTYPE* pa,
                  int uflg, int vflg, int* wn, PNTTYPE* pw, int* iw)	//<<< ����_���ׯ���擾
/********************************************************************/
{
	int i, j, vv, us, ue, vs, uv, pc=0;

	us = 0, vs = 0, ue = un, vv = vn;									// �ϐ�������
	if(uflg%2==1) {														// <U�n�_����̏ꍇ>
		vs++, vv--;
	}
	if(uflg>=2) {														// <U�I�_����̏ꍇ>
		vv--;
	}
	if(vflg%2==1) {														// <V�n�_����̏ꍇ>
		us++;
	}
	if(vflg>=2) {														// <V�I�_����̏ꍇ>
		ue--;
	}
	uv = vs+(vv-1)/2;
	for(i=us; i<ue; i++) {												// <<U�����ׯ��>>
		if(md) {														// �ʉߓ_�͒f�ʓ_��
			pw[pc] = pa[i+uv*un];
		} else {														// ����_�͒f�ʓ_����
			for(j=0; j<DIM; j++) {
				pw[pc].p[j] = (pa[i+uv*un].p[j]+pa[i+(uv+1)*un].p[j])/2.0;
			}
		}
		iw[pc++] = 2;
	}
	*wn = pc;
}

/****************************************************************/
void TrackerVLine(int un, int vn, PNTTYPE* pa, int vflg,
                  int ix, int tn, int* wn, PNTTYPE* pw, int* iw)		//<<< ����_���ׯ���擾
/****************************************************************/
{
	int i, st;

	st = tn;
	if(vflg%2==1) {														// <V�n�_����̏ꍇ>
		ix++;
	}
	for(i=0; i<vn; i++) {												// <<V�����ׯ��>>
		pw[st] = pa[ix+i*un], iw[st++] = 0;
	}
	*wn = vn;
}

/*******************************************************/
void SelAreaCtrl(int plane, BOXTYPE area,
                 int pn, PNTTYPE* pa, int* ia, int flg)					//<<< ����_�I��(�ر)
/*******************************************************/
{
	BOXTYPE box;
	int     i;
	double  xmin, xmax, ymin, ymax, zmin, zmax;

	View->GetTRLimit(&xmin, &xmax, &ymin, &ymax, &zmin, &zmax);			// �ׯ���͈͌���擾
	for(i=0; i<DIM; i++) {												// �ޯ�����ވʒu�␳
		if(area.bp[0].p[i]<area.bp[1].p[i]) {
			box.bp[0].p[i] = area.bp[0].p[i], box.bp[1].p[i] = area.bp[1].p[i];
		} else {
			box.bp[0].p[i] = area.bp[1].p[i], box.bp[1].p[i] = area.bp[0].p[i];
		}
	}
	for(i=0; i<pn; i++) {
		ia[i] = (!flg) ? 0 : ia[i];										// �I��S����
		if(xmin<pa[i].p[0]&&pa[i].p[0]<xmax&&
           ymin<pa[i].p[1]&&pa[i].p[1]<ymax&&
           zmin<pa[i].p[2]&&pa[i].p[2]<zmax) {							// <����͈͓�>
			if(BoxCheck(plane, box, pa[i])) {
				ia[i] = (ia[i]) ? 0 : 1;
			}
		}
	}
}

/*****************************************************/
void GetBasePoint(int un, int vn, PNTTYPE* pa,
                  int ix, PNTTYPE* cpt, VECTYPE* vec)					//<<< ��_�擾
/*****************************************************/
{
	PNTTYPE* pt = new PNTTYPE[View->m_MaxCCtA];
	int      i, j, cnt;

	for(i=0, cnt=0; i<vn; i++) {
		pt[i] = pa[ix+i*un];											// V����_���o
		for(j=0; j<DIM; j++) {
			if(fabs(pt[0].p[j]-pt[i].p[j])>EPSILON) {					// �ٓ_����
				cnt++; break;
			}
		}
	}
	CalcCent(pt, vn, cpt);												// V�f�ʒ��S�擾
	CalcNVec(pt, vn, vec);												// V�f�ʖ@���޸�َ擾
	if(cnt==0) {														// <�ٓ_���̏ꍇ>
		vec->p[0] = 0.0, vec->p[1] = 0.0, vec->p[2] = 0.0;				// �@���޸�ٖ�
	}
	delete[] pt;
}
