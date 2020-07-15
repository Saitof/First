/////////////////////////////////////////////////////////////////////////////
// ���̐����R�}���h��
// CCommand.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"

/**********************************************************************/
void CrtCurve(int pn, int bc, int mx, double wt, PNTTYPE* pa, int* ea)	//<<< �Ȑ�����
/**********************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	CreatCmd(CRTCURVE, 4+pn, pn, &cmp);									// ����ޗ̈�m��
	PutParaInt(cmp, 0, pn);												// ����_���ݒ�
	PutParaInt(cmp, 1, bc);												// ���EӰ�ސݒ�
	PutParaInt(cmp, 2, mx);												// �Ȑ����J�Ԑ�
	PutParaInt(cmp, 3, (int)(wt*1000.0));								// �Ȑ��d�݌W��
	for(i=0; i<pn; i++) {
		PutParaInt(cmp, 4+i, ea[i]);									// ײ�Ӱ�ސݒ�
		PutParaPnt(cmp, i, pa[i]);										// ����_�ݒ�
	}
	SetCmdPtr(cmp);														// ����ސݒ�
	ExecCommand(FALSE);													// ����ގ��s
	GetObjPtr(&op);														// �Ώۗ��̎擾
	View->SetDispList(op);												// �ި���ڲ�ؽēo�^
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*******************************************************/
void RedoCrtCurve(int pn, int bc, PNTTYPE* pa, int* ea)					//<<< �Ȑ�����
/*******************************************************/				//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn, sbc;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTCURVE) {									// <�Ȑ�����>
			ChangeCmd(cmp, 4+pn, pn);									// ����ޗ̈�ύX
			spn = GetParaInt(cmp, 0);									// ����_���擾
			PutParaInt(cmp, 0, pn);										// ����_���ݒ�
			sbc = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			PutParaInt(cmp, 1, bc);										// ���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 4+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(pn!=spn||bc!=sbc) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/*************************************************************/
void ChgCurve(OBJTYPE* so, int pn, int bc, int mx, double wt,
              PNTTYPE* pa, int* ea)										//<<< �Ȑ��ύX
/*************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, i;

	if(so==NULL) {														// <���̎w��Ȃ�>
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
	} else {															// <���̎w�肠��>
		op = so;														// �w�藧��
	}
	if(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// �擪����ގ擾
		if(GetCmdNo(cmp)==CRTCURVE) {									// <�Ȑ�����>
			CreatCmd(CHGCURVE, 4+pn, pn, &cmp);							// ����ޗ̈�m��
			PutParaInt(cmp, 0, pn);										// ����_���ݒ�
			PutParaInt(cmp, 1, bc);										// ���EӰ�ސݒ�
			PutParaInt(cmp, 2, mx);										// �Ȑ����J�Ԑ�
			PutParaInt(cmp, 3, (int)(wt*1000.0));						// �Ȑ��d�݌W��
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 4+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			SetObjPtr(op);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(TRUE);											// ����ގ��s
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*******************************************************/
void RedoChgCurve(int pn, int bc, PNTTYPE* pa, int* ea)					//<<< �Ȑ��ύX
/*******************************************************/				//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn, sbc;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CHGCURVE) {									// <�Ȑ��ύX>
			ChangeCmd(cmp, 4+pn, pn);									// ����ޗ̈�ύX
			spn = GetParaInt(cmp, 0);									// ����_���擾
			PutParaInt(cmp, 0, pn);										// ����_���ݒ�
			sbc = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			PutParaInt(cmp, 1, bc);										// ���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 4+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(pn!=spn||bc!=sbc) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/******************************************/
void SetCurve(int pn, int bc, PNTTYPE* pa)								//<<< �Ȑ�����_����
/******************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	CreatCmd(SETCURVE, 2, pn, &cmp);									// ����ޗ̈�m��
	PutParaInt(cmp, 0, pn);												// ����_���ݒ�
	PutParaInt(cmp, 1, bc);												// ���EӰ�ސݒ�
	for(i=0; i<pn; i++) {
		PutParaPnt(cmp, i, pa[i]);										// ����_�ݒ�
	}
	SetCmdPtr(cmp);														// ����ސݒ�
	ExecCommand(FALSE);													// ����ގ��s
	GetObjPtr(&op);														// �Ώۗ��̎擾
	View->SetDispList(op);												// �ި���ڲ�ؽēo�^
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/**********************************************/
void RedoSetCurve(int pn, int bc, PNTTYPE* pa)							//<<< �Ȑ�����_����
/**********************************************/						//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, spn, sbc;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SETCURVE) {									// <�Ȑ�����_����>
			ChangeCmd(cmp, 2, pn);										// ����ޗ̈�ύX
			spn = GetParaInt(cmp, 0);									// ����_���擾
			PutParaInt(cmp, 0, pn);										// ����_���ݒ�
			sbc = GetParaInt(cmp, 1);									// ���EӰ�ގ擾
			PutParaInt(cmp, 1, bc);										// ���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(pn!=spn||bc!=sbc) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/********************************************************/
void CrtSurface(int un, int vn, int ub, int vb, int mx,
                double wt, int fg, PNTTYPE* pa, int* ea)				//<<< �Ȗʐ���
/********************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn;

	pn = un*vn;															// �����_��
	CreatCmd(CRTSURF, 7+pn, pn, &cmp);									// ����ޗ̈�m��
	PutParaInt(cmp, 0, un);												// U����_���ݒ�
	PutParaInt(cmp, 1, vn);												// V����_���ݒ�
	PutParaInt(cmp, 2, ub);												// U���EӰ�ސݒ�
	PutParaInt(cmp, 3, vb);												// V���EӰ�ސݒ�
	PutParaInt(cmp, 4, mx);												// �Ȑ����J�Ԑ�
	PutParaInt(cmp, 5, (int)(wt*1000.0));								// �Ȑ��d�݌W��
	PutParaInt(cmp, 6, fg);												// �د��Ӱ��
	for(i=0; i<pn; i++) {
		PutParaInt(cmp, 7+i, ea[i]);									// ײ�Ӱ�ސݒ�
		PutParaPnt(cmp, i, pa[i]);										// ����_�ݒ�
	}
	SetCmdPtr(cmp);														// ����ސݒ�
	ExecCommand(FALSE);													// ����ގ��s
	GetObjPtr(&op);														// �Ώۗ��̎擾
	View->SetDispList(op);												// �ި���ڲ�ؽēo�^
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***************************************************/
void RedoCrtSurface(int un, int vn, int ub, int vb,
                    PNTTYPE* pa, int* ea)								//<<< �Ȗʐ���
/***************************************************/					//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTSURF) {									// <�Ȗʐ���>
			pn = un*vn;													// �����_��
			ChangeCmd(cmp, 7+pn, pn);									// ����ޗ̈�ύX
			sun = GetParaInt(cmp, 0);									// U����_���擾
			PutParaInt(cmp, 0, un);										// U����_���ݒ�
			svn = GetParaInt(cmp, 1);									// V����_���擾
			PutParaInt(cmp, 1, vn);										// V����_���ݒ�
			sub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			PutParaInt(cmp, 2, ub);										// U���EӰ�ސݒ�
			svb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			PutParaInt(cmp, 3, vb);										// V���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 7+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/************************************************************/
void RedoCrtSurface2(int no, int un, int vn, int ub, int vb,
                     PNTTYPE* pa, int* ea)								//<<< �Ȗʐ���2
/************************************************************/			//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb, cnt=0;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTSURF) {									// <�Ȗʐ���>
			if(no==cnt) {
				pn = un*vn;												// �����_��
				ChangeCmd(cmp, 7+pn, pn);								// ����ޗ̈�ύX
				sun = GetParaInt(cmp, 0);								// U����_���擾
				PutParaInt(cmp, 0, un);									// U����_���ݒ�
				svn = GetParaInt(cmp, 1);								// V����_���擾
				PutParaInt(cmp, 1, vn);									// V����_���ݒ�
				sub = GetParaInt(cmp, 2);								// U���EӰ�ގ擾
				PutParaInt(cmp, 2, ub);									// U���EӰ�ސݒ�
				svb = GetParaInt(cmp, 3);								// V���EӰ�ގ擾
				PutParaInt(cmp, 3, vb);									// V���EӰ�ސݒ�
				for(i=0; i<pn; i++) {
					PutParaInt(cmp, 7+i, ea[i]);						// ײ�Ӱ�ސݒ�
					PutParaPnt(cmp, i, pa[i]);							// ����_�ݒ�
				}
				GetCmdObj(cmp, &op);									// �ҏW�Ώۗ��̎擾
				RedoCommand(op);										// ����ލĎ��s
				View->SetDispList(op);									// �ި���ڲ�ؽēo�^
				break;
			}
			cnt++;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/************************************************************/
void ChgSurface(OBJTYPE* so, int un, int vn, int ub, int vb,
                int mx, double wt, PNTTYPE* pa, int* ea)				//<<< �ȖʕύX
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, pn, sm, i;

	if(so==NULL) {														// <���̎w��Ȃ�>
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
	} else {															// <���̎w�肠��>
		op = so;														// �w�藧��
	}
	if(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// �擪����ގ擾
		if(GetCmdNo(cmp)==CRTSURF) {									// <�Ȗʐ���>
			sm = GetParaInt(cmp, 6);									// �د��Ӱ�ގ擾
			pn = un*vn;													// �����_��
			CreatCmd(CHGSURF, 7+pn, pn, &cmp);							// ����ޗ̈�m��
			PutParaInt(cmp, 0, un);										// U����_���ݒ�
			PutParaInt(cmp, 1, vn);										// V����_���ݒ�
			PutParaInt(cmp, 2, ub);										// U���EӰ�ސݒ�
			PutParaInt(cmp, 3, vb);										// V���EӰ�ސݒ�
			PutParaInt(cmp, 4, mx);										// �Ȑ����J�Ԑ�
			PutParaInt(cmp, 5, (int)(wt*1000.0));						// �Ȑ��d�݌W��
			PutParaInt(cmp, 6, sm);										// �د��Ӱ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 7+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			SetObjPtr(op);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(TRUE);											// ����ގ��s
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***************************************************/
void RedoChgSurface(int un, int vn, int ub, int vb,
                    PNTTYPE* pa, int* ea)								//<<< �ȖʕύX
/***************************************************/					//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CHGSURF) {									// <�ȖʕύX>
			pn = un*vn;													// �����_��
			ChangeCmd(cmp, 7+pn, pn);									// ����ޗ̈�ύX
			sun = GetParaInt(cmp, 0);									// U����_���擾
			PutParaInt(cmp, 0, un);										// U����_���ݒ�
			svn = GetParaInt(cmp, 1);									// V����_���擾
			PutParaInt(cmp, 1, vn);										// V����_���ݒ�
			sub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			PutParaInt(cmp, 2, ub);										// U���EӰ�ސݒ�
			svb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			PutParaInt(cmp, 3, vb);										// V���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 7+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/************************************************************/
void SetSurface(int un, int vn, int ub, int vb, PNTTYPE* pa)			//<<< �Ȗʐ���_����
/************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn;

	pn = un*vn;															// �����_��
	CreatCmd(SETSURF, 4, pn, &cmp);										// ����ޗ̈�m��
	PutParaInt(cmp, 0, un);												// U����_���ݒ�
	PutParaInt(cmp, 1, vn);												// V����_���ݒ�
	PutParaInt(cmp, 2, ub);												// U���EӰ�ސݒ�
	PutParaInt(cmp, 3, vb);												// V���EӰ�ސݒ�
	for(i=0; i<pn; i++) {
		PutParaPnt(cmp, i, pa[i]);										// ����_�ݒ�
	}
	SetCmdPtr(cmp);														// ����ސݒ�
	ExecCommand(FALSE);													// ����ގ��s
	GetObjPtr(&op);														// �Ώۗ��̎擾
	View->SetDispList(op);												// �ި���ڲ�ؽēo�^
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/****************************************************************/
void RedoSetSurface(int un, int vn, int ub, int vb, PNTTYPE* pa)		//<<< �Ȗʐ���_����
/****************************************************************/		//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==SETSURF) {									// <�Ȗʐ���_����>
			pn = un*vn;													// �����_��
			ChangeCmd(cmp, 4, pn);										// ����ޗ̈�ύX
			sun = GetParaInt(cmp, 0);									// U����_���擾
			PutParaInt(cmp, 0, un);										// U����_���ݒ�
			svn = GetParaInt(cmp, 1);									// V����_���擾
			PutParaInt(cmp, 1, vn);										// V����_���ݒ�
			sub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			PutParaInt(cmp, 2, ub);										// U���EӰ�ސݒ�
			svb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			PutParaInt(cmp, 3, vb);										// V���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/***************************************/
void Thickness(OBJTYPE* op, double len)									//<<< �Ȗʌ��݂Â�
/***************************************/
{
	CMDTYPE* cmp;

	CreatCmd(THICKNESS, 1, 0, &cmp);									// ����ޗ̈�m��
	PutParaInt(cmp, 0, (int)(len*1000.0));								// ���ݕ��ݒ�
	SetObjPtr(op);														// �Ώۗ��̐ݒ�
	SetCmdPtr(cmp);														// ����ސݒ�
	ExecCommand(FALSE);													// ����ގ��s
	GetObjPtr(&op);														// �Ώۗ��̎擾
	View->SetDispList(op);												// �ި���ڲ�ؽēo�^
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/***********************************/
void MakeDirectCommand(OBJTYPE* op)										//<<< ���ڐ�������ލ쐬
/***********************************/
{
	OIFTYPE* oif;
	CMDTYPE* cmp;
	PNTTYPE  pnt;
	int      vnum, lnum, vno, flg, i;

	vnum = GetVtxNum(op);												// ���_���擾
	lnum = GetFlpNum1(op);												// ��ٰ�ߐ��擾
	CreatCmd(DIRECT, lnum+2, vnum, &cmp);								// ����ޗ̈�擾
	for(i=0; i<vnum; i++) {
		GetVtx(op, i, &pnt);											// ���_�擾
		PutParaPnt(cmp, i, pnt);										// ���W���Ұ��ݒ�
	}
	PutParaInt(cmp, 0, vnum);											// ���_���ݒ�
	PutParaInt(cmp, 1, lnum);											// ��ٰ�ߐ��ݒ�
	for(i=0; i<lnum; i++) {
		GetFlp1(op, i, &vno, &flg);										// ��ٰ�ߏ��擾
		if(flg) {
			PutParaInt(cmp, i+2, -(vno+1));								// �������Ұ��ݒ�
		} else {
			PutParaInt(cmp, i+2, vno+1);
		}
	}
	AppendCmd(op, cmp);													// ����&����ސڑ�
	LevelCmd(cmp);														// �������ٺ���ސڑ�
	SetObjPtr(op);														// �����Ώۗ��̐ݒ�
	GetObjOif(op, &oif);												// ���̍\������߲���擾
	SetOifName(0, oif, OBJ_NAME33);										// �\����(���ڐ�������)
	OnCmdFlg();															// ����ޖ��ۑ�ON
}

/************************************/
void RedoDirect(int pn, PNTTYPE* pa)									//<<< ���ڐ���
/************************************/									//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i;

	GetObjPtr(&op);														// �����߲���擾
	BaseCmdPtr(0, op, &cmp);											// �擪������߲���擾
	if(cmp!=NULL) {
		if(GetCmdNo(cmp)==DIRECT&&GetPntNum(cmp)==pn) {					// <���ں����&���W���Ұ�=>
			for(i=0; i<pn; i++) {
				PutParaPnt(cmp, i, pa[i]);								// ���W���Ұ��X�V
			}
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
	}
}

/******************************************************************/
void CrtSweepSurf(int un, int vn, int ub, int vb, int mx,
                  double wt, int sm, int pl, PNTTYPE* pa, int* ea)		//<<< �|���Ȗʐ���
/******************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn;

	pn = un+vn;															// �����_��
	CreatCmd(CRTSWEEP, 8+pn, pn, &cmp);									// ����ޗ̈�m��
	PutParaInt(cmp, 0, un);												// U����_���ݒ�
	PutParaInt(cmp, 1, vn);												// V����_���ݒ�
	PutParaInt(cmp, 2, ub);												// U���EӰ�ސݒ�
	PutParaInt(cmp, 3, vb);												// V���EӰ�ސݒ�
	PutParaInt(cmp, 4, mx);												// �Ȑ����J�Ԑ�
	PutParaInt(cmp, 5, (int)(wt*1000.0));								// �Ȑ��d�݌W��
	PutParaInt(cmp, 6, sm);												// �د��Ӱ��
	PutParaInt(cmp, 7, pl);												// U���ʐݒ�
	for(i=0; i<pn; i++) {
		PutParaInt(cmp, 8+i, ea[i]);									// ײ�Ӱ�ސݒ�
		PutParaPnt(cmp, i, pa[i]);										// ����_�ݒ�
	}
	SetCmdPtr(cmp);														// ����ސݒ�
	ExecCommand(FALSE);													// ����ގ��s
	GetObjPtr(&op);														// �Ώۗ��̎擾
	View->SetDispList(op);												// �ި���ڲ�ؽēo�^
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*****************************************************/
void RedoCrtSweepSurf(int un, int vn, int ub, int vb,
                      PNTTYPE* pa, int* ea)								//<<< �|���Ȗʐ���
/*****************************************************/					//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CRTSWEEP) {									// <�|���Ȗʐ���>
			pn = un+vn;													// �����_��
			ChangeCmd(cmp, 8+pn, pn);									// ����ޗ̈�ύX
			sun = GetParaInt(cmp, 0);									// U����_���擾
			PutParaInt(cmp, 0, un);										// U����_���ݒ�
			svn = GetParaInt(cmp, 1);									// V����_���擾
			PutParaInt(cmp, 1, vn);										// V����_���ݒ�
			sub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			PutParaInt(cmp, 2, ub);										// U���EӰ�ސݒ�
			svb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			PutParaInt(cmp, 3, vb);										// V���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 8+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}

/**************************************************************/
void ChgSweepSurf(OBJTYPE* so, int un, int vn, int ub, int vb,
                  int mx, double wt, PNTTYPE* pa, int* ea)				//<<< �|���ȖʕύX
/**************************************************************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      dsp, edt, pn, sm, pl, i;

	if(so==NULL) {														// <���̎w��Ȃ�>
		BaseObjPtr(2, &op, &dsp, &edt);									// �����Ώۗ��̎擾
	} else {															// <���̎w�肠��>
		op = so;														// �w�藧��
	}
	if(op!=NULL) {
		BaseCmdPtr(0, op, &cmp);										// �擪����ގ擾
		if(GetCmdNo(cmp)==CRTSWEEP) {									// <�|���Ȗʐ���>
			sm = GetParaInt(cmp, 6);									// �د��Ӱ�ގ擾
			pl = GetParaInt(cmp, 7);									// U���ʎ擾
			pn = un+vn;													// �����_��
			CreatCmd(CHGSWEEP, 8+pn, pn, &cmp);							// ����ޗ̈�m��
			PutParaInt(cmp, 0, un);										// U����_���ݒ�
			PutParaInt(cmp, 1, vn);										// V����_���ݒ�
			PutParaInt(cmp, 2, ub);										// U���EӰ�ސݒ�
			PutParaInt(cmp, 3, vb);										// V���EӰ�ސݒ�
			PutParaInt(cmp, 4, mx);										// �Ȑ����J�Ԑ�
			PutParaInt(cmp, 5, (int)(wt*1000.0));						// �Ȑ��d�݌W��
			PutParaInt(cmp, 6, sm);										// �د��Ӱ�ސݒ�
			PutParaInt(cmp, 7, pl);										// U���ʐݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 8+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			SetObjPtr(op);												// �Ώۗ��̐ݒ�
			SetCmdPtr(cmp);												// ����ސݒ�
			ExecCommand(TRUE);											// ����ގ��s
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
		}
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/*****************************************************/
void RedoChgSweepSurf(int un, int vn, int ub, int vb,
                      PNTTYPE* pa, int* ea)								//<<< �|���ȖʕύX
/*****************************************************/					//    (�Ď��s)
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, pn, sun, svn, sub, svb;

	LevelBaseCmdPtr(&cmp);												// �����Ώۺ���ގ擾
	while(cmp!=NULL) {
		if(GetCmdNo(cmp)==CHGSWEEP) {									// <�|���ȖʕύX>
			pn = un+vn;													// �����_��
			ChangeCmd(cmp, 8+pn, pn);									// ����ޗ̈�ύX
			sun = GetParaInt(cmp, 0);									// U����_���擾
			PutParaInt(cmp, 0, un);										// U����_���ݒ�
			svn = GetParaInt(cmp, 1);									// V����_���擾
			PutParaInt(cmp, 1, vn);										// V����_���ݒ�
			sub = GetParaInt(cmp, 2);									// U���EӰ�ގ擾
			PutParaInt(cmp, 2, ub);										// U���EӰ�ސݒ�
			svb = GetParaInt(cmp, 3);									// V���EӰ�ގ擾
			PutParaInt(cmp, 3, vb);										// V���EӰ�ސݒ�
			for(i=0; i<pn; i++) {
				PutParaInt(cmp, 8+i, ea[i]);							// ײ�Ӱ�ސݒ�
				PutParaPnt(cmp, i, pa[i]);								// ����_�ݒ�
			}
			GetCmdObj(cmp, &op);										// �ҏW�Ώۗ��̎擾
			RedoCommand(op);											// ����ލĎ��s
			View->SetDispList(op);										// �ި���ڲ�ؽēo�^
			break;
		}
		LevelNextCmdPtr(&cmp);											// ������ނ�
	}
	if(un!=sun||vn!=svn||ub!=sub||vb!=svb) {
		View->SetDataUse();												// �ް��g�p���ݒ�
	}
}
