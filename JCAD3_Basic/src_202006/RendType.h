/////////////////////////////////////////////////////////////////////////////
// �����_�����O(RayTracing)����w�b�_
// RendType.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "DataType.h"

#define EPS_R   0.0001                                                  // ���l�덷
#define PTNUM   1000000                                                 // ���_��
#define LLENGTH 400                                                     // ���������v�Z�̊�{��
#define THRES   0.125                                                   // �����ر�ݸނ̂������l
#define MOVEDST 0.33                                                    // �����ر�ݸނ̎����ړ���

typedef struct llist {
    struct  llist* next;                                                // ���߲��
    int     vno;                                                        // ���_No
    VECTYPE vec;                                                        // �@���޸��
} LPTYPE;                                                               //<<��ٰ�ߏ��>>

typedef struct flist {
    struct  flist* next;                                                // ���߲��
    PNTTYPE cpt;                                                        // �ʒ��S�_
    double  r;                                                          // �ʂ��܂ލŏ������a
    VECTYPE vec;                                                        // �@���޸��
    LPTYPE* lpl;                                                        // ��ٰ�ߏ���߲��
} FATYPE;                                                               //<<�ʏ��>>

typedef struct olist {
    struct  olist* next;                                                // ���߲��
    int     ano;                                                        // �\�ʑ���No
    PNTTYPE cpt;                                                        // ���̒��S�_
    double  r;                                                          // ���̂��܂ލŏ������a
    PNTTYPE cp8[8];                                                     // 8�������S�_
    double  r8[8];                                                      // 8�����ŏ������a
    FATYPE* fal[8];                                                     // �ʏ���߲��
} OBTYPE;                                                               //<<���̏��>>

typedef struct {
    double m2[3][3];
} RMAT;                                                                 //<<��]��د��>>

typedef struct {
    double r, g, b;
} RGBT;                                                                 //<<����ײ�RGB���>>

CRendDlg* Rend;                                                         // �����ݸ��޲�۸��߲��
PNTTYPE Pdat[PTNUM];                                                    // ���_���W�̈�
OBTYPE* Olist, * Olast;                                                 // ���̏���߲��
FATYPE* Ffst, * Flst;                                                   // �ʏ���߲��
LPTYPE* Lfst, * Llst;                                                   // ��ٰ�ߏ���߲��
LIGHT   Lgt[LGMAX];                                                     // �������
int     LgtNum;                                                         // ������
double  LgtLeng;                                                        // ����ݸތ㌸����{��
ATTRIBT Atr[ATMAX];                                                     // �\�ʑ������
double  BkR, BkG, BkB;                                                  // �w�i�F(RGB)
int     MaxLevel;                                                       // �ő压���T������
int     AntiAlias;                                                      // �����ر�ݸ�
int     EdgeFlag;                                                       // �Ő��׸�
PNTTYPE Eye;                                                            // ���_
PNTTYPE CentPs;                                                         // �Q�Ɠ_(�����_)
VECTYPE EyeVec;                                                         // �����޸��
int     Szx, Szy;                                                       // �޲�۸ޥ����
double  Scale;                                                          // ����ݸޔ{��
int     Vtnum;                                                          // ���_���W����
RGBT    ScanLine[2][PXLMAX];                                            // �����ر�ݸޗp�ޯ̧
short   CalcMat[2][4];                                                  // �����ر�ݸތv�Z����
double  Xmin_f, Xmax_f, Ymin_f, Ymax_f, Zmin_f, Zmax_f;                 // �ʍŏ���ő�擾�p
double  Xmin_o, Xmax_o, Ymin_o, Ymax_o, Zmin_o, Zmax_o;                 // ����      �V

//============================<EOF:RendType.h>=============================
