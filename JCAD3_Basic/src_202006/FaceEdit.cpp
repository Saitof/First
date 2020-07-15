/////////////////////////////////////////////////////////////////////////////
// ��ʕҏW ���Z��
// FaceEdit.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/20 by M.Fukushima
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
#include "EditCtrlPoint1.h"
#include "CreatCurvePoint.h"
#include "NumFunc.h"
#include "Select.h"
#include "FaceEditType.h"

/******************************/
static void InitWorkArea(void)											//<<< ��ʕҏW��������
/******************************/
{
	Flist = NULL, Fcnt = 0;
}

/*******************************************************/
static void SetFaeData(double x1, double x2, double y1,
                       double y2, VECTYPE vc, int st)					//<<< �ʏ���ް��o�^
/*******************************************************/
{
	FAETYPE* tf;

	tf = (FAETYPE *)malloc(sizeof(FAETYPE));							// �ʏ��̈�m��
	if(tf==NULL) {
		MemErr(IDS_MEMERR5);
	}
	tf->xmin = x1, tf->xmax = x2;
	tf->ymin = y1, tf->ymax = y2;
	tf->vec = vc,  tf->stfl = st;
	tf->next = Flist, Flist = tf, Fcnt++;
}

/************************************************/
static FAETYPE* MergeF(FAETYPE* la, FAETYPE* lb)						//<<< X�ŏ��l���
/************************************************/
{
	FAETYPE* lc, * ld, * rc;

	lc = (FAETYPE *)malloc(sizeof(FAETYPE));							// ��Ɨ̈�m��
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
static FAETYPE* F_Mergsrt(FAETYPE* mlist, int n)						//<<< X�ŏ��l�̿��
/************************************************/
{
	FAETYPE* la, * lb, * lm, * rc;
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
		lm=(FAETYPE *)MergeF(F_Mergsrt(la, n/2), F_Mergsrt(lb, n-n/2));	// ��ٰ�ߔԍ����
		rc = lm;
	}
	return rc;
}

/*************************************/
static void MakeFaceList(OBJTYPE* op)									//<<< �ʏ��ؽč쐬
/*************************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxOPnt];
	PNTTYPE  pt;
	VECTYPE  vc;
	int      i, num, ffg, st, pn;
	double   minx, miny, maxx, maxy;

	num = GetFlpNum1(op);												// ��ٰ�ߐ��擾
	for(i=0, st=-1; i<num; i++) {
		GetFlpVtx1(op, i, &pt, &ffg);									// ��ٰ�ߒ��_�擾
		if(st==-1) {
			pn = 0, st = i;												// ��ٰ�ߊJ�n����
			minx=pt.p[0], maxx=pt.p[0], miny=pt.p[1], maxy=pt.p[1];		// XY�ŏ��ő叉���l
		}
		pa[pn++] = pt;													// ���_���W�ۑ�
		if(pt.p[0]<minx) {												// XY�ŏ��ő�擾
			minx = pt.p[0];
		}
		if(pt.p[0]>maxx) {
			maxx = pt.p[0];
		}
		if(pt.p[1]<miny) {
			miny = pt.p[1];
		}
		if(pt.p[1]>maxy) {
			maxy = pt.p[1];
		}
		if(ffg) {														// <<��ٰ�ߏI��>>
			if(minx<=XPos2&&maxx>=XPos1&&miny<=YPos2&&maxy>=YPos1) {	// <��Ɨ̈���̖�>
				CalcNVec(pa, pn, &vc);									// �@���޸�َ擾
				if(vc.p[2]>EPSILON) {									// <������̖�>
					SetFaeData(minx, maxx, miny, maxy, vc, st);			// �ʏ���ް��o�^
				}
			}
			st = -1;													// ٰ�ߊJ�n�׸ޏ�����
		}
	}
	if(Flist!=NULL) {
		Flist = F_Mergsrt(Flist, Fcnt);									// X�ŏ��l�̿��
	}
	delete[] pa;
}

/************************************************************************/
static BOOL CheckInFace(PNTTYPE pt, FAETYPE* fp, OBJTYPE* op, double* z)//<<< �ʓ���������
/************************************************************************/
{
	PNTTYPE p1, p2, sp, ep;
	int     ix, f1, f2, chx, chy, ucx, dcx, ucy, dcy, ret=FALSE;
	double  d1, d2, sx, sy, ex, ey;

	chx = 0, ucx = 0, dcx = 0, chy = 0, ucy = 0, dcy = 0;
	ix = fp->stfl;														// ��ٰ�ߊJ�n�ԍ�
	while(TRUE) {
		GetFlpVtx1(op, ix, &p1, &f1);									// ��ٰ�ߒ��_�擾
		if(!f1) {
			GetFlpVtx1(op, ix+1, &p2, &f2);								// ��ٰ�ߒ��_�擾
		} else {
			GetFlpVtx1(op, fp->stfl, &p2, &f2);
		}
		if(p1.p[0]<p2.p[0]) {											// �����������n�_
			sp = p1, ep = p2;
		} else {
			sp = p2, ep = p1;
		}
		if(sp.p[0]<=pt.p[0]&&pt.p[0]<ep.p[0]) {							// <�w��_X������X�Ɋ�>
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			d1 = ey-sy,   d2 = ex-sx;
			d1 = d1/d2;													// �X��
			d2 = sy-d1*sx;												// �萔
			d2 = d1*pt.p[0]+d2;											// Y�l�v�Z
			if(d2-pt.p[1]>EPSILON) {									// ������_���w��_����
				ucx++;
			} else if(pt.p[1]-d2>EPSILON) {								// ������_���w��_��菬
				dcx++;
			} else {													// �ڂ���ꍇ
				chx = 1; break;
			}
		}
		if(p1.p[1]<p2.p[1]) {											// �����������n�_
			sp = p1, ep = p2;
		} else {
			sp = p2, ep = p1;
		}
		if(sp.p[1]<=pt.p[1]&&pt.p[1]<ep.p[1]) {							// <�w��_Y������Y�Ɋ�>
			sx = sp.p[0], sy = sp.p[1];
			ex = ep.p[0], ey = ep.p[1];
			d1 = ex-sx,   d2 = ey-sy;
			d1 = d1/d2;													// �X��
			d2 = sx-d1*sy;												// �萔
			d2 = d1*pt.p[1]+d2;											// X�l�v�Z
			if(d2-pt.p[0]>EPSILON) {									// ������_���w��_����
				ucy++;
			} else if(pt.p[0]-d2>EPSILON) {								// ������_���w��_��菬
				dcy++;
			} else {													// �ڂ���ꍇ
				chy = 1; break;
			}
		}
		if((fabs(pt.p[0]-p1.p[0])<EPSILON&&
            fabs(pt.p[1]-p1.p[1])<EPSILON)||
           (fabs(pt.p[0]-p2.p[0])<EPSILON&&
            fabs(pt.p[1]-p2.p[1])<EPSILON)) {							// �n�_���I�_����v����ꍇ
			chx = 1; break;
		}
		if(f1) {														// ��ٰ�ߏI��=>�����I��
			break;
		}
		ix++;
	}
	ucx = ucx%2, dcx = dcx%2;											// 2��1���
	ucy = ucy%2, dcy = dcy%2;
	if(((ucx&&dcx)&&(ucy&&ucy))||chx||chy) {							// <�ʂ̒��ɑ���>
		GetFlpVtx1(op, fp->stfl, &p1, &f1);								// ��ٰ�ߒ��_�擾
		*z = (fp->vec.p[0]*p1.p[0]+fp->vec.p[1]*p1.p[1]+
              fp->vec.p[2]*p1.p[2]-fp->vec.p[0]*pt.p[0]-
              fp->vec.p[1]*pt.p[1])/fp->vec.p[2];						// �w��_(XY)�ʒu�ʏ��Z
		ret = TRUE;
	} else {
		*z = 0.0;
	}
	return ret;
}

/******************************/
static void FreeWorkArea(void)											//<<< ��Ɨ̈���
/******************************/
{
	FAETYPE* wfp, * dfp;

	wfp = Flist;
	while(wfp) {														// �ʏ��̈���
		dfp = wfp, wfp = wfp->next;
		free(dfp);
	}
}

/****************************/
void ObjectSurfaceEdit(void)											//<<< �Ȗʎw��ړ�
/****************************/
{
	PNTTYPE* cpt = new PNTTYPE[View->m_MaxSCtA];
	OBJTYPE* op, * op2, * bop;
	CMDTYPE* cmp;
	PNTTYPE  pnt, pvc;
	VECTYPE  vec, rvc;
	BOXTYPE  box;
	FAETYPE* wfa;
	CString  str;
	int      i, f, cnt=0, ith, imv;
	double   inp=0.0, th, z, zmax, zmin=DBL_MAX;

	View->CheckCtrlKey1();												// ���䷰�m�F(�N����)
	if(View->GetNumInp()!=0) {											// <���l���͂���>
		inp = View->GetBaseZPos();										// ���Z�ʒu�擾
		CInputDlg dlg(25);
		dlg.SetValue(inp);												// �����l�\��
		if(dlg.DoModal()==IDOK) {										// ���Z�ʒu����
			dlg.GetValue(&inp);
			View->SetBaseZPos(inp);										// ���Z�ʒu�ݒ�
		} else {
			delete[] cpt;
			return;														// �޲�۸޷�ݾق͒��~
		}
	}
	SelBaseObjPtr(1, &bop);												// ��Ȗʗ��̎擾
	if(bop==NULL) {														// ��ȖʂȂ��͒��~
		delete[] cpt;
		return;
	}
	XPos1 = DBL_MAX, XPos2 = 1.0-DBL_MAX;
	YPos1 = DBL_MAX, YPos2 = 1.0-DBL_MAX;
	SelBaseObjPtr(2, &op);												// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		if(cnt>=View->m_MaxSCtA) {										// <�ő�z��ȏ�>
			str.Format(IDS_ERR_PROCESS, View->m_MaxSCtA);				// �װү����
			AfxMessageBox(str);
			cnt = -1; break;
		}
		if(op->part==2) {												// <�Ưĥ�q�̏ꍇ>
			op2 = op->oset;												// �Ưĥ�eOP���g�p
			if(op2==NULL) {												// �Ưĥ�eOP��NULL�͎�OP
				op2 = op;
			} else if(op2->edtflg==0) {									// �Ưĥ�eOP����Ώۂ͎�OP
				op2 = op;
			}
		} else {														// <�Ưĥ�q�ȊO�̏ꍇ>
			op2 = op;
		}
		GetObjBox(op2, &box);											// ����BOX���ގ擾
		BoxCenter(box, &pnt);											// BOX�����擾
		cpt[cnt++] = pnt;												// ���S�_�ۑ�
		if(pnt.p[0]<XPos1) {											// �ŏ��ő�擾
			XPos1 = pnt.p[0];
		}
		if(pnt.p[0]>XPos2) {
			XPos2 = pnt.p[0];
		}
		if(pnt.p[1]<YPos1) {
			YPos1 = pnt.p[1];
		}
		if(pnt.p[1]>YPos2) {
			YPos2 = pnt.p[1];
		}
		if(box.bp[0].p[2]<zmin) {
			zmin = box.bp[0].p[2];
		}
		SelNextObjPtr(2, &op);											// ���ҏW�Ώۗ��̎擾
	}
	if(cnt<0) {															// �ő�z��ȏ�͒��~
		delete[] cpt;
		return;
	}
	zmin -= inp;														// Z�ʒu���͒l�����Z
	StartMenu();														// �ƭ��J�n����
	InitLevel();														// Undo/Redo������
	InitWorkArea();														// ��ʕҏW��������
	MakeFaceList(bop);													// ���̖�ؽč쐬
	if(Flist!=NULL) {													// <��ؽėL�̏ꍇ>
		cnt = 0;
		SelBaseObjPtr(2, &op);											// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			pnt = cpt[cnt++], pnt.p[2] = zmin;							// BOX�����擾
			wfa = Flist;												// �擪�ʎ擾
			zmax = 1.0-DBL_MAX, f = 0;
			while(wfa!=NULL) {
				if(CheckInFace(pnt, wfa, bop, &z)) {					// <�ʓ����݂̏ꍇ>
					if(fabs(z-zmax)<0.01) {								// <Z�ő�ɋߎ������>
						for(i=0; i<DIM; i++) {
							vec.p[i] += wfa->vec.p[i];					// �@���޸�ى��Z
						}
					} else if(z>zmax) {									// <Z�ő�̖�>
						zmax = z, vec = wfa->vec, f = 1;				// Z�l/�@���޸�ٕۑ�
					}
				}
				wfa = wfa->next;										// ���ʎ擾
			}
			if(!f) {													// <�ʓ����݂Ȃ�>
				zmax = zmin;											// �ړ��Ȃ�
				vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 1.0;			// ��]�Ȃ�
			} else {													// �ʓ����݂͒P���޸�ى�
				VecN(&vec);
			}
			if(fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON) {		// ����������]�p�x�Z�o
				th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
				th = atan2(th, vec.p[2]);
				th = (th<-10||th>10) ? 0.0 : th;
			} else {
				th = 0.0;
			}
			ith = (int)((180.0*th/PI)*1000.0);							// ����������]�p�x(����)
			if(ith!=0) {												// <����������]����>
				rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
				VecN(&rvc);												// �P���޸�ى�
				for(i=0; i<DIM; i++) {
					pvc.p[i] = rvc.p[i];
				}
				CreatCmd(ROTATE, 1, 2, &cmp);							// ����ޗ̈�m��
				PutParaInt(cmp, 0, ith);								// ��]�p�x�ݒ�
				PutParaPnt(cmp, 0, pnt);								// ��]�����W�ݒ�
				PutParaPnt(cmp, 1, pvc);								// ��]�������޸�ِݒ�
				SetObjPtr(op);											// �Ώۗ��̐ݒ�
				SetCmdPtr(cmp);											// ����ސݒ�
				ExecCommand(FALSE);										// ��]����ގ��s
			}
			imv = (int)((zmax-zmin)*1000.0);							// Z�����ړ���(����)
			if(imv!=0) {												// <Z�����ړ�����>
				CreatCmd(MOVE, 3, 0, &cmp);								// ����ޗ̈�m��
				PutParaInt(cmp, 0, 0);									// �ړ��ʐݒ�
				PutParaInt(cmp, 1, 0);
				PutParaInt(cmp, 2, imv);
				SetObjPtr(op);											// �Ώۗ��̐ݒ�
				SetCmdPtr(cmp);											// ����ސݒ�
				ExecCommand(FALSE);										// �ړ�����ގ��s
			}
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			SelNextObjPtr(2, &op);										// ���ҏW�Ώۗ��̎擾
		}
	}
	FreeWorkArea();														// ��Ɨ̈���
	EndJMenu1();														// �ƭ��I������
	View->SetDataUse();													// �ް��g�p���ݒ�
	delete[] cpt;
}

/**********************************************************************/
void ObjectAdjustCP(int on, int xn, PNTTYPE* px, int* ix, PNTTYPE* pa,
					PNTTYPE* pb, PNTTYPE* pc, PNTTYPE* pd,
					PNTTYPE* pw, int* ia, int* ib, int* ic)				//<<< ����_���̍��킹
/**********************************************************************/
{
	OBJTYPE* op, * op1;
	CMDTYPE* cmp;
	FAETYPE* wfa;
	PNTTYPE  zer;
	int      i, f, i1, i2, un, vn, ub, vb, an, rp, cnt=0;
	double   wd[10], wt, inp=0.0, z, zmax;

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;
	View->CheckCtrlKey1();												// ���䷰�m�F(�N����)
	if(View->GetNumInp()!=0) {											// <���l���͂���>
		inp = View->GetBaseZPos();										// ���Z�ʒu�擾
		CInputDlg dlg(25);
		dlg.SetValue(inp);												// �����l�\��
		if(dlg.DoModal()==IDOK) {										// ���Z�ʒu����
			dlg.GetValue(&inp);
			View->SetBaseZPos(inp);										// ���Z�ʒu�ݒ�
		} else {
			return;														// �޲�۸޷�ݾق͒��~
		}
	}
	SelBaseObjPtr(1, &op);												// ��Ȗʗ��̎擾
	if(op==NULL) {														// ��ȖʂȂ��͒��~
		return;
	}
	XPos1 = DBL_MAX, XPos2 = 1.0-DBL_MAX;
	YPos1 = DBL_MAX, YPos2 = 1.0-DBL_MAX;
	for(i=0; i<xn; i++) {
		if(ix[i]>0) {													// <�I�𐧌�_�̏ꍇ>
			if(px[i].p[0]<XPos1) {										// �ŏ��ő�擾
				XPos1 = px[i].p[0];
			}
			if(px[i].p[0]>XPos2) {
				XPos2 = px[i].p[0];
			}
			if(px[i].p[1]<YPos1) {
				YPos1 = px[i].p[1];
			}
			if(px[i].p[1]>YPos2) {
				YPos2 = px[i].p[1];
			}
		}
	}
	StartMenu();														// �ƭ��J�n����
	InitLevel();														// Undo/Redo������
	InitWorkArea();														// ��ʕҏW��������
	MakeFaceList(op);													// ���̖�ؽč쐬
	if(Flist!=NULL) {													// <<<��ؽėL�̏ꍇ>>>
		for(i=0; i<xn; i++) {
			if(ix[i]>0) {												// <<�I�𐧌�_�̏ꍇ>>
				wfa = Flist;											// �擪�ʎ擾
				zmax = 1.0-DBL_MAX, f = 0;
				while(wfa!=NULL) {
					if(CheckInFace(px[i], wfa, op, &z)) {				// <�ʓ����݂̏ꍇ>
						if(z>zmax) {
							zmax = z, f = 1;							// Z�ő������
						}
					}
					wfa = wfa->next;									// ���ʎ擾
				}
				if(f) {													// <�ʓ����݂���>
					px[i].p[2] = zmax+inp;								// Z�l�͖ʏ�+�␳�l
				}
			}
		}
	}
	FreeWorkArea();														// ��Ɨ̈���
	BaseObjPtr(2, &op, &i1, &i2);										// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {													// <<�ҏW���̗L�̏ꍇ>>
		if(GetSelNo(op)!=1) {											// ��Ȑ��ȊO�o�^
			SetSelNo(op, 2);
		}
		NextObjPtr(2, &op, &i1, &i2);									// ���ҏW���̂�
	}
	OffEdtAllObject();													// �ҏW���̑S����
	SelBaseObjPtr(2, &op);												// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {													// <<�ҏW���̗L�̏ꍇ>>
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		i1 = GetObjAtr(op);												// ���̑����擾
		if((i1==10||i1==11)&&cmp!=NULL) {								// <�Ȑ��̏ꍇ>
			vn = GetParaInt(cmp, 0);									// ����_���擾
			vb = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			rp = GetParaInt(cmp, 2);									// �Ȑ��������擾
			wt = (double)GetParaInt(cmp, 3)/1000.0;						// �Ȑ��d�݌W���擾
			for(i=0; i<vn; i++) {
				GetParaPnt(cmp, i, &pa[i]);								// ����_�擾
				ia[i] = GetParaInt(cmp, 4+i);							// ײ�Ӱ�ގ擾
			}
			SetCurve(vn, vb, pa);										// �Ȑ�����_����
			GetObjPtr(&op1);											// ����_OP�ۑ�
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			NextCmdPtr(&cmp);											// �ȖʕҏW������
			while(cmp!=NULL) {
				RedoCommandCopy(op1, cmp);								// ����ޕ��ʎ��s
				NextCmdPtr(&cmp);										// ������ނ�
			}
			for(i=0; i<vn; i++) {
				GetVtx(op1, i, &pb[i]);									// ���̕ҏW�㐧��_�擾
				if(ix[cnt+i]==4) {										// <�I���ϐ���_>
					ib[i] = 1;											// �w������
					pc[i] = zer;										// Z�ړ�����
					pc[i].p[2] = px[cnt+i].p[2]-pb[i].p[2];
				} else {												// <���I�𐧌�_>
					ib[i] = 0;											// �w���Ȃ�
					pc[i] = zer;										// �ړ��Ȃ�
				}
			}
			FreeObj(op1, 1);											// �ҏW�㗧�̍폜
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<vn; i++) {
					ic[i] = 1;											// ���͗L�͑S�͈͐ݒ�
				}
			}
			OnEdtFlg(1, op);											// �ҏW�Ώۗ��̓o�^
			for(i=0; i<vn; i++) {
				pw[i*2] = pb[i];										// ����_���Ȑ��ҏW�p�ɕύX
			}
			MoveECurve(0, wd, pc, op, vn, vb, pa, pw, ib, pd, ic);		// ����_�ړ�
			ChgCurve(NULL, vn, vb, rp, wt, pd, ia);						// �Ȑ��ύX
			OffEdtFlg(1, op);											// �ҏW�Ώۗ��̉���
			cnt += vn;													// ����_�����v
		} else if((i1==20||i1==21)&&cmp!=NULL) {						// <�ʏ�Ȗʂ̏ꍇ>
			un = GetParaInt(cmp, 0);									// U����_���擾
			vn = GetParaInt(cmp, 1);									// V����_���擾
			ub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			vb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			rp = GetParaInt(cmp, 4);									// �Ȑ��������擾
			wt = (double)GetParaInt(cmp, 5)/1000.0;						// �Ȑ��d�݌W���擾
			an = un*vn;													// ����_��
			for(i=0; i<an; i++) {
				GetParaPnt(cmp, i, &pa[i]);								// ����_�擾
				ia[i] = GetParaInt(cmp, 7+i);							// ײ�Ӱ�ގ擾
			}
			SetSurface(un, vn, ub, vb, pa);								// �Ȗʐ���_����
			GetObjPtr(&op1);											// ����_OP�ۑ�
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			NextCmdPtr(&cmp);											// �ȖʕҏW������
			while(cmp!=NULL) {
				if(GetCmdNo(cmp)!=THICKNESS) {							// <���݂Â��ȊO�̏ꍇ>
					RedoCommandCopy(op1, cmp);							// ����ޕ��ʎ��s
				}
				NextCmdPtr(&cmp);										// ������ނ�
			}
			for(i=0; i<an; i++) {
				GetVtx(op1, i, &pb[i]);									// ���̕ҏW�㐧��_�擾
				if(ix[cnt+i]==4) {										// <�I���ϐ���_>
					ib[i] = 1;											// �w������
					pc[i] = zer;										// Z�ړ�����
					pc[i].p[2] = px[cnt+i].p[2]-pb[i].p[2];
				} else {												// <���I�𐧌�_>
					ib[i] = 0;											// �w���Ȃ�
					pc[i] = zer;										// �ړ��Ȃ�
				}
			}
			FreeObj(op1, 1);											// �ҏW�㗧�̍폜
			if(View->GetSurfaceTension()!=0&&on>1) {
				for(i=0; i<an; i++) {
					ic[i] = 1;											// ���͗L�͑S�͈͐ݒ�
				}
			}
			OnEdtFlg(1, op);											// �ҏW�Ώۗ��̓o�^
			MoveCPoint(0, pc, op, un, vn, ub, vb, wt,
                       ia, ib, pa, pb, pd, ic);							// ����_�ړ�
			ChgSurface(NULL, un, vn, ub, vb, rp, wt, pd, ia);			// �ȖʕύX
			OffEdtFlg(1, op);											// �ҏW�Ώۗ��̉���
			cnt += an;													// ����_�����v
		}
		SelNextObjPtr(2, &op);											// ���ҏW���̂�
	}
	EndJMenu1();														// �ƭ��I������
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***********************/
void ObjectAdjust(void)													//<<< �Ȗʗ��̍��킹
/***********************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxSCtA];
	int*     ia = new int[View->m_MaxSCtA];
	OBJTYPE* op, * bop;
	CMDTYPE* cmp;
	PNTTYPE  pt1, pt2;
	VECTYPE  vec, rvc;
	FAETYPE* wfa;
	CString  str;
	int      i, j, f, un, vn, ub, vb, rp, an;
	double   rot[3][3], wt, inp=0.0, th, z, zmax, zmin=DBL_MAX;

	View->CheckCtrlKey1();												// ���䷰�m�F(�N����)
	if(View->GetNumInp()!=0) {											// <���l���͂���>
		inp = View->GetBaseZPos();										// ���Z�ʒu�擾
		CInputDlg dlg(25);
		dlg.SetValue(inp);												// �����l�\��
		if(dlg.DoModal()==IDOK) {										// ���Z�ʒu����
			dlg.GetValue(&inp);
			View->SetBaseZPos(inp);										// ���Z�ʒu�ݒ�
		} else {
			delete[] pa;
			delete[] ia;
			return;														// �޲�۸޷�ݾق͒��~
		}
	}
	SelBaseObjPtr(1, &bop);												// ��Ȗʗ��̎擾
	if(bop==NULL) {														// ��ȖʂȂ��͒��~
		delete[] pa;
		delete[] ia;
		return;
	}
	XPos1 = DBL_MAX, XPos2 = 1.0-DBL_MAX;
	YPos1 = DBL_MAX, YPos2 = 1.0-DBL_MAX;
	SelBaseObjPtr(2, &op);												// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		i = GetObjAtr(op);												// ���̑����擾
		BaseCmdPtr(1, op, &cmp);										// �L���擪����ގ擾
		if((i==20||i==21)&&cmp!=NULL) {									// <�ʏ�Ȗʂ̏ꍇ>
			un = GetParaInt(cmp, 0);									// U����_���擾
			vn = GetParaInt(cmp, 1);									// V����_���擾
			an = un*vn;													// ����_��
			for(i=0; i<an; i++) {
				GetParaPnt(cmp, i, &pt1);								// ����_�擾
				if(pt1.p[0]<XPos1) {									// �ŏ��ő�擾
					XPos1 = pt1.p[0];
				}
				if(pt1.p[0]>XPos2) {
					XPos2 = pt1.p[0];
				}
				if(pt1.p[1]<YPos1) {
					YPos1 = pt1.p[1];
				}
				if(pt1.p[1]>YPos2) {
					YPos2 = pt1.p[1];
				}
				if(pt1.p[2]<zmin) {
					zmin = pt1.p[2];
				}
			}
		} else {														// �ʏ�ȖʈȊO�͒��~
			delete[] pa;
			delete[] ia;
			return;
		}
		SelNextObjPtr(2, &op);											// ���ҏW�Ώۗ��̎擾
	}
	zmin -= inp;														// Z�ʒu���͒l�����Z
	StartMenu();														// �ƭ��J�n����
	InitLevel();														// Undo/Redo������
	InitWorkArea();														// ��ʕҏW��������
	MakeFaceList(bop);													// ���̖�ؽč쐬
	if(Flist!=NULL) {													// <��ؽėL�̏ꍇ>
		OffEdtAllObject();												// �ҏW���̑S����
		SelBaseObjPtr(2, &op);											// �ҏW�Ώۗ��̎擾
		while(op!=NULL) {
			i = GetObjAtr(op);											// ���̑����擾
			BaseCmdPtr(1, op, &cmp);									// �L���擪����ގ擾
			if((i==20||i==21)&&cmp!=NULL) {								// <�ʏ�Ȗʂ̏ꍇ>
				un = GetParaInt(cmp, 0);								// U����_���擾
				vn = GetParaInt(cmp, 1);								// V����_���擾
				ub = GetParaInt(cmp, 2);								// U���EӰ�ގ擾
				vb = GetParaInt(cmp, 3);								// V���EӰ�ގ擾
				rp = GetParaInt(cmp, 4);								// �Ȑ��������擾
				wt = (double)GetParaInt(cmp, 5)/1000.0;					// �Ȑ��d�݌W���擾
				an = un*vn;												// ����_��
				for(i=0; i<an; i++) {
					GetParaPnt(cmp, i, &pt1);							// ����_�擾
					wfa = Flist;										// �擪�ʎ擾
					zmax = 1.0-DBL_MAX, f = 0;
					while(wfa!=NULL) {
						if(CheckInFace(pt1, wfa, bop, &z)) {			// <�ʓ����݂̏ꍇ>
							if(fabs(z-zmax)<0.01) {						// <Z�ő�ɋߎ������>
								for(j=0; j<DIM; j++) {
									vec.p[j] += wfa->vec.p[j];			// �@���޸�ى��Z
								}
							} else if(z>zmax) {							// <Z�ő�̖�>
								zmax = z, vec = wfa->vec, f = 1;		// Z�l/�@���޸�ٕۑ�
							}
						}
						wfa = wfa->next;								// ���ʎ擾
					}
					if(!f) {											// <�ʓ����݂Ȃ�>
						zmax = zmin;									// �ړ��Ȃ�
						vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 1.0;	// ��]�Ȃ�
					} else {											// �ʓ����݂͒P���޸�ى�
						VecN(&vec);
					}
					pt2.p[0]=0.0, pt2.p[1]=0.0, pt2.p[2]=pt1.p[2]-zmin;	// ��]�O��_�Ƃ̕ψʗ�
					pt1.p[2] = zmax;									// ��]���_
					if(fabs(vec.p[0])>EPSILON||fabs(vec.p[1])>EPSILON) {// ����������]�p�x�Z�o
						th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);
						th = atan2(th, vec.p[2]);
						th = (th<-10||th>10) ? 0.0 : th;
					} else {
						th = 0.0;
					}
					rvc.p[0] = -vec.p[1], rvc.p[1] = vec.p[0], rvc.p[2] = 0.0;
					VecN(&rvc);											// �P���޸�ى�
					SetRotateMatrix(rvc, th, rot);						// ��]��د���쐬
					VecRotateMove(&pt2, rot, pt1);						// ��]�ړ�
					pa[i] = pt2;
					ia[i] = GetParaInt(cmp, 7+i);						// ײ�Ӱ�ގ擾
				}
				OnEdtFlg(1, op);										// �ҏW�Ώۗ��̓o�^
				ChgSurface(NULL, un, vn, ub, vb, rp, wt, pa, ia);		// �ȖʕύX
				OffEdtFlg(1, op);										// �ҏW�Ώۗ��̉���
			}
			SelNextObjPtr(2, &op);										// ���ҏW�Ώۗ��̎擾
		}
	}
	FreeWorkArea();														// ��Ɨ̈���
	EndJMenu1();														// �ƭ��I������
	View->SetDataUse();													// �ް��g�p���ݒ�
	delete[] pa;
	delete[] ia;
}
