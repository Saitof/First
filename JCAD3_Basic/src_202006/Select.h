/////////////////////////////////////////////////////////////////////////////
// ���̑I�𕔃v���g�^�C�v�錾
// Select.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void OnDspAllObject(void);												// ���̕\��(�S����)
void OffDspAllObject(void);												// ���̔�\��(�S����)
void OnDspGroupObject(int);												// ���̕\��(��ٰ��)
void OffDspGroupObject(int);											// ���̔�\��(��ٰ��)
void EdtPointObject(int, PNTTYPE, int);									// �ҏW���̑I��(�߲��)
void EdtAreaObject(int, BOXTYPE, int);									// �ҏW���̑I��(�ر)
void OnEdtGroupObject(int);												// �ҏW���̑I��(��ٰ��)
void OnEdtAllObject(void);												// �ҏW���̑S�I��(�\������)
void OffEdtAllObject(void);												// �ҏW���̑S����
void OffEdtOtherObject(OBJTYPE*);										// �w��O�ҏW�Ώۉ���

//==============================<EOF:Select.h>===============================
