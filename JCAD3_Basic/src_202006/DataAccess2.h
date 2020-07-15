/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X���암�v���g�^�C�v�錾
// DataAccess2.h
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATAACCESS2_H
#define ___DATAACCESS2_H

#include "DataType.h"

void CreatOif(int, OBJTYPE*, char*, OIFTYPE*, OIFTYPE**);				// ���̍\�����̈�m��
void FreeOif(int, OIFTYPE*);											// ���̍\�����̈���
void SetOifObj(OIFTYPE*, OBJTYPE*);										// �\����񗧑��߲���ݒ�
void GetOifObj(OIFTYPE*, OBJTYPE**);									// �\����񗧑��߲���擾
void GetObjOif(OBJTYPE*, OIFTYPE**);									// ���̍\������߲���擾
void BaseOifPtr(int, int, OIFTYPE**);									// ���̏���ް��߲���擾
void NextOifPtr(int, int, OIFTYPE**);									// �����̏���߲���ړ�
void SetOifName(int, OIFTYPE*, char*);									// ���̍\�����̐ݒ�
void GetOifName(OIFTYPE*, char*);										// ���̍\�����̎擾
void SetOifLevel(OIFTYPE*, int);										// ���̍\�����ِݒ�
int  GetOifLevel(OIFTYPE*);												// ���̍\�����َ擾
void InitOifNo(void);													// ���̍\������No������
void SetOifNo(OIFTYPE*, int);											// ���̍\������No�ݒ�
int  GetOifNo(OIFTYPE*);												// ���̍\������No�擾
void SortOifData(void);													// ���̍\���ް����
void GetDataBasePtr(OBJTYPE**, PNTTYPE**, int**);						// �ް��ް��擪�߲���擾
void OnResetObj(OBJTYPE*);												// ؾ�ė��̓o�^
void OffResetObj(void);													// ؾ�ė��̉���
BOOL GetResetObjMode(void);												// ؾ�ė���Ӱ�ގ擾
void CreatObj(int, int, int, OBJTYPE**);								// �����ް��̈�m��
void ChangeObj(OBJTYPE*, int, int, int);								// �����ް��̈�ύX
void TempObj(OBJTYPE*);													// �����ް��ꎞ�폜
void FreeObj(OBJTYPE*, int);											// �����ް��̈���
void UndoObj(OBJTYPE*);													// ���̕���
void LevelObj(int, OBJTYPE*);											// �������ٗ��̐ڑ�
void BaseObjPtr(int, OBJTYPE**, int*, int*);							// �����ް��߲���擾
BOOL CheckObjPtr(int, OBJTYPE*);										// �w�藧�̑�������
void NextObjPtr(int, OBJTYPE**, int*, int*);							// �������߲���ړ�
void GroupBaseObjPtr(int, OBJTYPE**);									// �����ް��߲���擾(group)
void GroupNextObjPtr(int, OBJTYPE**);									// �������߲���ړ�(group)
void LevelBaseObjPtr(int, OBJTYPE**);									// �������ِ擪���̎擾
void LevelNextObjPtr(int, OBJTYPE**);									// �������َ����̈ړ�
void SelBaseObjPtr(int, OBJTYPE**);										// �����ް��߲���擾
void SelNextObjPtr(int, OBJTYPE**);										// �������߲���ړ�
void ColorBaseObjPtr(int, OBJTYPE**);									// �װ�ʗ����ް��߲���擾
void ColorNextObjPtr(int, OBJTYPE**);									// �װ�ʎ������߲���ړ�
void SetObjPtr(OBJTYPE*);												// �����Ώۗ��̐ݒ�
void GetObjPtr(OBJTYPE**);												// �����Ώۗ��̎擾
void OnDspFlg(int, OBJTYPE*);											// �\��(�L��)�׸�ON
void OffDspFlg(int, OBJTYPE*);											// �\��(�L��)�׸�OFF
void OnEdtFlg(int, OBJTYPE*);											// �ҏW�Ώ��׸�ON
void OffEdtFlg(int, OBJTYPE*);											// �ҏW�Ώ��׸�OFF
void SetObjNo(OBJTYPE*, int);											// ����No�ݒ�
int  GetObjNo(OBJTYPE*);												// ����No�擾
void ChangeTestFlg(int);												// ýėp�׸ޕύX
void PutObjAtr(OBJTYPE*, int);											// ����No�ݒ�
int  GetObjAtr(OBJTYPE*);												// ����No�擾
void PutObjPrt(OBJTYPE*, int, int);										// �߰�No�ݒ�
int  GetObjPrt(OBJTYPE*);												// �߰�No�擾
void PutObjCol(OBJTYPE*, int);											// �װNo�ݒ�
int  GetObjCol(OBJTYPE*);												// �װNo�擾
void PutListNo(OBJTYPE*, unsigned int);									// �ި���ڲ�ؽ�No�ݒ�
unsigned int GetListNo(OBJTYPE*);										// �ި���ڲ�ؽ�No�擾
void PutListMode(OBJTYPE*, int);										// ؽ�Ӱ�ސݒ�
int  GetListMode(OBJTYPE*);												// ؽ�Ӱ�ގ擾
void PutGroup(OBJTYPE*, int*);											// ��ٰ�ߏ��ݒ�
void GetGroup(OBJTYPE*, int*);											// ��ٰ�ߏ��擾
void SetGroupNo(OBJTYPE*, int, int);									// ��ٰ�ߔԍ��ݒ�
void SetSelNo(OBJTYPE*, int);											// �I��No�ݒ�
int  GetSelNo(OBJTYPE*);												// �I��No�擾
int  GetSelectNo(void);													// ���̑I��No�擾
void ResetSelNo(void);													// �I��Noؾ��
int  GetVtxNum(OBJTYPE*);												// ���̒��_���擾
void PutVtx(OBJTYPE*, int, PNTTYPE);									// �w�蒸�_�ݒ�
void GetVtx(OBJTYPE*, int, PNTTYPE*);									// �w�蒸�_�擾
int  GetFlpNum1(OBJTYPE*);												// ���̖�ٰ�ߐ��擾1
void PutFlp1(OBJTYPE*, int, int, int);									// �w���ٰ�ߔԍ��ݒ�1
void GetFlp1(OBJTYPE*, int, int*, int*);								// �w���ٰ�ߔԍ��擾1
void GetFlpVtx1(OBJTYPE*, int, PNTTYPE*, int*);							// �w���ٰ�ߍ��W�擾1
int  GetFlpNum2(OBJTYPE*);												// ���̖�ٰ�ߐ��擾2
void PutFlp2(OBJTYPE*, int, int, int);									// �w���ٰ�ߔԍ��ݒ�2
void GetFlp2(OBJTYPE*, int, int*, int*);								// �w���ٰ�ߔԍ��擾2
void GetFlpVtx2(OBJTYPE*, int, PNTTYPE*, int*);							// �w���ٰ�ߍ��W�擾2
void SetObjBox(OBJTYPE*);												// �����ޯ���ݒ�
void GetObjBox(OBJTYPE*, BOXTYPE*);										// �����ޯ���擾
void SetScrBox(void);													// ��ذ��ޯ���ݒ�
void GetScrBox(BOXTYPE*);												// ��ذ��ޯ���擾
void GetAreaBox(int, BOXTYPE*);											// �ر�ޯ���擾
void GetUnitBox(OBJTYPE*, BOXTYPE*);									// �Ưė����ޯ���擾
void ChangeObjVtx(int, OBJTYPE*, double, double, double);				// ���̒��_�ύX
void InitCVtx(void);													// ���_��ď�������
void SetCVtx(PNTTYPE);													// ��Ē��_�ݒ�
void SetCFace(void);													// ��ٰ�ߏI���ݒ�
int  SortCVtx(void);													// ���_����&����
void SetCVtxObj(OBJTYPE*);												// ���_���ݒ�

#endif ___DATAACCESS2_H

//============================<EOF:DataAccess2.h>============================