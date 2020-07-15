/////////////////////////////////////////////////////////////////////////////
// 数値関数部プロトタイプ宣言
// NumFunc.h
//---------------------------------------------------------------------------
// LastEdit : 2009/12/19 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
double VecL(VECTYPE*);													// ﾍﾞｸﾄﾙ長さ
void VecO(VECTYPE*, VECTYPE*, VECTYPE*);								// 3次元ﾍﾞｸﾄﾙ外積
void VecN(VECTYPE*);													// 単位ﾍﾞｸﾄﾙ化(3D)
void VecV(PNTTYPE*, PNTTYPE*, VECTYPE*);								// 始点->終点ﾍﾞｸﾄﾙ
double PntD(PNTTYPE*, PNTTYPE*);										// 2点間距離(3D)
double CalcArea(double, double, double);								// 三角形面積算出
void SetRotateMatrix(VECTYPE axis, double th, double rot[][3]);			// 回転ﾏﾄﾘｯｸｽｾｯﾄ
void VecRotate(PNTTYPE* pos, double rot[][3]);							// 頂点座標回転
void VecRotateMove(PNTTYPE* pos, double rot[][3], PNTTYPE dis);			// 頂点座標回転&平行移動
void Affine(OBJTYPE* op, PNTTYPE cp, double mat[3][3]);					// 頂点座標ｱﾌｨﾝ変換
void AffineVtx(int pn, PNTTYPE* pa, PNTTYPE cp, double mat[3][3]);		// 頂点座標ｱﾌｨﾝ変換
void CalcCent(PNTTYPE*, int, PNTTYPE*);									// 図形重心取得
void CalcBox(PNTTYPE*, int, BOXTYPE*);									// 図形BOXｻｲｽﾞ取得
void CalcNVec(PNTTYPE*, int, VECTYPE*);									// 図形法線ﾍﾞｸﾄﾙ取得
void CalcRotAngleVec(PNTTYPE*, double*, VECTYPE*);						// 図形法線ﾍﾞｸﾄﾙ&角度取得
double CalcRotAngle(PNTTYPE*);											// 図形角度取得
double CalcAngle(PNTTYPE*, int, int);									// 3点のなす角度取得
double CalcBaseAngle(PNTTYPE, PNTTYPE, int);							// 基準線角度取得
void CalcLinePoint(int, int, double, double, PNTTYPE*, PNTTYPE*);		// 線分交点算出
double DistPointLine(PNTTYPE*, PNTTYPE*, PNTTYPE*, int*, double*,
                     PNTTYPE*);											// 点<->線分最短距離取得
double BoxCenter(BOXTYPE, PNTTYPE*);									// ﾎﾞｯｸｽｾﾝﾀｰ
BOOL BoxCheck(int, BOXTYPE, PNTTYPE);									// ﾎﾞｯｸｽ内存在確認
void GetTensionFltValue(int, double, double, double*);					// 張力設定値変換(実数)
void GetTensionPntValue(int, double, double, PNTTYPE*);					// 張力設定値変換(頂点)
BOOL CheckNextPoint(PNTTYPE, PNTTYPE);									// 次点有効ﾁｪｯｸ
void ScrToPnt(int, SCRTYPE*, PNTTYPE*);									// SCRTYPE<=>PNTTYPE変換
double CalcVecAngle(VECTYPE*, VECTYPE*);								// 2ﾍﾞｸﾄﾙ間角度算出

//===============================<EOF:NumFunc.h>=============================