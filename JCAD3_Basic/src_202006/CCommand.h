/////////////////////////////////////////////////////////////////////////////
// ���̐����R�}���h���v���g�^�C�v�錾
// CCommand.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
int  MakeSCurve(int*, int, int, double, PNTTYPE*, int*);				// S�Ȑ�����
void CrtCurve(int, int, int, double, PNTTYPE*, int*);					// �Ȑ�����
void RedoCrtCurve(int, int, PNTTYPE*, int*);							// �Ȑ�����(�Ď��s)
void ChgCurve(OBJTYPE*, int, int, int, double, PNTTYPE*, int*);			// �Ȑ��ύX
void RedoChgCurve(int, int, PNTTYPE*, int*);							// �Ȑ��ύX(�Ď��s)
void SetCurve(int, int, PNTTYPE*);										// �Ȑ�����_����
void RedoSetCurve(int, int, PNTTYPE*);									// �Ȑ�����_����(�Ď��s)
void CrtSurface(int, int, int, int, int, double, int, PNTTYPE*, int*);	// �Ȗʐ���
void RedoCrtSurface(int, int, int, int, PNTTYPE*, int*);				// �Ȗʐ���(�Ď��s)
void RedoCrtSurface2(int, int, int, int, int, PNTTYPE*, int*);			// �Ȗʐ���2(�Ď��s)
void ChgSurface(OBJTYPE*, int, int, int, int, int, double, PNTTYPE*,
                int*);													// �ȖʕύX
void RedoChgSurface(int, int, int, int, PNTTYPE*, int*);				// �ȖʕύX(�Ď��s)
void SetSurface(int, int, int, int, PNTTYPE*);							// �Ȗʐ���_����
void RedoSetSurface(int, int, int, int, PNTTYPE*);						// �Ȗʐ���_����(�Ď��s)
void Thickness(OBJTYPE*, double);										// �Ȗʌ��݂Â�
void MakeDirectCommand(OBJTYPE*);										// ���ڐ�������ލ쐬
void RedoDirect(int, PNTTYPE*);											// ���ڐ���(�Ď��s)
void CrtSweepSurf(int, int, int, int, int, double, int, int, PNTTYPE*,
                  int*);												// �|���Ȗʐ���
void RedoCrtSweepSurf(int, int, int, int, PNTTYPE*, int*);				// �|���Ȗʐ���(�Ď��s)
void ChgSweepSurf(OBJTYPE*, int, int, int, int, int, double, PNTTYPE*,
                  int*);												// �|���ȖʕύX
void RedoChgSweepSurf(int, int, int, int, PNTTYPE*, int*);				// �|���ȖʕύX(�Ď��s)

//==============================<EOF:CCommand.h>=============================