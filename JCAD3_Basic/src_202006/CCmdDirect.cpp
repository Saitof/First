/////////////////////////////////////////////////////////////////////////////
// ���̐��� ���ڐ����R�}���h��
// CCmdDirect.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataAccess1.h"
#include "DataAccess2.h"

/*************************/
OBJTYPE* MakeDirect(void)												//<<< ���ڐ��������
/*************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      i, vnum, lnum, vno;

	GetCmdPtr(&cmp);													// �����Ώۺ���ގ擾
	vnum = GetParaInt(cmp, 0);											// ���_���擾
	lnum = GetParaInt(cmp, 1);											// ��ٰ�ߐ��擾
	CreatObj(vnum, lnum, 0, &op);										// ���̗̈�m��
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	LevelObj(0, op);													// �������ِ������̐ڑ�
	if(!GetCmdExeStp()) {												// <<<����ގ��s��~�Ȃ�>>>
		for(i=0; i<vnum; i++) {
			GetParaPnt(cmp, i, &pnt);									// ���W���Ұ��擾
			PutVtx(op, i, pnt);											// ���_�ݒ�
		}
		for(i=0; i<lnum; i++) {
			vno = GetParaInt(cmp, i+2);									// �������Ұ��擾
			if(vno>0) {
				PutFlp1(op, i, vno-1, 0);								// ��ٰ�ߏ��ݒ�
			} else {
				PutFlp1(op, i, -vno-1, 1);
			}
		}
	}
	PutObjAtr(op, 0);													// ����No�ݒ�(DIRECT)
	PutObjPrt(op, 0, 0);												// �߰�No�ݒ�(�ʏ�)
	PutObjCol(op, 0);													// �װNo�ݒ�(�ް���)
	if(!GetCmdExeStp()&&!GetResetObjMode()) {							// <����ޒ�~��&RedoCommand�ȊO>
		SetObjBox(op);													// �ޯ�����ސݒ�
	}
	return op;
}
