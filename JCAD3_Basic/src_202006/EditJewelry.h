/////////////////////////////////////////////////////////////////////////////
// •óÎŠÖŒW•ÒW•”ƒvƒƒgƒ^ƒCƒvéŒ¾
// EditJewelry.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
BOOL KeyboardJwlRing(int, int, double*, double*);						// »²½Ş,Ú‡ŠÔŠu“ü—Í
BOOL KeyboardStoneForm(int*);											// Î˜gŒ`ó“ü—Í
BOOL KeyboardRoundSize(double*);										// Î˜g’¼Œa“ü—Í
BOOL KeyboardSquareSize(double*, double*);								// Î˜g’¼Œa“ü—Í
BOOL KeyboardProng(int*, double*, double*, double*);					// ’Üî•ñ“ü—Í
BOOL KeyboardArrange(double*, int*);									// ”z’uî•ñ“ü—Í
void TrackerStone(int, PNTTYPE, double, double, double, double,
                  int*, PNTTYPE*, int*, int*);							// Î˜gÄ×¯¶°ì¬
void RoundStone(PNTTYPE, double, double, double, int, int,
                PNTTYPE*, int*, PNTTYPE*, int*);						// ×³İÄŞÎ˜g¶¬
void SquareStone(PNTTYPE, double, double, double, double, double,
                 int, PNTTYPE*, int*, PNTTYPE*, int*);					// ½¸´±Î˜g¶¬
void SetProng(int, double, double, double, BOXTYPE, int, PNTTYPE*);		// Î˜g’Üİ’è

//=============================<EOF:EditJewelry.h>===========================