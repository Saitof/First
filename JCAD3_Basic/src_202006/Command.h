/////////////////////////////////////////////////////////////////////////////
// コマンド起動制御部プロトタイプ宣言
// Command.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void     ExecCommand(BOOL);												// ｺﾏﾝﾄﾞ実行
OBJTYPE* MakeSurface(void);												// 曲面生成
OBJTYPE* MakeDirect(void);												// 直接生成
OBJTYPE* MakeCurve(void);												// 曲線生成
OBJTYPE* ChangeSurface(void);											// 曲面編集
OBJTYPE* ChangeCurve(void);												// 曲線編集
OBJTYPE* SetSurface(void);												// 曲面制御点生成
OBJTYPE* SetCurve(void);												// 曲線制御点生成
OBJTYPE* ThicknessObject(void);											// 曲面厚みづけ
OBJTYPE* MoveObject(void);												// 平行移動
OBJTYPE* RotateObject(void);											// 回転回転
OBJTYPE* MirrorObject(void);											// 鏡像移動
OBJTYPE* ScaleObject(void);												// ｻｲｽﾞ変更
OBJTYPE* BendObject(void);												// 円筒曲げ
OBJTYPE* SetGroup(void);												// ｸﾞﾙｰﾌﾟ情報設定
OBJTYPE* SetColor(void);												// ｶﾗｰNo設定
OBJTYPE* SetAttribute(void);											// 属性No設定
OBJTYPE* MakeSweepSurf(void);											// 掃引曲面生成
OBJTYPE* ChangeSweepSurf(void);											// 掃引曲面編集
OBJTYPE* TwistObject(void);												// ひねり
OBJTYPE* ResizeObject1(void);											// 直線指定ﾘｻｲｽﾞ1
OBJTYPE* SetParts(void);												// ﾊﾟｰﾂNo設定
OBJTYPE* ResizeObject2(void);											// 曲線指定ﾘｻｲｽﾞ1
OBJTYPE* ShearObject(void);												// せん断
OBJTYPE* RingObject(void);												// ﾘﾝｸﾞ
OBJTYPE* EarthObject(void);												// ｱｰｽ
OBJTYPE* SpaceObject(void);												// ｽﾍﾟｰｽ
OBJTYPE* ResizeObject3(void);											// 直線指定ﾘｻｲｽﾞ2
OBJTYPE* ResizeObject4(void);											// 曲線指定ﾘｻｲｽﾞ2
OBJTYPE* CommandError(void);											// ｺﾏﾝﾄﾞｴﾗｰ
void     RedoCommand(OBJTYPE*);											// ｺﾏﾝﾄﾞ再実行
void     RedoStartCommand(OBJTYPE*);									// ﾒﾆｭｰ開始時ｺﾏﾝﾄﾞ再実行
void     RedoCreateCommand(OBJTYPE*);									// 生成ｺﾏﾝﾄﾞ再実行
void     RedoRingBeforeCommand(OBJTYPE* op);							// ﾘﾝｸﾞｻｲｽﾞ前ｺﾏﾝﾄﾞ再実行
void     RedoCommandCopy(OBJTYPE*, CMDTYPE*);							// ｺﾏﾝﾄﾞ複写実行

//==============================<EOF:Command.h>==============================