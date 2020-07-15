/*******************************************************************************
 *    ImageKit7    Ver. 1.2.0                                                  *
 *                                                                             *
 *    for C/C++    DLL�Ŏg�p                                                   *
 *                                                                             *
 *    <ImageKit7.h>                                          (��)�j���[�g��    *
 *******************************************************************************/
#ifndef IMAGEKIT7_INCLUDE
#define IMAGEKIT7_INCLUDE

// GDI+�p
#ifndef ULONG_PTR
#define ULONG_PTR ULONG
#endif

#ifndef HINTERNET
typedef LPVOID HINTERNET;
#endif

#pragma pack (push, 8)
/*******************************************************************************
 *                           �C���[�W���p�\����                              *
 *******************************************************************************/
typedef struct {
	short	BitCount;					// �s�N�Z���^�r�b�g
	short	PalCount;					// �g�p�p���b�g��
	short	Mask1632;					// �P�U�A�R�Q�r�b�g�J���[�̎� 1-�}�X�N�f�[�^����(RGB555�t�H�[�}�b�g)
										//                            2-�}�X�N�f�[�^����(RGB565�t�H�[�}�b�g)
										//                            3-�}�X�N�f�[�^�Ȃ�(RGB555�t�H�[�}�b�g)
										//                            4-�}�X�N�f�[�^����(RGB888�t�H�[�}�b�g)
										//                            5-�}�X�N�f�[�^�Ȃ�(RGB888�t�H�[�}�b�g)
	long	Xdpi;						// X Dot/Inch
	long	Ydpi;						// Y Dot/Inch
	long	Width;						// ���̃s�N�Z����
	long	Height;						// �c�̃s�N�Z����
	long	WidthByte;					// ���̃o�C�g��
	BOOL	Gray;						// TRUE:�O���[  FALSE:�O���[�łȂ�
	long	ImageSize;					// �C���[�W�T�C�Y
	short	ImageType;					// 0:�s�� 1:���X�^�C���[�W 2:WMF 3:EMF 4:DXF 5:SVG 6:SXF
} IKIMAGE_INFO;
typedef IKIMAGE_INFO * PTR_IKIMAGE_INFO;

/*******************************************************************************
 *                       �C���[�W�t�@�C�����p�\����                          *
 *******************************************************************************/
typedef struct {
	short	BitCount;					// �s�N�Z���^�r�b�g
	long	Xdpi;						// X Dot/Inch
	long	Ydpi;						// Y Dot/Inch
	long	Width;						// ���̃s�N�Z����
	long	Height;						// �c�̃s�N�Z����
	long	WidthByte;					// ���̃o�C�g��
	long	ImageSize;					// �C���[�W�T�C�Y
	short	FileType;					// �t�@�C���^�C�v
	short	Interlace;					// �C���^���[�X  �O�F�Ȃ�  �P�F����
	short	MaxPage;					// �C���[�W�y�[�W��
	long	FileSize;					// �t�@�C���T�C�Y
	char	Comment[2048];				// �R�����g
	WORD	CommentLen;					// �R�����g�̒���
	short	CreationTimeYear;			// �쐬����			�N
	short	CreationTimeMonth;			//					��
	short	CreationTimeDay;			//					��
	short	CreationTimeHour;			//					��
	short	CreationTimeMinute;			//					��
	short	CreationTimeSecond;			//					�b
	short	LastAccessTimeYear;			// �ŏI�A�N�Z�X���� �N
	short	LastAccessTimeMonth;		//					��
	short	LastAccessTimeDay;			//					��
	short	LastAccessTimeHour;			//					��
	short	LastAccessTimeMinute;		//					��
	short	LastAccessTimeSecond;		//					�b
	short	LastWriteTimeYear;			// �ŏI�X�V����		�N
	short	LastWriteTimeMonth;			//					��
	short	LastWriteTimeDay;			//					��
	short	LastWriteTimeHour;			//					��
	short	LastWriteTimeMinute;		//					��
	short	LastWriteTimeSecond;		//					�b
} IKFILE_INFO;
typedef IKFILE_INFO * PTR_IKFILE_INFO;

/*******************************************************************************
 *                            �X�L���i����p�\����                             *
 *******************************************************************************/
typedef struct {
	WORD	ProtocolMajor;				// TWAIN�̃��W���[�o�[�W����
	WORD	ProtocolMinor;				// TWAIN�̃}�C�i�[�o�[�W����
	char	Manufacturer[34];			// ���[�J��
	WORD	ManufacturerLen;			// ���[�J���̒���
	char	ProductFamily[34];			// ���i�t�@�~���[��
	WORD	ProductFamilyLen;			// ���i�t�@�~���[���̒���
	char	ProductName[34];			// ���i�t�@�~���[��
	WORD	ProductNameLen;				// ���i�t�@�~���[���̒���
	WORD	SourceMajor;				// �f�[�^�\�[�X�̃��W���[�o�[�W����
	WORD	SourceMinor;				// �f�[�^�\�[�X�̃}�C�i�[�o�[�W����
	char	SourceVersionInfo[34];		// �f�[�^�\�[�X�̃o�[�W�������
	WORD	SourceVersionInfoLen;		// �f�[�^�\�[�X�̃o�[�W�������̒���
} IKSCAN_DATASOURCEINFO;
typedef IKSCAN_DATASOURCEINFO * PTR_IKSCAN_DATASOURCEINFO;

typedef struct {
	WORD	seSize;						// IKSCAN_EXEC�\���̂̃T�C�Y
	WORD	PixelType;					// 0:BW 1:GRAY 2:RGB_COLOR 3:PALETTE_COLOR
	float	Left;						// ���̈ʒu(0�̓f�t�H���g�l���g�p) 
	float	Top;						// ��̈ʒu
	float	Right;						// �E�̈ʒu
	float	Bottom;						// ���̈ʒu
	long	XResolution;				// X������DPI(�ݒ肷��Ƃ��̒P�ʂ̓C���`)
	long	YResolution;				// Y������DPI(�ݒ肷��Ƃ��̒P�ʂ̓C���`)
	float	XScaling;					// X�����̃X�P�[���l
	float	YScaling;					// Y�����̃X�P�[���l
	WORD	BitDepth;					// ��f�r�b�g��
	short	Brightness;					// �u���C�g�l�X
	short	Contrast;					// �R���g���X�g(�ʓx�j
	float	Gamma;						// �K���}
	short	Highlight;					// �n�C���C�g
	short	Threshold;					// �X���b�V���z�[���h�i�������l�j
	short	Shadow;						// �V���h�E
	short	Indicator;					// �C���W�P�[�^�[�̕\��(0�ȊO�\��)
	short	ScanMode;					// ��荞�݃��[�h(�X�L���i 0:���e��,1:ADF�Ж�,2:ADF����,3-6:�f�W�J��)
	LPSTR	ScanDsName;					// �X�L�����f�o�C�X�̖���
	WORD	UiMode;						// 0:UI�\���|���Ȃ� 1:UI�\���|���� 2:UI��\��
	WORD	UnitMode;					// �ǂݎ��P�� 0:�C���` 1:cm  2:�p�C�J 3:�|�C���g 4:twip 5:�s�N�Z��
	WORD	UnitFlag;					// 0:�f�[�^�\�[�X 1:ImageKit���Ő���(UnitMode = 1,2,3,4,5���Ώ�)
	short	PageCount;					// ��荞�ݖ���(ADF�y�уf�W�J���̎��ɗL��)
	WORD	Orientation;				// ���� 0~3
	WORD	TransferMode;				// �]�����[�h 0:Ȳè��, 2:��ذ
	WORD	Compression;				// ���k���@ TransferMode = 2 �̏ꍇ�ɗL��
	WORD	PaperSize;					// �p���T�C�Y >= 0 (0�̏ꍇ��Left,Top,Right,Bottom���L��)
	WORD	BitDepthReduction;			// �F�[�x
	LPSTR	HalfTone;					// �n�[�t�g�[��
	WORD	FileFormat;					// �t�@�C���t�H�[�}�b�g
	short	JpegQuality;				// JPEG�i���W��
	LPSTR	FileName;					// �t�@�C����
	WORD	ImageFilter;				// �C���[�W�t�B���^�[
	WORD	NoiseFilter;				// �m�C�Y�t�B���^�[
	WORD	DropoutColor;				// �h���b�v�A�E�g�J���[
	WORD	BorderDetection;			// �̈�؂�o��
	WORD	Deskew;						// �X���␳
	WORD	ScanningSpeed;				// �ǂݎ�葬�x
	WORD	MoireFilter;				// ���A������
	WORD	Sharpness;					// �V���[�v�l�X
	WORD	RotateBack;					// ADF�̗��ʃX�L�������̗��ʉ�]
	WORD	ExtUiMode;					// �g��UI���[�h
	WORD	DynamicThreshold;			//�_�C�i�~�b�N�X���b�V���z�[���h
	float	ColorBWRatio;				// �J���[�Ɣ����̊��� (0.01�`50.00�j
	WORD	IgnoreBackColor;			// 0:�w�i�F�𖳎����Ȃ��A1:�w�i�F�𖳎�
} IKSCAN_EXEC;
typedef IKSCAN_EXEC * PTR_IKSCAN_EXEC;

typedef struct {
	float	Min;						// �ŏ��l
	float	Max;						// �ő�l
	float	Step;						// �X�e�b�v�l
	float	Default;					// �f�t�H���g�l
	float	Current;					// ���݂̐ݒ�l
} IKSCAN_RANGE;
typedef IKSCAN_RANGE * PTR_IKSCAN_RANGE;

typedef struct {
	float	XResolution;				// X�����̉𑜓x
	float	YResolution;				// Y�����̉𑜓x
	long	Width;						// �C���[�W�̕�
	long	Height;						// �C���[�W�̍���
	short	BitDepth;					// ��f�r�b�g��
	short	PixelType;					// �s�N�Z���^�C�v
	WORD	Compression;				// ���k���@
} IKSCAN_IMAGEINFO;
typedef IKSCAN_IMAGEINFO * PTR_IKSCAN_IMAGEINFO;

/*******************************************************************************
 *                             �v�����g�p�\����                                *
 *******************************************************************************/
typedef struct {
	short		CharSet;				// �����Z�b�g
	char		FontName[LF_FACESIZE];	// �t�H���g���� 32�o�C�g
	WORD		FontSize;				// �t�H���g�T�C�Y�i�|�C���g�j
	BYTE		FontBold;				// 0�ȊO�̂Ƃ��A�{�[���h�̂̃t�H���g
	BYTE		FontItalic;				// 0�ȊO�̂Ƃ��A�C�^���b�N�̂̃t�H���g
	BYTE		FontUnderline;			// 0�ȊO�̂Ƃ��A�����t���̃t�H���g<Direction��CharAngle�̒l�ɂ�薳���̏ꍇ����>
	BYTE		FontStrikeOut;			// 0�ȊO�̂Ƃ��A�ł��������t���̃t�H���g<Direction��CharAngle�̒l�ɂ�薳���̏ꍇ����>
	BYTE		Transparent;			// 0�ȊO�̂Ƃ��A���߁i�����j
	BYTE		Direction;				// ������̕��� 0:������E 1:�E���獶 2:�ォ�牺 3:�������
	short		CharAngle;				// �����̉�]�p�x 1�x�P��(�����v����) 0--360
	long		CharExtra;				// ������
	BYTE		HCentering;				// �����Z���^�����O 0:�� 1:���� 2:�E <IKDrawText�ŗL��>
	BYTE		VCentering;				// �����Z���^�����O 0:�� 1:���� 2:�� <IKDrawText�ŗL��>
	BYTE		RotateString;			// 0:�����̉�],0�ȊO:������̉�] <IKTextOut>
	COLORREF	TextColor1;				// �e�L�X�g�F1 (IK6�ł�TextForeColor)
	COLORREF	TextColor2;				// �e�L�X�g�F2 (IK6�ł�TextBackColor)
	BYTE		Alpha1;					// TextColor1�̃A���t�@ IKDrawString�ŗL��
	BYTE		Alpha2;					// TextColor2�̃A���t�@ IKDrawString�ŗL��
	WORD		HotkeyPrefix;			// IKDrawText 0:�v���t�B�b�N�X�Ȃ�,1:�v���t�B�b�N�X�\��
										// IKDrawString 0:�v���t�B�b�N�X�Ȃ�, 1:�v���t�B�b�N�X�\��, 2:�v���t�B�b�N�X��\��
} IKPRINT_TEXTINFO;
typedef IKPRINT_TEXTINFO * PTR_IKPRINT_TEXTINFO;

typedef struct {
	long		PenWidth;				// �y���̕� (�s�N�Z�� or  0.1mm�P��) <�P�ʂ�mm�̏ꍇ��Max 1cm>
	BYTE		PenStyle;				// 0:�k�� 1:����  2..6: 
	BYTE		PenMode;				// �y���̃��[�h 1..16:
	COLORREF	PenColor;				// �y���̕`��F
	BYTE		BrushStyle;				// 0:�k�� 1:SOLID 2..7:HATCH
	COLORREF	BrushColor;				// �u���V�̕`��F
	BYTE		Transparent;			// 0�ȊO�̂Ƃ��A���߁i�p�^�[���u���V�̏ꍇ�j
	COLORREF	BackColor;				// Transparent = 0, �p�^�[���u���V�̏ꍇ�ɗL��
} IKPRINT_DRAWINFO;
typedef IKPRINT_DRAWINFO * PTR_IKPRINT_DRAWINFO;

typedef struct {
	short		Orientation;			// ����̌��� 1:�c(DMORIENT_PORTRAIT) 2:��(DMORIENT_LANDSCAPE)
	short		PaperSize;				// �p���T�C�Y 8:A3(DMPAPER_A3) 9:A4(DMPAPER_A4)
	short		Zoom;					// �k�����鎞�̕S����(0~100)
	WORD		Copies;					// �������
	short		PaperBin;				// �p���g���C
	short		XResolution;			// �v�����^��X�����̉𑜓x
	short		YResolution;			// �v�����^��Y�����̉𑜓x
	short		ColorMode;				// 1:���m�N��,2:�J���[
	short		Duplex;					// 1~3
	BYTE		Collate;				// �m���P�ʂň���n0�ȊO:���P�ʂň��
	short		CustomPaperWidth;		// PaperSize=256(���[�U��`�T�C�Y)�̎��̗p���̉���(0.1mm�P��)
	short		CustomPaperHeight;		// PaperSize=256(���[�U��`�T�C�Y)�̎��̗p���̏c��(0.1mm�P��)
} IKPRINT_DEVMODEINFO;
typedef IKPRINT_DEVMODEINFO * PTR_IKPRINT_DEVMODEINFO;

typedef struct {
	BYTE		Collate;				// �m���P�ʂň���n0�ȊO:���P�ʂň���Ƀ`�F�b�N
	WORD		Copies;					// �������
	WORD		FromPage;				// ����J�n�y�[�W
	WORD		ToPage;					// ����I���y�[�W
	WORD		MaxPage;				// ����ő�y�[�W
	WORD		MinPage;				// ����ŏ��y�[�W
	DWORD		Options;				// �m����n�_�C�A���O�̕\���Ɠ���̎w��
	BYTE		PrintRange;				// ����͈͂̎�� 0:���ׂ�,1:�I����������,2:�y�[�W�w��
	BYTE		PrintToFile;			// �m�t�@�C���֏o�́n0�ȊO:�t�@�C���֏o�͂Ƀ`�F�b�N
} IKPRINT_DIALOG;
typedef IKPRINT_DIALOG * PTR_IKPRINT_DIALOG;

/*******************************************************************************
 *                           �G�t�F�N�g�����p�\����                            *
 *******************************************************************************/
typedef struct {
	HANDLE		hImgBmh;				// �C���[�W�n���h��
	HANDLE		hMskBmh;				// �}�X�N�C���[�W�n���h��
} IKSELECT_IMAGE;
typedef IKSELECT_IMAGE * PTR_IKSELECT_IMAGE;

typedef struct {
	BYTE	Pal;						// �ݒ�E�擾�����p���b�g�ԍ�
	BYTE	Red;						// �ݒ�E�擾�����ԐF
	BYTE	Green;						// �ݒ�E�擾�����ΐF
	BYTE	Blue;						// �ݒ�E�擾�����F
	char	Reserved[128];				// �c�h�a�A�N�Z�X���
} IKDIB_INFO;
typedef IKDIB_INFO * PTR_IKDIB_INFO;

/*******************************************************************************
 *                               Exif���p�\����                              *
 *******************************************************************************/
// ��摜�A�T���l�C���摜 ���
typedef struct {
	unsigned short	Compression;						// ���k�̎�ށi��摜��JPEG�̏ꍇ�̓Z�b�g����Ȃ��j
	unsigned short	Orientation;						// �摜����
	unsigned short	YCbCrPositioning;					// YCC �̉�f�\���iY ��C �̈ʒu�j
	unsigned long	XResolution[2];						// �摜�̕��̉𑜓x
	unsigned long	YResolution[2];						// �摜�̍����̉𑜓x
	unsigned short	ResolutionUnit;						// �摜�̕��ƍ����̉𑜓x�̒P��
	char			DateTime[20];						// �t�@�C���ύX����
	char			ImageDescription[1024];				// �摜�^�C�g��
	WORD			ImageDescriptionLen;				// �摜�^�C�g���̒���
	char			Make[1024];							// �摜���͋@��̃��[�J�[��
	WORD			MakeLen;							// �摜���͋@��̃��[�J�[���̒���
	char			Model[1024];						// �摜���͋@��̃��f����
	WORD			ModelLen;							// �摜���͋@��̃��f�����̒���
	char			Software[1024];						// �g�p�\�t�g�E�F�A��
	WORD			SoftwareLen;						// �g�p�\�t�g�E�F�A���̒���
	char			Artist[1024];						// ��Җ�
	WORD			ArtistLen;							// ��Җ��̒���
	char			Copyright[1024];					// �B�e���쌠��/�ҏW���쌠��
	WORD			CopyrightLen;						// �B�e���쌠��/�ҏW���쌠�҂̒���
	HANDLE			ThumbImageHandle;					// �T���l�C���摜�̃n���h���i�T���l�C���̂݁j
} IMAGE_TAG;
typedef IMAGE_TAG * PTR_IMAGE_TAG;

// Exif ���
typedef struct {
	// A �o�[�W�����Ɋւ���^�O
	char			Exif_Version[5];					// Exif �o�[�W�����i��F"0220"�j
	char			FlashPix_Version[5];				// FlashPix �o�[�W�����i��F"0100"�j
	// B �摜�f�[�^�̓����Ɋւ���^�O
	unsigned short	ColorSpace;							// �F��ԏ��
	// C �摜�f�[�^�̍\���Ɋւ���^�O
	unsigned long	PixelXDimension;					// �����摜��
	unsigned long	PixelYDimension;					// �����摜����
	char			ComponentsConfiguration[5];			// �e�R���|�[�l���g�̈Ӗ�
	unsigned long	CompressedBitsPerPixel[2];			// �摜���k���[�h
	// D ���[�U���Ɋւ���^�O
	char			MakerNote[1024];					// ���[�J�[�m�[�g
	WORD			MakerNoteLen;						// ���[�J�[�m�[�g�̒���
	char			UserComment[1024];					// ���[�U�[�R�����g
	WORD			UserCommentLen;						// ���[�U�[�R�����g�̒���
	char			UserCommentID[8];					// ���[�U�[�R�����g���ʃR�[�h
	// F ���t�Ɋւ���^�O
	char			DateTimeOriginal[20];				// ���摜�f�[�^�̐�������
	char			DateTimeDigitized[20];				// �f�B�W�^���f�[�^�̍쐬����
	char			SubSecTime[10];						// DateTime �̃T�u�Z�b�N
	char			SubSecTimeOriginal[10];				// DateTimeOriginal �̃T�u�Z�b�N
	char			SubSecTimeDigitized[10];			// DateTimeDigitized �̃T�u�Z�b�N
	// G �B�e�����Ɋւ���^�O
	unsigned long	ExposureTime[2];					// �I�o����
	long			ShutterSpeedValue[2];				// �V���b�^�[�X�s�[�h
	unsigned long	ApertureValue[2];					// �i��l
	long			BrightnessValue[2];					// �P�x�l
	long			ExposureBiasValue[2];				// �I���␳�l
	unsigned long	MaxApertureValue[2];				// �����Y�ŏ��e�l
	unsigned long	SubjectDistance[2];					// ��ʑ̋���
	unsigned short	MeteringMode;						// ��������
	unsigned short	LightSource;						// ����
	unsigned short	Flash;								// �t���b�V��
	unsigned long	FocalLength[2];						// �����Y�œ_����
	unsigned long	FNumber[2];							// F �i���o�[
	unsigned short	ExposureProgram;					// �I�o�v���O����
	char			SpectralSensitivity[1024];			// �X�y�N�g�����x
	WORD			SpectralSensitivityLen;				// �X�y�N�g�����x�̒���
	unsigned short	ISOSpeedRatings;					// ISO �X�s�[�h���[�g
	unsigned long	FlashEnergy[2];						// �t���b�V�����x
	unsigned long	FocalPlaneXResolution[2];			// �œ_�ʂ̕��̉𑜓x
	unsigned long	FocalPlaneYResolution[2];			// �œ_�ʂ̍����̉𑜓x
	unsigned short	FocalPlaneResolutionUnit;			// �œ_�ʉ𑜓x�P��
	unsigned short	SubjectLocationX;					// ��ʑ̈ʒu�iX���W�l�j
	unsigned short	SubjectLocationY;					// ��ʑ̈ʒu�iY���W�l�j
	unsigned long	ExposureIndex[2];					// �I�o�C���f�b�N�X
	unsigned short	SensingMethod;						// �Z���T�[����
	BYTE			FileSource;							// �t�@�C���\�[�X
	BYTE			SceneType;							// �V�[���^�C�v
} EXIF_TAG;
typedef EXIF_TAG * PTR_EXIF_TAG;

// GPS ���
typedef struct {
	char			GPSVersion[5];						// GPS �^�O�o�[�W�����i��F"2000"�j
	char			MapDatum[16];						// ���ʂ̗p�����n�}�f�[�^
	char			Latitude[2];						// "N":�k��, 'S":���
	unsigned long	LatitudeD[2];						// �x
	unsigned long	LatitudeM[2];						// ��
	unsigned long	LatitudeS[2];						// �b
	char			Longitude[2];						// "E":���o, "W":���o
	unsigned long	LongitudeD[2];						// �x
	unsigned long	LongitudeM[2];						// ��
	unsigned long	LongitudeS[2];						// �b
	unsigned long	Altitude[2];						// ���x�i���j
} GPS_TAG;
typedef GPS_TAG * PTR_GPS_TAG;

typedef struct {
	IMAGE_TAG		MainImageInfo;						// ��摜���
	IMAGE_TAG		ThumbImageInfo;						// �T���l�C���摜���
	EXIF_TAG		ExifInfo;							// Exif���
	GPS_TAG			GPSInfo;							// GPS���
} EXIF_INFO;
typedef EXIF_INFO * PTR_EXIF_INFO;

#pragma pack (pop)

/*******************************************************************************
 *                               �R�[���o�b�N�֐�                              *
 *******************************************************************************/
// �t�@�C���̓ǂݏ�����G�t�F�N�g�������ɌĂяo����郆�[�U�֐��̏���
typedef BOOL (FAR PASCAL *IKPROCESSPROC)(short Percent);

// �����̓ǂݍ��ݒ��ɌĂяo�����R�[���o�b�N�֐��̏���
typedef BOOL (FAR PASCAL *IK_BEFORESCANPROC)(PTR_IKSCAN_IMAGEINFO ImageInfo, long Count);
typedef BOOL (FAR PASCAL *IK_SCANNINGPROC)(short Percent);
typedef BOOL (FAR PASCAL *IK_AFTERSCANPROC)(HANDLE DibHandle, HANDLE OrgHandle, long Count, short BitOrder);

/*******************************************************************************
 *                                 �֐��̐錾                                  *
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
