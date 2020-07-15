/////////////////////////////////////////////////////////////////////////////
// 平面分割作業用構造体及びマクロ定義
// CutType.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/11 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
typedef struct cvltp {
	struct  cvltp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	int     no;															// 面ﾙｰﾌﾟ番号
	PNTTYPE pt;															// 頂点座標
	int     vt;															// 頂点番号
	int     flg;														// 面ﾙｰﾌﾟ終了ﾌﾗｸﾞ
} CVLTYPE;																//<< 頂点･ﾙｰﾌﾟ設定用 >>

typedef struct ccvtp {
	struct  ccvtp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	PNTTYPE spt;														// 開始頂点座標
	PNTTYPE ept;														// 終了頂点座標
} CCVTYPE;																//<< 分割断面設定用 >>

typedef struct ccptp {
	struct  ccptp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	PNTTYPE spt;														// 開始頂点座標
	PNTTYPE ept;														// 終了頂点座標
} CCPTYPE;																//<< 線分配列 >>

typedef struct ccltp {
	struct   ccltp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	PNTTYPE  spt;														// 開始頂点座標
	PNTTYPE  ept;														// 終了頂点座標
	CCPTYPE* cp;														// 線分配列のﾎﾟｲﾝﾀ
} CCLTYPE;																//<< 面分割線分配列 >>

CVLTYPE* Avlist, * Bvlist;												// A/Bﾘｽﾄﾎﾟｲﾝﾀ
CCVTYPE* Cvlist;														// 分割断面ﾘｽﾄﾎﾟｲﾝﾀ
CCLTYPE* Aclist, * Bclist;												// A/B線分配列ﾘｽﾄﾎﾟｲﾝﾀ
CCPTYPE* Cplist;														// 分割交点ﾘｽﾄﾎﾟｲﾝﾀ
int      Avcnt, Bvcnt;													// A/Bﾘｽﾄ数
int      Cvcnt;															// 分割断面ﾘｽﾄ数
int      Accnt, Bccnt;													// A/B線分配列数
int      Cpcnt;															// 分割交点数
double   Oper, Sper;													// 進捗率用ﾊﾟｰｾﾝﾄ
double   ReszC;															// ｻｲｽﾞ補正値

//==============================<EOF:CutType.h>==============================
