/////////////////////////////////////////////////////////////////////////////
// ���̐��� �Ȗʐ����R�}���h��
// CCmdSurface.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"

/******************************************************************/
static void SetSectionPoint(int flg, int un, int vn,
                            PNTTYPE* pa, PNTTYPE* ps, PNTTYPE* pe)		//<<< �f�ʒ��_�ۑ�
/******************************************************************/
{
	int i, j=0;

	if(!flg) {															// <U��&V�J�̏ꍇ>
		for(i=0; i<un-1; i++) {
			ps[i] = pa[i];												// U�J�n�f�ʕۑ�
		}
		for(i=un-2; i>=0; i--) {
			pe[j++] = pa[i+un*(vn-1)];									// U�I���f�ʕۑ�
		}
	} else {															// <U�J&V�̏ꍇ>
		for(i=0; i<vn-1; i++) {
			ps[i] = pa[i*un];											// V�J�n�f�ʕۑ�
		}
		for(i=vn-2; i>=0; i--) {
			pe[j++] = pa[(i+1)*un-1];									// V�I���f�ʕۑ�
		}
	}
}

/*******************************************************/
static int CheckFacePoint(int pn, PNTTYPE* pa,
                          int* f1, int* f2, double* th)					//<<< �f�ʕ����m�F
/*******************************************************/
{
	PNTTYPE pt[3];
	VECTYPE vc1, vc2;
	int     i, j, j1, d, ch1, ch2, cnt=0;
	double  th1, th2, ln1, ln2;

	CalcNVec(pa, pn, &vc1);												// �@���޸�َ擾
	for(i=0, d=0, th1=0.0; i<DIM; i++) {
		if(th1<fabs(vc1.p[i])) {										// �ő�l���W�����擾
			th1 = fabs(vc1.p[i]), d = i;
		}
	}
	for(i=0; i<pn; i++) {												// �A��3�_�𒊏o
		if(i==0) {
			pt[0] = pa[pn-1], pt[1] = pa[0],    pt[2] = pa[1];
		} else if(i==pn-1) {
			pt[0] = pa[pn-2], pt[1] = pa[pn-1], pt[2] = pa[0];
		} else {
			pt[0] = pa[i-1],  pt[1] = pa[i],    pt[2] = pa[i+1];
		}
		CalcRotAngleVec(pt, &th1, &vc2);								// 3�_�Ԋp�x�擾
		ch1 = 1;														// �׸ޏ�����(���Ό�)
		ch1 = (vc1.p[d]<0.0&&vc2.p[d]<0.0) ? 0 : ch1;					// �ʖ@���޸�قƓ�������̏ꍇ��0
		ch1 = (vc1.p[d]>0.0&&vc2.p[d]>0.0) ? 0 : ch1;
		ch1 = (th1<0.139) ? 0 : ch1;									// �p�x8�x������0
		if(ch1) {														// <<�p�x���t�����_>>
			th1 = ((PI+th1)/2.0)-th1;									// ��p�x
			ln1 = DBL_MAX;
			ch2 = 0;
			for(j=0, j1=0; j<pn; j++) {
				if(j!=i) {
					pt[2] = pa[j];										// �Ώۓ_�ݒ�
					CalcRotAngleVec(pt, &th2, &vc2);					// �Ώۓ_�Ƃ̊p�x�擾
					if((vc1.p[d]<0.0&&vc2.p[d]<0.0)||
						(vc1.p[d]>0.0&&vc2.p[d]>0.0)) {					// <�@��������v>
						if(fabs(th2-th1)<0.785) {						// <��p�x��45�x����>
							ln2 = PntD(&pt[1], &pa[j]);					// ��_�Ƃ̋����Z�o
							if(ln2<ln1) {								// �ŒZ�����ް���ۑ�
								j1 = j, ln1 = ln2, ch2 = 1;
							}
						}
					}
				}
			}
			if(ch2) {													// �������ԍ���ݒ�
				f1[cnt] = i, f2[cnt] = j1, th[cnt++] = th1;
			}
		}
	}
	return cnt;
}

/*************************************************/
static int CheckSectionPoint(int pn, PNTTYPE* pa)						//<<< �f�ʒ��_�����m�F
/*************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCPnt];
	int*     f1 = new int[View->m_MaxCPnt];
	int*     f2 = new int[View->m_MaxCPnt];
	double*  th = new double[View->m_MaxCPnt];
	int      i, i1, i2, j, n1, n2, n3, st, ed, c0, c1, c2, c3, e1, e2, sp, cnt=0;
	double   sth;

	for(i=0; i<pn; i++) {												// ���_��񕡎�
		p1[i] = pa[i];
	}
	n1 = pn;
	while(TRUE) {
		c0 = CheckFacePoint(n1, p1, f1, f2, th);						// �f�ʕ����m�F
		if(c0==0) {														// <�f�ʕ�����>
			break;
		} else {														// <�f�ʕ����L>
			n2 = 0, i1 = 0, i2 = 0, sp = 1, c3 = INT_MAX;
			for(i=0; i<c0; i++) {
				if(f1[i]<f2[i]) {										// �Œ[�ŏ��������������
					st = f1[i], ed = f2[i];
				} else {
					st = f2[i], ed = f1[i];
				}
				for(j=0, c1=0, c2=0, e1=0, e2=0; j<c0; j++) {
					if(j!=i) {
						if(f1[j]<st||f1[j]>ed) {
							e1 = 1;
						}
						if(f2[j]<st||f2[j]>ed) {
							c1++;
						}
						if(f1[j]>st&&f1[j]<ed) {
							e2 = 1;
						}
						if(f2[j]>st&&f2[j]<ed) {
							c2++;
						}
						if(e1&&e2) {
							break;
						}
					}
				}
				if((!e1)&&c1<c3) {										// �����������ݒ�
					i1 = i, i2 = 0, c3 = c1, sp = 0;
				}
				if((!e2)&&c2<c3) {
					i1 = i, i2 = 1, c3 = c2, sp = 0;
				}
			}
			if(sp) {													// <�����ް���>
				for(sth=PI, i=0, i1=0; i<c0; i++) {
					if(sth>th[i]) {										// ��p�x�ŏ�����
						i1 = i, sth = th[i];
					}
				}
				if(f1[i1]<f2[i1]) {										// �ŏ��ʒu�ɕ�����
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				for(n2=0, i=0; i<=st; i++) {							// ������ʒ��_���m�F
					n2++;
				}
				for(i=ed; i<n1; i++) {
					n2++;
				}
				for(n3=0, i=st; i<=ed; i++) {
					n3++;
				}
				if(n2>2&&n3>2) {										// 2�ʂƂ����핪����UP
					cnt++;
				}
				break;													// �����I��
			} else {													// <�����ް��L>
				if(f1[i1]<f2[i1]) {										// �����ʒu�ɕ�����
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				if(!i2) {												// ���񕪊��ʐݒ�
					for(i=0; i<=st; i++) {
						p2[n2++] = p1[i];
					}
					for(i=ed; i<n1; i++) {
						p2[n2++] = p1[i];
					}
				} else {
					for(i=st; i<=ed; i++) {
						p2[n2++] = p1[i];
					}
				}
				if(n2>2) {												// <�L�������ʂ̏ꍇ>
					cnt++;												// ������UP
					if(!i2) {											// ���񕪊��ʂ̐ݒ�
						for(n2=0, i=st; i<=ed; i++) {
							p2[n2++] = p1[i];
						}
					} else {
						for(n2=0, i=0; i<=st; i++) {
							p2[n2++] = p1[i];
						}
						for(i=ed; i<n1; i++) {
							p2[n2++] = p1[i];
						}
					}
					for(i=0; i<n2; i++) {
						p1[i] = p2[i];									// ���m�F�ް����
					}
					n1 = n2;
				} else {												// <���������ʂ̏ꍇ>
					for(sth=PI, i=0, i1=0; i<c0; i++) {
						if(sth>th[i]) {									// ��p�x�ŏ�����
							i1 = i, sth = th[i];
						}
					}
					if(f1[i1]<f2[i1]) {									// �ŏ��ʒu�ɕ�����
						st = f1[i1], ed = f2[i1];
					} else {
						st = f2[i1], ed = f1[i1];
					}
					for(n2=0, i=0; i<=st; i++) {						// ������̖ʒ��_���m�F
						n2++;
					}
					for(i=ed; i<n1; i++) {
						n2++;
					}
					for(n3=0, i=st; i<=ed; i++) {
						n3++;
					}
					if(n2>2&&n3>2) {									// 2�ʂƂ����핪����UP
						cnt++;
					}
					break;												// �����I��
				}
			}
		}
	}
	delete[] p1;
	delete[] p2;
	delete[] f1;
	delete[] f2;
	delete[] th;
	return cnt;
}

/***************************************************/
static int UPointCheck(int un, int vn, PNTTYPE* pa)						//<<< U����_����m�F
/***************************************************/
{
	PNTTYPE pnt;
	int     i, j, chk, ret=0;

	pnt = pa[0];
	for(i=1, chk=0; i<un; i++) {										// <�擪Uٰ�ߊm�F>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i].p[j])>EPSILON) {						// ����_�łȂ�
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// ����_��1���Z
		ret += 1;
	}
	pnt = pa[un*(vn-1)];
	for(i=un*(vn-1)+1, chk=0; i<un*vn; i++) {							// <�ŏIUٰ�ߊm�F>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i].p[j])>EPSILON) {						// ����_�łȂ�
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// ����_��2���Z
		ret += 2;
	}
	return ret;
}

/***************************************************/
static int VPointCheck(int un, int vn, PNTTYPE* pa)						//<<< V����_����m�F
/***************************************************/
{
	PNTTYPE pnt;
	int     i, j, chk, ret=0;

	pnt = pa[0];
	for(i=1, chk=0; i<vn; i++) {										// <�擪Vٰ�ߊm�F>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i*un].p[j])>EPSILON) {					// ����_�łȂ�
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// ����_��1���Z
		ret += 1;
	}
	pnt = pa[un-1];
	for(i=1, chk=0; i<vn; i++) {										// <�ŏIVٰ�ߊm�F>
		for(j=0; j<DIM; j++) {
			if(fabs(pnt.p[j]-pa[i*un+un-1].p[j])>EPSILON) {				// ����_�łȂ�
				chk = 1; break;
			}
		}
	}
	if(chk==0) {														// ����_��2���Z
		ret += 2;
	}
	return ret;
}

/**************************************************/
static void SetVtx(OBJTYPE* op, PNTTYPE* pa,
                   int ub, int vb, int un, int vn)						//<<< �Ȗʒ��_�o�^
/**************************************************/
{
	int i, j, uu, vno=0;

	uu = un;															// U����_���ۑ�
	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	for(j=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i+uu*j]);								// ���_�o�^
		}
	}
}

/********************************************************/
static void SetVtxU(OBJTYPE* op, int flg,
                    PNTTYPE* pa, int ub, int un, int vn)				//<<< ���_�o�^(U����_)
/********************************************************/
{
	int i, j, uu, vno=0;

	uu = un;															// U����_���ۑ�
	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(flg==1||flg==3) {												// <�擪Uٰ�ߓ���>
		PutVtx(op, vno++, pa[0]);										// ���_�o�^
	} else {															// <�擪Uٰ�ߒʏ�>
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i]);
		}
	}
	for(j=1; j<vn-1; j++) {
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i+uu*j]);
		}
	}
	if(flg==2||flg==3) {												// <�ŏIUٰ�ߓ���>
		PutVtx(op, vno++, pa[uu*(vn-1)]);
	} else {															// <�ŏIUٰ�ߒʏ�>
		for(i=0; i<un; i++) {
			PutVtx(op, vno++, pa[i+uu*(vn-1)]);
		}
	}
}

/********************************************************/
static void SetVtxV(OBJTYPE* op, int flg,
                    PNTTYPE* pa, int vb, int un, int vn)				//<<< ���_�o�^(V����_)
/********************************************************/
{
	int i, j, vno=0;

	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	for(i=0; i<un; i++) {
		PutVtx(op, vno++, pa[i]);										// ���_�o�^
	}
	if(flg==1) {														// <�擪Vٰ�ߓ���>
		for(j=1; j<vn; j++) {
			for(i=1; i<un; i++) {
				PutVtx(op, vno++, pa[i+un*j]);
			}
		}
	} else if(flg==2) {													// <�ŏIVٰ�ߓ���>
		for(j=1; j<vn; j++) {
			for(i=0; i<un-1; i++) {
				PutVtx(op, vno++, pa[i+un*j]);
			}
		}
	} else {															// <�擪/�ŏI����>
		for(j=1; j<vn; j++) {
			for(i=1; i<un-1; i++) {
				PutVtx(op, vno++, pa[i+un*j]);
			}
		}
	}
}

/**************************************************/
static void SetFac(int* lnum, OBJTYPE* op,
                   int ub, int vb, int un, int vn)						//<<< �Ȗʖ�ٰ��1�o�^
/**************************************************/
{
	int i, j, lno;

	lno = *lnum;
	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	if(ub&&vb) {														// <U��&V�̏ꍇ>
		for(j=0; j<vn; j++) {
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*j+i, 0);							// ��ٰ�ߓo�^
				if(i==un-1) {
					PutFlp1(op, lno++, un*j, 0);
				} else {
					PutFlp1(op, lno++, un*j+i+1, 0);
				}
				if(j==vn-1) {
					if(i==un-1) {
						PutFlp1(op, lno++, 0, 0);
					} else {
						PutFlp1(op, lno++, i+1, 0);
					}
					PutFlp1(op, lno++, i, 1);
				} else {
					if(i==un-1) {
						PutFlp1(op, lno++, un*(j+1), 0);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					}
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		}
	} else if(ub&&(!vb)) {												// <U��&V�J�̏ꍇ>
		for(j=0; j<vn-1; j++) {
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*j+i, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, un*j, 0);
					PutFlp1(op, lno++, un*(j+1), 0);
				} else {
					PutFlp1(op, lno++, un*j+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
				}
				PutFlp1(op, lno++, un*(j+1)+i, 1);
			}
		}
	} else if((!ub)&&vb) {												// <U�J&V�̏ꍇ>
		for(j=0; j<vn; j++) {
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*j+i, 0);
				PutFlp1(op, lno++, un*j+i+1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, i+1, 0);
					PutFlp1(op, lno++, i, 1);
				} else {
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		}
	} else {															// <U�J&V�J�̏ꍇ>
		for(j=0; j<vn-1; j++) {
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*j+i, 0);
				PutFlp1(op, lno++, un*j+i+1, 0);
				PutFlp1(op, lno++, un*(j+1)+i+1, 0);
				PutFlp1(op, lno++, un*(j+1)+i, 1);
			}
		}
	}
	*lnum = lno;
}

/****************************************************/
static void SetFacU(int* lnum, OBJTYPE* op,
                    int flg, int ub, int un, int vn)					//<<< ��ٰ��1�o�^(U����_)
/****************************************************/
{
	int i, j, lno;

	lno = *lnum;
	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(flg==1) {														// <<�擪Uٰ�ߓ���>>
		if(ub) {														// <U�̏ꍇ>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, 0, 0);								// ��ٰ�ߓo�^
				if(i==un-1) {
					PutFlp1(op, lno++, 1, 0);
				} else {
					PutFlp1(op, lno++, i+2, 0);
				}
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-1; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					if(i==un-1) {
						PutFlp1(op, lno++, un*(j-1)+1, 0);
						PutFlp1(op, lno++, un*j+1, 0);
					} else {
						PutFlp1(op, lno++, un*(j-1)+i+2, 0);
						PutFlp1(op, lno++, un*j+i+2, 0);
					}
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
		} else {														// <U�J�̏ꍇ>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, i+2, 0);
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-1; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					PutFlp1(op, lno++, un*(j-1)+i+2, 0);
					PutFlp1(op, lno++, un*j+i+2, 0);
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
		}
	} else if(flg==2) {													// <<�ŏIUٰ�ߓ���>>
		if(ub) {														// <U�̏ꍇ>
			for(j=0; j<vn-2; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, (un*j)+i, 0);
					if(i==un-1) {
						PutFlp1(op, lno++, un*j, 0);
						PutFlp1(op, lno++, un*(j+1), 0);
					} else {
						PutFlp1(op, lno++, un*j+i+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					}
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*(vn-2)+i, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, un*(vn-2), 0);
				} else {
					PutFlp1(op, lno++, un*(vn-2)+i+1, 0);
				}
				PutFlp1(op, lno++, un*(vn-1), 1);
			}
		} else {														// <U�J�̏ꍇ>
			for(j=0; j<vn-2; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*j+i, 0);
					PutFlp1(op, lno++, un*j+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*(vn-2)+i, 0);
				PutFlp1(op, lno++, un*(vn-2)+i+1, 0);
				PutFlp1(op, lno++, un*(vn-1), 1);
			}
		}
	} else {															// <<�擪/�ŏI����>>
		if(ub) {														// <U�̏ꍇ>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, 0, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, 1, 0);
				} else {
					PutFlp1(op, lno++, i+2, 0);
				}
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-2; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					if(i==un-1) {
						PutFlp1(op, lno++, un*(j-1)+1, 0);
						PutFlp1(op, lno++, un*j+1, 0);
					} else {
						PutFlp1(op, lno++, un*(j-1)+i+2, 0);
						PutFlp1(op, lno++, un*j+i+2, 0);
					}
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
			for(i=0; i<un; i++) {
				PutFlp1(op, lno++, un*(vn-3)+i+1, 0);
				if(i==un-1) {
					PutFlp1(op, lno++, un*(vn-3)+1, 0);
				} else {
					PutFlp1(op, lno++, un*(vn-3)+i+2, 0);
				}
				PutFlp1(op, lno++, un*(vn-2)+1, 1);
			}
		} else {														// <U�J�̏ꍇ>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, i+2, 0);
				PutFlp1(op, lno++, i+1, 1);
			}
			for(j=1; j<vn-2; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*(j-1)+i+1, 0);
					PutFlp1(op, lno++, un*(j-1)+i+2, 0);
					PutFlp1(op, lno++, un*j+i+2, 0);
					PutFlp1(op, lno++, un*j+i+1, 1);
				}
			}
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno++, un*(vn-3)+i+1, 0);
				PutFlp1(op, lno++, un*(vn-3)+i+2, 0);
				PutFlp1(op, lno++, un*(vn-2)+1, 1);
			}
		}
	}
	*lnum = lno;
}

/****************************************************/
static void SetFacV(int* lnum, OBJTYPE* op,
                    int flg, int vb, int un, int vn)					//<<< ��ٰ��1�o�^(V����_)
/****************************************************/
{
	int i, j, ms1, ms2, lno;

	lno = *lnum;
	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	if(flg==1) {														// <<�擪Vٰ�ߓ���>>
		if(vb) {														// <V�̏ꍇ>
			for(j=0; j<vn; j++) {
				PutFlp1(op, lno++, 0, 0);								// ��ٰ�ߓo�^
				PutFlp1(op, lno++, (un-1)*j+1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, 1, 1);
				} else {
					PutFlp1(op, lno++, (un-1)*(j+1)+1, 1);
				}
				for(i=1; i<un-1; i++) {
					PutFlp1(op, lno++, (un-1)*j+i, 0);
					PutFlp1(op, lno++, (un-1)*j+i+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, (un-1)*(j+1)+i+1, 0);
						PutFlp1(op, lno++, (un-1)*(j+1)+i, 1);
					}
				}
			}
		} else {														// <V�J�̏ꍇ>
			for(j=0; j<vn-1; j++) {
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, (un-1)*j+1, 0);
				PutFlp1(op, lno++, (un-1)*(j+1)+1, 1);
				for(i=1; i<un-1; i++) {
					PutFlp1(op, lno++, (un-1)*j+i, 0);
					PutFlp1(op, lno++, (un-1)*j+i+1, 0);
					PutFlp1(op, lno++, (un-1)*(j+1)+i+1, 0);
					PutFlp1(op, lno++, (un-1)*(j+1)+i, 1);
				}
			}
		}
	} else if(flg==2) {													// <<�ŏIVٰ�ߓ���>>
		if(vb) {														// <V�̏ꍇ>
			for(j=0; j<vn; j++) {
				if(j==0) {												// �␳(-)���ݒ�
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = j-1, ms2 = j;
				}
				for(i=0; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
					}
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, un-2, 1);
				} else {
					PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
				}
			}
		} else {														// <V�J�̏ꍇ>
			for(j=0; j<vn-1; j++) {
				if(j==0) {
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = j-1, ms2 = j;
				}
				for(i=0; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
			}
		}
	} else {															// <<�擪/�ŏI����>>
		if(vb) {														// <V�̏ꍇ>
			for(j=0; j<vn; j++) {
				if(j==0) {												// �␳(-)���ݒ�
					ms1 = 0, ms2 = 1;
				} else if(j==1) {
					ms1 = 1, ms2 = 3;
				} else {
					ms1 = 2*j-1, ms2 = 2*j+1;
				}
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, un*j-ms1+1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, 1, 1);
				} else {
					PutFlp1(op, lno++, un*(j+1)-ms2+1, 1);
				}
				for(i=1; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
					}
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				if(j==vn-1) {
					PutFlp1(op, lno++, un-2, 1);
				} else {
					PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
				}
			}
		} else {														// <V�J�̏ꍇ>
			for(j=0; j<vn-1; j++) {
				if(j==0) {
					ms1 = 0, ms2 = 1;
				} else if(j==1) {
					ms1 = 1, ms2 = 3;
				} else {
					ms1 = 2*j-1, ms2 = 2*j+1;
				}
				PutFlp1(op, lno++, 0, 0);
				PutFlp1(op, lno++, un*j-ms1+1, 0);
				PutFlp1(op, lno++, un*(j+1)-ms2+1, 1);
				for(i=1; i<un-2; i++) {
					PutFlp1(op, lno++, un*j+i-ms1, 0);
					PutFlp1(op, lno++, un*j+i-ms1+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i-ms2, 1);
				}
				PutFlp1(op, lno++, un*(j+1)-ms1-2, 0);
				PutFlp1(op, lno++, un-1, 0);
				PutFlp1(op, lno++, un*(j+2)-ms2-2, 1);
			}
		}
	}
	*lnum = lno;
}

/********************************************************/
static void SetSectionFace(int* num, OBJTYPE* op,
                           int pn, PNTTYPE* pa, int* va)				//<<< �f�ʒ��_ٰ�ߐݒ�
/********************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCPnt];
	int*     vv = new int[View->m_MaxCPnt];
	int*     f1 = new int[View->m_MaxCPnt];
	int*     f2 = new int[View->m_MaxCPnt];
	double*  th = new double[View->m_MaxCPnt];
	int      i, i1, i2, j, wn, st, ed, c0, c1, c2, c3, e1, e2, sp, lno;
	double   sth;

	lno = *num;
	while(TRUE) {
		c0 = CheckFacePoint(pn, pa, f1, f2, th);						// �f�ʕ����m�F
		if(c0==0) {														// <<<�f�ʕ�����>>>
			for(i=0; i<pn-1; i++) {
				PutFlp1(op, lno++, va[i], 0);							// ��ٰ�ߓo�^
			}
			PutFlp1(op, lno++, va[i], 1);
			break;														// �ݒ芮��
		} else {														// <<<�f�ʕ����L>>>
			wn = 0, i1 = 0, i2 = 0, sp = 1, c3 = INT_MAX;
			for(i=0; i<c0; i++) {
				if(f1[i]<f2[i]) {										// �Œ[�ŏ��������������
					st = f1[i], ed = f2[i];
				} else {
					st = f2[i], ed = f1[i];
				}
				for(j=0, c1=0, c2=0, e1=0, e2=0; j<c0; j++) {
					if(j!=i) {
						if(f1[j]<st||f1[j]>ed) {
							e1 = 1;
						}
						if(f2[j]<st||f2[j]>ed) {
							c1++;
						}
						if(f1[j]>st&&f1[j]<ed) {
							e2 = 1;
						}
						if(f2[j]>st&&f2[j]<ed) {
							c2++;
						}
						if(e1&&e2) {
							break;
						}
					}
				}
				if((!e1)&&c1<c3) {										// �����������ݒ�
					i1 = i, i2 = 0, c3 = c1, sp = 0;
				}
				if((!e2)&&c2<c3) {
					i1 = i, i2 = 1, c3 = c2, sp = 0;
				}
			}
			if(sp) {													// <<�����ް���>>
				for(sth=PI, i=0, i1=0; i<c0; i++) {
					if(sth>th[i]) {										// ��p�x�ŏ�����
						i1 = i, sth = th[i];
					}
				}
				if(f1[i1]<f2[i1]) {										// �ŏ��ʒu������
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				for(wn=0, i=0; i<=st; i++) {							// ����������ٰ�ߒ��o
					vv[wn++] = va[i];
				}
				for(i=ed; i<pn; i++) {
					vv[wn++] = va[i];
				}
				if(wn>2) {												// <�L��ٰ�߂̏ꍇ>
					for(i=0; i<wn-1; i++) {
						PutFlp1(op, lno++, vv[i], 0);
					}
					PutFlp1(op, lno++, vv[i], 1);
				}
				for(wn=0, i=st; i<=ed; i++) {							// ����������ٰ�ߒ��o
					vv[wn++] = va[i];
				}
				if(wn>2) {												// <�L��ٰ�߂̏ꍇ>
					for(i=0; i<wn-1; i++) {
						PutFlp1(op, lno++, vv[i], 0);
					}
					PutFlp1(op, lno++, vv[i], 1);
				}
				break;													// �ݒ芮��
			} else {													// <<�����ް��L>>
				if(f1[i1]<f2[i1]) {										// �����ʒu������
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				if(!i2) {												// ���񕪊��ʐݒ�
					for(i=0; i<=st; i++) {
						vv[wn++] = va[i];
					}
					for(i=ed; i<pn; i++) {
						vv[wn++] = va[i];
					}
				} else {
					for(i=st; i<=ed; i++) {
						vv[wn++] = va[i];
					}
				}
				if(wn>2) {												// <�L��ٰ�߂̏ꍇ>
					for(i=0; i<wn-1; i++) {
						PutFlp1(op, lno++, vv[i], 0);
					}
					PutFlp1(op, lno++, vv[i], 1);
					if(!i2) {											// ���񕪊��ʐݒ�
						for(wn=0, i=st; i<=ed; i++) {
							pp[wn] = pa[i], vv[wn++] = va[i];
						}
					} else {
						for(wn=0, i=0; i<=st; i++) {
							pp[wn] = pa[i], vv[wn++] = va[i];
						}
						for(i=ed; i<pn; i++) {
							pp[wn] = pa[i], vv[wn++] = va[i];
						}
					}
					for(i=0; i<wn; i++) {								// ���ް����
						pa[i] = pp[i], va[i] = vv[i];
					}
					pn = wn;
				} else {												// <���������ʂ̏ꍇ>
					for(sth=PI, i=0, i1=0; i<c0; i++) {
						if(sth>th[i]) {									// ��p�x�ŏ�����
							i1 = i, sth = th[i];
						}
					}
					if(f1[i1]<f2[i1]) {									// �ŏ��ʒu������
						st = f1[i1], ed = f2[i1];
					} else {
						st = f2[i1], ed = f1[i1];
					}
					for(wn=0, i=0; i<=st; i++) {						// ����������ٰ�ߒ��o
						vv[wn++] = va[i];
					}
					for(i=ed; i<pn; i++) {
						vv[wn++] = va[i];
					}
					if(wn>2) {											// <�L��ٰ�߂̏ꍇ>
						for(i=0; i<wn-1; i++) {
							PutFlp1(op, lno++, vv[i], 0);
						}
						PutFlp1(op, lno++, vv[i], 1);
					}
					for(wn=0, i=st; i<=ed; i++) {						// ����������ٰ�ߒ��o
						vv[wn++] = va[i];
					}
					if(wn>2) {											// <�L��ٰ�߂̏ꍇ>
						for(i=0; i<wn-1; i++) {
							PutFlp1(op, lno++, vv[i], 0);
						}
						PutFlp1(op, lno++, vv[i], 1);
					}
					break;												// �ݒ芮��
				}
			}
		}
	}
	*num = lno;
	delete[] pp;
	delete[] vv;
	delete[] f1;
	delete[] f2;
	delete[] th;
}

/*************************************************************/
static void SetFacS(int* lnum, OBJTYPE* op, int ub, int vb,
                    int un, int vn, PNTTYPE* ps, PNTTYPE* pe)			//<<< �f�ʖ�ٰ�ߓo�^
/*************************************************************/
{
	int* va = new int[View->m_MaxCPnt];
	int  i, j=0;

	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	if(ub&&(!vb)) {														// <<<U��&V�J�̏ꍇ>>>
		for(i=un-1; i>=0; i--) {										// U�J�n�f�ʒ��_�ԍ����o
			va[j++] = i;
		}
		SetSectionFace(lnum, op, un, ps, va);							// U�J�n�f�ʒ��_ٰ�ߐݒ�
		for(i=0; i<un; i++) {											// U�I���f�ʒ��_�ԍ����o
			va[i] = i+un*(vn-1);
		}
		SetSectionFace(lnum, op, un, pe, va);							// U�I���f�ʒ��_ٰ�ߐݒ�
	} else if((!ub)&&vb) {												// <<<U�J&V�̏ꍇ>>>
		for(i=0; i<vn; i++) {											// V�J�n�f�ʒ��_�ԍ����o
			va[i] = i*un;
		}
		SetSectionFace(lnum, op, vn, ps, va);							// V�J�n�f�ʒ��_ٰ�ߐݒ�
		for(i=vn-1; i>=0; i--) {										// V�I���f�ʒ��_�ԍ����o
			va[j++] = i*un+un-1;
		}
		SetSectionFace(lnum, op, vn, pe, va);							// V�I���f�ʒ��_ٰ�ߐݒ�
	}
	delete[] va;
}

/**************************************************************/
static void SetFacUS(int* lnum, OBJTYPE* op, int ufg,
                     int un, int vn, PNTTYPE* ps, PNTTYPE* pe)			//<<< �f�ʖ�ٰ�ߓo�^(U)
/**************************************************************/
{
	int* va = new int[View->m_MaxCPnt];
	int  i, j=0;

	un--;																// U�͎n�_/�I�_����
	if(ufg==1) {														// <<<�擪Uٰ�ߓ���>>>
		for(i=un-1; i>=0; i--) {										// U�I���f�ʒ��_�ԍ����o
			va[j++] = un*(vn-2)+i+1;
		}
		SetSectionFace(lnum, op, un, pe, va);							// U�I���f�ʒ��_ٰ�ߐݒ�
	} else {															// <<<�ŏIUٰ�ߓ���>>>
		for(i=0; i<un; i++) {											// U�J�n�f�ʒ��_�ԍ����o
			va[i] = i;
		}
		SetSectionFace(lnum, op, un, ps, va);							// U�J�n�f�ʒ��_ٰ�ߐݒ�
	}
	delete[] va;
}

/**************************************************************/
static void SetFacVS(int* lnum, OBJTYPE* op, int vfg,
                     int un, int vn, PNTTYPE* ps, PNTTYPE* pe)			//<<< �f�ʖ�ٰ�ߓo�^(V)
/**************************************************************/
{
	int* va = new int[View->m_MaxCPnt];
	int  i, j=0, ms;

	vn--;																// V�͎n�_/�I�_����
	if(vfg==1) {														// <<<�擪Vٰ�ߓ���>>>
		for(i=vn-1; i>=0; i--) {										// V�I���f�ʒ��_�ԍ����o
			va[j++] = (un-1)*(i+1);
		}
		SetSectionFace(lnum, op, vn, pe, va);							// V�I���f�ʒ��_ٰ�ߐݒ�
	} else {															// <<<�ŏIVٰ�ߓ���>>>
		for(i=0; i<vn; i++) {
			ms = (i<2) ? 0 : i-1;
			va[i] = i*un-ms;											// V�J�n�f�ʒ��_�ԍ����o
		}
		SetSectionFace(lnum, op, vn, ps, va);							// V�J�n�f�ʒ��_ٰ�ߐݒ�
	}
	delete[] va;
}

/***********************************************************/
static void SetFac2(OBJTYPE* op, int ub, int vb,
                    int un, int vn, int uo, int vo, int mx)				//<<< �Ȗʖ�ٰ��2�o�^
/***********************************************************/
{
	int i, j, div, lno=0;

	for(i=0, div=1; i<mx; i++) {										// �����}�����v�Z
		div = div*2;
	}
	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	if(ub) {															// <<U�̏ꍇ>>
		for(i=0; i<vo; i++) {
			for(j=0; j<un; j++) {
				PutFlp2(op, lno++, div*un*i+j, 0);
			}
			PutFlp2(op, lno++, div*un*i, 1);
		}
	} else {															// <<U�J�̏ꍇ>>
		for(i=0; i<vo; i++) {
			for(j=0; j<un-1; j++) {
				PutFlp2(op, lno++, div*un*i+j, 0);
			}
			PutFlp2(op, lno++, div*un*i+j, 1);
		}
	}
	if(vb) {															// <<V�̏ꍇ>>
		for(i=0; i<uo; i++) {
			for(j=0; j<vn; j++) {
				PutFlp2(op, lno++, i*div+un*j, 0);
			}
			PutFlp2(op, lno++, i*div, 1);
		}
	} else {															// <<V�J�̏ꍇ>>
		for(i=0; i<uo; i++) {
			for(j=0; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*j, 0);
			}
			PutFlp2(op, lno++, i*div+un*j, 1);
		}
	}
}

/************************************************************/
static void SetFacU2(OBJTYPE* op, int flg, int ub,
                     int un, int vn, int uo, int vo, int mx)			//<<< ��ٰ��2�o�^(U����)
/************************************************************/
{
	int i, j, div, lno=0;

	for(i=0, div=1; i<mx; i++) {										// �����}�����v�Z
		div = div*2;
	}
	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(flg==1) {														// <<�擪Uٰ�ߓ���>>
		if(ub) {														// <U�̏ꍇ>
			for(i=1; i<vo; i++) {
				for(j=0; j<un; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+1, 1);
			}
		} else {														// <U�J�̏ꍇ>
			for(i=1; i<vo; i++) {
				for(j=0; j<un-1; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+j+1, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno++, 0, 0);
			for(j=1; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*(j-1)+1, 0);
			}
			PutFlp2(op, lno++, i*div+un*(j-1)+1, 1);
		}
	} else if(flg==2) {													// <<�ŏIUٰ�ߓ���>>
		if(ub) {														// <U�̏ꍇ>
			for(i=0; i<vo-1; i++) {
				for(j=0; j<un; j++) {
					PutFlp2(op, lno++, un*div*i+j, 0);
				}
				PutFlp2(op, lno++, un*div*i, 1);
			}
		} else {														// <U�J�̏ꍇ>
			for(i=0; i<vo-1; i++) {
				for(j=0; j<un-1; j++) {
					PutFlp2(op, lno++, un*div*i+j, 0);
				}
				PutFlp2(op, lno++, un*div*i+j, 1);
			}
		}
		for(i=0; i<uo; i++) {
			for(j=0; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*j, 0);
			}
			PutFlp2(op, lno++, un*(vn-1), 1);
		}
	} else {															// <<�擪/�ŏI����>>
		if(ub) {														// <U�̏ꍇ>
			for(i=1; i<vo-1; i++) {
				for(j=0; j<un; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+1, 1);
			}
		} else {														// <U�J�̏ꍇ>
			for(i=1; i<vo-1; i++) {
				for(j=0; j<un-1; j++) {
					PutFlp2(op, lno++, un*(div*i-1)+j+1, 0);
				}
				PutFlp2(op, lno++, un*(div*i-1)+j+1, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno++, 0, 0);
			for(j=1; j<vn-1; j++) {
				PutFlp2(op, lno++, i*div+un*(j-1)+1, 0);
			}
			PutFlp2(op, lno++, un*(vn-2)+1, 1);
		}
	}
}

/************************************************************/
static void SetFacV2(OBJTYPE* op, int flg, int vb,
                     int un, int vn, int uo, int vo, int mx)			//<<< ��ٰ��2�o�^(V����)
/************************************************************/
{
	int i, j, div, ms, lno=0;

	for(i=0, div=1; i<mx; i++) {										// �����}�����v�Z
		div = div*2;
	}
	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	if(flg==1) {														// <�擪Vٰ�ߓ���>
		for(i=0; i<vo; i++) {
			PutFlp2(op, lno++, 0, 0);									// ��ٰ�ߓo�^
			for(j=1; j<un-1; j++) {
				PutFlp2(op, lno++, (un-1)*div*i+j, 0);
			}
			PutFlp2(op, lno++, (un-1)*div*i+j, 1);
		}
		if(vb) {														// <V�̏ꍇ>
			for(i=1; i<uo; i++) {
				for(j=0; j<vn; j++) {
					PutFlp2(op, lno++, i*div+(un-1)*j, 0);
				}
				PutFlp2(op, lno++, i*div, 1);
			}
		} else {														// <V�J�̏ꍇ>
			for(i=1; i<uo; i++) {
				for(j=0; j<vn-1; j++) {
					PutFlp2(op, lno++, i*div+(un-1)*j, 0);
				}
				PutFlp2(op, lno++, i*div+(un-1)*j, 1);
			}
		}
	} else if(flg==2) {													// <�ŏIVٰ�ߓ���>
		for(i=0; i<vo; i++) {
			ms = (div*i<2) ? 0 : div*i-1;								// �␳(-)���ݒ�
			for(j=0; j<un-1; j++) {
				PutFlp2(op, lno++, un*div*i+j-ms, 0);
			}
			PutFlp2(op, lno++, un-1, 1);
		}
		if(vb) {														// <V�̏ꍇ>
			for(i=0; i<uo-1; i++) {
				for(j=0; j<vn; j++) {
					ms = (j<2) ? 0 : j-1;
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				PutFlp2(op, lno++, i*div, 1);
			}
		} else {														// <V�J�̏ꍇ>
			for(i=0; i<uo-1; i++) {
				for(j=0; j<vn-1; j++) {
					ms = (j<2) ? 0 : j-1;
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				ms = (j<2) ? 0 : j-1;
				PutFlp2(op, lno++, i*div+un*j-ms, 1);
			}
		}
	} else {															// <�擪/�ŏI����>
		for(i=0; i<vo; i++) {
			if(div*i==0) {												// �␳(-)���ݒ�
				ms = 0;
			} else if(div*i==1) {
				ms = 1;
			} else {
				ms = 2*div*i-1;
			}
			PutFlp2(op, lno++, 0, 0);
			for(j=1; j<un-1; j++) {
				PutFlp2(op, lno++, un*div*i+j-ms, 0);
			}
			PutFlp2(op, lno++, un-1, 1);
		}
		if(vb) {														// <V�̏ꍇ>
			for(i=1; i<uo-1; i++) {
				for(j=0; j<vn; j++) {
					if(j==0) {
						ms = 0;
					} else if(j==1) {
						ms = 1;
					} else {
						ms = 2*j-1;
					}
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				PutFlp2(op, lno++, i*div, 1);
			}
		} else {														// <V�J�̏ꍇ>
			for(i=1; i<uo-1; i++) {
				for(j=0; j<vn-1; j++) {
					if(j==0) {
						ms = 0;
					} else if(j==1) {
						ms = 1;
					} else {
						ms = 2*j-1;
					}
					PutFlp2(op, lno++, i*div+un*j-ms, 0);
				}
				if(j==0) {
					ms = 0;
				} else if(j==1) {
					ms = 1;
				} else {
					ms = 2*j-1;
				}
				PutFlp2(op, lno++, i*div+un*j-ms, 1);
			}
		}
	}
}

/***********************************************************/
static void SCurve(int* pn, int bc, double wt, PNTTYPE* pa)				//<<< �A���Ȑ�����
/***********************************************************/
{
	PNTTYPE* rp = new PNTTYPE[View->m_MaxCPnt];
	int      rn, i, j;

	if(*pn<2) {															// ����_��2�����͒��~
		delete[] rp;
		return;
	}
	rn = *pn;
	if(rn>2) {															// <����_��3�ȏ�>
		for(j=0; j<DIM; j++) {
			rp[1].p[j] = (pa[0].p[j]+pa[1].p[j])/2.0;					// ���ԓ_�擪�Ȑ��v�Z
		}
		for(i=0; i<rn-2; i++) {
			for(j=0; j<DIM; j++) {										// �擪�ȊO�Ȑ��v�Z
				rp[2*i+3].p[j] = (pa[i+1].p[j]+pa[i+2].p[j])/2.0;
				rp[2*i+2].p[j] = (rp[2*i+1].p[j]+rp[2*i+3].p[j]+
                                  wt*pa[i+1].p[j])/(wt+2.0);
			}
		}
		if(bc) {														// <���E���̏ꍇ>
			for(j=0; j<DIM; j++) {										// �n�_�Ȑ��v�Z
				rp[0].p[j] = (rp[1].p[j]+rp[2*(rn-1)-1].p[j]+
                              wt*pa[0].p[j])/(wt+2.0);
			}
			rp[2*(rn-1)] = rp[0];										// �I�_�͎n�_�Ɠ���
		} else {														// <���E���J�̏ꍇ>
			rp[0] = pa[0];												// �n�_/�I�_�ύX��
			rp[2*(rn-1)] = pa[rn-1];
		}
	} else {															// <����_��2�ȉ�>
		for(i=0; i<rn-1; i++) {
			rp[2*i] = pa[i];
			for(j=0; j<DIM; j++) {										// ���ԓ_�v�Z
				rp[2*i+1].p[j] = (pa[i].p[j]+pa[i+1].p[j])/2.0;
			}
		}
		rp[2*(rn-1)] = pa[rn-1];
	}
	*pn = 2*rn-1;														// �V����_�u��
	for(i=0; i<*pn; i++) {
		pa[i] = rp[i];
	}
	delete[] rp;
}

/****************************************************/
static void SCurves(int flg, int* pn, int bc,
                    double wt, PNTTYPE* pa, int* ea)					//<<< S�Ȑ��̐���
/****************************************************/
{
	PNTTYPE* wpa = new PNTTYPE[View->m_MaxCPnt];
	int*     wea = new int[View->m_MaxCPnt];
	PNTTYPE  spa;
	int      sea, cn, en, wn, sn, nn, i, flg1, flg2;

	if(*pn<2) {															// ����_��2�������~
		delete[] wpa;
		delete[] wea;
		return;
	}
	for(i=0, en=0; i<*pn; i++) {
		if(ea[i]==flg||ea[i]==3) {										// �ܐ��_���m�F
			en++;
		}
	}
	cn = *pn;
	if(en==0) {															// <<�ܐ��_���̏ꍇ>>
		SCurve(&cn, bc, wt, pa);										// �A���Ȑ�����
		wea[0] = ea[0];													// �擪�Ȑ�Ӱ�ސݒ�
		for(i=1; i<*pn; i++) {
			wea[2*i-1] = (ea[i-1]>ea[i]) ? ea[i] : ea[i-1];				// �}���_�͑O��̏���ݒ�
			wea[2*i] = ea[i];											// �ݒ�ϓ_�s�ύX
		}
		for(i=0; i<cn; i++) {											// ��Ɨ̈��萳�K����
			ea[i] = wea[i];
		}
	} else {															// <<�ܐ��_�L�̏ꍇ>>
		wn = 0, sn = 0, en = 0;
		if(bc&&ea[0]!=flg&&ea[0]!=3) {									// <���E����&�ܐ��_��>
			cn--;
			while(1) {													// �擪���ܐ��_�܂ŌJ�Ԃ�
				if(ea[0]==flg||ea[0]==3) {
					break;
				}
				spa = pa[0], sea = ea[0];								// �擪�ް��ۑ�
				for(i=0; i<cn-1; i++) {									// 1�O�ɼ��
					pa[i] = pa[i+1], ea[i] = ea[i+1];
				}
				pa[cn-1] = spa,	ea[cn-1] = sea;							// �Ō�ɋ��擪�ݒ�
				en++;
			}
			pa[cn] = pa[0], ea[cn++] = ea[0];							// �擪�ǉ�
			en = 2*en;
		}
		while(1) {
			for(i=sn, nn=0; i<cn; i++) {								// �擪���珇���s��
				wpa[wn+(nn++)] = pa[i];
				if(i!=sn&&(ea[i]==flg||ea[i]==3)) {						// 2�_�̐ܐ��_�Ԑ���_���o
					sn = i; break;
				}
			}
			if(i==cn) {
				break;
			}
			SCurve(&nn, 0, wt, &wpa[wn]);								// �A���Ȑ�����
			wn = wn+nn-1;												// �ܐ��_�͑O��ŋ��ʎg�p
		}
		cn = wn+1;
		wea[0] = ea[0];													// �擪�Ȑ�Ӱ�ސݒ�
		flg1 = (ea[0]==flg||ea[0]==3) ? ea[0]-flg : ea[0];				// �ܐ��͊Y������Ӱ�ތ�
		for(i=1; i<*pn; i++) {
			flg2 = (ea[i]==flg||ea[i]==3) ? ea[i]-flg : ea[i];			// �ܐ��͊Y������Ӱ�ތ�
			wea[2*i-1] = (flg1>flg2) ? flg2 : flg1;						// �}���_�͑O��̏���ݒ�
			wea[2*i] = ea[i];											// �ݒ�ϓ_�s�ύX
			flg1 = flg2;												// �����_������O�_
		}
		if(en==0) {
			for(i=0; i<cn; i++) {										// ��Ɨ̈��萳�K����
				pa[i] = wpa[i], ea[i] = wea[i];
			}
		} else {
			for(i=0; i<cn-1; i++) {
				if(i+en<cn) {
					pa[i+en] = wpa[i], ea[i+en] = wea[i];
				} else {
					pa[i+en-cn+1] = wpa[i], ea[i+en-cn+1] = wea[i];
				}
			}
			pa[0] = pa[cn-1], ea[0] = ea[cn-1];
		}
	}
	*pn = cn;
	delete[] wpa;
	delete[] wea;
}

/*****************************************************************/
static void MakeSSurface(int* un, int* vn, int ub, int vb,
                         int mx, double wt, PNTTYPE* pa, int* ea)		//<<< S�Ȗʐ���
/*****************************************************************/
{
	PNTTYPE* t1 = new PNTTYPE[View->m_MaxSPnt];
	int*     e1 = new int[View->m_MaxSPnt];
	int      uu, vv, pn, i, j, k, m, u, v, s, uv;
	double   w1, w2;

	uu = *un, vv = *vn;													// U/V����_��
	for(i=0; i<mx; i++) {												// ���������J�Ԃ�
		for(j=0, s=0; j<vv; j++) {										// <<U����S�Ȑ���>>
			for(m=0; m<uu; m++) {										// U1�s���ް����o
				t1[s+m] = pa[uu*j+m], e1[s+m] = ea[uu*j+m];
			}
			u = uu;
			SCurves(1, &u, ub, wt, &t1[s], &e1[s]);						// S�Ȑ�����
			s = u*(j+1);
		}
		for(j=0; j<vv; j++) {
			for(m=0; m<u; m++) {										// U����S�Ȑ������ʋL��
				pa[2*u*j+m] = t1[u*j+m], ea[2*u*j+m] = e1[u*j+m];
			}
		}
		for(j=0, s=0; j<u; j++) {										// <<V����S�Ȑ���>>
			for(m=0; m<vv; m++) {										// V1�s���ް����o
				t1[s+m] = pa[2*u*m+j], e1[s+m] = ea[2*u*m+j];
			}
			v = vv;
			SCurves(2, &v, vb, wt, &t1[s], &e1[s]);						// S�Ȑ�����
			s = v*(j+1);
		}
		for(j=0; j<u; j++) {
			for(m=0; m<v; m++) {										// V����S�Ȑ������ʋL��
				pa[u*m+j] = t1[v*j+m], ea[u*m+j] = e1[v*j+m];
			}
		}
		uu = u, vv = v;													// �VU/V����_��
	}
	pn = uu*vv;
	for(j=0; j<pn; j++) {												// ���ܰ�����
		t1[j] = pa[j], e1[j] = ea[j];
	}
	for(i=1; i<vv-1; i++) {												// <<���ԓ_�����v�Z>>
		for(j=1; j<uu-1; j++) {
			uv = i*uu+j;
			if(e1[uv]==0) {												// <U&V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu-1].p[k]+t1[uv-uu+1].p[k]+
                         t1[uv+uu-1].p[k]+t1[uv+uu+1].p[k];
					w2 = t1[uv-uu].p[k]+t1[uv-1].p[k]+
                         t1[uv+1].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                                   t1[uv].p[k])/((wt+4.0)*(wt+4.0));
				}
			} else if(e1[uv]==1) {										// <V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			} else if(e1[uv]==2) {										// <U�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
		}
	}
	if(ub) {															// <<U���[�����v�Z>>
		for(i=1; i<vv-1; i++) {											// <U�̏ꍇ>
			uv = i*uu;
			if(e1[uv]==0) {												// <U&V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-2].p[k]+t1[uv-uu+1].p[k]+
                         t1[2*uu+uv-2].p[k]+t1[uv+uu+1].p[k];
					w2 = t1[uv-uu].p[k]+t1[uv+uu-2].p[k]+
                         t1[uv+1].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                                   t1[uv].p[k])/((wt+4.0)*(wt+4.0));
					pa[uv+uu-1].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==1) {										// <V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[uv+uu-1].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==2) {										// <U�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv+uu-2].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[uv+uu-1].p[k] = pa[uv].p[k];
				}
			}
		}
	} else {															// <U�J�̏ꍇ>
		for(i=1; i<vv-1; i++) {
			uv = i*uu;
			if(e1[uv]==0||e1[uv]==1) {									// <V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
			uv = (i+1)*uu-1;
			if(e1[uv]==0||e1[uv]==1) {									// <V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-uu].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
		}
		if(vb) {														// <V�̏ꍇ>
			uv = 0;
			if(e1[uv]==0||e1[uv]==1) {									// <V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-2)*uu].p[k]+t1[uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv-1)*uu].p[k] = pa[uv].p[k];
				}
			}
			uv = uu-1;
			if(e1[uv]==0||e1[uv]==1) {									// <V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-1)*uu-1].p[k]+t1[2*uu-1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv*uu)-1].p[k] = pa[uv].p[k];
				}
			}
		}
	}
	if(vb) {															// <<V���[�����v�Z>>
		for(i=1; i<uu-1; i++) {											// <V�̏ꍇ>
			uv = i;
			if(e1[uv]==0) {												// <U&V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-2)*uu+uv-1].p[k]+t1[(vv-2)*uu+uv+1].p[k]+
                         t1[uu+uv-1].p[k]+t1[uu+uv+1].p[k];
					w2 = t1[(vv-2)*uu+uv].p[k]+t1[uv-1].p[k]+
                         t1[uv+1].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                                   t1[uv].p[k])/((wt+4.0)*(wt+4.0));
					pa[(vv-1)*uu+uv].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==1) {										// <V�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[(vv-2)*uu+uv].p[k]+t1[uv+uu].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv-1)*uu+uv].p[k] = pa[uv].p[k];
				}
			} else if(e1[uv]==2) {										// <U�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[(vv-1)*uu+uv].p[k] = pa[uv].p[k];
				}
			}
		}
	} else {															// <V�J�̏ꍇ>
		for(i=1; i<uu-1; i++) {
			uv = i;
			if(e1[uv]==0||e1[uv]==2) {									// <U�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
			uv = (vv-1)*uu+i;
			if(e1[uv]==0||e1[uv]==2) {									// <U�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uv-1].p[k]+t1[uv+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
				}
			}
		}
		if(ub) {														// <U�̏ꍇ>
			uv = 0;
			if(e1[uv]==0||e1[uv]==2) {									// <U�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[uu-2].p[k]+t1[1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[uu-1].p[k] = pa[uv].p[k];
				}
			}
			uv = (vv-1)*uu;
			if(e1[uv]==0||e1[uv]==2) {									// <U�Ȑ��_>
				for(k=0; k<DIM; k++) {
					w1 = t1[vv*uu-2].p[k]+t1[(vv-1)*uu+1].p[k];
					pa[uv].p[k] = (w1+(wt+2.0)*t1[uv].p[k])/(wt+4.0);
					pa[vv*uu-1].p[k] = pa[uv].p[k];
				}
			}
		}
	}
	if(ub&&vb) {														// <U��V�̏ꍇ>
		if(e1[0]==0) {													// <U&V�Ȑ��_>
			for(k=0; k<DIM; k++) {
				w1 = t1[(vv-1)*uu-2].p[k]+t1[(vv-2)*uu+1].p[k]+
                     t1[2*uu-2].p[k]+t1[uu+1].p[k];
				w2 = t1[(vv-2)*uu].p[k]+t1[uu-2].p[k]+t1[1].p[k]+t1[uu].p[k];
				pa[0].p[k] = (w1+(wt+2.0)*w2+(wt+2.0)*(wt+2.0)*
                              t1[0].p[k])/((wt+4.0)*(wt+4.0));
				pa[uu-1].p[k] = pa[0].p[k];
				pa[(vv-1)*uu].p[k] = pa[0].p[k];
				pa[vv*uu-1].p[k] = pa[0].p[k];
			}
		} else if(e1[0]==1) {											// <V�Ȑ��_>
			for(k=0; k<DIM; k++) {
				w1 = t1[(vv-2)*uu].p[k]+t1[uu].p[k];
				pa[0].p[k] = (w1+(wt+2.0)*t1[0].p[k])/(wt+4.0);
				pa[uu-1].p[k] = pa[0].p[k];
				pa[(vv-1)*uu].p[k] = pa[0].p[k];
				pa[vv*uu-1].p[k] = pa[0].p[k];
			}
		} else if(e1[0]==2) {											// <U�Ȑ��_>
			for(k=0; k<DIM; k++) {
				w1 = t1[uu-2].p[k]+t1[1].p[k];
				pa[0].p[k] = (w1+(wt+2.0)*t1[0].p[k])/(wt+4.0);
				pa[uu-1].p[k] = pa[0].p[k];
				pa[(vv-1)*uu].p[k] = pa[0].p[k];
				pa[vv*uu-1].p[k] = pa[0].p[k];
			}
		}
	}
	*un = uu;
	*vn = vv;
	delete[] t1;
	delete[] e1;
}

/**************************/
OBJTYPE* MakeSurface(void)												//<<< �Ȗʐ��������
/**************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc, un, vn, uo, vo, ch, uu, vv, ub, vb, mx, wt;
	int      vnum=0, lnum=0, lnum2=0, sflg, uflg=0, vflg=0;
	double   dwt;

	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	un = GetParaInt(cmp, 0);											// U����_���擾
	vn = GetParaInt(cmp, 1);											// V����_���擾
	ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	mx = GetParaInt(cmp, 4);											// �Ȑ����J�Ԑ�
	if(GetCurveReptSW()) {												// <�Ȗʍו����I��>
		mx = GetCurveReptNum();											// �Ȗʕ������擾
	}
	wt = GetParaInt(cmp, 5);											// �Ȑ��d�݌W��
	sflg = GetParaInt(cmp, 6);											// �د��Ӱ��
	dwt = wt/1000.0;													// �d�݌W�������ϊ�
	uo = un, vo = vn;													// U/V����_���ۑ�
	for(j=0, pc=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			ea[pc] = GetParaInt(cmp, un*j+i+7);							// Ӱ�ގ擾
			if(ea[pc]>3) {												// Ӱ�ވُ�̏ꍇ
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U���[�͐ܐ�
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V���[�͐ܐ�
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V�J�̎l��
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+i+7, ea[pc]);						// Ӱ�ޕύX
			}
			GetParaPnt(cmp, un*j+i, &pa[pc++]);							// ����_�擾
		}
		if(ub) {														// <U�̏ꍇ>
			ea[pc] = GetParaInt(cmp, un*j+7);							// Ӱ�ގ擾
			if(ea[pc]>3) {												// Ӱ�ވُ�̏ꍇ
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U���[�͐ܐ�
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V���[�͐ܐ�
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V�J�̎l��
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+7, ea[pc]);						// Ӱ�ޕύX
			}
			GetParaPnt(cmp, un*j, &pa[pc++]);							// ����_�擾
		}
	}
	if(ub) {															// U�͐���_��UP
		un++;
	}
	if(vb) {															// <V�̏ꍇ>
		for(i=0; i<un; i++) {											// Ӱ��,����_�擾
			ea[pc] = ea[i], pa[pc++] = pa[i];
		}
		vn++;															// ����_��UP
	}
	if(un>1&&vn>1) {
		MakeSSurface(&un, &vn, ub, vb, mx, dwt, pa, ea);				// S�Ȗʐ���
		if(!vb) {
			uflg = UPointCheck(un, vn, pa);								// U���ꐧ��_�m�F
		}
		if(!ub) {
			vflg = VPointCheck(un, vn, pa);								// V���ꐧ��_�m�F
		}
		if(ub&&(!vb)) {													// <U��&V�J�̏ꍇ>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U�f�ʒ��_�ۑ�
		} else if((!ub)&&vb) {											// <U�J&V�̏ꍇ>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V�f�ʒ��_�ۑ�
		}
		uu = (ub) ? un-1 : un;											// U�͒��_����
		vv = (vb) ? vn-1 : vn;											// V�͒��_����
		vnum  = uu*vv;													// ���_���擾
		lnum  = (un-1)*(vn-1)*4;										// ��ٰ��1���擾
		lnum2 = un*vo+vn*uo;											// ��ٰ��2���擾
		if(uflg==1||uflg==2) {											// <<U��[����_����>>
			vnum -= uu-1, lnum -= un-1, lnum2 -= un;
			if(ub&&sflg) {												// <�f�ʗL�̏ꍇ>
				if(uflg==1) {
					i = CheckSectionPoint(un-1, pe);					// �I���f�ʒ��_�����m�F
				} else {
					i = CheckSectionPoint(un-1, ps);					// �J�n�f�ʒ��_�����m�F
				}
				lnum += 2*i+un-1;
			}
		} else if(uflg==3) {											// <<U���[����_����>>
			vnum -= 2*(uu-1), lnum -= 2*(un-1), lnum2 -= 2*un;
		} else if(vflg==1||vflg==2) {									// <<V��[����_����>>
			vnum -= vv-1, lnum -= vn-1, lnum2 -= vn;
			if(vb&&sflg) {												// <�f�ʗL�̏ꍇ>
				if(vflg==1) {
					i = CheckSectionPoint(vn-1, pe);					// �I���f�ʒ��_�����m�F
				} else {
					i = CheckSectionPoint(vn-1, ps);					// �J�n�f�ʒ��_�����m�F
				}
				lnum += 2*i+vn-1;
			}
		} else if(vflg==3) {											// <<V���[����_����>>
			vnum -= 2*(vv-1), lnum -= 2*(vn-1), lnum2 -= 2*vn;
		} else {														// <<����_������>>
			if(ub&&(!vb)&&sflg) {										// <U�f�ʗL�̏ꍇ>
				i = CheckSectionPoint(un-1, ps);						// �J�n�f�ʒ��_�����m�F
				j = CheckSectionPoint(un-1, pe);						// �I���f�ʒ��_�����m�F
				lnum += 2*(un-1)+2*(i+j);
			} else if((!ub)&&vb&&sflg) {								// <V�f�ʗL�̏ꍇ>
				i = CheckSectionPoint(vn-1, ps);						// �J�n�f�ʒ��_�����m�F
				j = CheckSectionPoint(vn-1, pe);						// �I���f�ʒ��_�����m�F
				lnum += 2*(vn-1)+2*(i+j);
			}
		}
	}
	CreatObj(vnum, lnum, lnum2, &op);									// ���̗̈�m��
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	LevelObj(0, op);													// �������ِ������̐ڑ�
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<����ގ��s��~�Ȃ�>>>
		lnum = 0;
		if(uflg==1||uflg==2||uflg==3) {									// <<U�[����_����>>
			SetVtxU(op, uflg, pa, ub, un, vn);							// ���_���W�o�^(U����)
			SetFacU(&lnum, op, uflg, ub, un, vn);						// ��ٰ��1�o�^(U����)
			if(ub&&sflg&&uflg!=3) {										// <�د�ނň�[�s����>
				SetFacUS(&lnum, op, uflg, un, vn, ps, pe);				// ��[�f�ʓo�^
			}
			SetFacU2(op, uflg, ub, un, vn, uo, vo, mx);					// ��ٰ��2�o�^(U����)
		} else if(vflg==1||vflg==2||vflg==3) {							// <<V�[����_����>>
			SetVtxV(op, vflg, pa, vb, un, vn);							// ���_���W�o�^(V����)
			SetFacV(&lnum, op, vflg, vb, un, vn);						// ��ٰ��1�o�^(V����)
			if(vb&&sflg&&vflg!=3) {										// <�د�ނň�[�s����>
				SetFacVS(&lnum, op, vflg, un, vn, ps, pe);				// ��[�f�ʓo�^
			}
			SetFacV2(op, vflg, vb, un, vn, uo, vo, mx);					// ��ٰ��2�o�^(V����)
		} else {														// <<����_������>>
			SetVtx(op, pa, ub, vb, un, vn);								// ���_���W�o�^
			SetFac(&lnum, op, ub, vb, un, vn);							// ��ٰ��1�o�^
			if(sflg) {													// <�د��>
				SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);				// �f�ʓo�^
			}
			SetFac2(op, ub, vb, un, vn, uo, vo, mx);					// ��ٰ��2�o�^
		}
	}
	if((ub&&vb)||(vb&&uflg==3)||(ub&&vflg==3)||(sflg&&(ub||vb))) {		// ����No�ݒ�(SURFACE)
		PutObjAtr(op, 20);
	} else {
		PutObjAtr(op, 21);
	}
	PutObjPrt(op, 0, 0);												// �߰�No�ݒ�(�ʏ�)
	PutObjCol(op, 0);													// �װNo�ݒ�(�ް���)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <����ޒ�~��&RedoCommand�ȊO>
		SetObjBox(op);													// �ޯ�����ސݒ�
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}

/****************************/
OBJTYPE* ChangeSurface(void)											//<<< �ȖʕҏW�����
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc, un, vn, uo, vo, ch, uu, vv, ub, vb, mx, wt;
	int      vnum=0, lnum=0, lnum2=0, sflg, uflg=0, vflg=0;
	double   dwt;

	GetObjPtr(&op);														// �����Ώۗ��̎擾
	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	un = GetParaInt(cmp, 0);											// U����_���擾
	vn = GetParaInt(cmp, 1);											// V����_���擾
	ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	mx = GetParaInt(cmp, 4);										// �Ȑ����J�Ԑ�
	if(GetCurveReptSW()) {												// <�Ȗʍו����I��>
		mx = GetCurveReptNum();											// �Ȗʕ������擾
	}
	wt = GetParaInt(cmp, 5);											// �Ȑ��d�݌W��
	sflg = GetParaInt(cmp, 6);											// �د��Ӱ��
	dwt = wt/1000.0;													// �d�݌W�������ϊ�
	uo = un, vo = vn;													// U/V����_���ۑ�
	for(j=0, pc=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			ea[pc] = GetParaInt(cmp, un*j+i+7);							// Ӱ�ގ擾
			if(ea[pc]>3) {												// Ӱ�ވُ�̏ꍇ
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U���[�͐ܐ�
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V���[�͐ܐ�
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V�J�̎l��
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+i+7, ea[pc]);						// Ӱ�ޕύX
			}
			GetParaPnt(cmp, un*j+i, &pa[pc++]);							// ����_�擾
		}
		if(ub) {														// <U�̏ꍇ>
			ea[pc] = GetParaInt(cmp, un*j+7);							// Ӱ�ގ擾
			if(ea[pc]>3) {												// Ӱ�ވُ�̏ꍇ
				ea[pc] = 0, ch = 0;
				if((!ub)&&(pc%un==0||pc%un==un-1)) {					// U���[�͐ܐ�
					ea[pc] = 1; ch++;
				}
				if((!vb)&&(pc<un||pc>=un*(vn-1))) {						// V���[�͐ܐ�
					ea[pc] = 2; ch++;
				}
				if(ch==2) {												// U/V�J�̎l��
					ea[pc] = 3;
				}
				PutParaInt(cmp, un*j+7, ea[pc]);						// Ӱ�ޕύX
			}
			GetParaPnt(cmp, un*j, &pa[pc++]);							// ����_�擾
		}
	}
	if(ub) {															// U�͐���_��UP
		un++;
	}
	if(vb) {															// <V�̏ꍇ>
		for(i=0; i<un; i++) {											// Ӱ��,����_�擾
			ea[pc] = ea[i], pa[pc++] = pa[i];
		}
		vn++;															// ����_��UP
	}
	if(un>1&&vn>1) {
		MakeSSurface(&un, &vn, ub, vb, mx, dwt, pa, ea);				// S�Ȗʐ���
		if(!vb) {
			uflg = UPointCheck(un, vn, pa);								// U���ꐧ��_�m�F
		}
		if(!ub) {
			vflg = VPointCheck(un, vn, pa);								// V���ꐧ��_�m�F
		}
		if(ub&&(!vb)) {													// <U��&V�J�̏ꍇ>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U�f�ʒ��_�ۑ�
		} else if((!ub)&&vb) {											// <U�J&V�̏ꍇ>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V�f�ʒ��_�ۑ�
		}
		uu = (ub) ? un-1 : un;											// U�͒��_����
		vv = (vb) ? vn-1 : vn;											// V�͒��_����
		vnum  = uu*vv;													// ���_���擾
		lnum  = (un-1)*(vn-1)*4;										// ��ٰ��1���擾
		lnum2 = un*vo+vn*uo;											// ��ٰ��2���擾
		if(uflg==1||uflg==2) {											// <<U��[����_����>>
			vnum -= uu-1, lnum -= un-1, lnum2 -= un;
			if(ub&&sflg) {												// <�f�ʗL�̏ꍇ>
				if(uflg==1) {
					i = CheckSectionPoint(un-1, pe);					// �I���f�ʒ��_�����m�F
				} else {
					i = CheckSectionPoint(un-1, ps);					// �J�n�f�ʒ��_�����m�F
				}
				lnum += 2*i+un-1;
			}
		} else if(uflg==3) {											// <<U���[����_����>>
			vnum -= 2*(uu-1), lnum -= 2*(un-1), lnum2 -= 2*un;
		} else if(vflg==1||vflg==2) {									// <<V��[����_����>>
			vnum -= vv-1, lnum -= vn-1, lnum2 -= vn;
			if(vb&&sflg) {												// <�f�ʗL�̏ꍇ>
				if(vflg==1) {
					i = CheckSectionPoint(vn-1, pe);					// �I���f�ʒ��_�����m�F
				} else {
					i = CheckSectionPoint(vn-1, ps);					// �J�n�f�ʒ��_�����m�F
				}
				lnum += 2*i+vn-1;
			}
		} else if(vflg==3) {											// <<V���[����_����>>
			vnum -= 2*(vv-1), lnum -= 2*(vn-1), lnum2 -= 2*vn;
		} else {														// <<����_������>>
			if(ub&&(!vb)&&sflg) {										// <U�f�ʗL�̏ꍇ>
				i = CheckSectionPoint(un-1, ps);						// �J�n�f�ʒ��_�����m�F
				j = CheckSectionPoint(un-1, pe);						// �I���f�ʒ��_�����m�F
				lnum += 2*(un-1)+2*(i+j);
			} else if((!ub)&&vb&&sflg) {								// <V�f�ʗL�̏ꍇ>
				i = CheckSectionPoint(vn-1, ps);						// �J�n�f�ʒ��_�����m�F
				j = CheckSectionPoint(vn-1, pe);						// �I���f�ʒ��_�����m�F
				lnum += 2*(vn-1)+2*(i+j);
			}
		}
	}
	AppendCmd(op, cmp);													// ����&����ސڑ�
	ChangeObj(op, vnum, lnum, lnum2);									// ���̗̈�ύX
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<����ގ��s��~�Ȃ�>>>
		lnum = 0;
		if(uflg==1||uflg==2||uflg==3) {									// <<U�[����_����>>
			SetVtxU(op, uflg, pa, ub, un, vn);							// ���_���W�o�^(U����)
			SetFacU(&lnum, op, uflg, ub, un, vn);						// ��ٰ��1�o�^(U����)
			if(ub&&sflg&&uflg!=3) {										// <�د�ނň�[�s����>
				SetFacUS(&lnum, op, uflg, un, vn, ps, pe);				// ��[�f�ʓo�^
			}
			SetFacU2(op, uflg, ub, un, vn, uo, vo, mx);					// ��ٰ��2�o�^(U����)
		} else if(vflg==1||vflg==2||vflg==3) {							// <<V�[����_����>>
			SetVtxV(op, vflg, pa, vb, un, vn);							// ���_���W�o�^(V����)
			SetFacV(&lnum, op, vflg, vb, un, vn);						// ��ٰ��1�o�^(V����)
			if(vb&&sflg&&vflg!=3) {										// <�د�ނň�[�s����>
				SetFacVS(&lnum, op, vflg, un, vn, ps, pe);				// ��[�f�ʓo�^
			}
			SetFacV2(op, vflg, vb, un, vn, uo, vo, mx);					// ��ٰ��2�o�^(V����)
		} else {														// <<����_������>>
			SetVtx(op, pa, ub, vb, un, vn);								// ���_���W�o�^
			SetFac(&lnum, op, ub, vb, un, vn);							// ��ٰ��1�o�^
			if(sflg) {													// <�د��>
				SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);				// �f�ʓo�^
			}
			SetFac2(op, ub, vb, un, vn, uo, vo, mx);					// ��ٰ��2�o�^
		}
	}
	if((ub&&vb)||(vb&&uflg==3)||(ub&&vflg==3)||(sflg&&(ub||vb))) {		// ����No�ݒ�(SURFACE)
		PutObjAtr(op, 20);
	} else {
		PutObjAtr(op, 21);
	}
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <����ޒ�~��&RedoCommand�ȊO>
		SetObjBox(op);													// �ޯ�����ސݒ�
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}

/*************************/
OBJTYPE* SetSurface(void)												//<<< �Ȗʐ���_���������
/*************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, un, vn, ub, vb, vnum=0, lnum=0, vno=0, lno=0;

	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	un = GetParaInt(cmp, 0);											// U����_���擾
	vn = GetParaInt(cmp, 1);											// V����_���擾
	ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	for(j=0; j<vn; j++) {
		for(i=0; i<un; i++) {
			GetParaPnt(cmp, un*j+i, &pa[un*j+i]);						// ����_�擾
		}
	}
	if(un>1&&vn>1) {
		vnum = un*vn;													// ���_���擾
		if(ub&&vb) {													// ��ٰ�ߐ��擾
			lnum = 4*un*vn;
		} else if(ub&&(!vb)) {
			lnum = 4*un*(vn-1);
		} else if((!ub)&&vb) {
			lnum = 4*(un-1)*vn;
		} else {
			lnum = 4*(un-1)*(vn-1);
		}
	}
	CreatObj(vnum, lnum, 0, &op);										// ���̗̈�m��
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	LevelObj(0, op);													// �������ِ������̐ڑ�
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<����ގ��s��~�Ȃ�>>>
		for(j=0; j<vn; j++) {
			for(i=0; i<un; i++) {
				PutVtx(op, vno++, pa[un*j+i]);							// ���_���W�ݒ�
			}
		}
		if(ub&&vb) {													// <U��&V�̏ꍇ>
			for(j=0; j<vn; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// ��ٰ�ߐݒ�
					if(i==un-1) {
						PutFlp1(op, lno++, un*j, 0);
					} else {
						PutFlp1(op, lno++, un*j+i+1, 0);
					}
					if(j==vn-1) {
						if(i==un-1) {
							PutFlp1(op, lno++, 0, 0);
						} else {
							PutFlp1(op, lno++, i+1, 0);
						}
						PutFlp1(op, lno++, i, 1);
					} else {
						if(i==un-1) {
							PutFlp1(op, lno++, un*(j+1), 0);
						} else {
							PutFlp1(op, lno++, un*(j+1)+i+1, 0);
						}
						PutFlp1(op, lno++, un*(j+1)+i, 1);
					}
				}
			}
		} else if(ub&&(!vb)) {											// <U��&V�J�̏ꍇ>
			for(j=0; j<vn-1; j++) {
				for(i=0; i<un; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// ��ٰ�ߐݒ�
					if(i==un-1) {
						PutFlp1(op, lno++, un*j, 0);
						PutFlp1(op, lno++, un*(j+1), 0);
					} else {
						PutFlp1(op, lno++, un*j+i+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					}
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		} else if((!ub)&&vb) {											// <U�J&V�̏ꍇ>
			for(j=0; j<vn; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// ��ٰ�ߐݒ�
					PutFlp1(op, lno++, un*j+i+1, 0);
					if(j==vn-1) {
						PutFlp1(op, lno++, i+1, 0);
						PutFlp1(op, lno++, i, 1);
					} else {
						PutFlp1(op, lno++, un*(j+1)+i+1, 0);
						PutFlp1(op, lno++, un*(j+1)+i, 1);
					}
				}
			}
		} else {														// <U�J&V�J�̏ꍇ>
			for(j=0; j<vn-1; j++) {
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno++, un*j+i, 0);						// ��ٰ�ߐݒ�
					PutFlp1(op, lno++, un*j+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i+1, 0);
					PutFlp1(op, lno++, un*(j+1)+i, 1);
				}
			}
		}
	}
	if(ub||vb) {
		PutObjAtr(op, 20);												// ����No�ݒ�(SURFACE)
	} else {
		PutObjAtr(op, 21);
	}
	PutObjPrt(op, 0, 0);												// �߰�No�ݒ�(�ʏ�)
	PutObjCol(op, 0);													// �װNo�ݒ�(�ް���)
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
		OnEdtFlg(1, op);												// edtflg:ON
	}
	delete[] pa;
	return op;
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
	sc = fabs(1.0/cos(th/2));											// �f�ʊg��{��
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

/********************************************************************/
static void MakeSSweepSurf(int pl, int* un, int* vn, int ub, int vb,
                           int mx, double wt, PNTTYPE* pa, int* ea)		//<<< S�|���Ȗʐ���
/********************************************************************/
{
	PNTTYPE* up = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* vp = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* wup = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* wvp = new PNTTYPE[View->m_MaxCPnt];
	int*     ue = new int[View->m_MaxCPnt];
	int*     ve = new int[View->m_MaxCPnt];
	PNTTYPE  wp[3], pt;
	int      i, j, k, d1, d2, d3, uu, vv, uc, vc;
	VECTYPE  vec, rvc;
	double   rota[3][3], rotb[3][3], th;

	uu = *un, vv = *vn;													// U/V����_��
	for(uc=0, i=0; i<uu; i++) {
		up[i] = pa[i], ue[i] = ea[i];									// U����_���o
		if(ue[i]!=0) {													// U�ܐ��_����
			uc++;
		}
	}
	wp[0] = up[0], wp[1] = up[1];										// U����_1,2�_�ڕۑ�
	for(vc=0, i=0; i<vv; i++) {
		vp[i] = pa[i+uu], ve[i] = ea[i+uu];								// V����_���o
		if(ve[i]!=0) {													// V�ܐ��_����
			vc++;
		}
	}
	for(i=0; i<mx; i++) {												// ���������J�Ԃ�
		SCurves(1, &uu, ub, wt, up, ue);								// U����S�Ȑ�����
		SCurves(1, &vv, vb, wt, vp, ve);								// V����S�Ȑ�����
	}
	for(i=0; i<uu; i++) {												// U��Ɨ̈敡��
		wup[i] = up[i];
	}
	for(i=1; i<uu-1; i++) {
		if(ue[i]!=1) {
			for(j=0; j<DIM; j++) {										// U�Ȑ��_����
				up[i].p[j] = (wup[i-1].p[j]+wup[i+1].p[j]+
                              (wt+2.0)*wup[i].p[j])/(wt+4.0);
			}
		}
	}
	if(ub&&uc==0) {														// <U���E:��&�S�Ȑ��̏ꍇ>
		for(j=0; j<DIM; j++) {											// 2�_��&�ŏI-1�_�ڎ���
			up[0].p[j] = (wup[uu-2].p[j]+wup[1].p[j]+
                          (wt+2.0)*wup[0].p[j])/(wt+4.0);
		}
		up[uu-1] = up[0];												// �n�_�ƏI�_�͓���
	}
	for(i=0; i<vv; i++) {
		wvp[i] = vp[i];													// V��Ɨ̈敡��
	}
	for(i=1; i<vv-1; i++) {
		if(ve[i]!=1) {
			for(j=0; j<DIM; j++) {										// V�Ȑ��_����
				vp[i].p[j] = (wvp[i-1].p[j]+wvp[i+1].p[j]+
                              (wt+2.0)*wvp[i].p[j])/(wt+4.0);
			}
		}
	}
	if(vb&&vc==0) {														// <V���E:��&�S�Ȑ��̏ꍇ>
		for(j=0; j<DIM; j++) {											// 2�_��&�ŏI-1�_�ڎ���
			vp[0].p[j] = (wvp[vv-2].p[j]+wvp[1].p[j]+
                          (wt+2.0)*wvp[0].p[j])/(wt+4.0);
		}
		vp[vv-1] = vp[0];												// �n�_�ƏI�_�͓���
	}
	if(pl==-1) {														// <���ʎw�薳�ɏꍇ(�V��)>
		for(i=0; i<DIM; i++) {
			vec.p[i] = wp[1].p[i]-wp[0].p[i];							// �ް�������_�޸�َ擾
		}
		VecN(&vec);														// �P���޸�ى�
		d1 = 0, d2 = 1, d3 = 2;											// ���W���ݒ�
		rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = 1.0;					// ����p�x����]
		th = atan2(vec.p[1], vec.p[0]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[2])>EPSILON) {									// <����ʏ�łȂ��ꍇ>
			for(i=0; i<vv; i++) {
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
		for(i=0; i<DIM; i++) {
			vec.p[i] = up[1].p[i]-up[0].p[i];							// �ް������_�޸�َ擾
		}
		VecN(&vec);														// �P���޸�ى�
		d1 = pl;														// ���W���ݒ�
		d2 = (d1+1>2) ? 0 : d1+1;
		d3 = (d2+1>2) ? 0 : d2+1;
		rvc.p[d1] = 0.0, rvc.p[d2] = 0.0, rvc.p[d3] = -1.0;				// ���(d1)�p�x����]
		th = atan2(vec.p[d1], vec.p[d2]);
		th = (th<-10||th>10) ? 0.0 : th;
		SetRotateMatrix(rvc, th, rota);
		if(fabs(vec.p[d3])>EPSILON) {									// <����ʏ�łȂ��ꍇ>
			for(i=0; i<vv; i++) {
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
	for(i=0; i<vv; i++) {												// <<�ް����n�_����>>
		pt = vp[i];
		VecRotateMove(&pt, rota, up[0]);								// �ް����n�_�����ɉ�]�ړ�
		pa[i*uu] = pt;
		VecRotate(&vp[i], rota);										// �n�_�����ɉ�]
	}
	for(i=1; i<uu-1; i++) {												// <<�ް������ԓ_����>>
		CalcRotAngleVec(&up[i-1], &th, &vec);							// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		SetRotateMatrix(vec, th, rotb);									// ����_�p��د��
		if(!ue[i]) {													// <�|���������Ȑ��_>
			for(j=0; j<vv; j++) {
				pt = vp[j];
				VecRotateMove(&pt, rota, up[i]);						// �O��_���Ԋp�x�ɉ�]�ړ�
				pa[j*uu+i] = pt;
				VecRotate(&vp[j], rotb);								// ���_�����ɉ�]
			}
		} else {														// <�|���������ܐ��_>
			for(j=0; j<vv; j++) {
				wvp[j] = vp[j];
				VecRotate(&wvp[j], rota);								// �O��_���Ԋp�x�ɉ�]
				VecRotate(&vp[j], rotb);								// ���_�����ɉ�]
			}
			ScaleSection(wvp, vv, vec, th, d1, d2, d3);					// �f�ʊg��
			for(j=0; j<vv; j++) {										// �|���_�Ɉړ�
				for(k=0; k<DIM; k++) {
					pa[j*uu+i].p[k] = wvp[j].p[k]+up[i].p[k];
				}
			}
		}
	}
	if(!ub) {															// <<<���E:�J�̏ꍇ>>>
		for(i=0; i<vv; i++) {											// <<�ް����I�_����>>
			pt = vp[i];
			for(j=0; j<DIM; j++) {
				pt.p[j] += up[uu-1].p[j];								// �I�_�ړ�
			}
			pa[i*uu+uu-1] = pt;
		}
	} else {															// <<<���E:�̏ꍇ>>>
		wp[0] = up[uu-2], wp[1] = up[0], wp[2] = up[1];					// �I�_�Ǝn�_��A��
		CalcRotAngleVec(wp, &th, &vec);									// �O��_�̊p�x�擾
		SetRotateMatrix(vec, th/2.0, rota);								// ����_�p��د��
		if(!ue[0]) {													// <�|���������Ȑ��_>
			for(i=0; i<vv; i++) {
				pt = vp[i];
				VecRotateMove(&pt, rota, up[0]);
				pa[i*uu] = pt;											// �J�n/�I���f��
				pa[i*uu+uu-1] = pt;
			}
		} else {														// <�|���������ܐ��_>
			for(i=0; i<vv; i++) {
				VecRotate(&vp[i], rota);
			}
			ScaleSection(vp, vv, vec, th, d1, d2, d3);					// �f�ʊg��
			for(i=0; i<vv; i++) {										// �|���_�Ɉړ�
				for(j=0; j<DIM; j++) {									// �J�n/�I���f��
					pa[i*uu].p[j] = vp[i].p[j]+up[0].p[j];
					pa[i*uu+uu-1].p[j] = vp[i].p[j]+up[0].p[j];
				}
			}
		}
	}
	*un = uu, *vn = vv;
	delete[] up;
	delete[] vp;
	delete[] wup;
	delete[] wvp;
	delete[] ue;
	delete[] ve;
}

/****************************/
OBJTYPE* MakeSweepSurf(void)											//<<< �|���Ȗʐ��������
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc=0, un, vn, uo, vo, uu, vv;
	int      pl, ub, vb, mx, wt, vnum=0, lnum=0, lnum2=0, sflg;
	double   dwt;

	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	un = GetParaInt(cmp, 0);											// U����_���擾
	vn = GetParaInt(cmp, 1);											// V����_���擾
	ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	mx = GetParaInt(cmp, 4);											// �Ȑ����J�Ԑ�
	if(GetCurveReptSW()) {												// <�Ȗʍו����I��>
		mx = GetCurveReptNum();											// �Ȗʕ������擾
	}
	wt = GetParaInt(cmp, 5);											// �Ȑ����d�݌W��
	sflg = GetParaInt(cmp, 6);											// �د��Ӱ��
	pl = GetParaInt(cmp, 7);											// U���ʎ擾
	dwt = wt/1000.0;													// �d�݌W�������ϊ�
	uo = un, vo = vn;													// U/V����_���ۑ�
	for(i=0; i<un; i++) {												// <U����>
		ea[pc] = GetParaInt(cmp, i+8);									// Ӱ�ގ擾
		GetParaPnt(cmp, i, &pa[pc++]);									// ����_�擾
	}
	if(ub) {															// <U�̏ꍇ>
		ea[pc] = GetParaInt(cmp, 8);									// Ӱ�ގ擾
		GetParaPnt(cmp, 0, &pa[pc++]);									// ����_�擾
		un++;															// ����_��UP
	}
	for(i=0; i<vn; i++) {
		ea[pc] = GetParaInt(cmp, i+uo+8);								// Ӱ�ގ擾
		GetParaPnt(cmp, i+uo, &pa[pc++]);								// ����_�擾
	}
	if(vb) {															// <V�̏ꍇ>
		ea[pc] = GetParaInt(cmp, uo+8);									// Ӱ�ގ擾
		GetParaPnt(cmp, uo, &pa[pc++]);									// ����_�擾
		vn++;															// ����_��UP
	}
	if(un>1&&vn>1) {
		MakeSSweepSurf(pl, &un, &vn, ub, vb, mx, dwt, pa, ea);			// S�|���Ȗʐ���
		if(ub&&(!vb)) {													// <U��&V�J�̏ꍇ>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U�f�ʒ��_�ۑ�
		} else if((!ub)&&vb) {											// <U�J&V�̏ꍇ>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V�f�ʒ��_�ۑ�
		}
		uu = (ub) ? un-1 : un;											// U�͒��_����
		vv = (vb) ? vn-1 : vn;											// V�͒��_����
		vnum  = uu*vv;													// ���_���擾
		lnum  = (un-1)*(vn-1)*4;										// ��ٰ��1���擾
		lnum2 = un*vo+vn*uo;											// ��ٰ��2���擾
		if(ub&&(!vb)&&sflg) {											// <U�f�ʗL�̏ꍇ>
			i = CheckSectionPoint(un-1, ps);							// �J�n�f�ʒ��_�����m�F
			j = CheckSectionPoint(un-1, pe);							// �I���f�ʒ��_�����m�F
			lnum += 2*(un-1)+2*(i+j);
		} else if((!ub)&&vb&&sflg) {									// <V�f�ʗL�̏ꍇ>
			i = CheckSectionPoint(vn-1, ps);							// �J�n�f�ʒ��_�����m�F
			j = CheckSectionPoint(vn-1, pe);							// �I���f�ʒ��_�����m�F
			lnum += 2*(vn-1)+2*(i+j);
		}
	}
	CreatObj(vnum, lnum, lnum2, &op);									// ���̗̈�m��
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	LevelObj(0, op);													// �������ِ������̐ڑ�
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<����ގ��s��~�Ȃ�>>>
		lnum = 0;
		SetVtx(op, pa, ub, vb, un, vn);									// ���_���W�o�^
		SetFac(&lnum, op, ub, vb, un, vn);								// ��ٰ��1�o�^
		if(sflg) {
			SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);					// �f�ʓo�^
		}
		SetFac2(op, ub, vb, un, vn, uo, vo, mx);						// ��ٰ��2�o�^
	}
	if((ub&&vb)||(sflg&&(ub||vb))) {									// ����No�ݒ�(SWEEP)
		PutObjAtr(op, 22);
	} else {
		PutObjAtr(op, 23);
	}
	PutObjPrt(op, 0, 0);												// �߰�No�ݒ�(�ʏ�)
	PutObjCol(op, 0);													// �װNo�ݒ�(�ް���)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <����ޒ�~��&RedoCommand�ȊO>
		SetObjBox(op);													// �ޯ�����ސݒ�
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}

/******************************/
OBJTYPE* ChangeSweepSurf(void)											//<<< �|���ȖʕҏW�����
/******************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCPnt];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCPnt];
	int*     ea = new int[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, j, pc=0, un, vn, uo, vo, uu, vv, pl;
	int      ub, vb, mx, wt, vnum=0, lnum=0, lnum2=0, sflg;
	double   dwt;

	GetObjPtr(&op);														// �����Ώۗ��̎擾
	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	un = GetParaInt(cmp, 0);											// U����_���擾
	vn = GetParaInt(cmp, 1);											// V����_���擾
	ub = GetParaInt(cmp, 2);											// U���EӰ�ގ擾
	vb = GetParaInt(cmp, 3);											// V���EӰ�ގ擾
	mx = GetParaInt(cmp, 4);											// �Ȑ����J�Ԑ�
	if(GetCurveReptSW()) {												// <�Ȗʍו����I��>
		mx = GetCurveReptNum();											// �Ȗʕ������擾
	}
	wt = GetParaInt(cmp, 5);											// �Ȑ����d�݌W��
	sflg = GetParaInt(cmp, 6);											// �د��Ӱ��
	pl = GetParaInt(cmp, 7);											// U���ʎ擾
	dwt = wt/1000.0;													// �d�݌W�������ϊ�
	uo = un, vo = vn;													// U/V����_���ۑ�
	for(i=0; i<un; i++) {												// <U����>
		ea[pc] = GetParaInt(cmp, i+8);									// Ӱ�ގ擾
		GetParaPnt(cmp, i, &pa[pc++]);									// ����_�擾
	}
	if(ub) {															// <U�̏ꍇ>
		ea[pc] = GetParaInt(cmp, 8);									// Ӱ�ގ擾
		GetParaPnt(cmp, 0, &pa[pc++]);									// ����_�擾
		un++;															// ����_��UP
	}
	for(i=0; i<vn; i++) {
		ea[pc] = GetParaInt(cmp, i+uo+8);								// Ӱ�ގ擾
		GetParaPnt(cmp, i+uo, &pa[pc++]);								// ����_�擾
	}
	if(vb) {															// <V�̏ꍇ>
		ea[pc] = GetParaInt(cmp, uo+8);									// Ӱ�ގ擾
		GetParaPnt(cmp, uo, &pa[pc++]);									// ����_�擾
		vn++;															// ����_��UP
	}
	if(un>1&&vn>1) {
		MakeSSweepSurf(pl, &un, &vn, ub, vb, mx, dwt, pa, ea);			// S�|���Ȗʐ���
		if(ub&&(!vb)) {													// <U��&V�J�̏ꍇ>
			SetSectionPoint(0, un, vn, pa, ps, pe);						// U�f�ʒ��_�ۑ�
		} else if((!ub)&&vb) {											// <U�J&V�̏ꍇ>
			SetSectionPoint(1, un, vn, pa, ps, pe);						// V�f�ʒ��_�ۑ�
		}
		uu = (ub) ? un-1 : un;											// U�͒��_����
		vv = (vb) ? vn-1 : vn;											// V�͒��_����
		vnum  = uu*vv;													// ���_���擾
		lnum  = (un-1)*(vn-1)*4;										// ��ٰ��1���擾
		lnum2 = un*vo+vn*uo;											// ��ٰ��2���擾
		if(ub&&(!vb)&&sflg) {											// <U�f�ʗL�̏ꍇ>
			i = CheckSectionPoint(un-1, ps);							// �J�n�f�ʒ��_�����m�F
			j = CheckSectionPoint(un-1, pe);							// �I���f�ʒ��_�����m�F
			lnum += 2*(un-1)+2*(i+j);
		} else if((!ub)&&vb&&sflg) {									// <V�f�ʗL�̏ꍇ>
			i = CheckSectionPoint(vn-1, ps);							// �J�n�f�ʒ��_�����m�F
			j = CheckSectionPoint(vn-1, pe);							// �I���f�ʒ��_�����m�F
			lnum += 2*(vn-1)+2*(i+j);
		}
	}
	AppendCmd(op, cmp);													// ����&����ސڑ�
	ChangeObj(op, vnum, lnum, lnum2);									// ���̗̈�ύX
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()&&un>1&&vn>1) {									// <<<����ގ��s��~�Ȃ�>>>
		lnum = 0;
		SetVtx(op, pa, ub, vb, un, vn);									// ���_���W�o�^
		SetFac(&lnum, op, ub, vb, un, vn);								// ��ٰ��1�o�^
		if(sflg) {
			SetFacS(&lnum, op, ub, vb, un, vn, ps, pe);					// �f�ʓo�^
		}
		SetFac2(op, ub, vb, un, vn, uo, vo, mx);						// ��ٰ��2�o�^
	}
	if((ub&&vb)||(sflg&&(ub||vb))) {									// ����No�ݒ�(SWEEP)
		PutObjAtr(op, 22);
	} else {
		PutObjAtr(op, 23);
	}
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <����ޒ�~��&RedoCommand�ȊO>
		SetObjBox(op);													// �ޯ�����ސݒ�
	}
	delete[] pa;
	delete[] ps;
	delete[] pe;
	delete[] ea;
	return op;
}
