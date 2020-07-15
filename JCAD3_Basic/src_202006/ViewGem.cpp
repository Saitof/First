/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbView�N���X(CJcad3GlbView)��`
// ViewGem.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "DataFile1.h"
#include "Select.h"
#include "NumFunc.h"

/************************************************/
static BOOL InputGemSize(double* xd, double* hd)                        //<<< �΂̒��a,��������
/************************************************/
{
	CInputDlg dlg(8);
	dlg.SetValue(*xd, *hd);												// �����l�\��
	if(dlg.DoModal()==IDOK) {											// ���l�����޲�۸ޕ\��
		dlg.GetValue(xd, hd);											// ���ގ擾
		return TRUE;
	} else {
		return FALSE;
	}
}

/************************************************************/
static BOOL InputGemSize(double* xd, double* yd, double* hd)            //<<< �΂�X,Y,��������
/************************************************************/
{
	CInputDlg dlg(7);
	dlg.SetValue(*xd, *yd, *hd);										// �����l�\��
	if(dlg.DoModal()==IDOK) {											// ���l�����޲�۸ޕ\��
		dlg.GetValue(xd, yd, hd);										// ���ގ擾
		return TRUE;
	} else {
		return FALSE;
	}
}

/****************************************************/
static void SetBrilliantVtx(PNTTYPE* pa, double xd,
                            double hd, PNTTYPE cent)                    //<<< ���ر�Ē��_�ݒ�
/****************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, d3, d4, z0, z1, z2, z3, z4;

	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		z0 = -xd*0.431, z1 = -xd*0.33, z2 = 0.0;
		z4 = xd*0.146, z3 = z4*2.0/3.0;
	} else {															// �����w��L�̏ꍇ
		z0 = -hd*0.431/0.577, z1 = -hd*0.33/0.577, z2 = 0.0;
		z4 = hd*0.146/0.577, z3 = z4*2.0/3.0;
	}
	dth = PI/8.0;
	d2 = xd/2.0;
	d1 = (d2-d2*z1/z0)/cos(dth);
	d4 = xd*0.575/2.0;
	d3 = ((d2-d4)/3.0+d4)/cos(dth);
	pa[vn].p[0] = 0, pa[vn].p[1] = 0, pa[vn++].p[2] = z0;				// ���_�̐ݒ�
	for(th=(2.0*PI)+dth, i=0; i<16; th-=dth, i++) {
		sp = sin(th), cp = cos(th);
		if(i%2) {
			pa[vn].p[0] = d4*cp, pa[vn].p[1] = d4*sp, pa[vn++].p[2] = z4;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
		} else {
			pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
			pa[vn].p[0] = d3*cp, pa[vn].p[1] = d3*sp, pa[vn++].p[2] = z3;
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/*************************************************************/
static void SetMarquiseVtx(PNTTYPE* pa, double xd, double yd,
                           double hd, PNTTYPE cent)                     //<<< ϰ���ޒ��_�ݒ�
/*************************************************************/
{
	PNTTYPE pt, p0, p1;
	VECTYPE vc;
	int     i, j, sw, vn=0;
	double  dx, dy, l0, l1, l2, l3, th, t0, t1, z0, z1, z3, z4, dd;

	if(xd>yd) {
		dx = yd, dy = xd, sw = 1;
	} else {
		dx = xd, dy = yd, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		z0 = -dx*0.431, z1 = -dx*0.33, z4 = dx*0.146, z3 = z4*2.0/3.0;
	} else {															// �����w��L�̏ꍇ
		z0 = -hd*0.431/0.577, z1 = -hd*0.33/0.577;
		z4 = hd*0.146/0.577, z3 = z4*2.0/3.0;
	}
	for(i=0; i<DIM; i++) {
		p0.p[i] = 0.0, p1.p[i] = 0.0;									// ��_������
	}
	l0 = sqrt(dx*dx+dy*dy)/2.0;											// ��_�Z�o�v�Z
	t0 = acos(dx/(l0*2.0));
	t0 = (t0<-10||t0>10) ? 0.0 : t0;
	t1 = PI-t0*2.0;
	l0 = l0/(cos(t0)*2.0);
	l1 = dx/2.0-((dx/2.0-dx*0.575/2.0)/3.0+dx*0.575/2.0)/cos(PI/8.0);
	l2 = dx/2.0-dx*0.575/2.0;
	p1.p[0] = l0-dx/2.0;												// ��_�ݒ�
	t0 = t1/4.0;
	pa[vn] = p0, pa[vn++].p[2] = z0;									// ���_�̐ݒ�
	for(th=PI+t1-t0, i=0; i<8; th-=t0, i++) {
		pt.p[0] = l0*cos(th)+p1.p[0], pt.p[1] = l0*sin(th), pt.p[2] = 0.0;
		if(i!=7) {
			if(i%2) {
				VecV(&pt, &p1, &vc);									// �޸�َ擾
				VecN(&vc);												// �P���޸�ى�
				pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
				pa[vn++].p[2] = z4;
				pa[vn++] = pt;
			} else {
				VecV(&pt, &pa[0], &vc);									// �޸�َ擾
				VecN(&vc);												// �P���޸�ى�
				l3 = PntD(&pt, &pa[0]);									// 2�_�Ԃ̋���
				l3 = l3*z1/z0;											// �L������
				pa[vn].p[0] = pt.p[0]+l3*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l3*vc.p[1];
				pa[vn++].p[2] = pt.p[2]+l3*vc.p[2];
				pa[vn++] = pt;
				VecV(&pt, &p1, &vc);									// �޸�َ擾
				VecN(&vc);												// �P���޸�ى�
				pa[vn].p[0] = pt.p[0]+l1*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l1*vc.p[1];
				pa[vn++].p[2] = z3;
			}
		} else {
			VecV(&pt, &p0, &vc);										// �޸�َ擾
			VecN(&vc);													// �P���޸�ى�
			l2 *= dy/dx;
			pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
			pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
			pa[vn++].p[2] = z4;
			pa[vn++] = pt;
		}
	}
	for(i=1; i<21; i++) {
		pa[i+20].p[0] = -pa[i].p[0];									// 180�x���]����
		pa[i+20].p[1] = -pa[i].p[1];
		pa[i+20].p[2] = pa[i].p[2];
	}
	if(sw) {															// <XY�����ւ����ꍇ>
		for(i=0; i<41; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90�x��]
		}
	}
	for(i=0; i<41; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/**************************************************************/
static void SetPearShapeVtx(PNTTYPE* pa, double xd, double yd,
                            double hd, PNTTYPE cent)                    //<<< �߱����ߒ��_�ݒ�
/**************************************************************/
{
	PNTTYPE pt, p0, p1;
	VECTYPE vc;
	int     i, j, sw, vn=0;
	double  dx, dy, md, l0, l1, l2, l3, th, t0, t1, sp, cp, d1, d2, d3, d4, z0, z1, z2, z3, z4;

	if(xd>yd) {
		dx = yd, dy = xd, sw = 1;
	} else {
		dx = xd, dy = yd, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		z0 = -dx*0.431, z1 = -dx*0.33, z2 = 0.0;
		z4 = dx*0.146, z3 = z4*2.0/3.0;
	} else {															// �����w��L�̏ꍇ
		z0 = -hd*0.431/0.577, z1 = -hd*0.33/0.577, z2 = 0.0;
		z4 = hd*0.146/0.577, z3 = z4*2.0/3.0;
	}
	md = (dy-dx)/2.0;
	dy = (dy-dx/2.0)*2.0;
	d2 = dx/2.0;
	d1 = (d2-d2*z1/z0)/cos(PI/8.0);
	d4 = dx*0.575/2.0;
	d3 = ((d2-d4)/3.0+d4)/cos(PI/8.0);
	for(i=0; i<DIM; i++) {
		p0.p[i] = 0.0, p1.p[i] = 0.0;									// ��_������
	}
	l0 = sqrt(dx*dx+dy*dy)/2.0;											// ��_�Z�o�v�Z
	t0 = acos(dx/(l0*2.0));
	t0 = (t0<-10||t0>10) ? 0.0 : t0;
	t1 = PI-(t0*2.0);
	l0 = l0/(cos(t0)*2.0);
	l1 = d2-d3, l2 = d2-d4;
	p1.p[0] = l0-dx/2.0;												// ��_�ݒ�
	t0 = t1/4.0;
	pa[vn] = p0, pa[vn++].p[2] = z0;									// ���_�̐ݒ�
	for(th=PI+t1-t0, i=0; i<8; th-=t0, i++) {
		pt.p[0] = l0*cos(th)+p1.p[0], pt.p[1] = l0*sin(th), pt.p[2] = 0;
		if(i!=7) {
			if(i%2) {
				VecV(&pt, &p1, &vc);									// �޸�َ擾
				VecN(&vc);												// �P���޸�ى�
				pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
				pa[vn++].p[2] = z4;
				pa[vn++] = pt;
			} else {
				VecV(&pt, &pa[0], &vc);									// �޸�َ擾
				VecN(&vc);												// �P���޸�ى�
				l3 = PntD(&pt, &pa[0]);									// 2�_�Ԃ̋���
				l3 = l3*z1/z0;											// �L������
				pa[vn].p[0] = pt.p[0]+l3*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l3*vc.p[1];
				pa[vn++].p[2] = pt.p[2]+l3*vc.p[2];
				pa[vn++] = pt;
				VecV(&pt, &p1, &vc);									// �޸�َ擾
				VecN(&vc);												// �P���޸�ى�
				pa[vn].p[0] = pt.p[0]+l1*vc.p[0];
				pa[vn].p[1] = pt.p[1]+l1*vc.p[1];
				pa[vn++].p[2] = z3;
			}
		} else {
			VecV(&pt, &p0, &vc);										// �޸�َ擾
			VecN(&vc);													// �P���޸�ى�
			l2 *= dy/dx;
			pa[vn].p[0] = pt.p[0]+l2*vc.p[0];
			pa[vn].p[1] = pt.p[1]+l2*vc.p[1];
			pa[vn++].p[2] = z4;
			pa[vn++] = pt;
		}
	}
	for(i=1; i<21; i++) {
		pa[i+20].p[0] = -pa[i].p[0];									// 180�x���]����
		pa[i+20].p[1] = -pa[i].p[1];
		pa[i+20].p[2] = pa[i].p[2];
	}
	t0 = PI/8.0;
	for(th=PI, vn=9, i=0; i<9; th-=t0, i++) {							// ������׳��ނɕό`
		sp = sin(th), cp = cos(th);
		if(i%2) {
			pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
			pa[vn].p[0] = d3*cp, pa[vn].p[1] = d3*sp, pa[vn++].p[2] = z3;
		} else {
			pa[vn].p[0] = d4*cp, pa[vn].p[1] = d4*sp, pa[vn++].p[2] = z4;
			pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
		}
	}
	for(i=0; i<41; i++) {
		pa[i].p[1] += md;												// ���̂̒��S�����_�ɂ���
	}
	if(sw) {															// <XY�����ւ����ꍇ>
		for(i=0; i<41; i++) {
			md = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -md;	// 90�x��]
		}
	}
	for(i=0; i<41; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/***************************************************************/
static void SetHeartShapeVtx(PNTTYPE* pa, double xd, double yd,
                             double hd, PNTTYPE cent)                   //<<< ʰļ���ߒ��_�ݒ�
/***************************************************************/
{
	PNTTYPE cp;
	int     i, j;
	double  minx, miny, minz, maxx, maxy, maxz;
	double  dx, dy, dh, gx, gy, gz, mat[3][3], sx, sy, sz;

	dx = xd, dy = yd;													// �w��X/Y����
	dh = (dx<dy) ? dx : dy;												// �Z�Ӓ����̗p
	dh = (fabs(hd)<EPSILON) ? dh*0.577 : hd;							// �w�荂��
	minx = DBL_MAX, miny = DBL_MAX, minz = DBL_MAX;
	maxx = 1.0-DBL_MAX, maxy = 1.0-DBL_MAX, maxz = 1.0-DBL_MAX;
	for(i=0; i<41; i++) {
		minx = (minx>pa[i].p[0]) ? pa[i].p[0] : minx;					// �ŏ�/�ő�擾
		miny = (miny>pa[i].p[1]) ? pa[i].p[1] : miny;
		minz = (minz>pa[i].p[2]) ? pa[i].p[2] : minz;
		maxx = (maxx<pa[i].p[0]) ? pa[i].p[0] : maxx;
		maxy = (maxy<pa[i].p[1]) ? pa[i].p[1] : maxy;
		maxz = (maxz<pa[i].p[2]) ? pa[i].p[2] : maxz;
	}
	gx = maxx-minx, gy = maxy-miny, gz = maxz-minz;						// ����X����/Y����/����
	cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;						// �̨ݕϊ��̊�_
	sx = dx/gx, sy = dy/gy, sz = dh/gz;									// X/Y/Z�����̔{��
	mat[0][0] = sx,  mat[0][1] = 0.0, mat[0][2] = 0.0;					// �̨ݕϊ���د��
	mat[1][0] = 0.0, mat[1][1] = sy,  mat[1][2] = 0.0;
	mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = sz;
	AffineVtx(41, pa, cp, mat);											// ���_�̨̱ݕϊ�
	for(i=0; i<41; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/*************************************************/
static void SetSingleVtx(PNTTYPE* pa, double xd,
                         double hd, PNTTYPE cent)                       //<<< �ݸ�ْ��_�ݒ�
/*************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, z0, z1, z2;

	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		z0 = -xd*0.431, z1 = 0.0, z2 = xd*0.146;
	} else {															// �����w��L�̏ꍇ
		z0 = -hd*0.431/0.577, z1 = 0.0, z2 = hd*0.146/0.577;
	}
	dth = PI/4.0;
	d1 = xd/2.0;
	d2 = xd*0.575/2.0;
	pa[vn].p[0] = 0.0, pa[vn].p[1] = 0.0, pa[vn++].p[2] = z0;			// ���_�̐ݒ�
	for(th=2.0*PI, i=0; i<8; th-=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
		pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

double zpa[7] = { 0.447, 0.402, 0.246, 0.0, -0.56, -0.94, -1.34 };
double xpa[6] = { 0.434, 0.346, 0.173, 0.0, 0.29, 0.58 };
double tan60  = 2.4142;

/************************************************************/
static void SetEmeraldVtx(PNTTYPE* pa, double xd, double yd,
                          double hd, PNTTYPE cent)                      //<<< �����ޒ��_�ݒ�
/************************************************************/
{
	int    i, j, sw, vn=0;
	double dx, dy, dd, wzpa[7];

	if(xd>yd) {
		dx = yd/2.0, dy = xd/2.0, sw = 1;
	} else {
		dx = xd/2.0, dy = yd/2.0, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		for(i=0; i<7; i++) {
			wzpa[i] = dx*zpa[i];
		}
	} else {															// �����w��L�̏ꍇ
		for(i=0; i<7; i++) {
			wzpa[i] = hd*zpa[i]/1.787;
		}
	}
	for(i=0; i<6; i++) {
		dd = (xpa[i]/tan60+0.4)*dx;										// ���_�̐ݒ�
		pa[vn].p[0] = dx-dx*xpa[i], pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = dy-dx*xpa[i], pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = dy-dx*xpa[i], pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dx*xpa[i]), pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dx*xpa[i]), pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = -(dy-dx*xpa[i]), pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = -(dy-dx*xpa[i]), pa[vn++].p[2] = wzpa[i];
		pa[vn].p[0] = dx-dx*xpa[i], pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wzpa[i];
	}
	pa[vn].p[0] = 0.0, pa[vn].p[1] = dy-dx, pa[vn++].p[2] = wzpa[6];
	pa[vn].p[0] = 0.0, pa[vn].p[1] = -(dy-dx), pa[vn++].p[2] = wzpa[6];
	if(sw) {															// <XY�����ւ����ꍇ>
		for(i=0; i<vn; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90�x��]
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

double bzpa[4] = { 0.45, 0.0, -0.8, -1.2 };
double bxpa[3] = { 0.4, 0.0, 0.5 };

/*************************************************************/
static void SetBaguetteVtx(PNTTYPE* pa, double xd, double yd,
                           double hd, PNTTYPE cent)                     //<<< �޹ޯĒ��_�ݒ�
/*************************************************************/
{
	int    i, j, sw, vn=0;
	double dx, dy, dd, wbzpa[4];

	if(xd>yd) {
		dx = yd/2.0, dy = xd/2.0, sw = 1;
	} else {
		dx = xd/2.0, dy = yd/2.0, sw = 0;
	}
	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		for(i=0; i<4; i++) {
			wbzpa[i] = dx*bzpa[i];
		}
    } else {															// �����w��L�̏ꍇ
		for(i=0; i<4; i++) {
			wbzpa[i] = hd*bzpa[i]/1.65;
		}
	}
	for(i=0; i<3; i++) {
		dd = bxpa[i]*dx;												// ���_�̐ݒ�
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
	}
	pa[vn].p[0] = 0.0, pa[vn].p[1] = dy-dx, pa[vn++].p[2] = wbzpa[3];
	pa[vn].p[0] = 0.0, pa[vn].p[1] = -(dy-dx), pa[vn++].p[2] = wbzpa[3];
	if(sw) {															// <XY�����ւ����ꍇ>
		for(i=0; i<vn; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90�x��]
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/***********************************************************************/
static void SetTaperedVtx(PNTTYPE* pa, double xd, double yd, double hd,
                          double xh, double yh, int fg, PNTTYPE cent)   //<<< ð�߰�ޥ�޹ޯĒ��_�ݒ�
/***********************************************************************/
{
	VECTYPE v1, v2;
	int     i, j, i1, i2, sw, ix, vn=0, uf=0;
	double  dx, dy, hx, hy, dd, xx, ln, wbzpa[4];

	if(xd>yd) {
		dx = yd/2.0, dy = xd/2.0, hx = yh, hy = xh, sw = 1;
	} else {
		dx = xd/2.0, dy = yd/2.0, hx = xh, hy = yh, sw = 0;
	}
	if(xd>yd&&xh>0) {
		fg = (!fg) ? 1 : 0;
	}
	xx = (dx-hx>dy-hy) ? dy-hy : dx-hx;
	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		for(i=0; i<4; i++) {
			wbzpa[i] = xx*bzpa[i];
		}
	} else {															// �����w��L�̏ꍇ
		for(i=0; i<4; i++) {
			wbzpa[i] = hd*bzpa[i]/1.65;
		}
	}
	for(i=0; i<3; i++) {
		dd = bxpa[i]*xx;												// ���_�̐ݒ�
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = dy-dd, pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = -(dx-dd), pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
		pa[vn].p[0] = dx-dd, pa[vn].p[1] = -(dy-dd), pa[vn++].p[2] = wbzpa[i];
	}
	pa[vn].p[0] = 0.0, pa[vn].p[1] = dy-dx, pa[vn++].p[2] = wbzpa[3];
	pa[vn].p[0] = 0.0, pa[vn].p[1] = -(dy-dx), pa[vn++].p[2] = wbzpa[3];
	if(hx>0) {															// �␳�l������ꍇ��
		if(!fg) {
			pa[6].p[0] += hx, pa[7].p[0] -= hx;							// �␳�l���̍��W�ړ�
		} else {
			pa[5].p[0] += hx, pa[4].p[0] -= hx;
		}
		uf = 1, ix = 1;
	} else if(hy>0) {
		if(!fg) {
			pa[6].p[1] += hy, pa[5].p[1] -= hy;
		} else {
			pa[7].p[1] += hy, pa[4].p[1] -= hy;
		}
		uf = 1, ix = 0;
	}
	if(uf) {															// <�␳�L>
		for(i=0; i<4; i++) {
			i1 = (i-1<0) ? 3 : i-1;
			i2 = (i+1>3) ? 0 : i+1;
			VecV(&pa[i+4], &pa[i1+4], &v1);								// �אڂ���2�_�ւ�
			VecN(&v1);													// �P���޸�ق��Z�o
			VecV(&pa[i+4], &pa[i2+4], &v2);
			VecN(&v2);
			for(j=0; j<DIM; j++) {										// �޸�ق̘a
				v1.p[j] += v2.p[j];
			}
			VecN(&v1);													// �P���޸�ى�
			ln = fabs((pa[i+4].p[ix]-pa[i].p[ix])/v1.p[ix]);			// �V�����_�Ɉړ�
			for(j=0; j<2; j++) {
				pa[i].p[j] = pa[i+4].p[j]+v1.p[j]*ln;
			}
			ln = fabs((pa[i+4].p[ix]-pa[i+8].p[ix])/v1.p[ix]);
			for(j=0; j<2; j++) {
				pa[i+8].p[j] = pa[i+4].p[j]+v1.p[j]*ln;
			}
		}
		VecV(&pa[5], &pa[4], &v1);										// �אڂ���2�_�ւ�
		VecN(&v1);														// �P���޸�ق��Z�o
		VecV(&pa[5], &pa[6], &v2);
		VecN(&v2);
		for(j=0; j<DIM; j++) {
			v1.p[j] += v2.p[j];											// �޸�ق̘a
		}
		VecN(&v1);														// �P���޸�ى�
		ln = fabs(pa[5].p[0]/v1.p[0]);									// �V�����_�Ɉړ�
		for(j=0; j<2; j++) {
			pa[12].p[j] = pa[5].p[j]+v1.p[j]*ln;
		}
		VecV(&pa[6], &pa[5], &v1);										// �אڂ���2�_�ւ�
		VecN(&v1);														// �P���޸�ق��Z�o
		VecV(&pa[6], &pa[7], &v2);
		VecN(&v2);
		for(j=0; j<DIM; j++) {
			v1.p[j] += v2.p[j];											// �޸�ق̘a
		}
		VecN(&v1);														// �P���޸�ى�
		ln = fabs(pa[6].p[0]/v1.p[0]);									// �V�����_�Ɉړ�
		for(j=0; j<2; j++) {
			pa[13].p[j] = pa[6].p[j]+v1.p[j]*ln;
		}
	}
	if(sw) {															// <XY�����ւ����ꍇ>
		for(i=0; i<vn; i++) {
			dd = pa[i].p[0], pa[i].p[0] = pa[i].p[1], pa[i].p[1] = -dd;	// 90�x��]
		}
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/***********************************************************************/
static void SetRoseVtx(PNTTYPE* pa, double xd, double hd, PNTTYPE cent) //<<< ۰�ޒ��_�ݒ�
/***********************************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, z0, z1, z2;

	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		z0 = xd*0.2, z1 = xd*0.12, z2 = 0.0;
	} else {															// �����w��L�̏ꍇ
		z0 = hd, z1 = hd*0.12/0.2, z2 = 0.0;
	}
	d2 = xd/2.0;
	d1 = d2*0.76;
	pa[vn].p[0] = 0.0, pa[vn].p[1] = 0.0, pa[vn++].p[2] = z0;			// ���_�̐ݒ�
	dth = PI/3.0;
	for(th=0, i=0; i<6; th+=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
	}
	dth = PI/6;
	for(th=0, i=0; i<12; th+=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/***************************************************/
static void SetCabochonVtx(PNTTYPE* pa, double xd,
                           double hd, PNTTYPE cent)                     //<<< ��޼�ݒ��_�ݒ�
/***************************************************/
{
	int    i, j, vn=0;
	double dth, th, sp, cp, d1, d2, d3, d4, d5, d6, z0, z1, z2, z3, z4, z5, z6;

	if(fabs(hd)<EPSILON) {												// �����w�薳�̏ꍇ
		z0 = xd*0.3, z1 = xd*0.29, z2 = xd*0.26, z3 = xd*0.21;
		z4 = xd*0.14, z5 = xd*0.07, z6 = 0.0;
	} else {															// �����w��L�̏ꍇ
		z0 = hd, z1 = hd*0.29/0.3, z2 = hd*0.26/0.3, z3 = hd*0.21/0.3;
		z4 = hd*0.14/0.3, z5 = hd*0.07/0.3, z6 = 0.0;
	}
	d1 = xd*0.155, d2 = xd*0.266, d3 = xd*0.357;
	d4 = xd*0.431, d5 = xd*0.474, d6 = xd*0.5;
	pa[vn].p[0] = 0.0, pa[vn].p[1] = 0.0, pa[vn++].p[2] = z0;			// ���_�̐ݒ�
	dth = PI/12.0;
	for(th=0, i=0; i<24; th+=dth, i++) {
		sp = sin(th), cp = cos(th);
		pa[vn].p[0] = d1*cp, pa[vn].p[1] = d1*sp, pa[vn++].p[2] = z1;
		pa[vn].p[0] = d2*cp, pa[vn].p[1] = d2*sp, pa[vn++].p[2] = z2;
		pa[vn].p[0] = d3*cp, pa[vn].p[1] = d3*sp, pa[vn++].p[2] = z3;
		pa[vn].p[0] = d4*cp, pa[vn].p[1] = d4*sp, pa[vn++].p[2] = z4;
		pa[vn].p[0] = d5*cp, pa[vn].p[1] = d5*sp, pa[vn++].p[2] = z5;
		pa[vn].p[0] = d6*cp, pa[vn].p[1] = d6*sp, pa[vn++].p[2] = z6;
	}
	for(i=0; i<vn; i++) {
		for(j=0; j<DIM; j++) {
			pa[i].p[j] += cent.p[j];									// ��ʾ����ړ�
		}
	}
}

/*************************************************************/
BOOL CJcad3GlbView::ReadGem(int no, PNTTYPE* cpt, double* dh)           //<<< ��ζ�ē���
/*************************************************************/
{
	PNTTYPE* pa = new PNTTYPE[150];
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  cent, cp;
	CString  fname;
	BOOL     bOK=TRUE;
	int      i, j, vn;
	double   mat[3][3], xd, yd, hd=0.0, dd, wx, wy;

	xd = GetDispHeight()/2.0, yd = GetDispHeight()/2.0;					// ��ʂ̒����̔����擾
	if(GetNumInp()!=0) {												// <���l���͂���>
		if(no==0||no==5||no==9) {
			if((bOK=InputGemSize(&m_GemRndSize,
                                 &m_GemRndHeigt))==TRUE) {				// �΂̒��a,��������
				xd = m_GemRndSize, hd = m_GemRndHeigt;
			}
		} else {
			if((bOK=InputGemSize(&m_GemSqrSize1, &m_GemSqrSize2,
                                 &m_GemSqrHeigt))==TRUE) {				// �΂�X,Y,��������
				xd = m_GemSqrSize1, yd = m_GemSqrSize2, hd = m_GemSqrHeigt;
			}
		}
	}
	if(bOK) {															// <�w�跬ݾوȊO>
		if(no==0) {														// ̧�ٖ�(׳������ر��)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==1) {												//       (��������ر��)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==2) {												//       (ϰ�������ر��)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==3) {												//       (�߱��������ر��)
			fname = GetAppPath(0)+BRI_NAME;
		} else if(no==4) {												//       (ʰļ�������ر��)
			fname = GetAppPath(0)+HEA_NAME;
		} else if(no==5) {												//       (�ݸ��)
			fname = GetAppPath(0)+SNG_NAME;
		} else if(no==6) {												//       (������)
			fname = GetAppPath(0)+EMR_NAME;
		} else if(no==7) {												//       (�޹ޯ�)
			fname = GetAppPath(0)+BGE_NAME;
		} else if(no==8) {												//       (ð�߰���޹ޯ�)
			fname = GetAppPath(0)+BGE_NAME;
		} else if(no==9) {												//       (۰��)
			fname = GetAppPath(0)+ROS_NAME;
		} else {														//       (��޼��)
			fname = GetAppPath(0)+CAB_NAME;
		}
		InitLevel();													// Undo/Redo������
		if(ReadJsdFile(fname, 0, 0)) {									// ��ζ�ē���
			AfxMessageBox(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
			BackLevel();												// Undo/Redo�߂�
			delete[] pa;
			return FALSE;												// ���͕s�\�͒��~
		}
		OffEdtAllObject();												// �ҏW���̑S����
		GetObjPtr(&op);													// ���̂��߲�Ď擾
		GetObjOif(op, &oif);											// ���̍\������߲���擾
		if(no==0) {
			SetOifName(1, oif, OBJ_NAME19);								// �\����(׳������ر��)
		} else if(no==1) {
			SetOifName(1, oif, OBJ_NAME20);								//       (��������ر��)
		} else if(no==2) {
			SetOifName(1, oif, OBJ_NAME21);								//       (ϰ�������ر��)
		} else if(no==3) {
			SetOifName(1, oif, OBJ_NAME22);								//       (�߱��������ر��)
		} else if(no==4) {
			SetOifName(1, oif, OBJ_NAME23);								//       (ʰļ�������ر��)
		} else if(no==5) {
			SetOifName(1, oif, OBJ_NAME24);								//       (�ݸ��)
		} else if(no==6) {
			SetOifName(1, oif, OBJ_NAME25);								//       (������)
		} else if(no==7) {
			SetOifName(1, oif, OBJ_NAME26);								//       (�޹ޯ�)
		} else if(no==8) {
			SetOifName(1, oif, OBJ_NAME27);								//       (ð�߰���޹ޯ�)
		} else if(no==9) {
			SetOifName(1, oif, OBJ_NAME28);								//       (۰��)
		} else {
			SetOifName(1, oif, OBJ_NAME29);								//       (��޼��)
		}
		OnEdtFlg(1, op);												// �ҏW���̐ݒ�
		GetCentPnt(&cent);												// ��ʂ̒��S�擾
		if(no==0) {														// <׳���>
			SetBrilliantVtx(pa, xd, hd, cent);							// ���ر�Ē��_�ݒ�
			vn = 41;													// ���_��
		} else if(no==1) {												// <�����>
			cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
			if(xd>yd) {													// X���̕��������ꍇ��
				dd = xd, xd = yd, yd = dd;								// X��Y�����
				wx = yd/xd, wy = 1.0;									// X��Y�̔䗦
			} else {
				wx = 1.0, wy = yd/xd;									// X��Y�̔䗦
			}
			SetBrilliantVtx(pa, xd, hd, cp);							// ���ر�Ē��_�ݒ�
			vn = 41;													// ���_��
			mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
			mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
			mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
			AffineVtx(vn, pa, cp, mat);									// ���_�̨̱ݕϊ�
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pa[i].p[j] += cent.p[j];							// ��ʾ����ړ�
				}
			}
		} else if(no==2) {												// <ϰ����>
			SetMarquiseVtx(pa, xd, yd, hd, cent);						// ϰ���ޒ��_�ݒ�
			vn = 41;													// ���_��
		} else if(no==3) {												// <�߱�����>
			SetPearShapeVtx(pa, xd, yd, hd, cent);						// �߱����ߒ��_�ݒ�
			vn = 41;													// ���_��
		} else if(no==4) {												// <ʰļ����>
			for(i=0; i<41; i++) {
				GetVtx(op, i, &pa[i]);									// ���_�擾
			}
			SetHeartShapeVtx(pa, xd, yd, hd, cent);						// ʰļ���ߒ��_�ݒ�
			vn = 41;													// ���_��
		} else if(no==5) {												// <�ݸ��>
			SetSingleVtx(pa, xd, hd, cent);								// �ݸ�ْ��_�ݒ�
			vn = 17;													// ���_��
		} else if(no==6) {												// <������>
			SetEmeraldVtx(pa, xd, yd, hd, cent);						// �����ޒ��_�ݒ�
			vn = 50;													// ���_��
		} else if(no==7) {												// <�޹ޯ�>
			SetBaguetteVtx(pa, xd, yd, hd, cent);						// �޹ޯĒ��_�ݒ�
			vn = 14;													// ���_��
		} else if(no==8) {												// <ð�߰�ޥ�޹ޯ�>
			SetTaperedVtx(pa, xd, yd, hd, 0.0, 0.0, 0, cent);			// ð�߰�ޥ�޹ޯĒ��_�ݒ�
			vn = 14;													// ���_��
		} else if(no==9) {												// <۰��>
			SetRoseVtx(pa, xd, hd, cent);								// ۰�ޒ��_�ݒ�
			vn = 19;													// ���_��
		} else {														// <��޼��>
			cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
			if(xd>yd) {													// X���̕��������ꍇ��
				dd = xd, xd = yd, yd = dd;								// X��Y�����
				wx = yd/xd, wy = 1.0;									// X��Y�̔䗦
			} else {
				wx = 1.0, wy = yd/xd;									// X��Y�̔䗦
			}
			SetCabochonVtx(pa, xd, hd, cp);								// ��޼�ݒ��_�ݒ�
			vn = 145;													// ���_��
			mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
			mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
			mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
			AffineVtx(vn, pa, cp, mat);									// ���_�̨̱ݕϊ�
			for(i=0; i<vn; i++) {
				for(j=0; j<DIM; j++) {
					pa[i].p[j] += cent.p[j];							// ��ʾ����ړ�
				}
			}
		}
		RedoDirect(vn, pa);												// ���ڐ�������ލĎ��s
		*cpt = cent;													// ��ʾ������W
		*dh  = hd;														// �w�荂��
	}
	delete[] pa;
	return bOK;
}

/***************************************************************/
static void TrackerGem1(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa)         //<<< ػ��ޗp�ׯ���擾
/***************************************************************/       //<<< ׳��ތn��ζ��
{
	pa[0].p[0] = box.bp[0].p[0];										// �ׯ�����W�ݒ�
	pa[0].p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
	pa[0].p[2] = cent.p[2];
	pa[1].p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
	pa[1].p[1] = box.bp[0].p[1];
	pa[1].p[2] = cent.p[2];
	pa[2].p[0] = box.bp[1].p[0];
	pa[2].p[1] = (box.bp[0].p[1]+box.bp[1].p[1])/2.0;
	pa[2].p[2] = cent.p[2];
	pa[3].p[0] = (box.bp[0].p[0]+box.bp[1].p[0])/2.0;
	pa[3].p[1] = box.bp[1].p[1];
	pa[3].p[2] = cent.p[2];
}

/***************************************************************/
static void TrackerGem2(BOXTYPE box, PNTTYPE cent, PNTTYPE* pa)         //<<< ػ��ޗp�ׯ���擾
/***************************************************************/       //<<< ڸČn��ζ��
{
	pa[0] = box.bp[0], pa[0].p[2] = cent.p[2];							// �ׯ�����W�ݒ�
	pa[1].p[0] = box.bp[1].p[0];
	pa[1].p[1] = box.bp[0].p[1];
	pa[1].p[2] = cent.p[2];
	pa[2] = box.bp[1], pa[2].p[2] = cent.p[2];
	pa[3].p[0] = box.bp[0].p[0];
	pa[3].p[1] = box.bp[1].p[1];
	pa[3].p[2] = cent.p[2];
}

/**********************************************************/
static void TrackerGem3(BOXTYPE box, PNTTYPE cent,
                        double xh, double yh, PNTTYPE* pa)              //<<< ػ��ޗp�ׯ���擾
/**********************************************************/            //<<< ð�߰�ޥ�޹ޯ�
{
	int    i;
	double wx, wy;

	pa[0] = box.bp[0];													// �ׯ�����W�ݒ�
	pa[1].p[0] = box.bp[1].p[0], pa[1].p[1] = box.bp[0].p[1];
	pa[2] = box.bp[1];
	pa[3].p[0] = box.bp[0].p[0], pa[3].p[1] = box.bp[1].p[1];
	for(i=0; i<4; i++) {
		pa[i].p[2] = cent.p[2];
	}
	wx = box.bp[1].p[0]-cent.p[0], wy = box.bp[1].p[1]-cent.p[1];
	xh = (fabs(xh)<EPSILON) ? wx/2.0 : xh;
	yh = (fabs(yh)<EPSILON) ? wy/2.0 : yh;
	for(i=0; i<4; i++) {
		pa[2*i+4] = pa[i], pa[2*i+5] = pa[i];
	}
	pa[4].p[1] += yh, pa[5].p[0] += xh;
	pa[6].p[0] -= xh, pa[7].p[1] += yh;
	pa[8].p[1] -= yh, pa[9].p[0] -= xh;
	pa[10].p[0] += xh, pa[11].p[1] -= yh;
}

/**************************************************************/
static void TrackerGemH(BOXTYPE box, PNTTYPE gct, PNTTYPE* pa)          //<<< �����p�ׯ���擾
/**************************************************************/
{
	int    gno;
	double z, hd;

	pa[0] = gct;
	z = box.bp[1].p[2]-box.bp[0].p[2];									// ��ζ�Ă̍���
	gno = View->GetGemNo();												// ��ζ��No�擾
	if(gno==5) {														// �ݸ�٥���
		hd = z*0.146/0.577;
	} else if(gno==6) {													// �����ޥ���
		hd = z*0.447/1.787;
	} else if(gno==7) {													// �޹ޯĥ���
		hd = z*0.45/1.65;
	} else if(gno==8) {													// ð�߰�ޥ�޹ޯĥ���
		hd = z*0.45/1.65;
	} else if(gno==9) {													// ۰�ޥ���
		hd = z;
	} else if(gno==10) {												// ��޼�ݥ���
		hd = z;
	} else {															// ���ر�ĥ���
		hd = z*0.146/0.577;
	}
	pa[0].p[2] += hd;													// ���S��Z�����Z
}

/********************************/
static double CalcGemH(double z)                                        //<<< ��ζ�č����Z�o
/********************************/
{
	int    gno;
	double hd;

	gno = View->GetGemNo();												// ��ζ��No�擾
	if(gno==5) {														// �ݸ�٥���
		hd = z*0.577/0.146;
	} else if(gno==6) {													// �����ޥ���
		hd = z*1.787/0.447;
	} else if(gno==7) {													// �޹ޯĥ���
		hd = z*1.65/0.45;
	} else if(gno==8) {													// ð�߰�ޥ�޹ޯĥ���
		hd = z*1.65/0.45;
	} else if(gno==9) {													// ۰�ޥ���
		hd = z;
	} else if(gno==10) {												// ��޼�ݥ���
		hd = z;
	} else {															// ���ر�ĥ���
		hd = z*0.577/0.146;
	}
	return hd;
}

/*****************************************************************/
void CJcad3GlbView::MouseGemSquare(UINT flg, const CPoint& point,
                                   int plane, int ms)                   //<<< ��ζ��1
/*****************************************************************/     // ׳���,�ݸ��,۰��
{
	PNTTYPE pnt;
	BOXTYPE box;
	int     ht, i, esw=0, col=0;
	double  dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// �����}�̏ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// ϳ��ړ�
		if(flg&MK_LBUTTON) {											// ϳ��̍��t�������̂ݗL��
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(Sphase!=0) {												// �����ȊO�͓�������
				if(Sphase==1) {											// ���a�ύX�̏ꍇ
					if(Si3==-1) {
						esw = (pnt.p[0]>=Sbase.p[0]) ? 1 : esw;
					} else if(Si3==1) {
						esw = (pnt.p[0]<=Sbase.p[0]) ? 1 : esw;
					}
					if(Si4==-1)	{
						esw = (pnt.p[1]>=Sbase.p[1]) ? 1 : esw;
					} else if(Si4==1) {
						esw = (pnt.p[1]<=Sbase.p[1]) ? 1 : esw;
					}
				} else {												// �����ύX�̏ꍇ
					esw = (pnt.p[2]<=Spnt1.p[2]) ? 1 : esw;
				}
				if(esw==0) {											// <<<�װ���̏ꍇ>>>
					if(Sphase==1) {
						Sd1 = fabs(pnt.p[Si2]-Sbase.p[Si2])*2.0;		// ���a�ύX
						dd = 0.0;										// �������w��
					} else {
						dd = fabs(pnt.p[2]-Spnt1.p[2]);					// ��ʒ��S����㕔��
						dd = CalcGemH(dd);								// ��ζ�Ă̍���
					}
					if(m_GemNo==0) {									// <׳���>
						SetBrilliantVtx(Spd, Sd1, dd, Spnt1);			// ���ر�Ē��_�ݒ�
						i = 41;											// ���_��
					} else if(m_GemNo==5) {								// <�ݸ��>
						SetSingleVtx(Spd, Sd1, dd, Spnt1);				// �ݸ�ْ��_�ݒ�
						i = 17;											// ���_��
					} else {											// <۰��>
						SetRoseVtx(Spd, Sd1, dd, Spnt1);				// ۰�ޒ��_�ݒ�
						i = 19;											// ���_��
					}
					RedoDirect(i, Spd);									// ���ڐ�������ލĎ��s
					GetAreaBox(2, &box);								// �ޯ�����ގ擾
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1],
                                         box.bp[1].p[2]-box.bp[0].p[2]);
					Display(0);											// �ĕ`��
				}
			}
		}
	} else if(ms==1) {													// ϳ����t����
		if(Sphase==0) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spa, 4))!=-1) {		// �ׯ��1����
				Si2 = ht%2;
				Si3 = (fabs(Spa[ht].p[0]-Sbase.p[0])<EPSILON) ? 0		// �(���S)�_��
                                    : ((Spa[ht].p[0]<Sbase.p[0])?-1:1);	// �X�V�_�ʒu�֌W
				Si4 = (fabs(Spa[ht].p[1]-Sbase.p[1])<EPSILON) ? 0
                                    : ((Spa[ht].p[1]<Sbase.p[1])?-1:1);
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 1;
			} else if(Tracker2.HitTracker(1, 1, pnt, Spb, 1)!=-1) {		// �ׯ��2����
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// ϳ����t���
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		TrackerGem1(box, Sbase, Spa);									// �ׯ��1�擾
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// �ׯ��1�ݒ�
		TrackerGemH(box, Spnt1, Spb);									// �ׯ��2�擾
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// �ׯ��2�ݒ�
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==0) {													// ��������
		StartMenu();													// �ƭ��J�n����
		CheckCtrlKey1();												// ���䷰�m�F(�N����)
		if(!ReadGem(m_GemNo, &Spnt1, &dd)) {							// ��ζ�ē���
			ExitMenu(0, 1);												// �ƭ��I��
		} else {
			SetNumDisp(2);												// ���l�\����������
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			if(m_NumInp!=0) {											// ���l���͂̏ꍇ
				ExitMenu(0, 1);											// �ƭ��I��
				EndJMenu1();											// �ƭ��I������
			} else {													// ���l���͖��̏ꍇ
				GetAreaBox(2, &box);									// �I�𗧑̂��ޯ�����ގ擾
				BoxCenter(box, &Sbase);									// �ޯ�������擾
				Sd1 = box.bp[1].p[0]-box.bp[0].p[0];					// ��ζ�Ă̒��a
				dd = box.bp[1].p[2]-box.bp[0].p[2];
				m_pNumDisp->SetParam(Sd1, Sd1, dd);
				TrackerGem1(box, Sbase, Spa);							// �ׯ��1�擾
				Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;			// �ׯ��1�F�ݒ�
				Tens[0] = 5, Tens[1] = 6, Tens[2] = 5, Tens[3] = 6;		// �ׯ��1��߼�ݐݒ�
				Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);				// �ׯ��1�ݒ�
				TrackerGemH(box, Spnt1, Spb);							// �ׯ��2�擾
				Sib[0] = 0, TensC[0] = 6;								// �ׯ��2�F/��߼�ݐݒ�
				Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);				// �ׯ��2�\��
			}
		}
		Display(0);														// �ĕ`��
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// ϳ��E�t����/�I��
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/****************************************************************/      //<<< ��ζ��2
void CJcad3GlbView::MouseGemRect(UINT flg, const CPoint& point,         // �����,ϰ����,�߱�����,
                                 int plane, int ms)                     // ʰļ����,������,�޹ޯ�,
/****************************************************************/      // ��޼��
{
	OBJTYPE* op;
	PNTTYPE  pnt, cp;
	BOXTYPE  box;
	int      ht, esw=0, col=0, smd=0, i, j, k;
	double   mat[3][3], d1, d2, wx, wy;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// �����}�̏ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// ϳ��ړ�
		if(flg&MK_LBUTTON) {											// ϳ��̍��t�������̂ݗL��
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(Sphase!=0) {												// �����ȊO�͓�������
				if(Sphase==1) {											// ���a�ύX�̏ꍇ
					if(flg&m_LimKey) {
						smd = LimitPoint(plane, &pnt, &Spnt2);			// ϳ����͐������W
					}
					if(Si3==-1) {
						esw = (pnt.p[0]>=Sbase.p[0]) ? 1 : esw;
					} else if(Si3==1) {
						esw = (pnt.p[0]<=Sbase.p[0]) ? 1 : esw;
					}
					if(Si4==-1) {
						esw = (pnt.p[1]>=Sbase.p[1]) ? 1 : esw;
					} else if(Si4==1) {
						esw = (pnt.p[1]<=Sbase.p[1]) ? 1 : esw;
					}
				} else {												// �����ύX�̏ꍇ
					esw = (pnt.p[2]<=Spnt1.p[2]) ? 1 : esw;
				}
				if(esw==0) {											// <<<�װ���̏ꍇ>>>
					if(Sphase==1) {
						Sd1 = fabs(pnt.p[0]-Sbase.p[0])*2.0;			// X���ޕύX
						Sd2 = fabs(pnt.p[1]-Sbase.p[1])*2.0;			// Y���ޕύX
						if(smd==1) {									// Ӱ��:2D�ϓ�
							if(Sd1<Sd2) {								// ���������̔{���̗p
								Sd2 = Sd1;
							} else {
								Sd1 = Sd2;
							}
						}
						d2 = 0.0;										// �������w��
					} else {
						d2 = fabs(pnt.p[2]-Spnt1.p[2]);					// ��ʒ��S����㕔��
						d2 = CalcGemH(d2);								// ��ζ�Ă̍���
					}
					if(m_GemNo==1) {									// <�����>
						cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
						if(Sd1>Sd2) {									// X���̕��������ꍇ��
							d1 = Sd2;									// Y���̗p
							wx = Sd1/Sd2, wy = 1.0;						// X��Y�̔䗦
						} else {
							d1 = Sd1;									// X���̗p
							wx = 1.0, wy = Sd2/Sd1;						// X��Y�̔䗦
						}
						SetBrilliantVtx(Spd, d1, d2, cp);				// ���ر�Ē��_�ݒ�
						k = 41;											// ���_��
						mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
						mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
						mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
						AffineVtx(k, Spd, cp, mat);						// ���_�̨̱ݕϊ�
						for(i=0; i<k; i++) {
							for(j=0; j<DIM; j++) {
								Spd[i].p[j] += Spnt1.p[j];				// ��ʾ����ړ�
							}
						}
					} else if(m_GemNo==2) {								// <ϰ����>
						SetMarquiseVtx(Spd, Sd1, Sd2, d2, Spnt1);		// ϰ���ޒ��_�ݒ�
						k = 41;											// ���_��
					} else if(m_GemNo==3) {								// <�߱�����>
						SetPearShapeVtx(Spd, Sd1, Sd2, d2, Spnt1);		// �߱����ߒ��_�ݒ�
						k = 41;											// ���_��
					} else if(m_GemNo==4) {								// <ʰļ����>
						for(i=0; i<41; i++) {
							Spd[i] = Spc[i];
						}
						SetHeartShapeVtx(Spd, Sd1, Sd2, d2, Spnt1);		// ʰļ���ߒ��_�ݒ�
						k = 41;											// ���_��
					} else if(m_GemNo==6) {								// <������>
						SetEmeraldVtx(Spd, Sd1, Sd2, d2, Spnt1);		// �����ޒ��_�ݒ�
						k = 50;											// ���_��
					} else if(m_GemNo==7) {								// <�޹ޯ�>
						SetBaguetteVtx(Spd, Sd1, Sd2, d2, Spnt1);		// �޹ޯĒ��_�ݒ�
						k = 14;											// ���_��
					} else {											// <��޼��>
						cp.p[0] = 0.0, cp.p[1] = 0.0, cp.p[2] = 0.0;
						if(Sd1>Sd2) {									// X���̕��������ꍇ��
							d1 = Sd2;									// Y���̗p
							wx = Sd1/Sd2, wy = 1.0;						// X��Y�̔䗦
							
						} else {
							d1 = Sd1;									// X���̗p
							wx = 1.0, wy = Sd2/Sd1;						// X��Y�̔䗦
						}
						SetCabochonVtx(Spd, d1, d2, cp);				// ��޼�ݒ��_�ݒ�
						k = 145;										// ���_��
						mat[0][0] = wx,  mat[0][1] = 0.0, mat[0][2] = 0.0;
						mat[1][0] = 0.0, mat[1][1] = wy,  mat[1][2] = 0.0;
						mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = 1.0;
						AffineVtx(k, Spd, cp, mat);						// ���_�̨̱ݕϊ�
						for(i=0; i<k; i++) {
							for(j=0; j<DIM; j++) {
								Spd[i].p[j] += Spnt1.p[j];				// ��ʾ����ړ�
							}
						}
					}
					RedoDirect(k, Spd);									// ���ڐ�������ލĎ��s
					GetAreaBox(2, &box);								// �ޯ�����ގ擾
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1],
                                         box.bp[1].p[2]-box.bp[0].p[2]);
					Display(0);											// �ĕ`��
				}
			}
		}
	} else if(ms==1) {													// ϳ����t����
		if(Sphase==0) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spa, 4))!=-1) {		// �ׯ��1����
				Spnt2 = Spa[ht];										// ػ��ލX�V�_�ݒ�
				Si3 = (Spnt2.p[0]<Sbase.p[0]) ? -1						// �(���S)�_��
                                       : ((Spnt2.p[0]>Sbase.p[0])?1:0);	// �X�V�_�ʒu�֌W
				Si4 = (Spnt2.p[1]<Sbase.p[1]) ? -1
                                       : ((Spnt2.p[1]>Sbase.p[1])?1:0);
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 1;
			} else if(Tracker2.HitTracker(1, 1, pnt, Spb, 1)!=-1) {		// �ׯ��2����
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// ϳ����t���
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		TrackerGem2(box, Sbase, Spa);									// �ׯ��1�擾
		Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);						// �ׯ��1�ݒ�
		TrackerGemH(box, Spnt1, Spb);									// �ׯ��2�擾
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// �ׯ��2�ݒ�
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==0) {													// ��������
		StartMenu();													// �ƭ��J�n����
		CheckCtrlKey1();												// ���䷰�m�F(�N����)
		if(!ReadGem(m_GemNo, &Spnt1, &d1)) {							// ��ζ�ē���
			ExitMenu(0, 1);												// �ƭ��I��
		} else {
			SetNumDisp(2);												// ���l�\����������
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			if(m_NumInp!=0) {											// ���l���͂̏ꍇ
				ExitMenu(0, 1);											// �ƭ��I��
				EndJMenu1();											// �ƭ��I������
			} else {													// ���l���͖��̏ꍇ
				GetAreaBox(2, &box);									// �I�𗧑̂��ޯ�����ގ擾
				BoxCenter(box, &Sbase);									// �ޯ�������擾
				Sd1 = box.bp[1].p[0]-box.bp[0].p[0];					// ��ζ�Ă�X����
				Sd2 = box.bp[1].p[1]-box.bp[0].p[1];					// ��ζ�Ă�Y����
				d1 = box.bp[1].p[2]-box.bp[0].p[2];
				m_pNumDisp->SetParam(Sd1, Sd2, d1);
				TrackerGem2(box, Sbase, Spa);							// �ׯ��1�擾
				Sia[0] = 0, Sia[1] = 0, Sia[2] = 0, Sia[3] = 0;			// �ׯ��1�F�ݒ�
				Tens[0] = 7, Tens[1] = 7, Tens[2] = 7, Tens[3] = 7;		// �ׯ��1��߼�ݐݒ�
				Tracker1.Setup(Spa, Sia, Tens, 4, 0, 7);				// �ׯ��1�ݒ�
				TrackerGemH(box, Spnt1, Spb);							// �ׯ��2�擾
				Sib[0] = 0, TensC[0] = 6;								// �ׯ��2�F/��߼�ݐݒ�
				Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);				// �ׯ��2�\��
				if(m_GemNo==4) {										// <ʰļ����>
					GetObjPtr(&op);										// �����߲�Ď擾
					for(i=0; i<41; i++) {
						GetVtx(op, i, &Spc[i]);							// ���_�ۑ�
						for(j=0; j<DIM; j++) {
							Spc[i].p[j] -= Spnt1.p[j];
						}
					}
				}
			}
		}
		Display(0);														// �ĕ`��
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// ϳ��E�t����/�I��
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}

/****************************************************************/
void CJcad3GlbView::MouseGemRect2(UINT flg, const CPoint& point,
                                  int plane, int ms)                    //<<< ��ζ��3
/****************************************************************/      // ð�߰���޹ޯ�
{
	PNTTYPE pnt, cpt, mip, map;
	BOXTYPE box;
	int     ht, esw=0, col=0, smd=0, i, i1, i2;
	double  dd;

	if(ms!=4&&plane!=XY&&plane!=YZ&&plane!=ZX) {						// �����}�̏ꍇ
		EyeChange(flg, point, plane, ms);								// ���_�ύX
		return;
	}
	if(ms==2) {															// ϳ��ړ�
		if(flg&MK_LBUTTON) {											// ϳ��̍��t�������̂ݗL��
			CalcScreenWorld(0, &pnt, point, plane);						// ���W�_�擾
			if(Sphase!=0) {												// �����ȊO�͓�������
				if(Sphase==1) {											// ���a�ύX�̏ꍇ
					if(flg&m_LimKey) {
						smd = LimitPoint(plane, &pnt, &Spnt2);			// ϳ����͐������W
					}
					if(!Si6) {											// <���ޕύX>
						if(Si7==-1) {
							esw = (pnt.p[0]>=Sbase.p[0]) ? 1 : esw;
						} else if(Si7==1) {
							esw = (pnt.p[0]<=Sbase.p[0]) ? 1 : esw;
						}
						if(Si8==-1) {
							esw = (pnt.p[1]>=Sbase.p[1]) ? 1 : esw;
						} else if(Si8==1) {
							esw = (pnt.p[1]<=Sbase.p[1]) ? 1 : esw;
						}
					} else {											// <�l���ύX>
						if(!Si8) {										// �ύX�\�͈͎擾
							if(Si7==0||Si7==1) {
								i1 = 0, i2 = 1;
							} else {
								i1 = 3, i2 = 2;
							}
						} else {
							if(Si7==0||Si7==3) {
								i1 = 0, i2 = 3;
							} else {
								i1 = 1, i2 = 2;
							}
						}
						for(i=0; i<DIM; i++) {
							cpt.p[i] = (Spa[i1].p[i]+Spa[i2].p[i])/2.0;	// ���ԓ_�擾
						}
						if(!Si8) {
							if(Si7==0||Si7==3) {
								mip = Spa[Si7], map = cpt;
							} else {
								mip = cpt, map = Spa[Si7];
							}
						} else {
							if(Si7==0||Si7==1) {
								mip = Spa[Si7], map = cpt;
							} else {
								mip = cpt, map = Spa[Si7];
							}
						}
						esw = (pnt.p[Si8]<=mip.p[Si8]) ? 1 : esw;
						esw = (pnt.p[Si8]>=map.p[Si8]) ? 1 : esw;
					}
				} else {												// �����ύX�̏ꍇ
					esw = (pnt.p[2]<=Spnt1.p[2]) ? 1 : esw;
				}
				if(esw==0) {											// <<<�װ���̏ꍇ>>>
					if(Sphase==1) {
						if(!Si6) {										// �ύX��̻���
							Sd1 = fabs(pnt.p[0]-Sbase.p[0])*2.0;		// X���ޕύX
							Sd2 = fabs(pnt.p[1]-Sbase.p[1])*2.0;		// Y���ޕύX
							if(smd==1) {								// Ӱ��:2D�ϓ�
								if(Sd1<Sd2) {							// ���������̔{���̗p
									Sd2 = Sd1;
								} else {
									Sd1 = Sd2;
								}
							}
							Sd3 = 0.0, Sd4 = 0.0, Sd5 = 0.0, Si9 = 0;	// ����/�l�����ر
						} else {										// �l���̕ύX
							if(!Si8) {
								Sd4 = fabs(pnt.p[0]-Spa[Si7].p[0]);
								Sd5 = 0.0;
								Si9 = (Si7==0||Si7==1) ? 0 : 1;
							} else {
								Sd4 = 0.0;
								Sd5 = fabs(pnt.p[1]-Spa[Si7].p[1]);
								Si9 = (Si7==0||Si7==3) ? 0 : 1;
							}
						}
					} else {
						Sd3 = fabs(pnt.p[2]-Spnt1.p[2]);				// ��ʒ��S����㕔��
						Sd3 = CalcGemH(Sd3);							// ��ζ�Ă̍���
					}
					SetTaperedVtx(Spd, Sd1, Sd2, Sd3, Sd4, Sd5,
                                  Si9, Spnt1);							// ð�߰�ޥ�޹ޯĒ��_�ݒ�
					RedoDirect(14, Spd);								// ���ڐ�������ލĎ��s
					GetAreaBox(2, &box);								// �ޯ�����ގ擾
					m_pNumDisp->SetParam(box.bp[1].p[0]-box.bp[0].p[0],
                                         box.bp[1].p[1]-box.bp[0].p[1],
                                         box.bp[1].p[2]-box.bp[0].p[2]);
					Display(0);											// �ĕ`��
				}
			}
		}
	} else if(ms==1) {													// ϳ����t����
		if(Sphase==0) {													// <��_�擾>
			CalcScreenWorld(1, &pnt, point, plane);						// ���W�_�擾
			if((ht=Tracker1.HitTracker(1, 0, pnt, Spa, 12))!=-1) {		// �ׯ��1����
				Spnt2 = Spa[ht];										// ػ��ލX�V�_�ݒ�
				if(ht<4) {												// <<���ޕύX�̏ꍇ>>
					Si6 = 0;											// ���ޕύXӰ��
					Si7 = (Spnt2.p[0]<Sbase.p[0]) ? -1					// �(���S)�_��
                                       : ((Spnt2.p[0]>Sbase.p[0])?1:0);	// �X�V�_�ʒu�֌W
					Si8 = (Spnt2.p[1]<Sbase.p[1]) ? -1
                                       : ((Spnt2.p[1]>Sbase.p[1])?1:0);
				} else {												// <<�l���ύX�̏ꍇ>>
					Si6 = 1;											// �l���ύXӰ��
					Si7 = (ht-4)/2;										// �ҏW���ۑ�
					Si8 = (ht==5||ht==6||ht==9||ht==10) ? 0 : 1;
				}
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 1;
			} else if(Tracker2.HitTracker(1, 1, pnt, Spb, 1)!=-1) {		// �ׯ��2����
				Tracker1.Setup(&Sbase, &col, NULL, 1, 0, 2);			// �ׯ��1�ݒ�
				Tracker2.Reset(0);										// �ׯ��2����
				Display(0);												// �ĕ`��
				Sphase = 2;
			}
		}
	} else if(ms==3) {													// ϳ����t���
		GetAreaBox(2, &box);											// �ޯ�����ގ擾
		TrackerGem3(box, Sbase, Sd4, Sd5, Spa);							// �ׯ��1�擾
		Tracker1.Setup(Spa, Sia, Tens, 12, 0, 7);						// �ׯ��1�ݒ�
		TrackerGemH(box, Spnt1, Spb);									// �ׯ��2�擾
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// �ׯ��2�ݒ�
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==11) {													// <<<DEL>>>
		Sd4 = 0.0, Sd5 = 0.0, Si9 = 0;
		SetTaperedVtx(Spd, Sd1, Sd2, Sd3, Sd4, Sd5, Si9, Spnt1);		// ð�߰�ޥ�޹ޯĒ��_�ݒ�
		RedoDirect(14, Spd);											// ���ڐ�������ލĎ��s
		GetAreaBox(2, &box);											// �I�𗧑̂��ޯ�����ގ擾
		TrackerGem3(box, Sbase, Sd4, Sd5, Spa);							// �ׯ��1�擾
		Tracker1.Setup(Spa, Sia, Tens, 12, 0, 7);						// �ׯ��1�ݒ�
		TrackerGemH(box, Spnt1, Spb);									// �ׯ��2�擾
		Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);						// �ׯ��2�ݒ�
		Display(0);														// �ĕ`��
		Sphase = 0;
	} else if(ms==0) {													// ��������
		Sd1 = 0.0, Sd2 = 0.0, Sd4 = 0.0, Sd5 = 0.0;
		Si6 = 0, Si7 = 0, Si8 = 0, Si9 = 0;
		StartMenu();													// �ƭ��J�n����
		CheckCtrlKey1();												// ���䷰�m�F(�N����)
		if(!ReadGem(m_GemNo, &Spnt1, &Sd3)) {							// ��ζ�ē���
			ExitMenu(0, 1);												// �ƭ��I��
		} else {
			SetNumDisp(2);												// ���l�\����������
			CountEdtObj();												// �ҏW�Ώۗ��̶���
			GetAreaBox(2, &box);										// �I�𗧑̂��ޯ�����ގ擾
			BoxCenter(box, &Sbase);										// �ޯ�������擾
			Sd1 = box.bp[1].p[0]-box.bp[0].p[0];						// ��ζ�Ă�X����
			Sd2 = box.bp[1].p[1]-box.bp[0].p[1];						// ��ζ�Ă�Y����
			dd = box.bp[1].p[2]-box.bp[0].p[2];
			m_pNumDisp->SetParam(Sd1, Sd2, dd);
			TrackerGem3(box, Sbase, Sd4, Sd5, Spa);						// �ׯ��1�擾
			Sia[0]=0, Sia[1]=0, Sia[2]=0, Sia[3]=0, Sia[4]=2, Sia[5]=2;	// �ׯ��1�F�ݒ�
			Sia[6]=2, Sia[7]=2, Sia[8]=2, Sia[9]=2, Sia[10]=2, Sia[11]=2;
			Tens[0] = 7, Tens[1] = 7, Tens[2] = 7, Tens[3] = 7;			// �ׯ��1��߼�ݐݒ�
			Tens[4] = 6, Tens[5] = 5, Tens[6] = 5, Tens[7] = 6;
			Tens[8] = 6, Tens[9] = 5, Tens[10] = 5, Tens[11] = 6;
			Tracker1.Setup(Spa, Sia, Tens, 12, 0, 7);					// �ׯ��1�ݒ�
			TrackerGemH(box, Spnt1, Spb);								// �ׯ��2�擾
			Sib[0] = 0, TensC[0] = 6;									// �ׯ��2�F/��߼�ݐݒ�
			Tracker2.Setup(Spb, Sib, TensC, 1, 1, 7);					// �ׯ��2�\��
		}
		Display(0);														// �ĕ`��
		Sphase = 0;
		m_CursorNo = (m_ActMenu!=NONE) ? 1 : m_CursorNo;				// ����No�ݒ�
	} else if(ms==4||ms==9) {											// ϳ��E�t����/�I��
		Tracker1.Reset(1);												// �ׯ��1����
		Tracker2.Reset(1);												// �ׯ��2����
		Display(0);														// �ĕ`��
		ExitMenu(0, 1);													// �ƭ��I��
		EndJMenu1();													// �ƭ��I������
	}
}
