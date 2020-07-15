/////////////////////////////////////////////////////////////////////////////
// 立体属性設定コマンド部プロトタイプ宣言
// Attribute.h
//---------------------------------------------------------------------------
// LastEdit : 2009/11/30 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
void WriteAttributeFile(FILE*, OBJTYPE*);								// 属性Noﾌｧｲﾙ保存
void WriteAttributeFileJgd(FILE*, OBJTYPE*, int);						// 属性Noﾌｧｲﾙ保存(JGD用)
void WriteColorFile(FILE*, OBJTYPE*);									// ｶﾗｰNoﾌｧｲﾙ保存
void WriteColorFileJgd(FILE*, OBJTYPE*, int);							// ｶﾗｰNoﾌｧｲﾙ保存(JGD用)
void WriteGroupFile(FILE*, OBJTYPE*);									// ｸﾞﾙｰﾌﾟ情報ﾌｧｲﾙ保存
void WriteGroupFileJgd(FILE*, OBJTYPE*, int);							// ｸﾞﾙｰﾌﾟ情報ﾌｧｲﾙ保存(JGD用)
void WritePartsFile(FILE*, OBJTYPE*);									// ﾊﾟｰﾂNoﾌｧｲﾙ保存
void WritePartsFileJgd(FILE*, OBJTYPE*, int);							// ﾊﾟｰﾂNoﾌｧｲﾙ保存(JGD用)

//=============================<EOF:Attribute.h>=============================