/////////////////////////////////////////////////////////////////////////////
// �r�Ȑ� ����_<->�ʉߓ_ �ϊ���
// ConvSCurve.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/05/27 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"

/****************************************************************/
static void MakeSCurveMatrix(double wt, int bc, int pn, int* ea,
                             double* ma, double* mb)					//<<< S�Ȑ��ϊ���د���쐬
/****************************************************************/
{
	int* ew = new int[View->m_MaxCPnt+1];
	int  i, i1;

	if(pn<1) {															// ����_��1�����͒��~
		delete[] ew;
		return;
	}
	i1 = pn*pn;
	for(i=0; i<i1; i++) {												// ��د��������
		ma[i] = 0.0, mb[i] = 0.0;
	}
	for(i=0; i<pn; i++) {
		ew[i] = ea[i];													// ײ�Ӱ�ޕ���
	}
	if(!bc) {															// ���E�J�͎n�_�I�_��ܐ��_
		ew[0] = 1, ew[pn-1] = 1;
	}
	for(i=0; i<pn; i++) {
		if(ew[i]==1) {													// <�ܐ��_�̏ꍇ>
			ma[i*pn+i] = 1.0, mb[i*pn+i] = 1.0;
		} else {														// <�Ȑ��_�̏ꍇ>
			ma[i*pn+i] = wt+2.0, mb[i*pn+i] = wt+4.0;
			if(i==0) {
				ma[1] = 1.0, ma[pn-1] = 1.0;
			} else if(i==pn-1) {
				ma[pn*pn-2] = 1.0, ma[pn*pn-pn] = 1.0;
			} else {
				ma[i*pn+i-1] = 1.0, ma[i*pn+i+1] = 1.0;
			}
		}
	}
	delete[] ew;
}

/************************************************************/
static BOOL CalcSCurveMatrix(int pn, double* ma, double* mb)			//<<< S�Ȑ��ϊ���د���v�Z
/************************************************************/
{
	double* wa = new double[View->m_MaxCCtA];
	double* wb = new double[View->m_MaxCCtA];
	int     i, j, k, l, er;
	double  d;

	for(i=0; i<pn; i++) {
		for(er=1, j=i; j<pn; j++) {										// �Y��(n�sn��)�ʒu���L��
			d = ma[i*pn+i];												// �ȍs����������
			if(fabs(d)>DEPSILON) {										// 0�łȂ���Ύ�������
				er = 0; break;
			}
			for(l=0; l<pn; l++) {
				wa[l] = ma[i*pn+l], wb[l] = mb[i*pn+l];					// �����ް��s��ۑ�
			}
			for(k=i; k<pn-1; k++) {
				for(l=0; l<pn; l++) {
					ma[i*pn+l] = ma[(i+1)*pn+l];						// �������s��O�ɼ��
					mb[i*pn+l] = mb[(i+1)*pn+l];
				}
			}
			for(l=0; l<pn; l++) {
				ma[(pn-1)*pn+l] = wa[l], mb[(pn-1)*pn+l] = wb[l];		// �����ް��s�𖖔��s�Ɉړ�
			}
		}
		if(er) {														// �L���ް����ʹװү���ތ�I��
			AfxMessageBox(IDS_ERR_CONV);
			delete[] wa;
			delete[] wb;
			return FALSE;
		}
		for(l=0; l<pn; l++) {											// �Y��(n�sn��)�ʒu�̌W����
			ma[i*pn+l] /= d;											// �Y��(n)�s���ް�������
			mb[i*pn+l] /= d;											// [�Y��(n�sn��)�W��=1.0]
		}
		for(j=0; j<pn; j++) {
			d = ma[j*pn+i];												// �Y��(n)�s�ȊO�̍s��
			if((j!=i)&&(fabs(d)>DEPSILON)) {							// �Y��(n)��̌W�����L���ȏꍇ
				for(l=0; l<pn; l++) {									// �Y��(n)�s�ȊO�̍s���ް�����
					ma[j*pn+l] -= (d*ma[i*pn+l]);						// �W���~�Y��(n)�s���ް������Z
					mb[j*pn+l] -= (d*mb[i*pn+l]);						// [n���n�s�ȊO�̍s�̌W��=0.0]
				}
			}
		}
	}
	delete[] wa;
	delete[] wb;
	return TRUE;
}

/*******************************************************/
void SetCurveMatrix1(int md, double wt, int bc, int pn,					//<<< S�Ȗʕϊ���د���ݒ�1
					 int* ea, double* ma, double* mb)					//    (�ʉߓ_=>����_)
/*******************************************************/
{
	if(md) {															// <�ʉߓ_�w��̏ꍇ>
		MakeSCurveMatrix(wt, bc, pn, ea, ma, mb);						// S�Ȑ��ϊ���د���쐬
		CalcSCurveMatrix(pn, ma, mb);									// �ʉߓ_=>����_�ϊ���د��
	}
}

/*******************************************************/
void SetCurveMatrix2(int md, double wt, int bc, int pn,					//<<< S�Ȗʕϊ���د���ݒ�2
					 int* ea, double* ma, double* mb)					//    (����_=>�ʉߓ_)
/*******************************************************/
{
	if(md) {															// <�ʉߓ_�w��̏ꍇ>
		MakeSCurveMatrix(wt, bc, pn, ea, ma, mb);						// S�Ȑ��ϊ���د���쐬
		CalcSCurveMatrix(pn, mb, ma);									// ����_=>�ʉߓ_�ϊ���د��
	}
}

/*******************************************************/
void SetCurveMatrix3(int md, double wt, int bc, int pn,					//<<< S�Ȗʕϊ���د���ݒ�3
					 int* ea, double* ma, double* mb)					//    (����_<=>�ʉߓ_)
/*******************************************************/
{
	double* wa = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double* wb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	int     i, mx;

	if(md) {															// <�ʉߓ_�w��̏ꍇ>
		MakeSCurveMatrix(wt, bc, pn, ea, ma, mb);						// S�Ȑ��ϊ���د���쐬
		mx = pn*pn;
		for(i=0; i<mx; i++) {											// �ϊ���د������
			wa[i] = ma[i], wb[i] = mb[i];
		}
		CalcSCurveMatrix(pn, wb, ma);									// ����_=>�ʉߓ_�ϊ���د���v�Z
		CalcSCurveMatrix(pn, wa, mb);									// �ʉߓ_=>����_�ϊ���د���v�Z
	}
	delete[] wa;
	delete[] wb;
}

/***********************************************************************/
static void CalcCtrlPoint(int pn, PNTTYPE* pa, PNTTYPE* pb, double* ma)	//<<< ����_<=>�ʉߓ_�v�Z���
/***********************************************************************/
{
	int    i, j, k;
	double d[3];

	for(i=0; i<pn; i++) {
		d[0] = 0.0, d[1] = 0.0, d[2] = 0.0;
		for(j=0; j<pn; j++) {
			for(k=0; k<3; k++) {
				d[k] += pa[j].p[k]*ma[i*pn+j];
			}
		}
		pb[i].p[0] = d[0], pb[i].p[1] = d[1], pb[i].p[2] = d[2];
	}
}

/************************************************/
void CalcCurvePoint(int md, int pn, PNTTYPE* pa,						//<<< ����_<=>�ʉߓ_�v�Z
					PNTTYPE* pb, double* ma)							//    (�Ȑ��ϊ�)
/************************************************/
{
	int i;

	if(md) {															// <�ʉߓ_�w��̏ꍇ>
		CalcCtrlPoint(pn, pa, pb, ma);									// ����_<=>�ʉߓ_�v�Z
	} else {															// <����_�w��̏ꍇ>
		for(i=0; i<pn; i++) {
			pb[i] = pa[i];												// ����_����
		}
	}
}

/*************************************************************/
void CalcSurfacePoint(int md, int f1, int f2, int un, int vn,
                      int ub, int vb, double wt, int* ea,				//<<< ����_<=>�ʉߓ_�v�Z
                      PNTTYPE* pa, PNTTYPE* pb)							//    (�ʏ�Ȗʕϊ�)
/*************************************************************/
{
	PNTTYPE* p1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* p2 = new PNTTYPE[View->m_MaxCCtA];
	int*     i1 = new int[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	int      i, j;

	if(f1==0) {															// <<<�ʉߓ_=>����_�ϊ�>>>
		for(i=0; i<un; i++) {											// <<V�����̕ϊ�>>
			if(i==0||f2==0) {											// <�擪��/�ҏW���ϊ��̏ꍇ>
				for(j=0; j<vn; j++) {
					i1[j] = (ea[i+un*j]>1)?1:0;							// V�����Ȑ�Ӱ�ޒ��o
				}
				SetCurveMatrix1(md, wt, vb, vn, i1, ma, mb);			// �ʉߓ_=>����_�ϊ���د��
			}
			for(j=0; j<vn; j++) {
				p1[j] = pa[i+un*j];										// V��������_���o
			}
			CalcCurvePoint(md, vn, p1, p2, mb);							// �ʉߓ_=>����_�ϊ�
			for(j=0; j<vn; j++) {
				pb[i+un*j] = p2[j];										// V��������_�ݒ�
			}
		}
		for(i=0; i<vn; i++) {											// <<U�����̕ϊ�>>
			if(i==0||f2==0) {											// <�擪��/�ҏW���ϊ��̏ꍇ>
				for(j=0; j<un; j++) {
					i1[j] = (ea[i*un+j]%2==1)?1:0;						// U�����Ȑ�Ӱ�ޒ��o
				}
				SetCurveMatrix1(md, wt, ub, un, i1, ma, mb);			// �ʉߓ_=>����_�ϊ���د��
			}
			for(j=0; j<un; j++) {
				p1[j] = pb[i*un+j];										// U��������_���o
			}
			CalcCurvePoint(md, un, p1, p2, mb);							// �ʉߓ_=>����_�ϊ�
			for(j=0; j<un; j++) {
				pb[i*un+j] = p2[j];										// U��������_�ݒ�
			}
		}
	} else {															// <<<����_=>�ʉߓ_�ϊ�>>>
		for(i=0; i<vn; i++) {											// <<U�����̕ϊ�>>
			if(i==0||f2==0) {											// <�擪��/�ҏW���ϊ��̏ꍇ>
				for(j=0; j<un; j++) {
					i1[j] = (ea[i*un+j]%2==1)?1:0;						// U�����Ȑ�Ӱ�ޒ��o
				}
				SetCurveMatrix2(md, wt, ub, un, i1, ma, mb);			// ����_=>�ʉߓ_�ϊ���د��
			}
			for(j=0; j<un; j++) {
				p1[j] = pa[i*un+j];										// U��������_���o
			}
			CalcCurvePoint(md, un, p1, p2, ma);							// ����_=>�ʉߓ_�ϊ�
			for(j=0; j<un; j++) {
				pb[i*un+j] = p2[j];										// U��������_�ݒ�
			}
		}
		for(i=0; i<un; i++) {											// <<V�����̕ϊ�>>
			if(i==0||f2==0) {											// <�擪��/�ҏW���ϊ��̏ꍇ>
				for(j=0; j<vn; j++) {
					i1[j] = (ea[i+un*j]>1)?1:0;							// V�����Ȑ�Ӱ�ޒ��o
				}
				SetCurveMatrix2(md, wt, vb, vn, i1, ma, mb);			// ����_=>�ʉߓ_�ϊ���د��
			}
			for(j=0; j<vn; j++) {
				p1[j] = pb[i+un*j];										// V��������_���o
			}
			CalcCurvePoint(md, vn, p1, p2, ma);							// ����_=>�ʉߓ_�ϊ�
			for(j=0; j<vn; j++) {
				pb[i+un*j] = p2[j];										// V��������_�ݒ�
			}
		}
	}
	delete[] p1;
	delete[] p2;
	delete[] i1;
	delete[] ma;
	delete[] mb;
}
