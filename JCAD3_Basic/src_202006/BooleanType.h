/////////////////////////////////////////////////////////////////////////////
// 図形演算作業用構造体及びマクロ定義
// BooleanType.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/24by M.Fukushima
/////////////////////////////////////////////////////////////////////////////

typedef struct bfatp {
	struct  bfatp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	double  xmin;														// 面のX最小値
	double  xmax;														// 面のX最大値
	double  ymin;														// 面のY最小値
	double  ymax;														// 面のY最大値
	double  zmin;														// 面のZ最小値
	double  zmax;														// 面のZ最大値
	VECTYPE vec;														// 面の法線ﾍﾞｸﾄﾙ
	PNTTYPE inpt;														// 面の中に存在する点
	int     stfl;														// 面ﾙｰﾌﾟ開始番地
} BFATYPE;																//<< 面情報抽出用 >>

typedef struct bfctp {
	struct  bfctp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	int     flg;														// ﾃﾞｰﾀﾌﾗｸﾞ(0:出力,1:比較)
	int     sno;														// 始点番号(ﾙｰﾌﾟ/頂点)
	int     eno;														// 終点番号(ﾙｰﾌﾟ/頂点)
	PNTTYPE pnt;														// 交点
} BFCTYPE;																//<< 面交点算出用 >>

typedef struct bcltp {
	struct  bcltp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	int     sfg;														// 始点･ﾃﾞｰﾀﾌﾗｸﾞ
	int     ssn;														// 始点･始点番号
	int     sen;														// 始点･終点番号
	PNTTYPE spt;														// 始点･交点
	int     efg;														// 終点･ﾃﾞｰﾀﾌﾗｸﾞ
	int     esn;														// 終点･始点番号
	int     een;														// 終点･終点番号
	PNTTYPE ept;														// 終点･交点
	int     pfg;														// 出力ﾌﾗｸﾞ
	short   cfs;														// 始点･確認ﾌﾗｸﾞ
	short   cfe;														// 終点･確認ﾌﾗｸﾞ
} BCLTYPE;																//<< 面交差稜線算出用 >>

typedef struct bcptp {
	struct  bcptp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	PNTTYPE pnt;														// 頂点座標
} BCPTYPE;																//<< 分割線頂点座標 >>

typedef struct bcdtp {
	struct   bcdtp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	int      flg;														// 分割線ﾌﾗｸﾞ(0:Out,1:In)
	int      ssn;														// ﾙｰﾌﾟ開始番地(Out)
	int      sen;
	PNTTYPE  spt;														// ﾙｰﾌﾟ開始頂点
	double   sln;														// 開始頂点位置
	int      sfg;														// 開始ﾁｪｯｸﾌﾗｸﾞ
	int      esn;														// ﾙｰﾌﾟ終了番地(Out)
	int      een;
	PNTTYPE  ept;														// ﾙｰﾌﾟ終了頂点
	double   eln;														// 終了頂点位置
	int      efg;														// 終了ﾁｪｯｸﾌﾗｸﾞ
	int      sno;														// 設定番号
	int      pfg;														// 出力ﾌﾗｸﾞ
	BCPTYPE* cpp;														// 頂点座標のﾎﾟｲﾝﾀ
} BCDTYPE;																//<< 分割線先頭ﾃﾞｰﾀ >>

typedef struct bvltp {
	struct  bvltp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	int     no;															// 面ﾙｰﾌﾟ番号
	PNTTYPE pt;															// 頂点座標
	int     vt;															// 頂点番号
	short   lfg;														// 面ﾙｰﾌﾟ終了ﾌﾗｸﾞ
	short   pfg;														// 面ﾙｰﾌﾟ出力ﾌﾗｸﾞ
} BVLTYPE;																//<< 頂点･ﾙｰﾌﾟ設定用 >>

typedef struct bvttp {
	struct  bvttp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	PNTTYPE pt;															// 頂点座標
	int     vt;															// 頂点番号
} BVTTYPE;																//<< 頂点設定用 >>

typedef struct boptp {
	struct   boptp* next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	OBJTYPE* op;														// 対象立体のﾎﾟｲﾝﾀ
	int      flg;														// ﾌﾗｸﾞ
	PNTTYPE  cpt;														// 対象立体中心点
	double   leng;														// 対象立体基準点距離
} BOPTYPE;																//<< 図形演算対象立体ﾃﾞｰﾀ >>

BFATYPE* Aflist, * Bflist;												// 面情報抽出ﾘｽﾄﾎﾟｲﾝﾀ
BVLTYPE* A0list, * A1list, * B0list, * B1list;							// 頂点･ﾙｰﾌﾟ設定ﾘｽﾄﾎﾟｲﾝﾀ
BFCTYPE* Fclist;														// 面交点算出用ﾘｽﾄﾎﾟｲﾝﾀ
BCLTYPE* Cllist;														// 面交差稜線算出用ﾘｽﾄﾎﾟｲﾝﾀ
BCDTYPE* Cdlist;														// 分割線先頭ﾃﾞｰﾀﾘｽﾄﾎﾟｲﾝﾀ
int      Afcnt, Bfcnt;													// 面情報ﾘｽﾄ数
int      A0cnt, A1cnt, B0cnt, B1cnt;									// 頂点･面ﾙｰﾌﾟﾘｽﾄ数
int      Cocnt, Cicnt;													// 分割線先頭ﾃﾞｰﾀﾘｽﾄ数
int      ErrFlg1, ErrFlg2;												// ｴﾗｰﾌﾗｸﾞ
double   Spr;															// 進捗率用ﾊﾟｰｾﾝﾄ
double   Ppr;															// 演算毎のﾊﾟｰｾﾝﾄ
double   ReszB;															// ｻｲｽﾞ補正値

//============================<EOF:BooleanType.h>============================
