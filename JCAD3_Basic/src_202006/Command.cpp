/////////////////////////////////////////////////////////////////////////////
// �R�}���h�N�����䕔
// Command.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"

typedef  struct  cmdst {
	OBJTYPE* (*comfunc)();
} CTBTYPE;																// �����ð���

CTBTYPE Cmdtbl[] = {
	{ MakeSurface },													// �Ȗʐ���
	{ MakeDirect },														// ���ڐ���
	{ MakeCurve },														// �Ȑ�����
	{ ChangeSurface },													// �ȖʕҏW
	{ ChangeCurve },													// �Ȑ��ҏW
	{ SetSurface },														// �Ȗʐ���_����
	{ SetCurve },														// �Ȑ�����_����
	{ ThicknessObject },												// �Ȗʌ��݂Â�
	{ MoveObject },														// ���s�ړ�
	{ RotateObject },													// ��]�ړ�
	{ MirrorObject },													// �����ړ�
	{ ScaleObject },													// ���ޕύX
	{ BendObject },														// �~���Ȃ�
	{ SetGroup },														// ��ٰ�ߐݒ�
	{ SetColor },														// �װNo�ݒ�
	{ SetAttribute },													// ����No�ݒ�
	{ MakeSweepSurf },													// �|���Ȗʐ���
	{ ChangeSweepSurf },												// �|���ȖʕҏW
	{ TwistObject },													// �Ђ˂�
	{ ResizeObject1 },													// �����w��ػ���1
	{ SetParts },														// �߰�No�ݒ�
	{ ResizeObject2 },													// �Ȑ��w��ػ���1
	{ ShearObject },													// ����f
	{ RingObject },														// �ݸ�
	{ EarthObject },													// ���
	{ SpaceObject },													// ��߰�
	{ ResizeObject3 },													// �����w��ػ���2
	{ ResizeObject4 },													// �Ȑ��w��ػ���2
	{ CommandError }
};

/**************************/
void ExecCommand(BOOL stp)												//<<< ����ގ��s
/**************************/
{
	CMDTYPE* cmp;
	CTBTYPE* pc;
	OBJTYPE* op;
	int      cno;

	GetCmdPtr(&cmp);													// ������߲���擾
	cno = GetCmdNo(cmp);												// �����No�擾
	cno = (cno>MAXCMD) ? MAXCMD+1 : cno;
	pc = &Cmdtbl[cno];
	if(stp) {															// <����ގ��s��~����>
		OnCmdExeStp();													// ����ގ��s��~�׸�ON
		op = (*pc->comfunc)();											// ����ދN��
		OffCmdExeStp();													// ����ގ��s��~�׸�OFF
	} else {															// <����ގ��s��~�Ȃ�>
		op = (*pc->comfunc)();											// ����ދN��
	}
	if(op!=NULL) {
		SetObjPtr(op);													// �߲�����
		OnCmdFlg();														// ����ޖ��ۑ�ON
	}
}

/***************************/
OBJTYPE* CommandError(void)												//<<< ����޴װ����
/***************************/
{
	AfxMessageBox(IDS_ERR_COMMAND);										// �װү���ޏo��
	return NULL;
}

/*****************************/
void RedoCommand(OBJTYPE* op)											//<<< ����ލĎ��s
/*****************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ؾ�ė��̓o�^
	BaseCmdPtr(1, op, &cmp);											// �L������ގ擾
	while(cmp!=NULL) {
		SetCmdPtr(cmp);													// �����Ώۺ���ސݒ�
		SetObjPtr(op);													// �����Ώۗ��̓o�^
		ExecCommand(FALSE);												// ����ދN��
		NextCmdPtr(&cmp);												// ������ގ擾
	}
	OffResetObj();														// ؾ�ė��̉���
	SetObjBox(op);														// �ޯ�����ސݒ�
}

/**********************************/
void RedoStartCommand(OBJTYPE* op)										//<<< �ƭ��J�n������ލĎ��s
/**********************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ؾ�ė��̓o�^
	BaseCmdPtr(1, op, &cmp);											// �L������ގ擾
	while(cmp!=NULL) {
		if(cmp->onxt!=NULL) {											// <�ŏI(���ݎ��s��)����ވȊO>
			SetCmdPtr(cmp);												// �����Ώۺ���ސݒ�
			SetObjPtr(op);												// �����Ώۗ��̓o�^
			ExecCommand(FALSE);											// ����ދN��
		}
		NextCmdPtr(&cmp);												// ������ގ擾
	}
	OffResetObj();														// ؾ�ė��̉���
	SetObjBox(op);														// �ޯ�����ސݒ�
}

/***********************************/
void RedoCreateCommand(OBJTYPE* op)										//<<< ��������ލĎ��s
/***********************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ؾ�ė��̓o�^
	BaseCmdPtr(1, op, &cmp);											// �L������ގ擾
	if(cmp!=NULL) {
		SetCmdPtr(cmp);													// �����Ώۺ���ސݒ�
		SetObjPtr(op);													// �����Ώۗ��̓o�^
		ExecCommand(FALSE);												// ����ދN��
	}
	OffResetObj();														// ؾ�ė��̉���
	SetObjBox(op);														// �ޯ�����ސݒ�
}

/***************************************/
void RedoRingBeforeCommand(OBJTYPE* op)									//<<< �ݸ޻��ޑO����ލĎ��s
/***************************************/
{
	CMDTYPE* cmp;

	OnResetObj(op);														// ؾ�ė��̓o�^
	BaseCmdPtr(1, op, &cmp);											// �L������ގ擾
	while(cmp!=NULL) {
		if(cmp==op->rccmp) {											// �ݸތ��ݺ���ނ͏������~
			break;
		}
		SetCmdPtr(cmp);													// �����Ώۺ���ސݒ�
		SetObjPtr(op);													// �����Ώۗ��̓o�^
		ExecCommand(FALSE);												// ����ދN��
		NextCmdPtr(&cmp);												// ������ގ擾
	}
	OffResetObj();														// ؾ�ė��̉���
	SetObjBox(op);														// �ޯ�����ސݒ�
}

/***********************************************/
void RedoCommandCopy(OBJTYPE* op, CMDTYPE* cmp)							//<<< ����ޕ��ʎ��s
/***********************************************/
{
	CMDTYPE* cptr;
	PNTTYPE  pd;
	int      cno, i, inum, pnum;

	cno = GetCmdNo(cmp);												// �����No�擾
	inum = GetIntNum(cmp);												// �������Ұ����擾
	pnum = GetPntNum(cmp);												// ���W���Ұ����擾
	CreatCmd(cno, inum, pnum, &cptr);									// ����ޗ̈�m��
	for(i=0; i<inum; i++) {
		PutParaInt(cptr, i, GetParaInt(cmp, i));						// �������Ұ��ݒ�
	}
	for(i=0; i<pnum; i++) {
		GetParaPnt(cmp, i, &pd);										// ���W���Ұ��擾
		PutParaPnt(cptr, i, pd);										// ���W���Ұ��ݒ�
	}
	SetObjPtr(op);														// �Ώۗ��̐ݒ�
	SetCmdPtr(cptr);													// ����ސݒ�
	ExecCommand(FALSE);													// ����ދN��
}
