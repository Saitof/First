/////////////////////////////////////////////////////////////////////////////
// ���̕ҏW�R�}���h��
// ECommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "RCommand.h"
#include "Select.h"
#include "NumFunc.h"

/******************************************/
void Move(double dx, double dy, double dz)								//<<< ���s�ړ�
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(MOVE, 3, 0, &cmp);										// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// �ړ��ʐݒ�
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �ړ�����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*******************************************************/
static void CopyMove(double dx, double dy,
                     double dz, PNTTYPE cp, PNTTYPE sc)					//<<< ���s�ړ�(���ʗ���)
/*******************************************************/
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		SetObjPtr(op);													// ���ʑΏۗ��̐ݒ�
		wop = CopyObject();												// ���̕���
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <�{���ύX�L�̏ꍇ>
			CreatCmd(SCALE, 3, 1, &cmp);								// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(sc.p[0]*1000.0));					// �e�����{���ݒ�
			PutParaInt(cmp, 1, (int)(sc.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(sc.p[2]*1000.0));
			PutParaPnt(cmp, 0, cp);										// �ϊ����S���W�ݒ�
			SetObjPtr(wop);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ���ޕύX����ގ��s
		}
		CreatCmd(MOVE, 3, 0, &cmp);										// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// �ړ��ʐݒ�
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		SetObjPtr(wop);													// �����߲���ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �ړ�����ގ��s
		View->SetDispList(wop);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*******************************************************************/
static void GetLnBase(double dx, double dy, double dz, PNTTYPE* pa)		//<<< ���������_�擾
/*******************************************************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt, cpt, pt1, pt2, pp1, pp2;
	VECTYPE  vec;
	BOXTYPE  box;
	int      i, dsp, edt, num;
	double   ln, min1=DBL_MAX, min2=DBL_MAX;

	GetAreaBox(2, &box);												// �ޯ�����ގ擾
	ln = BoxCenter(box, &cpt);											// �ޯ�������擾
	vec.p[0] = dx, vec.p[1] = dy, vec.p[2] = dz;						// �ړ������޸��
	VecN(&vec);															// �P���޸�ى�
	for(i=0; i<DIM; i++) {
		pt1.p[i] = cpt.p[i]+vec.p[i]*ln;								// �ړ��������1�_(A)
	}
	for(i=0; i<DIM; i++) {
		vec.p[i] *= -1.0;												// �t�ړ������޸��
	}
	for(i=0; i<DIM; i++) {
		pt2.p[i] = cpt.p[i]+vec.p[i]*ln;								// �ړ��������1�_(B)
	}
	for(i=0; i<DIM; i++) {
		pp1.p[i] = 0.0, pp2.p[i] = 0.0;
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		num = GetVtxNum(op);											// ���_���擾
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pnt);										// ���_�擾
			ln = PntD(&pnt, &pt1);										// 2�_�ԋ���(3D)
			if(ln<min1) {												// A�_�ɍł��߂��_
				min1 = ln, pp1 = pnt;
			}
			ln = PntD(&pnt, &pt2);										// 2�_�ԋ���(3D)
			if(ln<min2) {												// B�_�ɍł��߂��_
				min2 = ln, pp2 = pnt;
			}
		}
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	pa[0] = pp1, pa[1] = pp2;
}

/********************************************************************/
static void CalcLn(PNTTYPE cp, PNTTYPE ws, PNTTYPE* pa, PNTTYPE* ln)	//<<< ���ޕύX�㋗���Z�o
/********************************************************************/
{
	PNTTYPE pt[2];
	int     i, j;
	double  dd;

	for(i=0; i<2; i++) {
		for(j=0; j<DIM; j++) {
			dd = pa[i].p[j]-cp.p[j];									// ��_->�Ώۓ_�޸��
			dd = dd*ws.p[j];											// �޸�ٔ{���ώZ
			pt[i].p[j] = dd+cp.p[j];									// ���ޕύX��Ώۓ_
		}
	}
	for(i=0; i<DIM; i++) {
		ln->p[i] = fabs(pt[0].p[i]-pt[1].p[i]);							// �Ώۓ_�ԋ���
	}
}

/****************************************************************/
static void CopyMoveCopy(double dx, double dy, double dz,
                         int cn, PNTTYPE cp, PNTTYPE sc, int sf)		//<<< ���s�ړ�����
/****************************************************************/		//    (���ʗ���)
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  pa[2], ln1, ln2, ss, is, ws;
	int      i, j, dsp, edt;
	double   ix, iy, iz, wx, wy, wz;

	if((fabs(sc.p[0])>EPSILON||
        fabs(sc.p[1])>EPSILON||
        fabs(sc.p[2])>EPSILON)&&sf!=0) {
		GetLnBase(dx, dy, dz, pa);										// ���������_�擾
		for(i=0; i<DIM; i++) {
			ss.p[i] = (1.0+sc.p[i]<0.01) ? 0.01 : 1.0+sc.p[i];			// �{���v�Z
		}
		CalcLn(cp, ss, pa, &ln1);										// 1�ڂ̋����Z�o
	}
	ix = dx, iy = dy, iz = dz, is = sc;									// ��ړ���															// �J�n�{��
	for(i=0; i<cn; i++) {												// ���ʐ����s��
		wx = dx, wy = dy, wz = dz;
		if(fabs(sc.p[0])>EPSILON||
           fabs(sc.p[1])>EPSILON||
           fabs(sc.p[2])>EPSILON) {										// <�{���ύX�L�̏ꍇ>
			for(j=0; j<DIM; j++) {
				is.p[j] += sc.p[j];										// �{�����Z
				ws.p[j] = (1.0+is.p[j]<0.01) ? 0.01 : 1.0+is.p[j];		// �{���v�Z
			}
			if(sf==1) {													// <���Ԋu�A���̏ꍇ>
				CalcLn(cp, ws, pa, &ln2);								// n+1�ڂ̋����Z�o
				if(dx>0) {												// 1�ڂ�n+1�ڂ̍����Z
					wx = dx-(ln1.p[0]-ln2.p[0]);
				} else if(dx<0) {
					wx = dx+(ln1.p[0]-ln2.p[0]);
				}
				if(dy>0) {
					wy = dy-(ln1.p[1]-ln2.p[1]);
				} else if(dy<0) {
					wy = dy+(ln1.p[1]-ln2.p[1]);
				}
				if(dz>0) {
					wz = dz-(ln1.p[2]-ln2.p[2]);
				} else if(dz<0) {
					wz = dz+(ln1.p[2]-ln2.p[2]);
				}
			} else if(sf==2) {											// <�s���Ԋu�A���̏ꍇ>
				wx = dx*ws.p[0]/ss.p[0];								// 1�ڂ�n+1�ڂ̔䗦
				wy = dy*ws.p[1]/ss.p[1];
				wz = dz*ws.p[2]/ss.p[2];
			}
		}
		ix += wx, iy += wy, iz += wz;									// �ړ��ʉ��Z
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
		while(op!=NULL) {
			SetObjPtr(op);												// ���ʑΏۗ��̐ݒ�
			wop = CopyObject();											// ���̕���
			if(fabs(sc.p[0])>EPSILON||
               fabs(sc.p[1])>EPSILON||
               fabs(sc.p[2])>EPSILON) {									// <�{���ύX�L�̏ꍇ>
				CreatCmd(SCALE, 3, 1, &cmp);							// ����ޗ̈�m��
				PutParaInt(cmp, 0, (int)(ws.p[0]*1000.0));				// �e�����{���ݒ�
				PutParaInt(cmp, 1, (int)(ws.p[1]*1000.0));
				PutParaInt(cmp, 2, (int)(ws.p[2]*1000.0));
				PutParaPnt(cmp, 0, cp);									// �ϊ����S���W�ݒ�
				SetObjPtr(wop);											// �Ώۗ��̐ݒ�
				SetCmdPtr(cmp);											// ����ސݒ�
				ExecCommand(FALSE);										// ���ޕύX����ގ��s
			}
			CreatCmd(MOVE, 3, 0, &cmp);									// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(ix*1000.0));						// �ړ��ʐݒ�
			PutParaInt(cmp, 1, (int)(iy*1000.0));
			PutParaInt(cmp, 2, (int)(iz*1000.0));
			SetObjPtr(wop);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// �ړ�����ގ��s
			View->SetDispList(wop);										// �ި���ڲ�ؽēo�^
			NextObjPtr(2, &op, &dsp, &edt);								// �����̂�
		}
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*****************************************************/
static void RedoMove(double dx, double dy, double dz)					//<<< ���s�ړ�(�Ď��s)
/*****************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==MOVE) {										// <�ړ�����ނ̏ꍇ>
			PutParaInt(cmp, 0, (int)(dx*1000.0));						// �ړ��ʐݒ�
			PutParaInt(cmp, 1, (int)(dy*1000.0));
			PutParaInt(cmp, 2, (int)(dz*1000.0));
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/**********************************************************/
void ObjMove(OBJTYPE* op, double dx, double dy, double dz)				//<<< �w�藧�̂̈ړ�
/**********************************************************/
{
	CMDTYPE* cmp;

	if(op!=NULL) {
		CreatCmd(MOVE, 3, 0, &cmp);										// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// �ړ��ʐݒ�
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �ړ�����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/************************************************************/
void ObjectMove(int flg, int cn, PNTTYPE sc,
                SCRTYPE bp, double dx, double dy, double dz)			//<<< ���̕��s�ړ�
/************************************************************/
{
	BOXTYPE box;
	PNTTYPE cpt, wsc;
	int     i;

	if(flg==0) {														// <����̂�>
		InitLevel();													// Undo/Redo������
		if(cn>0) {														// <�ړ����ʂ̏ꍇ>
			for(i=0; i<DIM; i++) {
				wsc.p[i] = 1.0;
			}
			if(fabs(sc.p[0]-1.0)>EPSILON||
               fabs(sc.p[1]-1.0)>EPSILON||
               fabs(sc.p[2]-1.0)>EPSILON) {								// <�{���ύX�L>
				GetAreaBox(2, &box);									// �ޯ�����ގ擾
				for(i=0; i<DIM; i++) {
					if(bp.p[i]==0) {									// ��_�Z�o
						cpt.p[i] = box.bp[0].p[i];
					} else if(bp.p[i]==2) {
						cpt.p[i] = box.bp[1].p[i];
					} else {
						cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
					}
					wsc.p[i] = (sc.p[i]<0.01) ? 0.01 : sc.p[i];			// �{���v�Z
				}
			}
			CopyMove(dx, dy, dz, cpt, wsc);								// ���ʗ��̈ړ�
		} else {														// <�ړ��̏ꍇ>
			Move(dx, dy, dz);											// ���s�ړ�
		}
	} else {															// <2��ڈȍ~>
		RedoMove(dx, dy, dz);											// ���s�ړ��Ď��s
	}
}

/*********************************************************/
void EndMove(int cn, PNTTYPE sc, int sf,
             SCRTYPE bp, double dx, double dy, double dz)				//<<< ���s�ړ��I������
/*********************************************************/
{
	OBJTYPE* op;
	BOXTYPE  box;
	PNTTYPE  cpt, wsc;
	int      i;

	if(cn>1) {															// <2�ȏ㕡�ʂ̏ꍇ>
		for(i=0; i<DIM; i++) {
			wsc.p[i] = 0.0;
		}
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <�{���ύX�L>
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			for(i=0; i<DIM; i++) {
				wsc.p[i] = sc.p[i]-1.0;									// �{��������
				if(bp.p[i]==0) {										// ��_�Z�o
					cpt.p[i] = box.bp[0].p[i];
				} else if(bp.p[i]==2) {
					cpt.p[i] = box.bp[1].p[i];
				} else {
					cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
				}
			}
		}
		CopyMoveCopy(dx, dy, dz, cn-1, cpt, wsc, sf);					// ���̈ړ�����(2�ȍ~)
	}
	if(cn>0) {															// <���ʂ̏ꍇ>
		OffEdtAllObject();												// �ҏW���̑S����
		LevelBaseObjPtr(0, &op);										// �������̎擾
		while(op) {
			OnEdtFlg(1, op);											// edtflg:ON
			LevelNextObjPtr(0, &op);
		}
	}
}

/***********************************************/
void Rotate(PNTTYPE cp, VECTYPE vec, double th)							//<<< ��]�ړ�
/***********************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      dsp, edt, i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(ROTATE, 1, 2, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(th*1000.0));							// ��]�p�x�ݒ�
		PutParaPnt(cmp, 0, cp);											// ��]�����W�ݒ�
		PutParaPnt(cmp, 1, pnt);										// ��]�������޸�ِݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// ��]����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***********************************************************/
static void CopyRotate(PNTTYPE cp, VECTYPE vec, double th,
                       PNTTYPE mov, PNTTYPE bp, PNTTYPE sc)				//<<< ��]�ړ�(���̕���)
/***********************************************************/
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, dsp, edt;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		SetObjPtr(op);													// ���ʑΏۗ��̐ݒ�
		wop = CopyObject();												// ���̕���
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <�{���ύX�L�̏ꍇ>
			CreatCmd(SCALE, 3, 1, &cmp);								// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(sc.p[0]*1000.0));					// �e�����{���ݒ�
			PutParaInt(cmp, 1, (int)(sc.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(sc.p[2]*1000.0));
			PutParaPnt(cmp, 0, bp);										// �ϊ����S���W�ݒ�
			SetObjPtr(wop);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ���ޕύX����ގ��s
		}
		CreatCmd(ROTATE, 1, 2, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(th*1000.0));							// ��]�p�x�ݒ�
		PutParaPnt(cmp, 0, cp);											// ��]�����W�ݒ�
		PutParaPnt(cmp, 1, pnt);										// ��]�������޸�ِݒ�
		SetObjPtr(wop);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// ��]����ގ��s
		CreatCmd(MOVE, 3, 0, &cmp);										// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(mov.p[0]*1000.0));						// �ړ��ʐݒ�
		PutParaInt(cmp, 1, (int)(mov.p[1]*1000.0));
		PutParaInt(cmp, 2, (int)(mov.p[2]*1000.0));
		SetObjPtr(wop);													// �����߲���ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �ړ�����ގ��s
		View->SetDispList(wop);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*********************************************************/
static void GetThBase(PNTTYPE cp, PNTTYPE* pa, int plane)				//<<< �p�x�����_�擾
/*********************************************************/
{
	OBJTYPE* op;
	PNTTYPE  pnt, minp1, maxp1, minp2, maxp2;
	VECTYPE  vec;
	int      i, i1, i2, i3, dsp, edt, num;
	double   th, t1, t2, min1=100.0, max1=-100.0, min2=100.0, max2=-100.0;

	for(i=0; i<DIM; i++) {
		minp1.p[i] = 0.0, maxp1.p[i] = 0.0;
		minp2.p[i] = 0.0, maxp2.p[i] = 0.0;
	}
	i1 = plane;
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		num = GetVtxNum(op);											// ���_���擾
		for(i=0; i<num; i++) {
			GetVtx(op, i, &pnt);										// ���_�擾
			vec.p[i1] = pnt.p[i1]-cp.p[i1];								// ���S�_������޸��
			vec.p[i2] = pnt.p[i2]-cp.p[i2];
			vec.p[i3] = 0.0;
			VecN(&vec);													// �P���޸�ى�
			th = atan2(vec.p[i2], vec.p[i1]);							// i1������̊p�x
			th = (th<-10||th>10) ? 0.0 : th;
			th = (th<0) ? th+(2.0*PI) : th;
			if(min1>th) {												// i1�ŏ��ۑ�
				min1 = th, pnt.p[i3] = 0.0, minp1 = pnt;
			}
			if(max1<th) {												// i1�ő�ۑ�
				max1 = th, pnt.p[i3] = 0.0, maxp1 = pnt;
			}
			th = atan2(vec.p[i1], vec.p[i2]);							// i2������̊p�x
			th = (th<-10||th>10) ? 0.0 : th;
			th = (th<0) ? th+(2.0*PI) : th;
			if(min2>th) {												// i2�ŏ��ۑ�
				min2 = th, pnt.p[i3] = 0.0, minp2 = pnt;
			}
			if(max2<th) {												// i2�ő�ۑ�
				max2 = th, pnt.p[i3] = 0.0, maxp2 = pnt;
			}
		}
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	t1 = (max1-min1>PI) ? 2.0*PI-(max1-min1) : max1-min1;				// 180�x�����ϊ�
	t2 = (max2-min2>PI) ? 2.0*PI-(max2-min2) : max2-min2;
	if(t1>t2) {															// �p�x��̗p
		pa[0] = minp1, pa[1] = maxp1;
	} else {
		pa[0] = minp2, pa[1] = maxp2;
	}
}

/*************************************************************/
static double CalcTh(PNTTYPE pc, PNTTYPE pb,
                     PNTTYPE* pa, PNTTYPE sc, int i1, int i2)			//<<< ���ޕύX��p�x�Z�o
/*************************************************************/
{
	PNTTYPE pt[2];
	VECTYPE svc, rvc;
	int     i, j;
	double  th=0.0, wd;

	for(i=0; i<2; i++) {
		for(j=0; j<DIM; j++) {
			wd = pa[i].p[j]-pb.p[j];									// ��_->�Ώۓ_�޸��
			wd = wd*sc.p[j];											// �޸�ٔ{���ώZ
			pt[i].p[j] = wd+pb.p[j];									// ���ޕύX��Ώۓ_
		}
	}
	VecV(&pt[0], &pc, &svc);											// 1->2�_���޸�َ擾
	VecV(&pc, &pt[1], &rvc);											// 2->3�_���޸�َ擾
	VecN(&svc); VecN(&rvc);												// �P���޸�ى�
	if(fabs(svc.p[i1]-rvc.p[i1])>EPSILON||
       fabs(svc.p[i2]-rvc.p[i2])>EPSILON) {								// 2�޸�يԊp�x
		wd = acos(svc.p[i1]*rvc.p[i1]+svc.p[i2]*rvc.p[i2]);
		wd = (wd<-10||wd>10) ? 0.0 : wd;
		wd = PI - fabs(wd);
		th = (th<wd) ? wd : th;
	}
	return 180.0*th/PI;													// �p�x
}

/******************************************************************/
static void CopyRotateCopy(PNTTYPE cp, VECTYPE vec,
                           double th, int pl, PNTTYPE mov,
                           int cn, PNTTYPE bp, PNTTYPE sc, int sf)		//<<< ��]�ړ�����
/******************************************************************/	//   (���ʗ���)
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  pnt, smv, pc, pb, pa[2], ssc, isc, wsc;
	int      i, j, i1, i2, dsp, edt;
	double   ith, wth, it1, it2;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	if((fabs(sc.p[0])>EPSILON||
        fabs(sc.p[1])>EPSILON||
        fabs(sc.p[2])>EPSILON)&&sf!=0) {
		GetThBase(cp, pa, pl);											// �p�x�����_�擾
		i1 = pl;
		i2 = (pl+1>2) ? 0 : pl+1;
		for(i=0; i<DIM; i++) {
			ssc.p[i] = (1.0+sc.p[i]<0.01) ? 0.01 : 1.0+sc.p[i];			// �{���v�Z
			pc.p[i] = 0.0, pb.p[i] = 0.0;								// �ް�������
		}
		pc.p[i1] = cp.p[i1], pc.p[i2] = cp.p[i2];						// ��]���S�_
		pb.p[i1] = bp.p[i1], pb.p[i2] = bp.p[i2];						// ���ޕύX��_
		it1 = CalcTh(pc, pb, pa, ssc, i1, i2);							// 1�ڊp�x
	}
	smv = mov;															// ��ψʗ�
	isc = sc;															// �J�n�{��
	ith = th;															// ���]�p�x
	for(i=0; i<cn; i++) {												// ���ʐ����s��
		wth = th;
		if(fabs(sc.p[0])>EPSILON||
           fabs(sc.p[1])>EPSILON||
           fabs(sc.p[2])>EPSILON) {										// <�{���ύX�L�̏ꍇ>
			for(j=0; j<DIM; j++) {
				isc.p[j] += sc.p[j];									// �{�����Z
				wsc.p[j] = (1.0+isc.p[j]<0.01) ? 0.01 : 1.0+isc.p[j];	// �{���v�Z
			}
			if(sf==1) {													// <���Ԋu�A���̏ꍇ>
				it2 = CalcTh(pc, pb, pa, wsc, i1, i2);					// n+1�ڂ̊p�x
				it2 = it1-it2;											// 1�ڂ�n+1�ڂ̍�
				if(th>0) {
					wth = th-it2;
				} else if(th<0) {
					wth = th+it2;
				}
			} else if(sf==2) {											// <�s���Ԋu�A���̏ꍇ>
				it2 = CalcTh(pc, pb, pa, wsc, i1, i2);					// n+1�ڂ̊p�x
				wth = th*it2/it1;										// 1�ڂ�n+1�ڂ̔䗦
			}
		}
		ith += wth;														// ��]�p�x���Z
		for(j=0; j<DIM; j++) {
			smv.p[j] += mov.p[j];										// �ψʗʉ��Z
		}
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
		while(op!=NULL) {
			SetObjPtr(op);												// ���ʑΏۗ��̐ݒ�
			wop = CopyObject();											// ���̕���
			if(fabs(sc.p[0])>EPSILON||
               fabs(sc.p[1])>EPSILON||
               fabs(sc.p[2])>EPSILON) {									// <�{���ύX�L�̏ꍇ>
				CreatCmd(SCALE, 3, 1, &cmp);							// ����ޗ̈�m��
				PutParaInt(cmp, 0, (int)(wsc.p[0]*1000.0));				// �e�����{���ݒ�
				PutParaInt(cmp, 1, (int)(wsc.p[1]*1000.0));
				PutParaInt(cmp, 2, (int)(wsc.p[2]*1000.0));
				PutParaPnt(cmp, 0, bp);									// �ϊ����S���W�ݒ�
				SetObjPtr(wop);											// �Ώۗ��̐ݒ�
				SetCmdPtr(cmp);											// ����ސݒ�
				ExecCommand(FALSE);										// ���ޕύX����ގ��s
			}
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(ith*1000.0));						// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, cp);										// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pnt);									// ��]�������޸�ِݒ�
			SetObjPtr(wop);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
			CreatCmd(MOVE, 3, 0, &cmp);									// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(smv.p[0]*1000.0));					// �ړ��ʐݒ�
			PutParaInt(cmp, 1, (int)(smv.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(smv.p[2]*1000.0));
			SetObjPtr(wop);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// �ړ�����ގ��s
			View->SetDispList(wop);										// �ި���ڲ�ؽēo�^
			NextObjPtr(2, &op, &dsp, &edt);								// �����̂�
		}
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/**********************************************************/
static void RedoRotate(PNTTYPE cp, VECTYPE vec, double th)				//<<< ��]�ړ�(�Ď��s)
/**********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==ROTATE) {										// <��]����ނ̏ꍇ>
			PutParaInt(cmp, 0, (int)(th*1000.0));						// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, cp);										// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pnt);									// ��]�������޸�ِݒ�
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/***************************************************************/
void ObjRotate(OBJTYPE* op, PNTTYPE cp, VECTYPE vec, double th)			//<<< �w�藧�̂̉�]
/***************************************************************/
{
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i;

	for(i=0; i<DIM; i++) {
		pnt.p[i] = vec.p[i];
	}
	if(op!=NULL) {
		CreatCmd(ROTATE, 1, 2, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(th*1000.0));							// ��]�p�x�ݒ�
		PutParaPnt(cmp, 0, cp);											// ��]�����W�ݒ�
		PutParaPnt(cmp, 1, pnt);										// ��]�������޸�ِݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// ��]����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/***********************************************************/
void ObjectRotate(int flg, int cn, PNTTYPE sc, SCRTYPE bp,
                  PNTTYPE cp, int pl, double th, double mv)				//<<< ���̉�]�ړ�
/***********************************************************/
{
	VECTYPE vec;
	BOXTYPE box;
	PNTTYPE cpt, mov, wsc;
	int     i;

	for(i=0; i<DIM; i++) {
		vec.p[i] = 0.0, mov.p[i] = 0.0, wsc.p[i] = 1.0;
	}
	i = (pl-1<0) ? 2 : pl-1;
	vec.p[i] = -1.0;													// ��]�������޸�َZ�o
	mov.p[i] = mv;														// �ψʗʐݒ�
	if(flg==0) {														// <����̂�>
		InitLevel();													// Undo/Redo������
		if(cn>0) {														// <�ړ����ʂ̏ꍇ>
			if(fabs(sc.p[0]-1.0)>EPSILON||
               fabs(sc.p[1]-1.0)>EPSILON||
               fabs(sc.p[2]-1.0)>EPSILON) {								// <�{���ύX�L>
				GetAreaBox(2, &box);									// �ޯ�����ގ擾
				for(i=0; i<DIM; i++) {
					if(bp.p[i]==0) {									// ��_�Z�o
						cpt.p[i] = box.bp[0].p[i];
					} else if(bp.p[i]==2) {
						cpt.p[i] = box.bp[1].p[i];
					} else {
						cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
					}
					wsc.p[i] = (sc.p[i]<0.01) ? 0.01 : sc.p[i];			// �{���v�Z
				}
			}
			CopyRotate(cp, vec, th, mov, cpt, wsc);						// ���ʗ��̉�]
		} else {														// <��]�̏ꍇ>
			Rotate(cp, vec, th);										// ��]�ړ�
			Move(mov.p[0], mov.p[1], mov.p[2]);							// ���s�ړ�
		}
	} else {															// <2��ڈȍ~>
		RedoRotate(cp, vec, th);										// ��]�ړ��Ď��s
		RedoMove(mov.p[0], mov.p[1], mov.p[2]);							// ���s�ړ��Ď��s
	}
}

/********************************************************/
void EndRotate(int cn, PNTTYPE sc, int sf, SCRTYPE bp,
               PNTTYPE cp, int pl, double th, double mv)				//<<< ��]�ړ��I������
/********************************************************/
{
	OBJTYPE* op;
	VECTYPE  vec;
	PNTTYPE  mov, cpt, wsc;
	BOXTYPE  box;
	int      i;

	for(i=0; i<DIM; i++) {
		vec.p[i] = 0.0, mov.p[i] = 0.0, wsc.p[i] = 0.0;
	}
	i = (pl-1<0) ? 2 : pl-1;
	vec.p[i] = -1.0;													// ��]�������޸�َZ�o
	mov.p[i] = mv;														// �ψʗʐݒ�
	if(cn>1) {															// <2�ȏ㕡�ʂ̏ꍇ>
		if(fabs(sc.p[0]-1.0)>EPSILON||
           fabs(sc.p[1]-1.0)>EPSILON||
           fabs(sc.p[2]-1.0)>EPSILON) {									// <�{���ύX�L>
			GetAreaBox(2, &box);										// �ޯ�����ގ擾
			for(i=0; i<DIM; i++) {
				wsc.p[i] = sc.p[i]-1.0;									// �{��������
				if(bp.p[i]==0) {										// ��_�Z�o
					cpt.p[i] = box.bp[0].p[i];
				} else if(bp.p[i]==2) {
					cpt.p[i] = box.bp[1].p[i];
				} else {
					cpt.p[i] = (box.bp[0].p[i]+box.bp[1].p[i])/2.0;
				}
			}
		}
		CopyRotateCopy(cp, vec, th, pl, mov, cn-1, cpt, wsc, sf);		// ���̉�]����(2�ȍ~)
	}
	if(cn>0) {															// <���ʂ̏ꍇ>
		OffEdtAllObject();												// �ҏW���̑S����
		LevelBaseObjPtr(0, &op);										// �������̎擾
		while(op) {
			OnEdtFlg(1, op);											// edtflg:ON
			LevelNextObjPtr(0, &op);
		}
	}
}

/*******************************************************/
static void Mirror(PNTTYPE spt, PNTTYPE ept, int plane)					//<<< �����ړ�
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(MIRROR, 1, 2, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, plane);										// ���Ұ��ݒ�
		PutParaPnt(cmp, 0, spt);
		PutParaPnt(cmp, 1, ept);
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// ��������ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***********************************************************/
static void RedoMirror(PNTTYPE spt, PNTTYPE ept, int plane)				//<<< �����ړ�(�Ď��s)
/***********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==MIRROR) {										// <��������ނ̏ꍇ>
			PutParaInt(cmp, 0, plane);									// ���Ұ��ݒ�
			PutParaPnt(cmp, 0, spt);
			PutParaPnt(cmp, 1, ept);
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/******************************************************************/
void ObjectMirror(int flg, int cf, PNTTYPE sp, PNTTYPE ep, int pl)		// ���̋����ړ�
/******************************************************************/
{
	OBJTYPE* op;

	if(flg==0) {														// <����̂�>
		InitLevel();													// Undo/Redo������
		if(cf!=0) {														// <���ʂ̏ꍇ>
			Copy();														// ���̕���
			OffEdtAllObject();											// �ҏW���̑S����
			LevelBaseObjPtr(0, &op);									// �������̎擾
			while(op) {
				OnEdtFlg(1, op);										// edtflg:ON
				LevelNextObjPtr(0, &op);
			}
		}
		Mirror(sp, ep, pl);												// �����ړ�
	} else {															// <2��ڈȍ~>
		RedoMirror(sp, ep, pl);											// �����ړ��Ď��s
	}
}

/*******************************************************/
void Scale(PNTTYPE cp, double sx, double sy, double sz)					//<<< ���ޕύX
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(SCALE, 3, 1, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(sx*1000.0));							// �e�����{���ݒ�
		PutParaInt(cmp, 1, (int)(sy*1000.0));
		PutParaInt(cmp, 2, (int)(sz*1000.0));
		PutParaPnt(cmp, 0, cp);											// �ϊ����S���W�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// ���ޕύX����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/******************************************************************/
static void RedoScale(PNTTYPE cp, double sx, double sy, double sz)		//<<< ���ޕύX(�Ď��s)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SCALE) {										// <���ޕύX����ނ̏ꍇ>
			PutParaInt(cmp, 0, (int)(sx*1000.0));						// �e�����{���ݒ�
			PutParaInt(cmp, 1, (int)(sy*1000.0));
			PutParaInt(cmp, 2, (int)(sz*1000.0));
			PutParaPnt(cmp, 0, cp);										// �ϊ����S���W�ݒ�
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/**********************************************************************/
void ObjectScale(int flg, PNTTYPE cp, double sx, double sy, double sz)	//<<< ���̻��ޕύX
/**********************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Scale(cp, sx, sy, sz);											// ���ޕύX
	} else {															// <2��ڈȍ~>
		RedoScale(cp, sx, sy, sz);										// ���ޕύX�Ď��s
	}
}

/***********************************************************/
void Resize1(int pln, int mpf, double s1, double s2,
             double s3, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)				//<<< �����w��ػ���1
/***********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(RESIZE1, 5, 3, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, pln);										// �w�蕽�ʐݒ�
		PutParaInt(cmp, 1, mpf);										// �����׸ސݒ�
		PutParaInt(cmp, 2, (int)(s1*1000.0));							// �{��1�ݒ�
		PutParaInt(cmp, 3, (int)(s2*1000.0));							// �{��2�ݒ�
		PutParaInt(cmp, 4, (int)(s3*1000.0));							// �{��3�ݒ�
		PutParaPnt(cmp, 0, p1);											// ��_1�ݒ�
		PutParaPnt(cmp, 1, p2);											// ��_2�ݒ�
		PutParaPnt(cmp, 2, p3);											// ��_3�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �����w��ػ��޺���ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/**********************************************************************/
static void RedoResize1(int pln, int mpf, double s1, double s2,
                        double s3, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)	//<<< �����w��ػ���1(�Ď��s)
/**********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE1) {									// <�����w��ػ���1�̏ꍇ>
			PutParaInt(cmp, 0, pln);									// �w�蕽�ʐݒ�
			PutParaInt(cmp, 1, mpf);									// �����׸ސݒ�
			PutParaInt(cmp, 2, (int)(s1*1000.0));						// �{��1�ݒ�
			PutParaInt(cmp, 3, (int)(s2*1000.0));						// �{��2�ݒ�
			PutParaInt(cmp, 4, (int)(s3*1000.0));						// �{��3�ݒ�
			PutParaPnt(cmp, 0, p1);										// ��_1�ݒ�
			PutParaPnt(cmp, 1, p2);										// ��_2�ݒ�
			PutParaPnt(cmp, 2, p3);										// ��_3�ݒ�
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/******************************************************/
void ObjectResize1(int flg, int pln, int mpf,
                   double s1, double s2, double s3,
                   PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)					//<<< ���̒����w��ػ���1
/******************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Resize1(pln, mpf, s1, s2, s3, p1, p2, p3);						// �����w��ػ���1
	} else {															// <2��ڈȍ~>
		RedoResize1(pln, mpf, s1, s2, s3, p1, p2, p3);					// �����w��ػ���1�Ď��s
	}
}

/*****************************************************************/
void Resize2(int f1, int f2, int d1, int d2,
             double ln, PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)		//<<< �Ȑ��w��ػ���1
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(RESIZE2, 6+pn, 1+pn, &cmp);							// ����ޗ̈�m��
		PutParaInt(cmp, 0, f1);											// �����׸ސݒ�
		PutParaInt(cmp, 1, f2);											// �����׸ސݒ�
		PutParaInt(cmp, 2, d1);											// ���������ݒ�
		PutParaInt(cmp, 3, d2);											// ���ގw������ݒ�
		PutParaInt(cmp, 4, (int)(ln*1000.0));							// ������ݒ�
		PutParaInt(cmp, 5, pn);											// ����_���ݒ�
		for(i=0; i<pn; i++) {
			PutParaInt(cmp, i+6, ia[i]);								// �Ȑ�Ӱ�ސݒ�
		}
		PutParaPnt(cmp, 0, bs);											// ��_�ݒ�
		for(i=0; i<pn; i++) {
			PutParaPnt(cmp, i+1, pa[i]);								// ����_�ݒ�
		}
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �Ȑ��w��ػ��޺���ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/******************************************************************/
static void RedoResize2(int f1, int f2, int d1, int d2, double ln,
                        PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)		//<<< �Ȑ��w��ػ���1(�Ď��s)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE2) {									// <<�Ȑ��w��ػ���1�̏ꍇ>>
			spn = GetParaInt(cmp, 5);									// ����_���擾
			if(pn!=spn) {												// <����_�����قȂ�ꍇ>
				ChangeCmd(cmp, 6+pn, 1+pn);								// ����ޗ̈�ύX
			}
			PutParaInt(cmp, 0, f1);										// �����׸ސݒ�
			PutParaInt(cmp, 1, f2);										// �����׸ސݒ�
			PutParaInt(cmp, 2, d1);										// ���������ݒ�
			PutParaInt(cmp, 3, d2);										// ���ގw������ݒ�
			PutParaInt(cmp, 4, (int)(ln*1000.0));						// ������ݒ�
			PutParaInt(cmp, 5, pn);										// ����_���ݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, i+6, ia[i]);							// �Ȑ�Ӱ�ސݒ�
			}
			PutParaPnt(cmp, 0, bs);										// ��_�ݒ�
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i+1, pa[i]);							// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(pn!=spn) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/***********************************************************************/
void ObjectResize2(int flg, int f1, int f2, int d1, int d2,
                   double ln, PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)	//<<< ���̋Ȑ��w��ػ���1
/***********************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Resize2(f1, f2, d1, d2, ln, bs, pn, pa, ia);					// �Ȑ��w��ػ���1
	} else {															// <2��ڈȍ~>
		RedoResize2(f1, f2, d1, d2, ln, bs, pn, pa, ia);				// �Ȑ��w��ػ���1�Ď��s
	}
}

/************************************************************/
void Bend(int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)			//<<< �~���Ȃ�
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(BEND, 3, 2, &cmp);										// ����ޗ̈�m��
		PutParaInt(cmp, 0, fc);											// �Ȃ��鐅�������ݒ�
		PutParaInt(cmp, 1, vc);											// �Ȃ��鐂�������ݒ�
		PutParaInt(cmp, 2, (int)(th*1000.0));							// �Ȃ���p�x�ݒ�
		PutParaPnt(cmp, 0, sp);											// ����n�_�ݒ�
		PutParaPnt(cmp, 1, ep);											// ����I�_�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �~���Ȃ�����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***********************************************************************/
static void RedoBend(int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)	//<<< �~���Ȃ�(�Ď��s)
/***********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==BEND) {										// <�~���Ȃ�����ނ̏ꍇ>
			PutParaInt(cmp, 0, fc);										// �Ȃ��鐅�������ݒ�
			PutParaInt(cmp, 1, vc);										// �Ȃ��鐂�������ݒ�
			PutParaInt(cmp, 2, (int)(th*1000.0));						// �Ȃ���p�x�ݒ�
			PutParaPnt(cmp, 0, sp);										// ����n�_�ݒ�
			PutParaPnt(cmp, 1, ep);										// ����I�_�ݒ�
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/**************************************************/
void ObjectBend(int flg, int fc, int vc,
                double th, PNTTYPE sp, PNTTYPE ep)						//<<< ���̉~���Ȃ�
/**************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Bend(fc, vc, th, sp, ep);										// �~���Ȃ�
	} else {															// <2��ڈȍ~>
		RedoBend(fc, vc, th, sp, ep);									// �~���Ȃ��Ď��s
	}
}

/****************************************************/
void Twist(int dr, double th, double ln, PNTTYPE cp)					//<<< �Ђ˂�
/****************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(TWIST, 3, 1, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, dr);											// �Ђ˂莲�����ݒ�
		PutParaInt(cmp, 1, (int)(th*1000.0));							// �Ђ˂�p�x�ݒ�
		PutParaInt(cmp, 2, (int)(ln*1000.0));							// �Ђ˂莲���ݒ�
		PutParaPnt(cmp, 0, cp);											// �Ђ˂��_�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �Ђ˂����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***************************************************************/
static void RedoTwist(int dr, double th, double ln, PNTTYPE cp)			//<<< �Ђ˂�(�Ď��s)
/***************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==TWIST) {										// <�Ђ˂����ނ̏ꍇ>
			PutParaInt(cmp, 0, dr);										// �Ђ˂莲�����ݒ�
			PutParaInt(cmp, 1, (int)(th*1000.0));						// �Ђ˂�p�x�ݒ�
			PutParaInt(cmp, 2, (int)(ln*1000.0));						// �Ђ˂莲���ݒ�
			PutParaPnt(cmp, 0, cp);										// �Ђ˂��_�ݒ�
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/*******************************************************************/
void ObjectTwist(int flg, int dr, double th, double ln, PNTTYPE cp)		//<<< ���̂Ђ˂�
/*******************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Twist(dr, th, ln, cp);											// �Ђ˂�
	} else {															// <2��ڈȍ~>
		RedoTwist(dr, th, ln, cp);										// �Ђ˂�Ď��s
	}
}

/*****************************************************/
void Shear(int dr1, int dr2, double inc, PNTTYPE bas)					//<<< ����f
/*****************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(SHEAR, 3, 1, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, dr1);										// ����f���������ݒ�
		PutParaInt(cmp, 1, dr2);										// ����f���������ݒ�
		PutParaInt(cmp, 2, (int)(inc*1000.0));							// ����f�X���ݒ�
		PutParaPnt(cmp, 0, bas);										// ����f��_�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// ����f����ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/****************************************************************/
static void RedoShear(int dr1, int dr2, double inc, PNTTYPE bas)		//<<< ����f(�Ď��s)
/****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SHEAR) {										// <����f����ނ̏ꍇ>
			PutParaInt(cmp, 0, dr1);									// ����f���������ݒ�
			PutParaInt(cmp, 1, dr2);									// ����f���������ݒ�
			PutParaInt(cmp, 2, (int)(inc*1000.0));						// ����f�X���ݒ�
			PutParaPnt(cmp, 0, bas);									// ����f��_�ݒ�
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
    }
}

/********************************************************************/
void ObjectShear(int flg, int dr1, int dr2, double inc, PNTTYPE bas)	//<<< ���̂���f
/********************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Shear(dr1, dr2, inc, bas);										// ����f
	} else {															// <2��ڈȍ~>
		RedoShear(dr1, dr2, inc, bas);									// ����f�Ď��s
	}
}

/************************************************************/
void Ring(PNTTYPE cp, double sx, double sy, double sz,
          int fc, int vc, double th, PNTTYPE sp, PNTTYPE ep)			//<<< �ݸ޻��ލ��킹
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(RING, 6, 3, &cmp);										// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(sx*1000.0));							// �e�����{���ݒ�
		PutParaInt(cmp, 1, (int)(sy*1000.0));
		PutParaInt(cmp, 2, (int)(sz*1000.0));
		PutParaInt(cmp, 3, fc);											// �Ȃ��鐅�������ݒ�
		PutParaInt(cmp, 4, vc);											// �Ȃ��鐂�������ݒ�
		PutParaInt(cmp, 5, (int)(th*1000.0));							// �Ȃ���p�x�ݒ�
		PutParaPnt(cmp, 0, cp);											// �ϊ����S���W�ݒ�
		PutParaPnt(cmp, 1, sp);											// ����n�_�ݒ�
		PutParaPnt(cmp, 2, ep);											// ����I�_�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(TRUE);												// ����ގ��s
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/******************************************************/
void Earth(int rc, int sc, int sp, double dx,
           double dy, double dz, double tx, double ty,
           double tz, double sx, double sy, double sz)					//<<< ���
/******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {													// ���������ޯ�����ގ擾��
		RedoCreateCommand(op);											// ��������ލĎ��s
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	GetAreaBox(2, &ebx);												// �ر�ޯ���擾(�����Ώۗ���)
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		GetObjBox(op, &obx);											// �����ޯ���擾
		GetUnitBox(op, &ubx);											// �Ưė����ޯ���擾
		box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);					// ����/�Ư�/�ر
		BoxCenter(box, &rcp);											// BOX���S�擾
		box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);					// ����/�Ư�/�ر
		BoxCenter(box, &scp);											// BOX���S�擾
		scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];					// Z�����ʒu�ݒ�
		scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
		CreatCmd(EARTH, 9, 2, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// �ړ��ʐݒ�
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		PutParaInt(cmp, 3, (int)(tx*1000.0));							// ��]�p�x�ݒ�
		PutParaInt(cmp, 4, (int)(ty*1000.0));
		PutParaInt(cmp, 5, (int)(tz*1000.0));
		PutParaInt(cmp, 6, (int)(sx*1000.0));							// �e�����{���ݒ�
		PutParaInt(cmp, 7, (int)(sy*1000.0));
		PutParaInt(cmp, 8, (int)(sz*1000.0));
		PutParaPnt(cmp, 0, rcp);										// ��]�����W�ݒ�
		PutParaPnt(cmp, 1, scp);										// �ϊ����S���W�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(TRUE);												// ����ގ��s
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*****************************************************************/
static void RedoEarth(int rc, int sc, int sp, int ch, double dx,
                      double dy, double dz, double tx, double ty,
                      double tz, double sx, double sy, double sz)		//<<< ���(�Ď��s)
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	if(ch) {															// <���S�_�ύX�L�̏ꍇ>
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
		while(op!=NULL) {												// ���������ޯ�����ގ擾��
			RedoCreateCommand(op);										// ��������ލĎ��s
			NextObjPtr(2, &op, &dsp, &edt);								// �����̂�
		}
		GetAreaBox(2, &ebx);											// �ر�ޯ���擾(�����Ώۗ���)
		LevelBaseCmdPtr(&cmp);											// �����Ώۺ���ގ擾
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==EARTH) {									// <�������ނ̏ꍇ>
				GetCmdObj(cmp, &op);									// �ҏW�Ώۗ��̎擾
				GetObjBox(op, &obx);									// �����ޯ���擾
				GetUnitBox(op, &ubx);									// �Ưė����ޯ���擾
				box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);			// ����/�Ư�/�ر
				BoxCenter(box, &rcp);									// BOX���S�擾
				box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);			// ����/�Ư�/�ر
				BoxCenter(box, &scp);									// BOX���S�擾
				scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];			// Z�����ʒu�ݒ�
				scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// �ړ��ʐݒ�
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// ��]�p�x�ݒ�
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// �e�����{���ݒ�
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				PutParaPnt(cmp, 0, rcp);								// ��]�����W�ݒ�
				PutParaPnt(cmp, 1, scp);								// �ϊ����S���W�ݒ�
			}
			LevelNextCmdPtr(&cmp);										// ������ނ�
		}
		LevelBaseCmdPtr(&cmp);											// �����Ώۺ���ގ擾
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==EARTH) {									// <�������ނ̏ꍇ>
				GetCmdObj(cmp, &op);									// �ҏW�Ώۗ��̎擾
				RedoCommand(op);										// ����ލĎ��s
				View->SetDispList(op);									// �ި���ڲ�ؽēo�^
			}
			LevelNextCmdPtr(&cmp);										// ������ނ�
		}
	} else {															// <���S�_�ύX���̏ꍇ>
		LevelBaseCmdPtr(&cmp);											// �����Ώۺ���ގ擾
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==EARTH) {									// <�������ނ̏ꍇ>
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// �ړ��ʐݒ�
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// ��]�p�x�ݒ�
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// �e�����{���ݒ�
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				GetCmdObj(cmp, &op);									// �ҏW�Ώۗ��̎擾
				RedoCommand(op);										// ����ލĎ��s
				View->SetDispList(op);									// �ި���ڲ�ؽēo�^
			}
			LevelNextCmdPtr(&cmp);										// ������ނ�
		}
	}
}

/********************************************************************/
void ObjectEarth(int flg, int rc, int sc, int sp, int ch, double dx,
                 double dy, double dz, double tx, double ty,
                 double tz, double sx, double sy, double sz)			//<<< ���̱��
/********************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Earth(rc, sc, sp, dx, dy, dz, tx, ty, tz, sx, sy, sz);			// ���
	} else {															// <2��ڈȍ~>
		RedoEarth(rc, sc, sp, ch, dx, dy, dz, tx, ty, tz, sx, sy, sz);	// ����Ď��s
	}
}

/******************************************************/
void Space(int rc, int sc, int sp, double dx,
           double dy, double dz, double tx, double ty,
           double tz, double sx, double sy, double sz)					//<<< ��߰�
/******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	GetAreaBox(2, &ebx);												// �ر�ޯ���擾(�����Ώۗ���)
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		GetObjBox(op, &obx);											// �����ޯ���擾
		GetUnitBox(op, &ubx);											// �Ưė����ޯ���擾
		box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);					// ����/�Ư�/�ر
		BoxCenter(box, &rcp);											// BOX���S�擾
		box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);					// ����/�Ư�/�ر
		BoxCenter(box, &scp);											// BOX���S�擾
		scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];					// Z�����ʒu�ݒ�
		scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
		CreatCmd(SPACE, 9, 2, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, (int)(dx*1000.0));							// �ړ��ʐݒ�
		PutParaInt(cmp, 1, (int)(dy*1000.0));
		PutParaInt(cmp, 2, (int)(dz*1000.0));
		PutParaInt(cmp, 3, (int)(tx*1000.0));							// ��]�p�x�ݒ�
		PutParaInt(cmp, 4, (int)(ty*1000.0));
		PutParaInt(cmp, 5, (int)(tz*1000.0));
		PutParaInt(cmp, 6, (int)(sx*1000.0));							// �e�����{���ݒ�
		PutParaInt(cmp, 7, (int)(sy*1000.0));
		PutParaInt(cmp, 8, (int)(sz*1000.0));
		PutParaPnt(cmp, 0, rcp);										// ��]�����W�ݒ�
		PutParaPnt(cmp, 1, scp);										// �ϊ����S���W�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(TRUE);												// ����ގ��s
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*****************************************************************/
static void RedoSpace(int rc, int sc, int sp, int ch, double dx,
                      double dy, double dz, double tx, double ty,
                      double tz, double sx, double sy, double sz)		//<<< ��߰�(�Ď��s)
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  rcp, scp;
	BOXTYPE  box, obx, ubx, ebx;
	int      dsp, edt;

	if(ch) {															// <���S�_�ύX�L�̏ꍇ>
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
		while(op!=NULL) {												// ���������ޯ�����ގ擾��
			RedoStartCommand(op);										// �ƭ��J�n������ލĎ��s
			NextObjPtr(2, &op, &dsp, &edt);								// �����̂�
		}
		GetAreaBox(2, &ebx);											// �ر�ޯ���擾(�����Ώۗ���)
		LevelBaseCmdPtr(&cmp);											// �����Ώۺ���ގ擾
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==SPACE) {									// <��߰�����ނ̏ꍇ>
				GetCmdObj(cmp, &op);									// �ҏW�Ώۗ��̎擾
				GetObjBox(op, &obx);									// �����ޯ���擾
				GetUnitBox(op, &ubx);									// �Ưė����ޯ���擾
				box = (rc==0) ? obx : ((rc==1) ? ubx : ebx);			// ����/�Ư�/�ر
				BoxCenter(box, &rcp);									// BOX���S�擾
				box = (sc==0) ? obx : ((sc==1) ? ubx : ebx);			// ����/�Ư�/�ر
				BoxCenter(box, &scp);									// BOX���S�擾
				scp.p[2] = (sp==0) ? box.bp[0].p[2] : scp.p[2];			// Z�����ʒu�ݒ�
				scp.p[2] = (sp==2) ? box.bp[1].p[2] : scp.p[2];
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// �ړ��ʐݒ�
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// ��]�p�x�ݒ�
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// �e�����{���ݒ�
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				PutParaPnt(cmp, 0, rcp);								// ��]�����W�ݒ�
				PutParaPnt(cmp, 1, scp);								// �ϊ����S���W�ݒ�
			}
			LevelNextCmdPtr(&cmp);										// ������ނ�
		}
		LevelBaseCmdPtr(&cmp);											// �����Ώۺ���ގ擾
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==SPACE) {									// <��߰�����ނ̏ꍇ>
				GetCmdObj(cmp, &op);									// �ҏW�Ώۗ��̎擾
				RedoCommand(op);										// ����ލĎ��s
				View->SetDispList(op);									// �ި���ڲ�ؽēo�^
			}
			LevelNextCmdPtr(&cmp);										// ������ނ�
		}
	} else {															// <���S�_�ύX���̏ꍇ>
		LevelBaseCmdPtr(&cmp);											// �����Ώۺ���ގ擾
		while(cmp!=NULL) {
			if(GetCmdNo(cmp)==SPACE) {									// <��߰�����ނ̏ꍇ>
				PutParaInt(cmp, 0, (int)(dx*1000.0));					// �ړ��ʐݒ�
				PutParaInt(cmp, 1, (int)(dy*1000.0));
				PutParaInt(cmp, 2, (int)(dz*1000.0));
				PutParaInt(cmp, 3, (int)(tx*1000.0));					// ��]�p�x�ݒ�
				PutParaInt(cmp, 4, (int)(ty*1000.0));
				PutParaInt(cmp, 5, (int)(tz*1000.0));
				PutParaInt(cmp, 6, (int)(sx*1000.0));					// �e�����{���ݒ�
				PutParaInt(cmp, 7, (int)(sy*1000.0));
				PutParaInt(cmp, 8, (int)(sz*1000.0));
				GetCmdObj(cmp, &op);									// �ҏW�Ώۗ��̎擾
				RedoCommand(op);										// ����ލĎ��s
				View->SetDispList(op);									// �ި���ڲ�ؽēo�^
			}
			LevelNextCmdPtr(&cmp);										// ������ނ�
		}
	}
}

/********************************************************************/
void ObjectSpace(int flg, int rc, int sc, int sp, int ch, double dx,
                 double dy, double dz, double tx, double ty,
                 double tz, double sx, double sy, double sz)			//<<< ���̽�߰�
/********************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Space(rc, sc, sp, dx, dy, dz, tx, ty, tz, sx, sy, sz);			// ��߰�
	} else {															// <2��ڈȍ~>
		RedoSpace(rc, sc, sp, ch, dx, dy, dz, tx, ty, tz, sx, sy, sz);	// ��߰��Ď��s
	}
}

/***************************************************************/
void Resize3(int pln, int mpf, double s1, double s2, double s3,
             int vflg, double v1, double v2,
             PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)						//<<< �����w��ػ���2
/***************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(RESIZE3, 8, 3, &cmp);									// ����ޗ̈�m��
		PutParaInt(cmp, 0, pln);										// �w�蕽�ʐݒ�
		PutParaInt(cmp, 1, mpf);										// �����׸ސݒ�
		PutParaInt(cmp, 2, (int)(s1*1000.0));							// �{��1�ݒ�
		PutParaInt(cmp, 3, (int)(s2*1000.0));							// �{��2�ݒ�
		PutParaInt(cmp, 4, (int)(s3*1000.0));							// �{��3�ݒ�
		PutParaInt(cmp, 5, vflg);										// ���������׸ސݒ�
		PutParaInt(cmp, 6, (int)(v1*1000.0));							// �����͈�1�ݒ�
		PutParaInt(cmp, 7, (int)(v2*1000.0));							// �����͈�2�ݒ�
		PutParaPnt(cmp, 0, p1);											// ��_1�ݒ�
		PutParaPnt(cmp, 1, p2);											// ��_2�ݒ�
		PutParaPnt(cmp, 2, p3);											// ��_3�ݒ�
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �����w��ػ��޺���ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/******************************************************************/
static void RedoResize3(int pln, int mpf, double s1, double s2,
                        double s3, int vflg, double v1, double v2,
                        PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)				//<<< �����w��ػ���2(�Ď��s)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE3) {									// <�����w��ػ���2�̏ꍇ>
			PutParaInt(cmp, 0, pln);									// �w�蕽�ʐݒ�
			PutParaInt(cmp, 1, mpf);									// �����׸ސݒ�
			PutParaInt(cmp, 2, (int)(s1*1000.0));						// �{��1�ݒ�
			PutParaInt(cmp, 3, (int)(s2*1000.0));						// �{��2�ݒ�
			PutParaInt(cmp, 4, (int)(s3*1000.0));						// �{��3�ݒ�
			PutParaInt(cmp, 5, vflg);									// ���������׸ސݒ�
			PutParaInt(cmp, 6, (int)(v1*1000.0));						// �����͈�1�ݒ�
			PutParaInt(cmp, 7, (int)(v2*1000.0));						// �����͈�2�ݒ�
			PutParaPnt(cmp, 0, p1);										// ��_1�ݒ�
			PutParaPnt(cmp, 1, p2);										// ��_2�ݒ�
			PutParaPnt(cmp, 2, p3);										// ��_3�ݒ�
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/*****************************************************************/
void ObjectResize3(int flg, int pln, int mpf, double s1,
                   double s2, double s3, int vflg, double v1,
                   double v2, PNTTYPE p1, PNTTYPE p2, PNTTYPE p3)		//<<< ���̒����w��ػ���2
/*****************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Resize3(pln, mpf, s1, s2, s3, vflg, v1, v2, p1, p2, p3);		// �����w��ػ���2
	} else {															// <2��ڈȍ~>
		RedoResize3(pln, mpf, s1, s2, s3, vflg, v1, v2, p1, p2, p3);	// �����w��ػ���2�Ď��s
	}
}

/*******************************************************/
void Resize4(int f1, int f2, int d1, int d2, int d3,
             double ln, int vflg, double v1, double v2,
             PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)					//<<< �Ȑ��w��ػ���2
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		CreatCmd(RESIZE4, 10+pn, 1+pn, &cmp);							// ����ޗ̈�m��
		PutParaInt(cmp, 0, f1);											// �����׸ސݒ�
		PutParaInt(cmp, 1, f2);											// �����׸ސݒ�
		PutParaInt(cmp, 2, d1);											// ���������ݒ�
		PutParaInt(cmp, 3, d2);											// ���ގw������ݒ�
		PutParaInt(cmp, 4, d3);											// ���������ݒ�
		PutParaInt(cmp, 5, (int)(ln*1000.0));							// ������ݒ�
		PutParaInt(cmp, 6, vflg);										// ���������׸ސݒ�
		PutParaInt(cmp, 7, (int)(v1*1000.0));							// �����͈�1�ݒ�
		PutParaInt(cmp, 8, (int)(v2*1000.0));							// �����͈�2�ݒ�
		PutParaInt(cmp, 9, pn);											// ����_���ݒ�
		for(i=0; i<pn; i++) {
			PutParaInt(cmp, i+10, ia[i]);								// �Ȑ�Ӱ�ސݒ�
		}
		PutParaPnt(cmp, 0, bs);											// ��_�ݒ�
		for(i=0; i<pn; i++) {
			PutParaPnt(cmp, i+1, pa[i]);								// ����_�ݒ�
		}
		SetObjPtr(op);													// �Ώۗ��̐ݒ�
		SetCmdPtr(cmp);													// ����ސݒ�
		ExecCommand(FALSE);												// �Ȑ��w��ػ��޺���ގ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/******************************************************************/
static void RedoResize4(int f1, int f2, int d1, int d2, int d3,
                        double ln, int vflg, double v1, double v2,
                        PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)		//<<< �Ȑ��w��ػ���2(�Ď��s)
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==RESIZE4) {									// <<�Ȑ��w��ػ��ނ̏ꍇ>>
			spn = GetParaInt(cmp, 9);									// ����_���擾
			if(pn!=spn) {												// <����_�����قȂ�ꍇ>
				ChangeCmd(cmp, 10+pn, 1+pn);								// ����ޗ̈�ύX
			}
			PutParaInt(cmp, 0, f1);										// �����׸ސݒ�
			PutParaInt(cmp, 1, f2);										// �����׸ސݒ�
			PutParaInt(cmp, 2, d1);										// ���������ݒ�
			PutParaInt(cmp, 3, d2);										// ���ގw������ݒ�
			PutParaInt(cmp, 4, d3);										// ���������ݒ�
			PutParaInt(cmp, 5, (int)(ln*1000.0));						// ������ݒ�
			PutParaInt(cmp, 6, vflg);									// ���������׸ސݒ�
			PutParaInt(cmp, 7, (int)(v1*1000.0));						// �����͈�1�ݒ�
			PutParaInt(cmp, 8, (int)(v2*1000.0));						// �����͈�2�ݒ�
			PutParaInt(cmp, 9, pn);										// ����_���ݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, i+10, ia[i]);							// �Ȑ�Ӱ�ސݒ�
			}
			PutParaPnt(cmp, 0, bs);										// ��_�ݒ�
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i+1, pa[i]);							// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(pn!=spn) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/*******************************************************************/
void ObjectResize4(int flg, int f1, int f2, int d1, int d2, int d3,
                   double ln, int vflg, double v1, double v2,
                   PNTTYPE bs, int pn, PNTTYPE* pa, int* ia)			//<<< ���̋Ȑ��w��ػ���2
/*******************************************************************/
{
	if(flg==0) {														// <1���>
		InitLevel();													// Undo/Redo������
		Resize4(f1, f2, d1, d2, d3, ln, vflg, v1, v2, bs, pn, pa, ia);	// �Ȑ��w��ػ���2
	} else {															// <2��ڈȍ~>
		RedoResize4(f1, f2, d1, d2, d3, ln,
                    vflg, v1, v2, bs, pn, pa, ia);						// �Ȑ��w��ػ���2�Ď��s
	}
}
