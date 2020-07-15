/////////////////////////////////////////////////////////////////////////////
// 曲面制御点生成部プロトタイプ宣言
// CreatCtrlPoint1.h
//---------------------------------------------------------------------------
// LastEdit : 2009/05/27 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void SetBasePoint(int, int, BOXTYPE, PNTTYPE*);							// 断面生成座標設定
BOOL InitCreat(int, int, int, PNTTYPE*, int*, int*);					// 生成初期処理
void InitCreat2(int, int, int, PNTTYPE*, int*);							// 生成初期処理2
int  GetCurveCenter(int, int, int, PNTTYPE*, PNTTYPE*);					// 2D図形中心取得
void ChangeRotate2D(int, int, int, PNTTYPE*);							// 2D断面回転更新
void InitSweep(PNTTYPE, int, int, int, PNTTYPE*);						// 掃引初期処理
void CalcSweepPoint(int, int, int, int, int, int, int, PNTTYPE*,
                    int*, PNTTYPE*, int*, PNTTYPE*, int*, double*);		// 掃引体制御点計算
BOOL InsertSweepPoint(int, int, int, int*, int*, PNTTYPE*, int*);		// 掃引体中間制御点挿入
void SetCircleSection(int, int, double, int, int, PNTTYPE*,
                      PNTTYPE*, PNTTYPE*, int*);						// 円断面割付
void SetCurveSection(int, int, int, int, int, int, BOXTYPE, int,
                     int, PNTTYPE*, int*, PNTTYPE*, int*);				// 曲線断面割付
void CalcCurveSection(int, int, int, int, int, int, PNTTYPE*,
                      PNTTYPE*, int*, PNTTYPE*, int*);                  // 曲線割付断面計算
void CrtSectionSweep(int, int, int, int, int, int, PNTTYPE*,
                     PNTTYPE*, int*, PNTTYPE*, int*, PNTTYPE*, int*);	// 割付断面掃引体生成
void AddCenterPoint(int*, int, int, PNTTYPE*, int*);					// 端面中心点追加
void CreatSetCurve(int, int, int, int, int, int);						// 曲線組合せ

//==========================<EOF:CreatCtrlPoint1.h>==========================
