/////////////////////////////////////////////////////////////////////////////
// �Ȗʐ���_�������v���g�^�C�v�錾
// CreatCtrlPoint1.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/27 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void SetBasePoint(int, int, BOXTYPE, PNTTYPE*);							// �f�ʐ������W�ݒ�
BOOL InitCreat(int, int, int, PNTTYPE*, int*, int*);					// ������������
void InitCreat2(int, int, int, PNTTYPE*, int*);							// ������������2
int  GetCurveCenter(int, int, int, PNTTYPE*, PNTTYPE*);					// 2D�}�`���S�擾
void ChangeRotate2D(int, int, int, PNTTYPE*);							// 2D�f�ʉ�]�X�V
void InitSweep(PNTTYPE, int, int, int, PNTTYPE*);						// �|����������
void CalcSweepPoint(int, int, int, int, int, int, int, PNTTYPE*,
                    int*, PNTTYPE*, int*, PNTTYPE*, int*, double*);		// �|���̐���_�v�Z
BOOL InsertSweepPoint(int, int, int, int*, int*, PNTTYPE*, int*);		// �|���̒��Ԑ���_�}��
void SetCircleSection(int, int, double, int, int, PNTTYPE*,
                      PNTTYPE*, PNTTYPE*, int*);						// �~�f�ʊ��t
void SetCurveSection(int, int, int, int, int, int, BOXTYPE, int,
                     int, PNTTYPE*, int*, PNTTYPE*, int*);				// �Ȑ��f�ʊ��t
void CalcCurveSection(int, int, int, int, int, int, PNTTYPE*,
                      PNTTYPE*, int*, PNTTYPE*, int*);                  // �Ȑ����t�f�ʌv�Z
void CrtSectionSweep(int, int, int, int, int, int, PNTTYPE*,
                     PNTTYPE*, int*, PNTTYPE*, int*, PNTTYPE*, int*);	// ���t�f�ʑ|���̐���
void AddCenterPoint(int*, int, int, PNTTYPE*, int*);					// �[�ʒ��S�_�ǉ�
void CreatSetCurve(int, int, int, int, int, int);						// �Ȑ��g����

//==========================<EOF:CreatCtrlPoint1.h>==========================
