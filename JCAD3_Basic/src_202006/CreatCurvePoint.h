/////////////////////////////////////////////////////////////////////////////
// ÈüÒWìvg^Cvé¾
// CreatCurvePoint.h
//---------------------------------------------------------------------------
// LastEdit : 2010/01/18 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void InsertCurvePoint(int*, int*, PNTTYPE*, int*);						// SÈüÔ§ä_}ü
void CalcRPoint(int, int, int, double, int*, PNTTYPE*, int*);			// R¸_ÊuZo
void CalcSpiralPoint(PNTTYPE, int, double, double, int, int,
                     PNTTYPE*, int*);									// ù§ä_vZ
void TrackerECurve(int, double*, int, int, PNTTYPE*, int*, int*,
                   int*, PNTTYPE*, int*);								// ÈüÒWÄ×¯¶°æ¾
void DeleteCurvePoint(int, double*, double*, int, double, int*, int,
                      PNTTYPE*, int*, int*);							// §ä_¥Êß_í
void PickECurve(int, int*, int, double*, double*, int*, int, int,
                double, int*, OBJTYPE*, int, PNTTYPE*,
                PNTTYPE*, int*, int*, int*);							// ÈüÒW _Ið
void MoveECurve(int, double*, PNTTYPE*, OBJTYPE*, int, int,
                PNTTYPE*, PNTTYPE*, int*, PNTTYPE*, int*);				// §ä_¥Êß_Ú®
BOOL ExtractionCurve(int, int, int, int, int, int, int,
                     PNTTYPE*, int*, int*, int*, PNTTYPE*, int*);		// Èüo

//==========================<EOF:CreatCurvePoint.h>==========================