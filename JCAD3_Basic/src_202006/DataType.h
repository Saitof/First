/////////////////////////////////////////////////////////////////////////////
// 内部データベース構造体及びマクロ定義
// DataType.h
//---------------------------------------------------------------------------
// LastEdit : 2010/03/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#ifndef ___DATATYPE_H
#define ___DATATYPE_H

#define EPSILON   0.00001												// 許容誤差
#define DEPSILON  0.0000001												// 許容誤差
#define PI        3.14159265358979323846264338							// 円周率
#define DIM       3														// 次元数
#define TRUE      1														// 正常終了
#define FALSE     0														// 異常終了
#define ERROR1    -1													// 異常終了1
#define ERROR2    -2													// 異常終了2
#define ON        1														// ON
#define OFF       0														// OFF
#define MAX_LEN   (50+1)												// 最大ﾌｧｲﾙ名
#define CRTSURF   0														// 曲面生成ｺﾏﾝﾄﾞNo
#define DIRECT    1														// 直接生成ｺﾏﾝﾄﾞNo
#define CRTCURVE  2														// 曲線生成ｺﾏﾝﾄﾞNo
#define CHGSURF   3														// 曲面変更ｺﾏﾝﾄﾞNo
#define CHGCURVE  4														// 曲線変更ｺﾏﾝﾄﾞNo
#define SETSURF   5														// 曲面設定ｺﾏﾝﾄﾞNo
#define SETCURVE  6														// 曲線設定ｺﾏﾝﾄﾞNo
#define THICKNESS 7														// 厚み付けｺﾏﾝﾄﾞNo
#define MOVE      8														// 平行移動ｺﾏﾝﾄﾞNo
#define ROTATE    9														// 回転移動ｺﾏﾝﾄﾞNo
#define MIRROR    10													// 鏡像移動ｺﾏﾝﾄﾞNo
#define SCALE     11													// ｻｲｽﾞ変更ｺﾏﾝﾄﾞNo
#define BEND      12													// 円筒曲げｺﾏﾝﾄﾞNo
#define GROUP     13													// ｸﾞﾙｰﾌﾟ設定ｺﾏﾝﾄﾞNo
#define COLOR     14													// ｶﾗｰNo設定ｺﾏﾝﾄﾞNo
#define ATTRIBUTE 15													// 属性No設定ｺﾏﾝﾄﾞNo
#define CRTSWEEP  16													// 掃引曲面生成ｺﾏﾝﾄﾞNo
#define CHGSWEEP  17													// 掃引曲面変更ｺﾏﾝﾄﾞNo
#define TWIST     18													// ひねりｺﾏﾝﾄﾞNo
#define RESIZE1   19													// 直線指定ﾘｻｲｽﾞ1ｺﾏﾝﾄﾞNo
#define PARTS     20													// ﾊﾟｰﾂNo設定ｺﾏﾝﾄﾞNo
#define RESIZE2   21													// 曲線指定ﾘｻｲｽﾞ1ｺﾏﾝﾄﾞNo
#define SHEAR     22													// せん断ｺﾏﾝﾄﾞNo
#define RING      23													// ﾘﾝｸﾞｺﾏﾝﾄﾞNo
#define EARTH     24													// ｱｰｽｺﾏﾝﾄﾞNo
#define SPACE     25													// ｽﾍﾟｰｽｺﾏﾝﾄﾞNo
#define RESIZE3   26													// 直線指定ﾘｻｲｽﾞ2ｺﾏﾝﾄﾞNo
#define RESIZE4   27													// 曲線指定ﾘｻｲｽﾞ2ｺﾏﾝﾄﾞNo
#define ENDCMD    15													// 立体最終ｺﾏﾝﾄﾞNo(属性No)
#define MAXCMD    27													// 最終ｺﾏﾝﾄﾞNo
#define DELCMD    50													// 削除ｺﾏﾝﾄﾞNo(ﾊﾞｯｸｱｯﾌﾟ用)
#define UNDOCMD   51													// UndoｺﾏﾝﾄﾞNo(ﾊﾞｯｸｱｯﾌﾟ用)
#define REDOCMD   52													// RedoｺﾏﾝﾄﾞNo(ﾊﾞｯｸｱｯﾌﾟ用)
#define GLBDCMD   70													// 先頭ｺﾏﾝﾄﾞNo(ﾃﾞｰﾀ集用)
#define RPWDCMD   98													// 読込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞNo(JGD/GGD用)
#define WPWDCMD   99													// 書込ﾊﾟｽﾜｰﾄﾞｺﾏﾝﾄﾞNo(JGD/GGD用)
#define MAXINTAN  10000000												// 整数ﾊﾟﾗﾒｰﾀ配列数
#define MAXPNTAN  4000000												// 頂点ﾊﾟﾗﾒｰﾀ配列数
#define MAXUNDON  100													// Undo/Redo配列数
#define MAXCCTLN  150      //100										// 最大曲線制御点数
#define MAXSCTLN  1500     //1000										// 最大曲面制御点数
#define MAXCCTLA  152      //102										// 曲線制御点配列数
#define MAXSCTLA  1650     //1100										// 曲面制御点配列数
#define MAXWCTLN  3000     //2000										// 作業用制御点配列数
#define MAXCPNTN  2500     //1680										// 最大曲線頂点数
#define MAXSPNTN  400000   //288000										// 最大曲面頂点数
#define MAXOPNTN  1000000  //1000000									// 最大立体頂点数
#define MAXBPNTN  30000    //30000										// 最大断面頂点数
#define LGMAX     8														// ﾚｲ･ﾄﾚｰｽ光源情報配列数
#define ATMAX     19													// ﾚｲ･ﾄﾚｰｽ表面属性配列数
#define POVCMAX   18													// POV-Rayｶﾗｰ数
#define PXLMAX    2000													// 1ｽｷｬﾝﾗｲﾝの最大ﾋﾟｸｾﾙ数
#define JSD_LENG  256													// JSD/JSFﾌｧｲﾙ ﾚｺｰﾄﾞ長
#define CMD_LENG  512													// CMDﾌｧｲﾙ ﾚｺｰﾄﾞ長
#define ONM_LENG  24													// OIF名称 ﾚｺｰﾄﾞ長
#define LIC_NAME  "Jcad3_Glb.lic"										// ﾗｲｾﾝｽﾌｧｲﾙ名
#define OFL_NAME  "Offline_Authent_Glb.dat"								// ｵﾌﾗｲﾝ認証ﾌｧｲﾙ名
#define JCN_NAME  "JColor_New.dat"										// JCAD3ｶﾗｰ設定ﾌｧｲﾙ名(新版)
#define JCO_NAME  "JColor_Old.dat"										// JCAD3ｶﾗｰ設定ﾌｧｲﾙ名(旧版)
#define MAP_NAME  "mapping.bmp"											// 環境ﾏｯﾋﾟﾝｸﾞﾌｧｲﾙ名
#define REN_NAME  "GLRender.dat"										// 初期OpenGLﾚﾝﾀﾞﾘﾝｸﾞﾌｧｲﾙ名
#define COL_NAME  "GLColor.dat"											// 初期OpenGLｶﾗｰﾌｧｲﾙ名
#define TEN_NAME  "RayTEnv.dat"											// 初期ﾚｲ･ﾄﾚｰｽ環境ﾌｧｲﾙ名
#define LGT_NAME  "RayTLight.dat"										// 初期ﾚｲ･ﾄﾚｰｽ光源ﾌｧｲﾙ名
#define TCL_NAME  "RayTColor.dat"										// 初期ﾚｲ･ﾄﾚｰｽｶﾗｰﾌｧｲﾙ名
#define SY1_NAME  "SysEnv1.bak"											// ｼｽﾃﾑ環境設定保存ﾌｧｲﾙ名1
#define SY2_NAME  "SysEnv2.bak"											// ｼｽﾃﾑ環境設定保存ﾌｧｲﾙ名2
#define SY3_NAME  "SysEnv3.bak"											// ｼｽﾃﾑ環境設定保存ﾌｧｲﾙ名3
#define SY4_NAME  "SysEnv4.bak"											// ｼｽﾃﾑ環境設定保存ﾌｧｲﾙ名4
#define SY5_NAME  "SysEnv5.bak"											// ｼｽﾃﾑ環境設定保存ﾌｧｲﾙ名5
#define RN1_NAME  "RndEnv1.bak"											// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名1
#define RN2_NAME  "RndEnv2.bak"											// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名2
#define RN3_NAME  "RndEnv3.bak"											// ﾚﾝﾀﾞﾘﾝｸﾞ環境設定保存ﾌｧｲﾙ名3
#define RT1_NAME  "RayEnv1.bak"											// ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名1
#define RT2_NAME  "RayEnv2.bak"											// ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名2
#define RT3_NAME  "RayEnv3.bak"											// ﾚｲﾄﾚｰｼﾝｸﾞ環境設定保存ﾌｧｲﾙ名3
#define PV1_NAME  "PovEnv1.bak"											// POV-Ray環境設定保存ﾌｧｲﾙ名1
#define PV2_NAME  "PovEnv2.bak"											// POV-Ray環境設定保存ﾌｧｲﾙ名2
#define PV3_NAME  "PovEnv3.bak"											// POV-Ray環境設定保存ﾌｧｲﾙ名3
#define JCS_NAME  "JColor.sav"    										// JCAD3ｶﾗｰ設定保存ﾌｧｲﾙ名
#define FPS_NAME  "FilePaths.sav"    									// ﾌｧｲﾙﾊﾟｽ保存ﾌｧｲﾙ名
#define RDS_NAME  "GLRender.sav"										// OpenGLﾚﾝﾀﾞﾘﾝｸﾞ保存ﾌｧｲﾙ名
#define SIS_NAME  "ScrInfo.sav"											// 画面制御情報保存ﾌｧｲﾙ名
#define DTS_NAME  "SystemData.sav"										// ｼｽﾃﾑﾃﾞｰﾀ保存ﾌｧｲﾙ名
#define MES_NAME  "MemData.sav"											// ﾒﾓﾘ領域ﾃﾞｰﾀ保存ﾌｧｲﾙ名
#define RTS_NAME  "RayTrace.sav"										// ﾚｲ･ﾄﾚｰｽ保存ﾌｧｲﾙ名
#define POV_NAME  "PovRay.sav"											// POV-Ray保存ﾌｧｲﾙ名
#define POV_DEF1  "Setting_Defo.txt"									// POV-Ray定義ﾌｧｲﾙ名(初期値)
#define POV_DEF2  "user_01.inc"											// ﾕｰｻﾞ定義ﾌｧｲﾙ名1(初期値)
#define POV_DEF3  "user_02.inc"											// ﾕｰｻﾞ定義ﾌｧｲﾙ名2(初期値)
#define CFAC_NAME "Cfac32.sav"											// CFAC32用ﾃﾞｰﾀ保存ﾌｧｲﾙ名
#define SLCA_NAME "Slca32.sav"											// SLCA32用ﾃﾞｰﾀ保存ﾌｧｲﾙ名
#define NCET_NAME "Ncet32.sav"											// NCET32用ﾃﾞｰﾀ保存ﾌｧｲﾙ名
#define NCRV_NAME "Ncrv32.sav"											// NCRV32用ﾃﾞｰﾀ保存ﾌｧｲﾙ名
#define NCSM_NAME "Ncsm32.sav"											// NCSM32用ﾃﾞｰﾀ保存ﾌｧｲﾙ名
#define TMP_NAME  "temp_file.tmp"										// 一時作業ﾌｧｲﾙ名
#define NCT_NAME  "nc_file.tmp"											// NC表示用作業ﾌｧｲﾙ名
#define CNV_NAME  "_conv_jpg_tempfile_.bmp"								// JPEGﾌｧｲﾙ変換作業ﾌｧｲﾙ名
#define PCB_NAME  "PlaneCutBackup_%d.jgd"								// 平面分割ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名
#define BLB_NAME  "BooleanBackup_%d.jgd"								// 図形演算ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名
#define BUP_NAME  "BackUp.bak"											// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名(1)
#define BOD_NAME  "BackUp.old"											// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ名(2)
#define COP_NAME1 "CopyData.cdt"										// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ名1
#define COP_NAME2 "CopyData_%c.cdt"										// ｺﾋﾟｰﾃﾞｰﾀﾌｧｲﾙ名2
#define GLT_NAME  "gltest.pri"											// OpenGLﾃｽﾄﾌﾟﾘﾐﾃｨﾌﾞ名
#define SPH_NAME  "sphere.pri"											// 球体ﾌﾟﾘﾐﾃｨﾌﾞ名
#define CUB_NAME  "cube.pri"											// 直方体ﾌﾟﾘﾐﾃｨﾌﾞ名
#define CYL_NAME  "cylinder.pri"										// 円柱ﾌﾟﾘﾐﾃｨﾌﾞ名
#define CON_NAME  "cone.pri"											// 円錐ﾌﾟﾘﾐﾃｨﾌﾞ名
#define CIR_NAME  "circle.pri"											// 円ﾌﾟﾘﾐﾃｨﾌﾞ名
#define SQU_NAME  "square.pri"											// 正方形ﾌﾟﾘﾐﾃｨﾌﾞ名
#define PRO_NAME  "prong.pri"											// 石枠･爪ﾌﾟﾘﾐﾃｨﾌﾞ名
#define BRI_NAME  "brilliant.jwl"										// ﾌﾞﾘﾘｱﾝﾄ ｶｯﾄ名
#define HEA_NAME  "heartbri.jwl"										// ﾊｰﾄｼｪｰﾌﾟﾌﾞﾘﾘｱﾝﾄ名
#define SNG_NAME  "single.jwl"											// ｼﾝｸﾞﾙ ｶｯﾄ名
#define EMR_NAME  "emerald.jwl"											// ｴﾒﾗﾙﾄﾞ ｶｯﾄ名
#define BGE_NAME  "baguette.jwl"										// ﾊﾞｹﾞｯﾄ ｶｯﾄ名
#define ROS_NAME  "rose.jwl"											// ﾛｰｽﾞ ｶｯﾄ名
#define CAB_NAME  "cabochon.jwl"										// ｶﾎﾞｼｮﾝ ｶｯﾄ名
#define G1X_NAME  "glb1_xy.glb"											// GLB1 XY平面
#define G1Y_NAME  "glb1_yz.glb"											// GLB1 YZ平面
#define G1Z_NAME  "glb1_zx.glb"											// GLB1 ZX平面
#define G2X_NAME  "glb2_xy.glb"											// GLB2 XY平面
#define G2Y_NAME  "glb2_yz.glb"											// GLB2 YZ平面
#define G2Z_NAME  "glb2_zx.glb"											// GLB2 ZX平面
#define G3X_NAME  "glb3_xy.glb"											// GLB3 XY平面
#define G3Y_NAME  "glb3_yz.glb"											// GLB3 YZ平面
#define G3Z_NAME  "glb3_zx.glb"											// GLB3 ZX平面
#define G4X_NAME  "glb4_xy.glb"											// GLB4 XY平面
#define G4Y_NAME  "glb4_yz.glb"											// GLB4 YZ平面
#define G4Z_NAME  "glb4_zx.glb"											// GLB4 ZX平面
#define OBJ_NAME00 "名称未定                "							// 立体名称
#define OBJ_NAME01 "曲線(閉)                "
#define OBJ_NAME02 "曲線(開)                "
#define OBJ_NAME03 "通常曲面掃引体          "
#define OBJ_NAME04 "通常曲面掃引体[面体]    "
#define OBJ_NAME05 "掃引曲面掃引体          "
#define OBJ_NAME06 "掃引曲面掃引体[面体]    "
#define OBJ_NAME07 "通常曲面回転体          "
#define OBJ_NAME08 "通常曲面回転体[面体]    "
#define OBJ_NAME09 "掃引曲面回転体          "
#define OBJ_NAME10 "掃引曲面回転体[面体]    "
#define OBJ_NAME11 "通常曲面錐体            "
#define OBJ_NAME12 "通常曲面錐体[面体]      "
#define OBJ_NAME13 "円(2D)                  "
#define OBJ_NAME14 "長方形(2D)              "
#define OBJ_NAME15 "球体(3D)                "
#define OBJ_NAME16 "直方体(3D)              "
#define OBJ_NAME17 "円柱(3D)                "
#define OBJ_NAME18 "円錐(3D)                "
#define OBJ_NAME19 "ﾗｳﾝﾄﾞ･ﾌﾞﾘﾘｱﾝﾄ･ｶｯﾄ       "
#define OBJ_NAME20 "ｵｰﾊﾞﾙ･ﾌﾞﾘﾘｱﾝﾄ･ｶｯﾄ       "
#define OBJ_NAME21 "ﾏｰｷｰｽﾞ･ﾌﾞﾘﾘｱﾝﾄ･ｶｯﾄ      "
#define OBJ_NAME22 "ﾍﾟｱｼｪｰﾌﾟ･ﾌﾞﾘﾘｱﾝﾄ･ｶｯﾄ    "
#define OBJ_NAME23 "ﾊｰﾄｼｪｰﾌﾟ･ﾌﾞﾘﾘｱﾝﾄ･ｶｯﾄ    "
#define OBJ_NAME24 "ｼﾝｸﾞﾙ･ｶｯﾄ               "
#define OBJ_NAME25 "ｴﾒﾗﾙﾄﾞ･ｶｯﾄ              "
#define OBJ_NAME26 "ﾊﾞｹﾞｯﾄ･ｶｯﾄ              "
#define OBJ_NAME27 "ﾃｰﾊﾟｰﾄﾞ･ﾊﾞｹﾞｯﾄ･ｶｯﾄ      "
#define OBJ_NAME28 "ﾛｰｽﾞ･ｶｯﾄ                "
#define OBJ_NAME29 "ｶﾎﾞｼｮﾝ･ｶｯﾄ              "
#define OBJ_NAME30 "石座                    "
#define OBJ_NAME31 "爪                      "
#define OBJ_NAME32 "ﾃｷｽﾄ                    "
#define OBJ_NAME33 "直接生成立体            "
#define OBJ_NAME34 "ﾍﾟｰｽﾄ立体               "
#define OBJ_NAME35 "ﾕﾆｯﾄ                    "
#define OBJ_NAME36 "石枠                    "
#define OIF_TITLE  "OBJINFO "

typedef struct {
	double   p[DIM];
} PNTTYPE;																//<< 座標点 >>

typedef struct {
	int      p[DIM];
} SCRTYPE;																//<< ｽｸﾘｰﾝ系座標点 >>

typedef struct {
	double   p[DIM];
} VECTYPE;																//<< ﾍﾞｸﾄﾙ >>

typedef struct {
	PNTTYPE  bp[2];														// 対角の座標
} BOXTYPE;																//<< ﾎﾞｯｸｽ >>

typedef struct cmdtp {
	struct cmdtp* onxt;													// 次ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	struct cmdtp* lnxt;													// 同一ﾚﾍﾞﾙ内次ﾎﾟｲﾝﾀ
	struct objtp* op;													// 対象立体ﾎﾟｲﾝﾀ
	BYTE     cno;														// ｺﾏﾝﾄﾞNo
	int      isa;														// 整数ﾊﾟﾗﾒｰﾀ開始番地
	int      inum;														// 整数ﾊﾟﾗﾒｰﾀ数
	int      psa;														// 点座標ﾊﾟﾗﾒｰﾀ開始番地
	int      pnum;														// 点座標ﾊﾟﾗﾒｰﾀ数
	int      clvl;														// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
} CMDTYPE;																//<< ｺﾏﾝﾄﾞ情報 >>

typedef struct cpctp {
	struct cpctp* next;													// 次ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	BYTE     cno;														// ｺﾏﾝﾄﾞNo
	int      isa;														// 整数ﾊﾟﾗﾒｰﾀ開始番地
	int      inum;														// 整数ﾊﾟﾗﾒｰﾀ数
	int      psa;														// 点座標ﾊﾟﾗﾒｰﾀ開始番地
	int      pnum;														// 点座標ﾊﾟﾗﾒｰﾀ数
} CPCTYPE;																//<< ｺﾋﾟｰｺﾏﾝﾄﾞ情報 >>

typedef struct objtp {
	struct objtp* onxt;													// 次立体ﾎﾟｲﾝﾀ
	struct objtp* cnxt;													// 同一生成ﾚﾍﾞﾙ内次ﾎﾟｲﾝﾀ
	struct objtp* dnxt;													// 同一削除ﾚﾍﾞﾙ内次ﾎﾟｲﾝﾀ
	struct objtp* oset;													// ｾｯﾄ立体ﾎﾟｲﾝﾀ
	struct oiftp* oift;													// 立体情報ﾎﾟｲﾝﾀ
	CMDTYPE* bcmp;														// 先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	CMDTYPE* ccmp;														// 有効先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	CMDTYPE* rscmp;														// ﾘﾝｸﾞ先頭ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	CMDTYPE* rccmp;														// ﾘﾝｸﾞ現在ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	CMDTYPE* ecmp;														// ｱｰｽｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	BYTE     attr;														// 属性No
	BYTE     part;														// ﾊﾟｰﾂNo
	BYTE     color;														// ｶﾗｰNo
	BYTE     dspflg;													// 表示(有効)ﾌﾗｸﾞ
	BYTE     edtflg;													// 編集対象ﾌﾗｸﾞ
	BYTE     lmode;														// ﾘｽﾄ･ﾓｰﾄﾞ
	BYTE     selno;														// 選択No
	unsigned int group;													// ｸﾞﾙｰﾌﾟ情報
	unsigned int list;													// ﾃﾞｨｽﾌﾟﾚｲﾘｽﾄ番号
	BOXTYPE  box;														// ﾎﾞｯｸｽｻｲｽﾞ
	int      vsa;														// 頂点座標開始番地
	int      vnum;														// 頂点座標数
	int      lsa;														// 面ﾙｰﾌﾟ開始番地
	int      lnum1;														// 面ﾙｰﾌﾟ数1
	int      lnum2;														// 面ﾙｰﾌﾟ数2
	int      ono;														// 立体No
} OBJTYPE;																//<< 立体情報 >>

typedef struct oiftp {
	struct oiftp* next;													// 次立体ﾎﾟｲﾝﾀ
	OBJTYPE* op;														// 対象立体ﾎﾟｲﾝﾀ
	char     name[ONM_LENG];											// 立体名称(全角12文字)
	short    level;														// ﾚﾍﾞﾙNo
	short    ono;														// 立体No
} OIFTYPE;																//<< 立体構成情報 >>

typedef struct {
	short    urflg;														// Undo/Redoﾌﾗｸﾞ
	short    cmflg;														// Redoｺﾏﾝﾄﾞﾌﾗｸﾞ
	CMDTYPE* cmp;														// 対象ｺﾏﾝﾄﾞ先頭ﾎﾟｲﾝﾀ
	OBJTYPE* cop;														// 対象立体先頭ﾎﾟｲﾝﾀ(生成)
	OBJTYPE* dop;														// 対象立体先頭ﾎﾟｲﾝﾀ(削除)
	int      clvl;														// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
} UNDTYPE;																//<< Undo/Redo情報 >>

typedef struct vttp {
	struct vttp* next;													// 次ﾃﾞｰﾀへﾎﾟｲﾝﾀ
	int      no;														// 面ﾙｰﾌﾟ番号
	PNTTYPE  pt;														// 頂点座標
	int      vt;														// 頂点番号
	BYTE     flg;														// 面ﾙｰﾌﾟ終了ﾌﾗｸﾞ
} VTYPE;																//<< 頂点情報ｿｰﾄ用 >>

typedef struct oitdf {
	char     title[8];													// ﾀｲﾄﾙ("OBJINFO ")
	struct {
		char level[3];													// ﾚﾍﾞﾙNo
		char ono[4];													// 立体No
		char name[ONM_LENG];											// 立体名称
	} dat[8];
} OIFDATA;																//<< 立体情報ﾌｧｲﾙ定義 >>

typedef struct {
	int      flg;														// ﾌﾗｸﾞ(0:無効, 1:有効)
	int      type;														// 光源ﾀｲﾌﾟ(0:平行, 1:点)
	int      k;															// 点光源減衰ﾀｲﾌﾟ
	COLORREF lc;														// 光源色RGB
	double   x1, y1, z1;												// 光源座標位置
	double   x2, y2, z2;												// 光源視線位置
} LIGHT;																//<< 光源情報 >>

typedef struct {
	COLORREF ka;														// 環境光RGB
	COLORREF kd;														// 拡散反射光RGB
	COLORREF ks;														// 鏡面反射係数RGB
	COLORREF ct;														// 透過調整係数RGB
	int      trace;														// 反射屈折計算(映込)有無
	double   n;															// 視線屈折率(-1-)
	int      model;														// ﾓﾃﾞﾙ(0:Phong,1:Cook-T)
	int      m1;														// ﾊｲﾗｲﾄの広がり1(Phong)
	double   m2;														// ﾊｲﾗｲﾄの広がり2(Cook-T)
	double   nr, ng, nb;												// 分光屈折率(Cook-T)
} ATTRIBT;																//<< 表面属性情報 >>

typedef struct cwktp {
	struct cwktp* next;													// 次ﾃﾞｰﾀへﾎﾟｲﾝﾀ
	struct owktp* owk;													// 立体作業ﾃﾞｰﾀﾎﾟｲﾝﾀ
	CMDTYPE* cmp;														// ｺﾏﾝﾄﾞﾎﾟｲﾝﾀ
	int      clvl;														// ｺﾏﾝﾄﾞﾚﾍﾞﾙNo
	BYTE     flg;														// 処理ﾌﾗｸﾞ
} CWKTYPE;																//<< ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀ >>

typedef struct owktp {
	struct owktp* next;													// 次ﾃﾞｰﾀへﾎﾟｲﾝﾀ
	OBJTYPE* op;														// 立体ﾎﾟｲﾝﾀ
	CWKTYPE* cwk;														// ｺﾏﾝﾄﾞ作業ﾃﾞｰﾀﾎﾟｲﾝﾀ
} OWKTYPE;																//<< 立体作業ﾃﾞｰﾀ >>

typedef struct nvttp {
	struct   nvttp *next;												// 次ﾃﾞｰﾀのﾎﾟｲﾝﾀ
	int      no;														// 面ﾙｰﾌﾟ番号
	PNTTYPE  pt;														// 頂点座標
	VECTYPE  vc;														// 法線ﾍﾞｸﾄﾙ
	short    fg1;														// 法線補整ﾌﾗｸﾞ1
	short    fg2;														// 法線補整ﾌﾗｸﾞ2
} NVTYPE;																//<< 頂点情報ｿｰﾄ用 >>

#endif ___DATATYPE_H

//==============================<EOF:DataType.h>=============================
