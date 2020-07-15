/////////////////////////////////////////////////////////////////////////////
// �R�}���h�֘A �}�`���Z��
// RCmdBoolean.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include <limits.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "BooleanType.h"
#include "NumFunc.h"
CProgressDlg* m_pProgressB=NULL;

/******************************/
static void InitWorkArea(void)											//<<< �}�`���Z��������
/******************************/
{
	Aflist = NULL, Bflist = NULL, A0list = NULL;
	A1list = NULL, B0list = NULL, B1list = NULL;
	Afcnt = 0, Bfcnt = 0, A0cnt = 0;
	A1cnt = 0, B0cnt = 0, B1cnt = 0;
}

/********************************************************/
static BOOL InPointSub(int d, PNTTYPE cp, PNTTYPE* mi,
                       PNTTYPE* pa, int pn, PNTTYPE* ip)				//<<< �}�`�����ݓ_�擾���
/********************************************************/
{
	PNTTYPE* pt = new PNTTYPE[100];
	PNTTYPE* pp = new PNTTYPE[100];
	double*  ln = new double[100];
	PNTTYPE  p1, p2, la;
	int      i, j, k, l, ct=0, ret=FALSE;
	double   m, lb;

	for(i=0; i<pn; i++) {
		j = (i==pn-1) ? 0 : i+1;
		if(pa[i].p[d]<pa[j].p[d]) {										// d���W������p1,�傪p2
			p1 = pa[i], p2 = pa[j];
		} else {
			p1 = pa[j], p2 = pa[i];
		}
		if(p1.p[d]<=cp.p[d]&&p2.p[d]>cp.p[d]&&ct<100) {					// <���ԓ_��ʂ����>
			for(k=0; k<DIM; k++) {
				la.p[k] = p2.p[k]-p1.p[k];								// �����̒���
			}
			lb = (cp.p[d]-p1.p[d])/la.p[d];								// �J�n�_����ʒ��S�_�܂ł̔䗦
			for(k=0; k<DIM; k++) {
				pt[ct].p[k] = p1.p[k]+lb*la.p[k];						// �����̖ʒ��S�_�ʒu�̍��W
			}
			ct++;
		}
	}
	if(ct==2) {															// <�L�����̏ꍇ>
		for(i=0; i<DIM; i++) {
			ip->p[i] = (pt[0].p[i]+pt[1].p[i])/2.0;						// 2�_�̒��S�_
		}
		ret = TRUE;
	} else if(ct%2==0) {												// <�����̏ꍇ>
		for(i=0; i<ct; i++) {
			ln[i] = PntD(mi, &pt[i]);									// �ŏ��_����̋���
		}
		for(i=0, k=0; i<ct; i++) {
			for(j=0, l=0, lb=0.0; j<ct; j++) {
				if(lb<ln[j]) {											// �L���ȍŒ������_
					lb = ln[j], l = j;
				}
			}
			pp[k++] = pt[l], ln[l] = -10.0;								// �Œ��������ɕ���
		}
		for(i=0, l=0, lb=0.0; i<ct/2; i++) {
			m = PntD(&pp[2*i], &pp[2*i+1]);								// 2�_���̋���
			if(lb<m) {													// �Œ������_�ۑ�
				lb = m, l = i;
			}
		}
		for(i=0; i<DIM; i++) {
			ip->p[i] = (pp[2*l].p[i]+pp[2*l+1].p[i])/2.0;				// 2�_�̒��S�_
		}
		ret = TRUE;
	}
	delete[] pt;
	delete[] pp;
	delete[] ln;
	return ret;
}

/**************************************************************/
static void CalcInPoint(PNTTYPE* pa, int pn,
                        PNTTYPE* mi, PNTTYPE* ma, PNTTYPE* ip)			//<<< �}�`�����ݓ_�擾
/**************************************************************/
{
	PNTTYPE size, cp;
	int     i, j, d1, d2, d3;
	double  dp;

	for(i=0; i<DIM; i++) {												// X,Y,Z�����l�ݒ�
		mi->p[i] = DBL_MAX, ma->p[i] = 1.0-DBL_MAX;
	}
	for(i=0; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			if(mi->p[j]>pa[i].p[j]) {									// �ʍŏ��l�擾
				mi->p[j] = pa[i].p[j];
			}
			if(ma->p[j]<pa[i].p[j]) {									// �ʍő�l�擾
				ma->p[j] = pa[i].p[j];
			}
		}
	}
	for(i=0; i<DIM; i++) {
		size.p[i] = ma->p[i]-mi->p[i];									// �ʂ̻���
		cp.p[i] = (ma->p[i]+mi->p[i])/2.0;								// �ʂ̒��S�_
	}
	for(i=0, d1=0, dp=0.0; i<DIM; i++) {
		if(dp<size.p[i]) {												// �ő廲�ތ���
			dp = size.p[i], d1 = i;
		}
	}
	size.p[d1] = -10.0;
	for(i=0, d2=0, dp=0.0; i<DIM; i++) {
		if(dp<size.p[i]) {												// �����ތ���
			dp = size.p[i], d2 = i;
		}
	}
	if(d1!=0&&d2!=0) {													// �ŏ����ތ���
		d3 = 0;
	} else if(d1!=1&&d2!=1) {
		d3 = 1;
	} else {
		d3 = 2;
	}
	if(!InPointSub(d1, cp, mi, pa, pn, ip)) {							// �ʓ����ݓ_�擾���(1st)
		if(!InPointSub(d2, cp, mi, pa, pn, ip)) {						// �ʓ����ݓ_�擾���(2nd)
			if(!InPointSub(d3, cp, mi, pa, pn, ip)) {					// �ʓ����ݓ_�擾���(3rd)
				*ip = pa[0];											// 3��Ƃ�NG��1�_�ڂ��g�p
				ErrFlg1 = 1;											// �װ�׸�ON
			}
		}
	}
}

/***************************************************************/
static void SetFaData(int flg, double x1, double x2, double y1,
                      double y2, double z1, double z2,
                      VECTYPE vc, PNTTYPE ip, int st)					//<<< �ʏ���ް��o�^
/***************************************************************/
{
	BFATYPE* tf;

	tf = (BFATYPE *)malloc(sizeof(BFATYPE));							// �ʏ��̈�m��
	if(tf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tf->xmin = x1, tf->xmax = x2;
	tf->ymin = y1, tf->ymax = y2;
	tf->zmin = z1, tf->zmax = z2;
	tf->vec  = vc, tf->inpt = ip, tf->stfl = st;
	if(!flg) {															// <A��>
		tf->next = Aflist, Aflist = tf, Afcnt++;
	} else {															// <B��>
		tf->next = Bflist, Bflist = tf, Bfcnt++;
	}
}

/************************************************/
static BFATYPE* MergeF(BFATYPE* la, BFATYPE* lb)						//<<< X�ŏ��l���
/************************************************/
{
	BFATYPE* lc, * ld, * rc;

	lc = (BFATYPE *)malloc(sizeof(BFATYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->xmin<=lb->xmin) {										// X�ŏ��l�̑召�Ŕ�r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/************************************************/
static BFATYPE* F_Mergsrt(BFATYPE* mlist, int n)						//<<< X�ŏ��l�̿��
/************************************************/
{
	BFATYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BFATYPE *)MergeF(F_Mergsrt(la, n/2),
                               F_Mergsrt(lb, n-n/2));					// ��ٰ�ߔԍ����
		rc = lm;
	}
	return rc;
}

/**********************************************/
static void MakeFaceList(int flg, OBJTYPE* op)							//<<< �ʏ��ؽč쐬
/**********************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE  pt, mi, ma, ip;
	VECTYPE  vc;
	int      i, num, ffg, st, pn, ipr=0;
	double   dpr;

	num = GetFlpNum1(op);												// ��ٰ�ߐ��擾
	for(i=0, st=-1; i<num; i++) {
		GetFlpVtx1(op, i, &pt, &ffg);									// ��ٰ�ߒ��_�擾
		if(st==-1) {													// ��ٰ�ߊJ�n����
			pn = 0, st = i;
		}
		pa[pn++] = pt;													// ���_���W�ۑ�
		if(ffg) {														// <<��ٰ�ߏI��>>
			CalcNVec(pa, pn, &vc);										// �@���޸�َ擾
			CalcInPoint(pa, pn, &mi, &ma, &ip);							// �ʓ����ݓ_�擾
			SetFaData(flg, mi.p[0], ma.p[0], mi.p[1], ma.p[1],
                      mi.p[2], ma.p[2], vc, ip, st);					// �ʏ���ް��o�^
			st = -1;													// ٰ�ߊJ�n�׸ޏ�����
		}
		dpr = (Ppr/6.0)*((double)i/(double)num);						// �����߰���
		if(ipr<(int)(Spr+dpr)) {										// <�߰���UP>
			ipr = (int)(Spr+dpr);
			m_pProgressB->SetProgress(ipr);								// �i���\��
		}
	}
	Spr += Ppr/6.0;														// �ݐ��߰���
	if(!flg) {															// A��X�ŏ��l�̿��
		Aflist = F_Mergsrt(Aflist, Afcnt);
	} else {															// B��X�ŏ��l�̿��
		Bflist = F_Mergsrt(Bflist, Bfcnt);
	}
	delete[] pa;
}

/*************************************************************/
static void SetFcData(int flg, int sno, int eno, PNTTYPE pnt)			//<<< �ʌ�_�Z�o�ް��o�^
/*************************************************************/
{
	BFCTYPE* tf;
	int      wno;

	tf = (BFCTYPE *)malloc(sizeof(BFCTYPE));							// �ʌ�_�Z�o�̈�m��
	if(tf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tf->flg = flg;
	if(sno<eno) {
		tf->sno = sno, tf->eno = eno;
	} else {
		tf->sno = eno, tf->eno = sno;
	}
	if((!flg)&&tf->eno-tf->sno>1) {										// <��ٰ�߂Ŏn�I�_>
		wno = tf->sno, tf->sno = tf->eno, tf->eno = wno;				// �I�_���n�_�ɖ߂�
	}
	tf->pnt = pnt;
	tf->next = Fclist, Fclist = tf;
}

/*********************************************************/
static BFCTYPE* MergeFc(BFCTYPE* la, BFCTYPE* lb, int id)				//<<< �ʌ�_�Z�o���
/*********************************************************/
{
	BFCTYPE* lc, * ld, * rc;

	lc = (BFCTYPE *)malloc(sizeof(BFCTYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pnt.p[id]<=lb->pnt.p[id]) {								// ���_���W�召�Ŕ�r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/*********************************************************/
static BFCTYPE* Fc_Mergsrt(BFCTYPE* mlist, int n, int id)				//<<< �ʌ�_�Z�o�̿��
/*********************************************************/
{
	BFCTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BFCTYPE *)MergeFc(Fc_Mergsrt(la, n/2, id),
                                Fc_Mergsrt(lb, n-n/2, id), id);			// ���_���W���
		rc = lm;
	}
	return rc;
}

/*************************************************************************/
static void SetClDataSub(int sfg, int ssn, int sen, PNTTYPE spt, int efg,
                         int esn, int een, PNTTYPE ept, int pfg)		//<<< �ʌ����Ő��ް��ݒ�
/*************************************************************************/
{
	BCLTYPE* tc;

	tc = (BCLTYPE *)malloc(sizeof(BCLTYPE));							// �ʌ����Ő��̈�m��
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->sfg = sfg, tc->ssn = ssn, tc->sen = sen, tc->spt = spt;			// �n�_�����
	tc->efg = efg, tc->esn = esn, tc->een = een, tc->ept = ept;			// �I�_�����
	tc->pfg = pfg;														// �o���׸�
	tc->cfs = 0;														// �m�F�׸�
	tc->cfe = 0;
	tc->next = Cllist, Cllist = tc;
}

/***************************/
static BOOL SetClData(void)												//<<< �ʌ����Ő��ް��ݒ�
/***************************/
{
	BFCTYPE* wfc, * sf1=NULL, * sf2=NULL, * wf0=NULL, * wf1=NULL;
	BOOL     ret=FALSE;
	int      psw=0, pf0=0, pf1=0, wp0=0, wp1=0;

	wfc = Fclist;
	while(wfc!=NULL) {
		if(!wfc->flg) {													// �o�͑��ʂ̏ꍇ
			wp0 = 1;
			pf0 = (!pf0) ? 1 : 0;
		} else {														// ��r���ʂ̏ꍇ
			wp1 = 1;
			pf1 = (!pf1) ? 1 : 0;
		}
		if(psw==2) {													// <�o���ް��L>
			SetClDataSub(sf1->flg, sf1->sno, sf1->eno, sf1->pnt,
                         sf2->flg, sf2->sno, sf2->eno, sf2->pnt, 0);	// �ʌ����Ő��ް��ݒ�
			ret = TRUE;													// RET=�o�͍ς�
			psw = 0;
			if(!wfc->flg&&
               fabs(wfc->pnt.p[0]-sf2->pnt.p[0])<EPSILON&&
               fabs(wfc->pnt.p[1]-sf2->pnt.p[1])<EPSILON&&
               fabs(wfc->pnt.p[2]-sf2->pnt.p[2])<EPSILON) {				// �o�͏I�_�Ƌߎ��ŏo�͑���
				SetClDataSub(sf2->flg, sf2->sno, sf2->eno, sf2->pnt,
                             wfc->flg, wfc->sno, wfc->eno, wfc->pnt, 1);// �ʌ����Ő��ް��ݒ�(��)
			}
		}
		if(!pf0&&!pf1) {												// <���������I��>
			psw = 0;
			if(wp0&&!wp1) {												// �o�͑��ް��ۑ�
				wf0 = wfc;
			}
			if(!wp0&&wp1) {												// ��r���ް��ۑ�
				wf1 = wfc;
			}
		} else if(pf0&&pf1) {											// <���������J�n>
			if(wfc->flg&&
               fabs(wfc->pnt.p[0]-sf1->pnt.p[0])<EPSILON&&
               fabs(wfc->pnt.p[1]-sf1->pnt.p[1])<EPSILON&&
               fabs(wfc->pnt.p[2]-sf1->pnt.p[2])<EPSILON) {				// �J�n�O�_�Ƌߎ��ŏo�͑���
				SetClDataSub(sf1->flg, sf1->sno, sf1->eno, sf1->pnt,
                             wfc->flg, wfc->sno, wfc->eno, wfc->pnt, 1);// �ʌ����Ő��ް��ݒ�(��)
				ret = TRUE;												// RET=�o�͍ς�
			}
			sf1 = wfc;													// �ް�����
			psw = 1;													// �o���׸�:�n�_�ݒ�
		} else {														// <������J�n>
			if(psw==0) {												// �n�_�߲���ۑ�
				sf1 = wfc;
			} else if(psw==1) {											// �I�_�߲���ۑ�
				sf2 = wfc;
				psw = 2;
			}
			if(pf0&&wf1!=NULL) {										// �o�͑��ް��ۑ�
				wf0 = wfc;
			}
			if(pf1&&wf0!=NULL) {										// ��r���ް��ۑ�
				wf1 = wfc;
			}
			if(wf0!=NULL&&wf1!=NULL) {
				if(abs(wf0->pnt.p[0]-wf1->pnt.p[0])<EPSILON&&
                   abs(wf0->pnt.p[1]-wf1->pnt.p[1])<EPSILON&&
                   abs(wf0->pnt.p[2]-wf1->pnt.p[2])<EPSILON) {			// �o�͑��Ɣ�r�����ߎ���
					SetClDataSub(wf0->flg, wf0->sno, wf0->eno,
                                 wf0->pnt, wf1->flg, wf1->sno,
                                 wf1->eno, wf1->pnt, 1);				// �ʌ����Ő��ް��ݒ�(��)
					ret = TRUE;											// RET=�o�͍ς�
				}
				wp0 = 0, wp1 = 0, wf0 = NULL, wf1 = NULL;
			}
		}
		wfc = wfc->next;												// ���߲����
	}
	if(psw==2) {														// <�o���ް��L>
		SetClDataSub(sf1->flg, sf1->sno, sf1->eno, sf1->pnt,
                     sf2->flg, sf2->sno, sf2->eno, sf2->pnt, 0);		// �ʌ����Ő��ް��ݒ�
		ret = TRUE;														// RET=�o�͍ς�
	}
	return ret;
}

/******************************************************/
static BOOL CheckCrossFace(BFATYPE* fp1, BFATYPE* fp2,
                           OBJTYPE* op1, OBJTYPE* op2)					//<<< �ʌ�������
/******************************************************/
{
	BFCTYPE* wfc, * dfc;
	PNTTYPE  zr, mi, ma, cp, sp, ep, pt, pp;
	VECTYPE  vc;
	int      i, ix, ix1, sv, ev, sf, ef, cnt1=0, cnt2=0;
	BOOL     ret=FALSE;
	double   d1, d2, n[3], t, ln;

	for(i=0; i<DIM; i++) {
		zr.p[i] = 0.0;													// ���_���W
	}
	Fclist = NULL;														// ��Ɨ̈揉����
	for(i=0; i<DIM; i++) {
		mi.p[i] = DBL_MAX, ma.p[i] = 1.0-DBL_MAX;						// X,Y,Z�����l�ݒ�
	}
	vc = fp2->vec;														// ����:fp2�@���޸��
	cp = fp2->inpt;														// ����:fp2��̓_
	d1 = vc.p[0]*cp.p[0]+vc.p[1]*cp.p[1]+vc.p[2]*cp.p[2];				// ����:fp2�萔
	ix = fp1->stfl;														// ����:fp1ٰ�ߊJ�n�ԍ�
	while(TRUE) {
		GetFlpVtx1(op1, ix, &sp, &sf);									// ����:fp1ٰ�ߒ��_�擾
		ix1 = (!sf) ? ix+1 : fp1->stfl;									// ����:fp1��ٰ�ߒ��_�擾
		GetFlpVtx1(op1, ix1, &ep, &ef);
		if(PntD(&zr, &sp)>PntD(&zr, &ep)) {								// <�I�_�̕������_�ɋ߂�>
			pt = sp, sp = ep, ep = pt;									// �n�_,�I�_����
		}
		for(i=0; i<DIM; i++) {
			n[i] = ep.p[i]-sp.p[i];										// ����:fp1�����޸��
		}
		d2 = vc.p[0]*sp.p[0]+vc.p[1]*sp.p[1]+vc.p[2]*sp.p[2];			// ��_�Z�o�v�Z
		t = vc.p[0]*n[0]+vc.p[1]*n[1]+vc.p[2]*n[2];
		if(fabs(t)>DEPSILON) {
			t = (d1-d2)/t;
			for(i=0; i<DIM; i++) {
				pp.p[i] = sp.p[i]+n[i]*t;								// ����:fp2&������_
			}
			if(((sp.p[0]<=pp.p[0]&&pp.p[0]<=ep.p[0])||
                (ep.p[0]<=pp.p[0]&&pp.p[0]<=sp.p[0]))&&
               ((sp.p[1]<=pp.p[1]&&pp.p[1]<=ep.p[1])||
                (ep.p[1]<=pp.p[1]&&pp.p[1]<=sp.p[1]))&&
               ((sp.p[2]<=pp.p[2]&&pp.p[2]<=ep.p[2])||
                (ep.p[2]<=pp.p[2]&&pp.p[2]<=sp.p[2]))) {				// <�����͈͓�>
				SetFcData(0, ix, ix1, pp);								// �ʌ�_�ݒ�
				cnt1++;													// ����
				for(i=0; i<DIM; i++) {
					if(mi.p[i]>pp.p[i]) {								// �ŏ���ő�l�擾
						mi.p[i] = pp.p[i];
					}
					if(ma.p[i]<pp.p[i]) {
						ma.p[i] = pp.p[i];
					}
				}
			}
		}
		if(sf) {														// ��ٰ�ߏI��=>�����I��
			break;
		}
		ix++;
	}
	vc = fp1->vec;														// ����:fp1�@���޸��
	cp = fp1->inpt;														// ����:fp1��̓_
	d1 = vc.p[0]*cp.p[0]+vc.p[1]*cp.p[1]+vc.p[2]*cp.p[2];				// ����:fp1�萔
	ix = fp2->stfl;														// ����:fp2ٰ�ߊJ�n�ԍ�
	while(TRUE) {
		GetFlp1(op2, ix, &sv, &sf);										// ����:fp2ٰ�ߒ��_�擾
		GetVtx(op2, sv, &sp);
		if(!sf) {														// ����:fp2��ٰ�ߒ��_�擾
			GetFlp1(op2, ix+1, &ev, &ef);
		} else {
			GetFlp1(op2, fp2->stfl, &ev, &ef);
		}
		GetVtx(op2, ev, &ep);
		if(PntD(&zr, &sp)>PntD(&zr, &ep)) {								// <�I�_�̕������_�ɋ߂�>
			pt = sp, sp = ep, ep = pt;									// �n�_,�I�_����
		}
		for(i=0; i<DIM; i++) {
			n[i] = ep.p[i]-sp.p[i];										// ����:fp2�����޸��
		}
		d2 = vc.p[0]*sp.p[0]+vc.p[1]*sp.p[1]+vc.p[2]*sp.p[2];			// ��_�Z�o�v�Z
		t = vc.p[0]*n[0]+vc.p[1]*n[1]+vc.p[2]*n[2];
		if(fabs(t)>DEPSILON) {
			t = (d1-d2)/t;
			for(i=0; i<DIM; i++) {
				pp.p[i] = sp.p[i]+n[i]*t;								// ����:fp1&������_
			}
			if(((sp.p[0]<=pp.p[0]&&pp.p[0]<=ep.p[0])||
                (ep.p[0]<=pp.p[0]&&pp.p[0]<=sp.p[0]))&&
               ((sp.p[1]<=pp.p[1]&&pp.p[1]<=ep.p[1])||
                (ep.p[1]<=pp.p[1]&&pp.p[1]<=sp.p[1]))&&
               ((sp.p[2]<=pp.p[2]&&pp.p[2]<=ep.p[2])||
                (ep.p[2]<=pp.p[2]&&pp.p[2]<=sp.p[2]))) {				// <�����͈͓�>
				SetFcData(1, sv, ev, pp);								// �ʌ�_�ݒ�
				cnt2++;													// ����
				for(i=0; i<DIM; i++) {
					if(mi.p[i]>pp.p[i]) {								// �ŏ���ő�l�擾
						mi.p[i] = pp.p[i];
					}
					if(ma.p[i]<pp.p[i]) {
						ma.p[i] = pp.p[i];
					}
				}
			}
		}
		if(sf) {														// ��ٰ�ߏI��=>�����I��
			break;
		}
		ix++;
	}
	if((cnt1!=0&&cnt2!=0)&&(cnt1==1||cnt2==1)) {						// <<��̌�_����>>
		ErrFlg2 = 1;													// �װ�׸�ON
	} else if(cnt1>1&&cnt2>1) {											// <<�L���ް��L>>
		ln = ma.p[0]-mi.p[0];											// X,Y,Z�̑傫�����ſ��
		ix = 0;
		if(ln<ma.p[1]-mi.p[1]) {
			ix = 1;
			ln = ma.p[1]-mi.p[1];
		}
		if(ln<ma.p[2]-mi.p[2]) {
			ix = 2;
		}
		Fclist = Fc_Mergsrt(Fclist, cnt1+cnt2, ix);						// �ʌ�_�Z�o�ް����
		ret = SetClData();												// �ʌ����Ő��ް��ݒ�
	}
	wfc = Fclist;
	while(wfc) {														// �ʌ�_�Z�o�̈���
		dfc = wfc, wfc = wfc->next;
		free(dfc);
	}
	return ret;
}

/************************************************************/
static int CheckInFace(PNTTYPE cp, BFATYPE* fp, OBJTYPE* op)			//<<< �ʓ���������
/************************************************************/
{
	PNTTYPE p1, p2, sp, ep;
	int     ix, f1, f2, chx, chy, ucx, dcx, ucy, dcy, ret=0;
	double  d1, d2, sx, sy, ex, ey;

	while(TRUE) {
		chx = 0, ucx = 0, dcx = 0;
		ix = fp->stfl;													// ��ٰ�ߊJ�n�ԍ�
		while(TRUE) {
			GetFlpVtx1(op, ix, &p1, &f1);								// ��ٰ�ߒ��_�擾
			if(!f1) {
				GetFlpVtx1(op, ix+1, &p2, &f2);							// ��ٰ�ߒ��_�擾
			} else {
				GetFlpVtx1(op, fp->stfl, &p2, &f2);
			}
			if(p1.p[0]<p2.p[0]) {										// �����������n�_
				sp = p1, ep = p2;
			} else if(p1.p[0]>p2.p[0]) {
				sp = p2, ep = p1;
			} else if(p1.p[1]<p2.p[1]) {
				sp = p1, ep = p2;
			} else {
				sp = p2, ep = p1;
			}
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			if(sx<=cp.p[0]&&cp.p[0]<ex) {								// <�w��_X������X�Ɋ�>
				d1 = ey-sy, d2 = ex-sx;
				d1 = d1/d2;												// �X��
				d2 = sy-d1*sx;											// �萔
				d2 = d1*cp.p[0]+d2;										// Y�l�v�Z
				if(d2>cp.p[1]) {										// ������_���w��_����
					ucx++;
				} else if(cp.p[1]>d2) {									// ������_���w��_��菬
					dcx++;
				} else {												// �ڂ���ꍇ
					chx = 1; break;
				}
			}
			if(f1) {													// ��ٰ�ߏI��=>�����I��
				break;
			}
			ix++;
		}
		chy = 0, ucy = 0, dcy = 0;
		ix = fp->stfl;													// ��ٰ�ߊJ�n�ԍ�
		while(TRUE) {
			GetFlpVtx1(op, ix, &p1, &f1);								// ��ٰ�ߒ��_�擾
			if(!f1) {													// ��ٰ�ߒ��_�擾
				GetFlpVtx1(op, ix+1, &p2, &f2);
			} else {
				GetFlpVtx1(op, fp->stfl, &p2, &f2);
			}
			if(p1.p[1]<p2.p[1]) {										// �����������n�_
				sp = p1, ep = p2;
			} else if(p1.p[1]>p2.p[1]) {
				sp = p2, ep = p1;
			} else if(p1.p[0]<p2.p[0]) {
				sp = p1, ep = p2;
			} else {
				sp = p2, ep = p1;
			}
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			if(sy<=cp.p[1]&&cp.p[1]<ey) {								// <�w��_Y������Y�Ɋ�>
				d1 = ex-sx, d2 = ey-sy;
				d1 = d1/d2;												// �X��
				d2 = sx-d1*sy;											// �萔
				d2 = d1*cp.p[1]+d2;										// X�l�v�Z
				if(d2>cp.p[0]) {										// ������_���w��_����
					ucy++;
				} else if(cp.p[0]>d2) {									// ������_���w��_��菬
					dcy++;
				} else {												// �ڂ���ꍇ
					chy = 1; break;
				}
			}
			if(f1) {													// ��ٰ�ߏI��=>�����I��
				break;
			}
			ix++;
		}
		if((ucx+dcx)%2==0&&chx==0&&(ucy+dcy)%2==0&&chy==0) {			// X/Y����������OK
			break;
		}
		cp.p[0] += 0.0011;												// ��ײ�͖ʓ��_�ʒu�ύX
		cp.p[1] += 0.0013;
	}
	ucx = ucx%2, dcx = dcx%2;											// 2��1���
	ucy = ucy%2, dcy = dcy%2;
	if((ucx&&dcx)&&(ucy&&dcy)) {										// <�ʂ̒��ɑ���>
		d2 = (fp->vec.p[0]*fp->inpt.p[0]+fp->vec.p[1]*fp->inpt.p[1]+
              fp->vec.p[2]*fp->inpt.p[2]-fp->vec.p[0]*cp.p[0]-
              fp->vec.p[1]*cp.p[1])/fp->vec.p[2];						// �w��_(XY)�ʒu�ʏ��Z
		if(d2>cp.p[2]) {												// �w��_���ʂ���
			ret = 1;
		} else if(d2<cp.p[2]) {											// �w��_���ʂ���
			ret = -1;
		}
	}
	return ret;
}

/*******************************************************************/
static int SearchClData(int f1, BCLTYPE* c1, int* f2, BCLTYPE** c2)		//<<< �ʌ����Ő��ް�����
/*******************************************************************/
{
	BCLTYPE* wc;
	int      sn, en, ret=0;

	if(!f1) {															// ��������
		sn = c1->esn, en = c1->een;
	} else {
		sn = c1->ssn, en = c1->sen;
	}
	wc = Cllist;
	while(wc!=NULL) {
		if(c1!=wc&&wc->pfg==0) {										// <�ΏۈȊO�̐����ް�>
			if(wc->sfg&&sn==wc->ssn&&en==wc->sen&&wc->cfs==0) {			// �n�_��������L
				ret = 1;												// ���m�F�ς�
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->efg) {											// ���Α���OUT�͊m�F�ς�
					ret = 2;
				}
				*f2 = 0, *c2 = wc; break;
			}
			if(wc->efg&&sn==wc->esn&&en==wc->een&&wc->cfe==0) {			// �I�_��������L
				ret = 1;												// ���m�F�ς�
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->sfg) {											// ���Α���OUT�͊m�F�ς�
					ret = 2;
				}
				*f2 = 1, *c2 = wc; break;
			}
		}
		wc = wc->next;													// ���߲����
	}
	if(ret!=0) {														// �����ς݂͏I��
		return ret;
	}
	wc = Cllist;
	while(wc!=NULL) {
		if(c1!=wc&&wc->pfg==1) {										// <�ΏۈȊO�̉��ް�>
			if(wc->sfg&&sn==wc->ssn&&en==wc->sen&&wc->cfs==0) {			// �n�_��������L
				ret = 1;												// ���m�F�ς�
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->efg) {											// ���Α���OUT�͊m�F�ς�
					ret = 2;
				}
				*f2 = 1, *c2 = wc; break;
			}
			if(wc->efg&&sn==wc->esn&&en==wc->een&&wc->cfe==0) {			// �I�_��������L
				ret = 1;												// ���m�F�ς�
				wc->cfs = 1, wc->cfe = 1;
				if(!wc->sfg) {											// ���Α���OUT�͊m�F�ς�
					ret = 2;
				}
				*f2 = 1, *c2 = wc; break;
			}
		}
		wc = wc->next;													// ���߲����
	}
	return ret;
}

/********************************/
static void CompleteClData(void)										//<<< �ʌ����Ő���������
/********************************/
{
	BCLTYPE* wc;

	wc = Cllist;
	while(wc!=NULL) {
		if(wc->cfs==1&&wc->cfe==1) {									// ���m�F�ς݂�
			wc->pfg = 2, wc->cfs = 2, wc->cfe = 2;						// �����m�F�ς݂�
		}
		wc = wc->next;													// ���߲����
	}
}

/**********************************/
static void IncompleteClData(void)										//<<< �ʌ����Ő�����������
/**********************************/
{
	BCLTYPE* wc;

	wc = Cllist;
	while(wc!=NULL) {
		if(wc->cfs==1&&wc->cfe==1) {									// ���m�F�ς݂�
			wc->cfs = 0, wc->cfe = 0;									// �����
		}
		wc = wc->next;													// ���߲����
	}
}

/****************************/
static int CheckClData(void)											//<<< �ʌ����Ő��ް��m�F
/****************************/
{
	BCLTYPE* wc, * wc1, * wc2;
	int      fg1, fg2, ret, cnt=0;

	wc = Cllist;
	while(wc!=NULL) {													// <<�����ް��m�F>>
		if(!wc->sfg&&!wc->efg&&wc->pfg==0) {							// �n�_��I�_��OUT��
			wc->pfg = 2, wc->cfs = 2, wc->cfe = 2;						// �m�F�ς�
		} else if((!wc->sfg||!wc->efg)&&wc->pfg==0) {					// �n�_�d�����_�ް��m�F
			wc->cfs = 1, wc->cfe = 1;									// ���m�F�ς�
			fg1 = (!wc->sfg) ? 0 : 1;									// �����ް����
			wc1 = wc;
			while(TRUE) {
				ret = SearchClData(fg1, wc1, &fg2, &wc2);				// �ʌ����Ő��ް�����
				if(ret!=1) {											// �������L�ȊO�͏I��
					break;
				}
				fg1 = fg2, wc1 = wc2;									// �������ް����
			}
			if(ret==2) {
				CompleteClData();										// �����m�F�ς�
			} else {
				IncompleteClData();										// ���m�F�ςݎ��
				wc->pfg = -1;											// IN���ɓ���������͖���
			}
		}
		wc = wc->next;													// ���߲����
	}
	wc = Cllist;
	while(wc!=NULL) {													// <<���ް��m�F>>
		if(!wc->sfg&&!wc->efg&&wc->pfg==1) {							// �n�_��I�_��OUT��
			wc->pfg = 2, wc->cfs = 2, wc->cfe = 2;						// �m�F�ς�
		} else if((!wc->sfg||!wc->efg)&&wc->pfg==1) {					// �n�_�d�����_�ް��m�F
			wc->cfs = 1, wc->cfe = 1;									// ���m�F�ς�
			fg1 = (!wc->sfg) ? 0 : 1;									// �����ް����
			wc1 = wc;
			while(TRUE) {
				ret = SearchClData(fg1, wc1, &fg2, &wc2);				// �ʌ����Ő��ް�����
				if(ret!=1) {											// �������L�ȊO�͏I��
					break;
				}
				fg1 = fg2, wc1 = wc2;									// �������ް����
			}
			if(ret==2) {
				CompleteClData();										// �����m�F�ς�
			} else {
				IncompleteClData();										// ���m�F�ςݎ��
				wc->pfg = -1;											// IN���ɓ���������͖���
			}
		}
		wc = wc->next;													// ���߲����
	}
	wc = Cllist;
	while(wc!=NULL) {
		if(wc->pfg==2) {												// �L���ް�����
			wc->pfg = 0, wc->cfs = 0, wc->cfe = 0;
			cnt++;
		} else if(wc->pfg!=-1) {
			cnt++;
		}
		wc = wc->next;													// ���߲����
	}
	return cnt;
}

/**********************************************************************/
static void SetCdData(int flg, int ssn, int sen, PNTTYPE spt, int sno)	//<<< �������擪�ް��ݒ�
/**********************************************************************/
{
	BCDTYPE* tc;
	int      i;

	tc = (BCDTYPE *)malloc(sizeof(BCDTYPE));							// �������擪�̈�m��
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->flg = flg, tc->ssn = ssn, tc->sen = sen, tc->spt = spt;
	tc->sln = 0.0, tc->sfg = 0,   tc->esn = 0,   tc->een = 0;
	for(i=0; i<DIM; i++) {
		tc->ept.p[i] = 0;
	}
	tc->eln = 0.0, tc->efg = 0, tc->sno = sno, tc->pfg = 0;
	tc->cpp = NULL;
	tc->next = Cdlist, Cdlist = tc;
	if(!flg) {
		Cocnt++;
	} else {
		Cicnt++;
	}
}

/***************************************************/
static void SetCdEln(int esn, int een, PNTTYPE ept)						//<<< �������擪��I���Ԓn�ݒ�
/***************************************************/
{
	Cdlist->esn = esn;
	Cdlist->een = een;
	Cdlist->ept = ept;
}

/***********************************/
static void SetCpData1(PNTTYPE pnt)										//<<< ���������_���W�ݒ�1
/***********************************/
{
	BCPTYPE* tc, * wcp;

	tc = (BCPTYPE *)malloc(sizeof(BCPTYPE));							// ���������_�̈�m��
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->next = NULL, tc->pnt = pnt;
	wcp = Cdlist->cpp;													// �ް������ڑ�
	if(wcp==NULL) {
		Cdlist->cpp = tc;
	} else {
		while(wcp->next!=NULL) {
			wcp = wcp->next;
		}
		wcp->next = tc;
	}
}

/***********************************/
static void SetCpData2(PNTTYPE pnt)										//<<< ���������_���W�ݒ�2
/***********************************/
{
	BCPTYPE* tc;

	tc = (BCPTYPE *)malloc(sizeof(BCPTYPE));							// ���������_�̈�m��
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->next = Cdlist->cpp;
	tc->pnt = pnt;
	Cdlist->cpp = tc;													// �ް��擪�ڑ�
}

/********************************************************************/
static BOOL SearchNearCdData(int sn, int en, int* flg, BCDTYPE** cd)	//<<< ��ʒu�ŒZ�ް�����
/********************************************************************/
{
	BCDTYPE* wc, * sc;
	BOOL     ret=FALSE;
	int      fg;
	double   ln;

	wc = Cdlist, sc = NULL, ln = DBL_MAX;
	while(wc!=NULL) {
		if(!wc->flg&&!wc->sfg) {										// <OUT���ŊJ�n��������>
			if(sn==wc->ssn&&en==wc->sen&&(sn!=wc->esn||en!=wc->een)) {	// �J�nٰ�߂���v
				if(ln>wc->sln) {										// �ŒZ�_��ۑ�
					sc = wc, ln = wc->sln, fg = 0;
				}
			}
		}
		if(!wc->flg&&!wc->efg) {										// <OUT���ŏI����������>
			if(sn==wc->esn&&en==wc->een&&(sn!=wc->ssn||en!=wc->sen)){	// �I��ٰ�߂���v
				if(ln>wc->eln) {										// �ŒZ�_��ۑ�
					sc = wc, ln = wc->eln, fg = 1;
				}
			}
		}
		wc = wc->next;
	}
	if(sc!=NULL) {														// <�ŒZ�_���������ꍇ>
		ret = TRUE;
		if(!fg) {														// �����ς�
			sc->sfg = 1;
		} else {
			sc->efg = 1;
		}
		*flg = fg;														// �J�n/�I���׸ނ��߲�����
		*cd = sc;
	}
	return ret;
}

/**************************************************/
static void ChangeCdData(BFATYPE* fp, OBJTYPE* op)						//<<< �������擪�ʒu�ύX
/**************************************************/
{
	double*  ln = new double[View->m_MaxOPnt];
	BCDTYPE* wcd, * wc1, * wc2;
	BCPTYPE* wcp;
	PNTTYPE  p1, p2, p3;
	int      i=0, j, k, f, stf, fg1, fg2;
	double   aln, l11, l12, l21, l22;

	stf = fp->stfl;														// �Ώۖʂ̖�ٰ�ߊJ�n�ʒu
	while(TRUE) {
		GetFlpVtx1(op, stf+i, &p1, &f);									// �Ώە��ʒ��_���W�擾
		if(i==0) {														// �擪�͒��_�ۑ�+�ޯ̧�ر
			p3 = p1, ln[0] = 0.0, aln = 0.0;
		} else {
			aln += PntD(&p1, &p2);										// ����_�ƑO��_�̋��������Z
			ln[i] = aln;												// ���݂܂ł̋������v��ݒ�
		}
		p2 = p1;														// ���񒸓_��ۑ�
		i++;
		if(f) {															// ��ٰ�ߏI���ŏI��
			break;
		}
	}
	aln += PntD(&p3, &p2);												// �ŏI�_�ƊJ�n�_�̋��������Z
	wcd = Cdlist;
	while(wcd!=NULL) {
		if(!wcd->flg) {
			GetFlpVtx1(op, wcd->ssn, &p1, &j);							// �J�nٰ�ߊ�ʒu�̒��_
			wcd->sln = PntD(&p1, &wcd->spt);							// �J�n��_�ƌ�_�̋���
			GetFlpVtx1(op, wcd->esn, &p1, &j);							// �I��ٰ�ߊ�ʒu�̒��_
			wcd->eln = PntD(&p1, &wcd->ept);							// �I����_�ƌ�_�̋���
		}
		wcd = wcd->next;
	}
	i = 0;
	while(TRUE) {
		GetFlp1(op, stf+i, &j, &f);										// �Ώە��ʒ��_No�擾
		j = (!f) ? i+1 : 0;
		if(SearchNearCdData(stf+i, stf+j, &fg1, &wc1)) {				// ��ʒu�ŒZ�ް�����
			while(TRUE) {
				if(SearchNearCdData(stf+i, stf+j, &fg2, &wc2)) {		// ��ʒu�ŒZ�ް�����
					l11 = (!fg1) ? wc1->sln : wc1->eln;					// wc1�̊�_����
					l12 = (!fg2) ? wc2->sln : wc2->eln;					// wc2�̊�_����
					if(fabs(l11-l12)<EPSILON) {							// <�덷�͈͓��̏ꍇ>
						l11 += ln[i], l12 += ln[i];						// ٰ�ߊJ�n�ʒu����̋���
						l21 = (!fg1) ? wc1->eln : wc1->sln;				// wc1���Α��̊�_����
						k = (!fg1) ? wc1->esn : wc1->ssn;				// wc1���Α��̊�ʒu
						l21 += ln[k-stf];								// ٰ�ߊJ�n�ʒu����̋���
						l22 = (!fg2) ? wc2->eln : wc2->sln;				// wc2���Α��̊�_����
						k = (!fg2) ? wc2->esn : wc2->ssn;				// wc2���Α��̊�ʒu
						l22 += ln[k-stf];								// ٰ�ߊJ�n�ʒu����̋���
						if((l11=l11-l21)<0.0) {							// �����ʒu���甽�Α��ւ̋���
							l11 += aln;
						}
						if((l12=l12-l22)<0.0) {
							l12 += aln;
						}
						if(l11>l12) {									// <�O�オ�t�̏ꍇ>
							p1 = (!fg1) ? wc1->spt : wc1->ept;			// wc1�̒��_
							p2 = (!fg2) ? wc2->spt : wc2->ept;			// wc2�̒��_
							l11 = (!fg1) ? wc1->sln : wc1->eln;			// wc1�̊�_����
							l12 = (!fg2) ? wc2->sln : wc2->eln;			// wc2�̊�_����
							if(!fg1) {									// <wc1���J�n���̏ꍇ>
								wc1->spt = p2, wc1->sln = l12;			// wc2�̒��_�ƒu��
								wcp = wc1->cpp, wcp->pnt = p2;			// �������̒��_���u��
							} else {									// <wc1���I�����̏ꍇ>
								wc1->ept = p2, wc1->eln = l12;			// wc2�̒��_�ƒu��
								wcp = wc1->cpp;
								while(wcp!=NULL&&wcp->next!=NULL) {		// wcp�̍ŏI�ް�
									wcp = wcp->next;
								}
								wcp->pnt = p2;							// �������̒��_���u��
							}
							if(!fg2) {									// <wc2���J�n���̏ꍇ>
								wc2->spt = p1, wc2->sln = l11;			// wc1�̒��_�ƒu��
								wcp = wc2->cpp, wcp->pnt = p1;			// �������̒��_���u��
							} else {									// <wc2���I�����̏ꍇ>
								wc2->ept = p1, wc2->eln = l11;			// wc1�̒��_�ƒu��
								wcp = wc2->cpp;
								while(wcp!=NULL&&wcp->next!=NULL) {		// wcp�̍ŏI�ް�
									wcp = wcp->next;
								}
								wcp->pnt = p1;							// �������̒��_���u��
							}
							fg2 = fg1, wc2 = wc1;						// ���_��u��=>wc1����
						}
					}
					fg1 = fg2, wc1 = wc2;								// �ް����
				} else {
					break;
				}
			}
		}
		i++;
		if(f) {															// ��ٰ�ߏI���ŏI��
			break;
		}
	}
	delete[] ln;
}

/******************************/
static int CheckInClData(void)											//<<< �ʌ����Ő��ް�(IN)�m�F
/******************************/
{
	BCLTYPE* sc, * wc;
	int      cnt=0, ssn, sen, esn ,een, er1, er2;

	sc = Cllist;
	while(sc!=NULL) {
		if(!sc->pfg) {													// <�L���ް�>
			ssn = sc->ssn, sen = sc->sen, er1 = 1;						// �n�_���ް�
			esn = sc->esn, een = sc->een, er2 = 1;						// �I�_���ް�
			wc = Cllist;
			while(wc!=NULL) {
				if(!wc->pfg&&sc!=wc) {									// <���̗L���ް�>
					if((ssn==wc->ssn&&sen==wc->sen)||
                       (ssn==wc->esn&&sen==wc->een)) {					// �n�_���A���ް�����
						er1 = 0;
					}
					if((esn==wc->ssn&&een==wc->sen)||
                       (esn==wc->esn&&een==wc->een)) {					// �I�_���A���ް�����
						er2 = 0;
					}
					if(!er1&&!er2) {									// �n�_��I�_�A���L�ŏI��
						break;
					}
				}
				wc = wc->next;
			}
			if(er1||er2) {												// �n�_��I�_�A�����͖����ް�
				sc->pfg = -1;
				cnt++;
			}
		}
		sc = sc->next;
	}
	return cnt;															// �����ް�����
}

/*************************************************/
static void MakeCutLine(BFATYPE* fp, OBJTYPE* op)						//<<< �����ʕ������쐬
/*************************************************/
{
	BCLTYPE* wc, * sc;
	BCDTYPE* cd;
	BCPTYPE* cp;
	PNTTYPE  spt, ept;
	CString  str;
	int      cnt, ssn, sen, esn, een, efg, ct1, ct2;

	Cdlist = NULL, Cocnt = 0, Cicnt = 0, ct1 = 0, ct2 = 0;				// ��Ɨ̈揉����
	if((cnt=CheckClData())<1) {											// �ʌ����Ő��ް��m�F
		return;
	}
	while(TRUE) {
		wc = Cllist;													// <<<OUT��ٰ�߂̍쐬>>>
		while(wc!=NULL) {												// �������擪�ް�����(OUT)
			if(!wc->pfg&&(!wc->sfg||!wc->efg)) {						// <<�L���ް��L>>
				if(!wc->sfg) {											// <�n�_���L��>
					ssn = wc->ssn, sen = wc->sen, spt = wc->spt;
					efg = wc->efg;
					esn = wc->esn, een = wc->een, ept = wc->ept;
				} else {												// <�I�_���L��>
					ssn = wc->esn, sen = wc->een, spt = wc->ept;
					efg = wc->sfg;
					esn = wc->ssn, een = wc->sen, ept = wc->spt;
				}
				wc->pfg = 1;											// �����ς�
				ct1++, ct2++;
				sc = wc;												// �߲���ۑ�
				SetCdData(0, ssn, sen, spt, ct2);						// �������擪�ް��ݒ�(OUT)
				SetCpData1(spt);										// ���������_���W�ݒ�
				SetCpData1(ept);
				if(!efg) {												// <�n�_�I�_�̗������L��>
					SetCdEln(esn, een, ept);
				} else {												// <�n�_�I�_�̈�����L��>
					break;
				}
			}
			wc = wc->next;												// ���߲����
		}
		if(wc==NULL) {													// �擪�ް����͏I��
			break;
		}
		wc = Cllist;
		while(wc!=NULL) {												// <<�n�_����v����>>
			if(!wc->pfg&&wc->sfg&&esn==wc->ssn&&een==wc->sen&&wc!=sc) {
				wc->pfg = 1;											// �����ς�
				ct1++;
				SetCpData1(wc->ept);									// ���������_���W�ݒ�
				if(wc->efg) {											// <���ް��L>
					esn = wc->esn, een = wc->een;						// �������ԍ��ۑ�
					sc = wc, wc = Cllist;								// �߲���ۑ�
					continue;											// �߲����擪�ɖ߂�
				} else {												// <���ް���>
					SetCdEln(wc->esn, wc->een, wc->ept);				// �I���Ԓn�ݒ�
					break;
				}
			}
			if(!wc->pfg&&wc->efg&&esn==wc->esn&&een==wc->een&&wc!=sc) {	// <<�I�_����v����>>
				wc->pfg = 1;											// �����ς�
				ct1++;
				SetCpData1(wc->spt);									// ���������_���W�ݒ�
				if(wc->sfg) {											// <���ް��L>
					esn = wc->ssn, een = wc->sen;						// �������ԍ��ۑ�
					sc = wc, wc = Cllist;								// �߲���ۑ�
					continue;											// �߲����擪�ɖ߂�
				} else {												// <���ް���>
					SetCdEln(wc->ssn, wc->sen, wc->spt);				// �I���Ԓn�ݒ�
					break;
				}
			}
			wc = wc->next;												// ���߲����
		}
	}
	ChangeCdData(fp, op);												// �������擪�ʒu�ύX
	cd = Cdlist;
	while(cd!=NULL) {													// <<<���Α��֕������쐬>>>
		SetCdData(0, cd->esn, cd->een, cd->ept, cd->sno);				// �������擪�ް��ݒ�(OUT)
		cp = cd->cpp;
		while(cp!=NULL) {
			SetCpData2(cp->pnt);										// ���������_���W�ݒ�
			cp = cp->next;												// ���߲����
		}
		SetCdEln(cd->ssn, cd->sen, cd->spt);							// �I���Ԓn�ݒ�
		cd = cd->next;													// ���߲����
	}
	if(cnt-ct1<3) {														// IN���̉\����3�����͏I��
		return;
	}
	cnt -= CheckInClData();												// �ʌ����Ő��ް�(IN)�m�F
	while(TRUE) {
		ct2 = CheckInClData();											// �ʌ����Ő��ް�(IN)�m�F
		cnt -= ct2;														// �����ް������Z
		if(ct2==0) {													// �����ް��Ȃ��Ŋm�F���~
			break;
		}
	}
	if(cnt-ct1<3) {														// IN���̉\����3�����͏I��
		return;
	}
	while(TRUE) {
		if(cnt<=ct1) {													// �S�������ςݒ��~
			break;
		}
		wc = Cllist;													// <<<IN��ٰ�ߍ쐬>>>
		while(wc!=NULL) {												// �������擪�ް�����(IN)
			if(!wc->pfg) {												// <<�L���ް��L>>
				esn = wc->esn, een = wc->een;							// �������ԍ��ۑ�
				wc->pfg = 1;											// �����ς�
				ct1++;
				SetCdData(1, 0, 0, wc->ept, 0);							// �������擪�ް��ݒ�(IN)
				SetCpData1(wc->ept);									// ���������_���W�ݒ�
				break;
			}
			wc = wc->next;												// ���߲����
		}
		if(wc==NULL) {													// �擪�ް����͏I��
			break;
		}
		wc = Cllist;
		while(wc!=NULL) {												// ���������_������
			if(!wc->pfg&&esn==wc->ssn&&een==wc->sen) {					// <<�n�_����v>>
				esn = wc->esn, een = wc->een;							// �������ԍ��ۑ�
				wc->pfg = 1;											// �����ς�
				ct1++;
				SetCpData1(wc->ept);									// ���������_���W�ݒ�
				wc = Cllist;
				continue;												// �߲����擪�ɖ߂�
			}
			if(!wc->pfg&&esn==wc->esn&&een==wc->een) {					// <<�I�_����v>>
				esn = wc->ssn, een = wc->sen;							// �������ԍ��ۑ�
				wc->pfg = 1;											// �����ς�
				ct1++;
				SetCpData1(wc->spt);									// ���������_���W�ݒ�
				wc = Cllist;
				continue;												// �߲����擪�ɖ߂�
			}
			wc = wc->next;												// ���߲����
		}
	}
}

/******************************************************/
static BOOL CheckOutLoop(PNTTYPE pt, int sn, int en,
                         BCDTYPE** cd1, BCDTYPE** cd2)					//<<< OUTٰ���ް��m�F
/******************************************************/
{
	BCDTYPE* wc, * w1, * w2;
	PNTTYPE  pnt;
	BOOL     ret=FALSE;
	double   ln, ln1, ln2;

	wc = Cdlist, w1 = NULL, ln1 = DBL_MAX;
	while(wc!=NULL) {
		if(wc->pfg==0) {												// <<�������̏ꍇ>>
			if(sn==wc->esn&&en==wc->een) {								// <�I�_���w������ƈ�v>
				pnt = wc->ept;
				ln = PntD(&pt, &pnt);									// ��_����̋���
				if(ln<ln1) {											// ��_�ɍŋߓ_�ۑ�
					w1 = wc, ln1 = ln;
				}
			}
		}
		wc = wc->next;													// ���߲����
	}
	if(w1!=NULL) {
		wc = Cdlist, w2 = NULL, ln2 = DBL_MAX;
		while(wc!=NULL) {
			if(wc->pfg==0) {											// <<�������̏ꍇ>>
				if(sn==wc->ssn&&en==wc->sen) {							// <�n�_���w������ƈ�v>
					pnt = wc->spt;
					ln = PntD(&pt, &pnt);								// ��_����̋���
					if(ln>ln1&&ln<ln2) {								// �I�_���ɍŋߓ_�ۑ�
						w2 = wc, ln2 = ln;
					}
				}
			}
			wc = wc->next;												// ���߲����
		}
		if(w2!=NULL) {													// �m�F�߲���ݒ�
			*cd1 = w1, *cd2 = w2;
			ret = TRUE;
		}
	}
	return ret;
}

/*********************************************************/
static BOOL CheckCdData(int ssn, int* een, BCPTYPE** cpp)				//<<< �������擪�ް��m�F
/*********************************************************/
{
	BCDTYPE* wcd;
	BOOL     ret=FALSE;

	if(Cocnt>0) {														// <<�L���ް��L>>
		wcd = Cdlist;
		while(wcd!=NULL) {
			if((!wcd->flg)&&wcd->pfg==0&&wcd->ssn==ssn) {				// <OUT�ŊJ�nٰ�߈�v>
				*een = wcd->een;										// �I��ٰ��
				*cpp = wcd->cpp;										// ���_���W�߲��
				wcd->pfg = 1;											// �����ς�
				Cocnt--;												// ����DOWN
				ret = TRUE; break;
			}
			wcd = wcd->next;											// ���߲����
		}
	}
	return ret;
}

/***********************************************************/
static void CheckInLoop(int x, int y, PNTTYPE* pa, int* pn)				//<<< INٰ���ް��m�F
/***********************************************************/
{
	PNTTYPE* pb = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE* pc = new PNTTYPE[View->m_MaxOPnt];
	BCDTYPE* wcd;
	BCPTYPE* wcp;
	PNTTYPE  sp, ep, ip;
	int      an, bn, ap, bp, i, j, uc, dc, cnt=0;
	double   d, ln1, ln2;

	an = *pn, wcd = Cdlist;
	while(wcd!=NULL) {
		if(wcd->flg&&wcd->pfg==0) {										// <<<�L��IN��ٰ��>>>
			ip = wcd->ept;
			for(i=0, uc=0, dc=0; i<an; i++) {
				j = (i==an-1) ? 0 : i+1;
				if(pa[i].p[x]<pa[j].p[x]) {								// x���W������p1,�傪p2
					sp = pa[i], ep = pa[j];
				} else {
					sp = pa[j], ep = pa[i];
				}
				if((ep.p[x]-sp.p[x]<EPSILON&&
                   (sp.p[x]-EPSILON<ip.p[x]&&ip.p[x]<ep.p[x]+EPSILON))||
                   (sp.p[x]<=ip.p[x]&&ip.p[x]<=ep.p[x])) {				// <�w��_�������Ɋ�>
					d = (ep.p[y]-sp.p[y])*(ip.p[x]-sp.p[x])/
                        (ep.p[x]-sp.p[x]);								// �n�_���������̋���(X)
					d = d+sp.p[y];										// ������X���W(mm)
					if(d>ip.p[y]) {										// ������_���w��_����
						uc++;
					} else if(d<ip.p[y]) {								// ������_���w��_��菬
						dc++;
					}
					if(ip.p[y]-EPSILON<d&&d<ip.p[y]+EPSILON) {			// <�����Ɛڂ���>
						uc = 1, dc = 1; break;							// �ʓ�����
					}
				}
			}
			uc = uc%2, dc = dc%2;										// 2��1���
			if(uc&&dc) {												// <�ʂ̒��ɑ���>
				wcd->pfg = 2;
				Cicnt--;
				cnt++;
			}
		}
		wcd = wcd->next;												// ���߲����
	}
	if(cnt>0) {
		wcd = Cdlist;
		while(wcd!=NULL) {
			if(wcd->flg&&wcd->pfg==2) {									// <<<�L��IN��ٰ��>>>
				wcd->pfg = 1;											// �����ς�
				wcp = wcd->cpp, bn = 0;
				while(wcp!=NULL) {
					pb[bn++] = wcp->pnt;								// IN��ٰ�ߒ��_���o
					wcp = wcp->next;									// ���߲����
				}
				ap = 0, bp = 0, ln1 = DBL_MAX;							// pa,pb�ԍŒZ�����_����
				for(i=0; i<an; i++) {
					for(j=0; j<bn; j++) {
						ln2 = PntD(&pa[i], &pb[j]);
						if(ln2<ln1) {
							ap = i, bp = j, ln1 = ln2;
						}
					}
				}
				for(i=0; i<an; i++) {									// pa�̍ŒZ�_���擪��
					pc[i] = pa[ap++];									// �Ȃ�悤�ɕ���
					if(ap>an-1) {
						ap = 0;
					}
				}
				for(i=0; i<an; i++) {
					pa[i] = pc[i];
				}
				pa[an++] = pa[0];										// pa�̍Ō�Ɏn�_��ǉ�
				for(i=0; i<bn; i++) {									// pb�̍ŒZ�_���擪��
					pc[i] = pb[bp++];									// �Ȃ�悤�ɕ���
					if(bp>bn-1) {
						bp = 0;
					}
				}
				for(i=0; i<bn; i++) {
					pb[i] = pc[i];
				}
				pb[bn++] = pb[0];										// pb�̍Ō�Ɏn�_�ǉ�
				for(i=0; i<bn; i++) {
					pa[an++] = pb[i];									// pa��pb�ǉ�
				}
			}
			wcd = wcd->next;											// ���߲����
		}
		*pn = an;
	}
	delete[] pb;
	delete[] pc;
}

/*****************************************************************/
static int CheckCutFacePoint(int pn, PNTTYPE* pa, VECTYPE vv,
                             int d, int* f1, int* f2, double* th)		//<<< �����ʍĕ����m�F
/*****************************************************************/
{
	PNTTYPE pt[3];
	VECTYPE v;
	int     i, j, j1, ch1, ch2, mod, cnt=0;
	double  th1, th2, th3, th4, ln1, ln2;

	for(i=0; i<pn; i++) {
		if(i==0) {														// �A������3�_���o
			pt[0] = pa[pn-1], pt[1] = pa[i], pt[2] = pa[1];
		} else if(i==pn-1) {
			pt[0] = pa[pn-2], pt[1] = pa[i], pt[2] = pa[0];
		} else {
			pt[0] = pa[i-1], pt[1] = pa[i], pt[2] = pa[i+1];
		}
		CalcRotAngleVec(pt, &th1, &v);									// 3�_�Ԋp�x�擾
		ch1 = 1;														// �׸ޏ�����(���Ό�)
		if(vv.p[d]<0&&v.p[d]<0) {										// �ʂ̖@���޸�ق�
			ch1 = 0;													// ��������̏ꍇ��0
		}
		if(vv.p[d]>0&&v.p[d]>0) {
			ch1 = 0;
		}
		if(th1<PI/60.0) {												// �p�x3�x������0
			ch1 = 0;
		}
		if(ch1) {														// <<�p�x���t�����_>>
			mod = 0;
			th2 = (PI+th1)/2.0-th1;										// ��̊p�x
			ln1 = DBL_MAX;
			ch2 = 0;
RECHECK:
			for(j=0, j1=0; j<pn; j++) {
				if(j!=i) {
					pt[2] = pa[j];										// �Ώۓ_�ݒ�
					CalcRotAngleVec(pt, &th3, &v);						// �Ώۓ_�Ƃ̊p�x�擾
					if((vv.p[d]<0&&v.p[d]<0)||(vv.p[d]>0&&v.p[d]>0)) {	// <�@��������v>
						if(!mod) {										// Ӱ��:0�͊�`60�x����
							th4 = PI/3.0;
						} else {										// Ӱ��:1�͊�`�ΏۗŐ���-5�x
							th4 = (PI+th1)/2.0-PI/36.0;
						}
						if(fabs(th3-th2)<th4) {							// <��p�x�����w��l����>
							ln2 = PntD(&pt[1], &pa[j]);					// ��_�Ƃ̋����Z�o
							if(ln2<ln1) {								// �ŒZ�����ް��ۑ�
								j1 = j, ln1 = ln2, ch2 = 1;
							}
						}
					}
				}
			}
			if(ch2) {													// �������ԍ��ݒ�
				f1[cnt] = i, f2[cnt] = j1;
				th[cnt++] = th2;
			} else if(!mod) {											// �ް��Ȃ��͍�����
				mod = 1;
				goto RECHECK;
			}
		}
	}
	return cnt;
}

/***************************************************/
static void SetVlData(int fg1, int fg2, PNTTYPE pt)						//<<< �}�`���Z���_�ݒ�
/***************************************************/
{
	BVLTYPE* tv;

	tv = (BVLTYPE *)malloc(sizeof(BVLTYPE));							// �}�`���Z���_�̈�m��
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->pt = pt, tv->vt = -1, tv->lfg = 0, tv->pfg = 0;
	if(!fg1) {															// <<A��>>
		if(!fg2) {														// <�O��>
			tv->next = A0list, tv->no = A0cnt++, A0list = tv;
		} else {														// <����>
			tv->next = A1list, tv->no = A1cnt++, A1list = tv;
		}
	} else {															// <<B��>>
		if(!fg2) {														// <�O��>
			tv->next = B0list, tv->no = B0cnt++, B0list = tv;
		} else {														// <����>
			tv->next = B1list, tv->no = B1cnt++, B1list = tv;
		}
	}
}

/******************************************/
static void SetVlDataEnd(int fg1, int fg2)								//<<< �}�`���Z��ٰ�ߏI���ݒ�
/******************************************/
{
	if(!fg1) {															// <<A��>>
		if(!fg2) {														// <�O��>
			A0list->lfg = 1;
		} else {														// <����>
			A1list->lfg = 1;
		}
	} else {															// <<B��>>
		if(!fg2) {														// <�O��>
			B0list->lfg = 1;
		} else {														// <����>
			B1list->lfg = 1;
		}
	}
}

/*********************************************************************/
static double CalcTriangleArea(PNTTYPE* pa, PNTTYPE* pb, PNTTYPE* pc)	//<<< �O�p�`�̖ʐώZ�o
/*********************************************************************/
{
	double a, b, c;

	a = PntD(pa, pb);													// 3�ӂ̒���
	b = PntD(pb, pc);
	c = PntD(pc, pa);
	return CalcArea(a, b, c);											// �O�p�`�̖ʐ�
}

/*****************************************************************/
static int CalcTrianglePoint(PNTTYPE* pa, int pn, int ix, int pm)		//<<< �O�p�`���o�ʒu�Z�o
/*****************************************************************/
{
	double* th = new double[View->m_MaxOPnt];
	double* sq = new double[View->m_MaxOPnt];
	PNTTYPE wp[3];
	int     i, j, st, ed, ch, ret=0;
	double  t1, t2, s1, s2, s3;

	for(i=0; i<pn; i++) {
		th[i] = 0.0, sq[i] = 0.0;										// ��Ɨ̈揉����
		st = (i==0) ? pn-1 : i-1;										// �Ώۓ_�̑O�_
		ed = (i==pn-1) ? 0 : i+1;										// �Ώۓ_�̌�_
		wp[0] = pa[st], wp[1] = pa[i], wp[2] = pa[ed];					// �Ώۂ̎O�p�`
		t1 = CalcAngle(wp, ix, pm);										// �Ώۓ_�̂Ȃ��p�x
		if(t1>0.0) {													// <���ް�ݸނ��Ă��Ȃ��_>
			s1 = CalcTriangleArea(&pa[st], &pa[i], &pa[ed]);			// �O�p�`�̖ʐώZ�o 
			for(ch=1, j=0; j<pn; j++) {
				if(j!=st&&j!=i&&j!=ed) {								// �Ώۂ̎O�p�`�ȊO�̓_
					wp[2] = pa[j];										// �O�_�ƑΏۓ_��
					t2 = CalcAngle(wp, ix, pm);							// ���̑��̓_�̂Ȃ��p�x
					if(t2>t1) {											// <�Ώۓ_�p�x���傫��>
						s2 = CalcTriangleArea(&pa[st], &pa[i], &pa[j]);	// �O�_-�Ώۓ_-���̑��̓_�̖ʐ�
						s3 = CalcTriangleArea(&pa[i], &pa[ed], &pa[j]);	// �Ώۓ_-��_-���̑��̓_�̖ʐ�
						if(s1>s2+s3) {									// �ΏێO�p�`���ɂ��̑��̓_�͖���
							ch = 0; break;
						}
					}
				}
			}
			if(ch) {													// �L���ȎO�p�`
				th[i] = t1, sq[i] = s1;
			}
		}
	}
	t1 = PI/6.0, t2 = PI*5.0/6.0;										// �L���͈́F30�x�`150�x
	for(i=0; i<2; i++) {
		for(ch=0, s1=0.0, j=0; j<pn; j++) {
			if(th[j]>t1&&th[j]<t2) {									// <�L���͈͓�>
				if(sq[j]>s1) {											// �ő�̖ʐς�����
					ret = j, s1 = sq[j], ch = 1;
				}
			}
		}
		if(ch) {														// ����OK�͏I��
			break;
		}
		t1 = 0.0, t2 = PI;												// 2�x�ڂ͗L���͈́F0�x�`180�x
	}
	delete[] th;
	delete[] sq;
	return ret;
}

/*********************************************************************/
static void SetCutFace(int fg, VECTYPE vv, int ix,
					   PNTTYPE* pa, int pn, BFATYPE* fp, OBJTYPE* op)	//<<< �����ʏ��ݒ�
/*********************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE  wp[3], pt, mi, ma, ip;
	VECTYPE  vc;
	BFATYPE* wfp;
	int      i, j, nn, st, ed, pm, tp, uct, dct, flg;

	for(j=0; j<DIM; j++) {
		pt.p[j] = DBL_MAX;												// ��r�̈揉����
	}
	for(nn=0, i=0; i<pn; i++) {
		for(st=0, j=0; j<DIM; j++) {
			if(fabs(pa[i].p[j]-pt.p[j])>EPSILON) {						// ���ꂩ���قȂ��OK
				st = 1; break;
			}
		}
		if(st) {														// �O�_�ƈقȂ�Ώo��
			pp[nn++] = pa[i];
		}
		pt = pa[i];														// ��r�̈�X�V
	}
	for(st=0, j=0; j<DIM; j++) {										// �n�_�ƏI�_�̊m�F
		if(fabs(pp[0].p[j]-pp[nn-1].p[j])>EPSILON) {					// ���ꂩ���قȂ��OK
			st = 1; break;
		}
	}
	if(!st) {															// �����ꍇ�͒��_����
		nn--;
	}
	if(nn<3) {
		delete[] pp;
		return;
	}
	CalcNVec(pp, nn, &vc);												// �@���޸�َ擾
	if((vv.p[ix]>0&&vc.p[ix]<0)||(vv.p[ix]<0&&vc.p[ix]>0)) {			// <�޸�ق̌������t>
		st = 0;
		ed = nn-1;
		while(st<ed) {
			pt = pp[st], pp[st] = pp[ed], pp[ed] = pt, ed--, st++;		// ���_���]
		}
	}
	pm = (vv.p[ix]>0.0) ? 0 : 1;										// �@���ő�����̐����׸�
	CalcInPoint(pp, nn, &mi, &ma, &ip);									// �ʓ����ݓ_�擾
	wfp = fp, uct = 0, dct = 0, flg = 0;
	while(wfp!=NULL) {
		if(ma.p[0]<wfp->xmin) {											// X�ő���ŏ�����͏I��
			break;
		}
		if(ip.p[0]>=wfp->xmin&&ip.p[0]<=wfp->xmax&&
           ip.p[1]>=wfp->ymin&&ip.p[1]<=wfp->ymax) {					// <�ʓ��ɑ��ݓ_�L>
			i = CheckInFace(ip, wfp, op);								// �ʓ���������
			if(i>0) {													// �ʓ��̏�ɑ���
				uct++;
			} else if(i<0) {											// �ʓ��̉��ɑ���
				dct++;
			}
		}
		wfp = wfp->next;												// ���߲����
	}
	uct = uct%2, dct = dct%2;											// �ʂ�2��1���
	if(uct&&dct) {														// ���̓��ɑ���
		flg = 1;
	}
	while(nn>=3) {
		tp = CalcTrianglePoint(pp, nn, ix, pm);							// �O�p�`���o�ʒu�Z�o
		st = (tp==0) ? nn-1 : tp-1;										// ���o�ʒu�̑O�_
		ed = (tp==nn-1) ? 0 : tp+1;										// ���o�ʒu�̌�_
		wp[0] = pp[st], wp[1] = pp[tp], wp[2] = pp[ed];					// ���o�Ώۂ̎O�p�`
		for(i=0; i<3; i++) {
			SetVlData(fg, flg, wp[i]);									// �}�`���Z���_�ݒ�
		}
		SetVlDataEnd(fg, flg);											// �}�`���Z��ٰ�ߏI���ݒ�
		for(i=tp; i<nn-1; i++) {
			pp[i] = pp[i+1];											// ���o�_�ȍ~��O�l��
		}
		nn--;															// �����ʒ��_�����Z
	}
	delete[] pp;
}

/****************************************************************/
static void SetInOutCF(int fg, VECTYPE vv, int ix, PNTTYPE* pa,
                       int pn, BFATYPE* fp, OBJTYPE* op, int lv)		//<<< ���O�����ʂ̐ݒ�
/****************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxBPnt];
	int*     f1 = new int[View->m_MaxBPnt];
	int*     f2 = new int[View->m_MaxBPnt];
	double*  th = new double[View->m_MaxBPnt];
	int      i, i1, i2, j, cnt, wn, st, ed, ct1, ct2, ct3, er1, er2, stp;
	double   sth;

	while(TRUE) {
		cnt = CheckCutFacePoint(pn, pa, vv, ix, f1, f2, th);			// �����ʍĕ����m�F
		if(cnt==0||lv>100) {											// <�ʂ̕�����/���ُ��>
			SetCutFace(fg, vv, ix, pa, pn, fp, op);						// �����ʂ̐ݒ�
			break;														// �����I��
		} else {														// <�ʂ̕����L>
			wn = 0, i1 = 0, i2 = 0, stp = 1;
			ct3 = INT_MAX;
			for(i=0; i<cnt; i++) {
				if(f1[i]<f2[i]) {										// �ł��[�ōł������
					st = f1[i], ed = f2[i];								// ���Ȃ�����������
				} else {
					st = f2[i], ed = f1[i];
				}
				for(j=0, ct1=0, ct2=0, er1=0, er2=0; j<cnt; j++) {
					if(j!=i) {
						if(f1[j]<st||f1[j]>ed) {
							er1 = 1;
						}
						if(f2[j]<st||f2[j]>ed) {
							ct1++;
						}
						if(f1[j]>st&&f1[j]<ed) {
							er2 = 1;
						}
						if(f2[j]>st&&f2[j]<ed) {
							ct2++;
						}
						if(er1&&er2) {
							break;
						}
					}
				}
				if((!er1)&&ct1<ct3) {									// �����������ݒ�
					i1 = i, i2 = 0, ct3 = ct1, stp = 0;
				}
				if((!er2)&&ct2<ct3) {
					i1 = i, i2 = 1, ct3 = ct2, stp = 0;
				}
			}
			if(stp) {													// <�������������Ȃ�>
				for(sth=PI, i=0, i1=0; i<cnt; i++) {
					if(sth>th[i]) {										// ��p�x�̍ŏ�����
						i1 = i, sth = th[i];
					}
				}
				if(f1[i1]<f2[i1]) {										// ������
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				for(wn=0, i=0; i<=st; i++) {
					pp[wn++] = pa[i];
				}
				for(i=ed; i<pn; i++) {
					pp[wn++] = pa[i];
				}
				if(wn>2) {
					SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);		// ���O�����ʂ̐ݒ�
				}
				for(wn=0, i=st; i<=ed; i++) {
					pp[wn++] = pa[i];
				}
				if(wn>2) {
					SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);		// ���O�����ʂ̐ݒ�
				}
				break;													// �����I��
			} else {													// <�����������L>
				if(f1[i1]<f2[i1]) {										// ������
					st = f1[i1], ed = f2[i1];
				} else {
					st = f2[i1], ed = f1[i1];
				}
				if(!i2) {												// ���񕪊��ʐݒ�
					for(i=0; i<=st; i++) {
						pp[wn++] = pa[i];
					}
					for(i=ed; i<pn; i++) {
						pp[wn++] = pa[i];
					}
				} else {
					for(i=st; i<=ed; i++) {
						pp[wn++] = pa[i];
					}
				}
				if(wn>2) {												// <�L�������ʂ̏ꍇ>
					SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);		// ���O�����ʂ̐ݒ�
					if(!i2) {											// ���񕪊��ʐݒ�
						for(wn=0, i=st; i<=ed; i++) {
							pp[wn++] = pa[i];
						}
					} else {
						for(wn=0, i=0; i<=st; i++) {
							pp[wn++] = pa[i];
						}
						for(i=ed; i<pn; i++) {
							pp[wn++] = pa[i];
						}
					}
					for(i=0; i<wn; i++) {
						pa[i] = pp[i];
					}
					pn = wn;
				} else {												// <���������ʂ̏ꍇ>
					for(sth=PI, i=0, i1=0; i<cnt; i++) {
						if(sth>th[i]) {									// ��p�x�̍ŏ�����
							i1 = i, sth = th[i];
						}
					}
					if(f1[i1]<f2[i1]) {									// ������
						st = f1[i1], ed = f2[i1];
					} else {
						st = f2[i1], ed = f1[i1];
					}
					for(wn=0, i=0; i<=st; i++) {
						pp[wn++] = pa[i];
					}
					for(i=ed; i<pn; i++) {
						pp[wn++] = pa[i];
					}
					if(wn>2) {
						SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);	// ���O�����ʂ̐ݒ�
					}
					for(wn=0, i=st; i<=ed; i++) {
						pp[wn++] = pa[i];
					}
					if(wn>2) {
						SetInOutCF(fg, vv, ix, pp, wn, fp, op, lv+1);	// ���O�����ʂ̐ݒ�
					}
					break;												// �����I��
				}
			}
		}
	}
	delete[] pp;
	delete[] f1;
	delete[] f2;
	delete[] th;
}

/*******************************************************************/
static void SetInOut(int fg, VECTYPE vv, int ix,
                     PNTTYPE* pa, int pn, BFATYPE* fp, OBJTYPE* op)		//<<< ���O�����ʂ̐ݒ�
/*******************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxBPnt];
	PNTTYPE  pt;
	VECTYPE  vc;
	int      i, st, ed, wn;

	CalcNVec(pa, pn, &vc);												// �@���޸�َ擾
	if((vv.p[ix]>0&&vc.p[ix]<0)||(vv.p[ix]<0&&vc.p[ix]>0)) {			// <�޸�ق̌������t>
		st = 0, ed = pn-1;
		while(st<ed) {
			pt = pa[st], pa[st] = pa[ed], pa[ed] = pt, ed--, st++;		// ���_���]
		}
	}
	st = 0;
	while(TRUE) {
		ed = (st+View->m_MaxBPnt<pn) ? st+View->m_MaxBPnt : pn;			// ����ݒ�͈�(�I��)
		for(wn=0, i=st; i<ed; i++) {
			pp[wn++] = pa[i];											// ����ݒ��ް�����
		}
		SetInOutCF(fg, vv, ix, pp, wn, fp, op, 0);						// ���O�����ʂ̐ݒ�
		if(ed==pn) {													// �I��
			break;
		}
		st = ed;														// �I���͈͕ۑ�(����J�n)
	}
	delete[] pp;
}

/*****************************************************************/
static void MakeCutFace(int fg, BFATYPE* fp1,
                        BFATYPE* fp2, OBJTYPE* op1, OBJTYPE* op2)		//<<< �����ʕ���
/*****************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	int*     va = new int[View->m_MaxOPnt];
	short*   sf = new short[View->m_MaxOPnt];
	BCDTYPE* wc1, * wc2;
	BCPTYPE* wcp;
	PNTTYPE  pt, mi, ma, ip;
	VECTYPE  v1, v2;
	int      ix, i, j, i1, i2, pn, ed, st, vn, flg, eln, stp, mx=0;
	double   d;

	v1 = fp1->vec;														// �w��ʖ@���޸��
	d = fabs(v1.p[0]);													// �޸�ٍő�����擾
	ix = 0, i1 = 1, i2 = 2;
	if(d<fabs(v1.p[1])) {
		d = fabs(v1.p[1]);
		ix = 1, i1 = 2, i2 = 0;
	}
	if(d<fabs(v1.p[2])) {
		ix = 2, i1 = 0, i2 = 1;
	}
	i = fp1->stfl;														// �Ώە���ٰ�ߊJ�n�ԍ�
	while(TRUE) {
		GetFlp1(op1, i, &vn, &flg);										// �Ώە��ʒ��_�ԍ��擾
		va[mx] = vn;													// ���_�ԍ��ۑ�
		sf[mx++] = 0;													// �����׸ޏ�����
		if(flg) {														// ٰ�ߏI��
			break;
		}
		i++;
	}
	if(Cicnt>0) {														// <<<IN��ٰ�ߗL>>>
		wc1 = Cdlist;
		while(wc1!=NULL) {
			if(wc1->flg) {												// <<<IN��ٰ�߂̏ꍇ>>>
				wcp = wc1->cpp, pn = 0;
				while(wcp!=NULL) {
					pa[pn++] = wcp->pnt;								// IN��ٰ�ߒ��_���o
					wcp = wcp->next;									// ���߲����
				}
				if(pn>2) {												// <<�L�����_���L>>
					CalcNVec(pa, pn, &v2);								// �@���޸�َ擾
					if((v1.p[ix]>0&&v2.p[ix]>0)||
                       (v1.p[ix]<0&&v2.p[ix]<0)) {						// <�޸�ق�����>
						SetInOut(fg, v1, ix, pa, pn, fp2, op2);			// ���O�����ʂ̐ݒ�
					} else {											// <�޸�ق��قȂ�>
						CalcInPoint(pa, pn, &mi, &ma, &ip);				// �ʓ����ݓ_�擾
						wc1->ept = ip;									// �擾�_�ۑ�
					}
					st = 0, ed = pn-1;
					while(st<ed) {
						pt = pa[st], pa[st] = pa[ed], pa[ed] = pt;		// ���_���]
						ed--, st++;
					}
					CalcNVec(pa, pn, &v2);								// �@���޸�ٍĎ擾
					if((v1.p[ix]>0&&v2.p[ix]>0)||
                       (v1.p[ix]<0&&v2.p[ix]<0)) {						// <�޸�ق�����>
						SetInOut(fg, v1, ix, pa, pn, fp2, op2);			// ���O�����ʂ̐ݒ�
					} else {											// <�޸�ق��قȂ�>
						wcp = wc1->cpp, pn = 0;
						while(wcp!=NULL) {								// ���͓��������Ȃ̂�
							wcp->pnt = pa[pn++];						// IN��ٰ�ߒ��_�Đݒ�
							wcp = wcp->next;							// ���߲����
						}
						CalcInPoint(pa, pn, &mi, &ma, &ip);				// �ʓ����ݓ_�擾
						wc1->ept = ip;									// �擾�_�ۑ�
					}
				} else {												// <<�L�����_����>>
					wc1->pfg = 1, Cicnt--;
				}
			}
			wc1 = wc1->next;											// ���߲����
		}
	}
	if(Cocnt>0) {														// <��ٰ��,�ڑ���ٰ�ߌ���>
		i = fp1->stfl;													// �Ώە���ٰ�ߊJ�n�ԍ�
		while(TRUE) {
			GetFlpVtx1(op1, i, &pt, &flg);								// �Ώە��ʒ��_���W�擾
			j = (!flg) ? i+1 : fp1->stfl;
			while(TRUE) {
				if(CheckOutLoop(pt, i, j, &wc1, &wc2)) {				// OUTٰ���ް��m�F
					if(wc1==wc2) {										// <��ٰ�߂̏ꍇ>
						wc1->pfg = 1, Cocnt--;							// �����ς�
						wcp = wc1->cpp, pn = 0;
						while(wcp!=NULL) {
							pa[pn++] = wcp->pnt;						// ��ٰ�ߒ��_���o
							wcp = wcp->next;							// ���߲����
						}
						if(pn>2) {
							if(Cicnt>0) {
								CheckInLoop(i1, i2, pa, &pn);			// INٰ�ߊm�F
							}
							SetInOut(fg, v1, ix, pa, pn, fp2, op2);		// ���O�����ʐݒ�
						}
					} else {											// <�ڑ�ٰ�߂̏ꍇ>
						wc1->esn = wc2->esn;							// wc1��wc2��ڑ����邽��
						wc1->een = wc2->een;							// wc2�̏I�_���wc1����
						wc1->ept = wc2->ept;
						if(wc1->cpp==NULL) {							// wc2�̕������_��
							wc1->cpp = wc2->cpp;						// wc1�̖����Ɉړ�
						} else {
							wcp = wc1->cpp;
							while(wcp->next!=NULL) {
								wcp=wcp->next;
							}
							wcp->next = wc2->cpp;
						}
						wc2->pfg = 1, Cocnt--;							// wc2�͏����ς�
						wc2->cpp = NULL;								// wc2�̕������_�͖�
					}
				} else {
					break;
				}
			}
			if(flg) {													// ٰ�ߏI���ŏI������
				break;
			}
			i++;														// �����_��
		}
	}
	while(TRUE) {
		eln = 0, stp = 0, pn = 0;										// ��Ɨ̈揉����
		while(TRUE) {
			wcp = NULL;
			for(i=eln; i<mx; i++) {
				if(!sf[i]) {											// <�������̏ꍇ>
					sf[i] = 1;											// �����ς�
					GetVtx(op1, va[i], &pa[pn++]);						// ���_���W�擾
					j = fp1->stfl + i;									// ��ٰ�ߔԍ�
					if(CheckCdData(j, &eln, &wcp)) {					// �������擪�ް��m�F
						eln = eln-fp1->stfl; break;						// ����̊J�n��ٰ�ߔԍ�
					}
				} else {												// <�����ς̏ꍇ>
					if(pn>0) {											// �ް��ݒ�ς݂͏I��
						stp = 1; break;
					}
				}
			}
			if(i>=mx||stp) {
				break;
			}
			while(wcp!=NULL) {											// <�������L�̏ꍇ>
				pa[pn++] = wcp->pnt;									// ���������_�擾
				wcp = wcp->next;										// ���߲����
			}
		}
		if(pn>0) {														// <�L�����_�L>
			if(Cicnt>0) {
				CheckInLoop(i1, i2, pa, &pn);							// INٰ�ߊm�F
			}
			SetInOut(fg, v1, ix, pa, pn, fp2, op2);						// ���O�����ʂ̐ݒ�
		} else {														// <�L�����_��>
			break;
		}
	}
	delete[] pa;
	delete[] va;
	delete[] sf;
}

/****************************************************************/
static void SetInOutFace(int fg1, int fg2, OBJTYPE* op, int sno)		//<<< ���O�ʏ��̐ݒ�
/****************************************************************/
{
	PNTTYPE pt;
	int     ix, ffg;

	ix = sno;															// ��ٰ�ߊJ�n�ԍ�
	while(TRUE) {
		GetFlpVtx1(op, ix, &pt, &ffg);									// ��ٰ�ߒ��_�擾
		SetVlData(fg1, fg2, pt);										// �}�`���Z���_�ݒ�
		if(ffg) {														// ��ٰ�ߏI���͏����I��
			break;
		}
		ix++;
	}
	SetVlDataEnd(fg1, fg2);												// �}�`���Z��ٰ�ߏI���ݒ�
}

/***************************************************************/
static void MakeBooleanList(int fg, OBJTYPE* op1, OBJTYPE* op2)			//<<< �}�`���Zؽč쐬
/***************************************************************/
{
	BFATYPE* fl1, * fl2, * wfp;
	BCLTYPE* wcl, * dcl;
	BCDTYPE* wcd, * dcd;
	BCPTYPE* wcp, * dcp;
	PNTTYPE  pt;
	int      num, cnt=0, ipr=0, cflg, flg, uct, dct, ret;
	double   dpr;

	if(!fg) {															// <A��>
		fl1 = Aflist, fl2 = Bflist, num = Afcnt;
	} else {															// <B��>
		fl1 = Bflist, fl2 = Aflist, num = Bfcnt;
	}
	while(fl1!=NULL) {
		cflg = 0, flg = 0, uct = 0, dct = 0;							// ��Ɨ̈揉����
		Cllist = NULL, pt = fl1->inpt, wfp = fl2;
		while(wfp!=NULL) {
			if(fl1->xmax<wfp->xmin) {									// X�ő���ŏ�����͏I��
				break;
			}
			if(fl1->xmin<=wfp->xmax&&
               fl1->ymin<=wfp->ymax&&fl1->ymax>=wfp->ymin) {			// <<�ʂ�X-Y�̈悪����>>
				if(fl1->zmin<=wfp->zmax&&fl1->zmax>=wfp->zmin) {		// <�ʂ�Z�̈悪����>
					if(CheckCrossFace(fl1, wfp, op1, op2)) {			// �ʌ�������
						cflg = 1;										// �������׸�ON
					}
				}
				if(!cflg&&
                   pt.p[0]>=wfp->xmin&&pt.p[0]<=wfp->xmax&&
                   pt.p[1]>=wfp->ymin&&pt.p[1]<=wfp->ymax) {			// <�������Ŗʓ��ɓ_�L>
					ret = CheckInFace(fl1->inpt, wfp, op2);				// �ʓ���������
					if(ret>0) {											// �ʓ��̏�ɑ���
						uct++;
					} else if(ret<0) {									// �ʓ��̉��ɑ���
						dct++;
					}
				}
			}
			wfp = wfp->next;											// ���߲����
		}
		if(cflg) {														// <<�����ʂ̏ꍇ>>
			MakeCutLine(fl1, op1);										// �����ʕ������쐬
			wcl = Cllist;
			while(wcl) {												// �ʌ����Ő��̈���
				dcl = wcl, wcl = wcl->next;
				free(dcl);
			}
			MakeCutFace(fg, fl1, fl2, op1, op2);						// �����ʕ���
			wcd = Cdlist;
			while(wcd) {
				wcp = wcd->cpp;
				while(wcp) {											// ���������_�̈���
					dcp = wcp, wcp = wcp->next;
					free(dcp);
				}
				dcd = wcd, wcd = wcd->next;
				free(dcd);												// ���������擪�̈���
			}
		} else {														// <<�������ʂ̏ꍇ>>
			uct = uct%2, dct = dct%2;									// �ʂ�2��1���
			if(uct&&dct) {												// ���̓��ɑ���
				flg = 1;
			}
			SetInOutFace(fg, flg, op1, fl1->stfl);						// ���O�ʏ��ݒ�
		}
		cnt++;															// ��������UP
		dpr = (Ppr/6.0)*((double)cnt/(double)num);						// ���݂��߰���
		if(ipr<(int)(Spr+dpr)) {										// <<�߰���UP>>
			ipr = (int)(Spr+dpr);
			m_pProgressB->SetProgress(ipr);								// �i���\��
		}
		fl1 = fl1->next;												// ���߲����
	}
	Spr += Ppr/6.0;														// �ݐ��߰���
}

/***************************/
static void ReverseB1(void)												//<<< B1��ٰ�ߔ��]
/***************************/
{
	BVLTYPE* wvl1, * wvl2;
	int      cnt=0;

	wvl1 = B1list, wvl2 = NULL;
	while(wvl1!=NULL) {
		wvl1->no = cnt++;												// ��ٰ�ߔԍ��t�ݒ�
		if(wvl1->lfg) {													// <����ٰ�ߍŏI>
			wvl1->lfg = 0;												// �ŏI����
			if(wvl2!=NULL) {											// ���O���ŏI��
				wvl2->lfg = 1;
			}
		}
		wvl2 = wvl1;													// ���O�Ƃ��ĕۑ�
		wvl1 = wvl1->next;												// ���߲����
	}
	if(wvl2!=NULL) {													// �Ō���ŏI��
		wvl2->lfg = 1;
	}
}

/*******************************************************************/
static void ConnectVlData(BVLTYPE** list1, BVLTYPE* list2, int cnt)		//<<< ��ٰ�ߐڑ�
/*******************************************************************/
{
	BVLTYPE* wvl;

	wvl = *list1;
	if(wvl!=NULL) {														// <ؽ�1���L��>
		while(wvl->next!=NULL) {										// ���߲����NULL����
			wvl = wvl->next;
		}
		wvl->next = list2;												// ؽ�2�ڑ�
	} else {															// <ؽ�1������>
		*list1 = list2;													// ؽ�2ؽ�1��
	}
	wvl = list2;
	while(wvl!=NULL) {													// ؽ�2��
		wvl->no = wvl->no+cnt;											// ��ٰ�ߔԍ����Z
		wvl = wvl->next;												// ���߲����
	}
}

/********************************************************/
static BVLTYPE* MergeV(BVLTYPE* la, BVLTYPE* lb, int id)				//<<< ���_���W���
/********************************************************/
{
	BVLTYPE* lc, * ld, * rc;

	lc = (BVLTYPE *)malloc(sizeof(BVLTYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[id]<=lb->pt.p[id]) {								// ���_���W�̑召�Ŕ�r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/********************************************************/
static BVLTYPE* V_Mergsrt(BVLTYPE* mlist, int n, int id)				//<<< ���_���W���
/********************************************************/
{
	BVLTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BVLTYPE *)MergeV(V_Mergsrt(la, n/2, id),
                               V_Mergsrt(lb, n-n/2, id), id);			// ���_���W���
		rc = lm;
	}
	return rc;
}

/*************************************************/
static BVLTYPE* MergeVn(BVLTYPE* la, BVLTYPE* lb)						//<<< ��ٰ�ߔԍ����
/*************************************************/
{
	BVLTYPE* lc, * ld, * rc;

	lc = (BVLTYPE *)malloc(sizeof(BVLTYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// ��ٰ�ߔԍ��̑召�Ŕ�r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/*************************************************/
static BVLTYPE* V_Mergsrtn(BVLTYPE* mlist, int n)						//<<< ��ٰ�ߔԍ����
/*************************************************/
{
	BVLTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BVLTYPE *)MergeVn(V_Mergsrtn(la, n/2),
                                V_Mergsrtn(lb, n-n/2));					// ��ٰ�ߔԍ����
		rc = lm;
	}
	return rc;
}

/**************************************/
static BOOL CheckLoopVtx(BVLTYPE* bvl)									//<<< ��ٰ�ߒ��_�d���m�F
/**************************************/
{
	BVLTYPE* wvp, * bv1, * bv2, * sv1, * sv2;
	int      s1, s2, n1, n2;

	wvp = bvl, s1 = -1, n1 = -1, sv1 =NULL;
	while(wvp!=NULL) {
		if(!wvp->pfg) {													// <�L���ް�>
			if(s1==-1) {												// �擪�L�����_No�ۑ�
				s1 = wvp->vt, sv1 = wvp;
			}
			if(wvp->vt==n1) {											// ���O�_�Ɠ��꒸�_�͖���
				wvp->pfg = 1;
				return TRUE;
			}
			n1 = wvp->vt;												// ���񒸓_�𒼑O�_�Ƃ��ĕۑ�
		}
		if(wvp->lfg) {													// �Y���ʂ̏I��
			break;
		}
		wvp = wvp->next;
	}
	if(n1!=-1&&n1==s1) {												// �擪�ƍŏI�̊m�F
		sv1->pfg = 1;
		return TRUE;
	}
	wvp = bvl, s1 = -1, s2 = -1, n1 = -1, n2 = -1;
	sv1 = NULL, sv2 = NULL, bv1 = NULL, bv2 = NULL;
	while(wvp!=NULL) {
		if(!wvp->pfg) {													// <�L���ް�>
			if(s2==-1) {												// �擪2�L�����_No�ۑ�
				s2 = s1, s1 = wvp->vt, sv2 = sv1, sv1 = wvp;
			}
			if(wvp->vt==n2) {											// 2�_�O�Ɠ��꒸�_�͖���
				wvp->pfg = 1, bv1->pfg = 1;
				return TRUE;
			}
			n2 = n1, n1 = wvp->vt, bv2 = bv1, bv1 = wvp;				// ���񒸓_�𒼑O�_�Ƃ��ĕۑ�
		}
		if(wvp->lfg) {													// �Y���ʂ̏I��
			break;
		}
		wvp = wvp->next;
	}
	if(n2!=-1&&n2==s2) {												// �ŏI-1�Ɛ擪�̊m�F
		bv1->pfg = 1, sv2->pfg = 1;
		return TRUE;
	}
	if(n1!=-1&&n1==s1) {												// �ŏI�Ɛ擪+1�̊m�F
		sv1->pfg = 1, sv2->pfg = 1;
		return TRUE;
	}
	return FALSE;
}

/**************************************************************/
static void RemakeObject(int mode, int atr, int col, int* gno)			//<<< �}�`���Z���̍쐬
/**************************************************************/
{
	BVLTYPE* list, * wvp, * svp, * nvp, * evp;
	BVTTYPE* vtl=NULL, * bvt=NULL, * tvt;
	OBJTYPE* op;
	PNTTYPE  pt;
	int      i, cnt, vct=0, lct=0;

	if(mode==0) {														// <<A-B�̏ꍇ>>
		ReverseB1();													// B1��ٰ�ߔ��]
		ConnectVlData(&A0list, B1list, A0cnt);							// ��ٰ�ߐڑ�(A0+B1)
		B1list = NULL;													// B1�߲���ر
		list = A0list, cnt  = A0cnt + B1cnt;
	} else if(mode==1) {												// <<A+B�̏ꍇ>>
		ConnectVlData(&A0list, B0list, A0cnt);							// ��ٰ�ߐڑ�(A0+B0)
		B0list = NULL;													// B0�߲���ر
		list = A0list, cnt  = A0cnt + B0cnt;
	} else if(mode==2) {												// <<A�~B�̏ꍇ>>
		ConnectVlData(&A1list, B1list, A1cnt);							// ��ٰ�ߐڑ�(A1+B1)
		B1list = NULL;													// B1�߲���ر
		list = A1list, cnt  = A1cnt + B1cnt;
	}
	if(cnt==0) {														// �L���ް��Ȃ��͒��~
		return;
	}
	wvp = list;
	while(wvp!=NULL) {
		for(i=0; i<DIM; i++) {
			wvp->pt.p[i] -= ReszB;										// ���_�ް��߂�
		}
		wvp = wvp->next;
	}
	list = V_Mergsrt(list, cnt, 2);										// ���_���W:Z���
	list = V_Mergsrt(list, cnt, 1);										// ���_���W:Y���
	list = V_Mergsrt(list, cnt, 0);										// ���_���W:X���
	Spr += Ppr/6.0;														// �ݐ��߰���
	m_pProgressB->SetProgress((int)Spr);								// �i���\��
	pt = list->pt;														// �����l�ݒ�
	pt.p[0] -= 1.0;
	wvp = list;
	while(wvp!=NULL) {													// ���K���_������
		if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
           fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
           fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {						// <�ݒ�ςƈقȂ�ꍇ>
			wvp->vt = vct++;											// ���_�ԍ��ۑ�
			pt = wvp->pt;
			tvt = (BVTTYPE *)malloc(sizeof(BVTTYPE));					// ���_�ް��̈�m��
			if(tvt==NULL) {
				MemErr(IDS_MEMERR5);
			}
			tvt->pt = wvp->pt, tvt->vt = wvp->vt, tvt->next = NULL;
			if(bvt==NULL) {												// �擪�ް����߲���ۑ�
				vtl = tvt;
			} else {													// �ȊO�͑O�ް��ɘA��
				bvt->next = tvt;
			}
			bvt = tvt;													// �����߲����ۑ�
		} else {														// ���͒��_�ԍ��ۑ�
			wvp->vt = vct-1;
		}
		wvp = wvp->next;
	}
	list = V_Mergsrtn(list, cnt);										// ��ٰ�ߔԍ����
	Spr += Ppr/6.0;														// �ݐ��߰���
	m_pProgressB->SetProgress((int)Spr);								// �i���\��
	wvp = list, svp = NULL;
	while(wvp!=NULL) {
		if(svp==NULL) {													// ��ٰ�߂̊J�n�ʒu�ۑ�
			svp = wvp;
		}
		if(wvp->lfg) {													// ��ٰ�߂̏I���ʒu
			while(TRUE) {
				if(!CheckLoopVtx(svp)) {								// ��ٰ�ߒ��_�d���m�F
					break;
				}
			}
			nvp = svp, cnt = 0, evp = NULL;
			while(nvp!=NULL) {
				if(!nvp->pfg) {											// �L���ް�����
					cnt++;
					evp = nvp;
				}
				if(nvp->lfg) {											// �Y���ʂ̏I��
					break;
				}
				nvp = nvp->next;
			}
			if(cnt>0&&cnt<3) {											// �L���ް���3������
				nvp = svp;
				while(nvp!=NULL) {
					nvp->pfg = 1;										// �Y���ʂ�S�Ė���
					if(nvp->lfg) {										// �Y���ʂ̏I��
						break;
					}
					nvp = nvp->next;
				}
			} else if(cnt>=3&&evp!=NULL) {								// �L���ŏI�ް�
				evp->lfg = 1;
			}
			svp = NULL;
		}
		wvp = wvp->next;
	}
	wvp = list, cnt = 0;
	while(wvp!=NULL) {
		if(!wvp->pfg) {													// �L���ް�����
			cnt++;
		}
		wvp = wvp->next;
	}
	CreatObj(vct, cnt, 0, &op);											// ���̗̈�m��
	LevelObj(0, op);													// �������ِ������̐ڑ�
	tvt = vtl;
	while(tvt!=NULL) {
		PutVtx(op, tvt->vt, tvt->pt);									// ���_���W�ݒ�
		tvt = tvt->next;
	}
	wvp = list;
	while(wvp!=NULL) {
		if(!wvp->pfg) {
			PutFlp1(op, lct++, wvp->vt, wvp->lfg);						// ��ٰ��1�ݒ�
		}
		wvp = wvp->next;
	}
	if(atr==1) {														// <A�����̑�����JEWEL>
		PutObjAtr(op, 1);												// ����No�ݒ�(DIRECT[GEM])
	} else {															// <A�����̑�����JEWEL�ȊO>
		PutObjAtr(op, 0);												// ����No�ݒ�(DIRECT)
	}
	PutObjPrt(op, 0, 1);												// �߰�No�ݒ�(�ʏ�)
	PutObjCol(op, col);													// �װNo�ݒ�
	PutGroup(op, gno);													// ��ٰ�ߏ��ݒ�
	SetObjBox(op);														// �ޯ�����ސݒ�
	MakeDirectCommand(op);												// ���ڐ�������ސ���
	if(mode==0) {														// �擪�߲�����ĕۑ�
		A0list = list;
	} else if(mode==1) {
		A0list = list;
	} else if(mode==2) {
		A1list = list;
	}
	tvt = vtl;
	while(tvt) {														// ���_�ް��̈���
		bvt = tvt, tvt = tvt->next;
		free(bvt);
	}
}

/******************************/
static void FreeWorkArea(void)											//<<< ��Ɨ̈���
/******************************/
{
	BFATYPE* wfp, * dfp;
	BVLTYPE* wvp, * dvp;

	wfp = Aflist;
	while(wfp) {														// A���ʏ��̈���
		dfp = wfp, wfp = wfp->next;
		free(dfp);
	}
	wfp = Bflist;
	while(wfp) {														// B���ʏ��̈���
		dfp = wfp, wfp = wfp->next;
		free(dfp);
	}
	wvp = A0list;
	while(wvp) {														// A����Ɨ̈���
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = A1list;
	while(wvp) {														// A����Ɨ̈���
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = B0list;
	while(wvp) {														// B����Ɨ̈���
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = B1list;
	while(wvp) {														// B����Ɨ̈���
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
}

/***************************************************************/
static void CalcBoolean(OBJTYPE* opa, OBJTYPE* opb,
                        int mode, int flg1, int flg2, int delb)			//<<< �}�`���Z�v�Z
/***************************************************************/
{
	int    gno[32], atr, col;
	double wkrs;

	InitWorkArea();														// �}�`���Z��������
	ReszB = EPSILON*1.1;												// �␳�l(A������)
	wkrs = ReszB + EPSILON*1.1;											// �␳�l(B������)
	ChangeObjVtx(0, opa, ReszB, ReszB, ReszB);							// A�����̒��_�ύX
	ChangeObjVtx(0, opb, wkrs, wkrs, wkrs);								// B�����̒��_�ύX
	atr = GetObjAtr(opa);												// ���̑����擾
	col = GetObjCol(opa);												// �װNo�擾
	GetGroup(opa, gno);													// ��ٰ�ߏ��擾
	MakeFaceList(0, opa);												// A�����̖�ؽč쐬
	MakeFaceList(1, opb);												// B�����̖�ؽč쐬
	MakeBooleanList(0, opa, opb);										// A���}�`���Zؽč쐬
	MakeBooleanList(1, opb, opa);										// B���}�`���Zؽč쐬
	RemakeObject(mode, atr, col, gno);									// �}�`���Z���̍쐬
	FreeWorkArea();														// ��Ɨ̈���
	ChangeObjVtx(1, opa, ReszB, ReszB, ReszB);							// A�����̒��_�߂�
	ChangeObjVtx(1, opb, wkrs, wkrs, wkrs);								// B�����̒��_�߂�
	if(flg1) {															// <<�w�藧�̂̏ꍇ>>
		if(delb==0||delb==2) {											// <�폜����>
			LevelObj(1, opa);											// �������ٍ폜���̐ڑ�
			TempObj(opa);												// ���̈ꎞ�폜
		}
	} else {															// <���ԗ��̂̏ꍇ>
		FreeObj(opa, 1);												// ��Ɨ��̍폜
	}
	if(flg2) {															// <<�w�藧�̂̏ꍇ>>
		if(delb==0) {													// <�폜����>
			LevelObj(1, opb);											// �������ٍ폜���̐ڑ�
			TempObj(opb);												// ���̈ꎞ�폜
		}
	} else {															// <���ԗ��̂̏ꍇ>
		FreeObj(opb, 1);												// ��Ɨ��̍폜
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/************************************************************************/
static void SetBopData(OBJTYPE* op, int flg, PNTTYPE cpt, BOPTYPE** bop)//<<< ���Z�Ώۗ����ް��o�^
/************************************************************************/
{
	BOPTYPE* top, * wop;

	top = (BOPTYPE *)malloc(sizeof(BOPTYPE));							// ���Z�Ώۗ��̗̈�m��
	if(top==NULL) {
		MemErr(IDS_MEMERR5);
	}
	top->op = op, top->flg = flg, top->cpt = cpt;
	top->leng = 0, top->next = NULL;
	wop = *bop;															// �ް������ڑ�
	if(wop==NULL) {
		*bop = top;
	} else {
		while(wop->next!=NULL) {
			wop = wop->next;
		}
		wop->next = top;
	}
}

/************************************/
static void SetBopLeng(BOPTYPE* bop)									//<<< ���Z�Ώۗ��̊�����o�^
/************************************/
{
	BOPTYPE* wop;
	PNTTYPE  pnt;
	int      i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = DBL_MAX;
	}
	wop = bop;
	while(wop!=NULL) {
		for(i=0; i<DIM; i++) {
			if(pnt.p[i]>wop->cpt.p[i]) {
				pnt.p[i] = wop->cpt.p[i];								// �ŏ��̒��S�_���W����
			}
		}
		wop = wop->next;
	}
	wop = bop;
	while(wop!=NULL) {
		wop->leng = PntD(&pnt, &wop->cpt);								// ��_����̋���
		wop = wop->next;
	}
}

/*************************************************/
static BOPTYPE* MergeLn(BOPTYPE* la, BOPTYPE* lb)						//<<< ��������
/*************************************************/
{
	BOPTYPE* lc, * ld, * rc;

	lc = (BOPTYPE *)malloc(sizeof(BOPTYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->leng<=lb->leng) {										// ������̑召�Ŕ�r
			lc->next = la, lc = la, la = la->next;
		} else {
			lc->next = lb, lc = lb, lb = lb->next;
		}
	} while(la!=NULL&&lb!=NULL);
	lc->next = (la==NULL) ? lb : la;
	rc = ld->next;
	free(ld);															// ��Ɨ̈���
	return rc;
}

/*************************************************/
static BOPTYPE* Ln_Mergsrt(BOPTYPE* mlist, int n)						//<<< ��������
/*************************************************/
{
	BOPTYPE* la, * lb, * lm, * rc;
	int      i;

	if(mlist->next==NULL) {
		rc = mlist;
	} else {
		la = mlist;
		for(i=2; i<=n/2; i++) {
			mlist = mlist->next;
		}
		lb = mlist->next;
		mlist->next = NULL;
		lm = (BOPTYPE *)MergeLn(Ln_Mergsrt(la, n/2),
                                Ln_Mergsrt(lb, n-n/2));					// ��������
		rc = lm;
	}
	return rc;
}

/**************************************/
void BooleanObject(int mode, int delb)									//<<< ���̐}�`���Z
/**************************************/
{
	CWaitCursor wait;													// ���ĥ���ٕ\��
	OBJTYPE* op;
	BOXTYPE  box;
	PNTTYPE  pnt;
	BOPTYPE* aop=NULL, * bop=NULL, * sop, * wop, * dop;
	CString  str;
	int      i1, i2, act=0, bct=0;

	m_pProgressB = new CProgressDlg(1);
	m_pProgressB->Create();												// �޲�۸ޕ\��
	ErrFlg1 = 0, ErrFlg2 = 0;											// �װ�׸ޏ�����
	SelBaseObjPtr(1, &op);												// A�����̎擾
	while(op!=NULL) {
		GetObjBox(op, &box);											// BOX���ގ擾
		BoxCenter(box, &pnt);											// BOX�����擾
		SetBopData(op, 1, pnt, &aop);									// ���Z�Ώ�A�����ް��o�^
		act++;															// A�����̶���
		SelNextObjPtr(1, &op);											// �����̂�
	}
	BaseObjPtr(2, &op, &i1, &i2);										// B�����̎擾
	while(op!=NULL) {
		if(GetSelNo(op)!=1) {
			GetObjBox(op, &box);										// BOX���ގ擾
			BoxCenter(box, &pnt);										// BOX�����擾
			SetBopData(op, 1, pnt, &bop);								// ���Z�Ώ�B�����ް��o�^
			bct++;														// B�����̶���
			SetSelNo(op, 2);											// �I��No�F2
		}
		NextObjPtr(2, &op, &i1, &i2);									// �����̂�
	}
	if(act<1||bct<1) {													// �Ώۗ��̖��͏I��
		return;
	}
	if(act>1) {
		SetBopLeng(aop);												// ���Z�Ώۗ��̊�����o�^
		aop = Ln_Mergsrt(aop, act);										// ��������
	}
	if(bct>1) {
		SetBopLeng(bop);												// ���Z�Ώۗ��̊�����o�^
		bop = Ln_Mergsrt(bop, bct);										// ��������
	}
	Spr = 0.0, Ppr = 100.0/(act+bct-1);									// 1��̉��Z���߰���
	while(TRUE) {
		wop = aop, sop = NULL;
		while(wop!=NULL&&wop->next!=NULL) {
			op = wop->op;												// ��ڂ̗���
			i1 = wop->flg;
			wop = wop->next;											// ��(������)�̗��̂�
			CalcBoolean(op, wop->op, 1, i1, wop->flg, 0);				// �}�`���Z(���+������)
			GetObjPtr(&op);												// ���Z�㗧���߲���擾
			GetObjBox(op, &box);										// BOX���ގ擾
			BoxCenter(box, &pnt);										// BOX�����擾
			SetBopData(op, 0, pnt, &sop);								// ���Z��A�����ް��o�^
			wop = wop->next;
		}
		if(wop!=NULL) {
			SetBopData(wop->op, wop->flg, wop->cpt, &sop);				// ���Z��A�����ް��o�^(�c��)
		}
		wop = aop;
		while(wop) {													// ���Z�Ώۗ��̗̈���
			dop = wop, wop = wop->next;
			free(dop);
		}
		aop = sop;
		if(aop->next==NULL) {											// ���Z�Ώ�1�ŏI��
			break;
		}
	}
	while(TRUE) {
		wop = bop, sop = NULL;
		while(wop!=NULL&&wop->next!=NULL) {
			op = wop->op;												// ��ڂ̗���
			i1 = wop->flg;
			wop = wop->next;											// ��(������)�̗��̂�
			CalcBoolean(op, wop->op, 1, i1, wop->flg, delb);			// �}�`���Z(���+������)
			GetObjPtr(&op);												// ���Z�㗧���߲���擾
			GetObjBox(op, &box);										// BOX���ގ擾
			BoxCenter(box, &pnt);										// BOX�����擾
			SetBopData(op, 0, pnt, &sop);								// ���Z��B�����ް��o�^
			wop = wop->next;
		}
		if(wop!=NULL) {
			SetBopData(wop->op, wop->flg, wop->cpt, &sop);				// ���Z��B�����ް��o�^(�c��)
		}
		wop = bop;
		while(wop) {													// ���Z�Ώۗ��̗̈���
			dop = wop, wop = wop->next;
			free(dop);
		}
		bop = sop;
		if(bop->next==NULL) {											// ���Z�Ώ�1�ŏI��
			break;
		}
	}
	CalcBoolean(aop->op, bop->op, mode, aop->flg, bop->flg, delb*2);	// �}�`���Z(A����,B����)
	if(ErrFlg1) {
		str.Format(IDS_ERR_DATA, 1);									// �x��ү����(1)
		AfxMessageBox(str);
	}
	if(ErrFlg2) {
		str.Format(IDS_ERR_DATA, 2);									// �x��ү����(2)
		AfxMessageBox(str);
	}
	GetObjPtr(&op);														// �Ώۗ��̎擾
	OnEdtFlg(1, op);													// �ҏW�Ώ��׸�ON
	View->SetDispList(op);												// �ި���ڲ�ؽēo�^
	wop = aop;
	while(wop) {														// ���Z�Ώ�A���̗̈���
		dop = wop, wop = wop->next;
		free(dop);
	}
	wop = bop;
	while(wop) {														// ���Z�Ώ�B���̗̈���
		dop = wop, wop = wop->next;
		free(dop);
	}
	m_pProgressB->DestroyWindow();										// �޲�۸ޔp��
	delete m_pProgressB;
}
