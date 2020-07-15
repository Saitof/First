/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW����_���암
// EditCtrlPoint2.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/06/26 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "NumFunc.h"

/*********************************************************************/
void DeleteCLine(int wn, int* iw, int ub, int vb, int uflg, int vflg,
				 int* un, int* vn, PNTTYPE* pa, int* ia, int* ta)		//<<< �Ώې���_��폜
/*********************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCCtA];
	int*     pi = new int[View->m_MaxSCtA];
	int*     pt = new int[View->m_MaxSCtA];
	int*     df = new int[View->m_MaxSCtA];
	PNTTYPE  cpt;
	int      i, j, ix, pn, uu, vv, uc, vc, vl, ua, va;
	int      uw1, uw2, vw1, vw2, us1, us2, vs1, vs2, f1, f2;

	ua = 0, va = 0, uu = *un, uc = *un, vv = *vn, vc = *vn, vl = *vn;	// �����l�ݒ�
	pn = uu*vv;															// ������_��
	for(i=0; i<pn; i++) {
		df[i] = 0;														// �폜�׸ޏ�����
	}
	if(uflg%2==1) {														// <U�n�_����̏ꍇ>
		va++, vl--;
	}
	if(uflg>=2) {														// <U�I�_����̏ꍇ>
		vl--;
	}
	if(vflg%2==1) {														// <V�n�_����̏ꍇ>
		ua++;
	}
	for(i=0; i<wn; i++) {
		if(iw[i]) {														// <<�폜�Ώۂ̏ꍇ>>
			if(i<vl) {													// <V�����w��>
				for(j=0; j<uu; j++) {
					df[j+(i+va)*uu] = 1;								// �폜�׸�ON
				}
				vc--;													// V�񐔌��Z
			} else {													// <U�����w��>
				for(j=0; j<vv; j++) {
					df[j*uu+i-vl+ua] = 1;								// �폜�׸�ON
				}
				uc--;													// U�񐔌��Z
			}
		}
	}
	for(i=0, ix=0; i<pn; i++) {											// <�폜�ΏۊO�̂�>
		if(!df[i]) {													// ����_,�Ȑ�Ӱ��,���͕���
			pp[ix] = pa[i], pi[ix] = ia[i], pt[ix++] = ta[i];
		}
	}
	uw1 = 0, uw2 = 0, vw1 = 0, vw2 = 0, us1 = 0, us2 = 0, vs1 = 0, vs2 = 0;
	uw1 = ((!vb)&&iw[0]==1) ? 1 : uw1;									// ���[�ʎw��&����_�����m�F
	uw2 = ((!vb)&&iw[vl-1]==1) ? 1 : uw2;
	vw1 = ((!ub)&&iw[vl]==1) ? 1 : vw1;
	vw2 = ((!ub)&&iw[wn-1]==1) ? 1 : vw2;
	us1 = (uflg%2==1) ? 1 : us1;
	us2 = (uflg>=2) ? 1 : us2;
	vs1 = (vflg%2==1) ? 1 : vs1;
	vs2 = (vflg>=2) ? 1 : vs2;
	if(uw1) {															// <<U�J�n�f�ʍ폜>>
		if(us1) {														// <U�J�n�f�ʓ���_>
			for(i=0; i<uc; i++) {
				f1 = pi[i+uc]%2, f2 = 1;								// �Ȑ�Ӱ�ޕύX
				pi[i+uc] = f1+2*f2;
				ps[i] = pp[i+uc];										// �J�n�f�ʒ��o
			}
			CalcCent(ps, uc, &cpt);										// �����擾
			for(i=0; i<uc; i++) {
				pp[i] = cpt;											// ����_�������
			}
		} else {														// <U�J�n�f�ʕs����>
			for(i=0; i<uc; i++) {
				f1 = pi[i]%2, f2 = 1;									// �Ȑ�Ӱ�ޕύX
				pi[i] = f1+2*f2;
			}
		}
	}
	if(uw2) {															// <<U�I���f�ʍ폜>>
		if(us2) {														// <U�I���f�ʓ���_>
			for(i=0; i<uc; i++) {
				f1 = pi[i+uc*(vc-2)]%2, f2 = 1;							// �Ȑ�Ӱ�ޕύX
				pi[i+uc*(vc-2)] = f1+2*f2;
				ps[i] = pp[i+uc*(vc-2)];								// �I���f�ʒ��o
			}
			CalcCent(ps, uc, &cpt);										// �����擾
			for(i=0; i<uc; i++) {
				pp[i+uc*(vc-1)] = cpt;									// ����_�������
			}
		} else {														// <U�I���f�ʕs����>
			for(i=0; i<uc; i++) {
				f1 = pi[i+uc*(vc-1)]%2, f2 = 1;							// �Ȑ�Ӱ�ޕύX
				pi[i+uc*(vc-1)] = f1+2*f2;
			}
		}
	}
	if(vw1) {															// <<V�J�n�f�ʍ폜>>
		if(vs1) {														// <V�J�n�f�ʓ���_>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[i*uc+1]/2;								// �Ȑ�Ӱ�ޕύX
				pi[i*uc+1] = f1+2*f2;
				ps[i] = pp[i*uc+1];										// �J�n�f�ʒ��o
			}
			CalcCent(ps, vc, &cpt);										// �����擾
			for(i=0; i<vc; i++) {
				pp[i*uc] = cpt;											// ����_�������
			}
		} else {														// <V�J�n�f�ʕs����>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[i*uc]/2;								// �Ȑ�Ӱ�ޕύX
				pi[i*uc] = f1+2*f2;
			}
		}
	}
	if(vw2) {															// <<V�I���f�ʍ폜>>
		if(vs2) {														// <V�I���f�ʓ���_>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[(i+1)*uc-2]/2;							// �Ȑ�Ӱ�ޕύX
				pi[(i+1)*uc-2] = f1+2*f2;
				ps[i] = pp[(i+1)*uc-2];									// �I���f�ʒ��o
			}
			CalcCent(ps, vc, &cpt);										// �����擾
			for(i=0; i<vc; i++) {
				pp[(i+1)*uc-1] = cpt;									// ����_�������
			}
		} else {														// <V�I���f�ʕs����>
			for(i=0; i<vc; i++) {
				f1 = 1, f2 = pi[(i+1)*uc-1]/2;							// �Ȑ�Ӱ�ޕύX
				pi[(i+1)*uc-1] = f1+2*f2;
			}
		}
	}
	for(i=0; i<ix; i++) {
		pa[i] = pp[i], ia[i] = pi[i], ta[i] = pt[i];					// ����_,�Ȑ�Ӱ��,���͕���
	}
	*un = uc, *vn = vc;													// �폜��U,V����_��
	delete[] pp;
	delete[] ps;
	delete[] pi;
	delete[] pt;
	delete[] df;
}

/*************************************************************/
static void NearCenterPoint(PNTTYPE cp, BOXTYPE bx,
							PNTTYPE* pa, int pn, PNTTYPE* pt)			//<<< ���S�ŒZ�_�擾
/*************************************************************/
{
	PNTTYPE minp, maxp;
	int     i, j;
	double  x, y, z, min1, max1;

	x = bx.bp[1].p[0]-bx.bp[0].p[0];									// BOX X����
	y = bx.bp[1].p[1]-bx.bp[0].p[1];									// BOX Y����
	z = (bx.bp[0].p[2]+bx.bp[1].p[2])/2.0;								// BOX Z���S�ʒu
	j = (x>y) ? 0 : 1;													// �ő廲�ޕ���
	minp = pa[0], min1 = pa[0].p[j];									// �ŏ���ő叉���l
	maxp = pa[0], max1 = pa[0].p[j];
	for(i=1; i<pn; i++) {
		if(pa[i].p[j]<min1) {											// �ŏ�����
			minp = pa[i], min1 = pa[i].p[j];
		}
		if(pa[i].p[j]>max1) {											// �ő匟��
			maxp = pa[i], max1 = pa[i].p[j];
		}
	}
	minp.p[2] = z, maxp.p[2] = z;										// Z�l�͒��S�ʒu
    *pt = (PntD(&cp, &minp)<PntD(&cp, &maxp)) ? minp : maxp;			// ���S�_�ɋ߂���
}

/********************************************************************/
void InsertCLine(int no, int flg, int ub, int* un, int* vn, int uf,
                 int* cn, int bn, PNTTYPE cpt, PNTTYPE* pa, int* ia)	//<<< �Ώې���_��}��
/********************************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pd = new PNTTYPE[View->m_MaxCCtA];
	int*     ib = new int[View->m_MaxSCtA];
	int*     ic = new int[View->m_MaxCCtA];
	PNTTYPE  pw[4], cpc, cpd, mov;
	VECTYPE  vc1, vc2, rvc;
	BOXTYPE  bxc, bxd;
	int      i, j, k, uu, vv, ul, cc, i1, i2, f1, f2, ct1=0, ct2=0;
	double   rot[3][3], th;

	uu = *un, vv = *vn, ul = 2*uu, cc = *cn;							// �n�_���Z�l������
	ul = (!ub) ? ul-1 : ul;												// U�J�͑}���_=�f�ʐ�-1
	if(no<ul) {															// <<U�����w��>>
		if(cc>-1) {														// <���ʒf�ʂ��L���ȏꍇ>
			for(i=0; i<vv; i++) {
				pc[i] = pa[cc+i*uu], ic[i] = ia[cc+i*uu];				// ���ʒf�ʒ��o
			}
		}
		for(i=0; i<vv; i++) {
			for(j=0; j<uu; j++) {
				pb[ct1] = pa[j+i*uu], ib[ct1++] = ia[j+i*uu];			// ����_��Ȑ�Ӱ�ޕ���
				if(no%2==1&&j==no/2) {									// <<�}���ʒu�̏ꍇ>>
					if(j==uu-1) {										// <U�̎n�I�_>
						i1 = (i+1)*uu-1, i2 = i*uu;
					} else {											// <��L�ȊO>
						i1 = j+i*uu, i2 = j+i*uu+1;
					}
					for(k=0; k<DIM; k++) {
						pb[ct1].p[k] = (pa[i1].p[k]+pa[i2].p[k])/2.0;	// ���ԍ��W
					}
					f1 = ia[i1]/2, f2 = ia[i2]/2;						// V�Ȑ�Ӱ��
					f1 = (f1&&f2) ? 1 : 0;								// ���_�ܐ��͐ܐ�
					ib[ct1++] = flg+f1*2;								// �Ȑ�Ӱ�ސݒ�
					pd[ct2++] = pb[ct1-1];								// �}���f�ʒ��o
				} else if(no%2==0&&j==no/2) {							// <<�u���ʒu�̏ꍇ>>
					pd[ct2++] = pb[ct1-1];								// �u���f�ʒ��o
				}
			}
		}
		if(no%2==1) {													// <�}���̏ꍇ>
			uu++;														// U�񐔉��Z
			cc = (cc>-1&&cc>no/2) ? cc+1 : cc;							// ���ʈʒu���O�̑}��
			no = no/2 + 1;												// �}���f��No
		} else {														// <�u���̏ꍇ>
			no = no/2;													// �u���f��No
		}
		if(cc>-1) {														// <<���ʒf�ʂ��L���ȏꍇ>>
			CalcNVec(pc, vv, &vc1);										// ���ʒf�ʖ@���޸�َ擾
			CalcNVec(pd, vv, &vc2);										// �}���f�ʖ@���޸�َ擾
			if(fabs(vc1.p[0]-vc2.p[0])>EPSILON||
               fabs(vc1.p[1]-vc2.p[1])>EPSILON||
               fabs(vc1.p[2]-vc2.p[2])>EPSILON) {						// 2�޸�يԂ̊p�x
				th = acos(vc1.p[0]*vc2.p[0]+vc1.p[1]*vc2.p[1]+vc1.p[2]*vc2.p[2]);
				th = (th<-10||th>10) ? 0.0 : th;
			} else {
				th = 0.0;
			}
			for(i=0, j=1; i<DIM; i++) {
				j = (fabs(vc1.p[i])>EPSILON) ? 0 : j;					// vc1����
			}
			if(j) {
				th = 0.0;
			}
			for(i=0, j=1; i<DIM; i++) {
				j = (fabs(vc2.p[i])>EPSILON) ? 0 : j;					// vc2����
			}
			if(j) {
				th = 0.0;
			}
			if(th<EPSILON) {											// <�L���p�x�����̏ꍇ>
				rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = -1.0;
			} else if(th>PI/90&&th<PI*89/90) {							// <2�`88�x�̏ꍇ>
				for(i=0; i<DIM; i++) {
					pw[0].p[i] = 0.0;									// ���_��2�@���޸�ق�
					pw[1].p[i] = vc1.p[i]*1000.0;						// �Ȃ�3�_
					pw[2].p[i] = vc2.p[i]*1000.0;
				}
				CalcNVec(pw, 3, &rvc);									// 2�޸�يԂ̖@���޸��
			} else {													// <�ȊO�̏ꍇ>
				CalcCent(pc, vv, &pw[0]);								// �f�ʂ̒��S�_
				CalcCent(pd, vv, &pw[2]);
				for(i=0; i<DIM; i++) {
					pw[1].p[i] = pw[0].p[i]+vc1.p[i]*1000.0;			// �f�ʏ�̒��S�_��
					pw[3].p[i] = pw[2].p[i]+vc2.p[i]*1000.0;			// �@���޸�ٕ����̓_
				}
				CalcNVec(pw, 4, &rvc);									// 2�޸�يԂ̖@���޸��
			}
			SetRotateMatrix(rvc, th, rot);								// ��]��د���쐬
			CalcBox(pc, vv, &bxc);										// ���ʒf��BOX���ގ擾
			if(ub) {													// ���ʒf�ʒ��S�ŒZ�_�擾
				NearCenterPoint(cpt, bxc, pc, vv, &cpc);
			} else {													// ���ʒf�ʒ��S�_�擾
				BoxCenter(bxc, &cpc);
			}
			CalcBox(pd, vv, &bxd);										// �}���f��BOX���ގ擾
			if(ub) {													// �}���f�ʒ��S�ŒZ�_�擾
				NearCenterPoint(cpt, bxd, pd, vv, &cpd);
			} else {													// �}���f�ʒ��S�_�擾
				BoxCenter(bxd, &cpd);
			}
			for(i=0; i<vv; i++) {
				for(j=0; j<DIM; j++) {
					pc[i].p[j] -= cpc.p[j];								// ��]���S�_�����_
				}
				VecRotateMove(&pc[i], rot, cpc);						// ��]�㌳�ʒu�ɖ߂�
			}
			if(bn==0) {
				cpc.p[2] = bxc.bp[1].p[2], cpd.p[2] = bxd.bp[1].p[2];	// ���ʊ�ʒu�ύX
			} else if(bn==2) {
				cpc.p[2] = bxc.bp[0].p[2], cpd.p[2] = bxd.bp[0].p[2];
			}
			for(i=0; i<DIM; i++) {
				mov.p[i] = cpd.p[i]-cpc.p[i];							// �ړ��ʎZ�o
			}
			if(!ub&&(no==0||no==uu-1)) {								// U:�J�ŗ��[�̏ꍇ��
				for(i=0; i<vv; i++) {									// �K���AU�Ȑ�Ӱ��=1
					j = ic[i]/2, ic[i] = j*2+1;
				}
			}
			for(i=0; i<vv; i++) {
				for(j=0; j<DIM; j++) {
					pc[i].p[j] += mov.p[j];								// ���ʒf�ʈړ�
				}
				pb[no+i*uu] = pc[i], ib[no+i*uu] = ic[i];				// ���ʒf�ʐݒ�
			}
			if(uf%2==1) {
				pb[no] = pd[0];											// �n�_�����͑}���f�ʎn�_�̗p
			}
			if(uf>1) {
				pb[no+(vv-1)*uu] = pd[vv-1];							// �I�_�����͑}���f�ʏI�_�̗p
			}
		}
	} else {															// <<V�����w��>>
		for(i=0, no=no-ul; i<vv; i++) {
			for(j=0; j<uu; j++) {
				pb[ct1] = pa[j+i*uu], ib[ct1++] = ia[j+i*uu];			// ����_��Ȑ�Ӱ�ޕ���
			}
			if(i==no) {													// <<�}���ʒu�̏ꍇ>>
				for(j=0; j<uu; j++) {
					if(i==vv-1) {										// <V�̎n�I�_>
						i1 = j+(vv-1)*uu, i2 = j;
					} else {											// <��L�ȊO>
						i1 = j+i*uu, i2 = j+(i+1)*uu;
					}
					for(k=0; k<DIM; k++) {
						pb[ct1].p[k] = (pa[i1].p[k]+pa[i2].p[k])/2.0;	// ���ԓ_���W
					}
					pd[ct2++] = pb[ct1];								// �}���f�ʒ��o
					f1 = ia[i1]%2, f2 = ia[i2]%2;						// U�Ȑ�Ӱ��
					f1 = (f1&&f2) ? 1 : 0;								// ���_�ܐ��͐ܐ�
					ib[ct1++] = f1+flg*2;								// �Ȑ�Ӱ�ސݒ�
				}
			}
		}
		vv++;															// V�񐔉��Z
	}
	j = uu*vv;															// �}���㐧��_��
	*un = uu, *vn = vv, *cn = cc;
	for(i=0; i<j; i++) {
		pa[i] = pb[i], ia[i] = ib[i];									// ����_��Ȑ�Ӱ�ޕ���
	}
	delete[] pb;
	delete[] pc;
	delete[] pd;
	delete[] ib;
	delete[] ic;
}

/**************************************************************/
void PutSPoint(int fg, int ix, int un, int vn, int vflg,
			   PNTTYPE scp, PNTTYPE* pb, int* ib, PNTTYPE cpt,
			   VECTYPE vec, PNTTYPE* pa, PNTTYPE* pc, int* ia)			//<<< �f�ʐ���_���X�V
/**************************************************************/
{
	PNTTYPE pnt;
	VECTYPE rvc;
	int     i, j, i1, i2, i3, vd;
	double  rot[3][3], rth, dd;

	for(i=0, vd=0, dd=0.0; i<DIM; i++) {
		if(dd<fabs(vec.p[i])) {											// ��f���޸�ٍő��������
			dd = fabs(vec.p[i]), vd = i;
		}
	}
    i1 = (vd+1>2) ? 0 : vd+1;											// ���W���ݒ�
    i2 = (i1+1>2) ? 0 : i1+1;
    i3 = (i2+1>2) ? 0 : i2+1;
	if(fg) {															// <���̏���>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] -= scp.p[j];									// �f�ʕҏW�ʒu�ړ�
			}
		}
	} else {															// <���̔����>
		for(i=0; i<vn; i++) {
			for(j=0; j<DIM; j++) {
				pb[i].p[j] -= cpt.p[j];									// �f�ʕҏW�ʒu�ړ�
			}
		}
	}
	rth = sqrt(vec.p[i1]*vec.p[i1]+vec.p[i2]*vec.p[i2]);				// ��ʕ�������
	if(rth>EPSILON) {													// <��ʂɂȂ��ꍇ>
		rvc.p[i1] = -vec.p[i2], rvc.p[i2] = vec.p[i1], rvc.p[i3] = 0.0;
		VecN(&rvc);
		rth = atan2(rth, vec.p[i3]);									// ��]�p�x�Z�o
		rth = (rth<-10||rth>10) ? 0.0 : rth;
		rth = (rth>PI/2) ? rth+PI : rth;
		SetRotateMatrix(rvc, rth, rot);									// ��]��د���쐬
		for(i=0; i<vn; i++) {
			VecRotate(&pb[i], rot);										// ��ʂɉ�]
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pb[i].p[j] += cpt.p[j];										// �f�ʒ��S�ʒu�ړ�
		}
	}
	pnt.p[0] = 0.0, pnt.p[1] = 0.0, pnt.p[2] = 0.0;
	i1 = un*vn;
	for(i=0; i<i1; i++) {
		pc[i] = pnt;													// �ړ��ʔz��S�ر
	}
	ix = (vflg%2==1) ? ix+1 : ix;										// V�n�_����̏ꍇ
	for(i=0; i<vn; i++) {												// <<V�����ׯ��>>
		for(j=0; j<DIM; j++) {
			pc[ix+i*un].p[j] = pb[i].p[j]-pa[ix+i*un].p[j];				// ����_�ړ��ʎZ�o
		}
		i1 = ia[ix+i*un]%2;												// V�Ȑ�Ӱ�ޕۑ�
		ia[ix+i*un] = i1+2*ib[i];										// �Ȑ�Ӱ�ސݒ�
	}
}

/***********************************************************/
static void ScaleSection(PNTTYPE* pa, int pn, VECTYPE vec,
						 double th, int d1, int d2, int d3)				//<<< �f�ʊg��
/***********************************************************/
{
	VECTYPE rv1, rv2, fvc;
	int     i, r1, r2;
	double  rot[3][3], t1, t2, sc;

	r1 = 0, r2 = 0;
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

/*************************************************************/
void SetSweepCtrl(int pl, int un, int vn, int ub,
				  PNTTYPE* pa, int* ia, PNTTYPE* pb, int* ib)			//<<< �|���Ȗʐ���_�ϊ�
/*************************************************************/
{
	PNTTYPE* up = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* vp = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* wvp = new PNTTYPE[View->m_MaxCPnt];
	int*     ui = new int[View->m_MaxCPnt];
	int*     vi = new int[View->m_MaxCPnt];
	PNTTYPE  wp[3], pt;
	VECTYPE  vec, rvc;
	int      i, j, k, d1, d2, d3;
	double   rota[3][3], rotb[3][3], th;

	for(i=0; i<un; i++) {
		up[i] = pa[i], ui[i] = ia[i];									// U����_��񒊏o
	}
	for(i=0; i<vn; i++) {
		vp[i] = pa[i+un], vi[i] = ia[i+un]*2;							// V����_��񒊏o
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] = up[1].p[i]-up[0].p[i];								// �ް����޸�َ擾
	}
	VecN(&vec);															// �P���޸�ى�
	if(pl==-1) {														// <���ʎw�薳�ɏꍇ(�V��)>
		d1 = 0, d2 = 1, d3 = 2;											// ���W���ݒ�
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// ����p�x����]
		th = atan2(vec.p[1], vec.p[0]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[2])>EPSILON) {									// <����ʏ�łȂ��ꍇ>
			for(i=0; i<vn; i++) {
				VecRotate(&vp[i], rota);
			}
			th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
			if(th<EPSILON) {
				rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
				th = (vec.p[2]<0.0) ? -PI/2.0 : PI/2.0;
			} else {
				rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
				VecN(&rvc);
				th = atan2(vec.p[2], th);
				th = (th<-10||th>10) ? 0.0 : th;
			}
			SetRotateMatrix(rvc, -th, rota);
		}
	} else {															// <���ʎw��L�̏ꍇ(����)>
		d1 = pl;														// ���W���ݒ�
        d2 = (d1+1>2) ? 0 : d1+1;
        d3 = (d2+1>2) ? 0 : d2+1;
		rvc.p[d1] = 0.0, rvc.p[d2] = 0.0, rvc.p[d3] = -1.0;				// ���(d1)�p�x����]
		th = atan2(vec.p[d1], vec.p[d2]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[d3])>EPSILON) {									// <����ʏ�łȂ��ꍇ>
			for(i=0; i<vn; i++) {
				VecRotate(&vp[i], rota);
			}
			th = sqrt(vec.p[d1]*vec.p[d1]+vec.p[d2]*vec.p[d2]);
			if(th<EPSILON) {
				rvc.p[d1] = 0.0, rvc.p[d2] = 1.0, rvc.p[d3] = 0.0;
				th = (vec.p[d3]<0.0) ? PI/2.0 : 0.0;
			} else {
				rvc.p[d1] = -vec.p[d2], rvc.p[d2] = vec.p[d1], rvc.p[d3] = 0.0;
				VecN(&rvc);
				th = atan2(vec.p[d3], th);
				th = (th<-10||th>10) ? 0.0 : th;
			}
			SetRotateMatrix(rvc, -th, rota);
		}
	}
	for(i=0; i<vn; i++) {												// <<�ް����n�_����>>
		pt = vp[i];
		VecRotateMove(&pt, rota, up[0]);								// �ް����n�_�����ɉ�]�ړ�
		pb[i*un] = pt, ib[i*un] = vi[i]+ui[0];
		VecRotate(&vp[i], rota);										// �n�_�����ɉ�]
	}
	for(i=1; i<un-1; i++) {												// <<�ް������ԓ_����>>
		CalcRotAngleVec(&up[i-1], &th, &vec);							// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		SetRotateMatrix(vec, th, rotb);									// ����_�p��د��
		if(!ui[i]) {													// <�|���������Ȑ��_>
			for(j=0; j<vn; j++) {
				pt = vp[j];
				VecRotateMove(&pt, rota, up[i]);						// �O��_���Ԋp�x�ɉ�]�ړ�
				pb[j*un+i] = pt, ib[j*un+i] = vi[j]+ui[i];
				VecRotate(&vp[j], rotb);								// ���_�����ɉ�]
			}
		} else {														// <�|���������ܐ��_>
			for(j=0; j<vn; j++) {
				wvp[j] = vp[j];
				VecRotate(&wvp[j], rota);								// �O��_���Ԋp�x�ɉ�]
				VecRotate(&vp[j], rotb);								// ���_�����ɉ�]
			}
			ScaleSection(wvp, vn, vec, th, d1, d2, d3);					// �f�ʊg��
			for(j=0; j<vn; j++) {
				for(k=0; k<DIM; k++) {
					pb[j*un+i].p[k] = wvp[j].p[k]+up[i].p[k];			// �|���_�Ɉړ�
				}
				ib[j*un+i] = vi[j]+ui[i];
			}
		}
	}
	if(!ub) {															// <<<���E:�J�̏ꍇ>>>
		for(i=0; i<vn; i++) {											// <<�ް����I�_����>>
			pt = vp[i];
			for(j=0; j<DIM; j++) {
				pt.p[j] += up[un-1].p[j];								// �I�_�ړ�
			}
			pb[i*un+un-1] = pt, ib[i*un+un-1] = vi[i]+ui[un-1];
		}
	} else {															// <<<���E:�̏ꍇ>>>
		wp[0] = up[un-2], wp[1] = up[un-1], wp[2] = up[0];				// �I�_�Ǝn�_��A��
		CalcRotAngleVec(wp, &th, &vec);									// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		SetRotateMatrix(vec, th, rotb);									// ����_�p��د��
		if(!ui[un-1]) {													// <�|���������Ȑ��_>
			for(i=0; i<vn; i++) {
				pt = vp[i];
				VecRotateMove(&pt, rota, up[un-1]);						// �I���f��
				pb[i*un+un-1] = pt, ib[i*un+un-1] = vi[i]+ui[un-1];
				VecRotate(&vp[i], rotb);								// ���_�����ɉ�]
			}
		} else {														// <�|���������ܐ��_>
			for(i=0; i<vn; i++) {
				wvp[i] = vp[i];
				VecRotate(&wvp[i], rota);								// �O��_���Ԋp�x�ɉ�]
				VecRotate(&vp[i], rotb);								// ���_�����ɉ�]
			}
			ScaleSection(wvp, vn, vec, th, d1, d2, d3);					// �f�ʊg��
			for(i=0; i<vn; i++) {										// �I���f��
				for(j=0; j<DIM; j++) {
					pb[i*un+un-1].p[j] = wvp[i].p[j]+up[un-1].p[j];
				}
				ib[i*un+un-1] = vi[i]+ui[un-1];
			}
		}
		wp[0] = up[un-1], wp[1] = up[0], wp[2] = up[1];					// �I�_�Ǝn�_��A��
		CalcRotAngleVec(wp, &th, &vec);									// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		if(!ui[0]) {													// <�|���������Ȑ��_>
			for(i=0; i<vn; i++) {
				pt = vp[i];
				VecRotateMove(&pt, rota, up[0]);						// �J�n�f��
				pb[i*un] = pt;
			}
		} else {														// <�|���������ܐ��_>
			for(i=0; i<vn; i++) {
				VecRotate(&vp[i], rota);								// �O��_���Ԋp�x�ɉ�]
			}
			ScaleSection(vp, vn, vec, th, d1, d2, d3);					// �f�ʊg��
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pb[i*un].p[j] = vp[i].p[j]+up[0].p[j];				// �J�n�f��
				}
			}
		}
	}
	delete[] up;
	delete[] vp;
	delete[] wvp;
	delete[] ui;
	delete[] vi;
}

/********************************************************/
void DeleteSweep(int wn, int* iw, int ub, int vb,
				 int* un, int* vn, PNTTYPE* pa, int* ia)				//<<< �|������_��폜
/********************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCCtA*2];
	int*     pi = new int[View->m_MaxCCtA*2];
	int*     df = new int[View->m_MaxCCtA*2];
	int      i, ix, pn, uu, vv, uc, vc;

	uu = *un, uc = *un, vv = *vn, vc = *vn;
	pn = uu+vv;															// ������_��
	for(i=0; i<pn; i++) {
		df[i] = 0;														// �폜�׸ޏ�����
	}
	for(i=0; i<wn; i++) {
		if(iw[i]) {														// <�폜�Ώۂ̏ꍇ>
			if(i<vv) {													// V�����w��
				df[uu+i] = 1, vc--;
			} else {													// U�����w��
				df[i-vv] = 1, uc--;
			}
		}
	}
	for(i=0, ix=0; i<pn; i++) {											// <�폜�ΏۊO�̂�>
		if(!df[i]) {													// ����_,�Ȑ�Ӱ�ޕ���
			pp[ix] = pa[i], pi[ix++] = ia[i];
		}
	}
	for(i=0; i<ix; i++) {
		pa[i] = pp[i], ia[i] = pi[i];									// ����_,�Ȑ�Ӱ�ޕ���
	}
	if(!ub) {
		ia[0] = 1, ia[uc-1] = 1;
	}
	if(!vb) {
		ia[uc] = 1, ia[uc+vc-1] = 1;
	}
	*un = uc, *vn = vc;													// �폜��U,V����_��
	delete[] pp;
	delete[] pi;
	delete[] df;
}

/*********************************************************/
void MoveSweep(PNTTYPE* ma, int pl, int un, int vn,
			   PNTTYPE* pa, int wn, int* iw, PNTTYPE* pb)				//<<< �|������_�ړ�
/*********************************************************/
{
	PNTTYPE pt;
	VECTYPE vec, rvc;
	int     i, j, d1, d2, d3;
	double  rota[3][3], rotb[3][3], th;

	for(i=0; i<DIM; i++) {
		vec.p[i] = pa[1].p[i]-pa[0].p[i];								// �ް����޸�َ擾
	}
	VecN(&vec);															// �P���޸�ى�
	if(pl==-1) {														// <���ʎw�薳�ɏꍇ(�V��)>
		d1 = 1, d2 = 0, d3 = 2;											// ���W���ݒ�
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// ����p�x����]
		th = atan2(vec.p[1], vec.p[0]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, -th, rota);
		if(fabs(vec.p[2])>EPSILON) {									// <����ʏ�łȂ��ꍇ>
			th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
			if(th<EPSILON) {
				rvc.p[0] = 0.0, rvc.p[1] = 1.0, rvc.p[2] = 0.0;
				th = (vec.p[2]<0.0) ? -PI/2.0 : PI/2.0;
			} else {
				rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
				VecN(&rvc);
				th = atan2(vec.p[2], th);
				th = (th<-10||th>10) ? 0.0 : th;
			}
			SetRotateMatrix(rvc, th, rotb);
		}
	} else {															// <���ʎw��L�̏ꍇ(����)>
		d1 = pl;														// ���W���ݒ�
        d2 = (d1+1>2) ? 0 : d1+1;
        d3 = (d2+1>2) ? 0 : d2+1;
		rvc.p[d1] = 0.0, rvc.p[d2] = 0.0, rvc.p[d3] = -1.0;				// ���(d1)�p�x����]
		th = atan2(vec.p[d1], vec.p[d2]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, -th, rota);
		if(fabs(vec.p[d3])>EPSILON) {									// <����ʏ�łȂ��ꍇ>
			th = sqrt(vec.p[d1]*vec.p[d1]+vec.p[d2]*vec.p[d2]);
			if(th<EPSILON) {
				rvc.p[d1] = 0.0, rvc.p[d2] = 1.0, rvc.p[d3] = 0.0;
				th = (vec.p[d3]<0.0) ? PI/2.0 : 0.0;
			} else {
				rvc.p[d1] = -vec.p[d2], rvc.p[d2] = vec.p[d1], rvc.p[d3] = 0.0;
				VecN(&rvc);
				th = atan2(vec.p[d3], th);
				th = (th<-10||th>10) ? 0.0 : th;
			}
			SetRotateMatrix(rvc, th, rotb);
		}
	}
	for(i=0; i<wn; i++) {
		pb[i] = pa[i];													// ����_����
	}
	for(i=0; i<wn; i++) {
		if(iw[i]) {														// <<�ړ��Ώۂ̏ꍇ>>
			if(i<vn) {													// <V�����w��>
				pt = ma[i];												// �ړ��ʂ�f�ʏ�ɕϊ�
				VecRotate(&pt, rota);									// �ް����n�_��������߂�
				if(fabs(vec.p[d3])>EPSILON) {							// ����ʏ�łȂ��ꍇ
					VecRotate(&pt, rotb);
				}
				pt.p[d2] = 0;
				for(j=0; j<DIM; j++) {
					pb[un+i].p[j] += pt.p[j];							// �ړ��ʕ����Z
				}
			} else {													// <U�����w��>
				for(j=0; j<DIM; j++) {
					pb[i-vn].p[j] += ma[i].p[j];						// �ړ��ʕ����Z
				}
			}
		}
	}
}

/********************************************************/
void InsertSweep(int ix, int flg, int ub,
				 int* un, int* vn, PNTTYPE* pa, int* ia)				//<<< �|������_��}��
/********************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCCtA*2];
	int*     pi = new int[View->m_MaxCCtA*2];
	int      i, j, uu, vv, ul, i1, i2, pc=0;

	uu = *un, vv = *vn, ul = uu;
	ul = (!ub) ? ul-1 : ul;												// U�J�̏ꍇ
	if(ix<ul) {															// <<U�����w��>>
		for(i=0; i<uu; i++) {
			pp[pc] = pa[i], pi[pc++] = ia[i];							// ����_��Ȑ�Ӱ�ޕ���
			if(i==ix) {													// <<�}���ʒu>>
				if(i==uu-1) {											// <U�̎n�I�_>
					i1 = uu-1, i2 = 0;
				} else {												// <��L�ȊO>
					i1 = i, i2 = i+1;
				}
				for(j=0; j<DIM; j++) {
					pp[pc].p[j] = (pa[i1].p[j]+pa[i2].p[j])/2.0;		// ���ԓ_���W
				}
				pi[pc++] = flg;											// �Ȑ�Ӱ�ސݒ�
			}
		}
		for(i=0; i<vv; i++) {
			pp[pc] = pa[i+uu], pi[pc++] = ia[i+uu];						// ����_��Ȑ�Ӱ�ޕ���
		}
		uu++;															// U�񐔉��Z
	} else {															// <<V�����w��>>
		for(i=0; i<uu; i++) {
			pp[pc] = pa[i], pi[pc++] = ia[i];							// ����_��Ȑ�Ӱ�ޕ���
		}
		ix = ix-ul;
		for(i=0; i<vv; i++) {
			pp[pc] = pa[i+uu], pi[pc++] = ia[i+uu];						// ����_��Ȑ�Ӱ�ޕ���
			if(i==ix) {													// <<�}���ʒu>>
				if(i==vv-1) {											// <V�̎n�I�_>
					i1 = vv-1+uu, i2 = uu;
				} else {												// <��L�ȊO>
					i1 = i+uu, i2 = i+1+uu;
				}
				for(j=0; j<DIM; j++) {
					pp[pc].p[j] = (pa[i1].p[j]+pa[i2].p[j])/2.0;		// ���ԓ_���W
				}
				pi[pc++] = flg;											// �Ȑ�Ӱ�ސݒ�
			}
		}
		vv++;															// V�񐔉��Z
	}
	pc = uu+vv;															// ������_��
	for(i=0; i<pc; i++) {
		pa[i] = pp[i], ia[i] = pi[i];									// ����_��Ȑ�Ӱ�ޕ���
	}
	*un = uu, *vn = vv;													// �}����U/V����_��
	delete[] pp;
	delete[] pi;
}

/*****************************************************************/
void CtrlPointSpace(PNTTYPE* pa, PNTTYPE* pb, int* ia, int an,
                    PNTTYPE rcp, PNTTYPE scp, int flg, int plane,
                    double dx, double dy, double dz, double rx,
                    double ry, double rz, double sx, double sy,
                    double sz, double mv)								//<<< ����_��߰�(CPS)
/*****************************************************************/
{
	PNTTYPE pnt, spt, ept, dis, dsx, dsy, dsz, zer;
	VECTYPE vec;
	int     i, j, i1, i2, i3;
	double  wk, tx, ty, tz, mat[3][3], matx[3][3], maty[3][3], matz[3][3];

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;
	if(flg==0) {														// <���ޕύX�̏ꍇ>
		tx = (sx<0.01) ? 0.01 : sx;										// �L���{������
		ty = (sy<0.01) ? 0.01 : sy;
		tz = (sz<0.01) ? 0.01 : sz;
		mat[0][0] = tx,  mat[0][1] = 0.0, mat[0][2] = 0.0;				// ���ޕύX��د���ݒ�
		mat[1][0] = 0.0, mat[1][1] = ty,  mat[1][2] = 0.0;
		mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = tz;
		for(i=0; i<DIM; i++) {											// �ړ��ʌv�Z
			for(wk=0.0, j=0; j<DIM; j++) {
				wk += mat[i][j]*scp.p[j];
			}
			dis.p[i] = scp.p[i]-wk;
		}
	} else {															// <���ˏ�ړ��̏ꍇ>
		i1 = plane;														// ���W���ݒ�
		i2 = (i1+1>2) ? 0 : i1+1;
		i3 = (i2+1>2) ? 0 : i2+1;
		spt = scp, spt.p[i3] = 0.0;										// ���ޕύX��_���n�_
	}
	tx = rx*PI/180.0, ty = ry*PI/180.0, tz = rz*PI/180.0;				// ��]�p�x�ϊ�
	vec.p[0] = -1.0, vec.p[1] = 0.0, vec.p[2] = 0.0;					// X���p��]���
	SetRotateMatrix(vec, tx, matx);										// ��]��د���ݒ�
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {									// �ړ��ʌv�Z
			wk += matx[i][j]*rcp.p[j];
		}
		dsx.p[i] = rcp.p[i]-wk;
	}
	vec.p[0] = 0.0, vec.p[1] = -1.0, vec.p[2] = 0.0;					// Y���p��]���
	SetRotateMatrix(vec, ty, maty);										// ��]��د���ݒ�
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {									// �ړ��ʌv�Z
			wk += maty[i][j]*rcp.p[j];
		}
		dsy.p[i] = rcp.p[i]-wk;
	}
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = -1.0;					// Z���p��]���
	SetRotateMatrix(vec, tz, matz);										// ��]��د���ݒ�
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {									// �ړ��ʌv�Z
			wk += matz[i][j]*rcp.p[j];
		}
		dsz.p[i] = rcp.p[i]-wk;
	}
	for(i=0; i<an; i++) {
		if(ia[i]==1) {													// <<�ړ��Ώۂ̐���_>>
			pnt = pa[i];
			if(flg==0) {												// <���ޕύX�̏ꍇ>
				VecRotateMove(&pnt, mat, dis);							// ���_���W���ޕύX
			} else {													// <���ˏ�ړ��̏ꍇ>
				ept = pnt, ept.p[i3] = 0.0;								// �I�_
				VecV(&spt, &ept, &vec);									// �n�_->�I�_�޸��
				VecN(&vec);												// �P���޸�ى�(3D)
				for(j=0; j<DIM; j++) {
					pnt.p[j] += mv*vec.p[j];							// ��_������ˏ�Ɉړ�
				}
			}
			VecRotateMove(&pnt, matx, dsx);								// ���_���W��]�ړ�(X��)
			VecRotateMove(&pnt, maty, dsy);								// ���_���W��]�ړ�(Y��)
			VecRotateMove(&pnt, matz, dsz);								// ���_���W��]�ړ�(Z��)
			pnt.p[0] += dx, pnt.p[1] += dy, pnt.p[2] += dz;				// �ړ��ʕ��␳
			for(j=0; j<DIM; j++) {
				pb[i].p[j] = pnt.p[j]-pa[i].p[j];						// �ړ��O����_�Ƃ̍�
			}
		} else {
			pb[i] = zer;												// �ΏۊO�̓[��
		}
	}
}
