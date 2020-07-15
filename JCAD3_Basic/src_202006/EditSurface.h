/////////////////////////////////////////////////////////////////////////////
// �ȖʕҏW���암�v���g�^�C�v�錾
// EditSurface.h
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void GetSurfCmd1(CMDTYPE*, int*, int*, int*, int*, int*, double*,
                 int*, PNTTYPE*, int*);									// �Ȗʺ���ގ擾1
void GetSurfCmd2(CMDTYPE*, int, int*, int*, int*, int*, int*, int*,
                 double*, int*, PNTTYPE*, int*, int*, PNTTYPE*, int*);	// �Ȗʺ���ގ擾2
void ExecSurface(int, OBJTYPE*, int, int, int, int, int, double,
                 int, PNTTYPE*, int*, PNTTYPE*, OBJTYPE**);				// �ʏ�Ȗʎ��s
void ExecSweepSurf(OBJTYPE*, int, int, int, int, int, double, int,
                   int, PNTTYPE*, int*, PNTTYPE*, PNTTYPE*, OBJTYPE**);	// �|���Ȗʎ��s
void AttachObj(OBJTYPE*);												// ����ލĐڑ�/�Ď��s
void PickPointMove(int, int, int*, int, int, int, int, double,
                   int, int, int, int, OBJTYPE*, PNTTYPE*,
                   PNTTYPE*, int*, int*, int*);							// ����_�ړ� �_�I��
void PickLineMove(int, int, int*, int, int, int, int, int,
                  double, int, int, int, int, int, int*,
                  OBJTYPE*, PNTTYPE*, PNTTYPE*, PNTTYPE*,
                  PNTTYPE*, int*, int*, int*, int*, int*);				// ����_��ړ� ��I��
BOOL CheckLineCopy(int, int, int*, int, int, int, int, int, int*, int*);// ����_�񕡎�����
void PickLineInsert(int, int, int*, int*, int*, int, int, int*,
                    int, int, int, int*, int, OBJTYPE*, PNTTYPE,
                    PNTTYPE*, PNTTYPE*, PNTTYPE*, int*, int*);			// ����_��}�� ��I��
BOOL PickLineDivide(int, int, int*, int, int, int, int, int, double,
                    int, OBJTYPE*, PNTTYPE*, PNTTYPE*, int*, int*);		// ����_�񕪊�
void PickSurfLine(int, int*, int, int, OBJTYPE*, int*, int*, int*);		// ����_��I��
void PickSPointMove1(int, int, double*, double*, int, int, int, int,
                     int, int, double, int, int, int*, PNTTYPE*,
                     PNTTYPE*, PNTTYPE*, PNTTYPE*, PNTTYPE*, VECTYPE*,
                     PNTTYPE*, int*, int*, int*, int*, int*, int*,
                     int*, OBJTYPE*, OBJTYPE*, OBJTYPE**, OBJTYPE**);	// �f�ʐ���_�ړ� ��I��
void PickSPointMove2(int, int*, int, double*, double*, int, int, double,
                     int, PNTTYPE*, PNTTYPE*, int*, int*, int*);		// �f�ʐ���_�ړ� �_�I��
void ChangeSPointMove(int, int, int, int, int, int, int, int, int,
                      double, int, int, int, PNTTYPE, VECTYPE, PNTTYPE,
                      PNTTYPE*, PNTTYPE*, PNTTYPE*, PNTTYPE*,
                      PNTTYPE*, int*, int*, int*, int*, CMDTYPE*,
                      OBJTYPE*, OBJTYPE*, OBJTYPE*, OBJTYPE*);			// �f�ʐ���_�ړ� �f�ʕύX
void PickBPointMove(int, int*, int, int, OBJTYPE*, int*, int*);			// �ް����ړ� ��I��
BOOL InitCScale(int*, int*);											// �g��/�k����������
void EraseObjSPointMove(int*, int*, OBJTYPE*);							// �f�ʐ���_�ړ� ���̔�\��
void DispObjSPointMove(int, int*, OBJTYPE*);							// �f�ʐ���_�ړ� ���̕\��
void AdjustCPoint(int, int, int, PNTTYPE*, int, int, int*, PNTTYPE*,
                  PNTTYPE*, PNTTYPE*, PNTTYPE*, int*, int*, int*);		// ����_�Ȑ����킹
BOOL GetSurfaceCurve(int, int, int, int, int, int, int, int,
                     OBJTYPE*, int*, int*, PNTTYPE*);					// �ȖʋȐ��擾

//============================<EOF:EditSurface.h>============================