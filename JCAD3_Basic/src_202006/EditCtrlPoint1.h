/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW����_���암�v���g�^�C�v�錾
// EditCtrlPoint1.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void MoveCPoint(int, PNTTYPE*, OBJTYPE*, int, int, int, int, double,
                int*, int*, PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);		// ����_�ړ�
void TwistCLine(PNTTYPE*, VECTYPE*, double, int, int*, int, OBJTYPE*,
               int, int, int, int, PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);	// ����_�Ђ˂�
void ScaleCLine(PNTTYPE*, VECTYPE*, int, int, double, int, int*, int,
     OBJTYPE*, int, int, int, int, PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);	// ����_�g��/�k��
void MoveSCurve(int, double*, PNTTYPE*, OBJTYPE*, int, int, PNTTYPE*,
                PNTTYPE*, int*, PNTTYPE*, int*);						// �f�ʐ���_�ړ�
void MoveSPoint(OBJTYPE*, int, int, int, int, PNTTYPE*, PNTTYPE*,
                PNTTYPE*, int*);										// �f�ʐ���_�ړ�
void MoveBPoint(int, PNTTYPE*, PNTTYPE*, OBJTYPE*, int, int,
                int, int, double, int, int, int*, int*,
                PNTTYPE*, PNTTYPE*, PNTTYPE*, int*);					// �ް����ړ�

//===========================<EOF:EditCtrlPoint1.h>==========================