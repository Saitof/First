/////////////////////////////////////////////////////////////////////////////
// �Ȗʐ���_�������v
// CreatCtrlPoint2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "EditCtrlTracker.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/*******************************/
BOOL InputBasePoint1(int* base)											//<<< �f�ʐ����ʒu����1
/*******************************/
{
	CSetBaseDlg2 dlg;
	dlg.SetValue(*base);												// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(base);												// ��_�擾
		return TRUE;
	} else {
		return FALSE;
	}
}

/*****************************************************/
BOOL InputBasePoint2(int* bas1, int* bas2, int* size)					//<<< �f�ʐ����ʒu����2
/*****************************************************/
{
	CSetBaseDlg3 dlg;
	dlg.SetValue(*bas1, *bas2, *size);									// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(bas1, bas2, size);									// ��_�擾
		return TRUE;
	} else {
		return FALSE;
	}
}

/*********************************************/
void ChangeLoop(int an, PNTTYPE* pa, int* ia)							//<<< ��ٰ�ߔ��]
/*********************************************/
{
	PNTTYPE spa;
	int     sia, st, ed;

	st = 0, ed = an-1;													// �ŏ�/�ő�ԍ�
	while(st<ed) {														// <�ő�<�ŏ���>
		spa = pa[st], sia = ia[st];										// �ŏ����ۑ�
		pa[st] = pa[ed], ia[st] = ia[ed];								// �ő呤->�ŏ���
		pa[ed] = spa, ia[ed] = sia;										// �ŏ���->�ő呤
		ed--, st++;														// �ő匸�Z/�ŏ����Z
	}
}

/********************************************************/
void InitRevolve(int bc, int an, PNTTYPE* pa,
                 int* ia, int* bn, PNTTYPE* pb, int* ib)				//<<< ��]��������
/********************************************************/
{
	PNTTYPE pnt;
	int     i, j, cnt=0;

	if(bc) {															// <<���E�̏ꍇ>>
		for(i=0; i<an-1; i++) {
			if(ia[i]!=0&&ia[i+1]!=0) {									// <�A���ܐ�Ӱ��>
				for(j=0; j<DIM; j++) {
					pnt.p[j] = (pa[i].p[j]+pa[i+1].p[j])/2.0;			// ���ԓ_���W�擾
				}
				pb[cnt] = pnt, ib[cnt++] = 0;							// �ׯ���z��ݒ�
			}
		}
		if(ia[an-1]!=0&&ia[0]!=0) {										// <�I�_&�n�_���ܐ�>
			if(ia[an-1]!=0&&ia[0]!=0) {
				for(j=0; j<DIM; j++) {
					pnt.p[j] = (pa[an-1].p[j]+pa[0].p[j])/2.0;			// ���ԓ_���W�擾
				}
				pb[cnt] = pnt, ib[cnt++] = 0;							// �ׯ���z��ݒ�
			}
		}
	}
	*bn = cnt;															// �ׯ���z��
}

/*********************************************************************/
static void Revolve(int flg, int plane, double wt, int an,
                    PNTTYPE* pa, int* ia, PNTTYPE p1, PNTTYPE p2,
                    double rt, int div, int sf, PNTTYPE* pc, int* ic)	//<<< ��]����_����
/*********************************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	int*     i2 = new int[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	PNTTYPE  cpt, scp, sc1, sc2;
	VECTYPE  vec, wvc;
	int      md, i, j, d1, d2, d3, st, ed, cnt=0;
	double   rot[3][3], drt, th, dth, ath;

	md = (fabs(rt-360.0)<EPSILON) ? 0 : 1;								// ���E:�J��
	drt = PI*rt/180.0;													// ��]���̐����p�x
	d1 = plane;															// ���W���ݒ�
	d2 = (d1+1>2) ? 0 : d1+1;
	d3 = (d2+1>2) ? 0 : d2+1;
	for(i=0; i<an; i++) {
		w1[i].p[d1] = pa[i].p[d1]-p1.p[d1];								// 2D�}�`����
		w1[i].p[d2] = pa[i].p[d2]-p1.p[d2];
		w1[i].p[d3] = 0.0;												// �w�蕽�ʊO���W
		i1[i] = ia[i];													// �Ȑ�Ӱ�ޕ���
	}
	th = atan2(p2.p[d2]-p1.p[d2], p2.p[d1]-p1.p[d1]);					// d2���ɑ΂���p�x
	th = (th<-10||th>10) ? 0.0 : th;
	th = th-PI/2.0;
	vec.p[d1] = 0.0, vec.p[d2] = 0.0, vec.p[d3] = -1.0;					// ��]���ݒ�
	SetRotateMatrix(vec, th, rot);										// ��]��د���쐬
	dth = (!md) ? drt/div : drt/(div-1);								// 1��������]�p�x
	for(i=0; i<an; i++) {
		VecRotate(&w1[i], rot);											// ��]�����S�։�]�ړ�
	}
	if(md) {
		for(i=0; i<an; i++) {
			w2[i] = w1[i], i2[i] = i1[i];								// 2D���ۑ�
		}
	}
	if(flg>1) {															// <<2D���E���J>>
		w1[an].p[d1] = 0.0;												// ��]���Ɛ�������_�ǉ�
		w1[an].p[d2] = w1[an-1].p[d2];
		w1[an].p[d3] = 0.0;
		w1[an+1].p[d1] = 0.0;
		w1[an+1].p[d2] = w1[0].p[d2];
		w1[an+1].p[d3] = 0.0;
		CalcNVec(w1, an+2, &wvc);										// �@���޸�َ擾
		if(wvc.p[d3]>0.0) {
			ChangeLoop(an, w1, i1);										// �޸�ٕ��͖�ٰ�ߔ��]
			if(md) {
				ChangeLoop(an, w2, i2);
			}
		}
	}
	if(flg>0) {															// <<2D���E����>>
		CalcCent(w1, an, &cpt);											// 2D�}�`���S�擾
		if(cpt.p[d1]<0) {
			ChangeLoop(an, w1, i1);										// ���S���͖�ٰ�ߔ��]
			if(md) {
				ChangeLoop(an, w2, i2);
			}
		}
	}
	if(flg==3) {														// <<��]���S�_�L�̏ꍇ>>
		for(j=0; j<div; j++) {											// <�J�n�_��]���S>
			pc[cnt].p[d1] = 0.0;										// ����_�z��쐬
			pc[cnt].p[d2] = w1[0].p[d2];
			pc[cnt].p[d3] = 0.0;
			ic[cnt++] = i1[0]+1;										// �Ȑ�Ӱ�ސݒ�
		}
	}
	if(flg>0&&sf) {														// <�f�ʒ��S�L�̏ꍇ>
		scp.p[d1] = pa[an].p[d1]-p1.p[d1];								// �f�ʒ��S����
		scp.p[d2] = pa[an].p[d2]-p1.p[d2];
		scp.p[d3] = 0.0;
		VecRotate(&scp, rot);											// ��]�����S�։�]�ړ�
		sc1.p[d1] = scp.p[d1]*cos(0.0);									// �J�n�f�ʒ��S�_
		sc1.p[d2] = scp.p[d2];
		sc1.p[d3] = scp.p[d1]*sin(0.0);
		sc2.p[d1] = scp.p[d1]*cos(drt);									// �I���f�ʒ��S�_
		sc2.p[d2] = scp.p[d2];
		sc2.p[d3] = scp.p[d1]*sin(drt);
	}
	if(!md) {
		st = 0, ed = div;
	} else {
		st = 1, ed = div-1;
	}
	for(i=0; i<an; i++) {
		ath = 0.0;
		if(md) {														// <�~�ʊJ�n�f��>
			if(flg>0&&sf) {												// �f�ʒ��S�L
				pc[cnt] = sc1, ic[cnt++] = i1[i]+1;
			}
			pc[cnt].p[d1] = w2[i].p[d1]*cos(0.0);
			pc[cnt].p[d2] = w2[i].p[d2];
			pc[cnt].p[d3] = w2[i].p[d1]*sin(0.0);
			ic[cnt++] = i1[i]+1;
			ath = dth;
		}
		for(j=st; j<ed; ath+=dth, j++) {								// <�ʏ�f��>
			pc[cnt].p[d1] = w1[i].p[d1]*cos(ath);						// ����_�z��쐬
			pc[cnt].p[d2] = w1[i].p[d2];
			pc[cnt].p[d3] = w1[i].p[d1]*sin(ath);
			ic[cnt++] = i1[i];											// �Ȑ�Ӱ�ސݒ�
		}
		if(md) {														// <�~�ʏI���f��>
			pc[cnt].p[d1] = w2[i].p[d1]*cos(ath);
			pc[cnt].p[d2] = w2[i].p[d2];
			pc[cnt].p[d3] = w2[i].p[d1]*sin(ath);
			ic[cnt++] = i1[i]+1;
			if(flg>0&&sf) {												// �f�ʒ��S�L
				pc[cnt] = sc2, ic[cnt++] = i1[i]+1;
			}
		}
	}
	if(flg==3) {														// <<��]���S�_�L�̏ꍇ>>
		for(j=0; j<div; j++) {											// <�ŏI�_��]���S>
			pc[cnt].p[d1] = 0.0;										// ����_�z��쐬
			pc[cnt].p[d2] = w1[an-1].p[d2];
			pc[cnt].p[d3] = 0.0;
			ic[cnt++] = i1[an-1]+1;										// �Ȑ�Ӱ�ސݒ�
		}
	}
	SetRotateMatrix(vec, -th, rot);										// ��]��د���쐬
	for(i=0; i<cnt; i++) {
		VecRotateMove(&pc[i], rot, p1);									// ���ʒu�։�]�ړ�
	}
	d1 = (flg==3) ? an+2 : an;											// �f�ʕ�������_��
	d2 = (flg>0&&sf) ? div+2 : div;										// ��]��������_��
	for(i=0; i<d2; i++) {
		i1[i] = 0;														// ��]������ײ�Ӱ��
	}
	if(!md) {															// ��(360�x��])�̏ꍇ
		d3 = 1;															// ���E����:��
	} else {															// ��^(360�x������])�̏ꍇ
		d3 = 0;															// ���E����:�J
		i1[0] = 1;														// ���[��ײ�Ӱ��:�ܐ�
		i1[d2-1] = 1;
		if(flg>0&&sf) {													// �f�ʗL�͎����ܐ�
			i1[1] = 1, i1[d2-2] = 1;
		}
	}
	SetCurveMatrix1(1, wt, d3, d2, i1, ma, mb);							// �ʉߓ_=>����_�ϊ���د��
	if(flg!=3) {														// �ʉߓ_�ϊ��͈͐ݒ�
		st = 0, ed = d1;
	} else {
		st = 1, ed = d1-1;
	}
	for(i=st; i<ed; i++) {
		for(j=0; j<d2; j++) {
			w1[j] = pc[j+d2*i];
		}
		CalcCurvePoint(1, d2, w1, w2, mb);								// �ʉߓ_=>����_�ϊ�
		for(j=0; j<d2; j++) {
			pc[j+d2*i] = w2[j];
		}
	}
	delete[] w1;
	delete[] w2;
	delete[] i1;
	delete[] i2;
	delete[] ma;
	delete[] mb;
}

/******************************************************************/
static void SweepRev(PNTTYPE cpt, int plane, double wt, int an,
                     PNTTYPE* pa, int* ia, PNTTYPE p1, PNTTYPE p2,
                     double rt, int div, PNTTYPE* pb, int* ib)			//<<< ��]�p�|���Ȗʐ���
/******************************************************************/
{
	PNTTYPE* wk = new PNTTYPE[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	PNTTYPE  wpt;
	VECTYPE  vec;
	int      i, d1, d2, d3, bc, st, ed, md=0, cnt=0;
	double   rot[3][3], drt, dth, th, ath=0.0;

	drt = PI*rt/180.0;													// ��]���̐����p�x
	if(fabs(rt-360.0)<EPSILON) {										// <360�x��]��>
		dth = drt/div, st = 0, ed = div, bc = 1;						// 1��������]�p�x
	} else {															// <360�x������]��>
		dth = drt/(div-1), st = 1, ed = div-1, md = 1, bc = 0;
	}
	d1 = plane;															// ���W���ݒ�
	d2 = (d1+1>2) ? 0 : d1+1;
	d3 = (d2+1>2) ? 0 : d2+1;
	wpt.p[d1] = cpt.p[d1]-p1.p[d1];										// 2D���S�_����
	wpt.p[d2] = cpt.p[d2]-p1.p[d2];
	wpt.p[d3] = 0.0;
	th = atan2(p2.p[d2]-p1.p[d2], p2.p[d1]-p1.p[d1]);					// d2���ɑ΂���p�x
	th = (th<-10||th>10) ? 0.0 : th;
	th = th-PI/2.0;
	vec.p[d1] = 0.0, vec.p[d2] = 0.0, vec.p[d3] = -1.0;					// ��]���ݒ�
	SetRotateMatrix(vec, th, rot);										// ��]��د���쐬
	VecRotate(&wpt, rot);												// ��]�����S�։�]�ړ�
	dth = (wpt.p[d1]<0) ? -dth : dth;									// ��]���̔��Α��͊p�x���t
	if(md) {															// <�~�ʊJ�n�f�ʒ��S�_>
		pb[cnt].p[d1] = wpt.p[d1]*cos(0.0);
		pb[cnt].p[d2] = wpt.p[d2];
		pb[cnt].p[d3] = wpt.p[d1]*sin(0.0);
		ib[cnt++] = 1;
		ath = dth;
	}
	for(i=st; i<ed; ath+=dth, i++) {									// <�f�ʒ��S�_>
		pb[cnt].p[d1] = wpt.p[d1]*cos(ath);								// ����_�z��쐬
		pb[cnt].p[d2] = wpt.p[d2];
		pb[cnt].p[d3] = wpt.p[d1]*sin(ath);
		ib[cnt++] = 0;													// �Ȑ�Ӱ�ސݒ�
	}
	if(md) {															// <�~�ʏI���f�ʒ��S�_>
		pb[cnt].p[d1] = wpt.p[d1]*cos(ath);
		pb[cnt].p[d2] = wpt.p[d2];
		pb[cnt].p[d3] = wpt.p[d1]*sin(ath);
		ib[cnt++] = 1;
	}
	SetRotateMatrix(vec, -th, rot);										// ��]��د���쐬
	for(i=0; i<cnt; i++) {
		VecRotateMove(&pb[i], rot, p1);									// ���ʒu�։�]�ړ�
	}
	SetCurveMatrix1(1, wt, bc, cnt, ib, ma, mb);						// �ʉߓ_=>����_�ϊ���د��
	CalcCurvePoint(1, cnt, pb, wk, mb);									// �ʉߓ_=>����_�ϊ�
	for(i=0; i<cnt; i++) {
		pb[i] = wk[i];
	}
	for(i=0; i<an; i++) {
		pb[cnt+i] = pa[i], ib[cnt+i] = ia[i];							// �f�ʐ���_���ǉ�
	}
	delete[] wk;
	delete[] ma;
	delete[] mb;
}

/*********************************************************************/
void CalcRevolvePoint(int fg, int sw, int af, int bf, int ap, int an,
                      int ac, int bn, int bc, double th, PNTTYPE p1,
                      PNTTYPE p2, PNTTYPE p3, PNTTYPE* pa, int* ia)		//<<< ��]�̐���_�v�Z
/*********************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	int*     ib = new int[View->m_MaxSCtA];
	int      f, n1, n2, rp, sm, bp;
	double   wt;

	if(!sw) {															// <<�ʏ�Ȑ�>>
		if(fg==2) {														// <�I����>
			if(bf==1) {													// ��]���S�_�L:f=3
				f = 3;
			} else if(!ac) {											// ��]���S�_��&���E�J:f=2
				f = 2;
			} else {													// ��]���S�_��&���E��:f=1
				f = 1;
			}
			n1 = 2*af+bn, n2 = 2*bf+an;									// ����_��
		} else {														// <�I�����ȊO>
			f = 0;														// �I�����ȊO:f=0
			n1 = bn, n2 = an;											// ����_��
		}
		wt = View->GetCurveWeight();									// �Ȑ��d�݌W���擾
		Revolve(f, ap, wt, an, pa, ia, p1, p2, th, bn, af, pb, ib);		// ��]����_����
		if(!fg) {
			rp = View->GetCurveRepeat();								// �Ȑ��������擾
			sm = View->GetSolidMode();									// �د��Ӱ�ގ擾
			CrtSurface(bn, an, bc, ac, rp, wt, sm, pb, ib);				// �Ȗʐ���(����)
		} else {
			RedoCrtSurface(n1, n2, bc, ac, pb, ib);						// �Ȗʐ���(2��ڈȍ~)
		}
	} else {															// <<�|���Ȑ�>>
		bp = (ap+1>2) ? 0 : ap+1;										// �|������
		wt = View->GetCurveWeight();									// �Ȑ��d�݌W���擾
		SweepRev(p3, ap, wt, an, pa, ia, p1, p2, th, bn, pb, ib);		// ��]�p�|���Ȗʐ���
		if(!fg) {
			rp = View->GetCurveRepeat();								// �Ȑ��������擾
			sm = View->GetSolidMode();									// �د��Ӱ�ގ擾
			CrtSweepSurf(bn, an, bc, ac, rp, wt, sm, bp, pb, ib);		// �|���Ȗʐ���(����)
		} else {
			RedoCrtSweepSurf(bn, an, bc, ac, pb, ib);					// �|���Ȗʐ���(2��ڈȍ~)
		}
	}
	delete[] pb;
	delete[] ib;
}

/****************************************************************/
static void SetSRevolve(int plane, int hit, int an, PNTTYPE* pa,
                        int* ia, PNTTYPE* p1, PNTTYPE* p2)				//<<< �د���ް��쐬
/****************************************************************/
{
	PNTTYPE spa;
	int     sia, i, j, ix, cnt=-1;
	double  dd;

	ix = (plane-1<0) ? 2 : plane-1;										// �Ώە��ʊO�̍��W
	dd = p1->p[ix];														// ���W�l�ۑ�
	for(i=0; i<an; i++) {
		if(ia[0]!=0&&ia[1]!=0) {										// �A���ܐ�����UP
			cnt++;
		}
		spa = pa[0], sia = ia[0];										// ���݂̊m�F�ʒu��
		for(j=1; j<an; j++) {											// �ؖڂ�1�O�ɼ��
			pa[j-1] = pa[j], ia[j-1] = ia[j];
		}
		pa[an-1] = spa, ia[an-1] = sia;
		if(hit==cnt) {													// <�w��A���ܐ��̏ꍇ>
			*p1 = pa[0];												// �n�_->��]��1�_
			*p2 = pa[an-1];												// �I�_->��]��1�_
			p1->p[ix] = dd, p2->p[ix] = dd;								// ���ʊO���W����
			break;
		}
	}
}

/******************************************************************/
void SetSRevolvePoint(int af, int ap, int no, int an, int bn,
                      double th, PNTTYPE* pa, int* ia, PNTTYPE pt)		//<<< ��]�̿د�ސ���_�쐬
/******************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	int*     ib = new int[View->m_MaxSCtA];
	PNTTYPE  p1, p2;
	int      rp, sm;
	double   wt;

	p1 = pt;
	rp = View->GetCurveRepeat();										// �Ȑ��������擾
	wt = View->GetCurveWeight();										// �Ȑ��d�݌W���擾
	sm = View->GetSolidMode();											// �د��Ӱ�ގ擾
	SetSRevolve(ap, no, an, pa, ia, &p1, &p2);							// �د���ް��쐬
	Revolve(1, ap, wt, an, pa, ia, p1, p2, th, bn, af, pb, ib);			// ��]����_����
	CrtSurface(bn, an, 1, 0, rp, wt, sm, pb, ib);						// �Ȗʐ���
	delete[] pb;
	delete[] ib;
}

/********************************************************/
BOOL GetRevolveInfo(int f1, int f2, double* th, int* rd)				//<<< ��]�̏��擾
/********************************************************/
{
	BOOL ret=TRUE;

	if(f1!=0) {															// <���l���͂̏ꍇ>
		CInputDlg dlg(6);
		dlg.SetValue(*th);												// �����l�\��
		if(dlg.DoModal()==IDOK) {										// �����޲�۸ޕ\��
			dlg.GetValue(th);											// �����p�x
		} else {
			ret = FALSE;
		}
	}
	if(*th>0&&ret) {													// <�L�������p�x>
		if(!f2) {														// �޲�۸ޏȗ����̏ꍇ
			CInputDlg dlg(11);
			dlg.SetValue(*rd);											// �����l�\��
			if(dlg.DoModal()==IDOK) {									// �޲�۸ޕ\��
				dlg.GetValue(rd);										// ��]������
			} else {
				ret = FALSE;
			}
		}
	}
	return ret;
}

/**************************************************************/
void InitPyramid(int plane, int an, PNTTYPE* pa, PNTTYPE* pnt)			//<<< ����������
/**************************************************************/
{
	PNTTYPE ocnt, dcnt;
	int     i;

	CalcCent(pa, an, &ocnt);											// 2D���S�_�擾
	View->GetCentPnt(&dcnt);											// ��ʒ��S�擾
	i = (plane-1<0) ? 2 : plane-1;										// 2D���ʖ@������
	dcnt.p[i] += View->GetDispHeight()/2.0;								// ��ʐ������[�_
	ocnt.p[i] = (ocnt.p[i]+dcnt.p[i])/2.0;								// 2D���S&�[�_����
	*pnt = ocnt;
}

/**************************************************************/
void Pyramid(int flg, int plane, int an, PNTTYPE* pa, int* ia,
             PNTTYPE pnt, int div, PNTTYPE* pb, int* ib)				//<<< ������_����
/**************************************************************/
{
	PNTTYPE pt;
	int     un, ix, i, j, k;

	ix = (plane-1<0) ? 2 : plane-1;										// 2D���ʖ@������
	if(flg>0) {															// <<�ŏI���s�̏ꍇ>>
		flg--;
		if(pa[0].p[ix]>pnt.p[ix]) {										// �������͖�ٰ�ߔ��]
			ChangeLoop(an, pa, ia);
		}
	}
	un = div+flg+2;														// U��������_��
	for(i=0; i<an; i++) {												// <V����_����>
		for(j=0; j<DIM; j++) {
			pt.p[j] = pnt.p[j]-pa[i].p[j];								// �n�I�_�ԕψʗ�
			pt.p[j] = pt.p[j]/(div+1);									// �}�������ɕ���
		}
		if(flg) {														// <<2D���S�L�̏ꍇ>>
			pb[un*i] = pa[an], ib[un*i] = ia[i]+1;						// 2D���S, U�擪Ӱ��
		}
		pb[un*i+flg] = pa[i], ib[un*i+flg] = ia[i]+1;					// U�擪����_, U�擪Ӱ��
		for(j=1; j<=div; j++) {
			for(k=0; k<DIM; k++) {
				pb[un*i+j+flg].p[k] = pa[i].p[k]+pt.p[k]*j;				// �}��������_
			}
			ib[un*i+j+flg] = ia[i];										// �}����Ӱ��
		}
		pb[un*i+div+flg+1] = pnt, ib[un*i+div+flg+1] = ia[i]+1;			// U�ŏI����_, U�ŏIӰ��
	}
}

/*****************************************************************/
void CreatSpiralObject(int pl, int vn, int vb, int fg, double dm,
                       double hg, int cn, int rn, PNTTYPE cp,
                       PNTTYPE sp, PNTTYPE* pa, int* ia)				//<<< �����Ȗʐ���
/*****************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	int*     ib = new int[View->m_MaxSCtA];
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  bpt, pnt;
	VECTYPE  vec;
	int      i, j, k, i1, i2, i3, un, mx, sm;
	double   rot[3][3], th, sth=0.0, mi1, mi2, ma1, ma2, ct1, ct2, wt;

	i1 = pl;															// �f�ʕ��ʐݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	un = 2*fg+cn*rn+1;													// U����_��
	mi1 = DBL_MAX, mi2 = DBL_MAX;
	ma1 = 1.0-DBL_MAX, ma2 = 1.0-DBL_MAX;
	for(i=0; i<vn; i++) {
		mi1 = (pa[i].p[i1]<mi1) ? pa[i].p[i1] : mi1;					// �f�ʍŏ���ő�擾
		ma1 = (pa[i].p[i1]>ma1) ? pa[i].p[i1] : ma1;
		mi2 = (pa[i].p[i2]<mi2) ? pa[i].p[i2] : mi2;
		ma2 = (pa[i].p[i2]>ma2) ? pa[i].p[i2] : ma2;
		pa[i].p[i3] = 0.0;												// �f�ʕ��ʊO���W�ر
	}
	cp.p[i3] = 0.0;
	ct1 = mi1-dm/2.0, ct2 = (mi2+ma2)/2.0;								// �f�ʊ�_
	for(i=0; i<vn; i++) {
		pa[i].p[i1] -= ct1, pa[i].p[i2] -= ct2;							// ��_�����_�ֈړ�
	}
	cp.p[i1] -= ct1, cp.p[i2] -= ct2;
	bpt = sp;															// �J�n���S�ʒu
	bpt.p[i2] -= hg*rn/2.0;
	th = 2.0*PI/cn;														// 1����_�̉�]�p�x
	hg /= cn;															// 1����_�̍���
	vec.p[i1] = 0.0, vec.p[i2] = -1.0, vec.p[i3] = 0.0;					// ��]�޸��
	if(fg) {															// <�f�ʒ��S����>
		for(k=0; k<DIM; k++) {
			pnt.p[k] = bpt.p[k]+cp.p[k];
		}
		for(j=0; j<vn; j++) {
			pb[j*un] = pnt, ib[j*un] = 3;
		}
	}
	for(j=0; j<vn; j++) {												// <��1�f��>
		for(k=0; k<DIM; k++) {
			pnt.p[k] = bpt.p[k]+pa[j].p[k];
		}
		pb[j*un+fg] = pnt, ib[j*un+fg] = ia[j]+1;
	}
	for(i=1; i<=cn*rn; i++) {											// <��2�f�ʈȍ~>
		sth += th;														// ��]�p�x������������Z
		bpt.p[i2] += hg;
		SetRotateMatrix(vec, sth, rot);									// ��]��د���쐬
		for(j=0; j<vn; j++) {
			pnt = pa[j];
			VecRotateMove(&pnt, rot, bpt);
			pb[j*un+fg+i] = pnt;
			ib[j*un+fg+i] = (i==cn*rn) ? ia[j]+1 : ia[j];
		}
	}
	if(fg) {															// <�f�ʒ��S����>
		VecRotateMove(&cp, rot, bpt);
		for(j=0; j<vn; j++) {
			pb[j*un+un-1] = cp, ib[j*un+un-1] = 3;
		}
	}
	mx = View->GetCurveRepeat();										// �Ȑ����J�Ԑ��擾
	wt = View->GetCurveWeight();										// �Ȑ��d�݌W���擾
	sm = View->GetSolidMode();											// �د��Ӱ�ގ擾
	CrtSurface(un, vn, 0, vb, mx, wt, sm, pb, ib);						// �Ȗʐ���
	GetObjPtr(&op);														// �Ȗʗ���OP�擾
	i = GetObjAtr(op);													// ���̑����擾
	GetObjOif(op, &oif);												// ���̍\������߲���擾
	if(i==20) {
		SetOifName(0, oif, OBJ_NAME03);									// �̍\����(�ʏ�|����)
	} else {
		SetOifName(0, oif, OBJ_NAME04);									//         (�ʏ�|����[�ʑ�])
	}
	OnEdtFlg(1, op);													// �Ȗ�edtflg:ON
	delete[] pb;
	delete[] ib;
}

/******************************************************/
void ReverseSurf(int un, int vn, PNTTYPE* pa, int* ea)					//<<< �Ȗʐ���_���]
/******************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int*     ew = new int[View->m_MaxSCtA];
	int      i, j, cnt=0;

	for(i=0; i<vn; i++) {
		for(j=un-1; j>=0; j--) {
			pw[cnt] = pa[j+un*i], ew[cnt++] = ea[j+un*i];				// U�����̕��т𔽓]
		}
	}
	j = un*vn;
	for(i=0; i<j; i++) {
		pa[i] = pw[i], ea[i] = ew[i];
	}
	delete[] pw;
	delete[] ew;
}

/****************************************************/
BOOL CheckSurface(int n, int *un, int *vn,
                  int* ub, int* vb, int* md, int* p)					//<<< �ȖʘA������
/****************************************************/
{
	int i, err1=0, err2=0;

	for(i=0; i<n; i++) {
		if(ub[i]==1) {													// U���EӰ��:������
			err1 = 1; break;
		}
		if(vb[i]==1) {													// V���EӰ��:������
			err2 = 1; break;
		}
	}
	if(!err1) {
		for(i=1; i<n; i++) {											// U���EӰ��:�J�̏ꍇ
			err1 = (vn[0]!=vn[i]||vb[0]!=vb[i]) ? 1 : err1;				// ����_����V���E����
		}
	}
	if(!err2) {
		for(i=1; i<n; i++) {											// V���EӰ��:�J�̏ꍇ
			err2 = (un[0]!=un[i]||ub[0]!=ub[i]) ? 1 : err2;				// ����_����U���E����
		}
	}
	if(!err1) {															// ���EӰ��
		*p = 0;
	} else if(!err2) {
		*p = 1, err1 = 0;
	} else {
		*p = 0;
	}
	return err1;
}

/*************************************************************/
void GetSEPoint(int m1, int m2, int n, OBJTYPE** op, int* un,
                int* vn, int* ub, int* vb, PNTTYPE* pa,
                PNTTYPE* pb, SCRTYPE* pc, int* c1, int* c2)				//<<< �n�_�I�_����_�擾
/*************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	CMDTYPE* cmp;
	int      i, j, k, l, i1, i2, i3, i4, i5, i6, ct=0;

	if(m1==0) {															// <<�Ȑ��̏ꍇ>>
		for(i=0; i<n; i++) {
			BaseCmdPtr(1, op[i], &cmp);									// �L���擪����ގ擾
			GetParaPnt(cmp, 0, &pa[i]);									// �n�_����_�擾
			GetParaPnt(cmp, un[i]-1, &pb[i]);							// �I�_����_�擾
			pc[i].p[0] = 0, pc[i].p[1] = 0;								// �n�_��I�_������
			ct += un[i];												// �A����������_��
		}
		*c2 = 1;														// ���Ε�������_��
	} else {															// <<�ʏ�Ȗʂ̏ꍇ>>
		if(m2==0) {														// <U�A���̏ꍇ>
			for(i=0; i<n; i++) {
				i1 = un[i], i2 = vn[i], i3 = ub[i], i4 = vb[i];
				BaseCmdPtr(1, op[i], &cmp);								// �L���擪����ގ擾
				k = i1*i2;												// ������_��
				for(j=0; j<k; j++) {
					GetParaPnt(cmp, j, &p1[j]);							// ����_�擾
				}
				CheckCLine(i3, i4, i1, i2, p1, &i5, &i6);				// ����_���������
				if(i6%2) {												// �n�_����
					pc[i].p[0] = 1, k = 1;
				} else {
					pc[i].p[0] = 0, k = 0;
				}
				if(i6>1) {												// �I�_����
					pc[i].p[1] = 1, l = i1-2;
				} else {
					pc[i].p[1] = 0, l = i1-1;
				}
				for(j=0; j<i2; j++) {
					p2[j] = p1[j*i1+k];
				}
				CalcCent(p2, i2, &pa[i]);								// �n�_�[�ʒ��S�擾
				for(j=0; j<i2; j++) {
					p2[j] = p1[j*i1+l];
				}
				CalcCent(p2, i2, &pb[i]);								// �I�_�[�ʒ��S�擾
				ct += i1;												// �A������_��
			}
			*c2 = i2;													// ���Ε�������_��
		} else {														// <V�A���̏ꍇ>
			for(i=0; i<n; i++) {
				i1 = un[i], i2 = vn[i], i3 = ub[i], i4 = vb[i];
				BaseCmdPtr(1, op[i], &cmp);								// �L���擪����ގ擾
				k = i1*i2;												// ������_��
				for(j=0; j<k; j++) {
					GetParaPnt(cmp, j, &p1[j]);							// ����_�擾
				}
				CheckCLine(i3, i4, i1, i2, p1, &i5, &i6);				// ����_���������
				if(i5%2) {												// �n�_����
					pc[i].p[0] = 1, k = i1;
				} else {
					pc[i].p[0] = 0, k = 0;
				}
				if(i5>1) {												// �I�_����
					pc[i].p[1] = 1, l = i1*(i2-2);
				} else {
					pc[i].p[1] = 0, l = i1*(i2-1);
				}
				for(j=0; j<i1; j++) {
					p2[j] = p1[j+k];
				}
				CalcCent(p2, i1, &pa[i]);								// �n�_�[�ʒ��S�擾
				for(j=0; j<i1; j++) {
					p2[j] = p1[j+l];
				}
				CalcCent(p2, i1, &pb[i]);								// �I�_�[�ʒ��S�擾
				ct += i2;												// �A����������_��
			}
			*c2 = i1;													// ���Ε�������_��
		}
	}
	*c1 = ct;
	delete[] p1;
	delete[] p2;
}

/***************************************************************/
BOOL ConctSEPoint(int n, PNTTYPE* pa, PNTTYPE* pb, SCRTYPE* pd)			//<<< �n�_�I�_����_�A��
/***************************************************************/
{
	SCRTYPE* wd = new SCRTYPE[View->m_MaxCCtA];
	SCRTYPE* er = new SCRTYPE[View->m_MaxCCtA];
	PNTTYPE  pt1, pt2;
	int      i, j, k, l, i1, i2, fg, j1, j2, sw, en=0;
	double   lw, ln;

	for(i=0; i<n; i++) {												// �A���󋵏�����
		pd[i].p[0] = -1, pd[i].p[1] = -1;
	}
	for(i=0; i<n-1; i++) {												// <<�A���񐔕�>>
		ln = DBL_MAX, i1 = -1, i2 = -1;									// �ް�������
		for(j=0; j<n*2; j++) {
			if(j<n) {
				pt1 = pa[j%n], fg = pd[j%n].p[0];
			} else {
				pt1 = pb[j%n], fg = pd[j%n].p[1];
			}
			if(fg<0) {													// <1�_�ڂ����A��>
				for(k=0; k<n*2; k++) {
					if(k<n) {
						pt2 = pa[k%n], fg = pd[k%n].p[0];
					} else {
						pt2 = pb[k%n], fg = pd[k%n].p[1];
					}
					if(j%n!=k%n&&fg<0) {								// <2�_�ڂ����A��>
						for(l=0, sw=1; l<en; l++) {
							if((j==er[l].p[0]&&k==er[l].p[1])||
                               (j==er[l].p[1]&&k==er[l].p[0])) {		// ��ײ�ް��ƈ�v��
								sw = 0; break;							// �����ް�
							}
						}
						if(sw) {										// ��ײ�ް��ȊO
							lw = PntD(&pt1, &pt2);						// 2�_�ԋ���
							if(lw<ln) {									// �ŏ���2�_�ԋ���
								ln = lw, i1 = j, i2 = k;
							}
						}
					}
				}
			}
		}
		if(i1>=0) {
			for(j=0; j<n; j++) {
				wd[j] = pd[j];											// ��Ɨ̈�ɺ�߰
			}
			if(i1<n) {													// ����No
				j1 = wd[i1%n].p[1], wd[i1%n].p[1] = -2;
			} else {
				j1 = wd[i1%n].p[0], wd[i1%n].p[0] = -2;
			}
			j2 = i2%n;													// ٰ�ߕԒn
			if(j1==-1) {
				sw = 1;
			} else {
				for(j=0, sw=0; j<n; j++) {
					for(k=0, l=-1; k<n; k++) {
						if(wd[k].p[0]==j1) {							// �n�_���Ŏ����̂�����
							j1 = wd[k].p[1], wd[k].p[1] = -2;			// ������No
							if(k==j2) {									// ٰ�ߕԒn���B�ʹװ
								sw = -1;
							} else if(j1==-1) {							// ������No���͐���I��
								sw = 1;
							}
							l = 0; break;
						}
					}
					if(l==-1) {											// �n�_���ɂȂ��ꍇ
						for(k=0; k<n; k++) {
							if(wd[k].p[1]==j1) {						// �I�_���Ŏ����̂�����
								j1 = wd[k].p[0], wd[k].p[0] = -2;		// ������No
								if(k==j2) {								// ٰ�ߕԒn���B�ʹװ
									sw = -1;
								} else if(j1==-1) {						// ������No���͐���I��
									sw = 1;
								}
								break;
							}
						}
					}
					if(sw!=0) {
						break;
					}
				}
			}
			if(sw!=-1) {												// <����ȏꍇ>
				if(i1<n) {												// �ŒZ�_�ƘA��
					pd[i1%n].p[0] = i;
				} else {
					pd[i1%n].p[1] = i;
				}
				if(i2<n) {
					pd[i2%n].p[0] = i;
				} else {
					pd[i2%n].p[1] = i;
				}
				en = 0;
			} else {													// <�A�������ٰ�ߕ̏ꍇ>
				i--;
				er[en].p[0] = i1, er[en].p[1] = i2;						// �Y���ް��ȊO����ײ
				if(++en>=n) {
					delete[] wd;
					delete[] er;
					return TRUE;
				}
			}
		}
	}
	delete[] wd;
	delete[] er;
	return FALSE;
}

/*************************************************/
BOOL GetConctObj(int n, SCRTYPE* pd, SCRTYPE* pw)						//<<< �A�����̏��擾
/*************************************************/
{
	int i, j, k, l;

	for(i=0, k=-1; i<n; i++) {
		if(pd[i].p[0]==-1) {											// �n�_���Ő擪���̂�����
			pw[0].p[0] = i, pw[0].p[1] = 0;								// �擪����No�o�^
			k = pd[i].p[1], pd[i].p[1] = -2;							// ������No
			break;
		}
	}
	if(k==-1) {															// �n�_���ɂȂ��ꍇ
		for(i=0; i<n; i++) {
			if(pd[i].p[1]==-1) {										// �I�_���Ő擪���̂�����
				pw[0].p[0] = i, pw[0].p[1] = 1;							// �擪����No�o�^
				k = pd[i].p[0], pd[i].p[0] = -2;						// ������No
				break;
			}
		}
	}
	if(k==-1) {															// �����s�\�Ŵװ
		return TRUE;
	}
	for(i=1; i<n; i++) {
		for(j=0, l=-1; j<n; j++) {
			if(pd[j].p[0]==k) {											// �n�_���Ŏ����̂�����
				pw[i].p[0] = j, pw[i].p[1] = 0;							// ������No�o�^
				k = pd[j].p[1], pd[j].p[1] = -2, l = 0;					// ������No
				break;
			}
		}
		if(l==-1) {														// �n�_���ɂȂ��ꍇ
			for(j=0; j<n; j++) {
				if(pd[j].p[1]==k) {										// �I�_���Ŏ����̂�����
					pw[i].p[0] = j, pw[i].p[1] = 1;						// ������No�o�^
					k = pd[j].p[0], pd[j].p[0] = -2, l = 0;				// ������No
					break;
				}
			}
		}
		if(l==-1) {														// �����s�\�Ŵװ
			return TRUE;
		}
	}
	return FALSE;
}

/**********************************************************/
void ConctCurve(int clf, int n, OBJTYPE** op, SCRTYPE* no)				//<<< �Ȑ��A��
/**********************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCCtA];
	int*     ea = new int[View->m_MaxCCtA];
	CMDTYPE* cmp;
	int      i, j, ono, flg, wn, pn=0, bc, mx;
	double   wt;

	for(i=0; i<n; i++) {
		ono = no[i].p[0];												// ����No
		flg = no[i].p[1];												// �n�_��I�_�׸�
		BaseCmdPtr(1, op[ono], &cmp);									// �L���擪����ގ擾
		wn = GetParaInt(cmp, 0);										// ����_���擾
		if(i==0) {														// <�擪�Ȑ�>
			bc = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			bc = (clf) ? 1 : bc;										// �A���̏ꍇ�͋��E:��
			mx = GetParaInt(cmp, 2);									// �Ȑ����J�Ԑ�
			wt = GetParaInt(cmp, 3)/1000.0;								// �Ȑ��d�݌W��
		}
		if(!flg) {														// <�n�_�A���̏ꍇ>
			for(j=0; j<wn; j++) {										// ������
				GetParaPnt(cmp, j, &pa[pn]);							// ����_�擾
				ea[pn] = GetParaInt(cmp, j+4);							// ײ�Ӱ�ގ擾
				pn++;
			}
		} else {														// <�I�_�A���̏ꍇ>
			for(j=wn-1; j>=0; j--) {									// �~��
				GetParaPnt(cmp, j, &pa[pn]);							// ����_�擾
				ea[pn] = GetParaInt(cmp, j+4);							// ײ�Ӱ�ގ擾
				pn++;
			}
		}
	}
	CrtCurve(pn, bc, mx, wt, pa, ea);									// �Ȑ�����
	delete[] pa;
	delete[] ea;
}

/*********************************************************/
void ConctSurface(int clf, int md, int n,
                  OBJTYPE** op, SCRTYPE* no, SCRTYPE* fa)				//<<< �ʏ�ȖʘA��
/*********************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int*     ea = new int[View->m_MaxSCtA];
	int*     eb = new int[View->m_MaxSCtA];
	int*     ec = new int[View->m_MaxSCtA];
	int*     ew = new int[View->m_MaxSCtA];
	CMDTYPE* cmp;
	int      i, j, k, l, ono, flg, sfg, efg, us, ue, vs, ve, ct;
	int      uw, vw, uu, un=0, vn=0, ub, vb, mx, fg, chg;
	double   wt, m, ln;

	for(i=0; i<n; i++) {
		ono = no[i].p[0];												// ����No
		flg = no[i].p[1];												// �n�_��I�_�׸�
		sfg = fa[i].p[0];												// �n�_/�I�_���������׸�
		efg = fa[i].p[1];
		BaseCmdPtr(1, op[ono], &cmp);									// �L���擪����ގ擾
		uw = GetParaInt(cmp, 0);										// U����_���擾
		vw = GetParaInt(cmp, 1);										// V����_���擾
		if(i==0) {														// <�擪�ʏ�Ȗ�>
			if(md==0) {													// U�A����V����_������
				vn = vw;
			} else {													// V�A����U����_������
				un = uw;
			}
			ub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			vb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			chg = (ub==0&&vb==0) ? 1 : 0;								// �������J�Ϳد��Ӱ�ޕύX
			if(clf) {													// <�A���̏ꍇ>
				if(md==0) {												// U�A����U���E:��
					ub = 1;
				} else {												// V�A����V���E:��
					vb = 1;
				}
			}
			mx = GetParaInt(cmp, 4);									// �Ȑ����J�Ԑ�
			wt = GetParaInt(cmp, 5)/1000.0;								// �Ȑ����d�݌W��
			fg = GetParaInt(cmp, 6);									// �د��Ӱ��
			if(!clf) {													// <�A���łȂ��ꍇ>
				if(!flg) {												// �n�_�A���͎n�_�����L��
					sfg = 0;
				} else {												// �I�_�A���͏I�_�����L��
					efg = 0;
				}
			}
		} else if(i==n-1) {												// <�ŏI�ʏ�Ȗ�>
			if(!clf) {													// <�A���łȂ��ꍇ>
				if(!flg) {												// �n�_�A���͏I�_�����L��
					efg = 0;
				} else {												// �I�_�A���͎n�_�����L��
					sfg = 0;
				}
			}
		}
		us = 0, ue = uw, vs = 0, ve = vw, uu = uw;
		if(md==0) {														// <U�A���̏ꍇ>
			if(sfg) {													// �����_��ΏۊO
				us += 1, uw -= 1;
			}
			if(efg) {
				ue -= 1, uw -= 1;
			}
		} else {														// <V�A���̏ꍇ>
			if(sfg) {													// �����_��ΏۊO
				vs += 1, vw -= 1;
			}
			if(efg) {
				ve -= 1, vw -= 1;
			}
		}
		for(j=vs, ct=0; j<ve; j++) {
			for(k=us; k<ue; k++) {
				GetParaPnt(cmp, uu*j+k, &pa[ct]);						// ����_�擾
				ea[ct++] = GetParaInt(cmp, uu*j+k+7);					// Ӱ�ގ擾
			}
		}
		if(flg) {														// <�I�_�A���̏ꍇ>
			for(j=vw-1, ct=0; j>=0; j--) {
				for(k=uw-1; k>=0; k--) {
					pw[ct] = pa[uw*j+k], ew[ct++] = ea[uw*j+k];			// �~���ɕ��ёւ�
				}
			}
			for(j=0; j<ct; j++) {
				pa[j] = pw[j], ea[j] = ew[j];
			}
		}
		if(i==0) {														// <<<�擪�ʏ�Ȗ�>>>
			for(j=0; j<ct; j++) {
				pc[j] = pa[j], ec[j] = ea[j];
			}
		} else {														// <<<�ȍ~�ʏ�Ȗ�>>>
			if(md==0) {													// <<U�A���̏ꍇ>>
				if(vb) {												// <V:�̏ꍇ>
					for(j=0, ln=DBL_MAX; j<vn; j++) {
						for(k=0, l=j, m=0.0; k<vn; k++) {
							m += PntD(&pb[un*(k+1)-1], &pa[uw*l]);		// �A���ʒu����_�������v
							l = (l+1>=vn) ? 0 : l+1;
						}
						if(m<ln) {										// �ŒZ���v�����_
							ln = m, vs = j;
						}
					}
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// �o�^�ς��ް�
						}
						for(k=0; k<uw; k++) {
							pc[ct] = pa[uw*vs+k];						// �����ް��ǉ�
							ec[ct++] = ea[uw*vs+k];
						}
						vs = (vs+1>=vn) ? 0 : vs+1;
					}
				} else {												// <V:�J�̏ꍇ>
					if(PntD(&pb[un-1], &pa[0])<
                       PntD(&pb[un-1], &pa[uw*(vn-1)])) {				// �n�_�����߂��ꍇ��
						vs = 0, l = 0;									// 0���珸��
					} else {											// �I�_�����߂��ꍇ��
						vs = vn-1, l = 1;								// vn-1����~��
					}
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// �o�^�ς��ް�
						}
						for(k=0; k<uw; k++) {
							pc[ct] = pa[uw*vs+k];						// �����ް��ǉ�
							ec[ct++] = ea[uw*vs+k];
						}
						if(!l) {
							vs++;
						} else {
							vs--;
						}
					}
				}
			} else {													// <<V�A���̏ꍇ>>
				if(ub) {												// <U:�̏ꍇ>
					for(j=0, ln=DBL_MAX; j<un; j++) {
						for(k=0, l=j, m=0.0; k<un; k++) {
							m += PntD(&pb[un*(vn-1)+k], &pa[l]);		// �A���ʒu����_�������v
							l = (l+1>=un) ? 0 : l+1;
						}
						if(m<ln) {										// �ŒZ���v�����_
							ln = m, us = j;
						}
					}
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// �o�^�ς��ް�
						}
					}
					for(j=0; j<vw; j++) {
						for(k=0, l=us; k<un; k++) {
							pc[ct] = pa[un*j+l], ec[ct++] = ea[un*j+l];	// �����ް��ǉ�
							l = (l+1>=un) ? 0 : l+1;
						}
					}
				} else {												// <U:�J�̏ꍇ>
					for(j=0, ct=0; j<vn; j++) {
						for(k=0; k<un; k++) {
							pc[ct] = pb[un*j+k], ec[ct++] = eb[un*j+k];	// �o�^�ς��ް�
						}
					}
					if(PntD(&pb[un*(vn-1)], &pa[0])<
                       PntD(&pb[un*(vn-1)], &pa[un-1])) {				// �n�_�����߂��ꍇ��
						for(j=0; j<vw; j++) {
							for(k=0; k<un; k++) {						// �����ɍ����ް��ǉ�
								pc[ct] = pa[un*j+k];
								ec[ct++] = ea[un*j+k];
							}
						}
					} else {											// �I�_�����߂��ꍇ��
						for(j=0; j<vw; j++) {
							for(k=un-1; k>=0; k--) {					// �~���ɍ����ް��ǉ�
								pc[ct] = pa[un*j+k];
								ec[ct++] = ea[un*j+k];
							}
						}
					}
				}
			}
		}
		if(md==0) {														// U�A���̏ꍇ
			un += uw;
		} else {														// V�A���̏ꍇ
			vn += vw;
		}
		ct = un*vn;
		for(j=0; j<ct; j++) {
			pb[j] = pc[j], eb[j] = ec[j];								// �ޯ̧���ւ�
		}
	}
	fg = (chg==1&&n==1) ? 0 : fg;										// 1���̂ŗ������J�͕K���د��:OFF
	CrtSurface(un, vn, ub, vb, mx, wt, fg, pb, eb);						// �Ȗʐ���
	delete[] pa;
	delete[] pb;
	delete[] pc;
	delete[] pw;
	delete[] ea;
	delete[] eb;
	delete[] ec;
	delete[] ew;
}
