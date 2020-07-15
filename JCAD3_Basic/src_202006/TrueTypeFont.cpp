/////////////////////////////////////////////////////////////////////////////
// TrueTypeフォント立体作成部
// TrueTypeFont.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/04/09 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "resource.h"
#include "Jcad3Glb.h"
#include "Jcad3GlbView.h"
#include "DataAccess1.h"
#include "DataAccess2.h"
#include "CCommand.h"
#include "ECommand.h"
#include "NumFunc.h"
#include "Select.h"
#include "TrueTypeData.h"
#include "TrueTypeInstData.h"
#include "TrueTypeGeometry.h"
#include "TrueTypeVector.h"
#include "TrueTypeInst.h"

int m_TtfErr;															// TTF読込ｴﾗｰﾌﾗｸﾞ

/***********************************/
static void* TtfMalloc(size_t size)										//<<< ﾒﾓﾘ確保
/***********************************/
{
	void* block;

	block = (void *)malloc(size);
	if(block==NULL) {
		MemErr(IDS_MEMERR5);
	}
	return (void *)((char *)block);
}

/**************************************************/
static void* TtfCalloc(size_t nitems, size_t size)						//<<< ﾒﾓﾘ確保
/**************************************************/
{
	void*  block;
	size_t actsize;

	actsize = nitems*size;
	block = (void *)malloc(actsize);
	if(block==NULL) {
		MemErr(IDS_MEMERR5);
	}
	memset(block, 0, actsize);
	return (void *)((char *)block);
}

/******************************/
static void TtfFree(void* ptr)											//<<< ﾒﾓﾘ解放
/******************************/
{
	void* block;

	if(ptr!=NULL) {
		block = (void *)((char *)ptr); free(block);
	}
}

/***********************************************/
static void* TtfRealloc(void* ptr, size_t size)							//<<< ﾒﾓﾘ再割当
/***********************************************/
{
	void* block;

	if(size==0) {
		if(ptr) {
			TtfFree(ptr);
		}
		return NULL;
	} else if(ptr==NULL) {
		return TtfMalloc(size);
	}
	block = (void *)((char *)ptr);
	block = (void *)realloc(block, size);
	if(block==NULL) {
		MemErr(IDS_MEMERR5);
	}
	return (void *)((char *)block);
}

/*********************************/
static SHORT ReadShort(FILE* fp)										//<<< TTF SHORT入力
/*********************************/
{
	int i0, i1=0;

	if((i0=fgetc(fp))==EOF||(i1=fgetc(fp))==EOF) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	if(i0&0x80) {
		return -(((255-i0)<<8)|(255-i1))-1;
	} else {
		return (i0<<8)|i1;
	}
}

/**********************************/
static USHORT ReadUShort(FILE* fp)										//<<< TTF USHORT入力
/**********************************/
{
	int i0, i1=0;

	if((i0=fgetc(fp))==EOF||(i1=fgetc(fp))==EOF) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	return (USHORT)((((USHORT)i0)<<8)|((USHORT)i1));
}

/******************************/
static LONG ReadLong(FILE* fp)											//<<< TTF LONG入力
/******************************/
{
	LONG i0, i1=0, i2=0, i3=0;

	if((i0=fgetc(fp))==EOF||(i1=fgetc(fp))==EOF||
       (i2=fgetc(fp))==EOF||(i3=fgetc(fp))==EOF) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	if(i0 & 0x80) {
		return -(((255-i0)<<24)|((255-i1)<<16)|((255-i2)<<8)|(255-i3))-1;
	} else {
		return (i0<<24)|(i1<<16)|(i2<<8)|i3;
	}
}

/********************************/
static ULONG ReadULong(FILE* fp)										//<<< TTF ULONG入力
/********************************/
{
	int i0, i1=0, i2=0, i3=0;

	if((i0=fgetc(fp))==EOF||(i1=fgetc(fp))==EOF||
       (i2=fgetc(fp))==EOF||(i3=fgetc(fp))==EOF) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	return (ULONG)((((ULONG)i0)<<24)|(((ULONG)i1)<<16)|(((ULONG)i2)<<8)|((ULONG)i3));
}

/*****************************************************/
static int CompareTag(BYTE* ttf_tag, BYTE* known_tag)					//<<< TTF ﾀｸﾞの比較
/*****************************************************/
{
	return (ttf_tag[0]==known_tag[0]&&ttf_tag[1]==known_tag[1]&&
            ttf_tag[2]==known_tag[2]&&ttf_tag[3]==known_tag[3]);
}

/*************************************************/
static FontFileInfo *OpenFontFile(char* filename)						//<<< FFTﾌｧｲﾙOpen
/*************************************************/
{
	FontFileInfo* fontlist;

	fontlist = (FontFileInfo *)TtfCalloc(1, sizeof(FontFileInfo));		// 領域確保
	if(fopen_s(&fontlist->fp, filename, "rb")!=0) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTFOpenｴﾗｰ
		m_TtfErr = 1;
	}
	memcpy(fontlist->filename, filename, _MAX_PATH);
	fontlist->platformID = 3;
	fontlist->specificID = 1;
	return fontlist;
}

/****************************************************/
static void ProcessHeadTable(FontFileInfo* ffile,
                             long head_table_offset)					//<<< Font Header Table設定
/****************************************************/
{
	sfnt_FontHeader fontHeader;

	fseek(ffile->fp, head_table_offset, SEEK_SET);						// Read Head Table
	fontHeader.version = ReadLong(ffile->fp);
	fontHeader.fontRevision = ReadLong(ffile->fp);
	fontHeader.checkSumAdjustment = ReadULong(ffile->fp);
	fontHeader.magicNumber = ReadULong(ffile->fp);						// should be 0x5F0F3CF5
	fontHeader.flags = ReadUShort(ffile->fp);
	fontHeader.unitsPerEm = ReadUShort(ffile->fp);
	fontHeader.created.bc = ReadULong(ffile->fp);
	fontHeader.created.ad = ReadULong(ffile->fp);
	fontHeader.modified.bc = ReadULong(ffile->fp);
	fontHeader.modified.ad = ReadULong(ffile->fp);
	fontHeader.xMin = ReadShort(ffile->fp);
	fontHeader.yMin = ReadShort(ffile->fp);
	fontHeader.xMax = ReadShort(ffile->fp);
	fontHeader.yMax = ReadShort(ffile->fp);
	fontHeader.macStyle = ReadUShort(ffile->fp);
	fontHeader.lowestRecPPEM = ReadUShort(ffile->fp);
	fontHeader.fontDirectionHint = ReadShort(ffile->fp);
	fontHeader.indexToLocFormat = ReadShort(ffile->fp);
	fontHeader.glyphDataFormat = ReadShort(ffile->fp);
	if(fontHeader.magicNumber!=0x5F0F3CF5) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	ffile->indexToLocFormat = fontHeader.indexToLocFormat;
	ffile->unitsPerEm = fontHeader.unitsPerEm;
}

/****************************************************/
static void ProcessMaxpTable(FontFileInfo* ffile,
                             long maxp_table_offset)					//<<< Font Maxp Table設定
/****************************************************/
{
	USHORT temp16;

	fseek(ffile->fp, maxp_table_offset+4, SEEK_SET);
	ffile->numGlyphs = ReadUShort(ffile->fp);
	temp16 = ReadUShort(ffile->fp);
	temp16 = ReadUShort(ffile->fp);
	temp16 = ReadUShort(ffile->fp);
	temp16 = ReadUShort(ffile->fp);
	temp16 = ReadUShort(ffile->fp);
	temp16 = ReadUShort(ffile->fp);
	ffile->maxStorage = ReadUShort(ffile->fp);
	ffile->maxFunctionDefs = ReadUShort(ffile->fp);
	ffile->maxInstructionDefs = ReadUShort(ffile->fp);
	ffile->maxStackElements = ReadUShort(ffile->fp);
	ffile->maxSizeOfInstructions = ReadUShort(ffile->fp);
}

/****************************************************/
static void ProcessLocaTable(FontFileInfo* ffile,
                             long loca_table_offset)					//<<< Font Loca Table設定
/****************************************************/
{
	int i;

	fseek(ffile->fp, loca_table_offset, SEEK_SET);
	ffile->loca_table =
				(ULONG *)TtfMalloc((ffile->numGlyphs+1)*sizeof(ULONG));	// 領域確保
	if(ffile->indexToLocFormat==0) {									// short version
		for(i=0; i<ffile->numGlyphs; i++) {
			ffile->loca_table[i]=((ULONG)ReadUShort(ffile->fp))<<1;
		}
	} else {															// long version
		for(i=0; i<ffile->numGlyphs; i++) {
			ffile->loca_table[i] = ReadULong(ffile->fp);
		}
	}
}

/******************************************************************/
static void ProcessFpgmTable(FontFileInfo* ffile,
                             long table_offset, long table_length)		//<<< Font Fpgm Table設定
/******************************************************************/
{
	int i;

	fseek(ffile->fp, table_offset, SEEK_SET);
	ffile->fpgm_length = table_length;									// Font Program Length
	ffile->fpgm_table =(BYTE *)TtfMalloc((table_length+1)*sizeof(BYTE));// 領域確保
	for(i=0; i<table_length; i++) {
		if(fread(&ffile->fpgm_table[i], sizeof(BYTE), 1, ffile->fp)!=1){
			AfxMessageBox(IDS_ERR_TTF1);								// TTF入力ｴﾗｰ
			m_TtfErr = 1; break;
		}
	}
}

/*****************************************************************/
static void ProcessCvtTable(FontFileInfo* ffile,
                            long table_offset, long table_length)		//<<< Font Cvt Table設定
/*****************************************************************/
{
	int i;

	fseek(ffile->fp, table_offset, SEEK_SET);
	ffile->cvt_length = table_length;									// Control Value Length
	ffile->cvt_table = (LONG *)TtfMalloc((table_length+1)*sizeof(LONG));// 領域確保
	for(i=0; i<table_length; i++) {
		ffile->cvt_table[i] = (LONG)ReadShort(ffile->fp);
	}
}

/****************************************************/
static FontFileInfo* ProcessFontFile(char* filename)					//<<< TTFﾌｧｲﾙ情報設定
/****************************************************/
{
	FontFileInfo* ffile;
	sfnt_TTC_Header TTC_Header;
	sfnt_OffsetTable OffsetTable;
	sfnt_TableDirectory Table;
	unsigned i;
	long head_table_offset = 0;
	long loca_table_offset = 0;
	long maxp_table_offset = 0;
	long fpgm_table_offset = 0, fpgm_table_length = 0;
	long cvt_table_offset = 0, cvt_table_length = 0;

	ffile = OpenFontFile(filename);										// FFTﾌｧｲﾙOpen
	if(fread(&TTC_Header.Tag, sizeof(BYTE), 4, ffile->fp) != 4) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	if((TTC_Header.Tag[0]=='t')&&(TTC_Header.Tag[1]=='t')&&
       (TTC_Header.Tag[2]=='c')&&(TTC_Header.Tag[3]=='f')) {			// <TTCの場合>
		TTC_Header.version = ReadLong(ffile->fp);
		TTC_Header.DirectoryCount = ReadULong(ffile->fp);
		TTC_Header.TableDirectory = ReadULong(ffile->fp);
		fseek(ffile->fp, TTC_Header.TableDirectory, SEEK_SET);
	} else {															// <TTFの場合>
		fseek(ffile->fp, 0, SEEK_SET);
	}
	OffsetTable.version = ReadLong(ffile->fp);
	OffsetTable.numTables = ReadUShort(ffile->fp);
	OffsetTable.searchRange = ReadUShort(ffile->fp);
	OffsetTable.entrySelector = ReadUShort(ffile->fp);
	OffsetTable.rangeShift = ReadUShort(ffile->fp);
	if(OffsetTable.numTables>40) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	for(i=0; i<OffsetTable.numTables&&i<40; i++) {
		if(fread(&Table.tag, sizeof(BYTE), 4, ffile->fp)!=4) {
			AfxMessageBox(IDS_ERR_TTF1);								// TTF入力ｴﾗｰ
			m_TtfErr = 1;
		}
		Table.checkSum = ReadULong(ffile->fp);
		Table.offset = ReadULong(ffile->fp);
		Table.length = ReadULong(ffile->fp);
		if(CompareTag(Table.tag, tag_CharToIndexMap)) {
			ffile->cmap_table_offset = Table.offset;
		} else if(CompareTag(Table.tag, tag_GlyphData)) {
			ffile->glyf_table_offset = Table.offset;
		} else if(CompareTag(Table.tag, tag_FontHeader)) {
			head_table_offset = Table.offset;
		} else if(CompareTag(Table.tag, tag_IndexToLoc)) {
			loca_table_offset = Table.offset;
		} else if(CompareTag(Table.tag, tag_MaxProfile)) {
			maxp_table_offset = Table.offset;
		} else if(CompareTag(Table.tag, tag_FontProgram)) {
			fpgm_table_offset = Table.offset;
			fpgm_table_length = Table.length;
		} else if(CompareTag(Table.tag, tag_ControlValue)) {
			cvt_table_offset = Table.offset;
			cvt_table_length = Table.length;
		}
	}
	if(ffile->cmap_table_offset==0||ffile->glyf_table_offset==0||
       head_table_offset==0||loca_table_offset==0||
       maxp_table_offset==0) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	ProcessHeadTable(ffile, head_table_offset);							// Font Header Table設定
	if((ffile->indexToLocFormat!=0&&ffile->indexToLocFormat!=1)||
       (ffile->unitsPerEm<16||ffile->unitsPerEm>16384)) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	ProcessMaxpTable(ffile, maxp_table_offset);							// Font Maxp Table設定
	if(ffile->numGlyphs<=0) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	ProcessLocaTable(ffile, loca_table_offset);							// Font Loca Table設定
	if(fpgm_table_offset>0&&fpgm_table_length>0) {
		ProcessFpgmTable(ffile,fpgm_table_offset,fpgm_table_length);	// Font Fpgm Table設定
	}
	if(cvt_table_offset>0&&cvt_table_length>0) {
		ProcessCvtTable(ffile, cvt_table_offset, cvt_table_length);		// Font Cvt Table設定
	}
	return ffile;
}

/******************************************************/
static USHORT ProcessFormat0Glyph(FontFileInfo* ffile,
                                  unsigned int search)
/******************************************************/
{
	BYTE temp_idx;

	fseek(ffile->fp, (long)search, SEEK_CUR);
	if(fread(&temp_idx, 1, 1, ffile->fp)!=1) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	return (USHORT)(temp_idx);
}

/******************************************************/
static USHORT ProcessFormat4Glyph(FontFileInfo* ffile,
                                  unsigned int search)
/******************************************************/
{
	int      i;
	unsigned int glyph_idx=0;
	USHORT   temp16;

	if(ffile->segCount==0) {
		ffile->segCount = ReadUShort(ffile->fp)>>1;
		ffile->searchRange = ReadUShort(ffile->fp);
		ffile->entrySelector = ReadUShort(ffile->fp);
		ffile->rangeShift = ReadUShort(ffile->fp);
		ffile->endCount = (USHORT *)TtfMalloc(ffile->segCount*sizeof(USHORT));
		ffile->startCount = (USHORT *)TtfMalloc(ffile->segCount*sizeof(USHORT));
		ffile->idDelta = (USHORT *)TtfMalloc(ffile->segCount*sizeof(USHORT));
		ffile->idRangeOffset = (USHORT *)TtfMalloc(ffile->segCount*sizeof(USHORT));
		for(i=0; i<ffile->segCount; i++) {
			ffile->endCount[i] = ReadUShort(ffile->fp);
		}
		temp16 = ReadUShort(ffile->fp);
		for(i=0; i<ffile->segCount; i++) {
			ffile->startCount[i] = ReadUShort(ffile->fp);
		}
		for(i=0; i<ffile->segCount; i++) {
			ffile->idDelta[i] = ReadUShort(ffile->fp);
		}
		ffile->glyphIDoffset = ftell(ffile->fp);
		for(i=0; i<ffile->segCount; i++) {
			ffile->idRangeOffset[i] = ReadUShort(ffile->fp);
		}
	}
glyph_search:
	for(i=0; i< ffile->segCount; i++) {
		if(search<=ffile->endCount[i]) {
			if(search>=ffile->startCount[i]) {
				if(ffile->idRangeOffset[i]==0) {
					glyph_idx = search+ffile->idDelta[i];
				} else {
					fseek(ffile->fp, ffile->glyphIDoffset+2*i+ffile->idRangeOffset[i]+
                          2*(search-ffile->startCount[i]), SEEK_SET);
					glyph_idx = ReadUShort(ffile->fp);
					if(glyph_idx!=0) {
						glyph_idx = glyph_idx+ffile->idDelta[i];
					}
				}
			}
			break;
		}
	}
	if(glyph_idx==0&&search<0x100) {
		search+=0xF000;
		goto glyph_search;
	}
	return glyph_idx;
}

/******************************************************/
static USHORT ProcessFormat6Glyph(FontFileInfo* ffile,
                                  unsigned int search)
/******************************************************/
{
	USHORT firstCode, entryCount;
	BYTE   glyph_idx;

	firstCode = ReadUShort(ffile->fp);
	entryCount = ReadUShort(ffile->fp);
	if(search>=firstCode&&search<(unsigned int)(firstCode+entryCount)) {
		fseek(ffile->fp, (long)(search-firstCode), SEEK_CUR);
		glyph_idx = (BYTE)ReadUShort(ffile->fp);
	} else {
		glyph_idx = 0;
	}
	return glyph_idx;
}

/**********************************************************/
static USHORT ProcessCharMap(FontFileInfo* ffile,
                             unsigned int search, int mbc)				//<<< ｷｬﾗｸﾀﾏｯﾋﾟﾝｸﾞ取得
/**********************************************************/
{
	sfnt_platformEntry cmapEntry;
	sfnt_mappingTable  encodingTable;
	long    old_table_offset, entry_offset;
	int     i, table_count;
	BYTE    pmb[2];
	wchar_t pwc;

	if(mbc==1) {														// <1byteｺｰﾄﾞ>
		pmb[0] = search&0xFF;
		mbtowc(&pwc, (const char *)pmb, 1);
		search = (unsigned int)pwc;
	} else {															// <2byteｺｰﾄﾞ>
		pmb[0] = (search>>8)&0xFF;
		pmb[1] = search&0xFF;
		mbtowc(&pwc, (const char *)pmb, 2);
		search = (unsigned int)pwc;
		if(search<0x80) {
			return 0;
		}
	}
	fseek(ffile->fp, ffile->cmap_table_offset+2, SEEK_SET);
	table_count = ReadUShort(ffile->fp);
	for(i=0; i<table_count; i++) {
		cmapEntry.platformID = ReadUShort(ffile->fp);
		cmapEntry.specificID = ReadUShort(ffile->fp);
		cmapEntry.offset     = ReadULong(ffile->fp);
		if(ffile->platformID!=cmapEntry.platformID) {
			continue;
		}
		entry_offset = cmapEntry.offset;
		old_table_offset = ftell(ffile->fp);
		fseek(ffile->fp, ffile->cmap_table_offset+entry_offset, SEEK_SET);
		encodingTable.format = ReadUShort(ffile->fp);
		encodingTable.length = ReadUShort(ffile->fp);
		encodingTable.version = ReadUShort(ffile->fp);
		if(encodingTable.format==0) {
			return(ProcessFormat0Glyph(ffile, search));
		} else if(encodingTable.format==4) {
			return(ProcessFormat4Glyph(ffile, search));
		} else if(encodingTable.format==6) {
			return(ProcessFormat6Glyph(ffile, search));
		}
		fseek(ffile->fp, old_table_offset, SEEK_SET);
	}
	return 0;
}

/************************************************************/
static void InstGlyphOutline(GlyphOutline* ttglyph,
                             ExecContext exc, USHORT length)
/************************************************************/
{
	GlyphZone pts;
	int       i;

	pts.nPoint = ttglyph->numPoints;
	pts.nContour = ttglyph->header.numContours;
	pts.Org = (Vector *)TtfCalloc(pts.nPoint+2, sizeof(Vector));
	pts.Cur = (Vector *)TtfCalloc(pts.nPoint*2, sizeof(Vector));
	pts.Tag = (BYTE *)TtfCalloc(pts.nPoint+2, sizeof(BYTE));
	pts.Contour = (USHORT *)TtfCalloc(pts.nContour+2, sizeof(USHORT));
	for(i=0; i<pts.nContour; i++) {
		pts.Contour[i] = ttglyph->endPoints[i];
	}
	for(i=0; i<pts.nPoint; i++) {
		pts.Org[i].x = ttglyph->x[i];
		pts.Org[i].y = ttglyph->y[i];
		pts.Cur[i] = pts.Org[i];
		pts.Tag[i] = ttglyph->flags[i]&CURVE_TAG_ON;
	}
	exc->Pts = pts;
	SetCodeRange(exc, 2, exc->GlyphIns, length);
	if(RunContext(exc)==SUCCESS) {
		for(i=0; i<pts.nContour; i++) {
			ttglyph->endPoints[i] = pts.Contour[i];
		}
		for(i=0; i<pts.nPoint; i++) {
			ttglyph->x[i] = (SHORT)pts.Cur[i].x;
			ttglyph->y[i] = (SHORT)pts.Cur[i].y;
			ttglyph->flags[i] = pts.Tag[i];
		}
	}
	if(pts.Contour) {
		TtfFree(pts.Contour);
	}
	if(pts.Tag) {
		TtfFree(pts.Tag);
	}
	if(pts.Cur) {
		TtfFree(pts.Cur);
	}
	if(pts.Org) {
		TtfFree(pts.Org);
	}
}

/*****************************************************************/
static GlyphOutline *ExtractGlyphOutline(FontFileInfo* ffile,
                                         ExecContext exc,
                                         unsigned int* glyph_idx,
                                         unsigned int c)
/*****************************************************************/
{
	GlyphOutline* ttglyph, * sub_ttglyph;
	unsigned int  sub_glyph_index;
	int    i;
	long   current_pos;
	USHORT n, n2, temp16, flags, length;
	SHORT  nc, nc2, arg1, arg2, coord, xoff, yoff;
	BYTE   flag, repeat_count, temp8;
	CHAR   barg1, barg2;
	DBL    xscale, yscale, scale01, scale10;

	ttglyph = (GlyphOutline *)TtfCalloc(1, sizeof(GlyphOutline));
	ttglyph->myMetrics = *glyph_idx;
	if(c!=' ') {
		fseek(ffile->fp, ffile->glyf_table_offset+ffile->loca_table[*glyph_idx],
              SEEK_SET);
		ttglyph->header.numContours = ReadShort(ffile->fp);
		ttglyph->header.xMin = ReadShort(ffile->fp);
		ttglyph->header.yMin = ReadShort(ffile->fp);
		ttglyph->header.xMax = ReadShort(ffile->fp);
		ttglyph->header.yMax = ReadShort(ffile->fp);
	}
	nc = ttglyph->header.numContours;
	if(nc>0) {
		ttglyph->endPoints = (USHORT *)TtfMalloc(nc*sizeof(USHORT));
		for(i=0; i<nc; i++) {
			ttglyph->endPoints[i] = ReadUShort(ffile->fp);
		}
		length = ReadUShort(ffile->fp);									// ｸﾞﾘﾌ ｲﾝｽﾄﾗｸｼｮﾝ長
		for(i=0; i<length; i++) {
			if(fread(&exc->GlyphIns[i], sizeof(BYTE), 1, ffile->fp)!=1) {
				AfxMessageBox(IDS_ERR_TTF1);							// TTF入力ｴﾗｰ
				m_TtfErr = 1; break;
			}
		}
		n = ttglyph->numPoints = ttglyph->endPoints[nc-1]+1;
		ttglyph->flags = (BYTE *)TtfMalloc(n*sizeof(BYTE));
		for(i=0; i<ttglyph->numPoints; i++) {
			if(fread(&ttglyph->flags[i], sizeof(BYTE), 1, ffile->fp)!=1) {
				AfxMessageBox(IDS_ERR_TTF1);							// TTF入力ｴﾗｰ
				m_TtfErr = 1;
			}
			if(ttglyph->flags[i]&REPEAT_FLAGS) {
				if(fread(&repeat_count, sizeof(BYTE), 1, ffile->fp)!=1) {
					AfxMessageBox(IDS_ERR_TTF1);						// TTF入力ｴﾗｰ
					m_TtfErr = 1;
				}
				for(; repeat_count>0; repeat_count--, i++) {
					ttglyph->flags[i+1] = ttglyph->flags[i];
				}
			}
		}
		ttglyph->x = (SHORT *)TtfMalloc(n*sizeof(SHORT));
		ttglyph->y = (SHORT *)TtfMalloc(n*sizeof(SHORT));
		for(coord=0, i=0; i<ttglyph->numPoints; i++) {
			flag = ttglyph->flags[i];
			if(flag&XSHORT) {
				if(fread(&temp8, 1, 1, ffile->fp)!=1) {
					AfxMessageBox(IDS_ERR_TTF1);						// TTF入力ｴﾗｰ
					m_TtfErr = 1;
				}
				if(flag&SHORT_X_IS_POS) {
					coord += temp8;
				} else {
					coord -= temp8;
				}
			} else if(!(flag&NEXT_X_IS_ZERO)) {
				coord += ReadShort(ffile->fp);
			}
			if(coord>ttglyph->header.xMax) {
				ttglyph->header.xMax = coord;
			}
			if(coord<ttglyph->header.xMin) {
				ttglyph->header.xMin = coord;
			}
			ttglyph->x[i] = coord;
		}
		for(coord=0, i=0; i< ttglyph->numPoints; i++) {
			flag = ttglyph->flags[i];
			if(flag&YSHORT) {
				if(fread(&temp8, 1, 1, ffile->fp)!=1) {
					AfxMessageBox(IDS_ERR_TTF1);						// TTF入力ｴﾗｰ
					m_TtfErr = 1;
				}
				if(flag&SHORT_Y_IS_POS) {
					coord += temp8;
				} else {
					coord -= temp8;
				}
			} else if(!(flag&NEXT_Y_IS_ZERO)) {
				coord += ReadShort(ffile->fp);
			}
			if(coord>ttglyph->header.yMax) {
				ttglyph->header.yMax = coord;
			}
			if(coord<ttglyph->header.yMin) {
				ttglyph->header.yMin = coord;
			}
			ttglyph->y[i] = coord;
		}
	} else if(nc<0) {
		ttglyph->header.numContours = 0;
		ttglyph->numPoints = 0;
		do {
			xoff = 0;
			yoff = 0;
			xscale = 1;
			yscale = 1;
			scale01 = 0;
			scale10 = 0;
			flags = ReadUShort(ffile->fp);
			sub_glyph_index = ReadUShort(ffile->fp);
			if(flags&ARG_1_AND_2_ARE_WORDS) {
				arg1 = ReadShort(ffile->fp);
				arg2 = ReadShort(ffile->fp);
			} else {
				temp16 = ReadUShort(ffile->fp);
				barg1 = (temp16>>8)&0xFF;
				barg2 = temp16&0xFF;
				arg1 = (SHORT)barg1;
				arg2 = (SHORT)barg2;
			}
			if(flags&WE_HAVE_A_SCALE) {
				xscale = yscale = (DBL)ReadShort(ffile->fp)/0x4000;
			} else if(flags&WE_HAVE_AN_X_AND_Y_SCALE) {
				xscale = (DBL)ReadShort(ffile->fp)/0x4000;
				yscale = (DBL)ReadShort(ffile->fp)/0x4000;
			} else if(flags&WE_HAVE_A_TWO_BY_TWO) {
				xscale  = (DBL)ReadShort(ffile->fp)/0x4000;
				scale01 = (DBL)ReadShort(ffile->fp)/0x4000;
				scale10 = (DBL)ReadShort(ffile->fp)/0x4000;
				yscale  = (DBL)ReadShort(ffile->fp)/0x4000;
			}
			if(flags&ARGS_ARE_XY_VALUES) {
				xoff = arg1;
				yoff = arg2;
			} else {
				AfxMessageBox(IDS_ERR_TTF1);							// TTF入力ｴﾗｰ
				m_TtfErr = 1;
				continue;
			}
			current_pos = ftell(ffile->fp);
			sub_ttglyph = ExtractGlyphOutline(ffile, exc, &sub_glyph_index, c);
			if(flags&USE_MY_METRICS) {
				ttglyph->myMetrics = sub_glyph_index;
			}
			fseek(ffile->fp, current_pos, SEEK_SET);
			if((nc2=sub_ttglyph->header.numContours)==0) {
				continue;
			}
			nc = ttglyph->header.numContours;
			n = ttglyph->numPoints;
			n2 = sub_ttglyph->numPoints;
			ttglyph->endPoints = (USHORT *)TtfRealloc(ttglyph->endPoints,
													(nc+nc2)*sizeof(USHORT));
			ttglyph->flags = (BYTE *)TtfRealloc(ttglyph->flags, (n+n2)*sizeof(BYTE));
			ttglyph->x = (SHORT *)TtfRealloc(ttglyph->x, (n+n2)*sizeof(SHORT));
			ttglyph->y = (SHORT *)TtfRealloc(ttglyph->y, (n+n2)*sizeof(SHORT));
			ttglyph->header.numContours += nc2;
			ttglyph->numPoints += n2;
			for(i=0; i<nc2; i++) {
				ttglyph->endPoints[i+nc] = sub_ttglyph->endPoints[i]+n;
			}
			for(i=0; i<n2; i++) {
				ttglyph->flags[i+n] = sub_ttglyph->flags[i];
				ttglyph->x[i+n] = (SHORT)(xscale*sub_ttglyph->x[i]+
										scale01*sub_ttglyph->y[i]+xoff);
				ttglyph->y[i+n] = (SHORT)(scale10*sub_ttglyph->x[i]+
										yscale*sub_ttglyph->y[i]+yoff);
				if(ttglyph->x[i+n]<ttglyph->header.xMin) {
					ttglyph->header.xMin = ttglyph->x[i+n];
				}
				if(ttglyph->x[i+n]>ttglyph->header.xMax) {
					ttglyph->header.xMax = ttglyph->x[i+n];
				}
				if(ttglyph->y[i+n]<ttglyph->header.yMin) {
					ttglyph->header.yMin = ttglyph->y[i+n];
				}
				if(ttglyph->y[i+n]>ttglyph->header.yMax) {
					ttglyph->header.yMax = ttglyph->y[i+n];
				}
			}
			if(sub_ttglyph->y) {
				TtfFree(sub_ttglyph->y);
			}
			if(sub_ttglyph->x) {
				TtfFree(sub_ttglyph->x);
			}
			if(sub_ttglyph->flags) {
				TtfFree(sub_ttglyph->flags);
			}
			if(sub_ttglyph->endPoints) {
				TtfFree(sub_ttglyph->endPoints);
			}
			TtfFree(sub_ttglyph);
		} while(flags&MORE_COMPONENTS);
		if(flags&WE_HAVE_INSTRUCTIONS) {
			length = ReadUShort(ffile->fp);								// ｸﾞﾘﾌ ｲﾝｽﾄﾗｸｼｮﾝ長
			for(i=0; i<length; i++) {
				if(fread(&exc->GlyphIns[i], sizeof(BYTE), 1, ffile->fp)!=1) {
					AfxMessageBox(IDS_ERR_TTF1);						// TTF入力ｴﾗｰ
					m_TtfErr = 1; break;
				}
			}
			InstGlyphOutline(ttglyph, exc, length);
		}
	}
	return ttglyph;
}

/*********************************************************************/
static GlyphPtr ConvertOutlineToGlyph(FontFileInfo* ffile,
                                      GlyphOutline* ttglyph, int rvs)
/*********************************************************************/
{
	GlyphPtr glyph;
	DBL*     temp_x, * temp_y, s_x, s_y;
	BYTE*    temp_f, s_f;
	USHORT   i, j, k, id, last_j, st, ed;

	glyph = (Glyph *)TtfMalloc(sizeof(Glyph));
	for(i=0, id=0, last_j=0; i<(USHORT)ttglyph->header.numContours; i++) {
		j = ttglyph->endPoints[i]-last_j+1;
		if(j>1) {
			id++;
		}
		last_j = ttglyph->endPoints[i] + 1;
	}
	if(id>0) {
		glyph->contours = (Contour *)TtfMalloc(id*sizeof(Contour));
	} else {
		glyph->contours = NULL;
	}
	memcpy(&glyph->header, &ttglyph->header, sizeof(GlyphHeader));
	glyph->header.numContours = id;
	for(i=0, id=0, last_j=0; i<(USHORT)ttglyph->header.numContours; i++) {
		j = ttglyph->endPoints[i]-last_j+1;
		if(j>1) {
			temp_x = (DBL *)TtfMalloc((j+1)*sizeof(DBL));
			temp_y = (DBL *)TtfMalloc((j+1)*sizeof(DBL));
			temp_f = (BYTE *)TtfMalloc((j+1)*sizeof(BYTE));
			for(k=0; k<j; k++) {
				temp_x[k] = (DBL)ttglyph->x[last_j+k]/(DBL)ffile->unitsPerEm;
				temp_y[k] = (DBL)ttglyph->y[last_j+k]/(DBL)ffile->unitsPerEm;
				temp_f[k] = ttglyph->flags[last_j+k];
			}
			temp_x[j] = (DBL)ttglyph->x[last_j]/(DBL)ffile->unitsPerEm;
			temp_y[j] = (DBL)ttglyph->y[last_j]/(DBL)ffile->unitsPerEm;
			temp_f[j] = ttglyph->flags[last_j];
			glyph->contours[id].count = j;
			glyph->contours[id].x = temp_x;
			glyph->contours[id].y = temp_y;
			glyph->contours[id].flags = temp_f;
			id++;
		}
		last_j = ttglyph->endPoints[i] + 1;
	}
	if(rvs) {															// <<ﾙｰﾌﾟ反転の場合>>
		for(i=0; i<glyph->header.numContours; i++) {
			st = 0;														// 最小/最大番号
			ed = glyph->contours[i].count-1;
			while(st<ed) {												// <最大<最小迄>
				s_x = glyph->contours[i].x[st];							// 最小側保存
				s_y = glyph->contours[i].y[st];
				s_f = glyph->contours[i].flags[st];
				glyph->contours[i].x[st] = glyph->contours[i].x[ed];	// 最大側->最小側
				glyph->contours[i].y[st] = glyph->contours[i].y[ed];
				glyph->contours[i].flags[st] = glyph->contours[i].flags[ed];
				glyph->contours[i].x[ed] = s_x;							// 最小側->最大側
				glyph->contours[i].y[ed] = s_y;
				glyph->contours[i].flags[ed] = s_f;
				ed--;													// 最大減算/最小加算
				st++;
			}
		}
	}
	return glyph;
}

/**********************************************************************/
static GlyphPtr ExtractGlyphInfo(FontFileInfo* ffile, ExecContext exc,
                                 unsigned int* glyph_idx,
                                 unsigned int c, int rvs)
/**********************************************************************/
{
	GlyphOutline* ttglyph;
	GlyphPtr      glyph;

	ttglyph = ExtractGlyphOutline(ffile, exc, glyph_idx, c);
	glyph = ConvertOutlineToGlyph(ffile, ttglyph, rvs);
	glyph->c = c;
	glyph->glyph_index = *glyph_idx;
	glyph->myMetrics = ttglyph->myMetrics;
	if(ttglyph) {
		if(ttglyph->y) {
			TtfFree(ttglyph->y);
		}
		if(ttglyph->x) {
			TtfFree(ttglyph->x);
		}
		if(ttglyph->flags) {
			TtfFree(ttglyph->flags);
		}
		if(ttglyph->endPoints) {
			TtfFree(ttglyph->endPoints);
		}
		TtfFree(ttglyph);
	}
	return glyph;
}

/**********************************************************************/
static GlyphPtr ProcessCharacter(FontFileInfo* ffile, ExecContext exc,
                                 unsigned int search,
                                 unsigned int* glyph_idx,
                                 int mbc, int rvs)						//<<< 輪郭ﾃﾞｰﾀ検索
/**********************************************************************/
{
	GlyphPtr glyph;

	for(glyph=ffile->glyphs; glyph!=NULL; glyph=glyph->next) {			// 処理済を検索
		if(glyph->c==search) {
			*glyph_idx = glyph->glyph_index;							// 処理済は処理中止
			return glyph;
		}
	}
	*glyph_idx = ProcessCharMap(ffile, search, mbc);					// ｷｬﾗｸﾀﾏｯﾋﾟﾝｸﾞ取得
	for(glyph=ffile->glyphs; glyph!=NULL; glyph=glyph->next) {
		if(glyph->glyph_index==*glyph_idx) {
			*glyph_idx = glyph->glyph_index;							// 処理済は処理中止
			return glyph;
		}
	}
	glyph = ExtractGlyphInfo(ffile, exc, glyph_idx, search, rvs);
	glyph->next = ffile->glyphs;
	ffile->glyphs = glyph;
	return glyph;
}

/*********************************************/
static void FreeFontInfo(FontFileInfo* ffile)							//<<< TTFﾌｧｲﾙ情報領域解放
/*********************************************/
{
	GlyphPtr glyphs, tempglyph;
	int      i;

	if(ffile->fp!=NULL) {
		fclose(ffile->fp);												// TTFﾌｧｲﾙClose
	}
	for(glyphs=ffile->glyphs; glyphs!=NULL;) {
		for(i=0; i<glyphs->header.numContours; i++) {
			TtfFree(glyphs->contours[i].flags);
			TtfFree(glyphs->contours[i].y);
			TtfFree(glyphs->contours[i].x);
		}
		if(glyphs->contours!=NULL) {
			TtfFree(glyphs->contours);
		}
		tempglyph = glyphs;
		glyphs = glyphs->next;
		TtfFree(tempglyph);
	}
	if(ffile->segCount!=0) {
		TtfFree(ffile->idRangeOffset);
		TtfFree(ffile->idDelta);
		TtfFree(ffile->startCount);
		TtfFree(ffile->endCount);
	}
	if(ffile->cvt_table!=NULL) {
		TtfFree(ffile->cvt_table);
	}
	if(ffile->fpgm_table!=NULL) {
		TtfFree(ffile->fpgm_table);
	}
	if(ffile->loca_table!=NULL) {
		TtfFree(ffile->loca_table);
	}
	TtfFree(ffile);
}

/*******************************************************************/
static void PolygonizeContour(GlyphPtr glyph, int ctnum, int reslt,
                              double depth, POLYGON& polygon)			//<<< Polygonﾃﾞｰﾀ出力
/*******************************************************************/
{
	vector p1, p2, p3;
	int    i, j, c, fontpcnt, polypcnt=0, polyi=0, p1type, p2type, p3type;
	double t;

	if(glyph->header.numContours<1) {
		return;
	}
	if(ctnum>=glyph->header.numContours) {
		return;
	}
	fontpcnt = glyph->contours[ctnum].count;
	for(i=1; i<fontpcnt; i++) {
		p1type = glyph->contours[ctnum].flags[i-1]&ONCURVE;
		p2type = glyph->contours[ctnum].flags[i]&ONCURVE;
		j = (i==fontpcnt-1) ? 0 : i+1;
		p3type = glyph->contours[ctnum].flags[j]&ONCURVE;
		if(p1type) {
			if(p2type) {
				polypcnt++;
				if(i==fontpcnt-1) {
					polypcnt++;
				}
			} else {
				if(p3type) {
					polypcnt += reslt;
					if(i==fontpcnt-2) {
						polypcnt++;
					}
					i++;
				} else {
					polypcnt += reslt;
				}
			}
		} else {
			if(!p2type) {
				if(p3type) {
					polypcnt += reslt;
					if(i==fontpcnt-2) {
						polypcnt++;
					}
					i++;
				} else {
					polypcnt += reslt;
				}
			}
		}
	}
	polygon.numpoints = polypcnt;
	if(polygon.pointlist!=NULL) {
		delete polygon.pointlist;
	}
	if(polypcnt==0) {
		polygon.pointlist = NULL;
	} else {
		polygon.pointlist = new vector[polypcnt];
	}
	for(i=1; i<fontpcnt; i++) {
		p1type = glyph->contours[ctnum].flags[i-1]&ONCURVE;
		p1 = vector(glyph->contours[ctnum].x[i-1],
                    glyph->contours[ctnum].y[i-1], depth);
		p2type = glyph->contours[ctnum].flags[i]&ONCURVE;
		p2 = vector(glyph->contours[ctnum].x[i],
                    glyph->contours[ctnum].y[i], depth);
		j = (i==fontpcnt-1) ? 0 : i+1;
		p3type = glyph->contours[ctnum].flags[j]&ONCURVE;
		p3 = vector(glyph->contours[ctnum].x[j],
                    glyph->contours[ctnum].y[j], depth);
		if(p1type) {
			if(p2type) {
				polygon.pointlist[polyi++] = p1;
				if(i==fontpcnt-1) {
					polygon.pointlist[polyi++]=p2;
				}
			} else {
				if(p3type) {
					for(c=0; c<reslt; c++) {
						t = (DBL)c/reslt;
						polygon.pointlist[polyi++] =
						ApproximateQuadraticSpline(p1, p2, p3, t);
					}
					if(i==fontpcnt-2) {
						polygon.pointlist[polyi++]=p3;
					}
					i++;
				} else {
					for(c=0; c<reslt; c++) {
						t = (DBL)c/reslt;
						polygon.pointlist[polyi++] =
						ApproximateQuadraticSpline(p1, p2, midpoint(p2,p3), t);
					}
				}
			}
		} else {
			if(!p2type) {
				if(p3type) {
					for(c=0; c<reslt; c++) {
						t = (DBL)c/reslt;
						polygon.pointlist[polyi++] =
						ApproximateQuadraticSpline(midpoint(p1,p2),p2, p3, t);
					}
					if(i==fontpcnt-2) {
						polygon.pointlist[polyi++]=p3;
					}
					i++;
				} else {
					for(c=0; c<reslt; c++) {
						t = (DBL)c/reslt;
						polygon.pointlist[polyi++] =
						ApproximateQuadraticSpline(midpoint(p1,p2),p2, midpoint(p2,p3), t);
					}
				}
			}
		}
	}
	if(polygon.numpoints>0) {
		if(polygon.pointlist[0]==polygon.pointlist[polygon.numpoints-1]) {
			polygon.numpoints--;
		}
		polygon.Correct();
		polygon.orientation = polygon.findOrientation();
	}
}

/*******************************************************/
static int CreateFaces(GlyphPtr glyph, ParamData param,
                       TRIANGLELIST& frontTriangleList,
                       TRIANGLELIST& backTriangleList)					//<<< 平面ﾃﾞｰﾀ出力
/*******************************************************/
{
	POLYGON* frontPolyArray=NULL;
	POLYGON* backPolyArray=NULL;
	POLYGON* testPolyArray;
	int      i, j, k, outermCont, isAPair, ctCount;
	int**    relationship;
	double   frontZPos, backZPos;

	ctCount = glyph->header.numContours;
	frontZPos = param.Depth/2;
	backZPos  = -param.Depth/2;
	if(ctCount<1) {
		return 1;
	}
	frontPolyArray = new POLYGON[ctCount];
	if(frontPolyArray==NULL) {
		return 1;
	}
	backPolyArray = new POLYGON[ctCount];
	if(backPolyArray==NULL) {
		if(frontPolyArray) {
			delete frontPolyArray;
		}
		return 1;
	}
	relationship = new int* [ctCount];
	if(relationship==NULL) {
		if(frontPolyArray) {
			delete frontPolyArray;
		}
		if(backPolyArray) {
			delete backPolyArray;
		}
		return 1;
	}
	for(i=0; i<ctCount; i++) {
		relationship[i] = new int[ctCount];
		if(relationship[i]==NULL) {
			if(frontPolyArray) {
				delete frontPolyArray;
			}
			if(backPolyArray) {
				delete frontPolyArray;
			}
			for(j=0; j<i; j++) {
				delete relationship[j];
			}
			delete relationship;
			return 1;
		}
	}
	testPolyArray = frontPolyArray;
	for(i=0; i<ctCount; i++) {
		PolygonizeContour(glyph, i, param.Resolution,
                          frontZPos, frontPolyArray[i]);
		PolygonizeContour(glyph, i, param.Resolution,
                          backZPos, backPolyArray[i]);
	}
	for(i=0; i<ctCount; i++) {
		for(j=0; j<ctCount; j++) {
			if(i==j) {
				relationship[i][j] = TT_IGNORE;
			} else if(testPolyArray[j].isInside(testPolyArray[i])) {
				relationship[i][j] = TT_INSIDE;
			} else {
				relationship[i][j] = TT_OUTSIDE;
			}
		}
	}
	for(i=0; i<ctCount; i++) {
		if(testPolyArray[i].orientation==CLOCKWISE) {
			for(j=0; j<ctCount; j++) {
				isAPair=FALSE;
				if(relationship[i][j]==TT_INSIDE&&
					testPolyArray[j].orientation==ANTICLOCKWISE) {
					isAPair=TRUE;
					for(k=0; k<ctCount; k++) {
						if(k==j||k==i||relationship[k][j]==TT_IGNORE) {
							continue;
						}
						if(relationship[k][j]==TT_INSIDE) {
							if(relationship[i][k]==TT_INSIDE) {
								isAPair=FALSE; break;
							}
						} else {
							continue;
						}
					}
				}
				if(isAPair) {
					frontPolyArray[i].Combine(frontPolyArray[j]);
					backPolyArray[i].Combine(backPolyArray[j]);
					relationship[i][j] = TT_COMBINED;
				}
			}
		}
	}
	for(i=0; i<ctCount; i++) {
		if(frontPolyArray[i].orientation==CLOCKWISE) {
			frontPolyArray[i].SetDepth(frontZPos);
			frontPolyArray[i].Triangulate(frontTriangleList);
		} else {
			outermCont = TRUE;
			for(j=0; j<ctCount; j++) {
				if(relationship[j][i]==TT_INSIDE||relationship[j][i]==TT_COMBINED) {
					outermCont = FALSE; break;
				}
			}
			if(outermCont) {
				frontPolyArray[i].SetDepth(frontZPos);
				frontPolyArray[i].Triangulate(frontTriangleList);
			}
		}
		if(backPolyArray[i].orientation==CLOCKWISE) {
			backPolyArray[i].SetDepth(backZPos);
			backPolyArray[i].Triangulate(backTriangleList);
		} else {
			outermCont = TRUE;
			for(j=0; j<ctCount; j++) {
				if(relationship[j][i]==TT_INSIDE||relationship[j][i]==TT_COMBINED) {
					outermCont = FALSE; break;
				}
			}
			if(outermCont) {
				backPolyArray[i].SetDepth(backZPos);
				backPolyArray[i].Triangulate(backTriangleList);
			}
		}
	}
	for(i=0; i<ctCount; i++) {											// ﾒﾓﾘ解放
		if(frontPolyArray[i].pointlist) {
			delete frontPolyArray[i].pointlist;
		}
		if(backPolyArray[i].pointlist) {
			delete backPolyArray[i].pointlist;
		}
		if(relationship[i]) {
			delete relationship[i];
		}
	}
	if(frontPolyArray) {
		delete frontPolyArray;
	}
	if(backPolyArray) {
		delete backPolyArray;
	}
	if(relationship) {
		delete relationship;
	}
	return 0;
}

/*******************************************************/
static int CreateSides(GlyphPtr glyph, ParamData param,
                       TRIANGLELIST& sideTriangleList)					//<<< 側面ﾃﾞｰﾀ出力
/*******************************************************/
{
	TRIANGLE* t1;
	TRIANGLE* t2;
	POLYGON*  polyArray;
	vector    previous, current1, current2, next;
	vector    previousFacet, currentFacet, nextFacet;
	vector    previousFacetNormal, currentFacetNormal, nextFacetNormal;
	vector    averageNormal1, averageNormal2, p1, p2, p3, p4, zDir(0,0,1);
	USHORT    i, j;
	int       numberOfPoints, ctCount;
	double    angle1, angle2, bisectorLength1, bisectorLength2;
	double    zpos1, zpos2, zpos3, zpos4;

	ctCount = glyph->header.numContours;
	if(ctCount<1) {
		return 1;
	}
	zpos1 = param.Depth/2;
	zpos2 = param.Depth/2;
	zpos3 = -param.Depth/2;
	zpos4 = -param.Depth/2;
	polyArray = new POLYGON[ctCount];
	if(polyArray==NULL) {
		return 1;
	}
	for(i=0; i<ctCount; i++) {
		PolygonizeContour(glyph, i, param.Resolution, 0.0, polyArray[i]);
	}
	for(i=0; i<ctCount; i++) {
		numberOfPoints = polyArray[i].numpoints;
		for(j=0; j<numberOfPoints; j++) {
			if(j==numberOfPoints-2) {
				previous = polyArray[i].pointlist[j-1];
				current1 = polyArray[i].pointlist[j];
				current2 = polyArray[i].pointlist[j+1];
				next = polyArray[i].pointlist[0];
			} else if(j==numberOfPoints-1) {
				previous = polyArray[i].pointlist[j-1];
				current1 = polyArray[i].pointlist[j];
				current2 = polyArray[i].pointlist[0];
				next = polyArray[i].pointlist[1];
			} else if(j==0) {
				previous = polyArray[i].pointlist[numberOfPoints-1];
				current1 = polyArray[i].pointlist[j];
				current2 = polyArray[i].pointlist[j+1];
				next = polyArray[i].pointlist[j+2];
			} else {
				previous = polyArray[i].pointlist[j-1];
				current1 = polyArray[i].pointlist[j];
				current2 = polyArray[i].pointlist[j+1];
				next = polyArray[i].pointlist[j+2];
			}
			previousFacet = ~(current1-previous);
			currentFacet = ~(current2-current1);
			nextFacet = ~(next-current2);
			previousFacetNormal = zDir^previousFacet;
			currentFacetNormal = zDir^currentFacet;
			nextFacetNormal = zDir^nextFacet;
			bisectorLength1 = previousFacetNormal%currentFacetNormal;
			if(bisectorLength1<-1.0) {									// Make sure acos gets a number
				bisectorLength1 = -1.0;
			} else if(bisectorLength1>1.0) {							//   between -1 and 1.
				bisectorLength1 = 1.0;
			}
			bisectorLength2 = currentFacetNormal%nextFacetNormal;
			if(bisectorLength2<-1.0) {									// Make sure acos gets a number
				bisectorLength2 = -1.0;
			} else if(bisectorLength2>1.0) {							//   between -1 and 1.
				bisectorLength2 = 1.0;
			}
			angle1 = acos(bisectorLength1);
			angle2 = acos(bisectorLength2);
			if(angle1<param.Threshold) {
				averageNormal1 = ~(previousFacetNormal+currentFacetNormal);
			} else {
				averageNormal1 = currentFacetNormal;
			}
			if(angle2<param.Threshold) {
				averageNormal2 = ~(currentFacetNormal+nextFacetNormal);
			} else {
				averageNormal2 = currentFacetNormal;
			}
			p1 = current1; p1.z=zpos2;
			p2 = current2; p2.z=zpos2;
			p3 = current1; p3.z=zpos3;
			p4 = current2; p4.z=zpos3;
			t1 = new TRIANGLE(p1,p3,p4,averageNormal1,averageNormal1,averageNormal2);
			if(t1==NULL) {
				MemErr(IDS_MEMERR5);
			}
			t2 = new TRIANGLE(p1,p4,p2,averageNormal1,averageNormal2,averageNormal2);
			if(t2==NULL) {
				MemErr(IDS_MEMERR5);
			}
			sideTriangleList.Add(t1);
			sideTriangleList.Add(t2);
		}
	}
	for(i=0; i<ctCount; i++) {
		if(polyArray[i].pointlist) {
			delete polyArray[i].pointlist;
		}
	}
	if(polyArray) {
		delete polyArray;
	}
	return 0;
}

/**********************************************************/
static void OutputTriangleData(TRIANGLELIST& triangleList,
                               int flg, double scale)					//<<< 三角ﾊﾟｯﾁ出力
/**********************************************************/
{
	TRIANGLE* t;
	PNTTYPE   p1, p2, p3;
	int       i;

	triangleList.gotoFirst();
	for(i=0; i<triangleList.Count(); i++) {
		t = triangleList.Current();
		p1.p[0] = t->v1.x*scale;
		p1.p[1] = t->v1.y*scale;
		p1.p[2] = t->v1.z;
		p2.p[0] = t->v2.x*scale;
		p2.p[1] = t->v2.y*scale;
		p2.p[2] = t->v2.z;
		p3.p[0] = t->v3.x*scale;
		p3.p[1] = t->v3.y*scale;
		p3.p[2] = t->v3.z;
		if(!flg) {														// ｿｰﾄ頂点設定
			SetCVtx(p1);
			SetCVtx(p2);
			SetCVtx(p3);
		} else {
			SetCVtx(p3);
			SetCVtx(p2);
			SetCVtx(p1);
		}
		SetCFace();														// 面ﾙｰﾌﾟ終了設定
		triangleList.gotoNext();
	}
}

/*************************************************************/
void CreatTrueTypeFont(char* file, char* text, double depth,
                       double height, double intval, int rvs)			//<<< TrueType立体生成
/*************************************************************/
{
	ParamData     Param;
	FontFileInfo* ffile;
	ExecContext	  exc;
	GlyphPtr      glyph;
	TRIANGLELIST  frontFaceTriangleList;
	TRIANGLELIST  backFaceTriangleList;
	TRIANGLELIST  sideTriangleList;
	OBJTYPE*      op;
	OIFTYPE*      oif;
	PNTTYPE       gcnt, cpt, bcnt, mov;
	VECTYPE       vec;
	BOXTYPE       box;
	int           i, j, slen, mbc, tglCnt, ret, vn, ln, tvn, sflg, st;
	unsigned int  search, glyph_idx, last_idx=0;
	double        size, scale, ylen;

	m_TtfErr = 0;
	vec.p[0] = 0.0, vec.p[1] = 0.0, vec.p[2] = -1.0;					// 回転ﾍﾞｸﾄﾙ
	cpt.p[0] = 0.0, cpt.p[1] = 0.0, cpt.p[2] = 0.0;						// 回転中心点
	slen = strlen(text);												// 文字数取得
	Param.String = text;												// ﾊﾟﾗﾒｰﾀ設定
	Param.StringLength = slen;
	Param.Depth = depth;
	Param.Threshold = 0.5236;
	Param.Resolution = 5;
	ffile = ProcessFontFile(file);										// TTFﾌｧｲﾙ情報設定
	if(m_TtfErr) {														// TTF読込ｴﾗｰは処理中止
		return;
	}
	exc = NewContext(ffile);											// ﾌｫﾝﾄﾌﾟﾛｸﾞﾗﾑ取得
	StartMenu();														// ﾒﾆｭｰ開始処理
	InitLevel();														// Undo/Redo初期化
	View->GetCentPnt(&gcnt);											// 3面図中心点
	OffEdtAllObject();													// 編集立体全解除
	for(i=0, st=0; i<slen; i++) {
		sflg = 0;														// ｽﾍﾟｰｽﾌﾗｸﾞ初期化
		mbc = mblen(&text[i], MB_CUR_MAX);
		if(mbc==1) {													// <1byteｺｰﾄﾞ>
			search = ((unsigned int)text[i])&0xff;
			if(search==SPACE_CODE1) {									// 1byteｺｰﾄﾞのｽﾍﾟｰｽ
				sflg = 1;
			}
		} else {														// <2byteｺｰﾄﾞ>
			search = ((((unsigned int)text[i])&0xff)<<8)|
                      (((unsigned int)text[i+1])&0xff);
			if(search==SPACE_CODE2) {									// 2byteｺｰﾄﾞのｽﾍﾟｰｽ
				sflg = 2;
			}
		}
		if(!sflg) {														// <<ｽﾍﾟｰｽｺｰﾄﾞ以外の場合>>
			glyph = ProcessCharacter(ffile, exc, search, &glyph_idx,
                                     mbc, rvs);							// 輪郭ﾃﾞｰﾀ取得
			if(mbc!=1&&glyph_idx==0) {									// <2byteで輪郭なし>
				mbc = 1;
				search = ((unsigned int)text[i])&0xff;					// 1byteｺｰﾄﾞとして
				glyph = ProcessCharacter(ffile, exc, search, &glyph_idx,
                                         mbc, rvs);						// 輪郭ﾃﾞｰﾀ取得
			}
			if(m_TtfErr) {
				break;
			}
			if(glyph_idx==0) {
				AfxMessageBox(IDS_ERR_TTF2);							// 指定文字無ｴﾗｰ
				break;
			}
			ret = CreateFaces(glyph, Param, frontFaceTriangleList,
                              backFaceTriangleList);					// 平面ﾃﾞｰﾀ作成
			if(ret!=0) {
				break;
			}
			ret = CreateSides(glyph, Param, sideTriangleList);			// 側面ﾃﾞｰﾀ作成
			if(ret!=0) {
				frontFaceTriangleList.Empty();
				backFaceTriangleList.Empty();
				break;
			}
			tglCnt = frontFaceTriangleList.Count();						// 三角ﾊﾟｯﾁ面数集計
			tglCnt += backFaceTriangleList.Count();
			tglCnt += sideTriangleList.Count();
			vn = tglCnt*3;
			ln = tglCnt*3;
			if(!st) {													// <先頭の場合>
				size = (DBL)(glyph->header.yMax-glyph->header.yMin)/	// Y方向のﾌｫﾝﾄｻｲｽﾞ
                       (DBL)ffile->unitsPerEm;
				scale = height/size;									// 倍率
			}
			InitCVtx();													// 頂点ｿｰﾄ初期処理
			OutputTriangleData(frontFaceTriangleList, 1, scale);		// 三角ﾊﾟｯﾁ出力
			OutputTriangleData(backFaceTriangleList, 0, scale);
			OutputTriangleData(sideTriangleList, 1, scale);
			if((tvn=SortCVtx())>0) {									// 頂点情報のｿｰﾄ&ｶｳﾝﾄ
				CreatObj(tvn, ln, 0, &op);								// 立体領域確保
				LevelObj(0, op);										// 同一ﾚﾍﾞﾙ生成立体接続
				SetCVtxObj(op);											// 頂点情報設定
				PutObjAtr(op, 0);										// 属性No設定(DIRECT)
				PutObjPrt(op, 0, 1);									// ﾊﾟｰﾂNo設定(通常)
				PutObjCol(op, 0);										// ｶﾗｰNo設定(ｺﾞｰﾙﾄﾞ)
				SetObjBox(op);											// ﾎﾞｯｸｽｻｲｽﾞ設定
				MakeDirectCommand(op);									// 直接生成ｺﾏﾝﾄﾞ生成
				ObjRotate(op, cpt, vec, -90.0);							// 立体回転移動
				GetObjBox(op, &box);									// BOXｻｲｽﾞ取得
				BoxCenter(box, &bcnt);									// BOXｾﾝﾀｰ取得
				ylen = box.bp[1].p[1]-box.bp[0].p[1];					// Y方向ｻｲｽﾞ
				if(!st) {												// <先頭の場合>
					for(j=0; j<DIM; j++) {
						mov.p[j] = gcnt.p[j]-bcnt.p[j];					// 立体中心を画面中心へ移動
					}
					st = 1;
				} else {												// <先頭以外の場合>
					gcnt.p[1] += ylen/2.0f;								// 立体移動位置へ移動
					mov.p[1] = gcnt.p[1]-bcnt.p[1];						// 現中心との差
				}
				ObjMove(op, mov.p[0], mov.p[1], mov.p[2]);				// 立体平行移動
				View->SetDispList(op);									// ﾃﾞｨｽﾌﾟﾚｲ･ﾘｽﾄ登録
				GetObjOif(op, &oif);									// 立体構成情報ﾎﾟｲﾝﾀ取得
				SetOifName(1, oif, OBJ_NAME32);							// 体構成名(ﾃｷｽﾄ)
				OnEdtFlg(1, op);										// 編集立体設定
			}
			frontFaceTriangleList.Empty();								// 作業領域解放
			backFaceTriangleList.Empty();
			sideTriangleList.Empty();
			gcnt.p[1] += ylen/2.0+intval;								// 次立体の端点へ移動
		} else if(sflg==1) {
			gcnt.p[1] += height*0.5+intval;								// 半角ｽﾍﾟｰｽ分移動
		} else {
			gcnt.p[1] += height+intval;									// 全角ｽﾍﾟｰｽ分移動
		}
		i += (mbc-1);													// 2byteｺｰﾄﾞは+1
	}
	DelContext(exc);													// ｲﾝｽﾄﾗｸｼｮﾝ領域解放
	FreeFontInfo(ffile);												// TTFﾌｧｲﾙ情報領域解放
	EndJMenu1();														// ﾒﾆｭｰ終了処理
	View->CountEdtObj();												// 編集対象立体ｶｳﾝﾄ
	View->SetDataUse();													// ﾃﾞｰﾀ使用率設定
	View->AutoZoom();													// 3面図の自動倍率
}

/**************************************************************/
static void ProcessNameTable(FILE* fp, long name_table_offset,
                             int nid, char* fname)						//<<< Font Name Table設定
/**************************************************************/
{
	int     i, j, k, l;
	ULONG   stringStorageOffset, old_offset;
	USHORT  numNameRecords, PlatformID, SpecificID, LanguageID;
	USHORT  NameID, StringLength, StringOffset;
	BYTE    buf[2];
	char    pmb[2];
	wchar_t pwc;

	fseek(fp, name_table_offset, SEEK_SET);
	ReadUShort(fp);														// Skip Format Selector
	numNameRecords = ReadUShort(fp);									// Get number of Name Records
	stringStorageOffset = (ULONG)ReadUShort(fp);						// Get offset from beginning of
	stringStorageOffset += name_table_offset;							// Make this a file offset
	for(i=0; i<numNameRecords; i++) {
		PlatformID = ReadUShort(fp);
		SpecificID = ReadUShort(fp);
		LanguageID = ReadUShort(fp);
		NameID = ReadUShort(fp);
		StringLength = ReadUShort(fp);
		StringOffset = ReadUShort(fp);
		if(PlatformID!=3||SpecificID!=1) {
			continue;
		}
		if(LanguageID!=LANGUAGE1&&LanguageID!=LANGUAGE2) {
			continue;
		}
		if(NameID==nid) {												// <指定NameIDの場合>
			old_offset = ftell(fp);
			fseek(fp, stringStorageOffset+StringOffset, SEEK_SET);
			memset(fname, NULL, 1024);
			for(j=0, k=0; j<StringLength/2; j++) {
				if(fread(&buf[0], 1, 1, fp)!=1) {
					AfxMessageBox(IDS_ERR_TTF1);						// TTF入力ｴﾗｰ
					m_TtfErr = 1; break;
				}
				if(fread(&buf[1], 1, 1, fp)!=1) {
					AfxMessageBox(IDS_ERR_TTF1);						// TTF入力ｴﾗｰ
					m_TtfErr = 1; break;
				}
				pmb[0] = 0x0;
				pmb[1] = 0x0;
				pwc = (buf[0]<<8)|buf[1];
				wctomb_s(&l, pmb, 2, pwc);
				if(pmb[0]!=0x0) {
					fname[k++] = pmb[0];
				}
				if(pmb[1]!=0x0) {
					fname[k++] = pmb[1];
				}
			}
			if(LanguageID==LANGUAGE1) {									// 第一優先言語ありは終了
				break;
			}
			fseek(fp, old_offset, SEEK_SET);
		}
	}
}

/********************************************/
void GetFontFileName(char* file, char* name)							//<<< Fontﾌｧｲﾙ名取得
/********************************************/
{
	char*               fname = new char[1024];
	FILE*               fp;
	sfnt_TTC_Header     TTC_Header;
	sfnt_OffsetTable    OffsetTable;
	sfnt_TableDirectory	Table;
	unsigned            i;
	long                name_table_offset = 0;

	m_TtfErr = 0;
	if(fopen_s(&fp, file, "rb")!=0) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTFOpenｴﾗｰ
		m_TtfErr = 1;
	}
	if(fread(&TTC_Header.Tag, sizeof(BYTE), 4, fp) != 4) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	if((TTC_Header.Tag[0]=='t')&&(TTC_Header.Tag[1]=='t')&&
		(TTC_Header.Tag[2]=='c')&&(TTC_Header.Tag[3]=='f')) {			// <TTCの場合>
		TTC_Header.version = ReadLong(fp);
		TTC_Header.DirectoryCount = ReadULong(fp);
		TTC_Header.TableDirectory = ReadULong(fp);
		fseek(fp, TTC_Header.TableDirectory, SEEK_SET);
	} else {															// <TTFの場合>
		fseek(fp, 0, SEEK_SET);
	}
	OffsetTable.version = ReadLong(fp);
	OffsetTable.numTables = ReadUShort(fp);
	OffsetTable.searchRange = ReadUShort(fp);
	OffsetTable.entrySelector = ReadUShort(fp);
	OffsetTable.rangeShift = ReadUShort(fp);
	if(OffsetTable.numTables>40) {
		AfxMessageBox(IDS_ERR_TTF1);									// TTF入力ｴﾗｰ
		m_TtfErr = 1;
	}
	for(i=0; i<OffsetTable.numTables&&i<40; i++) {
		if(fread(&Table.tag, sizeof(BYTE), 4, fp)!=4) {
			AfxMessageBox(IDS_ERR_TTF1);								// TTF入力ｴﾗｰ
			m_TtfErr = 1;
		}
		Table.checkSum = ReadULong(fp);
		Table.offset   = ReadULong(fp);
		Table.length   = ReadULong(fp);
		if(CompareTag(Table.tag, tag_FontName)) {
			name_table_offset = Table.offset;
		}
	}
	memset(fname, NULL, 1024);
	if(m_TtfErr) {
		delete[] fname;
		return;
	}
	ProcessNameTable(fp, name_table_offset, 4, fname);					// Font Name Table設定
	if(fp!=NULL) {														// TTFﾌｧｲﾙClose
		fclose(fp);
	}
	strcpy_s(name, 1024, fname);
	delete[] fname;
	return;
}
