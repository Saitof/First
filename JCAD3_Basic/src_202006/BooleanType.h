/////////////////////////////////////////////////////////////////////////////
// �}�`���Z��Ɨp�\���̋y�у}�N����`
// BooleanType.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/24by M.Fukushima
/////////////////////////////////////////////////////////////////////////////

typedef struct bfatp {
	struct  bfatp* next;												// ���ް����߲��
	double  xmin;														// �ʂ�X�ŏ��l
	double  xmax;														// �ʂ�X�ő�l
	double  ymin;														// �ʂ�Y�ŏ��l
	double  ymax;														// �ʂ�Y�ő�l
	double  zmin;														// �ʂ�Z�ŏ��l
	double  zmax;														// �ʂ�Z�ő�l
	VECTYPE vec;														// �ʂ̖@���޸��
	PNTTYPE inpt;														// �ʂ̒��ɑ��݂���_
	int     stfl;														// ��ٰ�ߊJ�n�Ԓn
} BFATYPE;																//<< �ʏ�񒊏o�p >>

typedef struct bfctp {
	struct  bfctp* next;												// ���ް����߲��
	int     flg;														// �ް��׸�(0:�o��,1:��r)
	int     sno;														// �n�_�ԍ�(ٰ��/���_)
	int     eno;														// �I�_�ԍ�(ٰ��/���_)
	PNTTYPE pnt;														// ��_
} BFCTYPE;																//<< �ʌ�_�Z�o�p >>

typedef struct bcltp {
	struct  bcltp* next;												// ���ް����߲��
	int     sfg;														// �n�_��ް��׸�
	int     ssn;														// �n�_��n�_�ԍ�
	int     sen;														// �n�_��I�_�ԍ�
	PNTTYPE spt;														// �n�_���_
	int     efg;														// �I�_��ް��׸�
	int     esn;														// �I�_��n�_�ԍ�
	int     een;														// �I�_��I�_�ԍ�
	PNTTYPE ept;														// �I�_���_
	int     pfg;														// �o���׸�
	short   cfs;														// �n�_��m�F�׸�
	short   cfe;														// �I�_��m�F�׸�
} BCLTYPE;																//<< �ʌ����Ő��Z�o�p >>

typedef struct bcptp {
	struct  bcptp* next;												// ���ް����߲��
	PNTTYPE pnt;														// ���_���W
} BCPTYPE;																//<< ���������_���W >>

typedef struct bcdtp {
	struct   bcdtp* next;												// ���ް����߲��
	int      flg;														// �������׸�(0:Out,1:In)
	int      ssn;														// ٰ�ߊJ�n�Ԓn(Out)
	int      sen;
	PNTTYPE  spt;														// ٰ�ߊJ�n���_
	double   sln;														// �J�n���_�ʒu
	int      sfg;														// �J�n�����׸�
	int      esn;														// ٰ�ߏI���Ԓn(Out)
	int      een;
	PNTTYPE  ept;														// ٰ�ߏI�����_
	double   eln;														// �I�����_�ʒu
	int      efg;														// �I�������׸�
	int      sno;														// �ݒ�ԍ�
	int      pfg;														// �o���׸�
	BCPTYPE* cpp;														// ���_���W���߲��
} BCDTYPE;																//<< �������擪�ް� >>

typedef struct bvltp {
	struct  bvltp* next;												// ���ް����߲��
	int     no;															// ��ٰ�ߔԍ�
	PNTTYPE pt;															// ���_���W
	int     vt;															// ���_�ԍ�
	short   lfg;														// ��ٰ�ߏI���׸�
	short   pfg;														// ��ٰ�ߏo���׸�
} BVLTYPE;																//<< ���_�ٰ�ߐݒ�p >>

typedef struct bvttp {
	struct  bvttp* next;												// ���ް����߲��
	PNTTYPE pt;															// ���_���W
	int     vt;															// ���_�ԍ�
} BVTTYPE;																//<< ���_�ݒ�p >>

typedef struct boptp {
	struct   boptp* next;												// ���ް����߲��
	OBJTYPE* op;														// �Ώۗ��̂��߲��
	int      flg;														// �׸�
	PNTTYPE  cpt;														// �Ώۗ��̒��S�_
	double   leng;														// �Ώۗ��̊�_����
} BOPTYPE;																//<< �}�`���Z�Ώۗ����ް� >>

BFATYPE* Aflist, * Bflist;												// �ʏ�񒊏oؽ��߲��
BVLTYPE* A0list, * A1list, * B0list, * B1list;							// ���_�ٰ�ߐݒ�ؽ��߲��
BFCTYPE* Fclist;														// �ʌ�_�Z�o�pؽ��߲��
BCLTYPE* Cllist;														// �ʌ����Ő��Z�o�pؽ��߲��
BCDTYPE* Cdlist;														// �������擪�ް�ؽ��߲��
int      Afcnt, Bfcnt;													// �ʏ��ؽĐ�
int      A0cnt, A1cnt, B0cnt, B1cnt;									// ���_���ٰ��ؽĐ�
int      Cocnt, Cicnt;													// �������擪�ް�ؽĐ�
int      ErrFlg1, ErrFlg2;												// �װ�׸�
double   Spr;															// �i�����p�߰���
double   Ppr;															// ���Z�����߰���
double   ReszB;															// ���ޕ␳�l

//============================<EOF:BooleanType.h>============================
