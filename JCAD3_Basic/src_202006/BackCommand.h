/////////////////////////////////////////////////////////////////////////////
// 立体編集コマンド逆実行(戻し)部プロトタイプ宣言
// BackCommand.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/14 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void BackMove(int, PNTTYPE*, PNTTYPE*, double, double, double);			// 逆平行移動
void BackRotate(int, PNTTYPE*, PNTTYPE*, PNTTYPE, PNTTYPE, double);		// 逆回転移動
void BackMirror(int, PNTTYPE*, PNTTYPE*, PNTTYPE, PNTTYPE, int);		// 逆鏡像移動
void BackScale(int, PNTTYPE*, PNTTYPE*, PNTTYPE, double, double,
               double);													// 逆ｻｲｽﾞ変更
void BackResize1(int, PNTTYPE*, PNTTYPE*, int, int, double, double,
                 double, PNTTYPE, PNTTYPE, PNTTYPE);					// 逆直線指定ﾘｻｲｽﾞ1
void BackResize2(int, PNTTYPE*, PNTTYPE*, int, int, int, int, double,
                 PNTTYPE, int, PNTTYPE*, int*);							// 逆曲線指定ﾘｻｲｽﾞ1
void BackBend(int, PNTTYPE*, PNTTYPE*, int, int, double, PNTTYPE,
              PNTTYPE);													// 逆円筒曲げ
void BackTwist(int, PNTTYPE*, PNTTYPE*, int, double, double, PNTTYPE);	// 逆ひねり
void BackShear(int, PNTTYPE*, PNTTYPE*, int, int, double, PNTTYPE);		// 逆せん断
void BackResize3(int, PNTTYPE*, PNTTYPE*, int, int,
                 double, double, double, int, double,
                 double, PNTTYPE, PNTTYPE, PNTTYPE);					// 逆直線指定ﾘｻｲｽﾞ2
void BackResize4(int, PNTTYPE*, PNTTYPE*, int, int, int,
                 int, int, double, int, double, double,
                 PNTTYPE, int, PNTTYPE*, int*);							// 逆曲線指定ﾘｻｲｽﾞ2

//============================<EOF:BackCommand.h>============================