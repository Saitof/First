/////////////////////////////////////////////////////////////////////////////
// ���j�b�g���̕ҏW�R�}���h��
// ECmdUnit.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "RCommand.h"
#include "NumFunc.h"

/**********************************************************/
void InitArrange(int pln, int cn, double th, PNTTYPE ccp,
                 int* lno, int* mna, int* mxa,
                 PNTTYPE* ocp, VECTYPE* cvc, VECTYPE* rvc)				//<<< �Θg�z�u��������
/**********************************************************/
{
	OBJTYPE* op, * wop;
	CMDTYPE* cmp;
	PNTTYPE  cent, pnt, zer, pt1, pt2, dis;
	VECTYPE  vec, wvc, rtv;
	BOXTYPE  box;
	int      i, j, k, i1, i2, i3, wln, dsp, edt, st;
	double   ft, wk, rot[3][3];

	i1 = pln;															// ���W���ݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	GetAreaBox(2, &box);												// �ر�ޯ���擾
	BoxCenter(box, &cent);												// �ޯ�������擾
	pnt.p[i1] = 0.0, pnt.p[i2] = 0.0, pnt.p[i3] = -1.0;					// ��]�������޸��
	zer.p[i1] = 0.0, zer.p[i2] = 0.0, zer.p[i3] = 0.0;					// ����]���S�_
	for(i=0; i<cn; i++) {
		st = 0;
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
		while(op!=NULL) {
			SetObjPtr(op);												// ���ʑΏۗ��̐ݒ�
			wop = CopyObject();											// ���̕���
			if(st==0) {
				if(i==0) {
					BaseCmdPtr(0, wop, &cmp);							// ���ʺ���ގ擾
					wln = GetCmdLvl(cmp);								// ���ݺ��������No�ۑ�
				}
				mna[i] = GetObjNo(wop);									// �擪��ŏI����No�ۑ�
				mxa[i] = GetObjNo(wop);
				st = 1;
			} else {
				mxa[i] = GetObjNo(wop);									// �ŏI����No�ۑ�
			}
			SetObjPtr(wop);												// �Ώۗ��̐ݒ�
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��[���ʉ�]�p]
			PutParaInt(cmp, 0, (int)(th*i*1000.0));						// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, ccp);									// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pnt);									// ��]�������޸�ِݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
			CreatCmd(MOVE, 3, 0, &cmp);									// ����ޗ̈�m��[���a�����p]
			PutParaInt(cmp, 0, 0);										// �ړ��ʐݒ�
			PutParaInt(cmp, 1, 0);
			PutParaInt(cmp, 2, 0);
			SetCmdLvl(cmp, wln+1);										// �����������No�ύX(+1)
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// �ړ�����ގ��s
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��[������]�p]
			PutParaInt(cmp, 0, 0);										// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, zer);									// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pnt);									// ��]�������޸�ِݒ�
			SetCmdLvl(cmp, wln+2);										// �����������No�ύX(+2)
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��[������]�p]
			PutParaInt(cmp, 0, 0);										// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, zer);									// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pnt);									// ��]�������޸�ِݒ�
			SetCmdLvl(cmp, wln+3);										// �����������No�ύX(+3)
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
			NextObjPtr(2, &op, &dsp, &edt);								// �����̂�
		}
	}
	Delete();															// ���Ώۗ��̍폜
	LevelBaseObjPtr(0, &op);											// �������̌���
	while(op!=NULL) {
		OnEdtFlg(1, op);												// �ҏW�Ώۗ��̂�
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextObjPtr(0, &op);										// ���������̌���
	}
	pt1 = ccp;
	pt1.p[i3] = 0.0;
	rtv.p[i1] = 0.0, rtv.p[i2] = 0.0, rtv.p[i3] = -1.0;					// ��]�޸��
	for(i=0; i<cn; i++) {
		ft = th*i*PI/180.0;												// ��]�p�x
		SetRotateMatrix(rtv, ft, rot);									// ��]��د���ݒ�
		for(j=0; j<DIM; j++) {											// �ړ��ʌv�Z
			for(wk=0.0, k=0; k<DIM; k++) {
				wk += rot[j][k]*ccp.p[k];
			}
			dis.p[j] = ccp.p[j]-wk;
		}
		pt2 = cent;														// ���ʌ����S�_
		VecRotateMove(&pt2, rot, dis);									// ���_���W��]�ړ�
		ocp[i] = pt2;													// ���ʐ撆�S�_
		pt2.p[i3] = 0;
		VecV(&pt1, &pt2, &vec);											// �z�񒆐S=>���ʐ撆�S�޸��
		VecN(&vec);														// �P���޸�ى�
		cvc[i] = vec;
		wvc.p[i1] = -vec.p[i2], wvc.p[i2] = vec.p[i1], wvc.p[i3] = vec.p[i3];
		rvc[i] = wvc;													// ���S�޸�ق̒����޸��
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
	*lno = wln;
}

/*************************/
void EndArrange(int clvl)												//<<< �Θg�z�u�I������
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp, * dcmp;
	int      del, dx, dy, dz, th;

	LevelBaseObjPtr(0, &op);											// �������̌���
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// �擪����ގ擾
		while(cmp!=NULL) {
			del = 0;
			if(GetCmdLvl(cmp)==clvl+1) {								// <���a�����p�����>
				dx = GetParaInt(cmp, 0);								// �ړ��ʎ擾
				dy = GetParaInt(cmp, 1);
				dz = GetParaInt(cmp, 2);
				del = (dx==0&&dy==0&&dz==0) ? 1 : del;
				SetCmdLvl(cmp, clvl);									// ���������No�߂�
			} else if(GetCmdLvl(cmp)>clvl+1) {							// <����/������]�p�����>
				th = GetParaInt(cmp, 0);								// ��]�p�x�擾
				del = (th==0) ? 1 : del;
				SetCmdLvl(cmp, clvl);									// ���������No�߂�
			}
			if(del) {
				dcmp = cmp;
			}
			NextCmdPtr(&cmp);											// ������ނ�
			if(del) {
				FreeCmd(op, dcmp, 1);									// ��������ލ폜
			}
		}
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextObjPtr(0, &op);										// ���������̌���
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***********************************************************************/
static void UnitMove(int lvl, int sno, int eno, PNTTYPE mv, PNTTYPE cp)	//<<< �����ƯĈړ�
/***********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// �������̂̌���
	while(op!=NULL) {
		if(GetObjNo(op)>=sno&&GetObjNo(op)<=eno) {						// <<�Ώ��Ư�>>
			BaseCmdPtr(0, op, &cmp);									// �擪����ގ擾
			while(cmp!=NULL) {
				if(GetCmdLvl(cmp)==lvl+1) {								// <���a�����p�����>
					PutParaInt(cmp, 0, (int)(mv.p[0]*1000.0));			// �ړ��ʐݒ�
					PutParaInt(cmp, 1, (int)(mv.p[1]*1000.0));
					PutParaInt(cmp, 2, (int)(mv.p[2]*1000.0));
				} else if(GetCmdLvl(cmp)>lvl+1) {						// <����/������]�p�����>
					PutParaPnt(cmp, 0, cp);								// ��]�����W�ݒ�
				}
				NextCmdPtr(&cmp);										// ������ނ�
			}
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextObjPtr(0, &op);										// ���������̌���
	}
}

/********************************************************************/
void ArrangeMove(double mv, int lno, int cn, int* mna,
                 int* mxa, PNTTYPE* ocp, PNTTYPE* mcp, VECTYPE* cvc)	//<<< �Θg�ړ�����
/********************************************************************/
{
	PNTTYPE pt;
	int     i, j;

	for(i=0; i<cn; i++) {
		for(j=0; j<DIM; j++) {
			pt.p[j] = cvc[i].p[j]*mv;									// ����ړ���
			mcp[i].p[j] = ocp[i].p[j] + pt.p[j];						// �ړ��㒆�S�_
		}
		UnitMove(lno, mna[i], mxa[i], pt, mcp[i]);						// �����ƯĈړ�
	}
}

/**************************************************/
static void UnitRotate1(int lvl, int sno, int eno,
                        double th, PNTTYPE cp)							//<<< �����Ưĉ�]1
/**************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// �������̌���
	while(op!=NULL) {
		if(GetObjNo(op)>=sno&&GetObjNo(op)<=eno) {						// <<�Ώ��Ư�>>
			BaseCmdPtr(0, op, &cmp);									// �擪����ގ擾
			while(cmp!=NULL) {
				if(GetCmdLvl(cmp)==lvl+2) {								// <������]�p�����>
					PutParaInt(cmp, 0, (int)(th*1000.0));				// ��]�p�x�ݒ�
					PutParaPnt(cmp, 0, cp);								// ��]�����W�ݒ�
				}
				NextCmdPtr(&cmp);										// ������ނ�
			}
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextObjPtr(0, &op);										// ���������̌���
	}
}

/*****************************************************/
void ArrangeRotate1(double th, int lno, int cn,
                    int* mna, int* mxa, PNTTYPE* mcp)					//<<< �Θg��]����1
/*****************************************************/
{
	int	i;

	for(i=0; i<cn; i++) {
		UnitRotate1(lno, mna[i], mxa[i], th, mcp[i]);					// �����Ưĉ�]1
	}
}

/**********************************************************/
static void UnitRotate2(int lvl, int sno, int eno,
                        double th, PNTTYPE cp, VECTYPE vc)				//<<< �����Ưĉ�]2
/**********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pvc;
	int      i;

	for(i=0; i<DIM; i++) {
		pvc.p[i] = vc.p[i];
	}
	LevelBaseObjPtr(0, &op);											// �������̂̌���
	while(op!=NULL) {
		if(GetObjNo(op)>=sno&&GetObjNo(op)<=eno) {						// <<�Ώ��Ư�>>
			BaseCmdPtr(0, op, &cmp);									// �擪����ގ擾
			while(cmp!=NULL) {
				if(GetCmdLvl(cmp)==lvl+3) {								// <������]�p�����>
					PutParaInt(cmp, 0, (int)(th*1000.0));				// ��]�p�x�ݒ�
					PutParaPnt(cmp, 0, cp);								// ��]�����W�ݒ�
					PutParaPnt(cmp, 1, pvc);							// ��]�������޸�ِݒ�
				}
				NextCmdPtr(&cmp);										// ������ނ�
			}
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		LevelNextObjPtr(0, &op);										// ���������̌���
	}
}

/*********************************************************/
void ArrangeRotate2(double th, int lno, int cn, int* mia,
                    int* maa, PNTTYPE* mcp, VECTYPE* rvc)				//<<< �Θg��]����2
/*********************************************************/
{
	int	i;

	for(i=0; i<cn; i++) {
		UnitRotate2(lno, mia[i], maa[i], th, mcp[i], rvc[i]);			// �����Ưĉ�]2
	}
}

/****************************************************************/
void InitBasePos(int flg, int ifg, int pln, int in, PNTTYPE* ip,
                 int* on, int* oia, int* oib, PNTTYPE* opa)				//<<< ��ړ���������
/****************************************************************/
{
	PNTTYPE* wc = new PNTTYPE[View->m_MaxSCtA];
	double*  ln = new double[View->m_MaxCPnt];
	OBJTYPE* op;
	OBJTYPE* wop;
	CMDTYPE* cmp;
	PNTTYPE  pv, zr, cp, sp, ep, pt;
	BOXTYPE  box;
	CString  str;
	int      i, j, k, f, i1, i2, i3, i4, w1, w2, mi2, ma2, cnt=0;
	double   a, b, t1, t2, sln, mi1, ma1, dd;

	i1 = pln;															// ���W���ݒ�
	i2 = (i1+1>2) ? 0 : i1+1;
	i3 = (i2+1>2) ? 0 : i2+1;
	zr.p[0] = 0.0, zr.p[1] = 0.0, zr.p[2] = 0.0;
	pv = zr, pv.p[i3] = -1.0;											// ��]���޸�ِݒ�
	BaseObjPtr(2, &op, &w1, &w2);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		if(cnt>=View->m_MaxSCtl) {										// <�ő�z�񐔈ȏ�>
			str.Format(IDS_ERR_PROCESS, View->m_MaxSCtA);				// �װү����
			AfxMessageBox(str);
			break;
		}
		if(op->part==2) {												// <�Ưĥ�q�̏ꍇ>
			wop = op->oset;												// �Ưĥ�e���߱
			if(wop==NULL) {
				wop = op;
			} else if(wop->edtflg==0) {
				wop = op;
			}
		} else {														// <�Ưĥ�q�ȊO�̏ꍇ>
			wop = op;
		}
		GetObjBox(wop, &box);											// �����ޯ���擾
		BoxCenter(box, &cp);											// �ޯ�������擾
		wc[cnt] = cp;													// ���̒��S�_�ۑ�
		if(ifg) {														// <<��������>>
			oia[cnt] = GetObjNo(op);									// ����No�ݒ�
			SetObjPtr(op);												// �����߲���ݒ�
			CreatCmd(MOVE, 3, 0, &cmp);									// ����ޗ̈�m��[�ړ��p]
			PutParaInt(cmp, 0, 0);										// �ړ��ʐݒ�
			PutParaInt(cmp, 1, 0);
			PutParaInt(cmp, 2, 0);
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// �ړ�����ގ��s
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��[XY��]�p]
			PutParaInt(cmp, 0, 0);										// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, zr);										// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pv);										// ��]�������޸�ِݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
		}
		cnt++;
		NextObjPtr(2, &op, &w1, &w2);									// �����̂�
	}
	*on = cnt;															// �Ώۗ��̐�
	for(ln[0]=0.0, sln=0.0, i=1; i<in; i++) {
		sln += PntD(&ip[i-1], &ip[i]);									// ���v��������
		ln[i] = sln;													// �ݐϐ�������
	}
	if(flg==0) {														// <���[�J]�̏ꍇ>
		sp = ip[0], ep = ip[in-1];										// �����n�_��I�_
		i4 = (fabs(sp.p[i1]-ep.p[i1])>fabs(sp.p[i2]-ep.p[i2])) ? i1:i2;	// �ő卷������
		if(sp.p[i4]<ep.p[i4]) {											// ����ŏ��ő�擾
			mi1 = sp.p[i4], mi2 = 0, ma1 = ep.p[i4], ma2 = in-1;
		} else {
			mi1 = ep.p[i4], mi2 = in-1, ma1 = sp.p[i4], ma2 = 0;
		}
		if(fabs(sp.p[i1]-ep.p[i1])<EPSILON) {							// <i1�����s����>
			f = 1, a = sp.p[i1], b = 0.0;
		} else if(fabs(sp.p[i2]-ep.p[i2])<EPSILON) {					// <i2�����s����>
			f = 2, a = 0.0, b = sp.p[i2];
		} else {														// <i1,i2���񕽍s����>
			a = (ep.p[i2]-sp.p[i2])/(ep.p[i1]-sp.p[i1]);				// �����X��
			if(fabs(a)<EPSILON) {										// �X�����w��i2���ƕ��s
				f = 2, a = 0.0, b = sp.p[i2];
			} else {
				b = (ep.p[i1]-sp.p[i1])/(ep.p[i2]-sp.p[i2]);			// �����t�X��
				if(fabs(b)<EPSILON) {									// �X�����w��i1���ƕ��s
					f = 1, a = sp.p[i1], b = 0.0;
				} else {												// i1,i2�Ɗ��S�ɔ񕽍s
					f = 0, b = sp.p[i2]-a*sp.p[i1];						// �����萔
				}
			}
		}
		for(i=0; i<cnt; i++) {
			CalcLinePoint(pln, f, a, b, &wc[i], &pt);					// ������_�Z�o
			pt.p[i3] = sp.p[i3];										// ���ʐ����������W
			if(pt.p[i4]<=mi1) {											// <��_����ŏ��ȉ�>
				oib[i] = (int)(ln[mi2]/sln*100000.0);					// �����䗦�ݒ�
				opa[i] = ip[mi2];										// ������_�ݒ�
			} else if(pt.p[i4]>=ma1) {									// <��_����ő�ȏ�>
				oib[i] = (int)(ln[ma2]/sln*100000.0);
				opa[i] = ip[ma2];
			} else {													// <��_�����>
				dd = pt.p[i4];
				for(j=0; j<in-1; j++) {
					if((ip[j].p[i4]<=dd&&dd<ip[j+1].p[i4])||
						(ip[j+1].p[i4]<dd&&dd<=ip[j].p[i4])) {			// �Y������������
						t1 = dd-ip[j].p[i4];
						t2 = ip[j+1].p[i4]-ip[j].p[i4];
						t1 = t1/t2;										// �����䗦
						if(t1<EPSILON) {								// �䗦�␳
							t1 = 0.0;
						} else if(fabs(t1-1.0)<EPSILON) {
							j++;
							t1 = 0.0;
						}
						t2 = PntD(&ip[j], &ip[j+1])*t1;					// ��������
						oib[i] = (int)((ln[j]+t2)/sln*100000.0);		// �����䗦�ݒ�
						opa[i] = pt;									// ������_�ݒ�
						break;
					}
				}
			}
		}
	} else {															// <��~/���[��]�̏ꍇ>
		for(i=0; i<cnt; i++) {
			for(a=DBL_MAX, k=0, j=0; j<in-1; j++) {						// �ȉ~�̍ŒZ������_����
				b = DistPointLine(&ip[j], &ip[j+1], &wc[i], &w1, &t1, &pt);
				if(b<a) {
					a = b, k = j, w2 = w1, t2 = t1, pv = pt;
				}
			}
			oib[i] = (int)((ln[k]+t2)/sln*100000.0);					// �����䗦�ݒ�
			opa[i] = pv;												// ������_�ݒ�
		}
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
	delete[] wc;
	delete[] ln;
}

/*********************/
void EndBasePos(void)													//<<< ��ړ��I������
/*********************/
{
	OBJTYPE* op;
	CMDTYPE* cmp, * dcmp;
	int      del, dx, dy, dz, th, dsp, edt;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		del = 0;
		if(GetCmdNo(cmp)==MOVE) {										// <�ړ������>
			dx = GetParaInt(cmp, 0);									// �ړ��ʎ擾
			dy = GetParaInt(cmp, 1);
			dz = GetParaInt(cmp, 2);
			del = (dx==0&&dy==0&&dz==0) ? 1 : del;
		} else if(GetCmdNo(cmp)==ROTATE) {								// <��]�����>
			th = GetParaInt(cmp, 0);									// ��]�p�x�擾
			del = (th==0) ? 1 : del;
		}
		if(del) {
			dcmp = cmp;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
		if(del) {
			GetCmdObj(dcmp, &op);										// �ҏW�Ώۗ��̎擾
			FreeCmd(op, dcmp, 1);										// ��������ލ폜
		}
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
static void BasePosMove(int ono, double dx, double dy, double dz)		//<<< ��ړ�
/*****************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// �ҏW�Ώۗ��̎擾
		if(GetObjNo(op)==ono&&GetCmdNo(cmp)==MOVE) {					// <�Ώۗ��̈ړ������>
			PutParaInt(cmp, 0, (int)(dx*1000.0));						// �ړ��ʐݒ�
			PutParaInt(cmp, 1, (int)(dy*1000.0));
			PutParaInt(cmp, 2, (int)(dz*1000.0));
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/*********************************************************/
static void BasePosRotate(int ono, double th, PNTTYPE cp)				//<<< ���]
/*********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// �ҏW�Ώۗ��̎擾
		if(GetObjNo(op)==ono&&GetCmdNo(cmp)==ROTATE) {					// <�Ώۗ��̉�]�����>
			PutParaInt(cmp, 0, (int)(th*1000.0));						// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, cp);										// ��]�����W�ݒ�
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
}

/*******************************************************/
void EditBasePos(int flg, int pln, int ufg, int on,
                 PNTTYPE* op, int nn, PNTTYPE* np,
                 int no, int* ia, int* ib, PNTTYPE* pa)					//<<< ��ړ��ҏW����
/*******************************************************/
{
	PNTTYPE* wnp = new PNTTYPE[100];
	double*  oln = new double[View->m_MaxCPnt];
	double*  nln = new double[View->m_MaxCPnt];
	OBJTYPE* obj;
	PNTTYPE  opt, npt, mv, p0, p1, p2, p3, p4;
	VECTYPE  vec;
	int      i, j, i0, i1, i2, oa, na, os, ns;
	double   t1, t2, t3, t4, ol, nl, l1, l2, dd;

	p0.p[0] = 0.0, p0.p[1] = 0.0, p0.p[2] = 0.0;						// ��Ɨ̈揉����
	for(oln[0]=0.0, ol=0.0, i=1; i<on; i++) {							// <������>
		l1 = PntD(&op[i-1], &op[i]);									// ��������
		ol += l1;														// ���v��������
		oln[i] = ol;													// �ݐϐ�������
	}
	if(flg==1) {														// <<��~�̏ꍇ>>
		i1 = (pln+1>2) ? 0 : pln+1;										// ���W���ݒ�
		i2 = (i1+1>2) ? 0 : i1+1;
		for(i=0; i<nn; i++) {
			wnp[i] = np[i];												// �V�������W�ۑ�
			np[i].p[i2] = op[i].p[i2];									// i2���͋��������g�p
		}
		for(nln[0]=0.0, nl=0.0, i=1; i<nn; i++) {						// <�V����>
			l2 = PntD(&np[i-1], &np[i]);								// ��������
			nl += l2;													// ���v��������
			nln[i] = nl;												// �ݐϐ�������
		}
	} else {															// <<���[�J]/���[��]�̏ꍇ>>
		for(nln[0]=0.0, nl=0.0, i=1; i<nn; i++) {						// <�V����>
			l2 = PntD(&np[i-1], &np[i]);								// ��������
			nl += l2;													// ���v��������
			nln[i] = nl;												// �ݐϐ�������
		}
	}
	for(i=0; i<no; i++) {
		opt = pa[i];													// ������ʒu
		l1 = ib[i]/100000.0;											// �����䗦(��)
		l2 = (ufg) ? l1*ol/nl : l1;										// �����䗦(�V)
		if(l1<EPSILON) {												// ����No
			oa = 0, os = 1;
		} else if(fabs(l1-1.0)<EPSILON) {
			oa = on-1, os = 1;
		} else {
			for(j=1; j<on; j++) {										// <������>
				if(oln[j]/ol>l1) {
					break;
				}
			}
			oa = j-1;													// �Y���ʒu����No
			if(fabs(l1-oln[oa]/ol)<EPSILON)	{							// �n�_�ߎ�
				os = 1;
			} else if(fabs(l1-oln[oa+1]/ol)<EPSILON) {					// �I�_�ߎ��͎�����
				os = 1, oa++;
			} else {													// ��������
				os = 0;
			}
		}
		if(l2<EPSILON) {												// ����No
			na = 0, ns = 1;
		} else if(fabs(l2-1.0)<EPSILON) {
			na = nn-1, ns = 1;
		} else {
			for(j=1; j<nn; j++) {										// <�V����>
				if(nln[j]/nl>l2) {
					break;
				}
			}
			na = j-1;													// �Y���ʒu����No
			if(fabs(l2-nln[na]/nl)<EPSILON) {							// �n�_�ߎ�
				ns = 1;
			} else if(fabs(l2-nln[na+1]/nl)<EPSILON) {					// �I�_�ߎ��͎�����
				ns = 1, na++;
			} else {													// ��������
				ns = 0;
			}
		}
		if(ns==1) {														// <�����[�̏ꍇ>
			npt = np[na];												// ���݊�ʒu
			if(flg==1) {												// <<��~�̏ꍇ>>
				npt = wnp[na];											// ���݊�ʒu
			}
		} else {														// <�����Ԃ̏ꍇ>
			l2 = nl*l2-nln[na];											// �Y�������㋗��
			VecV(&np[na], &np[na+1], &vec);								// ���������޸��
			VecN(&vec);													// �P���޸�ى�
			for(j=0; j<DIM; j++) {
				npt.p[j] = np[na].p[j]+l2*vec.p[j];						// ���݊�ʒu
			}
			if(flg==1) {												// <<��~�̏ꍇ>>
				VecV(&wnp[na], &wnp[na+1], &vec);						// ���������޸��
				VecN(&vec);												// �P���޸�ى�
				for(j=0; j<DIM; j++) {
					npt.p[j] = wnp[na].p[j]+l2*vec.p[j];				// ���݊�ʒu
				}
			}
		}
		for(j=0; j<DIM; j++) {
			mv.p[j] = npt.p[j]-opt.p[j];								// �ړ��ʎZ�o
		}
		BasePosMove(ia[i], mv.p[0], mv.p[1], mv.p[2]);					// ��ړ�
		if(os==1) {														// <<<�������[�̏ꍇ>>>
			if(oa>0&&oa<on-1) {											// <<�����Ԑ����̏ꍇ>>
				i0 = oa, i1 = oa+1, i2 = oa-1;
			} else {													// <<�����[�����̏ꍇ>>
				if(flg==0) {											// <���[�J]�̏ꍇ>
					if(oa==0) {
						i0 = 0, i1 = 1, i2 = -1;
					} else {
						i0 = on-2, i1 = on-1, i2 = -1;
					}
				} else {												// <��~/���[��]�̏ꍇ>
					i0 = 0, i1 = 1, i2 = on-2;
				}
			}
		} else {														// <<<�������Ԃ̏ꍇ>>>
			i0 = oa, i1 = oa+1, i2 = -1;
		}
		for(j=0; j<DIM; j++) {
			p1.p[j] = op[i1].p[j]-op[i0].p[j];							// ����W�ݒ�
		}
		if(i2!=-1) {
			for(j=0; j<DIM; j++) {
				p2.p[j] = op[i2].p[j]-op[i0].p[j];
			}
		} else {
			for(j=0; j<DIM; j++) {
				p2.p[j] = -p1.p[j];
			}
		}
		if((t3=CalcBaseAngle(p0, p1, pln))<0) {							// ��p�x�擾
			t3 += 2.0*PI;
		}
		if((t4=CalcBaseAngle(p0, p2, pln))<0) {
			t4 += 2.0*PI;
		}
		t1 = (t3+t4)/2.0;
		if(t3>PI) {
			if(t1<t3-PI||t1>t3) {
				t1 = (t1-PI<0) ? t1-PI+(2.0*PI) : t1-PI;				// ���������ɕ␳
			}
		} else {
			if(t1>t3&&t1<t3+PI) {
				t1 = (t1-PI<0) ? t1-PI+(2.0*PI) : t1-PI;
			}
		}
		if(ns==1) {														// <<<�V�����[�̏ꍇ>>>
			if(na>0&&na<nn-1) {											// <<�V���Ԑ����̏ꍇ>>
				i0 = na, i1 = na+1, i2 = na-1;
			} else {													// <<�V���[�����̏ꍇ>>
				if(flg==0) {											// <���[�J]�̏ꍇ>
					if(na==0) {
						i0 = 0, i1 = 1, i2 = -1;
					} else {
						i0 = nn-2, i1 = nn-1, i2 = -1;
					}
				} else {												// <��~/���[��]�̏ꍇ>
					i0 = 0, i1 = 1, i2 = nn-2;
				}
			}
		} else {														// <<<�V�����Ԃ̏ꍇ>>>
			i0 = na, i1 = na+1, i2 = -1;
		}
		for(j=0; j<DIM; j++) {
			p3.p[j] = np[i1].p[j]-np[i0].p[j];							// ����W�ݒ�
		}
		if(i2!=-1) {
			for(j=0; j<DIM; j++) {
				p4.p[j] = np[i2].p[j]-np[i0].p[j];
			}
		} else {
			for(j=0; j<DIM; j++) {
				p4.p[j] = -p3.p[j];
			}
		}
		if((t3=CalcBaseAngle(p0, p3, pln))<0) {							// ��p�x�擾
			t3 += 2.0*PI;
		}
		if((t4=CalcBaseAngle(p0, p4, pln))<0) {
			t4 += 2.0*PI;
		}
		t2 = (t3+t4)/2.0;
		if(t3>PI) {
			if(t2<t3-PI||t2>t3) {
				t2 = (t2-PI<0) ? t2-PI+(2.0*PI) : t2-PI;				// ���������ɕ␳
			}
		} else {
			if(t2>t3&&t2<t3+PI) {
				t2 = (t2-PI<0) ? t2-PI+(2.0*PI) : t2-PI;
			}
		}
		dd = 180.0*(t1-t2)/PI;											// �ψʊp�x
		BasePosRotate(ia[i], dd, npt);									// ���]
	}
	BaseObjPtr(2, &obj, &i1, &i2);										// �����Ώۗ��̎擾
	while(obj!=NULL) {
		RedoCommand(obj);												// ����ލĎ��s
		View->SetDispList(obj);											// �ި���ڲ�ؽēo�^
		NextObjPtr(2, &obj, &i1, &i2);									// �����̂�
	}
	if(flg==1) {														// <<��~�̏ꍇ>>
		for(i=0; i<nn; i++) {
			np[i] = wnp[i];												// �V�������W����
		}
	}
	delete[] wnp;
	delete[] oln;
	delete[] nln;
}

/*************************************************************/
static void InitLinesEdit(int d1, int d2, int d3, PNTTYPE sp,
                          PNTTYPE ep, int on, PNTTYPE* cpt,
                          double* len, PNTTYPE* crs)					//<<< �Ȑ��w��ړ���������
/*************************************************************/
{
	int    i, f, md;
	double a, b;

	if(on<1) {															// ���̐�0�ȉ��͒��~
		return;
	}
	md = (fabs(sp.p[d1]-ep.p[d1])>fabs(sp.p[d2]-ep.p[d2])) ? d1 : d2;	// �ő卷������(md)
	if(fabs(sp.p[d1]-ep.p[d1])<EPSILON) {								// <d1�����s����>
		f = 1, a = sp.p[d1], b = 0.0;
	} else if(fabs(sp.p[d2]-ep.p[d2])<EPSILON) {						// <d2�����s����>
		f = 2, a = 0.0, b = sp.p[d2];
	} else {															// <d1,d2���񕽍s����>
		a = (ep.p[d2]-sp.p[d2])/(ep.p[d1]-sp.p[d1]);					// �����X��
		if(fabs(a)<EPSILON) {											// �X�����w��d2���ƕ��s
			f = 2, a = 0.0, b = sp.p[d2];
		} else {
			b = (ep.p[d1]-sp.p[d1])/(ep.p[d2]-sp.p[d2]);				// �����t�X��
			if(fabs(b)<EPSILON) {										// �X�����w��d1���ƕ��s
				f = 1, a = sp.p[d1], b = 0.0;
			} else {													// d1,d2�Ɗ��S�ɔ񕽍s
				f = 0, b = sp.p[d2]-a*sp.p[d1];
			}
		}
	}
	for(i=0; i<on; i++) {
		CalcLinePoint(d1, f, a, b, &cpt[i], &crs[i]);					// ������_�Z�o
		crs[i].p[d3] = sp.p[d3];										// ���ʐ����������W
		len[i] = PntD(&sp, &crs[i]);									// ����n�_�Ƃ̋���
	}
}

/********************************************************************/
static void ObjLinesEdit(OBJTYPE* op, int d1, int d2, int d3,
                         int uf, PNTTYPE sp, PNTTYPE ep, double oln,
                         int on, double* len, PNTTYPE* crs)				//<<< �Ȑ��w��ړ�(1�Ȑ���)
/********************************************************************/
{
	PNTTYPE* cv = new PNTTYPE[View->m_MaxCPnt];
	double*  fta = new double[View->m_MaxCPnt];
	double*  vta = new double[View->m_MaxCPnt];
	double*  vln = new double[View->m_MaxCPnt];
	OBJTYPE* wop;
	OBJTYPE* cop;
	CMDTYPE* cmp;
	PNTTYPE  wp, ncrs, nsp, nep, stp, pvc;
	VECTYPE  svc, wvc, wv1, wv2, wv3, wv4;
	int      i, p, vn, st, ed, fg=0, ix=0, pm=0, ct=0;
	double   ln, nln, wln, fth, vth, w1, w2, th1, th2;

	vn = GetVtxNum(op);													// ���̒��_���擾
	for(i=0; i<vn; i++) {
		GetVtx(op, i, &cv[i]);											// ���̒��_�擾
	}
	if(PntD(&wp, &cv[0])>PntD(&sp, &cv[vn-1])) {						// <�Ȑ��I�_������n�_�ɋ߂�>
		st = 0, ed = vn-1;												// �n�_/�I�_�ԍ�
		while(st<ed) {													// <�I�_<�n�_�܂ŌJ�Ԃ�>
			wp = cv[st], cv[st] = cv[ed], cv[ed] = wp;					// �n�_����I�_������ւ�
			ed--, st++;													// �I�_���Z/�n�_���Z
		}
	}
	for(vln[0]=0.0, nln=0.0, i=1; i<vn; i++) {
		nln += PntD(&cv[i-1], &cv[i]);									// ���v��������
		vln[i] = nln;													// �������ݐϋ���
	}
	CalcNVec(cv, vn, &wvc);												// �Ȑ��@���޸�َ擾
	if(fabs(wvc.p[2])<EPSILON) {										// <�Ȑ��������ɑ��݂���ꍇ>
		fg = 1;															// �Ȑ������׸�ON
		ix = (fabs(wvc.p[0])>fabs(wvc.p[1])) ? 1 : ix;					// ������ݒ�
	}
	VecV(&sp, &ep, &svc);												// ����޸�َ擾
	VecN(&svc);															// �P���޸�ى�
	if(fabs(svc.p[0])>EPSILON||fabs(svc.p[1])>EPSILON) {				// ���!=Z���̏ꍇ��
		fth = atan2(svc.p[1], svc.p[0]);								// ��������p�x
		fth = (fth<-10||fth>10) ? 0.0 : fth;
		vth = sqrt(svc.p[0]*svc.p[0]+svc.p[1]*svc.p[1]);				// �������������
		vth = atan2(svc.p[2], vth);										// ��������p�x
		vth = (vth<-10||vth>10) ? 0.0 : vth;
	} else {															// ���=Z���̏ꍇ��
		fth = 0.0;														// ��������p�x
		vth = (svc.p[2]>0) ? PI/2.0 : -PI/2.0;							// ��������p�x
		svc.p[0] = 0.0, svc.p[1] = 1.0, svc.p[2] = 0.0;					// ��]���p�ɉ��޸�ِݒ�
	}
	VecV(&cv[0], &cv[1], &wvc);											// �Ȑ������޸�َ擾
	VecN(&wvc);															// �P���޸�ى�
	if(fabs(wvc.p[0])>EPSILON||fabs(wvc.p[1])>EPSILON) {				// �Ȑ�����!=Z���̏ꍇ��
		wv1 = wvc, wv1.p[2] = 0;										// ���������̂ݒ��o
		VecN(&wv1);														// �P���޸�ى�
	} else {															// Z���̏ꍇ�͊p�x=0���޸��
		wv1.p[0] = 1.0, wv1.p[1] = 0.0, wv1.p[2] = 0.0;
	}
	w1 = sqrt(wvc.p[0]*wvc.p[0]+wvc.p[1]*wvc.p[1]);						// �Ȑ���������������
	wv2.p[0] = w1, wv2.p[1] = wvc.p[2], wv2.p[2] = 0.0;					// ����������XY���ʂɓ��e
	VecN(&wv2);															// �P���޸�ى�
	if(fg) {															// <�Ȑ��������̏ꍇ>
		if(wv1.p[ix]>EPSILON) {											// ���������:��
			pm = 1;
		} else if(wv1.p[ix]<-EPSILON) {									// ���������:��
			pm = -1;
		}
	}
	fta[1] = atan2(wv1.p[1], wv1.p[0]);									// �Ȑ����������p�x
	fta[1] = (fta[1]<-10||fta[1]>10) ? 0.0 : fta[1];
	vta[1] = atan2(wv2.p[1], wv2.p[0]);									// �Ȑ����������p�x
	vta[1] = (vta[1]<-10||vta[1]>10) ? 0.0 : vta[1];
	for(i=2; i<vn; i++) {
		VecV(&cv[i-1], &cv[i], &wvc);									// �Ȑ������޸�َ擾
		VecN(&wvc);														// �P���޸�ى�
		if(fabs(wvc.p[0])>EPSILON||fabs(wvc.p[1])>EPSILON) {			// �Ȑ�����!=Z���̏ꍇ��
			wv3 = wvc, wv3.p[2] = 0;									// ���������̂ݒ��o
			VecN(&wv3);													// �P���޸�ى�
		} else {														// Z���̏ꍇ�͊p�x=0���޸��
			wv3.p[0] = 1.0, wv3.p[1] = 0.0, wv3.p[2] = 0.0;
		}
		w1 = sqrt(wvc.p[0]*wvc.p[0]+wvc.p[1]*wvc.p[1]);					// �Ȑ���������������
		wv4.p[0] = w1, wv4.p[1] = wvc.p[2], wv4.p[2] = 0.0;				// ����������XY���ʂɓ��e
		VecN(&wv4);														// �P���޸�ى�
		if(fg&&pm==0) {													// <�Ȑ�:�����Ŋ:���̏ꍇ>
			if(wv3.p[ix]>EPSILON) {										// ���������:��
				pm = 1;
			} else if(wv3.p[ix]<-EPSILON) {								// ���������:��
				pm = -1;
			}
		}
		w1 = atan2(wv1.p[1], wv1.p[0]);									// �O�����̐����p�x
		w1 = (w1<-10||w1>10) ? 0.0 : w1;
		w2 = atan2(wv3.p[1], wv3.p[0]);									// ��������̐����p�x
		w2 = (w2<-10||w2>10) ? 0.0 : w2;
		w2 -= w1;														// �O�����Ƃ̐����p�x��
		w2 = (fabs(w2)>PI-EPSILON&&fabs(w2)<PI+EPSILON) ? 0.0 : w2;		// 180�x���]��0�x
		fta[i] = fta[i-1]+w2;											// �����p�x�����Z
		w1 = atan2(wv2.p[1], wv2.p[0]);									// �O�����̐����p�x
		w1 = (w1<-10||w1>10) ? 0.0 : w1;
		w2 = atan2(wv4.p[1], wv4.p[0]);									// ��������̐����p�x
		w2 = (w2<-10||w2>10) ? 0.0 : w2;
		if(fg) {														// <<�Ȑ��������̏ꍇ>>
			if(pm==1&&wv1.p[ix]<-EPSILON) {								// <�:�����޸��:��>
				w1 = (w1>0.0) ? PI-w1 : -PI-w1;							// �p�x�ϊ�
			} else if(pm==-1&&wv1.p[ix]>EPSILON) {						// <�:�����޸��:��>
				w1 = (w1>0.0) ? PI-w1 : -PI-w1;							// �p�x�ϊ�
			}
			if(pm==1&&wv3.p[ix]<-EPSILON) {								// <�:�����޸��:��>
				w2 = (w2>0.0) ? PI-w2 : -PI-w2;							// �p�x�ϊ�
			} else if(pm==-1&&wv3.p[ix]>EPSILON) {						// <�:�����޸��:��>
				w2 = (w2>0.0) ? PI-w2 : -PI-w2;							// �p�x�ϊ�
			}
		}
		w2 -= w1;														// �O�����Ƃ̐����p�x��
		w2 = (fabs(w2)>PI-EPSILON&&fabs(w2)<PI+EPSILON) ? 0.0 : w2;		// 180�x���]��0�x
		vta[i] = vta[i-1]+w2;											// �����p�x�����Z
		wv1 = wv3, wv2 = wv4;											// ��������޸�ٕۑ�
	}
	SelBaseObjPtr(2, &wop);												// �ҏW�Ώۗ��̎擾
	while(wop!=NULL) {
		if(ct>=on) {													// �ő嗧�̐��ŏI��
			break;
		}
		if(uf) {														// <���̊ԋ����ύX����>
			wln = len[ct]*nln/oln;
		} else {														// <���̊ԋ����ύX�Ȃ�>
			wln = len[ct];
		}
		if(wln<0) {														// <����n�_�����̏ꍇ>
			p = 1, nsp = cv[0], nep = cv[1], stp = nsp;					// �Ȑ��Ή��̐V����
			ln = wln;													// ����n�_���ް���钷��
		} else if(wln>vln[vn-1]) {										// <����I�_�ȏ�̏ꍇ>
			p = vn-1, nsp = cv[vn-2], nep = cv[vn-1], stp = nep;		// �Ȑ��Ή��̐V����
			ln = wln-vln[vn-1];											// ����I�_���ް���钷��
		} else {														// <����̎n�_�`�I�_�̏ꍇ>
			for(p=1; p<vn-1; p++) {										// �Y����������
				if(wln<vln[p]) {
					break;
				}
			}
			nsp = cv[p-1], nep = cv[p], stp = nsp;						// �Ȑ��Ή��̐V����
			ln = wln-vln[p-1];											// �����n�_���ް���钷��
		}
		VecV(&nsp, &nep, &wvc);											// �Ȑ������޸�َ擾
		VecN(&wvc);														// �P���޸�ى�
		for(i=0; i<DIM; i++) {
			ncrs.p[i] = stp.p[i]+ln*wvc.p[i];							// �Ȑ��Ή��̐V��_�ʒu
		}
		for(i=0; i<DIM; i++) {
			ncrs.p[i] -= crs[ct].p[i];									// �V��_�ւ̈ړ���
		}
		th1 = 180.0*(fth-fta[p])/PI;									// ������]�p�x
		th2 = 180.0*(vth-vta[p])/PI;									// ������]�p�x
		SetObjPtr(wop);													// ���ʑΏۗ��̐ݒ�
		cop = CopyObject();												// ���̕���
		if(fabs(th2)>EPSILON) {											// <�����L����]�p�x����>
			wvc.p[0] = -svc.p[1], wvc.p[1] = svc.p[0], wvc.p[2] = 0.0;	// ����̒����޸��
			VecN(&wvc);													// �P���޸�ى�
			for(i=0; i<DIM; i++) {
				pvc.p[i] = wvc.p[i];									// ����������]���޸��
			}
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(th2*1000.0));						// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, crs[ct]);								// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pvc);									// ��]�������޸�ِݒ�
			SetObjPtr(cop);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
		}
		if(fabs(th1)>EPSILON) {											// <�����L����]�p�x����>
			pvc.p[0] = 0.0, pvc.p[1] = 0.0, pvc.p[2] = -1.0;			// ����������]���޸��
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(th1*1000.0));						// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, crs[ct]);								// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, pvc);									// ��]�������޸�ِݒ�
			SetObjPtr(cop);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
		}
		if(fabs(ncrs.p[0])>EPSILON||
           fabs(ncrs.p[1])>EPSILON||
           fabs(ncrs.p[2])>EPSILON) {									// <�L���ړ��ʂ���>
			CreatCmd(MOVE, 3, 0, &cmp);									// ����ޗ̈�m��
			PutParaInt(cmp, 0, (int)(ncrs.p[0]*1000.0));				// �ړ��ʐݒ�
			PutParaInt(cmp, 1, (int)(ncrs.p[1]*1000.0));
			PutParaInt(cmp, 2, (int)(ncrs.p[2]*1000.0));
			SetObjPtr(cop);												// �����߲���ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// �ړ�����ގ��s
		}
		View->SetDispList(cop);											// �ި���ڲ�ؽēo�^
		SelNextObjPtr(2, &wop);											// �����̂�
		ct++;
	}
	delete[] cv;
	delete[] fta;
	delete[] vta;
	delete[] vln;
}

/****************************************************/
void ObjectLinesEdit(int d1, int d2, int d3, int uf,
                     PNTTYPE sp, PNTTYPE ep)							//<<< �Ȑ��w��ړ�����
/****************************************************/
{
	PNTTYPE* cp = new PNTTYPE[View->m_MaxSCtA];
	PNTTYPE* cr = new PNTTYPE[View->m_MaxSCtA];
	double*  ln = new double[View->m_MaxSCtA];
	OBJTYPE* op, * op2, * co[10];
	PNTTYPE  pt;
	BOXTYPE  box;
	CString  str;
	int      i, c1=0, c2=0;
	double   sln;

	SelBaseObjPtr(1, &op);												// ����Ȑ����̎擾
	while(op!=NULL) {
		if(c1>=10) {													// <�ő�z�񐔈ȏ�>
			str.Format(IDS_ERR_PROCESS, 10);							// �װү����
			AfxMessageBox(str);
			break;
		}
		co[c1++] = op;													// ����Ȑ�����OP�ۑ�
		SelNextObjPtr(1, &op);											// �����̂�
	}
	SelBaseObjPtr(2, &op);												// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		if(c2>=View->m_MaxSCtl) {										// <�ő�z�񐔈ȏ�>
			str.Format(IDS_ERR_PROCESS, View->m_MaxSCtA);				// �װү����
			AfxMessageBox(str);
			break;
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
		GetObjBox(op2, &box);											// �����ޯ���擾
		BoxCenter(box, &pt);											// �ޯ�������擾
		cp[c2++] = pt;													// ���̒��S�_�ۑ�
		SelNextObjPtr(2, &op);											// �����̂�
	}
	InitLinesEdit(d1, d2, d3, sp, ep, c2, cp, ln, cr);					// �Ȑ��w��ړ���������
	sln = PntD(&sp, &ep);												// 2�_�ԋ���(3D)
	StartMenu();														// �ƭ��J�n����
	InitLevel();														// Undo/Redo������
	for(i=0; i<c1; i++) {
		ObjLinesEdit(co[i], d1, d2, d3, uf, sp, ep, sln, c2, ln, cr);	// �Ȑ��w��ړ�(1�Ȑ���)
	}
	SelBaseObjPtr(2, &op);												// �ҏW�Ώۗ��̎擾
	while(op!=NULL) {
		op2 = op;														// �폜�����߲���ۑ�
		SelNextObjPtr(2, &op);											// �����̂�
		LevelObj(1, op2);												// �������ٍ폜���̐ڑ�
		TempObj(op2);													// ���̈ꎞ�폜
	}
	LevelBaseObjPtr(0, &op);											// �������̎擾
	while(op) {
		OnEdtFlg(1, op);												// edtflg:ON
		LevelNextObjPtr(0, &op);
	}
	EndJMenu1();														// �ƭ��I������
	delete[] cp;
	delete[] cr;
	delete[] ln;
}
