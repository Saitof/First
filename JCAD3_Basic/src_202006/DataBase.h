/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X����w�b�_
// DataBase.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATABASE_H
#define ___DATABASE_H

#include "DataType.h"

//----- �����N������p -----//
extern int      Demo_Flg;												// �̌����׸�
extern int      Init_Flg;												// �����N������׸�
extern int      File_Flg;												// ̧�ٕۑ��׸�
extern CString  FileName;												// �ۑ�̧�ٖ�
extern int      Spsh_Flg;												// ���ׯ���\���׸�
extern UINT     Msg_No;													// ү����No

//----- �o�b�N�A�b�v����p�ϐ� -----//
extern int      Cmd_Lvl;												// ���������No
extern int      Cmd_Lvl_Sv;												// �ۑ��p���������No
extern int      Obj_No;													// ����No

//----- �f�[�^�̈摀��p�ϐ� -----//
extern int*     Int_ary;												// �������Ұ��z��
extern PNTTYPE* Pnt_ary;												// ���_���Ұ��z��
extern UNDTYPE* Undo_ary;												// Undo/Redo�z��
extern int      Int_adr;												// �������Ұ��z���擪�Ԓn
extern int      Pnt_adr;												// ���_���Ұ��z���擪�Ԓn
extern int      Undo_adr;												// Undo/Redo�z����(���د�)
extern int      Int_max;												// �������Ұ��z��
extern int      Pnt_max;												// ���_���Ұ��z��
extern int      Undo_max;												// Undo/Redo�z��
extern int      CCtl_max;												// �ő�Ȑ�����_��
extern int      SCtl_max;												// �ő�Ȗʐ���_��
extern int      CCtlA_max;												// �ő�Ȑ�����_�z��
extern int      SCtlA_max;												// �ő�Ȗʐ���_�z��
extern int      WCtl_max;												// ��Ɨp����_�z��
extern int      CPnt_max;												// �ő�Ȑ����_��
extern int      SPnt_max;												// �ő�Ȗʒ��_��
extern int      OPnt_max;												// �ő嗧�̒��_��
extern int      BPnt_max;												// �ő�f�ʒ��_��

//----- �R�}���h����p�ϐ� -----//
extern CMDTYPE* Temp_cmp;												// �ꎞ�폜����ސ擪�߲��
extern CMDTYPE* Gc_cmp;													// �ė��p����ސ擪�߲��
extern CMDTYPE* Rf_cmp;													// �����Ώۺ�����߲��
extern CMDTYPE* Tach_cmp;												// �ꎞ�ؗ����p�߲��
extern short    Cmd_flg;												// ����ޕۑ��׸�(0:��,1:��)
extern short    Cmd_upd;												// �ۑ��׸ލX�V(0:�L,1:��)
extern short    Cmd_stp;												// ����ގ��s��~�׸�(0:��,1:�L)
extern CPCTYPE* Copy_cmp;												// ��߰����ސ擪�߲��

//----- �I�u�W�F�N�g����p�ϐ� -----//
extern OBJTYPE* Base_op;												// �L�����̐擪�߲��
extern OBJTYPE* Temp_op;												// �ꎞ�폜���̐擪�߲��
extern OBJTYPE* Gc_op;													// �ė��p���̐擪�߲��
extern OBJTYPE* Rf_op;													// �����Ώۗ����߲��
extern OBJTYPE* Reset_op;												// ؾ�ė����߲��
extern OBJTYPE* Oset_op;												// ��ė����߲��
extern BOXTYPE  Scr_Box;												// ��ذ��ޯ��
extern int      SelectNo;												// ���̑I��No

//----- ���̍\����񑀍�p�ϐ� -----//
extern OIFTYPE* Base_oif;												// �L�����̍\���擪�߲��
extern OIFTYPE* Temp_oif;												// �ꎞ�폜���̍\���擪�߲��
extern OIFTYPE* Gc_oif;													// �ė��p���̍\���擪�߲��

//----- Undo/Redo����p�ϐ� -----//
extern short    Undo_flg;												// �׸�(0:��,1:�L)
extern short    Undo_stp;												// �׸�(0:�ۑ���,1:�ۑ��L)

//----- ���_���\�[�g�p -----//
extern int      Now_no;													// ���_�Ő����ݒ�ԍ�
extern VTYPE*   Vp;														// ��ėp���_�z���߲��
extern NVTYPE*  NVp;													// ��ėp���_�z���߲��

//----- RayTrace�����ϐ� -----//
extern COLORREF BkRGB;                                                  // �w�i�F
extern short    Max_Lvl;                                                // �ő�T������
extern short    Anti;                                                   // �����ر�ݸ�
extern short    Edge;                                                   // �Ő��׸�
extern short    StgFlg;                                                 // �ð�ޗL���׸�
extern double   StgSize[4];                                             // �ð�޻���

//----- RayTrace�������ϐ� -----//
extern LIGHT    Lgt_Tbl[LGMAX];                                         // �������z��

//----- RayTrace�\�ʏ��ϐ� -----//
extern ATTRIBT  Atr_Tbl[ATMAX];                                         // �\�ʑ����z��

//----- POV-Ray�����ϐ� -----//
extern CString  PovInFile;												// POV-Ray��`̧�ٖ�(���߽)
extern CString  PovInPath;												// POV-Ray��`̧���߽
extern CString  PovOutPath;												// POV-Ray�o��̫����߽
extern BOOL     PovSmooth;												// �X���[�W���O�L��
extern double   PovAngle;												// �אږ@���x�N�g���p�x��
extern BOOL     PovTranslate;											// Translate�L��
extern double   PovTranslateX;											// Translate��(X)
extern double   PovTranslateY;											// Translate��(Y)
extern double   PovTranslateZ;											// Translate��(Z)
extern BOOL     PovRotate;												// Rotate�L��
extern double   PovRotateX;												// Rotate��(X)
extern double   PovRotateY;												// Rotate��(Y)
extern double   PovRotateZ;												// Rotate��(Z)
extern BOOL     PovScale;												// Scale�L��
extern double   PovScaleX;												// Scale��(X)
extern double   PovScaleY;												// Scale��(Y)
extern double   PovScaleZ;												// Scale��(Z)
extern BOOL     PovAnima;												// ��Ұ��ݗL��
extern int      PovAnimaZ;												// ��Ұ��ݕ�����(Z)
extern int      PovAnimaY;												// ��Ұ��ݕ�����(Y)
extern CString  PovDefFile1;											// հ�ޒ�`̧�ٖ�1(���߽)
extern CString  PovDefFile2;											// հ�ޒ�`̧�ٖ�2(���߽)
extern CString  PovDefFile1W;											// հ�ޒ�`̧�ٖ�1WK(���߽)
extern CString  PovDefFile2W;											// հ�ޒ�`̧�ٖ�2WK(���߽)
extern CString  PovDefPath;												// հ�ޒ�`̧���߽
extern int      PovTarget;												// photons target �L/��
extern int      PovCollect;												// photons collect on/off
extern int      PovReflection;											// photons reflection on/off
extern int      PovRefraction;											// photons refraction on/off
extern int      PovShape[POVCMAX];										// ��޼ު�Č`��
extern int      PovTex1[POVCMAX];										// ø���No1
extern int      PovTex2[POVCMAX];										// ø���No2
extern int      PovTex3[POVCMAX];										// ø���No3
extern int      PovTex4[POVCMAX];										// ø���No4
extern int      PovTex5[POVCMAX];										// ø���No5

//----- ���̥����ލ�Ɨp�ϐ� -----//
extern OWKTYPE* Base_owk;												// ���̍���ް��擪�߲��
extern OWKTYPE* Rf_owk;													// �����Ώۗ��̍���ް��߲��

//----- �Ȗʍו�����Ɨp�ϐ� -----//
extern short    Curve_Rept_SW;											// �Ȗʍו���SW(0:��,1:�L)
extern short    Curve_Rept_Num;											// �Ȗʍו�����

#endif ___DATABASE_H

//==============================<EOF:DataBase.h>=============================