/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X���암�v���g�^�C�v�錾
// DataAccess1.h
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATAACCESS1_H
#define ___DATAACCESS1_H

#include "DataType.h"

void ReadMemDataFile(void);												// ��ؗ̈��ް��ۑ�̧�ٓǍ�
void WriteMemDataFile(void);											// ��ؗ̈��ް��ۑ�̧�ُ���
int  GetIntMax(void);													// �������Ұ��z�񐔎擾
void SetIntMax(int);													// �������Ұ��z�񐔐ݒ�
int  GetPntMax(void);													// ���W���Ұ��z�񐔎擾
void SetPntMax(int);													// ���W���Ұ��z�񐔐ݒ�
int  GetUndoMax(void);													// Undo/Redo���Ұ��z�񐔎擾
void SetUndoMax(int);													// Undo/Redo���Ұ��z�񐔐ݒ�
int  GetCCtlMax(void);													// �ő�Ȑ�����_���擾
void SetCCtlMax(int);													// �ő�Ȑ�����_���ݒ�
int  GetCCtlAMax(void);													// �Ȑ�����_�z�񐔎擾
int  GetSCtlMax(void);													// �ő�Ȗʐ���_���擾
void SetSCtlMax(int);													// �ő�Ȗʐ���_���ݒ�
int  GetSCtlAMax(void);													// �Ȗʐ���_�z�񐔎擾
int  GetWCtlMax(void);													// ��Ɨp����_�z�񐔎擾
void SetWCtlMax(int);													// ��Ɨp����_�z�񐔐ݒ�
int  GetCPntMax(void);													// �ő�Ȑ����_���擾
void SetCPntMax(int);													// �ő�Ȑ����_���ݒ�
int  GetSPntMax(void);													// �ő�Ȗʒ��_���擾
void SetSPntMax(int);													// �ő�Ȗʒ��_���ݒ�
int  GetOPntMax(void);													// �ő嗧�̒��_���擾
void SetOPntMax(int);													// �ő嗧�̒��_���ݒ�
int  GetBPntMax(void);													// �ő�f�ʒ��_���擾
void SetBPntMax(int);													// �ő�f�ʒ��_���ݒ�
void InitVar(void);														// �ϐ�������(���ыN����)
void ResetVar(void);													// �ϐ�������(���щғ���)
void FreeMem(void);														// ��؉��(���яI����)
void ChangeAreaAdr(int, int, int, int);									// �z����ڽ�ύX
void SetDemoFlg(int);													// �̌����׸ސݒ�
int  GetDemoFlg(void);													// �̌����׸ގ擾
void SetInitFlg(int);													// �����N������׸ސݒ�
int  GetInitFlg(void);													// �����N������׸ގ擾
void SetFileFlg(int);													// ̧�ٕۑ��׸ސݒ�
int  GetFileFlg(void);													// ̧�ٕۑ��׸ގ擾
void SetFileName(CString);												// �ۑ�̧�ٖ��ݒ�
CString GetFileName(void);												// �ۑ�̧�ٖ��擾
void SetSplashMsg(UINT);												// ���ׯ���\����ү���ސݒ�
void OutSplashMsg(void);												// ���ׯ���\����ү���ޏo��
void StartBackUp(int);													// �ޯ����ߊJ�n����
void StartMenu(void);													// �ƭ��J�n����
void EndJMenu1(void);													// �ƭ��I������1
void EndJMenu2(void);													// �ƭ��I������2
void FileBackUp1(void);													// ̧���ޯ����ߏ���1
void FileBackUp2(void);													// ̧���ޯ����ߏ���2
void UndoMenu(int);														// Undo�ƭ�����
void RedoMenu(int);														// Redo�ƭ�����
BOOL RestoreBackUp(void);												// �ƭ���������
void MemErr(unsigned int);												// ��؈ُ�ү����
double GetIntUse(void);													// �������Ұ��g�p���擾
double GetPntUse(void);													// ���_���Ұ��g�p���擾
int  GetIntUseNum(void);												// �������Ұ��g�p���擾
int  GetPntUseNum(void);												// ���_���Ұ��g�p���擾
void CreatCmd(int, int, int, CMDTYPE**);								// ����ޗ̈�m��
void ChangeCmd(CMDTYPE*, int, int);										// ����ޗ̈�ύX
void TempCmd(OBJTYPE*, CMDTYPE*);										// ����ވꎞ�폜
void FreeCmd(OBJTYPE*, CMDTYPE*, int);									// ����ޗ̈���
void AppendCmd(OBJTYPE*, CMDTYPE*);										// �����&���̐ڑ�
void UndoCmd(OBJTYPE*, CMDTYPE*);										// �����&���̍Đڑ�
void LevelCmd(CMDTYPE*);												// �������ٺ���ސڑ�
void DetachCmd(OBJTYPE*, CMDTYPE*);										// ����ވꎞ�ؗ���
void AttachCmd(OBJTYPE*);												// ����ލĐڑ�
void InitCmdLvl(CMDTYPE*);												// ��������ُ�����
void SetCmdLvl(CMDTYPE*, int);											// ��������ِݒ�
int  GetCmdLvl(CMDTYPE*);												// ��������َ擾
void BaseCmdPtr(int, OBJTYPE*, CMDTYPE**);								// �擪������߲���擾
void NextCmdPtr(CMDTYPE**);												// ��������߲���擾
void NextAllCmdPtr(CMDTYPE**);											// ��������߲��(�S����ޑΏ�)�擾
void LevelBaseCmdPtr(CMDTYPE**);										// �������ٓ��擪����ގ擾
void LevelNextCmdPtr(CMDTYPE**);										// �������ٓ�������ގ擾
void EditCmdPtr(int, OBJTYPE*, CMDTYPE**);								// �ҏW������߲���擾
void SetEditCmdPtr(int, OBJTYPE*, CMDTYPE*);							// �ҏW������߲���ݒ�
void ResetEditCmdPtr(int, OBJTYPE*);									// �ҏW������߲������
void SetCmdPtr(CMDTYPE*);												// �����Ώۺ���ސݒ�
void GetCmdPtr(CMDTYPE**);												// �����Ώۺ���ގ擾
void GetCmdObj(CMDTYPE*, OBJTYPE**);									// ����ޑΏۗ��̎擾
int  GetCmdNo(CMDTYPE*);												// �����No�擾
int  GetObjCmdNo(int flg, OBJTYPE* op);									// ���̺����No�擾
int  GetIntNum(CMDTYPE*);												// �������Ұ����擾
void PutParaInt(CMDTYPE*, int, int);									// �������Ұ��l�ݒ�
int  GetParaInt(CMDTYPE*, int);											// �������Ұ��l�擾
int  GetPntNum(CMDTYPE*);												// ���W���Ұ����擾
void PutParaPnt(CMDTYPE*, int, PNTTYPE);								// ���W���Ұ��l�ݒ�
void GetParaPnt(CMDTYPE*, int, PNTTYPE*);								// ���W���Ұ��l�擾
void OnCmdFlgStp(void);													// �ۑ��׸ލX�V���~ON
void OffCmdFlgStp(void);												// �ۑ��׸ލX�V���~OFF
void OnCmdFlg(void);													// ����ޖ��ۑ��׸�ON
void OffCmdFlg(void);													// ����ޖ��ۑ��׸�OFF
BOOL CheckCmdFlg(void);													// ����ޖ��ۑ��׸ފm�F
void OnCmdExeStp(void);													// ����ގ��s��~�׸�ON
void OffCmdExeStp(void);												// ����ގ��s��~�׸�OFF
int  GetCmdExeStp(void);												// ����ގ��s��~�׸ގ擾
void CreatCopyCmd(int, int, int, CPCTYPE**);							// ��߰����ޗ̈�m��
void FreeCopyCmd(void);													// ��߰����ޗ̈���
void BaseCopyCmdPtr(CPCTYPE**);											// �擪��߰������߲���擾
void NextCopyCmdPtr(CPCTYPE**);											// ����߰������߲���擾
int  GetCopyCmdNo(CPCTYPE*);											// ��߰�����No�擾
int  GetCopyIntNum(CPCTYPE*);											// ��߰����ސ������Ұ����擾
void PutCopyParaInt(CPCTYPE*, int, int);								// ��߰����ސ������Ұ��l�ݒ�
void GetCopyParaInt(CPCTYPE*, int, int*);								// ��߰����ސ������Ұ��l�擾
int  GetCopyPntNum(CPCTYPE*);											// ��߰����ލ��W���Ұ����擾
void PutCopyParaPnt(CPCTYPE*, int, PNTTYPE);							// ��߰����ލ��W���Ұ��l�ݒ�
void GetCopyParaPnt(CPCTYPE*, int, PNTTYPE*);							// ��߰����ލ��W���Ұ��l�擾
void OnUndoRedo(void);													// Undo�׸�ON
void OffUndoRedo(void);													// Undo�׸�OFF
BOOL CheckUndoRedo(void);												// Undo�׸ފm�F
void OnUndoStop(void);													// Undo�ۑ���~�׸�ON
void OffUndoStop(void);													// Undo�ۑ���~�׸�OFF
void InitLevel(void);													// Undo/Redo������
void BackLevel(void);													// Undo/Redo�߂�
BOOL CheckUndo(void);													// Undo����
void Undo(int*);														// Undo����
BOOL CheckRedo(void);													// Redo����
void Redo(int*);														// Redo����

#endif ___DATAACCESS1_H

//============================<EOF:DataAccess1.h>============================