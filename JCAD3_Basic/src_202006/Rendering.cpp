/////////////////////////////////////////////////////////////////////////////
// �����_�����O(RayTracing)��
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
static void CalcMatrix(VECTYPE* ax, double th, RMAT* rot)               //<<< ��]��د���v�Z
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
                             RMAT* matz, RMAT* matx)                    //<<< ��]��د���쐬
/****************************************************/
{
    VECTYPE vec, ax;
    int     i, j;
    double  th;

    for(i=0; i<DIM; i++) {
        vec.p[i] = -eye->p[i];
    }
    if(fabs(vec.p[0])<EPS_R&&fabs(vec.p[1])<EPS_R) {                    // Z����]�p�x�v�Z
        th = PI/2.0;
    } else {
        th = atan2(vec.p[1], vec.p[0]) + PI/2.0;
    }
    if(flg) {
        th = -th;                                                       // flg:ON�͖߂�
    }
    if(fabs(th)>EPS_R) {
        ax.p[0] = 0.0, ax.p[1] = 0.0, ax.p[2] = -1.0;
        CalcMatrix(&ax, th, matz);                                      // ��]��د���v�Z
    } else {
        for(i=0; i<DIM; i++) {
            for(j=0; j<DIM; j++) {
                matz->m2[i][j] = 0.0;
            }
        }
        matz->m2[0][0] = 1.0, matz->m2[1][1] = 1.0, matz->m2[2][2] = 1.0;
    }
    th = sqrt(vec.p[0]*vec.p[0]+vec.p[1]*vec.p[1]);                     // X����]�p�x�v�Z
    if(th<EPS_R&&fabs(vec.p[2])<EPS_R) {
        th = 0.0;
    } else {
        th = atan2(th, vec.p[2]);
    }
    if(flg) {
        th = -th;                                                       // flg:ON�͖߂�
    }
    if(fabs(th)>EPS_R) {
        ax.p[0] = -1.0, ax.p[1] = 0.0, ax.p[2] = 0.0;
        CalcMatrix(&ax, th, matx);                                      // ��]��د���v�Z
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
static void MoveVertex(PNTTYPE* pnt, RMAT* rot)                         //<<< ���_��]
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
static void MoveVector(VECTYPE* vec, RMAT* rot)                         //<<< �޸�ى�]
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
static void InitFVec(void)                                              //<<< �޸�ِݒ菉������
/**************************/
{
    Xmin_f = DBL_MAX,  Ymin_f = DBL_MAX,  Zmin_f = DBL_MAX;
    Xmin_o = DBL_MAX,  Ymin_o = DBL_MAX,  Zmin_o = DBL_MAX;
    Xmax_f = -DBL_MAX, Ymax_f = -DBL_MAX, Zmax_f = -DBL_MAX;
    Xmax_o = -DBL_MAX, Ymax_o = -DBL_MAX, Zmax_o = -DBL_MAX;
    Lfst = NULL, Llst = NULL, Ffst = NULL, Flst = NULL;
}

/******************************************/
static void SetLptype(int vno, PNTTYPE pt)                              //<<< ��ٰ�ߏ��ݒ�
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
    if(Xmin_f>pt.p[0]) {                                                // �ʂ̍ŏ���ő�擾
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
static void SetFatype(VECTYPE nvc)                                      //<<< �ʏ��ݒ�
/**********************************/
{
    FATYPE* fat;
    LPTYPE* lpt;
    PNTTYPE cp;
    double  r, rmax;

    cp.p[0] = (Xmax_f+Xmin_f)/2.0;                                      // �ʒ��S�_
    cp.p[1] = (Ymax_f+Ymin_f)/2.0;
    cp.p[2] = (Zmax_f+Zmin_f)/2.0;
    lpt = Lfst, rmax = 0.0;
    while(lpt!=NULL) {
        r = PntD(&cp, &Pdat[lpt->vno]);                                 // 2�_�ԋ���
        if(r>rmax) {
            rmax = r;                                                   // �ő唼�a�擾
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
    if(Xmin_o>Xmin_f) {                                                 // ���̂̍ŏ���ő�擾
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
    Xmin_f = DBL_MAX,  Ymin_f = DBL_MAX,  Zmin_f = DBL_MAX;             // �ϐ�������
    Xmax_f = -DBL_MAX, Ymax_f = -DBL_MAX, Zmax_f = -DBL_MAX;
    Lfst = NULL, Llst = NULL;
}

/***********************************************/
static void SetObtype(OBJTYPE* op, VECTYPE* lv)                         //<<< ���̏��ݒ�
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
        while(lpt!=NULL) {                                              // ��ٰ�ߖ��ɖ@���޸�ِݒ�
            lpt->vec = lv[cnt++];
            lpt = lpt->next;
        }
        fat = fat->next;
    }
    cp.p[0] = (Xmax_o+Xmin_o)/2.0;                                      // ���̒��S�_
    cp.p[1] = (Ymax_o+Ymin_o)/2.0;
    cp.p[2] = (Zmax_o+Zmin_o)/2.0;
    fat = Ffst, rmax = 0.0;
    while(fat!=NULL) {
        lpt = fat->lpl;
        while(lpt!=NULL) {
            r = PntD(&cp, &Pdat[lpt->vno]);                             // 2�_�ԋ���
            if(r>rmax) {
                rmax = r;                                               // �ő唼�a�擾
            }
            lpt = lpt->next;
        }
        fat = fat->next;
    }
    cp8[0].p[0] = (Xmax_o+cp.p[0])/2.0;                                 // 8�������̒��S�_
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
    while(fat!=NULL) {                                                  // 8�������ɖʏ���U��
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
                r = PntD(&cp8[i], &Pdat[lpt->vno]);                     // 2�_�ԋ���
                if(r>rmax8[i]) {
                    rmax8[i] = r;                                       // �ő唼�a�擾
                }
                lpt = lpt->next;
            }
            fat = fat->next;
        }
    }
    if((obt=(OBTYPE *)malloc(sizeof(OBTYPE)))==NULL) {
        MemErr(IDS_MEMERR5);
    }
    obt->ano = GetObjCol(op);                                           // �װ(�\�ʑ���)
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
static void SetObjFVec1(OBJTYPE* op, int vnum, int lnum)                //<<< �Ȗʖ@���޸�ِݒ�
/********************************************************/              //   (�ȖʁF�Ő��p)
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

    InitFVec();                                                         // �޸�ِݒ菉������
    for(i=0; i<vnum; i++) {
        fg[i] = -2;                                                     // �׸ޏ�����
    }
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // ��ٰ�ߔԍ��擾
        pa[cnt] = Pdat[Vtnum+vno];                                      // ���_���W�ۑ�
        lp[cnt] = i;                                                    // ٰ�ߔԍ��ۑ�
        pn[cnt++] = vno;                                                // ���_�ԍ��ۑ�
        SetLptype(vno, pa[cnt-1]);                                      // ��ٰ�ߏ��ݒ�
        if(flg==1) {                                                    // <<<ٰ�ߍŏI�̏ꍇ>>>
            CalcNVec(pa, cnt, &v1);                                     // �@���޸�َ擾
            SetFatype(v1);                                              // �ʏ��ݒ�
            for(j=0; j<cnt; j++) {
                if(fg[pn[j]]==-2) {                                     // <<�����̏ꍇ>>
                    pv[pn[j]] = v1;                                     // �Ώے��_�޸�ِݒ�
                    fg[pn[j]] = -1;                                     // �׸ލX�V
                } else {                                                // <<�����ȊO�̏ꍇ>>
                    v2 = pv[pn[j]];                                     // �o�^���޸��
                    VecN(&v2);                                          // �P���޸�ى�
                    if(fabs(v1.p[0]-v2.p[0])>EPS_R||                    // 2�޸�يԊp�x
                       fabs(v1.p[1]-v2.p[1])>EPS_R||
                       fabs(v1.p[2]-v2.p[2])>EPS_R) {
                        th = acos(v1.p[0]*v2.p[0]+v1.p[1]*v2.p[1]+v1.p[2]*v2.p[2]);
						th = (th<-10||th>10) ? 0.0 : th;
                    } else {
                        th = 0.0;
                    }
                    if(fabs(th)<2.0*PI/9.0) {                           // <40�x����(�Ȑ�)>
                        for(k=0; k<DIM; k++) { 
                            pv[pn[j]].p[k] += v1.p[k];                  // �Ώے��_�޸�ى��Z
                        }
                    } else {                                            // <40�x�ȏ�(�ܐ�)>
                        if(fg[pn[j]]==-1) {                             // <�ŏ��̐ܐ�>
                            if(an>=View->m_MaxSPnt) {                   // �ް��ْ��~
                                stp = 1; break;
                            }
                            fg[pn[j]] = an;                             // �ܐ��z��ԍ��ݒ�
                            av[an]    = v1;                             // �Ώے��_�޸�ِݒ�
                            ln[an]    = lp[j];                          // ��ٰ�ߔԍ��ݒ�
                            nx[an++]  = -1;                             // ���ް��ԍ�������
                        } else {                                        // <2���ڈȍ~�̐ܐ�>
                            if(an>=View->m_MaxSPnt) {                   // �ް��ْ��~
                                stp = 1; break;
                            }
                            k = fg[pn[j]];
                            while(1) {
                                if(nx[k]==-1) {                         // �ŏI�ܐ��z��ԍ�����
                                    break;
                                }
                                k = nx[k];
                            }
                            nx[k]    = an;                              // �ܐ��z��ԍ��ݒ�
                            av[an]   = v1;                              // �Ώے��_�޸�ِݒ�
                            ln[an]   = lp[j];                           // ��ٰ�ߔԍ��ݒ�
                            nx[an++] = -1;                              // ���ް��ԍ�������
                        }
                    }
                }
            }
            if(stp) {                                                   // �ް��ْ��~
                break;
            }
            cnt = 0;
        }
    }
    if(!stp) {                                                          // <�ް�OK�̏ꍇ>
        for(i=0; i<vnum; i++) {
            VecN(&pv[i]);                                               // �P���޸�ى�
            if(fg[i]>=0) {                                              // <�ܐ��L�̏ꍇ>
                cnt = 0;
                j = fg[i];
                while(1) {
                    wv[cnt]   = av[j];                                  // �ܐ��޸�ٕ���
                    wf[cnt++] = 0;                                      // �����׸ޏ�����
                    if(nx[j]==-1) {
                        break;
                    }
                    j = nx[j];
                }
                for(j=0; j<cnt; j++) {
                    if(wf[j]==0) {                                      // <�������޸��>
                        v1 = wv[j];
                        for(k=j+1; k<cnt; k++) {
                            v2 = v1;
                            VecN(&v2);                                  // �P���޸�ى�
                            v3 = wv[k];
                            if(fabs(v2.p[0]-v3.p[0])>EPS_R||            // 2�޸�يԊp�x
                               fabs(v2.p[1]-v3.p[1])>EPS_R||
                               fabs(v2.p[2]-v3.p[2])>EPS_R) {
                                th = acos(v2.p[0]*v3.p[0]+v2.p[1]*v3.p[1]+v2.p[2]*v3.p[2]);
								th = (th<-10||th>10) ? 0.0 : th;
                            } else {
                                th = 0.0;
                            }
                            if(fabs(th)<2.0*PI/9.0) {                   // <40�x����(�Ȑ�)>
                                for(l=0; l<DIM; l++) {
                                    v1.p[l] += v3.p[l];                 // �Ώے��_�޸�ى��Z
                                }
                                wf[k] = 1;                              // �����r��
                            }
                        }
                        VecN(&v1);                                      // �P���޸�ى�
                        wv[j] = v1;                                     // ���e�͈͓��W�v��
                        wf[j] = 2;                                      // ������
                        for(k=j+1; k<cnt; k++) {
                            if(wf[k]==1) {                              // <�����r���̏ꍇ>
                                wv[k] = v1;                             // ���e�͈͓��W�v��
                                wf[k] = 2;                              // ������
                            }
                        }
                    }
                }
                cnt = 0, j = fg[i];
                while(1) {
                    av[j] = wv[cnt++];                                  // �ܐ��޸�ٕ���
                    if(nx[j]==-1) {
                        break;
                    }
                    j = nx[j];
                }
            }
        }
        for(i=0; i<lnum; i++) {
            GetFlp1(op, i, &vno, &flg);                                 // ��ٰ�ߔԍ��擾
            if(fg[vno]<0) {                                             // <�Sٰ�ߏW�v>
                lv[i] = pv[vno];                                        // ��ٰ�ߖ@���޸��
            } else {                                                    // <��L�ȊO>
                v1 = pv[vno], j = fg[vno];
                while(1) {
                    if(i==ln[j]) {                                      // <ٰ�ߔԍ���v>
                        v1 = av[j]; break;
                    }
                    if(nx[j]==-1) {                                     // �޸�ْu��
                        break;
                    }
                    j = nx[j];
                }
                lv[i] = v1;                                             // ��ٰ�ߖ@���޸��
            }
        }
        SetObtype(op, lv);                                              // ���̏��ݒ�
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
static void SetObjFVec2(OBJTYPE* op, int vnum, int lnum)                //<<< �Ȗʖ@���޸�ِݒ�
/********************************************************/              //   (�ȖʁF�Ő���)
{
    PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
    VECTYPE* va = new VECTYPE[View->m_MaxSPnt];
    VECTYPE* lv = new VECTYPE[View->m_MaxSPnt*4+View->m_MaxCPnt*2];
    int*     pn = new int[View->m_MaxCPnt];
    VECTYPE  vc;
    int      i, j, k, cnt=0, vno, flg;

    InitFVec();                                                         // �޸�ِݒ菉������
    for(i=0; i<DIM; i++) {
        vc.p[i] = 0.0;                                                  // �޸�ُ�����
    }
    for(i=0; i<vnum; i++) {
        va[i] = vc;
    }
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // ��ٰ�ߔԍ��擾
        pa[cnt] = Pdat[Vtnum+vno];                                      // ���_���W�ۑ�
        pn[cnt++] = vno;                                                // ���_�ԍ��ۑ�
        SetLptype(vno, pa[cnt-1]);                                      // ��ٰ�ߏ��ݒ�
        if(flg==1) {                                                    // <<<ٰ�ߍŏI�̏ꍇ>>>
            CalcNVec(pa, cnt, &vc);                                     // �@���޸�َ擾
            SetFatype(vc);                                              // �ʏ��ݒ�
            for(j=0; j<cnt; j++) {
                for(k=0; k<DIM; k++) { 
                    va[pn[j]].p[k] += vc.p[k];                          // �Ώے��_�޸�ى��Z
                }
            }
            cnt = 0;
        }
    }
    for(i=0; i<vnum; i++) {
        VecN(&va[i]);                                                   // �P���޸�ى�
    }
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // ��ٰ�ߔԍ��擾
        lv[i] = va[vno];                                                // ��ٰ�ߖ@���޸��
    }
    SetObtype(op, lv);                                                  // ���̏��ݒ�
    delete[] pa;
    delete[] va;
    delete[] lv;
    delete[] pn;
}

/**********************************************/
static void SetObjFVec3(OBJTYPE* op, int lnum)                          //<<< �ʖ@���޸�ِݒ�
/**********************************************/                        //   (��ζ�ėp)
{
    PNTTYPE* pa = new PNTTYPE[View->m_MaxCPnt];
    VECTYPE* lv = new VECTYPE[View->m_MaxSPnt*4+View->m_MaxCPnt*2];
    VECTYPE  nvc;
    int      i, j, cnt=0, stn=0, vno, flg;

    InitFVec();                                                         // �޸�ِݒ菉������
    for(i=0; i<lnum; i++) {
        GetFlp1(op, i, &vno, &flg);                                     // ��ٰ�ߔԍ��擾
        pa[cnt++] = Pdat[Vtnum+vno];                                    // ���_���W�ۑ�
        SetLptype(vno, pa[cnt-1]);                                      // ��ٰ�ߏ��ݒ�
        if(flg==1) {                                                    // <<<ٰ�ߍŏI�̏ꍇ>>>
            CalcNVec(pa, cnt, &nvc);                                    // �@���޸�َ擾
            SetFatype(nvc);                                             // �ʏ��ݒ�
            for(j=0; j<cnt; j++) {
                lv[stn+j] = nvc;                                        // ��ٰ�ߒP�ʂɐݒ�
            }
            stn = i+1, cnt = 0;                                         // ��ٰ�ߊJ�n�ԍ�
        }
    }
    SetObtype(op, lv);                                                  // ���̏��ݒ�
    delete[] pa;
    delete[] lv;
}

/*************************************/
static void SetStageFVec(PNTTYPE* pa)                                   //<<< �ð�ޖ@���޸�ِݒ�
/*************************************/
{
    VECTYPE* lv = new VECTYPE[24];
    OBJTYPE  op;
    PNTTYPE  pt[4];
    VECTYPE  nvc;
    int      i;

    op.color = 12;                                                      // �װ(�\�ʑ���):�ð��
    for(i=0; i<8; i++) {
        Pdat[Vtnum+i] = pa[i];                                          // ���_�o�^
    }
    InitFVec();                                                         // �޸�ِݒ菉������
    SetLptype(0, pa[0]), pt[0] = pa[0];                                 // ��ٰ�ߏ��ݒ�
    SetLptype(1, pa[1]), pt[1] = pa[1];
    SetLptype(2, pa[2]), pt[2] = pa[2];
    SetLptype(3, pa[3]), pt[3] = pa[3];
    CalcNVec(pt, 4, &nvc);                                              // �@���޸�َ擾
    SetFatype(nvc);                                                     // �ʏ��ݒ�
    for(i=0; i<4; i++) {
        lv[i+0] = nvc;                                                  // ��ٰ�ߒP�ʂɐݒ�
    }
    SetLptype(4, pa[4]), pt[0] = pa[4];                                 // ��ٰ�ߏ��ݒ�
    SetLptype(7, pa[7]), pt[1] = pa[7];
    SetLptype(6, pa[6]), pt[2] = pa[6];
    SetLptype(5, pa[5]), pt[3] = pa[5];
    CalcNVec(pt, 4, &nvc);                                              // �@���޸�َ擾
    SetFatype(nvc);                                                     // �ʏ��ݒ�
    for(i=0; i<4; i++) {
        lv[i+4] = nvc;                                                  // ��ٰ�ߒP�ʂɐݒ�
    }
    SetLptype(0, pa[0]), pt[0] = pa[0];                                 // ��ٰ�ߏ��ݒ�
    SetLptype(3, pa[3]), pt[1] = pa[3];
    SetLptype(7, pa[7]), pt[2] = pa[7];
    SetLptype(4, pa[4]), pt[3] = pa[4];
    CalcNVec(pt, 4, &nvc);                                              // �@���޸�َ擾
    SetFatype(nvc);                                                     // �ʏ��ݒ�
    for(i=0; i<4; i++) {
        lv[i+8] = nvc;                                                  // ��ٰ�ߒP�ʂɐݒ�
    }
    SetLptype(1, pa[1]), pt[0] = pa[1];                                  // ��ٰ�ߏ��ݒ�
    SetLptype(5, pa[5]), pt[1] = pa[5];
    SetLptype(6, pa[6]), pt[2] = pa[6];
    SetLptype(2, pa[2]), pt[3] = pa[2];
    CalcNVec(pt, 4, &nvc);                                              // �@���޸�َ擾
    SetFatype(nvc);                                                     // �ʏ��ݒ�
    for(i=0; i<4; i++) {
        lv[i+12] = nvc;                                                 // ��ٰ�ߒP�ʂɐݒ�
    }
    SetLptype(0, pa[0]), pt[0] = pa[0];                                 // ��ٰ�ߏ��ݒ�
    SetLptype(4, pa[4]), pt[1] = pa[4];
    SetLptype(5, pa[5]), pt[2] = pa[5];
    SetLptype(1, pa[1]), pt[3] = pa[1];
    CalcNVec(pt, 4, &nvc);                                              // �@���޸�َ擾
    SetFatype(nvc);                                                     // �ʏ��ݒ�
    for(i=0; i<4; i++) {
        lv[i+16] = nvc;                                                 // ��ٰ�ߒP�ʂɐݒ�
    }
    SetLptype(3, pa[3]), pt[0] = pa[3];                                 // ��ٰ�ߏ��ݒ�
    SetLptype(2, pa[2]), pt[1] = pa[2];
    SetLptype(6, pa[6]), pt[2] = pa[6];
    SetLptype(7, pa[7]), pt[3] = pa[7];
    CalcNVec(pt, 4, &nvc);                                              // �@���޸�َ擾
    SetFatype(nvc);                                                     // �ʏ��ݒ�
    for(i=0; i<4; i++) {
        lv[i+20] = nvc;                                                 // ��ٰ�ߒP�ʂɐݒ�
    }
    SetObtype(&op, lv);                                                 // ���̏��ݒ�
    Vtnum += 8;
    delete[] lv;
}

/***************************/
static void DoScaling(void)                                             //<<< �\������޳�ཹ��ݸ�
/***************************/
{
    OBTYPE* obt;
    FATYPE* fat;
    int     i, j;

    for(i=0; i<Vtnum; i++) {
        for(j=0; j<DIM; j++) {
            Pdat[i].p[j] *= Scale;                                      // ���_���W�𽹰�ݸ�
        }
    }
    obt = Olist;
    while(obt!=NULL) {
        for(i=0; i<DIM; i++) {
            obt->cpt.p[i] *= Scale;                                     // ���̒��S�_�𽹰�ݸ�
        }
        obt->r *= Scale;                                                // ���̍ŏ������a�𽹰�ݸ�
        for(i=0; i<8; i++) {
            for(j=0; j<DIM; j++) {
                obt->cp8[i].p[j] *= Scale;                              // 8�������S�_�𽹰�ݸ�
            }
            obt->r8[i] *= Scale;                                        // 8�����ŏ������a�𽹰�ݸ�
            fat = obt->fal[i];
            while(fat!=NULL) {
                for(j=0; j<DIM; j++) {
                    fat->cpt.p[j] *= Scale;                             // �ʒ��S�_�𽹰�ݸ�
                }
                fat->r *= Scale;                                        // �ʍŏ������a�𽹰�ݸ�
                fat = fat->next;
            }
        }
        obt = obt->next;
    }
    for(i=0; i<DIM; i++) {
        Eye.p[i] *= Scale;                                              // ���_���W�𽹰�ݸ�
    }
    for(i=0; i<LgtNum; i++) {
        if(Lgt[i].type==1) {                                            // �_�����̏ꍇ
            Lgt[i].x1 *= Scale, Lgt[i].y1 *= Scale, Lgt[i].z1 *= Scale; // �������W�𽹰�ݸ�
        }
    }
    LgtLeng = Scale*LLENGTH;                                            // ������{���𽹰�ݸ�
}

/******************************/
static BOOL MakeRendData(void)                                          //<<< �����ݸ��ް��쐬
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

    View->GetCentPsPnt(&ps);                                            // �Q�Ɠ_�擾
    View->GetEyePnt(&pe);                                               // ���_�擾
    View->GetEyeSvPnt(&pp);                                             // ���_SV�擾
    th = PI*15.0/180.0;                                                 // 15�x��׼ޱ�
    ln = PntD(&ps, &pe);                                                // ���_-�Q�Ɠ_����
    Scale = (double)(Szy-1)/(2.0*ln*tan(th));                           // ����ݸޔ{��
    for(i=0; i<DIM; i++) {
        EyeVec.p[i] = ps.p[i]-pp.p[i];                                  // �����޸��
        CentPs.p[i] = ps.p[i];                                          // �Q�Ɠ_(�����_)
        Eye.p[i]    = pe.p[i]-CentPs.p[i];                              // ���_(�Q�Ɠ_�����_)
    }
    VecN(&EyeVec);                                                      // �P���޸�ى�
    MakeRotateMatrix(0, &EyeVec, &matz, &matx);                         // ��]��د���쐬
    MoveVertex(&Eye, &matz);                                            // ���_��](Z)
    MoveVertex(&Eye, &matx);                                            // ���_��](X)
    GetObjBottom(btm);                                                  // �ð���߼޼�ݎ擾
    GetObjCent(oct);                                                    // �\�����̾����擾
    GetRayTraceEnv(&bk, &MaxLevel, &AntiAlias, &EdgeFlag, &i, sts);     // �����擾
    BkR = (double)GetRValue(bk)/255.0;                                  // �w�i�F(RGB)
    BkG = (double)GetGValue(bk)/255.0;
    BkB = (double)GetBValue(bk)/255.0;
    flg[0] = ((i&0x1)>0 ? 1 : 0);                                       // �ð�ޗL���׸�
    flg[1] = ((i&0x2)>0 ? 1 : 0);
    flg[2] = ((i&0x4)>0 ? 1 : 0);
    flg[3] = ((i&0x8)>0 ? 1 : 0);
    for(i=0; i<ATMAX; i++) {
        GetRayTraceColor(i, &Atr[i]);                                   // �\�ʑ������擾
    }
    for(i=LgtNum=0; i<LGMAX; i++) {
        GetRayTraceLight(i, &lt);                                       // �������擾
        if(lt.flg) {                                                    // �������L���̏ꍇ
            if(lt.type==1) {                                            // �_�����̏ꍇ
                pt.p[0] = lt.x1+oct[0]-CentPs.p[0];                     // �Q�Ɠ_�����_�Ƃ���
                pt.p[1] = lt.y1+oct[1]-CentPs.p[1];                     // �_�������W�ʒu
                pt.p[2] = lt.z1+oct[2]-CentPs.p[0];
                MoveVertex(&pt, &matz);                                 // �����ʒu��](Z)
                MoveVertex(&pt, &matx);                                 // �����ʒu��](X)
                lt.x1 = pt.p[0], lt.y1 = pt.p[1], lt.z1 = pt.p[2];
            } else {                                                    // ���s�����̏ꍇ
                vc.p[0] = lt.x2-lt.x1;
                vc.p[1] = lt.y2-lt.y1;
                vc.p[2] = lt.z2-lt.z1;
                VecN(&vc);                                              // �P���޸�ى�
                MoveVector(&vc, &matz);                                 // �����޸�ى�](Z)
                MoveVector(&vc, &matx);                                 // �����޸�ى�](X)
                VecN(&vc);                                              // �P���޸�ى�
                lt.x1 = vc.p[0], lt.y1 = vc.p[1], lt.z1 = vc.p[2];
            }
            Lgt[LgtNum++] = lt;
        }
    }
    Olist = NULL, Olast = NULL, Vtnum = 0;
    BaseObjPtr(1, &op, &i1, &i2);                                       // �\�������߲���擾
    if(op==NULL) {
        return FALSE;
    }
    while(op) {
        vnum = GetVtxNum(op);                                           // ���_���擾
        lnum = GetFlpNum1(op);                                          // ��ٰ�ߐ��擾
        for(i=0; i<vnum; i++) {
            GetVtx(op, i, &pt);                                         // ���_���W�l�擾
            for(j=0; j<DIM; j++) {
                pt.p[j] -= CentPs.p[j];
            }
            MoveVertex(&pt, &matz);                                     // ���_��](Z)
            MoveVertex(&pt, &matx);                                     // ���_��](X)
            Pdat[Vtnum+i] = pt;
        }
        if(GetObjAtr(op)!=1) {                                          // �Ȗʖ@���޸�ِݒ�
            if(EdgeFlag) {
                SetObjFVec1(op, vnum, lnum);                            // (�ȖʁF�Ő��p)
            } else {
                SetObjFVec2(op, vnum, lnum);                            // (�ȖʁF�Ő���)
            }
        } else {
            SetObjFVec3(op, lnum);                                      // (��ζ��)
        }
        Vtnum += vnum;
        NextObjPtr(1, &op, &i1, &i2);                                   // �������߲���擾
    }
    if(flg[0]||flg[1]||flg[2]||flg[3]) {                                // <�ð�ޗL�̏ꍇ>
        x = sts[0]/2.0, y = sts[1]/2.0;                                 // X/2.0, Y/2.0
        z = btm[2]-sts[3];                                              // ���Z�ʒu
        st[0].p[0] = btm[0]-x, st[0].p[1] = btm[1]-y, st[0].p[2] = z;   // �ð�ޗp���_
        st[1].p[0] = btm[0]-x, st[1].p[1] = btm[1]+y, st[1].p[2] = z;
        st[2].p[0] = btm[0]+x, st[2].p[1] = btm[1]-y, st[2].p[2] = z;
        st[3].p[0] = btm[0]+x, st[3].p[1] = btm[1]+y, st[3].p[2] = z;
        z += sts[2];                                                    // �w�ʥ���ʍ���
        st[4].p[0] = btm[0]-x, st[4].p[1] = btm[1]-y, st[4].p[2] = z;
        st[5].p[0] = btm[0]-x, st[5].p[1] = btm[1]+y, st[5].p[2] = z;
        st[6].p[0] = btm[0]+x, st[6].p[1] = btm[1]-y, st[6].p[2] = z;
        st[7].p[0] = btm[0]+x, st[7].p[1] = btm[1]+y, st[7].p[2] = z;
        if(flg[0]) {                                                    // <���>
            pa[0] = st[0], pa[1] = st[2], pa[2] = st[3], pa[3] = st[1];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i+4].p[2] -= 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // �Q�Ɠ_�����_
                }
                MoveVertex(&pa[i], &matz);                              // ���_��](Z)
                MoveVertex(&pa[i], &matx);                              // ���_��](X)
            }
            SetStageFVec(pa);                                           // �ð�ޖ@���޸�ِݒ�
        }
        if(flg[1]) {                                                    // <�w��>
            pa[0] = st[0], pa[1] = st[1], pa[2] = st[5], pa[3] = st[4];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i].p[0] += 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // �Q�Ɠ_�����_
                }
                MoveVertex(&pa[i], &matz);                              // ���_��](Z)
                MoveVertex(&pa[i], &matx);                              // ���_��](X)
            }
            SetStageFVec(pa);                                           // �ð�ޖ@���޸�ِݒ�
        }
        if(flg[2]) {                                                    // <�E����>
            pa[0] = st[1], pa[1] = st[3], pa[2] = st[7], pa[3] = st[5];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i].p[1] -= 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // �Q�Ɠ_�����_
                }
                MoveVertex(&pa[i], &matz);                              // ���_��](Z)
                MoveVertex(&pa[i], &matx);                              // ���_��](X)
            }
            SetStageFVec(pa);                                           // �ð�ޖ@���޸�ِݒ�
        }
        if(flg[3]) {                                                    // <������>
            pa[0] = st[0], pa[1] = st[4], pa[2] = st[6], pa[3] = st[2];
            for(i=0; i<4; i++) {
                pa[i+4] = pa[i], pa[i].p[1] += 1.0;
            }
            for(i=0; i<8; i++) {
                for(j=0; j<DIM; j++) {
                    pa[i].p[j] -= CentPs.p[j];                          // �Q�Ɠ_�����_
                }
                MoveVertex(&pa[i], &matz);                              // ���_��](Z)
                MoveVertex(&pa[i], &matx);                              // ���_��](X)
            }
            SetStageFVec(pa);                                           // �ð�ޖ@���޸�ِݒ�
        }
    }
    DoScaling();                                                        // �\������޳�ཹ��ݸ�
    return TRUE;
}

/******************************************************************/
static BOOL CalcCrossLine(PNTTYPE* p1, PNTTYPE* p2, VECTYPE* v1,
                          VECTYPE* v2, PNTTYPE* crs, VECTYPE* nvc)      //<<< �������v�Z
/******************************************************************/
{
    PNTTYPE pmin, pmax;
    VECTYPE vmin, vmax;
    double  pr;
    int     i;

    if(p1->p[0]<p2->p[0]) {                                             // X�����ɓ��ւ�
        pmin = *p1, pmax = *p2, vmin = *v1, vmax = *v2;
    } else {
        pmin = *p2, pmax = *p1, vmin = *v2, vmax = *v1;
    }
    if(pmin.p[0]<=0.0&&pmax.p[0]>0.0) {                                 // X��0�_��ʉ߂���ꍇ
        pr = pmax.p[0]/(pmax.p[0]-pmin.p[0]);                           // 0�_�܂ł̔䗦
        for(i=0; i<DIM; i++) {
            crs->p[i] = pmax.p[i]+(pmin.p[i]-pmax.p[i])*pr;             // 0�_�ʒu�̒��_
            nvc->p[i] = vmax.p[i]+(vmin.p[i]-vmax.p[i])*pr;             // 0�_�ʒu���޸��
        }
        VecN(nvc);                                                      // �P���޸�ى�
        return TRUE;
    } else {
        return FALSE;
    }
}

/*****************************************************************/
static BOOL CalcCrossFace(FATYPE* fat, PNTTYPE* scr, RMAT* matz,
                          RMAT* matx, PNTTYPE* crs, VECTYPE* nvc)       //<<< �����ʌv�Z
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
    spt = Pdat[wlp->vno];                                               // �J�n�_
    for(i=0; i<DIM; i++) {
        spt.p[i] -= scr->p[i];                                          // ��ʒu�����_
    }
    MoveVertex(&spt, matz);                                             // ���_��](Z)
    MoveVertex(&spt, matx);                                             // ���_��](X)
    svc = wlp->vec;                                                     // �J�n�_�޸��
    MoveVector(&svc, matz);                                             // �J�n�_�޸�ى�](Z)
    MoveVector(&svc, matx);                                             // �J�n�_�޸�ى�](X)
    bpt = spt, bvc = svc;                                               // �J�n�_�𒼑O�_
    while(wlp->next!=NULL) {
        wlp = wlp->next;
        cpt = Pdat[wlp->vno];                                           // ����_
        for(i=0; i<DIM; i++) {
            cpt.p[i] -= scr->p[i];                                      // ��ʒu�����_
        }
        MoveVertex(&cpt, matz);                                         // ���_��](Z)
        MoveVertex(&cpt, matx);                                         // ���_��](X)
        cvc = wlp->vec;                                                 // ����_�޸��
        MoveVector(&cvc, matz);                                         // �޸�ى�](Z)
        MoveVector(&cvc, matx);                                         // �޸�ى�](X)
        if(CalcCrossLine(&bpt, &cpt, &bvc, &cvc, &crs_p, &crs_v)) {     // �������v�Z
            if(crs_p.p[1]>=0.0) {
                pls++;
                if(pls_l>crs_p.p[1]) {
                    pls_p = crs_p, pls_v = crs_v, pls_l = crs_p.p[1];   // �ł�0�ɋ߂��ް��ۑ�
                }
            } else {
                mis++;
                if(mis_l<crs_p.p[1]) {
                    mis_p = crs_p, mis_v = crs_v, mis_l = crs_p.p[1];   // �ł�0�ɋ߂��ް��ۑ�
                }
            }
        }
        bpt = cpt, bvc = cvc;                                           // ����_�𒼑O�_
    }
    if(CalcCrossLine(&bpt, &spt, &bvc, &svc, &crs_p, &crs_v)) {         // �������v�Z
        if(crs_p.p[1]>=0.0) {
            pls++;
            if(pls_l>crs_p.p[1]) {
                pls_p = crs_p, pls_v = crs_v, pls_l = crs_p.p[1];       // �ł�0�ɋ߂��ް��ۑ�
            }
        } else {
            mis++;
            if(mis_l<crs_p.p[1]) {
                mis_p = crs_p, mis_v = crs_v, mis_l = crs_p.p[1];       // �ł�0�ɋ߂��ް��ۑ�
            }
        }
    }
    if(pls%2==1&&mis%2==1) {
        pr = pls_p.p[1]/(pls_p.p[1]-mis_p.p[1]);                        // 0�_�܂ł̔䗦
        for(i=0; i<DIM; i++) {
            crs->p[i] = pls_p.p[i]+(mis_p.p[i]-pls_p.p[i])*pr;          // 0�_�ʒu�̒��_
            nvc->p[i] = pls_v.p[i]+(mis_v.p[i]-pls_v.p[i])*pr;          // 0�_�ʒu���޸��
        }
        VecN(nvc);                                                      // �P���޸�ى�
        return TRUE;
    } else {
        return FALSE;
    }
}

/*******************************************************************/
static BOOL GetCrossPoint(PNTTYPE* eye, PNTTYPE* scr, PNTTYPE* crs,
                          VECTYPE* nvc, VECTYPE* fnv, int* ano)         //<<< ���̌�_�Z�o
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
        wvc.p[i] = scr->p[i]-eye->p[i];                                 // ���_->�`��ʒu�޸��
        weye.p[i] = eye->p[i]-scr->p[i];                                // ���_(�`��_�����_)
    }
    VecN(&wvc);                                                         // �P���޸�ى�
    MakeRotateMatrix(0, &wvc, &matz, &matx);                            // ��]��د���쐬
    MoveVertex(&weye, &matz);                                           // ���_��](Z)
    MoveVertex(&weye, &matx);                                           // ���_��](X)
    obt = Olist;
    while(obt!=NULL) {
        pnt = obt->cpt;                                                 // �ŏ��ɗ��̒��S�_������
        for(i=0; i<DIM; i++) {
            pnt.p[i] -= scr->p[i];                                      // �`��ʒu�����_�Ƃ���
        }
        MoveVertex(&pnt, &matz);                                        // ���_��](Z)
        MoveVertex(&pnt, &matx);                                        // ���_��](X)
        ln = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]);                 // XY���ʏ��(0,0)����̋���
        if(ln<obt->r+EPS_R&&pnt.p[2]-obt->r<weye.p[2]+EPS_R) {
            for(i=0; i<8; i++) {
                pnt = obt->cp8[i];                                      // ����8�������S�_������
                for(j=0; j<DIM; j++) {
                    pnt.p[j] -= scr->p[j];                              // �`��ʒu�����_�Ƃ���
                }
                MoveVertex(&pnt, &matz);                                // ���_��](Z)
                MoveVertex(&pnt, &matx);                                // ���_��](X)
                ln = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]);         // XY���ʏ��(0,0)����̋���
                if(ln<obt->r8[i]+EPS_R&&pnt.p[2]-obt->r8[i]<weye.p[2]+EPS_R) {
                    fat = obt->fal[i];
                    while(fat!=NULL) {
                        pnt = fat->cpt;
                        for(j=0; j<DIM; j++) {
                            pnt.p[j] -= scr->p[j];                      // �`��ʒu�����_�Ƃ���
                        }
                        MoveVertex(&pnt, &matz);                        // ���_��](Z)
                        MoveVertex(&pnt, &matx);                        // ���_��](X)
                        ln = sqrt(pnt.p[0]*pnt.p[0]+pnt.p[1]*pnt.p[1]); // XY���ʏ��(0,0)����̋���
                        if(ln<fat->r+EPS_R&&pnt.p[2]-fat->r<weye.p[2]+EPS_R) {
                            if(CalcCrossFace(fat, scr, &matz, &matx, &wcrs, &wvec)) {
                                if(weye.p[2]>wcrs.p[2]) {
                                    cnt++;
                                    if(min>weye.p[2]-wcrs.p[2]) {       // ���_����̋���
                                        min = weye.p[2]-wcrs.p[2];      // ���_�ɍł��߂�����
                                        *nvc = wvec;                    // �אږʂ����������@���޸��
                                        *fnv = fat->vec;                // �ʖ@���޸��
                                        san = obt->ano;                 // �\�ʑ���No�ۑ�
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
            crs->p[i] = eye->p[i]+min*wvc.p[i];                         // ��_
        }
        MakeRotateMatrix(1, &wvc, &matz, &matx);                        // ��]��د���쐬(�߂�)
        MoveVector(nvc, &matx);                                         // �@���޸�ى�](X)
        MoveVector(nvc, &matz);                                         // �@���޸�ى�](Z)
        VecN(nvc);                                                      // �P���޸�ى�
        *ano = san;                                                     // �\�ʑ���No
        return TRUE;
    } else {
        return FALSE;
    }
}

/*******************************************************************/
static void GetLight(int ix, PNTTYPE* crs, VECTYPE* lvc, double* r,
                     double* g, double* b, double* dist)                //<<< �������擾
/*******************************************************************/
{
    double p,l;

    if(Lgt[ix].type==0) {                                               // ���s�����̏ꍇ
        lvc->p[0] = -Lgt[ix].x1;                                        // �����޸��
        lvc->p[1] = -Lgt[ix].y1;
        lvc->p[2] = -Lgt[ix].z1;
        *r = (double)GetRValue(Lgt[ix].lc)/255.0;                       // ����RGB
        *g = (double)GetGValue(Lgt[ix].lc)/255.0;
        *b = (double)GetBValue(Lgt[ix].lc)/255.0;
        *dist = DBL_MAX;
    } else if(Lgt[ix].type==1) {                                        // �_�����̏ꍇ
        lvc->p[0] = Lgt[ix].x1-crs->p[0];                               // �����޸��
        lvc->p[1] = Lgt[ix].y1-crs->p[1];
        lvc->p[2] = Lgt[ix].z1-crs->p[2];
        l = sqrt(lvc->p[0]*lvc->p[0]+lvc->p[1]*lvc->p[1]+lvc->p[2]*lvc->p[2]);
        if(l<1.e-7) {
            l = 1.0; 
        }
        if(Lgt[ix].k==0) {                                              // �����Ȃ�
            p = 1.0;
        } else if(Lgt[ix].k==1) {                                       // ������1��Ō���
            p = LgtLeng/(l+LgtLeng);
        } else if(Lgt[ix].k==2) {                                       // ������2��Ō���
            p = LgtLeng/(l*l+LgtLeng);
        }
        *r = ((double)GetRValue(Lgt[ix].lc)/255.0)*p;                   // ����RGB
        *g = ((double)GetGValue(Lgt[ix].lc)/255.0)*p;
        *b = ((double)GetBValue(Lgt[ix].lc)/255.0)*p;
        *dist = l;
        VecN(lvc);                                                      // �P���޸�ى�
    }
}

/**********************************************************/
static void Shadow(PNTTYPE* crs, VECTYPE* lvc, double dst,
                   double* sr, double* sg, double* sb)                  //<<< �e�̌v�Z
/**********************************************************/
{
    PNTTYPE pt1, pt2, pnt;
    VECTYPE wnv, wfn;
    int     i, ano;

    *sr = 1.0, *sg = 1.0, *sb = 1.0;
    for(i=0; i<DIM; i++) {
        pt1.p[i] = crs->p[i]+lvc->p[i]*Scale*0.05;                      // ��_��\�ʂ��痣��
        pt2.p[i] = crs->p[i]+lvc->p[i]*Scale*100.0;                     // ���������̉����_
    }
    if(GetCrossPoint(&pt1, &pt2, &pnt, &wnv, &wfn, &ano)) {             // ���̌�_�Z�o
        if(PntD(crs, &pnt)<dst) {                                       // �����Ƃ̊Ԃɗ��̗L
            *sr = (double)GetRValue(Atr[ano].ct)/255.0;                 // ���ߒ����W��
            *sg = (double)GetGValue(Atr[ano].ct)/255.0;
            *sb = (double)GetBValue(Atr[ano].ct)/255.0;
        }
    }
}

/****************************************************/
static double HighLight1(int ano, VECTYPE* lvc,                         //<<< ʲײĂ̌v�Z
                         VECTYPE* vvc, VECTYPE* nvc)                    //   (Phong Model)
/****************************************************/
{
    double rx, ry, rz, rv, ln, rvn=0.0;
    int    i, m;

    ln = 2*(lvc->p[0]*nvc->p[0]+lvc->p[1]*nvc->p[1]+lvc->p[2]*nvc->p[2]);
    rx = ln*nvc->p[0]-lvc->p[0];
    ry = ln*nvc->p[1]-lvc->p[1];
    rz = ln*nvc->p[2]-lvc->p[2];
    rv = rx*vvc->p[0]+ry*vvc->p[1]+rz*vvc->p[2];                        // Cos��
    if(rv>=0.0) {
        m = Atr[ano].m1;                                                // ʲײĂ̍L����
        for(rvn=1.0, i=0; i<m; i++) {
            rvn *= rv;                                                  // Cos��**N
        }
    }
    return rvn;
}

/****************************************************/
static double HighLight2(int ano, VECTYPE* lvc,                         //<<< ʲײĂ̌v�Z
                         VECTYPE* vvc, VECTYPE* nvc)                    //   (Cook-Torrance Model)
/****************************************************/
{
    VECTYPE hvc;
    double  cos1, cos2, tan2, a, m=0.0, rvn=0.0;
    int     i;

    for(i=0; i<DIM; i++) {
        hvc.p[i] = lvc->p[i]+vvc->p[i];                                 // lvc��vvc�̘a
    }
    VecN(&hvc);                                                         // �P���޸�ى�
    cos1 = hvc.p[0]*nvc->p[0]+hvc.p[1]*nvc->p[1]+hvc.p[2]*nvc->p[2];    // Cos��
    cos2 = cos1*cos1;
    if(cos2<0.0000001) {
        cos2 = 0.0000001;
    }
    tan2 = 1.0/cos2-1.0;
    m = (Atr[ano].m2>0.0000001) ? (double)Atr[ano].m2 : 0.0000001;      // ʲײĂ̍L����
    a = -tan2/(m*m);
    rvn = exp(a)/(m*m*cos2*cos2);
    return rvn;
}

/**********************************************************************/
static void Shading(int ano, PNTTYPE* crs, VECTYPE* vvc, VECTYPE* nvc,
                    double n, double* r, double* g, double* b)          //<<< ����ިݸ�
/**********************************************************************/
{
    VECTYPE lvc;
    double  kdr, kdg, kdb, ksr, ksg, ksb, ir, ig, ib;
    double  dist, ln, sr, sg, sb, rvn=0.0, cs, w1, w2, w3, n1, n2, n3;
    int     i;

    *r  = (double)GetRValue(Atr[ano].ka)/255.0;                         // ����
    *g  = (double)GetGValue(Atr[ano].ka)/255.0;
    *b  = (double)GetBValue(Atr[ano].ka)/255.0;
    kdr = (double)GetRValue(Atr[ano].kd)/255.0;                         // �g�U���ˌW��
    kdg = (double)GetGValue(Atr[ano].kd)/255.0;
    kdb = (double)GetBValue(Atr[ano].kd)/255.0;
    ksr = (double)GetRValue(Atr[ano].ks)/255.0;                         // ���ʔ��ˌW��
    ksg = (double)GetGValue(Atr[ano].ks)/255.0;
    ksb = (double)GetBValue(Atr[ano].ks)/255.0;
    for(i=0; i<LgtNum; i++) {
        GetLight(i, crs, &lvc, &ir, &ig, &ib, &dist);                   // �������擾
        ln = lvc.p[0]*nvc->p[0]+lvc.p[1]*nvc->p[1]+lvc.p[2]*nvc->p[2];  // Cos��
        if(ln<0) {
            ln = 0.0;
        } else {
            Shadow(crs, &lvc, dist, &sr, &sg, &sb);                     // �e�̌v�Z
            ir *= sr, ig *= sg, ib *= sb;
        }
        if(!Atr[ano].model) {                                           // <Phong����>
            if(Atr[ano].m1>0) {                                         // ʲײėL��
                rvn = HighLight1(ano, &lvc, vvc, nvc);                  // Phong Model
            }
        } else {                                                        // <Cook-Torrance����>
            if(Atr[ano].m2>0.0) {                                       // ʲײėL��
                rvn = HighLight2(ano, &lvc, vvc, nvc);                  // Cook-Torrance Model
                cs = vvc->p[0]*nvc->p[0]+vvc->p[1]*nvc->p[1]+vvc->p[2]*nvc->p[2];
                if(cs<0.0) {
                    cs = 0.0;
                }
                n1 = Atr[ano].nr, n2 = Atr[ano].ng, n3 = Atr[ano].nb;   // ���ʔ��ˌW���v�Z
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
        *r += ir*(kdr*ln+ksr*rvn);                                      // �\�ʋP�x(RGB)
        *g += ig*(kdg*ln+ksg*rvn);
        *b += ib*(kdb*ln+ksb*rvn);
    }
}

/*********************************************************/
static void RayTrace(int lvl, PNTTYPE* eye, PNTTYPE* scr,
                     double* r, double* g, double* b)                   //<<< �����ǐՏ���
/*********************************************************/
{
    PNTTYPE crs, p1, p2;
    VECTYPE nvc, fnv, evc, vvc, wnv, tvc, rvc;
    int     i, ano;
    double  ksr, ksg, ksb, ctr, ctg, ctb, tr, tg, tb, rr, rg, rb;
    double  sign, n1, n2, cs1, cs2, a, ks1, ks2, ct1, en;

    if(GetCrossPoint(eye, scr, &crs, &nvc, &fnv, &ano)) {               // ��_��@���޸�َZ�o
        for(i=0; i<DIM; i++) {
            evc.p[i] = scr->p[i]-eye->p[i];                             // �����޸��
            vvc.p[i] = eye->p[i]-scr->p[i];                             // �����޸��(�t)
        }
        VecN(&evc);                                                     // �P���޸�ى�
        VecN(&vvc);
        n1 = Atr[ano].n;                                                // ���ܗ�
        sign = evc.p[0]*fnv.p[0]+evc.p[1]*fnv.p[1]+evc.p[2]*fnv.p[2];   // �����Ɩʖ@���̔�r
        if(sign<=0) {                                                   // <�\��>
            for(i=0; i<DIM; i++) {
                wnv.p[i] = nvc.p[i];                                    // ��_�@���޸�ق͂��̂܂�
            }
            n2 = n1;                                                    // ���΋��ܗ�(��)
        } else {                                                        // <����>
            for(i=0; i<DIM; i++) {
                wnv.p[i] = -1.0*nvc.p[i];                               // ��_�@���޸�ق͋t�]
            }
            n2 = 1.0/n1;                                                // ���΋��ܗ�(�o)
        }
        Shading(ano, &crs, &vvc, &wnv, n1, r, g, b);                    // ����ިݸ�
        if(Atr[ano].trace&&lvl<MaxLevel) {                              // <�ő压���ǐ����ٖ���>
            ksr = (double)GetRValue(Atr[ano].ks)/255.0;                 // ���ʔ��ˌW��
            ksg = (double)GetGValue(Atr[ano].ks)/255.0;
            ksb = (double)GetBValue(Atr[ano].ks)/255.0;
            ctr = (double)GetRValue(Atr[ano].ct)/255.0;                 // ���ߒ����W��
            ctg = (double)GetGValue(Atr[ano].ct)/255.0;
            ctb = (double)GetBValue(Atr[ano].ct)/255.0;
            if(ctr+ctg+ctb>0.01) {                                      // <����(����)>
                cs1 = vvc.p[0]*wnv.p[0]+vvc.p[1]*wnv.p[1]+vvc.p[2]*wnv.p[2];
                cs2 = 1.0-(1.0-cs1*cs1)/(n2*n2);
                if(cs2<0.0) {
                    ksr = 1.0, ksg = 1.0, ksb = 1.0;                    // ���S����
                    for(i=0; i<DIM; i++) {
                        tvc.p[i] = -1.0*wnv.p[i];
                    }
                } else {
                    cs2 = sqrt(cs2), a = n2*cs2-cs1;
                    ks1 = a/(n2*cs2+cs1), ks1 *= ks1;
                    ks2 = (n2*cs1-cs2)/(n2*cs1+cs2), ks2 *= ks2;
                    ks1 = 0.5*(ks1+ks2);                                // ���ˌW��
                    ct1 = 1.0-ks1;                                      // ���ߌW��
                    tvc.p[0] = (evc.p[0]-wnv.p[0]*a)/n2;                // �����޸��
                    tvc.p[1] = (evc.p[1]-wnv.p[1]*a)/n2;
                    tvc.p[2] = (evc.p[2]-wnv.p[2]*a)/n2;
                    if(ct1>0.1) {
                        VecN(&tvc);                                     // �P���޸�ى�
                        for(i=0; i<DIM; i++) {
                            p1.p[i] = crs.p[i]+tvc.p[i]*Scale*0.05;     // ��_��\�ʂ��痣��
                            p2.p[i] = crs.p[i]+tvc.p[i]*Scale*100.0;    // �����޸�ق̉����_
                        }
                        RayTrace(lvl+1, &p1, &p2, &tr, &tg, &tb);       // �����ǐՏ���
                        *r += ((double)GetRValue(Atr[ano].ct)/255.0)*ct1*tr;// ���ߏ����Z
                        *g += ((double)GetGValue(Atr[ano].ct)/255.0)*ct1*tg;
                        *b += ((double)GetBValue(Atr[ano].ct)/255.0)*ct1*tb;
                    }
                    ksr = ks1, ksg = ks1, ksb = ks1;
                }
            }
            if(ksr+ksg+ksb>0.1) {                                       // <����>
                en = 2*(evc.p[0]*wnv.p[0]+evc.p[1]*wnv.p[1]+evc.p[2]*wnv.p[2]);
                rvc.p[0] = evc.p[0]-wnv.p[0]*en;                        // ���ʔ����޸��
                rvc.p[1] = evc.p[1]-wnv.p[1]*en;
                rvc.p[2] = evc.p[2]-wnv.p[2]*en;
                VecN(&rvc);                                             // �P���޸�ى�
                for(i=0; i<DIM; i++) {
                    p1.p[i] = crs.p[i]+rvc.p[i]*Scale*0.05;             // ��_��\�ʂ��痣��
                    p2.p[i] = crs.p[i]+rvc.p[i]*Scale*100.0;            // ���ʔ����޸�ق̉����_
                }
                RayTrace(lvl+1, &p1, &p2, &rr, &rg, &rb);               // �����ǐՏ���
                *r += ksr*rr, *g += ksg*rg, *b += ksb*rb;               // ���ʔ��ˏ����Z
            }
        }
    } else {
        *r = BkR, *g = BkG, *b = BkB;                                   // ��_�Ȃ��͔w�i�F
    }
}

/*******************************/
static void InitAntiAlias(void)                                         //<<< �����ر�ݸޏ�������
/*******************************/
{
    int i, j;

    for(i=0; i<2; i++) {
        for(j=0; j<Szx; j++) {
            ScanLine[i][j].r = -1.0;                                    // ����ײ�RGB������
            ScanLine[i][j].g = -1.0;
            ScanLine[i][j].b = -1.0;
        }
    }
    CalcMat[0][0] = -1, CalcMat[1][0] = -1;                             // �����ر�ݸނ̎����ړ��ʐݒ�
    CalcMat[0][1] =  1, CalcMat[1][1] = -1;
    CalcMat[0][2] = -1, CalcMat[1][2] =  1;
    CalcMat[0][3] =  1, CalcMat[1][3] =  1;
}

/***********************************************************/
static int CheckSub(RGBT rgb, double r, double g, double b)             //<<< �װ�������
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
                      double r, double g, double b)                     //<<< �װ����
/***************************************************/
{
    int chk=0;

    if(x>0) {
        chk = CheckSub(ScanLine[y1][x-1], r, g, b);                     // ����s�̍�������
        if(!chk) {
            chk = CheckSub(ScanLine[y2][x-1], r, g, b);                 // �O�s�̍�������
        }
    }
    if(!chk) {
        chk = CheckSub(ScanLine[y2][x], r, g, b);                       // �O�s�̌��݈ʒu����
    }
    return chk;
}

/******************************/
static void DrawRendData(void)                                          //<<< �����ݸ��ް��`��
/******************************/
{
    PNTTYPE scr, wsc;
    int     i, x, y, y1, y2, ir, ig, ib;
    double  r, g, b, sr, sg, sb, e1, e2, tn, sv;

    if(AntiAlias) {
        InitAntiAlias();                                                // �����ر�ݸޏ�������
    }
    scr.p[2] = 0.0, wsc.p[2] = 0.0;                                     // Z�`��ʒu
    for(y=0; y<Szy; y++) {
        if(Rend->CheckStpSW()) {                                        // ���~SW:ON�͒��~
            break;
        }
        y1 = y%2, y2 = (y+1)%2;                                         // y1:����s y2:�O�s
        scr.p[1] = (double)(Szy/2-y);                                   // Y�`��ʒu
        for(x=0; x<Szx; x++) {
            scr.p[0] = (double)(x-Szx/2);                               // X�`��ʒu
            RayTrace(1, &Eye, &scr, &r, &g, &b);                        // �����ǐՏ���
            if(AntiAlias&&CheckColor(y1, y2, x, r, g, b)) {             // �����ر�ݸގ��s
                sr = r, sg = g, sb = b;                                 // �ʏ��RGB��ݒ�
                for(i=0; i<4; i++) {
                    wsc.p[0] = scr.p[0]+(double)CalcMat[0][i]*MOVEDST;  // X�`��ʒu
                    wsc.p[1] = scr.p[1]+(double)CalcMat[1][i]*MOVEDST;  // Y�`��ʒu
                    RayTrace(1, &Eye, &wsc, &r, &g, &b);                // �����ǐՏ���
                    sr += r, sg += g, sb += b;                          // �����ر�ݸޗpRGB���Z
                }
                r = sr/5.0, g = sg/5.0, b = sb/5.0;                     // RGB�̕��ω�
            }
            if(AntiAlias) {
                ScanLine[y1][x].r = r;                                  // �����ر�ݸގ���
                ScanLine[y1][x].g = g;                                  // RGB��ۑ�
                ScanLine[y1][x].b = b;
            }
            if(r<0.0) {
                r = 0.0;                                                // R��0.0-1.0�ɕ␳
            }
            if((sv=r)>1.0) {
                sv = 1.0;
            }
            e1 = exp(r), e2 = exp(-r);
            tn = (e1-e2)/(e1+e2), tn = tn+(sv-tn)*0.55;
            ir = (int)(tn*255.0);
            if(g<0.0) {
                g = 0.0;                                                // G��0.0-1.0�ɕ␳
            }
            if((sv=g)>1.0) {
                sv = 1.0;
            }
            e1 = exp(g), e2 = exp(-g);
            tn = (e1-e2)/(e1+e2), tn = tn+(sv-tn)*0.55;
            ig = (int)(tn*255.0);
            if(b<0.0) {
                b = 0.0;                                                // B��0.0-1.0�ɕ␳
            }
            if((sv=b)>1.0) {
                sv = 1.0;
            }
            e1 = exp(b), e2 = exp(-b);
            tn = (e1-e2)/(e1+e2), tn = tn+(sv-tn)*0.55;
            ib = (int)(tn*255.0);
            Rend->SetRendColor(x, y, ir, ig, ib);                       // �޲�۸ޕ`��
        }
        if(AntiAlias) {
            for(x=0; x<Szx; x++) {                                      // �����ر�ݸގ���
                ScanLine[y2][x] = ScanLine[y1][x];                      // ��Xײݗp��RGB����
            }
        }
    }
}

/**************************/
static void WorkFree(void)                                              //<<< ��Ɨ̈���
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
UINT RayTracingProc(LPVOID prm)                                         //<<< �����ݸ�(ڲ��ڰ�ݸ�)
/*******************************/
{
    OBJTYPE* op;
    int      i1, i2;

    Rend = (CRendDlg*)prm;
    Rend->GetRendSize(&Szx, &Szy);                                      // �`�滲�ގ擾
    BaseObjPtr(1, &op, &i1, &i2);                                       // �\�������߲���擾
    if(op!=NULL) {
        if(!MakeRendData()) {                                           // �����ݸ��ް��쐬
            WorkFree();                                                 // �װ�͍�Ɨ̈���
        } else {
            DrawRendData();                                             // �����ݸ��ް��`��
            WorkFree();                                                 // ��Ɨ̈���
        }
    }
    Rend->SetRendColor(-1, -1, 0, 0, 0);                                // ڲ��ڰ�ݸޏI��
    return 0;
}
