/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW����_���암�v���g�^�C�v�錾
// EditCtrlPoint2.h
//---------------------------------------------------------------------------
// LastEdit : 2009/06/26 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void DeleteCLine(int, int*, int, int, int, int, int*, int*, PNTTYPE*,
                 int*, int*);											// �Ώې���_��폜
void InsertCLine(int, int, int, int*, int*, int, int*, int, PNTTYPE,
                 PNTTYPE*, int*);										// �Ώې���_��}��
void PutSPoint(int, int, int, int, int, PNTTYPE, PNTTYPE*, int*,
               PNTTYPE, VECTYPE, PNTTYPE*, PNTTYPE*, int*);				// �f�ʐ���_���X�V
void SetSweepCtrl(int, int, int, int, PNTTYPE*, int*, PNTTYPE*, int*);	// �|���Ȗʐ���_�ϊ�
void DeleteSweep(int, int*, int, int, int*, int*, PNTTYPE*, int*);		// �|������_��폜
void MoveSweep(PNTTYPE*, int, int, int, PNTTYPE*, int, int*, PNTTYPE*);	// �|������_�ړ�
void InsertSweep(int, int, int, int*, int*, PNTTYPE*, int*);			// �|������_��}��
void CtrlPointSpace(PNTTYPE*, PNTTYPE*, int*, int, PNTTYPE, PNTTYPE,
                    int, int, double, double, double, double, double,
                    double, double, double, double, double);			// ����_��߰�(CPS)

//===========================<EOF:EditCtrlPoint2.h>==========================
