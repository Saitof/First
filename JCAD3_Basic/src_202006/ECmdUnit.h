/////////////////////////////////////////////////////////////////////////////
// ���j�b�g���̕ҏW�R�}���h���v���g�^�C�v�錾
// ECmdUnit.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void InitArrange(int, int, double, PNTTYPE, int*, int*, int*,
                 PNTTYPE*, VECTYPE*, VECTYPE*);							// �Θg�z�u��������
void EndArrange(int);													// �Θg�z�u�I������
void ArrangeMove(double, int, int, int*, int*, PNTTYPE*, PNTTYPE*,
                 VECTYPE*);												// �Θg�ړ�����
void ArrangeRotate1(double, int, int, int*, int*, PNTTYPE*);			// �Θg��]����1
void ArrangeRotate2(double, int, int, int*, int*, PNTTYPE*, VECTYPE*);	// �Θg��]����2
void InitBasePos(int, int, int, int, PNTTYPE*, int*, int*, int*,
                 PNTTYPE*);												// ��ړ���������
void EndBasePos(void);													// ��ړ��I������
void EditBasePos(int, int, int, int, PNTTYPE*, int, PNTTYPE*,
                 int, int*, int*, PNTTYPE*);							// ��ړ��ҏW����
void ObjectLinesEdit(int, int, int, int, PNTTYPE, PNTTYPE);				// �Ȑ��w��ҏW����

//==============================<EOF:ECmdUnit.h>=============================