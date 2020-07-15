/////////////////////////////////////////////////////////////////////////////
// TrueTypeフォント操作用 構造体及びマクロ定義
// TrueTypeData.h
//---------------------------------------------------------------------------
// LastEdit : 2009/02/12 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#define SEEK_SET 0														// seek from start of file
#define SEEK_CUR 1														// relative to current position

// For decoding glyph coordinate bit flags
#define ONCURVE        0x01
#define XSHORT         0x02
#define YSHORT         0x04
#define REPEAT_FLAGS   0x08												// repeat flag n times
#define SHORT_X_IS_POS 0x10												// the short vector is positive
#define NEXT_X_IS_ZERO 0x10												// the relative x coordinate is zero
#define SHORT_Y_IS_POS 0x20												// the short vector is positive
#define NEXT_Y_IS_ZERO 0x20												// the relative y coordinate is zero

// For decoding multi-component glyph bit flags
#define ARG_1_AND_2_ARE_WORDS    0x0001
#define ARGS_ARE_XY_VALUES       0x0002
#define ROUND_XY_TO_GRID         0x0004
#define WE_HAVE_A_SCALE          0x0008
//      RESERVED                 0x0010
#define MORE_COMPONENTS          0x0020
#define WE_HAVE_AN_X_AND_Y_SCALE 0x0040
#define WE_HAVE_A_TWO_BY_TWO     0x0080
#define WE_HAVE_INSTRUCTIONS     0x0100
#define USE_MY_METRICS           0x0200

#define TT_INSIDE   0
#define TT_OUTSIDE  1
#define TT_IGNORE   2
#define TT_COMBINED 3

#define LANGUAGE1 0x0411												// 日本語ID
#define LANGUAGE2 0x0409												// 英語ID

#define SPACE_CODE1	0x20												// 半角ｽﾍﾟｰｽ
#define SPACE_CODE2	0x8140												// 全角ｽﾍﾟｰｽ

typedef char           CHAR;
typedef unsigned char  BYTE;
typedef short          SHORT;
typedef unsigned short USHORT;
typedef int            INT;
typedef unsigned int   UINT;
typedef long           LONG;
typedef unsigned long  ULONG;
typedef double         DBL;

static BYTE tag_CharToIndexMap[] = "cmap";
static BYTE tag_FontHeader[] =     "head";
static BYTE tag_GlyphData[] =      "glyf";
static BYTE tag_IndexToLoc[] =     "loca";
static BYTE tag_MaxProfile[] =     "maxp";
static BYTE tag_FontName[] =       "name";
static BYTE tag_FontProgram[] =    "fpgm";
static BYTE tag_ControlValue[] =   "cvt ";

//---------- TrueTypeﾌｫﾝﾄ の 定義 ----------//
typedef struct {
	BYTE   Tag[4];
	LONG   version;
	ULONG  DirectoryCount;
	ULONG  TableDirectory;
} sfnt_TTC_Header;

typedef struct {
	LONG   version;														// 0x10000 (1.0)
	USHORT numTables;													// number of tables
	USHORT searchRange;													// (max2<=numTables)*16
	USHORT entrySelector;												// log2 (max2<=numTables)
	USHORT rangeShift;													// numTables*16-searchRange
} sfnt_OffsetTable;

typedef struct {
	BYTE   tag[4];
	ULONG  checkSum;
	ULONG  offset;
	ULONG  length;
} sfnt_TableDirectory;

typedef struct {
	ULONG  bc;
	ULONG  ad;
} longDateTime;

typedef struct {
	LONG   version;														// for this table, set to 1.0
	LONG   fontRevision;												// For Font Manufacturer
	ULONG  checkSumAdjustment;
	ULONG  magicNumber;													// signature, must be 0x5F0F3CF5==MAGIC
	USHORT flags;
	USHORT unitsPerEm;													// How many in Font Units per EM
	longDateTime created;
	longDateTime modified;
	SHORT  xMin;														// Font wide bounding box in ideal space
	SHORT  yMin;														// Baselines and metrics are NOT worked
	SHORT  xMax;														// into these numbers
	SHORT  yMax;
	USHORT macStyle;													// macintosh style word
	USHORT lowestRecPPEM;												// lowest recommended pixels per EM
	SHORT  fontDirectionHint;
	SHORT  indexToLocFormat;											// 0 - short offsets, 1 - long offsets
	SHORT  glyphDataFormat;
} sfnt_FontHeader;

typedef struct {
	USHORT platformID;
	USHORT specificID;
	ULONG  offset;
} sfnt_platformEntry;

typedef struct {
	USHORT format;
	USHORT length;
	USHORT version;
} sfnt_mappingTable;

//---------- TrueTypeﾌｫﾝﾄ 作業領域 の 定義 ----------//
typedef struct {
	SHORT  numContours;
	SHORT  xMin;
	SHORT  yMin;
	SHORT  xMax;
	SHORT  yMax;
} GlyphHeader;

typedef struct {
	GlyphHeader header;
	USHORT numPoints;
	USHORT* endPoints;
	BYTE*  flags;
	SHORT* x, * y;
	USHORT myMetrics;
} GlyphOutline;

typedef struct {
	USHORT count;														// Number of points in the contour
	BYTE*  flags;														// On/off curve flags
	DBL*   x, * y;														// Coordinates of control vertices
} Contour;

typedef struct GlyphStruct *GlyphPtr;

typedef struct GlyphStruct {
	GlyphHeader header;													// Count and sizing information about this glyph
	USHORT glyph_index;											// Internal glyph index for this character
	Contour* contours;												// Array of outline contours
	GlyphPtr next;													// Next cached glyph
	USHORT c;														// Character code
	USHORT myMetrics;												// Which glyph index this is for metrics
} Glyph;

typedef struct FontFileInfoStruct {
	CHAR   filename[_MAX_PATH];
	FILE*  fp;
	USHORT platformID;												// Which character encoding to use
	USHORT specificID;
	ULONG  cmap_table_offset;										// File locations for these tables
	ULONG  glyf_table_offset;
	USHORT numGlyphs;												// How many symbols in this file
	USHORT unitsPerEm;												// The "resoultion" of this font
	SHORT indexToLocFormat;											// 0 - short format, 1 - long format
	ULONG* loca_table;												// Mapping from characters to glyphs
	GlyphPtr glyphs;													// Cached info for this font
	ULONG  glyphIDoffset;											// Offset for Type 4 encoding tables
	USHORT segCount, searchRange, entrySelector, rangeShift;		// Counts for Type 4 encoding tables
	USHORT* startCount, * endCount, * idDelta, * idRangeOffset;		// Type 4 (MS) encoding tables
	USHORT maxStorage;												// Storange Area Locations 数
	USHORT maxFunctionDefs;											// FDEF 数
	USHORT maxInstructionDefs;										// IDEF 数
	USHORT maxStackElements;										// 最大 Stack 数
	USHORT maxSizeOfInstructions;									// 最大 ｸﾞﾘﾌ ｲﾝｽﾄﾗｸｼｮﾝ ﾊﾞｲﾄ数
	ULONG  fpgm_length;												// Font Program Length
	BYTE*  fpgm_table;												// Font Program Table pointer
	ULONG  cvt_length;												// Control Value Length
	LONG*  cvt_table;												// Control Value Table pointer
} FontFileInfo;

typedef struct {
	CHAR* String;
	INT   StringLength;
	DBL   Depth;
	DBL   Threshold;
	INT   Resolution;
} ParamData;

//===========================<EOF:TrueTypeData.h>============================
