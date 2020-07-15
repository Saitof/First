/////////////////////////////////////////////////////////////////////////////
// TrueTypeフォント操作用 構造体及びマクロ定義
// TrueTypeInstData.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#define SUCCESS  0
#define FAILURE  1

#define ERR_OK					0										// ｲﾝｽﾄﾗｸｼｮﾝ ｴﾗｰ
#define ERR_INVALID_CODERANGE	1
#define ERR_CODE_OVERFLOW		2
#define ERR_INVALID_OPCODE		3
#define ERR_DEBUG_OPCODE		4
#define ERR_INVALID_REFERENCE	5
#define ERR_STACK_OVERFLOW		6
#define ERR_TOO_MANY_FUNC_DEF	7
#define ERR_TOO_MANY_INST_DEF	8
#define ERR_NESTED_DEF			9
#define ERR_ENDF_IN_EXC_STRM	10
#define ERR_TOO_FEW_ARGUMENT	11
#define ERR_BAD_ARGUMENT		12
#define ERR_DIVIDE_BY_ZERO		13
#define ERR_EXECUTION_TOO_LONG	14

#define ROUND_OFF				0
#define ROUND_TO_GRID			1
#define ROUND_UP_TO_GRID		2
#define ROUND_DOWN_TO_GRID		3
#define ROUND_TO_HALF_GRID		4
#define ROUND_TO_DOUBLE_GRID	5
#define ROUND_SUPER				6
#define ROUND_SUPER_45			7

#define CURVE_TAG_ON			1
#define CURVE_TAG_TOUCH_X		8
#define CURVE_TAG_TOUCH_Y		16
#define CURVE_TAG_TOUCH_BOTH	(CURVE_TAG_TOUCH_X | CURVE_TAG_TOUCH_Y)

typedef char			CHAR;
typedef unsigned char	BYTE;
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef int				INT;
typedef unsigned int	UINT;
typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned short	uFWord;
typedef double			DBL;

//---------- ｲﾝｽﾄﾗｸｼｮﾝ 作業領域 の 定義 ----------//
typedef struct {
	LONG	x;
	LONG	y;
} Vector;

typedef struct {
	USHORT	nPoint;														// number of points in zone
	SHORT	nContour;													// number of contours
	Vector*	Org;														// original point coordinates
	Vector*	Cur;														// current point coordinates
	BYTE*	Tag;														// current touch flags
	USHORT*	Contour;													// contour end points
} GlyphZone;

typedef struct {
	USHORT	xPpem;														// horizontal pixels per EM
	USHORT	yPpem;														// vertical pixels per EM
	LONG	xScale;														// two scales used to convert font units
	LONG	yScale;														// to 26.6 frac. pixel coordinates..
} F_Size_Metrics;

typedef struct {
	LONG	xRatio;
	LONG	yRatio;
	USHORT	Ppem;														// maximum ppem size
	LONG	Ratio;														// current ratio
	LONG	Scale;
	LONG	Compens[4];													// device-specific compensations
	BYTE	Rotate;														// 'is the glyph rotated?'-flag
	BYTE	Stretch;													// 'is the glyph stretched?'-flag
} T_Size_Metrics;

typedef struct {
	SHORT	x;
	SHORT	y;
} UnitVector;

typedef struct {
	USHORT		Rp0;
	USHORT		Rp1;
	USHORT		Rp2;
	UnitVector	dVector;
	UnitVector	pVector;
	UnitVector	fVector;
	LONG		Loop;
	LONG		MinDist;
	INT			RoundStat;
	BYTE		AutoFlip;
	LONG		CtrlValCI;
	LONG		SglWidthCI;
	LONG		SglWidthVal;
	SHORT		DeltaBase;
	SHORT		DeltaShift;
	BYTE		InstCtrl;
	BYTE		ScanCtrl;
	INT			ScanType;
	USHORT		Gep0;
	USHORT		Gep1;
	USHORT		Gep2;
} GraphicsState;

typedef struct {
	INT		Range;														// in which code range is it located?
	LONG	Start;														// where does it start?
	UINT	Opc;														// function #, or instruction code
	BYTE	Active;														// is it active?
} DefRecord, *DefArray;

typedef struct {
	INT		CalRange;
	LONG	CalIP;
	LONG	CurCount;
	LONG	CurRestart;
} CalRecord, *CalArray;

typedef struct {
	BYTE*	Base;
	ULONG	Size;
} CodeRange;
typedef CodeRange  CodeRangeTable[2];

typedef struct ExecContextRec_*  ExecContext;

typedef LONG (*Round_Func)(ExecContext exc, LONG dist, LONG comps);
typedef LONG (*Project_Func)(ExecContext exc, Vector* v1, Vector* v2);
typedef void (*Move_Func)(ExecContext exc, GlyphZone* zon, USHORT pnt, LONG dist);
typedef LONG (*Get_CVT_Func)(ExecContext exc, LONG idx);
typedef void (*Set_CVT_Func)(ExecContext exc, LONG idx, LONG val);

typedef struct ExecContextRec_ {
	INT				Error;												// last execution error
	LONG			Top;												// top of exec. stack
	UINT			StackSize;											// size of exec. stack
	LONG*			Stack;												// current exec. stack
	LONG			Arg;
	UINT			NewTop;												// new top after exec.
	GlyphZone		Zp0,												// zone records
					Zp1,
					Zp2,
					Pts,
					Twilight;
	F_Size_Metrics	FMetric;
	T_Size_Metrics	TMetric;											// size metrics
	GraphicsState	GS;													// current graphics state
	INT				cRange;												// current code range number
	BYTE*			Code;												// current code range
	LONG			IP;													// current instruction pointer
	LONG			CdSize;												// size of current range
	BYTE			OpCd;												// current opcode
	INT				Length;												// length of current opcode
	BYTE			StepIns;											// true if the interpreter must increment IP after ins. exec
	LONG			CvtSize;
	LONG*           Cvt;
	BYTE*			GlyphIns;											// glyph instructions buffer
	UINT			numFDef;											// number of function defs
	UINT			maxFDef;											// maximum number of function defs
	DefArray		FDef;												// table of FDefs entries
	UINT			numIDef;											// number of instruction defs
	UINT			maxIDef;											// maximum number of ins defs
	DefArray		IDef;												// table of IDefs entries
	UINT			maxFunc;											// maximum function index
	UINT			maxIns;												// maximum instruction index
	INT				ClTop,												// top of call stack during execution
					ClSize;												// size of call stack
	CalArray		ClStack;											// call stack
	CodeRangeTable	CodeRTable;											// valid code ranges useful for the debugger
	USHORT			StrSize;											// size of current storage
	LONG*			Storage;											// storage area
	LONG			Period;												// values used for the
	LONG			Phase;												// `SuperRounding'
	LONG			Thrhld;
	LONG			FdotP;												// dot product of freedom and projection vectors
	Round_Func		FuncRound;											// current rounding function
	Project_Func	FuncProj,											// current projection function
					FuncDProj,											// current dual proj. function
					FuncFProj;											// current freedom proj. func
	Move_Func		FuncMove;											// current point move function
	Get_CVT_Func	FuncReadCvt;										// read a cvt entry
	Set_CVT_Func	FuncWriteCvt;										// write a cvt entry (in pixels)
	Set_CVT_Func	FuncMoveCvt;										// incr a cvt entry (in pixels)
} ExecContextRec;

typedef struct {
	LONG	xx, xy;
	LONG	yx, yy;
} Matrix;

typedef struct {
	SHORT	nContour;													// number of contours in glyph
	SHORT	nPoint;														// number of points in the glyph
	Vector*	Point;														// the outline's points
	CHAR*	Tag;														// the points flags
	SHORT*	Contour;													// the contour end points
	INT		Flag;														// outline masks
} Outline;

typedef struct {
	INT		Idx;
	USHORT	Flag;
	INT		Arg1;
	INT		Arg2;
	Matrix	Trans;
} SubGlyph;

typedef struct {
	Outline		Oline;													// outline
	UINT		nSubglyph;												// number of subglyphs
	SubGlyph*	Subglyph;												// subglyphs
	Vector*		EPoint;													// extra points table
} GlyphLoad;

typedef struct {
	GlyphLoad	Base;
	GlyphLoad	Curent;
} GlyphLoader;

//=========================<EOF:TrueTypeInstData.h>==========================
