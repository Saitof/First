/////////////////////////////////////////////////////////////////////////////
// �Ώ̎��ҏW���암�v���g�^�C�v�錾
// Symmetry.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL InputRevolveDivi(int*);											// ��]�Ώ̕������擾
void SetSymmetryLine(int, int, PNTTYPE, int, int, int, int*, int*);		// �Ώ̎��ݒ�
BOOL CheckSymmetryArea(int*, PNTTYPE, int, PNTTYPE, int, int, int);		// �Ώ̗̈�����
int  CopySymmetryArea(int, PNTTYPE, int, int, int, int*, int*,
                      PNTTYPE*, int*, int*, int*);						// �Ώ̗̈敡��
void PickSymmetryArea(int, int, int, int, int, int, int*, int*);		// �Ώ̗̈��ׯ���I��
void MoveSymmetryArea(PNTTYPE, PNTTYPE, int, PNTTYPE, int, int, int, int,
                      int, int, int, int, int*, PNTTYPE*, PNTTYPE*);	// �Ώ̗̈�ړ��ʐݒ�

//==============================<EOF:Symmetry.h>=============================