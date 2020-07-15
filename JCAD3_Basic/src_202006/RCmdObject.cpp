/////////////////////////////////////////////////////////////////////////////
// �R�}���h�֘A ���̕���/�폜��
// RCmdObject.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"
#include "NumFunc.h"

/*************************/
OBJTYPE* CopyObject(void)												//<<< ����&����ޕ���
/*************************/
{
	OBJTYPE* op, * optr;
	OIFTYPE* oif;
	CMDTYPE* cmp, * cptr;
	PNTTYPE  pd;
	int      i, num1, num2, num3, id1, id2, cno, gno[32];
	char     name[24];

	GetObjPtr(&op);														// �����Ώۗ��̎擾
	if(!CheckObjPtr(0, op)) {											// ��������
		return NULL;
	}
	num1 = GetVtxNum(op);												// ���_���擾
	num2 = GetFlpNum1(op);												// ��ٰ�ߐ��擾
	num3 = GetFlpNum2(op);
	CreatObj(num1, num2, num3, &optr);									// ���̗̈�m��
	LevelObj(0, optr);													// �������ِ������̐ڑ�
	for(i=0; i<num1; ++i) {
		GetVtx(op, i, &pd);												// ���_�擾
		PutVtx(optr, i, pd);											// ���_�ݒ�
	}
	for(i=0; i<num2; ++i) {												// ��ٰ��1
		GetFlp1(op, i, &id1, &id2);										// ��ٰ�ߏ��擾
		PutFlp1(optr, i, id1, id2);										// ��ٰ�ߏ��ݒ�
	}
	for(i=0; i<num3; ++i) {												// ��ٰ��2
		GetFlp2(op, i, &id1, &id2);										// ��ٰ�ߏ��擾
		PutFlp2(optr, i, id1, id2);										// ��ٰ�ߏ��ݒ�
	}
	PutObjAtr(optr, GetObjAtr(op));										// ����No�ݒ�
	PutObjPrt(optr, GetObjPrt(op), 0);									// �߰�No�ݒ�
	PutObjCol(optr, GetObjCol(op));										// �װNo�ݒ�
	GetGroup(op, gno);													// ��ٰ�ߏ��擾
	PutGroup(optr, gno);												// ��ٰ�ߏ��ݒ�
	SetObjBox(optr);													// �ޯ�����ސݒ�
	GetObjOif(op, &oif);												// ���̍\������߲���擾
	GetOifName(oif, name);												// ���̍\�����擾
	GetObjOif(optr, &oif);												// ���̍\������߲���擾
	SetOifName(0, oif, name);											// ���̍\�����ݒ�
	BaseCmdPtr(1, op, &cmp);
	while(cmp!=NULL) {
		cno = GetCmdNo(cmp);											// �����No�擾
		if(cno==CHGSURF) {												// �����No�ύX
			cno = CRTSURF;
		}
		if(cno==CHGCURVE) {
			cno = CRTCURVE;
		}
		if(cno==CHGSWEEP) {
			cno = CRTSWEEP;
		}
		num1 = GetIntNum(cmp);											// �������Ұ����擾
		num2 = GetPntNum(cmp);											// ���W���Ұ����擾
		CreatCmd(cno, num1, num2, &cptr);								// ����ޗ̈�m��
		for(i=0; i<num1; ++i) {
			PutParaInt(cptr, i, GetParaInt(cmp, i));					// �������Ұ��ݒ�
		}
		for(i=0; i<num2; ++i) {
			GetParaPnt(cmp, i, &pd);									// ���W���Ұ��擾
			PutParaPnt(cptr, i, pd);									// ���W���Ұ��ݒ�
		}
		AppendCmd(optr, cptr);											// ����&����ސڑ�
		NextCmdPtr(&cmp);												// ������ނ�
	}
	return optr;
}

/***************/
void Copy(void)															//<<< ���̕���
/***************/
{
	OBJTYPE* op, * wop;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		SetObjPtr(op);													// ���ʑΏۗ��̐ݒ�
		wop = CopyObject();												// ���̕���
		SetObjPtr(wop);													// �����߲���ݒ�
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
	}
}

/*****************/
void Delete(void)														//<<< ���̍폜
/*****************/
{
	OBJTYPE* op, * dop;
	int      dsp, edt;

	BaseObjPtr(2, &op, &dsp, &edt);										// �����Ώۗ��̎擾
	while(op!=NULL) {
		dop = op;														// �폜�����߲���ۑ�
		NextObjPtr(2, &op, &dsp, &edt);									// �����̂�
		LevelObj(1, dop);												// �������ٍ폜���̐ڑ�
		TempObj(dop);													// ���̈ꎞ�폜
	}
}

/*******************************/
void InitPasteObject(int* clvl)											//<<< �����߰�ď�������
/*******************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	CPCTYPE* cpc;
	PNTTYPE  pp, zer, vec;
	BOXTYPE  box;
	int      i, cno, ino, pno, pi, lvl;

	zer.p[0] = 0.0, zer.p[1] = 0.0, zer.p[2] = 0.0;						// ��Ɨ̈揉����
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = -1.0;
	InitLevel();														// Undo/Redo������
	BaseCopyCmdPtr(&cpc);												// �擪��߰������߲���擾
	while(cpc!=NULL) {
		cno = GetCopyCmdNo(cpc);										// ��߰�����No�擾
		ino = GetCopyIntNum(cpc);										// ��߰����ސ�����א��擾
		pno = GetCopyPntNum(cpc);										// ��߰����ލ��W��א��擾
		CreatCmd(cno, ino, pno, &cmp);									// �ʏ����ޗ̈�m��
		for(i=0; i<ino; i++) {
			GetCopyParaInt(cpc, i, &pi);								// ��߰�������Ұ��擾
			PutParaInt(cmp, i, pi);										// �ʏ퐮�����Ұ��ݒ�
		}
		for(i=0; i<pno; i++) {
			GetCopyParaPnt(cpc, i, &pp);								// ��߰���W���Ұ��擾
			PutParaPnt(cmp, i, pp);										// �ʏ���W���Ұ��ݒ�
		}
		SetCmdPtr(cmp);													// �����Ώۺ���ސݒ�
		ExecCommand(FALSE);												// ����ގ��s
		if(cno==ENDCMD) {												// <�ŏI����ނ̏ꍇ>
			GetObjPtr(&op);												// ���̂��߲���擾
			GetObjOif(op, &oif);										// ���̍\������߲���擾
			SetOifName(0, oif, OBJ_NAME34);								// ���̍\�����ݒ�
			if(op->part==2&&op->oset!=NULL) {							// <�Ưĥ�q�̏ꍇ>
				GetObjBox(op->oset, &box);								// ����BOX�擾
			} else {													// <�Ưĥ�q�ȊO�̏ꍇ>
				GetObjBox(op, &box);									// ����BOX�擾
			}
			BoxCenter(box, &pp);										// �ޯ�������擾
			BaseCmdPtr(0, op, &cmp);									// ��������ގ擾
			lvl = GetCmdLvl(cmp);										// ���ݺ��������No�ۑ�
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��[����Z����]�p]
			PutParaInt(cmp, 0, 0);										// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, pp);										// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, vec);									// ��]�������޸�ِݒ�
			SetCmdLvl(cmp, lvl+1);										// �����������No�ύX(+1)
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
			CreatCmd(MOVE, 3, 0, &cmp);									// ����ޗ̈�m��[�ړ������p]
			PutParaInt(cmp, 0, 0);										// �ړ��ʐݒ�
			PutParaInt(cmp, 1, 0);
			PutParaInt(cmp, 2, 0);
			SetCmdLvl(cmp, lvl+2);										// �����������No�ύX(+2)
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// �ړ�����ގ��s
			CreatCmd(ROTATE, 1, 2, &cmp);								// ����ޗ̈�m��[��]�����p]
			PutParaInt(cmp, 0, 0);										// ��]�p�x�ݒ�
			PutParaPnt(cmp, 0, zer);									// ��]�����W�ݒ�
			PutParaPnt(cmp, 1, vec);									// ��]�������޸�ِݒ�
			SetCmdLvl(cmp, lvl+3);										// �����������No�ύX(+3)
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(FALSE);											// ��]����ގ��s
			OnEdtFlg(1, op);											// ����edtflg:ON
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
		NextCopyCmdPtr(&cpc);											// ����߰����ނ�
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
	MainWnd->SetGroupObj();												// ��ٰ�ߗ��̐ݒ�
	*clvl = lvl;
}

/*****************************/
void EndPasteObject(int clvl)											//<<< �����߰�ďI������
/*****************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp, * dcmp;
	int      del, dx, dy, dz, th;

	LevelBaseObjPtr(0, &op);											// �������̌���
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// �擪����ގ擾
		while(cmp!=NULL) {
			del = 0;
			if(GetCmdLvl(cmp)==clvl+1) {								// <����Z����]�p�����>
				th = GetParaInt(cmp, 0);								// ��]�p�x�擾
				if(th==0) {
					del = 1;
				}
				SetCmdLvl(cmp, clvl);
			} else if(GetCmdLvl(cmp)==clvl+2) {							// <�ړ������p�����>
				dx = GetParaInt(cmp, 0);								// �ړ��ʎ擾
				dy = GetParaInt(cmp, 1);
				dz = GetParaInt(cmp, 2);
				if(dx==0&&dy==0&&dz==0) {
					del = 1;
				}
				SetCmdLvl(cmp, clvl);
			} else if(GetCmdLvl(cmp)==clvl+3) {							// <��]�����p�����>
				th = GetParaInt(cmp, 0);								// ��]�p�x�擾
				if(th==0) {
					del = 1;
				}
				SetCmdLvl(cmp, clvl);
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

/*******************************************************/
void EditPasteObject1(int clvl, PNTTYPE mov, double th,
                      PNTTYPE cpt, PNTTYPE vec)							//<<< �����߰�Ĕz�u(�ړ����])
/*******************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// �������̂̌���
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// �擪����ގ擾
		while(cmp!=NULL) {
			if(GetCmdLvl(cmp)==clvl+2) {								// <�ړ������p�����>
				PutParaInt(cmp, 0, (int)(mov.p[0]*1000.0));				// �ړ��ʐݒ�
				PutParaInt(cmp, 1, (int)(mov.p[1]*1000.0));
				PutParaInt(cmp, 2, (int)(mov.p[2]*1000.0));
			} else if(GetCmdLvl(cmp)==clvl+3) {							// <��]�����p�����>
				PutParaInt(cmp, 0, (int)(th*1000.0));					// ��]�p�x�ݒ�
				PutParaPnt(cmp, 0, cpt);								// ��]�����W�ݒ�
				PutParaPnt(cmp, 1, vec);								// ��]�������޸�ِݒ�
			}
			NextCmdPtr(&cmp);											// ������ނ�
		}
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextObjPtr(0, &op);										// ���������̌���
    }
}

/******************************************/
void EditPasteObject2(int clvl, double th)								//<<< �����߰�Z����]
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// �������̂̌���
	while(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// �擪����ގ擾
		while(cmp!=NULL) {
			if(GetCmdLvl(cmp)==clvl+1) {								// <Z����]�p�����>
				PutParaInt(cmp, 0, (int)(th*1000.0));					// ��]�p�x�ݒ�
			}
			NextCmdPtr(&cmp);											// ������ނ�
		}
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextObjPtr(0, &op);										// ���������̌���
    }
}
