/////////////////////////////////////////////////////////////////////////////
// ���ʕ�����Ɨp�\���̋y�у}�N����`
// CutType.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/11 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
typedef struct cvltp {
	struct  cvltp* next;												// ���ް����߲��
	int     no;															// ��ٰ�ߔԍ�
	PNTTYPE pt;															// ���_���W
	int     vt;															// ���_�ԍ�
	int     flg;														// ��ٰ�ߏI���׸�
} CVLTYPE;																//<< ���_�ٰ�ߐݒ�p >>

typedef struct ccvtp {
	struct  ccvtp* next;												// ���ް����߲��
	PNTTYPE spt;														// �J�n���_���W
	PNTTYPE ept;														// �I�����_���W
} CCVTYPE;																//<< �����f�ʐݒ�p >>

typedef struct ccptp {
	struct  ccptp* next;												// ���ް����߲��
	PNTTYPE spt;														// �J�n���_���W
	PNTTYPE ept;														// �I�����_���W
} CCPTYPE;																//<< �����z�� >>

typedef struct ccltp {
	struct   ccltp* next;												// ���ް����߲��
	PNTTYPE  spt;														// �J�n���_���W
	PNTTYPE  ept;														// �I�����_���W
	CCPTYPE* cp;														// �����z����߲��
} CCLTYPE;																//<< �ʕ��������z�� >>

CVLTYPE* Avlist, * Bvlist;												// A/Bؽ��߲��
CCVTYPE* Cvlist;														// �����f��ؽ��߲��
CCLTYPE* Aclist, * Bclist;												// A/B�����z��ؽ��߲��
CCPTYPE* Cplist;														// ������_ؽ��߲��
int      Avcnt, Bvcnt;													// A/BؽĐ�
int      Cvcnt;															// �����f��ؽĐ�
int      Accnt, Bccnt;													// A/B�����z��
int      Cpcnt;															// ������_��
double   Oper, Sper;													// �i�����p�߰���
double   ReszC;															// ���ޕ␳�l

//==============================<EOF:CutType.h>==============================
