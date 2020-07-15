/////////////////////////////////////////////////////////////////////////////
// �����f�[�^�x�[�X�\���̋y�у}�N����`
// DataType.h
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATATYPE_H
#define ___DATATYPE_H

#define EPSILON   0.00001												// ���e�덷
#define DEPSILON  0.0000001												// ���e�덷
#define PI        3.14159265358979323846264338							// �~����
#define DIM       3														// ������
#define TRUE      1														// ����I��
#define FALSE     0														// �ُ�I��
#define ERROR1    -1													// �ُ�I��1
#define ERROR2    -2													// �ُ�I��2
#define ON        1														// ON
#define OFF       0														// OFF
#define MAX_LEN   (50+1)												// �ő�̧�ٖ�
#define CRTSURF   0														// �Ȗʐ��������No
#define DIRECT    1														// ���ڐ��������No
#define CRTCURVE  2														// �Ȑ����������No
#define CHGSURF   3														// �ȖʕύX�����No
#define CHGCURVE  4														// �Ȑ��ύX�����No
#define SETSURF   5														// �Ȗʐݒ�����No
#define SETCURVE  6														// �Ȑ��ݒ�����No
#define THICKNESS 7														// ���ݕt�������No
#define MOVE      8														// ���s�ړ������No
#define ROTATE    9														// ��]�ړ������No
#define MIRROR    10													// �����ړ������No
#define SCALE     11													// ���ޕύX�����No
#define BEND      12													// �~���Ȃ������No
#define GROUP     13													// ��ٰ�ߐݒ�����No
#define COLOR     14													// �װNo�ݒ�����No
#define ATTRIBUTE 15													// ����No�ݒ�����No
#define CRTSWEEP  16													// �|���Ȗʐ��������No
#define CHGSWEEP  17													// �|���ȖʕύX�����No
#define TWIST     18													// �Ђ˂�����No
#define RESIZE1   19													// �����w��ػ���1�����No
#define PARTS     20													// �߰�No�ݒ�����No
#define RESIZE2   21													// �Ȑ��w��ػ���1�����No
#define SHEAR     22													// ����f�����No
#define RING      23													// �ݸ޺����No
#define EARTH     24													// ��������No
#define SPACE     25													// ��߰������No
#define RESIZE3   26													// �����w��ػ���2�����No
#define RESIZE4   27													// �Ȑ��w��ػ���2�����No
#define ENDCMD    15													// ���̍ŏI�����No(����No)
#define MAXCMD    27													// �ŏI�����No
#define DELCMD    50													// �폜�����No(�ޯ����ߗp)
#define UNDOCMD   51													// Undo�����No(�ޯ����ߗp)
#define REDOCMD   52													// Redo�����No(�ޯ����ߗp)
#define GLBDCMD   70													// �擪�����No(�ް��W�p)
#define RPWDCMD   98													// �Ǎ��߽ܰ�޺����No(JGD/GGD�p)
#define WPWDCMD   99													// �����߽ܰ�޺����No(JGD/GGD�p)
#define MAXINTAN  10000000												// �������Ұ��z��
#define MAXPNTAN  4000000												// ���_���Ұ��z��
#define MAXUNDON  100													// Undo/Redo�z��
#define MAXCCTLN  150      //100										// �ő�Ȑ�����_��
#define MAXSCTLN  1500     //1000										// �ő�Ȗʐ���_��
#define MAXCCTLA  152      //102										// �Ȑ�����_�z��
#define MAXSCTLA  1650     //1100										// �Ȗʐ���_�z��
#define MAXWCTLN  3000     //2000										// ��Ɨp����_�z��
#define MAXCPNTN  2500     //1680										// �ő�Ȑ����_��
#define MAXSPNTN  400000   //288000										// �ő�Ȗʒ��_��
#define MAXOPNTN  1000000  //1000000									// �ő嗧�̒��_��
#define MAXBPNTN  30000    //30000										// �ő�f�ʒ��_��
#define LGMAX     8														// ڲ��ڰ��������z��
#define ATMAX     19													// ڲ��ڰ��\�ʑ����z��
#define POVCMAX   18													// POV-Ray�װ��
#define PXLMAX    2000													// 1����ײ݂̍ő��߸�ِ�
#define JSD_LENG  256													// JSD/JSF̧�� ں��ޒ�
#define CMD_LENG  512													// CMḐ�� ں��ޒ�
#define ONM_LENG  24													// OIF���� ں��ޒ�
#define LIC_NAME  "Jcad3_Glb.lic"										// ײ�ݽ̧�ٖ�
#define OFL_NAME  "Offline_Authent_Glb.dat"								// ��ײݔF��̧�ٖ�
#define JCN_NAME  "JColor_New.dat"										// JCAD3�װ�ݒ�̧�ٖ�(�V��)
#define JCO_NAME  "JColor_Old.dat"										// JCAD3�װ�ݒ�̧�ٖ�(����)
#define MAP_NAME  "mapping.bmp"											// ��ϯ��ݸ�̧�ٖ�
#define REN_NAME  "GLRender.dat"										// ����OpenGL�����ݸ�̧�ٖ�
#define COL_NAME  "GLColor.dat"											// ����OpenGL�װ̧�ٖ�
#define TEN_NAME  "RayTEnv.dat"											// ����ڲ��ڰ���̧�ٖ�
#define LGT_NAME  "RayTLight.dat"										// ����ڲ��ڰ�����̧�ٖ�
#define TCL_NAME  "RayTColor.dat"										// ����ڲ��ڰ��װ̧�ٖ�
#define SY1_NAME  "SysEnv1.bak"											// ���ъ��ݒ�ۑ�̧�ٖ�1
#define SY2_NAME  "SysEnv2.bak"											// ���ъ��ݒ�ۑ�̧�ٖ�2
#define SY3_NAME  "SysEnv3.bak"											// ���ъ��ݒ�ۑ�̧�ٖ�3
#define SY4_NAME  "SysEnv4.bak"											// ���ъ��ݒ�ۑ�̧�ٖ�4
#define SY5_NAME  "SysEnv5.bak"											// ���ъ��ݒ�ۑ�̧�ٖ�5
#define RN1_NAME  "RndEnv1.bak"											// �����ݸފ��ݒ�ۑ�̧�ٖ�1
#define RN2_NAME  "RndEnv2.bak"											// �����ݸފ��ݒ�ۑ�̧�ٖ�2
#define RN3_NAME  "RndEnv3.bak"											// �����ݸފ��ݒ�ۑ�̧�ٖ�3
#define RT1_NAME  "RayEnv1.bak"											// ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�1
#define RT2_NAME  "RayEnv2.bak"											// ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�2
#define RT3_NAME  "RayEnv3.bak"											// ڲ�ڰ�ݸފ��ݒ�ۑ�̧�ٖ�3
#define PV1_NAME  "PovEnv1.bak"											// POV-Ray���ݒ�ۑ�̧�ٖ�1
#define PV2_NAME  "PovEnv2.bak"											// POV-Ray���ݒ�ۑ�̧�ٖ�2
#define PV3_NAME  "PovEnv3.bak"											// POV-Ray���ݒ�ۑ�̧�ٖ�3
#define JCS_NAME  "JColor.sav"    										// JCAD3�װ�ݒ�ۑ�̧�ٖ�
#define FPS_NAME  "FilePaths.sav"    									// ̧���߽�ۑ�̧�ٖ�
#define RDS_NAME  "GLRender.sav"										// OpenGL�����ݸޕۑ�̧�ٖ�
#define SIS_NAME  "ScrInfo.sav"											// ��ʐ�����ۑ�̧�ٖ�
#define DTS_NAME  "SystemData.sav"										// �����ް��ۑ�̧�ٖ�
#define MES_NAME  "MemData.sav"											// ��ؗ̈��ް��ۑ�̧�ٖ�
#define RTS_NAME  "RayTrace.sav"										// ڲ��ڰ��ۑ�̧�ٖ�
#define POV_NAME  "PovRay.sav"											// POV-Ray�ۑ�̧�ٖ�
#define POV_DEF1  "Setting_Defo.txt"									// POV-Ray��`̧�ٖ�(�����l)
#define POV_DEF2  "user_01.inc"											// հ�ޒ�`̧�ٖ�1(�����l)
#define POV_DEF3  "user_02.inc"											// հ�ޒ�`̧�ٖ�2(�����l)
#define CFAC_NAME "Cfac32.sav"											// CFAC32�p�ް��ۑ�̧�ٖ�
#define SLCA_NAME "Slca32.sav"											// SLCA32�p�ް��ۑ�̧�ٖ�
#define NCET_NAME "Ncet32.sav"											// NCET32�p�ް��ۑ�̧�ٖ�
#define NCRV_NAME "Ncrv32.sav"											// NCRV32�p�ް��ۑ�̧�ٖ�
#define NCSM_NAME "Ncsm32.sav"											// NCSM32�p�ް��ۑ�̧�ٖ�
#define TMP_NAME  "temp_file.tmp"										// �ꎞ���̧�ٖ�
#define NCT_NAME  "nc_file.tmp"											// NC�\���p���̧�ٖ�
#define CNV_NAME  "_conv_jpg_tempfile_.bmp"								// JPEĢ�ٕϊ����̧�ٖ�
#define PCB_NAME  "PlaneCutBackup_%d.jgd"								// ���ʕ����ޯ�����̧�ٖ�
#define BLB_NAME  "BooleanBackup_%d.jgd"								// �}�`���Z�ޯ�����̧�ٖ�
#define BUP_NAME  "BackUp.bak"											// �ޯ�����̧�ٖ�(1)
#define BOD_NAME  "BackUp.old"											// �ޯ�����̧�ٖ�(2)
#define COP_NAME1 "CopyData.cdt"										// ��߰�ް�̧�ٖ�1
#define COP_NAME2 "CopyData_%c.cdt"										// ��߰�ް�̧�ٖ�2
#define GLT_NAME  "gltest.pri"											// OpenGLý�����è�ޖ�
#define SPH_NAME  "sphere.pri"											// ��������è�ޖ�
#define CUB_NAME  "cube.pri"											// ����������è�ޖ�
#define CYL_NAME  "cylinder.pri"										// �~������è�ޖ�
#define CON_NAME  "cone.pri"											// �~������è�ޖ�
#define CIR_NAME  "circle.pri"											// �~����è�ޖ�
#define SQU_NAME  "square.pri"											// �����`����è�ޖ�
#define PRO_NAME  "prong.pri"											// �Θg�������è�ޖ�
#define BRI_NAME  "brilliant.jwl"										// ���ر�� ��Ė�
#define HEA_NAME  "heartbri.jwl"										// ʰļ�������ر�Ė�
#define SNG_NAME  "single.jwl"											// �ݸ�� ��Ė�
#define EMR_NAME  "emerald.jwl"											// ������ ��Ė�
#define BGE_NAME  "baguette.jwl"										// �޹ޯ� ��Ė�
#define ROS_NAME  "rose.jwl"											// ۰�� ��Ė�
#define CAB_NAME  "cabochon.jwl"										// ��޼�� ��Ė�
#define G1X_NAME  "glb1_xy.glb"											// GLB1 XY����
#define G1Y_NAME  "glb1_yz.glb"											// GLB1 YZ����
#define G1Z_NAME  "glb1_zx.glb"											// GLB1 ZX����
#define G2X_NAME  "glb2_xy.glb"											// GLB2 XY����
#define G2Y_NAME  "glb2_yz.glb"											// GLB2 YZ����
#define G2Z_NAME  "glb2_zx.glb"											// GLB2 ZX����
#define G3X_NAME  "glb3_xy.glb"											// GLB3 XY����
#define G3Y_NAME  "glb3_yz.glb"											// GLB3 YZ����
#define G3Z_NAME  "glb3_zx.glb"											// GLB3 ZX����
#define G4X_NAME  "glb4_xy.glb"											// GLB4 XY����
#define G4Y_NAME  "glb4_yz.glb"											// GLB4 YZ����
#define G4Z_NAME  "glb4_zx.glb"											// GLB4 ZX����
#define OBJ_NAME00 "���̖���                "							// ���̖���
#define OBJ_NAME01 "�Ȑ�(��)                "
#define OBJ_NAME02 "�Ȑ�(�J)                "
#define OBJ_NAME03 "�ʏ�Ȗʑ|����          "
#define OBJ_NAME04 "�ʏ�Ȗʑ|����[�ʑ�]    "
#define OBJ_NAME05 "�|���Ȗʑ|����          "
#define OBJ_NAME06 "�|���Ȗʑ|����[�ʑ�]    "
#define OBJ_NAME07 "�ʏ�Ȗʉ�]��          "
#define OBJ_NAME08 "�ʏ�Ȗʉ�]��[�ʑ�]    "
#define OBJ_NAME09 "�|���Ȗʉ�]��          "
#define OBJ_NAME10 "�|���Ȗʉ�]��[�ʑ�]    "
#define OBJ_NAME11 "�ʏ�Ȗʐ���            "
#define OBJ_NAME12 "�ʏ�Ȗʐ���[�ʑ�]      "
#define OBJ_NAME13 "�~(2D)                  "
#define OBJ_NAME14 "�����`(2D)              "
#define OBJ_NAME15 "����(3D)                "
#define OBJ_NAME16 "������(3D)              "
#define OBJ_NAME17 "�~��(3D)                "
#define OBJ_NAME18 "�~��(3D)                "
#define OBJ_NAME19 "׳��ޥ���ر�ĥ���       "
#define OBJ_NAME20 "����٥���ر�ĥ���       "
#define OBJ_NAME21 "ϰ���ޥ���ر�ĥ���      "
#define OBJ_NAME22 "�߱����ߥ���ر�ĥ���    "
#define OBJ_NAME23 "ʰļ���ߥ���ر�ĥ���    "
#define OBJ_NAME24 "�ݸ�٥���               "
#define OBJ_NAME25 "�����ޥ���              "
#define OBJ_NAME26 "�޹ޯĥ���              "
#define OBJ_NAME27 "ð�߰�ޥ�޹ޯĥ���      "
#define OBJ_NAME28 "۰�ޥ���                "
#define OBJ_NAME29 "��޼�ݥ���              "
#define OBJ_NAME30 "�΍�                    "
#define OBJ_NAME31 "��                      "
#define OBJ_NAME32 "÷��                    "
#define OBJ_NAME33 "���ڐ�������            "
#define OBJ_NAME34 "�߰�ė���               "
#define OBJ_NAME35 "�Ư�                    "
#define OBJ_NAME36 "�Θg                    "
#define OIF_TITLE  "OBJINFO "

typedef struct {
	double   p[DIM];
} PNTTYPE;																//<< ���W�_ >>

typedef struct {
	int      p[DIM];
} SCRTYPE;																//<< ��ذ݌n���W�_ >>

typedef struct {
	double   p[DIM];
} VECTYPE;																//<< �޸�� >>

typedef struct {
	PNTTYPE  bp[2];														// �Ίp�̍��W
} BOXTYPE;																//<< �ޯ�� >>

typedef struct cmdtp {
	struct cmdtp* onxt;													// ��������߲��
	struct cmdtp* lnxt;													// �������ٓ����߲��
	struct objtp* op;													// �Ώۗ����߲��
	BYTE     cno;														// �����No
	int      isa;														// �������Ұ��J�n�Ԓn
	int      inum;														// �������Ұ���
	int      psa;														// �_���W���Ұ��J�n�Ԓn
	int      pnum;														// �_���W���Ұ���
	int      clvl;														// ���������No
} CMDTYPE;																//<< ����ޏ�� >>

typedef struct cpctp {
	struct cpctp* next;													// ��������߲��
	BYTE     cno;														// �����No
	int      isa;														// �������Ұ��J�n�Ԓn
	int      inum;														// �������Ұ���
	int      psa;														// �_���W���Ұ��J�n�Ԓn
	int      pnum;														// �_���W���Ұ���
} CPCTYPE;																//<< ��߰����ޏ�� >>

typedef struct objtp {
	struct objtp* onxt;													// �������߲��
	struct objtp* cnxt;													// ���ꐶ�����ٓ����߲��
	struct objtp* dnxt;													// ����폜���ٓ����߲��
	struct objtp* oset;													// ��ė����߲��
	struct oiftp* oift;													// ���̏���߲��
	CMDTYPE* bcmp;														// �擪������߲��
	CMDTYPE* ccmp;														// �L���擪������߲��
	CMDTYPE* rscmp;														// �ݸސ擪������߲��
	CMDTYPE* rccmp;														// �ݸތ��ݺ�����߲��
	CMDTYPE* ecmp;														// ���������߲��
	BYTE     attr;														// ����No
	BYTE     part;														// �߰�No
	BYTE     color;														// �װNo
	BYTE     dspflg;													// �\��(�L��)�׸�
	BYTE     edtflg;													// �ҏW�Ώ��׸�
	BYTE     lmode;														// ؽĥӰ��
	BYTE     selno;														// �I��No
	unsigned int group;													// ��ٰ�ߏ��
	unsigned int list;													// �ި���ڲؽĔԍ�
	BOXTYPE  box;														// �ޯ������
	int      vsa;														// ���_���W�J�n�Ԓn
	int      vnum;														// ���_���W��
	int      lsa;														// ��ٰ�ߊJ�n�Ԓn
	int      lnum1;														// ��ٰ�ߐ�1
	int      lnum2;														// ��ٰ�ߐ�2
	int      ono;														// ����No
} OBJTYPE;																//<< ���̏�� >>

typedef struct oiftp {
	struct oiftp* next;													// �������߲��
	OBJTYPE* op;														// �Ώۗ����߲��
	char     name[ONM_LENG];											// ���̖���(�S�p12����)
	short    level;														// ����No
	short    ono;														// ����No
} OIFTYPE;																//<< ���̍\����� >>

typedef struct {
	short    urflg;														// Undo/Redo�׸�
	short    cmflg;														// Redo������׸�
	CMDTYPE* cmp;														// �Ώۺ���ސ擪�߲��
	OBJTYPE* cop;														// �Ώۗ��̐擪�߲��(����)
	OBJTYPE* dop;														// �Ώۗ��̐擪�߲��(�폜)
	int      clvl;														// ���������No
} UNDTYPE;																//<< Undo/Redo��� >>

typedef struct vttp {
	struct vttp* next;													// ���ް����߲��
	int      no;														// ��ٰ�ߔԍ�
	PNTTYPE  pt;														// ���_���W
	int      vt;														// ���_�ԍ�
	BYTE     flg;														// ��ٰ�ߏI���׸�
} VTYPE;																//<< ���_���ėp >>

typedef struct oitdf {
	char     title[8];													// ����("OBJINFO ")
	struct {
		char level[3];													// ����No
		char ono[4];													// ����No
		char name[ONM_LENG];											// ���̖���
	} dat[8];
} OIFDATA;																//<< ���̏��̧�ْ�` >>

typedef struct {
	int      flg;														// �׸�(0:����, 1:�L��)
	int      type;														// ��������(0:���s, 1:�_)
	int      k;															// �_������������
	COLORREF lc;														// �����FRGB
	double   x1, y1, z1;												// �������W�ʒu
	double   x2, y2, z2;												// ���������ʒu
} LIGHT;																//<< ������� >>

typedef struct {
	COLORREF ka;														// ����RGB
	COLORREF kd;														// �g�U���ˌ�RGB
	COLORREF ks;														// ���ʔ��ˌW��RGB
	COLORREF ct;														// ���ߒ����W��RGB
	int      trace;														// ���ˋ��܌v�Z(�f��)�L��
	double   n;															// �������ܗ�(-1-)
	int      model;														// ����(0:Phong,1:Cook-T)
	int      m1;														// ʲײĂ̍L����1(Phong)
	double   m2;														// ʲײĂ̍L����2(Cook-T)
	double   nr, ng, nb;												// �������ܗ�(Cook-T)
} ATTRIBT;																//<< �\�ʑ������ >>

typedef struct cwktp {
	struct cwktp* next;													// ���ް����߲��
	struct owktp* owk;													// ���̍���ް��߲��
	CMDTYPE* cmp;														// ������߲��
	int      clvl;														// ���������No
	BYTE     flg;														// �����׸�
} CWKTYPE;																//<< ����ލ���ް� >>

typedef struct owktp {
	struct owktp* next;													// ���ް����߲��
	OBJTYPE* op;														// �����߲��
	CWKTYPE* cwk;														// ����ލ���ް��߲��
} OWKTYPE;																//<< ���̍���ް� >>

typedef struct nvttp {
	struct   nvttp *next;												// ���ް����߲��
	int      no;														// ��ٰ�ߔԍ�
	PNTTYPE  pt;														// ���_���W
	VECTYPE  vc;														// �@���޸��
	short    fg1;														// �@���␮�׸�1
	short    fg2;														// �@���␮�׸�2
} NVTYPE;																//<< ���_���ėp >>

#endif ___DATATYPE_H

//==============================<EOF:DataType.h>=============================
