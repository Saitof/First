/////////////////////////////////////////////////////////////////////////////
// �Ȑ��ҏW���암�v���g�^�C�v�錾
// CreatCurvePoint.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void InsertCurvePoint(int*, int*, PNTTYPE*, int*);						// S�Ȑ����Ԑ���_�}��
void CalcRPoint(int, int, int, double, int*, PNTTYPE*, int*);			// R���_�ʒu�Z�o
void CalcSpiralPoint(PNTTYPE, int, double, double, int, int,
                     PNTTYPE*, int*);									// ��������_�v�Z
void TrackerECurve(int, double*, int, int, PNTTYPE*, int*, int*,
                   int*, PNTTYPE*, int*);								// �Ȑ��ҏW�ׯ���擾
void DeleteCurvePoint(int, double*, double*, int, double, int*, int,
                      PNTTYPE*, int*, int*);							// ����_��ʉߓ_�폜
void PickECurve(int, int*, int, double*, double*, int*, int, int,
                double, int*, OBJTYPE*, int, PNTTYPE*,
                PNTTYPE*, int*, int*, int*);							// �Ȑ��ҏW �_�I��
void MoveECurve(int, double*, PNTTYPE*, OBJTYPE*, int, int,
                PNTTYPE*, PNTTYPE*, int*, PNTTYPE*, int*);				// ����_��ʉߓ_�ړ�
BOOL ExtractionCurve(int, int, int, int, int, int, int,
                     PNTTYPE*, int*, int*, int*, PNTTYPE*, int*);		// �Ȑ����o

//==========================<EOF:CreatCurvePoint.h>==========================