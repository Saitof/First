/////////////////////////////////////////////////////////////////////////////
// ��Ί֌W�ҏW��
// EditJewelry.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "InputsDlg.h"
#include "ButtonDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "CCommand.h"
#include "ECommand.h"
#include "RCommand.h"
#include "DataFile1.h"
#include "Select.h"
#include "NumFunc.h"
#include "CreatCtrlPoint2.h"
#include "ConvSCurve.h"

/*********************************************************************/
BOOL KeyboardJwlRing(int kind, int mode, double* size, double* space)	//<<< ����,�ڍ��Ԋu����
/*********************************************************************/
{
	CInputDlg dlg(17+2*kind+mode);
	dlg.SetValue(*size, *space);										// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(size, space);										// ����,�ڍ��Ԋu����
		return TRUE;
	} else {
		return FALSE;
	}
}

/********************************/
BOOL KeyboardStoneForm(int* frm)										//<<< �Θg�`�����
/********************************/
{
	CGemsetDlg dlg;
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(frm);												// �`�����
		return TRUE;
	} else {
		return FALSE;
	}
}

/**********************************/
BOOL KeyboardRoundSize(double* xd)										//<<< �Θg���a����
/**********************************/
{
	CInputDlg dlg(4);
	dlg.SetValue(*xd);													// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(xd);												// ���ޓ���
		return TRUE;
	} else {
		return FALSE;
	}
}

/***********************************************/
BOOL KeyboardSquareSize(double* xd, double* yd)							//<<< �ΘgX,Y���ޓ���
/***********************************************/
{
	CInputDlg dlg(3);
	dlg.SetValue(*xd, *yd);												// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(xd, yd);											// ���ޓ���
		return TRUE;
	} else {
		return FALSE;
	}
}

/***************************************************************/
BOOL KeyboardProng(int* nm, double* ln, double* dm, double* rv)			//<<< �܏�����
/***************************************************************/
{
	CSetProngDlg dlg;
	dlg.SetValue(*nm, *ln, *dm, *rv);									// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(nm, ln, dm, rv);									// �܏�����
		return TRUE;
	} else {
		return FALSE;
	}
}

/*******************************************/
BOOL KeyboardArrange(double* ath, int* ano)								//<<< �z�u������
/*******************************************/
{
	CInputDlg dlg(12);
	dlg.SetValue(*ath, *ano);											// �����l�\��
	if(dlg.DoModal()==IDOK) {											// �����޲�۸ޕ\��
		dlg.GetValue(ath, ano);											// �̈�,������
		return TRUE;
	} else {
		return FALSE;
	}
}

/************************************************************************/
void TrackerStone(int flg, PNTTYPE base, double xs, double ys, double xh,
                  double yh, int* cn, PNTTYPE* pc, int* ic, int* op)	//<<< �Θg�ׯ���쐬
/************************************************************************/
{
	int    i;
	double ws;

	pc[0] = base, pc[1] = base, pc[2] = base, pc[3] = base;
	ic[0] = 0, ic[1] = 0, ic[2] = 0, ic[3] = 0;
	if(flg==0) {														// <׳��ސΘg�̏ꍇ>
		pc[0].p[0] -= xs, pc[1].p[1] -= ys;
		pc[2].p[0] += xs, pc[3].p[1] += ys;
		op[0] = 5, op[1] = 6, op[2] = 5, op[3] = 6;
		*cn = 4;
	} else if(flg==1) {													// <����ِΘg�̏ꍇ>
		pc[0].p[0] -= xs, pc[0].p[1] -= ys;
		pc[1].p[0] += xs, pc[1].p[1] -= ys;
		pc[2].p[0] += xs, pc[2].p[1] += ys;
		pc[3].p[0] -= xs, pc[3].p[1] += ys;
		op[0] = 7, op[1] = 7, op[2] = 7, op[3] = 7;
		*cn = 4;
	} else {															// <�����Θg�̏ꍇ>
		pc[0].p[0] -= xs, pc[0].p[1] -= ys;
		pc[1].p[0] += xs, pc[1].p[1] -= ys;
		pc[2].p[0] += xs, pc[2].p[1] += ys;
		pc[3].p[0] -= xs, pc[3].p[1] += ys;
		op[0] = 7, op[1] = 7, op[2] = 7, op[3] = 7;
		xh = (fabs(xh)<EPSILON&&yh>0.0) ? yh : xh;
		yh = (fabs(yh)<EPSILON&&xh>0.0) ? xh : yh;
		ws = (xs<ys) ? xs : ys;
		if(fabs(xh)<EPSILON&&fabs(yh)<EPSILON) {
			xh = ws/2.0, yh = ws/2.0;
		}
		for(i=0; i<4; i++) {
			pc[2*i+4] = pc[i], ic[2*i+4] = 2;
			pc[2*i+5] = pc[i], ic[2*i+5] = 2;
		}
		pc[4].p[1] += yh,  pc[5].p[0] += xh;
		pc[6].p[0] -= xh,  pc[7].p[1] += yh;
		pc[8].p[1] -= yh,  pc[9].p[0] -= xh;
		pc[10].p[0] += xh, pc[11].p[1] -= yh;
		op[4] = 6, op[5] = 5, op[6] = 5, op[7] = 6;
		op[8] = 6, op[9] = 5, op[10] = 5, op[11] = 6;
		*cn = 12;
	}
}

/**********************************************************************/
void RoundStone(PNTTYPE base, double mi, double xs, double ys, int dv,
                int an, PNTTYPE* pa, int* ia, PNTTYPE* pb, int* ib)		//<<< ׳��ސΘg����
/**********************************************************************/
{
	PNTTYPE* w1 = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* w2 = new PNTTYPE[View->m_MaxCCtA];
	int*     ea = new int[View->m_MaxCCtA];
	double*  ma = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	double*  mb = new double[View->m_MaxCCtA*View->m_MaxCCtA];
	int      i, j, cnt=0;
	double   ath, dth, wt;

	dth = 2.0*PI/dv;													// 1������]�p�x
	for(i=0; i<an; i++) {
		w1[i].p[0] = 0.0;												// �w�蕽�ʊO���W
		w1[i].p[1] = pa[i].p[1]-mi;										// 2D�}�`����
		w1[i].p[2] = pa[i].p[2];
	}
	for(i=0; i<an; i++) {
		for(ath=0.0, j=0; j<dv; ath+=dth, j++) {						// ����_�z��ݒ�
			pb[cnt].p[0] = (xs-w1[i].p[1])*cos(ath)+base.p[0];
			pb[cnt].p[1] = (ys-w1[i].p[1])*sin(ath)+base.p[1];
			pb[cnt].p[2] = w1[i].p[2];
			ib[cnt++]=ia[i];											// �Ȑ�Ӱ�ސݒ�
		}
	}
	for(i=0; i<dv; i++) {
		ea[i] = 0;														// ��]������ײ�Ӱ��
	}
	wt = View->GetCurveWeight();										// �Ȑ��d�݌W���擾
	SetCurveMatrix1(1, wt, 1, dv, ea, ma, mb);							// �ʉߓ_=>����_�ϊ���د��
	for(i=0; i<an; i++) {
		for(j=0; j<dv; j++) {
			w1[j] = pb[j+dv*i];
		}
		CalcCurvePoint(1, dv, w1, w2, mb);								// �ʉߓ_=>����_�ϊ�
		for(j=0; j<dv; j++) {
			pb[j+dv*i] = w2[j];
		}
	}
	delete[] w1;
	delete[] w2;
	delete[] ea;
	delete[] ma;
	delete[] mb;
}

/************************************************************/
void SquareStone(PNTTYPE base, double mi, double xs,
                 double ys, double xh, double yh, int an,
                 PNTTYPE* pa, int* ia, PNTTYPE* pb, int* ib)			//<<< �����Θg����
/************************************************************/
{
	PNTTYPE* pp = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE* wp = new PNTTYPE[View->m_MaxCCtA];
	PNTTYPE  ov[8], bp[4];
	VECTYPE  vec, rvc, v1, v2;
	int      i, j;
	double   rot[3][3], th, dd;

	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = -1.0;					// ��]���ݒ�
	for(i=0; i<an; i++) {
		pp[i].p[0] = 0.0;												// �w�蕽�ʊO���W
		pp[i].p[1] = pa[i].p[1]-mi;										// 2D�}�`����
		pp[i].p[2] = pa[i].p[2];
	}
	for(i=0; i<4; i++) {
		bp[i] = base;													// ��_�Z�o
	}
	bp[0].p[0] -= xs, bp[0].p[1] -= ys;
	bp[1].p[0] += xs, bp[1].p[1] -= ys;
	bp[2].p[0] += xs, bp[2].p[1] += ys;
	bp[3].p[0] -= xs, bp[3].p[1] += ys;
	if(fabs(xh)<EPSILON&&fabs(yh)<EPSILON) {							// <<�␳���̏ꍇ>>
		for(i=0; i<4; i++) {
			ov[2*i] = bp[i];											// ��_�ݒ�
			if(i==3) {													// ��_���ԓ_�ݒ�
				for(j=0; j<DIM; j++) {
					ov[2*i+1].p[j] = (bp[3].p[j]+bp[0].p[j])/2.0;
				}
			} else {
				for(j=0; j<DIM; j++) {
					ov[2*i+1].p[j] = (bp[i].p[j]+bp[i+1].p[j])/2.0;
				}
			}
		}
	} else {															// <<�␳�L�̏ꍇ>>
		xh = (fabs(xh)<EPSILON) ? yh : xh;								// �����0���������
		yh = (fabs(yh)<EPSILON) ? xh : yh;
		for(i=0; i<4; i++) {											// ��_�ݒ�
			ov[2*i] = bp[i], ov[2*i+1] = bp[i];
		}
		ov[0].p[1] += yh, ov[1].p[0] += xh;								// ��_�␳
		ov[2].p[0] -= xh, ov[3].p[1] += yh;
		ov[4].p[1] -= yh, ov[5].p[0] -= xh;
		ov[6].p[0] += xh, ov[7].p[1] -= yh;
	}
	for(i=0; i<8; i++) {												// �אڒ��_�Ƃ��޸�َ擾
		if(i==0) {
			for(j=0; j<DIM; j++) {
				v1.p[j] = ov[7].p[j]-ov[0].p[j];
				v2.p[j] = ov[1].p[j]-ov[0].p[j];
			}
		} else if(i==7) {
			for(j=0; j<DIM; j++) {
				v1.p[j] = ov[6].p[j]-ov[7].p[j];
				v2.p[j] = ov[0].p[j]-ov[7].p[j];
			}
		} else {
			for(j=0; j<DIM; j++) {
				v1.p[j] = ov[i-1].p[j]-ov[i].p[j];
				v2.p[j] = ov[i+1].p[j]-ov[i].p[j];
			}
		}
		VecN(&v1);														// �P���޸�ى�
		VecN(&v2);
		for(j=0; j<DIM; j++) {
			vec.p[j] = (v1.p[j]+v2.p[j])/2.0;							// ��]�����޸�ٌv�Z
		}
		VecN(&vec);														// �P���޸�ى�
		for(j=0; j<an; j++) {
			wp[j] = pp[j];
		}
		if(fabs(vec.p[1])>EPSILON) {
			th = atan2(vec.p[0], vec.p[1]);								// ��]�p�x�v�Z
			th = (th<-10||th>10) ? 0.0 : th;							// Y���ޕ␳�l
			dd = (i==0||i==3||i==4||i==7) ? fabs(1.0/sin(th)) : fabs(1.0/cos(th));
			for(j=0; j<an; j++) {
				wp[j].p[1] = pp[j].p[1]*dd;								// Y���W�̂ݕ␳
			}
		} else {
			if(v2.p[0]>0.5) {
				th = 0.0;
			} else if(v2.p[0]<-0.5) {
				th = PI;
			} else if(v2.p[1]<-0.5) {
				th = PI/2.0;
			} else {
				th = -PI/2.0;
			}
		}
		SetRotateMatrix(rvc, th, rot);									// ��]��د���쐬
		for(j=0; j<an; j++) {
			VecRotateMove(&wp[j], rot, ov[i]);							// �f�ʉ�]�ړ�
			pb[8*j+i] = wp[j];											// ����_�z��ݒ�
			ib[8*j+i] = ia[j]+1;										// �Ȑ�Ӱ�ސݒ�
		}
	}
	delete[] pp;
	delete[] wp;
}


/**********************************************************/
static void ResetSurfaceObject(OBJTYPE* op, OBJTYPE** rop)				//<<<  �Ȗʗ��̍č쐬
/**********************************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	int*     ia = new int[View->m_MaxSCtA];
	OBJTYPE* cop;
	CMDTYPE* cmp;
	int      un, vn, ub, vb, mx, wt, fg, ct, i, j, mir=0;
	double   dwt;

	if(op!=NULL) {
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		un = GetParaInt(cmp, 0);										// U����_���擾
		vn = GetParaInt(cmp, 1);										// V����_���擾
		ub = GetParaInt(cmp, 2);										// U���EӰ�ގ擾
		vb = GetParaInt(cmp, 3);										// V���EӰ�ގ擾
		mx = GetParaInt(cmp, 4);										// �Ȑ����J�Ԑ�
		wt = GetParaInt(cmp, 5);										// �Ȑ��d�݌W��
		dwt = wt/1000.0;
		fg = GetParaInt(cmp, 6);										// �د��Ӱ��
		for(i=0, ct=0; i<vn; i++) {
			for(j=0; j<un; j++) {
				GetParaPnt(cmp, un*i+j, &pa[ct]);						// ����_�擾
				ia[ct++] = GetParaInt(cmp, un*i+j+7);					// Ӱ�ގ擾
			}
		}
		SetSurface(un, vn, ub, vb, pa);									// �Ȗʐ���_����
		GetObjPtr(&cop);												// ����_OP�ۑ�
		NextCmdPtr(&cmp);												// �ȖʕҏW������
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==MIRROR) {									// ��������ސ�����
				mir++;
			}
			if(GetCmdNo(cmp)!=THICKNESS) {								// ���ݕt���ȊO��
				RedoCommandCopy(cop, cmp);								// ����ޕ��ʎ��s
			}
			NextCmdPtr(&cmp);											// ������ނ�
		}
		j = un*vn;
		for(i=0; i<j; i++) {
			GetVtx(cop, i, &pa[i]);										// ���̕ҏW�㐧��_�擾
		}
		FreeObj(cop, 1);												// ���̍폜
		if(mir%2==1) {
			ReverseSurf(un, vn, pa, ia);								// �Ȗʐ���_���]
		}
		CrtSurface(un, vn, ub, vb, mx, dwt, fg, pa, ia);				// �Ȗʐ���
		GetObjPtr(&cop);												// �������̎擾
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==THICKNESS) {								// ���ݕt����
				RedoCommandCopy(cop, cmp);								// ����ޕ��ʎ��s
			}
			NextCmdPtr(&cmp);											// ������ނ�
		}
		*rop = cop;
	}
	delete[] pa;
	delete[] ia;
}

/******************************************************/
void SetProng(int nm, double ln, double dm, double rv,
			  BOXTYPE obx, int an, PNTTYPE* pa)							//<<< �Θg�ܐݒ�
/******************************************************/
{
	OBJTYPE** sop = new OBJTYPE*[16];
	OBJTYPE** gop = new OBJTYPE*[5];
	OBJTYPE*  op, * wop;
	OIFTYPE*  oif;
	PNTTYPE   pt1, pt2, pt3, cpt;
	VECTYPE   rvc;
	BOXTYPE   box;
	CString   fname;
	int       i, j, cnt=0, gcnt=0;
	double    th, sln, wln, xd, yd, zd;

	BoxCenter(obx, &cpt);												// �ޯ�����S�擾
	cpt.p[1] = obx.bp[0].p[1];											// ���S��YZ�ŏ��_
	cpt.p[2] = obx.bp[0].p[2];
	pt1 = pa[0];
	for(i=1; i<an; i++) {
		pt1.p[1] = (pa[i].p[1]<pt1.p[1]) ? pa[i].p[1] : pt1.p[1];		// 2D-Y�ŏ��_
		pt1.p[2] = (pa[i].p[2]<pt1.p[2]) ? pa[i].p[2] : pt1.p[2];		// 2D-Z�ŏ��_
	}
	for(i=0; i<DIM; i++) {
		pt1.p[i] = cpt.p[i]-pt1.p[i];									// 2D�ړ��ʎZ�o
	}
	for(i=0; i<an; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += pt1.p[j];										// �ޯ����_�ړ�
		}
	}
	pt1 = pa[0];
	for(i=1; i<an; i++) {
		pt1.p[1] = (pa[i].p[1]<pt1.p[1]) ? pa[i].p[1] : pt1.p[1];		// �ړ���2D-Y�ŏ��_
		pt1.p[2] = (pa[i].p[2]<pt1.p[2]) ? pa[i].p[2] : pt1.p[2];		// �ړ���2D-Z�ŏ��_
	}
	sln = PntD(&pt1, &pa[0]);
	for(i=1, j=0; i<an; i++) {
		if((wln=PntD(&pt1, &pa[i]))<sln) {								// �ŏ������_����
			sln = wln, j = i;
		}
	}
	pt1 = pa[j], pt2 = pa[0];											// ������_
	for(i=1; i<an; i++) {
		pt2.p[1] = (pa[i].p[1]<pt2.p[1]) ? pa[i].p[1] : pt2.p[1];		// 2D-Y�ŏ��_
		pt2.p[2] = (pa[i].p[2]>pt2.p[2]) ? pa[i].p[2] : pt2.p[2];		// 2D-Z�ő�_
	}
	sln = PntD(&pt2, &pa[0]);
	for(i=1, j=0; i<an; i++) {
		if((wln=PntD(&pt2, &pa[i]))<sln) {								// �ŏ������_����
			sln = wln, j = i;
		}
	}
	pt2 = pa[j];														// �㕔��_
	BaseObjPtr(2, &op, &i, &j);											// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		gop[gcnt++] = op;												// �΍����̕ۑ�
		NextObjPtr(2, &op, &i, &j);										// ���ҏW���̂�
	}
	OffEdtAllObject();													// �ҏW���̑S����
	fname = GetAppPath(0)+PRO_NAME;										// ̧�ٖ��擾
	if(ReadJsdFile(fname, 0, 0)!=0) {									// ����è�ޓ���
		AfxMessageBox(IDS_CANT_READ_FILE);								// �װү����
		delete[] sop;
		delete[] gop;
		return;															// �������~
	}
	GetObjPtr(&op);														// �����߲���擾
	sop[cnt++] = op;													// �����߲���ۑ�
	OnEdtFlg(1, op);													// ����edtflg:ON
	GetObjBox(op, &box);												// �ޯ�����ގ擾
	BoxCenter(box, &cpt);												// �ޯ�����S�擾
	wln = PntD(&pt1, &pt2);												// ��_�ԋ���
	xd = box.bp[1].p[0]-box.bp[0].p[0];									// �Ǎ����ގZ�o
	yd = box.bp[1].p[1]-box.bp[0].p[1];
	zd = box.bp[1].p[2]-box.bp[0].p[2];
	xd = dm/xd;															// X�����{���Z�o
	yd = dm/yd;															// Y�����{���Z�o
	zd = (ln+wln)/zd;													// Z�����{���Z�o
	Scale(cpt, xd, yd, zd);												// �w�軲��ػ���
	GetObjBox(op, &box);												// �ޯ�����ގ擾
	BoxCenter(box, &cpt);												// �ޯ�����S�擾
	pt3.p[0]=cpt.p[0], pt3.p[1]=box.bp[1].p[1],	pt3.p[2]=box.bp[0].p[2];// �ܑ���_
	th = atan2(pt2.p[1]-pt1.p[1], pt2.p[2]-pt1.p[2]);					// ��]�p�x
	th = (th<-10||th>10) ? 0.0 : th;
	th = 180.0/PI*th;
	rvc.p[0] = -1.0, rvc.p[1] = 0.0, rvc.p[2] = 0.0;					// ��]��
	for(i=0; i<DIM; i++) {
		pt3.p[i] = pt1.p[i]-pt3.p[i];									// �ړ���
	}
	Move(pt3.p[0], pt3.p[1]+rv, pt3.p[2]);								// �܈ړ�
	Rotate(pt1, rvc, th);												// �܉�]
	BoxCenter(obx, &cpt);												// �ޯ�����S�擾
	rvc.p[0] = 0.0, rvc.p[1] = 0.0, rvc.p[2] = -1.0;					// ���ʉ�]��
	th = 360.0/nm;														// ���ʊp�x
	if(nm>=4&&nm%2==0) {												// <�ܐ�:4�ȏ�ŋ�����>
		Rotate(cpt, rvc, th/2.0);										// �܉�](��]�p�x:���ʊp�x/2)
	}
	for(i=1; i<nm; i++) {												// <2�ڈȍ~�𕡎ʥ��]>
		Copy();															// ���̕���
		GetObjPtr(&wop);												// �����߲���擾
		sop[cnt++] = wop;												// �����߲���ۑ�
		OffEdtFlg(1, op);												// ����edtflg:OFF
		OnEdtFlg(1, wop);												// ����edtflg:ON
		Rotate(cpt, rvc, th*i);											// �܉�]
		OffEdtFlg(1, wop);												// ����edtflg:OFF
		OnEdtFlg(1, op);												// ����edtflg:ON
	}
	for(i=0; i<cnt; i++) {
		ResetSurfaceObject(sop[i], &wop);								// �Ȗʗ��̍č쐬
		FreeObj(sop[i], 1);												// ���̍폜
		sop[i] = wop;
	}
	for(i=0; i<cnt; i++) {
		PutObjPrt(sop[i], 2, 1);										// �߰�No�ݒ�(�q)
		GetObjOif(sop[i], &oif);										// ���̍\������߲���擾
		SetOifLevel(oif, 1);											// �\������(1)
		SetOifName(0, oif, OBJ_NAME31);									// �\����(��)
		OnEdtFlg(1, sop[i]);											// ���ʒ�edtflg:ON
	}
	for(i=0; i<gcnt; i++) {
		OnEdtFlg(1, gop[i]);											// �΍�edtflg:ON
	}
	delete[] sop;
	delete[] gop;
}
