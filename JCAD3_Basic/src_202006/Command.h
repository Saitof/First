/////////////////////////////////////////////////////////////////////////////
// �R�}���h�N�����䕔�v���g�^�C�v�錾
// Command.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void     ExecCommand(BOOL);												// ����ގ��s
OBJTYPE* MakeSurface(void);												// �Ȗʐ���
OBJTYPE* MakeDirect(void);												// ���ڐ���
OBJTYPE* MakeCurve(void);												// �Ȑ�����
OBJTYPE* ChangeSurface(void);											// �ȖʕҏW
OBJTYPE* ChangeCurve(void);												// �Ȑ��ҏW
OBJTYPE* SetSurface(void);												// �Ȗʐ���_����
OBJTYPE* SetCurve(void);												// �Ȑ�����_����
OBJTYPE* ThicknessObject(void);											// �Ȗʌ��݂Â�
OBJTYPE* MoveObject(void);												// ���s�ړ�
OBJTYPE* RotateObject(void);											// ��]��]
OBJTYPE* MirrorObject(void);											// �����ړ�
OBJTYPE* ScaleObject(void);												// ���ޕύX
OBJTYPE* BendObject(void);												// �~���Ȃ�
OBJTYPE* SetGroup(void);												// ��ٰ�ߏ��ݒ�
OBJTYPE* SetColor(void);												// �װNo�ݒ�
OBJTYPE* SetAttribute(void);											// ����No�ݒ�
OBJTYPE* MakeSweepSurf(void);											// �|���Ȗʐ���
OBJTYPE* ChangeSweepSurf(void);											// �|���ȖʕҏW
OBJTYPE* TwistObject(void);												// �Ђ˂�
OBJTYPE* ResizeObject1(void);											// �����w��ػ���1
OBJTYPE* SetParts(void);												// �߰�No�ݒ�
OBJTYPE* ResizeObject2(void);											// �Ȑ��w��ػ���1
OBJTYPE* ShearObject(void);												// ����f
OBJTYPE* RingObject(void);												// �ݸ�
OBJTYPE* EarthObject(void);												// ���
OBJTYPE* SpaceObject(void);												// ��߰�
OBJTYPE* ResizeObject3(void);											// �����w��ػ���2
OBJTYPE* ResizeObject4(void);											// �Ȑ��w��ػ���2
OBJTYPE* CommandError(void);											// ����޴װ
void     RedoCommand(OBJTYPE*);											// ����ލĎ��s
void     RedoStartCommand(OBJTYPE*);									// �ƭ��J�n������ލĎ��s
void     RedoCreateCommand(OBJTYPE*);									// ��������ލĎ��s
void     RedoRingBeforeCommand(OBJTYPE* op);							// �ݸ޻��ޑO����ލĎ��s
void     RedoCommandCopy(OBJTYPE*, CMDTYPE*);							// ����ޕ��ʎ��s

//==============================<EOF:Command.h>==============================