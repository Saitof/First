/////////////////////////////////////////////////////////////////////////////
// 立体編集コマンド部プロトタイプ宣言
// ECommand.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void Move(double, double, double);										// 平行移動
void ObjMove(OBJTYPE*, double, double, double);							// 指定立体の移動
void ObjectMove(int, int, PNTTYPE, SCRTYPE, double, double, double);	// 立体平行移動
void EndMove(int, PNTTYPE, int, SCRTYPE, double, double, double);		// 平行移動終了処理
void Rotate(PNTTYPE, VECTYPE, double);									// 回転移動
void ObjRotate(OBJTYPE*, PNTTYPE, VECTYPE, double);						// 指定立体の回転
void ObjectRotate(int, int, PNTTYPE, SCRTYPE, PNTTYPE, int,
                  double, double);										// 立体回転移動
void EndRotate(int, PNTTYPE, int, SCRTYPE, PNTTYPE, int,
               double, double);											// 回転移動終了処理
void ObjectMirror(int, int, PNTTYPE, PNTTYPE, int);						// 立体鏡像移動
void Scale(PNTTYPE, double, double, double);							// ｻｲｽﾞ変更
void ObjectScale(int, PNTTYPE, double, double, double);					// 立体ｻｲｽﾞ変更
void Resize1(int, int, double, double, double, PNTTYPE,
             PNTTYPE, PNTTYPE);											// 直線指定ﾘｻｲｽﾞ1
void ObjectResize1(int, int, int, double, double, double,
                   PNTTYPE, PNTTYPE, PNTTYPE);							// 立体直線指定ﾘｻｲｽﾞ1
void Resize2(int, int, int, int, double, PNTTYPE, int,
             PNTTYPE*, int*);											// 曲線指定ﾘｻｲｽﾞ2
void ObjectResize2(int, int, int, int, int, double, PNTTYPE,
                   int, PNTTYPE*, int*);								// 立体曲線指定ﾘｻｲｽﾞ2
void Bend(int, int, double, PNTTYPE, PNTTYPE);							// 円筒曲げ
void ObjectBend(int, int, int, double, PNTTYPE, PNTTYPE);				// 立体円筒曲げ
void Twist(int, double, double, PNTTYPE);								// ひねり
void ObjectTwist(int, int, double, double, PNTTYPE);					// 立体ひねり
void Shear(int, int, double, PNTTYPE);									// せん断
void ObjectShear(int, int, int, double, PNTTYPE);						// 立体せん断
void Ring(PNTTYPE, double, double, double, int, int, double,
          PNTTYPE, PNTTYPE);											// ﾘﾝｸﾞｻｲｽﾞ合わせ
void Earth(int, int, int, double, double, double, double,
           double, double, double, double, double);						// ｱｰｽ
void ObjectEarth(int, int, int, int, int, double, double, double,
                 double, double, double, double, double, double);		// 立体ｱｰｽ
void Space(int, int, int, double, double, double, double,
           double, double, double, double, double);						// ｽﾍﾟｰｽ
void ObjectSpace(int, int, int, int, int, double, double, double,
                 double, double, double, double, double, double);		// 立体ｽﾍﾟｰｽ
void Resize3(int, int, double, double, double, int,
             double, double, PNTTYPE, PNTTYPE, PNTTYPE);				// 直線指定ﾘｻｲｽﾞ2
void ObjectResize3(int, int, int, double, double, double, int,
                   double, double, PNTTYPE, PNTTYPE, PNTTYPE);			// 立体直線指定ﾘｻｲｽﾞ2
void Resize4(int, int, int, int, int, double, int, double,
			 double, PNTTYPE, int, PNTTYPE*, int*);						// 曲線指定ﾘｻｲｽﾞ2
void ObjectResize4(int, int, int, int, int, int, double, int,
                   double, double, PNTTYPE, int, PNTTYPE*, int*);		// 立体曲線指定ﾘｻｲｽﾞ2

//==============================<EOF:ECommand.h>=============================