/////////////////////////////////////////////////////////////////////////////
// �Ȗʐ���_�������v���g�^�C�v�錾
// CreatCtrlPoint2.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/21 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL InputBasePoint1(int*);												// �f�ʐ����ʒu����1
BOOL InputBasePoint2(int*, int*, int*);									// �f�ʐ����ʒu����2
void ChangeLoop(int, PNTTYPE*, int*);									// ��ٰ�ߔ��]
void InitRevolve(int, int, PNTTYPE*, int*, int*, PNTTYPE*,int*);		// ��]��������
void CalcRevolvePoint(int, int, int, int, int, int, int, int, int,
                    double, PNTTYPE, PNTTYPE, PNTTYPE, PNTTYPE*, int*);	// ��]�̐���_�v�Z
void SetSRevolvePoint(int, int, int, int, int, double, PNTTYPE*,
                      int*, PNTTYPE);									// ��]�̿د�ސ���_�쐬
BOOL GetRevolveInfo(int, int, double*, int*);							// ��]�̏��擾
void InitPyramid(int, int, PNTTYPE*, PNTTYPE*);							// ����������
void Pyramid(int, int, int, PNTTYPE*, int*, PNTTYPE, int,
             PNTTYPE*, int*);											// ������_����
void CreatSpiralObject(int, int, int, int, double, double, int, int,
                       PNTTYPE, PNTTYPE, PNTTYPE*, int*);				// �����Ȗʐ���
void ReverseSurf(int, int, PNTTYPE*, int*);								// �Ȗʐ���_���]
BOOL CheckSurface(int, int*, int*, int*, int*, int*, int*);				// �ȖʘA������
void GetSEPoint(int, int, int, OBJTYPE**, int*, int*, int*,
                int*, PNTTYPE*, PNTTYPE*, SCRTYPE*, int*, int*);		// �n�_�I�_����_�擾
BOOL ConctSEPoint(int, PNTTYPE*, PNTTYPE*, SCRTYPE*);					// �n�_�I�_����_�A��
BOOL GetConctObj(int, SCRTYPE*, SCRTYPE*);								// �A�����̏��擾
void ConctCurve(int, int, OBJTYPE**, SCRTYPE*);							// �Ȑ��A��
void ConctSurface(int, int, int, OBJTYPE**, SCRTYPE*, SCRTYPE*);		// �ʏ�ȖʘA��

//==========================<EOF:CreatCtrlPoint2.h>==========================