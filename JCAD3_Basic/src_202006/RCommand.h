/////////////////////////////////////////////////////////////////////////////
// �R�}���h�֘A���v���g�^�C�v�錾
// RCommand.h
//---------------------------------------------------------------------------
// LastEdit : 2009/06/25 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
OBJTYPE* CopyObject(void);												// ����&����ޕ���
void Copy(void);														// ���̕���
void Delete(void);														// ���̍폜
void InitPasteObject(int*);												// �����߰�ď�������
void EndPasteObject(int);												// �����߰�ďI������
void EditPasteObject1(int, PNTTYPE, double, PNTTYPE, PNTTYPE);			// �����߰�Ĕz�u(�ړ����])
void EditPasteObject2(int, double);										// �����߰�Z����]
void PCutObject(PNTTYPE, PNTTYPE, int);									// ���̕��ʕ���
void BooleanObject(int, int);											// ���̐}�`���Z

//=============================<EOF:RCommand.h>==============================
