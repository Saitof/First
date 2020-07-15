/////////////////////////////////////////////////////////////////////////////
// ���̐��� �Ȗʌ��݂Â��R�}���h��
// CCmdThick.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/11/03 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"

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

/*********************************************************/
static BOOL GetThickFVec(OBJTYPE* op, double ln,
						 int vnum, int lnum, PNTTYPE* pa)				//<<< �ʖ@���޸�َ擾
/*********************************************************/
{
	VECTYPE* pv = new VECTYPE[View->m_MaxSPnt];
	VECTYPE* av = new VECTYPE[View->m_MaxSPnt];
	VECTYPE* wv = new VECTYPE[View->m_MaxCPnt];
	int*     fg = new int[View->m_MaxSPnt];
	int*     nx = new int[View->m_MaxSPnt];
	int*     wf = new int[View->m_MaxCPnt];
	PNTTYPE  pt[5], wpt;
	VECTYPE  v1, v2, v3, v4;
	int      pn[5], i, j, k, l, cnt, an, vno, flg, stp;
	double   th;

	for(i=0; i<vnum; i++) {
		fg[i] = -2;														// �׸ޏ�����
	}
	for(i=0, cnt=0, an=0, stp=0; i<lnum; i++) {
		GetFlp1(op, i, &vno, &flg);										// ��ٰ�ߔԍ��擾
		GetVtx(op, vno, &wpt);											// ���_���W�擾
		pt[cnt] = wpt;													// ���_���W�ۑ�
		pn[cnt++] = vno;												// ���_�ԍ��ۑ�
		if(flg==1) {													// <<<ٰ�ߍŏI�̏ꍇ>>>
			CalcNVec(pt, cnt, &v1);										// �@���޸�َ擾
			for(j=0; j<cnt; j++) {
				if(fg[pn[j]]==-2) {										// <<�����̏ꍇ>>
					pv[pn[j]] = v1;										// �Ώے��_�޸�ِݒ�
					fg[pn[j]] = -1;										// �׸ލX�V
				} else {												// <<�����ȊO�̏ꍇ>>
					v2 = pv[pn[j]];										// �o�^�ς��޸��
					VecN(&v2);											// �P���޸�ى�
					if(fabs(v1.p[0]-v2.p[0])>EPSILON||
						fabs(v1.p[1]-v2.p[1])>EPSILON||
						fabs(v1.p[2]-v2.p[2])>EPSILON) {				// 2�޸�يԊp�x
						th = acos(v1.p[0]*v2.p[0]+v1.p[1]*v2.p[1]+v1.p[2]*v2.p[2]);
						th = (th<-10||th>10) ? 0.0 : th;
					} else {
						th = 0.0;
					}
					if(fabs(th)<2*PI/9) {								// <40�x����(�Ȑ�)>
						for(k=0; k<DIM; k++) {
							pv[pn[j]].p[k] += v1.p[k];					// �Ώے��_�޸�ى��Z
						}
					} else {											// <45�x�ȏ�(�ܐ�)>
						if(fg[pn[j]]==-1) {								// <�ŏ��̐ܐ�>
							if(an>=View->m_MaxSPnt) {					// �ް��ْ��~
								stp = 1; break;
							}
							fg[pn[j]] = an;								// �ܐ��z��ԍ��ݒ�
							av[an] = v1;								// �Ώے��_�޸�ِݒ�
							nx[an++]  = -1;								// ���ް��ԍ�������
						} else {										// <2���ڈȍ~�̐ܐ�>
							if(an>=View->m_MaxSPnt) {					// �ް��ْ��~
								stp = 1; break;
							}
							k = fg[pn[j]];
							while(1) {
								if(nx[k]==-1) {							// �ŏI�ܐ��z��ԍ�����
									break;
								}
								k = nx[k];
							}
							nx[k] = an;									// �ܐ��z��ԍ��ݒ�
							av[an] = v1;								// �Ώے��_�޸�ِݒ�
							nx[an++] = -1;								// ���ް��ԍ�������
						}
					}
				}
			}
			if(stp) {													// �ް��ْ��~
				break;
			}
			cnt = 0;													// ���_����������
		}
	}
	if(stp) {															// <�ް��ق̏ꍇ>
		delete[] pv;
		delete[] av;
		delete[] wv;
		delete[] fg;
		delete[] nx;
		delete[] wf;
		return FALSE;
	} else {															// <�ް�OK�̏ꍇ>
		for(i=0; i<vnum; i++) {
			VecN(&pv[i]);												// �P���޸�ى�
			v1 = pv[i];
			if(fg[i]>=0) {												// <�ܐ��L�̏ꍇ>
				cnt = 0;
				j = fg[i];
				while(1) {
					wv[cnt] = av[j];									// �ܐ��޸�ٕ���
					wf[cnt++] = 0;										// �����׸ޏ�����
					if(nx[j]==-1) {
						break;
					}
					j = nx[j];
				}
				for(j=0; j<cnt; j++) {
					if(wf[j]==0) {										// <�������޸��>
						v2 = wv[j];
						for(k=j+1; k<cnt; k++) {
							v3 = v2;
							VecN(&v3);									// �P���޸�ى�
							v4 = wv[k];
							if(fabs(v3.p[0]-v4.p[0])>EPSILON||
                               fabs(v3.p[1]-v4.p[1])>EPSILON||
                               fabs(v3.p[2]-v4.p[2])>EPSILON) {			// 2�޸�يԊp�x
								th = acos(v3.p[0]*v4.p[0]+v3.p[1]*v4.p[1]+v3.p[2]*v4.p[2]);
								th = (th<-10||th>10) ? 0.0 : th;
							} else {
								th = 0.0;
							}
							if(fabs(th)<2*PI/9) {						// <40�x����(�Ȑ�)>
								for(l=0; l<DIM; l++) {
									v2.p[l] += v4.p[l];					// �Ώے��_�޸�ى��Z
								}
								wf[k] = 1;								// �����ς�
							}
						}
						VecN(&v2);										// �P���޸�ى�
						wv[j] = v2;										// ���e�͈͓��W�v��
					}
				}
				for(an=1, j=0; j<cnt; j++) {
					if(wf[j]==0) {
						for(k=0; k<DIM; k++) {
							v1.p[k] += wv[j].p[k];						// �W�v�ω��Z
						}
						an++;
					}
				}
				v2 = v1;
				for(j=0; j<DIM; j++) {
					v2.p[j] /= an;										// ���Z���ŏ��Z
				}
				th = VecL(&v2);											// �޸�ْ���
				th = (2.0/th)/2.0;										// ���߂��޸�ْ�
				VecN(&v1);												// �P���޸�ى�
				for(j=0; j<DIM; j++) {
					v1.p[j] *= th;										// ��������Z
				}
			}
			for(j=0; j<DIM; j++) {
				pa[i].p[j] += v1.p[j]*ln;								// �@���޸�ٕ��ړ�
			}
		}
		delete[] pv;
		delete[] av;
		delete[] wv;
		delete[] fg;
		delete[] nx;
		delete[] wf;
		return TRUE;
	}
}

/***********************************************************/
static void PutThickVtx(OBJTYPE* op, int vnum, PNTTYPE* pa)				//<<< ���_���W�ݒ�
/***********************************************************/
{
	int i;

	for(i=0; i<vnum; i++) {
		PutVtx(op, vnum+i, pa[i]);										// ���_���W�ݒ�
	}
}

/**************************************************************/
static void PutThickFlp(OBJTYPE* op, int vn, int ln1, int ln2)			//<<< ��ٰ�ߐݒ�
/**************************************************************/
{
	int i, vno, flg;

	for(i=0; i<ln2; i++) {
		GetFlp1(op, i+ln1, &vno, &flg);									// ��ٰ��2�擾
		PutFlp2(op, i, vno, flg);										// ��ٰ��2���
		PutFlp2(op, i+ln2, vno+vn, flg);								// ��ٰ��2�ݒ�
	}
	for(i=0; i<ln1; i++) {
		GetFlp1(op, i, &vno, &flg);										// ��ٰ��1�擾
		PutFlp1(op, i+ln1, vno+vn, flg);								// ��ٰ��1�ݒ�
	}
}

/*****************************************************************/
static void ReverseFlp(OBJTYPE* op, int ln, int lnum1, int lnum2)		//<<< ��ٰ�ߔ��]
/*****************************************************************/
{
	int i, j, st, ed, sp, bl, fl, vn1, vn2, fg1, fg2;

	if(ln>0) {															// ���]�͈͎w��
		st = 0, ed = lnum1;
	} else {
		st = lnum1, ed = 2*lnum1;
	}
	for(sp=st-1, i=st; i<ed; i++) {
		GetFlp1(op, i, &vn1, &fg1);										// ��ٰ��1���擾
		if(fg1) {														// <��ٰ�ߏI���̏ꍇ>
			bl = i;														// �Y��ٰ�ߍŌ��
			fl = (i-sp)/2;
			for(j=sp+1; j<sp+1+fl; j++) {								// �Y��ٰ�ߍőO����
				GetFlp1(op, j, &vn1, &fg1);								// �Ō���ƒ��_�ԍ�����
				GetFlp1(op, bl, &vn2, &fg2);
				PutFlp1(op, j, vn2, fg1);
				PutFlp1(op, bl, vn1, fg2);
				bl--;
			}
			sp = i;														// ��ٰ�ߊJ�n�ʒu(+1)
		}
	}
	if(ln>0) {															// ���]�͈͎w��
		st = 0, ed = lnum2;
	} else {
		st = lnum2, ed = 2*lnum2;
	}
	for(sp=st-1, i=st; i<ed; i++) {
		GetFlp2(op, i, &vn1, &fg1);										// ��ٰ��2���擾
		if(fg1) {														// <��ٰ�ߏI���̏ꍇ>
			bl = i;														// �Y��ٰ�ߍŌ��
			fl = (i-sp)/2;
			for(j=sp+1; j<sp+1+fl; j++) {								// �Y��ٰ�ߍőO����
				GetFlp2(op,  j, &vn1, &fg1);							// �Ō���ƒ��_�ԍ�����
				GetFlp2(op, bl, &vn2, &fg2);
				PutFlp2(op,  j, vn2, fg1);
				PutFlp2(op, bl, vn1, fg2);
				bl--;
			}
			sp = i;														// ��ٰ�ߊJ�n�ʒu(+1)
		}
	}
}

/****************************************************************/
static void PutThickFlpU(OBJTYPE* op, int uflg, int vflg,
						 int vnum, int* ln1, int* ln2, int ln,
						 int ub, int un, int vn, int uo, int mx)		//<<< U�f��ٰ�ߐݒ�
/****************************************************************/
{
	int i, lno1, lno2, ad1, ad2, div;

	lno1 = *ln1;														// ���݂̖�ٰ��1��
	lno2 = *ln2;														// ���݂̖�ٰ��2��
	if(ln>0) {
		ad1 = vnum, ad2 = 0;
	} else {
		ad1 = 0, ad2 = vnum;
	}
	for(i=0, div=1; i<mx; i++) {										// �����}�����v�Z
		div = div*2;
	}
	if(ub) {															// U�͎n�_/�I�_����
		un--;
	}
	if(uflg==0) {														// <<Uٰ�ߕs����>>
		if(ub) {														// <U��&V�J�̏ꍇ>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno1++, i+ad1, 0);							// ��ٰ��1�o�^
				PutFlp1(op, lno1++, i+ad2, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, i+ad2+1, 0);
					PutFlp1(op, lno1++, i+ad1+1, 1);
				}
				PutFlp1(op, lno1++, un*(vn-1)+i+ad2, 0);
				PutFlp1(op, lno1++, un*(vn-1)+i+ad1, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, un*(vn-1)+ad1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+ad2, 1);
				} else {
					PutFlp1(op, lno1++, un*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad2+1, 1);
				}
			}
		} else {														// <U�J&V�J�̏ꍇ>
			for(i=0; i<un-1; i++) {										// <���ʐ擪ٰ��>
				PutFlp1(op, lno1++, i+ad1, 0);
				PutFlp1(op, lno1++, i+ad2, 0);
				PutFlp1(op, lno1++, i+ad2+1, 0);
				PutFlp1(op, lno1++, i+ad1+1, 1);
			}
			if(vflg==0) {												// <Vٰ�ߕs����>
				for(i=0; i<un-1; i++) {
					PutFlp1(op, lno1++, un*(vn-1)+i+ad2, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, un*(vn-1)+i+ad2+1, 1);
				}
			} else if(vflg==1) {										// <�擪Vٰ�ߓ���>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, (un-1)*(vn-1)+ad1+1, 0);
				PutFlp1(op, lno1++, (un-1)*(vn-1)+ad2+1, 1);
				for(i=1; i<un-1; i++) {
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2+1, 1);
				}
			} else if(vflg==2) {										// <�ŏIVٰ�ߓ���>
				for(i=0; i<un-2; i++) {
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2+1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad1+2, 0);
					PutFlp1(op, lno1++, (un-1)*(vn-1)+i+ad2+2, 1);
				}
				PutFlp1(op, lno1++, (un-1)*vn+ad2, 0);
				PutFlp1(op, lno1++, (un-1)*vn+ad1, 0);
				PutFlp1(op, lno1++, un-1+ad1, 0);
				PutFlp1(op, lno1++, un-1+ad2, 1);
			} else {													// <�擪/�ŏI����>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, (un-2)*(vn-1)+ad1+2, 0);
				PutFlp1(op, lno1++, (un-2)*(vn-1)+ad2+2, 1);
				for(i=1; i<un-2; i++) {
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad2+1, 0);
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad1+1, 0);
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad1+2, 0);
					PutFlp1(op, lno1++, (un-2)*(vn-1)+i+ad2+2, 1);
				}
				PutFlp1(op, lno1++, (un-2)*vn+ad2+1, 0);
				PutFlp1(op, lno1++, (un-2)*vn+ad1+1, 0);
				PutFlp1(op, lno1++, un-1+ad1, 0);
				PutFlp1(op, lno1++, un-1+ad2, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno2++, i*div+ad1, 0);							// ��ٰ��2�o�^
			PutFlp2(op, lno2++, i*div+ad2, 1);
		}
		if(vflg==0) {													// <Vٰ�ߕs����>
			for(i=0; i<uo; i++) {
				PutFlp2(op, lno2++, un*(vn-1)+i*div+ad2, 0);
				PutFlp2(op, lno2++, un*(vn-1)+i*div+ad1, 1);
			}
		} else if(vflg==1) {											// <�擪Vٰ�ߓ���>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<uo; i++) {
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad2, 0);
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad1, 1);
			}
		} else if(vflg==2) {											// <�ŏIVٰ�ߓ���>
			for(i=0; i<uo-1; i++) {
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad2+1, 0);
				PutFlp2(op, lno2++, (un-1)*(vn-1)+i*div+ad1+1, 1);
			}
			PutFlp2(op, lno2++, un-1+ad2, 0);
			PutFlp2(op, lno2++, un-1+ad1, 1);
		} else {														// <�擪/�ŏI����>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<uo-1; i++) {
				PutFlp2(op, lno2++, (un-2)*(vn-1)+i*div+ad2+1, 0);
				PutFlp2(op, lno2++, (un-2)*(vn-1)+i*div+ad1+1, 1);
			}
			PutFlp2(op, lno2++, un-1+ad2, 0);
			PutFlp2(op, lno2++, un-1+ad1, 1);
		}
	} else if(uflg==1) {												// <<�擪Uٰ�ߓ���>>
		if(ub) {														// <U��&V�J�̏ꍇ>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno1++, un*(vn-2)+i+ad2+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad1+1, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, un*(vn-2)+ad1+1, 0);
					PutFlp1(op, lno1++, un*(vn-2)+ad2+1, 1);
				} else {
					PutFlp1(op, lno1++, un*(vn-2)+i+ad1+2, 0);
					PutFlp1(op, lno1++, un*(vn-2)+i+ad2+2, 1);
				}
			}
		} else {														// <U�J&V�J�̏ꍇ>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno1++, un*(vn-2)+i+ad2+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad1+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad1+2, 0);
				PutFlp1(op, lno1++, un*(vn-2)+i+ad2+2, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno2++, un*(vn-2)+i*div+ad2+1, 0);
			PutFlp2(op, lno2++, un*(vn-2)+i*div+ad1+1, 1);
		}
	} else if(uflg==2) {												// <<�ŏIUٰ�ߓ���>>
		if(ub) {														// <U��&V�J�̏ꍇ>
			for(i=0; i<un; i++) {
				PutFlp1(op, lno1++, i+ad1, 0);
				PutFlp1(op, lno1++, i+ad2, 0);
				if(i==un-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, i+ad2+1, 0);
					PutFlp1(op, lno1++, i+ad1+1, 1);
				}
			}
		} else {														// <U�J&V�J�̏ꍇ>
			for(i=0; i<un-1; i++) {
				PutFlp1(op, lno1++, i+ad1, 0);
				PutFlp1(op, lno1++, i+ad2, 0);
				PutFlp1(op, lno1++, i+ad2+1, 0);
				PutFlp1(op, lno1++, i+ad1+1, 1);
			}
		}
		for(i=0; i<uo; i++) {
			PutFlp2(op, lno2++, i*div+ad1, 0);
			PutFlp2(op, lno2++, i*div+ad2, 1);
		}
	}
	*ln1 = lno1;														// �X�V���ٰ��1��
	*ln2 = lno2;														// �X�V���ٰ��2��
}

/****************************************************************/
static void PutThickFlpV(OBJTYPE* op, int uflg, int vflg,
						 int vnum, int* ln1, int* ln2, int ln,
						 int vb, int un, int vn, int vo, int mx)		//<<< V�f��ٰ�ߐݒ�
/****************************************************************/
{
	int i, lno1, lno2, ad1, ad2, div, ms1, ms2;

	lno1 = *ln1;														// ���ݖ�ٰ��1��
	lno2 = *ln2;														// ���ݖ�ٰ��2��
	if(ln<0) {
		ad1 = vnum, ad2 = 0;
	} else {
		ad1 = 0, ad2 = vnum;
	}
	for(i=0, div=1; i<mx; i++) {										// �����}�����v�Z
		div = div*2;
	}
	if(vb) {															// V�͎n�_/�I�_����
		vn--;
	}
	if(vflg==0) {														// <<Vٰ�ߕs����>>
		if(vb) {														// <U�J&V�̏ꍇ>
			for(i=0; i<vn; i++) {
				PutFlp1(op, lno1++, un*i+ad1, 0);						// ��ٰ��1�o�^
				PutFlp1(op, lno1++, un*i+ad2, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, un*(i+1)+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 1);
				}
				PutFlp1(op, lno1++, un-1+un*i+ad2, 0);
				PutFlp1(op, lno1++, un-1+un*i+ad1, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, un-1+ad1, 0);
					PutFlp1(op, lno1++, un-1+ad2, 1);
				} else {
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad2, 1);
				}
			}
		} else {														// <U�J&V�J�̏ꍇ>
			if(uflg==0||uflg==2) {										// <���ʐ擪ٰ��>
				for(i=0; i<vn-1; i++) {
					PutFlp1(op, lno1++, un*i+ad1, 0);
					PutFlp1(op, lno1++, un*i+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 1);
				}
			} else {
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, 1+ad2, 0);
				PutFlp1(op, lno1++, 1+ad1, 1);
				for(i=1; i<vn-1; i++) {
					PutFlp1(op, lno1++, un*(i-1)+ad1+1, 0);
					PutFlp1(op, lno1++, un*(i-1)+ad2+1, 0);
					PutFlp1(op, lno1++, un*i+ad2+1, 0);
					PutFlp1(op, lno1++, un*i+ad1+1, 1);
				}
			}
			if(uflg==0) {												// <Uٰ�ߕs����>
				for(i=0; i<vn-1; i++) {
					PutFlp1(op, lno1++, un-1+un*i+ad2, 0);
					PutFlp1(op, lno1++, un-1+un*i+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad2, 1);
				}
			} else if(uflg==1) {										// <�擪Uٰ�ߓ���>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, un+ad1, 0);
				PutFlp1(op, lno1++, un+ad2, 1);
				for(i=1; i<vn-1; i++) {
					PutFlp1(op, lno1++, un*i+ad2, 0);
					PutFlp1(op, lno1++, un*i+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad2, 1);
				}
			} else if(uflg==2) {										// <�ŏIUٰ�ߓ���>
				for(i=0; i<vn-2; i++) {
					PutFlp1(op, lno1++, un-1+un*i+ad2, 0);
					PutFlp1(op, lno1++, un-1+un*i+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un-1+un*(i+1)+ad2, 1);
				}
				PutFlp1(op, lno1++, un*(vn-1)+ad2-1, 0);
				PutFlp1(op, lno1++, un*(vn-1)+ad1-1, 0);
				PutFlp1(op, lno1++, un*(vn-1)+ad1, 0);
				PutFlp1(op, lno1++, un*(vn-1)+ad2, 1);
			} else {													// <�擪/�ŏI����>
				PutFlp1(op, lno1++, ad2, 0);
				PutFlp1(op, lno1++, ad1, 0);
				PutFlp1(op, lno1++, un+ad1, 0);
				PutFlp1(op, lno1++, un+ad2, 1);
				for(i=1; i<vn-2; i++) {
					PutFlp1(op, lno1++, un*i+ad2, 0);
					PutFlp1(op, lno1++, un*i+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad1, 0);
					PutFlp1(op, lno1++, un*(i+1)+ad2, 1);
				}
				PutFlp1(op, lno1++, un*(vn-2)+ad2, 0);
				PutFlp1(op, lno1++, un*(vn-2)+ad1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+ad1+1, 0);
				PutFlp1(op, lno1++, un*(vn-2)+ad2+1, 1);
			}
		}
		if(uflg==0||uflg==2) {
			for(i=0; i<vo; i++) {
				PutFlp2(op, lno2++, div*un*i+ad1, 0);					// ��ٰ��2�o�^
				PutFlp2(op, lno2++, div*un*i+ad2, 1);
			}
		} else {
			PutFlp2(op, lno2++, ad1, 0);
			PutFlp2(op, lno2++, ad2, 1);
			for(i=1; i<vo; i++) {
				PutFlp2(op, lno2++, un*(div*i-1)+ad1+1, 0);
				PutFlp2(op, lno2++, un*(div*i-1)+ad2+1, 1);
			}
		}
		if(uflg==0) {													// <Uٰ�ߕs����>
			for(i=0; i<vo; i++) {
				PutFlp2(op, lno2++, un-1+div*un*i+ad2, 0);
				PutFlp2(op, lno2++, un-1+div*un*i+ad1, 1);
			}
		} else if(uflg==1) {											// <�擪Uٰ�ߓ���>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<vo; i++) {
				PutFlp2(op, lno2++, div*un*i+ad2, 0);
				PutFlp2(op, lno2++, div*un*i+ad1, 1);
			}
		} else if(uflg==2) {											// <�ŏIUٰ�ߓ���>
			for(i=0; i<vo-1; i++) {
				PutFlp2(op, lno2++, un-1+div*un*i+ad2, 0);
				PutFlp2(op, lno2++, un-1+div*un*i+ad1, 1);
			}
			PutFlp2(op, lno2++, un*(vn-1)+ad2, 0);
			PutFlp2(op, lno2++, un*(vn-1)+ad1, 1);
		} else {														// <�擪/�ŏI����>
			PutFlp2(op, lno2++, ad2, 0);
			PutFlp2(op, lno2++, ad1, 1);
			for(i=1; i<vo-1; i++) {
				PutFlp2(op, lno2++, div*un*i+ad2, 0);
				PutFlp2(op, lno2++, div*un*i+ad1, 1);
			}
			PutFlp2(op, lno2++, un*(vn-2)+ad2+1, 0);
			PutFlp2(op, lno2++, un*(vn-2)+ad1+1, 1);
		}
	} else if(vflg==1) {												// <<�擪Vٰ�ߓ���>>
		if(vb) {														// <U�J&V�̏ꍇ>
			for(i=0; i<vn; i++) {
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad2, 0);
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad1, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, un-1+ad1, 0);
					PutFlp1(op, lno1++, un-1+ad2, 1);
				} else {
					PutFlp1(op, lno1++, (un-1)*(i+2)+ad1, 0);
					PutFlp1(op, lno1++, (un-1)*(i+2)+ad2, 1);
				}
			}
		} else {														// <U�J&V�J�̏ꍇ>
			for(i=0; i<vn-1; i++) {
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad2, 0);
				PutFlp1(op, lno1++, (un-1)*(i+1)+ad1, 0);
				PutFlp1(op, lno1++, (un-1)*(i+2)+ad1, 0);
				PutFlp1(op, lno1++, (un-1)*(i+2)+ad2, 1);
			}
		}
		for(i=0; i<vo; i++) {
			PutFlp2(op, lno2++, (un-1)*(div*i+1)+ad2, 0);
			PutFlp2(op, lno2++, (un-1)*(div*i+1)+ad1, 1);
		}
	} else if(vflg==2) {												// <<�ŏIVٰ�ߓ���>>
		if(vb) {														// <U�J&V�̏ꍇ>
			for(i=0; i<vn; i++) {
				if(i==0) {												// �␳(-)���ݒ�
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = i-1, ms2 = i;
				}
				PutFlp1(op, lno1++, un*i-ms1+ad1, 0);
				PutFlp1(op, lno1++, un*i-ms1+ad2, 0);
				if(i==vn-1) {
					PutFlp1(op, lno1++, ad2, 0);
					PutFlp1(op, lno1++, ad1, 1);
				} else {
					PutFlp1(op, lno1++, un*(i+1)-ms2+ad2, 0);
					PutFlp1(op, lno1++, un*(i+1)-ms2+ad1, 1);
				}
			}
		} else {														// <U�J&V�J�̏ꍇ>
			for(i=0; i<vn-1; i++) {
				if(i==0) {
					ms1 = 0, ms2 = 0;
				} else {
					ms1 = i-1, ms2 = i;
				}
				PutFlp1(op, lno1++, un*i-ms1+ad1, 0);
				PutFlp1(op, lno1++, un*i-ms1+ad2, 0);
				PutFlp1(op, lno1++, un*(i+1)-ms2+ad2, 0);
				PutFlp1(op, lno1++, un*(i+1)-ms2+ad1, 1);
			}
		}
		for(i=0; i<vo; i++) {
			ms1 = (div*i<2) ? 0 : div*i-1;
			PutFlp2(op, lno2++, un*div*i-ms1+ad1, 0);
			PutFlp2(op, lno2++, un*div*i-ms1+ad2, 1);
		}
	}
	*ln1 = lno1;														// �X�V���ٰ��1��
	*ln2 = lno2;														// �X�V���ٰ��2��
}

/******************************/
OBJTYPE* ThicknessObject(void)											//<<< �Ȗʌ��݂Â������
/******************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSPnt];
	OBJTYPE* op;
	CMDTYPE* cmp, * bcp;
	int      i, j, ln, un, vn, uo, vo, ub, vb, mx, pc, atr;
	int      ovn, oln1, oln2, nvn, nln1, nln2, uflg=0, vflg=0;
	double   dln;

	GetObjPtr(&op);														// �����Ώۗ��̎擾
	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	ln = -GetParaInt(cmp, 0);											// ���ݕ��擾
	BaseCmdPtr(1, op, &bcp);											// �L���擪����ގ擾
	un = GetParaInt(bcp, 0);											// U����_���擾
	vn = GetParaInt(bcp, 1);											// V����_���擾
	ub = GetParaInt(bcp, 2);											// U���EӰ�ގ擾
	vb = GetParaInt(bcp, 3);											// V���EӰ�ގ擾
	mx = GetParaInt(bcp, 4);											// �Ȑ����J�Ԑ�
	if(GetCurveReptSW()) {												// <�Ȗʍו����I��>
		mx = GetCurveReptNum();											// �Ȗʕ������擾
	}
	uo = un, vo = vn;
	if((atr=GetObjAtr(op))==21) {										// <�Ȗʺ���ނ̏ꍇ>
		for(j=0, pc=0; j<vn; j++) {										// ����_�擾
			for(i=0; i<un; i++) {
				GetParaPnt(bcp, un*j+i, &pa[pc++]);
			}
			if(ub) {													// <U�̏ꍇ>
				GetParaPnt(bcp, un*j, &pa[pc++]);
			}
		}
		if(ub) {														// U����_��UP
			un++;
		}
		if(vb) {														// <V�̏ꍇ>
			for(i=0; i<un; i++) {
				pa[pc++] = pa[i];
			}
			vn++;														// V����_��UP
		}
		if(!vb) {
			uflg = UPointCheck(un, vn, pa);								// U����_�����m�F
		}
		if(!ub) {
			vflg = VPointCheck(un, vn, pa);								// V����_�����m�F
		}
	} else {															// <�|���Ȗʺ���ނ̏ꍇ>
		if(ub) {
			un++;														// U����_��UP
		}
		if(vb) {
			vn++;														// V����_��UP
		}
	}
	for(i=0, pc=1; i<mx; i++) {											// �����}�����v�Z
		pc = 2*pc;
	}
	dln = ln/1000.0;													// ���ݕ�������
	un = (un-1)*pc+1;													// U�Ȑ����㒸�_��
	vn = (vn-1)*pc+1;													// V�Ȑ����㒸�_��
	ovn  = GetVtxNum(op);												// �����_���W���擾
	oln1 = GetFlpNum1(op);												// ����ٰ��1���擾
	oln2 = GetFlpNum2(op);												// ����ٰ��2���擾
	nvn  = 2*ovn;														// �V���_���W��
	nln1 = 2*oln1;														// �V��ٰ��1��
	nln2 = 2*oln2;														// �V��ٰ��2��
	if(!vb) {															// V�J�̒f��
		if(uflg==0) {
			nln1 += 2*(un-1)*4, nln2 += 2*uo*2;
		} else if(uflg==1||uflg==2) {
			nln1 += (un-1)*4, nln2 += uo*2;
		}
	}
	if(!ub) {															// U�J�̒f��
		if(vflg==0) {
			nln1 += 2*(vn-1)*4, nln2 += 2*vo*2;
		} else if(vflg==1||vflg==2) {
			nln1 += (vn-1)*4, nln2 += vo*2;
		}
	}
	for(i=0; i<ovn; i++) {
		GetVtx(op, i, &pa[i]);											// ���_���W�擾
	}
	if(GetThickFVec(op, dln, ovn, oln1, pa)) {							// �@���޸�وړ�
		AppendCmd(op, cmp);												// ����&����ސڑ�
		ChangeObj(op, nvn, nln1, nln2);									// ���̗̈�ύX
		LevelCmd(cmp);													// �������ٺ���ސڑ�
		if(!GetCmdExeStp()) {											// <<<����ގ��s��~�Ȃ�>>>
			PutThickVtx(op, ovn, pa);									// ���_���W�ݒ�
			PutThickFlp(op, ovn, oln1, oln2);							// ��ٰ�ߐݒ�
			ReverseFlp(op, ln, oln1, oln2);								// ��ٰ�ߔ��]
			nln1 = 2*oln1, nln2 = 2*oln2;
			if(!vb) {
				PutThickFlpU(op, uflg, vflg, ovn, &nln1, &nln2, ln,
			                 ub, un, vn, uo, mx);						// U�f��ٰ�ߐݒ�
	        }
		    if(!ub) {
			    PutThickFlpV(op, uflg, vflg, ovn, &nln1, &nln2, ln,
				             vb, un, vn, vo, mx);						// V�f��ٰ�ߐݒ�
			}
		}
		if(atr==21) {
			PutObjAtr(op, 20);											// ����No�ݒ�(SURFACE)
		} else {
			PutObjAtr(op, 22);											// ����No�ݒ�(SWEEP)
		}
		if(!GetCmdExeStp()&&!GetResetObjMode()) {						// <����ޒ�~��&RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	delete[] pa;
	return op;
}
