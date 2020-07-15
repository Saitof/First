/////////////////////////////////////////////////////////////////////////////
// ���̑����ݒ�R�}���h��
// Attribute.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"

/***************************/
OBJTYPE* SetAttribute(void)												//<<< ����No�ݒ�����
/***************************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;
	int      atr;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	if((atr=GetParaInt(cmp, 0))!=30) {									// ��JEWEL�ȊO:�������ݒ�
		PutObjAtr(op, atr);
	} else {															// ��JEWEL:DIRECT[GEM]����
		PutObjAtr(op, 1);
	}
	FreeCmd(op, cmp, 0);												// ����No����ލ폜
	return op;
}

/**********************************************/
void WriteAttributeFile(FILE* fp, OBJTYPE* op)							//<<< ����No̧�ٕۑ�
/**********************************************/
{
	fprintf(fp, "C%02d 1 0\n", ATTRIBUTE);								// ̧�قɏ���
	fprintf(fp, "%d\n", GetObjAtr(op));
}

/***********************************************************/
void WriteAttributeFileJgd(FILE* fp, OBJTYPE* op, int obno)				//<<< ����No̧�ٕۑ�(JGD�p)
/***********************************************************/
{
	fprintf(fp, "C%02d %d %d 1 0\n", ATTRIBUTE, obno, -1);				// ̧�قɏ���
	fprintf(fp, "%d\n", GetObjAtr(op));
}

/***********************/
OBJTYPE* SetColor(void)													//<<< �װNo�ݒ�����
/***********************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	PutObjCol(op, GetParaInt(cmp, 0));									// �װNo�ݒ�
	FreeCmd(op, cmp, 0);												// �װNo����ލ폜
	return op;
}

/******************************************/
void WriteColorFile(FILE* fp, OBJTYPE* op)								//<<< �װNo̧�ٕۑ�
/******************************************/
{
	fprintf(fp, "C%02d 1 0\n", COLOR);									// ̧�قɏ���
	fprintf(fp, "%d\n", GetObjCol(op));
}

/*******************************************************/
void WriteColorFileJgd(FILE* fp, OBJTYPE* op, int obno)					//<<< �װNo̧�ٕۑ�(JGD�p)
/*******************************************************/
{
	fprintf(fp, "C%02d %d %d 1 0\n", COLOR, obno, -1);					// ̧�قɏ���
	fprintf(fp, "%d\n", GetObjCol(op));
}

/***********************/
OBJTYPE* SetGroup(void)													//<<< ��ٰ�ߐݒ�����
/***********************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;
	int      i, prm, gno[32];

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	for(i=0; i<8; i++) {
		prm = GetParaInt(cmp, i);										// ���Ұ��擾
		gno[i] = (prm%2==1) ? 1 : 0;									// ��ٰ��1�`8�ݒ�
		prm = (prm%2) ? prm-1 : prm;
		prm = prm/2;
		gno[i+8] = (prm%2==1) ? 1 : 0;									// ��ٰ��9�`16�ݒ�
		prm = (prm%2) ? prm-1 : prm;
		prm = prm/2;
		gno[i+16] = (prm%2==1) ? 1 : 0;									// ��ٰ��17�`24�ݒ�
		gno[i+24] = (prm/2==1) ? 1 : 0;									// ��ٰ��25�`32�ݒ�
	}
	PutGroup(op, gno);													// ��ٰ�ߏ��ݒ�
	FreeCmd(op, cmp, 0);												// ��ٰ�ߺ���ލ폜
	return op;
}

/******************************************/
void WriteGroupFile(FILE* fp, OBJTYPE* op)								//<<< ��ٰ�ߏ��̧�ٕۑ�
/******************************************/
{
	int	i, gno[32], gno2[8];

	GetGroup(op, gno);													// ��ٰ�ߏ��擾
	for(i=0; i<8; i++) {
		gno2[i] = gno[i]+(gno[i+8]*2)+(gno[i+16]*4)+(gno[i+24]*8);		// 1-8:+1,9-16:+2,17-24:+4,25-32:+8
	}
	fprintf(fp, "C%02d 8 0\n", GROUP);									// ̧�قɏ���
	fprintf(fp, "%d %d %d %d ",  gno2[0], gno2[1], gno2[2], gno2[3]);
	fprintf(fp, "%d %d %d %d\n", gno2[4], gno2[5], gno2[6], gno2[7]);
}

/*******************************************************/
void WriteGroupFileJgd(FILE* fp, OBJTYPE* op, int obno)					//<<< ��ٰ�ߏ��̧�ٕۑ�(JGD�p)
/*******************************************************/
{
	int	i, gno[32], gno2[8];

	GetGroup(op, gno);													// ��ٰ�ߏ��擾
	for(i=0; i<8; i++) {
		gno2[i] = gno[i]+(gno[i+8]*2)+(gno[i+16]*4)+(gno[i+24]*8);		// 1-8:+1,9-16:+2,17-24:+4,25-32:+8
	}
	fprintf(fp, "C%02d %d %d 8 0\n", GROUP, obno, -1);					// ̧�قɏ���
	fprintf(fp, "%d %d %d %d ",  gno2[0], gno2[1], gno2[2], gno2[3]);
	fprintf(fp, "%d %d %d %d\n", gno2[4], gno2[5], gno2[6], gno2[7]);
}

/***********************/
OBJTYPE* SetParts(void)													//<<< �߰�No�ݒ�����
/***********************/
{
	CMDTYPE* cmp;
	OBJTYPE* op;

	GetCmdPtr(&cmp);													// ������߲���擾
	GetObjPtr(&op);														// �Ώۗ��̎擾
	AppendCmd(op, cmp);													// ����&����ސڑ�
	PutObjPrt(op, GetParaInt(cmp, 0), 1);								// �߰�No�ݒ�
	FreeCmd(op, cmp, 0);												// �߰�No����ލ폜
	return op;
}

/******************************************/
void WritePartsFile(FILE* fp, OBJTYPE* op)								//<<< �߰�No̧�ٕۑ�
/******************************************/
{
	fprintf(fp, "C%02d 1 0\n", PARTS);									// ̧�قɏ���
	fprintf(fp, "%d\n", GetObjPrt(op));
}

/*******************************************************/
void WritePartsFileJgd(FILE* fp, OBJTYPE* op, int obno)					//<<< �߰�No̧�ٕۑ�(JGD�p)
/*******************************************************/
{
	fprintf(fp, "C%02d %d %d 1 0\n", PARTS, obno, -1);					// ̧�قɏ���
	fprintf(fp, "%d\n", GetObjPrt(op));
}
