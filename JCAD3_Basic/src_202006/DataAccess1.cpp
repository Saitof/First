/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X���암
// DataAccess1.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataBase.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "Command.h"

/**********************************************************/
/*****                   �̈揉����                   *****/
/**********************************************************/
/**************************/
void ReadMemDataFile(void)												//<<< ��ؗ̈��ް��ۑ�̧�ٓǍ�
/**************************/
{
	FILE*   fp;
	CString path;
	int     er=0, upd=0;

	path = GetAppPath(0)+MES_NAME;										// ��ؗ̈��ް��ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "r")==0) {									// ̧��OPEN
		while (TRUE) {
			if(fscanf_s(fp, "%d%d%d", &Int_max, &Pnt_max, &Undo_max)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &CCtl_max, &SCtl_max)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d%d", &CCtlA_max, &SCtlA_max, &WCtl_max)!=3) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &CPnt_max, &SPnt_max)!=2) {
				er = 1; break;
			}
			if(fscanf_s(fp, "%d%d", &OPnt_max, &BPnt_max)!=2) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
		if(er) {
			SetSplashMsg(IDS_CANT_READ_FILE);							// ��ް�̧�ق�Ǎ�NG�
		}
	} else {
		upd = 1;														// ̧�قȂ�
	}
	if(upd||er) {														// <�����l�ݒ�>
		Int_max   = MAXINTAN, Pnt_max  = MAXPNTAN, Undo_max  = MAXUNDON;
		CCtl_max  = MAXCCTLN, SCtl_max = MAXSCTLN, CCtlA_max = MAXCCTLA;
		SCtlA_max = MAXSCTLA, WCtl_max = MAXWCTLN, CPnt_max  = MAXCPNTN;
		SPnt_max  = MAXSPNTN, OPnt_max = MAXOPNTN, BPnt_max  = MAXBPNTN;
		WriteMemDataFile();												// ��ؗ̈��ް��ۑ�̧�ُ���
	}
}

/***************************/
void WriteMemDataFile(void)												//<<< ��ؗ̈��ް��ۑ�̧�ُ���
/***************************/
{
	FILE*   fp;
	CString path;
	int     er=0;

	path = GetAppPath(0)+MES_NAME;										// ��ؗ̈��ް��ۑ�̧�ٖ�
	if(fopen_s(&fp, path, "w")==0) {									// ̧��OPEN
		while (TRUE) {
			if(fprintf(fp, "%d %d %d\n", Int_max, Pnt_max, Undo_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", CCtl_max, SCtl_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d %d\n", CCtlA_max, SCtlA_max, WCtl_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", CPnt_max, SPnt_max)<0) {
				er = 1; break;
			}
			if(fprintf(fp, "%d %d\n", OPnt_max, BPnt_max)<0) {
				er = 1; break;
			}
			break;
		}
		fclose(fp);														// ̧��CLOSE
	} else {
		er = 1;															// ̧�قȂ�
	}
	if(er) {
		AfxMessageBox(IDS_ERR_FILEWRITE);								// �̧�ٕۑ�NG�
	}
}

/*******************/
int GetIntMax(void)														//<<< �������Ұ��z�񐔎擾
/*******************/
{
	return Int_max;
}

/***********************/
void SetIntMax(int max)													//<<< �������Ұ��z�񐔐ݒ�
/***********************/
{
	Int_max = max;
}

/*******************/
int GetPntMax(void)														//<<< ���W���Ұ��z�񐔎擾
/*******************/
{
	return Pnt_max;
}

/***********************/
void SetPntMax(int max)													//<<< ���W���Ұ��z�񐔐ݒ�
/***********************/
{
	Pnt_max = max;
}

/********************/
int GetUndoMax(void)													//<<< Undo/Redo���Ұ��z�񐔎擾
/********************/
{
	return Undo_max;
}

/************************/
void SetUndoMax(int max)												//<<< Undo/Redo���Ұ��z�񐔐ݒ�
/************************/
{
	Undo_max = max;
}

/********************/
int GetCCtlMax(void)													//<<< �ő�Ȑ�����_���擾
/********************/
{
	return CCtl_max;
}

/************************/
void SetCCtlMax(int max)												//<<< �ő�Ȑ�����_���ݒ�
/************************/
{
	CCtl_max = max, CCtlA_max = max + 2;
}

/********************/
int GetCCtlAMax(void)													//<<< �Ȑ�����_�z�񐔎擾
/********************/
{
	return CCtlA_max;
}

/********************/
int GetSCtlMax(void)													//<<< �ő�Ȗʐ���_���擾
/********************/
{
	return SCtl_max;
}

/************************/
void SetSCtlMax(int max)												//<<< �ő�Ȗʐ���_���ݒ�
/************************/
{
	SCtl_max = max, SCtlA_max = (int)(max*1.1);
}

/*********************/
int GetSCtlAMax(void)													//<<< �Ȗʐ���_�z�񐔎擾
/*********************/
{
	return SCtlA_max;
}

/********************/
int GetWCtlMax(void)													//<<< ��Ɨp����_�z�񐔎擾
/********************/
{
	return WCtl_max;
}

/************************/
void SetWCtlMax(int max)												//<<< ��Ɨp����_�z�񐔐ݒ�
/************************/
{
	WCtl_max = max;
}

/********************/
int GetCPntMax(void)													//<<< �ő�Ȑ����_���擾
/********************/
{
	return CPnt_max;
}

/************************/
void SetCPntMax(int max)												//<<< �ő�Ȑ����_���ݒ�
/************************/
{
	CPnt_max = max;
}

/********************/
int GetSPntMax(void)													//<<< �ő�Ȗʒ��_���擾
/********************/
{
	return SPnt_max;
}

/************************/
void SetSPntMax(int max)												//<<< �ő�Ȗʒ��_���ݒ�
/************************/
{
	SPnt_max = max;
}

/********************/
int GetOPntMax(void)													//<<< �ő嗧�̒��_���擾
/********************/
{
	return OPnt_max;
}

/************************/
void SetOPntMax(int max)												//<<< �ő嗧�̒��_���ݒ�
/************************/
{
	OPnt_max = max;
}

/********************/
int GetBPntMax(void)													//<<< �ő�f�ʒ��_���擾
/********************/
{
	return BPnt_max;
}

/************************/
void SetBPntMax(int max)												//<<< �ő�f�ʒ��_���ݒ�
/************************/
{
	BPnt_max = max;
}

/*******************************/
static void CreateMemArea(void)											//<<< �ް��̈�m��
/*******************************/
{
	Int_ary = (int *)malloc(sizeof(int)*Int_max);						// �������Ұ��z��̈�m��
	if(Int_ary==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Pnt_ary = (PNTTYPE *)malloc(sizeof(PNTTYPE)*Pnt_max);				// ���W���Ұ��z��̈�m��
	if(Pnt_ary==NULL) {
		MemErr(IDS_MEMERR5);
	}
	Undo_ary = (UNDTYPE *)malloc(sizeof(UNDTYPE)*Undo_max);				// Undo/Redo�z��̈�m��
	if(Undo_ary==NULL) {
		MemErr(IDS_MEMERR5);
	}
}

/**************************/
static void StartVar(void)												//<<< �J�n�ϐ�������
/**************************/
{
	int	i;

	Temp_cmp = NULL, Rf_cmp = NULL, Tach_cmp = NULL, Copy_cmp = NULL;	// ����ޑ���p�ϐ�������
	Int_adr = 0, Pnt_adr = 0, Cmd_flg = 0, Cmd_upd = 0, Cmd_stp = 0;
	Base_op = NULL, Temp_op = NULL, Rf_op = NULL, Reset_op = NULL;		// ���̑���p�ϐ�������
	Oset_op = NULL, SelectNo = 0, Base_oif = NULL, Temp_oif = NULL;
	for(i=0; i<Undo_max; i++) {
		Undo_ary[i].urflg = -1, Undo_ary[i].cmflg = -1;					// Undo����p�ϐ�������
		Undo_ary[i].cmp = NULL, Undo_ary[i].cop = NULL;
		Undo_ary[i].dop = NULL;
	}
	Undo_adr = 0, Undo_flg = 1, Undo_stp = 0;
	for(i=0; i<DIM; i++) {
		Scr_Box.bp[0].p[i] = 0.0, Scr_Box.bp[1].p[i] = 0.0;
	}
	Base_owk = NULL, Rf_owk = NULL;										// ���̥����ލ�Ɨp�ϐ�������
	Curve_Rept_SW = 0;
	Curve_Rept_Num = 3;	//202006 2 -> 3 �ɕύX							// �Ȗʍו�����Ɨp�ϐ�������
}

/******************/
void InitVar(void)														//<<< �ϐ�������(���ыN����)
/******************/
{
	Demo_Flg = 0, Init_Flg = 0, File_Flg = 0;							// �����N������p�ϐ�������
	Spsh_Flg = 0, Msg_No = 0;											// ���ׯ���֘A�ϐ�������
	Gc_cmp = NULL, Gc_op  = NULL, Gc_oif = NULL;						// �ė��p�ϐ�������
	ReadMemDataFile();													// ��ؗ̈��ް��ۑ�̧�ٓǍ�
	CreateMemArea();													// �ް��̈�m��
	StartVar();															// �J�n�ϐ�������
}

/*******************/
void ResetVar(void)														//<<< �ϐ�������(���щғ���)
/*******************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	CMDTYPE* cmp;
	CPCTYPE* cpc, * dcpc;

	op = Base_op;														// <�L�����̗̈敔>
	while(op!=NULL) {													// ����ޗ̈���
		if((cmp=op->bcmp)!=NULL) {
			while(cmp->onxt!=NULL) {
				cmp = cmp->onxt;
			}
			cmp->onxt = Gc_cmp, Gc_cmp = op->bcmp;
		}
		op = op->onxt;
	}
	if(Base_op!=NULL) {													// ���̗̈���
		op = Base_op;
		while(op->onxt!=NULL) {
			op = op->onxt;
		}
		op->onxt = Gc_op, Gc_op = Base_op;
	}
	if(Base_oif!=NULL) {												// <�L���\���̈敔>
		oif = Base_oif;
		while(oif->next!=NULL) {
			oif = oif->next;											// �̈���
		}
		oif->next = Gc_oif, Gc_oif = Base_oif;
	}
	op = Temp_op;														// <�ꎞ�폜���̗̈敔>
	while(op!=NULL) {													// ����ޗ̈���
		if((cmp=op->bcmp)!=NULL) {
			while(cmp->onxt!=NULL) {
				cmp = cmp->onxt;
			}
			cmp->onxt = Gc_cmp, Gc_cmp = op->bcmp;
		}
		op = op->onxt;
	}
	if(Temp_op!=NULL) {													// ���̗̈���
		op = Temp_op;
		while(op->onxt!=NULL) {
			op = op->onxt;
		}
		op->onxt = Gc_op, Gc_op = Temp_op;
	}
	if(Temp_oif!=NULL) {												// <�ꎞ�폜�\���̈敔>
		oif = Temp_oif;
		while(oif->next!=NULL) {
			oif = oif->next;											// �̈���
		}
		oif->next = Gc_oif, Gc_oif = Temp_oif;
	}
	if(Temp_cmp!=NULL) {												// <�ꎞ�폜����ޗ̈敔>
		cmp = Temp_cmp;
		while(cmp->onxt!=NULL) {
			cmp = cmp->onxt;											// ����ޗ̈���
		}
		cmp->onxt = Gc_cmp, Gc_cmp = Temp_cmp;
	}
	cpc = Copy_cmp;														// <��߰����ޗ̈敔>
	while(cpc!=NULL) {
		dcpc = cpc, cpc = cpc->next;
		free(dcpc);														// ��߰����ޗ̈���
	}
	StartVar();															// �J�n�ϐ�������
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/******************/
void FreeMem(void)														//<<< ��؉��(���яI����)
/******************/
{
	OBJTYPE* op, * dop;
	OIFTYPE* oif, * doif;
	CMDTYPE* cmp, * dcmp;
	CPCTYPE* cpc, * dcpc;

	op = Base_op;														// <�L�����̗̈敔>
	while(op!=NULL) {
		cmp = op->bcmp;
		while(cmp!=NULL) {
			dcmp = cmp, cmp = cmp->onxt;
			free(dcmp);													// ����ޗ̈���
		}
		dop = op, op = op->onxt;
		free(dop);														// ���̗̈���
	}
	oif = Base_oif;														// <�L���\���̈敔>
	while(oif!=NULL) {
		doif = oif, oif = oif->next;
		free(doif);														// �̈���
	}
	op = Temp_op;														// <�ꎞ�폜���̗̈敔>
	while(op!=NULL) {
		cmp = op->bcmp;
		while(cmp!=NULL) {
			dcmp = cmp, cmp = cmp->onxt;
			free(dcmp);													// ����ޗ̈���
		}
		dop = op, op = op->onxt;
		free(dop);														// ���̗̈���
	}
	oif = Temp_oif;														// <�ꎞ�폜�\���̈敔>
	while(oif!=NULL) {
		doif = oif, oif = oif->next;
		free(doif);														// �̈���
	}
	cmp = Temp_cmp;														// <�ꎞ�폜����ޗ̈敔>
	while(cmp!=NULL) {
		dcmp = cmp, cmp = cmp->onxt;
		free(dcmp);														// ����ޗ̈���
	}
	op = Gc_op;															// <�폜���̗̈敔>
	while(op!=NULL) {
		dop = op, op = op->onxt;
		free(dop);														// ���̗̈���
	}
	oif = Gc_oif;														// <�폜�\���̈敔>
	while(oif!=NULL) {
		doif = oif, oif = oif->next;
		free(doif);														// �̈���
	}
	cmp = Gc_cmp;														// <�ꎞ�폜����ޗ̈敔>
	while(cmp!=NULL) {
		dcmp = cmp, cmp = cmp->onxt;
		free(dcmp);														// ����ޗ̈���
	}
	cpc = Copy_cmp;														// <��߰����ޗ̈敔>
	while(cpc!=NULL) {
		dcpc = cpc, cpc = cpc->next;
		free(dcpc);														// ��߰����ޗ̈���
	}
}

/**********************************************************/
/*****                 �̈�A�h���X�ύX               *****/
/**********************************************************/
/******************************************************/
void ChangeAreaAdr(int isa, int icn, int psa, int pcn)					//<<< �z����ڽ�ύX
/******************************************************/
{
	OBJTYPE* optr;
	CMDTYPE* cptr;
	CPCTYPE* pptr;
	OWKTYPE* owptr;
	CWKTYPE* cwptr;

	optr = Base_op;														// Base�J�n�Ԓn�ύX
	while(optr!=NULL) {
		optr->vsa = (optr->vsa>psa) ? optr->vsa+pcn : optr->vsa;
		optr->lsa = (optr->lsa>isa) ? optr->lsa+icn : optr->lsa;
		cptr = optr->bcmp;
		while(cptr!=NULL) {
			cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
			cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
			cptr = cptr->onxt;
		}
		optr = optr->onxt;
	}
	optr = Temp_op;														// Temp�J�n�Ԓn�ύX
	while(optr!=NULL) {
		optr->vsa = (optr->vsa>psa) ? optr->vsa+pcn : optr->vsa;
		optr->lsa = (optr->lsa>isa) ? optr->lsa+icn : optr->lsa;
		cptr = optr->bcmp;
		while(cptr!=NULL) {
			cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
			cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
			cptr = cptr->onxt;
		}
		optr = optr->onxt;
	}
	cptr = Temp_cmp;													// Temp�J�n�Ԓn�ύX
	while(cptr!=NULL) {
		cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
		cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
		cptr = cptr->onxt;
	}
	pptr = Copy_cmp;													// Copy�J�n�Ԓn�ύX
	while(pptr!=NULL) {
		pptr->isa = (pptr->isa>isa) ? pptr->isa+icn : pptr->isa;
		pptr->psa = (pptr->psa>psa) ? pptr->psa+pcn : pptr->psa;
		pptr = pptr->next;
	}
	cptr = Tach_cmp;													// �ꎞ�ؗ������J�n�Ԓn�ύX
	while(cptr!=NULL) {
		cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
		cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
		cptr = cptr->onxt;
	}
	owptr = Base_owk;													// �J�n���̍���߲��
	while(owptr!=NULL) {
		cwptr = owptr->cwk;												// ���̕ʊJ�n����ލ���߲��
		while(cwptr!=NULL) {
			if(cwptr->flg!=2) {											// <�����s�̺����>
				cptr = cwptr->cmp;										// �Ώۺ�����߲��
				cptr->isa = (cptr->isa>isa) ? cptr->isa+icn : cptr->isa;
				cptr->psa = (cptr->psa>psa) ? cptr->psa+pcn : cptr->psa;
			}
			cwptr = cwptr->next;										// ������ލ���߲��
		}
		owptr = owptr->next;											// �����̍���߲��
	}

}

/**********************************************************/
/*****                  �����N������p                *****/
/**********************************************************/
/************************/
void SetDemoFlg(int flg)												//<<< �̌����׸ސݒ�
/************************/
{
	Demo_Flg = flg;
}

/********************/
int GetDemoFlg(void)													//<<< �̌����׸ގ擾
/********************/
{
	return Demo_Flg;
}

/************************/
void SetInitFlg(int flg)												//<<< �����N������׸ސݒ�
/************************/
{
	Init_Flg = flg;
}

/********************/
int GetInitFlg(void)													//<<< �����N������׸ގ擾
/********************/
{
	return Init_Flg;
}

/************************/
void SetFileFlg(int flg)												//<<< ̧�ٕۑ��׸ސݒ�
/************************/
{
	File_Flg = flg;
}

/********************/
int GetFileFlg(void)													//<<< ̧�ٕۑ��׸ގ擾
/********************/
{
	return File_Flg;
}

/*******************************/
void SetFileName(CString fname)											//<<< �ۑ�̧�ٖ��ݒ�
/*******************************/
{
	FileName = fname;
}

/*************************/
CString GetFileName(void)												//<<< �ۑ�̧�ٖ��擾
/*************************/
{
	return FileName;
}

/***************************/
void SetSplashMsg(UINT mno)												//<<< ���ׯ���\����ү���ސݒ�
/***************************/
{
	if(Spsh_Flg==0) {													// <���ׯ���\����>
		Msg_No = mno;													// ү����No�ۑ�
	} else {															// <���ׯ����\��>
		AfxMessageBox(mno);												// ү���ޕ\��
	}
}

/***********************/
void OutSplashMsg(void)													//<<< ���ׯ���\����ү���ޏo��
/***********************/
{
	if(Msg_No!=0) {														// <ү����No����>
		AfxMessageBox(Msg_No);											// ү���ޕ\��
	}
	Spsh_Flg = 1;														// ���ׯ���\���I��
	Msg_No = 0;															// ү����No�ر
}

/**********************************************************/
/*****              �o�b�N�A�b�v�t�@�C���p            *****/
/**********************************************************/
/**************************/
void StartBackUp(int mode)												//<<< �ޯ����ߊJ�n����
/**************************/
{
	FILE*   fp;
	CString bfile, ofile;

	if(Demo_Flg!=0) {
		return;															// �̌��ł͒��~
	}
	Cmd_Lvl = 0, Cmd_Lvl_Sv = 0, Obj_No = 0;							// �ޯ����ߗp�ϐ�������
	bfile = GetAppPath(0)+BUP_NAME;										// �ޯ�����̧�ٖ��擾
	ofile = GetAppPath(0)+BOD_NAME;										// �ޯ����ߕۑ�̧�ٖ��擾
	if(mode) {															// <�����N�����̂�>
		remove(ofile);													// �ۑ�̧�ٍ폜
		rename(bfile, ofile);											// �ޯ�����->�ۑ�̧��
	}
	if(fopen_s(&fp, bfile, "w")==0) {
		fclose(fp);														// �ޯ�����̧�ُ�����
	} else {
		SetSplashMsg(IDS_BACKUP_ERR);									// �װү���ޏo��
	}
}

/********************/
void StartMenu(void)													//<<< �ƭ��J�n����
/********************/
{
	Cmd_Lvl_Sv = Cmd_Lvl;												// �J�n�����������No�ۑ�
}

/********************/
void EndJMenu1(void)													//<<< �ƭ��I������1
/********************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, df, ef, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// �̌��ł͒��~
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ����ގ��s�����~
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	bfile = GetAppPath(0)+BUP_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "a+")==0) {	    							// �ޯ�����̧��OPEN
		clvl = Undo_ary[Undo_adr].clvl;
		if(clvl>Cmd_Lvl_Sv) {											// <�������ޗL>
			LevelBaseObjPtr(1, &op);									// �ꎞ�폜���̎擾
			while(op!=NULL) {
				fprintf(fp, "%d %d\n", clvl, op->ono);					// ͯ�ް��񏑍�
				fprintf(fp, "C%02d %d %d\n", DELCMD, 0, 0);				// ����ޏ�񏑍�
				LevelNextObjPtr(1, &op);
			}
		}
		BaseObjPtr(2, &op, &df, &ef);									// �L�����̐擪�擾
		while(op) {
			BaseCmdPtr(0, op, &cmp);									// ����ސ擪�擾
			while(cmp) {
				clvl = cmp->clvl;										// ���������No
				if(clvl>Cmd_Lvl_Sv) {									// <�������ޗL>
					cn = GetCmdNo(cmp);									// �����No�擾
					in = GetIntNum(cmp);								// �������Ұ����擾
					pn = GetPntNum(cmp);								// ���W���Ұ����擾
					fprintf(fp, "%d %d\n", clvl, op->ono);				// ͯ�ް��񏑍�
					fprintf(fp, "C%02d %d %d\n", cn, in, pn);			// ����ޏ�񏑍�
					for(i=0; i<in; i++) {
						fprintf(fp, "%d ", GetParaInt(cmp, i));			// �������Ұ�����
					}
					fprintf(fp, "\n");									// ���s
					for(i=0; i<pn; i++) {
						GetParaPnt(cmp, i, &p);							// ���W���Ұ��擾&����
						fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
					}
				}
				NextCmdPtr(&cmp);										// ������ނ�
			}
			NextObjPtr(2, &op, &df, &ef);								// ���L�����̂�
		}
		fclose(fp);														// �ޯ�����̧��CLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// �װү���ޏo��
	}
	if(Cmd_Lvl>10000) {
		AfxMessageBox(IDS_LEVEL_MAX);									// �x��ү���ޏo��
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/********************/
void EndJMenu2(void)													//<<< �ƭ��I������2
/********************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, df, ef, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// �̌��ł͒��~
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ����ގ��s�����~
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	bfile = GetAppPath(0)+BUP_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "a+")==0) {				    				// �ޯ�����̧��OPEN
		clvl = Undo_ary[Undo_adr].clvl;
		if(clvl>Cmd_Lvl_Sv) {											// <�������ޗL>
			LevelBaseObjPtr(1, &op);									// �ꎞ�폜���̎擾
			while(op!=NULL) {
				fprintf(fp, "%d %d\n", clvl, op->ono);					// ͯ�ް��񏑍�
				fprintf(fp, "C%02d %d %d\n", DELCMD, 0, 0);				// ����ޏ�񏑍�
				LevelNextObjPtr(1, &op);
			}
		}
		BaseObjPtr(1, &op, &df, &ef);									// �\�����̐擪�擾
		while(op) {
			BaseCmdPtr(0, op, &cmp);									// ����ސ擪�擾
			while(cmp) {
				clvl = cmp->clvl;										// ���������No
				if(clvl>Cmd_Lvl_Sv) {									// <�������ޗL>
					cn = GetCmdNo(cmp);									// �����No�擾
					in = GetIntNum(cmp);								// �������Ұ����擾
					pn = GetPntNum(cmp);								// ���W���Ұ����擾
					fprintf(fp, "%d %d\n", clvl, op->ono);				// ͯ�ް��񏑍�
					fprintf(fp, "C%02d %d %d\n", cn, in, pn);			// ����ޏ�񏑍�
					for(i=0; i<in; i++) {
						fprintf(fp, "%d ", GetParaInt(cmp, i));			// �������Ұ�����
					}
					fprintf(fp, "\n");									// ���s
					for(i=0; i<pn; i++) {
						GetParaPnt(cmp, i, &p);							// ���W���Ұ��擾&����
						fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
					}
				}
				NextCmdPtr(&cmp);										// ������ނ�
			}
			NextObjPtr(1, &op, &df, &ef);								// ���\�����̂�
		}
		fclose(fp);														// �ޯ�����̧��CLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// �װү���ޏo��
	}
	if(Cmd_Lvl>10000) {
		AfxMessageBox(IDS_LEVEL_MAX);									// �x��ү���ޏo��
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/**********************/
void FileBackUp1(void)													//<<< ̧���ޯ����ߏ���1
/**********************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// �̌��ł͒��~
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ����ގ��s�����~
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	bfile = GetAppPath(0)+BUP_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "a+")==0) {									// �ޯ�����̧��OPEN
		clvl = Undo_ary[Undo_adr].clvl;
		if(clvl>Cmd_Lvl_Sv) {											// <�������ޗL>
			LevelBaseObjPtr(0, &op);									// �������̎擾
			while(op) {
				BaseCmdPtr(0, op, &cmp);								// ����ސ擪�擾
				while(cmp) {
					clvl = cmp->clvl;									// ���������No
					if(clvl>Cmd_Lvl_Sv) {								// <�������ޗL>
						cn = GetCmdNo(cmp);								// �����No�擾
						in = GetIntNum(cmp);							// �������Ұ����擾
						pn = GetPntNum(cmp);							// ���W���Ұ����擾
						fprintf(fp, "%d %d\n", clvl, op->ono);			// ͯ�ް��񏑍�
						fprintf(fp, "C%02d %d %d\n", cn, in, pn);		// ����ޏ�񏑍�
						for(i=0; i<in; i++) {
							fprintf(fp, "%d ", GetParaInt(cmp, i));		// �������Ұ�����
						}
						fprintf(fp, "\n");								// ���s
						for(i=0; i<pn; i++) {
							GetParaPnt(cmp, i, &p);						// ���W���Ұ��擾&����
							fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
						}
					}
					NextCmdPtr(&cmp);									// ������ނ�
				}
				LevelNextObjPtr(0, &op);
			}
		}
		fclose(fp);														// �ޯ�����̧��CLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// �װү���ޏo��
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/**********************/
void FileBackUp2(void)													//<<< ̧���ޯ����ߏ���2
/**********************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	FILE*    fp;
	OWKTYPE* owk;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	int      i, clvl, cn, in, pn;

	if(Demo_Flg!=0) {
		return;															// �̌��ł͒��~
	}
	if(Cmd_Lvl_Sv==Cmd_Lvl) {
		return;															// ����ގ��s�����~
	}
	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	bfile = GetAppPath(0)+BUP_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "a+")==0) {									// �ޯ�����̧��OPEN
		owk = Base_owk;													// �J�n���̍���߲��
		while(owk!=NULL) {
			if(owk->op!=NULL) {											// <�����߲���o�^�ς�>
				op = owk->op;
				BaseCmdPtr(0, op, &cmp);								// ����ސ擪�擾
				while(cmp) {
					clvl = cmp->clvl;									// ���������No
					if(clvl>Cmd_Lvl_Sv) {								// <�������ޗL>
						cn = GetCmdNo(cmp);								// �����No�擾
						in = GetIntNum(cmp);							// �������Ұ����擾
						pn = GetPntNum(cmp);							// ���W���Ұ����擾
						fprintf(fp, "%d %d\n", clvl, op->ono);			// ͯ�ް��񏑍�
						fprintf(fp, "C%02d %d %d\n", cn, in, pn);		// ����ޏ�񏑍�
						for(i=0; i<in; i++) {
							fprintf(fp, "%d ", GetParaInt(cmp, i));		// �������Ұ�����
						}
						fprintf(fp, "\n");								// ���s
						for(i=0; i<pn; i++) {
							GetParaPnt(cmp, i, &p);						// ���W���Ұ��擾&����
							fprintf(fp, "%f %f %f\n", p.p[0], p.p[1], p.p[2]);
						}
					}
					NextCmdPtr(&cmp);									// ������ނ�
				}
			}
			owk = owk->next;											// �����̍���߲��
		}
		fclose(fp);														// �ޯ�����̧��CLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// �װү���ޏo��
	}
	SetScrBox();														// ��ذ��ޯ���ݒ�
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
}

/***********************/
void UndoMenu(int clvl)													//<<< Undo�ƭ�����
/***********************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	FILE*   fp;
	CString bfile;

	if(Demo_Flg!=0) {
		return;															// �̌��ł͒��~
	}
	bfile = GetAppPath(0)+BUP_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "a+")==0) {				    				// �ޯ�����̧��OPEN
		fprintf(fp, "%d %d\n", clvl, 0);								// ͯ�ް��񏑍�
		fprintf(fp, "C%02d %d %d\n", UNDOCMD, 0, 0);					// ����ޏ�񏑍�
		fclose(fp);														// �ޯ�����̧��CLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// �װү���ޏo��
	}
}

/***********************/
void RedoMenu(int clvl)													//<<< Redo�ƭ�����
/***********************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	FILE*   fp;
	CString bfile;

	if(Demo_Flg!=0) {
		return;															// �̌��ł͒��~
	}
	bfile = GetAppPath(0)+BUP_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "a+")==0) {				    				// �ޯ�����̧��OPEN
		fprintf(fp, "%d %d\n", clvl, 0);								// ͯ�ް��񏑍�
		fprintf(fp, "C%02d %d %d\n", REDOCMD, 0, 0);					// ����ޏ�񏑍�
		fclose(fp);														// �ޯ�����̧��CLOSE
	} else {
		AfxMessageBox(IDS_BACKUP_ERR);									// �װү���ޏo��
	}
}

/***************************************************/
static void UpdateCheck(BYTE* ary, int no, int chk)						//<<< �����X�V
/***************************************************/
{
	BYTE bt;
	int  i, j;

	i = no/8, j = no%8;
	bt = 0x1,  bt <<= 7-j;
	if(chk) {
		ary[i] |= bt;
	} else {
		bt ^= 0xff, ary[i] &= bt;
	}
}

/**************************************/
static int GetCheck(BYTE* ary, int no)									//<<< �����擾
/**************************************/
{
	BYTE bt;
	int  i, j;

	i = no/8, j = no%8;
	bt = ary[i], bt >>= 7-j, i = bt & 0x1;
	return i;
}

/***********************************/
static BOOL CheckCmdArea(BYTE* cmd)										//<<< ���������No����
/***********************************/
{
	char*   cc = new char[100];
	FILE*   fp;
	CString bfile;
	BOOL    ret=FALSE;
	int     i, dmy1, clvl, ono, cn, in, pn;
	float   dmy2;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	bfile = GetAppPath(0)+BOD_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "r")==0) {									// �ޯ�����̧��OPEN
		while(TRUE) {
			if(fscanf_s(fp, "%d%d", &clvl, &ono)!=2) {
				break;													// ͯ�ް���擾
			} else {
				ret = TRUE;												// ����ޏ��擾
			}
			if(fscanf_s(fp, "%s%d%d", cc, 100, &in, &pn)!=3) {
				ret = FALSE; break;
			}
			if(cc[0]!='C') {
				ret = FALSE; break;										// �ް��ُ�ŏI��
			}
			sscanf_s(&cc[1], "%d", &cn);								// ���l�ϊ�
			for(i=0; i<in; i++) {
				if(fscanf_s(fp, "%d", &dmy1)!=1) {						// �������Ұ��擾
					ret = FALSE; break;
				}
			}
			for(i=0; i<pn; i++) {										// ���W���Ұ��擾
				if(fscanf_s(fp, "%f%f%f", &dmy2, &dmy2, &dmy2)!=3) {
					ret = FALSE; break;
				}
			}
			if(cn==UNDOCMD) {
				UpdateCheck(cmd, clvl, 0);								// ����ޖ����̏ꍇ
			} else if(cn==REDOCMD) {
				UpdateCheck(cmd, clvl, 1);								// ����ޗL���̏ꍇ
			}
			if(!ret) {
				break;
			}
		}
		fclose(fp);														// �ޯ�����̧��CLOSE
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] cc;
	return ret;
}

/**********************************************/
static BOOL CheckObjArea(BYTE* cmd, BYTE* obj)							//<<< ����No����
/**********************************************/
{
	char*   cc = new char[100];
	FILE*   fp;
	CString bfile;
	BOOL    ret=FALSE;
	int     i, dmy1, clvl, ono, cn, in, pn;
	float   dmy2;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	bfile = GetAppPath(0)+BOD_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "r")==0) {									// �ޯ�����̧��OPEN
		while(TRUE) {
			if(fscanf_s(fp, "%d%d", &clvl, &ono)!=2) {
				break;													// ͯ�ް���擾
			} else {
				ret = TRUE;												// ����ޏ��擾
			}
			if(fscanf_s(fp, "%s%d%d", cc, 100, &in, &pn)!=3) {
				ret = FALSE; break;
			}
			if(cc[0]!='C') {
				ret = FALSE; break;										// �ް��ُ�ŏI��
			}
			sscanf_s(&cc[1], "%d", &cn);								// ���l�ϊ�
			for(i=0; i<in; i++) {
				if(fscanf_s(fp, "%d", &dmy1)!=1) {						// �������Ұ��擾
					ret = FALSE; break;
				}
			}
			for(i=0; i<pn; i++) {										// ���W���Ұ��擾
				if(fscanf_s(fp, "%f%f%f", &dmy2, &dmy2, &dmy2)!=3) {
					ret = FALSE; break;
				}
			}
			if(cn==CRTSURF||cn==DIRECT||cn==CRTCURVE||cn==SETSURF||
				cn==SETCURVE||cn==THICKNESS||cn==CRTSWEEP) {			// <���������>
				if(GetCheck(cmd, clvl)) {
					UpdateCheck(obj, ono, 1);							// ���̗L��
				}
			}
			if(cn==DELCMD) {											// ���̍폜��
				if(GetCheck(cmd, clvl)) {
					UpdateCheck(obj, ono, 0);							// ���̖���
				}
			}
			if(!ret) {
				break;
			}
		}
		fclose(fp);														// �ޯ�����̧��CLOSE
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
	delete[] cc;
	return ret;
}

/*********************************************/
static BOOL RestoreObject(int no, BYTE* cmd)							//<<< ���̕���
/*********************************************/
{
    char*    cc = new char[100];
    char*    b1 = new char[100];
    char*    b2 = new char[100];
    char*    b3 = new char[100];
	FILE*    fp;
	OBJTYPE* op;
	CMDTYPE* cmp;
	PNTTYPE  p;
	CString  bfile;
	BOOL     ret=FALSE;
	int      i, j, clvl, ono, cn, in, pn, sw, ok=0;

	setlocale(LC_ALL, "English");										// ۹�ق��p��ɕύX
	bfile = GetAppPath(0)+BOD_NAME;										// �ޯ�����̧�ٖ��擾
	if(fopen_s(&fp, bfile, "r")==0) {									// �ޯ�����̧��OPEN
		while(TRUE) {
			if(fscanf_s(fp, "%d%d", &clvl, &ono)!=2) {
				break;													// ͯ�ް���擾
			} else {
				ret=TRUE;												// ����ޏ��擾
			}
			sw = (no==ono) ? 1 : 0;										// �Ώۗ�������
			if(fscanf_s(fp, "%s%d%d", cc, 100, &in, &pn)!=3) {
				ret = FALSE; break;
			}
			if(sw) {
				if(cc[0]!='C') {
					ret=FALSE; break;									// �ް��ُ�ŏI��
				}
				sscanf_s(&cc[1], "%d", &cn);							// ���l�ϊ�
				sw = (cn>MAXCMD) ? 0 : sw;								// ���̺��������
				if(sw) {
					sw = (!GetCheck(cmd, clvl)) ? 0 : sw;				// �L�����������
				}
			}
			if(sw) {													// <����ޏo��OK>
				CreatCmd(cn, in, pn, &cmp);								// ����ޗ̈�m��
				for(i=0; i<in; i++) {
					if(fscanf_s(fp, "%d", &j)!=1) {						// �������Ұ��擾
						ret = FALSE; break;
					}
					PutParaInt(cmp, i, j);								// �������Ұ��ݒ�
				}
				for(i=0; i<pn; i++) {                                   // ���W���Ұ��擾
					if(fscanf_s(fp, "%s%s%s", b1, 100, b2, 100, b3, 100)!=3) {
						ret = FALSE; break;
					}
					p.p[0] = atof(b1), p.p[1] = atof(b2), p.p[2] = atof(b3);
					PutParaPnt(cmp, i, p);								// ���W���Ұ��ݒ�
				}
				if(!ret) {
					break;
				}
				SetCmdPtr(cmp);											// �����Ώۺ���ސݒ�
				ExecCommand(TRUE);										// ����ގ��s
				ok = 1;
			} else {													// <����ޏo��NG>
				for(i=0; i<in; i++) {
					if(fscanf_s(fp, "%d", &j)!=1) {						// �������Ұ��擾
						ret = FALSE; break;
					}
				}
				for(i=0; i<pn; i++) {			                        // ���W���Ұ��擾
					if(fscanf_s(fp, "%s%s%s", b1, 100, b2, 100, b3, 100)!=3) {
						ret = FALSE; break;
					}
				}
				if(!ret) {
					break;
				}
			}
		}
		fclose(fp);														// �ޯ�����̧��CLOSE
	}
	if(ok) {															// <����ޏo�͍�>
		GetObjPtr(&op);													// �����߲���擾
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
	}
	setlocale(LC_ALL, "");												// ۹�ق���̫�ĂɕύX
    delete[] cc;
    delete[] b1;
    delete[] b2;
    delete[] b3;
	return ret;
}

/************************/
BOOL RestoreBackUp(void)												//<<< �ƭ���������
/************************/
{
	CWaitCursor wait;													// ���ĥ���ق�\��
	BYTE* cmd = new BYTE[1500];
	BYTE* obj = new BYTE[1500];
	int   i;

	if(Demo_Flg!=0) {
		delete[] cmd;
		delete[] obj;
		return FALSE;													// �̌��ł͒��~
	}
	for(i=0; i<1500; i++) {
		cmd[i] = 0xff;													// ���������������(All-OK)
		obj[i] = 0x0;													// ��������������(All-NG)
	}
	if(!CheckCmdArea(cmd)) {
		delete[] cmd;
		delete[] obj;
		return FALSE;													// ���������No����
	}
	if(!CheckObjArea(cmd, obj)) {
		delete[] cmd;
		delete[] obj;
		return FALSE;													// ����No����
	}
	for(i=0; i<12000; i++) {
		if(GetCheck(obj, i)) {											// �L�����̂̏ꍇ
			if(!RestoreObject(i, cmd)) {								// ���̕���
				delete[] cmd;
				delete[] obj;
				return FALSE;
			}
			View->Display(0);											// �ĕ`��
		}
	}
	View->SetDataUse();													// �ް��g�p���ݒ�
	delete[] cmd;
	delete[] obj;
	return TRUE;
}

/**********************************************************/
/*****                  �f�[�^�g�p���p                *****/
/**********************************************************/
/****************************/
void MemErr(unsigned int id)											//<<< ��؈ُ�ү����
/****************************/
{
	AfxMessageBox(id);													// �װү���ޏo��
	MainWnd->DestroyWindow();											// JCAD3�I��
	exit(1);															// �I��
}

/**********************/
double GetIntUse(void)													//<<< �������Ұ��g�p���擾
/**********************/
{
	double par;

	if((par=((double)Int_adr/Int_max)*100.0)>=100.0) {
		MemErr(IDS_MEMERR1);
	}
	return par;
}

/**********************/
double GetPntUse(void)													//<<< ���_���Ұ��g�p���擾
/**********************/
{
	double par;

	if((par=((double)Pnt_adr/Pnt_max)*100.0)>=100.0) {
		MemErr(IDS_MEMERR2);
	}
	return par;
}

/**********************/
int GetIntUseNum(void)													//<<< �������Ұ��g�p���擾
/**********************/
{
	return Int_adr;
}

/**********************/
int GetPntUseNum(void)													//<<< ���_���Ұ��g�p���擾
/**********************/
{
	return Pnt_adr;
}

/**********************************************************/
/*****                    �R�}���h�p                  *****/
/**********************************************************/
/*******************************************************/
void CreatCmd(int cno, int ino, int pno, CMDTYPE** cmp)					//<<< ����ޗ̈�m��
/*******************************************************/
{
	if(Gc_cmp==NULL) {													// <GC���̏ꍇ>
		*cmp = (CMDTYPE *)malloc(sizeof(CMDTYPE));						// �V�K�̈�m��
		if(*cmp==NULL) {
			MemErr(IDS_MEMERR5);
		}
	} else {															// <GC�L�̏ꍇ>
		*cmp = Gc_cmp, Gc_cmp = Gc_cmp->onxt;							// GC�擪�g�p
	}
	(*cmp)->inum = ino;													// �������Ұ��̈�m��
	(*cmp)->isa = Int_adr, Int_adr += ino;
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR1);
	}
	(*cmp)->pnum = pno;													// ���W���Ұ��̈�m��
	(*cmp)->psa = Pnt_adr, Pnt_adr += pno;
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR2);
	}
	(*cmp)->cno = (BYTE)cno;											// �����No�ݒ�
	(*cmp)->clvl = Cmd_Lvl;												// ���������No�ݒ�
	(*cmp)->onxt = NULL;												// �߲��������
	(*cmp)->lnxt = NULL;
	(*cmp)->op = NULL;
}

/**********************************************/
void ChangeCmd(CMDTYPE* cmp, int ino, int pno)							//<<< ����ޗ̈�ύX
/**********************************************/
{
	int oin, oia, opn, opa, src, chn, i;

	if(cmp->inum==ino&&cmp->pnum==pno) {
		return;															// �̈�ύX�����~
	}
	oin = cmp->inum, oia = Int_adr;										// ���̈搔�ۑ�
	opn = cmp->pnum, opa = Pnt_adr;
	cmp->inum = ino, Int_adr += ino-oin;								// �������Ұ��̈�m��
	if(Int_adr>=Int_max) {
		MemErr(IDS_MEMERR1);
	}
	cmp->pnum = pno, Pnt_adr += pno-opn;								// ���W���Ұ��̈�m��
	if(Pnt_adr>=Pnt_max) {
		MemErr(IDS_MEMERR2);
	}
	src = cmp->isa+oin;													// �̈���(�������Ұ�)
	if(oia<Int_adr) {
		chn = Int_adr-oia;
		if(chn!=0) {
			for(i=oia-1; i>=src; i--) {
				Int_ary[i+chn] = Int_ary[i];
			}
		}
	} else {
		chn = oia-Int_adr;
		if(chn!=0) {
			for(i=src; i<oia; i++) {
				Int_ary[i-chn] = Int_ary[i];
			}
		}
	}
	src = cmp->psa+opn;													// �̈���(���W���Ұ�)
	if(opa<Pnt_adr) {
		chn = Pnt_adr-opa;
		if(chn!=0) {
			for(i=opa-1; i>=src; i--) {
				Pnt_ary[i+chn] = Pnt_ary[i];
			}
		}
	} else {
		chn = opa-Pnt_adr;
		if(chn!=0) {
			for(i=src; i<opa; i++) {
				Pnt_ary[i-chn] = Pnt_ary[i];
			}
		}
	}
	ChangeAreaAdr(cmp->isa, ino-oin, cmp->psa, pno-opn);				// �z����ڽ�ύX
}

/***************************************/
void TempCmd(OBJTYPE* op, CMDTYPE* cmp)									//<<< ����ވꎞ�폜
/***************************************/
{
	CMDTYPE* cptr;

	if(op->bcmp==cmp) {													// <�擪����ނ̏ꍇ>
		op->bcmp = cmp->onxt;											// ������ޗ��̐ڑ�
		if(op->ccmp==cmp) {
			op->ccmp = cmp->onxt;										// �L���擪�����->�������
		}
	} else {															// <�擪�ȊO����ނ̏ꍇ>
		if((cptr=op->bcmp)==NULL) {
			return;
		}
		while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
			cptr = cptr->onxt;											// �O����ތ���
		}
		if(cptr->onxt==NULL) {
			return;														// �w�����ޖ����~
		}
		cptr->onxt = cmp->onxt;											// �O����ގ�����ސڑ�
		if(op->ccmp==cmp) {
			op->ccmp = cptr;											// �L���擪�����->�O�����
		}
		if(op->rscmp==cmp) {											// <�ݸސ擪����ސݒ�L�̏ꍇ>
			op->rscmp = NULL;											// �ݸސ擪����ޏ�����
			cptr = op->ccmp;
			while(cptr!=NULL) {
				if(cptr->cno==RING) {
					op->rscmp = cptr;									// �ŏ����ݸ޺��Đݒ�
					break;
				}
				cptr = cptr->onxt;
			}
		}
		if(op->rccmp==cmp) {											// <�ݸތ��ݺ���ސݒ�L�̏ꍇ>
			op->rccmp = NULL;											// �ݸތ��ݺ���ޏ�����
			cptr = op->ccmp;
			while(cptr!=NULL) {
				if(cptr->cno==RING) {
					op->rccmp = cptr;									// �Ō���ݸ޺��Đݒ�
				}
				cptr = cptr->onxt;
			}
		}
		if(op->ecmp==cmp) {												// <�������ސݒ�L�̏ꍇ>
			op->ecmp = NULL;											// �������ޏ�����
			cptr = op->ccmp;
			while(cptr!=NULL) {
				if(cptr->cno==EARTH) {
					op->ecmp = cptr;									// �Ō�̱������ސݒ�
				}
				cptr = cptr->onxt;
			}
		}
	}
	cmp->onxt = Temp_cmp, Temp_cmp = cmp;								// Temp�擪�ڑ�
}

/************************************************/
void FreeCmd(OBJTYPE* op, CMDTYPE* cmp, int flg)						//<<< ����ޗ̈���
/************************************************/
{
	CMDTYPE* cptr;
	int      src, i;

	if(op!=NULL) {														// <���̎w��̏ꍇ>
		if(op->bcmp==cmp) {												// <�擪����ނ̏ꍇ>
			op->bcmp = cmp->onxt;										// ������ޗ��̐ڑ�
			if(op->ccmp==cmp) {
				op->ccmp = cmp->onxt;									// �L���擪�����->�������
			}
		} else {														// <�擪�ȊO����ނ̏ꍇ>
			if((cptr=op->bcmp)==NULL) {
				return;
			}
			while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
				cptr = cptr->onxt;										// �O����ތ���
			}
			if(cptr->onxt==NULL) {
				return;													// �w�����ޖ����~
			}
			cptr->onxt = cmp->onxt;										// �O����ގ�����ސڑ�
			if(op->ccmp==cmp) {
				op->ccmp = cptr;										// �L���擪�����->�O�����
			}
			if(op->rscmp==cmp) {										// <�ݸސ擪����ސݒ�L�̏ꍇ>
				op->rscmp = NULL;										// �ݸސ擪����ޏ�����
				cptr = op->ccmp;
				while(cptr!=NULL) {
					if(cptr->cno==RING) {
						op->rscmp = cptr;								// �ŏ����ݸ޺��Đݒ�
						break;
					}
					cptr = cptr->onxt;
				}
			}
			if(op->rccmp==cmp) {										// <�ݸތ��ݺ���ސݒ�L�̏ꍇ>
				op->rccmp = NULL;										// �ݸތ��ݺ���ޏ�����
				cptr = op->ccmp;
				while(cptr!=NULL) {
					if(cptr->cno==RING) {
						op->rccmp = cptr;								// �Ō���ݸ޺��Đݒ�
					}
					cptr = cptr->onxt;
				}
			}
			if(op->ecmp==cmp) {											// <�������ސݒ�L�̏ꍇ>
				op->ecmp = NULL;										// �������ޏ�����
				cptr = op->ccmp;
				while(cptr!=NULL) {
					if(cptr->cno==EARTH) {
						op->ecmp = cptr;								// �Ō�̱������ސݒ�
					}
					cptr = cptr->onxt;
				}
			}
		}
	} else {															// <���̖��w��̏ꍇ>
		if(Temp_cmp==cmp) {												// <Temp�擪�̏ꍇ>
			Temp_cmp = cmp->onxt;										// �������Temp�ڑ�
		} else {														// <Temp�擪�łȂ��ꍇ>
			if((cptr=Temp_cmp)==NULL) {
				return;													// Temp������ތ���
			}
			while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
				cptr = cptr->onxt;
			}
			if(cptr->onxt==NULL) {
				return;													// �w�����ޖ����~
			}
			cptr->onxt = cmp->onxt;										// �w��O��ڑ�
		}
	}
	cmp->onxt = Gc_cmp, Gc_cmp = cmp;									// GC�擪�ڑ�
	if(flg) {															// <Undo/Redo����>
		if(Undo_ary[Undo_adr].cmp==cmp) {								// <���ِ擪�̏ꍇ>
			Undo_ary[Undo_adr].cmp = cmp->lnxt;							// ������ސ擪�ڑ�
		} else {														// <���ِ擪�łȂ��ꍇ>
			if((cptr=Undo_ary[Undo_adr].cmp)!=NULL) {					// �擪������ތ���
				while(cptr->lnxt!=NULL&&cptr->lnxt!=cmp) {
					cptr = cptr->lnxt;
				}
				if(cptr->lnxt==cmp) {
					cptr->lnxt = cmp->lnxt;								// �w��O��ڑ�
				}
			}
		}
	}
	if(cmp->inum!=0) {													// �������Ұ��̈���
		src = cmp->isa+cmp->inum;
		for(i=src; i<Int_adr; i++) {
			Int_ary[i-cmp->inum] = Int_ary[i];
		}
		Int_adr -= cmp->inum;
	}
	if(cmp->pnum!=0) {													// ���W���Ұ��̈���
		src = cmp->psa+cmp->pnum;
		for(i=src; i<Pnt_adr; i++) {
			Pnt_ary[i-cmp->pnum] = Pnt_ary[i];
		}
		Pnt_adr -= cmp->pnum;
	}
	ChangeAreaAdr(cmp->isa, -cmp->inum, cmp->psa, -cmp->pnum);			// �z����ڽ�ύX
}

/*****************************************/
void AppendCmd(OBJTYPE* op, CMDTYPE* cmp)								//<<< �����&���̐ڑ�
/*****************************************/
{
	CMDTYPE* cptr;

	if(Reset_op!=NULL) {
		return;															// �ڑ��ς݂͒��~����
	}
	cmp->op = op;														// OP�ݒ�
	if(op->bcmp==NULL) {												// <��������ނ̏ꍇ>
		op->bcmp = cmp, op->ccmp = cmp;									// ���̺���ސڑ�
	} else {															// <�ҏW����ނ̏ꍇ>
		if(cmp->cno==CHGSURF||cmp->cno==CHGCURVE||cmp->cno==CHGSWEEP) {	// <�Ȗ�/�Ȑ��ύX�̏ꍇ>
			cptr = op->ccmp;											// �L���擪����ގ��ڑ�
			cmp->onxt = cptr->onxt, cptr->onxt = cmp;
			op->ccmp = cmp;												// �L���擪����ޕύX
		} else {														// <�ȖʕҏW�ȊO�̏ꍇ>
			cptr = op->bcmp;
			while(cptr->onxt!=NULL) {
				cptr = cptr->onxt;										// �ŏI����ތ���
			}
			cptr->onxt = cmp;											// �ŏI�ڑ�
			if(cmp->cno==RING) {										// <�ݸ޺���ނ̏ꍇ>
				if(op->rscmp==NULL) {
					op->rscmp = cmp;									// �ݸސ擪����ޕύX
				}
				op->rccmp = cmp;										// �ݸތ��ݺ���ޕύX
			}
			if(cmp->cno==EARTH) {										// <�������ނ̏ꍇ>
				op->ecmp = cmp;											// �������ޕύX
			}
		}
	}
}

/***************************************/
void UndoCmd(OBJTYPE* op, CMDTYPE* cmp)									//<<< �����&���̍Đڑ�
/***************************************/
{
	CMDTYPE* cptr;

	if(Temp_cmp==cmp) {													// <Temp�擪�̏ꍇ>
		Temp_cmp = cmp->onxt;											// �������Temp�ڑ�
	} else {															// <Temp�擪�łȂ��ꍇ>
		if((cptr=Temp_cmp)==NULL) {
			return;
		}
		while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
			cptr = cptr->onxt;											// Temp������ތ���
		}
		if(cptr->onxt==NULL) {
			return;														// �w�����ޖ����~
		}
		cptr->onxt = cmp->onxt;											// �w��O��ڑ�
	}
	cmp->onxt = NULL;
	cmp->op = op;														// OP�ݒ�
	if(op->bcmp==NULL) {												// <��������ނ̏ꍇ>
		op->bcmp = cmp, op->ccmp = cmp;									// ���̺���ސڑ�
	} else {															// <�ҏW����ނ̏ꍇ>
		if(cmp->cno==CHGSURF||cmp->cno==CHGCURVE||cmp->cno==CHGSWEEP) {	// <�Ȗ�/�Ȑ��ύX�̏ꍇ>
			cptr = op->ccmp;											// �L���擪����ގ��ڑ�
			cmp->onxt = cptr->onxt, cptr->onxt = cmp;
			op->ccmp = cmp;												// �L���擪����ޕύX
		} else {														// <�ȖʕҏW�ȊO�̏ꍇ>
			cptr = op->bcmp;
			while(cptr->onxt!=NULL) {
				cptr = cptr->onxt;										// �ŏI����ތ���
			}
			cptr->onxt = cmp;											// �ŏI�ڑ�
			if(cmp->cno==RING) {										// <�ݸ޺���ނ̏ꍇ>
				if(op->rscmp==NULL) {
					op->rscmp = cmp;									// �ݸސ擪����ޕύX
				}
				op->rccmp = cmp;										// �ݸތ��ݺ���ޕύX
			}
			if(cmp->cno==EARTH) {										// <�������ނ̏ꍇ>
				op->ecmp = cmp;											// �������ޕύX
			}
		}
	}
}

/***************************/
void LevelCmd(CMDTYPE* cmp)												//<<< �������ٺ���ސڑ�
/***************************/
{
	CMDTYPE* cptr;
	int      wadr;

    if(Undo_stp==1) {
		return;															// Undo�ۑ���~�����~
	}
	if(Reset_op!=NULL) {
		return;															// �ڑ��ϒ��~
	}
	if(Undo_ary[Undo_adr].cmp==NULL) {									// <�擪����ނ̏ꍇ>
		Undo_ary[Undo_adr].cmp = cmp;									// �z��ڑ�
		Undo_ary[Undo_adr].urflg = 1;									// �����ƭ�Undo��
		Undo_ary[Undo_adr].cmflg = 0;									// �O���ƭ�Redo��
		wadr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;
		Undo_ary[wadr].cmflg = 1;										// �����ƭ�Redo�s��
	} else {															// <�擪�ȊO����ނ̏ꍇ>
		cptr = Undo_ary[Undo_adr].cmp;
		while(cptr->lnxt!=NULL) {
			cptr = cptr->lnxt;											// �ŏI����ތ���
		}
		cptr->lnxt = cmp;												// �ŏI�ڑ�
	}
}

/*****************************************/
void DetachCmd(OBJTYPE* op, CMDTYPE* cmp)								//<<< ����ވꎞ�ؗ���
/*****************************************/
{
	CMDTYPE* cptr;

	if(op->bcmp!=NULL) {												// <����ޗL�̏ꍇ>
		cptr = op->bcmp;
		if(cptr==cmp) {													// <�擪�w��̏ꍇ>
			Tach_cmp = cptr->onxt;										// �w�莟�߲���ۑ�
			cptr->onxt = NULL;											// �߲���ؗ���
		} else {														// <�擪�w��ȊO�̏ꍇ>
			while(cptr->onxt!=NULL&&cptr->onxt!=cmp) {
				cptr = cptr->onxt;
			}
			if(cptr->onxt==cmp) {										// <�w���߲���L�̏ꍇ>
				cptr = cptr->onxt;										// �w�������߲��
				Tach_cmp = cptr->onxt;									// �w�莟�߲���ۑ�
				cptr->onxt = NULL;										// �߲���ؗ���
			}
		}
		op->rscmp = NULL;												// �ݸސ擪����ޏ�����
		cptr = op->ccmp;
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rscmp = cptr;										// �ŏ����ݸ޺��Đݒ�
				break;
			}
			cptr = cptr->onxt;
		}
		op->rccmp = NULL;												// �ݸތ��ݺ���ޏ�����
		cptr = op->ccmp;
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rccmp = cptr;										// �Ō���ݸ޺��Đݒ�
			}
			cptr = cptr->onxt;
		}
		op->ecmp = NULL;												// �������ޏ�����
		cptr = op->ccmp;
		while(cptr!=NULL) {
			if(cptr->cno==EARTH) {
				op->ecmp = cptr;										// �Ō�̱������ސݒ�
			}
			cptr = cptr->onxt;
		}
	}
}

/***************************/
void AttachCmd(OBJTYPE* op)												//<<< ����ލĐڑ�
/***************************/
{
	CMDTYPE* cptr;

	if(op->bcmp!=NULL) {												// <����ޗL�̏ꍇ>
		cptr = op->bcmp;
		while(cptr->onxt!=NULL) {
			cptr = cptr->onxt;											// �ŏI����ތ���
		}
		cptr->onxt = Tach_cmp;											// �ŏI�Đڑ�
	}
	Tach_cmp = NULL;													// �ۑ��̈揉����
	op->rscmp = NULL;													// �ݸސ擪����ޏ�����
	cptr = op->ccmp;
	while(cptr!=NULL) {
		if(cptr->cno==RING) {
			op->rscmp = cptr;											// �ŏ����ݸ޺��Đݒ�
			break;
		}
		cptr = cptr->onxt;
	}
	op->rccmp = NULL;													// �ݸތ��ݺ���ޏ�����
	cptr = op->ccmp;
	while(cptr!=NULL) {
		if(cptr->cno==RING) {
			op->rccmp = cptr;											// �Ō���ݸ޺��Đݒ�
		}
		cptr = cptr->onxt;
	}
	op->ecmp = NULL;													// �������ޏ�����
	cptr = op->ccmp;
	while(cptr!=NULL) {
		if(cptr->cno==EARTH) {
			op->ecmp = cptr;											// �Ō�̱������ސݒ�
		}
		cptr = cptr->onxt;
	}
}

/*****************************/
void InitCmdLvl(CMDTYPE* cmp)											//<<< ��������ُ�����
/*****************************/
{
	cmp->clvl = Cmd_Lvl;
}

/**************************************/
void SetCmdLvl(CMDTYPE* cmp, int clvl)									//<<< ��������ِݒ�
/**************************************/
{
	cmp->clvl = clvl;
}

/***************************/
int GetCmdLvl(CMDTYPE* cmp)												//<<< ��������َ擾
/***************************/
{
	return cmp->clvl;
}

/****************************************************/
void BaseCmdPtr(int flg, OBJTYPE* op, CMDTYPE** cmp)					//<<< �擪������߲���擾
/****************************************************/
{
	*cmp = (!flg) ? op->bcmp : op->ccmp;								// �ް������/�L���擪�����
}

/******************************/
void NextCmdPtr(CMDTYPE** cmp)											//<<< ��������߲���擾
/******************************/
{
	OBJTYPE* op;
	BOOL     nxt=FALSE;

	if(*cmp!=NULL) {
		op = (*cmp)->op;
		if(*cmp==op->rccmp) {											// ������ނ��ݸތ��ݺ���ނ̏ꍇ��
			*cmp = op->rscmp;											// �ݸސ擪����ނ֒u��������
		}
		if(*cmp==op->ccmp) {											// <������ނ��L���擪����ނ̏ꍇ>
			if(op->ecmp!=NULL) {										// �������ނ��L����
				if(op->ecmp!=op->ccmp) {								// �L���擪����ނƈقȂ�ꍇ��
					*cmp = op->ecmp;									// ������ނͱ�������
				} else {												// �L���擪����ނƓ����ꍇ��
					nxt = TRUE;											// ������ނ�������
				}
			} else {													// �������ނ������ȏꍇ��
				nxt = TRUE;												// ������ނ�������
			}
		} else if(*cmp==op->ecmp) {										// <������ނ��������ނ̏ꍇ>
			*cmp = (op->ccmp)->onxt;									// ������ނ͗L���擪����ނ̎�
			if((*cmp)!=NULL) {
				if(*cmp==op->ecmp||
                   ((*cmp)->cno==RING&&*cmp!=op->rscmp)) {				// ����������͖������ݸ޺���ނ̏ꍇ��
					nxt = TRUE;											// ������ނ�������
				}
			}
		} else {														// <������ނ���L�ȊO�̏ꍇ>
			nxt = TRUE;													// ������ނ�������
		}
		while(nxt&&(*cmp)!=NULL) {										// <������ތ�������>
			*cmp = (*cmp)->onxt;										// �������
			if(*cmp==NULL) {
				break;
			}
			if(((*cmp)->cno!=EARTH&&(*cmp)->cno!=RING)||				// ������ނ����&�ݸވȊO��
               (*cmp)==op->rscmp) {										// �L�����ݸ޺���ނ͌����I��
				break;
			}
		}
		if(*cmp!=NULL) {
			if(*cmp==op->rscmp) {										// ������ނ��ݸސ擪����ނ̏ꍇ��
				*cmp = op->rccmp;										// �ݸތ��ݺ���ނ֒u��������
			}
		}
	}
}

/*********************************/
void NextAllCmdPtr(CMDTYPE** cmp)										//<<< ��������߲��(�S����ޑΏ�)�擾
/*********************************/
{
	if(*cmp!=NULL) {
		*cmp = (*cmp)->onxt;											// �������(������)
	}
}

/***********************************/
void LevelBaseCmdPtr(CMDTYPE** cmp)										//<<< �������ٓ��擪����ގ擾
/***********************************/
{
	*cmp = Undo_ary[Undo_adr].cmp;
}

/***********************************/
void LevelNextCmdPtr(CMDTYPE** cmp)										//<<< �������ٓ�������ގ擾
/***********************************/
{
	if(*cmp!=NULL) {
		*cmp = (*cmp)->lnxt;											// ������ސݒ�
	}
}

/****************************************************/
void EditCmdPtr(int flg, OBJTYPE* op, CMDTYPE** cmp)					//<<< �ҏW������߲���擾
/****************************************************/					// flg 0:�ݸސ擪, 1:�ݸތ���, 2:���
{
	*cmp = (flg==0) ? op->rscmp : ((flg==1) ? op->rccmp : op->ecmp);	// �ݸސ擪�����/�ݸތ��ݺ����/��������
}

/******************************************************/
void SetEditCmdPtr(int flg, OBJTYPE* op, CMDTYPE* cmp)					//<<< �ҏW������߲���ݒ�
/******************************************************/				// flg 0:�ݸސ擪, 1:�ݸތ���, 2:���
{
	CMDTYPE* cptr;

	cptr = op->ccmp;
	if(flg==0) {														// <�ݸސ擪����ނ̏ꍇ>
		op->rscmp = NULL;												// �ݸސ擪����ޏ�����
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rscmp = cptr;										// �ŏ����ݸ޺��Đݒ�
				break;
			}
			cptr = cptr->onxt;
		}
	} else if(flg==1) {													// <�ݸތ��ݺ���ނ̏ꍇ>
		op->rccmp = NULL;												// �ݸތ��ݺ���ޏ�����
		while(cptr!=NULL) {
			if(cptr->cno==RING) {
				op->rccmp = cptr;										// �Ō���ݸ޺��Đݒ�
			}
			cptr = cptr->onxt;
		}
	} else {															// <�������ނ̏ꍇ>
		op->ecmp = NULL;												// �������ޏ�����
		while(cptr!=NULL) {
			if(cptr->cno==EARTH) {
				op->ecmp = cptr;										// �Ō�̱������ސݒ�
			}
			cptr = cptr->onxt;
		}
	}
}

/******************************************/
void ResetEditCmdPtr(int flg, OBJTYPE* op)								//<<< �ҏW������߲������
/******************************************/							// flg 0:�ݸސ擪, 1:�ݸތ���, 2:���
{
	if(flg==0) {
		op->rscmp = NULL;												// �ݸސ擪����ޏ�����
	} else if(flg==1) {
		op->rccmp = NULL;												// �ݸތ��ݺ���ޏ�����
	} else {
		op->ecmp = NULL;												// �������ޏ�����
	}
}

/****************************/
void SetCmdPtr(CMDTYPE* cmp)											//<<< �����Ώۺ���ސݒ�
/****************************/
{
	Rf_cmp = cmp;
}

/*****************************/
void GetCmdPtr(CMDTYPE** cmp)											//<<< �����Ώۺ���ގ擾
/*****************************/
{
	*cmp = Rf_cmp;
}

/******************************************/
void GetCmdObj(CMDTYPE* cmp, OBJTYPE** op)								//<<< ����ޑΏۗ��̎擾
/******************************************/
{
	*op = cmp->op;
}

/**************************/
int GetCmdNo(CMDTYPE* cmp)												//<<< �����No�擾
/**************************/
{
	return (int)cmp->cno;
}

/*************************************/
int GetObjCmdNo(int flg, OBJTYPE* op)									//<<< ���̺����No�擾
/*************************************/									// 0:�擪�����, 1:�L���擪�����
{
	CMDTYPE* cmp;
	int      cno=0;

	if(op!=NULL) {
		cmp = (flg==0) ? op->bcmp : op->ccmp;
		cno = (cmp!=NULL) ? (int)cmp->cno : 0;
	}
	return cno;
}

/***************************/
int GetIntNum(CMDTYPE *cmp)												//<<< �������Ұ����擾
/***************************/
{
	return cmp->inum;
}

/***********************************************/
void PutParaInt(CMDTYPE* cmp, int no, int data)							//<<< �������Ұ��l�ݒ�
/***********************************************/
{
	Int_ary[cmp->isa+no] = data;
}

/************************************/
int GetParaInt(CMDTYPE* cmp, int no)									//<<< �������Ұ��l�擾
/************************************/
{
	return Int_ary[cmp->isa+no];
}

/***************************/
int GetPntNum(CMDTYPE* cmp)												//<<< ���W���Ұ����擾
/***************************/
{
	return cmp->pnum;
}

/***************************************************/
void PutParaPnt(CMDTYPE* cmp, int no, PNTTYPE data)						//<<< ���W���Ұ��l�ݒ�
/***************************************************/
{
	Pnt_ary[cmp->psa+no] = data;
}

/****************************************************/
void GetParaPnt(CMDTYPE* cmp, int no, PNTTYPE* data)					//<<< ���W���Ұ��l�擾
/****************************************************/
{
	*data = Pnt_ary[cmp->psa+no];
}

/**********************/
void OnCmdFlgStp(void)													//<<< �ۑ��׸ލX�V���~ON
/**********************/
{
	Cmd_upd = 1;
}

/***********************/
void OffCmdFlgStp(void)													//<<< �ۑ��׸ލX�V���~OFF
/***********************/
{
	Cmd_upd = 0;
}

/*******************/
void OnCmdFlg(void)														//<<< ����ޖ��ۑ��׸�ON
/*******************/
{
	Cmd_flg = (Cmd_upd!=1) ? 1 : Cmd_flg;
}

/********************/
void OffCmdFlg(void)													//<<< ����ޖ��ۑ��׸�OFF
/********************/
{
	Cmd_flg = 0;
}

/**********************/
BOOL CheckCmdFlg(void)													//<<< ����ޖ��ۑ��׸ފm�F
/**********************/
{
	if(Cmd_flg==1) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**********************/
void OnCmdExeStp(void)													//<<< ����ގ��s��~�׸�ON
/**********************/
{
	Cmd_stp = 1;
}

/***********************/
void OffCmdExeStp(void)													//<<< ����ގ��s��~�׸�OFF
/***********************/
{
	Cmd_stp = 0;
}

/**********************/
int GetCmdExeStp(void)													//<<< ����ގ��s��~�׸ގ擾
/**********************/
{
	return (int)Cmd_stp;
}

/**********************************************************/
/*****                �R�s�[ �R�}���h�p               *****/
/**********************************************************/
/*************************************************************/
void CreatCopyCmd(int cno, int inum, int pnum, CPCTYPE** cmp)			//<<< ��߰����ޗ̈�m��
/*************************************************************/
{
	CPCTYPE* cpc;

	*cmp = (CPCTYPE *)malloc(sizeof(CPCTYPE));							// �V�K�̈�m��
	if(*cmp==NULL) {
		MemErr(IDS_MEMERR5);
	}
	(*cmp)->inum = inum;												// �������Ұ��̈�m��
	if(inum) {
		(*cmp)->isa = Int_adr, Int_adr += inum;
		if(Int_adr>=Int_max) {
			MemErr(IDS_MEMERR1);
		}
	} else {
		(*cmp)->isa = 0;
	}
	(*cmp)->pnum = pnum;												// ���W���Ұ��̈�m��
	if(pnum) {
		(*cmp)->psa = Pnt_adr, Pnt_adr += pnum;
		if(Pnt_adr>=Pnt_max) {
			MemErr(IDS_MEMERR2);
		}
	} else {
		(*cmp)->psa = 0;
	}
	(*cmp)->cno = (BYTE)cno;											// �����No�ݒ�
	(*cmp)->next = NULL;												// �߲��������
	if(Copy_cmp==NULL) {												// �擾�̈��Copy_cmp
		Copy_cmp = *cmp;												// �ɐڑ�
	} else {
		cpc = Copy_cmp;
		while(cpc->next!=NULL) {
			cpc = cpc->next;
		}
		cpc->next = *cmp;
	}
}

/**************************************/
static void FreeParaArea(CPCTYPE* cmp)									//<<< ���Ұ��̈���
/**************************************/
{
	int src, i;

	if(cmp->inum!=0) {													// �������Ұ��̈���
		src = cmp->isa+cmp->inum;
		for(i=src; i<Int_adr; i++) {
			Int_ary[i-cmp->inum] = Int_ary[i];
		}
		Int_adr -= cmp->inum;
	}
	if(cmp->pnum!=0) {													// ���W���Ұ��̈���
		src = cmp->psa+cmp->pnum;
		for(i=src; i<Pnt_adr; i++) {
			Pnt_ary[i-cmp->pnum] = Pnt_ary[i];
		}
		Pnt_adr -= cmp->pnum;
	}
	ChangeAreaAdr(cmp->isa, -cmp->inum, cmp->psa, -cmp->pnum);			// �z����ڽ�ύX
}

/**********************/
void FreeCopyCmd(void)													//<<< ��߰����ޗ̈���
/**********************/
{
	CPCTYPE* cpc;

	while(Copy_cmp!=NULL) {
		cpc = Copy_cmp, Copy_cmp = Copy_cmp->next;						// Copy_cmp����؂藣��
		FreeParaArea(cpc);												// ���Ұ��̈���
		free(cpc);														// ��؉��
	}
	Copy_cmp = NULL;
}

/**********************************/
void BaseCopyCmdPtr(CPCTYPE** cmp)										//<<< �擪��߰������߲���擾
/**********************************/
{
	*cmp = Copy_cmp;													// �ް������
}

/**********************************/
void NextCopyCmdPtr(CPCTYPE** cmp)										//<<< ����߰������߲���擾
/**********************************/
{
	if(*cmp!=NULL) {
		*cmp = (*cmp)->next;											// ������ސݒ�
	}
}

/******************************/
int GetCopyCmdNo(CPCTYPE* cmp)											//<<< ��߰�����No�擾
/******************************/
{
	return (int)cmp->cno;
}

/*******************************/
int GetCopyIntNum(CPCTYPE* cmp)											//<<< ��߰�����
/*******************************/										//    �������Ұ����擾
{
	return cmp->inum;
}

/***************************************************/
void PutCopyParaInt(CPCTYPE* cmp, int no, int data)						//<<< ��߰�����
/***************************************************/					//    �������Ұ��l�ݒ�
{
	Int_ary[cmp->isa+no] = data;
}

/****************************************************/
void GetCopyParaInt(CPCTYPE* cmp, int no, int* data)					//<<< ��߰�����
/****************************************************/					//    �������Ұ��l�擾
{
	*data = Int_ary[cmp->isa+no];
}

/*******************************/
int GetCopyPntNum(CPCTYPE* cmp)											//<<< ��߰�����
/*******************************/										//    ���W���Ұ����擾
{
	return cmp->pnum;
}

/*******************************************************/
void PutCopyParaPnt(CPCTYPE* cmp, int no, PNTTYPE data)					//<<< ��߰�����
/*******************************************************/				//    ���W���Ұ��l�ݒ�
{
	Pnt_ary[cmp->psa+no] = data;
}

/********************************************************/
void GetCopyParaPnt(CPCTYPE* cmp, int no, PNTTYPE* data)				//<<< ��߰�����
/********************************************************/				//    ���W���Ұ��l�擾
{
	*data = Pnt_ary[cmp->psa+no];
}

/*********************************************************/
/*****              �t�������^�q�������p             *****/
/*********************************************************/
/*********************/
void OnUndoRedo(void)													//<<< Undo�׸�ON
/*********************/
{
	Undo_flg = 1;
}

/**********************/
void OffUndoRedo(void)													//<<< Undo�׸�OFF
/**********************/
{
	Undo_flg = 0;
}

/************************/
BOOL CheckUndoRedo(void)												//<<< Undo�׸ފm�F
/************************/
{
	return Undo_flg;
}

/*********************/
void OnUndoStop(void)													//<<< Undo�ۑ���~�׸�ON
/*********************/
{
	Undo_stp = 1;
}

/**********************/
void OffUndoStop(void)													//<<< Undo�ۑ���~�׸�OFF
/**********************/
{
	Undo_stp = 0;
}

/********************/
void InitLevel(void)													//<<< Undo/Redo������
/********************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      wadr;

    Undo_adr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;				// ����UP
	if(Undo_ary[Undo_adr].urflg==0) {									// <Undo��Ԃ̏ꍇ>
		LevelBaseObjPtr(0, &op);										// �������̎擾
		while(op!=NULL) {
			FreeObj(op, 0);												// �S�������̍폜
			LevelNextObjPtr(0, &op);
		}
		LevelBaseCmdPtr(&cmp);											// �ҏW����ގ擾
		while(cmp!=NULL) {
			FreeCmd(op, cmp, 0);										// �S����ލ폜
			LevelNextCmdPtr(&cmp);
		}
	} else if(Undo_ary[Undo_adr].urflg==1) {							// <Undo����Ԃ̏ꍇ>
		LevelBaseObjPtr(1, &op);										// �ꎞ�폜���̎擾
		while(op!=NULL) {
			FreeObj(op, 0);												// �S�ꎞ�폜���̍폜
			LevelNextObjPtr(1, &op);
		}
	}
	Undo_ary[Undo_adr].urflg = -1;										// Undo�̈揉����
	Undo_ary[Undo_adr].cmflg = -1;
	Undo_ary[Undo_adr].cmp = NULL;
	Undo_ary[Undo_adr].cop = NULL;
	Undo_ary[Undo_adr].dop = NULL;
	Undo_ary[Undo_adr].clvl = ++Cmd_Lvl;
    wadr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;
	Undo_ary[wadr].cmflg = -1;
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/********************/
void BackLevel(void)													//<<< Undo/Redo�߂�
/********************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;

	LevelBaseObjPtr(0, &op);											// �������̎擾
	while(op!=NULL) {
		FreeObj(op, 0);													// �S�������̍폜
		LevelNextObjPtr(0, &op);
	}
	LevelBaseCmdPtr(&cmp);												// �ҏW����ގ擾
	while(cmp!=NULL) {
		FreeCmd(op, cmp, 0);											// �S����ލ폜
		LevelNextCmdPtr(&cmp);
	}
    Undo_adr = (Undo_adr-1<0) ? Undo_max-1 : Undo_adr-1;				// ����DOWN
    Cmd_Lvl = (Cmd_Lvl-1<0) ? 0 : Cmd_Lvl-1;							// ���������NoDOWN
	View->SetDataUse();													// �ް��g�p���ݒ�
}

/********************/
BOOL CheckUndo(void)													//<<< Undo�m�F
/********************/
{
	if(Undo_flg==0) {
		return FALSE;
	}
	if(Undo_ary[Undo_adr].urflg==1) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/*******************/
void Undo(int* gno)														//<<< Undo����
/*******************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, ogn[32];

	for(i=0; i<32; i++) {
		gno[i] = 0;
	}
	LevelBaseObjPtr(0, &op);											// �������̌���
	while(op!=NULL) {
		GetGroup(op, ogn);												// ��ٰ�ߏ��擾
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// �Ώ۸�ٰ��ON
		}
		TempObj(op);													// ���̈ꎞ�폜
		LevelNextObjPtr(0, &op);										// ���������̌���
	}
	LevelBaseCmdPtr(&cmp);												// �ҏW����ތ���
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// ����ޑΏۗ��̎擾
		GetGroup(op, ogn);												// ��ٰ�ߏ��擾
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// �Ώ۸�ٰ��ON
		}
		TempCmd(op, cmp);												// ����ވꎞ�폜
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextCmdPtr(&cmp);											// ���ҏW����ތ���
	}
	LevelBaseObjPtr(1, &op);											// �폜���̌���
	while(op!=NULL) {
		UndoObj(op);													// ���̍Đڑ�
		RedoCommand(op);												// ����ލĎ��s
		GetGroup(op, ogn);												// ��ٰ�ߏ��擾
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// �Ώ۸�ٰ��ON
		}
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextObjPtr(1, &op);										// ���폜���̌���
	}
	UndoMenu(Undo_ary[Undo_adr].clvl);									// Undo�ƭ�����
	Undo_ary[Undo_adr].urflg = 0;										// Undo��
    Undo_adr = (Undo_adr-1<0) ? Undo_max-1 : Undo_adr-1;				// ����DOWN
}

/********************/
BOOL CheckRedo(void)													//<<< Redo�m�F
/********************/
{
	int wadr;

	if(Undo_flg==0) {
		return FALSE;
	}
	wadr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;					// ���̈���m�F
	if(Undo_ary[wadr].urflg!=0) {
		return FALSE;
	}
	if(Undo_ary[wadr].cmflg!=0&&Undo_ary[Undo_adr].urflg!=0) {
		return FALSE;
	}
	return TRUE;
}

/*******************/
void Redo(int* gno)														//<<< Redo����
/*******************/
{
	OBJTYPE* op;
	CMDTYPE* cmp;
	int      i, ogn[32];

	for(i=0; i<32; i++) {
		gno[i] = 0;
	}
    Undo_adr = (Undo_adr+1>Undo_max-1) ? 0 : Undo_adr+1;				// ����UP
	LevelBaseObjPtr(0, &op);											// �������̌���
	while(op!=NULL) {
		UndoObj(op);													// ���̍Đڑ�
		RedoCommand(op);												// ����ލĎ��s
		GetGroup(op, ogn);												// ��ٰ�ߏ��擾
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// �Ώ۸�ٰ��ON
		}
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextObjPtr(0, &op);										// ���������̌���
	}
	LevelBaseCmdPtr(&cmp);												// �ҏW����ތ���
	while(cmp!=NULL) {
		GetCmdObj(cmp, &op);											// ����ޑΏۗ��̎擾
		GetGroup(op, ogn);												// ��ٰ�ߏ��擾
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// �Ώ۸�ٰ��ON
		}
		UndoCmd(op, cmp);												// ����ލĐڑ�
		RedoCommand(op);												// ����ލĎ��s
		View->SetDispList(op);											// �ި���ڲ�ؽēo�^
		LevelNextCmdPtr(&cmp);											// ���ҏW����ތ���
	}
	LevelBaseObjPtr(1, &op);											// �폜���̌���
	while(op!=NULL) {
		TempObj(op);													// ���̈ꎞ�폜
		GetGroup(op, ogn);												// ��ٰ�ߏ��擾
		for(i=0; i<32; i++) {
			gno[i] = (ogn[i]) ? 1 : gno[i];								// �Ώ۸�ٰ��ON
		}
		LevelNextObjPtr(1, &op);										// ���폜���̌���
	}
	RedoMenu(Undo_ary[Undo_adr].clvl);									// Redo�ƭ�����
	Undo_ary[Undo_adr].urflg = 1;										// Redo��
}
