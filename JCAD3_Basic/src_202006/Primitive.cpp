/////////////////////////////////////////////////////////////////////////////
// プリミティブ入力部
// Primitive.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/02/20 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include "stdafx.h"
#include "Jcad3Glb.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "InputDlg.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "ECommand.h"
#include "DataFile1.h"
#include "Select.h"
#include "NumFunc.h"

/************************************************************/
static BOOL InputBoxSize(double* xd, double* yd, double* zd)			//<<< ﾎﾞｯｸｽｻｲｽﾞ入力
/************************************************************/
{
    CInputDlg dlg(2);
    dlg.SetValue(*xd, *yd, *zd);                                        // 既存値表示
    if(dlg.DoModal()==IDOK) {                                           // 数値入力ﾀﾞｲｱﾛｸﾞ表示
        dlg.GetValue(xd, yd, zd);                                       // ﾎﾞｯｸｽｻｲｽﾞ取得
        return TRUE;
    } else {
        return FALSE;
    }
}

/**************************/
int ReadPrimitive(int pno)												//<<< ﾌﾟﾘﾐﾃｨﾌﾞ入力
/**************************/
{
	OBJTYPE* op;
	OIFTYPE* oif;
	PNTTYPE  oct, dct;
	BOXTYPE  box;
	VECTYPE  vec;
	CString  fname;
	int      id;
	double   dd, dx, dy, dz=0.0;

    View->CheckCtrlKey1();                                              // 制御ｷｰ確認(起動時)
    if(pno<2) {                                                         // <<2Dﾌﾟﾘﾐﾃｨﾌﾞ>>
        dx = View->GetDispHeight()/2.0;                                 // 画面長半分取得
        dy = View->GetDispHeight()/2.0;
        if(View->GetNumInp()!=0) {                                      // <数値入力あり>
            dx = View->GetPrimXSize2D();                                // 既存値取得
            dy = View->GetPrimYSize2D();
            dz = View->GetPrimZSize2D();
            if(InputBoxSize(&dx, &dy, &dz)) {                           // ﾎﾞｯｸｽｻｲｽﾞ入力
                View->SetPrimXSize2D(dx);                               // 既存値設定
                View->SetPrimYSize2D(dy);
                View->SetPrimZSize2D(dz);
            } else {
                dx = 0.0, dy = 0.0, dz = 0.0;
            }
        }
        if(dx>EPSILON&&dy>EPSILON) {                                    // X,Yが有効はZを無効
            dz = 0.0;
        }
        if(dx*dy<EPSILON&&dy*dz<EPSILON&&dz*dx<EPSILON) {               // 有効な2軸無は無効
            dx = 0.0, dy = 0.0, dz = 0.0;
        }
    } else {                                                            // <<3Dﾌﾟﾘﾐﾃｨﾌﾞ>>
        dx = View->GetDispHeight()/2.0;                                 // 画面長の半分取得
        dy = View->GetDispHeight()/2.0;
        dz = View->GetDispHeight()/2.0;
        if(View->GetNumInp()!=0) {                                      // <数値入力あり>
            dx = View->GetPrimXSize3D();                                // 既存値取得
            dy = View->GetPrimYSize3D();
            dz = View->GetPrimZSize3D();
            if(InputBoxSize(&dx, &dy, &dz)) {                           // ﾎﾞｯｸｽｻｲｽﾞ入力
                View->SetPrimXSize3D(dx);                               // 既存値設定
                View->SetPrimYSize3D(dy);
                View->SetPrimZSize3D(dz);
            } else {
                dx = 0.0, dy = 0.0, dz = 0.0;
            }
        }
        if(dx<EPSILON||dy<EPSILON||dz<EPSILON) {                        // 1軸でも0は全て無効
            dx = 0.0, dy = 0.0, dz = 0.0;
        }
    }
	if(dx>EPSILON||dy>EPSILON||dz>EPSILON) {							// <指定ｷｬﾝｾﾙ以外>
		if(pno==0) {
			fname = GetAppPath(0)+CIR_NAME;								// ﾌｧｲﾙﾊﾟｽ取得(円)
		} else if(pno==1) {
			fname = GetAppPath(0)+SQU_NAME;								//            (長方形)
		} else if(pno==2) {
			fname = GetAppPath(0)+SPH_NAME;								//            (球体)
		} else if(pno==3) {
			fname = GetAppPath(0)+CUB_NAME;								//            (直方体)
		} else if(pno==4) {
			fname = GetAppPath(0)+CYL_NAME;								//            (円柱)
		} else {
			fname = GetAppPath(0)+CON_NAME;								//            (円錐)
		}
		StartMenu();													// ﾒﾆｭｰ開始処理
		InitLevel();													// Undo/Redo初期化
		if((id=ReadJsdFile(fname, 0, 0))!=0) {							// ﾌﾟﾘﾐﾃｨﾌﾞ入力
			return id;
		}
		OffEdtAllObject();												// 編集立体全解除
		GetObjPtr(&op);													// 立体ﾎﾟｲﾝﾀ取得
		GetObjOif(op, &oif);											// 立体構成情報ﾎﾟｲﾝﾀ取得
		if(pno==0) {
			SetOifName(1, oif, OBJ_NAME13);								// 立体構成名称(円)
		} else if(pno==1) {
			SetOifName(1, oif, OBJ_NAME14);								//             (長方形)
		} else if(pno==2) {
			SetOifName(1, oif, OBJ_NAME15);								//             (球体)
		} else if(pno==3) {
			SetOifName(1, oif, OBJ_NAME16);								//             (直方体)
		} else if(pno==4) {
			SetOifName(1, oif, OBJ_NAME17);								//             (円柱)
		} else {
			SetOifName(1, oif, OBJ_NAME18);								//             (円錐)
		}
		OnEdtFlg(1, op);												// 編集立体設定
		GetObjBox(op, &box);											// ﾎﾞｯｸｽｻｲｽﾞ取得
		dd = BoxCenter(box, &oct);										// ﾎﾞｯｸｽ中心&長さ取得
		View->GetCentPnt(&dct);											// 画面中心取得
		OnCmdFlgStp();													// 未保存ﾌﾗｸﾞ更新中止ON
		if(dx<EPSILON) {												// <2DがY-Z指定の場合>
			vec.p[0] = 0.0, vec.p[1] = 1.0, vec.p[2] = 0.0;				// 回転軸はY軸
			Rotate(oct, vec, 90.0);										// 90度回転
			dx = dd;
		} else if(dy<EPSILON) {											// <2DがZ-X指定の場合>
			vec.p[0] = 1.0, vec.p[1] = 0.0, vec.p[2] = 0.0;				// 回転軸はX軸
			Rotate(oct, vec, 90.0);										// 90度回転
			dy = dd;
		} else if(dz<EPSILON) {											// <2DがX-Y指定の場合>
			dz = dd;
		}
		dx = dx/dd, dy = dy/dd, dz = dz/dd;								// X/Y/Z方向倍率算出
		if(fabs(dx-1.0)>EPSILON||
           fabs(dy-1.0)>EPSILON||
           fabs(dz-1.0)>EPSILON) {										// <拡大/縮小が必要な場合>
			Scale(oct, dx, dy, dz);										// 画面ｻｲｽﾞ半分にﾘｻｲｽﾞ
		}
		dx = dct.p[0] - oct.p[0];										// X方向移動量算出
		dy = dct.p[1] - oct.p[1];										// Y方向移動量算出
		dz = dct.p[2] - oct.p[2];										// Z方向移動量算出
		if(fabs(dx)>EPSILON||fabs(dy)>EPSILON||fabs(dz)>EPSILON) {		// <XYZの移動が必要な場合>
			Move(dx, dy, dz);											// 画面中心へ移動
		}
		OffCmdFlgStp();													// 未保存ﾌﾗｸﾞ更新中止OFF
		EndJMenu1();													// ﾒﾆｭｰ終了処理
		View->MenuChgSurfParam(1);										// 曲線ﾊﾟﾗﾒｰﾀ変更
	}
	return 0;
}
