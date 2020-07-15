/////////////////////////////////////////////////////////////////////////////
// ���l�֐���
// NumFunc.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/12/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "DataAccess2.h"

/***********************/
double VecL(VECTYPE* v)													//<<< �޸�ْ�
/***********************/
{
	int    i;
	double ln=0.0;

	for(i=0; i<DIM; i++) {
		ln += v->p[i]*v->p[i];
	}
	return sqrt(ln);
}

/***********************************************/
void VecO(VECTYPE* v1, VECTYPE* v2, VECTYPE* v)							//<<< 3D�޸�يO��
/***********************************************/
{
	v->p[0] = v1->p[1]*v2->p[2]-v1->p[2]*v2->p[1];
	v->p[1] = v1->p[2]*v2->p[0]-v1->p[0]*v2->p[2];
	v->p[2] = v1->p[0]*v2->p[1]-v1->p[1]*v2->p[0];
}

/*********************/
void VecN(VECTYPE* v)													//<<< �P���޸�ى�(3D)
/*********************/
{
	double vl;
	int    i;

	vl = VecL(v);
	if(vl>EPSILON) {
		for(i=0; i<DIM; i++) {
			v->p[i] = v->p[i]/vl;
		}
	}
}

/***********************************************/
void VecV(PNTTYPE* vs, PNTTYPE* ve, VECTYPE* v)							//<<< �n�_->�I�_�޸��
/***********************************************/
{
	int i;

	for(i=0; i<DIM; i++) {
		v->p[i] = ve->p[i] - vs->p[i];
	}
}

/*************************************/
double PntD(PNTTYPE* v1, PNTTYPE* v2)									//<<< 2�_�ԋ���(3D)
/*************************************/
{
	int    i;
	double dd, ln=0.0;

	for(i=0; i<DIM; i++) {
		dd = v1->p[i] - v2->p[i];
		ln += dd*dd;
	}
	return sqrt(ln);
}

/*********************************************/
double CalcArea(double a, double b, double c)							//<<< �O�p�`�ʐώZ�o
/*********************************************/
{
	double s, ret=0.0;

	s = (a+b+c)/2.0;													// ��݂̌����ɂ��
	ret = sqrt(s*(s-a)*(s-b)*(s-c));									// �ʐς��Z�o
	return ret;
}

/**************************************************************/
void SetRotateMatrix(VECTYPE axis, double th, double rot[][3])			//<<< ��]��د�����
/**************************************************************/
{
	double cx, sx, dx, r1, r2, r3, r12, r13, r23;

	cx = cos(th),   sx = sin(th),   dx = 1.0 - cx;
	r1 = axis.p[0], r2 = axis.p[1], r3 = axis.p[2];
	r12 = r1*r2,    r13 = r1*r3,    r23 = r2*r3;
	rot[0][0] = cx+dx*r1*r1;
	rot[0][1] = -sx*r3+dx*r12;
	rot[0][2] = sx*r2+dx*r13;
	rot[1][0] = sx*r3+dx*r12;
	rot[1][1] = cx+dx*r2*r2;
	rot[1][2] = -sx*r1+dx*r23;
	rot[2][0] = -sx*r2+dx*r13;
	rot[2][1] = sx*r1+dx*r23;
	rot[2][2] = cx+dx*r3*r3;
}

/*********************************************/
void VecRotate(PNTTYPE* pos, double rot[][3])							//<<< ���_���W��]
/*********************************************/
{
	PNTTYPE pnt;
	int     i, j;
	double  wk;

	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += rot[i][j]*pos->p[j];
		}
		pnt.p[i] = wk;
	}
	for(i=0; i<DIM; i++) {
		pos->p[i] = pnt.p[i];
	}
}

/**************************************************************/
void VecRotateMove(PNTTYPE* pos, double rot[][3], PNTTYPE dis)			//<<< ���_���W��]&���s�ړ�
/**************************************************************/
{
	int i;

	VecRotate(pos, rot);
	for(i=0; i<DIM; i++) {
		pos->p[i] += dis.p[i];
	}
}

/******************************************************/
void Affine(OBJTYPE* op, PNTTYPE cp, double mat[3][3])					//<<< ���_���W�̨ݕϊ�
/******************************************************/
{
	PNTTYPE dis, pnt;
	int     i, j, vnum;
	double  wk;

	for(i=0; i<DIM; i++) {												// �ړ��ʌv�Z
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += mat[i][j]*cp.p[j];
		}
		dis.p[i] = cp.p[i]-wk;
	}
	vnum = GetVtxNum(op);												// ���_���W���擾
	for(i=0; i<vnum; i++) {
		GetVtx(op, i, &pnt);											// ���_���W�擾
		VecRotateMove(&pnt, mat, dis);									// ���_���W����]�ړ�
		PutVtx(op, i, pnt);												// ���_���W�X�V
	}
}

/*****************************************************************/
void AffineVtx(int pn, PNTTYPE* pa, PNTTYPE cp, double mat[3][3])		//<<< ���_���W�̨ݕϊ�
/*****************************************************************/
{
	PNTTYPE dis;
	int     i, j;
	double  wk;

	for(i=0; i<DIM; i++) {												// �ړ��ʌv�Z
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += mat[i][j]*cp.p[j];
		}
		dis.p[i] = cp.p[i]-wk;
	}
	for(i=0; i<pn; i++) {
		VecRotateMove(&pa[i], mat, dis);								// ���_���W����]�ړ�
	}
}

/***********************************************/
void CalcCent(PNTTYPE* pa, int pn, PNTTYPE* cp)							//<<< �}�`�d�S�擾
/***********************************************/
{
	int    i, j;
	double tg;

	for(i=0; i<DIM; i++) {
		for(tg=0.0, j=0; j<pn; j++) {
			tg += pa[j].p[i];
		}
		cp->p[i] = tg/pn;
	}
}

/***********************************************/
void CalcBox(PNTTYPE* pa, int pn, BOXTYPE* box)							//<<< �}�`BOX���ގ擾
/***********************************************/
{
	PNTTYPE mip, map;
	int     i, j;

	mip = pa[0], map = pa[0];
	for(i=1; i<pn; i++) {
		for(j=0; j<DIM; j++) {
			mip.p[j] = (mip.p[j]>pa[i].p[j]) ? pa[i].p[j] : mip.p[j];	// �ŏ��l����
			map.p[j] = (map.p[j]<pa[i].p[j]) ? pa[i].p[j] : map.p[j];	// �ő�l����
		}
	}
	box->bp[0] = mip, box->bp[1] = map;
}

/************************************************/
void CalcNVec(PNTTYPE* pa, int pn, VECTYPE* vec)						//<<< �}�`�@���޸�َ擾
/************************************************/
{
	PNTTYPE p1, p2;
	VECTYPE v;
	int     i, j, j1, j2;
	double  d1, d2;

	v.p[0] = 0.0, v.p[1] = 0.0, v.p[2] = 0.0;
	for(i=0; i<pn; i++) {
		p1 = pa[i];
		p2 = (i==pn-1) ? pa[0] : pa[i+1];
		for(j=0; j<DIM; j++) {
			j1 = (j==2) ? 0 : j+1;
			j2 = (j1==2) ? 0 : j1+1;
			d1 = p1.p[j1] - p2.p[j1];
			d2 = p1.p[j2] + p2.p[j2];
			v.p[j] += d1*d2;
		}
	}
	VecN(&v);															// �P���޸�ى�
	*vec = v;
}

/***********************************************************/
void CalcRotAngleVec(PNTTYPE* pa, double* th, VECTYPE* vec)				//<<< �}�`�@���޸��&�p�x�擾
/***********************************************************/
{
	VECTYPE svc, rvc;
	double  dt;

	VecV(&pa[0], &pa[1], &svc);											// 1->2�_���޸�َ擾
	VecV(&pa[1], &pa[2], &rvc);											// 2->3�_���޸�َ擾
	VecN(&svc);															// �P���޸�ى�
	VecN(&rvc);															// �P���޸�ى�
	if(fabs(svc.p[0]-rvc.p[0])>EPSILON||
       fabs(svc.p[1]-rvc.p[1])>EPSILON||
       fabs(svc.p[2]-rvc.p[2])>EPSILON) {								// 2�޸�يԂ̊p�x
		dt = svc.p[0]*rvc.p[0]+svc.p[1]*rvc.p[1]+svc.p[2]*rvc.p[2];
		dt = (dt>1.0) ? 1.0 : ((dt<-1.0) ? -1.0 : dt);
		*th = acos(dt);
		*th = (*th<-10||*th>10) ? 0.0 : *th;
	} else {
		*th = 0.0;
	}
	if(*th>EPSILON) {
		CalcNVec(pa, 3, vec);											// 2�޸�يԂ̖@���޸��
	} else {
		vec->p[0] = 0.0, vec->p[1] = 0.0, vec->p[2] = 0.0;
	}
}

/********************************/
double CalcRotAngle(PNTTYPE* pa)										//<<< �}�`�p�x�擾
/********************************/
{
	VECTYPE svc, rvc;
	double  th=0.0, dt;

	VecV(&pa[0], &pa[1], &svc);											// 1->2�_���޸�َ擾
	VecV(&pa[1], &pa[2], &rvc);											// 2->3�_���޸�َ擾
	VecN(&svc);															// �P���޸�ى�
	VecN(&rvc);															// �P���޸�ى�
	if(fabs(svc.p[0]-rvc.p[0])>EPSILON||
       fabs(svc.p[1]-rvc.p[1])>EPSILON||
       fabs(svc.p[2]-rvc.p[2])>EPSILON) {								// 2�޸�يԂ̊p�x
		dt = svc.p[0]*rvc.p[0]+svc.p[1]*rvc.p[1]+svc.p[2]*rvc.p[2];
		dt = (dt>1.0) ? 1.0 : ((dt<-1.0) ? -1.0 : dt);
		th = acos(dt);
		th = (th<-10||th>10) ? 0.0 : th;
	}
	return th;
}

/*********************************************/
double CalcAngle(PNTTYPE* pa, int ix, int pm)							//<<< 3�_�̂Ȃ��p�x�擾
/*********************************************/
{
	VECTYPE sv, rv, vc;
	double  th=0.0, dt;

	VecV(&pa[0], &pa[1], &sv);											// 1->2�_���޸�َ擾
	VecN(&sv);															// �P���޸�ى�
	VecV(&pa[1], &pa[2], &rv);											// 2->3�_���޸�َ擾
	VecN(&rv);															// �P���޸�ى�
	if(fabs(sv.p[0]-rv.p[0])>EPSILON||
       fabs(sv.p[1]-rv.p[1])>EPSILON||
       fabs(sv.p[2]-rv.p[2])>EPSILON) {
		dt = sv.p[0]*rv.p[0]+sv.p[1]*rv.p[1]+sv.p[2]*rv.p[2];
		dt = (dt>1.0) ? 1.0 : ((dt<-1.0) ? -1.0 : dt);
		th = acos(dt);													// 2�޸�يԂ̊p�x
		th = (th<-10||th>10) ? 0.0 : th;
	} else {
		th = 0.0;
	}
	if(th>EPSILON) {
		CalcNVec(pa, 3, &vc);											// 2�޸�يԂ̖@���޸��
	} else {
		vc.p[0] = 0.0, vc.p[1] = 0.0, vc.p[2] = 0.0;
	}
	th = (pm==0&&vc.p[ix]<0.0) ? -th : ((pm==1&&vc.p[ix]>0.0)?-th:th);	// �@���޸�قƋt������ϲŽ
	return th;
}

/*********************************************************/
double CalcBaseAngle(PNTTYPE pt1, PNTTYPE pt2, int plane)				//<<< ����p�x�擾
/*********************************************************/
{
	int    i1, i2;
	double d1, d2, th;

	i1 = plane;															// ���W���ݒ�
	i2 = (plane+1>2) ? 0 : plane+1;
	d1 = pt2.p[i1] - pt1.p[i1];
	d2 = pt2.p[i2] - pt1.p[i2];
	th = atan2(d2, d1);													// ����p�x
	th = (th<-10||th>10) ? 0.0 : th;
	return th;
}

/******************************************************/
void CalcLinePoint(int plane, int flg, double a,
				   double b, PNTTYPE* ip, PNTTYPE* op)					//<<< ������_�Z�o
/******************************************************/
{
	int    i1, i2;
	double c;

	i1 = plane;															// ���W���ݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	if(flg==1) {														// <i1�����s����>
		op->p[i1] = a;													// i1�����W
		op->p[i2] = ip->p[i2];											// i2�����W
	} else if(flg==2) {													// <i2�����s����>
		op->p[i1] = ip->p[i1];											// i1�����W
		op->p[i2] = b;													// i2�����W
	} else {															// <i1,i2���񕽍s����>
		c = ip->p[i1]/a+ip->p[i2];										// �w��_��ʂ鐂���萔
		op->p[i1] = (c-b)*a/(a*a+1);									// i1�����W
		op->p[i2] = a*(c-b)*a/(a*a+1)+b;								// i2�����W
	}
}

/***********************************************************/
double DistPointLine(PNTTYPE* sp, PNTTYPE* ep, PNTTYPE* pt,
					 int* fg, double* sl, PNTTYPE* op)					//<<< �_<->�����ŒZ�����擾
/***********************************************************/
{
	int    i;
	double a, b, c, c1, c2, d, s, ln;

	a = PntD(sp, pt);													// �_<->�n�_����
	b = PntD(ep, pt);													// �_<->�I�_����
	c = PntD(sp, ep);													// �n�_<->�I�_����
	d = (a+b+c)/2;
	s = sqrt(d*(d-a)*(d-b)*(d-c));										// �O�p�`�ʐ�
	if(s<EPSILON) {														// <�ʐ�=0�̏ꍇ>
		if(fabs(a+c-b)<EPSILON) {
			ln = a, *fg = 1, *sl = 0, *op = *sp;
		} else if(fabs(b+c-a)<EPSILON) {
			ln = b, *fg = 2, *sl = c, *op = *ep;
		} else {
			ln = DBL_MAX, *fg = -1, *sl = 0, *op = *sp;
		}
	} else {															// <�ʐ�>0�̏ꍇ>
		ln = 2*s/c;														// �_����̐�����(����)
		c1 = (a*a-ln*ln<0) ? 0 : a*a-ln*ln;								// �����ŕ����������1
		c2 = (b*b-ln*ln<0) ? 0 : b*b-ln*ln;								// �����ŕ����������2
		c1 = sqrt(c1), c2 = sqrt(c2);
		if(c<c1||c<c2) {												// <����<��Ӎ��v>
			if(c1<c2) {													// ��ӒZ�̗p
				ln = a, *fg = 1, *sl = 0, *op = *sp;
			} else {
				ln = b, *fg = 2, *sl = c, *op = *ep;
			}
		} else {
			for(i=0; i<DIM; i++) {
				a = (ep->p[i]-sp->p[i])*c1/c;							// ������_�ʒu�擾
				op->p[i] = sp->p[i]+a;
			}
			*fg = 0, *sl = c1;
		}
	}
	return ln;
}

/*******************************************/
double BoxCenter(BOXTYPE box, PNTTYPE* pnt)								//<<< �ޯ������
/*******************************************/
{
	double x, y, z, ln=0;

	x = box.bp[1].p[0]-box.bp[0].p[0];									// X�����̒���
	y = box.bp[1].p[1]-box.bp[0].p[1];									// Y�����̒���
	z = box.bp[1].p[2]-box.bp[0].p[2];									// Z�����̒���
	pnt->p[0] = box.bp[0].p[0]+x/2.0;									// ���S�_�Z�o
	pnt->p[1] = box.bp[0].p[1]+y/2.0;
	pnt->p[2] = box.bp[0].p[2]+z/2.0;
	ln = (x<y) ? y : x;													// �ő咷���Z�o
	ln = (ln<z) ? z : ln;
	return ln;
}

/**************************************************/
BOOL BoxCheck(int plane, BOXTYPE box, PNTTYPE pnt)						//<<< �ޯ�������݊m�F
/**************************************************/
{
	int i1, i2;

	i1 = plane;															// ���W���ݒ�
	i2 = (plane+1>2) ? 0 : plane+1;
	if(pnt.p[i1]<box.bp[0].p[i1]) {
		return FALSE;
	}
	if(pnt.p[i1]>box.bp[1].p[i1]) {
		return FALSE;
	}
	if(pnt.p[i2]<box.bp[0].p[i2]) {
		return FALSE;
	}
	if(pnt.p[i2]>box.bp[1].p[i2]) {
		return FALSE;
	}
	return TRUE;
}

/**********************************************************************/
void GetTensionFltValue(int mode, double aln, double bln, double* dat)	//<<< ���͐ݒ�l�ϊ�(����)
/**********************************************************************/
{
	double dd;

	if(mode==0) {														// <Ӱ��:0>
		*dat = 0.0;
	} else if(mode==1) {												// <Ӱ��:1>
		dd = bln/aln*3.0;												// �߰��ĎZ�o
		dd = (4.0/(dd+1.0))-1.0;										// ���͌W���v�Z
		*dat = (*dat)*dd/3.0;											// ���͌v�Z�㐔�l
	} else if(mode==2) {												// <Ӱ��:2>
		dd = (aln-bln)/aln;												// �߰��ĎZ�o
		*dat = (*dat)*dd;												// ���͌v�Z�㐔�l
	} else if(mode==3) {												// <Ӱ��:3>
		dd = (aln-bln)/aln*3.0;											// �߰��ĎZ�o
		dd = 4.0-(4.0/(dd+1.0));										// ���͌W���v�Z
		*dat = (*dat)*dd/3.0;											// ���͌v�Z�㐔�l
	}
}

/***********************************************************************/
void GetTensionPntValue(int mode, double aln, double bln, PNTTYPE* dat)	//<<< ���͐ݒ�l�ϊ�(���_)
/***********************************************************************/
{
	double dd;

	if(mode==0) {														// <Ӱ��:0>
		dat->p[0] = 0.0;
		dat->p[1] = 0.0;
		dat->p[2] = 0.0;
	} else if(mode==1) {												// <Ӱ��:1>
		dd = bln/aln*3.0;												// �߰��ĎZ�o
		dd = (4.0/(dd+1.0))-1.0;										// ���͌W���v�Z
		dat->p[0] = dat->p[0]*dd/3.0;									// ���͌v�Z�㐔�l
		dat->p[1] = dat->p[1]*dd/3.0;
		dat->p[2] = dat->p[2]*dd/3.0;
	} else if(mode==2) {												// <Ӱ��:2>
		dd = (aln-bln)/aln;												// �߰��ĎZ�o
		dat->p[0] = dat->p[0]*dd;										// ���͌v�Z�㐔�l
		dat->p[1] = dat->p[1]*dd;
		dat->p[2] = dat->p[2]*dd;
	} else if(mode==3) {												// <Ӱ��:3>
		dd = (aln-bln)/aln*3.0;											// �߰��ĎZ�o
		dd = 4.0-(4.0/(dd+1.0));										// ���͌W���v�Z
		dat->p[0] = dat->p[0]*dd/3.0;									// ���͌v�Z�㐔�l
		dat->p[1] = dat->p[1]*dd/3.0;
		dat->p[2] = dat->p[2]*dd/3.0;
	}
}

/*********************************************/
BOOL CheckNextPoint(PNTTYPE pt1, PNTTYPE pt2)							//<<< ���_�L������
/*********************************************/
{
	if(fabs(pt1.p[0]-pt2.p[0])>0.099999||
       fabs(pt1.p[1]-pt2.p[1])>0.099999||
       fabs(pt1.p[2]-pt2.p[2])>0.099999) {								// 2�_�Ԃ̋���
		return TRUE;													// 1����0.1�o�ȏ��OK
	} else {
		return FALSE;
	}
}

/**************************************************/
void ScrToPnt(int flg, SCRTYPE* scr, PNTTYPE* pnt)						//<<< SCRTYPE<=>PNTTYPE�ϊ�
/**************************************************/
{
	int i;

	if(flg==0) {														// <SCRTYPE=>PNTTYPE>
		for(i=0; i<DIM; i++) {
			pnt->p[i] = (double)scr->p[i]/1000.0;
		}
	} else {															// <PNTTYPE=>SCRTYPE>
		for(i=0; i<DIM; i++) {
  			scr->p[i] = (int)(pnt->p[i]*1000.0+(pnt->p[i]<0?-0.5:0.5));
		}
	}
}

/*********************************************/
double CalcVecAngle(VECTYPE* v1, VECTYPE* v2)							//<<< 2�޸�يԊp�x�Z�o
/*********************************************/
{
	double th=0.0;

	if(fabs(v1->p[0]-v2->p[0])>EPSILON||
       fabs(v1->p[1]-v2->p[1])>EPSILON||
       fabs(v1->p[2]-v2->p[2])>EPSILON) {
		th = v1->p[0]*v2->p[0]+v1->p[1]*v2->p[1]+v1->p[2]*v2->p[2];
		th = (th>1.0) ? 1.0 : ((th<-1.0) ? -1.0 : th);
		th = acos(th);													// 2�޸�يԊp�x
		th = (th<-10||th>10) ? 0.0 : th;
	}
	return th;
}
