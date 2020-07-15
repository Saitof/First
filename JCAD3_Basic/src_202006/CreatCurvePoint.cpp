/////////////////////////////////////////////////////////////////////////////
// �Ȑ��ҏW���암
// CreatCurvePoint.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "BackCommand.h"
#include "InputDlg.h"
#include "NumFunc.h"
#include "ConvSCurve.h"

/***************************************************************/
void InsertCurvePoint(int* inst, int* pn, PNTTYPE* pa, int* ea)			//<<< S�Ȑ����Ԑ���_�}��
/***************************************************************/
{
	PNTTYPE pnt;
	int     i, j, in;

	CInputDlg dlg(23);
	dlg.SetValue(*inst);												// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �޲�۸ޕ\��
		dlg.GetValue(inst);												// �}������_���擾
		in = *inst;
	} else {
		in = 0;
	}
	if(in>0) {
		ea[0] = 1, ea[1] = 1;											// �n�_/�I�_�ܐ��_Ӱ��
		for(i=0; i<DIM; i++) {
			pnt.p[i] = pa[1].p[i]-pa[0].p[i];							// �n�I�_�ԕψʗ�
			pnt.p[i] = pnt.p[i]/(in+1);									// �}����������
		}
		pa[in+1] = pa[1], ea[in+1] = ea[1];								// �I�_����ɼ��
		*pn = 2+in;														// �V����_��
		for(i=1; i<=in; i++) {
			for(j=0; j<DIM; j++) {
				pa[i].p[j] = pa[0].p[j]+pnt.p[j]*i;						// �}��������_�ǉ�
			}
			ea[i] = 0;
		}
	}
}

/**************************************************/
void CalcRPoint(int md, int pl, int dv, double rd,
                int* pn, PNTTYPE* pa, int* ea)							//<<< R���_�ʒu�Z�o
/**************************************************/
{
	PNTTYPE pw[10], pts, pte, pt1, pt2, cpt;
	VECTYPE vec, vcs, vce;
	int     iw[10], i, i1, i2, n, ret=FALSE;
	double  th, ts, te, t1, t2, dd, ds, de;

	n = *pn;
	if(md==0) {															// <���ԓ_�̏ꍇ>
		for(i=n-3; i<n; i++) {
			pw[i-n+3] = pa[i], iw[i-n+3] = ea[i];						// �����ް�����
		}
	} else if(md==1) {													// <�n�_�̏ꍇ>
		pw[0] = pa[n-1], iw[0] = ea[n-1];								// �I�_�ް�����
		for(i=0; i<2; i++) {
			pw[i+1] = pa[i], iw[i+1] = ea[i];							// �n�_�ް�����
		}
	} else {															// <�I�_�̏ꍇ>
		for(i=n-2; i<n; i++) {
			pw[i-n+2] = pa[i], iw[i-n+2] = ea[i];						// �I�_�ް�����
		}
		pw[2] = pa[0], iw[2] = ea[0];									// �n�_�ް�����
	}
	th = PI-CalcRotAngle(&pw[0]);										// �p�x�擾
	if(th>PI*20.0/180.0&&th<PI*160.0/180.0) {							// <R>20�x&R<160�x�ŗL��>
		ds = PntD(&pw[1], &pw[0]);										// �Ώۓ_�ƑO��2�_��
		VecV(&pw[1], &pw[0], &vcs);										// �������޸�َZ�o
		VecN(&vcs);
		de = PntD(&pw[1], &pw[2]);
		VecV(&pw[1], &pw[2], &vce);
		VecN(&vce);
		dd = rd/tan(th/2.0);											// �Ώۓ_�����R�J�n����
		ds = (ds>de) ? de : ds;											// �O�㐧��_�ŒZ����
		if(dd>ds) {
			AfxMessageBox(IDS_ERR_CTRLL);								// �װү����
		} else {														// <<�w��R���̋����L>>
			if((n+3)*dv<=View->m_MaxCCtl) {								// <����_�������>
				ret = TRUE;
			} else {
				AfxMessageBox(IDS_MANY_CTRLP);							// �װү����
			}
		}
	} else {
		AfxMessageBox(IDS_ERR_ANGLE);									// �װү����
	}
	if(ret) {															// <R�w��OK>
		i1 = pl;														// �L�����W���ݒ�
		i2 = (pl+1>2) ? 0 : pl+1;
		for(i=0; i<DIM; i++) {
			vec.p[i] = (vcs.p[i]+vce.p[i])/2.0;							// �Ώۓ_�����R���S�_����
			pts.p[i] = pw[1].p[i]+vcs.p[i]*dd;							// R�J�n�_
			pte.p[i] = pw[1].p[i]+vce.p[i]*dd;							// R�I���_
		}
		VecN(&vec);
		ds = sqrt(rd*rd+dd*dd);											// �Ώۓ_�����R���S�_����
		for(i=0; i<DIM; i++) {
			cpt.p[i] = pw[1].p[i]+vec.p[i]*ds;							// R���S�_
		}
		if((ts=CalcBaseAngle(cpt, pts, pl))<0) {						// ��p�x�擾
			ts += 2.0*PI;
		}
		if((te=CalcBaseAngle(cpt, pte, pl))<0) {
			te += 2.0*PI;
		}
		if(fabs(ts-te)<PI) {
			t1 = (ts<te) ? PI-th : -(PI-th);							// R�����p�����p�x
		} else {
			t1 = (ts<te) ? -(PI-th) : PI-th;
		}
		t2 = ds-rd;														// �Ώۓ_��R�̋�����
		if(th>PI*80.0/180.0) {											// <�Ώۓ_�p�x>80�x>
			t1 = t1/3.5, t2 = (t2/6.0)*2.0*th/PI;
		} else if(th>PI*50.0/180.0) {									// <�Ώۓ_�p�x>50�x>
			t1 = t1/3.4, t2 = (t2/5.5)*2.0*th/PI;
		} else {														// <�Ώۓ_�p�x<=50�x>
			t1 = t1/3.3, t2 = (t2/5.0)*2.0*th/PI;
		}
		t2 = rd+t2;
		for(i=0; i<DIM; i++) {
			pt1.p[i] = cpt.p[i], pt2.p[i] = cpt.p[i];
		}
		pt1.p[i1] = cpt.p[i1]+t2*cos(ts+t1);
		pt1.p[i2] = cpt.p[i2]+t2*sin(ts+t1);
		pt2.p[i1] = cpt.p[i1]+t2*cos(te-t1);
		pt2.p[i2] = cpt.p[i2]+t2*sin(te-t1);
		iw[5] = iw[2], iw[4] = 1, iw[3] = 0, iw[2] = 0, iw[1] = 1;		// �Ȑ�Ӱ�޼��&�ݒ�
		pw[5] = pw[2], pw[4] = pte, pw[3] = pt2;						// ����_���&�ݒ�
		pw[2] = pt1,   pw[1] = pts;
		if(md==0) {														// <���ԓ_�̏ꍇ>
			for(i=1; i<6; i++) {
				pa[i+n-3] = pw[i], ea[i+n-3] = iw[i];					// R���_�ް��ݒ�
			}
		} else if(md==1) {												// <�n�_�̏ꍇ>
			for(i=n-1; i>0; i--) {
				pa[i+3] = pa[i], ea[i+3] = ea[i];						// �ް����
			}
			for(i=1; i<5; i++) {
				pa[i-1] = pw[i], ea[i-1] = iw[i];						// R���_�ް��ݒ�
			}
		} else {														// <�I�_�̏ꍇ>
			for(i=1; i<5; i++) {
				pa[i+n-2] = pw[i], ea[i+n-2] = iw[i];					// R���_�ް��ݒ�
			}
		}
		*pn = n+3;														// ����_��UP
	}
}

/******************************************************************/
void CalcSpiralPoint(PNTTYPE pnt, int pln, double dmt, double hgt,
                     int cno, int sno, PNTTYPE* pa, int* ia)			//<<< ��������_�v�Z
/******************************************************************/
{
	int    i, d1, d2, d3, cn;
	double rd, hg, shg, th, sth=0.0;

	d1 = pln;															// ���W���ݒ�
	d2 = (d1+1>2) ? 0 : d1+1;
	d3 = (d2+1>2) ? 0 : d2+1;
	cn = cno*sno+1;														// ������_��
	rd = dmt/2.0;														// ���a
	hg = hgt/cno;														// 1����_�̍���
	shg = pnt.p[d3]-(hgt*sno/2.0);										// �J�n�ʒu�̍���
	th = 2.0*PI/cno;													// 1����_�̉�]�p�x
	for(i=0; i<cn; i++) {
		pa[i].p[d1] = pnt.p[d1]+rd*cos(sth);							// ����_�z��쐬
		pa[i].p[d2] = pnt.p[d2]+rd*sin(sth);
		pa[i].p[d3] = shg;
		ia[i] = 0;														// �Ȑ�Ӱ�ސݒ�
		sth += th, shg += hg;											// ���a,�������Z
	}
	ia[0] = 1, ia[cn-1] = 1;											// �n�_��I�_�͐ܐ��_
}

/*********************************************************************/
static void TrackerCurvePoint(int bc, int an, PNTTYPE* pa, int* ia,
                              int* ic, int* bn, PNTTYPE* pb, int* ib)	//<<< �Ȑ��ҏW�ׯ���擾���
/*********************************************************************/
{
	int i, j;

	for(i=0; i<an; i++) {
		pb[i*2] = pa[i];												// ����_�ׯ���ݒ�
		ib[i*2] = (ic[i]==0) ? ia[i]*2 : 4;								// �F�ݒ�
	}
	for(i=1; i<an; i++) {
		for(j=0; j<DIM; j++) {
			pb[(i-1)*2+1].p[j] = (pa[i-1].p[j]+pa[i].p[j])/2.0;			// ���ԓ_�ׯ���ݒ�
		}
		ib[(i-1)*2+1] = 5;												// �F�ݒ�(���ԓ_)
	}
	if(bc) {															// <�Ȑ�(��)�̏ꍇ>
		for(j=0; j<DIM; j++) {
			pb[an*2-1].p[j] = (pa[0].p[j]+pa[an-1].p[j])/2.0;			// �n�_�ƏI�_�̒��ԓ_�ǉ�
		}
		ib[an*2-1] = 5;													// �F�ݒ�(���ԓ_)
		*bn = an*2;														// �ׯ����
	} else {															// <�Ȑ�(�J)�̏ꍇ>
		*bn = an*2-1;													// �ׯ����
	}
}

/*******************************************************************/
void TrackerECurve(int md, double* da, int bc, int an, PNTTYPE* pa,
                   int* ia, int* ic, int* bn, PNTTYPE* pb, int* ib)		//<<< �Ȑ��ҏW�ׯ���擾
/*******************************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];

	CalcCurvePoint(md, an, pa, pw, da);									// ����_=>�ʉߓ_�ϊ�
	TrackerCurvePoint(bc, an, pw, ia, ic, bn, pb, ib);					// �ׯ�����_�擾
	delete[] pw;
}

/************************************************************************/
void DeleteCurvePoint(int md, double* da, double* db, int bc, double wt,
                      int* no, int an, PNTTYPE* pa, int* ia, int* bn)	//<<< ����_��ʉߓ_�폜
/************************************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	int      i, ct;

	CalcCurvePoint(md, an, pa, pw, da);									// ����_=>�ʉߓ_�ϊ�
	for(i=0, ct=0; i<an; i++) {
		if(no[i]!=1) {													// ����_/Ӱ�ލĐݒ�
			pw[ct] = pw[i], ia[ct] = ia[i], no[ct++] = 0;
		}
	}
	SetCurveMatrix3(md, wt, bc, ct, ia, da, db);						// ����_<=>�ʉߓ_�ϊ���د��
	CalcCurvePoint(md, ct, pw, pa, db);									// �ʉߓ_=>����_�ϊ�
	*bn = ct;
	delete[] pw;
}

/***********************************************************/
static void ChangeCurveMode(int md, double* da, double* db,
                            int bc, double wt, int no,
                            int an, PNTTYPE* pa, int* ia)				//<<< �Ȑ�Ӱ�ޔ��]
/***********************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];

	CalcCurvePoint(md, an, pa, pw, da);									// ����_=>�ʉߓ_�ϊ�
	ia[no] = (ia[no]) ? 0 : 1;											// �ܐ�/�Ȑ����]
	SetCurveMatrix3(md, wt, bc, an, ia, da, db);						// ����_<=>�ʉߓ_�ϊ���د��
	CalcCurvePoint(md, an, pw, pa, db);									// �ʉߓ_=>����_�ϊ�
	delete[] pw;
}

/*****************************************************************/
static void AddCurvePoint(int md, double* da, double* db, int bc,
                          int rp, double wt, int no, int* ic,
                          int sft, int an, PNTTYPE* pa, int* ia)		//<<< ����_��ʉߓ_�ǉ�
/*****************************************************************/
{
	PNTTYPE* pw = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* px = new PNTTYPE[View->m_MaxSCtA];
	int*     ix = new int[View->m_MaxSCtA];
	PNTTYPE  pnt;
	int      i, j, pn, ut, wk;

	if(md) {															// <�ʉߓ_�w��̏ꍇ>
		pn = an;														// ����_��
		for(ut=1, i=0; i<rp-1; i++) {
			ut *= 2;													// �����ɂ�葝���钸�_��
		}
		for(i=0; i<pn; i++) {
			px[i] = pa[i], ix[i] = ia[i];								// ����_,�Ȑ�Ӱ�ޕ���
		}
		wk = MakeSCurve(&pn, bc, rp, wt, px, ix);						// �Ȑ���
		for(i=0; i<an; i++) {
			px[i] = px[2*ut*i+ut];										// ���ԓ_�����𒊏o
		}
		for(i=0; i<wk; i++) {											// ��Đ����߂�
			pnt = px[an-1];												// �Ō�����ް��ۑ�
			for(j=an-1; j>0; j--) {
				px[j] = px[j-1];										// ��ּ��
			}
			px[0] = pnt;												// �Ō���ް���擪��
		}
		CalcCurvePoint(md, an, pa, pw, da);								// ����_=>�ʉߓ_�ϊ�
		for(i=an-1; i>no; i--) {
			pw[i+1] = pw[i], ia[i+1] = ia[i], ic[i+1] = ic[i];			// �w��_�������
		}
		no++;															// �ǉ��ʒu�ԍ�
		pw[no] = px[no-1];												// ���Ԃ̒ʉߓ_�ǉ�
		ia[no] = (sft) ? 1 : 0;											// Shift�ܐ��_Ӱ��
		SetCurveMatrix3(md, wt, bc, an+1, ia, da, db);					// ����_<=>�ʉߓ_�ϊ���د��
		CalcCurvePoint(md, an+1, pw, pa, db);							// �ʉߓ_=>����_�ϊ�
	} else {															// <����_�w��̏ꍇ>
		for(i=an-1; i>no; i--) {
			pa[i+1] = pa[i], ia[i+1] = ia[i], ic[i+1] = ic[i];			// �w��_�������
		}
		no++;															// �ǉ��ʒu�ԍ�
		if(no==an) {													// <���E:�̍Ō�>
			for(i=0; i<DIM; i++) {
				pa[no].p[i] = (pa[no-1].p[i]+pa[0].p[i])/2.0;			// ����_�ݒ�
			}
		} else {														// <���Ԃ̐���_>
			for(i=0; i<DIM; i++) {
				pa[no].p[i] = (pa[no-1].p[i]+pa[no+1].p[i])/2.0;		// ����_�ݒ�
			}
		}
		ia[no] = (sft) ? 1 : 0;											// Shift�ܐ��_Ӱ��
	}
	delete[] pw;
	delete[] px;
	delete[] ix;
}

/****************************************************************/
void PickECurve(int pn, int* pk, int md, double* da, double* db,
                int* an, int bc, int rp, double wt, int* tn,
                OBJTYPE* op, int sf, PNTTYPE* pa, PNTTYPE* pb,
                int* ia, int* ib, int* ic)								//<<< �Ȑ��ҏW �_�I��
/****************************************************************/
{
	int i, j, pf=0;

	for(i=0; i<pn; i++) {
		if(pk[i]%2==0) {												// <����_�I���̏ꍇ>
			j = pk[i]/2;												// ����_�ԍ��擾
			ic[j] = (ic[j]) ? 0 : 1;									// �I����Ԕ��]
			if(sf) {
				ChangeCurveMode(md, da, db, bc, wt, j, *an, pa, ia);	// �Ȑ�Ӱ�ޔ��]
			}
			pf = 1;														// ����_�I���ς�
		}
	}
	if(!pf) {															// <<����_���I���̏ꍇ>>
		for(i=pn-1; i>=0; i--) {
			if(pk[i]%2!=0) {											// <����_�ǉ��̏ꍇ>
				j = (pk[i]-1)/2;										// �ǉ��ʒu���O�ԍ�
				AddCurvePoint(md, da, db, bc, rp, wt, j,
                              ic, sf, *an, pa, ia);						// ����_��ʉߓ_�ǉ�
				ic[++j] = 1;											// �Ώې���_�I��
				(*an)++;												// ����_�����Z
			}
		}
	}
	RedoChgCurve(*an, bc, pa, ia);										// �Ȑ�����
	TrackerECurve(md, da, bc, *an, pa, ia, ic, tn, pb, ib);				// �Ȑ��ҏW�ׯ���擾
	RedoSetCurve(*tn, bc, pb);											// �Ȑ�����_����
	for(i=0; i<*tn; i++) {
		GetVtx(op, i, &pb[i]);											// ���̕ҏW�㐧��_�擾
	}
}

/*****************************************************************/
static void MoveCurvePoint(OBJTYPE* op, PNTTYPE opt, PNTTYPE npt,
                           int mx, int fg, PNTTYPE* pc)					//<<< ����_�ړ�
/*****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	CMDTYPE* cmp;
	PNTTYPE  wpt, spt, p1, p2, p3;
	int      i, i1, i2, i3, i4, i5, i6, i7, wf, ct, cno;
	double   d1, d2, d3, d4, d5, d6, d7, d8, d9;

	if(fg==1) {															// <�ړ��Ώۂ̏ꍇ>
		while(mx>0) {
			wpt = opt;													// ���̕ҏW�O����_
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			NextCmdPtr(&cmp);											// �Ȑ��ҏW������
			ct = 0;														// ����޶���������
			while(cmp!=NULL) {
				if(ct>mx-1) {											// ����ތ��ȏ㒆�~
					break;
				}
				wf = (ct==mx-1) ? 1 : 0;								// �w����ڍX�V�L
				cno = GetCmdNo(cmp);									// �����No�擾
				if(cno==MOVE) {											// <���s�ړ��̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����ړ��ʎ擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����ړ��ʎ擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����ړ��ʎ擾
					BackMove(wf, &wpt, &npt, d1, d2, d3);				// �t���s�ړ����s
				} else if(cno==ROTATE) {								// <��]�ړ��̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// ��]�p�x�擾
					GetParaPnt(cmp, 0, &p1);							// ��]���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ��]���޸�َ擾
					BackRotate(wf, &wpt, &npt, p1, p2, d1);				// �t��]�ړ����s
				} else if(cno==MIRROR) {								// <�����ړ��̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �Ώە��ʎ擾
					GetParaPnt(cmp, 0, &p1);							// �������_�擾
					GetParaPnt(cmp, 1, &p2);							// �������_�擾
					BackMirror(wf, &wpt, &npt, p1, p2, i1);				// �t�����ړ����s
				} else if(cno==SCALE) {									// <���ޕύX�̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����{���擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����{���擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����{���擾
					GetParaPnt(cmp, 0, &p1);							// ���ޕύX���S�_�擾
					BackScale(wf, &wpt, &npt, p1, d1, d2, d3);			// �t���ޕύX���s
				} else if(cno==RESIZE1) {								// <�����w��ػ���1�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �w�蕽�ʎ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// �{��1�擾
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// �{��2�擾
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// �{��3�擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					GetParaPnt(cmp, 1, &p2);							// ��_2�擾
					GetParaPnt(cmp, 2, &p3);							// ��_3�擾
					BackResize1(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                p1, p2, p3);							// �t�����w��ػ���1���s
				} else if(cno==RESIZE2) {								// <�Ȑ��w��ػ���1�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �����׸ގ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					i3 = GetParaInt(cmp, 2);							// ���������擾
					i4 = GetParaInt(cmp, 3);							// ���ގw������擾
					d1 = (double)GetParaInt(cmp, 4)/1000.0;				// ������擾
					i5 = GetParaInt(cmp, 5);							// ����_���擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					for(i=0; i<i5; i++) {
						ia[i] = GetParaInt(cmp, i+6);					// �Ȑ�Ӱ�ގ擾
						GetParaPnt(cmp, i+1, &pa[i]);					// �Ȑ�����_�擾
					}
					BackResize2(wf, &wpt, &npt, i1, i2, i3, i4, d1,
                                p1, i5, pa, ia);						// �t�Ȑ��w��ػ���1���s
				} else if(cno==BEND) {									// <�~���Ȃ��̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �Ȃ����������擾
					i2 = GetParaInt(cmp, 1);							// �Ȃ����������擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// �Ȃ��p�x�擾
					GetParaPnt(cmp, 0, &p1);							// ����_�擾
					GetParaPnt(cmp, 1, &p2);							// ����_�擾
					BackBend(wf, &wpt, &npt, i1, i2, d1, p1, p2);		// �t�~���Ȃ����s
				} else if(cno==TWIST) {									// <�Ђ˂�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �Ђ˂莲�����擾
					d1 = (double)GetParaInt(cmp, 1)/1000.0;				// �Ђ˂�p�x�擾
					d2 = (double)GetParaInt(cmp, 2)/1000.0;				// �Ђ˂莲���擾
					GetParaPnt(cmp, 0, &p1);							// �Ђ˂��_�擾
					BackTwist(wf, &wpt, &npt, i1, d1, d2, p1);			// �t�Ђ˂���s
				} else if(cno==SHEAR) {									// <����f�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// ����f���������擾
					i2 = GetParaInt(cmp, 1);							// ����f���������擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// ����f�X���擾
					GetParaPnt(cmp, 0, &p1);							// ����_�擾
					BackShear(wf, &wpt, &npt, i1, i2, d1, p1);			// �t����f���s
				} else if(cno==RING) {									// <�ݸނ̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����{���擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����{���擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����{���擾
					i1 = GetParaInt(cmp, 3);							// �Ȃ����������擾
					i2 = GetParaInt(cmp, 4);							// �Ȃ����������擾
					d4 = (double)GetParaInt(cmp, 5)/1000.0;				// �Ȃ��p�x�擾
					GetParaPnt(cmp, 0, &p1);							// ���ޕύX���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ����_�擾
					GetParaPnt(cmp, 2, &p3);							// ����_�擾
					if(wf==0) {											// <�X�V�ΏۊO>
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// �t���ޕύX���s(�X�V��)
						BackBend(0, &wpt, &npt, i1, i2, d4, p2, p3);	// �t�~���Ȃ����s(�X�V��)
					} else {											// <�X�V�Ώ�>
						spt = wpt;										// ����_�ۑ�
						BackScale(0, &wpt, &npt, p1, d1, d2, d3);		// �t���ޕύX���s(�X�V��)
						BackBend(1, &wpt, &npt, i1, i2, d4, p2, p3);	// �t�~���Ȃ����s(�X�V�L)
						wpt = spt;										// ����_����
						BackScale(1, &wpt, &npt, p1, d1, d2, d3);		// �t���ޕύX���s(�X�V�L)
					}
				} else if(cno==EARTH) {									// <����̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����ړ��ʎ擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����ړ��ʎ擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����ړ��ʎ擾
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X����]�p�x�擾
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y����]�p�x�擾
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z����]�p�x�擾
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X�����{���擾
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y�����{���擾
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z�����{���擾
					GetParaPnt(cmp, 0, &p1);							// ��]���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ���ޕύX���S�_�擾
					if(wf==0) {											// <�X�V�ΏۊO>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V��)
					} else {											// <�X�V�Ώ�>
						spt = wpt;										// ����_�ۑ�
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V�L)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V�L)
					}
				} else if(cno==SPACE) {									// <��߰��̏ꍇ>
					d1 = (double)GetParaInt(cmp, 0)/1000.0;				// X�����ړ��ʎ擾
					d2 = (double)GetParaInt(cmp, 1)/1000.0;				// Y�����ړ��ʎ擾
					d3 = (double)GetParaInt(cmp, 2)/1000.0;				// Z�����ړ��ʎ擾
					d4 = (double)GetParaInt(cmp, 3)/1000.0;				// X����]�p�x�擾
					d5 = (double)GetParaInt(cmp, 4)/1000.0;				// Y����]�p�x�擾
					d6 = (double)GetParaInt(cmp, 5)/1000.0;				// Z����]�p�x�擾
					d7 = (double)GetParaInt(cmp, 6)/1000.0;				// X�����{���擾
					d8 = (double)GetParaInt(cmp, 7)/1000.0;				// Y�����{���擾
					d9 = (double)GetParaInt(cmp, 8)/1000.0;				// Z�����{���擾
					GetParaPnt(cmp, 0, &p1);							// ��]���S�_�擾
					GetParaPnt(cmp, 1, &p2);							// ���ޕύX���S�_�擾
					if(wf==0) {											// <�X�V�ΏۊO>
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(0, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V��)
					} else {											// <�X�V�Ώ�>
						spt = wpt;										// ����_�ۑ�
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						BackMove(1, &wpt, &npt, d1, d2, d3);			// �t���s�ړ����s(�X�V�L)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = 0.0, p3.p[2] = -1.0;	// Z���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d6);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(0, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						p3.p[0] = 0.0, p3.p[1] = -1.0, p3.p[2] = 0.0;	// Y���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d5);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(0, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V��)
						p3.p[0] = -1.0, p3.p[1] = 0.0, p3.p[2] = 0.0;	// X���p��]�޸��
						BackRotate(1, &wpt, &npt, p1, p3, d4);			// �t��]�ړ����s(�X�V��)
						wpt = spt;										// ����_����
						BackScale(1, &wpt, &npt, p2, d7, d8, d9);		// �t���ޕύX���s(�X�V�L)
					}
				} else if(cno==RESIZE3) {								// <�����w��ػ���2�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �w�蕽�ʎ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					d1 = (double)GetParaInt(cmp, 2)/1000.0;				// �{��1�擾
					d2 = (double)GetParaInt(cmp, 3)/1000.0;				// �{��2�擾
					d3 = (double)GetParaInt(cmp, 4)/1000.0;				// �{��3�擾
					i3 = GetParaInt(cmp, 5);							// ���������׸ގ擾
					d4 = (double)GetParaInt(cmp, 6)/1000.0;				// �����͈�1�擾
					d5 = (double)GetParaInt(cmp, 7)/1000.0;				// �����͈�2�擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					GetParaPnt(cmp, 1, &p2);							// ��_2�擾
					GetParaPnt(cmp, 2, &p3);							// ��_3�擾
					BackResize3(wf, &wpt, &npt, i1, i2, d1, d2, d3,
                                i3, d4, d5, p1, p2, p3);				// �t�����w��ػ���2���s
				} else if(cno==RESIZE4) {								// <�Ȑ��w��ػ���2�̏ꍇ>
					i1 = GetParaInt(cmp, 0);							// �����׸ގ擾
					i2 = GetParaInt(cmp, 1);							// �����׸ގ擾
					i3 = GetParaInt(cmp, 2);							// ���������擾
					i4 = GetParaInt(cmp, 3);							// ���ގw������擾
					i5 = GetParaInt(cmp, 4);							// ���������擾
					d1 = (double)GetParaInt(cmp, 5)/1000.0;				// ������擾
					i6 = GetParaInt(cmp, 6);							// ���������׸ގ擾
					d2 = (double)GetParaInt(cmp, 7)/1000.0;				// �����͈�1�擾
					d3 = (double)GetParaInt(cmp, 8)/1000.0;				// �����͈�2�擾
					i7 = GetParaInt(cmp, 9);							// ����_���擾
					GetParaPnt(cmp, 0, &p1);							// ��_1�擾
					for(i=0; i<i7; i++) {
						ia[i] = GetParaInt(cmp, i+10);					// �Ȑ�Ӱ�ގ擾
						GetParaPnt(cmp, i+1, &pa[i]);					// �Ȑ�����_�擾
					}
					BackResize4(wf, &wpt, &npt, i1, i2, i3, i4, i5,
                                d1, i6, d2, d3, p1, i7, pa, ia);		// �t�Ȑ��w��ػ���2���s
				}
				ct++;													// ����޶���UP
				NextCmdPtr(&cmp);										// ������ނ�
			}
			mx--;														// ����ތ����Z
		}
		*pc = npt;
	} else {															// <�ړ��ΏۊO�̏ꍇ>
		*pc = opt;
	}
	delete[] pa;
	delete[] ia;
}

/***********************************************************/
void MoveECurve(int md, double* db, PNTTYPE* ma,
                OBJTYPE* op, int pn, int bc, PNTTYPE* pa,
                PNTTYPE* pb, int* ia, PNTTYPE* pc, int* ta)				//<<< ����_��ʉߓ_�ړ�
/***********************************************************/
{
	PNTTYPE* mov = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxCCtA];
	int*     flg = new int[View->m_MaxCCtA];
	double*  len = new double[View->m_MaxCCtA];
	CMDTYPE* cmp;
	PNTTYPE  opt, npt;
	int      i, j, k, ten, p1, p2, sct, ect, sno, eno, mx=0, lst, ok;
	double   sln, wln;

	ten = View->GetSurfaceTension();									// ���͐ݒ�ԍ��擾
	BaseCmdPtr(1, op, &cmp);											// �L���擪����ގ擾
	NextCmdPtr(&cmp);													// �Ȑ��ҏW������
	while(cmp!=NULL) {
		mx++;															// ����ތ�����
		NextCmdPtr(&cmp);												// ������ނ�
	}
	opt.p[0] = 0.0, opt.p[1] = 0.0, opt.p[2] = 0.0;						// ���W�l:0�z��쐬
	for(i=0; i<pn; i++) {
		if(ia[i]) {
			mov[i] = ma[i], flg[i] = 1;									// �ړ���/�׸ޔz�񕡎�
		} else {
			mov[i] = opt,   flg[i] = 0;
		}
	}
	if(ten>0) {															// <<���͐ݒ�L�̏ꍇ>>
		for(i=0, j=0, p1=-1, p2=-1; i<pn-1; i++) {
			if(flg[i]!=flg[i+1]) {										// <�O���׸ވ�>
				j++;													// �w��_���E����
				if(flg[i]) {											// ���E1�ʒu
					p1 = i+1;
				} else {												// ���E2�ʒu
					p2 = i;
				}
			}
		}
		if(j<=2) {														// <�w��_�A��>
			for(i=0; i<pn; i++) {										// ����_�ԋ�������
				len[i] = (i==pn-1) ? PntD(&pb[i*2], &pb[0]) :
                                     PntD(&pb[i*2], &pb[(i+1)*2]);
			}
			if(p1==-1) {												// �w��_�O��ʒu&�_���ݒ�
				if(bc) {
					sct = (p2+1)/2, ect = (p2+1)/2, sno = p2, eno = pn-1;
				} else {
					sct = p2+1, ect = 0, sno = p2, eno = pn-1;
				}
			} else if(p2==-1) {
				if(bc) {
					sct = (pn-p1)/2, ect = (pn-p1)/2, sno = pn-1, eno = p1-1;
				} else {
					sct = 0, ect = pn-p1, sno = pn-1, eno = p1-1;
				}
			} else if(p1>p2) {
				if(bc) {
					sct = (p2+1+pn-p1)/2, ect = (p2+1+pn-p1)/2, sno = p2, eno = p1-1;
				} else {
					sct = p2+1, ect = pn-p1, sno = p2, eno = p1-1;
				}
			} else {
				sct = (p2-p1+1)/2, ect = (p2-p1+1)/2, sno = p2, eno = p1-1;
			}
			for(i=0, j=sno, lst=-1; i<sct; i++) {						// <�w��_�O����_�ړ�>
				lst = (ta[j]) ? j : lst;								// �ŏI���͗L���_����
				j = (j-1<0) ? pn-1 : j-1;
			}
			ok = (lst==-1) ? 0 : 1;										// ���͗L���_�L��
			lst = (lst-1<0) ? pn-1 : lst-1;								// �L���_�O�������܂�
			for(i=0, sln=0.0, j=sno; i<sct; i++) {
				sln = (ok) ? sln+len[j] : sln;							// ����_�ԋ����W�v
				ok = (j==lst) ? 0 : ok;									// �ŏI���͗L���_�ȍ~�͖���
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, wln=0.0, j=sno; i<sct; i++) {
				wln += len[j];											// �w�萧��_����̋���
				if(ta[j]) {												// <���͗L���_>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// ���͐ݒ�l�ϊ�
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// �w���׸�ON
					}
				}
				j = (j-1<0) ? pn-1 : j-1;
			}
			for(i=0, j=eno, lst=-1; i<ect; i++) {						// <�w��_�㐧��_�ړ�>
				lst = (ta[j]) ? j : lst;								// �ŏI���͗L���_����
				j = (j+1>pn-1) ? 0 : j+1;
			}
			ok = (lst==-1) ? 0 : 1;										// ���͗L���_�L��
			for(i=0, sln=0.0, j=eno; i<ect; i++) {
				k = (j+1>pn-1) ? 0 : j+1;
				sln = (ok||ta[k]) ? sln+len[j] : sln;					// ����_�ԋ����W�v
				ok = (j==lst) ? 0 : ok;									// �ŏI���͗L���_�ȍ~�͖���
				j = (j+1>pn-1) ? 0 : j+1;
			}
			for(i=0, wln=0.0, j=eno; i<ect; i++) {
				wln += len[j];											// �w�萧��_����̋���
				j = (j+1>pn-1) ? 0 : j+1;
				if(ta[j]) {												// <���͗L���_>
					mov[j] = ma[j];
					GetTensionPntValue(ten, sln, wln, &mov[j]);			// ���͐ݒ�l�ϊ�
					if(fabs(mov[j].p[0])>EPSILON||
                       fabs(mov[j].p[1])>EPSILON||
                       fabs(mov[j].p[2])>EPSILON) {
						flg[j] = 1;										// �w���׸�ON
					}
				}
			}
		}
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mov[i].p[j] += pb[i*2].p[j];								// ���̕ҏW&�ړ�����W
		}
	}
	CalcCurvePoint(md, pn, mov, cpt, db);								// �ʉߓ_=>����_�ϊ�
	if(md) {
		for(i=0; i<pn; i++) {
			flg[i] = 1;													// �ʉߓ_�͑S�Ĉړ��ς�
		}
	}
	for(i=0; i<pn; i++) {
		opt = pa[i], npt = cpt[i];										// �ړ��O/�ړ��� ����_
		MoveCurvePoint(op, opt, npt, mx, flg[i], &pc[i]);				// ����_�ړ�
	}
	delete[] mov;
	delete[] cpt;
	delete[] flg;
	delete[] len;
}

/************************************************************/
BOOL ExtractionCurve(int un, int vn, int ub, int vb, int f1,
                     int f2, int no, PNTTYPE* pa, int* ia,
                     int* pn, int* bc, PNTTYPE* pb, int*ib)				//<<< �Ȑ����o
/************************************************************/
{
	int i, wuv, wua, wva, flg, lno;
	BOOL ret=TRUE;

	wva = (f1%2==1) ? 1 : 0;											// U/V���E�ƕ␳���擾
	wua = (f2%2==1) ? 1 : 0;
	wuv = (f1%2==1) ? vn-1 : vn;
	wuv = (f1>=2) ? wuv-1 : wuv;
	if(no<wuv) {														// <Vײ�>
		flg = 1;
		lno = no+wva;
	} else {															// <Uײ�>
		flg = 0;
		lno = no-wuv+wua;
	}
	if(flg==0) {														// <<Uײݎw�莞>>
		*pn = vn;														// �Ȑ�����_��
		*bc = vb;														// �Ȑ����E����
		if(vn>View->m_MaxCCtl) {										// <����_����ȏ�>
			AfxMessageBox(IDS_MANY_CTRLP);								// �װү����
			ret = FALSE;
		} else if(vn<3) {												// <����_�����ȉ�>
			AfxMessageBox(IDS_FEW_CTRLP);								// �װү����
			ret = FALSE;
		}
		if(ret) {
			for(i=0; i<vn; i++) {
				pb[i] = pa[lno+(i*un)];									// ����_
				ib[i] = ia[lno+(i*un)]/2;								// �Ȑ�Ӱ��
			}
		}
	} else {															// <<Vײݎw�莞>>
		*pn = un;														// �Ȑ�����_��
		*bc = ub;														// �Ȑ����E����
		if(un>View->m_MaxCCtl) {										// <����_����ȏ�>
			AfxMessageBox(IDS_MANY_CTRLP);								// �װү����
			ret = FALSE;
		} else if(un<3) {												// <����_�����ȉ�>
			AfxMessageBox(IDS_FEW_CTRLP);								// �װү����
			ret = FALSE;
		}
		if(ret) {
			for(i=0; i<un; i++) {
				pb[i] = pa[(lno*un)+i];									// ����_
				ib[i] = ia[(lno*un)+i]%2;								// �Ȑ�Ӱ��
			}
		}
	}
	return ret;
}
