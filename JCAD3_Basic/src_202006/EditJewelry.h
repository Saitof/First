/////////////////////////////////////////////////////////////////////////////
// ��Ί֌W�ҏW���v���g�^�C�v�錾
// EditJewelry.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL KeyboardJwlRing(int, int, double*, double*);						// ����,�ڍ��Ԋu����
BOOL KeyboardStoneForm(int*);											// �Θg�`�����
BOOL KeyboardRoundSize(double*);										// �Θg���a����
BOOL KeyboardSquareSize(double*, double*);								// �Θg���a����
BOOL KeyboardProng(int*, double*, double*, double*);					// �܏�����
BOOL KeyboardArrange(double*, int*);									// �z�u������
void TrackerStone(int, PNTTYPE, double, double, double, double,
                  int*, PNTTYPE*, int*, int*);							// �Θg�ׯ���쐬
void RoundStone(PNTTYPE, double, double, double, int, int,
                PNTTYPE*, int*, PNTTYPE*, int*);						// ׳��ސΘg����
void SquareStone(PNTTYPE, double, double, double, double, double,
                 int, PNTTYPE*, int*, PNTTYPE*, int*);					// �����Θg����
void SetProng(int, double, double, double, BOXTYPE, int, PNTTYPE*);		// �Θg�ܐݒ�

//=============================<EOF:EditJewelry.h>===========================