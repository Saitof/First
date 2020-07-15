/////////////////////////////////////////////////////////////////////////////
// ���̕ҏW�R�}���h��
// EditCommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/14 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "NumFunc.h"

/*************************/
OBJTYPE* MoveObject(void)												//<<< ���s�ړ������
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, vnum, ix, iy, iz;
	double   dx, dy, dz;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		ix = GetParaInt(cmp, 0);										// �ړ��ʎ擾
		iy = GetParaInt(cmp, 1);
		iz = GetParaInt(cmp, 2);
		if(ix==0&&iy==0&&iz==0) {										// �ړ��ʖ��I��
			return op;
		}
		dx = ix/1000.0, dy = iy/1000.0, dz = iz/1000.0;					// �ړ��ʎ����ϊ�
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// ���_���W�擾
			pnt.p[0] += dx, pnt.p[1] += dy, pnt.p[2] += dz;				// �ړ��ʕ��␳
			PutVtx(op, i, pnt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ���ݒ�
		}
	}
	return op;
}

/***************************/
OBJTYPE* RotateObject(void)												//<<< ��]�ړ������
/***************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  cp, dis, pnt;
	VECTYPE  vec;
	int      i, j, vnum;
	double   th, rot[3][3], wk;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		if((i=GetParaInt(cmp, 0))==0) {									// ��]�p�x�擾
			return op;
		}
		th = i/1000.0*PI/180.0;
		GetParaPnt(cmp, 0, &cp);										// ��]�����W�擾
		GetParaPnt(cmp, 1, &pnt);										// ��]�������޸�َ擾
		vec.p[0] = pnt.p[0], vec.p[1] = pnt.p[1], vec.p[2] = pnt.p[2];
		VecN(&vec);														// �P���޸�ى�
		SetRotateMatrix(vec, th, rot);									// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// �ړ��ʌv�Z
				wk += rot[i][j]*cp.p[j];
			}
			dis.p[i] = cp.p[i]-wk;
		}
		vnum = GetVtxNum(op);											// ���_���W���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// ���_���W�擾
			VecRotateMove(&pnt, rot, dis);								// ���_���W��]�ړ�
			PutVtx(op, i, pnt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/***************************/
OBJTYPE* MirrorObject(void)												//<<< �����ړ������
/***************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt, spt, ept, pt;
	int      i, j, vnum, lnum, stp, bln, fln, vno1, vno2, flg1, flg2;
	double   as, bs, cs, ds, rs, x1, y1, z1, x2, y2, z2;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		i = (GetParaInt(cmp, 0)-1<0) ? 2 : GetParaInt(cmp, 0)-1;		// �ʕ��������K��
		GetParaPnt(cmp, 0, &spt);
		GetParaPnt(cmp, 1, &ept);
		pt = spt, pt.p[i] += 1.0;
		x1 = ept.p[0]-spt.p[0], y1 = ept.p[1]-spt.p[1], z1 = ept.p[2]-spt.p[2];
		x2 = pt.p[0]-spt.p[0], y2 = pt.p[1]-spt.p[1], z2 = pt.p[2]-spt.p[2];
		as = (y1*z2)-(z1*y2), bs = (z1*x2)-(x1*z2), cs = (x1*y2)-(y1*x2);
		ds = -(ept.p[0]*as+ept.p[1]*bs+ept.p[2]*cs);
		rs = sqrt(as*as+bs*bs+cs*cs);
		as /= rs, bs /= rs, cs /= rs, ds /= rs;
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// ���_���W�擾
			rs = (as*pnt.p[0]+bs*pnt.p[1]+cs*pnt.p[2]+ds)*2.0;
			pnt.p[0] -= rs*as, pnt.p[1] -= rs*bs, pnt.p[2] -= rs*cs;	// ���W�l�ϓ]
			PutVtx(op, i, pnt);											// ���_���W�X�V
		}
		lnum = GetFlpNum1(op);											// ��ٰ��1���擾
		for(stp=-1, i=0; i<lnum; i++) {
			GetFlp1(op, i, &vno1, &flg1);								// ��ٰ��1���擾
			if(flg1) {													// <��ٰ�ߏI���̏ꍇ>
				bln = i, fln = (i-stp)/2;								// �Y��ٰ�ߍŌ��
				for(j=stp+1; j<stp+1+fln; j++) {						// �Y��ٰ�ߍőO����
					GetFlp1(op, j, &vno1, &flg1);						// �Ō���ƒ��_�ԍ�����
					GetFlp1(op, bln, &vno2, &flg2);
					PutFlp1(op, j, vno2, flg1);
					PutFlp1(op, bln, vno1, flg2);
					bln--;
				}
				stp = i;												// ����ٰ�ߊJ�n�ʒu(+1)
			}
		}
		lnum = GetFlpNum2(op);											// ��ٰ�ߐ�2�擾
		for(stp=-1, i=0; i<lnum; i++) {
			GetFlp2(op, i, &vno1, &flg1);								// ��ٰ��2���擾
			if(flg1) {													// <��ٰ�ߏI���̏ꍇ>
				bln = i, fln = (i-stp)/2;								// �Y��ٰ�ߍŌ��
				for(j=stp+1; j<stp+1+fln; j++) {						// �Y��ٰ�ߍőO����
					GetFlp2(op, j, &vno1, &flg1);						// �Ō���ƒ��_�ԍ�����
					GetFlp2(op, bln, &vno2, &flg2);
					PutFlp2(op, j, vno2, flg1);
					PutFlp2(op, bln, vno1, flg2);
					bln--;
				}
				stp = i;												// ����ٰ�ߊJ�n�ʒu(+1)
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/**************************/
OBJTYPE* ScaleObject(void)												//<<< ���ޕύX�����
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  cp;
	int      ix, iy, iz;
	double   sx, sy, sz, mat[3][3];

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		ix = GetParaInt(cmp, 0);										// ���ޕύX�{���擾
		iy = GetParaInt(cmp, 1);
		iz = GetParaInt(cmp, 2);
		if(ix==1000&&iy==1000&&iz==1000) {								// ���{�͏I��
			return op;
		}
		GetParaPnt(cmp, 0, &cp);										// �ϊ����S���W�擾
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// �{�������ϊ�
		mat[0][0] = sx,  mat[0][1] = 0.0, mat[0][2] = 0.0;				// ��د���ݒ�
		mat[1][0] = 0.0, mat[1][1] = sy,  mat[1][2] = 0.0;
		mat[2][0] = 0.0, mat[2][1] = 0.0, mat[2][2] = sz;
		Affine(op, cp, mat);											// �̨ݕϊ�(���ޕύX)
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/*************************/
OBJTYPE* BendObject(void)												//<<< �~���Ȃ������
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  sp, ep, cp, rp, pnt, wsp[2], wep[2];
	VECTYPE  vec;
	int      i, vnum, fc, vc, dc, ith;
	double   dth, ln, dd, rr, wth, rots[3][3], rote[3][3];

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		fc = GetParaInt(cmp, 0);										// �Ȃ����������擾
		vc = GetParaInt(cmp, 1);										// �Ȃ����������擾
		ith = GetParaInt(cmp, 2);										// �Ȃ��p�x�擾
		GetParaPnt(cmp, 0, &sp);										// ����n�_�擾
		GetParaPnt(cmp, 1, &ep);										// ����I�_�擾
		ln = ep.p[fc]-sp.p[fc];											// �Ȃ��ʒ���
		if(ith==0||fabs(ln)<EPSILON) {									// �p�x/�ʂ̒���=0�͒��~
			return op;
		}
		dc = (fc+vc==1) ? 2 : ((fc+vc==2) ? 1 : 0);						// �Ȃ����s�����擾
		dth = ith/1000.0;												// �����p�x�Z�o
		dd = (ln*360.0)/(dth*PI*2.0);									// �Ȃ��ʂ̔��a
		dth = (dth*PI)/360.0;											// ��]����p�x(׼ޱ�)�Z�o
		vec.p[fc] = 0.0, vec.p[vc] = 0.0;								// ��]�޸�ِݒ�
		vec.p[dc] = ((fc==0&&vc==2)||(fc==1&&vc==0)||(fc==2&&vc==1))
                    ? -1.0 : 1.0;
		for(i=0; i<DIM; i++) {
			cp.p[i] = (sp.p[i]+ep.p[i])/2;								// ��_1(�Ȃ��O)�Z�o
		}
		rp = cp, rp.p[vc] = cp.p[vc]-dd;								// ��_2(�Ȃ���)�Z�o
		SetRotateMatrix(vec, dth, rots);								// ��]��د���쐬
		wsp[0] = sp;
		VecRotate(&wsp[0], rots);										// �n�_��]��ʒu�Z�o
		pnt = sp;
		rr = dd+pnt.p[vc]-cp.p[vc];										// �n�_�Ȃ���ʒu�Z�o
		wth = (pnt.p[fc]-cp.p[fc])/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wsp[1] = pnt;
		for(i=0; i<DIM; i++) {
			wsp[0].p[i] = wsp[1].p[i]-wsp[0].p[i];						// ��]�ƋȂ��덷�Z�o
		}
		SetRotateMatrix(vec, -dth, rote);								// ��]��د���쐬
		wep[0] = ep;
		VecRotate(&wep[0], rote);										// �I�_��]��ʒu�Z�o
		pnt = ep;
		rr = dd+pnt.p[vc]-cp.p[vc];										// �I�_�Ȃ���ʒu�Z�o
		wth = (pnt.p[fc]-cp.p[fc])/dd;
		pnt.p[fc] = rr*sin(wth)+rp.p[fc];
		pnt.p[vc] = rr*cos(wth)+rp.p[vc];
		wep[1] = pnt;
		for(i=0; i<DIM; i++) {
			wep[0].p[i] = wep[1].p[i]-wep[0].p[i];						// ��]�ƋȂ��덷�Z�o
		}
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// ���_���W�擾
			if(sp.p[fc]>pnt.p[fc]) {									// <��](����)�̏ꍇ>
				VecRotateMove(&pnt, rots, wsp[0]);						// ��]�ړ�(����)
			} else if(ep.p[fc]<pnt.p[fc]) {								// <��](�E��)�̏ꍇ>
				VecRotateMove(&pnt, rote, wep[0]);						// ��]�ړ�(�E��)
			} else {													// <�Ȃ�(����)�̏ꍇ>
				rr = dd+pnt.p[vc]-cp.p[vc];								// ���a�Z�o(��_1->)
				wth = (pnt.p[fc]-cp.p[fc])/dd;							// �p�x�Z�o(��_2->)
				pnt.p[fc] = rr*sin(wth)+rp.p[fc];						// �Ȃ���ʒu�Z�o���
				pnt.p[vc] = rr*cos(wth)+rp.p[vc];						// ��_2�Ɉړ�
			}
			PutVtx(op, i, pnt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/**************************/
OBJTYPE* TwistObject(void)												//<<< �Ђ˂�����
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt, cpt, dis;
	VECTYPE  vec;
	int      i, j, k, dir, ith, iln, num;
	double   dth, dln, wth, wk, rot[3][3];

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		dir = GetParaInt(cmp, 0);										// �Ђ˂莲�����擾
		ith = GetParaInt(cmp, 1);										// �Ђ˂�p�x�擾
		iln = GetParaInt(cmp, 2);										// �Ђ˂莲���擾
		if(ith==0||iln==0) {											// �p�x/���������~
			return op;
		}
		GetParaPnt(cmp, 0, &cpt);										// �Ђ˂��_�擾
		dth = ith/1000.0;												// �����p�x�Z�o
		dln = iln/1000.0;												// ���������Z�o
		vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = 0.0;
		vec.p[dir] = -1.0;
		num = GetVtxNum(op);											// ���_���W���擾
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pnt);										// ���_���W�擾
			wk = pnt.p[dir]-cpt.p[dir];									// �Y���ʒu����
			wth = PI*dth*wk/(180.0*dln);								// �Y���ʒu�p�x
			SetRotateMatrix(vec, wth, rot);								// ��]��د���ݒ�
			for(j=0; j<DIM; j++) {										// �ړ��ʌv�Z
				for(wk=0.0, k=0; k<DIM; k++) {
					wk += rot[j][k]*cpt.p[k];
				}
				dis.p[j] = cpt.p[j]-wk;
			}
			VecRotateMove(&pnt, rot, dis);								// ���_���W��]�ړ�
			PutVtx(op, i, pnt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject1(void)											//<<< �����w��ػ���1�����
/****************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pt, p1, p2, p3;
	int      i, j=0, pln, mpf, s1, s2, s3, i1, i2, sf1, ef1, ef2, num;
	double   sp1, sp2, ep1, ep2, ln1, ln2, bp, wln, ss1, ss2, es1, es2, wsc;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		pln = GetParaInt(cmp, 0);										// �w�蕽�ʎ擾
		mpf = GetParaInt(cmp, 1);										// �����׸ގ擾
		s1 = GetParaInt(cmp, 2);										// �{��1�擾
		s2 = GetParaInt(cmp, 3);										// �{��2�擾
		if(s2==-1000) {													// <�o�O�Ή� 07.01.09>
			s2 = -1;													// -1000�ł́AVer2.0.2�ȉ�
			PutParaInt(cmp, 3, s2);										// ���Ή��̂��߁A-1 �ɕύX
		}
		s3 = GetParaInt(cmp, 4);										// �{��3�擾
		GetParaPnt(cmp, 0, &p1);										// ��_1�擾
		GetParaPnt(cmp, 1, &p2);										// ��_2�擾
		GetParaPnt(cmp, 2, &p3);										// ��_3�擾
		for(i=0; i<DIM; i++) {
			if(fabs(p1.p[i]-p3.p[i])>EPSILON) {							// ������W���m�F
				j++; i1 = i;
			}
		}
		if(j!=1) {														// ��������~
			return op;
		}
		s1 = (s1>=0&&s1<10) ? 10 : s1;
		s2 = (s2>=0&&s2<10) ? 10 : s2;
		s3 = (s3>=0&&s3<10) ? 10 : s3;
		sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
		sf1 = 0,   ef1 = 0,   ef2 = 0;
		ss2 = 1.0, es2 = 1.0;
		if(s2<0) {														// <2�_Ӱ��>
			if(s1==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// ��_�ʒu
				sp1 = p1.p[i1], ep1 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep1 = p1.p[i1];
			}
			ln1 = ep1-sp1;												// ��_�ԋ���
			ss1 = s1/1000.0, es1 = s3/1000.0;							// ��{��
			sf1 = (s1!=1000) ? 1 : sf1;									// �{��!=1.0
			ef1 = (s3!=1000) ? 1 : ef1;
		} else {														// <3�_Ӱ��>
			if(s1==1000&&s2==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// ��_�ʒu
				sp1 = p1.p[i1], ep2 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep2 = p1.p[i1];
			}
			ep1 = p2.p[i1],  sp2 = p2.p[i1];
			ln1 = ep1-sp1,   ln2 = ep2-sp2;								// ��_�ԋ���
			ss1 = s1/1000.0, es1 = s2/1000.0;							// ��{��
			ss2 = s2/1000.0, es2 = s3/1000.0;
			sf1 = (s1!=1000) ? 1 : sf1;									// �{��!=1.0
			ef2 = (s3!=1000) ? 1 : ef2;
		}
		i2 = (pln==i1) ? ((pln+1>2) ? 0 : pln+1) : pln;					// ػ��ލ��W���擾
		bp = p1.p[i2];													// ػ��ފ�_
		num = GetVtxNum(op);											// ���_���W���擾
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pt);											// ���_���W�擾
			if(mpf==2||(mpf==0&&pt.p[i2]<bp)||(mpf==1&&pt.p[i2]>bp)) {	// <<����������v>>
				if(pt.p[i1]>=sp1&&pt.p[i1]<=ep1) {						// <�1�͈͓�>
					wln = pt.p[i1]-sp1;									// �n�_����̋���
					wsc = ss1+(es1-ss1)*wln/ln1;						// �Y���ʒu�̔{��
					pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
				} else if(pt.p[i1]>sp2&&pt.p[i1]<=ep2) {				// <�2�͈͓�>
					wln = pt.p[i1]-sp2;									// �n�_����̋���
					wsc = ss2+(es2-ss2)*wln/ln2;						// �Y���ʒu�̔{��
					pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
				} else if(sf1==1&&pt.p[i1]<sp1) {						// <�{��!=1.0�̊1�O>
					pt.p[i2] = ss1*(pt.p[i2]-bp)+bp;
				} else if(ef1==1&&pt.p[i1]>ep1) {						// <�{��!=1.0�̊1��>
					pt.p[i2] = es1*(pt.p[i2]-bp)+bp;
				} else if(ef2==1&&pt.p[i1]>ep2) {						// <�{��!=1.0�̊2��>
					pt.p[i2] = es2*(pt.p[i2]-bp)+bp;
				}
			}
			PutVtx(op, i, pt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject2(void)											//<<< �Ȑ��w��ػ���1�����
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  bs, pt;
	int      i, j, f1, f2, d1, d2, ln, pn, vnum;
	double   dln, pr, lw, dd;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		f1 = GetParaInt(cmp, 0);										// �����׸ގ擾
		f2 = GetParaInt(cmp, 1);										// �����׸ގ擾
		d1 = GetParaInt(cmp, 2);										// ���������擾
		d2 = GetParaInt(cmp, 3);										// ���ގw������擾
		ln = GetParaInt(cmp, 4);										// ������擾
		pn = GetParaInt(cmp, 5);										// ����_���擾
		if(ln==0||pn==0) {												// �������/����_���͒��~
			delete[] pa;
			delete[] ia;
			return op;
		}
		dln = ln/1000.0;												// �����������
		for(i=0; i<pn; i++) {
			ia[i] = GetParaInt(cmp, i+6);								// �Ȑ�Ӱ�ގ擾
		}
		GetParaPnt(cmp, 0, &bs);										// ��_�擾
		for(i=0; i<pn; i++) {
			GetParaPnt(cmp, i+1, &pa[i]);								// ����_�擾
		}
		MakeSCurve(&pn, 0, 3, 2.0, pa, ia);								// S�Ȑ�����
		if(f1) {														// <�����w��̏ꍇ>
			for(i=0; i<pn; i++) {
				dd = pa[i].p[d2]-bs.p[d2];								// ��_�Ƃ̻��ގw�������
				pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// ���̏ꍇ��0
			}
		} else {														// <�����w��̏ꍇ>
			for(i=0; i<pn; i++) {
				dd = bs.p[d2]-pa[i].p[d2];								// ��_�Ƃ̻��ގw�������
	            pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// ���̏ꍇ��0
			}
		}
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// ���_���W�擾
			if(f2||(f1&&pt.p[d2]>bs.p[d2])||(!f1&&pt.p[d2]<bs.p[d2])) {	// <���_���Ώۗ̈��>
				for(j=1; j<pn-1; j++) {									// ���_�̒��������ʒu����
					if(pt.p[d1]<pa[j].p[d1]) {							// �܂��Ȑ��̐���������
						break;
					}
				}
				pr=(pt.p[d1]-pa[j-1].p[d1])/(pa[j].p[d1]-pa[j-1].p[d1]);// �������ʒu�̔䗦
				pr = (pr<0.0) ? 0.0 : ((pr>1.0) ? 1.0 : pr);			// �䗦���͈͊O
				lw = (pa[j].p[d2]-pa[j-1].p[d2])*pr;					// �������̻��ގw������ʒu
				lw += pa[j-1].p[d2];									// ���_�ʒu�̻��ގw���������
				pr = lw/dln;											// ���ޕύX�䗦
				lw = pt.p[d2]-bs.p[d2];									// ��_�Ƃ̻��ގw�������
				pt.p[d2] = bs.p[d2]+lw*pr;								// ���ގw������ʒu�ύX
				PutVtx(op, i, pt);										// ���_���W�X�V
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ���ݒ�
		}
	}
	delete[] pa;
	delete[] ia;
	return op;
}

/**************************/
OBJTYPE* ShearObject(void)												//<<< ����f�����
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt, bas;
	int      i, vnum, d1, d2;
	double   inc, ln;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		d1 = GetParaInt(cmp, 0);										// ����f���������擾
		d2 = GetParaInt(cmp, 1);										// ����f���������擾
		if((i=GetParaInt(cmp, 2))==0) {									// �X���擾
			return op;
		}
		inc = i/1000.0;
		GetParaPnt(cmp, 0, &bas);										// ����f��_�擾
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pnt);										// ���_���W�擾
			ln = pnt.p[d1]-bas.p[d1];									// ��_����̐�����������
			ln = ln*inc;												// ����f���������ړ��ʎZ�o
			pnt.p[d2] += ln;											// �ړ��ʕ��␳
			PutVtx(op, i, pnt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ���ݒ�
		}
	}
	return op;
}

/*************************/
OBJTYPE* RingObject(void)												//<<< �ݸ޺����
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  cp, sp, ep, rp, pt, ws[2], we[2];
	VECTYPE  vec;
	int      i, ix, iy, iz, fc, vc, it, dc, vnum;
	double   sx, sy, sz, ln, dt, dd, rr, wt, mat1[3][3], mat2[3][3];

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		ix = GetParaInt(cmp, 0);										// ���ޕύX�{���擾
		iy = GetParaInt(cmp, 1);
		iz = GetParaInt(cmp, 2);
		fc = GetParaInt(cmp, 3);										// �Ȃ����������擾
		vc = GetParaInt(cmp, 4);										// �Ȃ����������擾
		it = GetParaInt(cmp, 5);										// �Ȃ��p�x�擾
		GetParaPnt(cmp, 0, &cp);										// �ϊ����S���W�擾
		GetParaPnt(cmp, 1, &sp);										// ����n�_�擾
		GetParaPnt(cmp, 2, &ep);										// ����I�_�擾
		ln = ep.p[fc] - sp.p[fc];										// �Ȃ��ʒ���
		if(ix==1000&&iy==1000&&iz==1000&&(it==0||fabs(ln)<EPSILON)) {	// ���{�Ŋp�x/�ʂ̒���=0�͒��~
			return op;
		}
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// �{�������ϊ�
		mat1[0][0] = sx,  mat1[0][1] = 0.0, mat1[0][2] = 0.0;			// ��د���ݒ�
		mat1[1][0] = 0.0, mat1[1][1] = sy,  mat1[1][2] = 0.0;
		mat1[2][0] = 0.0, mat1[2][1] = 0.0, mat1[2][2] = sz;
		Affine(op, cp, mat1);											// �̨ݕϊ�
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
		dc = (fc+vc==1) ? 2 : ((fc+vc==2) ? 1 : 0);						// �Ȃ����s�����擾
		dt = it/1000.0;													// �����p�x�Z�o
		dd = (ln*360.0)/(dt*PI*2.0);									// �Ȃ��ʂ̔��a
		dt = (dt*PI)/360.0;												// ��]����p�x(׼ޱ�)�Z�o
		vec.p[fc] = 0.0, vec.p[vc] = 0.0;								// ��]�޸�ِݒ�
		vec.p[dc] = ((fc==0&&vc==2)||(fc==1&&vc==0)||(fc==2&&vc==1))
                    ? -1.0 : 1.0;
		for(i=0; i<DIM; i++) {
			cp.p[i] = (sp.p[i]+ep.p[i])/2;								// ��_1(�Ȃ��O)�Z�o
		}
		rp = cp, rp.p[vc] = cp.p[vc]-dd;								// ��_2(�Ȃ���)�Z�o
		SetRotateMatrix(vec, dt, mat1);									// ��]��د���쐬
		ws[0] = sp;
		VecRotate(&ws[0], mat1);										// �n�_��]��ʒu�Z�o
		pt = sp;
		rr = dd+pt.p[vc]-cp.p[vc];										// �n�_�Ȃ���ʒu�Z�o
		wt = (pt.p[fc]-cp.p[fc])/dd;
		pt.p[fc] = rr*sin(wt)+rp.p[fc];
		pt.p[vc] = rr*cos(wt)+rp.p[vc];
		ws[1] = pt;
		for(i=0; i<DIM; i++) {
			ws[0].p[i] = ws[1].p[i]-ws[0].p[i];							// ��]�ƋȂ��덷�Z�o
		}
		SetRotateMatrix(vec, -dt, mat2);								// ��]��د���쐬
		we[0] = ep;
		VecRotate(&we[0], mat2);										// �I�_��]��ʒu�Z�o
		pt = ep;
		rr = dd+pt.p[vc]-cp.p[vc];										// �I�_�Ȃ���ʒu�Z�o
		wt = (pt.p[fc]-cp.p[fc])/dd;
		pt.p[fc] = rr*sin(wt)+rp.p[fc];
		pt.p[vc] = rr*cos(wt)+rp.p[vc];
		we[1] = pt;
		for(i=0; i<DIM; i++) {
			we[0].p[i] = we[1].p[i]-we[0].p[i];							// ��]�ƋȂ��덷�Z�o
		}
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// ���_���W�擾
			if(sp.p[fc]>pt.p[fc]) {										// <��](����)�̏ꍇ>
				VecRotateMove(&pt, mat1, ws[0]);						// ��]�ړ�(����)
			} else if(ep.p[fc]<pt.p[fc]) {								// <��](�E��)�̏ꍇ>
				VecRotateMove(&pt, mat2, we[0]);						// ��]�ړ�(�E��)
			} else {													// <�Ȃ�(����)�̏ꍇ>
				rr = dd+pt.p[vc]-cp.p[vc];								// ���a�Z�o(��_1->)
				wt = (pt.p[fc]-cp.p[fc])/dd;							// �p�x�Z�o(��_2->)
				pt.p[fc] = rr*sin(wt)+rp.p[fc];							// �Ȃ���ʒu�Z�o���
				pt.p[vc] = rr*cos(wt)+rp.p[vc];							// ��_2�Ɉړ�
			}
			PutVtx(op, i, pt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/**************************/
OBJTYPE* EarthObject(void)												//<<< ��������
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rp, cp, pt, dsx, dsy, dsz;
	VECTYPE  rv;
	int      i, j, mx, my, mz, rx, ry, rz, ix, iy, iz, vnum;
	double   dx, dy, dz, tx, ty, tz, sx, sy, sz, wk;
	double   matx[3][3], maty[3][3], matz[3][3];

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		mx = GetParaInt(cmp, 0);										// �ړ��ʎ擾
		my = GetParaInt(cmp, 1);
		mz = GetParaInt(cmp, 2);
		rx = GetParaInt(cmp, 3);										// ��]�p�x�擾
		ry = GetParaInt(cmp, 4);
		rz = GetParaInt(cmp, 5);
		ix = GetParaInt(cmp, 6);										// ���ޕύX�{���擾
		iy = GetParaInt(cmp, 7);
		iz = GetParaInt(cmp, 8);
		if(mx==0&&my==0&&mz==0&&rx==0&&ry==0&&rz==0&&
           ix==1000&&iy==1000&&iz==1000) {								// �ړ��ʖ�+�p�x��+���{�͏I��
			return op;
		}
		dx = mx/1000.0, dy = my/1000.0, dz = mz/1000.0;					// �ړ��ʎ����ϊ�
		tx = rx/1000.0*PI/180.0;										// ��]�p�x�ϊ�
		ty = ry/1000.0*PI/180.0;
		tz = rz/1000.0*PI/180.0;
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// �{�������ϊ�
		GetParaPnt(cmp, 0, &rp);										// ��]�����W�擾
		GetParaPnt(cmp, 1, &cp);										// �ϊ����S���W�擾
		matx[0][0] = sx,  matx[0][1] = 0.0, matx[0][2] = 0.0;			// ��د���ݒ�
		matx[1][0] = 0.0, matx[1][1] = sy,  matx[1][2] = 0.0;
		matx[2][0] = 0.0, matx[2][1] = 0.0, matx[2][2] = sz;
		Affine(op, cp, matx);											// �̨ݕϊ�(���ޕύX)
		rv.p[0] = -1.0, rv.p[1] = 0.0, rv.p[2] = 0.0;					// X���p��]���
		SetRotateMatrix(rv, tx, matx);									// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// �ړ��ʌv�Z
				wk += matx[i][j]*rp.p[j];
			}
			dsx.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = -1.0, rv.p[2] = 0.0;					// Y���p��]���
		SetRotateMatrix(rv, ty, maty);									// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// �ړ��ʌv�Z
				wk += maty[i][j]*rp.p[j];
			}
			dsy.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = 0.0, rv.p[2] = -1.0;					// Z���p��]���
		SetRotateMatrix(rv, tz, matz);									// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// �ړ��ʌv�Z
				wk += matz[i][j]*rp.p[j];
			}
			dsz.p[i] = rp.p[i]-wk;
		}
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// ���_���W�擾
			VecRotateMove(&pt, matx, dsx);								// ���_���W��]�ړ�(X��)
			VecRotateMove(&pt, maty, dsy);								// ���_���W��]�ړ�(X��)
			VecRotateMove(&pt, matz, dsz);								// ���_���W��]�ړ�(X��)
			pt.p[0] += dx, pt.p[1] += dy, pt.p[2] += dz;				// �ړ��ʕ��␳
			PutVtx(op, i, pt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ���ݒ�
		}
	}
	return op;
}

/**************************/
OBJTYPE* SpaceObject(void)												//<<< ��߰������
/**************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rp, cp, pt, dsx, dsy, dsz;
	VECTYPE  rv;
	int      i, j, mx, my, mz, rx, ry, rz, ix, iy, iz, vnum;
	double   dx, dy, dz, tx, ty, tz, sx, sy, sz, wk;
	double   matx[3][3], maty[3][3], matz[3][3];

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		mx = GetParaInt(cmp, 0);										// �ړ��ʎ擾
		my = GetParaInt(cmp, 1);
		mz = GetParaInt(cmp, 2);
		rx = GetParaInt(cmp, 3);										// ��]�p�x�擾
		ry = GetParaInt(cmp, 4);
		rz = GetParaInt(cmp, 5);
		ix = GetParaInt(cmp, 6);										// ���ޕύX�{���擾
		iy = GetParaInt(cmp, 7);
		iz = GetParaInt(cmp, 8);
		if(mx==0&&my==0&&mz==0&&rx==0&&ry==0&&rz==0&&
           ix==1000&&iy==1000&&iz==1000) {								// �ړ��ʖ�+�p�x��+���{�͏I��
			return op;
		}
		dx = mx/1000.0, dy = my/1000.0, dz = mz/1000.0;					// �ړ��ʎ����ϊ�
		tx = rx/1000.0*PI/180.0;										// ��]�p�x�ϊ�
		ty = ry/1000.0*PI/180.0;
		tz = rz/1000.0*PI/180.0;
		sx = ix/1000.0, sy = iy/1000.0, sz = iz/1000.0;					// �{�������ϊ�
		GetParaPnt(cmp, 0, &rp);										// ��]�����W�擾
		GetParaPnt(cmp, 1, &cp);										// �ϊ����S���W�擾
		matx[0][0] = sx,  matx[0][1] = 0.0, matx[0][2] = 0.0;			// ��د���ݒ�
		matx[1][0] = 0.0, matx[1][1] = sy,  matx[1][2] = 0.0;
		matx[2][0] = 0.0, matx[2][1] = 0.0, matx[2][2] = sz;
		Affine(op, cp, matx);											// �̨ݕϊ�(���ޕύX)
		rv.p[0] = -1.0, rv.p[1] = 0.0, rv.p[2] = 0.0;					// X���p��]���
		SetRotateMatrix(rv, tx, matx);									// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// �ړ��ʌv�Z
				wk += matx[i][j]*rp.p[j];
			}
			dsx.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = -1.0, rv.p[2] = 0.0;					// Y���p��]���
		SetRotateMatrix(rv, ty, maty);									// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// �ړ��ʌv�Z
				wk += maty[i][j]*rp.p[j];
			}
			dsy.p[i] = rp.p[i]-wk;
		}
		rv.p[0] = 0.0, rv.p[1] = 0.0, rv.p[2] = -1.0;					// Z���p��]���
		SetRotateMatrix(rv, tz, matz);									// ��]��د���ݒ�
		for(i=0; i<DIM; i++) {
			for(wk=0.0, j=0; j<DIM; j++) {								// �ړ��ʌv�Z
				wk += matz[i][j]*rp.p[j];
			}
			dsz.p[i] = rp.p[i]-wk;
		}
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// ���_���W�擾
			VecRotateMove(&pt, matx, dsx);								// ���_���W��]�ړ�(X��)
			VecRotateMove(&pt, maty, dsy);								// ���_���W��]�ړ�(X��)
			VecRotateMove(&pt, matz, dsz);								// ���_���W��]�ړ�(X��)
			pt.p[0] += dx, pt.p[1] += dy, pt.p[2] += dz;				// �ړ��ʕ��␳
			PutVtx(op, i, pt);											// ���_���W�X�V
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ���ݒ�
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject3(void)											//<<< �����w��ػ���2�����
/****************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pt, p1, p2, p3;
	int      i, j=0, pln, mpf, s1, s2, s3, vflg, v1, v2;
	int      i1, i2, i3, sf1, ef1, ef2, num;
	double   sp1, sp2, ep1, ep2, ln1, ln2, bp;
	double   wln, ss1, ss2, es1, es2, wsc, vr1, vr2;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		pln = GetParaInt(cmp, 0);										// �w�蕽�ʎ擾
		mpf = GetParaInt(cmp, 1);										// �����׸ގ擾
		s1 = GetParaInt(cmp, 2);										// �{��1�擾
		s2 = GetParaInt(cmp, 3);										// �{��2�擾
		s3 = GetParaInt(cmp, 4);										// �{��3�擾
		vflg = GetParaInt(cmp, 5);										// ���������׸ގ擾
		v1 = GetParaInt(cmp, 6);										// �����͈�1�擾
		v2 = GetParaInt(cmp, 7);										// �����͈�2�擾
		GetParaPnt(cmp, 0, &p1);										// ��_1�擾
		GetParaPnt(cmp, 1, &p2);										// ��_2�擾
		GetParaPnt(cmp, 2, &p3);										// ��_3�擾
		for(i=0; i<DIM; i++) {
			if(fabs(p1.p[i]-p3.p[i])>EPSILON) {							// ������W���m�F
				j++; i1 = i;
			}
		}
		if(j!=1) {														// ��������~
			return op;
		}
		vr1 = (double)v1/1000.0;										// ��������͈͎����ϊ�
		vr2 = (double)v2/1000.0;
		i3 = (pln+1>2) ? 0 : pln+1;										// ���������Z�o
		i3 = (i3+1>2) ? 0 : i3+1;
		s1 = (s1>=0&&s1<10) ? 10 : s1;
		s2 = (s2>=0&&s2<10) ? 10 : s2;
		s3 = (s3>=0&&s3<10) ? 10 : s3;
		sp2 = 0.0, ep2 = 0.0, ln2 = 0.0;
		sf1 = 0,   ef1 = 0,   ef2 = 0;
		ss2 = 1.0, es2 = 1.0;
		if(s2<0) {														// <2�_Ӱ��>
			if(s1==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// ��_�ʒu
				sp1 = p1.p[i1], ep1 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep1 = p1.p[i1];
			}
			ln1 = ep1-sp1;												// ��_�ԋ���
			ss1 = s1/1000.0, es1 = s3/1000.0;							// ��{��
			sf1 = (s1!=1000) ? 1 : sf1;									// �{��!=1.0
			ef1 = (s3!=1000) ? 1 : ef1;
		} else {														// <3�_Ӱ��>
			if(s1==1000&&s2==1000&&s3==1000) {
				return op;
			}
			if(p1.p[i1]<p3.p[i1]) {										// ��_�ʒu
				sp1 = p1.p[i1], ep2 = p3.p[i1];
			} else {
				sp1 = p3.p[i1], ep2 = p1.p[i1];
			}
			ep1 = p2.p[i1],  sp2 = p2.p[i1];
			ln1 = ep1-sp1,   ln2 = ep2-sp2;								// ��_�ԋ���
			ss1 = s1/1000.0, es1 = s2/1000.0;							// ��{��
			ss2 = s2/1000.0, es2 = s3/1000.0;
			sf1 = (s1!=1000) ? 1 : sf1;									// �{��!=1.0
			ef2 = (s3!=1000) ? 1 : ef2;
		}
		i2 = (pln==i1) ? ((pln+1>2) ? 0 : pln+1) : pln;					// ػ��ލ��W���擾
		bp = p1.p[i2];													// ػ��ފ�_
		num = GetVtxNum(op);											// ���_���W���擾
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pt);											// ���_���W�擾
			if((vflg==0)||												// <<<������������Ȃ�/
               (vflg==1&&pt.p[i3]>vr1-EPSILON)||						//    �����w��Ŕ͈͓�/
               (vflg==2&&pt.p[i3]<vr2+EPSILON)||						//    ����w��Ŕ͈͓�
               (vflg==3&&pt.p[i3]>vr1-EPSILON&&pt.p[i3]<vr2+EPSILON)) {	//    �㉺�w��Ŕ͈͓�>>>
				if(mpf==2||(mpf==0&&pt.p[i2]<bp)||
                           (mpf==1&&pt.p[i2]>bp)) {						// <<����������v>>
					if(pt.p[i1]>=sp1&&pt.p[i1]<=ep1) {					// <�1�͈͓�>
						wln = pt.p[i1]-sp1;								// �n�_����̋���
						wsc = ss1+(es1-ss1)*wln/ln1;					// �Y���ʒu�̔{��
						pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
					} else if(pt.p[i1]>sp2&&pt.p[i1]<=ep2) {			// <�2�͈͓�>
						wln = pt.p[i1]-sp2;								// �n�_����̋���
						wsc = ss2+(es2-ss2)*wln/ln2;					// �Y���ʒu�̔{��
						pt.p[i2] = wsc*(pt.p[i2]-bp)+bp;
					} else if(sf1==1&&pt.p[i1]<sp1) {					// <�{��!=1.0�̊1�O>
						pt.p[i2] = ss1*(pt.p[i2]-bp)+bp;
					} else if(ef1==1&&pt.p[i1]>ep1) {					// <�{��!=1.0�̊1��>
						pt.p[i2] = es1*(pt.p[i2]-bp)+bp;
					} else if(ef2==1&&pt.p[i1]>ep2) {					// <�{��!=1.0�̊2��>
						pt.p[i2] = es2*(pt.p[i2]-bp)+bp;
					}
				}
				PutVtx(op, i, pt);										// ���_���W�X�V
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ�����ސݒ�
		}
	}
	return op;
}

/****************************/
OBJTYPE* ResizeObject4(void)											//<<< �Ȑ��w��ػ���2�����
/****************************/
{
	PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
	int*     ia = new int[View->m_MaxCPnt];
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  bs, pt;
	int      i, j, f1, f2, d1, d2, d3, ln, vflg, v1, v2, pn, vnum;
	double   dln, pr, lw, dd, vr1, vr2;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	if(!GetCmdExeStp()) {												// <����ގ��s��~��>
		f1 = GetParaInt(cmp, 0);										// �����׸ގ擾
		f2 = GetParaInt(cmp, 1);										// �����׸ގ擾
		d1 = GetParaInt(cmp, 2);										// ���������擾
		d2 = GetParaInt(cmp, 3);										// ���ގw������擾
		d3 = GetParaInt(cmp, 4);										// ���������擾
		ln = GetParaInt(cmp, 5);										// ������擾
		vflg = GetParaInt(cmp, 6);										// ���������׸ގ擾
		v1 = GetParaInt(cmp, 7);										// �����͈�1�擾
		v2 = GetParaInt(cmp, 8);										// �����͈�2�擾
		pn = GetParaInt(cmp, 9);										// ����_���擾
		if(ln==0||pn==0) {												// �������/����_���͒��~
			delete[] pa;
			delete[] ia;
			return op;
		}
		dln = ln/1000.0;												// �����������
		vr1 = (double)v1/1000.0;										// ��������͈͎����ϊ�
		vr2 = (double)v2/1000.0;
		for(i=0; i<pn; i++) {
			ia[i] = GetParaInt(cmp, i+10);								// �Ȑ�Ӱ�ގ擾
		}
		GetParaPnt(cmp, 0, &bs);										// ��_�擾
		for(i=0; i<pn; i++) {
			GetParaPnt(cmp, i+1, &pa[i]);								// ����_�擾
		}
		MakeSCurve(&pn, 0, 3, 2.0, pa, ia);								// S�Ȑ�����
		if(f1) {														// <�����w��̏ꍇ>
			for(i=0; i<pn; i++) {
				dd = pa[i].p[d2]-bs.p[d2];								// ��_�Ƃ̻��ގw�������
				pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// ���̏ꍇ��0
			}
		} else {														// <�����w��̏ꍇ>
			for(i=0; i<pn; i++) {
				dd = bs.p[d2]-pa[i].p[d2];								// ��_�Ƃ̻��ގw�������
	            pa[i].p[d2] = (dd<0.0) ? 0.0 : dd;						// ���̏ꍇ��0
			}
		}
		vnum = GetVtxNum(op);											// ���_���擾
		for(i=0; i<vnum; i++) {
			GetVtx(op, i, &pt);											// ���_���W�擾
			if((vflg==0)||												// <<<������������Ȃ�/
               (vflg==1&&pt.p[d3]>vr1-EPSILON)||						//    �����w��Ŕ͈͓�/
               (vflg==2&&pt.p[d3]<vr2+EPSILON)||						//    ����w��Ŕ͈͓�
               (vflg==3&&pt.p[d3]>vr1-EPSILON&&pt.p[d3]<vr2+EPSILON)) {	//    �㉺�w��Ŕ͈͓�>>>
				if(f2||(f1&&pt.p[d2]>bs.p[d2])||
                       (!f1&&pt.p[d2]<bs.p[d2])) {						// <���_���Ώۗ̈��>
					for(j=1; j<pn-1; j++) {								// ���_�̒��������ʒu����
						if(pt.p[d1]<pa[j].p[d1]) {						// �܂��Ȑ��̐���������
							break;
						}
					}
					pr = (pt.p[d1]-pa[j-1].p[d1])/
                         (pa[j].p[d1]-pa[j-1].p[d1]);					// �������ʒu�̔䗦
					pr = (pr<0.0) ? 0.0 : ((pr>1.0) ? 1.0 : pr);		// �䗦���͈͊O
					lw = (pa[j].p[d2]-pa[j-1].p[d2])*pr;				// �������̻��ގw������ʒu
					lw += pa[j-1].p[d2];								// ���_�ʒu�̻��ގw���������
					pr = lw/dln;										// ���ޕύX�䗦
					lw = pt.p[d2]-bs.p[d2];								// ��_�Ƃ̻��ގw�������
					pt.p[d2] = bs.p[d2]+lw*pr;							// ���ގw������ʒu�ύX
					PutVtx(op, i, pt);									// ���_���W�X�V
				}
			}
		}
		if(!GetResetObjMode()) {										// <RedoCommand�ȊO>
			SetObjBox(op);												// �ޯ���ݒ�
		}
	}
	delete[] pa;
	delete[] ia;
	return op;
}
