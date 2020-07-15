/////////////////////////////////////////////////////////////////////////////
// �R�}���h�֘A ���ʕ�����
// RCmdPlaneCut.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include <limits.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "ECommand.h"
#include "RCommand.h"
#include "CutType.h"
#include "NumFunc.h"
CProgressDlg* m_pProgressC=NULL;

/***************************************/
static BOOL CheckCutObject(OBJTYPE* op)									//<<< �������̊m�F
/***************************************/
{
	BOXTYPE box;

	GetObjBox(op, &box);												// �����ޯ�����ގ擾
	if(box.bp[0].p[2]<0.0&&box.bp[1].p[2]>0.0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/******************************/
static void InitWorkArea(void)											//<<< ������������
/******************************/
{
	Avlist = NULL, Bvlist = NULL, Cvlist = NULL;
	Avcnt = 0, Bvcnt = 0, Cvcnt = 0;
}

/******************************************/
static void SetCutVtx(PNTTYPE pt, int flg)								//<<< �������_�ݒ�
/******************************************/
{
	CVLTYPE* tv;

	tv = (CVLTYPE *)malloc(sizeof(CVLTYPE));							// �������_�̈�m��
	if(tv==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tv->pt = pt, tv->vt = -1, tv->flg = 0;
	if(!flg) {															// <A��>
		tv->next = Avlist, tv->no = Avcnt++, Avlist = tv;
	} else {															// <B��>
		tv->next = Bvlist, tv->no = Bvcnt++, Bvlist = tv;
	}
}

/*******************************/
static void SetFaceEnd(int flg)											//<<< ������ٰ�ߏI���ݒ�
/*******************************/
{
	if(!flg) {															// <A��>
		Avlist->flg = 1;
	} else {															// <B��>
		Bvlist->flg = 1;
	}
}

/****************************************************/
static void SetFaceCutLine(PNTTYPE spt, PNTTYPE ept)					//<<< �ʕ������ݒ�
/****************************************************/
{
	CCVTYPE* tc;

	tc = (CCVTYPE *)malloc(sizeof(CCVTYPE));							// �ʕ������̈�m��
	if(tc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tc->spt = spt, tc->ept = ept;
	tc->next = Cvlist, Cvlist = tc, Cvcnt++;
}

/**************************************************************/
static void SetObjFace(OBJTYPE* op, int flg, int sno, int eno)			//<<< �ʏ��ݒ�
/**************************************************************/
{
	PNTTYPE pt;
	int     i, lfg;

	for(i=sno; i<=eno; i++) {
		GetFlpVtx1(op, i, &pt, &lfg);									// ��ٰ�ߒ��_�擾
		SetCutVtx(pt, flg);												// �������_�ݒ�
	}
	SetFaceEnd(flg);													// ������ٰ�ߏI���ݒ�
}

/*****************************/
static void InitCutWork(void)											//<<< ����ؽč쐬������
/*****************************/
{
	Aclist = NULL, Bclist = NULL, Cplist = NULL;
	Accnt = 0, Bccnt = 0, Cpcnt = 0;
}

/******************************************/
static void GetCLArea(PNTTYPE pt, int flg)								//<<< �ʕ��������̈�擾
/******************************************/
{
	CCLTYPE* ct;

	ct = (CCLTYPE *)malloc(sizeof(CCLTYPE));							// �ʕ��������̈�m��
	if(ct==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ct->spt = pt;
	ct->ept.p[0] = DBL_MAX, ct->ept.p[1] = DBL_MAX, ct->ept.p[2] = DBL_MAX;
	ct->cp = NULL;
	if(!flg) {															// <A��>
		ct->next = Aclist, Aclist = ct, Accnt++;
	} else {															// <B��>
		ct->next = Bclist, Bclist = ct, Bccnt++;
	}
}

/********************************************************/
static void GetCPArea(PNTTYPE spt, PNTTYPE ept, int flg)				//<<< �����̈�擾
/********************************************************/
{
	CCPTYPE* ct, * wc;

	ct = (CCPTYPE *)malloc(sizeof(CCPTYPE));							// �����̈�m��
	if(ct==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ct->next = NULL, ct->spt = spt, ct->ept = ept;
	if(!flg) {															// <A��>
		wc = Aclist->cp;
		if(wc==NULL) {
			Aclist->cp = ct;
		} else {
			while(wc->next!=NULL) {
				wc = wc->next;
			}
			wc->next = ct;
		}
		Aclist->ept = ept;
	} else {															// <B��>
		wc = Bclist->cp;
		if(wc==NULL) {
			Bclist->cp = ct;
		} else {
			while(wc->next!=NULL) {
				wc = wc->next;
			}
			wc->next = ct;
		}
		Bclist->ept = ept;
	}
}

/*************************************/
static void GetCrossArea(PNTTYPE spt)									//<<< ������_�̈�擾
/*************************************/
{
	CCPTYPE* ct;

	ct = (CCPTYPE *)malloc(sizeof(CCPTYPE));							// ������_�̈�m��
	if(ct==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ct->spt = spt;
	ct->ept.p[0] = DBL_MAX, ct->ept.p[1] = DBL_MAX, ct->ept.p[2] = DBL_MAX;
	ct->next = Cplist, Cplist = ct, Cpcnt++;
}

/****************************************************************/
static void GetCrossPoint(PNTTYPE pt1, PNTTYPE pt2, PNTTYPE* pt)		//<<< ��_�擾
/****************************************************************/
{
	PNTTYPE wp1, wp2;
	double  a, b, c;

	if(pt1.p[2]>pt2.p[2]) {
		wp1 = pt1, wp2 = pt2;
	} else {
		wp1 = pt2, wp2 = pt1;
	}
	c = wp2.p[2]/(wp1.p[2]-wp2.p[2]);									// �n�I�_�䗦
	a = wp1.p[0]-wp2.p[0];												// X������
	b = wp1.p[1]-wp2.p[1];												// Y������
	pt->p[0] = wp2.p[0]-a*c;											// X�����Z�o
	pt->p[1] = wp2.p[1]-b*c;											// Y�����Z�o
	pt->p[2] = 0.0;
}

/*****************************************************/
static void SetCutFace(OBJTYPE* op, int sno, int eno)					//<<< �����ʏ��ݒ�
/*****************************************************/
{
	PNTTYPE spt, pt1, pt2, cpt;
	int     i, lf, sfg, fg1, fg2;

	GetFlpVtx1(op, sno, &pt1, &lf);										// �J�n��ٰ�ߒ��_�擾
	fg1 = (pt1.p[2]>=0) ? 0 : 1;										// �ʒu����
	spt = pt1, sfg = fg1;												// �J�n�_���ۑ�
	GetCLArea(pt1, fg1);												// �ʕ��������̈�擾
	for(i=sno+1; i<=eno; i++) {
		GetFlpVtx1(op, i, &pt2, &lf);									// ��ٰ�ߒ��_�擾
		fg2 = (pt2.p[2]>=0) ? 0 : 1;									// �ʒu����
		if(fg1==fg2) {													// <<�O�_�Ɠ���ʒu>>
			GetCPArea(pt1, pt2, fg2);									// �����̈�擾
		} else {														// <<�O�_�ƈقȂ�ʒu>>
			if(fabs(pt1.p[2])>EPSILON&&fabs(pt2.p[2])>EPSILON) {		// <2�_��Z��0�ȊO>
				GetCrossPoint(pt1, pt2, &cpt);							// ��_�擾
				GetCPArea(pt1, cpt, fg1);								// �����̈�擾
				GetCLArea(cpt, fg2);									// �ʕ��������̈�擾
				GetCPArea(cpt, pt2, fg2);								// �����̈�擾
				GetCrossArea(cpt);										// ������_�̈�擾
			} else if(fabs(pt1.p[2])<EPSILON) {							// <1�_�ڂ�Z��0>
				GetCLArea(pt1, fg2);									// �ʕ��������̈�擾
				GetCPArea(pt1, pt2, fg2);								// �����̈�擾
				GetCrossArea(pt1);										// ������_�̈�擾
			} else {													// <2�_�ڂ�Z��0>
				GetCPArea(pt1, pt2, fg1);								// �����̈�擾
				GetCLArea(pt2, fg2);									// �ʕ��������̈�擾
				GetCrossArea(pt2);										// ������_�̈�擾
			}
		}
		pt1 = pt2, fg1 = fg2;											// 2�_�ڂ�1�_�ڂ�
	}
	if(fg1==sfg) {														// <<�I���_�Ɠ���ʒu>>
		GetCPArea(pt1, spt, sfg);										// �����̈�擾
	} else {															// <<�I���_�ƈق̈ʒu>>
		if(fabs(pt1.p[2])>EPSILON&&fabs(spt.p[2])>EPSILON) {			// <2�_��Z��0�ȊO>
			GetCrossPoint(pt1, spt, &cpt);								// ��_�擾
			GetCPArea(pt1, cpt, fg1);									// �����̈�擾
			GetCLArea(cpt, sfg);										// �ʕ��������̈�擾
			GetCPArea(cpt, spt, sfg);									// �����̈�擾
			GetCrossArea(cpt);											// ������_�̈�擾
		} else if(fabs(pt1.p[2])<EPSILON) {								// <�I���_��Z��0>
			GetCLArea(pt1, sfg);										// �ʕ��������̈�擾
			GetCPArea(pt1, spt, sfg);									// �����̈�擾
			GetCrossArea(pt1);											// ������_�̈�擾
		} else {														// <�J�n�_��Z��0>
			GetCPArea(pt1, spt, fg1);									// �����̈�擾
			GetCLArea(spt, sfg);										// �ʕ��������̈�擾
			GetCrossArea(spt);											// ������_�̈�擾
		}
	}
}

/*********************************************************/
static CCPTYPE* MergeCP(CCPTYPE* la, CCPTYPE* lb, int id)				//<<< ���_���W���
/*********************************************************/
{
	CCPTYPE* lc, * ld, * rc;

	lc = (CCPTYPE *)malloc(sizeof(CCPTYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->spt.p[id]<=lb->spt.p[id]) {								// ���_���W�̑召��r
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
static CCPTYPE* CPMergsrt(CCPTYPE* mlist, int n, int id)				//<<< ���_���W���
/********************************************************/
{
	CCPTYPE* la, * lb, * lm, * rc;
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
		lm = (CCPTYPE *)MergeCP(CPMergsrt(la, n/2, id),
                                CPMergsrt(lb, n-n/2, id), id);			// ���_���W���
		rc = lm;
	}
	return rc;
}

/****************************/
static void SetCutLine(void)											//<<< ���������ݒ�
/****************************/
{
	CCPTYPE* wcp, * scp;
	int      id;
	double   xmin, xmax, ymin, ymax;

	wcp = Cplist;														// X,Y�ŏ�/�ő�擾
	if(wcp!=NULL) {
		xmin = wcp->spt.p[0], xmax = wcp->spt.p[0];
		ymin = wcp->spt.p[1], ymax = wcp->spt.p[1];
	}
	while(wcp!=NULL) {
		if(xmin>wcp->spt.p[0]) {
			xmin = wcp->spt.p[0];
		}
		if(xmax<wcp->spt.p[0]) {
			xmax = wcp->spt.p[0];
		}
		if(ymin>wcp->spt.p[1]) {
			ymin = wcp->spt.p[1];
		}
		if(ymax<wcp->spt.p[1]) {
			ymax = wcp->spt.p[1];
		}
		wcp = wcp->next;
	}
	id = (xmax-xmin>ymax-ymin) ? 0 : 1;									// �m�F����
	Cplist = CPMergsrt(Cplist, Cpcnt, id);								// ���_���W���
	wcp = Cplist;
	while(wcp!=NULL) {
		scp = wcp, wcp = wcp->next;
		if(wcp!=NULL) {
			scp->ept = wcp->spt, wcp->ept = scp->spt;					// 2�_�Â¾��
			wcp = wcp->next;
		}
	}
}

/**********************************************/
static BOOL CheckPoint(PNTTYPE p1, PNTTYPE p2)							//<<< ���W�_����_�m�F
/**********************************************/
{
	if(fabs(p1.p[0]-p2.p[0])<EPSILON&&
       fabs(p1.p[1]-p2.p[1])<EPSILON&&
       fabs(p1.p[2]-p2.p[2])<EPSILON) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**************************************************/
static void SetNewFacePoint(CCLTYPE* slp, int flg)						//<<< �V��ٰ�ߒ��_�ݒ�
/**************************************************/
{
	CCPTYPE* wpp, * dpp;

	wpp = slp->cp;
	while(wpp!=NULL) {
		dpp = wpp;														// �߲���ۑ�
		wpp = wpp->next;												// ���߲�����
		SetCutVtx(dpp->spt, flg);										// �������_�ݒ�
		free(dpp);														// �����z��̈���
	}
}

/*************************************************************/
static BOOL SearchNextPoint(PNTTYPE p1, PNTTYPE* p2, int flg)			//<<< �������J�n�_����
/*************************************************************/
{
	CCPTYPE* wpp, * bpp;
	BOOL     ret=TRUE;

	wpp = Cplist;
	while(wpp!=NULL) {
		if(CheckPoint(p1, wpp->spt)) {									// ���W�_����m�F
			ret = FALSE;
			*p2 = wpp->ept;												// �����W�_���
			if(wpp==Cplist) {											// �Ώۗ̈�ؽĊO
				Cplist = wpp->next;
			} else {
				bpp->next = wpp->next;
			}
			SetCutVtx(wpp->spt, flg);									// �������_�ݒ�
			if(!flg) {
				SetFaceCutLine(wpp->spt, wpp->ept);						// �ʕ������ݒ�
			}
			free(wpp);													// �̈���
			Cpcnt--;
			break;
		}
		bpp = wpp, wpp = wpp->next;										// ���߲����ۑ�,����
	}
	return ret;
}

/************************************************************/
static BOOL SearchNextLine(PNTTYPE p1, PNTTYPE* p2, int flg)			//<<< ����������
/************************************************************/
{
	CCLTYPE* wlp, * blp;
	BOOL     ret=TRUE;

	if(!flg) {
		wlp = Aclist;
	} else {
		wlp = Bclist;
	}
	while(wlp!=NULL) {
		if(CheckPoint(p1, wlp->spt)) {									// ���W�_����m�F
			ret = FALSE;
			*p2 = wlp->ept;												// �����W�_���
			if(!flg) {
				if(wlp==Aclist) {										// �Ώۗ̈�ؽĊO
					Aclist = wlp->next;
				} else {
					blp->next = wlp->next;
				}
			} else {
				if(wlp==Bclist) {										// �Ώۗ̈�ؽĊO
					Bclist = wlp->next;
				} else {
					blp->next = wlp->next;
				}
			}
			SetNewFacePoint(wlp, flg);									// �V��ٰ�ߒ��_�ݒ�
			if(!flg) {													// �̈���
				free(wlp);
				Accnt--;
			} else {
				free(wlp);
				Bccnt--;
			}
			break;
		}
		blp = wlp, wlp = wlp->next;
	}
	return ret;
}

/****************************/
static void SetNewFace(void)											//<<< �V�ʏ��ݒ�
/****************************/
{
	CCLTYPE* slp;
	PNTTYPE  spt, ept, npt;

	while(TRUE) {														// <<<A������>>>
		if(Accnt<1) {													// �S������/�߲��NULL�I��
			break;
		}
		if(Aclist==NULL) {
			break;
		}
		slp = Aclist;													// �擪�����Ώ�
		Aclist = slp->next;												// ������ؽĊO
		spt = slp->spt;													// �J�n�_�ۑ�
		ept = slp->ept;													// �I���_�ۑ�
		SetNewFacePoint(slp, 0);										// �V��ٰ�ߒ��_�ݒ�
		free(slp);														// �̈���
		Accnt--;
		if(fabs(ept.p[2])>EPSILON) {									// <�I���_Z!=0>
			npt = ept;
			if(SearchNextLine(npt, &ept, 0)) {							// ����������
				SetFaceEnd(0);
				continue;												// �Y�������ʂ�
			}
		}
		while(TRUE) {
			if(SearchNextPoint(ept, &npt, 0)) {							// �������J�n�_����
				SetFaceEnd(0);
				break;													// �Y�������ʂ�
			}
			if(CheckPoint(spt, npt)) {									// ���W�_�̓���m�F
				SetFaceEnd(0);
				break;													// �n�_���ꎟ�ʂ�
			}
			if(SearchNextLine(npt, &ept, 0)) {							// ����������
				SetFaceEnd(0);
				break;													// �Y�������ʂ�
			}
			if(CheckPoint(spt, ept)) {									// ���W�_�̓���m�F
				SetFaceEnd(0);
				break;													// �n�_���ꎟ�ʂ�
			}
		}
	}
	while(TRUE) {														// <<<B������>>>
		if(Bccnt<1) {													// �S������/�߲��NULL�I��
			break;
		}
		if(Bclist==NULL) {
			break;
		}
		slp = Bclist;													// �擪�����Ώ�
		Bclist = slp->next;												// ������ؽĊO
		spt = slp->spt;													// �J�n�_�ۑ�
		ept = slp->ept;													// �I���_�ۑ�
		SetNewFacePoint(slp, 1);										// �V��ٰ�ߒ��_�ݒ�
		free(slp);														// �̈���
		Bccnt--;
		if(fabs(ept.p[2])>EPSILON) {									// <�I���_Z!=0>
			npt = ept;
			if(SearchNextLine(npt, &ept, 1)) {							// ����������
				SetFaceEnd(1);
				continue;												// �Y�������ʂ�
			}
		}
		while(TRUE) {
			if(SearchNextPoint(ept, &npt, 1)) {							// �������J�n�_����
				SetFaceEnd(1);
				break;													// �Y�������ʂ�
			}
			if(CheckPoint(spt, npt)) {									// ���W�_����m�F
				SetFaceEnd(1);
				break;													// �n�_���ꎟ�ʂ�
			}
			if(SearchNextLine(npt, &ept, 1)) {							// ����������
				SetFaceEnd(1);
				break;													// �Y�������ʂ�
			}
			if(CheckPoint(spt, ept)) {									// ���W�_����m�F
				SetFaceEnd(1);
				break;													// �n�_���ꎟ�ʂ�
			}
		}
	}
}

/*****************************/
static void FreeCutWork(void)											//<<< ������Ɨ̈���
/*****************************/
{
	CCLTYPE* wlp, * dlp;
	CCPTYPE* wpp, * dpp;

	wlp = Aclist;														// <A����Ɨ̈�>
	while(wlp) {
		wpp = wlp->cp;
		while(wpp) {													// �����z��̈���
			dpp = wpp, wpp = wpp->next;
			free(dpp);
		}
		dlp = wlp, wlp = wlp->next;
		free(dlp);														// �ʕ��������z��̈���
	}
	wlp = Bclist;														// <B����Ɨ̈�>
	while(wlp) {
		wpp = wlp->cp;
		while(wpp) {													// �����z��̈���
			dpp = wpp, wpp = wpp->next;
			free(dpp);
		}
		dlp = wlp, wlp = wlp->next;
		free(dlp);														// �ʕ��������z��̈���
	}
	wpp = Cplist;														// <������_�̈���>
	while(wpp) {														// �����z��̈���
		dpp = wpp, wpp = wpp->next;
		free(dpp);
	}
}

/************************************/
static void MakeCutList(OBJTYPE* op)									//<<< ����ؽč쐬
/************************************/
{
	PNTTYPE pt;
	int     i, lnum, flg, st, iper=0;
	double  dper, doper, min1, max1;

	doper = Oper/2.0;													// �{�����߰���
	lnum = GetFlpNum1(op);												// ��ٰ�ߐ��擾
	for(i=0, st=-1; i<lnum; i++) {
		GetFlpVtx1(op, i, &pt, &flg);									// ��ٰ�ߒ��_�擾
		if(st==-1) {													// ��ٰ�ߊJ�n����
			min1 = pt.p[2], max1 = pt.p[2], st = i;
		}
		if(min1>pt.p[2]) {												// �ʍŏ�Z�l�擾
			min1 = pt.p[2];
		}
		if(max1<pt.p[2]) {												// �ʍő�Z�l�擾
			max1 = pt.p[2];
		}
		if(flg) {														// <<��ٰ�ߏI��>>
			if(fabs(min1)<EPSILON&&fabs(max1)<EPSILON) {				// <�ʂ�Z=0��ɑ���>
				SetObjFace(op, 0, st, i);								// �ʏ��ݒ�(A��)
			} else if(min1>=0) {										// <�ʂ�Z���ʒu�ɑ���>
				SetObjFace(op, 0, st, i);								// �ʏ��ݒ�(A��)
			} else if(max1<=0) {										// <�ʂ�Z���ʒu�ɑ���>
				SetObjFace(op, 1, st, i);								// �ʏ��ݒ�(B��)
			} else {													// <�ʂ�Z�����ɍ���>
				InitCutWork();											// ����ؽč쐬������
				SetCutFace(op, st, i);									// �����ʏ��ݒ�(AB��)
				SetCutLine();											// ���������ݒ�
				SetNewFace();											// �V�ʏ��ݒ�
				FreeCutWork();											// ������Ɨ̈���
			}
			st = -1;													// ٰ�ߊJ�n�׸ޏ�����
		}
		dper = doper*((double)i/(double)lnum);							// �����߰���
		if(iper<(int)(Sper+dper)) {										// <�߰���UP>
			iper = (int)(Sper+dper);
			m_pProgressC->SetProgress(iper);							// �i���\��
		}
	}
	Sper += doper;														// �ݐ��߰���
}

/*********************************/
static void Search(CCVTYPE** cvl)										//<<< ����������
/*********************************/
{
	CCVTYPE* cvlo, * cvlt, * cvlb;

	cvlo = *cvl, cvlb = *cvl;											// �w������ް�
	cvlt = cvlo->next;													// �������ް�
	*cvl = cvlt;														// �������ݒ�
	while(cvlt) {
		if(fabs(cvlt->spt.p[0]-cvlo->ept.p[0])<EPSILON&&				// <�w������I�_�ƑΏې���
           fabs(cvlt->spt.p[1]-cvlo->ept.p[1])<EPSILON) {				//  �n�_����v�̏ꍇ>
			if(*cvl!=cvlt) {											// <�w������̎��ƈق̏ꍇ>
				*cvl = cvlt;											// ��������ݒ� 
				cvlb->next = cvlt->next;								// ����ؽĕ���
				cvlt->next = cvlo->next;								// ��������擪
			}
			return;
		}    
		cvlb = cvlt;													// �ŏI�m�F��ؽ��߲��
		cvlt = cvlt->next;												// ��������
	}
}

/*********************************/
static void Chain(CCVTYPE** cvlo)										//<<< ��������
/*********************************/
{
	CCVTYPE* cvl, * cvlt;

	cvl = *cvlo;
	while(cvl) {
		cvlt = cvl;
		Search(&cvlt);													// ����������
		cvl->next = cvlt;												// �������߲���ڑ�
		cvl = cvlt;														// ��������
	}
}

/********************************/
static void MakeCutSection(void)										//<<< �����f�ʍ쐬
/********************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	CCVTYPE* cvl;
	PNTTYPE  pt;
	VECTYPE  vec;
	int      i, fg1, fg2, cnt=0;

	Chain(&Cvlist);														// ��������
	cvl = Cvlist;
	if(cvl!=NULL) {
		pt = cvl->spt;
	}
	while(cvl!=NULL) {
		if(!CheckPoint(pt, cvl->spt)) {									// ���W�_����_�m�F
			if(cnt>2) {													// <�I��/�J�n�ق̗L���ް�>
				CalcNVec(pa, cnt, &vec);								// �@���޸�َ擾
				if(vec.p[2]<0.0) {										// ��ٰ�߈ʒu����
					fg1 = 0, fg2 = 1;
				} else {
					fg1 = 1, fg2 = 0;
				}
				for(i=0; i<cnt; i++) {
					SetCutVtx(pa[i], fg1);								// ��ٰ�ߓo�^
				}
				SetFaceEnd(fg1);
				for(i=cnt-1; i>=0; i--) {
					SetCutVtx(pa[i], fg2);								// �tٰ�ߓo�^
				}
				SetFaceEnd(fg2);
			}
			cnt = 0;
		}
		pa[cnt++] = cvl->spt;											// �J�n�_�ۑ�
		pt = cvl->ept;													// �I���_�ۑ�
		cvl = cvl->next;												// ��������
	}
	if(cnt>2) {															// <�I��/�J�n�ق̗L���ް�>
		CalcNVec(pa, cnt, &vec);										// �@���޸�َ擾
		if(vec.p[2]<0.0) {												// ��ٰ�߈ʒu����
			fg1 = 0, fg2 = 1;
		} else {
			fg1 = 1, fg2 = 0;
		}
		for(i=0; i<cnt; i++) {
			SetCutVtx(pa[i], fg1);										// ��ٰ�ߓo�^
		}
		SetFaceEnd(fg1);
		for(i=cnt-1; i>=0; i--) {
			SetCutVtx(pa[i], fg2);										// �tٰ�ߓo�^
		}
		SetFaceEnd(fg2);
	}
	delete[] pa;
}

/********************************************************/
static CVLTYPE* MergeV(CVLTYPE* la, CVLTYPE* lb, int id)				//<<< ���_���W���
/********************************************************/
{
	CVLTYPE* lc, * ld, * rc;

	lc = (CVLTYPE *)malloc(sizeof(CVLTYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->pt.p[id]<=lb->pt.p[id]) {								// ���_���W�̑召��r
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
static CVLTYPE* V_Mergsrt(CVLTYPE* mlist, int n, int id)				//<<< ���_���W���
/********************************************************/
{
	CVLTYPE* la, * lb, * lm, * rc;
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
		lm = (CVLTYPE *)MergeV(V_Mergsrt(la, n/2, id),
                               V_Mergsrt(lb, n-n/2, id), id);			// ���_���W���
		rc = lm;
	}
	return rc;
}

/*************************************************/
static CVLTYPE* MergeVn(CVLTYPE* la, CVLTYPE* lb)						//<<< ��ٰ�ߔԍ����
/*************************************************/
{
	CVLTYPE* lc, * ld, * rc;

	lc = (CVLTYPE *)malloc(sizeof(CVLTYPE));							// ��Ɨ̈�m��
	if(lc==NULL) {
		MemErr(IDS_MEMERR5);
	}
	ld = lc;
	do {
		if(la->no<=lb->no) {											// ��ٰ�ߔԍ��̑召��r
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
static CVLTYPE* V_Mergsrtn(CVLTYPE* mlist, int n)						//<<< ��ٰ�ߔԍ����
/*************************************************/
{
	CVLTYPE* la, * lb, * lm, * rc;
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
		lm = (CVLTYPE *)MergeVn(V_Mergsrtn(la, n/2),
                                V_Mergsrtn(lb, n-n/2));					// ��ٰ�ߔԍ����
		rc = lm;
	}
	return rc;
}

/****************************************************/
static void RemakeObject(int atr, int col, int* gno)					//<<< �������̍쐬
/****************************************************/
{
	CVLTYPE* wvp;
	OBJTYPE* op;
	PNTTYPE  pt;
	int      i, vct, lct, wct;
	double   dper;

	dper = Oper/20;														// �������߰���
	if(Avcnt>0) {														// <<<A������>>>
		wvp = Avlist;
		while(wvp!=NULL) {
			for(i=0; i<DIM; i++) {
				wvp->pt.p[i] -= ReszC;									// ���_�ް��߂�
			}
			wvp = wvp->next;
		}
		vct = 0, lct = 0;
		Avlist = V_Mergsrt(Avlist, Avcnt, 2);							// ���_���W:Z���
		Avlist = V_Mergsrt(Avlist, Avcnt, 1);							// ���_���W:Y���
		Avlist = V_Mergsrt(Avlist, Avcnt, 0);							// ���_���W:X���
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		pt = Avlist->pt;
		pt.p[0] = pt.p[0]-1.0;											// �����l�ݒ�
		wvp = Avlist;
		while(wvp!=NULL) {												// ���K���_������
			if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
               fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
               fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {					// <�ݒ�ςƈقȂ�ꍇ>
				wvp->vt = vct++;										// ���_�ԍ��ۑ�
				pt = wvp->pt;
			} else {													// ���͒��_�ԍ��ۑ�
				wvp->vt = vct-1;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		CreatObj(vct, Avcnt, 0, &op);									// ���̗̈�m��
		LevelObj(0, op);												// �������ِ������̐ڑ�
		vct = 0, wct = -1, wvp = Avlist;
		while(wvp!=NULL) {
			if(wct!=wvp->vt) {											// <�ԍ����ق̏ꍇ>
				PutVtx(op, vct++, wvp->pt);								// ���_���W�ݒ�
				wct = wvp->vt;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		Avlist = V_Mergsrtn(Avlist, Avcnt);								// ��ٰ�ߔԍ����
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		wvp = Avlist;
		while(wvp!=NULL) {
			PutFlp1(op, lct++, wvp->vt, wvp->flg);						// ��ٰ��1�ݒ�
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		if(atr==1) {													// <A�����̑�����JEWEL>
			PutObjAtr(op, 1);											// ����No�ݒ�(DIRECT[GEM])
		} else {														// <A�����̑�����JEWEL�ȊO>
			PutObjAtr(op, 0);											// ����No�ݒ�(DIRECT)
		}
		PutObjPrt(op, 0, 1);											// �߰�No�ݒ�(�ʏ�)
		PutObjCol(op, col);												// �װNo�ݒ�
		PutGroup(op, gno);												// ��ٰ�ߏ��ݒ�
		SetObjBox(op);													// �ޯ�����ސݒ�
		OnEdtFlg(1, op);												// �ҏW�Ώ��׸�ON
		MakeDirectCommand(op);											// ���ڐ�������ސ���
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
	}
	if(Bvcnt>0) {														// <<<B������>>>
		wvp = Bvlist;
		while(wvp!=NULL) {
			for(i=0; i<DIM; i++) {
				wvp->pt.p[i] -= ReszC;									// ���_�ް��߂�
			}
			wvp = wvp->next;
		}
		vct = 0, lct = 0;
		Bvlist = V_Mergsrt(Bvlist, Bvcnt, 2);							// ���_���W:Z���
		Bvlist = V_Mergsrt(Bvlist, Bvcnt, 1);							// ���_���W:Y���
		Bvlist = V_Mergsrt(Bvlist, Bvcnt, 0);							// ���_���W:X���
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		pt = Bvlist->pt;
		pt.p[0] = pt.p[0]-1.0;											// �����l�ݒ�
		wvp = Bvlist;
		while(wvp!=NULL) {												// ���K���_������
			if(fabs(pt.p[0]-wvp->pt.p[0])>EPSILON||
               fabs(pt.p[1]-wvp->pt.p[1])>EPSILON||
               fabs(pt.p[2]-wvp->pt.p[2])>EPSILON) {					// <�ݒ�ςƈقȂ�ꍇ>
				wvp->vt = vct++;										// ���_�ԍ��ۑ�
				pt = wvp->pt;
			} else {													// ���͒��_�ԍ��ۑ�
				wvp->vt = vct-1;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		CreatObj(vct, Bvcnt, 0, &op);									// ���̗̈�m��
		LevelObj(0, op);												// �������ِ������̐ڑ�
		vct = 0, wct = -1, wvp = Bvlist;
		while(wvp!=NULL) {
			if(wct!=wvp->vt) {											// <�ԍ����ق̏ꍇ>
				PutVtx(op, vct++, wvp->pt);								// ���_���W�ݒ�
				wct = wvp->vt;
			}
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		Bvlist = V_Mergsrtn(Bvlist, Bvcnt);								// ��ٰ�ߔԍ����
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		wvp = Bvlist;
		while(wvp!=NULL) {
			PutFlp1(op, lct++, wvp->vt, wvp->flg);						// ��ٰ��1�ݒ�
			wvp = wvp->next;
		}
		Sper += dper;
		m_pProgressC->SetProgress((int)Sper);							// �i���\��
		if(atr==1) {													// <A�����̑�����JEWEL>
			PutObjAtr(op, 1);											// ����No�ݒ�(DIRECT[GEM])
		} else {														// <A�����̑�����JEWEL�ȊO>
			PutObjAtr(op, 0);											// ����No�ݒ�(DIRECT)
		}
		PutObjPrt(op, 0, 1);											// �߰�No�ݒ�(�ʏ�)
		PutObjCol(op, col);												// �װNo�ݒ�
		PutGroup(op, gno);												// ��ٰ�ߏ��ݒ�
		OnEdtFlg(1, op);												// �ҏW�Ώ��׸�ON
		SetObjBox(op);													// �ޯ�����ސݒ�
		MakeDirectCommand(op);											// ���ڐ�������ސ���
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
	}
}

/******************************/
static void FreeWorkArea(void)											//<<< ��Ɨ̈���
/******************************/
{
	CVLTYPE* wvp, * dvp;
	CCVTYPE* wcp, * dcp;

	wvp = Avlist;
	while(wvp) {														// A����Ɨ̈���
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wvp = Bvlist;
	while(wvp) {														// B����Ɨ̈���
		dvp = wvp, wvp = wvp->next;
		free(dvp);
	}
	wcp = Cvlist;
	while(wcp) {														// �f�ʍ�Ɨ̈���
		dcp = wcp, wcp = wcp->next;
		free(dcp);
	}
}

/**********************************************************/
static void CutObject(double dz, double th, PNTTYPE pt,
                      VECTYPE vc1, VECTYPE vc2, int plane)				//<<< ���̕���(XY)����
/**********************************************************/
{
	OBJTYPE* op, * wop;
	int      i, i1, i2, oct=0, wct=0, atr, col, gno[32];

	BaseObjPtr(2, &op, &i1, &i2);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		oct++;															// ���̐�����
		NextObjPtr(2, &op, &i1, &i2);									// �����̂�
	}
	Oper = 100.0/oct, Sper = 0.0;										// 1�����߰���
	BaseObjPtr(2, &op, &i1, &i2);										// �����Ώۗ��̎擾
	while(op!=NULL&&oct>wct) {
		if(CheckCutObject(op)) {										// <<�����Ώۗ���>>
			atr = GetObjAtr(op);										// ���̑����擾
			col = GetObjCol(op);										// �װNo�擾
			GetGroup(op, gno);											// ��ٰ�ߏ��擾
			InitWorkArea();												// ��������
			ReszC = EPSILON*3.3;										// �␳�l
			ChangeObjVtx(0, op, ReszC, ReszC, ReszC);					// ���̒��_�ύX
			MakeCutList(op);											// ����ؽč쐬
			i1 = GetObjAtr(op);											// ���̑����擾
			if(i1!=10&&i1!=11&&i1!=21&&i1!=23) {
				MakeCutSection();										// �����f�ʍ쐬
			}
			RemakeObject(atr, col, gno);								// �������̍쐬
			FreeWorkArea();												// ��Ɨ̈���
			ChangeObjVtx(1, op, ReszC, ReszC, ReszC);					// ���̒��_�߂�
		} else {														// <<�����ΏۊO����>>
			SetObjPtr(op);												// ���ʑΏۗ��̐ݒ�
			wop = CopyObject();											// ���̂̕���
			SetObjPtr(wop);												// �����߲���ݒ�
			OnEdtFlg(1, wop);											// �ҏW�Ώ��׸�ON
			for(i=0; i<(int)Oper; i++) {
				m_pProgressC->SetProgress((int)Sper+i);					// �i���\��
			}
			Sper += Oper;
		}
		wop = op;														// �폜�����߲���ۑ�
		NextObjPtr(2, &op, &i1, &i2);									// �����̂�
		ObjMove(wop, 0, 0, dz);											// ���̈ʒu�֖߂�
		if(plane==0) {
			ObjRotate(wop, pt, vc2, -90.0);
		}
		ObjRotate(wop, pt, vc1, -th);
		LevelObj(1, wop);												// �������ٍ폜���̐ڑ�
		TempObj(wop);													// ���̂̈ꎞ�폜
		wct++;
	}
}

/**************************************************/
void PCutObject(PNTTYPE p1, PNTTYPE p2, int plane)						//<<< ���̕��ʕ���
/**************************************************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	VECTYPE vc1, vc2;
	double  th=0.0;

	m_pProgressC = new CProgressDlg(2);
	m_pProgressC->Create();												// �޲�۸ޕ\��
	if(plane==0) {														// <������XY����>
		vc1.p[0] = 0.0, vc1.p[1] = 0.0, vc1.p[2] = 1.0;
		if(fabs(p2.p[1]-p1.p[1])<EPSILON) {
			th = PI/2.0;
		} else {
			th = atan2(p2.p[0]-p1.p[0], p2.p[1]-p1.p[1]);
		}
		vc2.p[0] = 0.0, vc2.p[2] = 0.0, vc2.p[1] = 1.0;
	} else if(plane==1) {												// <������YZ����>
		vc1.p[0] = -1.0, vc1.p[1] = 0.0, vc1.p[2] = 0.0;
		if(fabs(p2.p[1]-p1.p[1])<EPSILON) {
			th = PI/2.0;
		} else {
			th = atan2(p2.p[2]-p1.p[2], p2.p[1]-p1.p[1]);
		}
	} else if(plane==2) {												// <������ZX����>
		vc1.p[1] = 1.0, vc1.p[0] = 0.0, vc1.p[2] = 0.0;
		if(fabs(p2.p[0]-p1.p[0])<EPSILON) {
			th = PI/2.0;
		} else {
			th = atan2(p2.p[2]-p1.p[2], p2.p[0]-p1.p[0]);
		}
	}
	th = (th<-10||th>10) ? 0.0 : th;
	th *= 180.0/PI;														// RAD->DEG �ϊ�
	th = (th>90.0) ? th-180.0 : th;
	Rotate(p1, vc1, th);												// XY���ʂ։�]�ړ�
	if(plane==0) {
		Rotate(p1, vc2, 90.0);
	}
	Move(0.0, 0.0, -p1.p[2]);
	CutObject(p1.p[2], th, p1, vc1, vc2, plane);						// ���ʕ�������
	Move(0, 0, p1.p[2]);												// ���ʒu�֖߂�
	if(plane==0) {
		Rotate(p1, vc2, -90.0);
	}
	Rotate(p1, vc1, -th);
	View->SetDataUse();													// �ް��g�p���ݒ�
	m_pProgressC->DestroyWindow();										// �޲�۸ޔp��
	delete m_pProgressC;
}
