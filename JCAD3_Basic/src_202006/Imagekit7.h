/*******************************************************************************
 *    ImageKit7    Ver. 1.2.0                                                  *
 *                                                                             *
 *    for C/C++    DLLで使用                                                   *
 *                                                                             *
 *    <ImageKit7.h>                                          (株)ニュートン    *
 *******************************************************************************/
#ifndef IMAGEKIT7_INCLUDE
#define IMAGEKIT7_INCLUDE

// GDI+用
#ifndef ULONG_PTR
#define ULONG_PTR ULONG
#endif

#ifndef HINTERNET
typedef LPVOID HINTERNET;
#endif

#pragma pack (push, 8)
/*******************************************************************************
 *                           イメージ情報用構造体                              *
 *******************************************************************************/
typedef struct {
	short	BitCount;					// ピクセル／ビット
	short	PalCount;					// 使用パレット数
	short	Mask1632;					// １６、３２ビットカラーの時 1-マスクデータあり(RGB555フォーマット)
										//                            2-マスクデータあり(RGB565フォーマット)
										//                            3-マスクデータなし(RGB555フォーマット)
										//                            4-マスクデータあり(RGB888フォーマット)
										//                            5-マスクデータなし(RGB888フォーマット)
	long	Xdpi;						// X Dot/Inch
	long	Ydpi;						// Y Dot/Inch
	long	Width;						// 幅のピクセル数
	long	Height;						// 縦のピクセル数
	long	WidthByte;					// 幅のバイト数
	BOOL	Gray;						// TRUE:グレー  FALSE:グレーでない
	long	ImageSize;					// イメージサイズ
	short	ImageType;					// 0:不明 1:ラスタイメージ 2:WMF 3:EMF 4:DXF 5:SVG 6:SXF
} IKIMAGE_INFO;
typedef IKIMAGE_INFO * PTR_IKIMAGE_INFO;

/*******************************************************************************
 *                       イメージファイル情報用構造体                          *
 *******************************************************************************/
typedef struct {
	short	BitCount;					// ピクセル／ビット
	long	Xdpi;						// X Dot/Inch
	long	Ydpi;						// Y Dot/Inch
	long	Width;						// 幅のピクセル数
	long	Height;						// 縦のピクセル数
	long	WidthByte;					// 幅のバイト数
	long	ImageSize;					// イメージサイズ
	short	FileType;					// ファイルタイプ
	short	Interlace;					// インタレース  ０：なし  １：あり
	short	MaxPage;					// イメージページ数
	long	FileSize;					// ファイルサイズ
	char	Comment[2048];				// コメント
	WORD	CommentLen;					// コメントの長さ
	short	CreationTimeYear;			// 作成日時			年
	short	CreationTimeMonth;			//					月
	short	CreationTimeDay;			//					日
	short	CreationTimeHour;			//					時
	short	CreationTimeMinute;			//					分
	short	CreationTimeSecond;			//					秒
	short	LastAccessTimeYear;			// 最終アクセス日時 年
	short	LastAccessTimeMonth;		//					月
	short	LastAccessTimeDay;			//					日
	short	LastAccessTimeHour;			//					時
	short	LastAccessTimeMinute;		//					分
	short	LastAccessTimeSecond;		//					秒
	short	LastWriteTimeYear;			// 最終更新日時		年
	short	LastWriteTimeMonth;			//					月
	short	LastWriteTimeDay;			//					日
	short	LastWriteTimeHour;			//					時
	short	LastWriteTimeMinute;		//					分
	short	LastWriteTimeSecond;		//					秒
} IKFILE_INFO;
typedef IKFILE_INFO * PTR_IKFILE_INFO;

/*******************************************************************************
 *                            スキャナ制御用構造体                             *
 *******************************************************************************/
typedef struct {
	WORD	ProtocolMajor;				// TWAINのメジャーバージョン
	WORD	ProtocolMinor;				// TWAINのマイナーバージョン
	char	Manufacturer[34];			// メーカ名
	WORD	ManufacturerLen;			// メーカ名の長さ
	char	ProductFamily[34];			// 製品ファミリー名
	WORD	ProductFamilyLen;			// 製品ファミリー名の長さ
	char	ProductName[34];			// 製品ファミリー名
	WORD	ProductNameLen;				// 製品ファミリー名の長さ
	WORD	SourceMajor;				// データソースのメジャーバージョン
	WORD	SourceMinor;				// データソースのマイナーバージョン
	char	SourceVersionInfo[34];		// データソースのバージョン情報
	WORD	SourceVersionInfoLen;		// データソースのバージョン情報の長さ
} IKSCAN_DATASOURCEINFO;
typedef IKSCAN_DATASOURCEINFO * PTR_IKSCAN_DATASOURCEINFO;

typedef struct {
	WORD	seSize;						// IKSCAN_EXEC構造体のサイズ
	WORD	PixelType;					// 0:BW 1:GRAY 2:RGB_COLOR 3:PALETTE_COLOR
	float	Left;						// 左の位置(0はデフォルト値を使用) 
	float	Top;						// 上の位置
	float	Right;						// 右の位置
	float	Bottom;						// 下の位置
	long	XResolution;				// X方向のDPI(設定するときの単位はインチ)
	long	YResolution;				// Y方向のDPI(設定するときの単位はインチ)
	float	XScaling;					// X方向のスケール値
	float	YScaling;					// Y方向のスケール値
	WORD	BitDepth;					// 画素ビット数
	short	Brightness;					// ブライトネス
	short	Contrast;					// コントラスト(彩度）
	float	Gamma;						// ガンマ
	short	Highlight;					// ハイライト
	short	Threshold;					// スレッシュホールド（しきい値）
	short	Shadow;						// シャドウ
	short	Indicator;					// インジケーターの表示(0以外表示)
	short	ScanMode;					// 取り込みモード(スキャナ 0:原稿台,1:ADF片面,2:ADF両面,3-6:デジカメ)
	LPSTR	ScanDsName;					// スキャンデバイスの名称
	WORD	UiMode;						// 0:UI表示－閉じない 1:UI表示－閉じる 2:UI非表示
	WORD	UnitMode;					// 読み取り単位 0:インチ 1:cm  2:パイカ 3:ポイント 4:twip 5:ピクセル
	WORD	UnitFlag;					// 0:データソース 1:ImageKit側で制御(UnitMode = 1,2,3,4,5が対象)
	short	PageCount;					// 取り込み枚数(ADF及びデジカメの時に有効)
	WORD	Orientation;				// 向き 0~3
	WORD	TransferMode;				// 転送モード 0:ﾈｲﾃｨﾌﾞ, 2:ﾒﾓﾘｰ
	WORD	Compression;				// 圧縮方法 TransferMode = 2 の場合に有効
	WORD	PaperSize;					// 用紙サイズ >= 0 (0の場合はLeft,Top,Right,Bottomが有効)
	WORD	BitDepthReduction;			// 色深度
	LPSTR	HalfTone;					// ハーフトーン
	WORD	FileFormat;					// ファイルフォーマット
	short	JpegQuality;				// JPEG品質係数
	LPSTR	FileName;					// ファイル名
	WORD	ImageFilter;				// イメージフィルター
	WORD	NoiseFilter;				// ノイズフィルター
	WORD	DropoutColor;				// ドロップアウトカラー
	WORD	BorderDetection;			// 領域切り出し
	WORD	Deskew;						// 傾き補正
	WORD	ScanningSpeed;				// 読み取り速度
	WORD	MoireFilter;				// モアレ除去
	WORD	Sharpness;					// シャープネス
	WORD	RotateBack;					// ADFの両面スキャン時の裏面回転
	WORD	ExtUiMode;					// 拡張UIモード
	WORD	DynamicThreshold;			//ダイナミックスレッシュホールド
	float	ColorBWRatio;				// カラーと白黒の割合 (0.01～50.00）
	WORD	IgnoreBackColor;			// 0:背景色を無視しない、1:背景色を無視
} IKSCAN_EXEC;
typedef IKSCAN_EXEC * PTR_IKSCAN_EXEC;

typedef struct {
	float	Min;						// 最小値
	float	Max;						// 最大値
	float	Step;						// ステップ値
	float	Default;					// デフォルト値
	float	Current;					// 現在の設定値
} IKSCAN_RANGE;
typedef IKSCAN_RANGE * PTR_IKSCAN_RANGE;

typedef struct {
	float	XResolution;				// X方向の解像度
	float	YResolution;				// Y方向の解像度
	long	Width;						// イメージの幅
	long	Height;						// イメージの高さ
	short	BitDepth;					// 画素ビット数
	short	PixelType;					// ピクセルタイプ
	WORD	Compression;				// 圧縮方法
} IKSCAN_IMAGEINFO;
typedef IKSCAN_IMAGEINFO * PTR_IKSCAN_IMAGEINFO;

/*******************************************************************************
 *                             プリント用構造体                                *
 *******************************************************************************/
typedef struct {
	short		CharSet;				// 文字セット
	char		FontName[LF_FACESIZE];	// フォント名称 32バイト
	WORD		FontSize;				// フォントサイズ（ポイント）
	BYTE		FontBold;				// 0以外のとき、ボールド体のフォント
	BYTE		FontItalic;				// 0以外のとき、イタリック体のフォント
	BYTE		FontUnderline;			// 0以外のとき、下線付きのフォント<DirectionやCharAngleの値により無効の場合あり>
	BYTE		FontStrikeOut;			// 0以外のとき、打ち消し線付きのフォント<DirectionやCharAngleの値により無効の場合あり>
	BYTE		Transparent;			// 0以外のとき、透過（文字）
	BYTE		Direction;				// 文字列の方向 0:左から右 1:右から左 2:上から下 3:下から上
	short		CharAngle;				// 文字の回転角度 1度単位(反時計周り) 0--360
	long		CharExtra;				// 文字間
	BYTE		HCentering;				// 水平センタリング 0:左 1:中央 2:右 <IKDrawTextで有効>
	BYTE		VCentering;				// 垂直センタリング 0:上 1:中央 2:下 <IKDrawTextで有効>
	BYTE		RotateString;			// 0:文字の回転,0以外:文字列の回転 <IKTextOut>
	COLORREF	TextColor1;				// テキスト色1 (IK6ではTextForeColor)
	COLORREF	TextColor2;				// テキスト色2 (IK6ではTextBackColor)
	BYTE		Alpha1;					// TextColor1のアルファ IKDrawStringで有効
	BYTE		Alpha2;					// TextColor2のアルファ IKDrawStringで有効
	WORD		HotkeyPrefix;			// IKDrawText 0:プリフィックスなし,1:プリフィックス表示
										// IKDrawString 0:プリフィックスなし, 1:プリフィックス表示, 2:プリフィックス非表示
} IKPRINT_TEXTINFO;
typedef IKPRINT_TEXTINFO * PTR_IKPRINT_TEXTINFO;

typedef struct {
	long		PenWidth;				// ペンの幅 (ピクセル or  0.1mm単位) <単位がmmの場合はMax 1cm>
	BYTE		PenStyle;				// 0:ヌル 1:実線  2..6: 
	BYTE		PenMode;				// ペンのモード 1..16:
	COLORREF	PenColor;				// ペンの描画色
	BYTE		BrushStyle;				// 0:ヌル 1:SOLID 2..7:HATCH
	COLORREF	BrushColor;				// ブラシの描画色
	BYTE		Transparent;			// 0以外のとき、透過（パターンブラシの場合）
	COLORREF	BackColor;				// Transparent = 0, パターンブラシの場合に有効
} IKPRINT_DRAWINFO;
typedef IKPRINT_DRAWINFO * PTR_IKPRINT_DRAWINFO;

typedef struct {
	short		Orientation;			// 印刷の向き 1:縦(DMORIENT_PORTRAIT) 2:横(DMORIENT_LANDSCAPE)
	short		PaperSize;				// 用紙サイズ 8:A3(DMPAPER_A3) 9:A4(DMPAPER_A4)
	short		Zoom;					// 縮小する時の百分率(0~100)
	WORD		Copies;					// 印刷部数
	short		PaperBin;				// 用紙トレイ
	short		XResolution;			// プリンタのX方向の解像度
	short		YResolution;			// プリンタのY方向の解像度
	short		ColorMode;				// 1:モノクロ,2:カラー
	short		Duplex;					// 1~3
	BYTE		Collate;				// ［部単位で印刷］0以外:部単位で印刷
	short		CustomPaperWidth;		// PaperSize=256(ユーザ定義サイズ)の時の用紙の横幅(0.1mm単位)
	short		CustomPaperHeight;		// PaperSize=256(ユーザ定義サイズ)の時の用紙の縦幅(0.1mm単位)
} IKPRINT_DEVMODEINFO;
typedef IKPRINT_DEVMODEINFO * PTR_IKPRINT_DEVMODEINFO;

typedef struct {
	BYTE		Collate;				// ［部単位で印刷］0以外:部単位で印刷にチェック
	WORD		Copies;					// 印刷部数
	WORD		FromPage;				// 印刷開始ページ
	WORD		ToPage;					// 印刷終了ページ
	WORD		MaxPage;				// 印刷最大ページ
	WORD		MinPage;				// 印刷最小ページ
	DWORD		Options;				// ［印刷］ダイアログの表示と動作の指定
	BYTE		PrintRange;				// 印刷範囲の種類 0:すべて,1:選択した部分,2:ページ指定
	BYTE		PrintToFile;			// ［ファイルへ出力］0以外:ファイルへ出力にチェック
} IKPRINT_DIALOG;
typedef IKPRINT_DIALOG * PTR_IKPRINT_DIALOG;

/*******************************************************************************
 *                           エフェクト処理用構造体                            *
 *******************************************************************************/
typedef struct {
	HANDLE		hImgBmh;				// イメージハンドル
	HANDLE		hMskBmh;				// マスクイメージハンドル
} IKSELECT_IMAGE;
typedef IKSELECT_IMAGE * PTR_IKSELECT_IMAGE;

typedef struct {
	BYTE	Pal;						// 設定・取得されるパレット番号
	BYTE	Red;						// 設定・取得される赤色
	BYTE	Green;						// 設定・取得される緑色
	BYTE	Blue;						// 設定・取得される青色
	char	Reserved[128];				// ＤＩＢアクセス情報
} IKDIB_INFO;
typedef IKDIB_INFO * PTR_IKDIB_INFO;

/*******************************************************************************
 *                               Exif情報用構造体                              *
 *******************************************************************************/
// 主画像、サムネイル画像 情報
typedef struct {
	unsigned short	Compression;						// 圧縮の種類（主画像でJPEGの場合はセットされない）
	unsigned short	Orientation;						// 画像方向
	unsigned short	YCbCrPositioning;					// YCC の画素構成（Y とC の位置）
	unsigned long	XResolution[2];						// 画像の幅の解像度
	unsigned long	YResolution[2];						// 画像の高さの解像度
	unsigned short	ResolutionUnit;						// 画像の幅と高さの解像度の単位
	char			DateTime[20];						// ファイル変更日時
	char			ImageDescription[1024];				// 画像タイトル
	WORD			ImageDescriptionLen;				// 画像タイトルの長さ
	char			Make[1024];							// 画像入力機器のメーカー名
	WORD			MakeLen;							// 画像入力機器のメーカー名の長さ
	char			Model[1024];						// 画像入力機器のモデル名
	WORD			ModelLen;							// 画像入力機器のモデル名の長さ
	char			Software[1024];						// 使用ソフトウェア名
	WORD			SoftwareLen;						// 使用ソフトウェア名の長さ
	char			Artist[1024];						// 作者名
	WORD			ArtistLen;							// 作者名の長さ
	char			Copyright[1024];					// 撮影著作権者/編集著作権者
	WORD			CopyrightLen;						// 撮影著作権者/編集著作権者の長さ
	HANDLE			ThumbImageHandle;					// サムネイル画像のハンドル（サムネイルのみ）
} IMAGE_TAG;
typedef IMAGE_TAG * PTR_IMAGE_TAG;

// Exif 情報
typedef struct {
	// A バージョンに関するタグ
	char			Exif_Version[5];					// Exif バージョン（例："0220"）
	char			FlashPix_Version[5];				// FlashPix バージョン（例："0100"）
	// B 画像データの特性に関するタグ
	unsigned short	ColorSpace;							// 色空間情報
	// C 画像データの構成に関するタグ
	unsigned long	PixelXDimension;					// 実効画像幅
	unsigned long	PixelYDimension;					// 実効画像高さ
	char			ComponentsConfiguration[5];			// 各コンポーネントの意味
	unsigned long	CompressedBitsPerPixel[2];			// 画像圧縮モード
	// D ユーザ情報に関するタグ
	char			MakerNote[1024];					// メーカーノート
	WORD			MakerNoteLen;						// メーカーノートの長さ
	char			UserComment[1024];					// ユーザーコメント
	WORD			UserCommentLen;						// ユーザーコメントの長さ
	char			UserCommentID[8];					// ユーザーコメント識別コード
	// F 日付に関するタグ
	char			DateTimeOriginal[20];				// 原画像データの生成日時
	char			DateTimeDigitized[20];				// ディジタルデータの作成日時
	char			SubSecTime[10];						// DateTime のサブセック
	char			SubSecTimeOriginal[10];				// DateTimeOriginal のサブセック
	char			SubSecTimeDigitized[10];			// DateTimeDigitized のサブセック
	// G 撮影条件に関するタグ
	unsigned long	ExposureTime[2];					// 露出時間
	long			ShutterSpeedValue[2];				// シャッタースピード
	unsigned long	ApertureValue[2];					// 絞り値
	long			BrightnessValue[2];					// 輝度値
	long			ExposureBiasValue[2];				// 露光補正値
	unsigned long	MaxApertureValue[2];				// レンズ最小Ｆ値
	unsigned long	SubjectDistance[2];					// 被写体距離
	unsigned short	MeteringMode;						// 測光方式
	unsigned short	LightSource;						// 光源
	unsigned short	Flash;								// フラッシュ
	unsigned long	FocalLength[2];						// レンズ焦点距離
	unsigned long	FNumber[2];							// F ナンバー
	unsigned short	ExposureProgram;					// 露出プログラム
	char			SpectralSensitivity[1024];			// スペクトル感度
	WORD			SpectralSensitivityLen;				// スペクトル感度の長さ
	unsigned short	ISOSpeedRatings;					// ISO スピードレート
	unsigned long	FlashEnergy[2];						// フラッシュ強度
	unsigned long	FocalPlaneXResolution[2];			// 焦点面の幅の解像度
	unsigned long	FocalPlaneYResolution[2];			// 焦点面の高さの解像度
	unsigned short	FocalPlaneResolutionUnit;			// 焦点面解像度単位
	unsigned short	SubjectLocationX;					// 被写体位置（X座標値）
	unsigned short	SubjectLocationY;					// 被写体位置（Y座標値）
	unsigned long	ExposureIndex[2];					// 露出インデックス
	unsigned short	SensingMethod;						// センサー方式
	BYTE			FileSource;							// ファイルソース
	BYTE			SceneType;							// シーンタイプ
} EXIF_TAG;
typedef EXIF_TAG * PTR_EXIF_TAG;

// GPS 情報
typedef struct {
	char			GPSVersion[5];						// GPS タグバージョン（例："2000"）
	char			MapDatum[16];						// 測位の用いた地図データ
	char			Latitude[2];						// "N":北緯, 'S":南緯
	unsigned long	LatitudeD[2];						// 度
	unsigned long	LatitudeM[2];						// 分
	unsigned long	LatitudeS[2];						// 秒
	char			Longitude[2];						// "E":東経, "W":西経
	unsigned long	LongitudeD[2];						// 度
	unsigned long	LongitudeM[2];						// 分
	unsigned long	LongitudeS[2];						// 秒
	unsigned long	Altitude[2];						// 高度（ｍ）
} GPS_TAG;
typedef GPS_TAG * PTR_GPS_TAG;

typedef struct {
	IMAGE_TAG		MainImageInfo;						// 主画像情報
	IMAGE_TAG		ThumbImageInfo;						// サムネイル画像情報
	EXIF_TAG		ExifInfo;							// Exif情報
	GPS_TAG			GPSInfo;							// GPS情報
} EXIF_INFO;
typedef EXIF_INFO * PTR_EXIF_INFO;

#pragma pack (pop)

/*******************************************************************************
 *                               コールバック関数                              *
 *******************************************************************************/
// ファイルの読み書きやエフェクト処理中に呼び出されるユーザ関数の書式
typedef BOOL (FAR PASCAL *IKPROCESSPROC)(short Percent);

// 文書の読み込み中に呼び出されるコールバック関数の書式
typedef BOOL (FAR PASCAL *IK_BEFORESCANPROC)(PTR_IKSCAN_IMAGEINFO ImageInfo, long Count);
typedef BOOL (FAR PASCAL *IK_SCANNINGPROC)(short Percent);
typedef BOOL (FAR PASCAL *IK_AFTERSCANPROC)(HANDLE DibHandle, HANDLE OrgHandle, long Count, short BitOrder);

/*******************************************************************************
 *                                 関数の宣言                                  *
 *******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Ik7Com.DLL
HBITMAP APIENTRY IKBitmapFromDib(HANDLE hDib);
void    APIENTRY IKClearClipBrd(HWND hWnd);
HANDLE  APIENTRY IKCopyImage(HANDLE Handle);
BOOL    APIENTRY IKBitBlt(HDC hDCDst, long XDst, long YDst, long Width, long Height, HDC hDCSrc, long XSrc, long YSrc, DWORD dwRop);
BOOL    APIENTRY IKStretchBlt(HDC hDCDst, long XDst, long YDst, long WidthDst, long HeightDst, HDC hDCSrc, long XSrc, long YSrc, long WidthSrc, long HeightSrc, DWORD dwRop);
HANDLE  APIENTRY IKCreateImage(long Width, long Height, short BitCount, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKDeleteBitmapObject(HBITMAP hBitmap);
HANDLE  APIENTRY IKDibFromBitmap(HBITMAP hBm, HPALETTE hPal);
BOOL    APIENTRY IKFreeMemory(HANDLE Handle);
BOOL    APIENTRY IKGetDpi(HANDLE Handle, long *Xdpi, long *Ydpi);
BOOL    APIENTRY IKGetDpiFromHdc(HDC hDC, long *Xdpi, long *Ydpi);
int     APIENTRY IKGetErrorStatus(void);
HANDLE  APIENTRY IKGetFromClipBrd(HWND hWnd, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKGetImageType(HANDLE Handle, PTR_IKIMAGE_INFO ImageInfo);
DWORD   APIENTRY IKGetMemorySize(HANDLE Handle);
BOOL    APIENTRY IKGetOneBitPalCount(HANDLE Handle, long *Pal0, long *Pal1);
BOOL    APIENTRY IKGetPalette(HANDLE Handle, short *Red, short *Green, short *Blue);
int     APIENTRY IKGetSystemPalette(void);
BOOL    APIENTRY IKIsClipBrdData(HWND hWnd);
HANDLE  APIENTRY IKResizeRefine1BitImage(HANDLE Handle, long Width, long Height);
BOOL    APIENTRY IKSetDpi(HANDLE Handle, long Xdpi, long Ydpi);
BOOL    APIENTRY IKSetPalette(HANDLE Handle, short *Red, short *Green, short *Blue);
BOOL    APIENTRY IKSetToClipBrd(HWND hWnd, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKGetDpiF(HANDLE Handle, float *Xdpi, float *Ydpi);
BOOL    APIENTRY IKSetDpiF(HANDLE Handle, float Xdpi, float Ydpi);

// Ik7VectCom.DLL
HANDLE  APIENTRY IKCreateVectImage(long Width, long Height, long Xdpi, long Ydpi);
HANDLE  APIENTRY IKCreateVectImageEx(long Type, long Width, long Height, long Xdpi, long Ydpi);
BOOL    APIENTRY IKDrawVectObject(HDC hDC, HANDLE Mbh, long DstOrgX, long DstOrgY, long SrcOrgX, long SrcOrgY, double XScale, double YScale, BOOL Black);
HANDLE  APIENTRY IKVectorToRaster(HANDLE Mbh, short BitCount, BOOL Black, BYTE BackRed, BYTE BackGre, BYTE BackBlu);
void    APIENTRY IKVectorGdipEnd(ULONG_PTR token);
ULONG_PTR APIENTRY IKVectorGdipStart(void);

// Ik7File.DLL
HANDLE  APIENTRY IKFileLoadAsRawData(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKFileSaveAsRawData(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKFileLoad(LPCSTR FileName, int Page, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKFileType(LPCSTR FileName, int Page, PTR_IKFILE_INFO FileType);
BOOL    APIENTRY IKOpenFileDialog(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL ExtendedDialog, BOOL Preview, BOOL FileInfo);
BOOL    APIENTRY IKOpenFileDlg(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL Preview);
BOOL    APIENTRY IKSaveFileDialog(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL ExtendedDialog, BOOL Preview, LPINT FileType, BOOL FileInfo);
BOOL    APIENTRY IKSaveFileDlg(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL Preview);
BOOL    APIENTRY IKSaveFileDlgEx(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL Preview, LPINT FileType);
BOOL    APIENTRY IKFTPGetFile(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKFTPPutFile(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKFTPDeleteFile(LPCTSTR ServerName, LPCTSTR DeleteFile, LPCTSTR UserName, LPCTSTR Password);
BOOL    APIENTRY IKFTPRenameFile(LPCTSTR ServerName, LPCTSTR OldFile, LPCTSTR NewFile, LPCTSTR UserName, LPCTSTR Password);
BOOL    APIENTRY IKHTTPGetFile(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKHTTPPutFile(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKBmpFileLoad(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKBmpFileSave(LPCSTR FileName, HANDLE Handle, BOOL Comp, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKJpegFileLoad(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKJpegFileSave(LPCSTR FileName, HANDLE Handle, int Type_, int Quality, int SubSamp, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKJpegExifInfo(LPCSTR FileName, PTR_EXIF_INFO ExifInfo);
HANDLE  APIENTRY IKGifFileLoad(LPCSTR FileName, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKGifFileSave(LPCSTR FileName, HANDLE Handle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, BOOL Anime, int Delay, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKTiffFileLoad(LPCSTR FileName, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKTiffFileSave(LPCSTR FileName, HANDLE Handle, int Type_, int Color, BOOL Append, LPCSTR Comment, BOOL OneStrip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKPngFileLoad(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKPngFileLoadEx(LPCSTR FileName, int Alpha, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKPngFileSave(LPCSTR FileName, HANDLE Handle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKPngFileSaveEx(LPCSTR FileName, HANDLE Handle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, BOOL Alpha, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKFpxFileLoad(LPCSTR FileName, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKFpxFileSave(LPCSTR FileName, HANDLE Handle, int Comp, int Quality, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKPcxFileLoad(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKPcxFileSave(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKWmfFileLoad(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKWmfFileSave(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKEmfFileLoad(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKEmfFileSave(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKDxfFileLoad(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKDxfFileSave(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKSvgFileLoad(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKSvgFileSave(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKJ2kFileLoad(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKJ2kFileSave(LPCSTR FileName, HANDLE Handle, BOOL JP2kFormat, BOOL Reversible, double Size, long TileWidth, long TileHeight, short NumrResLevel, short PrecinctWidth, short PrecinctHeight, short CodeBlockWidth, short CodeBlockHeight, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKSxfP21FileLoad(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKSxfP21FileSave(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKSxfSfcFileLoad(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKSxfSfcFileSave(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKRGBBmpPlaneFileLoad(LPCSTR RedFileName, LPCSTR GreenFileName, LPCSTR BlueFileName);
BOOL    APIENTRY IKRGBBmpPlaneFileSave(LPCSTR RedFileName, LPCSTR GreenFileName, LPCSTR BlueFileName, HANDLE Handle);
HANDLE  APIENTRY IKCMYKBmpPlaneFileLoad(LPCSTR CFileName, LPCSTR MFileName, LPCSTR YFileName, LPCSTR KFileName);
BOOL    APIENTRY IKCMYKBmpPlaneFileSave(LPCSTR CFileName, LPCSTR MFileName, LPCSTR YFileName, LPCSTR KFileName, HANDLE Handle);
HANDLE  APIENTRY IKYCCBmpPlaneFileLoad(LPCSTR YFileName, LPCSTR CrFileName, LPCSTR CbFileName);
BOOL    APIENTRY IKYCCBmpPlaneFileSave(LPCSTR YFileName, LPCSTR CrFileName, LPCSTR CbFileName, HANDLE Handle);

HANDLE  APIENTRY IKFileLoadMem(HANDLE Handle, int Page, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKFileTypeMem(HANDLE Handle, int Page, PTR_IKFILE_INFO FileType);
HANDLE  APIENTRY IKBmpFileLoadMem(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKBmpFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, BOOL Comp, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKJpegFileLoadMem(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKJpegFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, int Type, int Quality, int SubSamp, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKJpegExifInfoMem(HANDLE Handle, PTR_EXIF_INFO ExifInfo);
HANDLE  APIENTRY IKGifFileLoadMem(HANDLE Handle, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKGifFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, BOOL Interlace, BOOL Trans, BYTE TRed, BYTE TGreen, BYTE TBlue, BOOL Anime, int Delay, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKTiffFileLoadMem(HANDLE Handle, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKTiffFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, int Type_, int Color, BOOL Append, LPCSTR Comment, BOOL OneStrip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKPngFileLoadMem(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKPngFileLoadExMem(HANDLE Handle, int Alpha, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKPngFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKPngFileSaveExMem(HANDLE InHandle, HANDLE *OutHandle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, BOOL Alpha, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKFpxFileLoadMem(HANDLE Handle, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKFpxFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, int Comp, int Quality, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKPcxFileLoadMem(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKPcxFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKWmfFileLoadMem(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKWmfFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKEmfFileLoadMem(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKEmfFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKDxfFileLoadMem(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKDxfFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKSvgFileLoadMem(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKSvgFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKJ2kFileLoadMem(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKJ2kFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, BOOL JP2kFormat, BOOL Reversible, double Size, long TileWidth, long TileHeight, short NumrResLevel, short PrecinctWidth, short PrecinctHeight, short CodeBlockWidth, short CodeBlockHeight, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKSxfP21FileLoadMem(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKSxfP21FileSaveMem(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE  APIENTRY IKSxfSfcFileLoadMem(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL    APIENTRY IKSxfSfcFileSaveMem(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);

BOOL    APIENTRY IKFTPConnect(LPCTSTR ServerName, LPCTSTR UserName, LPCTSTR Password, HINTERNET *hOpen, HINTERNET *hConnect);
BOOL    APIENTRY IKFTPDisconnect(HINTERNET hOpen, HINTERNET hConnect);
BOOL    APIENTRY IKFTPGetFileEx(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);
BOOL    APIENTRY IKFTPPutFileEx(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);
BOOL    APIENTRY IKFTPDeleteFileEx(HINTERNET hConnect, LPCTSTR DeleteFile);
BOOL    APIENTRY IKFTPRenameFileEx(HINTERNET hConnect, LPCTSTR OldFile, LPCTSTR NewFile);
BOOL    APIENTRY IKHTTPConnect(LPCTSTR ServerName, LPCTSTR UserName, LPCTSTR Password, HINTERNET *hOpen, HINTERNET *hConnect);
BOOL    APIENTRY IKHTTPDisconnect(HINTERNET hOpen, HINTERNET hConnect);
BOOL    APIENTRY IKHTTPGetFileEx(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);
BOOL    APIENTRY IKHTTPPutFileEx(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);

// Ik7Scan.DLL
BOOL    APIENTRY IKScanCloseDS(HANDLE AppHandle, HANDLE SrcHandle);
int     APIENTRY IKScanExec(HANDLE AppHandle, HANDLE SrcHandle, PTR_IKSCAN_EXEC Image, IK_BEFORESCANPROC BeforeScanProc, IK_SCANNINGPROC ScanningProc, IK_AFTERSCANPROC AfterScanProc, DWORD *Index);
DWORD   APIENTRY IKScanGetBitDepth(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD PixelType, WORD *List, DWORD ListNum);
DWORD   APIENTRY IKScanGetCapEnum(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD CapNo, WORD *ConType, float *List, LPSTR StringList, DWORD *CurrentIndex, DWORD *DefaultIndex, DWORD ListNum);
BOOL    APIENTRY IKScanGetCapRange(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD CapNo, WORD *ConType, PTR_IKSCAN_RANGE Value);
BOOL    APIENTRY IKScanGetDSInfo(HANDLE AppHandle, LPSTR ScanDsName, PTR_IKSCAN_DATASOURCEINFO DS);
BOOL    APIENTRY IKScanGetMinimumSize(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD ScanMode, WORD UnitMode, float *Width, float *Height);
BOOL    APIENTRY IKScanGetPhysicalSize(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD ScanMode, WORD UnitMode, float *Width, float *Height);
DWORD   APIENTRY IKScanGetPixelType(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD *List, WORD ListNum);
HANDLE  APIENTRY IKScanInitialize(HWND hWnd, WORD MajorNum, WORD MinorNum, LPSTR VersionInfo, LPSTR Manufacturer, LPSTR ProductFamily, LPSTR ProductName);
BOOL    APIENTRY IKScanIsCapSupported(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD CapNo, int *Value);
BOOL    APIENTRY IKScanList(HANDLE AppHandle, LPSTR List, short *SourceCount);
int     APIENTRY IKScanListLen(HANDLE AppHandle);
HANDLE  APIENTRY IKScanOpenDS(HANDLE AppHandle, LPSTR ScanDsName);
BOOL    APIENTRY IKScanSelect(HANDLE AppHandle, PTR_IKSCAN_DATASOURCEINFO SelectDS);
BOOL    APIENTRY IKScanTerminate(HANDLE AppHandle);

// Ik7Print.DLL
BOOL    APIENTRY IKArc(LPVOID DeviceValue, LPRECT PrintRect, int x1, int y1, int x2, int y2, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKChord(LPVOID DeviceValue, LPRECT PrintRect, int x1, int y1, int x2, int y2, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKDrawText(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_TEXTINFO TextInfo, LPSTR Text, BOOL EnableFontScale, BOOL Clip, BYTE DeviceMode);
BOOL    APIENTRY IKEllipse(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
DWORD   APIENTRY IKEnumPrinters(LPSTR Printers, short *DefaultPrinterNo);
BOOL    APIENTRY IKFillRect(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
HANDLE  APIENTRY IKGetDevModeHandle(LPCSTR PrinterName, LPCSTR PrintFileName);
BOOL    APIENTRY IKGetDevModeInfo(HANDLE hDevMode, PTR_IKPRINT_DEVMODEINFO DevModeInfo);
HANDLE  APIENTRY IKGetImageFromHdc(HDC hDC, long Width, long Height, short BitCount);
BOOL    APIENTRY IKGetPaperSize(HDC hDC, LPRECT Rect, LPLONG Width, LPLONG Height, BYTE UnitMode);
BOOL    APIENTRY IKGetPixel(LPVOID DeviceValue, int x, int y, LPBYTE Red, LPBYTE Green, LPBYTE Blue, BYTE DeviceMode);
DWORD   APIENTRY IKGetPrinterPort(LPCSTR PrinterName, LPSTR PortName);
BOOL    APIENTRY IKGetTextExtent(HDC hDC, LPLONG Width, LPLONG Height, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE UnitMode);
BOOL    APIENTRY IKImageOut(HDC hDC, HANDLE Handle, LPRECT PrintRect, BOOL Aspect, BOOL DXFBlack, BYTE DeviceMode);
BOOL    APIENTRY IKImageOutToHwnd(HWND hWnd, HANDLE Handle, COLORREF Color, BOOL Aspect, BOOL DXFBlack);
BOOL    APIENTRY IKLine(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKPaint(LPVOID DeviceValue, int x, int y, COLORREF FColor, COLORREF TColor, BYTE PaintMode, BYTE DeviceMode);
BOOL    APIENTRY IKPie(LPVOID DeviceValue, LPRECT PrintRect, int x1, int y1, int x2, int y2, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKPolygon(LPVOID DeviceValue, LPPOINT lpPoint, int Points, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKPolyline(LPVOID DeviceValue, LPPOINT lpPoint, int Points, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKPreviewInit(HDC phDC, HDC dhDC);
BOOL    APIENTRY IKPrintAbortDoc(HDC hDC);
HDC     APIENTRY IKPrintCreateDC(LPCSTR PrinterName, LPCSTR PrintFileName, HANDLE hDevMode, short Mode);
BOOL    APIENTRY IKPrintDeleteDC(HDC hDC);
HDC     APIENTRY IKPrintDlg(HWND hWnd, HANDLE hDevMode, PTR_IKPRINT_DIALOG IkPrnDlg);
BOOL    APIENTRY IKPrintEndDoc(HDC hDC);
BOOL    APIENTRY IKPrintEndPage(HDC hDC);
BOOL    APIENTRY IKPrintStartDoc(HDC hDC, HWND hWnd, LPCSTR Caption, LPCSTR Message, LPCSTR ButtonName, LPCSTR DocName);
BOOL    APIENTRY IKPrintStartPage(HDC hDC);
BOOL    APIENTRY IKRectangle(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKReleaseDevModeHandle(HANDLE hDevMode);
BOOL    APIENTRY IKRoundRect(LPVOID DeviceValue, LPRECT PrintRect, int x, int y, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKSetDevModeInfo(HANDLE hDevMode, PTR_IKPRINT_DEVMODEINFO DevModeInfo);
BOOL    APIENTRY IKSetPixel(LPVOID DeviceValue, int x, int y, COLORREF Color, BYTE DeviceMode);
BOOL    APIENTRY IKSetPrint(HWND hWnd, LPCSTR PrintFileName);
BOOL    APIENTRY IKTextOut(LPVOID DeviceValue, int x, int y, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE DeviceMode);
DWORD   APIENTRY IKEnumPorts(LPSTR Ports);
BOOL    APIENTRY IKPolyBezier(LPVOID DeviceValue, LPPOINT lpPoint, DWORD nPoints, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKFrameRect(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
BOOL    APIENTRY IKDrawFocusRect(HDC hDC, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE UnitMode);
int     APIENTRY IKEnumPaperSizes(LPCSTR PrinterName, LPSTR PaperNames, LPWORD PaperNumbers, LPPOINT PaperSizes);
int     APIENTRY IKEnumPaperBins(LPCSTR PrinterName, LPSTR BinNames, LPWORD BinNumbers);
int     APIENTRY IKEnumResolutions(LPCSTR PrinterName, LPPOINT Resolutions);
int     APIENTRY IKPrintGetArrayNum(LPCSTR PrinterName, short CapNo);
DWORD   APIENTRY IKGetDefaultPrinter(LPSTR PrinterName);
BOOL    APIENTRY IKSetDefaultPrinter(LPCSTR PrinterName);
BOOL    APIENTRY IKDrawString(LPVOID DeviceValue, LPRECT PrintRect, short BrushType, short Style, HANDLE imgHandle, short TextRenderingHint, int FormatFlags, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE DeviceMode);
BOOL    APIENTRY IKMeasureString(HDC hDC, int AreaWidth, int AreaHeight, float *Width, float *Height, LPINT charactersFitted, LPINT linesFilled, int FormatFlags, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE UnitMode);
BOOL    APIENTRY IKSaveDevModeHandle(LPCSTR PrinterName, HANDLE hDevMode);
void    APIENTRY IKPrintGdipEnd(ULONG_PTR token);
ULONG_PTR APIENTRY IKPrintGdipStart(void);

// Ik7Effect.DLL
HANDLE    APIENTRY IKAffine(HANDLE Handle, int Px1, int Py1, int Px2, int Py2, int Px3, int Py3, int Px4, int Py4, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKAntiAlias(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKAutoSelectImageEx(HANDLE Handle, PTR_IKSELECT_IMAGE DstHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Mode, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKBlur(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKCanvas(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Direction, BYTE Depth, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKCheckSecretImage(HANDLE Handle1, HANDLE Handle2, int Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKChroma(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKConvertColor(HANDLE Handle, short PixelType, BOOL FixedPal, BOOL Dither, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKCustomFilter(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, LPINT Matrix, BYTE Div_, BYTE Level, BOOL Red, BOOL Green, BOOL Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKCutRectImage(HANDLE Handle, LPRECT Rect, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKEmboss(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE MskPattern, BYTE Edge, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKEndDibAccess(HANDLE hDib, PTR_IKDIB_INFO stpDibInfo);
BOOL      APIENTRY IKGetDibPixel(PTR_IKDIB_INFO stpDibInfo, long lx, long ly);
HANDLE    APIENTRY IKGlassTile(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int xSize, int ySize, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKLayer(HANDLE Handle1, HANDLE Handle2, BYTE Trans, BOOL TransColor, BYTE TRed, BYTE TGreen, BYTE TBlue, BYTE BRed, BYTE BGreen, BYTE BBlue, int x, int y, BOOL Clip, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKLayerEx(HANDLE Handle1, HANDLE Handle2, BYTE Trans, BOOL TransColor, BYTE TRed, BYTE TGreen, BYTE TBlue, BYTE BRed, BYTE BGreen, BYTE BBlue, int x, int y, BOOL Clip, BOOL Alpha, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKLens(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Refract, BOOL BackColor, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKMakeRGBAImage(HANDLE InHandle1, HANDLE InHandle2, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKMosaic(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKMotionBlur(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Mode, BYTE Length, int Angle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKOilPaint(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Mode, BYTE MaskSize, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKOutline(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Plane, BYTE Operator, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKPanorama(HANDLE Handle1, HANDLE Handle2, BYTE CutMode, int Px11, int Py11, int Px12, int Py12, int Px21, int Py21, int Px22, int Py22, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKPasteImage(HANDLE Handle, PTR_IKSELECT_IMAGE SrcHandle, int Angle, BOOL TurnX, BOOL TurnY, BYTE Trans, BOOL TransColor, BYTE TRed, BYTE TGreen, BYTE TBlue, BYTE BRed, BYTE BGreen, BYTE BBlue, int x, int y, BOOL Clip, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKRemoveNoise(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Mode, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKResizeEx(PTR_IKSELECT_IMAGE SrcHandle, PTR_IKSELECT_IMAGE DstHandle, long Width, long Height, BOOL Mode, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKRGBGamma(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Red, double Green, double Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKRGBLevel(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int Red, int Green, int Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKRGBRev(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BOOL Red, BOOL Green, BOOL Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKRGBSpline(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, LPPOINT lpSpPoint, int SpPoints, BOOL Red, BOOL Green, BOOL Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKRipple(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Amp, double Wavelen, double Phase, BOOL BackColor, BOOL Reflect, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKRotationEx(PTR_IKSELECT_IMAGE SrcHandle, PTR_IKSELECT_IMAGE DstHandle, int Angle, BOOL TurnX, BOOL TurnY, BOOL Mode, BYTE Red, BYTE Green, BYTE Blue, BOOL Clip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKSelectImageEx(HANDLE Handle, PTR_IKSELECT_IMAGE DstHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKSetDibPixel(PTR_IKDIB_INFO stpDibInfo, long lx, long ly);
HANDLE    APIENTRY IKSetGray(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKSetSecretImage(HANDLE Handle1, HANDLE Handle2, LPCSTR Text, LPCSTR FName, WORD FSize, BOOL Bold, BOOL Italic, BOOL Underline, BOOL StrikeOut, BYTE Direction, int Angle, int Level, int Left, int Top, int Right, int Bottom, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKSharp(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKSplitRGBAImage(HANDLE InHandle, HANDLE *OutHandle1, HANDLE *OutHandle2, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
BOOL      APIENTRY IKStartDibAccess(HANDLE hDib, PTR_IKDIB_INFO stpDibInfo);
HANDLE    APIENTRY IKUnifyColor(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Error, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKWaves(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Direction, BYTE WaveType, int Period, int Amp, double Phase, BYTE Edges, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKWhirlPinch(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Whirl, double Pinch, double Radius, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKYCCGamma(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Yb, double Cr, double Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKYCCLevel(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int Yb, int Cr, int Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKYCCRev(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BOOL Yb, BOOL Cr, BOOL Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKYCCSpline(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, LPPOINT lpSpPoint, int SpPoints, BOOL Yb, BOOL Cr, BOOL Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
IKSELECT_IMAGE    APIENTRY IKAutoSelectImage(HANDLE Handle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Mode, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
IKSELECT_IMAGE    APIENTRY IKResize(PTR_IKSELECT_IMAGE SrcHandle, long Width, long Height, BOOL Mode, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
IKSELECT_IMAGE    APIENTRY IKRotation(PTR_IKSELECT_IMAGE SrcHandle, int Angle, BOOL TurnX, BOOL TurnY, BOOL Mode, BYTE Red, BYTE Green, BYTE Blue, BOOL Clip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
IKSELECT_IMAGE    APIENTRY IKSelectImage(HANDLE Handle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
HANDLE    APIENTRY IKRedEyeRemoval(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Error, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);

// Ik7RasToVect.DLL
HANDLE APIENTRY IKRasterToVector(HANDLE Handle, BOOL BlackOnWhite, short Tolerance, long TimeOutSeconds, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);

#ifdef __cplusplus
}
#endif

// Ik7Com.DLL
typedef HBITMAP (APIENTRY *PROCIKBITMAPFROMDIB)(HANDLE hDib);
typedef void    (APIENTRY *PROCIKCLEARCLIPBRD)(HWND hWnd);
typedef HANDLE  (APIENTRY *PROCIKCOPYIMAGE)(HANDLE Handle);
typedef BOOL    (APIENTRY *PROCIKBITBLT)(HDC hDCDst, long XDst, long YDst, long Width, long Height, HDC hDCSrc, long XSrc, long YSrc, DWORD dwRop);
typedef BOOL    (APIENTRY *PROCIKSTRETCHBLT)(HDC hDCDst, long XDst, long YDst, long WidthDst, long HeightDst, HDC hDCSrc, long XSrc, long YSrc, long WidthSrc, long HeightSrc, DWORD dwRop);
typedef HANDLE  (APIENTRY *PROCIKCREATEIMAGE)(long Width, long Height, int BitCount, int Red, int Green, int Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKDELETEBITMAPOBJECT)(HBITMAP hBitmap);
typedef HANDLE  (APIENTRY *PROCIKDIBFROMBITMAP)(HBITMAP hBm, HPALETTE hPal);
typedef BOOL    (APIENTRY *PROCIKFREEMEMORY)(HANDLE Handle);
typedef BOOL    (APIENTRY *PROCIKGETDPI)(HANDLE Handle, long *Xdpi, long *Ydpi);
typedef BOOL    (APIENTRY *PROCIKGETDPIFROMHDC)(HDC hDC, long *Xdpi, long *Ydpi);
typedef int     (APIENTRY *PROCIKGETERRORSTATUS)(void);
typedef HANDLE  (APIENTRY *PROCIKGETFROMCLIPBRD)(HWND hWnd, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKGETIMAGETYPE)(HANDLE Handle, PTR_IKIMAGE_INFO ImageInfo);
typedef DWORD   (APIENTRY *PROCIKGETMEMORYSIZE)(HANDLE Handle);
typedef BOOL    (APIENTRY *PROCIKGETONEBITPALCOUNT)(HANDLE Handle, long *Pal0, long *Pal1);
typedef BOOL    (APIENTRY *PROCIKGETPALETTE)(HANDLE hDib, short *Red, short *Green, short *Blue);
typedef int     (APIENTRY *PROCIKGETSYSTEMPALETTE)(void);
typedef BOOL    (APIENTRY *PROCIKISCLIPBRDDATA)(HWND hWnd);
typedef HANDLE  (APIENTRY *PROCIKRESIZEREFINE1BITIMAGE)(HANDLE Handle, long Width, long Height);
typedef BOOL    (APIENTRY *PROCIKSETDPI)(HANDLE Handle, long Xdpi, long Ydpi);
typedef BOOL    (APIENTRY *PROCIKSETPALETTE)(HANDLE hDib, short *Red, short *Green, short *Blue);
typedef BOOL    (APIENTRY *PROCIKSETTOCLIPBRD)(HWND hWnd, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKGETDPIF)(HANDLE Handle, float *Xdpi, float *Ydpi);
typedef BOOL    (APIENTRY *PROCIKSETDPIF)(HANDLE Handle, float Xdpi, float Ydpi);

// Ik7VectCom.DLL
typedef HANDLE  (APIENTRY *PROCIKCREATEVECTIMAGE)(long Width, long Height, long Xdpi, long Ydpi);
typedef HANDLE  (APIENTRY *PROCIKCREATEVECTIMAGEEX)(long Type, long Width, long Height, long Xdpi, long Ydpi);
typedef BOOL    (APIENTRY *PROCIKDRAWVECTOBJECT)(HDC hDC, HANDLE Mbh, long DstOrgX, long DstOrgY, long SrcOrgX, long SrcOrgY, double XScale, double YScale, BOOL Black);
typedef HANDLE  (APIENTRY *PROCIKVECTORTORASTER)(HANDLE Mbh, short BitCount, BOOL Black, BYTE BackRed, BYTE BackGre, BYTE BackBlu);
typedef void    (APIENTRY *PROCIKVECTORGDIPEND)(ULONG_PTR token);
typedef ULONG_PTR (APIENTRY *PROCIKVECTORGDIPSTART)(void);

// Ik7File.DLL
typedef HANDLE  (APIENTRY *PROCIKFILELOADASRAWDATA)(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKFILESAVEASRAWDATA)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKFILELOAD)(LPCSTR FileName, int Page, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKFILETYPE)(LPCSTR FileName, int Page, PTR_IKFILE_INFO FileType);
typedef BOOL    (APIENTRY *PROCIKOPENFILEDIALOG)(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL ExtendedDialog, BOOL Preview, BOOL FileInfo);
typedef BOOL    (APIENTRY *PROCIKOPENFILEDLG)(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL Preview);
typedef BOOL    (APIENTRY *PROCIKSAVEFILEDIALOG)(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL ExtendedDialog, BOOL Preview, LPINT FileType, BOOL FileInfo);
typedef BOOL    (APIENTRY *PROCIKSAVEFILEDLG)(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL Preview);
typedef BOOL    (APIENTRY *PROCIKSAVEFILEDLGEX)(HWND hWnd, LPSTR FileName, LPCSTR FilePath, LPCSTR FileExt, BOOL Preview, LPINT FileType);
typedef BOOL    (APIENTRY *PROCIKFTPGETFILE)(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKFTPPUTFILE)(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKFTPDELETEFILE)(LPCTSTR ServerName, LPCTSTR DeleteFile, LPCTSTR UserName, LPCTSTR Password);
typedef BOOL    (APIENTRY *PROCIKFTPRENAMEFILE)(LPCTSTR ServerName, LPCTSTR OldFile, LPCTSTR NewFile, LPCTSTR UserName, LPCTSTR Password);
typedef BOOL    (APIENTRY *PROCIKHTTPGETFILE)(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKHTTPPUTFILE)(LPCTSTR ServerName, LPCTSTR RemoteFile, LPCTSTR LocalFile, LPCTSTR UserName, LPCTSTR Password, long TransPercent, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKBMPFILELOAD)(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKBMPFILESAVE)(LPCSTR FileName, HANDLE Handle, BOOL Comp, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKJPEGFILELOAD)(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKJPEGFILESAVE)(LPCSTR FileName, HANDLE Handle, int Type_, int Quality, int SubSamp, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKJPEGEXIFINFO)(LPCSTR FileName, PTR_EXIF_INFO ExifInfo);
typedef HANDLE  (APIENTRY *PROCIKGIFFILELOAD)(LPCSTR FileName, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKGIFFILESAVE)(LPCSTR FileName, HANDLE Handle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, BOOL Anime, int Delay, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKTIFFFILELOAD)(LPCSTR FileName, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKTIFFFILESAVE)(LPCSTR FileName, HANDLE Handle, int Type_, int Color, BOOL Append, LPCSTR Comment, BOOL OneStrip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKPNGFILELOAD)(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKPNGFILELOADEX)(LPCSTR FileName, int Alpha, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKPNGFILESAVE)(LPCSTR FileName, HANDLE Handle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKPNGFILESAVEEX)(LPCSTR FileName, HANDLE Handle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, BOOL Alpha, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKFPXFILELOAD)(LPCSTR FileName, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKFPXFILESAVE)(LPCSTR FileName, HANDLE Handle, int Comp, int Quality, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKPCXFILELOAD)(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKPCXFILESAVE)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKWMFFILELOAD)(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKWMFFILESAVE)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKEMFFILELOAD)(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKEMFFILESAVE)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKDXFFILELOAD)(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKDXFFILESAVE)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKSVGFILELOAD)(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKSVGFILESAVE)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKJ2KFILELOAD)(LPCSTR FileName, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKJ2KFILESAVE)(LPCSTR FileName, HANDLE Handle, BOOL JP2kFormat, BOOL Reversible, double Size, long TileWidth, long TileHeight, short NumrResLevel, short PrecinctWidth, short PrecinctHeight, short CodeBlockWidth, short CodeBlockHeight, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKSXFP21FILELOAD)(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKSXFP21FILESAVE)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKSXFSFCFILELOAD)(LPCSTR FileName, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKSXFSFCFILESAVE)(LPCSTR FileName, HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);

typedef HANDLE  (APIENTRY *PROCIKRGBBMPPLANEFILELOAD)(LPCSTR RedFileName, LPCSTR GreenFileName, LPCSTR BlueFileName);
typedef BOOL    (APIENTRY *PROCIKRGBBMPPLANEFILESAVE)(LPCSTR RedFileName, LPCSTR GreenFileName, LPCSTR BlueFileName, HANDLE Handle);
typedef HANDLE  (APIENTRY *PROCIKCMYKBMPPLANEFILELOAD)(LPCSTR CFileName, LPCSTR MFileName, LPCSTR YFileName, LPCSTR KFileName);
typedef BOOL    (APIENTRY *PROCIKCMYKBMPPLANEFILESAVE)(LPCSTR CFileName, LPCSTR MFileName, LPCSTR YFileName, LPCSTR KFileName, HANDLE Handle);
typedef HANDLE  (APIENTRY *PROCIKYCCBMPPLANEFILELOAD)(LPCSTR YFileName, LPCSTR CrFileName, LPCSTR CbFileName);
typedef BOOL    (APIENTRY *PROCIKYCCBMPPLANEFILESAVE)(LPCSTR YFileName, LPCSTR CrFileName, LPCSTR CbFileName, HANDLE Handle);

typedef HANDLE  (APIENTRY *PROCIKFILELOADMEM)(HANDLE Handle, int Page, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKFILETYPEMEM)(HANDLE Handle, int Page, PTR_IKFILE_INFO FileType);
typedef HANDLE  (APIENTRY *PROCIKBMPFILELOADMEM)(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKBMPFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, BOOL Comp, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKJPEGFILELOADMEM)(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKJPEGFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, int Type, int Quality, int SubSamp, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKJPEGEXIFINFOMEM)(HANDLE Handle, PTR_EXIF_INFO ExifInfo);
typedef HANDLE  (APIENTRY *PROCIKGIFFILELOADMEM)(HANDLE Handle, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKGIFFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, BOOL Interlace, BOOL Trans, BYTE TRed, BYTE TGreen, BYTE TBlue, BOOL Anime, int Delay, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKTIFFFILELOADMEM)(HANDLE Handle, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKTIFFFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, int Type_, int Color, BOOL Append, LPCSTR Comment, BOOL OneStrip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKPNGFILELOADMEM)(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKPNGFILELOADEXMEM)(HANDLE Handle, int Alph, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKPNGFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKPNGFILESAVEEXMEM)(HANDLE InHandle, HANDLE *OutHandle, BOOL Interlace, BOOL Trans, BYTE Red, BYTE Green, BYTE Blue, BOOL Alpha, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKFPXFILELOADMEM)(HANDLE Handle, int Page, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKFPXFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, int Comp, int Quality, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKPCXFILELOADMEM)(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKPCXFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKWMFFILELOADMEM)(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKWMFFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKEMFFILELOADMEM)(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKEMFFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKDXFFILELOADMEM)(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKDXFFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKSVGFILELOADMEM)(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKSVGFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKJ2KFILELOADMEM)(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKJ2KFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, BOOL JP2kFormat, BOOL Reversible, double Size, long TileWidth, long TileHeight, short NumrResLevel, short PrecinctWidth, short PrecinctHeight, short CodeBlockWidth, short CodeBlockHeight, LPCSTR Comment, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKSXFP21FILELOADMEM)(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKSXFP21FILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE  (APIENTRY *PROCIKSXFSFCFILELOADMEM)(HANDLE Handle, long Width, long Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL    (APIENTRY *PROCIKSXFSFCFILESAVEMEM)(HANDLE InHandle, HANDLE *OutHandle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);

typedef BOOL    (APIENTRY *PROCIKFTPCONNECT)(LPCTSTR ServerName, LPCTSTR UserName, LPCTSTR Password, HINTERNET *hOpen, HINTERNET *hConnect);
typedef BOOL    (APIENTRY *PROCIKFTPDISCONNECT)(HINTERNET hOpen, HINTERNET hConnect);
typedef BOOL    (APIENTRY *PROCIKFTPGETFILEEX)(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);
typedef BOOL    (APIENTRY *PROCIKFTPPUTFILEEX)(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);
typedef BOOL    (APIENTRY *PROCIKFTPDELETEFILEEX)(HINTERNET hConnect, LPCTSTR DeleteFile);
typedef BOOL    (APIENTRY *PROCIKFTPRENAMEFILEEX)(HINTERNET hConnect, LPCTSTR OldFile, LPCTSTR NewFile);
typedef BOOL    (APIENTRY *PROCIKHTTPCONNECT)(LPCTSTR ServerName, LPCTSTR UserName, LPCTSTR Password, HINTERNET *hOpen, HINTERNET *hConnect);
typedef BOOL    (APIENTRY *PROCIKHTTPDISCONNECT)(HINTERNET hOpen, HINTERNET hConnect);
typedef BOOL    (APIENTRY *PROCIKHTTPGETFILEEX)(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);
typedef BOOL    (APIENTRY *PROCIKHTTPPUTFILEEX)(HINTERNET hConnect, LPCTSTR RemoteFile, LPCTSTR LocalFile, long TransPercent, IKPROCESSPROC UserProc);

// Ik7Scan.DLL
typedef BOOL    (APIENTRY *PROCIKSCANCLOSEDS)(HANDLE AppHandle, HANDLE SrcHandle);
typedef int     (APIENTRY *PROCIKSCANEXEC)(HANDLE AppHandle, HANDLE SrcHandle, PTR_IKSCAN_EXEC Image, IK_BEFORESCANPROC BeforeScanProc, IK_SCANNINGPROC ScanningProc, IK_AFTERSCANPROC AfterScanProc, DWORD *Index);
typedef DWORD   (APIENTRY *PROCIKSCANGETBITDEPTH)(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD PixelType, WORD *List, DWORD ListNum);
typedef DWORD   (APIENTRY *PROCIKSCANGETCAPENUM)(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD CapNo, WORD *ConType, float *List, LPSTR StringList, DWORD *CurrentIndex, DWORD *DefaultIndex, DWORD ListNum);
typedef BOOL    (APIENTRY *PROCIKSCANGETCAPRANGE)(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD CapNo, WORD *ConType, PTR_IKSCAN_RANGE Value);
typedef BOOL    (APIENTRY *PROCIKSCANGETDSINFO)(HANDLE AppHandle, LPSTR ScanDsName, PTR_IKSCAN_DATASOURCEINFO DS);
typedef BOOL    (APIENTRY *PROCIKSCANGETMINIMUMSIZE)(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, short ScanMode, short UnitMode, float *Width, float *Height);
typedef BOOL    (APIENTRY *PROCIKSCANGETPHYSICALSIZE)(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD ScanMode, WORD UnitMode, float *Width, float *Height);
typedef DWORD   (APIENTRY *PROCIKSCANGETPIXELTYPE)(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD *List, WORD ListNum);
typedef HANDLE  (APIENTRY *PROCIKSCANINITIALIZE)(HWND hWnd, WORD MajorNum, WORD MinorNum, LPSTR VersionInfo, LPSTR Manufacturer, LPSTR ProductFamily, LPSTR ProductName);
typedef BOOL    (APIENTRY *PROCIKSCANISCAPSUPPORTED)(HANDLE AppHandle, HANDLE SrcHandle, LPSTR ScanDsName, WORD CapNo, int *Value);
typedef BOOL    (APIENTRY *PROCIKSCANLIST)(HANDLE AppHandle, LPSTR List, short *SourceCount);
typedef int     (APIENTRY *PROCIKSCANLISTLEN)(HANDLE AppHandle);
typedef HANDLE  (APIENTRY *PROCIKSCANOPENDS)(HANDLE AppHandle, LPSTR ScanDsName);
typedef BOOL    (APIENTRY *PROCIKSCANSELECT)(HANDLE AppHandle, PTR_IKSCAN_DATASOURCEINFO SelectDS);
typedef BOOL    (APIENTRY *PROCIKSCANTERMINATE)(HANDLE AppHandle);

// Ik7Print.DLL
typedef BOOL    (APIENTRY *PROCIKARC)(LPVOID DeviceValue, LPRECT PrintRect, int x1, int y1, int x2, int y2, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKCHORD)(LPVOID DeviceValue, LPRECT PrintRect, int x1, int y1, int x2, int y2, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKDRAWTEXT)(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_TEXTINFO TextInfo, LPSTR Text, BOOL EnableFontScale, BOOL Clip, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKELLIPSE)(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef DWORD   (APIENTRY *PROCIKENUMPRINTERS)(LPSTR Printers, short *DefaultPrinterNo);
typedef BOOL    (APIENTRY *PROCIKFILLRECT)(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef HANDLE  (APIENTRY *PROCIKGETDEVMODEHANDLE)(LPCSTR PrinterName, LPCSTR PrintFileName);
typedef BOOL    (APIENTRY *PROCIKGETDEVMODEINFO)(HANDLE hDevMode, PTR_IKPRINT_DEVMODEINFO DevModeInfo);
typedef HANDLE  (APIENTRY *PROCIKGETIMAGEFROMHDC)(HDC hDC, long Width, long Height, short BitCount);
typedef BOOL    (APIENTRY *PROCIKGETPAPERSIZE)(HDC hDC, LPRECT Rect, LPLONG Width, LPLONG Height, BYTE UnitMode);
typedef BOOL    (APIENTRY *PROCIKGETPIXEL)(LPVOID DeviceValue, int x, int y, LPBYTE Red, LPBYTE Green, LPBYTE Blue, BYTE DeviceMode);
typedef DWORD   (APIENTRY *PROCIKGETPRINTERPORT)(LPCSTR PrinterName, LPSTR PortName);
typedef BOOL    (APIENTRY *PROCIKGETTEXTEXTENT)(HDC hDC, LPLONG Width, LPLONG Height, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE UnitMode);
typedef BOOL    (APIENTRY *PROCIKIMAGEOUT)(HDC hDC, HANDLE Handle, LPRECT PrintRect, BOOL Aspect, BOOL DXFBlack, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKIMAGEOUTTOHWND)(HWND hWnd, HANDLE Handle, COLORREF Color, BOOL Aspect, BOOL DXFBlack);
typedef BOOL    (APIENTRY *PROCIKLINE)(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKPAINT)(LPVOID DeviceValue, int x, int y, COLORREF FColor, COLORREF TColor, BYTE PaintMode, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKPIE)(LPVOID DeviceValue, LPRECT PrintRect, int x1, int y1, int x2, int y2, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKPOLYGON)(LPVOID DeviceValue, LPPOINT lpPoint, int Points, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKPOLYLINE)(LPVOID DeviceValue, LPPOINT lpPoint, int Points, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKPREVIEWINIT)(HDC phDC, HDC dhDC);
typedef BOOL    (APIENTRY *PROCIKPRINTABORTDOC)(HDC hDC);
typedef HDC     (APIENTRY *PROCIKPRINTCREATEDC)(LPCSTR PrinterName, LPCSTR PrintFileName, HANDLE hDevMode, short Mode);
typedef BOOL    (APIENTRY *PROCIKPRINTDELETEDC)(HDC hDC);
typedef HDC     (APIENTRY *PROCIKPRINTDLG)(HWND hWnd, HANDLE hDevMode, PTR_IKPRINT_DIALOG IkPrnDlg);
typedef BOOL    (APIENTRY *PROCIKPRINTENDDOC)(HDC hDC);
typedef BOOL    (APIENTRY *PROCIKPRINTENDPAGE)(HDC hDC);
typedef BOOL    (APIENTRY *PROCIKPRINTSTARTDOC)(HDC hDC, HWND hWnd, LPCSTR Caption, LPCSTR Message, LPCSTR ButtonName, LPCSTR DocName);
typedef BOOL    (APIENTRY *PROCIKPRINTSTARTPAGE)(HDC hDC);
typedef BOOL    (APIENTRY *PROCIKRECTANGLE)(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKRELEASEDEVMODEHANDLE)(HANDLE hDevMode);
typedef BOOL    (APIENTRY *PROCIKROUNDRECT)(LPVOID DeviceValue, LPRECT PrintRect, int x, int y, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKSETDEVMODEINFO)(HANDLE hDevMode, PTR_IKPRINT_DEVMODEINFO DevModeInfo);
typedef BOOL    (APIENTRY *PROCIKSETPIXEL)(LPVOID DeviceValue, int x, int y, COLORREF Color, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKSETPRINT)(HWND hWnd, LPCSTR PrintFileName);
typedef BOOL    (APIENTRY *PROCIKTEXTOUT)(LPVOID DeviceValue, int x, int y, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE DeviceMode);
typedef DWORD   (APIENTRY *PROCIKENUMPORTS)(LPSTR Ports);
typedef BOOL    (APIENTRY *PROCIKPOLYBEZIER)(LPVOID DeviceValue, LPPOINT lpPoint, DWORD nPoints, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKFRAMERECT)(LPVOID DeviceValue, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKDRAWFOCUSRECT)(HDC hDC, LPRECT PrintRect, PTR_IKPRINT_DRAWINFO DrawInfo, BYTE UnitMode);
typedef int     (APIENTRY *PROCIKENUMPAPERSIZES)(LPCSTR PrinterName, LPSTR PaperNames, LPWORD PaperNumbers, LPPOINT PaperSizes);
typedef int     (APIENTRY *PROCIKENUMPAPERBINS)(LPCSTR PrinterName, LPSTR BinNames, LPWORD BinNumbers);
typedef int     (APIENTRY *PROCIKENUMRESOLUTIONS)(LPCSTR PrinterName, LPPOINT Resolutions);
typedef int     (APIENTRY *PROCIKPRINTGETARRAYNUM)(LPCSTR PrinterName, short CapNo);
typedef DWORD   (APIENTRY *PROCIKGETDEFAULTPRINTER)(LPSTR PrinterName);
typedef BOOL    (APIENTRY *PROCIKSETDEFAULTPRINTER)(LPCSTR PrinterName);
typedef BOOL    (APIENTRY *PROCIKDRAWSTRING)(LPVOID DeviceValue, LPRECT PrintRect, short BrushType, short Style, HANDLE imgHandle, short TextRenderingHint, int FormatFlags, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE DeviceMode);
typedef BOOL    (APIENTRY *PROCIKMEASURESTRING)(HDC hDC, int AreaWidth, int AreaHeight, float *Width, float *Height, LPINT charactersFitted, LPINT linesFilled, int FormatFlags, PTR_IKPRINT_TEXTINFO TextInfo, LPCSTR Text, BYTE UnitMode);
typedef BOOL    (APIENTRY *PROCIKSAVEDEVMODEHANDLE)(LPCSTR PrinterName, HANDLE hDevMode);
typedef void    (APIENTRY *PROCIKPRINTGDIPEND)(ULONG_PTR token);
typedef ULONG_PTR (APIENTRY *PROCIKPRINTGDIPSTART)(void);

// Ik7Effect.DLL
typedef HANDLE    (APIENTRY *PROCIKAFFINE)(HANDLE Handle, int Px1, int Py1, int Px2, int Py2, int Px3, int Py3, int Px4, int Py4, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKANTIALIAS)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKAUTOSELECTIMAGEEX)(HANDLE Handle, PTR_IKSELECT_IMAGE DstHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Mode, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKBLUR)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKCANVAS)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Direction, BYTE Depth, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKCHECKSECRETIMAGE)(HANDLE Handle1, HANDLE Handle2, int Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKCHROMA)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKCONVERTCOLOR)(HANDLE Handle, short PixelType, BOOL FixedPal, BOOL Dither, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKCUSTOMFILTER)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, LPINT Matrix, BYTE Div_, BYTE Level, BOOL Red, BOOL Green, BOOL Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKCUTRECTIMAGE)(HANDLE Handle, LPRECT Rect, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKEMBOSS)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE MskPattern, BYTE Edge, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKENDDIBACCESS)(HANDLE hDib, PTR_IKDIB_INFO stpDibInfo);
typedef BOOL      (APIENTRY *PROCIKGETDIBPIXEL)(PTR_IKDIB_INFO stpDibInfo, long lx, long ly);
typedef HANDLE    (APIENTRY *PROCIKGLASSTILE)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int xSize, int ySize, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKLAYER)(HANDLE Handle1, HANDLE Handle2, BYTE Trans, BOOL TransColor, BYTE TRed, BYTE TGreen, BYTE TBlue, BYTE BRed, BYTE BGreen, BYTE BBlue, int x, int y, BOOL Clip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKLAYEREX)(HANDLE Handle1, HANDLE Handle2, BYTE Trans, BOOL TransColor, BYTE TRed, BYTE TGreen, BYTE TBlue, BYTE BRed, BYTE BGreen, BYTE BBlue, int x, int y, BOOL Clip, BOOL Alpha, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKLENS)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Refract, BOOL BackColor, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKMAKERGBAIMAGE)(HANDLE InHandle1, HANDLE InHandle2, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKMOSAIC)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int Width, int Height, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKMOTIONBLUR)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Mode, BYTE Length, int Angle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKOILPAINT)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Mode, BYTE MaskSize, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKOUTLINE)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Plane, BYTE Operator, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKPANORAMA)(HANDLE Handle1, HANDLE Handle2, BYTE CutMode, int Px11, int Py11, int Px12, int Py12, int Px21, int Py21, int Px22, int Py22, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKPASTEIMAGE)(HANDLE Handle, PTR_IKSELECT_IMAGE SrcHandle, int Angle, BOOL TurnX, BOOL TurnY, BYTE cTrans, BOOL TransColor, BYTE TRed, BYTE TGreen, BYTE TBlue, BYTE BRed, BYTE BGreen, BYTE BBlue, int Cx, int Cy, BOOL Clip, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKREMOVENOISE)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Mode, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKRESIZEEX)(PTR_IKSELECT_IMAGE SrcHandle, PTR_IKSELECT_IMAGE DstHandle, long Width, long Height, BOOL Mode, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKRGBGAMMA)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Red, double Green, double Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKRGBLEVEL)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int Red, int Green, int Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKRGBREV)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BOOL Red, BOOL Green, BOOL Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKRGBSPLINE)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, LPPOINT lpSpPoint, int SpPoints, BOOL Red, BOOL Green, BOOL Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKRIPPLE)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Amp, double Wavelen, double Phase, BOOL BackColor, BOOL Reflect, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKROTATIONEX)(PTR_IKSELECT_IMAGE SrcHandle, PTR_IKSELECT_IMAGE DstHandle, int Angle, BOOL TurnX, BOOL TurnY, BOOL Mode, BYTE Red, BYTE Green, BYTE Blue, BOOL Clip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKSELECTIMAGEEX)(HANDLE Handle, PTR_IKSELECT_IMAGE DstHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKSETDIBPIXEL)(PTR_IKDIB_INFO stpDibInfo, long lx, long ly);
typedef HANDLE    (APIENTRY *PROCIKSETGRAY)(HANDLE Handle, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKSETSECRETIMAGE)(HANDLE Handle1, HANDLE Handle2, LPCSTR Text, LPCSTR FName, WORD FSize, BOOL Bold, BOOL Italic, BOOL Underline, BOOL StrikeOut, BYTE Direction, int Angle, int Level, int Left, int Top, int Right, int Bottom, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKSHARP)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKSPLITRGBAIMAGE)(HANDLE InHandle, HANDLE *OutHandle1, HANDLE *OutHandle2, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef BOOL      (APIENTRY *PROCIKSTARTDIBACCESS)(HANDLE hDib, PTR_IKDIB_INFO stpDibInfo);
typedef HANDLE    (APIENTRY *PROCIKUNIFYCOLOR)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Error, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKWAVES)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Direction, BYTE WaveType, int Period, int Amp, double Phase, BYTE Edges, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKWHIRLPINCH)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Whirl, double Pinch, double Radius, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKYCCGAMMA)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, double Yb, double Cr, double Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKYCCLEVEL)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, int Yb, int Cr, int Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKYCCREV)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BOOL Yb, BOOL Cr, BOOL Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKYCCSPLINE)(PTR_IKSELECT_IMAGE SrcHandle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, LPPOINT lpSpPoint, int SpPoints, BOOL Yb, BOOL Cr, BOOL Cb, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef IKSELECT_IMAGE    (APIENTRY *PROCIKAUTOSELECTIMAGE)(HANDLE Handle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Mode, BYTE Level, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef IKSELECT_IMAGE    (APIENTRY *PROCIKRESIZE)(PTR_IKSELECT_IMAGE SrcHandle, long Width, long Height, BOOL Mode, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef IKSELECT_IMAGE    (APIENTRY *PROCIKROTATION)(PTR_IKSELECT_IMAGE SrcHandle, int Angle, BOOL TurnX, BOOL TurnY, BOOL Mode, BYTE Red, BYTE Green, BYTE Blue, BOOL Clip, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef IKSELECT_IMAGE    (APIENTRY *PROCIKSELECTIMAGE)(HANDLE Handle, BYTE SelectMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);
typedef HANDLE    (APIENTRY *PROCIKREDEYEREMOVAL)(PTR_IKSELECT_IMAGE Src, BYTE SctMode, LPPOINT lpPoint, int Points, LPRECT Rect, BOOL InOut, BYTE Red, BYTE Green, BYTE Blue, BYTE Error, IKPROCESSPROC EffectUserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);

// Ik7RasToVect.DLL
typedef HANDLE    (APIENTRY *PROCIKRASTERTOVECTOR)(HANDLE Handle, BOOL BlackOnWhite, short Tolerance, long TimeOutSeconds, IKPROCESSPROC UserProc, LPCSTR Caption, LPCSTR Message, LPCSTR Button);

#endif      // IMAGEKIT7_INCLUDE
