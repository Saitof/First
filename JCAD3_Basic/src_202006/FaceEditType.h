/////////////////////////////////////////////////////////////////////////////
// ��ʕҏW�p�\���̋y�у}�N����`
// FaceEditType.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
typedef struct faetp {
	struct  faetp* next;												// ���ް����߲��
	double  xmin;														// �ʂ�X�ŏ��l
	double  xmax;														// �ʂ�X�ő�l
	double  ymin;														// �ʂ�Y�ŏ��l
	double  ymax;														// �ʂ�Y�ő�l
	VECTYPE vec;														// �ʂ̖@���޸��
	int     stfl;														// ��ٰ�ߊJ�n�Ԓn
} FAETYPE;																//<< �ʏ�񒊏o�p >>

FAETYPE* Flist;															// �ʏ�񒊏oؽ��߲��
int      Fcnt;															// �ʏ��ؽĐ�
double   XPos1, XPos2, YPos1, YPos2;									// ��Ɨ̈��߼޼��

//===========================<EOF:FaceEditType.h>============================
