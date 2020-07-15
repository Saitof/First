///////////////////////////////////////////////////////////////////////////////
// JCAD3共通ファイルアクセスプロトタイプ宣言
// ICFAC32_Api.h
//-----------------------------------------------------------------------------
// LastEdit: 2009/11/03 by M.Fukushima
///////////////////////////////////////////////////////////////////////////////
#ifndef ___CFAC_H
#define ___CFAC_H

int WINAPI InpDXFParam(int* ver, int* out, int* fac);					// DXFﾊﾟﾗﾒｰﾀ入力
int WINAPI InpSTLParam(int* typ, int* fac);								// STLﾊﾟﾗﾒｰﾀ入力
int WINAPI ChgWDXFFile(const char* fname, const char* tname,
                       int ver, int out);								// DXFﾌｧｲﾙ変換(Write)
int WINAPI ChgWSTLFile(const char* fname, const char* tname,
                       int typ);										// STLﾌｧｲﾙ変換(Write)
int WINAPI ChgRDXFFile(const char* fname, const char* tname);			// DXFﾌｧｲﾙ変換(Read)
int WINAPI ChgRSTLFile(const char* fname, const char* tname);			// STLﾌｧｲﾙ変換(Read)

#endif ___CFAC_H

//============================<EOF:CFAC32_Api.h>=============================