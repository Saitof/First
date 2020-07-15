/////////////////////////////////////////////////////////////////////////////
// �Ώ̎��ҏW���암
// Symmetry.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "NumFunc.h"

/******************************/
BOOL InputRevolveDivi(int* dn)											//<<< ��]�Ώ̕������擾
/******************************/
{
	CInputDlg dlg(24);
	dlg.SetValue(*dn);													// �����l�\��
	while(TRUE) {
		if(dlg.DoModal()==IDOK) {										// �޲�۸ޕ\��
			dlg.GetValue(dn);											// ��]�Ώ̕������擾
			if(*dn>View->m_MaxCCtl) {									// <����_����ȏ�>
				AfxMessageBox(IDS_ERR_RDIVI);							// ��L�ȊO�ʹװү����
			} else {
				return TRUE;
			}
		} else {
			return FALSE;
		}
	}
}

/************************************************************/
void SetSymmetryLine(int flg, int mod, PNTTYPE cpt, int pln,
                     int lin, int div, int* dr1, int* dr2)				//<<< �Ώ̎��ݒ�
/************************************************************/
{
	PNTTYPE* ps = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* pe = new PNTTYPE[View->m_MaxCCtA];
	int*     cl = new int[View->m_MaxCCtA];
	PNTTYPE  pt;
	VECTYPE  vec;
	double   th, rot[3][3];
	int      i, j;

	for(i=0; i<View->m_MaxCCtA; i++) {									// �װ���ݒ�(����)
		cl[i] = 0;
	}
	if(mod==1) {														// <<<1���Ώ̂̏ꍇ>>>
		if(!lin) {														// <�c�����̏ꍇ>
			if(pln==0) {
				*dr1 = 1, *dr2 = 0;
			} else if(pln==1) {
				*dr1 = 1, *dr2 = 2;
			} else {
				*dr1 = 0, *dr2 = 2;
			}
		} else {														// <�������̏ꍇ>
			if(pln==0) {
				*dr1 = 0, *dr2 = 1;
			} else if(pln==1) {
				*dr1 = 2, *dr2 = 1;
			} else {
				*dr1 = 2, *dr2 = 0;
			}
		}
		ps[0] = cpt, ps[0].p[*dr2] -= 2000.0;							// ײ�1�n�_
		pe[0] = cpt, pe[0].p[*dr2] += 2000.0;							// ײ�1�I�_
		View->Line.Setup(ps, pe, cl, 1, pln);							// ײݐݒ�
	} else if(mod==2) {													// <<<2���Ώ̂̏ꍇ>>>
		if(pln==0) {
			*dr1 = 0, *dr2 = 1;
		} else if(pln==1) {
			*dr1 = 1, *dr2 = 2;
		} else {
			*dr1 = 2, *dr2 = 0;
		}
		ps[0] = cpt, ps[0].p[*dr1] -= 2000.0;							// ײ�1�n�_
		pe[0] = cpt, pe[0].p[*dr1] += 2000.0;							// ײ�1�I�_
		ps[1] = cpt, ps[1].p[*dr2] -= 2000.0;							// ײ�2�n�_
		pe[1] = cpt, pe[1].p[*dr2] += 2000.0;							// ײ�2�I�_
		View->Line.Setup(ps, pe, cl, 2, pln);							// ײݐݒ�
	} else {															// <<<��]�Ώ̂̏ꍇ>>>
		for(i=0; i<DIM; i++) {
			vec.p[i] = 0.0;
		}
		j = (!flg) ? 2 : div;											// ײݐ�
		for(i=0; i<j; i++) {
			ps[i] = cpt;												// ײݎn�_�ݒ�
		}
		if(pln==0) {
			*dr1 = 0, *dr2 = 1, j = 0, vec.p[2] = -1.0;
		} else if(pln==1) {
			*dr1 = 2, *dr2 = 1, j = 1, vec.p[0] = -1.0;
		} else {
			*dr1 = 2, *dr2 = 0, j = 1, vec.p[1] = -1.0;
		}
		th = PI/div;													// 1����������̊p�x/2
		for(i=0; i<DIM; i++) {
			pe[0].p[i] = 0.0;
		}
		if(j==0) {
			pe[0].p[*dr1] -= 2000.0;
		} else {
			pe[0].p[*dr1] += 2000.0;
		}
		pe[1] = pe[0], pt = pe[0];
		SetRotateMatrix(vec, th, rot);									// ��]��د���쐬
		VecRotateMove(&pe[0], rot, cpt);								// ײ�1�I�_��]�ړ�
		VecRotate(&pt, rot);											// ��_��]
		SetRotateMatrix(vec, -th, rot);									// ��]��د���쐬
		VecRotateMove(&pe[1], rot, cpt);								// ײ�2�I�_��]�ړ�
		if(flg&&div>2) {												// <��ȕң��3�����ȏ�>
			th = 2.0*PI/div;											// 1����������̊p�x
			for(i=2; i<div; i++) {
				pe[i] = pt;
				SetRotateMatrix(vec, th*(i-1), rot);					// ��]��د���쐬
				VecRotateMove(&pe[i], rot, cpt);						// ײ�3�ȍ~�I�_��]�ړ�
			}
			i = div;													// ײݐ�
		} else {
			i = 2;
		}
		View->Line.Setup(ps, pe, cl, i, pln);							// ײݐݒ�
	}
	delete[] ps;
	delete[] pe;
	delete[] cl;
}

/**************************************************************/
BOOL CheckSymmetryArea(int* chk, PNTTYPE pnt, int mod,
                       PNTTYPE cpt, int div, int dr1, int dr2)			//<<< �Ώ̗̈�����
/**************************************************************/
{
	BOOL   ret=TRUE;
	int    wch=0, ch1=-1, ch2=-1;
	double th1, th2, l1, l2;

	if(mod==0) {														// �Ώ̎w��ȊO�͖�����OK
		return ret;
	} else if(mod==1) {													// <<<1���Ώ̂̏ꍇ>>>
		if(pnt.p[dr1]<cpt.p[dr1]) {										// ����_�����̈�
			wch = 1;
		} else if(pnt.p[dr1]>cpt.p[dr1]) {								// ����_�����̈�
			wch = 2;
		}
		if(*chk>0&&wch>0&&*chk!=wch) {									// ���ɗ̈挈��ςňٗ̈�����
			ret = FALSE;
		}
		if(*chk==0) {													// �̈斢����͍X�V
			*chk = wch;
		}
	} else if(mod==2) {													// <<<2���Ώ̂̏ꍇ>>>
		if(pnt.p[dr1]<cpt.p[dr1]) {										// ����_��dr1���������̈�
			ch1 = 1;
		} else if(pnt.p[dr1]>cpt.p[dr1]) {								// ����_��dr1���������̈�
			ch1 = 2;
		}
		if(pnt.p[dr2]<cpt.p[dr2]) {										// ����_��dr2���������̈�
			ch2 = 0;
		} else if(pnt.p[dr2]>cpt.p[dr2]) {								// ����_��dr2���������̈�
			ch2 = 2;
		}
		if(ch1>0) {
			if(ch2>=0) {												// �Ώ̗̈�No�ݒ�
				wch = ch1 + ch2;
			} else {
				if(ch1==1) {
					wch = -3;
				} else if(ch1==2) {
					wch = -4;
				}
			}
		} else {
			if(ch2==0) {
				wch = -1;
			} else if(ch2==2) {
				wch = -2;
			}
		}
		if(*chk==1) {													// ���ɗ̈挈��ςňٗ̈�����
			if(wch!=0&&wch!=1&&wch!=-1&&wch!=-3) {
				ret = FALSE;
			}
		} else if(*chk==2) {
			if(wch!=0&&wch!=2&&wch!=-1&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==3) {
			if(wch!=0&&wch!=3&&wch!=-2&&wch!=-3) {
				ret = FALSE;
			}
		} else if(*chk==4) {
			if(wch!=0&&wch!=4&&wch!=-2&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==-1) {
			if(wch!=0&&wch!=1&&wch!=2&&wch!=-1&&wch!=-3&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==-2) {
			if(wch!=0&&wch!=3&&wch!=4&&wch!=-2&&wch!=-3&&wch!=-4) {
				ret = FALSE;
			}
		} else if(*chk==-3) {
			if(wch!=0&&wch!=1&&wch!=3&&wch!=-1&&wch!=-2&&wch!=-3) {
				ret = FALSE;
			}
		} else if(*chk==-4) {
			if(wch!=0&&wch!=2&&wch!=4&&wch!=-1&&wch!=-2&&wch!=-4) {
				ret = FALSE;
			}
		}
		if((*chk==-1&&wch==-3)||(*chk==-3&&wch==-1)) {					// �����̋��E�I���͗̈挈��
			wch = 1;
		} else if((*chk==-1&&wch==-4)||(*chk==-4&&wch==-1)) {
			wch = 2;
		} else if((*chk==-2&&wch==-3)||(*chk==-3&&wch==-2)) {
			wch = 3;
		} else if((*chk==-2&&wch==-4)||(*chk==-4&&wch==-2)) {
			wch = 4;
		}
		if(*chk==0) {													// �̈斢����͍X�V
			*chk = wch;
		} else if(*chk<0&&wch!=0&&ret==TRUE) {
			*chk = wch;
		}
	} else if(mod==3) {													// <<<��]�Ώ̂̏ꍇ>>>
		if(fabs(pnt.p[dr1]-cpt.p[dr1])>EPSILON||
			fabs(pnt.p[dr2]-cpt.p[dr2])>EPSILON) {
			th1 = PI/div;												// �Ώ̗̈�̍ő�p�x/2
			l1 = pnt.p[dr1]-cpt.p[dr1];
			if(dr1==0) {
				l1 = -l1;
			}
			l2 = pnt.p[dr2]-cpt.p[dr2];
			if(dr2==0) {
				l2 = -l2;
			}
			th2 = atan2(l2, l1);										// ����_�̊p�x
			if(th1<fabs(th2)) {											// �Ώ̗̈������
				ret = FALSE;
			}
		}
	}
	return ret;
}

/*******************************************************/
static int CopySymmetry1(PNTTYPE cpt, int dr1, int* pn,
                         PNTTYPE* pa, int* ia, int* bc)					//<<< 1���Ώ̗̈敡��
/*******************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	int*     i2 = new int[View->m_MaxCCtA];
	int      i, j, n, ct=0, ret=0;
	double   ps, ln1, ln2;

	n = *pn;															// �w�萧��_��
	for(i=0; i<n; i++) {
		p1[i] = pa[i], i1[i] = ia[i];									// ����_��񕡎�
	}
	ps = cpt.p[dr1];													// �Ώ̎��̍��W
	ln1 = fabs(p1[0].p[dr1]-ps);										// �n�_�ƑΏ̎��̋���
	ln2 = fabs(p1[n-1].p[dr1]-ps);										// �I�_�ƑΏ̎��̋���
	for(i=0; i<n; i++) {
		p2[i] = p1[i], p2[i].p[dr1] = ps-(p1[i].p[dr1]-ps);				// �����ɐ���_���ړ�
		i2[i] = i1[i];
	}
	if(ln1<EPSILON&&ln2<EPSILON) {										// <�n�_��I�_���Ώ̎��ɐڂ���>
		*bc = 1;														// ���E�͕K����
		for(i=0; i<n; i++) {
			pa[ct] = p1[i], ia[ct++] = i1[i];							// �w�萧��_�������Őݒ�
		}
		for(i=n-2; i>0; i--) {
			pa[ct] = p2[i], ia[ct++] = i2[i];							// ���ʐ���_�̗��[�������~���Őݒ�
		}
	} else if(ln1<EPSILON||ln1<ln2) {									// <�n�_���ڂ���/�n�_�̕����߂�>
		for(i=n-1; i>=0; i--) {
			pa[ct] = p1[i], ia[ct++] = i1[i];							// �w�萧��_���~���Őݒ�
		}
		j = (ln1==0) ? 1 : 0;											// �ڂ���ꍇ�͕��ʑ��n�_��ΏۊO
		for(i=j; i<n; i++) {
			pa[ct] = p2[i], ia[ct++] = i2[i];							// ���ʑ�����_�������Őݒ�
		}
	} else {															// <�I�_���ڂ���/�I�_�̕����߂�>
		for(i=0; i<n; i++) {
			pa[ct] = p1[i], ia[ct++] = i1[i];							// �w�萧��_�������Őݒ�
		}
		j = (ln2==0) ? n-2 : n-1;										// �ڂ���ꍇ�͕��ʑ��I�_��ΏۊO
		for(i=j; i>=0; i--) {
			pa[ct] = p2[i], ia[ct++] = i2[i];							// ���ʐ���_���~���Őݒ�
		}
	}
	*pn = ct;															// �Ώ̕��ʌ�̐���_
	if(ln1==0) {														// �n�_�I�_���Ώ̎��ɐڂ�����
		ret += 1;
	}
	if(ln2==0) {
		ret += 1;
	}
	delete[] p1;
	delete[] p2;
	delete[] i1;
	delete[] i2;
	return ret;
}

/*****************************************************************/
static void CopySymmetry2(PNTTYPE cpt, int div, int dr1,
                          int dr2, int* pn, PNTTYPE* pa, int* ia)		//<<< ��]�Ώ̗̈敡��
/*****************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	PNTTYPE  pt;
	VECTYPE  vec;
	int      i, j, k, n, fg1, fg2, ct=0;
	double   rot[3][3], th, th1, th2, l1, l2;

	n = *pn;															// �w�萧��_��
	th = PI/div;														// �Ώ̗̈�ő�p�x/2(th)
	if(fabs(pa[0].p[dr1]-cpt.p[dr1])>EPSILON||
       fabs(pa[0].p[dr2]-cpt.p[dr2])>EPSILON) {
		l1 = (dr1==0) ? -(pa[0].p[dr1]-cpt.p[dr1]) :
                        pa[0].p[dr1]-cpt.p[dr1];
		l2 = (dr2==0) ? -(pa[0].p[dr2]-cpt.p[dr2]) :
                        pa[0].p[dr2]-cpt.p[dr2];
		th1 = atan2(l2, l1);											// �n�_�̊p�x(th1)
	} else {
		th1 = 0.0;
	}
	if(fabs(pa[n-1].p[dr1]-cpt.p[dr1])>EPSILON||
       fabs(pa[n-1].p[dr2]-cpt.p[dr2])>EPSILON) {
		l1 = (dr1==0) ? -(pa[n-1].p[dr1]-cpt.p[dr1]) :
                        pa[n-1].p[dr1]-cpt.p[dr1];
		l2 = (dr2==0) ? -(pa[n-1].p[dr2]-cpt.p[dr2]) :
                        pa[n-1].p[dr2]-cpt.p[dr2];
		th2 = atan2(l2, l1);											// �I�_�̊p�x(th2)
	} else {
		th2 = 0.0;
	}
	if(th+th1<th-th1) {													// �n�_�p�x��-th�x�ɋ߂�
		l1 = th+th1, fg1 = 1;
	} else {															// �n�_�p�x��+th�x�ɋ߂�
		l1 = th-th1, fg1 = 2;
	}
	if(th+th2<th-th2) {													// �I�_�p�x��-th�x�ɋ߂�
		l2 = th+th2, fg2 = 1;
	} else {															// �I�_�p�x��+th�x�ɋ߂�
		l2 = th-th2, fg2 = 2;
	}
	if(l1<l2) {															// <�n�_�̕����Ώ̐��ɋ߂�>
		for(i=n-1, j=0; i>=0; i--) {
			p1[j] = pa[i], i1[j++] = ia[i];								// ����_�����~���ɕ���
		}
	} else {															// <�I�_�̕����Ώ̐��ɋ߂�>
		for(i=0; i<n; i++) {
			p1[i] = pa[i], i1[i] = ia[i];								// ����_���������ɕ���
		}
		fg1 = fg2;
	}
	if(fg1==1) {														// -th�x�̑Ώ̐��ɋ߂��ꍇ(�t��])
		for(i=0; i<DIM; i++) {
			vec.p[i] = 1.0;
		}
	} else {															// +th�x�̑Ώ̐��ɋ߂��ꍇ(����])
		for(i=0; i<DIM; i++) {
			vec.p[i] = -1.0;
		}
	}
	vec.p[dr1] = 0.0, vec.p[dr2] = 0.0;
	th = 2.0*PI/div;													// �Ώ̗̈�ő�p�x(th)
	for(i=0; i<div; i++) {
		SetRotateMatrix(vec, th*i, rot);								// ��]��د���쐬
		for(j=0; j<n; j++) {
			for(k=0; k<DIM; k++) {
				pt.p[k] = p1[j].p[k]-cpt.p[k];
			}
			VecRotateMove(&pt, rot, cpt);								// ��]�ړ�
			pa[ct] = pt;												// ��]��̐���_��ݒ�
			ia[ct++] = i1[j];
		}
	}
	*pn = ct;															// �Ώ̕��ʌ�̐���_
	delete[] p1;
	delete[] i1;
}

/************************************************************/
int CopySymmetryArea(int mod, PNTTYPE cpt, int div, int dr1,
                     int dr2, int* pn, int* bc, PNTTYPE* pa,
                     int* ia, int* fg1, int* fg2)						//<<< �Ώ̗̈敡��
/************************************************************/
{
	int    rt, df1, df2, ret=0;
	double l1, l2, ln1, ln2;

	*fg1 = -1, *fg2 = -1;												// �����͐ڂ��Ȃ����
	if(mod==1) {														// <<<1���Ώ̂̏ꍇ>>>
		rt = CopySymmetry1(cpt, dr1, pn, pa, ia, bc);					// 1���Ώ̗̈敡��
		if(rt==1) {														// �ڂ�����������ݒ�
			*fg2 = dr1;
		} else if(rt==2) {
			*fg1 = dr1, *fg2 = dr1;
		}
	} else if(mod==2) {													// <<<2���Ώ̂̏ꍇ>>>
		l1 = fabs(pa[0].p[dr1]-cpt.p[dr1]);								// �n�_��dr1�Ώ̎��̋���
		l2 = fabs(pa[0].p[dr2]-cpt.p[dr2]);								// �n�_��dr2�Ώ̎��̋���
		if(l1<EPSILON&&l2<EPSILON) {									// �n�_���Ώ̎����S�ƈ�v
			ln1 = 0.0, df1 = 0;
		} else if(l1<l2) {												// �n�_��dr1�Ώ̎��ɋ߂�
			ln1 = l1, df1 = 1;
		} else {														// �n�_��dr2�Ώ̎��ɋ߂�
			ln1 = l2, df1 = 2;
		}
		l1 = fabs(pa[*pn-1].p[dr1]-cpt.p[dr1]);							// �I�_��dr1�Ώ̎��̋���
		l2 = fabs(pa[*pn-1].p[dr2]-cpt.p[dr2]);							// �I�_��dr2�Ώ̎��̋���
		if(l1<EPSILON&&l2<EPSILON) {									// �I�_���Ώ̎����S�ƈ�v
			ln2 = 0.0, df2 = 0;
		} else if(l1<l2) {												// �I�_��dr1�Ώ̎��ɋ߂�
			ln2 = l1, df2 = 1;
		} else {														// �I�_��dr2�Ώ̎��ɋ߂�
			ln2 = l2, df2 = 2;
		}
		if(ln1<EPSILON&&ln2<EPSILON) {									// <<2�_���ڂ���ꍇ>>
			if((df1==1&&df2==2)||(df1==2&&df2==1)) {					// <2�������ɐڂ���>
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1����1���Ώ̗̈敡��
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2����1���Ώ̗̈敡��
				*fg1 = dr2, *fg2 = dr1;									// �ڂ�����������ݒ�
			} else if(df1==1||df2==1) {									// <dr1�Ώ̎��ɐڂ���>
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1����1���Ώ̗̈敡��
				*fg1 = dr1, *fg2 = dr1, ret = 1;						// �ڂ�����������ݒ�
			} else {													// <dr2�Ώ̎��ɐڂ���>
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2����1���Ώ̗̈敡��
				*fg1 = dr2, *fg2 = dr2, ret = 2;						// �ڂ�����������ݒ�
			}
		} else if(df1==0||df2==0) {										// <<���ꂩ���Ώ̎����S�ƈ�v>>
			if(df1==1||df2==1) {										// <�c�肪dr1�Ώ̎��ɋ߂�>
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1����1���Ώ̗̈敡��
				*fg2 = dr1, ret = 1;									// �ڂ�����������ݒ�
			} else {													// <�c�肪dr2�Ώ̎��ɋ߂�>
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2����1���Ώ̗̈敡��
				*fg2 = dr2, ret = 2;									// �ڂ�����������ݒ�
			}
		} else {														// <<���̑��̏ꍇ>>
			if(ln1>=ln2) {
				df1 = df2;
			}
			if(df1==1) {												// <<dr1�Ώ̎��̕����߂�>>
				rt = CopySymmetry1(cpt, dr1, pn, pa, ia, bc);			// dr1����1���Ώ̗̈敡��
				if(rt==1) {												// �ڂ�����������ݒ�
					*fg2 = dr1;
				}
				CopySymmetry1(cpt, dr2, pn, pa, ia, bc);				// dr2����1���Ώ̗̈敡��
			} else {													// <<dr2�Ώ̎��̕����߂�>>
				rt = CopySymmetry1(cpt, dr2, pn, pa, ia, bc);			// dr2����1���Ώ̗̈敡��
				if(rt==1) {												// �ڂ�����������ݒ�
					*fg2 = dr2;
				}
				CopySymmetry1(cpt, dr1, pn, pa, ia, bc);				// dr1����1���Ώ̗̈敡��
			}
		}
	} else if(mod==3) {													// <<<��]�Ώ̂̏ꍇ>>>
		CopySymmetry2(cpt, div, dr1, dr2, pn, pa, ia);					// ��]�Ώ̗̈敡��
	}
	return ret;
}

/*********************************************************/
void PickSymmetryArea(int mod, int div, int dn, int tn,
                      int fg1, int fg2, int* pn, int* pk)				//<<< �Ώ̗̈��ׯ���I��
/*********************************************************/
{
	int i, j, no, mx, bn;

	no = *pn, mx = *pn;													// �ݒ�ς��ׯ��No��
	if(mod==1) {														// <<<1���Ώ̂̏ꍇ>>>
		bn = (fg2>-1) ? dn-1 : dn;										// �No
		for(i=mx-1; i>=0; i--) {
			j = bn-pk[i];												// �No�Ƃ̍�
			if(j>0&&bn+j<tn) {
				pk[no++] = bn+j;										// �Ώ̗̈��ׯ��No�ǉ�
			}
		}
	} else if(mod==2) {													// <<<2���Ώ̂̏ꍇ>>>
		bn = (fg2>-1) ? dn-1 : dn;										// �No
		for(i=mx-1; i>=0; i--) {
			j = bn-pk[i];												// �No�Ƃ̍�
			if(j>0) {
				pk[no++] = bn+j;										// �Ώ̗̈��ׯ��No�ǉ�
			}
		}
		mx = no;
		bn = (fg2>-1) ? dn*2-1 : dn*2+1;								// �No
		if(fg1>-1) {
			bn -= 1;
		}
		for(i=mx-1; i>=0; i--) {
			j = bn-pk[i];												// �No�Ƃ̍�
			if(j>0&&bn+j<tn) {
				pk[no++] = bn+j;										// �Ώ̗̈��ׯ��No�ǉ�
			}
		}
	} else if(mod==3) {													// <<<��]�Ώ̂̏ꍇ>>>
		for(i=1; i<div; i++) {
			for(j=0; j<mx; j++) {
				pk[no++] = (dn+1)*i+pk[j];								// �Ώ̗̈��ׯ��No�ǉ�
			}
		}
	}
	*pn = no;
}

/************************************************************/
static int CalcRAreaNo(PNTTYPE pos, PNTTYPE cpt, int div,
                       int dr1, int dr2, int tn, double* ta)			//<<< ��]�̈�No�v�Z
/************************************************************/
{
	int    i, no=-1;
	double th, wth, l1, l2;

	if(fabs(pos.p[dr1]-cpt.p[dr1])>EPSILON||
       fabs(pos.p[dr2]-cpt.p[dr2])>EPSILON) {
		l1 = (dr1==0) ? -(pos.p[dr1]-cpt.p[dr1]):pos.p[dr1]-cpt.p[dr1];
		l2 = (dr2==0) ? -(pos.p[dr2]-cpt.p[dr2]):pos.p[dr2]-cpt.p[dr2];
		th = atan2(l2, l1);												// ����ړ��_�p�x�Z�o
	} else {
		th = 0.0;
	}
	wth = fabs(th);														// �ړ��_�p�x��Βl
	for(i=0; i<tn; i++) {
		if(wth>ta[i]) {													// �����p�x���ް��No�ݒ�
			no = i;
		} else {														// �����p�x�����͏I��
			break;
		}
	}
	if(no==-1) {														// �p�xta[0]�ȉ���0
		no = 0;
	} else if(div%2==0&&no==tn-1) {										// div����&ta[tn-1]���div-1
		no = div-1;
	} else {															// �ȊO�̏ꍇ��
		if(th>0.0) {													// �p�x����2*no+1
			no = 2*no+1;
		} else {														// �p�x����2*no+2
			no = 2*no+2;
		}
	}
	if(no>1) {
		if(no%2==1) {													// �̈�No���E���
			no = (no/2)+1;
		} else {														// �����ɕ��ёւ�
			no = div-(no/2);
		}
	}
	return no;
}

/****************************************************************/
void MoveSymmetryArea(PNTTYPE pos, PNTTYPE mov, int mod,
                      PNTTYPE cpt, int div, int dr1, int dr2,
                      int fg1, int fg2, int rvf, int dn,
                      int pn, int *pk, PNTTYPE* pa, PNTTYPE* ma)		//<<< �Ώ̗̈�ړ��ʐݒ�
/****************************************************************/
{
	PNTTYPE* mv = new PNTTYPE[View->m_MaxCCtA];
	double*  ta = new double[View->m_MaxCCtA/2+1];
	VECTYPE  vec;
	int      i, j, no, tn;
	double   th, rot[3][3];

	if(dn>0) {															// <�p��Ӱ�ނ̏ꍇ>
		if(pk[0]==1&&fg1>-1) {											// �n�_�̐ڎ����������͖���
			mov.p[fg1] = 0.0;
		}
		if(pk[dn-1]==1&&fg2>-1) {										// �I�_�̐ڎ����������͖���
			mov.p[fg2] = 0.0;
		}
	}
	if(mod==1) {														// <<<1���Ώ̂̏ꍇ>>>
		for(i=0; i<3; i++) {
			mv[i] = mov;
		}
		if(pos.p[dr1]<cpt.p[dr1]) {
			mv[1].p[dr1] = -mov.p[dr1];									// �Ώ̗̈敉���̈ړ���
		} else if(pos.p[dr1]>cpt.p[dr1]) {
			mv[0].p[dr1] = -mov.p[dr1];									// �Ώ̗̈搳���̈ړ���
		}
		if(rvf) {														// <���������]����>
			if(pos.p[dr1]<cpt.p[dr1]) {
				mv[1].p[dr2] = -mov.p[dr2];								// �Ώ̗̈敉���̈ړ���
			} else if(pos.p[dr1]>cpt.p[dr1]) {
				mv[0].p[dr2] = -mov.p[dr2];								// �Ώ̗̈搳���̈ړ���
			}
		}
		for(i=0; i<pn; i++) {
			if(pa[i].p[dr1]<cpt.p[dr1]) {								// �Ώ̗̈敉��
				j = 0;
			} else if(pa[i].p[dr1]>cpt.p[dr1]) {						// �Ώ̗̈搳��
				j = 1;
			} else {													// �Ώ̎���
				j = 2;
			}
			ma[i] = mv[j];												// �ړ��ʐݒ�
		}
	} else if(mod==2) {													// <<<2���Ώ̂̏ꍇ>>>
		for(i=0; i<9; i++) {
			mv[i] = mov;
		}
		if(pos.p[dr1]<cpt.p[dr1]) {										// dr1�������̈ړ���
			mv[2].p[dr1] = -mov.p[dr1];
			mv[5].p[dr1] = -mov.p[dr1];
			mv[8].p[dr1] = -mov.p[dr1];
		} else if(pos.p[dr1]>cpt.p[dr1]) {								// dr1�������̈ړ���
			mv[0].p[dr1] = -mov.p[dr1];
			mv[3].p[dr1] = -mov.p[dr1];
			mv[6].p[dr1] = -mov.p[dr1];
		}
		if(pos.p[dr2]<cpt.p[dr2]) {										// dr2�������̈ړ���
			mv[6].p[dr2] = -mov.p[dr2];
			mv[7].p[dr2] = -mov.p[dr2];
			mv[8].p[dr2] = -mov.p[dr2];
		} else if(pos.p[dr2]>cpt.p[dr2]) {								// dr2�������̈ړ���
			mv[0].p[dr2] = -mov.p[dr2];
			mv[1].p[dr2] = -mov.p[dr2];
			mv[2].p[dr2] = -mov.p[dr2];
		}
		for(i=0; i<pn; i++) {
			if(pa[i].p[dr1]<cpt.p[dr1]) {								// dr1������
				j = 0;
			} else if(pa[i].p[dr1]>cpt.p[dr1]) {						// dr1������
				j = 2;
			} else {													// dr1����
				j = 1;
			}
			if(pa[i].p[dr2]<cpt.p[dr2]) {								// dr2������
				j += 0;
			} else if(pa[i].p[dr2]>cpt.p[dr2]) {						// dr2������
				j += 6;
			} else {													// dr2����
				j += 3;
			}
			ma[i] = mv[j];												// �ړ��ʐݒ�
		}
	} else if(mod==3) {													// <<<��]�Ώ̂̏ꍇ>>>
		ta[0] = PI/div;													// �Ώ̗̈�̍ő�p�x/2
		th = 2.0*PI/div;												// �Ώ̗̈�̍ő�p�x
		tn = div/2;														// �������̈敪����
		for(i=1; i<tn; i++) {
			ta[i] = ta[0]+i*th;											// �̈敪���p�x
		}
		no = CalcRAreaNo(pos, cpt, div, dr1, dr2, tn, ta);				// ��]�̈�No�v�Z
		for(i=0; i<DIM; i++) {
			vec.p[i] = -1.0;											// ��]���ݒ�
		}
		vec.p[dr1] = 0.0, vec.p[dr2] = 0.0;
		for(i=0; i<div; i++) {											// ��]�̈斈�̈ړ��ʌv�Z
			SetRotateMatrix(vec, th*i, rot);							// ��]��د���쐬
			if((j=no+i)>div-1) {
				j -= div;
			}
			mv[j] = mov;												// �w��ړ���
			VecRotate(&mv[j], rot);										// �ړ��ʉ�]
		}
		for(i=0; i<pn; i++) {
			no = CalcRAreaNo(pa[i], cpt, div, dr1, dr2, tn, ta);		// ��]�̈�No�v�Z
			ma[i] = mv[no];												// �ړ��ʐݒ�
		}
	} else {															// <<<�ʏ�Ӱ�ނ̏ꍇ>>>
		for(i=0; i<pn; i++) {
			ma[i] = mov;												// �S�ē�������̈ړ���
		}
	}
	delete[] mv;
	delete[] ta;
}
