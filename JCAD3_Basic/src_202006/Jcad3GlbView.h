/////////////////////////////////////////////////////////////////////////////
// CJcad3GlbViewクラス(CJcad3GlbView)宣言
// Jcad3GlbView.h
//---------------------------------------------------------------------------
// LastEdit : 2010/03/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <gl\glu.h>
#include "DataType.h"
#include "resource.h"
#include "Jcad3GlbDoc.h"
#include "InfoDlg.h"
#include "NumDispDlg.h"
#include "PushDlg.h"
#include "OifTreeDlg.h"
#include "RendDlg.h"
#include "CalcDlg.h"
#include "Tracker.h"
#include "Line.h"

#define XY           0
#define YZ           1
#define ZX           2
#define PRS          3
#define ALL          4
#define NONE         0

#define DL_BORDER    1
#define DL_XY        2
#define DL_YZ        3
#define DL_ZX        4
#define DL_PARS      5
#define DL_ORTHOXY   6
#define DL_ORTHOYZ   7
#define DL_ORTHOZX   8
#define DL_GRIDXY    9
#define DL_GRIDYZ    10
#define DL_GRIDZX    11
#define DL_GRIDXY2   12
#define DL_GRIDYZ2   13
#define DL_GRIDZX2   14
#define DL_LIGHT     15
#define DL_GOLD      16
#define DL_SILVER    17
#define DL_CRYSTAL   18
#define DL_RUBY      19
#define DL_EMERALD   20
#define DL_SAPPHIRE  21
#define DL_USER1     22
#define DL_USER2     23
#define DL_USER3     24
#define DL_USER4     25
#define DL_USER5     26
#define DL_USER6     27
#define DL_USER7     28
#define DL_USER8     29
#define DL_USER9     30
#define DL_USER10    31
#define DL_USER11    32
#define DL_USER12    33
#define DL_SELECT    34
#define DL_CHOOSE    35
#define DL_BOOL      36
#define DL_XYZGUIDE  37
#define DL_BASEPOINT 38
#define DL_BASELINE  39

#define SCT_X        0
#define SCT_Y        0
#define SCT_Z        0
#define EYE_X        50000
#define EYE_Y        50000
#define EYE_Z        50000
#define FOVY         20000
#define TRLEN        17.3205
#define GRSPC        500
#define GRINT        10
#define DRAGMIN      5

#define M_EDIT_PASTEOBJ  ID_EDIT_PASTEOBJ
#define M_SELECT_OBJ     ID_SELECT_OBJ
#define M_ZOOM_BOX       ID_ZOOM_BOX
#define M_SCROLL         ID_SCROLL
#define M_BASE_SET       ID_BASE_SET
#define M_ALLSHADING     ID_ALLSHADING
#define M_RENDERING      ID_RENDERING
#define M_CRT_CURVE      ID_CRT_CURVE
#define M_EDIT_CURVE     ID_EDIT_CURVE
#define M_CRT_EXTRCURVE  ID_CRT_EXTRCURVE
#define M_CRT_SWEEP      ID_CRT_SWEEP
#define M_CRT_REVOLVE    ID_CRT_REVOLVE
#define M_CRT_PYRAMID    ID_CRT_PYRAMID
#define M_CRT_SETSECTION ID_CRT_SETSECTION
#define M_EDIT_SPACE     ID_EDIT_SPACE
#define M_EDIT_MOVE      ID_EDIT_MOVE
#define M_EDIT_ROTATE    ID_EDIT_ROTATE
#define M_EDIT_MIRROR    ID_EDIT_MIRROR
#define M_EDIT_SCALE     ID_EDIT_SCALE
#define M_EDIT_RESIZE    ID_EDIT_RESIZE
#define M_EDIT_RESIZE2   ID_EDIT_RESIZE2
#define M_EDIT_BEND      ID_EDIT_BEND
#define M_EDIT_TWIST     ID_EDIT_TWIST
#define M_EDIT_SHEAR     ID_EDIT_SHEAR
#define M_EDIT_LINES     ID_EDIT_LINES
#define M_EDIT_LINE      ID_EDIT_LINE
#define M_EDIT_CIRCLE    ID_EDIT_CIRCLE
#define M_EDIT_PCUT      ID_EDIT_PCUT
#define M_SURF_CPS       ID_SURF_CPS
#define M_SURF_P_MOVE    ID_SURF_P_MOVE
#define M_SURF_L_MOVE    ID_SURF_L_MOVE
#define M_SURF_L_INST    ID_SURF_L_INST
#define M_SURF_L_ROTATE  ID_SURF_L_ROTATE
#define M_SURF_TWIST     ID_SURF_TWIST
#define M_SURF_SCALE     ID_SURF_SCALE
#define M_SURF_S_MOVE    ID_SURF_S_MOVE
#define M_SURF_B_MOVE    ID_SURF_B_MOVE
#define M_SURF_P_ADJUST  ID_SURF_P_ADJUST
#define M_SURF_S_ADJUST  ID_SURF_S_ADJUST
#define M_SURF_TENSION   ID_SURF_TENSION
#define M_JWL_STONE      ID_JWL_STONE
#define M_JWL_ARRANGE    ID_JWL_ARRANGE
#define M_JWL_GEM        ID_JWL_GEM
#define M_JWL_EARTH      ID_JWL_EARTH
#define M_MEASURE_DIST   ID_MEASURE_DIST
#define M_MEASURE_ANGLE  ID_MEASURE_ANGLE
#define M_MEASURE_CURVE  ID_MEASURE_CURVE
#define M_MEASURE_SCALE  ID_MEASURE_SCALE
#define M_MEASURE_GRAD   ID_MEASURE_GRAD

class CJcad3GlbView : public CView
{
protected: // シリアル化からのみ作成します。
	CJcad3GlbView();                                                    // ｸﾗｽの構築
	DECLARE_DYNCREATE(CJcad3GlbView)

// 属性
public:
	CTracker Tracker1;
	CTracker Tracker2;
	CTracker Tracker3;
	CTracker Tracker4;
	CLine    Line;
	CSize    ViewSize;
private:
	PIXELFORMATDESCRIPTOR m_PFD;
	CClientDC* m_pMainDC;
	HGLRC    m_hRC;
	HANDLE   m_hDIB;
	HCURSOR  m_hcursor1, m_hcursor2, m_hcursor3, m_hcursor4, m_hcursor5;
	int      m_CursorNo, m_CursorNo_S;
	DWORD    PalSize;
	CRect    PrRect;
	double   ScrDiv1, ScrDiv2;
	CRect    PlRect[4];
	LONG     PlSize, PlSizeH[4], PlSizeW[4];
	double   PlDstH[3], PlDstW[3];
	CPoint   BasePoint, CurPos;
	int      Plane, InitDLSetup;
	PNTTYPE  BaseCursor, Cursor;
	UINT     m_Char, m_CChar, m_FChar, m_FPres, m_DChar;
	UINT     m_TEXTURE[3], m_GLB[3], m_TEMP[3];
	UINT     m_BOX, m_CIRCLE, m_SCALE, m_PROT, m_BASE,  m_NCLIST, m_STAGE;
	int      m_ScrChg, m_DisplayFlg, m_MoveInit;
	int      m_PrtMode, m_PrtColor, m_PrtOut, m_PrtPos, m_PrtSize;
	CString  m_PrtPath, m_PrtFile;
	int      m_PrtTCheck, m_PrtSCheck, m_PrtKind;
	int      m_CapPlane, m_CapPos, m_CapKind, m_CapWidth, m_CapHeight, m_CapJComp;
	int      m_OpenGLTest;
	CString  m_MapFile, m_MapFile_S;
	COLORREF m_RColor[3], m_RColor_S[3], m_CColor[21][3], m_CColor_S[18][3];
	WORD     m_Shin[21], m_Shin_S[18];
	int      m_KeyEnv, m_AtrEnv, m_SecEnv, m_ShpEnv, m_ShdEnv, m_ScrEnv1, m_ScrEnv2, m_BtnEnv;
	UINT     m_NumKey, m_NNuKey, m_LimKey, m_NLiKey, m_ObjKey, m_NObKey, m_CtlKey, m_CurKey;
private:
	CStateDlg*     m_pState;
	CDataUseDlg*   m_pDataUse;
	CNumDispDlg*   m_pNumDisp;
	CScrollDlg*    m_pScrScroll;
	CEyeDlg*       m_pEyeChange;
	COifTreeDlg*   m_pOifTree;
	CFunctionDlg*  m_pFunction;
	CFunction2Dlg* m_pFunction2;
	CCurveCtrlDlg* m_pCurveCtrl;
	CRendDlg*      m_pRendDlg;
	CCalcDlg1*     m_pCalcDlg1;
	CCalcDlg2*     m_pCalcDlg2;
	int      m_SState, m_SDataUse, m_SNumDisp, m_SScrScroll, m_SEyeChange, m_SOifTree;
	int      m_SFuncDsp, m_SFunction, m_SFuncDsp2, m_SFunction2, m_SFuncMode;
	int      m_SRendDlg, m_SCalcDlg1, m_SCalcDlg2;
	CString  m_strCalc1, m_strCalc2, m_strCalc3, m_strCalc4;
	int      m_ActMenu, m_SaveMenu, m_InitMenu, m_EdtCount, m_DspCount;
	int      m_CopyBtn, m_NumBtn;
	int      m_CopyInp, m_NumInp, m_NNuInp, m_CtlInp, m_CurInp;
	int      m_SolidFlg, m_SweepFlg;
	int      m_CopyNum;
	int      m_BoxPlane, m_CirclePlane, m_ScalePlane, m_ProtPlane, m_BasePlane;
	int      m_DispMode;
	int      m_GemNo;
	int      m_Shading, m_Rendering, m_RendPlane;
	int      m_CurveAFlg, m_CurveRFlg;
	double   m_CurveAngle, m_CurveRadius;
	int      m_CurveRepeat, m_ObjCRpt;
	double   m_CurveWeight, m_ObjCWgt;
	int      m_SecBase1a, m_SecBase1b;
	int      m_SecBase2;
	int      m_SecBase3a, m_SecBase3b, m_SecBase3c;
	int      m_SecBase4;
	int      m_SweepDivi, m_RevolveDivi;
	int      m_SurfTension;
	int      m_RingKind;
	double   m_RingSize1, m_RingSize2, m_RingSpace1, m_RingSpace2;
	double   m_MoveAdd, m_ScaleAdd1, m_ScaleAdd2, m_RotateAdd1, m_RotateAdd2;
	double   m_BendAdd, m_LTwistAdd, m_LScaleAdd, m_TwistAdd, m_LRotateAdd;
	double   m_ShearAdd, m_ArangAdd1, m_ArangAdd2, m_ArangAdd3, m_CurveAdd;
	double   m_SweepAdd, m_ECurveAdd, m_PMoveAdd, m_LMoveAdd, m_SMoveAdd;
	double   m_BMoveAdd, m_ThickValue, m_RevolveTh;
	double   m_EarthAdd1, m_EarthAdd2, m_EarthAdd3;
	int      m_ScaleDir, m_ScalePnt;
	int      m_ProngNum;
	double   m_ProngLn, m_ProngDm, m_ProngRv, m_ArrangeTh;
	int      m_ArrangeNum, m_LinePNum, m_CirclePNum;
	double   m_CircleSize1, m_CircleSize2, m_PastePos;
	int      m_PyramidDivi, m_InstPNum, m_SymRDivi;
	double   m_BaseZPos;
	int      m_PixelSize1, m_PixelSize2, m_ObjCopyNum, m_ObjSynFlg;
	double   m_ObjXScale, m_ObjYScale, m_ObjZScale;
	int      m_ObjXPoint, m_ObjYPoint, m_ObjZPoint;
	double   m_SPDiameter, m_SPHeight;
	int      m_SPCtrlNum, m_SPSprlNum, m_TensionNo;
	CString  m_TmpltPath, m_TexPath, m_MapFilePath, m_NCFilePath, m_SaveFilePath;
	int      m_FontNo, m_FontCheck;
	double   m_TextHeight, m_TextDepth, m_TextIntval;
	double   m_PrimXSize2D, m_PrimYSize2D, m_PrimZSize2D;
	double   m_PrimXSize3D, m_PrimYSize3D, m_PrimZSize3D;
	double   m_JwlRndSize, m_JwlSqrSize1, m_JwlSqrSize2;
	double   m_GemRndSize, m_GemRndHeigt, m_GemSqrSize1, m_GemSqrSize2, m_GemSqrHeigt;
	int      m_CurveDvd, m_PolygonF, m_FileBUpF, m_SaveFileMax, m_PCutCnt, m_BoolCnt;
	double   m_XLmtR, m_YLmtR, m_ZLmtR;
	int      m_XLmt1, m_YLmt1, m_ZLmt1;
	int      m_XLmt2, m_YLmt2, m_ZLmt2;
	double   m_XLMin, m_YLMin, m_ZLMin;
	double   m_XLMax, m_YLMax, m_ZLMax;
	SCRTYPE  m_Cent3D, m_Cent3D_S[5], m_Cent3D_SS;
	int      m_Fovy, m_Fovy_S[5], m_Fovy_SS;
	SCRTYPE  m_CentPs, m_CentPs_S[5];
	SCRTYPE  m_Eye, m_Eye_S[5];
	SCRTYPE  m_EyeSv, m_EyeSv_S[5];
	double   m_TruckLen, m_TruckLen_S[5], m_ExactRev;
	VECTYPE  m_HzVec, m_HzVec_S[5];
	VECTYPE  m_VtVec, m_VtVec_S[5];
	int      m_DAraFlg;
	BOXTYPE  m_DAra;
	int      m_GridDisp, m_GridDisp_S[5];
	int      m_GridBind, m_GridBind_S[5];
	int      m_GridSpace, m_GridSpace_S[5];
	int      m_GridIntvl, m_GridIntvl_S[5];
	SCRTYPE  m_Bpoint, m_Bpoint_S[5];
	SCRTYPE  m_Blspnt, m_Blspnt_S[5];
	SCRTYPE  m_Blepnt, m_Blepnt_S[5];
	int      m_BpDisp, m_BpDisp_S[5];
	int      m_BpBind, m_BpBind_S[5];
	int      m_BlDisp, m_BlDisp_S[5];
	int      m_BlBind, m_BlBind_S[5];
	int      m_GlbDisp, m_GlbDisp_S[5];
	int      m_GlbChange, m_GlbChange_S[5];
	int      m_DspTmpNo, m_DspTmpNo_S[5];
	CString  m_TempPath[8][3], m_TempPath_S[5][8][3];
	CString  m_TexFile[3];
	int      m_TexFovy[3], m_TexDisp[3], m_TexRotate[3];
	double   m_TexMove1[3], m_TexMove2[3];
	float    m_TexScale[3];
	PNTTYPE  m_Ncpnt;
	int      m_Ncdflg, m_Ncpflg;
	int      m_ULineCnt, m_VLineCnt;
	int*     m_ULineNo, * m_VLineNo;
	GLubyte* m_TexImage[3];
	int      m_TexWidth[3];
	int      m_TexHeight[3];
public:
	int      m_MaxCCtl, m_MaxSCtl, m_MaxCCtA, m_MaxSCtA, m_MaxWCtl;
	int      m_MaxCPnt, m_MaxSPnt, m_MaxOPnt, m_MaxBPnt;
private:																// ﾒﾆｭｰ･ﾜｰｸ用
	PNTTYPE* Spa, * Spb, * Spc, * Spd, * Sps, * Spt, * Spw, * Spx, * Spy, * Spz;
	SCRTYPE* Ssa, * Ssb, * Ssc;
	VECTYPE* Sva, * Svb;
	PNTTYPE  Sbase, Spnt1, Spnt2, Spnt3, Spnt4, Spnt5;
	SCRTYPE  Sscr1;
	VECTYPE  Svec1, Svec2;
	int      Sphase, Sphase2;
	int      Si1, Si2, Si3, Si4, Si5, Si6, Si7, Si8, Si9, Si10, Si11, Si12, Si13,
	         Si14, Si15, Si16, Si17, Si18, Si19, Si20, Si21, Si22, Si23, Si24;
	int*     Sia, * Sib, * Sic, * Sis, * Siw, * Six, * Siy, * Siz, * Tens, * TensC;
	double*  Sda, * Sdb;
	double   Sd1, Sd2, Sd3, Sd4, Sd5, Sd6, Sd7;
	OBJTYPE* Sop1, * Sop2, * Sop3, * Sop4;
	OIFTYPE* Soif;
	CMDTYPE* Scmp;
private:																// ﾒﾆｭｰ以外ﾜｰｸ用
	VECTYPE  SvecA, SvecB;
	PNTTYPE  SpntA;
	SCRTYPE  SscrA, SscrB, SscrC;
	CPoint   Spoint;
	int      Wi0, Wi1, Wi2, Wi3, Wi4, Wi5, Wi6;

public:
	inline CJcad3GlbDoc* GetDocument(void) { return (CJcad3GlbDoc*)m_pDocument; }

// 操作
public:
	void Init(void);
	void InitChar(void);
	void SetCChar(int flg, UINT chr);
	UINT GetCChar(void) { return m_CChar; }
	void SetFChar(int flg, UINT chr);
	void GetFChar(int* func);
	void SetCapPlane(int pln) { m_CapPlane = pln; }
	int  GetCapPlane(void) { return m_CapPlane; }
	void SetCapPos(int pos) { m_CapPos = pos; }
	int  GetCapPos(void) { return m_CapPos; }
	void SetCapKind(int knd) { m_CapKind = knd; }
	int  GetCapKind(void) { return m_CapKind; }
	void SetCapWidth(int wid) { m_CapWidth = wid; }
	int  GetCapWidth(void) { return m_CapWidth; }
	void SetCapHeight(int hei) { m_CapHeight = hei; }
	int  GetCapHeight(void) { return m_CapHeight; }
	void SetCapJComp(int cmp) { m_CapJComp = cmp; }
	int  GetCapJComp(void) { return m_CapJComp; }
	void SetKeyEnv(int env) { m_KeyEnv = env; }
	int  GetKeyEnv(void) { return m_KeyEnv; }
	void SetAtrEnv(int env) { m_AtrEnv = env; }
	int  GetAtrEnv(void) { return m_AtrEnv; }
	void SetSecEnv(int env) { m_SecEnv = env; }
	int  GetSecEnv(void) { return m_SecEnv; }
	void SetShpEnv(int env) { m_ShpEnv = env; }
	int  GetShpEnv(void) { return m_ShpEnv; }
	void SetShdEnv(int env) { m_ShdEnv = env; }
	int  GetShdEnv(void) { return m_ShdEnv; }
	void SetScrEnv1(int env) { m_ScrEnv1 = env; }
	int  GetScrEnv1(void) { return m_ScrEnv1; }
	void SetScrEnv2(int env) { m_ScrEnv2 = env; }
	int  GetScrEnv2(void) { return m_ScrEnv2; }
	void SetBtnEnv(int env) { m_BtnEnv = env; }
	int  GetBtnEnv(void) { return m_BtnEnv; }
	void SetScreenNo(int no);
	void OutputCapData(CString path);
	void InitScreen(int cx, int cy);
	void ResetColor(void);
	void SetSeishaei(const SCRTYPE& cent, int fovy);
	void SetPerspective(void);
	void Display(int flg);
	void ViewDisplayXYZ(int plane);
	void ViewDisplayPRS(void);
	void GetDisplayColor(OBJTYPE* op, int edt, double* r, double* g, double* b);
	void DisplayAllObj3P(int flg);
	void DisplayAllObjPars(void);
	void DispGrid(int plane, int erase=0, int back=0);
	void SetDispList(OBJTYPE* op);
	void SetRenderList(OBJTYPE* op, VECTYPE* vc);
	void DelDispList(OBJTYPE*);
	void InitLineNo(OBJTYPE* op);
	void SetLineNo(int flg, int cnt, int* no);
	void AddLineColorList(OBJTYPE* op);
	void SetRenderMode(int mode);
	int  GetRenderMode(void) { return m_Rendering; }
	void InitNCDisp(void);
	void SetNCDisp(PNTTYPE spt, PNTTYPE ept);
	void DispNCDisp(int col);
	void EndNCDisp(void);
	UINT GetChar(void) { return m_Char; }
	int  GetPlSize(void) { return PlSize; }
	CPoint GetPlCenter(void);
	void CheckCtrlKey1(void);
	void CheckCtrlKey2(UINT flg);
	void SetCopyBtn(int cop);
	int  GetCopyBtn(void) { return m_CopyBtn; }
	void SetNumBtn(int num);
	int  GetNumBtn(void) { return m_NumBtn; }
	int  GetNumInp(void) { return m_NumInp; }
	int  CheckScrChg(CPoint point);
	void ChangeScreen(int flg, CPoint point);
	double GetDispHeight(void) { return (double)m_Fovy/1000; }
	void SetDispMode(int mode) { m_DispMode = mode; }
	int  GetDispMode(void) { return m_DispMode; }
	void SetGridDisp(int disp) { m_GridDisp = disp; }
	int  GetGridDisp(void) { return m_GridDisp; }
	void SetGridBind(int bind) { m_GridBind = bind; }
	int  GetGridBind(void) { return m_GridBind; }
	void SetGridSpace(int spc) { m_GridSpace = spc; }
	int  GetGridSpace(void) { return m_GridSpace; }
	void SetGridIntvl(int itv) { m_GridIntvl = itv; }
	int  GetGridIntvl(void) { return m_GridIntvl; }
	void SetBasePointDisp(int disp) { m_BpDisp = disp; }
	int  GetBasePointDisp(void) { return m_BpDisp; }
	void SetBasePointBind(int bind) { m_BpBind = bind; }
	int  GetBasePointBind(void) { return m_BpBind; }
	void SetBaseLineDisp(int disp) { m_BlDisp = disp; }
	int  GetBaseLineDisp(void) { return m_BlDisp; }
	void SetBaseLineBind(int bind) { m_BlBind = bind; }
	int  GetBaseLineBind(void) { return m_BlBind; }
	void SetDspGlb(int no) { m_GlbDisp = no; }
	int  GetDspGlb(void) { return m_GlbDisp; }
	void SetGlbChange(int chg) { m_GlbChange = chg; }
	int  GetGlbChange(void) { return m_GlbChange; }
	void SetDspTmpNo(int no) { m_DspTmpNo = no; }
	int  GetDspTmpNo(void) { return m_DspTmpNo; }
	int  GetSolidMode(void) { return m_SolidFlg; }
	int  GetGemNo(void) { return m_GemNo; }
	int  GetCurveRepeat(void) { return m_CurveRepeat; }
	void SetCurveRepeat(int rep) { m_CurveRepeat = rep; }
	double GetCurveWeight(void) { return m_CurveWeight; }
	void SetCurveWeight(double wgt) { m_CurveWeight = wgt; }
	int  GetSurfaceTension(void) { return m_SurfTension; }
	int  GetRingKind(void) { return m_RingKind; }
	void SetRingSize1(double size) { m_RingSize1 = size; }
	double GetRingSize1(void) { return m_RingSize1; }
	void SetRingSize2(double size) { m_RingSize2 = size; }
	double GetRingSize2(void) { return m_RingSize2; }
	void SetBaseZPos(double pos) { m_BaseZPos = pos; }
	double GetBaseZPos(void) { return m_BaseZPos; }
	void SetPixelSize1(int size) { m_PixelSize1 = size; }
	int  GetPixelSize1(void) { return m_PixelSize1; }
	void SetPixelSize2(int size) { m_PixelSize2 = size; }
	int  GetPixelSize2(void) { return m_PixelSize2; }
	void SetTmpltPath(CString path) { m_TmpltPath = path; }
	CString GetTmpltPath(void) { return m_TmpltPath; }
	void SetTexPath(CString path) { m_TexPath = path; }
	CString GetTexPath(void) { return m_TexPath; }
	void SetMapFilePath(CString path) { m_MapFilePath = path; }
	CString GetMapFilePath(void) { return m_MapFilePath; }
	void SetSaveFilePath(CString path) { m_SaveFilePath = path; }
	CString GetSaveFilePath(void) { return m_SaveFilePath; }
	void SetFontNo(int no) { m_FontNo = no; }
	int  GetFontNo(void) { return m_FontNo; }
	void SetFontCheck(int check) { m_FontCheck = check; }
	int  GetFontCheck(void) { return m_FontCheck; }
	void SetTextHeight(double ht) { m_TextHeight = ht; }
	double GetTextHeight(void) { return m_TextHeight; }
	void SetTextDepth(double dp) { m_TextDepth = dp; }
	double GetTextDepth(void) { return m_TextDepth; }
	void SetTextIntval(double it) { m_TextIntval = it; }
	double GetTextIntval(void) { return m_TextIntval; }
	void SetPrimXSize2D(double size) { m_PrimXSize2D = size; }
	double GetPrimXSize2D(void) { return m_PrimXSize2D; }
	void SetPrimYSize2D(double size) { m_PrimYSize2D = size; }
	double GetPrimYSize2D(void) { return m_PrimYSize2D; }
	void SetPrimZSize2D(double size) { m_PrimZSize2D = size; }
	double GetPrimZSize2D(void) { return m_PrimZSize2D; }
	void SetPrimXSize3D(double size) { m_PrimXSize3D = size; }
	double GetPrimXSize3D(void) { return m_PrimXSize3D; }
	void SetPrimYSize3D(double size) { m_PrimYSize3D = size; }
	double GetPrimYSize3D(void) { return m_PrimYSize3D; }
	void SetPrimZSize3D(double size) { m_PrimZSize3D = size; }
	double GetPrimZSize3D(void) { return m_PrimZSize3D; }
	void SetSaveFileMax(int max) { m_SaveFileMax = max; }
	int  GetSaveFileMax(void) { return m_SaveFileMax; }
	void InitDialog(void);
	void CreateDialogs(void);
	void CreateRendDlg(void);
	void CreateCalcDlg1(void);
	void CreateCalcDlg2(void);
	void SetCalc(CString a, CString b, CString c, CString d);
	void EndDialogs(void);
	void EndRendDlg(void);
	void EndCalcDlg1(void);
	void EndCalcDlg2(void);
	void ShowStateDlg(void);
	BOOL IsStateDlgShow(void) { return m_SState; }
	void HideStateDlg(void) { m_SState = 0; }
	void ShowDataUse(void);
	BOOL IsDataUseShow(void) { return m_SDataUse; }
	void HideDataUse(void) { m_SDataUse = 0; }
	void SetDataUse(void) { m_pDataUse->SetData(); }
	void SetNumDisp(int disp);
	BOOL IsNumDispShow(void) { return m_SNumDisp; }
	void HideNumDisp(void) { m_SNumDisp = 0; }
	void ShowScrollTool(void);
	BOOL IsScrollToolShow(void) { return m_SScrScroll; }
	void HideScrollTool(void) { m_SScrScroll = 0; }
	void ShowEyeChange(void);
	BOOL IsEyeChangeShow(void) { return m_SEyeChange; }
	void HideEyeChange(void) { m_SEyeChange = 0; }
	void ShowOifTree(void);
	void InitOifItem(void);
	void SetOifItem(OIFTYPE* oif);
	void DelOifItem(OIFTYPE* oif);
	void UpdateOifItem(OIFTYPE* oif);
	void SelectOifItem(OIFTYPE* oif);
	void UpdateDspFlg(int flg, OBJTYPE* op);
	void UpdateEdtFlg(int flg, OBJTYPE* op);
	BOOL IsOifTreeShow(void) { return m_SOifTree; }
	void HideOifTree(void) { m_SOifTree = 0; }
	void ChangeFuncDsp(int flg) { m_SFuncDsp = flg; }
	BOOL IsFuncDsp(void) { return m_SFuncDsp; }
	void ShowFunction(void);
	void HideFunction(void);
	BOOL IsFunctionShow(void) { return m_SFunction; }
	void MessageOut(int nID);
	void ChangeFuncDsp2(int flg) { m_SFuncDsp2 = flg; }
	BOOL IsFuncDsp2(void) { return m_SFuncDsp2; }
	void ShowFunction2(void);
	void HideFunction2(void);
	BOOL IsFunctionShow2(void) { return m_SFunction2; }
	void ChangeFuncMode(int flg) { m_SFuncMode = flg; }
	BOOL IsFuncMode(void) { return m_SFuncMode; }
	BOOL IsRendDlgShow(void) { return m_SRendDlg; }
	BOOL IsCalcDlg1Show(void) { return m_SCalcDlg1; }
	BOOL IsCalcDlg2Show(void) { return m_SCalcDlg2; }
	void ShowCurveCtrl(void);
	void HideCurveCtrl(void);
	void ClearCurveData(void);
	void GetCurveData(double* agl, double* rds);
	void SetCurveAngle(int flg, double agl);
	void SetCurveRadius(int flg, double rds);
	void ChangePrint(void);
	int  HitTestPlane(const CPoint&);
	void GetViewport(int plane, int* x, int* y, int* w, int* h);
	BOOL CalcScreenWorld(int bind, PNTTYPE*, const CPoint&, int plane);
	BOOL CalcWorldScreen(CPoint*, const PNTTYPE&, int plane);
	void LimitValue(int plane, double* dd);
	BOOL LimitPoint(int plane, PNTTYPE* pa, PNTTYPE* pb);
	void LimitAngle(int plane, PNTTYPE* pa, PNTTYPE* pb, double th);
	int  CountEdtObj(void);
	void CheckEdtObjPrm(void);
	int  GetEdtObjNum(void) { return m_EdtCount; }
	int  GetDspObjNum(void) { return m_DspCount; }
	void SetActMenu(int menu) { m_ActMenu = menu; }
	int  GetActMenu(void) { return m_ActMenu; }
	void SetSaveMenu(int menu) { m_SaveMenu = menu, m_CursorNo_S = m_CursorNo; }
	int  GetSaveMenu(void) { return m_SaveMenu; }
	void SetCursorNo(int no) { m_CursorNo = no; }
	int  GetCursorNo(void) { return m_CursorNo; }
	void InitActMenu(int mode);
	void ExitActMenu(int menu);
	void MenuCall(int menu, UINT nflg, const CPoint& point, int plane, int ev);
	void InitializeMenu(void);
	void ExitMenu(int f1, int f2);
	void InitTRLimit(void);
	BOOL SetTRLimit(OBJTYPE* op);
	void GetTRLimit(double* x1, double* x2, double* y1, double* y2, double* z1, double* z2);
	void InitWorkTable(void);
	void FreeWorkTable(void);
	void InitData(void);
	void InitRColorFile(BOOL upd);
	void InitRenderFile(int flg, BOOL upd);
	void SaveRndEnv1(void);
	void SaveRndEnv2(void);
	void SaveRndEnv3(void);
	void RestoreRndEnv1(void);
	void RestoreRndEnv2(void);
	void RestoreRndEnv3(void);
	void ReadRenderFile(void);
	void WriteRenderFile(void);
	void ReadSysDataFile(void);
	void WriteSysDataFile(void);
	void ReadScrInfoFile(void);
	void WriteScrInfoFile(void);
	void SaveRender(void);
	void RestoreRender(void);
	void GetColorRGB(int cno, int kind, int* r, int* g, int* b, int* s);
	void SetColorRGB(int cno, int kind, int r, int g, int b, int s);
	void GetRenderRGB(int kind, int* r, int* g, int* b);
	void SetRenderRGB(int kind, int r, int g, int b);
	void GetRenderMap(CString* file);
	void SetRenderMap(CString file);
	void StartRenderTest(void);
	void EndRenderTest(void);
	void SetCtrlKey(void);
	void EyeMove(int dir, double th);
	void SetGrid(void);
	void SetBase(void);
	void SetXYZGuide(void);
	BOOL SetGLTPoint(FILE* fp);
	BOOL SetGLTLine(FILE* fp);
	BOOL SetGLTPolyline(FILE* fp);
	BOOL SetGLTBox(FILE* fp);
	BOOL SetGLTSquare(FILE* fp);
	BOOL SetGLTArc(FILE* fp);
	BOOL ReadGLTTemplate(int mode, int no, int pln);
	BOOL ReadGLB(void);
	void DeleteGLB(void);
	void SetTmpFile(int no, CString x, CString y, CString z);
	void GetTmpFile(int no, CString* x, CString* y, CString* z);
	BOOL SetPoint(FILE* fp, int* id, char* cmd);
	BOOL SetLine(FILE* fp, int* id, char* cmd);
	BOOL SetCircle(FILE* fp, int* id, char* cmd);
	BOOL SetArc(FILE* fp, int* id, char* cmd);
	BOOL SetEllipse(FILE* fp, int* id, char* cmd);
	BOOL SetPolyline(FILE* fp, int* id, char* cmd);
	BOOL ReadDXFTemplate(int no, int pln);
	BOOL ReadTEMP(void);
	void DeleteTEMP(void);
	void SetTexFile(int flg, int kind, int plane, CString file);
	void GetTexInfo(int plane, CString* fl, double* mv1, double* mv2, int* rt, float* sc);
	void ChangeTexInfo(int flg, int plane, int mode);
	BOOL SetTexImageBMP(int plane, int flg);
	BOOL SetTexImageJPG(int plane);
	void ClearTexImage(int plane);
	void RotateTexImage(int flg, int plane);
	void ChangeTexPixels(void);
	void DrawTexPixels(int plane);
	void DeleteTexPixels(int plane);
	void DrawBox(PNTTYPE sp, PNTTYPE ep, int plane);
	void DeleteBox(void);
	void DrawCircle(PNTTYPE cp, PNTTYPE ep, int plane);
	void DeleteCircle(void);
	void MeasureSCALE(PNTTYPE p1, PNTTYPE p2);
	void DeleteSCALE(void);
	void MeasurePROT(PNTTYPE p1, PNTTYPE p2);
	void DeletePROT(void);
	BOOL CheckMeasure(int menu);
	void SetBaseData(int cn, int pn, int bc, PNTTYPE* pa, PNTTYPE* pb, int plane);
	void DeleteBaseData(void);
	void StartStageDisplay(BOXTYPE box);
	void EndStageDisplay(void);
	void SetStage(int* flg, double size[][3]);
	void DeleteStage(void);
	void SetLightPos(int no, LIGHT* lgt, double* oct);
	void ResetLightPos(void);

// メニュー
public:
	void EyeChange(UINT flg, const CPoint& point, int plane, int ms);
	void EyeChange(int dir, double th);
	void ZoomIn(void);
	void ZoomOut(void);
	void AutoZoom(void);
	void ExactZoom(void);
	void ZoomBox(UINT flg, const CPoint& point, int plane, int ms);
	void Scroll(UINT flg, const CPoint& point, int plane, int ms);
	void Scroll(int axis, int dir);
	void BackScreenState(void);
	void BasePointSet(UINT flg, const CPoint& point, int plane, int ms);
	void CentPars(void);
	void EyeSet(int flg);
	void TruckUp(void);
	void TruckDown(void);
	void SetHomePosition(int no);
	void RestoreHomePosition(int no);
	void GetCentPnt(PNTTYPE* pnt);
	void GetEyePnt(PNTTYPE* pnt);
	void GetEyeSvPnt(PNTTYPE* pnt);
	void GetCentPsPnt(PNTTYPE* pnt);
	BOOL GetJewelFVec(OBJTYPE* op);
	BOOL SetShadingData(OBJTYPE* op);
	BOOL ObjectRendering(void);
	void ObjectShading(OBJTYPE* op);
	void MenuRender(UINT flg, const CPoint& point, int plane, int ms);
	void MenuAllShad(UINT flg, const CPoint& point, int plane, int ms);
	void MousePasteObject(UINT flg, const CPoint& point, int plane, int ms);
	void ObjSelect(UINT flg, const CPoint& point, int plane, int ms);
	void ObjChoose(UINT flg, const CPoint& point, int plane, int ms);
	void MouseCurve(UINT flg, const CPoint& point, int plane, int ms);
	void MouseECurve(UINT flg, const CPoint& point, int plane, int ms);
	void MouseExtrCurve(UINT flg, const CPoint& point, int plane, int ms);
	void MenuCentCurve(void);
	void MouseSweep(UINT flg, const CPoint& point, int plane, int ms);
	void MouseRevolve(UINT flg, const CPoint& point, int plane, int ms);
	void MousePyramid(UINT flg, const CPoint& point, int plane, int ms);
	void MouseSetSection(UINT flg, const CPoint& point, int plane, int ms);
	void MenuSetCurve(void);
	void MenuSection(void);
	void MenuSpiral(void);
	void MenuResetSurface(void);
	void MenuConnect(void);
	void MenuThickness(void);
	void MouseSpace(UINT flg, const CPoint& point, int plane, int ms);
	void MouseMove(UINT flg, const CPoint& point, int plane, int ms);
	void MouseRotate(UINT flg, const CPoint& point, int plane, int ms);
	void MouseMirror(UINT flg, const CPoint& point, int plane, int ms);
	void MouseScale(UINT flg, const CPoint& point, int plane, int ms);
	void MouseResize3(UINT flg, const CPoint& point, int plane, int ms);
	void MouseResize4(UINT flg, const CPoint& point, int plane, int ms);
	void MouseBend(UINT flg, const CPoint& point, int plane, int ms);
	void MouseTwist(UINT flg, const CPoint& point, int plane, int ms);
	void MouseShear(UINT flg, const CPoint& point, int plane, int ms);
	void MouseLinesEdit(UINT flg, const CPoint& point, int plane, int ms);
	void MenuSurfaceEdit(void);
	void MouseLineEdit(UINT flg, const CPoint& point, int plane, int ms);
	void MouseCircleEdit(UINT flg, const CPoint& point, int plane, int ms);
	void MousePCut(UINT flg, const CPoint& point, int plane, int ms);
	void MenuBoolean(void);
	void MouseCPointSpace(UINT flg, const CPoint& point, int plane, int ms);
	void MousePointMove(UINT flg, const CPoint& point, int plane, int ms);
	void MouseLineMove(UINT flg, const CPoint& point, int plane, int ms);
	void MouseLineRotate(UINT flg, const CPoint& point, int plane, int ms);
	void MouseLineInsert(UINT flg, const CPoint& point, int plane, int ms);
	void MouseLineTwist(UINT flg, const CPoint& point, int plane, int ms);
	void MouseLineScale(UINT flg, const CPoint& point, int plane, int ms);
	void MouseSPointMove(UINT flg, const CPoint& point, int plane, int ms);
	void MouseBPointMove(UINT flg, const CPoint& point, int plane, int ms);
	void MouseLineAdjust(UINT flg, const CPoint& point, int plane, int ms);
	void MouseObjectAdjust(UINT flg, const CPoint& point, int plane, int ms);
	void MenuObjectAdjust(void);
	void MenuChgSurfParam(int mode);
	void MouseTension(UINT flg, const CPoint& point, int plane, int ms);
	void MenuJwlRing(void);
	void MouseJwlStone(UINT flg, const CPoint& point, int plane, int ms);
	void MouseJwlArrange(UINT flg, const CPoint& point, int plane, int ms);
	void MenuSetUnit(void);
	void MouseEarth(UINT flg, const CPoint& point, int plane, int ms);
	BOOL ReadGem(int no, PNTTYPE* cpt, double* dh);
	void MouseGemSquare(UINT flg, const CPoint& point, int plane, int ms);
	void MouseGemRect(UINT flg, const CPoint& point, int plane, int ms);
	void MouseGemRect2(UINT flg, const CPoint& point, int plane, int ms);
	void MenuMeasureVolume(void);
	void MouseMeasureDistance(UINT flg, const CPoint& point, int plane, int ms);
	void MouseMeasureAngle(UINT flg, const CPoint& point, int plane, int ms);
	void MouseMeasureCurve(UINT flg, const CPoint& point, int plane, int ms);
	void MouseMeasureScale(UINT flg, const CPoint& point, int plane, int ms);
	void MouseMeasureProt(UINT flg, const CPoint& point, int plane, int ms);
	int  DisplayNCProc(int mode, CString path, BOXTYPE* box);
	void MenuNCDisplay(void);
	void MenuSimulation(void);

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 実装
public:
	virtual ~CJcad3GlbView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitGL(void);
	BOOL SetupPixelFormat(HDC hdc, PIXELFORMATDESCRIPTOR* pPFD);
	void SetGLColor(int ix);
	void SetGLRender(void);
	BOOL SetGLRenderMap(void);
	void DDBtoDIB(CDC* pDC);
	void OutputPrinter(CDC* pDC);
	void OutputFile(void);

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	DECLARE_MESSAGE_MAP()
};

extern CJcad3GlbView* View;

//===========================<EOF:Jcad3GlbView.h>============================