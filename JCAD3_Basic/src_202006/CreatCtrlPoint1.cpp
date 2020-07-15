/////////////////////////////////////////////////////////////////////////////
// �Ȗʐ���_������
// CreatCtrlPoint1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/08 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "CreatCtrlPoint2.h"
#include "EditCtrlPoint1.h"
#include "EditCtrlPoint2.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/**************************************************************/
void SetBasePoint(int pln, int pos, BOXTYPE box, PNTTYPE* bas)			//<<< �f�ʐ������W�ݒ�
/**************************************************************/
{
	bas->p[0] = 0.0, bas->p[1] = 0.0, bas->p[2] = 0.0;
	if(pln==0) {
		if(pos<3) {
			bas->p[0] = box.bp[0].p[0];
		} else if(pos>5) {
			bas->p[0] = box.bp[1].p[0];
		} else {
			bas->p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
		}
		if(pos%3==0) {
			bas->p[1] = box.bp[0].p[1];
		} else if(pos%3==2) {
			bas->p[1] = box.bp[1].p[1];
		} else {
			bas->p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
		}
	} else if(pln==1) {
		if(pos<3) {
			bas->p[2] = box.bp[1].p[2];
		} else if(pos>5) {
			bas->p[2] = box.bp[0].p[2];
		} else {
			bas->p[2] = (box.bp[0].p[2]+box.bp[1].p[2])/2.0;
		}
		if(pos%3==0) {
			bas->p[1] = box.bp[0].p[1];
		} else if(pos%3==2) {
			bas->p[1] = box.bp[1].p[1];
		} else {
			bas->p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
		}
	} else {
		if(pos<3) {
			bas->p[2] = box.bp[1].p[2];
		} else if(pos>5) {
			bas->p[2] = box.bp[0].p[2];
		} else {
			bas->p[2] = (box.bp[0].p[2]+box.bp[1].p[2])/2.0;
		}
		if(pos%3==0) {
			bas->p[0] = box.bp[1].p[0];
		} else if(pos%3==2) {
			bas->p[0] = box.bp[0].p[0];
		} else {
			bas->p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
		}
	}
}

/***********************************************************************/
BOOL InitCreat(int flg, int bc, int an, PNTTYPE* pa,int* ia,int* plane)	//<<< ������������
/***********************************************************************/
{
	PNTTYPE spa;
	VECTYPE vec;
	int     sia, cnt, i, ix, x=0, y=0, z=0, ret;
	double  mad=0.0;

	for(i=1; i<an; i++) {
		x = (fabs(pa[0].p[0]-pa[i].p[0])>EPSILON) ? 1 : x;
		y = (fabs(pa[0].p[1]-pa[i].p[1])>EPSILON) ? 1 : y;
		z = (fabs(pa[0].p[2]-pa[i].p[2])>EPSILON) ? 1 : z;
	}
	CalcNVec(pa, an, &vec);												// �@���޸�َ擾
	ret = (x&&y&&z) ? FALSE : TRUE;										// 3�ʐ}��ɂȂ��ʹװ
	if(!x&&y&&z) {														// 2D�@��/���ʎ擾
		ix = 0;
		*plane = 1;
	} else if(!y&&z&&x) {
		ix = 1;
		*plane = 2;
	} else if(!z&&x&&y) {
		ix = 2;
		*plane = 0;
	} else if(!x&&!y&&z) {
		ix = 0;
		*plane = 1;
	} else if(!z&&!x&&y) {
		ix = 0;
		*plane = 1;
	} else if(!y&&!z&&x) {
		ix = 2;
		*plane = 0;
	} else {
		for(i=0; i<DIM; i++) {
			if(fabs(vec.p[i])>mad) {									// �@���޸�ٍő�����擾
				mad = fabs(vec.p[i]), ix = i;
			}
		}
		*plane = (ix+1>2) ? 0 : ix+1;
	}
	if(flg&&vec.p[ix]>0.0) {											// �޸�ِ��͖�ٰ�ߔ��]
		ChangeLoop(an, pa, ia);
	}
	if(bc) {															// <<���E�̏ꍇ>>
		for(i=0, cnt=0; i<an; i++) {
			cnt = (ia[i]==1) ? cnt+1 : cnt;								// �ܐ��_���m�F
		}
		if(cnt>0) {														// <�ܐ��_�L�̏ꍇ>
			while(1) {													// �擪���ܐ��_�܂ŌJ��
				if(ia[0]==1) {
					break;
				}
				spa = pa[0], sia = ia[0];								// �擪�ް��ۑ�
				for(i=0; i<an-1; i++) {
					pa[i] = pa[i+1], ia[i] = ia[i+1];					// 1�O�ɼ��
				}
				pa[an-1] = spa, ia[an-1] = sia;							// �Ō�ɋ��擪�ݒ�
			}
		}
	} else {															// <<���E���J�̏ꍇ>>
		ia[0] = 1, ia[an-1] = 1;										// �擪&�Ō�͐ܐ��_
	}
	for(i=0; i<an; i++) {
		ia[i] = ia[i]*2;												// Ӱ��2�{
	}
	return ret;
}

/**************************************************************/
void InitCreat2(int flg, int bc, int an, PNTTYPE* pa, int* ia)			//<<< ������������2
/**************************************************************/
{
	PNTTYPE spa;
	int     sia, cnt, i;

	if(flg) {
		ChangeLoop(an, pa, ia);											// ��ٰ�ߔ��]
	}
	if(bc) {															// <<���E�̏ꍇ>>
		for(i=0, cnt=0; i<an; i++) {
			cnt = (ia[i]==1) ? cnt+1 : cnt;								// �ܐ��_���m�F
		}
		if(cnt>0) {														// <�ܐ��_�L�̏ꍇ>
			while(1) {													// �擪���ܐ��_�܂�
				if(ia[0]==1) {
					break;
				}
				spa = pa[0], sia = ia[0];								// �擪�ް��ۑ�
				for(i=0; i<an-1; i++) {
					pa[i] = pa[i+1], ia[i] = ia[i+1];					// ��O�ɼ��
				}
				pa[an-1] = spa, ia[an-1] = sia;							// �Ō�ɋ��擪�ݒ�
			}
		}
	} else {															// <<���E�J�̏ꍇ>>
		ia[0] = 1, ia[an-1] = 1;										// �擪&�Ō�͐ܐ��_
	}
	for(i=0; i<an; i++) {
		ia[i] = ia[i]*2;												// Ӱ��2�{
	}
}

/************************************************************************/
int GetCurveCenter(int plane, int bc, int an, PNTTYPE* pa, PNTTYPE* pnt)//<<< 2D�}�`���S�擾
/************************************************************************/
{
	PNTTYPE cnt, wpa[3];
	VECTYPE vec;
	int     i, d, flg, ret=0;

	an = (bc) ? an-1 : an;												// ���E�͌��Z
	CalcCent(pa, an, &cnt);												// 2D���S�_�擾
	*pnt = cnt;
	d = (plane-1<0) ? 2 : plane-1;										// 2D�@�������擾
	if(bc==1) {															// <<���E�̏ꍇ>>
		ret = 1;														// �����͒��S��2D��
		wpa[0] = pa[0], wpa[1] = pa[1], wpa[2] = cnt;					// <1�_�ڂ̊m�F>
		CalcNVec(wpa, 3, &vec);											// �@���޸�َ擾
		flg = (vec.p[d]>0) ? 1 : 0;										// �޸�ِ���
		for(i=1; i<an-1; i++) {
			wpa[0] = pa[i], wpa[1] = pa[i+1], wpa[2] = cnt;				// <2�_�ڈȍ~�̊m�F>
			CalcNVec(wpa, 3, &vec);										// �@���޸�َ擾
			if(vec.p[d]>0) {											// <�޸�ِ�>
				if(flg!=1) {											// !=1 ==> 2D�O
					ret = 0; break;
				}
			} else {													// <�޸�ٕ�>
				if(flg!=0) {											// !=0 ==> 2D�O
					ret = 0; break;
				}
			}
		}
		wpa[0] = pa[an-1], wpa[1] = pa[0], wpa[2] = cnt;				// <�ŏI�_�m�F>
		CalcNVec(wpa, 3, &vec);											// �@���޸�َ擾
		if(vec.p[d]>0) {												// <�޸�ِ�>
			ret = (flg!=1) ? 0 : ret;									// !=1 ==> 2D�O
		} else {														// <�޸�ٕ�>
			ret = (flg!=0) ? 0 : ret;									// !=0 ==> 2D�O
		}
	}
	return ret;
}

/*********************************************************/
void ChangeRotate2D(int tn, int flg, int an, PNTTYPE* pa)				//<<< 2D�f�ʉ�]�X�V
/*********************************************************/
{
	VECTYPE vec;
	int     i, j, wan;
	double  rot[3][3], th=0.0;

	wan = an+flg;														// ���z��
	CalcNVec(pa, an, &vec);												// �@���޸�َ擾
	for(i=0; i<DIM; i++) {
		if(fabs(vec.p[i])>th) {											// �޸�ٍő�����擾
			th = fabs(vec.p[i]), j = i;
		}
	}
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 0.0, vec.p[j] = -1.0;	// ��]�޸�ِݒ�
	th = PI*tn/2.0;														// ��]�p�x
	SetRotateMatrix(vec, th, rot);										// ��]��د���쐬
	for(i=0; i<wan; i++) {
		VecRotate(&pa[i], rot);											// 2D�f�ʐ���_��]
	}
}

/******************************************************************/
void InitSweep(PNTTYPE pos, int flg, int pln, int an, PNTTYPE* pa)		//<<< �|����������
/******************************************************************/
{
	VECTYPE vec;
	int     i, wan;
	double  rot[3][3];

	wan = an+flg;														// ���z��
	if(pln==0) {														// <2D����:XY>
		for(i=0; i<wan; i++) {											// Z�l�␳
			pa[i].p[2] = 0.0;
		}
		pos.p[2] = 0.0;
		vec.p[0] = 0.0, vec.p[1] = 1.0, vec.p[2] = 0.0;					// Y����90�x��]
		SetRotateMatrix(vec, PI/2.0, rot);
		for(i=0; i<wan; i++) {
			VecRotate(&pa[i], rot);
		}
		VecRotate(&pos, rot);
	} else if(pln==2) {													// <2D����:ZX>
		for(i=0; i<wan; i++) {											// Y�l�␳
			pa[i].p[1] = 0.0;
		}
		pos.p[1] = 0.0;
		vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 1.0;					// Z����-90�x��]
		SetRotateMatrix(vec, -PI/2.0, rot);
		for(i=0; i<wan; i++) {
			VecRotate(&pa[i], rot);
		}
		VecRotate(&pos, rot);
	}
	pos.p[0] = 0.0;														// X�l�␳
	for(i=0; i<wan; i++) {												// 2D����_�ʒu�ֈړ�
		pa[i].p[0] = 0.0, pa[i].p[1] -= pos.p[1], pa[i].p[2] -= pos.p[2];
	}
}

/***********************************************************/
static void ScaleSection(PNTTYPE* pa, int pn, VECTYPE vec,
                         double th, int d1, int d2, int d3)				//<<< �f�ʊg��
/***********************************************************/
{
	VECTYPE rv1, rv2, fvc;
	int     i, r1=0, r2=0;
	double  rot[3][3], t1, t2, sc;

	t1 = sqrt(vec.p[d1]*vec.p[d1]+vec.p[d2]*vec.p[d2]);
	if(t1>EPSILON) {													// <��]����d3�łȂ��ꍇ>
		rv1.p[d1] = -vec.p[d2], rv1.p[d2] = vec.p[d1], rv1.p[d3] = 0.0;
		VecN(&rv1);
		t1 = atan2(t1, vec.p[d3]);
		t1 = (t1<-10||t1>10) ? 0.0 : t1;
		SetRotateMatrix(rv1, -t1, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d3�����֒f�ʉ�]
		}
		r1 = 1;
	}
	CalcNVec(pa, pn, &fvc);												// �f�ʖ@���޸�َ擾
	if(fabs(fvc.p[d1])>EPSILON) {										// <�f�ʂ�d1����łȂ��ꍇ>
		rv2.p[d1] = 0.0, rv2.p[d2] = 0.0, rv2.p[d3] = -1.0;
		t2 = atan2(fvc.p[d1], fvc.p[d2]);
		t2 = (t2<-10||t2>10) ? 0.0 : t2;
		SetRotateMatrix(rv2, -t2, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d1����֒f�ʉ�]
		}
		r2 = 1;
	}
	sc = fabs(1.0/cos(th/2.0));											// �f�ʊg��{��
	for(i=0; i<pn; i++) {
		pa[i].p[d1] = sc*pa[i].p[d1];									// �f�ʊg��
	}
	if(r2) {															// <d1����֒f�ʉ�]>
		SetRotateMatrix(rv2, t2, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d1���ォ��f�ʋt��]
		}
	}
	if(r1) {															// <d3�����֒f�ʉ�]>
		SetRotateMatrix(rv1, t1, rot);
		for(i=0; i<pn; i++) {
			VecRotate(&pa[i], rot);										// d3��������f�ʋt��]
		}
	}
}

/************************************************************/
static void Sweep(int md, int flg, int an, PNTTYPE* pa,
                  int* ia, int bc, int bn, PNTTYPE* pb,
                  int* ib, PNTTYPE* pc, int* ic, double* db)			//<<< �|������_����
/************************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w3 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  wp[3], cpt, pt, wpt;
	VECTYPE  vec, rvc;
	double   rota[3][3], rotb[3][3], th;
	int      i, j, k, wbn;

	if(bn==0) {															// <�ް����w�薳>
		for(i=0; i<an; i++) {
			pc[i] = pa[i], ic[i] = ia[i];								// 2D�}�`�g�p
		}
		delete[] w1;
		delete[] w2;
		delete[] w3;
		return;
	} else if(bn==1) {													// <�ް����w��1�_>
		for(i=0; i<an; i++) {
			for(j=0; j<DIM; j++){
				pc[i].p[j] = pa[i].p[j]+pb[0].p[j];						// 2D�}�`��1�_�ڂɈړ�
			}
			ic[i] = ia[i];
		}
		delete[] w1;
		delete[] w2;
		delete[] w3;
		return;
	}
	for(i=0; i<bn; i++) {
		w3[i] = pb[i];
	}
	CalcCurvePoint(md, bn, pb, w1, db);									// �ʉߓ_=>����_�ϊ�
	for(i=0; i<bn; i++) {
		pb[i] = w1[i];
	}
	for(i=0; i<an; i++) {												// 2D�}�`����
		w1[i] = pa[i];
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] = pb[1].p[i]-pb[0].p[i];								// �ް����޸�َ擾
	}
	VecN(&vec);															// �P���޸�ى�
	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;						// XY���ʏ�̉�]��
	th = atan2(vec.p[1], vec.p[0]);										// XY���ʏ�̉�]�p�x
	th = (th<-10||th>10) ? 0.0 : th;
	if(flg) {
		cpt = pa[an];													// 2D���S�L�͒��S�_������
	}
	SetRotateMatrix(rvc, th, rota);										// ��]��د���쐬
	if(fabs(vec.p[2])>EPSILON) {										// <Z�����̌X������>
		for(i=0; i<an; i++) {
			VecRotate(&w1[i], rota);									// XY���ʏ�̉�]
		}
		if(flg) {
			VecRotate(&cpt, rota);
		}
		th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
		if(th<EPSILON) {
			rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
			th = (vec.p[2]<0.0) ? -PI/2.0 : PI/2.0;
		} else {
			rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;	// Z�������̉�]��
			VecN(&rvc);
			th = atan2(vec.p[2], th);									// Z�������̉�]�p�x
			th = (th<-10||th>10) ? 0.0 : th;
		}
		SetRotateMatrix(rvc, -th, rota);								// ��]��د���쐬
	}
	if(flg) {															// <<<2D���S�L�̏ꍇ>>>
		wbn = bn+2;														// ���[�_�ǉ�
		wpt = cpt;
		VecRotateMove(&wpt, rota, pb[0]);								// �ް����n�_�����ɉ�]�ړ�
		for(i=0; i<an; i++) {											// <<�ް����n�_����>>
			pc[i*wbn] = wpt, ic[i*wbn] = ia[i]+1;						// �n�_���S�_
			pt = w1[i];
			VecRotateMove(&pt, rota, pb[0]);							// �ް����n�_�����ɉ�]�ړ�
			pc[i*wbn+1] = pt, ic[i*wbn+1] = ia[i]+1;
			VecRotate(&w1[i], rota);									// �n�_�����ɉ�]
		}
		VecRotate(&cpt, rota);											// �n�_�����ɉ�]
		for(i=1; i<bn-1; i++) {											// <<�ް������ԓ_����>>
			CalcRotAngleVec(&pb[i-1], &th, &vec);						// �O��_�̊p�x�擾
			SetRotateMatrix(vec, th/2.0, rota);							// ����_�p��د��
			SetRotateMatrix(vec, th, rotb);								// ����_�p��د��
			if(!ib[i]) {												// <�|���������Ȑ��_>
				for(j=0; j<an; j++) {
					pt = w1[j];
					VecRotateMove(&pt, rota, pb[i]);					// �O��_���Ԋp�x�ɉ�]�ړ�
					pc[j*wbn+i+1] = pt, ic[j*wbn+i+1] = ia[j]+ib[i];
					VecRotate(&w1[j], rotb);							// ���_�����ɉ�]
				}
			} else {													// <�|���������ܐ��_>
				for(j=0; j<an; j++) {
					w2[j] = w1[j];
					VecRotate(&w2[j], rota);							// �O��_���Ԋp�x�ɉ�]
					VecRotate(&w1[j], rotb);							// ���_�����ɉ�]
				}
				ScaleSection(w2, an, vec, th, 0, 1, 2);					// �f�ʊg��
				for(j=0; j<an; j++) {									// �|���_�Ɉړ�
					for(k=0; k<DIM; k++) {
						pc[j*wbn+i+1].p[k] = w2[j].p[k]+pb[i].p[k];
					}
					ic[j*wbn+i+1] = ia[j]+ib[i];
				}
			}
			VecRotate(&cpt, rotb);										// ���_�����ɉ�]
		}
		for(j=0; j<DIM; j++) {
			cpt.p[j] += pb[bn-1].p[j];									// �I�_�ړ�
		}
		for(i=0; i<an; i++) {											// <<�ް����I�_����>>
			pt = w1[i];
			for(j=0; j<DIM; j++) {
				pt.p[j] += pb[bn-1].p[j];								// �I�_�ړ�
			}
			pc[i*wbn+bn] = pt, ic[i*wbn+bn] = ia[i]+1;					// �I�_���S�_
			pc[i*wbn+bn+1] = cpt, ic[i*wbn+bn+1] = ia[i]+1;
		}
	} else {															// <<<2D���S���̏ꍇ>>>
		for(i=0; i<an; i++) {											// <<�ް����n�_����>>
			pt = w1[i];
			VecRotateMove(&pt, rota, pb[0]);							// �ް����n�_�����ɉ�]�ړ�
			pc[i*bn] = pt, ic[i*bn] = ia[i]+1;
			VecRotate(&w1[i], rota);									// �n�_�����ɉ�]
		}
		for(i=1; i<bn-1; i++) {											// <<�ް������ԓ_����>>
			CalcRotAngleVec(&pb[i-1], &th, &vec);						// �O��_�̊p�x�擾
			SetRotateMatrix(vec, th/2.0, rota);							// ����_�p��د��
			SetRotateMatrix(vec, th, rotb);								// ����_�p��د��
			if(!ib[i]) {												// <�|���������Ȑ��_>
				for(j=0; j<an; j++) {
					pt = w1[j];
					VecRotateMove(&pt, rota, pb[i]);					// �O��_���Ԋp�x�ɉ�]�ړ�
					pc[j*bn+i] = pt, ic[j*bn+i] = ia[j]+ib[i];
					VecRotate(&w1[j], rotb);							// ���_�����ɉ�]
				}
			} else {													// <�|���������ܐ��_>
				for(j=0; j<an; j++) {
					w2[j] = w1[j];
					VecRotate(&w2[j], rota);							// �O��_���Ԋp�x�ɉ�]
					VecRotate(&w1[j], rotb);							// ���_�����ɉ�]
				}
				ScaleSection(w2, an, vec, th, 0, 1, 2);					// �f�ʊg��
				for(j=0; j<an; j++) {									// �|���_�Ɉړ�
					for(k=0; k<DIM; k++) {
						pc[j*bn+i].p[k] = w2[j].p[k]+pb[i].p[k];
					}
					ic[j*bn+i] = ia[j]+ib[i];
				}
			}
		}
		if(!bc) {														// <<<���E:�J�̏ꍇ>>>
			for(i=0; i<an; i++) {										// <<�ް����I�_����>>
				pt = w1[i];
				for(j=0; j<DIM; j++) {
					pt.p[j] += pb[bn-1].p[j];							// �I�_�ړ�
				}
				pc[i*bn+bn-1] = pt, ic[i*bn+bn-1] = ia[i]+1;
			}
		} else {														// <<<���E:�̏ꍇ>>>
			wp[0] = pb[bn-2], wp[1] = pb[bn-1], wp[2] = pb[0];			// �I�_�Ǝn�_��A��
			CalcRotAngleVec(wp, &th, &vec);								// �O��_�̊p�x�擾
			SetRotateMatrix(vec, th/2.0, rota);							// ����_�p��د��
			SetRotateMatrix(vec, th, rotb);								// ����_�p��د��
			if(!ib[bn-1]) {												// <�|���������Ȑ��_>
				for(i=0; i<an; i++) {
					pt = w1[i];
					VecRotateMove(&pt, rota, pb[bn-1]);					// �I���f��
					pc[i*bn+bn-1] = pt, ic[i*bn+bn-1] = ia[i]+ib[bn-1];
					VecRotate(&w1[i], rotb);							// ���_�����ɉ�]
				}
			} else {													// <�|���������ܐ��_>
				for(i=0; i<an; i++) {
					w2[i] = w1[i];
					VecRotate(&w2[i], rota);							// �O��_���Ԋp�x�ɉ�]
					VecRotate(&w1[i], rotb);							// ���_�����ɉ�]
				}
				ScaleSection(w2, an, vec, th, 0, 1, 2);					// �f�ʊg��
				for(i=0; i<an; i++) {									// �I���f��
					for(j=0; j<DIM; j++) {
						pc[i*bn+bn-1].p[j] = w2[i].p[j]+pb[bn-1].p[j];
					}
					ic[i*bn+bn-1] = ia[i]+ib[bn-1];
				}
			}
			wp[0] = pb[bn-1], wp[1] = pb[0], wp[2] = pb[1];				// �I�_�Ǝn�_��A��
			CalcRotAngleVec(wp, &th, &vec);								// �O��_�̊p�x�擾
			SetRotateMatrix(vec, th/2.0, rota);							// ����_�p��د��
			if(!ib[0]) {												// <�|���������Ȑ��_>
				for(i=0; i<an; i++) {
					pt = w1[i];
					VecRotateMove(&pt, rota, pb[0]);					// �J�n�f��
					pc[i*bn] = pt, ic[i*bn] = ia[i]+ib[0];
				}
			} else {													// <�|���������ܐ��_>
				for(i=0; i<an; i++) {
					VecRotate(&w1[i], rota);							// �O��_���Ԋp�x�ɉ�]
				}
				ScaleSection(w1, an, vec, th, 0, 1, 2);					// �f�ʊg��
				for(i=0; i<an; i++) {									// �J�n�f��
					for(j=0; j<DIM; j++) {
						pc[i*bn].p[j] = w1[i].p[j]+pb[0].p[j];
					}
					ic[i*bn] = ia[i]+ib[0];
				}
			}
		}
	}
	for(i=0; i<bn; i++) {
		pb[i] = w3[i];													// �ʉߓ_���ް�����߂�
	}
	delete[] w1;
	delete[] w2;
	delete[] w3;
}

/*******************************************************************/
void CalcSweepPoint(int fg, int sw, int md, int sf, int an, int ac,
                    int bn, PNTTYPE* pa, int* ia, PNTTYPE* pb,
                    int* ib, PNTTYPE* pc, int* ic, double* db)			//<<< �|���̐���_�v�Z
/*******************************************************************/
{
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCCtA*2];
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int*     is = new int[View->m_MaxCCtA*2];
	int      i, rp, sm;
	double   wt;

	if(!sw) {															// <�ʏ�Ȗ�>
		Sweep(md, sf, an, pa, ia, 0, bn, pb, ib, pw, ic, db);			// �|������_����
		if(!fg) {
			rp = View->GetCurveRepeat();								// �Ȑ��������擾
			wt = View->GetCurveWeight();								// �Ȑ��d�݌W���擾
			sm = View->GetSolidMode();									// �د��Ӱ�ގ擾
			CrtSurface(bn, an, 0, ac, rp, wt, sm, pw, ic);				// �Ȗʐ���(����)
		} else {
			RedoCrtSurface(2*sf+bn, an, 0, ac, pw, ic);					// �Ȗʐ���(2��ڈȍ~)
		}
		CalcSurfacePoint(md, 1, 1, bn, an, 0, ac, wt, ic, pw, pc);		// ����_=>�ʉߓ_�ϊ�
	} else {															// <�|���Ȗ�>
		for(i=0; i<bn; i++) {
			ps[i] = pb[i], is[i] = ib[i];								// �|���ް����ݒ�
		}
		is[0] = 1, is[bn-1] = 1;										// �n�_�I�_�͐ܐ�
		for(i=0; i<an; i++) {
			ps[bn+i] = pa[i], is[bn+i] = ia[i];							// �f�ʋȐ��ݒ�
		}
		if(!fg) {
			rp = View->GetCurveRepeat();								// �Ȑ��������擾
			wt = View->GetCurveWeight();								// �Ȑ��d�݌W���擾
			sm = View->GetSolidMode();									// �د��Ӱ�ގ擾
			CrtSweepSurf(bn, an, 0, ac, rp, wt, sm, -1, ps, is);		// �|���Ȗʐ���(����)
		} else {
			RedoCrtSweepSurf(bn, an, 0, ac, ps, is);					// �|���Ȗʐ���(2��ڈȍ~)
		}
		SetSweepCtrl(-1, bn, an, 0, ps, is, pc, ic);					// �|���Ȗʐ���_�ϊ�
	}
	delete[] ps;
	delete[] pw;
	delete[] is;
}

/*************************************************************/
BOOL InsertSweepPoint(int fg, int n1, int n2,
                      int* dv, int* pn, PNTTYPE* pa, int* ea)			//<<< �|���̒��Ԑ���_�}��
/*************************************************************/
{
	PNTTYPE pnt;
	BOOL    ret=TRUE;
	int     i, j, dn;

	if(!fg) {															// �޲�۸ޏȗ����̏ꍇ
		dn = *dv;
		CInputDlg dlg(10);
		dlg.SetValue(dn);												// �����l�\��
		if(dlg.DoModal()==IDOK) {										// �޲�۸ޕ\��
			dlg.GetValue(&dn);											// �|���������擾
			*dv = dn;
		} else {														// ��ݾق͒��~
			ret = FALSE;
		}
	} else {															// �޲�۸ޏȗ���0
		dn = 0;
	}
	if(ret) {
		if(n1*(2*n2+2+dn)>View->m_MaxSCtl) {							// <<����_�������>>
			AfxMessageBox(IDS_MANY_CTRLP);								// �װү����
		} else {														// <<����_����ȉ�>>
			if(dn>0) {													// <�|��������>0>
				for(i=0; i<DIM; i++) {
					pnt.p[i] = pa[1].p[i]-pa[0].p[i];					// �n�I�_�ԕψʗ�
					pnt.p[i] = pnt.p[i]/(dn+1);							// �}����������
				}
				pa[dn+1] = pa[1], ea[dn+1] = ea[1];						// �I�_����ɼ��
				*pn = 2+dn;												// �V����_��
				for(i=1; i<=dn; i++) {
					for(j=0; j<DIM; j++) {
						pa[i].p[j] = pa[0].p[j]+pnt.p[j]*i;				// �}��������_�ǉ�
					}
					ea[i] = 0;
				}
			}
		}
	}
	return ret;
}

/*********************************************************************/
void SetCircleSection(int sn, int md, double ln, int un, int vn,
                      PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc, int* ic)	//<<< �~�f�ʊ��t
/*********************************************************************/
{
	PNTTYPE* pd = new PNTTYPE[View->m_MaxCCtA];
	int*     id = new int[View->m_MaxCCtA];
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, j, ct=0;
	double   d1, d2, rd;

	if(sn==1&&md==0) {													// <<�|����1�{�̑|����>>
		d1 = 2.0*PI/vn, d2 = PI;										// �J�n�p�x,�����p�x
		for(i=0; i<vn; i++) {
			pd[i].p[0] = 0.0;											// �f�ʐ���_(YZ)
			pd[i].p[1] = ln*cos(d2);
			pd[i].p[2] = ln*sin(d2);
			id[i] = 0, d2 -= d1;
		}
		for(i=0; i<un; i++) {
			for(j=0; j<vn; j++) {
				pc[ct] = pd[j], ic[ct++] = id[j];						// �|���_�����f�ʒǉ�
			}
		}
	} else if(sn==1&&md==1) {											// <<�|����1�{�̿د��>>
		CalcBox(pa, un, &box);											// �|����BOX���ގ擾
		BoxCenter(box, &cpt);											// �ޯ������
		for(i=0, rd=DBL_MAX; i<un; i++) {
			if((d1=PntD(&cpt, &pa[i]))<rd) {							// ���S�_����̍ŒZ����
				rd = d1;
			}
		}
		d1 = PI/(vn-1), d2 = PI/2.0;									// �J�n�p�x,�����p�x
		for(i=0; i<vn; i++) {
			pd[i].p[0] = 0.0;											// �f�ʐ���_(YZ)
			pd[i].p[1] = rd*cos(d2);
			pd[i].p[2] = rd*sin(d2);
			id[i] = 0, d2 -= d1;
		}
		id[0] = 1, id[vn-1] = 1;										// �n�_��I�_�͐ܐ��_
		for(i=0; i<un; i++) {
			d1 = PntD(&cpt, &pa[i])/rd;									// �ŒZ�����Ƃ̔{��
			for(j=0; j<vn; j++) {
				pc[ct] = pd[j];											// �|���_�����f�ʒǉ�
				pc[ct].p[1] = d1*pc[ct].p[1];
				ic[ct++]=id[j];
			}
		}
	} else {															// <<�|����2�{�̑|����>>
		for(i=0; i<un; i++) {
			rd = PntD(&pa[i], &pb[i])/2.0;								// 2�_�Ԃ̋���/2
			d1 = 2.0*PI/vn, d2 = PI;									// �J�n�p�x,�����p�x
			for(j=0; j<vn; j++) {
				pc[ct].p[0] = 0.0;										// �f�ʐ���_(YZ)
				pc[ct].p[1] = rd*cos(d2);
				pc[ct].p[2] = rd*sin(d2);
				ic[ct++] = 0, d2 -= d1;
			}
		}
	}
	delete[] pd;
	delete[] id;
}

/********************************************************************/
void SetCurveSection(int sn, int md, int no, int b1, int b2, int b3,
                     BOXTYPE bx, int vn, int vb, PNTTYPE* pa,
                     int* ia, PNTTYPE* pb, int* ib)						//<<< �Ȑ��f�ʊ��t
/********************************************************************/
{
	PNTTYPE bas;
	VECTYPE vec;
	int     i, j, bn, pln;
	double  rot[3][3], th=0.0, d1, d2;

	InitCreat(1, vb, vn, pa, ia, &pln);									// ������������
	for(i=0; i<vn; i++) {												// �Ȑ�Ӱ�ނ������ɖ߂�
		ia[i] = ia[i]/2;
	}
	if(sn==1&&md==0) {													// <<�|����1�{�̑|����>>
		bn = b1;														// ��ʒuNo
	} else if(sn==1&&md==1) {											// <<�|����1�{�̿د��>>
		bn = 4;															// ��ʒuNo(���ɒ��S)
	} else {															// <<�|����2�{�̑|����>>
		if(b3==1) {
			bn = 4;														// ��ʒuNo
		} else {
			if(b2==0) {
				bn = (b3==0) ? 7 : 1;
			} else if(b2==1) {
				bn = (b3==0) ? 5 : 3;
			} else if(b2==2) {
				bn = (b3==0) ? 3 : 5;
			} else {
				bn = (b3==0) ? 1 : 7;
			}
		}
	}
	SetBasePoint(pln, bn, bx, &bas);									// �f�ʐ�����_�ݒ�
	InitSweep(bas, 0, pln, vn, pa);										// �|����������
	if((sn==1&&md==1)||sn==2) {											// <<�د��/�|����2�{�̏ꍇ>>
		vec.p[0] = -1.0, vec.p[1] = 0.0, vec.p[2] = 0.0;				// ��]��
		if(sn==1&&md==1) {												// <�د�ނ̏ꍇ>
			d1 = pa[vn-1].p[1]-pa[0].p[1];
			d2 = pa[vn-1].p[2]-pa[0].p[2];
			th = atan2(d1, -d2);										// ��]�p�x
			th = (th<-10||th>10) ? 0.0 : th;
		} else {														// <�|����2�{�̏ꍇ>
			if(b2==0) {													// ��]�p�x
				th = PI;
			} else if(b2==1) {
				th = -PI/2.0;
			} else if(b2==2) {
				th = PI/2.0;
			}
		}
		SetRotateMatrix(vec, th, rot);									// ��]��د���쐬
		for(i=0; i<vn; i++) {
			VecRotate(&pa[i], rot);										// ����_�̉�]
		}
		if(sn==1&&md==1) {												// <�د�ނ̏ꍇ>
			for(i=0; i<DIM; i++) {
				bas.p[i] = (pa[0].p[i]+pa[vn-1].p[i])/2.0;				// �n�_-�I�_�̒��ԓ_
			}
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pa[i].p[j] -= bas.p[j];								// ���ԓ_�����_�ֈړ�
				}
			}
		}
	}
	for(i=0; i<vn; i++) {
		pb[vn*no+i] = pa[i], ib[vn*no+i] = ia[i];						// �Y���ʒu�ɒf�ʂ�ݒ�
	}
}

/***************************************************************/
void CalcCurveSection(int sn, int md, int un, int ub,
                      int vn, int vb, PNTTYPE* pa, PNTTYPE* pb,
                      int* ia, PNTTYPE* pc, int* ic)					//<<< �Ȑ����t�f�ʌv�Z
/***************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	int*     fg = new int[View->m_MaxCCtA];
	PNTTYPE  cpt;
	BOXTYPE  box;
	int      i, j, k, ct=0, st, ed, ss;
	double   d1, d2, ln1, ln2;

	if(sn==2) {															// <<�|������:2�̏ꍇ>>
		for(i=0; i<un; i++) {
			if(ia[i]>0) {												// <�f�ʊ��t�ς̏ꍇ>
				for(j=0, ln1=DBL_MAX, ln2=1.0-DBL_MAX; j<vn; j++) {
					if(pc[i*vn+j].p[1]<ln1) {							// �Y���f�ʂ̍ŏ�Y�l
						ln1 = pc[i*vn+j].p[1];
					}
					if(pc[i*vn+j].p[1]>ln2) {							// �Y���f�ʂ̍ő�Y�l
						ln2 = pc[i*vn+j].p[1];
					}
				}
				ln1 = ln2-ln1;											// �Y���f�ʂ�Y��
				ln2 = PntD(&pa[i], &pb[i]);								// �|��������_�ԋ���
				d1 = ln2/ln1;											// Y�����̔{��
				d2 = (ia[i]>1) ? 1.0 : d1;								// Z�����̔{��
				for(j=0; j<vn; j++) {
					pc[i*vn+j].p[1] = d1*pc[i*vn+j].p[1];				// Y�������ޒ���
					pc[i*vn+j].p[2] = d2*pc[i*vn+j].p[2];				// Z�������ޒ���
				}
			}
		}
	} else if(sn==1&&md==1) {											// <<�د�ނ̏ꍇ>>
		for(i=0, ln1=1.0-DBL_MAX; i<un; i++) {
			if(ia[i]>0) {												// <�f�ʊ��t�ς̏ꍇ>
				ln2 = PntD(&pc[i*vn], &pc[(i+1)*vn-1]);					// �n�_-�I�_����
				ln1 = (ln1<ln2) ? ln2 : ln1;							// �n�_-�I�_�ő勗��
			}
		}
		for(i=0; i<un; i++) {
			if(ia[i]>0) {
				pc[i*vn].p[1] = 0.0;									// �ő勗���Ɏn�_�𒲐�
				pc[i*vn].p[2] = ln1/2.0;
				pc[(i+1)*vn-1].p[1] = 0.0;								// �ő勗���ɏI�_�𒲐�
				pc[(i+1)*vn-1].p[2] = -ln1/2.0;
			}
		}
	}
	if(sn>1) {															// <�|����2�{�̏ꍇ>
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				p1[i].p[j] = (pa[i].p[j]+pb[i].p[j])/2.0;				// 2�{���ԓ_��|�����Ƃ���
			}
		}
	} else {
		for(i=0; i<un; i++) {											// 1�{�͂��̂܂ܑ|����
			p1[i] = pa[i];
		}
	}
	for(i=0, st=-1; i<un; i++) {
		if(ia[i]>0) {
			ct++;														// ���t�ϒf�ʐ�����
			st = (st==-1) ? i : st;										// �擪���t�ϒf�ʕۑ�
		}
	}
	if(ub==0||ct==1) {													// <�|��:�J/���t��:1�̏ꍇ>
		for(i=0; i<st; i++) {
			for(j=0; j<vn; j++) {
				pc[i*vn+j] = pc[st*vn+j], ic[i*vn+j] = ic[st*vn+j];		// �擪���t�ϒf�ʂ𕡎�
			}
			fg[i] = ia[st];												// ���ޒ����׸�
		}
	} else {															// �ȊO�͐擪���t�ϒf�ʕۑ�
		ss = st;
	}
	while(TRUE) {
		for(i=st+1, ed=-1, ln1=0.0; i<un; i++) {
			ln1 += PntD(&p1[i-1], &p1[i]);								// �|�����ԋ������v
			if(ia[i]>0) {												// �����t�ϒf�ʕۑ�
				ed = i; break;
			}
		}
		if(ed==-1) {													// �����t�ϒf�ʖ��͒��~
			ed = st; break;
		}
		if(st+1<ed) {
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					p2[i].p[j] = pc[ed*vn+i].p[j]-pc[st*vn+i].p[j];		// �O��̊��t�ϒf�ʂ̕ψʗ�
				}
			}
		}
		for(i=st+1, ln2=0.0; i<ed; i++) {
			ln2 += PntD(&p1[i-1], &p1[i]);								// �|�����ԋ����ݐ�
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pc[i*vn+j].p[k] = pc[st*vn+j].p[k]+					// �O�̊��t�ϒf�ʂ�
                                      p2[j].p[k]*ln2/ln1;				// �ψʗʂ̋����������Z
				}
				ic[i*vn+j] = (ic[st*vn+j]&&ic[ed*vn+j]) ? 1 : 0;		// �O��̋Ȑ�Ӱ��:1��1
			}
			fg[i] = (ia[st]==2&&ia[ed]==2) ? 2 : 1;						// ���ޒ����׸�
		}
		st = ed;														// ���t�ϒf�ʂ���
	}
	if(ub==0||ct==1) {													// <�|��:�J/���t��:1�̏ꍇ>
		for(i=ed+1; i<un; i++) {
			for(j=0; j<vn; j++) {
				pc[i*vn+j] = pc[ed*vn+j], ic[i*vn+j] = ic[ed*vn+j];		// �������t�ϒf�ʂ𕡎�
			}
			fg[i] = ia[ed];												// ���ޒ����׸�
		}
	} else {															// <�|��:��&���t��>2�̏ꍇ>
		for(i=ed+1, ct=0, ln1=0.0; i<un; i++) {
			ln1 += PntD(&p1[i-1], &p1[i]);								// �|�����ԋ������v
			ct++;
		}
		for(i=0; i<=ss; i++) {											// �|�����ԋ������v
			ln1+=(i==0)?PntD(&p1[un-1], &p1[0]):PntD(&p1[i-1], &p1[i]);
			ct++;
		}
		if(ct>1) {
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					p2[i].p[j] = pc[ss*vn+i].p[j]-pc[ed*vn+i].p[j];		// �O��̊��t�ϒf�ʂ̕ψʗ�
				}
			}
		}
		for(i=ed+1, ln2=0.0; i<un; i++) {
			ln2 += PntD(&p1[i-1], &p1[i]);								// �|�����ԋ����ݐ�
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pc[i*vn+j].p[k] = pc[ed*vn+j].p[k]+					// �O�̊��t�ϒf�ʂ�
                                      p2[j].p[k]*ln2/ln1;				// �ψʗʂ̋����������Z
				}
				ic[i*vn+j] = (ic[ed*vn+j]&&ic[ss*vn+j]) ? 1 : 0;		// �O��̋Ȑ�Ӱ��:1��1
			}
			fg[i] = (ia[ed]==2&&ia[ss]==2) ? 2 : 1;						// ���ޒ����׸�
		}
		for(i=0; i<ss; i++) {											// �|�����ԋ������v
			ln2+=(i==0)?PntD(&p1[un-1], &p1[0]):PntD(&p1[i-1], &p1[i]);
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pc[i*vn+j].p[k] = pc[ed*vn+j].p[k]+					// �O�̊��t�ϒf�ʂ�
                                      p2[j].p[k]*ln2/ln1;				// �ψʗʂ̋����������Z
				}
				ic[i*vn+j] = (ic[ed*vn+j]&&ic[ss*vn+j]) ? 1 : 0;		// �O��̋Ȑ�Ӱ��:1��1
			}
			fg[i] = (ia[ed]==2&&ia[ss]==2) ? 2 : 1;						// ���ޒ����׸�
		}
	}
	if(sn==2) {															// <<�|������:2�̏ꍇ>>
		for(i=0; i<un; i++) {
			if(ia[i]==0) {												// <���񊄕t�f�ʂ̏ꍇ>
				for(j=0, ln1=DBL_MAX, ln2=1.0-DBL_MAX; j<vn; j++) {
					if(pc[i*vn+j].p[1]<ln1) {							// �Y���f�ʂ̍ŏ�Y�l
						ln1 = pc[i*vn+j].p[1];
					}
					if(pc[i*vn+j].p[1]>ln2) {							// �Y���f�ʂ̍ő�Y�l
						ln2 = pc[i*vn+j].p[1];
					}
				}
				ln1 = ln2-ln1;											// �Y���f�ʂ�Y��
				ln2 = PntD(&pa[i], &pb[i]);								// �|��������_�ԋ���
				d1 = ln2/ln1;											// Y�����̔{��
				d2 = (fg[i]>1) ? 1.0 : d1;								// Z�����̔{��
				for(j=0; j<vn; j++) {
					pc[i*vn+j].p[1] = d1*pc[i*vn+j].p[1];				// Y�������ޒ���
					pc[i*vn+j].p[2] = d2*pc[i*vn+j].p[2];				// Z�������ޒ���
				}
			}
		}
	} else if(sn==1&&md==1) {											// <<�د�ނ̏ꍇ>>
		CalcBox(pa, un, &box);											// �|����BOX���ގ擾
		BoxCenter(box, &cpt);											// �ޯ������
		for(i=0; i<un; i++) {
			for(j=0, ln1=DBL_MAX, ln2=1.0-DBL_MAX; j<vn; j++) {
				ln1 = (pc[i*vn+j].p[1]<ln1) ? pc[i*vn+j].p[1] : ln1;	// �Y���f�ʂ̍ŏ�Y�l
				ln2 = (pc[i*vn+j].p[1]>ln2) ? pc[i*vn+j].p[1] : ln2;	// �Y���f�ʂ̍ő�Y�l
			}
			ln1 = ln2-ln1;												// �Y���f�ʂ�Y��
			ln2 = PntD(&cpt, &pa[i]);									// ���S�_����̋���
			d1 = ln2/ln1;												// Y�����̔{��
			for(j=0; j<vn; j++) {
				pc[i*vn+j].p[1] = d1*pc[i*vn+j].p[1];					// Y�������ޒ���
			}
		}
	}
	delete[] p1;
	delete[] p2;
	delete[] fg;
}

/***********************************************************/
static void SweepSection(int no, int un, int ub, int vn,
                         PNTTYPE* pa, int* ia, PNTTYPE* pb,
                         int* ib, PNTTYPE* pc, int* ic)					//<<< ���t�f�ʑ|���̐���
/***********************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  wp[3], pt;
	VECTYPE  vec, rvc;
	double   rota[3][3], rotb[3][3], th;
	int      i, j, k, stp;

	for(i=0; i<vn; i++) {
		w1[i] = pb[i];													// 2D�}�`����
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] = pa[1].p[i]-pa[0].p[i];								// �ް����޸�َ擾
	}
	VecN(&vec);															// �P���޸�ى�
	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;						// XY���ʏ�̉�]��
	th = atan2(vec.p[1], vec.p[0]);										// XY���ʏ�̉�]�p�x
	th = (th<-10||th>10) ? 0.0 : th;
	SetRotateMatrix(rvc, th, rota);										// ��]��د���쐬
	if(fabs(vec.p[2])>EPSILON) {										// <Z�����̌X������>
		for(i=0; i<vn; i++) {
			VecRotate(&w1[i], rota);									// XY���ʏ�̉�]
		}
		th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
		if(th<EPSILON) {
			rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
			th = (vec.p[2]<0.0) ? -PI/2.0 : PI/2.0;
		} else {
			rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;	// Z�������̉�]��
			VecN(&rvc);
			th = atan2(vec.p[2], th);									// Z�������̉�]�p�x
			th = (th<-10||th>10) ? 0.0 : th;
		}
		SetRotateMatrix(rvc, -th, rota);								// ��]��د���쐬
	}
	for(i=0, stp=0; i<vn; i++) {										// <<�ް����n�_����>>
		pt = w1[i];
		VecRotateMove(&pt, rota, pa[0]);								// �ް����n�_�����ɉ�]�ړ�
		if(no==0) {
			pc[i*un] = pt, ic[i*un] = ib[i]*2+1, stp = 1;
		}
		VecRotate(&w1[i], rota);										// �n�_�����ɉ�]
	}
	if(!ub&&stp) {
		delete[] w1;
		delete[] w2;
		return;
	}
	for(i=1, stp=0; i<un-1; i++) {										// <<�ް������ԓ_����>>
		CalcRotAngleVec(&pa[i-1], &th, &vec);							// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		SetRotateMatrix(vec, th, rotb);									// ����_�p��د��
		if(!ia[i]) {													// <�|���������Ȑ��_>
			for(j=0; j<vn; j++) {
				pt = w1[j];
				VecRotateMove(&pt, rota, pa[i]);						// �O��_���Ԋp�x�ɉ�]�ړ�
				if(no==i) {
					pc[j*un+i] = pt, ic[j*un+i] = ia[i]+ib[j]*2, stp = 1;
				}
				VecRotate(&w1[j], rotb);								// ���_�����ɉ�]
			}
		} else {														// <�|���������ܐ��_>
			for(j=0; j<vn; j++) {
				w2[j] = w1[j];
				VecRotate(&w2[j], rota);								// �O��_���Ԋp�x�ɉ�]
				VecRotate(&w1[j], rotb);								// ���_�����ɉ�]
			}
			if(no==i) {
				ScaleSection(w2, vn, vec, th, 0, 1, 2);					// �f�ʊg��
				for(j=0; j<vn; j++) {									// �|���_�Ɉړ�
					for(k=0; k<DIM; k++) {
						pc[j*un+i].p[k] = w2[j].p[k]+pa[i].p[k];
					}
					ic[j*un+i] = ia[i]+ib[j]*2;
				}
				stp = 1;
			}
		}
		if(stp) {
			delete[] w1;
			delete[] w2;
			return;
		}
	}
	if(!ub) {															// <<<���E:�J�̏ꍇ>>>
		for(i=0; i<vn; i++) {											// <<�ް����I�_����>>
			pt = w1[i];
			for(j=0; j<DIM; j++) {										// �I�_�ړ�
				pt.p[j] += pa[un-1].p[j];
			}
			pc[(i+1)*un-1] = pt, ic[(i+1)*un-1] = ib[i]*2+1;
		}
	} else {															// <<<���E:�̏ꍇ>>>
		wp[0] = pa[un-2], wp[1] = pa[un-1], wp[2] = pa[0];				// �I�_�Ǝn�_��A��
		CalcRotAngleVec(wp, &th, &vec);									// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		SetRotateMatrix(vec, th, rotb);									// ����_�p��د��
		if(!ia[un-1]) {													// <�|���������Ȑ��_>
			for(i=0; i<vn; i++) {
				pt = w1[i];
				VecRotateMove(&pt, rota, pa[un-1]);						// �I���f��
				pc[(i+1)*un-1] = pt;
				ic[(i+1)*un-1] = ia[un-1]+ib[i]*2;
				VecRotate(&w1[i], rotb);								// ���_�����ɉ�]
			}
		} else {														// <�|���������ܐ��_>
			for(i=0; i<vn; i++) {
				w2[i] = w1[i];
				VecRotate(&w2[i], rota);								// �O��_���Ԋp�x�ɉ�]
				VecRotate(&w1[i], rotb);								// ���_�����ɉ�]
			}
			ScaleSection(w2, vn, vec, th, 0, 1, 2);						// �f�ʊg��
			for(i=0; i<vn; i++) {										// �I���f��
				for(j=0; j<DIM; j++) {
					pc[(i+1)*un-1].p[j] = w2[i].p[j]+pa[un-1].p[j];
				}
				ic[(i+1)*un-1] = ia[un-1]+ib[i]*2;
			}
		}
		if(no!=0) {
			delete[] w1;
			delete[] w2;
			return;
		}
		wp[0] = pa[un-1], wp[1] = pa[0], wp[2] = pa[1];					// �I�_�Ǝn�_��A��
		CalcRotAngleVec(wp, &th, &vec);									// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		if(!ia[0]) {													// <�|���������Ȑ��_>
			for(i=0; i<vn; i++) {
				pt = w1[i];
				VecRotateMove(&pt, rota, pa[0]);						// �J�n�f��
				pc[i*un] = pt;
				ic[i*un] = ia[0]+ib[i]*2;
			}
		} else {														// <�|���������ܐ��_>
			for(i=0; i<vn; i++) {
				VecRotate(&w1[i], rota);								// �O��_���Ԋp�x�ɉ�]
			}
			ScaleSection(w1, vn, vec, th, 0, 1, 2);						// �f�ʊg��
			for(i=0; i<vn; i++) {										// �J�n�f��
				for(j=0; j<DIM; j++) {
					pc[i*un].p[j] = w1[i].p[j]+pa[0].p[j];
				}
				ic[i*un] = ia[0]+ib[i]*2;
			}
		}
	}
	delete[] w1;
	delete[] w2;
}

/**********************************************************************/
static void TwoLineSection(int no, int un, int ub, int vn, int* fg,
                           PNTTYPE* pa, PNTTYPE* pb, int* ia,
                           PNTTYPE* pc, int* ic, PNTTYPE* pd, int* id)	//<<< 2�|�����f�ʊ��t
/**********************************************************************/
{
	PNTTYPE wk[3], cpt, pt1, pt2, ptw;
	VECTYPE vc1, vc2, vc3, vc4, nvc, rvc;
	int     i, j, k, st, ed;
	double  rot[3][3], th, d1, d2;

	if(!ub) {															// �J��,0=>1
		st = 0, ed = 1;
	} else {															// ��,un-1=>1
		st = un-1, ed = 1;
	}
	for(i=0; i<DIM; i++) {
		ptw.p[i] = (pa[ed].p[i]-pa[st].p[i])+
                   (pb[ed].p[i]-pb[st].p[i])+pa[0].p[i];				// ���Z�|�������ʒu
	}
	VecV(&pa[0], &pb[0], &vc1);											// �|��1���|��2�޸��(vc1)
	VecN(&vc1);
	VecV(&pa[0], &ptw, &vc2);											// ���Z�|�������޸��(vc2)
	VecN(&vc2);
	VecO(&vc1, &vc2, &vc3);												// �޸�ق̊O��(vc1,vc2�����޸��)
	VecN(&vc3);
	VecO(&vc1, &vc3, &nvc);												// �޸�ق̊O��(�f�ʖ@���޸��)
	VecN(&nvc);
	for(i=0, j=0, d1=0.0; i<DIM; i++) {
		if((d2=fabs(vc2.p[i]))>d1) {									// ���Z�|�������޸�ٍő����
			d1 = d2, j = i;
		}
	}
	if((vc2.p[j]>0&&nvc.p[j]<0)||(vc2.p[j]<0&&nvc.p[j]>0)) {			// ���Z�|�������޸�قƈقȂ�
		for(i=0; i<DIM; i++) {											// �ꍇ�͒f�ʖ@���޸�ٔ��]
			nvc.p[i] *= -1.0;
		}
	}
	if(no==0) {															// <�擪�f�ʂ̏ꍇ>
		for(i=0, j=0, d1=0.0; i<DIM; i++) {
			cpt.p[i] = (pa[0].p[i]+pb[0].p[i])/2.0;						// �|��1��2�̒��S�_
			if((d2=fabs(vc3.p[i]))>d1) {								// �����޸�ٍő����
				d1 = d2, j = i;
			}
		}
		d2 = PntD(&pa[0], &pb[0]);										// 2�_�ԋ���
		for(i=0; i<DIM; i++) {
			ptw.p[i] = cpt.p[i]+nvc.p[i]*d2;							// �f�ʖ@���޸�ٕ����̓_
		}
		wk[0] = pa[0], wk[1] = pb[0], wk[2] = ptw;						// �|��1+�|��2+�@�������_
		CalcNVec(wk, 3, &vc1);											// �@���޸�َ擾
		*fg = (vc1.p[j]<0) ? 0 : 1;										// ��_�׸�
	}
	pt1.p[0] = 0.0, pt1.p[1] = -1.0, pt1.p[2] = 0.0;					// �f�ʍ�����_
	pt2.p[0] = 0.0, pt2.p[1] = 1.0,  pt2.p[2] = 0.0;					// �f�ʉE����_
	if(fabs(nvc.p[0])>EPSILON||fabs(nvc.p[1])>EPSILON) {				// <<�@���޸�ق�Z���ȊO>>
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// ��]��:Z��
		th = atan2(nvc.p[1], nvc.p[0]);									// ��]�p�x
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rot);									// ��]��د���쐬
		for(i=0; i<vn; i++) {
			VecRotate(&pc[i], rot);										// XY���ʏ�̉�]
		}
		VecRotate(&pt1, rot);											// �f�ʍ�����_�̉�]
		VecRotate(&pt2, rot);											// �f�ʉE����_�̉�]
		if(fabs(nvc.p[2])>EPSILON) {									// <�@���޸�ق�Z�l����>
			rvc.p[0] = -nvc.p[1], rvc.p[1] = nvc.p[0], rvc.p[2] = 0.0;	// ��]��:nvc�ɒ���
			VecN(&rvc);													// �P���޸�ى�
			th = sqrt(nvc.p[0]*nvc.p[0]+nvc.p[1]*nvc.p[1]);				// XY���ʏ�̒���
			th = atan2(nvc.p[2], th);									// ��]�p�x
			th = (th<-10||th>10) ? 0.0 : th;
			SetRotateMatrix(rvc, -th, rot);								// ��]��د���쐬
			for(i=0; i<vn; i++) {
				VecRotate(&pc[i], rot);									// Z�������̉�]
			}
			VecRotate(&pt1, rot);										// �f�ʍ�����_�̉�]
			VecRotate(&pt2, rot);										// �f�ʉE����_�̉�]
		}
	} else {															// <<�@���޸�ق�Z��>>
		rvc.p[0] = 0.0, rvc.p[1] = -1.0, rvc.p[2] = 0.0;				// ��]��:Y��
		th = (nvc.p[2]<0.0) ? PI/2.0 : -PI/2.0;							// ��]�p�x
		SetRotateMatrix(rvc, th, rot);									// ��]��د���쐬
		for(i=0; i<vn; i++) {
			VecRotate(&pc[i], rot);										// Z�������̉�]
		}
		VecRotate(&pt1, rot);											// �f�ʍ�����_�̉�]
		VecRotate(&pt2, rot);											// �f�ʉE����_�̉�]
	}
	for(i=1; i<=no; i++) {
		if(i==un-1&&!ub) {												// �I�_�ŊJ��,un-2=>un-1
			st = un-2, ed = un-1;
		} else if(i==un-1&&ub) {										// �I�_�ŕ�,un-2=>0
			st = un-2, ed = 0;
		} else {														// �ȊO��,i-1=>i+1
			st = i-1, ed = i+1;
		}
		for(j=0; j<DIM; j++) {
			ptw.p[j] = (pa[ed].p[j]-pa[st].p[j])+
                       (pb[ed].p[j]-pb[st].p[j])+pa[i].p[j];			// ���Z�|�������ʒu
		}
		VecV(&pa[i], &pb[i], &vc1);										// �|��1���|��2�޸��(vc1)
		VecN(&vc1);
		VecV(&pa[i], &ptw, &vc2);										// ���Z�|�������޸��(vc2)
		VecN(&vc2);
		VecO(&vc1, &vc2, &vc3);											// �޸�ق̊O��(vc1,vc2�����޸��)
		VecN(&vc3);
		VecO(&vc1, &vc3, &vc4);											// �޸�ق̊O��(�f�ʖ@���޸��)
		VecN(&vc4);
		for(j=0, k=0, d1=0.0; j<DIM; j++) {
			if((d2=fabs(vc2.p[j]))>d1) {								// ���Z�|�������޸�ٍő����
				d1 = d2, k = j;
			}
		}
		if((vc2.p[k]>0&&vc4.p[k]<0)||(vc2.p[k]<0&&vc4.p[k]>0)) {		// ���Z�|�������޸�قƈقȂ�
			for(j=0; j<DIM; j++) {										// �ꍇ�͒f�ʖ@���޸�ٔ��]
				vc4.p[j] *= -1.0;
			}
		}
		if(fabs(nvc.p[0]-vc4.p[0])>EPSILON||
           fabs(nvc.p[1]-vc4.p[1])>EPSILON||
           fabs(nvc.p[2]-vc4.p[2])>EPSILON) {
			th = nvc.p[0]*vc4.p[0]+nvc.p[1]*vc4.p[1]+nvc.p[2]*vc4.p[2];
			th = acos(th);												// ��]�p�x
			th = (th<-10||th>10) ? 0.0 : th;
		} else {
			th = 0.0;
		}
		if(th>EPSILON) {												// <��]�p�x��0����>
			for(j=0; j<DIM; j++) {										// 2�޸�ق̐����O�p�`
				wk[0].p[j] = 0.0;
				wk[1].p[j] = nvc.p[j]*1000.0;
				wk[2].p[j] = vc4.p[j]*1000.0;
			}
			CalcNVec(wk, 3, &rvc);										// �@���޸�َ擾
		} else {
			rvc = vc3;
		}
		SetRotateMatrix(rvc, th, rot);									// ��]��د���쐬
		for(j=0; j<vn; j++) {
			VecRotate(&pc[j], rot);										// vc4���̉�]
		}
		VecRotate(&pt1, rot);											// �f�ʍ�����_�̉�]
		VecRotate(&pt2, rot);											// �f�ʉE����_�̉�]
		nvc = vc4;														// ����@���޸�ٕۑ�
	}
	if(*fg==0) {														// �|������̊���޸��
		VecV(&pa[no], &pb[no], &vc1);
	} else {
		VecV(&pb[no], &pa[no], &vc1);
	}
	VecN(&vc1);															// �P���޸�ى�
	VecV(&pt1, &pt2, &vc2);												// �f�ʏ�̊���޸��
	VecN(&vc2);															// �P���޸�ى�
	if(fabs(vc1.p[0]-vc2.p[0])>EPSILON||
       fabs(vc1.p[1]-vc2.p[1])>EPSILON||
       fabs(vc1.p[2]-vc2.p[2])>EPSILON) {
		th = vc1.p[0]*vc2.p[0]+vc1.p[1]*vc2.p[1]+vc1.p[2]*vc2.p[2];
		th = acos(th);													// ��]�p�x
		th = (th<-10||th>10) ? 0.0 : th;
	} else {
		th = 0.0;
	}
	if(th>EPSILON) {													// <��]�p�x��0����>
		for(i=0; i<DIM; i++) {											// 2�޸�ق̐����O�p�`
			wk[0].p[i] = 0.0;
			wk[1].p[i] = vc2.p[i]*1000.0;
			wk[2].p[i] = vc1.p[i]*1000.0;
		}
		CalcNVec(wk, 3, &rvc);											// �@���޸�َ擾
	} else {
		rvc = nvc;
	}
	SetRotateMatrix(rvc, th, rot);										// ��]��د���쐬
	for(i=0; i<DIM; i++) {
		cpt.p[i] = (pa[no].p[i]+pb[no].p[i])/2.0;						// �|��1��2�̒��S�_
	}
	for(i=0; i<vn; i++) {
		VecRotateMove(&pc[i], rot, cpt);								// vc4���̉�]
		pd[i*un+no] = pc[i];											// �|���̐���_���ݒ�
		id[i*un+no] = ia[no]+ic[i]*2;
	}
}

/****************************************************************/
void CrtSectionSweep(int sn, int md, int un, int ub, int vn,
                     int vb, PNTTYPE* pa, PNTTYPE* pb, int* ia,
                     PNTTYPE* pc, int* ic, PNTTYPE* pd, int* id)		//<<< ���t�f�ʑ|���̐���
/****************************************************************/
{
	PNTTYPE cpt, pnt;
	VECTYPE vec;
	BOXTYPE box;
	int     i, j, pln=0, i1, i2, i3;
	double  rot[3][3], th;

	if(sn==1&&md==0) {													// <<�|����1�{�̑|����>>
		for(i=0; i<un; i++) {
			SweepSection(i, un, ub, vn, pa, ia, &pc[i*vn],
                         &ic[i*vn], pd, id);							// ���t�f�ʑ|���̐���
		}
	} else if(sn==1&&md==1) {											// <<�|����1�{�̿د��>>
		CalcBox(pa, un, &box);											// �|����BOX���ގ擾
		BoxCenter(box, &cpt);											// �ޯ������
		if(fabs(box.bp[0].p[0]-box.bp[1].p[0])<EPSILON) {				// �د�ޑ|�����w�蕽��
			pln = 1;
		} else if(fabs(box.bp[0].p[1]-box.bp[1].p[1])<EPSILON) {
			pln = 2;
		} else if(fabs(box.bp[0].p[2]-box.bp[1].p[2])<EPSILON) {
			pln = 0;
		}
		i1 = pln;														// ��Ɨp���W���ݒ�
		i2 = (i1+1>2) ? 0 : i1+1;
		i3 = (i2+1>2) ? 0 : i2+1;
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				pa[i].p[j] -= cpt.p[j];									// �|�������S�����_�Ɉړ�
			}
		}
		if(pln!=0) {													// <XY���ʈȊO�̏ꍇ>
			vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = -1.0;			// ��]��:Z��
			th = (pln==1) ? -PI/2.0 : PI/2.0;							// YZ�͍�90�x/ZX�͉E90�x
			SetRotateMatrix(vec, th, rot);								// ��]��د���쐬
			for(i=0; i<un*vn; i++) {
				VecRotate(&pc[i], rot);									// �f�ʐ���_��]
			}
			vec.p[0] = (pln==1) ? 0.0 : -1.0;							// YZ���ʂ͉�]��:Y��
			vec.p[1] = (pln==1) ? -1.0 : 0.0;							// ZX���ʂ͉�]��:X��
			vec.p[2] = 0.0;
			SetRotateMatrix(vec, th, rot);								// ��]��د���쐬
			for(i=0; i<un*vn; i++) {
				VecRotate(&pc[i], rot);									// �f�ʐ���_��]
			}
		}
		pnt = pc[0], pnt.p[i1] = 0.0, pnt.p[i2] = 0.0;					// �f�ʎn�_���W�␳
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				pd[i].p[j] = pnt.p[j]+cpt.p[j];							// �f�ʎn�_����_�ݒ�
			}
			id[i] = ic[0]*2;											// �f�ʎn�_�Ȑ�Ӱ�ސݒ�
		}
		pnt = pc[vn-1], pnt.p[i1] = 0.0, pnt.p[i2] = 0.0;				// �f�ʏI�_���W�␳
		for(i=0; i<un; i++) {
			for(j=0; j<DIM; j++) {
				pd[(vn-1)*un+i].p[j] = pnt.p[j]+cpt.p[j];				// �f�ʏI�_����_�ݒ�
			}
			id[(vn-1)*un+i] = ic[vn-1]*2;								// �f�ʏI�_�Ȑ�Ӱ�ސݒ�
		}
		vec.p[i1] = 0.0, vec.p[i2] = 0.0, vec.p[i3] = -1.0;
		for(i=0; i<un; i++) {
			th = atan2(pa[i].p[i1], pa[i].p[i2]);						// ��]�p�x
			th = (th<-10||th>10) ? 0.0 : th;
			SetRotateMatrix(vec, th, rot);								// ��]��د���쐬
			for(j=1; j<vn-1; j++) {
				pnt = pc[i*vn+j];
				VecRotateMove(&pnt, rot, cpt);							// �f�ʐ���_��]�ړ�
				pd[j*un+i] = pnt;										// �f�ʐ���_��Ȑ�Ӱ�ސݒ�
				id[j*un+i] = ic[i*vn+j]*2;
			}
		}
	} else {															// <<�|����2�{�̑|����>>
		if(!ub) {														// �|����:�J�͗��[�ܐ��_
			ia[0] = 1, ia[un-1] = 1;
		}
		for(i=0; i<un; i++) {
			TwoLineSection(i, un, ub, vn, &i1, pa, pb, ia,
                           &pc[i*vn], &ic[i*vn], pd, id);				// 2�|�����f�ʊ��t
		}
	}
}

/******************************************************************/
void AddCenterPoint(int* un, int vn, int vb, PNTTYPE* pa, int* ia)		//<<< �[�ʒ��S�_�ǉ�
/******************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxCCtA];
	int*     ib = new int[View->m_MaxCPnt];
	int*     ic = new int[View->m_MaxCCtA];
	OBJTYPE* op;
	PNTTYPE  pnt;
	VECTYPE  vec;
	int      uu, mx, i, j, i1, i2, i3, i4;
	double   d1, d2, wt;

	uu = *un;															// U����_��
	for(i=0,i1=0; i<vn; i++) {											// �J�n�f���ް����o
		pc[i1] = pa[i*uu], ic[i1++] = ia[i*uu]/2;
	}
	for(i=1, i1=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			if(fabs(pc[0].p[j]-pc[i].p[j])>EPSILON) {					// �J�n�f�ʓ���_����
				i1 = 1; break;
			}
		}
		if(i1) {
			break;
		}
	}
	if(i1) {															// <�J�n�f�ʂ�����_�łȂ�>
		mx = View->GetCurveRepeat();									// �Ȑ����J�Ԑ��擾
		wt = View->GetCurveWeight();									// �Ȑ��d�݌W���擾
		CrtCurve(vn, vb, mx, wt, pc, ic);								// �J�n�f�ʋȐ�����
		GetObjPtr(&op);													// �J�n�f�ʋȐ�OP�擾
		i2 = GetVtxNum(op);												// �J�n�f�ʒ��_���擾
		for(i=0; i<i2; i++) {
			GetVtx(op, i, &pb[i]);										// �J�n�f�ʋȐ����_�擾
		}
		FreeObj(op, 1);													// �J�n�f�ʋȐ��폜
		CalcNVec(pb, i2, &vec);											// �J�n�f�ʖ@���޸�َ擾
		for(i=0, i3=0, d1=0.0; i<DIM; i++) {
			if((d2=fabs(vec.p[i]))>d1) {								// �@���޸�ق̍ő����
				d1 = d2, i3 = i;
			}
		}
		i3 = (i3+1>2) ? 0 : i3+1;										// �J�n�f�ʂ̋ߎ�����No
		if(GetCurveCenter(i3, vb, i2, pb, &pnt)) {						// <�J�n�f�ʒ��S�_����>
			for(i=0, i4=0; i<vn; i++) {
				pb[i4] = pnt;											// �J�n�f�ʒ��S�_��ǉ�
				ib[i4++] = ic[i]*2+1;
				for(j=0; j<uu; j++) {
					pb[i4] = pa[i*uu+j], ib[i4++] = ia[i*uu+j];			// ����_�����Đݒ�
				}
			}
			uu += 1;													// U����_�������Z
			for(i=0; i<uu*vn; i++) {									// �Ȗʐ���_�𕜌�
				pa[i] = pb[i], ia[i] = ib[i];
			}
		}
	}
	for(i=0, i1=0; i<vn; i++) {											// �I���f���ް����o
		pc[i1] = pa[(i+1)*uu-1], ic[i1++] = ia[(i+1)*uu-1]/2;
	}
	for(i=1, i1=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			if(fabs(pc[0].p[j]-pc[i].p[j])>EPSILON) {					// �I���f�ʓ���_����
				i1 = 1; break;
			}
		}
		if(i1) {
			break;
		}
	}
	if(i1) {															// <�I���f�ʂ�����_�łȂ�>
		mx = View->GetCurveRepeat();									// �Ȑ����J�Ԑ��擾
		wt = View->GetCurveWeight();									// �Ȑ��d�݌W���擾
		CrtCurve(vn, vb, mx, wt, pc, ic);								// �I���f�ʋȐ�����
		GetObjPtr(&op);													// �I���f�ʋȐ�OP�擾
		i2 = GetVtxNum(op);												// �I���f�ʒ��_���擾
		for(i=0; i<i2; i++) {
			GetVtx(op, i, &pb[i]);										// �I���f�ʋȐ����_�擾
		}
		FreeObj(op, 1);													// �I���f�ʋȐ��폜
		CalcNVec(pb, i2, &vec);											// �I���f�ʖ@���޸�َ擾
		for(i=0, i3=0, d1=0.0; i<DIM; i++) {
			if((d2=fabs(vec.p[i]))>d1) {								// �@���޸�ق̍ő����
				d1 = d2, i3 = i;
			}
		}
		i3 = (i3+1>2) ? 0 : i3+1;										// �I���f�ʂ̋ߎ�����No
		if(GetCurveCenter(i3, vb, i2, pb, &pnt)) {						// <�I���f�ʒ��S�_����>
			for(i=0, i4=0; i<vn; i++) {
				for(j=0; j<uu; j++) {
					pb[i4] = pa[i*uu+j], ib[i4++] = ia[i*uu+j];			// ����_�����Đݒ�
				}
				pb[i4] = pnt, ib[i4++] = ic[i]*2+1;						// �I���f�ʒ��S�_��ǉ�
			}
			uu += 1;													// U����_�������Z
			for(i=0; i<uu*vn; i++) {									// �Ȗʐ���_�𕜌�
				pa[i] = pb[i], ia[i] = ib[i];
			}
		}
	}
	*un = uu;															// U����_��
	delete[] pb;
	delete[] pc;
	delete[] ib;
	delete[] ic;
}

/*******************************************************************/
void CreatSetCurve(int tn, int sd, int sd1, int sw, int pl, int ps)		//<<< �Ȑ��g����
/*******************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pb = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* blp = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* cpt = new PNTTYPE[100];
	PNTTYPE* cpw = new PNTTYPE[100];
	int*     ia = new int[View->m_MaxCCtA];
	int*     ib = new int[View->m_MaxCCtA];
	int*     ic = new int[View->m_MaxSCtA];
	int*     bli = new int[View->m_MaxSCtA];
	int*     cpi = new int[100];
	OBJTYPE* op1, * op2, * op3;
	OIFTYPE* oif;
	CMDTYPE* cmp1, * cmp2;
	PNTTYPE  pnt, bas;
	BOXTYPE  box;
	int      i, j, i1, i2, i3, mx, sm, un, vn, ub, vb, vpl, ad=0, ct1=0, ct1w, ct2;
	double   wk[10], wt, mi1, mi2, mi3, mi4, ma1, ma2, ma3, ma4;

	i1 = pl;															// �f�ʕ��ʐݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	mi1 = DBL_MAX, mi2 = DBL_MAX, mi3 = DBL_MAX, mi4 = DBL_MAX;
	ma1 = 1.0-DBL_MAX, ma2 = 1.0-DBL_MAX;
	ma3 = 1.0-DBL_MAX, ma4 = 1.0-DBL_MAX;
	SelBaseObjPtr(1, &op1);												// �f�ʋȐ��擪�߲���擾
	while(op1!=NULL) {
		GetObjBox(op1, &box);											// ����BOX���ގ擾
		pnt.p[i1] = (box.bp[0].p[i1]+box.bp[1].p[i1])/2.0;				// ���̂̒��S�_
		pnt.p[i2] = (box.bp[0].p[i2]+box.bp[1].p[i2])/2.0;
		pnt.p[i3] = 0.0;
		cpt[ct1] = pnt, cpi[ct1++] = 0;
		mi1 = (box.bp[0].p[i1]<mi1) ? box.bp[0].p[i1] : mi1;			// �S�f�ʂ�BOX���ގ擾
		ma1 = (box.bp[1].p[i1]>ma1) ? box.bp[1].p[i1] : ma1;
		mi2 = (box.bp[0].p[i2]<mi2) ? box.bp[0].p[i2] : mi2;
		ma2 = (box.bp[1].p[i2]>ma2) ? box.bp[1].p[i2] : ma2;
		mi3 = (pnt.p[i1]<mi3) ? pnt.p[i1] : mi3;						// �f�ʒ��S�_��BOX���ގ擾
		ma3 = (pnt.p[i1]>ma3) ? pnt.p[i1] : ma3;
		mi4 = (pnt.p[i2]<mi4) ? pnt.p[i2] : mi4;
		ma4 = (pnt.p[i2]>ma4) ? pnt.p[i2] : ma4;
		SelNextObjPtr(1, &op1);											// ���f�ʋȐ�
	}
	if(ct1==2||(ct1>2&&(fabs(mi3-ma3)<EPSILON||fabs(mi4-ma4)<EPSILON))){// <�f�ʂ�2��/�c�����̏ꍇ>
		cpt[ct1].p[i1] = ma1+1.0;										// ���ɂ���1�_��ǉ�
		cpt[ct1].p[i2] = ma2+1.0;
		cpt[ct1].p[i3] = 0.0;
		ct1w = ct1+1;
	} else {
		ct1w = ct1;
	}
	box.bp[0].p[i1] = mi1, box.bp[1].p[i1] = ma1;						// �S�f�ʂ�BOX����
	box.bp[0].p[i2] = mi2, box.bp[1].p[i2] = ma2;
	SetBasePoint(pl, ps, box, &bas);									// �f�ʐ������W�ݒ�
	SelBaseObjPtr(2, &op2);												// �|���Ȑ��擪�߲���擾
	while(op2!=NULL) {
		BaseCmdPtr(1, op2, &cmp2);										// �|���擪����ގ擾
		un = GetParaInt(cmp2, 0);										// ����_���擾
		ub = GetParaInt(cmp2, 1);										// ���EӰ�ގ擾
		for(i=0; i<un; i++) {
			ib[i] = GetParaInt(cmp2, 4+i);								// ײ�Ӱ�ގ擾
			GetParaPnt(cmp2, i, &pb[i]);								// �Ȑ�����_�擾
		}
		SetCurve(un, ub, pb);											// �Ȑ�����_����
		GetObjPtr(&op3);												// ����_OP�ۑ�
		BaseCmdPtr(1, op2, &cmp2);										// �L���擪����ގ擾
		NextCmdPtr(&cmp2);												// �Ȑ��ҏW������
		while(cmp2!=NULL) {
			RedoCommandCopy(op3, cmp2);									// ����ޕ��ʎ��s
			NextCmdPtr(&cmp2);											// ������ނ�
		}
		for(i=0; i<un; i++) {
			GetVtx(op3, i, &pb[i]);										// ���̕ҏW�㐧��_�擾
		}
		FreeObj(op3, 1);												// �ҏW�㗧�̍폜
		if(ct1==1) {													// <<�f��:1�̏ꍇ>>
			SelBaseObjPtr(1, &op1);										// �f�ʋȐ��擪�߲���擾
			if(op1!=NULL) {
				BaseCmdPtr(1, op1, &cmp1);								// �f�ʐ擪����ގ擾
				vn = GetParaInt(cmp1, 0);								// ����_���擾
				vb = GetParaInt(cmp1, 1);								// ���EӰ�ގ擾
				for(i=0; i<vn; i++) {
					ia[i] = GetParaInt(cmp1, 4+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp1, i, &pa[i]);						// �Ȑ�����_�擾
				}
				SetCurve(vn, vb, pa);									// �Ȑ�����_����
				GetObjPtr(&op3);										// ����_OP�ۑ�
				BaseCmdPtr(1, op1, &cmp1);								// �L���擪����ގ擾
				NextCmdPtr(&cmp1);										// �Ȑ��ҏW������
				while(cmp1!=NULL) {
					RedoCommandCopy(op3, cmp1);							// ����ޕ��ʎ��s
					NextCmdPtr(&cmp1);									// ������ނ�
				}
				for(i=0; i<vn; i++) {
					GetVtx(op3, i, &pa[i]);								// ���̕ҏW�㐧��_�擾
				}
				FreeObj(op3, 1);										// �ҏW�㗧�̍폜
				InitCreat(1, vb, vn, pa, ia, &vpl);						// ������������
				if(!sw) {												// <�ʏ�Ȗ�>
					j = GetVtxNum(op1);									// �Ȗʒ��_���擾
					for(i=0; i<j; i++) {
						GetVtx(op1, i, &pc[i]);							// �Ȗʒ��_���W�擾
					}
					ad = GetCurveCenter(vpl, vb, j, pc, &pnt);			// 2D�}�`���S�擾
					ad = ((!sd)||ub||sd1) ? 0 : ad;						// OFF�͒f�ʖ�
					if(ad) {											// 2D���S�ǉ�
						pa[vn] = pnt;
					}
				} else {												// <�|���Ȗ�>
					for(i=0; i<vn; i++) {
						ia[i] /= 2;										// �Ȑ�Ӱ�ޕϊ�(0/1)
					}
				}
				if(vn*(2*ad+un)<=View->m_MaxSCtl) {						// <����_����ȉ�>
					InitSweep(bas, ad, vpl, vn, pa);					// �|����������
					if(tn>0) {
						ChangeRotate2D(tn, ad, vn, pa);					// 2D�f�ʉ�]�X�V
					}
					if(!sw) {											// <�ʏ�Ȗ�>
						Sweep(0, ad, vn, pa, ia, ub, un, pb, ib,
                              pc, ic, wk);								// �|������_����
						mx = View->GetCurveRepeat();					// �Ȑ����J�Ԑ��擾
						wt = View->GetCurveWeight();					// �Ȑ��d�݌W���擾
						sm = View->GetSolidMode();						// �د��Ӱ�ގ擾
						CrtSurface(2*ad+un, vn, ub, vb, mx, wt, sm,
                                   pc, ic);								// �Ȗʐ���
					} else {											// <�|���Ȗ�>
						for(i=0; i<vn; i++) {
							pb[un+i] = pa[i], ib[un+i] = ia[i];			// �f�ʋȐ����ǉ�
						}
						mx = View->GetCurveRepeat();					// �Ȑ����J�Ԑ��擾
						wt = View->GetCurveWeight();					// �Ȑ��d�݌W���擾
						sm = View->GetSolidMode();						// �د��Ӱ�ގ擾
						CrtSweepSurf(un, vn, ub, vb, mx, wt, sm,
                                     -1, pb, ib);						// �|���Ȗʐ���(����)
					}
					GetObjPtr(&op3);									// �Ȗʗ���OP�擾
					i = GetObjAtr(op3);									// ���̑����擾
					GetObjOif(op3, &oif);								// ���̍\������߲���擾
					if(i==20) {
						SetOifName(0, oif, OBJ_NAME03);					// �̍\����(�ʏ�|����)
					} else if(i==21) {
						SetOifName(0, oif, OBJ_NAME04);					//         (�ʏ�|����[�ʑ�])
					} else if(i==22) {
						SetOifName(0, oif, OBJ_NAME05);					//         (�|���|����)
					} else {
						SetOifName(0, oif, OBJ_NAME06);					//         (�|���|����[�ʑ�])
					}
					OnEdtFlg(1, op3);									// �Ȗ�edtflg:ON
				} else {
					AfxMessageBox(IDS_MANY_CTRLP);						// ����_����ȏ�,�װү����
				}
			}
		} else {														// <<�f��:�����̏ꍇ>>
			for(i=0; i<ct1w; i++) {
				cpw[i] = cpt[i];
			}
			InitSweep(bas, 0, pl, ct1w, cpw);							// �����f�ʂ̏ꍇ��
			if(tn>0) {
				ChangeRotate2D(tn, 0, ct1w, cpw);						// 2D�f�ʉ�]�X�V
			}
			Sweep(0, 0, ct1w, cpw, cpi, ub, un, pb, ib, blp, bli, wk);	// �e�f�ʂ̑|�����擾
			ct2 = 0;
			SelBaseObjPtr(1, &op1);										// �f�ʋȐ��擪�߲���擾
			while(op1!=NULL) {
				BaseCmdPtr(1, op1, &cmp1);								// �f�ʐ擪����ގ擾
				vn = GetParaInt(cmp1, 0);								// ����_���擾
				vb = GetParaInt(cmp1, 1);								// ���EӰ�ގ擾
				for(i=0; i<vn; i++) {
					ia[i] = GetParaInt(cmp1, 4+i);						// ײ�Ӱ�ގ擾
					GetParaPnt(cmp1, i, &pa[i]);						// �Ȑ�����_�擾
				}
				SetCurve(vn, vb, pa);									// �Ȑ�����_����
				GetObjPtr(&op3);										// ����_OP�ۑ�
				BaseCmdPtr(1, op1, &cmp1);								// �L���擪����ގ擾
				NextCmdPtr(&cmp1);										// �Ȑ��ҏW������
				while(cmp1!=NULL) {
					RedoCommandCopy(op3, cmp1);							// ����ޕ��ʎ��s
					NextCmdPtr(&cmp1);									// ������ނ�
				}
				for(i=0; i<vn; i++) {
					GetVtx(op3, i, &pa[i]);								// ���̕ҏW�㐧��_�擾
				}
				FreeObj(op3, 1);										// �ҏW�㗧�̍폜
				InitCreat(1, vb, vn, pa, ia, &vpl);						// ������������
				if(!sw) {												// <�ʏ�Ȗ�>
					j = GetVtxNum(op1);									// �Ȗʒ��_���擾
					for(i=0; i<j; i++) {
						GetVtx(op1, i, &pc[i]);							// �Ȗʒ��_���W�擾
					}
					ad = GetCurveCenter(vpl, vb, j, pc, &pnt);			// 2D�}�`���S�擾
					ad = ((!sd)||ub||sd1) ? 0 : ad;						// OFF�͒f�ʖ�
					if(ad) {											// 2D���S�ǉ�
						pa[vn] = pnt;
					}
				} else {												// <�|���Ȗ�>
					for(i=0; i<vn; i++) {
						ia[i] /= 2;										// �Ȑ�Ӱ�ޕϊ�(0/1)
					}
				}
				if(vn*(2*ad+un)<=View->m_MaxSCtl) {						// <����_����ȉ�>
					for(i=0; i<un; i++) {
						pb[i] = blp[un*ct2+i], ib[i] = bli[un*ct2+i];	// �|�����𒊏o
					}
					InitSweep(cpt[ct2], ad, vpl, vn, pa);				// �|����������
					if(tn>0) {
						ChangeRotate2D(tn, ad, vn, pa);					// 2D�f�ʉ�]�X�V
					}
					if(!sw) {											// <�ʏ�Ȗ�>
						Sweep(0, ad, vn, pa, ia, ub, un, pb, ib,
                              pc, ic, wk);								// �|������_����
						mx = View->GetCurveRepeat();					// �Ȑ����J�Ԑ��擾
						wt = View->GetCurveWeight();					// �Ȑ��d�݌W���擾
						sm = View->GetSolidMode();						// �د��Ӱ�ގ擾
						CrtSurface(2*ad+un, vn, ub, vb, mx, wt, sm,
                                   pc, ic);								// �Ȗʐ���
					} else {											// <�|���Ȗ�>
						for(i=0; i<vn; i++) {
							pb[un+i] = pa[i], ib[un+i] = ia[i];			// �f�ʋȐ����ǉ�
						}
						mx = View->GetCurveRepeat();					// �Ȑ����J�Ԑ��擾
						wt = View->GetCurveWeight();					// �Ȑ��d�݌W���擾
						sm = View->GetSolidMode();						// �د��Ӱ�ގ擾
						CrtSweepSurf(un, vn, ub, vb, mx, wt, sm,
                                     -1, pb, ib);						// �|���Ȗʐ���
					}
					GetObjPtr(&op3);									// �Ȗʗ���OP�擾
					i = GetObjAtr(op3);									// ���̑����擾
					GetObjOif(op3, &oif);								// ���̍\������߲���擾
					if(i==20) {
						SetOifName(0, oif, OBJ_NAME03);					// �̍\����(�ʏ�|����)
					} else if(i==21) {
						SetOifName(0, oif, OBJ_NAME04);					//         (�ʏ�|����[�ʑ�])
					} else if(i==22) {
						SetOifName(0, oif, OBJ_NAME05);					//         (�|���|����)
					} else {
						SetOifName(0, oif, OBJ_NAME06);					//         (�|���|����[�ʑ�])
					}
					OnEdtFlg(1, op3);									// �Ȗ�edtflg:ON
				} else {
					AfxMessageBox(IDS_MANY_CTRLP);						// ����_����ȏ�,�װү����
				}
				SelNextObjPtr(1, &op1);									// ���f�ʋȐ�
				ct2++;
			}
		}
		SelNextObjPtr(2, &op2);											// ���|���Ȑ�
	}
	delete[] pa;
	delete[] pb;
	delete[] pc;
	delete[] blp;
	delete[] cpt;
	delete[] cpw;
	delete[] ia;
	delete[] ib;
	delete[] ic;
	delete[] bli;
	delete[] cpi;
}
