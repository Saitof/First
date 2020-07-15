/////////////////////////////////////////////////////////////////////////////
// レンダリング(RayTracing)操作ヘッダ
// RendType.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "DataType.h"

#define EPS_R   0.0001                                                  // 数値誤差
#define PTNUM   1000000                                                 // 頂点数
#define LLENGTH 400                                                     // 光源減衰計算の基本長
#define THRES   0.125                                                   // ｱﾝﾁｴｲﾘｱｼﾝｸﾞのしきい値
#define MOVEDST 0.33                                                    // ｱﾝﾁｴｲﾘｱｼﾝｸﾞの視線移動量

typedef struct llist {
    struct  llist* next;                                                // 次ﾎﾟｲﾝﾀ
    int     vno;                                                        // 頂点No
    VECTYPE vec;                                                        // 法線ﾍﾞｸﾄﾙ
} LPTYPE;                                                               //<<面ﾙｰﾌﾟ情報>>

typedef struct flist {
    struct  flist* next;                                                // 次ﾎﾟｲﾝﾀ
    PNTTYPE cpt;                                                        // 面中心点
    double  r;                                                          // 面を含む最小球半径
    VECTYPE vec;                                                        // 法線ﾍﾞｸﾄﾙ
    LPTYPE* lpl;                                                        // 面ﾙｰﾌﾟ情報ﾎﾟｲﾝﾀ
} FATYPE;                                                               //<<面情報>>

typedef struct olist {
    struct  olist* next;                                                // 次ﾎﾟｲﾝﾀ
    int     ano;                                                        // 表面属性No
    PNTTYPE cpt;                                                        // 立体中心点
    double  r;                                                          // 立体を含む最小球半径
    PNTTYPE cp8[8];                                                     // 8分割中心点
    double  r8[8];                                                      // 8分割最小球半径
    FATYPE* fal[8];                                                     // 面情報ﾎﾟｲﾝﾀ
} OBTYPE;                                                               //<<立体情報>>

typedef struct {
    double m2[3][3];
} RMAT;                                                                 //<<回転ﾏﾄﾘｯｸｽ>>

typedef struct {
    double r, g, b;
} RGBT;                                                                 //<<ｽｷｬﾝﾗｲﾝRGB情報>>

CRendDlg* Rend;                                                         // ﾚﾝﾀﾞﾘﾝｸﾞﾀﾞｲｱﾛｸﾞﾎﾟｲﾝﾀ
PNTTYPE Pdat[PTNUM];                                                    // 頂点座標領域
OBTYPE* Olist, * Olast;                                                 // 立体情報ﾎﾟｲﾝﾀ
FATYPE* Ffst, * Flst;                                                   // 面情報ﾎﾟｲﾝﾀ
LPTYPE* Lfst, * Llst;                                                   // 面ﾙｰﾌﾟ情報ﾎﾟｲﾝﾀ
LIGHT   Lgt[LGMAX];                                                     // 光源情報
int     LgtNum;                                                         // 光源数
double  LgtLeng;                                                        // ｽｹｰﾘﾝｸﾞ後減衰基本長
ATTRIBT Atr[ATMAX];                                                     // 表面属性情報
double  BkR, BkG, BkB;                                                  // 背景色(RGB)
int     MaxLevel;                                                       // 最大視線探査ﾚﾍﾞﾙ
int     AntiAlias;                                                      // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ
int     EdgeFlag;                                                       // 稜線ﾌﾗｸﾞ
PNTTYPE Eye;                                                            // 視点
PNTTYPE CentPs;                                                         // 参照点(注視点)
VECTYPE EyeVec;                                                         // 視線ﾍﾞｸﾄﾙ
int     Szx, Szy;                                                       // ﾀﾞｲｱﾛｸﾞ･ｻｲｽﾞ
double  Scale;                                                          // ｽｹｰﾘﾝｸﾞ倍率
int     Vtnum;                                                          // 頂点座標ｶｳﾝﾀ
RGBT    ScanLine[2][PXLMAX];                                            // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ用ﾊﾞｯﾌｧ
short   CalcMat[2][4];                                                  // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ計算方向
double  Xmin_f, Xmax_f, Ymin_f, Ymax_f, Zmin_f, Zmax_f;                 // 面最小･最大取得用
double  Xmin_o, Xmax_o, Ymin_o, Ymax_o, Zmin_o, Zmax_o;                 // 立体      〃

//============================<EOF:RendType.h>=============================
