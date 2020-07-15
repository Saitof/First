/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X
// DataBase.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/12/22 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DataBase.h"

//----- �����N������p -----//
int      Demo_Flg;														// �̌����׸�
int      Init_Flg;														// �����N������׸�
int      File_Flg;														// ̧�ٕۑ��׸�
CString  FileName;														// �ۑ�̧�ٖ�
int      Spsh_Flg;														// ���ׯ���\���׸�
UINT     Msg_No;														// ү����No

//----- �o�b�N�A�b�v����p�ϐ� -----//
int      Cmd_Lvl;														// ���������No
int      Cmd_Lvl_Sv;													// �ۑ��p���������No
int      Obj_No;														// ����No

//----- �f�[�^�̈摀��p�ϐ� -----//
int*     Int_ary;														// �������Ұ��z��
PNTTYPE* Pnt_ary;														// ���_���Ұ��z��
UNDTYPE* Undo_ary;														// Undo/Redo�z��
int      Int_adr;														// �������Ұ��z���擪�Ԓn
int      Pnt_adr;														// ���_���Ұ��z���擪�Ԓn
int      Undo_adr;														// Undo/Redo�z����(���د�)
int      Int_max;														// �������Ұ��z��
int      Pnt_max;														// ���_���Ұ��z��
int      Undo_max;														// Undo/Redo�z��
int      CCtl_max;														// �ő�Ȑ�����_��
int      SCtl_max;														// �ő�Ȗʐ���_��
int      CCtlA_max;														// �ő�Ȑ�����_�z��
int      SCtlA_max;														// �ő�Ȗʐ���_�z��
int      WCtl_max;														// ��Ɨp����_�z��
int      CPnt_max;														// �ő�Ȑ����_��
int      SPnt_max;														// �ő�Ȗʒ��_��
int      OPnt_max;														// �ő嗧�̒��_��
int      BPnt_max;														// �ő�f�ʒ��_��

//----- �R�}���h����p�ϐ� -----//
CMDTYPE* Temp_cmp;														// �ꎞ�폜����ސ擪�߲��
CMDTYPE* Gc_cmp;														// �ė��p����ސ擪�߲��
CMDTYPE* Rf_cmp;														// �����Ώۺ�����߲��
CMDTYPE* Tach_cmp;														// �ꎞ�ؗ����p�߲��
short    Cmd_flg;														// ����ޕۑ��׸�(0:��,1:��)
short    Cmd_upd;														// �ۑ��׸ލX�V(0:�L,1:��)
short    Cmd_stp;														// ����ގ��s��~�׸�(0:��,1:�L)
CPCTYPE* Copy_cmp;														// ��߰����ސ擪�߲��

//----- �I�u�W�F�N�g����p�ϐ� -----//
OBJTYPE* Base_op;														// �L�����̐擪�߲��
OBJTYPE* Temp_op;														// �ꎞ�폜���̐擪�߲��
OBJTYPE* Gc_op;															// �ė��p���̐擪�߲��
OBJTYPE* Rf_op;															// �����Ώۗ����߲��
OBJTYPE* Reset_op;														// ؾ�ė����߲��
OBJTYPE* Oset_op;														// ��ė����߲��
BOXTYPE  Scr_Box;														// ��ذ��ޯ��
int      SelectNo;														// ���̑I��No

//----- ���̍\����񑀍�p�ϐ� -----//
OIFTYPE* Base_oif;														// �L�����̍\���擪�߲��
OIFTYPE* Temp_oif;														// �ꎞ�폜���̍\���擪�߲��
OIFTYPE* Gc_oif;														// �ė��p���̍\���擪�߲��

//----- Undo/Redo����p�ϐ� -----//
short    Undo_flg;														// �׸�(0:��,1:�L)
short    Undo_stp;														// �׸�(0:�ۑ���,1:�ۑ��L)

//----- ���_���\�[�g�p -----//
int      Now_no;														// ���_�Ő����ݒ�ԍ�
VTYPE*   Vp;															// ��ėp���_�z���߲��
NVTYPE*  NVp;															// ��ėp���_�z���߲��

//----- RayTrace�����ϐ� -----//
COLORREF BkRGB;                                                         // �w�i�F
short    Max_Lvl;                                                       // �ő�T������
short    Anti;                                                          // �����ر�ݸ�
short    Edge;                                                          // �Ő��׸�
short    StgFlg;                                                        // �ð�ޗL���׸�
double   StgSize[4];                                                    // �ð�޻���

//----- RayTrace�������ϐ� -----//
LIGHT    Lgt_Tbl[LGMAX];                                                // �������z��

//----- RayTrace�\�ʏ��ϐ� -----//
ATTRIBT  Atr_Tbl[ATMAX];                                                // �\�ʑ����z��

//----- POV-Ray�����ϐ� -----//
CString  PovInFile;														// POV-Ray��`̧�ٖ�(���߽)
CString  PovInPath;														// POV-Ray��`̧���߽
CString  PovOutPath;													// POV-Ray�o��̫����߽
BOOL     PovSmooth;														// �X���[�W���O�L��
double   PovAngle;														// �אږ@���x�N�g���p�x��
BOOL     PovTranslate;													// Translate�L��
double   PovTranslateX;													// Translate��(X)
double   PovTranslateY;													// Translate��(Y)
double   PovTranslateZ;													// Translate��(Z)
BOOL     PovRotate;														// Rotate�L��
double   PovRotateX;													// Rotate��(X)
double   PovRotateY;													// Rotate��(Y)
double   PovRotateZ;													// Rotate��(Z)
BOOL     PovScale;														// Scale�L��
double   PovScaleX;														// Scale��(X)
double   PovScaleY;														// Scale��(Y)
double   PovScaleZ;														// Scale��(Z)
BOOL     PovAnima;														// ��Ұ��ݗL��
int      PovAnimaZ;														// ��Ұ��ݕ�����(Z)
int      PovAnimaY;														// ��Ұ��ݕ�����(Y)
CString  PovDefFile1;													// հ�ޒ�`̧�ٖ�1(���߽)
CString  PovDefFile2;													// հ�ޒ�`̧�ٖ�2(���߽)
CString  PovDefFile1W;													// հ�ޒ�`̧�ٖ�1WK(���߽)
CString  PovDefFile2W;													// հ�ޒ�`̧�ٖ�2WK(���߽)
CString  PovDefPath;													// հ�ޒ�`̧���߽
int      PovTarget;														// photons target �L/��
int      PovCollect;													// photons collect on/off
int      PovReflection;													// photons reflection on/off
int      PovRefraction;													// photons refraction on/off
int      PovShape[POVCMAX];												// ��޼ު�Č`��
int      PovTex1[POVCMAX];												// ø���No1
int      PovTex2[POVCMAX];												// ø���No2
int      PovTex3[POVCMAX];												// ø���No3
int      PovTex4[POVCMAX];												// ø���No4
int      PovTex5[POVCMAX];												// ø���No5

//----- ���̥����ލ�Ɨp�ϐ� -----//
OWKTYPE* Base_owk;														// ���̍���ް��擪�߲��
OWKTYPE* Rf_owk;														// �����Ώۗ��̍���ް��߲��

//----- �Ȗʍו�����Ɨp�ϐ� -----//
short    Curve_Rept_SW;													// �Ȗʍו���SW(0:��,1:�L)
short    Curve_Rept_Num;												// �Ȗʍו�����

//=============================<EOF:DataBase.cpp>============================