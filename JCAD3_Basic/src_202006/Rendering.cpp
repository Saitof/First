/////////////////////////////////////////////////////////////////////////////
// レンダリング(RayTracing)部
// Rendering.cpp
//---------------------------------------------------------------------------
// LastEdit : 2009/04/01 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "stdafx.h"
#include "MainFrm.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "DataAccess3.h"
#include "NumFunc.h"
#include "RendType.h"
#include "RendDlg.h"

/*********************************************************/
static void CalcMatrix(VECTYPE* ax, double th, RMAT* rot)               //<<< 回転ﾏﾄﾘｯｸｽ計算
/*********************************************************/
{
    double ct, st, ss;

    ct = cos(th), st = sin(th), ss = 1.0 - ct;
    rot->m2[0][0] = ct + ax->p[0]*ax->p[0]*ss;
    rot->m2[0][1] = -(ax->p[2]*st) + ax->p[0]*ax->p[1]*ss;
    rot->m2[0][2] = ax->p[1]*st + ax->p[0]*ax->p[2]*ss;
    rot->m2[1][0] = ax->p[2]*st + ax->p[0]*ax->p[1]*ss;
    rot->m2[1][1] = ct + ax->p[1]*ax->p[1]*ss;
    rot->m2[1][2] = -(ax->p[0]*st) + ax->p[1]*ax->p[2]*ss;
    rot->m2[2][0] = -(ax->p[1]*st) + ax->p[0]*ax->p[2]*ss;
    rot->m2[2][1] = ax->p[0]*st + ax->p[1]*ax->p[2]*ss;
    rot->m2[2][2] = ct + ax->p[2]*ax->p[2]*ss;
}

/****************************************************/
static void MakeRotateMatrix(int flg, VECTYPE* eye,
                             RMAT* matz, RMAT* matx)                    //<<< 回転ﾏﾄﾘｯｸｽ作成
/****************************************************/
{
    VECTYPE vec, ax;
    int     i, j;
    double  th;

    for(i=0; i<DIM; i++) {
        vec.p[i] = -eye->p[i];
    }
    if(fabs(vec.p[0])<EPS_R&&fabs(vec.p[1])<EPS_R) {                    // Z軸回転角度計算
        th = PI/2.0;
    } else {
        th = atan2(vec.p[1], vec.p[0]) + PI/2.0;
    }
    if(flg) {
        th = -th;                                                       // flg:ONは戻し
    }
    if(fabs(th)>EPS_R) {
        ax.p[0] = 0.0, ax.p[1] = 0.0, ax.p[2] = -1.0;
        CalcMatrix(&ax, th, matz);                                      // 回転ﾏﾄﾘｯｸｽ計算
    } else {
        for(i=0; i<DIM; i++) {
            for(j=0; j<DIM; j++) {
                matz->m2[i][j] = 0.0;
            }
        }
        matz->m2[0][0] = 1.0, matz->m2[1][1] = 1.0, matz->m2[2][2] = 1.0;
    }
    th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);                     // X軸回転角度計算
    if(th<EPS_R&&fabs(vec.p[2])<EPS_R) {
        th = 0.0;
    } else {
        th = atan2(th, vec.p[2]);
    }
    if(flg) {
        th = -th;                                                       // flg:ONは戻し
    }
    if(fabs(th)>EPS_R) {
        ax.p[0] = -1.0, ax.p[1] = 0.0, ax.p[2] = 0.0;
        CalcMatrix(&ax, th, matx);                                      // 回転ﾏﾄﾘｯｸｽ計算
    } else {
        for(i=0; i<DIM; i++) {
            for (j=0; j<DIM; j++) {
                matx->m2[i][j] = 0.0;
            }
        }
        matx->m2[0][0] = 1.0, matx->m2[1][1] = 1.0, matx->m2[2][2] = 1.0;
    }
}

/***********************************************/
static void MoveVertex(PNTTYPE* pnt, RMAT* rot)                         //<<< 頂点回転
/***********************************************/
{
    int    i, j;
    double wk, v[DIM];

    for(i=0; i<DIM; i++) {
        for(j=0, wk=0.0; j<DIM; j++) {
            wk += rot->m2[i][j]*pnt->p[j];
        }
        v[i] = wk;
    }
    for(i=0; i<DIM; i++) {
        pnt->p[i] = v[i];
    }
}

/***********************************************/
static void MoveVector(VECTYPE* vec, RMAT* rot)                         //<<< ﾍﾞｸﾄﾙ回転
/***********************************************/
{
    int    i, j;
    double wk, v[DIM];

    for(i=0; i<DIM; i++) {
        for(j=0, wk=0.0; j<DIM; j++) {
            wk += rot->m2[i][j]*vec->p[j];
        }
        v[i] = wk;
    }
    for(i=0; i<DIM; i++) {
        vec->p[i] = v[i];
    }
}

/**************************/
static void InitFVec(void)                                              //<<< ﾍﾞｸﾄﾙ設定初期処理
/**************************/
{
    Xmin_f = DBL_MAX,  Ymin_f = DBL_MAX,  Zmin_f = DBL_MAX;
    Xmin_o = DBL_MAX,  Ymin_o = DBL_MAX,  Zmin_o = DBL_MAX;
    Xmax_f = -DBL_MAX, Ymax_f = -DBL_MAX, Zmax_f = -DBL_MAX;
    Xmax_o = -DBL_MAX, Ymax_o = -DBL_MAX, Zmax_o = -DBL_MAX;
    Lfst = NULL, Llst = NULL, Ffst = NULL, Flst = NULL;
}

/******************************************/
static void SetLptype(int vno, PNTTYPE pt)                              //<<< 面ﾙｰﾌﾟ情報設定
/******************************************/
{
    LPTYPE* lpt;

    if((lpt=(LPTYPE *)malloc(sizeof(LPTYPE)))==NULL) {
        MemErr(IDS_MEMERR5);
    }
    if(Llst==NULL) {
        Lfst = lpt;
    } else {
        Llst->next = lpt;
    }
    lpt->vno  = Vtnum+vno;
    lpt->next = NULL;
    Llst = lpt;
    if(Xmin_f>pt.p[0]) {                                                // 面の最小･最大取得
        Xmin_f = pt.p[0];
    }
    if(Xmax_f<pt.p[0]) {
        Xmax_f = pt.p[0];
    }
    if(Ymin_f>pt.p[1]) {
        Ymin_f = pt.p[1];
    }
    if(Ymax_f<pt.p[1]) {
        Ymax_f = pt.p[1];
    }
    if(Zmin_f>pt.p[2]) {
        Zmin_f = pt.p[2];
    }
    if(Zmax_f<pt.p[2]) {
        Zmax_f = pt.p[2];
    }
}

/**********************************/
static void SetFatype(VECTYPE nvc)                                      //<<< 面情報設定
/**********************************/
{
    FATYPE* fat;
    LPTYPE* lpt;
    PNTTYPE cp;
    double  r, rmax;

    cp.p[0] = (Xmax_f+Xmin_f)/2.0;                                      // 面中心点
    cp.p[1] = (Ymax_f+Ymin_f)/2.0;
    cp.p[2] = (Zmax_f+Zmin_f)/2.0;
    lpt = Lfst, rmax = 0.0;
    while(lpt!=NULL) {
        r = PntD(&cp, &Pdat[lpt->vno]);                                 // 2点間距離
        if(r>rmax) {
            rmax = r;                                                   // 最大半径取得
        }
        lpt = lpt->next;
    }
    if((fat=(FATYPE *)malloc(sizeof(FATYPE)))==NULL) {
        MemErr(IDS_MEMERR5);
    }
    if(Flst==NULL) {
        Ffst = fat;
    } else {
        Flst->next = fat;
    }
    fat->cpt = cp;
    fat->r = rmax;
    fat->vec = nvc;
    fat->lpl = Lfst;
    fat->next = NULL;
    Flst = fat;
    if(Xmin_o>Xmin_f) {                                                 // 立体の最小･最大取得
        Xmin_o = Xmin_f;
    }
    if(Xmax_o<Xmax_f) {
        Xmax_o = Xmax_f;
    }
    if(Ymin_o>Ymin_f) {
        Ymin_o = Ymin_f;
    }
    if(Ymax_o<Ymax_f) {
        Ymax_o = Ymax_f;
    }
    if(Zmin_o>Zmin_f) {
        Zmin_o = Zmin_f;
    }
    if(Zmax_o<Zmax_f) {
        Zmax_o = Zmax_f;
    }
    Xmin_f = DBL_MAX,  Ymin_f = DBL_MAX,  Zmin_f = DBL_MAX;             // 変数初期化
    Xmax_f = -DBL_MAX, Ymax_f = -DBL_MAX, Zmax_f = -DBL_MAX;
    Lfst = NULL, Llst = NULL;
}

/***********************************************/
static void SetObtype(OBJTYPE* op, VECTYPE* lv)                         //<<< 立体情報設定
/***********************************************/
{
    OBTYPE* obt;
    FATYPE* fat, * wfa, * fst[8], * lst[8];
    LPTYPE* lpt;
    PNTTYPE cp, cp8[8];
    int     i, cnt;
    double  r, rmax, rmax8[8];

    fat = Ffst, cnt = 0;
    while(fat!=NULL) {
        lpt = fat->lpl;
        while(lpt!=NULL) {                                              // 面ﾙｰﾌﾟ毎に法線ﾍﾞｸﾄﾙ設定
            lpt->vec = lv[cnt++];
            lpt = lpt->next;
        }
        fat = fat->next;
    }
    cp.p[0] = (Xmax_o+Xmin_o)/2.0;                                      // 立体中心点
    cp.p[1] = (Ymax_o+Ymin_o)/2.0;
    cp.p[2] = (Zmax_o+Zmin_o)/2.0;
    fat = Ffst, rmax = 0.0;
    while(fat!=NULL) {
        lpt = fat->lpl;
        while(lpt!=NULL) {
            r = PntD(&cp, &Pdat[lpt->vno]);                             // 2点間距離
            if(r>rmax) {
                rmax = r;                                               // 最大半径取得
            }
            lpt = lpt->next;
        }
        fat = fat->next;
    }
    cp8[0].p[0] = (Xmax_o+cp.p[0])/2.0;                                 // 8分割毎の中心点
    cp8[0].p[1] = (Ymax_o+cp.p[1])/2.0;
    cp8[0].p[2] = (Zmax_o+cp.p[2])/2.0;
    cp8[1].p[0] = (Xmax_o+cp.p[0])/2.0;
    cp8[1].p[1] = (cp.p[1]+Ymin_o)/2.0;
    cp8[1].p[2] = (Zmax_o+cp.p[2])/2.0;
    cp8[2].p[0] = (cp.p[0]+Xmin_o)/2.0;
    cp8[2].p[1] = (Ymax_o+cp.p[1])/2.0;
    cp8[2].p[2] = (Zmax_o+cp.p[2])/2.0;
    cp8[3].p[0] = (cp.p[0]+Xmin_o)/2.0;
    cp8[3].p[1] = (cp.p[1]+Ymin_o)/2.0;
    cp8[3].p[2] = (Zmax_o+cp.p[2])/2.0;
    cp8[4].p[0] = (Xmax_o+cp.p[0])/2.0;
    cp8[4].p[1] = (Ymax_o+cp.p[1])/2.0;
    cp8[4].p[2] = (cp.p[2]+Zmin_o)/2.0;
    cp8[5].p[0] = (Xmax_o+cp.p[0])/2.0;
    cp8[5].p[1] = (cp.p[1]+Ymin_o)/2.0;
    cp8[5].p[2] = (cp.p[2]+Zmin_o)/2.0;
    cp8[6].p[0] = (cp.p[0]+Xmin_o)/2.0;
    cp8[6].p[1] = (Ymax_o+cp.p[1])/2.0;
    cp8[6].p[2] = (cp.p[2]+Zmin_o)/2.0;
    cp8[7].p[0] = (cp.p[0]+Xmin_o)/2.0;
    cp8[7].p[1] = (cp.p[1]+Ymin_o)/2.0;
    cp8[7].p[2] = (cp.p[2]+Zmin_o)/2.0;
    for(i=0; i<8; i++) {
        fst[i] = NULL, lst[i] = NULL;
    }
    fat = Ffst;
    while(fat!=NULL) {                                                  // 8分割毎に面情報を振分
        wfa = fat, fat = fat->next;
        i = (wfa->cpt.p[2]>=cp.p[2]) ? 0 : 4;
        if(wfa->cpt.p[0]>=cp.p[0]) {
            i += (wfa->cpt.p[1]>=cp.p[1]) ? 0 : 1;
        } else {
            i += (wfa->cpt.p[1]>=cp.p[1]) ? 2 : 3;
        }
        wfa->next = NULL;
        if(lst[i]==NULL) {
            fst[i] = wfa;
        } else {
            lst[i]->next = wfa;
        }
        lst[i] = wfa;
    }
    for(i=0; i<8; i++) {
        fat = fst[i], rmax8[i] = 0.0;
        while(fat!=NULL) {
            lpt = fat->lpl;
            while(lpt!=NULL) {
                r = PntD(&cp8[i], &Pdat[lpt->vno]);                     // 2点間距離
                if(r>rmax8[i]) {
                    rmax8[i] = r;                                       // 最大半径取得
                }
                lpt = lpt->next;
            }
            fat = fat->next;
        }
    }
    if((obt=(OBTYPE *)malloc(sizeof(OBTYPE)))==NULL) {
        MemErr(IDS_MEMERR5);
    }
    obt->ano = GetObjCol(op);                                           // ｶﾗｰ(表面属性)
    obt->cpt = cp;
    obt->r = rmax;
    for(i=0; i<8; i++) {
        obt->cp8[i] = cp8[i];
        obt->r8[i]  = rmax8[i];
        obt->fal[i] = fst[i];
    }
    obt->next = NULL;
    if(Olist==NULL) {
        Olist = obt;
    } else {
        Olast->next = obt;
    }
    Olast = obt;
}

/********************************************************/
static void SetObjFVec1(OBJTYPE* op, int vnum, int lnum)                //<<< 曲面法線ﾍﾞｸﾄﾙ設定
/********************************************************/              //   (曲面：稜線角)
{
    PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
    VECTYPE* pv = new VECTYPE[View->m_MaxSPnt];
    VECTYPE* av = new VECTYPE[View->m_MaxSPnt];
    VECTYPE* wv = new VECTYPE[View->m_MaxCPnt];
    VECTYPE* lv = new VECTYPE[View->m_MaxSPnt*4+View->m_MaxCPnt*2];
    int*     fg = new int[View->m_MaxSPnt];
    int*     ln = new int[View->m_MaxSPnt];
    int*     nx = new int[View->m_MaxSPnt];
    int*     wf = new int[View->m_MaxCPnt];
    int*     pn = new int[View->m_MaxCPnt];
    int*     lp = new int[View->m_MaxCPnt];
    VECTYPE  v1, v2, v3;
    int      i, j, k, l, cnt=0, an=0, stp=0, vno, flg;
    double   th;

    InitFVec();                                                         // ﾍﾞｸﾄﾙ設定初期処理
    for(i=0; i<vnum; i++) {
        fg[i] = -2;                                                     // ﾌﾗｸﾞ初期化
    }
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // 面ﾙｰﾌﾟ番号取得
        pa[cnt] = Pdat[Vtnum+vno];                                      // 頂点座標保存
        lp[cnt] = i;                                                    // ﾙｰﾌﾟ番号保存
        pn[cnt++] = vno;                                                // 頂点番号保存
        SetLptype(vno, pa[cnt-1]);                                      // 面ﾙｰﾌﾟ情報設定
        if(flg==1) {                                                    // <<<ﾙｰﾌﾟ最終の場合>>>
            CalcNVec(pa, cnt, &v1);                                     // 法線ﾍﾞｸﾄﾙ取得
            SetFatype(v1);                                              // 面情報設定
            for(j=0; j<cnt; j++) {
                if(fg[pn[j]]==-2) {                                     // <<初期の場合>>
                    pv[pn[j]] = v1;                                     // 対象頂点ﾍﾞｸﾄﾙ設定
                    fg[pn[j]] = -1;                                     // ﾌﾗｸﾞ更新
                } else {                                                // <<初期以外の場合>>
                    v2 = pv[pn[j]];                                     // 登録済ﾍﾞｸﾄﾙ
                    VecN(&v2);                                          // 単位ﾍﾞｸﾄﾙ化
                    if(fabs(v1.p[0]-v2.p[0])>EPS_R||                    // 2ﾍﾞｸﾄﾙ間角度
                       fabs(v1.p[1]-v2.p[1])>EPS_R||
                       fabs(v1.p[2]-v2.p[2])>EPS_R) {
                        th = acos(v1.p[0]*v2.p[0]+v1.p[1]*v2.p[1]+v1.p[2]*v2.p[2]);
						th = (th<-10||th>10) ? 0.0 : th;
                    } else {
                        th = 0.0;
                    }
                    if(fabs(th)<2.0*PI/9.0) {                           // <40度未満(曲線)>
                        for(k=0; k<DIM; k++) { 
                            pv[pn[j]].p[k] += v1.p[k];                  // 対象頂点ﾍﾞｸﾄﾙ加算
                        }
                    } else {                                            // <40度以上(折線)>
                        if(fg[pn[j]]==-1) {                             // <最初の折線>
                            if(an>=View->m_MaxSPnt) {                   // ﾃﾞｰﾀﾌﾙ中止
                                stp = 1; break;
                            }
                            fg[pn[j]] = an;                             // 折線配列番号設定
                            av[an]    = v1;                             // 対象頂点ﾍﾞｸﾄﾙ設定
                            ln[an]    = lp[j];                          // 面ﾙｰﾌﾟ番号設定
                            nx[an++]  = -1;                             // 次ﾃﾞｰﾀ番号初期化
                        } else {                                        // <2件目以降の折線>
                            if(an>=View->m_MaxSPnt) {                   // ﾃﾞｰﾀﾌﾙ中止
                                stp = 1; break;
                            }
                            k = fg[pn[j]];
                            while(1) {
                                if(nx[k]==-1) {                         // 最終折線配列番号検索
                                    break;
                                }
                                k = nx[k];
                            }
                            nx[k]    = an;                              // 折線配列番号設定
                            av[an]   = v1;                              // 対象頂点ﾍﾞｸﾄﾙ設定
                            ln[an]   = lp[j];                           // 面ﾙｰﾌﾟ番号設定
                            nx[an++] = -1;                              // 次ﾃﾞｰﾀ番号初期化
                        }
                    }
                }
            }
            if(stp) {                                                   // ﾃﾞｰﾀﾌﾙ中止
                break;
            }
            cnt = 0;
        }
    }
    if(!stp) {                                                          // <ﾃﾞｰﾀOKの場合>
        for(i=0; i<vnum; i++) {
            VecN(&pv[i]);                                               // 単位ﾍﾞｸﾄﾙ化
            if(fg[i]>=0) {                                              // <折線有の場合>
                cnt = 0;
                j = fg[i];
                while(1) {
                    wv[cnt]   = av[j];                                  // 折線ﾍﾞｸﾄﾙ複写
                    wf[cnt++] = 0;                                      // 処理ﾌﾗｸﾞ初期化
                    if(nx[j]==-1) {
                        break;
                    }
                    j = nx[j];
                }
                for(j=0; j<cnt; j++) {
                    if(wf[j]==0) {                                      // <未処理ﾍﾞｸﾄﾙ>
                        v1 = wv[j];
                        for(k=j+1; k<cnt; k++) {
                            v2 = v1;
                            VecN(&v2);                                  // 単位ﾍﾞｸﾄﾙ化
                            v3 = wv[k];
                            if(fabs(v2.p[0]-v3.p[0])>EPS_R||            // 2ﾍﾞｸﾄﾙ間角度
                               fabs(v2.p[1]-v3.p[1])>EPS_R||
                               fabs(v2.p[2]-v3.p[2])>EPS_R) {
                                th = acos(v2.p[0]*v3.p[0]+v2.p[1]*v3.p[1]+v2.p[2]*v3.p[2]);
								th = (th<-10||th>10) ? 0.0 : th;
                            } else {
                                th = 0.0;
                            }
                            if(fabs(th)<2.0*PI/9.0) {                   // <40度未満(曲線)>
                                for(l=0; l<DIM; l++) {
                                    v1.p[l] += v3.p[l];                 // 対象頂点ﾍﾞｸﾄﾙ加算
                                }
                                wf[k] = 1;                              // 処理途中
                            }
                        }
                        VecN(&v1);                                      // 単位ﾍﾞｸﾄﾙ化
                        wv[j] = v1;                                     // 許容範囲内集計済
                        wf[j] = 2;                                      // 処理済
                        for(k=j+1; k<cnt; k++) {
                            if(wf[k]==1) {                              // <処理途中の場合>
                                wv[k] = v1;                             // 許容範囲内集計済
                                wf[k] = 2;                              // 処理済
                            }
                        }
                    }
                }
                cnt = 0, j = fg[i];
                while(1) {
                    av[j] = wv[cnt++];                                  // 折線ﾍﾞｸﾄﾙ復元
                    if(nx[j]==-1) {
                        break;
                    }
                    j = nx[j];
                }
            }
        }
        for(i=0; i<lnum; i++) {
            GetFlp1(op, i, &vno, &flg);                                 // 面ﾙｰﾌﾟ番号取得
            if(fg[vno]<0) {                                             // <全ﾙｰﾌﾟ集計>
                lv[i] = pv[vno];                                        // 面ﾙｰﾌﾟ法線ﾍﾞｸﾄﾙ
            } else {                                                    // <上記以外>
                v1 = pv[vno], j = fg[vno];
                while(1) {
                    if(i==ln[j]) {                                      // <ﾙｰﾌﾟ番号一致>
                        v1 = av[j]; break;
                    }
                    if(nx[j]==-1) {                                     // ﾍﾞｸﾄﾙ置換
                        break;
                    }
                    j = nx[j];
                }
                lv[i] = v1;                                             // 面ﾙｰﾌﾟ法線ﾍﾞｸﾄﾙ
            }
        }
        SetObtype(op, lv);                                              // 立体情報設定
    }
    delete[] pa;
    delete[] pv;
    delete[] av;
    delete[] wv;
    delete[] lv;
    delete[] fg;
    delete[] ln;
    delete[] nx;
    delete[] wf;
    delete[] pn;
    delete[] lp;
}

/********************************************************/
static void SetObjFVec2(OBJTYPE* op, int vnum, int lnum)                //<<< 曲面法線ﾍﾞｸﾄﾙ設定
/********************************************************/              //   (曲面：稜線丸)
{
    PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
    VECTYPE* va = new VECTYPE[View->m_MaxSPnt];
    VECTYPE* lv = new VECTYPE[View->m_MaxSPnt*4+View->m_MaxCPnt*2];
    int*     pn = new int[View->m_MaxCPnt];
    VECTYPE  vc;
    int      i, j, k, cnt=0, vno, flg;

    InitFVec();                                                         // ﾍﾞｸﾄﾙ設定初期処理
    for(i=0; i<DIM; i++) {
        vc.p[i] = 0.0;                                                  // ﾍﾞｸﾄﾙ初期化
    }
    for(i=0; i<vnum; i++) {
        va[i] = vc;
    }
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // 面ﾙｰﾌﾟ番号取得
        pa[cnt] = Pdat[Vtnum+vno];                                      // 頂点座標保存
        pn[cnt++] = vno;                                                // 頂点番号保存
        SetLptype(vno, pa[cnt-1]);                                      // 面ﾙｰﾌﾟ情報設定
        if(flg==1) {                                                    // <<<ﾙｰﾌﾟ最終の場合>>>
            CalcNVec(pa, cnt, &vc);                                     // 法線ﾍﾞｸﾄﾙ取得
            SetFatype(vc);                                              // 面情報設定
            for(j=0; j<cnt; j++) {
                for(k=0; k<DIM; k++) { 
                    va[pn[j]].p[k] += vc.p[k];                          // 対象頂点ﾍﾞｸﾄﾙ加算
                }
            }
            cnt = 0;
        }
    }
    for(i=0; i<vnum; i++) {
        VecN(&va[i]);                                                   // 単位ﾍﾞｸﾄﾙ化
    }
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // 面ﾙｰﾌﾟ番号取得
        lv[i] = va[vno];                                                // 面ﾙｰﾌﾟ法線ﾍﾞｸﾄﾙ
    }
    SetObtype(op, lv);                                                  // 立体情報設定
    delete[] pa;
    delete[] va;
    delete[] lv;
    delete[] pn;
}

/**********************************************/
static void SetObjFVec3(OBJTYPE* op, int lnum)                          //<<< 面法線ﾍﾞｸﾄﾙ設定
/**********************************************/                        //   (宝石ｶｯﾄ用)
{
    PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
    VECTYPE* lv = new VECTYPE[View->m_MaxSPnt*4+View->m_MaxCPnt*2];
    VECTYPE  nvc;
    int      i, j, cnt=0, stn=0, vno, flg;

    InitFVec();                                                         // ﾍﾞｸﾄﾙ設定初期処理
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // 面ﾙｰﾌﾟ番号取得
        pa[cnt++] = Pdat[Vtnum+vno];                                    // 頂点座標保存
        SetLptype(vno, pa[cnt-1]);                                      // 面ﾙｰﾌﾟ情報設定
        if(flg==1) {                                                    // <<<ﾙｰﾌﾟ最終の場合>>>
            CalcNVec(pa, cnt, &nvc);                                    // 法線ﾍﾞｸﾄﾙ取得
            SetFatype(nvc);                                             // 面情報設定
            for(j=0; j<cnt; j++) {
                lv[stn+j] = nvc;                                        // 面ﾙｰﾌﾟ単位に設定
            }
            stn = i+1, cnt = 0;                                         // 次ﾙｰﾌﾟ開始番号
        }
    }
    SetObtype(op, lv);                                                  // 立体情報設定
    delete[] pa;
    delete[] lv;
}

/*************************************/
static void SetStageFVec(PNTTYPE* pa)                                   //<<< ｽﾃｰｼﾞ法線ﾍﾞｸﾄﾙ設定
/*************************************/
{
    VECTYPE* lv = new VECTYPE[24];
    OBJTYPE  op;
    PNTTYPE  pt[4];
    VECTYPE  nvc;
    int      i;

    op.color = 12;                                                      // ｶﾗｰ(表面属性):ｽﾃｰｼﾞ
    for(i=0; i<8; i++) {
        Pdat[Vtnum+i] = pa[i];                                          // 頂点登録
    }
    InitFVec();                                                         // ﾍﾞｸﾄﾙ設定初期処理
    SetLptype(0, pa[0]), pt[0] = pa[0];                                 // 面ﾙｰﾌﾟ情報設定
    SetLptype(1, pa[1]), pt[1] = pa[1];
    SetLptype(2, pa[2]), pt[2] = pa[2];
    SetLptype(3, pa[3]), pt[3] = pa[3];
    CalcNVec(pt, 4, &nvc);                                              // 法線ﾍﾞｸﾄﾙ取得
    SetFatype(nvc);                                                     // 面情報設定
    for(i=0; i<4; i++) {
        lv[i+0] = nvc;                                                  // 面ﾙｰﾌﾟ単位に設定
    }
    SetLptype(4, pa[4]), pt[0] = pa[4];                                 // 面ﾙｰﾌﾟ情報設定
    SetLptype(7, pa[7]), pt[1] = pa[7];
    SetLptype(6, pa[6]), pt[2] = pa[6];
    SetLptype(5, pa[5]), pt[3] = pa[5];
    CalcNVec(pt, 4, &nvc);                                              // 法線ﾍﾞｸﾄﾙ取得
    SetFatype(nvc);                                                     // 面情報設定
    for(i=0; i<4; i++) {
        lv[i+4] = nvc;                                                  // 面ﾙｰﾌﾟ単位に設定
    }
    SetLptype(0, pa[0]), pt[0] = pa[0];                                 // 面ﾙｰﾌﾟ情報設定
    SetLptype(3, pa[3]), pt[1] = pa[3];
    SetLptype(7, pa[7]), pt[2] = pa[7];
    SetLptype(4, pa[4]), pt[3] = pa[4];
    CalcNVec(pt, 4, &nvc);                                              // 法線ﾍﾞｸﾄﾙ取得
    SetFatype(nvc);                                                     // 面情報設定
    for(i=0; i<4; i++) {
        lv[i+8] = nvc;                                                  // 面ﾙｰﾌﾟ単位に設定
    }
    SetLptype(1, pa[1]), pt[0] = pa[1];                                  // 面ﾙｰﾌﾟ情報設定
    SetLptype(5, pa[5]), pt[1] = pa[5];
    SetLptype(6, pa[6]), pt[2] = pa[6];
    SetLptype(2, pa[2]), pt[3] = pa[2];
    CalcNVec(pt, 4, &nvc);                                              // 法線ﾍﾞｸﾄﾙ取得
    SetFatype(nvc);                                                     // 面情報設定
    for(i=0; i<4; i++) {
        lv[i+12] = nvc;                                                 // 面ﾙｰﾌﾟ単位に設定
    }
    SetLptype(0, pa[0]), pt[0] = pa[0];                                 // 面ﾙｰﾌﾟ情報設定
    SetLptype(4, pa[4]), pt[1] = pa[4];
    SetLptype(5, pa[5]), pt[2] = pa[5];
    SetLptype(1, pa[1]), pt[3] = pa[1];
    CalcNVec(pt, 4, &nvc);                                              // 法線ﾍﾞｸﾄﾙ取得
    SetFatype(nvc);                                                     // 面情報設定
    for(i=0; i<4; i++) {
        lv[i+16] = nvc;                                                 // 面ﾙｰﾌﾟ単位に設定
    }
    SetLptype(3, pa[3]), pt[0] = pa[3];                                 // 面ﾙｰﾌﾟ情報設定
    SetLptype(2, pa[2]), pt[1] = pa[2];
    SetLptype(6, pa[6]), pt[2] = pa[6];
    SetLptype(7, pa[7]), pt[3] = pa[7];
    CalcNVec(pt, 4, &nvc);                                              // 法線ﾍﾞｸﾄﾙ取得
    SetFatype(nvc);                                                     // 面情報設定
    for(i=0; i<4; i++) {
        lv[i+20] = nvc;                                                 // 面ﾙｰﾌﾟ単位に設定
    }
    SetObtype(&op, lv);                                                 // 立体情報設定
    Vtnum += 8;
    delete[] lv;
}

/***************************/
static void DoScaling(void)                                             //<<< 表示ｳｨﾝﾄﾞｳ内ｽｹｰﾘﾝｸﾞ
/***************************/
{
    OBTYPE* obt;
    FATYPE* fat;
    int     i, j;

    for(i=0; i<Vtnum; i++) {
        for(j=0; j<DIM; j++) {
            Pdat[i].p[j] *= Scale;                                      // 頂点座標をｽｹｰﾘﾝｸﾞ
        }
    }
    obt = Olist;
    while(obt!=NULL) {
        for(i=0; i<DIM; i++) {
            obt->cpt.p[i] *= Scale;                                     // 立体中心点をｽｹｰﾘﾝｸﾞ
        }
        obt->r *= Scale;                                                // 立体最小球半径をｽｹｰﾘﾝｸﾞ
        for(i=0; i<8; i++) {
            for(j=0; j<DIM; j++) {
                obt->cp8[i].p[j] *= Scale;                              // 8分割中心点をｽｹｰﾘﾝｸﾞ
            }
            obt->r8[i] *= Scale;                                        // 8分割最小球半径をｽｹｰﾘﾝｸﾞ
            fat = obt->fal[i];
            while(fat!=NULL) {
                for(j=0; j<DIM; j++) {
                    fat->cpt.p[j] *= Scale;                             // 面中心点をｽｹｰﾘﾝｸﾞ
                }
                fat->r *= Scale;                                        // 面最小球半径をｽｹｰﾘﾝｸﾞ
                fat = fat->next;
            }
        }
        obt = obt->next;
    }
    for(i=0; i<DIM; i++) {
        Eye.p[i] *= Scale;                                              // 視点座標をｽｹｰﾘﾝｸﾞ
    }
    for(i=0; i<LgtNum; i++) {
        if(Lgt[i].type==1) {                                            // 点光源の場合
            Lgt[i].x1 *= Scale, Lgt[i].y1 *= Scale, Lgt[i].z1 *= Scale; // 光源座標をｽｹｰﾘﾝｸﾞ
        }
    }
    LgtLeng = Scale*LLENGTH;                                            // 減衰基本長をｽｹｰﾘﾝｸﾞ
}

/******************************/
static BOOL MakeRendData(void)                                          //<<< ﾚﾝﾀﾞﾘﾝｸﾞﾃﾞｰﾀ作成
/******************************/
{
    OBJTYPE* op;
    PNTTYPE  ps, pe, pp, pt, st[8], pa[8];
    LIGHT    lt;
    VECTYPE  vc;
    RMAT     matz, matx;
    COLORREF bk;
    int      i, i1, i2, j, vnum, lnum, flg[4];
    double   th, ln, btm[3], oct[3], sts[4], x, y, z;

    View->GetCentPsPnt(&ps);                                            // 参照点取得
    View->GetEyePnt(&pe);                                               // 視点取得
    View->GetEyeSvPnt(&pp);                                             // 視点SV取得
    th = PI*15.0/180.0;                                                 // 15度のﾗｼﾞｱﾝ
    ln = PntD(&ps, &pe);                                                // 視点-参照点距離
    Scale = (double)(Szy-1)/(2.0*ln*tan(th));                           // ｽｹｰﾘﾝｸﾞ倍率
    for(i=0; i<DIM; i++) {
        EyeVec.p[i] = ps.p[i]-pp.p[i];                                  // 視線ﾍﾞｸﾄﾙ
        CentPs.p[i] = ps.p[i];                                          // 参照点(注視点)
        Eye.p[i]    = pe.p[i]-CentPs.p[i];                              // 視点(参照点を原点)
    }
    VecN(&EyeVec);                                                      // 単位ﾍﾞｸﾄﾙ化
    MakeRotateMatrix(0, &EyeVec, &matz, &matx);                         // 回転ﾏﾄﾘｯｸｽ作成
    MoveVertex(&Eye, &matz);                                            // 視点回転(Z)
    MoveVertex(&Eye, &matx);                                            // 視点回転(X)
    GetObjBottom(btm);                                                  // ｽﾃｰｼﾞﾎﾟｼﾞｼｮﾝ取得
    GetObjCent(oct);                                                    // 表示立体ｾﾝﾀｰ取得
    GetRayTraceEnv(&bk, &MaxLevel, &AntiAlias, &EdgeFlag, &i, sts);     // 環境情報取得
    BkR = (double)GetRValue(bk)/255.0;                                  // 背景色(RGB)
    BkG = (double)GetGValue(bk)/255.0;
    BkB = (double)GetBValue(bk)/255.0;
    flg[0] = ((i&0x1)>0 ? 1 : 0);                                       // ｽﾃｰｼﾞ有無ﾌﾗｸﾞ
    flg[1] = ((i&0x2)>0 ? 1 : 0);
    flg[2] = ((i&0x4)>0 ? 1 : 0);
    flg[3] = ((i&0x8)>0 ? 1 : 0);
    for(i=0; i<ATMAX; i++) {
        GetRayTraceColor(i, &Atr[i]);                                   // 表面属性情報取得
    }
    for(i=LgtNum=0; i<LGMAX; i++) {
        GetRayTraceLight(i, &lt);                                       // 光源情報取得
        if(lt.flg) {                                                    // 光源が有効の場合
            if(lt.type==1) {                                            // 点光源の場合
                pt.p[0] = lt.x1+oct[0]-CentPs.p[0];                     // 参照点を原点とする
                pt.p[1] = lt.y1+oct[1]-CentPs.p[1];                     // 点光源座標位置
                pt.p[2] = lt.z1+oct[2]-CentPs.p[0];
                MoveVertex(&pt, &matz);                                 // 光源位置回転(Z)
                MoveVertex(&pt, &matx);                                 // 光源位置回転(X)
                lt.x1 = pt.p[0], lt.y1 = pt.p[1], lt.z1 = pt.p[2];
            } else {                                                    // 平行光源の場合
                vc.p[0] = lt.x2-lt.x1;
                vc.p[1] = lt.y2-lt.y1;
                vc.p[2] = lt.z2-lt.z1;
                VecN(&vc);                                              // 単位ﾍﾞｸﾄﾙ化
                MoveVector(&vc, &matz);                                 // 光源ﾍﾞｸﾄﾙ回転(Z)
                MoveVector(&vc, &matx);                                 // 光源ﾍﾞｸﾄﾙ回転(X)
                VecN(&vc);                                              // 単位ﾍﾞｸﾄﾙ化
                lt.x1 = vc.p[0], lt.y1 = vc.p[1], lt.z1 = vc.p[2];
            }
            Lgt[LgtNum++] = lt;
        }
    }
    Olist = NULL, Olast = NULL, Vtnum = 0;
    BaseObjPtr(1, &op, &i1, &i2);                                       // 表示立体ﾎﾟｲﾝﾀ取得
    if(op==NULL) {
        return FALSE;
    }
    while(op) {
        vnum = GetVtxNum(op);                                           // 頂点数取得
        lnum = GetFlpNum1(op);                                          // 面ﾙｰﾌﾟ数取得
        for(i=0; i<vnum; i++) {
            GetVtx(op, i, &pt);                                         // 頂点座標値取得
            for(j=0; j<DIM; j++) {
                pt.p[j] -= CentPs.p[j];
            }
            MoveVertex(&pt, &matz);                                     // 頂点回転(Z)
            MoveVertex(&pt, &matx);                                     // 頂点回転(X)
            Pdat[Vtnum+i] = pt;
        }
        if(GetObjAtr(op)!=1) {                                          // 曲面法線ﾍﾞｸﾄﾙ設定
            if(EdgeFlag) {
                SetObjFVec1(op, vnum, lnum);                            // (曲面：稜線角)
            } else {
                SetObjFVec2(op, vnum, lnum);                            // (曲面：稜線丸)
            }
        } else {
            SetObjFVec3(op, lnum);                                      // (宝石ｶｯﾄ)
        }
        Vtnum += vnum;
        NextObjPtr(1, &op, &i1, &i2);                                   // 次立体ﾎﾟｲﾝﾀ取得
    }
    if(flg[0]||flg[1]||flg[2]||flg[3]) {                                // <ｽﾃｰｼﾞ有の場合>
        x = sts[0]/2.0, y = sts[1]/2.0;                                 // X/2.0, Y/2.0
        z = btm[2]-sts[3];                                              // 底面Z位置
        st[0].p[0] = btm[0]-x, st[0].p[1] = btm[1]-y, st[0].p[2] = z;   // ｽﾃｰｼﾞ用頂点
        st[1].p[0] = btm[0]-x, st[1].p[1] = btm[1]+y, st[1].p[2] = z;
        st[2].p[0] = btm[0]+x, st[2].p[1] = btm[1]-y, st[2].p[2] = z;
        st[3].p[0] = btm[0]+x, st[3].p[1] = btm[1]+y, st[3].p[2] = z;
        z += sts[2];                                                    // 背面･側面高さ
        st[4].p[0] = btm[0]-x, st[4].p[1] = btm[1]-y, st[4].p[2] = z;
        st[5].p[0] = btm[0]-x, st[5].p[1] = btm[1]+y, st[5].p[2] = z;
        st[6].p[0] = btm[0]+x, st[6].p[1] = btm[1]-y, st[6].p[2] = z;
        st[7].p[0] = btm[0]+x, st[7].p[1] = btm[1]+y, st[7].p[2] = z;
        if(flg[0]) {                                                    // <底面>
            pa[0] = st[0], pa[1] = st[2], pa[2] = st[3], pa[3] = st[1];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i+4].p[2] -= 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // 参照点を原点
                }
                MoveVertex(&pa[i], &matz);                              // 頂点回転(Z)
                MoveVertex(&pa[i], &matx);                              // 頂点回転(X)
            }
            SetStageFVec(pa);                                           // ｽﾃｰｼﾞ法線ﾍﾞｸﾄﾙ設定
        }
        if(flg[1]) {                                                    // <背面>
            pa[0] = st[0], pa[1] = st[1], pa[2] = st[5], pa[3] = st[4];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i].p[0] += 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // 参照点を原点
                }
                MoveVertex(&pa[i], &matz);                              // 頂点回転(Z)
                MoveVertex(&pa[i], &matx);                              // 頂点回転(X)
            }
            SetStageFVec(pa);                                           // ｽﾃｰｼﾞ法線ﾍﾞｸﾄﾙ設定
        }
        if(flg[2]) {                                                    // <右側面>
            pa[0] = st[1], pa[1] = st[3], pa[2] = st[7], pa[3] = st[5];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i].p[1] -= 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // 参照点を原点
                }
                MoveVertex(&pa[i], &matz);                              // 頂点回転(Z)
                MoveVertex(&pa[i], &matx);                              // 頂点回転(X)
            }
            SetStageFVec(pa);                                           // ｽﾃｰｼﾞ法線ﾍﾞｸﾄﾙ設定
        }
        if(flg[3]) {                                                    // <左側面>
            pa[0] = st[0], pa[1] = st[4], pa[2] = st[6], pa[3] = st[2];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i].p[1] += 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // 参照点を原点
                }
                MoveVertex(&pa[i], &matz);                              // 頂点回転(Z)
                MoveVertex(&pa[i], &matx);                              // 頂点回転(X)
            }
            SetStageFVec(pa);                                           // ｽﾃｰｼﾞ法線ﾍﾞｸﾄﾙ設定
        }
    }
    DoScaling();                                                        // 表示ｳｨﾝﾄﾞｳ内ｽｹｰﾘﾝｸﾞ
    return TRUE;
}

/******************************************************************/
static BOOL CalcCrossLine(PNTTYPE* p1, PNTTYPE* p2, VECTYPE* v1,
                          VECTYPE* v2, PNTTYPE* crs, VECTYPE* nvc)      //<<< 交差線計算
/******************************************************************/
{
    PNTTYPE pmin, pmax;
    VECTYPE vmin, vmax;
    double  pr;
    int     i;

    if(p1->p[0]<p2->p[0]) {                                             // X小順に入替え
        pmin = *p1, pmax = *p2, vmin = *v1, vmax = *v2;
    } else {
        pmin = *p2, pmax = *p1, vmin = *v2, vmax = *v1;
    }
    if(pmin.p[0]<=0.0&&pmax.p[0]>0.0) {                                 // Xが0点を通過する場合
        pr = pmax.p[0]/(pmax.p[0]-pmin.p[0]);                           // 0点までの比率
        for(i=0; i<DIM; i++) {
            crs->p[i] = pmax.p[i]+(pmin.p[i]-pmax.p[i])*pr;             // 0点位置の頂点
            nvc->p[i] = vmax.p[i]+(vmin.p[i]-vmax.p[i])*pr;             // 0点位置のﾍﾞｸﾄﾙ
        }
        VecN(nvc);                                                      // 単位ﾍﾞｸﾄﾙ化
        return TRUE;
    } else {
        return FALSE;
    }
}

/*****************************************************************/
static BOOL CalcCrossFace(FATYPE* fat, PNTTYPE* scr, RMAT* matz,
                          RMAT* matx, PNTTYPE* crs, VECTYPE* nvc)       //<<< 交差面計算
/*****************************************************************/
{
    LPTYPE* wlp;
    PNTTYPE spt, bpt, cpt, crs_p, pls_p, mis_p;
    VECTYPE svc, bvc, cvc, crs_v, pls_v, mis_v;
    int     i, pls=0, mis=0;
    double  pls_l, mis_l, pr;

    pls_l = DBL_MAX, mis_l = -DBL_MAX, wlp = fat->lpl;
    if(wlp==NULL) {
        return FALSE;
    }
    spt = Pdat[wlp->vno];                                               // 開始点
    for(i=0; i<DIM; i++) {
        spt.p[i] -= scr->p[i];                                          // 基準位置を原点
    }
    MoveVertex(&spt, matz);                                             // 頂点回転(Z)
    MoveVertex(&spt, matx);                                             // 頂点回転(X)
    svc = wlp->vec;                                                     // 開始点ﾍﾞｸﾄﾙ
    MoveVector(&svc, matz);                                             // 開始点ﾍﾞｸﾄﾙ回転(Z)
    MoveVector(&svc, matx);                                             // 開始点ﾍﾞｸﾄﾙ回転(X)
    bpt = spt, bvc = svc;                                               // 開始点を直前点
    while(wlp->next!=NULL) {
        wlp = wlp->next;
        cpt = Pdat[wlp->vno];                                           // 今回点
        for(i=0; i<DIM; i++) {
            cpt.p[i] -= scr->p[i];                                      // 基準位置を原点
        }
        MoveVertex(&cpt, matz);                                         // 頂点回転(Z)
        MoveVertex(&cpt, matx);                                         // 頂点回転(X)
        cvc = wlp->vec;                                                 // 今回点ﾍﾞｸﾄﾙ
        MoveVector(&cvc, matz);                                         // ﾍﾞｸﾄﾙ回転(Z)
        MoveVector(&cvc, matx);                                         // ﾍﾞｸﾄﾙ回転(X)
        if(CalcCrossLine(&bpt, &cpt, &bvc, &cvc, &crs_p, &crs_v)) {     // 交差線計算
            if(crs_p.p[1]>=0.0) {
                pls++;
                if(pls_l>crs_p.p[1]) {
                    pls_p = crs_p, pls_v = crs_v, pls_l = crs_p.p[1];   // 最も0に近いﾃﾞｰﾀ保存
                }
            } else {
                mis++;
                if(mis_l<crs_p.p[1]) {
                    mis_p = crs_p, mis_v = crs_v, mis_l = crs_p.p[1];   // 最も0に近いﾃﾞｰﾀ保存
                }
            }
        }
        bpt = cpt, bvc = cvc;                                           // 今回点を直前点
    }
    if(CalcCrossLine(&bpt, &spt, &bvc, &svc, &crs_p, &crs_v)) {         // 交差線計算
        if(crs_p.p[1]>=0.0) {
            pls++;
            if(pls_l>crs_p.p[1]) {
                pls_p = crs_p, pls_v = crs_v, pls_l = crs_p.p[1];       // 最も0に近いﾃﾞｰﾀ保存
            }
        } else {
            mis++;
            if(mis_l<crs_p.p[1]) {
                mis_p = crs_p, mis_v = crs_v, mis_l = crs_p.p[1];       // 最も0に近いﾃﾞｰﾀ保存
            }
        }
    }
    if(pls%2==1&&mis%2==1) {
        pr = pls_p.p[1]/(pls_p.p[1]-mis_p.p[1]);                        // 0点までの比率
        for(i=0; i<DIM; i++) {
            crs->p[i] = pls_p.p[i]+(mis_p.p[i]-pls_p.p[i])*pr;          // 0点位置の頂点
            nvc->p[i] = pls_v.p[i]+(mis_v.p[i]-pls_v.p[i])*pr;          // 0点位置のﾍﾞｸﾄﾙ
        }
        VecN(nvc);                                                      // 単位ﾍﾞｸﾄﾙ化
        return TRUE;
    } else {
        return FALSE;
    }
}

/*******************************************************************/
static BOOL GetCrossPoint(PNTTYPE* eye, PNTTYPE* scr, PNTTYPE* crs,
                          VECTYPE* nvc, VECTYPE* fnv, int* ano)         //<<< 立体交点算出
/*******************************************************************/
{
    OBTYPE* obt;
    FATYPE* fat;
    PNTTYPE weye, pnt, wcrs;
    VECTYPE wvc, wvec;
    RMAT    matx, matz;
    int     i, j, san, cnt=0;
    double  ln, min;

    min = DBL_MAX;
    for(i=0; i<DIM; i++) {
        wvc.p[i] = scr->p[i]-eye->p[i];                                 // 視点->描画位置ﾍﾞｸﾄﾙ
        weye.p[i] = eye->p[i]-scr->p[i];                                // 視点(描画点を原点)
    }
    VecN(&wvc);                                                         // 単位ﾍﾞｸﾄﾙ化
    MakeRotateMatrix(0, &wvc, &matz, &matx);                            // 回転ﾏﾄﾘｯｸｽ作成
    MoveVertex(&weye, &matz);                                           // 視点回転(Z)
    MoveVertex(&weye, &matx);                                           // 視点回転(X)
    obt = Olist;
    while(obt!=NULL) {
        pnt = obt->cpt;                                                 // 最初に立体中心点でﾁｪｯｸ
        for(i=0; i<DIM; i++) {
            pnt.p[i] -= scr->p[i];                                      // 描画位置を原点とする
        }
        MoveVertex(&pnt, &matz);                                        // 頂点回転(Z)
        MoveVertex(&pnt, &matx);                                        // 頂点回転(X)
        ln = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]);                 // XY平面上の(0,0)からの距離
        if(ln<obt->r+EPS_R&&pnt.p[2]-obt->r<weye.p[2]+EPS_R) {
            for(i=0; i<8; i++) {
                pnt = obt->cp8[i];                                      // 次に8分割中心点でﾁｪｯｸ
                for(j=0; j<DIM; j++) {
                    pnt.p[j] -= scr->p[j];                              // 描画位置を原点とする
                }
                MoveVertex(&pnt, &matz);                                // 頂点回転(Z)
                MoveVertex(&pnt, &matx);                                // 頂点回転(X)
                ln = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]);         // XY平面上の(0,0)からの距離
                if(ln<obt->r8[i]+EPS_R&&pnt.p[2]-obt->r8[i]<weye.p[2]+EPS_R) {
                    fat = obt->fal[i];
                    while(fat!=NULL) {
                        pnt = fat->cpt;
                        for(j=0; j<DIM; j++) {
                            pnt.p[j] -= scr->p[j];                      // 描画位置を原点とする
                        }
                        MoveVertex(&pnt, &matz);                        // 頂点回転(Z)
                        MoveVertex(&pnt, &matx);                        // 頂点回転(X)
                        ln = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]); // XY平面上の(0,0)からの距離
                        if(ln<fat->r+EPS_R&&pnt.p[2]-fat->r<weye.p[2]+EPS_R) {
                            if(CalcCrossFace(fat, scr, &matz, &matx, &wcrs, &wvec)) {
                                if(weye.p[2]>wcrs.p[2]) {
                                    cnt++;
                                    if(min>weye.p[2]-wcrs.p[2]) {       // 視点からの距離
                                        min = weye.p[2]-wcrs.p[2];      // 視点に最も近い距離
                                        *nvc = wvec;                    // 隣接面を加味した法線ﾍﾞｸﾄﾙ
                                        *fnv = fat->vec;                // 面法線ﾍﾞｸﾄﾙ
                                        san = obt->ano;                 // 表面属性No保存
                                    }
                                }
                            }
                        }
                        fat = fat->next;
                    }
                }
            }
        }
        obt = obt->next;
    }
    if(cnt>0) {
        for(i=0; i<DIM; i++) {
            crs->p[i] = eye->p[i]+min*wvc.p[i];                         // 交点
        }
        MakeRotateMatrix(1, &wvc, &matz, &matx);                        // 回転ﾏﾄﾘｯｸｽ作成(戻し)
        MoveVector(nvc, &matx);                                         // 法線ﾍﾞｸﾄﾙ回転(X)
        MoveVector(nvc, &matz);                                         // 法線ﾍﾞｸﾄﾙ回転(Z)
        VecN(nvc);                                                      // 単位ﾍﾞｸﾄﾙ化
        *ano = san;                                                     // 表面属性No
        return TRUE;
    } else {
        return FALSE;
    }
}

/*******************************************************************/
static void GetLight(int ix, PNTTYPE* crs, VECTYPE* lvc, double* r,
                     double* g, double* b, double* dist)                //<<< 光源情報取得
/*******************************************************************/
{
    double p,l;

    if(Lgt[ix].type==0) {                                               // 平行光源の場合
        lvc->p[0] = -Lgt[ix].x1;                                        // 光源ﾍﾞｸﾄﾙ
        lvc->p[1] = -Lgt[ix].y1;
        lvc->p[2] = -Lgt[ix].z1;
        *r = (double)GetRValue(Lgt[ix].lc)/255.0;                       // 光源RGB
        *g = (double)GetGValue(Lgt[ix].lc)/255.0;
        *b = (double)GetBValue(Lgt[ix].lc)/255.0;
        *dist = DBL_MAX;
    } else if(Lgt[ix].type==1) {                                        // 点光源の場合
        lvc->p[0] = Lgt[ix].x1-crs->p[0];                               // 光源ﾍﾞｸﾄﾙ
        lvc->p[1] = Lgt[ix].y1-crs->p[1];
        lvc->p[2] = Lgt[ix].z1-crs->p[2];
        l = sqrt(lvc->p[0]*lvc->p[0]+lvc->p[1]*lvc->p[1]+lvc->p[2]*lvc->p[2]);
        if(l<1.e-7) {
            l = 1.0; 
        }
        if(Lgt[ix].k==0) {                                              // 減衰なし
            p = 1.0;
        } else if(Lgt[ix].k==1) {                                       // 距離の1乗で減衰
            p = LgtLeng/(l+LgtLeng);
        } else if(Lgt[ix].k==2) {                                       // 距離の2乗で減衰
            p = LgtLeng/(l*l+LgtLeng);
        }
        *r = ((double)GetRValue(Lgt[ix].lc)/255.0)*p;                   // 光源RGB
        *g = ((double)GetGValue(Lgt[ix].lc)/255.0)*p;
        *b = ((double)GetBValue(Lgt[ix].lc)/255.0)*p;
        *dist = l;
        VecN(lvc);                                                      // 単位ﾍﾞｸﾄﾙ化
    }
}

/**********************************************************/
static void Shadow(PNTTYPE* crs, VECTYPE* lvc, double dst,
                   double* sr, double* sg, double* sb)                  //<<< 影の計算
/**********************************************************/
{
    PNTTYPE pt1, pt2, pnt;
    VECTYPE wnv, wfn;
    int     i, ano;

    *sr = 1.0, *sg = 1.0, *sb = 1.0;
    for(i=0; i<DIM; i++) {
        pt1.p[i] = crs->p[i]+lvc->p[i]*Scale*0.05;                      // 交点を表面から離す
        pt2.p[i] = crs->p[i]+lvc->p[i]*Scale*100.0;                     // 光源方向の仮頂点
    }
    if(GetCrossPoint(&pt1, &pt2, &pnt, &wnv, &wfn, &ano)) {             // 立体交点算出
        if(PntD(crs, &pnt)<dst) {                                       // 光源との間に立体有
            *sr = (double)GetRValue(Atr[ano].ct)/255.0;                 // 透過調整係数
            *sg = (double)GetGValue(Atr[ano].ct)/255.0;
            *sb = (double)GetBValue(Atr[ano].ct)/255.0;
        }
    }
}

/****************************************************/
static double HighLight1(int ano, VECTYPE* lvc,                         //<<< ﾊｲﾗｲﾄの計算
                         VECTYPE* vvc, VECTYPE* nvc)                    //   (Phong Model)
/****************************************************/
{
    double rx, ry, rz, rv, ln, rvn=0.0;
    int    i, m;

    ln = 2*(lvc->p[0]*nvc->p[0]+lvc->p[1]*nvc->p[1]+lvc->p[2]*nvc->p[2]);
    rx = ln*nvc->p[0]-lvc->p[0];
    ry = ln*nvc->p[1]-lvc->p[1];
    rz = ln*nvc->p[2]-lvc->p[2];
    rv = rx*vvc->p[0]+ry*vvc->p[1]+rz*vvc->p[2];                        // Cosα
    if(rv>=0.0) {
        m = Atr[ano].m1;                                                // ﾊｲﾗｲﾄの広がり
        for(rvn=1.0, i=0; i<m; i++) {
            rvn *= rv;                                                  // Cosα**N
        }
    }
    return rvn;
}

/****************************************************/
static double HighLight2(int ano, VECTYPE* lvc,                         //<<< ﾊｲﾗｲﾄの計算
                         VECTYPE* vvc, VECTYPE* nvc)                    //   (Cook-Torrance Model)
/****************************************************/
{
    VECTYPE hvc;
    double  cos1, cos2, tan2, a, m=0.0, rvn=0.0;
    int     i;

    for(i=0; i<DIM; i++) {
        hvc.p[i] = lvc->p[i]+vvc->p[i];                                 // lvcとvvcの和
    }
    VecN(&hvc);                                                         // 単位ﾍﾞｸﾄﾙ化
    cos1 = hvc.p[0]*nvc->p[0]+hvc.p[1]*nvc->p[1]+hvc.p[2]*nvc->p[2];    // Cosα
    cos2 = cos1*cos1;
    if(cos2<0.0000001) {
        cos2 = 0.0000001;
    }
    tan2 = 1.0/cos2-1.0;
    m = (Atr[ano].m2>0.0000001) ? (double)Atr[ano].m2 : 0.0000001;      // ﾊｲﾗｲﾄの広がり
    a = -tan2/(m*m);
    rvn = exp(a)/(m*m*cos2*cos2);
    return rvn;
}

/**********************************************************************/
static void Shading(int ano, PNTTYPE* crs, VECTYPE* vvc, VECTYPE* nvc,
                    double n, double* r, double* g, double* b)          //<<< ｼｪｰﾃﾞｨﾝｸﾞ
/**********************************************************************/
{
    VECTYPE lvc;
    double  kdr, kdg, kdb, ksr, ksg, ksb, ir, ig, ib;
    double  dist, ln, sr, sg, sb, rvn=0.0, cs, w1, w2, w3, n1, n2, n3;
    int     i;

    *r  = (double)GetRValue(Atr[ano].ka)/255.0;                         // 環境光
    *g  = (double)GetGValue(Atr[ano].ka)/255.0;
    *b  = (double)GetBValue(Atr[ano].ka)/255.0;
    kdr = (double)GetRValue(Atr[ano].kd)/255.0;                         // 拡散反射係数
    kdg = (double)GetGValue(Atr[ano].kd)/255.0;
    kdb = (double)GetBValue(Atr[ano].kd)/255.0;
    ksr = (double)GetRValue(Atr[ano].ks)/255.0;                         // 鏡面反射係数
    ksg = (double)GetGValue(Atr[ano].ks)/255.0;
    ksb = (double)GetBValue(Atr[ano].ks)/255.0;
    for(i=0; i<LgtNum; i++) {
        GetLight(i, crs, &lvc, &ir, &ig, &ib, &dist);                   // 光源情報取得
        ln = lvc.p[0]*nvc->p[0]+lvc.p[1]*nvc->p[1]+lvc.p[2]*nvc->p[2];  // Cosθ
        if(ln<0) {
            ln = 0.0;
        } else {
            Shadow(crs, &lvc, dist, &sr, &sg, &sb);                     // 影の計算
            ir *= sr, ig *= sg, ib *= sb;
        }
        if(!Atr[ano].model) {                                           // <Phongﾀｲﾌﾟ>
            if(Atr[ano].m1>0) {                                         // ﾊｲﾗｲﾄ有効
                rvn = HighLight1(ano, &lvc, vvc, nvc);                  // Phong Model
            }
        } else {                                                        // <Cook-Torranceﾀｲﾌﾟ>
            if(Atr[ano].m2>0.0) {                                       // ﾊｲﾗｲﾄ有効
                rvn = HighLight2(ano, &lvc, vvc, nvc);                  // Cook-Torrance Model
                cs = vvc->p[0]*nvc->p[0]+vvc->p[1]*nvc->p[1]+vvc->p[2]*nvc->p[2];
                if(cs<0.0) {
                    cs = 0.0;
                }
                n1 = Atr[ano].nr, n2 = Atr[ano].ng, n3 = Atr[ano].nb;   // 鏡面反射係数計算
                w1 = (n1*n1)+(cs*cs)-1.0, w1 = sqrt(w1);
                w2 = (w1-cs)/(w1+cs), w2 = w2*w2;
                w3 = (cs*(w1+cs)-1.0)/(cs*(w1-cs)+1.0), w3 = w3*w3;
                ksr = (w2*(1.0+w3))/2.0;
                w1 = (n2*n2)+(cs*cs)-1.0, w1 = sqrt(w1);
                w2 = (w1-cs)/(w1+cs), w2 = w2*w2;
                w3 = (cs*(w1+cs)-1.0)/(cs*(w1-cs)+1.0), w3 = w3*w3;
                ksg = (w2*(1.0+w3))/2.0;
                w1 = (n3*n3)+(cs*cs)-1.0, w1 = sqrt(w1);
                w2 = (w1-cs)/(w1+cs), w2 = w2*w2;
                w3 = (cs*(w1+cs)-1.0)/(cs*(w1-cs)+1.0), w3 = w3*w3;
                ksb = (w2*(1.0+w3))/2.0;
            }
        }
        *r += ir*(kdr*ln+ksr*rvn);                                      // 表面輝度(RGB)
        *g += ig*(kdg*ln+ksg*rvn);
        *b += ib*(kdb*ln+ksb*rvn);
    }
}

/*********************************************************/
static void RayTrace(int lvl, PNTTYPE* eye, PNTTYPE* scr,
                     double* r, double* g, double* b)                   //<<< 視線追跡処理
/*********************************************************/
{
    PNTTYPE crs, p1, p2;
    VECTYPE nvc, fnv, evc, vvc, wnv, tvc, rvc;
    int     i, ano;
    double  ksr, ksg, ksb, ctr, ctg, ctb, tr, tg, tb, rr, rg, rb;
    double  sign, n1, n2, cs1, cs2, a, ks1, ks2, ct1, en;

    if(GetCrossPoint(eye, scr, &crs, &nvc, &fnv, &ano)) {               // 交点･法線ﾍﾞｸﾄﾙ算出
        for(i=0; i<DIM; i++) {
            evc.p[i] = scr->p[i]-eye->p[i];                             // 視線ﾍﾞｸﾄﾙ
            vvc.p[i] = eye->p[i]-scr->p[i];                             // 視線ﾍﾞｸﾄﾙ(逆)
        }
        VecN(&evc);                                                     // 単位ﾍﾞｸﾄﾙ化
        VecN(&vvc);
        n1 = Atr[ano].n;                                                // 屈折率
        sign = evc.p[0]*fnv.p[0]+evc.p[1]*fnv.p[1]+evc.p[2]*fnv.p[2];   // 視線と面法線の比較
        if(sign<=0) {                                                   // <表側>
            for(i=0; i<DIM; i++) {
                wnv.p[i] = nvc.p[i];                                    // 交点法線ﾍﾞｸﾄﾙはそのまま
            }
            n2 = n1;                                                    // 相対屈折率(入)
        } else {                                                        // <裏側>
            for(i=0; i<DIM; i++) {
                wnv.p[i] = -1.0*nvc.p[i];                               // 交点法線ﾍﾞｸﾄﾙは逆転
            }
            n2 = 1.0/n1;                                                // 相対屈折率(出)
        }
        Shading(ano, &crs, &vvc, &wnv, n1, r, g, b);                    // ｼｪｰﾃﾞｨﾝｸﾞ
        if(Atr[ano].trace&&lvl<MaxLevel) {                              // <最大視線追跡ﾚﾍﾞﾙ未満>
            ksr = (double)GetRValue(Atr[ano].ks)/255.0;                 // 鏡面反射係数
            ksg = (double)GetGValue(Atr[ano].ks)/255.0;
            ksb = (double)GetBValue(Atr[ano].ks)/255.0;
            ctr = (double)GetRValue(Atr[ano].ct)/255.0;                 // 透過調整係数
            ctg = (double)GetGValue(Atr[ano].ct)/255.0;
            ctb = (double)GetBValue(Atr[ano].ct)/255.0;
            if(ctr+ctg+ctb>0.01) {                                      // <透過(屈折)>
                cs1 = vvc.p[0]*wnv.p[0]+vvc.p[1]*wnv.p[1]+vvc.p[2]*wnv.p[2];
                cs2 = 1.0-(1.0-cs1*cs1)/(n2*n2);
                if(cs2<0.0) {
                    ksr = 1.0, ksg = 1.0, ksb = 1.0;                    // 完全反射
                    for(i=0; i<DIM; i++) {
                        tvc.p[i] = -1.0*wnv.p[i];
                    }
                } else {
                    cs2 = sqrt(cs2), a = n2*cs2-cs1;
                    ks1 = a/(n2*cs2+cs1), ks1 *= ks1;
                    ks2 = (n2*cs1-cs2)/(n2*cs1+cs2), ks2 *= ks2;
                    ks1 = 0.5*(ks1+ks2);                                // 反射係数
                    ct1 = 1.0-ks1;                                      // 透過係数
                    tvc.p[0] = (evc.p[0]-wnv.p[0]*a)/n2;                // 透過ﾍﾞｸﾄﾙ
                    tvc.p[1] = (evc.p[1]-wnv.p[1]*a)/n2;
                    tvc.p[2] = (evc.p[2]-wnv.p[2]*a)/n2;
                    if(ct1>0.1) {
                        VecN(&tvc);                                     // 単位ﾍﾞｸﾄﾙ化
                        for(i=0; i<DIM; i++) {
                            p1.p[i] = crs.p[i]+tvc.p[i]*Scale*0.05;     // 交点を表面から離す
                            p2.p[i] = crs.p[i]+tvc.p[i]*Scale*100.0;    // 透過ﾍﾞｸﾄﾙの仮頂点
                        }
                        RayTrace(lvl+1, &p1, &p2, &tr, &tg, &tb);       // 視線追跡処理
                        *r += ((double)GetRValue(Atr[ano].ct)/255.0)*ct1*tr;// 透過情報加算
                        *g += ((double)GetGValue(Atr[ano].ct)/255.0)*ct1*tg;
                        *b += ((double)GetBValue(Atr[ano].ct)/255.0)*ct1*tb;
                    }
                    ksr = ks1, ksg = ks1, ksb = ks1;
                }
            }
            if(ksr+ksg+ksb>0.1) {                                       // <反射>
                en = 2*(evc.p[0]*wnv.p[0]+evc.p[1]*wnv.p[1]+evc.p[2]*wnv.p[2]);
                rvc.p[0] = evc.p[0]-wnv.p[0]*en;                        // 鏡面反射ﾍﾞｸﾄﾙ
                rvc.p[1] = evc.p[1]-wnv.p[1]*en;
                rvc.p[2] = evc.p[2]-wnv.p[2]*en;
                VecN(&rvc);                                             // 単位ﾍﾞｸﾄﾙ化
                for(i=0; i<DIM; i++) {
                    p1.p[i] = crs.p[i]+rvc.p[i]*Scale*0.05;             // 交点を表面から離す
                    p2.p[i] = crs.p[i]+rvc.p[i]*Scale*100.0;            // 鏡面反射ﾍﾞｸﾄﾙの仮頂点
                }
                RayTrace(lvl+1, &p1, &p2, &rr, &rg, &rb);               // 視線追跡処理
                *r += ksr*rr, *g += ksg*rg, *b += ksb*rb;               // 鏡面反射情報加算
            }
        }
    } else {
        *r = BkR, *g = BkG, *b = BkB;                                   // 交点なしは背景色
    }
}

/*******************************/
static void InitAntiAlias(void)                                         //<<< ｱﾝﾁｴｲﾘｱｼﾝｸﾞ初期処理
/*******************************/
{
    int i, j;

    for(i=0; i<2; i++) {
        for(j=0; j<Szx; j++) {
            ScanLine[i][j].r = -1.0;                                    // ｽｷｬﾝﾗｲﾝRGB初期化
            ScanLine[i][j].g = -1.0;
            ScanLine[i][j].b = -1.0;
        }
    }
    CalcMat[0][0] = -1, CalcMat[1][0] = -1;                             // ｱﾝﾁｴｲﾘｱｼﾝｸﾞの視線移動量設定
    CalcMat[0][1] =  1, CalcMat[1][1] = -1;
    CalcMat[0][2] = -1, CalcMat[1][2] =  1;
    CalcMat[0][3] =  1, CalcMat[1][3] =  1;
}

/***********************************************************/
static int CheckSub(RGBT rgb, double r, double g, double b)             //<<< ｶﾗｰﾁｪｯｸｻﾌﾞ
/***********************************************************/
{
    int chk=0;

    if(rgb.r>=0.0) {
        chk = (fabs(rgb.r-r)>THRES);
        if(!chk) {
            chk = (fabs(rgb.g-g)>THRES);
        }
        if(!chk) {
            chk = (fabs(rgb.b-b)>THRES);
        }
    }
    return chk;
}

/***************************************************/
static int CheckColor(int y1, int y2, int x,
                      double r, double g, double b)                     //<<< ｶﾗｰﾁｪｯｸ
/***************************************************/
{
    int chk=0;

    if(x>0) {
        chk = CheckSub(ScanLine[y1][x-1], r, g, b);                     // 今回行の左側ﾁｪｯｸ
        if(!chk) {
            chk = CheckSub(ScanLine[y2][x-1], r, g, b);                 // 前行の左側ﾁｪｯｸ
        }
    }
    if(!chk) {
        chk = CheckSub(ScanLine[y2][x], r, g, b);                       // 前行の現在位置ﾁｪｯｸ
    }
    return chk;
}

/******************************/
static void DrawRendData(void)                                          //<<< ﾚﾝﾀﾞﾘﾝｸﾞﾃﾞｰﾀ描画
/******************************/
{
    PNTTYPE scr, wsc;
    int     i, x, y, y1, y2, ir, ig, ib;
    double  r, g, b, sr, sg, sb, e1, e2, tn, sv;

    if(AntiAlias) {
        InitAntiAlias();                                                // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ初期処理
    }
    scr.p[2] = 0.0, wsc.p[2] = 0.0;                                     // Z描画位置
    for(y=0; y<Szy; y++) {
        if(Rend->CheckStpSW()) {                                        // 中止SW:ONは中止
            break;
        }
        y1 = y%2, y2 = (y+1)%2;                                         // y1:今回行 y2:前行
        scr.p[1] = (double)(Szy/2-y);                                   // Y描画位置
        for(x=0; x<Szx; x++) {
            scr.p[0] = (double)(x-Szx/2);                               // X描画位置
            RayTrace(1, &Eye, &scr, &r, &g, &b);                        // 視線追跡処理
            if(AntiAlias&&CheckColor(y1, y2, x, r, g, b)) {             // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ実行
                sr = r, sg = g, sb = b;                                 // 通常のRGBを設定
                for(i=0; i<4; i++) {
                    wsc.p[0] = scr.p[0]+(double)CalcMat[0][i]*MOVEDST;  // X描画位置
                    wsc.p[1] = scr.p[1]+(double)CalcMat[1][i]*MOVEDST;  // Y描画位置
                    RayTrace(1, &Eye, &wsc, &r, &g, &b);                // 視線追跡処理
                    sr += r, sg += g, sb += b;                          // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ用RGB加算
                }
                r = sr/5.0, g = sg/5.0, b = sb/5.0;                     // RGBの平均化
            }
            if(AntiAlias) {
                ScanLine[y1][x].r = r;                                  // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ時は
                ScanLine[y1][x].g = g;                                  // RGBを保存
                ScanLine[y1][x].b = b;
            }
            if(r<0.0) {
                r = 0.0;                                                // Rを0.0-1.0に補正
            }
            if((sv=r)>1.0) {
                sv = 1.0;
            }
            e1 = exp(r), e2 = exp(-r);
            tn = (e1-e2)/(e1+e2), tn = tn+(sv-tn)*0.55;
            ir = (int)(tn*255.0);
            if(g<0.0) {
                g = 0.0;                                                // Gを0.0-1.0に補正
            }
            if((sv=g)>1.0) {
                sv = 1.0;
            }
            e1 = exp(g), e2 = exp(-g);
            tn = (e1-e2)/(e1+e2), tn = tn+(sv-tn)*0.55;
            ig = (int)(tn*255.0);
            if(b<0.0) {
                b = 0.0;                                                // Bを0.0-1.0に補正
            }
            if((sv=b)>1.0) {
                sv = 1.0;
            }
            e1 = exp(b), e2 = exp(-b);
            tn = (e1-e2)/(e1+e2), tn = tn+(sv-tn)*0.55;
            ib = (int)(tn*255.0);
            Rend->SetRendColor(x, y, ir, ig, ib);                       // ﾀﾞｲｱﾛｸﾞ描画
        }
        if(AntiAlias) {
            for(x=0; x<Szx; x++) {                                      // ｱﾝﾁｴｲﾘｱｼﾝｸﾞ時は
                ScanLine[y2][x] = ScanLine[y1][x];                      // 次Xﾗｲﾝ用にRGBをｼﾌﾄ
            }
        }
    }
}

/**************************/
static void WorkFree(void)                                              //<<< 作業領域解放
/**************************/
{
    OBTYPE* obt;
    FATYPE* fat, * fad;
    LPTYPE* lpt, * lpd;
    int     i;

    while(Olist!=NULL) {
        obt = Olist, Olist = Olist->next;
        for(i=0; i<8; i++) {
            fat = obt->fal[i];
            while(fat!=NULL) {
                fad = fat, fat = fat->next, lpt = fad->lpl;
                while(lpt!=NULL) {
                    lpd = lpt, lpt = lpt->next;
                    free(lpd);
                }
                free(fad);
            }
        }
        free(obt);
    }
}

/*******************************/
UINT RayTracingProc(LPVOID prm)                                         //<<< ﾚﾝﾀﾞﾘﾝｸﾞ(ﾚｲ･ﾄﾚｰｼﾝｸﾞ)
/*******************************/
{
    OBJTYPE* op;
    int      i1, i2;

    Rend = (CRendDlg*)prm;
    Rend->GetRendSize(&Szx, &Szy);                                      // 描画ｻｲｽﾞ取得
    BaseObjPtr(1, &op, &i1, &i2);                                       // 表示立体ﾎﾟｲﾝﾀ取得
    if(op!=NULL) {
        if(!MakeRendData()) {                                           // ﾚﾝﾀﾞﾘﾝｸﾞﾃﾞｰﾀ作成
            WorkFree();                                                 // ｴﾗｰは作業領域解放
        } else {
            DrawRendData();                                             // ﾚﾝﾀﾞﾘﾝｸﾞﾃﾞｰﾀ描画
            WorkFree();                                                 // 作業領域解放
        }
    }
    Rend->SetRendColor(-1, -1, 0, 0, 0);                                // ﾚｲ･ﾄﾚｰｼﾝｸﾞ終了
    return 0;
}
