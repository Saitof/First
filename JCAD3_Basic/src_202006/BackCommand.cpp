/////////////////////////////////////////////////////////////////////////////
// ���̕ҏW�R�}���h�t���s(�߂�)��
// BackCommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/14 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "DataType.h"
#include "CCommand.h"
#include "NumFunc.h"

/**************************************************/
void BackMove(int flg, PNTTYPE* opt, PNTTYPE* npt,
              double dx, double dy, double dz)							//<<< �t���s�ړ�
/**************************************************/
{
	if(fabs(dx)>EPSILON||fabs(dy)>EPSILON||fabs(dz)>EPSILON) {
		opt->p[0] += dx, opt->p[1] += dy, opt->p[2] += dz;				// ���ް������s�ړ�
		if(flg==1) {													// <�X�V�׸�ON�̏ꍇ>
			npt->p[0] -= dx, npt->p[1] -= dy, npt->p[2] -= dz;			// �V�ް������s�ړ�
		}
	}
}

/****************************************************/
void BackRotate(int flg, PNTTYPE* opt, PNTTYPE* npt,
                PNTTYPE cp, PNTTYPE pnt, double th)						//<<< �t��]�ړ�
/****************************************************/
{
	PNTTYPE dis;
	VECTYPE vec;
	double  rth, wk, rot[3][3];
	int     i, j;

	if(fabs(th)<EPSILON) {												// �p�x���I��
		return;
	}
	rth = th*PI/180.0;
	vec.p[0] = pnt.p[0], vec.p[1] = pnt.p[1], vec.p[2] = pnt.p[2];
	VecN(&vec);															// �P���޸�ى�
	SetRotateMatrix(vec, rth, rot);										// ��]��د���ݒ�
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += rot[i][j]*cp.p[j];									// �ړ��ʌv�Z
		}
		dis.p[i] = cp.p[i]-wk;
	}
	VecRotateMove(opt, rot, dis);										// ���ް�����]�ړ�
	if(flg==1) {														// <�X�V�׸�ON�̏ꍇ>
		SetRotateMatrix(vec, -rth, rot);								// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {											// �ړ��ʌv�Z
			for(wk=0, j=0; j<DIM; j++) {
				wk += rot[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		VecRotateMove(npt, rot, dis);									// �V�ް��t��]�ړ�
	}
}

/****************************************************/
void BackMirror(int flg, PNTTYPE* opt, PNTTYPE* npt,
                PNTTYPE spt, PNTTYPE ept, int plane)					//<<< �t�����ړ�
/****************************************************/
{
	PNTTYPE pt;
	double  as, bs, cs, ds, rs, x1, y1, z1, x2, y2, z2;

	plane = (plane-1<0) ? 2 : plane-1;									// �ʕ��������K��
	pt = spt;
	pt.p[plane] += 1.0;
	x1 = ept.p[0]-spt.p[0], y1 = ept.p[1]-spt.p[1], z1 = ept.p[2]-spt.p[2];
	x2 = pt.p[0]-spt.p[0],  y2 = pt.p[1]-spt.p[1],  z2 = pt.p[2]-spt.p[2];
	as = (y1*z2)-(z1*y2),   bs = (z1*x2)-(x1*z2),   cs = (x1*y2)-(y1*x2);
	ds = -(ept.p[0]*as+ept.p[1]*bs+ept.p[2]*cs);
	rs = sqrt(as*as+bs*bs+cs*cs);
	as /= rs, bs /= rs, cs /= rs, ds /= rs;
	rs = (as*opt->p[0]+bs*opt->p[1]+cs*opt->p[2]+ds)*2.0;
	opt->p[0] -= rs*as, opt->p[1] -= rs*bs, opt->p[2] -= rs*cs;			// ���ް��������ړ�
	if(flg==1) {														// <�X�V�׸�ON�̏ꍇ>
		rs = (as*npt->p[0]+bs*npt->p[1]+cs*npt->p[2]+ds)*2.0;
		npt->p[0] -= rs*as, npt->p[1] -= rs*bs, npt->p[2] -= rs*cs;		// �V�ް��t�����ړ�
	}
}

/***********************************************************/
void BackScale(int flg, PNTTYPE* opt, PNTTYPE* npt,
               PNTTYPE cp, double sx, double sy, double sz)				//<<< �t���ޕύX
/***********************************************************/
{
	PNTTYPE dis;
	double  mat[3][3], wk;
	int     i, j;

	if(fabs(sx-1.0)<EPSILON&&fabs(sy-1.0)<EPSILON&&fabs(sz-1.0)<EPSILON) {
		return;															// ���{�I��
	}
	mat[0][0] = sx,  mat[0][1] = 0.0, mat[0][2] = 0.0;					// ��د���ݒ�
	mat[1][0] = 0.0, mat[1][1] = sy,  mat[1][2] = 0.0;
	mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = sz;
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += mat[i][j]*cp.p[j];									// �ړ��ʌv�Z
		}
		dis.p[i] = cp.p[i]-wk;
	}
	VecRotateMove(opt, mat, dis);										// ���ް������ޕύX
	if(flg==1) {														// <�X�V�׸�ON�̏ꍇ>
		mat[0][0] = 1.0/sx, mat[1][1] = 1.0/sy, mat[2][2] = 1.0/sz;		// ��د���ݒ�
		for(i=0; i<DIM; i++) {											// �ړ��ʌv�Z
			for(wk=0.0, j=0; j<DIM; j++) {
				wk += mat[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		VecRotateMove(npt, mat, dis);									// �V�ް��t���ޕύX
	}
}

/**************************************************************/
void BackResize1(int flg, PNTTYPE* opt, PNTTYPE* npt, int pln,
                 int mpf, double s1, double s2, double s3,
                 PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)					//<<< �t�����w��ػ���1
/**************************************************************/
{
	int    i, j=0, i1, i2, sf1, ef1, ef2;
	double sp1, sp2, ep1, ep2, bp, ss1, ss2, es1, es2, wsc, ln1, ln2, wln;

	for(i=0; i<DIM; i++) {
		if(fabs(p1.p[i]-p3.p[i])>EPSILON) {								// ������W���m�F
			j++;
			i1 = i;
		}
	}
	if(j!=1) {															// ��������~
		return;
	}
	s1 = (s1>=0.0&&s1<0.01) ? 0.01 : s1;
	s2 = (s2>=0.0&&s2<0.01) ? 0.01 : s2;
	s3 = (s3>=0.0&&s3<0.01) ? 0.01 : s3;
	sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
	sf1 = 0,   ef1 = 0,   ef2 = 0;
	ss2 = 1.0, es2 = 1.0;
	if(s2<0.0) {														// <2�_Ӱ��>
		if(fabs(s1-1.0)<EPSILON&&fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// ��_�ʒu
			sp1 = p1.p[i1], ep1 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep1 = p1.p[i1];
		}
		ln1 = ep1-sp1;													// ��_�ԋ���
		ss1 = s1, es1 = s3;												// ��{��
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// �{��!=1.0
		ef1 = (fabs(s3-1.0)>EPSILON) ? 1 : ef1;
	} else {															// <3�_Ӱ��>
		if(fabs(s1-1.0)<EPSILON&&
           fabs(s2-1.0)<EPSILON&&
           fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// ��_�ʒu
			sp1 = p1.p[i1], ep2 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep2 = p1.p[i1];
		}
		ep1 = p2.p[i1], sp2 = p2.p[i1];
		ln1 = ep1-sp1,  ln2 = ep2-sp2;									// ��_�ԋ���
		ss1 = s1, es1 = s2, ss2 = s2, es2 = s3;							// ��{��
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// �{��!=1.0
		ef2 = (fabs(s3-1.0)>EPSILON) ? 1 : ef2;
	}
	if(pln==i1) {														// ػ��ލ��W���擾
		i2 = (pln+1>2) ? 0 : pln+1;
	} else {
		i2 = pln;
	}
	bp = p1.p[i2];														// ػ��ފ�_
	if(mpf==2||(mpf==0&&opt->p[i2]<bp)||(mpf==1&&opt->p[i2]>bp)) {		// <<����������v>>
		if(opt->p[i1]>=sp1&&opt->p[i1]<=ep1) {							// <�1�͈͓�>
			wln = opt->p[i1]-sp1;										// �n�_����̋���
			wsc = ss1+(es1-ss1)*wln/ln1;								// �Y���ʒu�̔{��
			opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
		} else if(opt->p[i1]>sp2&&opt->p[i1]<=ep2) {					// <�2�͈͓�>
			wln = opt->p[i1]-sp2;										// �n�_����̋���
			wsc = ss2+(es2-ss2)*wln/ln2;								// �Y���ʒu�̔{��
			opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
		} else if(sf1==1&&opt->p[i1]<sp1) {								// <�{��!=1.0�̊1�O>
			opt->p[i2] = ss1*(opt->p[i2]-bp)+bp;
		} else if(ef1==1&&opt->p[i1]>ep1) {								// <�{��!=1.0�̊1��>
			opt->p[i2] = es1*(opt->p[i2]-bp)+bp;
		} else if(ef2==1&&opt->p[i1]>ep2) {								// <�{��!=1.0�̊2��>
			opt->p[i2] = es2*(opt->p[i2]-bp)+bp;
		}
	}
	if(flg==1) {														// <<<�X�V�׸�ON�̏ꍇ>>>
		if(mpf==2||(mpf==0&&npt->p[i2]<bp)||(mpf==1&&npt->p[i2]>bp)) {	// <<����������v>>
			if(npt->p[i1]>=sp1&&npt->p[i1]<=ep1) {						// <�1�͈͓�>
				wln = npt->p[i1]-sp1;									// �n�_����̋���
				wsc = 1.0/(ss1+(es1-ss1)*wln/ln1);						// �Y���ʒu�̋t�{��
				npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;					// �t�����w��ػ���
			} else if(npt->p[i1]>sp2&&npt->p[i1]<=ep2) {				// <�2�͈͓�>
				wln = npt->p[i1]-sp2;									// �n�_����̋���
				wsc = 1.0/(ss2+(es2-ss2)*wln/ln2);						// �Y���ʒu�̋t�{��
				npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;					// �t�����w��ػ���
			} else if(sf1==1&&npt->p[i1]<sp1) {							// <�{��!=1.0�̊1�O>
				npt->p[i2] = 1.0/ss1*(npt->p[i2]-bp)+bp;				// �t�����w��ػ���
			} else if(ef1==1&&npt->p[i1]>ep1) {							// <�{��!=1.0�̊1��>
				npt->p[i2] = 1.0/es1*(npt->p[i2]-bp)+bp;				// �t�����w��ػ���
			} else if(ef2==1&&npt->p[i1]>ep2) {							// <�{��!=1.0�̊2��>
				npt->p[i2] = 1.0/es2*(npt->p[i2]-bp)+bp;				// �t�����w��ػ���
			}
		}
	}
}

/***********************************************************/
void BackResize2(int flg, PNTTYPE* opt, PNTTYPE* npt,
                 int f1, int f2, int d1, int d2, double ln,
                 PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)				//<<< �t�Ȑ��w��ػ���1
/***********************************************************/
{
	int    i;
	double pr, lw, dd;

	if(fabs(ln)<EPSILON||pn==0) {										// �������/����_���͒��~
		return;
	}
	MakeSCurve(&pn, 0, 3, 2.0, pa, ia);									// S�Ȑ�����
	if(f1) {															// <�����w��̏ꍇ>
		for(i=0; i<pn; i++) {
			dd = pa[i].p[d2]-bs.p[d2];									// ��_�Ƃ̻��ގw�������
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// ���̏ꍇ��0
		}
	} else {															// <�����w��̏ꍇ>
		for(i=0; i<pn; i++) {
			dd = bs.p[d2]-pa[i].p[d2];									// ��_�Ƃ̻��ގw�������
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// ���̏ꍇ��0
		}
	}
	if(f2||(f1&&opt->p[d2]>bs.p[d2])||(!f1&&opt->p[d2]<bs.p[d2])) {		// <���_���Ώۗ̈��>
		for(i=1; i<pn-1; i++) {											// ���_�̒��������ʒu����
			if(opt->p[d1]<pa[i].p[d1]) {								// �܂��Ȑ��̐���������
				break;
			}
		}
		pr = (opt->p[d1]-pa[i-1].p[d1])/(pa[i].p[d1]-pa[i-1].p[d1]);	// �������ʒu�̔䗦
		pr = (pr<0.0) ? 0.0 : pr;										// �䗦���͈͊O
		pr = (pr>1.0) ? 1.0 : pr;
		lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;							// �������̻��ގw������ʒu
		lw += pa[i-1].p[d2];											// ���_�ʒu�̻��ގw���������
		pr = lw/ln;														// ���ޕύX�䗦
		lw = opt->p[d2]-bs.p[d2];										// ��_�Ƃ̻��ގw�������
		opt->p[d2] = bs.p[d2]+lw*pr;									// ���ގw������ʒu�ύX
	}
	if(flg==1) {														// <�X�V�׸�ON�̏ꍇ>
		if(f2||(f1&&npt->p[d2]>bs.p[d2])||(!f1&&npt->p[d2]<bs.p[d2])) {	// <���_���Ώۗ̈��>
			for(i=1; i<pn-1; i++) {										// ���_�̒��������ʒu����
				if(npt->p[d1]<pa[i].p[d1]) {							// �܂��Ȑ��̐���������
					break;
				}
			}
			pr = (npt->p[d1]-pa[i-1].p[d1])/(pa[i].p[d1]-pa[i-1].p[d1]);// �������ʒu�̔䗦
			pr = (pr<0.0) ? 0.0 : pr;									// �䗦���͈͊O
			pr = (pr>1.0) ? 1.0 : pr;
			lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;						// �������̻��ގw������ʒu
			lw += pa[i-1].p[d2];										// ���_�ʒu�̻��ގw���������
			pr = lw/ln;													// ���ޕύX�䗦
			lw = npt->p[d2]-bs.p[d2];									// ��_�Ƃ̻��ގw�������
			npt->p[d2] = bs.p[d2]+lw/pr;								// ���ގw������ʒu�ύX
		}
	}
}

/****************************************************************/
void BackBend(int flg, PNTTYPE* opt, PNTTYPE* npt,
              int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)		//<<< �t�~���Ȃ�
/****************************************************************/
{
	PNTTYPE pnt, cp, rp, wsp[2], wep[2];
	VECTYPE vec;
	int     i, dc;
	double  rots[3][3], rote[3][3], rtbs[3][3], rtbe[3][3];
	double  ln, dd, rth, wth, bth, d1, d2, rr;

	dc = (fc+vc==1) ? 2 : ((fc+vc==2) ? 1 : 0);							// �Ȃ��鉜�s�����擾
	ln = ep.p[fc]-sp.p[fc];												// �Ȃ���ʒ���
	if(fabs(th)<EPSILON||fabs(ln)<EPSILON) {							// �p�x/�ʒ���=0�͒��~
		return;
	}
	dd = (ln*360.0)/(th*PI);											// �Ȃ���ʒ��a
	rth = (th*PI)/360.0;												// ��]�p�x(׼ޱ�)�Z�o
	vec.p[fc] = 0.0;													// ��]�޸�ِݒ�
	vec.p[vc] = 0.0;
	vec.p[dc] = ((fc==0&&vc==2)||(fc==1&&vc==0)||(fc==2&&vc==1))
                ? -1.0 : 1.0;
	for(i=0; i<DIM; i++) {
		cp.p[i] = (sp.p[i]+ep.p[i])/2.0;								// ��_1(�Ȃ��O)�Z�o
	}
	rp = cp;															// ��_2(�Ȃ���)�Z�o
	rp.p[vc] = cp.p[vc]-dd/2.0;
	if(sp.p[fc]>opt->p[fc]) {											// <��](����)�̏ꍇ>
		SetRotateMatrix(vec, rth, rots);								// ��]��د���쐬
		SetRotateMatrix(vec, -rth, rtbs);								// ��]��د���쐬(�t)
		wsp[0] = sp;
		VecRotate(&wsp[0], rots);										// �n�_��]��ʒu�Z�o
		pnt = sp;
		rr = (dd/2.0)+pnt.p[vc]-cp.p[vc];								// �n�_�Ȃ���ʒu�Z�o
		wth = (pnt.p[fc]-cp.p[fc])*2.0/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wsp[1] = pnt;
		for(i=0; i<DIM; i++) {											// ��]�ƋȂ��덷�Z�o
			wsp[0].p[i] = wsp[1].p[i]-wsp[0].p[i];
		}
		VecRotateMove(opt, rots, wsp[0]);								// ��]�ړ�(����)
		if(flg==1) {													// <�X�V�׸�ON�̏ꍇ>
			for(i=0; i<DIM; i++) {
				npt->p[i] = npt->p[i]-wsp[0].p[i];						// �ړ�(����)[�t]
			}
			VecRotate(npt, rtbs);										// ��](����)[�t]
		}
	} else if(ep.p[fc]<opt->p[fc]) {									// <��](�E��)�̏ꍇ>
		SetRotateMatrix(vec, -rth, rote);								// ��]��د���쐬
		SetRotateMatrix(vec, rth, rtbe);								// ��]��د���쐬(�t)
		wep[0] = ep;
		VecRotate(&wep[0], rote);										// �I�_��]��ʒu�Z�o
		pnt = ep;
		rr = (dd/2.0)+pnt.p[vc]-cp.p[vc];								// �I�_�Ȃ���ʒu�Z�o
		wth = (pnt.p[fc]-cp.p[fc])*2.0/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wep[1] = pnt;
		for(i=0; i<DIM; i++) {
			wep[0].p[i] = wep[1].p[i]-wep[0].p[i];						// ��]�ƋȂ��덷�Z�o
		}
		VecRotateMove(opt, rote, wep[0]);								// ��]�ړ�(�E��)
		if(flg==1) {													// <�X�V�׸�ON�̏ꍇ>
			for(i=0; i<DIM; i++) {
				npt->p[i] = npt->p[i]-wep[0].p[i];						// �ړ�(�E��)[�t]
			}
			VecRotate(npt, rtbe);										// ��](�E��)[�t]
		}
	} else {															// <�Ȃ�(����)�̏ꍇ>
		rr = (dd/2.0)+opt->p[vc]-cp.p[vc];								// ���a�Z�o(��_1->)
		wth = (opt->p[fc]-cp.p[fc])*2.0/dd;								// �p�x�Z�o(��_2->)
		opt->p[fc] = rr*sin(wth)+rp.p[fc];								// �Ȃ���ʒu�Z�o���
		opt->p[vc] = rr*cos(wth)+rp.p[vc];								// ��_2�Ɉړ�
		if(flg==1) {													// <�X�V�׸�ON�̏ꍇ>
			d1 = npt->p[fc]-rp.p[fc];									// �V�ް����a
			d2 = npt->p[vc]-rp.p[vc];
			ln = sqrt(d1*d1+d2*d2);
			ln = (rr<0) ? -ln : ln;										// �V���a�𕉕ϊ�
			if(rth>0) {
				bth = atan2(npt->p[fc]-rp.p[fc], npt->p[vc]-rp.p[vc]);	// �V�ް���_�p�x
			} else {
				bth = atan2(rp.p[fc]-npt->p[fc], rp.p[vc]-npt->p[vc]);	// �V�ް���_�p�x
			}
			bth = (bth<-10||bth>10) ? 0.0 : bth;						// ���Α��ɏo���ꍇ�̕␳
			if(wth>0&&bth<0&&fabs(wth-(bth+2.0*PI))<PI) {
				bth += 2.0*PI;
			} else if(wth<0&&bth>0&&fabs(wth-(bth-2.0*PI))<PI) {
				bth -= 2.0*PI;
			}
			npt->p[fc] = cp.p[fc]+dd*bth/2.0;							// �V�ް��Ȃ��O�ʒu��
			npt->p[vc] = ln+cp.p[vc]-dd/2.0;							// �ϊ�(�t�ϊ�)
		}
	}
}

/********************************************************/
void BackTwist(int flg, PNTTYPE* opt, PNTTYPE* npt,
               int dr, double th, double ln, PNTTYPE cp)				//<<< �t�Ђ˂�
/********************************************************/
{
	PNTTYPE dis;
	VECTYPE vec;
	int     i, j;
	double  wth, wk, rot[3][3];

	if(fabs(th)<EPSILON||fabs(ln)<EPSILON) {							// �p�x/���������~
		return;
	}
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 0.0, vec.p[dr] = -1.0;	// �Ђ˂莲�޸�ٍ쐬
	wk = opt->p[dr]-cp.p[dr];											// �Y���ʒu����
	wth = PI*th*wk/(180.0*ln);											// �Y���ʒu�p�x
	SetRotateMatrix(vec, wth, rot);										// ��]��د���ݒ�
	for(i=0; i<DIM; i++) {
		for(wk=0.0, j=0; j<DIM; j++) {
			wk += rot[i][j]*cp.p[j];									// �ړ��ʌv�Z
		}
		dis.p[i] = cp.p[i]-wk;
	}
	VecRotateMove(opt, rot, dis);										// ���ް����Ђ˂�
	if(flg==1) {														// <�X�V�׸�ON�̏ꍇ>
		SetRotateMatrix(vec, -wth, rot);								// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {											// �ړ��ʌv�Z
			for(wk=0.0, j=0; j<DIM; j++) {
				wk += rot[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		VecRotateMove(npt, rot, dis);									// �V�ް��t�Ђ˂�
	}
}

/*********************************************************/
void BackShear(int flg, PNTTYPE* opt, PNTTYPE* npt,
               int dr1, int dr2, double inc, PNTTYPE bas)				//<<< �t����f
/*********************************************************/
{
	double ln;

	if(fabs(inc)<EPSILON) {												// �X�������~
		return;
	}
	ln = opt->p[dr1]-bas.p[dr1];										// ��_����̐�����������
	ln = ln*inc;														// ����f���������ړ��ʎZ�o
	opt->p[dr2] += ln;													// ���ړ��ʕ��␳
	if(flg==1) {														// <�X�V�׸�ON�̏ꍇ>
		ln = npt->p[dr1]-bas.p[dr1];									// ��_����̐�����������
		ln = ln*inc;													// ����f���������ړ��ʎZ�o
		npt->p[dr2] -= ln;												// �t�ړ��ʕ��␳
	}
}

/***********************************************************************/
void BackResize3(int flg, PNTTYPE* opt, PNTTYPE* npt, int pln, int mpf,
                 double s1, double s2, double s3, int vflg, double v1,
                 double v2, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)			//<<< �t�����w��ػ���2
/***********************************************************************/
{
	int    i, j=0, i1, i2, i3, sf1, ef1, ef2;
	double sp1, sp2, ep1, ep2, bp, ss1, ss2, es1, es2, wsc, ln1, ln2, wln;

	for(i=0; i<DIM; i++) {
		if(fabs(p1.p[i]-p3.p[i])>EPSILON) {								// ������W���m�F
			j++;
			i1 = i;
		}
	}
	if(j!=1) {															// ��������~
		return;
	}
	s1 = (s1>=0.0&&s1<0.01) ? 0.01 : s1;
	s2 = (s2>=0.0&&s2<0.01) ? 0.01 : s2;
	s3 = (s3>=0.0&&s3<0.01) ? 0.01 : s3;
	sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
	sf1 = 0,   ef1 = 0,   ef2 = 0;
	ss2 = 1.0, es2 = 1.0;
	if(s2<0.0) {														// <2�_Ӱ��>
		if(fabs(s1-1.0)<EPSILON&&fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// ��_�ʒu
			sp1 = p1.p[i1], ep1 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep1 = p1.p[i1];
		}
		ln1 = ep1-sp1;													// ��_�ԋ���
		ss1 = s1, es1 = s3;												// ��{��
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// �{��!=1.0
		ef1 = (fabs(s3-1.0)>EPSILON) ? 1 : ef1;
	} else {															// <3�_Ӱ��>
		if(fabs(s1-1.0)<EPSILON&&
           fabs(s2-1.0)<EPSILON&&
           fabs(s3-1.0)<EPSILON) {
			return;
		}
		if(p1.p[i1]<p3.p[i1]) {											// ��_�ʒu
			sp1 = p1.p[i1], ep2 = p3.p[i1];
		} else {
			sp1 = p3.p[i1], ep2 = p1.p[i1];
		}
		ep1 = p2.p[i1], sp2 = p2.p[i1];
		ln1 = ep1-sp1,  ln2 = ep2-sp2;									// ��_�ԋ���
		ss1 = s1, es1 = s2, ss2 = s2, es2 = s3;							// ��{��
		sf1 = (fabs(s1-1.0)>EPSILON) ? 1 : sf1;							// �{��!=1.0
		ef2 = (fabs(s3-1.0)>EPSILON) ? 1 : ef2;
	}
	if(pln==i1) {														// ػ��ލ��W���擾
		i2 = (pln+1>2) ? 0 : pln+1;
	} else {
		i2 = pln;
	}
	i3 = (pln+1>2) ? 0 : pln+1;											// ���������Z�o
	i3 = (i3+1>2) ? 0 : i3+1;
	bp = p1.p[i2];														// ػ��ފ�_
	if((vflg==0)||														// <<<������������Ȃ�/
       (vflg==1&&opt->p[i3]>v1-EPSILON)||								//    �����w��Ŕ͈͓�/
       (vflg==2&&opt->p[i3]<v2+EPSILON)||								//    ����w��Ŕ͈͓�
       (vflg==3&&opt->p[i3]>v1-EPSILON&&opt->p[i3]<v2+EPSILON)) {		//    �㉺�w��Ŕ͈͓�>>>
		if(mpf==2||(mpf==0&&opt->p[i2]<bp)||(mpf==1&&opt->p[i2]>bp)) {	// <<����������v>>
			if(opt->p[i1]>=sp1&&opt->p[i1]<=ep1) {						// <�1�͈͓�>
				wln = opt->p[i1]-sp1;									// �n�_����̋���
				wsc = ss1+(es1-ss1)*wln/ln1;							// �Y���ʒu�̔{��
				opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
			} else if(opt->p[i1]>sp2&&opt->p[i1]<=ep2) {				// <�2�͈͓�>
				wln = opt->p[i1]-sp2;									// �n�_����̋���
				wsc = ss2+(es2-ss2)*wln/ln2;							// �Y���ʒu�̔{��
				opt->p[i2] = wsc*(opt->p[i2]-bp)+bp;
			} else if(sf1==1&&opt->p[i1]<sp1) {							// <�{��!=1.0�̊1�O>
				opt->p[i2] = ss1*(opt->p[i2]-bp)+bp;
			} else if(ef1==1&&opt->p[i1]>ep1) {							// <�{��!=1.0�̊1��>
				opt->p[i2] = es1*(opt->p[i2]-bp)+bp;
			} else if(ef2==1&&opt->p[i1]>ep2) {							// <�{��!=1.0�̊2��>
				opt->p[i2] = es2*(opt->p[i2]-bp)+bp;
			}
		}
	}
	if(flg==1) {														// <<<<�X�V�׸�ON�̏ꍇ>>>>
		if((vflg==0)||													// <<<������������Ȃ�/
           (vflg==1&&npt->p[i3]>v1-EPSILON)||							//    �����w��Ŕ͈͓�/
           (vflg==2&&npt->p[i3]<v2+EPSILON)||							//    ����w��Ŕ͈͓�
           (vflg==3&&npt->p[i3]>v1-EPSILON&&npt->p[i3]<v2+EPSILON)) {	//    �㉺�w��Ŕ͈͓�>>>
			if(mpf==2||(mpf==0&&npt->p[i2]<bp)||
                       (mpf==1&&npt->p[i2]>bp)) {						// <<����������v>>
				if(npt->p[i1]>=sp1&&npt->p[i1]<=ep1) {					// <�1�͈͓�>
					wln = npt->p[i1]-sp1;								// �n�_����̋���
					wsc = 1.0/(ss1+(es1-ss1)*wln/ln1);					// �Y���ʒu�̋t�{��
					npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;				// �t�����w��ػ���
				} else if(npt->p[i1]>sp2&&npt->p[i1]<=ep2) {			// <�2�͈͓�>
					wln = npt->p[i1]-sp2;								// �n�_����̋���
					wsc = 1.0/(ss2+(es2-ss2)*wln/ln2);					// �Y���ʒu�̋t�{��
					npt->p[i2] = wsc*(npt->p[i2]-bp)+bp;				// �t�����w��ػ���
				} else if(sf1==1&&npt->p[i1]<sp1) {						// <�{��!=1.0�̊1�O>
					npt->p[i2] = 1.0/ss1*(npt->p[i2]-bp)+bp;			// �t�����w��ػ���
				} else if(ef1==1&&npt->p[i1]>ep1) {						// <�{��!=1.0�̊1��>
					npt->p[i2] = 1.0/es1*(npt->p[i2]-bp)+bp;			// �t�����w��ػ���
				} else if(ef2==1&&npt->p[i1]>ep2) {						// <�{��!=1.0�̊2��>
					npt->p[i2] = 1.0/es2*(npt->p[i2]-bp)+bp;			// �t�����w��ػ���
				}
			}
		}
	}
}

/************************************************************************/
void BackResize4(int flg, PNTTYPE* opt, PNTTYPE* npt, int f1, int f2,
                 int d1, int d2, int d3, double ln, int vflg, double v1,
                 double v2, PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)	//<<< �t�Ȑ��w��ػ���2
/************************************************************************/
{
	int    i;
	double pr, lw, dd;

	if(fabs(ln)<EPSILON||pn==0) {										// �������/����_���͒��~
		return;
	}
	MakeSCurve(&pn, 0, 3, 2.0, pa, ia);									// S�Ȑ�����
	if(f1) {															// <�����w��̏ꍇ>
		for(i=0; i<pn; i++) {
			dd = pa[i].p[d2]-bs.p[d2];									// ��_�Ƃ̻��ގw�������
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// ���̏ꍇ��0
		}
	} else {															// <�����w��̏ꍇ>
		for(i=0; i<pn; i++) {
			dd = bs.p[d2]-pa[i].p[d2];									// ��_�Ƃ̻��ގw�������
			pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;							// ���̏ꍇ��0
		}
	}
	if((vflg==0)||														// <<<������������Ȃ�/
       (vflg==1&&opt->p[d3]>v1-EPSILON)||								//    �����w��Ŕ͈͓�/
       (vflg==2&&opt->p[d3]<v2+EPSILON)||								//    ����w��Ŕ͈͓�
       (vflg==3&&opt->p[d3]>v1-EPSILON&&opt->p[d3]<v2+EPSILON)) {		//    �㉺�w��Ŕ͈͓�>>>
		if(f2||(f1&&opt->p[d2]>bs.p[d2])||(!f1&&opt->p[d2]<bs.p[d2])) {	// <���_���Ώۗ̈��>
			for(i=1; i<pn-1; i++) {										// ���_�̒��������ʒu����
				if(opt->p[d1]<pa[i].p[d1]) {							// �܂��Ȑ��̐���������
					break;
				}
			}
			pr = (opt->p[d1]-pa[i-1].p[d1])/(pa[i].p[d1]-pa[i-1].p[d1]);// �������ʒu�̔䗦
			pr = (pr<0.0) ? 0.0 : pr;									// �䗦���͈͊O
			pr = (pr>1.0) ? 1.0 : pr;
			lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;						// �������̻��ގw������ʒu
			lw += pa[i-1].p[d2];										// ���_�ʒu�̻��ގw���������
			pr = lw/ln;													// ���ޕύX�䗦
			lw = opt->p[d2]-bs.p[d2];									// ��_�Ƃ̻��ގw�������
			opt->p[d2] = bs.p[d2]+lw*pr;								// ���ގw������ʒu�ύX
		}
	}
	if(flg==1) {														// <<<�X�V�׸�ON�̏ꍇ>>>
		if((vflg==0)||													// <<<������������Ȃ�/
           (vflg==1&&npt->p[d3]>v1-EPSILON)||							//    �����w��Ŕ͈͓�/
           (vflg==2&&npt->p[d3]<v2+EPSILON)||							//    ����w��Ŕ͈͓�
           (vflg==3&&npt->p[d3]>v1-EPSILON&&npt->p[d3]<v2+EPSILON)) {	//    �㉺�w��Ŕ͈͓�>>>
			if(f2||(f1&&npt->p[d2]>bs.p[d2])||
                   (!f1&&npt->p[d2]<bs.p[d2])) {						// <���_���Ώۗ̈��>
				for(i=1; i<pn-1; i++) {									// ���_�̒��������ʒu����
					if(npt->p[d1]<pa[i].p[d1]) {						// �܂��Ȑ��̐���������
						break;
					}
				}
				pr = (npt->p[d1]-pa[i-1].p[d1])/
                     (pa[i].p[d1]-pa[i-1].p[d1]);						// �������ʒu�̔䗦
				pr = (pr<0.0) ? 0.0 : pr;								// �䗦���͈͊O
				pr = (pr>1.0) ? 1.0 : pr;
				lw = (pa[i].p[d2]-pa[i-1].p[d2])*pr;					// �������̻��ގw������ʒu
				lw += pa[i-1].p[d2];									// ���_�ʒu�̻��ގw���������
				pr = lw/ln;												// ���ޕύX�䗦
				lw = npt->p[d2]-bs.p[d2];								// ��_�Ƃ̻��ގw�������
				npt->p[d2] = bs.p[d2]+lw/pr;							// ���ގw������ʒu�ύX
			}
		}
	}
}
