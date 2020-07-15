/////////////////////////////////////////////////////////////////////////////
// ���l�֐����v���g�^�C�v�錾
// NumFunc.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
double VecL(VECTYPE*);													// �޸�ْ���
void VecO(VECTYPE*, VECTYPE*, VECTYPE*);								// 3�����޸�يO��
void VecN(VECTYPE*);													// �P���޸�ى�(3D)
void VecV(PNTTYPE*, PNTTYPE*, VECTYPE*);								// �n�_->�I�_�޸��
double PntD(PNTTYPE*, PNTTYPE*);										// 2�_�ԋ���(3D)
double CalcArea(double, double, double);								// �O�p�`�ʐώZ�o
void SetRotateMatrix(VECTYPE axis, double th, double rot[][3]);			// ��]��د�����
void VecRotate(PNTTYPE* pos, double rot[][3]);							// ���_���W��]
void VecRotateMove(PNTTYPE* pos, double rot[][3], PNTTYPE dis);			// ���_���W��]&���s�ړ�
void Affine(OBJTYPE* op, PNTTYPE cp, double mat[3][3]);					// ���_���W�̨ݕϊ�
void AffineVtx(int pn, PNTTYPE* pa, PNTTYPE cp, double mat[3][3]);		// ���_���W�̨ݕϊ�
void CalcCent(PNTTYPE*, int, PNTTYPE*);									// �}�`�d�S�擾
void CalcBox(PNTTYPE*, int, BOXTYPE*);									// �}�`BOX���ގ擾
void CalcNVec(PNTTYPE*, int, VECTYPE*);									// �}�`�@���޸�َ擾
void CalcRotAngleVec(PNTTYPE*, double*, VECTYPE*);						// �}�`�@���޸��&�p�x�擾
double CalcRotAngle(PNTTYPE*);											// �}�`�p�x�擾
double CalcAngle(PNTTYPE*, int, int);									// 3�_�̂Ȃ��p�x�擾
double CalcBaseAngle(PNTTYPE, PNTTYPE, int);							// ����p�x�擾
void CalcLinePoint(int, int, double, double, PNTTYPE*, PNTTYPE*);		// ������_�Z�o
double DistPointLine(PNTTYPE*, PNTTYPE*, PNTTYPE*, int*, double*,
                     PNTTYPE*);											// �_<->�����ŒZ�����擾
double BoxCenter(BOXTYPE, PNTTYPE*);									// �ޯ������
BOOL BoxCheck(int, BOXTYPE, PNTTYPE);									// �ޯ�������݊m�F
void GetTensionFltValue(int, double, double, double*);					// ���͐ݒ�l�ϊ�(����)
void GetTensionPntValue(int, double, double, PNTTYPE*);					// ���͐ݒ�l�ϊ�(���_)
BOOL CheckNextPoint(PNTTYPE, PNTTYPE);									// ���_�L������
void ScrToPnt(int, SCRTYPE*, PNTTYPE*);									// SCRTYPE<=>PNTTYPE�ϊ�
double CalcVecAngle(VECTYPE*, VECTYPE*);								// 2�޸�يԊp�x�Z�o

//===============================<EOF:NumFunc.h>=============================