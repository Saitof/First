/////////////////////////////////////////////////////////////////////////////
// ���̑����ݒ�R�}���h���v���g�^�C�v�錾
// Attribute.h
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void WriteAttributeFile(FILE*, OBJTYPE*);								// ����No̧�ٕۑ�
void WriteAttributeFileJgd(FILE*, OBJTYPE*, int);						// ����No̧�ٕۑ�(JGD�p)
void WriteColorFile(FILE*, OBJTYPE*);									// �װNo̧�ٕۑ�
void WriteColorFileJgd(FILE*, OBJTYPE*, int);							// �װNo̧�ٕۑ�(JGD�p)
void WriteGroupFile(FILE*, OBJTYPE*);									// ��ٰ�ߏ��̧�ٕۑ�
void WriteGroupFileJgd(FILE*, OBJTYPE*, int);							// ��ٰ�ߏ��̧�ٕۑ�(JGD�p)
void WritePartsFile(FILE*, OBJTYPE*);									// �߰�No̧�ٕۑ�
void WritePartsFileJgd(FILE*, OBJTYPE*, int);							// �߰�No̧�ٕۑ�(JGD�p)

//=============================<EOF:Attribute.h>=============================