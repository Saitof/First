/****************************************************************************************
 *    ImageKit7    Ver. 1.2.0                                                           *
 *                                                                                      *
 *    for C, C++   DLLで使用                                                            *
 *                                                                                      *
 *    <Imagekit7Def.h>                                               (株)ニュートン     *
 *                                                                                      *
 *  使用方法                                                                            *
 *    C, C++ で DLL を使用するプログラムを作成する場合、                                *
 *    本ファイルを下記のようにインクルードして下さい。                                  *
 *                                                                                      *
 *    メインとなるソースプログラムにインクルードする時は、                              *
 *           #include "Imagekit7.h"       <-- ImageKit7ヘッダファイル                   *
 *           #define  _MAIN_IK7_PROGRAM_  <-- ここでは、これが必要                      *
 *           #define  _IK7FILE_           <-- ファイルモジュールを使用する時            *
 *           #define  _IK7SCAN_           <-- スキャナモジュールを使用する時            *
 *           #define  _IK7PRINT_          <-- プリントモジュールを使用する時            *
 *           #define  _IK7EFFECT_         <-- エフェクトモジュールを使用する時          *
 *           #define  _IK7VECT_           <-- ベクトルデータを使用する時                *
 *           #define  _IK7RAS2VECT_       <-- ラスタデータのベクトル化処理を使用する時  *
 *           #include "Imagekit7Def.h"    <-- 本ヘッダファイル                          *
 *                                                                                      *
 *    処理の最初に LoadImageKitDll(lpDllPath) をコールして下さい。                      *
 *    処理の最後に UnloadImageKitDll()                                                  *
 *                                                                                      *
 *                                                                                      *
 *    その他のソースプログラムにインクルードする時は、                                  *
 *           #include "Imagekit7.h"       <-- ImageKit7ヘッダファイル                   *
 *           #define  _IK7FILE_           <-- ファイルモジュールを使用する時            *
 *           #define  _IK7SCAN_           <-- スキャナモジュールを使用する時            *
 *           #define  _IK7PRINT_          <-- プリントモジュールを使用する時            *
 *           #define  _IK7EFFECT_         <-- エフェクトモジュールを使用する時          *
 *           #define  _IK7VECT_           <-- ベクトルデータを使用する時                *
 *           #define  _IK7RAS2VECT_       <-- ラスタデータのベクトル化処理を使用する時  *
 *           #include "Imagekit7Def.h"    <-- 本ヘッダファイル                          *
 ****************************************************************************************/
#ifndef IMAGEKIT7_DEF_INCLUDE
#define IMAGEKIT7_DEF_INCLUDE

#define	IKCOM_MODULE		"IK7COM.DLL"
#define	IKFILE_MODULE		"IK7FILE.DLL"
#define	IKSCAN_MODULE		"IK7SCAN.DLL"
#define	IKPRINT_MODULE		"IK7PRINT.DLL"
#define	IKEFFECT_MODULE		"IK7EFFECT.DLL"
#define	IKVECT_MODULE		"IK7VECTCOM.DLL"
#define	IKRAS2VECT_MODULE	"IK7RASTOVECT.DLL"

#ifdef _MAIN_IK7_PROGRAM_
	#define _EXTERN_
#else
	#define _EXTERN_ extern
#endif

_EXTERN_ HINSTANCE hInstIKCOM;
#ifdef _IK7FILE_
_EXTERN_ HINSTANCE hInstIKFILE;
#endif
#ifdef _IK7SCAN_
_EXTERN_ HINSTANCE hInstIKSCAN;
#endif
#ifdef _IK7PRINT_
_EXTERN_ HINSTANCE hInstIKPRINT;
#endif
#ifdef _IK7EFFECT_
_EXTERN_ HINSTANCE hInstIKEFFECT;
#endif
#ifdef _IK7VECT_
_EXTERN_ HINSTANCE hInstIKVectCOM;
#endif
#ifdef _IK7RAS2VECT_
_EXTERN_ HINSTANCE hInstIKRasterToVector;
#endif

// IK7COM.DLL
_EXTERN_ PROCIKBITMAPFROMDIB			lpFuncIKBitmapFromDib;
_EXTERN_ PROCIKCLEARCLIPBRD				lpFuncIKClearClipBrd;
_EXTERN_ PROCIKCOPYIMAGE				lpFuncIKCopyImage;
_EXTERN_ PROCIKBITBLT					lpFuncIKBitBlt;
_EXTERN_ PROCIKSTRETCHBLT				lpFuncIKStretchBlt;
_EXTERN_ PROCIKCREATEIMAGE				lpFuncIKCreateImage;
_EXTERN_ PROCIKDELETEBITMAPOBJECT		lpFuncIKDeleteBitmapObject;
_EXTERN_ PROCIKDIBFROMBITMAP			lpFuncIKDibFromBitmap;
_EXTERN_ PROCIKFREEMEMORY				lpFuncIKFreeMemory;
_EXTERN_ PROCIKGETDPI					lpFuncIKGetDpi;
_EXTERN_ PROCIKGETDPIFROMHDC			lpFuncIKGetDpiFromHdc;
_EXTERN_ PROCIKGETERRORSTATUS			lpFuncIKGetErrorStatus;
_EXTERN_ PROCIKGETFROMCLIPBRD			lpFuncIKGetFromClipBrd;
_EXTERN_ PROCIKGETIMAGETYPE				lpFuncIKGetImageType;
_EXTERN_ PROCIKGETMEMORYSIZE			lpFuncIKGetMemorySize;
_EXTERN_ PROCIKGETONEBITPALCOUNT		lpFuncIKGetOneBitPalCount;
_EXTERN_ PROCIKGETPALETTE				lpFuncIKGetPalette;
_EXTERN_ PROCIKGETSYSTEMPALETTE			lpFuncIKGetSystemPalette;
_EXTERN_ PROCIKISCLIPBRDDATA			lpFuncIKIsClipBrdData;
_EXTERN_ PROCIKRESIZEREFINE1BITIMAGE	lpFuncIKResizeRefine1BitImage;
_EXTERN_ PROCIKSETDPI					lpFuncIKSetDpi;
_EXTERN_ PROCIKSETPALETTE				lpFuncIKSetPalette;
_EXTERN_ PROCIKSETTOCLIPBRD				lpFuncIKSetToClipBrd;
_EXTERN_ PROCIKGETDPIF					lpFuncIKGetDpiF;
_EXTERN_ PROCIKSETDPIF					lpFuncIKSetDpiF;

#define IKBitmapFromDib					lpFuncIKBitmapFromDib
#define IKClearClipBrd					lpFuncIKClearClipBrd
#define IKCopyImage						lpFuncIKCopyImage
#define IKBitBlt						lpFuncIKBitBlt
#define IKStretchBlt					lpFuncIKStretchBlt
#define IKCreateImage					lpFuncIKCreateImage
#define IKDeleteBitmapObject			lpFuncIKDeleteBitmapObject
#define IKDibFromBitmap					lpFuncIKDibFromBitmap
#define IKFreeMemory					lpFuncIKFreeMemory
#define IKGetDpi						lpFuncIKGetDpi
#define IKGetDpiFromHdc					lpFuncIKGetDpiFromHdc
#define IKGetErrorStatus				lpFuncIKGetErrorStatus
#define IKGetFromClipBrd				lpFuncIKGetFromClipBrd
#define IKGetImageType					lpFuncIKGetImageType
#define IKGetMemorySize					lpFuncIKGetMemorySize
#define IKGetOneBitPalCount				lpFuncIKGetOneBitPalCount
#define IKGetPalette					lpFuncIKGetPalette
#define IKGetSystemPalette				lpFuncIKGetSystemPalette
#define IKIsClipBrdData					lpFuncIKIsClipBrdData
#define IKResizeRefine1BitImage			lpFuncIKResizeRefine1BitImage
#define IKSetDpi						lpFuncIKSetDpi
#define IKSetPalette					lpFuncIKSetPalette
#define IKSetToClipBrd					lpFuncIKSetToClipBrd
#define IKGetDpiF						lpFuncIKGetDpiF
#define IKSetDpiF						lpFuncIKSetDpiF

// IK7FILE.DLL
#ifdef _IK7FILE_
_EXTERN_ PROCIKFILELOADASRAWDATA		lpFuncIKFileLoadAsRawData;
_EXTERN_ PROCIKFILESAVEASRAWDATA		lpFuncIKFileSaveAsRawData;
_EXTERN_ PROCIKFILELOAD					lpFuncIKFileLoad;
_EXTERN_ PROCIKFILETYPE					lpFuncIKFileType;
_EXTERN_ PROCIKOPENFILEDIALOG			lpFuncIKOpenFileDialog;
_EXTERN_ PROCIKOPENFILEDLG				lpFuncIKOpenFileDlg;
_EXTERN_ PROCIKSAVEFILEDIALOG			lpFuncIKSaveFileDialog;
_EXTERN_ PROCIKSAVEFILEDLG				lpFuncIKSaveFileDlg;
_EXTERN_ PROCIKSAVEFILEDLGEX			lpFuncIKSaveFileDlgEx;
_EXTERN_ PROCIKFTPGETFILE				lpFuncIKFTPGetFile;
_EXTERN_ PROCIKFTPPUTFILE				lpFuncIKFTPPutFile;
_EXTERN_ PROCIKFTPDELETEFILE			lpFuncIKFTPDeleteFile;
_EXTERN_ PROCIKFTPRENAMEFILE			lpFuncIKFTPRenameFile;
_EXTERN_ PROCIKHTTPGETFILE				lpFuncIKHTTPGetFile;
_EXTERN_ PROCIKHTTPPUTFILE				lpFuncIKHTTPPutFile;
_EXTERN_ PROCIKBMPFILELOAD				lpFuncIKBmpFileLoad;
_EXTERN_ PROCIKBMPFILESAVE				lpFuncIKBmpFileSave;
_EXTERN_ PROCIKJPEGFILELOAD				lpFuncIKJpegFileLoad;
_EXTERN_ PROCIKJPEGFILESAVE				lpFuncIKJpegFileSave;
_EXTERN_ PROCIKJPEGEXIFINFO				lpFuncIKJpegExifInfo;
_EXTERN_ PROCIKGIFFILELOAD				lpFuncIKGifFileLoad;
_EXTERN_ PROCIKGIFFILESAVE				lpFuncIKGifFileSave;
_EXTERN_ PROCIKTIFFFILELOAD				lpFuncIKTiffFileLoad;
_EXTERN_ PROCIKTIFFFILESAVE				lpFuncIKTiffFileSave;
_EXTERN_ PROCIKPNGFILELOAD				lpFuncIKPngFileLoad;
_EXTERN_ PROCIKPNGFILELOADEX			lpFuncIKPngFileLoadEx;
_EXTERN_ PROCIKPNGFILESAVE				lpFuncIKPngFileSave;
_EXTERN_ PROCIKPNGFILESAVEEX			lpFuncIKPngFileSaveEx;
_EXTERN_ PROCIKFPXFILELOAD				lpFuncIKFpxFileLoad;
_EXTERN_ PROCIKFPXFILESAVE				lpFuncIKFpxFileSave;
_EXTERN_ PROCIKPCXFILELOAD				lpFuncIKPcxFileLoad;
_EXTERN_ PROCIKPCXFILESAVE				lpFuncIKPcxFileSave;
_EXTERN_ PROCIKWMFFILELOAD				lpFuncIKWmfFileLoad;
_EXTERN_ PROCIKWMFFILESAVE				lpFuncIKWmfFileSave;
_EXTERN_ PROCIKEMFFILELOAD				lpFuncIKEmfFileLoad;
_EXTERN_ PROCIKEMFFILESAVE				lpFuncIKEmfFileSave;
_EXTERN_ PROCIKDXFFILELOAD				lpFuncIKDxfFileLoad;
_EXTERN_ PROCIKDXFFILESAVE				lpFuncIKDxfFileSave;
_EXTERN_ PROCIKSVGFILELOAD				lpFuncIKSvgFileLoad;
_EXTERN_ PROCIKSVGFILESAVE				lpFuncIKSvgFileSave;
_EXTERN_ PROCIKJ2KFILELOAD				lpFuncIKJ2kFileLoad;
_EXTERN_ PROCIKJ2KFILESAVE				lpFuncIKJ2kFileSave;
_EXTERN_ PROCIKSXFP21FILELOAD			lpFuncIKSxfP21FileLoad;
_EXTERN_ PROCIKSXFP21FILESAVE			lpFuncIKSxfP21FileSave;
_EXTERN_ PROCIKSXFSFCFILELOAD			lpFuncIKSxfSfcFileLoad;
_EXTERN_ PROCIKSXFSFCFILESAVE			lpFuncIKSxfSfcFileSave;

_EXTERN_ PROCIKRGBBMPPLANEFILELOAD		lpFuncIKRGBBmpPlaneFileLoad;
_EXTERN_ PROCIKRGBBMPPLANEFILESAVE		lpFuncIKRGBBmpPlaneFileSave;
_EXTERN_ PROCIKCMYKBMPPLANEFILELOAD		lpFuncIKCMYKBmpPlaneFileLoad;
_EXTERN_ PROCIKCMYKBMPPLANEFILESAVE		lpFuncIKCMYKBmpPlaneFileSave;
_EXTERN_ PROCIKYCCBMPPLANEFILELOAD		lpFuncIKYCCBmpPlaneFileLoad;
_EXTERN_ PROCIKYCCBMPPLANEFILESAVE		lpFuncIKYCCBmpPlaneFileSave;

_EXTERN_ PROCIKFILELOADMEM				lpFuncIKFileLoadMem;
_EXTERN_ PROCIKFILETYPEMEM				lpFuncIKFileTypeMem;
_EXTERN_ PROCIKBMPFILELOADMEM			lpFuncIKBmpFileLoadMem;
_EXTERN_ PROCIKBMPFILESAVEMEM			lpFuncIKBmpFileSaveMem;
_EXTERN_ PROCIKJPEGFILELOADMEM			lpFuncIKJpegFileLoadMem;
_EXTERN_ PROCIKJPEGFILESAVEMEM			lpFuncIKJpegFileSaveMem;
_EXTERN_ PROCIKJPEGEXIFINFOMEM			lpFuncIKJpegExifInfoMem;
_EXTERN_ PROCIKGIFFILELOADMEM			lpFuncIKGifFileLoadMem;
_EXTERN_ PROCIKGIFFILESAVEMEM			lpFuncIKGifFileSaveMem;
_EXTERN_ PROCIKTIFFFILELOADMEM			lpFuncIKTiffFileLoadMem;
_EXTERN_ PROCIKTIFFFILESAVEMEM			lpFuncIKTiffFileSaveMem;
_EXTERN_ PROCIKPNGFILELOADMEM			lpFuncIKPngFileLoadMem;
_EXTERN_ PROCIKPNGFILELOADEXMEM			lpFuncIKPngFileLoadExMem;
_EXTERN_ PROCIKPNGFILESAVEMEM			lpFuncIKPngFileSaveMem;
_EXTERN_ PROCIKPNGFILESAVEEXMEM			lpFuncIKPngFileSaveExMem;
_EXTERN_ PROCIKFPXFILELOADMEM			lpFuncIKFpxFileLoadMem;
_EXTERN_ PROCIKFPXFILESAVEMEM			lpFuncIKFpxFileSaveMem;
_EXTERN_ PROCIKPCXFILELOADMEM			lpFuncIKPcxFileLoadMem;
_EXTERN_ PROCIKPCXFILESAVEMEM			lpFuncIKPcxFileSaveMem;
_EXTERN_ PROCIKWMFFILELOADMEM			lpFuncIKWmfFileLoadMem;
_EXTERN_ PROCIKWMFFILESAVEMEM			lpFuncIKWmfFileSaveMem;
_EXTERN_ PROCIKEMFFILELOADMEM			lpFuncIKEmfFileLoadMem;
_EXTERN_ PROCIKEMFFILESAVEMEM			lpFuncIKEmfFileSaveMem;
_EXTERN_ PROCIKDXFFILELOADMEM			lpFuncIKDxfFileLoadMem;
_EXTERN_ PROCIKDXFFILESAVEMEM			lpFuncIKDxfFileSaveMem;
_EXTERN_ PROCIKSVGFILELOADMEM			lpFuncIKSvgFileLoadMem;
_EXTERN_ PROCIKSVGFILESAVEMEM			lpFuncIKSvgFileSaveMem;
_EXTERN_ PROCIKJ2KFILELOADMEM			lpFuncIKJ2kFileLoadMem;
_EXTERN_ PROCIKJ2KFILESAVEMEM			lpFuncIKJ2kFileSaveMem;
_EXTERN_ PROCIKSXFP21FILELOADMEM		lpFuncIKSxfP21FileLoadMem;
_EXTERN_ PROCIKSXFP21FILESAVEMEM		lpFuncIKSxfP21FileSaveMem;
_EXTERN_ PROCIKSXFSFCFILELOADMEM		lpFuncIKSxfSfcFileLoadMem;
_EXTERN_ PROCIKSXFSFCFILESAVEMEM		lpFuncIKSxfSfcFileSaveMem;

_EXTERN_ PROCIKFTPCONNECT				lpFuncIKFTPConnect;
_EXTERN_ PROCIKFTPDISCONNECT			lpFuncIKFTPDisconnect;
_EXTERN_ PROCIKFTPGETFILEEX				lpFuncIKFTPGetFileEx;
_EXTERN_ PROCIKFTPPUTFILEEX				lpFuncIKFTPPutFileEx;
_EXTERN_ PROCIKFTPDELETEFILEEX			lpFuncIKFTPDeleteFileEx;
_EXTERN_ PROCIKFTPRENAMEFILEEX			lpFuncIKFTPRenameFileEx;
_EXTERN_ PROCIKHTTPCONNECT				lpFuncIKHTTPConnect;
_EXTERN_ PROCIKHTTPDISCONNECT			lpFuncIKHTTPDisconnect;
_EXTERN_ PROCIKHTTPGETFILEEX			lpFuncIKHTTPGetFileEx;
_EXTERN_ PROCIKHTTPPUTFILEEX			lpFuncIKHTTPPutFileEx;

#define IKFileLoadAsRawData				lpFuncIKFileLoadAsRawData
#define IKFileSaveAsRawData				lpFuncIKFileSaveAsRawData
#define IKFileLoad						lpFuncIKFileLoad
#define IKFileType						lpFuncIKFileType
#define IKOpenFileDialog				lpFuncIKOpenFileDialog
#define IKOpenFileDlg					lpFuncIKOpenFileDlg
#define IKSaveFileDialog				lpFuncIKSaveFileDialog
#define IKSaveFileDlg					lpFuncIKSaveFileDlg
#define IKSaveFileDlgEx					lpFuncIKSaveFileDlgEx
#define IKFTPGetFile					lpFuncIKFTPGetFile
#define IKFTPPutFile					lpFuncIKFTPPutFile
#define IKFTPDeleteFile					lpFuncIKFTPDeleteFile
#define IKFTPRenameFile					lpFuncIKFTPRenameFile
#define IKHTTPGetFile					lpFuncIKHTTPGetFile
#define IKHTTPPutFile					lpFuncIKHTTPPutFile
#define IKBmpFileLoad					lpFuncIKBmpFileLoad
#define IKBmpFileSave					lpFuncIKBmpFileSave
#define IKJpegFileLoad					lpFuncIKJpegFileLoad
#define IKJpegFileSave					lpFuncIKJpegFileSave
#define IKJpegExifInfo					lpFuncIKJpegExifInfo
#define IKGifFileLoad					lpFuncIKGifFileLoad
#define IKGifFileSave					lpFuncIKGifFileSave
#define IKTiffFileLoad					lpFuncIKTiffFileLoad
#define IKTiffFileSave					lpFuncIKTiffFileSave
#define IKPngFileLoad					lpFuncIKPngFileLoad
#define IKPngFileLoadEx					lpFuncIKPngFileLoadEx
#define IKPngFileSave					lpFuncIKPngFileSave
#define IKPngFileSaveEx					lpFuncIKPngFileSaveEx
#define IKFpxFileLoad					lpFuncIKFpxFileLoad
#define IKFpxFileSave					lpFuncIKFpxFileSave
#define IKPcxFileLoad					lpFuncIKPcxFileLoad
#define IKPcxFileSave					lpFuncIKPcxFileSave
#define IKWmfFileLoad					lpFuncIKWmfFileLoad
#define IKWmfFileSave					lpFuncIKWmfFileSave
#define IKEmfFileLoad					lpFuncIKEmfFileLoad
#define IKEmfFileSave					lpFuncIKEmfFileSave
#define IKDxfFileLoad					lpFuncIKDxfFileLoad
#define IKDxfFileSave					lpFuncIKDxfFileSave
#define IKSvgFileLoad					lpFuncIKSvgFileLoad
#define IKSvgFileSave					lpFuncIKSvgFileSave
#define IKJ2kFileLoad					lpFuncIKJ2kFileLoad
#define IKJ2kFileSave					lpFuncIKJ2kFileSave
#define IKSxfP21FileLoad				lpFuncIKSxfP21FileLoad
#define IKSxfP21FileSave				lpFuncIKSxfP21FileSave
#define IKSxfSfcFileLoad				lpFuncIKSxfSfcFileLoad
#define IKSxfSfcFileSave				lpFuncIKSxfSfcFileSave

#define IKRGBBmpPlaneFileLoad			lpFuncIKRGBBmpPlaneFileLoad
#define IKRGBBmpPlaneFileSave			lpFuncIKRGBBmpPlaneFileSave
#define IKCMYKBmpPlaneFileLoad			lpFuncIKCMYKBmpPlaneFileLoad
#define IKCMYKBmpPlaneFileSave			lpFuncIKCMYKBmpPlaneFileSave
#define IKYCCBmpPlaneFileLoad			lpFuncIKYCCBmpPlaneFileLoad
#define IKYCCBmpPlaneFileSave			lpFuncIKYCCBmpPlaneFileSave

#define IKFileLoadMem					lpFuncIKFileLoadMem
#define IKFileTypeMem					lpFuncIKFileTypeMem
#define IKBmpFileLoadMem				lpFuncIKBmpFileLoadMem
#define IKBmpFileSaveMem				lpFuncIKBmpFileSaveMem
#define IKJpegFileLoadMem				lpFuncIKJpegFileLoadMem
#define IKJpegFileSaveMem				lpFuncIKJpegFileSaveMem
#define IKJpegExifInfoMem				lpFuncIKJpegExifInfoMem
#define IKGifFileLoadMem				lpFuncIKGifFileLoadMem
#define IKGifFileSaveMem				lpFuncIKGifFileSaveMem
#define IKTiffFileLoadMem				lpFuncIKTiffFileLoadMem
#define IKTiffFileSaveMem				lpFuncIKTiffFileSaveMem
#define IKPngFileLoadMem				lpFuncIKPngFileLoadMem
#define IKPngFileLoadExMem				lpFuncIKPngFileLoadExMem
#define IKPngFileSaveMem				lpFuncIKPngFileSaveMem
#define IKPngFileSaveExMem				lpFuncIKPngFileSaveExMem
#define IKFpxFileLoadMem				lpFuncIKFpxFileLoadMem
#define IKFpxFileSaveMem				lpFuncIKFpxFileSaveMem
#define IKPcxFileLoadMem				lpFuncIKPcxFileLoadMem
#define IKPcxFileSaveMem				lpFuncIKPcxFileSaveMem
#define IKWmfFileLoadMem				lpFuncIKWmfFileLoadMem
#define IKWmfFileSaveMem				lpFuncIKWmfFileSaveMem
#define IKEmfFileLoadMem				lpFuncIKEmfFileLoadMem
#define IKEmfFileSaveMem				lpFuncIKEmfFileSaveMem
#define IKDxfFileLoadMem				lpFuncIKDxfFileLoadMem
#define IKDxfFileSaveMem				lpFuncIKDxfFileSaveMem
#define IKSvgFileLoadMem				lpFuncIKSvgFileLoadMem
#define IKSvgFileSaveMem				lpFuncIKSvgFileSaveMem
#define IKJ2kFileLoadMem				lpFuncIKJ2kFileLoadMem
#define IKJ2kFileSaveMem				lpFuncIKJ2kFileSaveMem
#define IKSxfP21FileLoadMem				lpFuncIKSxfP21FileLoadMem
#define IKSxfP21FileSaveMem				lpFuncIKSxfP21FileSaveMem
#define IKSxfSfcFileLoadMem				lpFuncIKSxfSfcFileLoadMem
#define IKSxfSfcFileSaveMem				lpFuncIKSxfSfcFileSaveMem

#define IKFTPConnect					lpFuncIKFTPConnect
#define IKFTPDisconnect					lpFuncIKFTPDisconnect
#define IKFTPGetFileEx					lpFuncIKFTPGetFileEx
#define IKFTPPutFileEx					lpFuncIKFTPPutFileEx
#define IKFTPDeleteFileEx				lpFuncIKFTPDeleteFileEx
#define IKFTPRenameFileEx				lpFuncIKFTPRenameFileEx
#define IKHTTPConnect					lpFuncIKHTTPConnect
#define IKHTTPDisconnect				lpFuncIKHTTPDisconnect
#define IKHTTPGetFileEx					lpFuncIKHTTPGetFileEx
#define IKHTTPPutFileEx					lpFuncIKHTTPPutFileEx
#endif

// IK7SCAN.DLL
#ifdef _IK7SCAN_
_EXTERN_ PROCIKSCANCLOSEDS				lpFuncIKScanCloseDS;
_EXTERN_ PROCIKSCANEXEC					lpFuncIKScanExec;
_EXTERN_ PROCIKSCANGETBITDEPTH			lpFuncIKScanGetBitDepth;
_EXTERN_ PROCIKSCANGETCAPENUM			lpFuncIKScanGetCapEnum;
_EXTERN_ PROCIKSCANGETCAPRANGE			lpFuncIKScanGetCapRange;
_EXTERN_ PROCIKSCANGETDSINFO			lpFuncIKScanGetDSInfo;
_EXTERN_ PROCIKSCANGETMINIMUMSIZE		lpFuncIKScanGetMinimumSize;
_EXTERN_ PROCIKSCANGETPHYSICALSIZE		lpFuncIKScanGetPhysicalSize;
_EXTERN_ PROCIKSCANGETPIXELTYPE			lpFuncIKScanGetPixelType;
_EXTERN_ PROCIKSCANINITIALIZE			lpFuncIKScanInitialize;
_EXTERN_ PROCIKSCANISCAPSUPPORTED		lpFuncIKScanIsCapSupported;
_EXTERN_ PROCIKSCANLIST					lpFuncIKScanList;
_EXTERN_ PROCIKSCANLISTLEN				lpFuncIKScanListLen;
_EXTERN_ PROCIKSCANOPENDS				lpFuncIKScanOpenDS;
_EXTERN_ PROCIKSCANSELECT				lpFuncIKScanSelect;
_EXTERN_ PROCIKSCANTERMINATE			lpFuncIKScanTerminate;

#define IKScanCloseDS					lpFuncIKScanCloseDS
#define IKScanExec						lpFuncIKScanExec
#define IKScanGetBitDepth				lpFuncIKScanGetBitDepth
#define IKScanGetCapEnum				lpFuncIKScanGetCapEnum
#define IKScanGetCapRange				lpFuncIKScanGetCapRange
#define IKScanGetDSInfo					lpFuncIKScanGetDSInfo
#define IKScanGetMinimumSize			lpFuncIKScanGetMinimumSize
#define IKScanGetPhysicalSize			lpFuncIKScanGetPhysicalSize
#define IKScanGetPixelType				lpFuncIKScanGetPixelType
#define IKScanInitialize				lpFuncIKScanInitialize
#define IKScanIsCapSupported			lpFuncIKScanIsCapSupported
#define IKScanList						lpFuncIKScanList
#define IKScanListLen					lpFuncIKScanListLen
#define IKScanOpenDS					lpFuncIKScanOpenDS
#define IKScanSelect					lpFuncIKScanSelect
#define IKScanTerminate					lpFuncIKScanTerminate
#endif

// IK7PRINT.DLL
#ifdef _IK7PRINT_
_EXTERN_ PROCIKARC						lpFuncIKArc;
_EXTERN_ PROCIKCHORD					lpFuncIKChord;
_EXTERN_ PROCIKDRAWTEXT					lpFuncIKDrawText;
_EXTERN_ PROCIKELLIPSE					lpFuncIKEllipse;
_EXTERN_ PROCIKENUMPRINTERS				lpFuncIKEnumPrinters;
_EXTERN_ PROCIKFILLRECT					lpFuncIKFillRect;
_EXTERN_ PROCIKGETDEVMODEHANDLE			lpFuncIKGetDevModeHandle;
_EXTERN_ PROCIKGETDEVMODEINFO			lpFuncIKGetDevModeInfo;
_EXTERN_ PROCIKGETIMAGEFROMHDC			lpFuncIKGetImageFromHdc;
_EXTERN_ PROCIKGETPAPERSIZE				lpFuncIKGetPaperSize;
_EXTERN_ PROCIKGETPIXEL					lpFuncIKGetPixel;
_EXTERN_ PROCIKGETPRINTERPORT			lpFuncIKGetPrinterPort;
_EXTERN_ PROCIKGETTEXTEXTENT			lpFuncIKGetTextExtent;
_EXTERN_ PROCIKIMAGEOUT					lpFuncIKImageOut;
_EXTERN_ PROCIKIMAGEOUTTOHWND			lpFuncIKImageOutToHwnd;
_EXTERN_ PROCIKLINE						lpFuncIKLine;
_EXTERN_ PROCIKPAINT					lpFuncIKPaint;
_EXTERN_ PROCIKPIE						lpFuncIKPie;
_EXTERN_ PROCIKPOLYGON					lpFuncIKPolygon;
_EXTERN_ PROCIKPOLYLINE					lpFuncIKPolyline;
_EXTERN_ PROCIKPREVIEWINIT				lpFuncIKPreviewInit;
_EXTERN_ PROCIKPRINTABORTDOC			lpFuncIKPrintAbortDoc;
_EXTERN_ PROCIKPRINTCREATEDC			lpFuncIKPrintCreateDC;
_EXTERN_ PROCIKPRINTDELETEDC			lpFuncIKPrintDeleteDC;
_EXTERN_ PROCIKPRINTDLG					lpFuncIKPrintDlg;
_EXTERN_ PROCIKPRINTENDDOC				lpFuncIKPrintEndDoc;
_EXTERN_ PROCIKPRINTENDPAGE				lpFuncIKPrintEndPage;
_EXTERN_ PROCIKPRINTSTARTDOC			lpFuncIKPrintStartDoc;
_EXTERN_ PROCIKPRINTSTARTPAGE			lpFuncIKPrintStartPage;
_EXTERN_ PROCIKRECTANGLE				lpFuncIKRectangle;
_EXTERN_ PROCIKRELEASEDEVMODEHANDLE		lpFuncIKReleaseDevModeHandle;
_EXTERN_ PROCIKROUNDRECT				lpFuncIKRoundRect;
_EXTERN_ PROCIKSETDEVMODEINFO			lpFuncIKSetDevModeInfo;
_EXTERN_ PROCIKSETPIXEL					lpFuncIKSetPixel;
_EXTERN_ PROCIKSETPRINT					lpFuncIKSetPrint;
_EXTERN_ PROCIKTEXTOUT					lpFuncIKTextOut;
_EXTERN_ PROCIKENUMPORTS				lpFuncIKEnumPorts;
_EXTERN_ PROCIKPOLYBEZIER				lpFuncIKPolyBezier;
_EXTERN_ PROCIKFRAMERECT				lpFuncIKFrameRect;
_EXTERN_ PROCIKDRAWFOCUSRECT			lpFuncIKDrawFocusRect;
_EXTERN_ PROCIKENUMPAPERSIZES			lpFuncIKEnumPaperSizes;
_EXTERN_ PROCIKENUMPAPERBINS			lpFuncIKEnumPaperBins;
_EXTERN_ PROCIKENUMRESOLUTIONS			lpFuncIKEnumResolutions;
_EXTERN_ PROCIKPRINTGETARRAYNUM			lpFuncIKPrintGetArrayNum;
_EXTERN_ PROCIKGETDEFAULTPRINTER		lpFuncIKGetDefaultPrinter;
_EXTERN_ PROCIKSETDEFAULTPRINTER		lpFuncIKSetDefaultPrinter;
_EXTERN_ PROCIKDRAWSTRING				lpFuncIKDrawString;
_EXTERN_ PROCIKMEASURESTRING			lpFuncIKMeasureString;
_EXTERN_ PROCIKSAVEDEVMODEHANDLE		lpFuncIKSaveDevModeHandle;
_EXTERN_ PROCIKPRINTGDIPEND				lpFuncIKPrintGdipEnd;
_EXTERN_ PROCIKPRINTGDIPSTART			lpFuncIKPrintGdipStart;

#define IKArc							lpFuncIKArc
#define IKChord							lpFuncIKChord
#define IKDrawText						lpFuncIKDrawText
#define IKEllipse						lpFuncIKEllipse
#define IKEnumPrinters					lpFuncIKEnumPrinters
#define IKFillRect						lpFuncIKFillRect
#define IKGetDevModeHandle				lpFuncIKGetDevModeHandle
#define IKGetDevModeInfo				lpFuncIKGetDevModeInfo
#define IKGetImageFromHdc				lpFuncIKGetImageFromHdc
#define IKGetPaperSize					lpFuncIKGetPaperSize
#define IKGetPixel						lpFuncIKGetPixel
#define IKGetPrinterPort				lpFuncIKGetPrinterPort
#define IKGetTextExtent					lpFuncIKGetTextExtent
#define IKImageOut						lpFuncIKImageOut
#define IKImageOutToHwnd				lpFuncIKImageOutToHwnd
#define IKLine							lpFuncIKLine
#define IKPaint							lpFuncIKPaint
#define IKPie							lpFuncIKPie
#define IKPolygon						lpFuncIKPolygon
#define IKPolyline						lpFuncIKPolyline
#define IKPreviewInit					lpFuncIKPreviewInit
#define IKPrintAbortDoc					lpFuncIKPrintAbortDoc
#define IKPrintCreateDC					lpFuncIKPrintCreateDC
#define IKPrintDeleteDC					lpFuncIKPrintDeleteDC
#define IKPrintDlg						lpFuncIKPrintDlg
#define IKPrintEndDoc					lpFuncIKPrintEndDoc
#define IKPrintEndPage					lpFuncIKPrintEndPage
#define IKPrintStartDoc					lpFuncIKPrintStartDoc
#define IKPrintStartPage				lpFuncIKPrintStartPage
#define IKRectangle						lpFuncIKRectangle
#define IKReleaseDevModeHandle			lpFuncIKReleaseDevModeHandle
#define IKRoundRect						lpFuncIKRoundRect
#define IKSetDevModeInfo				lpFuncIKSetDevModeInfo
#define IKSetPixel						lpFuncIKSetPixel
#define IKSetPrint						lpFuncIKSetPrint
#define IKTextOut						lpFuncIKTextOut
#define IKEnumPorts						lpFuncIKEnumPorts
#define IKPolyBezier					lpFuncIKPolyBezier
#define IKFrameRect						lpFuncIKFrameRect
#define IKDrawFocusRect					lpFuncIKDrawFocusRect
#define IKEnumPaperSizes				lpFuncIKEnumPaperSizes
#define IKEnumPaperBins					lpFuncIKEnumPaperBins
#define IKEnumResolutions				lpFuncIKEnumResolutions
#define IKPrintGetArrayNum				lpFuncIKPrintGetArrayNum
#define IKGetDefaultPrinter				lpFuncIKGetDefaultPrinter
#define IKSetDefaultPrinter				lpFuncIKSetDefaultPrinter
#define IKDrawString					lpFuncIKDrawString
#define IKMeasureString					lpFuncIKMeasureString
#define IKSaveDevModeHandle				lpFuncIKSaveDevModeHandle
#define IKPrintGdipEnd					lpFuncIKPrintGdipEnd
#define IKPrintGdipStart				lpFuncIKPrintGdipStart
#endif

// IK7EFFECT.DLL
#ifdef _IK7EFFECT_
_EXTERN_ PROCIKAFFINE					lpFuncIKAffine;
_EXTERN_ PROCIKANTIALIAS				lpFuncIKAntiAlias;
_EXTERN_ PROCIKAUTOSELECTIMAGEEX		lpFuncIKAutoSelectImageEx;
_EXTERN_ PROCIKBLUR						lpFuncIKBlur;
_EXTERN_ PROCIKCANVAS					lpFuncIKCanvas;
_EXTERN_ PROCIKCHECKSECRETIMAGE			lpFuncIKCheckSecretImage;
_EXTERN_ PROCIKCHROMA					lpFuncIKChroma;
_EXTERN_ PROCIKCONVERTCOLOR				lpFuncIKConvertColor;
_EXTERN_ PROCIKCUSTOMFILTER				lpFuncIKCustomFilter;
_EXTERN_ PROCIKCUTRECTIMAGE				lpFuncIKCutRectImage;
_EXTERN_ PROCIKEMBOSS					lpFuncIKEmboss;
_EXTERN_ PROCIKENDDIBACCESS				lpFuncIKEndDibAccess;
_EXTERN_ PROCIKGETDIBPIXEL				lpFuncIKGetDibPixel;
_EXTERN_ PROCIKGLASSTILE				lpFuncIKGlassTile;
_EXTERN_ PROCIKLAYER					lpFuncIKLayer;
_EXTERN_ PROCIKLAYEREX					lpFuncIKLayerEx;
_EXTERN_ PROCIKLENS						lpFuncIKLens;
_EXTERN_ PROCIKMAKERGBAIMAGE			lpFuncIKMakeRGBAImage;
_EXTERN_ PROCIKMOSAIC					lpFuncIKMosaic;
_EXTERN_ PROCIKMOTIONBLUR				lpFuncIKMotionBlur;
_EXTERN_ PROCIKOILPAINT					lpFuncIKOilPaint;
_EXTERN_ PROCIKOUTLINE					lpFuncIKOutline;
_EXTERN_ PROCIKPANORAMA					lpFuncIKPanorama;
_EXTERN_ PROCIKPASTEIMAGE				lpFuncIKPasteImage;
_EXTERN_ PROCIKREMOVENOISE				lpFuncIKRemoveNoise;
_EXTERN_ PROCIKRESIZEEX					lpFuncIKResizeEx;
_EXTERN_ PROCIKRGBGAMMA					lpFuncIKRGBGamma;
_EXTERN_ PROCIKRGBLEVEL					lpFuncIKRGBLevel;
_EXTERN_ PROCIKRGBREV					lpFuncIKRGBRev;
_EXTERN_ PROCIKRGBSPLINE				lpFuncIKRGBSpline;
_EXTERN_ PROCIKRIPPLE					lpFuncIKRipple;
_EXTERN_ PROCIKROTATIONEX				lpFuncIKRotationEx;
_EXTERN_ PROCIKSELECTIMAGEEX			lpFuncIKSelectImageEx;
_EXTERN_ PROCIKSETDIBPIXEL				lpFuncIKSetDibPixel;
_EXTERN_ PROCIKSETGRAY					lpFuncIKSetGray;
_EXTERN_ PROCIKSETSECRETIMAGE			lpFuncIKSetSecretImage;
_EXTERN_ PROCIKSHARP					lpFuncIKSharp;
_EXTERN_ PROCIKSPLITRGBAIMAGE			lpFuncIKSplitRGBAImage;
_EXTERN_ PROCIKSTARTDIBACCESS			lpFuncIKStartDibAccess;
_EXTERN_ PROCIKUNIFYCOLOR				lpFuncIKUnifyColor;
_EXTERN_ PROCIKWAVES					lpFuncIKWaves;
_EXTERN_ PROCIKWHIRLPINCH				lpFuncIKWhirlPinch;
_EXTERN_ PROCIKYCCGAMMA					lpFuncIKYCCGamma;
_EXTERN_ PROCIKYCCLEVEL					lpFuncIKYCCLevel;
_EXTERN_ PROCIKYCCREV					lpFuncIKYCCRev;
_EXTERN_ PROCIKYCCSPLINE				lpFuncIKYCCSpline;
_EXTERN_ PROCIKAUTOSELECTIMAGE			lpFuncIKAutoSelectImage;
_EXTERN_ PROCIKRESIZE					lpFuncIKResize;
_EXTERN_ PROCIKROTATION					lpFuncIKRotation;
_EXTERN_ PROCIKSELECTIMAGE				lpFuncIKSelectImage;
_EXTERN_ PROCIKREDEYEREMOVAL			lpFuncIKRedEyeRemoval;

#define IKAffine						lpFuncIKAffine
#define IKAntiAlias						lpFuncIKAntiAlias
#define IKAutoSelectImageEx				lpFuncIKAutoSelectImageEx
#define IKBlur							lpFuncIKBlur
#define IKCanvas						lpFuncIKCanvas
#define IKCheckSecretImage				lpFuncIKCheckSecretImage
#define IKChroma						lpFuncIKChroma
#define IKConvertColor					lpFuncIKConvertColor
#define IKCustomFilter					lpFuncIKCustomFilter
#define IKCutRectImage					lpFuncIKCutRectImage
#define IKEmboss						lpFuncIKEmboss
#define IKEndDibAccess					lpFuncIKEndDibAccess
#define IKGetDibPixel					lpFuncIKGetDibPixel
#define IKGlassTile						lpFuncIKGlassTile
#define IKLayer							lpFuncIKLayer
#define IKLayerEx						lpFuncIKLayerEx
#define IKLens							lpFuncIKLens
#define IKMakeRGBAImage					lpFuncIKMakeRGBAImage
#define IKMosaic						lpFuncIKMosaic
#define IKMotionBlur					lpFuncIKMotionBlur
#define IKOilPaint						lpFuncIKOilPaint
#define IKOutline						lpFuncIKOutline
#define IKPanorama						lpFuncIKPanorama
#define IKPasteImage					lpFuncIKPasteImage
#define IKRemoveNoise					lpFuncIKRemoveNoise
#define IKResizeEx						lpFuncIKResizeEx
#define IKRGBGamma						lpFuncIKRGBGamma
#define IKRGBLevel						lpFuncIKRGBLevel
#define IKRGBRev						lpFuncIKRGBRev
#define IKRGBSpline						lpFuncIKRGBSpline
#define IKRipple						lpFuncIKRipple
#define IKRotationEx					lpFuncIKRotationEx
#define IKSelectImageEx					lpFuncIKSelectImageEx
#define IKSetDibPixel					lpFuncIKSetDibPixel
#define IKSetGray						lpFuncIKSetGray
#define IKSetSecretImage				lpFuncIKSetSecretImage
#define IKSharp							lpFuncIKSharp
#define IKSplitRGBAImage				lpFuncIKSplitRGBAImage
#define IKStartDibAccess				lpFuncIKStartDibAccess
#define IKUnifyColor					lpFuncIKUnifyColor
#define IKWaves							lpFuncIKWaves
#define IKWhirlPinch					lpFuncIKWhirlPinch
#define IKYCCGamma						lpFuncIKYCCGamma
#define IKYCCLevel						lpFuncIKYCCLevel
#define IKYCCRev						lpFuncIKYCCRev
#define IKYCCSpline						lpFuncIKYCCSpline
#define IKAutoSelectImage				lpFuncIKAutoSelectImage
#define IKResize						lpFuncIKResize
#define IKRotation						lpFuncIKRotation
#define IKSelectImage					lpFuncIKSelectImage
#define IKRedEyeRemoval					lpFuncIKRedEyeRemoval
#endif

// IK7VECTCOM.DLL
#ifdef _IK7VECT_
_EXTERN_ PROCIKCREATEVECTIMAGE			lpFuncIKCreateVectImage;
_EXTERN_ PROCIKCREATEVECTIMAGEEX		lpFuncIKCreateVectImageEx;
_EXTERN_ PROCIKDRAWVECTOBJECT			lpFuncIKDrawVectObject;
_EXTERN_ PROCIKVECTORTORASTER			lpFuncIKVectorToRaster;
_EXTERN_ PROCIKVECTORGDIPEND			lpFuncIKVectorGdipEnd;
_EXTERN_ PROCIKVECTORGDIPSTART			lpFuncIKVectorGdipStart;

#define IKCreateVectImage				lpFuncIKCreateVectImage
#define IKCreateVectImageEx				lpFuncIKCreateVectImageEx
#define IKDrawVectObject				lpFuncIKDrawVectObject
#define IKVectorToRaster				lpFuncIKVectorToRaster
#define IKVectorGdipEnd					lpFuncIKVectorGdipEnd
#define IKVectorGdipStart				lpFuncIKVectorGdipStart
#endif

// IK7RASTOVECT.DLL
#ifdef _IK7RAS2VECT_
_EXTERN_ PROCIKRASTERTOVECTOR			lpFuncIKRasterToVector;
#define IKRasterToVector				lpFuncIKRasterToVector
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MAIN_IK7_PROGRAM_
/****************************************************************
 * ＤＬＬのロード                                               *
 *                                                              *
 * （引数）                                                     *
 *    lpDllPath : ImageKitのDLLファイルが存在するディレクトリ名 *
 *                例 - c:\abc\                                  *
 ****************************************************************/
void LoadImageKitDll(LPCSTR lpDllPath)
{
	char Path[MAX_PATH];

	// IK7COM.DLL
	lpFuncIKBitmapFromDib			= NULL;
	lpFuncIKClearClipBrd			= NULL;
	lpFuncIKCopyImage				= NULL;
	lpFuncIKBitBlt					= NULL;
	lpFuncIKStretchBlt				= NULL;
	lpFuncIKCreateImage				= NULL;
	lpFuncIKDeleteBitmapObject		= NULL;
	lpFuncIKDibFromBitmap			= NULL;
	lpFuncIKFreeMemory				= NULL;
	lpFuncIKGetDpi					= NULL;
	lpFuncIKGetDpiFromHdc			= NULL;
	lpFuncIKGetErrorStatus			= NULL;
	lpFuncIKGetFromClipBrd			= NULL;
	lpFuncIKGetImageType			= NULL;
	lpFuncIKGetMemorySize			= NULL;
	lpFuncIKGetOneBitPalCount		= NULL;
	lpFuncIKGetPalette				= NULL;
	lpFuncIKGetSystemPalette		= NULL;
	lpFuncIKIsClipBrdData			= NULL;
	lpFuncIKResizeRefine1BitImage	= NULL;
	lpFuncIKSetDpi					= NULL;
	lpFuncIKSetPalette				= NULL;
	lpFuncIKSetToClipBrd			= NULL;
	lpFuncIKGetDpiF					= NULL;
	lpFuncIKSetDpiF					= NULL;

	if (lpDllPath) {
		lstrcpy(Path, lpDllPath);
		lstrcat(Path, IKCOM_MODULE);
	} else {
		lstrcpy(Path, IKCOM_MODULE);
	}
	hInstIKCOM = LoadLibrary(Path);
	if (hInstIKCOM) {
		lpFuncIKBitmapFromDib			= (PROCIKBITMAPFROMDIB)			GetProcAddress(hInstIKCOM, "IKBitmapFromDib");
		lpFuncIKClearClipBrd			= (PROCIKCLEARCLIPBRD)			GetProcAddress(hInstIKCOM, "IKClearClipBrd");
		lpFuncIKCopyImage				= (PROCIKCOPYIMAGE)				GetProcAddress(hInstIKCOM, "IKCopyImage");
		lpFuncIKBitBlt					= (PROCIKBITBLT)				GetProcAddress(hInstIKCOM, "IKBitBlt");
		lpFuncIKStretchBlt				= (PROCIKSTRETCHBLT)			GetProcAddress(hInstIKCOM, "IKStretchBlt");
		lpFuncIKCreateImage				= (PROCIKCREATEIMAGE)			GetProcAddress(hInstIKCOM, "IKCreateImage");
		lpFuncIKDeleteBitmapObject		= (PROCIKDELETEBITMAPOBJECT)	GetProcAddress(hInstIKCOM, "IKDeleteBitmapObject");
		lpFuncIKDibFromBitmap			= (PROCIKDIBFROMBITMAP)			GetProcAddress(hInstIKCOM, "IKDibFromBitmap");
		lpFuncIKFreeMemory				= (PROCIKFREEMEMORY)			GetProcAddress(hInstIKCOM, "IKFreeMemory");
		lpFuncIKGetDpi					= (PROCIKGETDPI)				GetProcAddress(hInstIKCOM, "IKGetDpi");
		lpFuncIKGetDpiFromHdc			= (PROCIKGETDPIFROMHDC)			GetProcAddress(hInstIKCOM, "IKGetDpiFromHdc");
		lpFuncIKGetErrorStatus			= (PROCIKGETERRORSTATUS)		GetProcAddress(hInstIKCOM, "IKGetErrorStatus");
		lpFuncIKGetFromClipBrd			= (PROCIKGETFROMCLIPBRD)		GetProcAddress(hInstIKCOM, "IKGetFromClipBrd");
		lpFuncIKGetImageType			= (PROCIKGETIMAGETYPE)			GetProcAddress(hInstIKCOM, "IKGetImageType");
		lpFuncIKGetMemorySize			= (PROCIKGETMEMORYSIZE)			GetProcAddress(hInstIKCOM, "IKGetMemorySize");
		lpFuncIKGetOneBitPalCount		= (PROCIKGETONEBITPALCOUNT)		GetProcAddress(hInstIKCOM, "IKGetOneBitPalCount");
		lpFuncIKGetPalette				= (PROCIKGETPALETTE)			GetProcAddress(hInstIKCOM, "IKGetPalette");
		lpFuncIKGetSystemPalette		= (PROCIKGETSYSTEMPALETTE)		GetProcAddress(hInstIKCOM, "IKGetSystemPalette");
		lpFuncIKIsClipBrdData			= (PROCIKISCLIPBRDDATA)			GetProcAddress(hInstIKCOM, "IKIsClipBrdData");
		lpFuncIKResizeRefine1BitImage	= (PROCIKRESIZEREFINE1BITIMAGE)	GetProcAddress(hInstIKCOM, "IKResizeRefine1BitImage");
		lpFuncIKSetDpi					= (PROCIKSETDPI)				GetProcAddress(hInstIKCOM, "IKSetDpi");
		lpFuncIKSetPalette				= (PROCIKSETPALETTE)			GetProcAddress(hInstIKCOM, "IKSetPalette");
		lpFuncIKSetToClipBrd			= (PROCIKSETTOCLIPBRD)			GetProcAddress(hInstIKCOM, "IKSetToClipBrd");
		lpFuncIKGetDpiF					= (PROCIKGETDPIF)				GetProcAddress(hInstIKCOM, "IKGetDpiF");
		lpFuncIKSetDpiF					= (PROCIKSETDPIF)				GetProcAddress(hInstIKCOM, "IKSetDpiF");
	}

	// IK7FILE.DLL
#ifdef _IK7FILE_
	lpFuncIKFileLoadAsRawData		= NULL;
	lpFuncIKFileSaveAsRawData		= NULL;
	lpFuncIKFileLoad				= NULL;
	lpFuncIKFileType				= NULL;
	lpFuncIKOpenFileDialog			= NULL;
	lpFuncIKOpenFileDlg				= NULL;
	lpFuncIKSaveFileDialog			= NULL;
	lpFuncIKSaveFileDlg				= NULL;
	lpFuncIKSaveFileDlgEx			= NULL;
	lpFuncIKFTPGetFile				= NULL;
	lpFuncIKFTPPutFile				= NULL;
	lpFuncIKFTPDeleteFile			= NULL;
	lpFuncIKFTPRenameFile			= NULL;
	lpFuncIKHTTPGetFile				= NULL;
	lpFuncIKHTTPPutFile				= NULL;
	lpFuncIKBmpFileLoad				= NULL;
	lpFuncIKBmpFileSave				= NULL;
	lpFuncIKJpegFileLoad			= NULL;
	lpFuncIKJpegFileSave			= NULL;
	lpFuncIKJpegExifInfo			= NULL;
	lpFuncIKGifFileLoad				= NULL;
	lpFuncIKGifFileSave				= NULL;
	lpFuncIKTiffFileLoad			= NULL;
	lpFuncIKTiffFileSave			= NULL;
	lpFuncIKPngFileLoad				= NULL;
	lpFuncIKPngFileLoadEx			= NULL;
	lpFuncIKPngFileSave				= NULL;
	lpFuncIKPngFileSaveEx			= NULL;
	lpFuncIKFpxFileLoad				= NULL;
	lpFuncIKFpxFileSave				= NULL;
	lpFuncIKPcxFileLoad				= NULL;
	lpFuncIKPcxFileSave				= NULL;
	lpFuncIKWmfFileLoad				= NULL;
	lpFuncIKWmfFileSave				= NULL;
	lpFuncIKEmfFileLoad				= NULL;
	lpFuncIKEmfFileSave				= NULL;
	lpFuncIKDxfFileLoad				= NULL;
	lpFuncIKDxfFileSave				= NULL;
	lpFuncIKSvgFileLoad				= NULL;
	lpFuncIKSvgFileSave				= NULL;
	lpFuncIKJ2kFileLoad				= NULL;
	lpFuncIKJ2kFileSave				= NULL;
	lpFuncIKSxfP21FileLoad			= NULL;
	lpFuncIKSxfP21FileSave			= NULL;
	lpFuncIKSxfSfcFileLoad			= NULL;
	lpFuncIKSxfSfcFileSave			= NULL;
	lpFuncIKRGBBmpPlaneFileLoad		= NULL;
	lpFuncIKRGBBmpPlaneFileSave		= NULL;
	lpFuncIKCMYKBmpPlaneFileLoad	= NULL;
	lpFuncIKCMYKBmpPlaneFileSave	= NULL;
	lpFuncIKYCCBmpPlaneFileLoad		= NULL;
	lpFuncIKYCCBmpPlaneFileSave		= NULL;

	lpFuncIKFileLoadMem				= NULL;
	lpFuncIKFileTypeMem				= NULL;
	lpFuncIKBmpFileLoadMem			= NULL;
	lpFuncIKBmpFileSaveMem			= NULL;
	lpFuncIKJpegFileLoadMem			= NULL;
	lpFuncIKJpegFileSaveMem			= NULL;
	lpFuncIKJpegExifInfoMem			= NULL;
	lpFuncIKGifFileLoadMem			= NULL;
	lpFuncIKGifFileSaveMem			= NULL;
	lpFuncIKTiffFileLoadMem			= NULL;
	lpFuncIKTiffFileSaveMem			= NULL;
	lpFuncIKPngFileLoadMem			= NULL;
	lpFuncIKPngFileLoadExMem		= NULL;
	lpFuncIKPngFileSaveMem			= NULL;
	lpFuncIKPngFileSaveExMem		= NULL;
	lpFuncIKFpxFileLoadMem			= NULL;
	lpFuncIKFpxFileSaveMem			= NULL;
	lpFuncIKPcxFileLoadMem			= NULL;
	lpFuncIKPcxFileSaveMem			= NULL;
	lpFuncIKWmfFileLoadMem			= NULL;
	lpFuncIKWmfFileSaveMem			= NULL;
	lpFuncIKEmfFileLoadMem			= NULL;
	lpFuncIKEmfFileSaveMem			= NULL;
	lpFuncIKDxfFileLoadMem			= NULL;
	lpFuncIKDxfFileSaveMem			= NULL;
	lpFuncIKSvgFileLoadMem			= NULL;
	lpFuncIKSvgFileSaveMem			= NULL;
	lpFuncIKJ2kFileLoadMem			= NULL;
	lpFuncIKJ2kFileSaveMem			= NULL;
	lpFuncIKSxfP21FileLoadMem		= NULL;
	lpFuncIKSxfP21FileSaveMem		= NULL;
	lpFuncIKSxfSfcFileLoadMem		= NULL;
	lpFuncIKSxfSfcFileSaveMem		= NULL;

	lpFuncIKFTPConnect				= NULL;
	lpFuncIKFTPDisconnect			= NULL;
	lpFuncIKFTPGetFileEx			= NULL;
	lpFuncIKFTPPutFileEx			= NULL;
	lpFuncIKFTPDeleteFileEx			= NULL;
	lpFuncIKFTPRenameFileEx			= NULL;
	lpFuncIKHTTPConnect				= NULL;
	lpFuncIKHTTPDisconnect			= NULL;
	lpFuncIKHTTPGetFileEx			= NULL;
	lpFuncIKHTTPPutFileEx			= NULL;

	if (lpDllPath) {
		lstrcpy(Path, lpDllPath);
		lstrcat(Path, IKFILE_MODULE);
	} else {
		lstrcpy(Path, IKFILE_MODULE);
	}
	hInstIKFILE = LoadLibrary(Path);
	if (hInstIKFILE) {
		lpFuncIKFileLoadAsRawData		= (PROCIKFILELOADASRAWDATA)		GetProcAddress(hInstIKFILE, "IKFileLoadAsRawData");
		lpFuncIKFileSaveAsRawData		= (PROCIKFILESAVEASRAWDATA)		GetProcAddress(hInstIKFILE, "IKFileSaveAsRawData");
		lpFuncIKFileLoad				= (PROCIKFILELOAD)				GetProcAddress(hInstIKFILE, "IKFileLoad");
		lpFuncIKFileType				= (PROCIKFILETYPE)				GetProcAddress(hInstIKFILE, "IKFileType");
		lpFuncIKOpenFileDialog			= (PROCIKOPENFILEDIALOG)		GetProcAddress(hInstIKFILE, "IKOpenFileDialog");
		lpFuncIKOpenFileDlg				= (PROCIKOPENFILEDLG)			GetProcAddress(hInstIKFILE, "IKOpenFileDlg");
		lpFuncIKSaveFileDialog			= (PROCIKSAVEFILEDIALOG)		GetProcAddress(hInstIKFILE, "IKSaveFileDialog");
		lpFuncIKSaveFileDlg				= (PROCIKSAVEFILEDLG)			GetProcAddress(hInstIKFILE, "IKSaveFileDlg");
		lpFuncIKSaveFileDlgEx			= (PROCIKSAVEFILEDLGEX)			GetProcAddress(hInstIKFILE, "IKSaveFileDlgEx");
		lpFuncIKFTPGetFile				= (PROCIKFTPGETFILE)			GetProcAddress(hInstIKFILE, "IKFTPGetFile");
		lpFuncIKFTPPutFile				= (PROCIKFTPPUTFILE)			GetProcAddress(hInstIKFILE, "IKFTPPutFile");
		lpFuncIKFTPDeleteFile			= (PROCIKFTPDELETEFILE)			GetProcAddress(hInstIKFILE, "IKFTPDeleteFile");
		lpFuncIKFTPRenameFile			= (PROCIKFTPRENAMEFILE)			GetProcAddress(hInstIKFILE, "IKFTPRenameFile");
		lpFuncIKHTTPGetFile				= (PROCIKHTTPGETFILE)			GetProcAddress(hInstIKFILE, "IKHTTPGetFile");
		lpFuncIKHTTPPutFile				= (PROCIKHTTPPUTFILE)			GetProcAddress(hInstIKFILE, "IKHTTPPutFile");
		lpFuncIKBmpFileLoad				= (PROCIKBMPFILELOAD)			GetProcAddress(hInstIKFILE, "IKBmpFileLoad");
		lpFuncIKBmpFileSave				= (PROCIKBMPFILESAVE)			GetProcAddress(hInstIKFILE, "IKBmpFileSave");
		lpFuncIKJpegFileLoad			= (PROCIKJPEGFILELOAD)			GetProcAddress(hInstIKFILE, "IKJpegFileLoad");
		lpFuncIKJpegFileSave			= (PROCIKJPEGFILESAVE)			GetProcAddress(hInstIKFILE, "IKJpegFileSave");
		lpFuncIKJpegExifInfo			= (PROCIKJPEGEXIFINFO)			GetProcAddress(hInstIKFILE, "IKJpegExifInfo");
		lpFuncIKGifFileLoad				= (PROCIKGIFFILELOAD)			GetProcAddress(hInstIKFILE, "IKGifFileLoad");
		lpFuncIKGifFileSave				= (PROCIKGIFFILESAVE)			GetProcAddress(hInstIKFILE, "IKGifFileSave");
		lpFuncIKTiffFileLoad			= (PROCIKTIFFFILELOAD)			GetProcAddress(hInstIKFILE, "IKTiffFileLoad");
		lpFuncIKTiffFileSave			= (PROCIKTIFFFILESAVE)			GetProcAddress(hInstIKFILE, "IKTiffFileSave");
		lpFuncIKPngFileLoad				= (PROCIKPNGFILELOAD)			GetProcAddress(hInstIKFILE, "IKPngFileLoad");
		lpFuncIKPngFileLoadEx			= (PROCIKPNGFILELOADEX)			GetProcAddress(hInstIKFILE, "IKPngFileLoadEx");
		lpFuncIKPngFileSave				= (PROCIKPNGFILESAVE)			GetProcAddress(hInstIKFILE, "IKPngFileSave");
		lpFuncIKPngFileSaveEx			= (PROCIKPNGFILESAVEEX)			GetProcAddress(hInstIKFILE, "IKPngFileSaveEx");
		lpFuncIKFpxFileLoad				= (PROCIKFPXFILELOAD)			GetProcAddress(hInstIKFILE, "IKFpxFileLoad");
		lpFuncIKFpxFileSave				= (PROCIKFPXFILESAVE)			GetProcAddress(hInstIKFILE, "IKFpxFileSave");
		lpFuncIKPcxFileLoad				= (PROCIKPCXFILELOAD)			GetProcAddress(hInstIKFILE, "IKPcxFileLoad");
		lpFuncIKPcxFileSave				= (PROCIKPCXFILESAVE)			GetProcAddress(hInstIKFILE, "IKPcxFileSave");
		lpFuncIKWmfFileLoad				= (PROCIKWMFFILELOAD)			GetProcAddress(hInstIKFILE, "IKWmfFileLoad");
		lpFuncIKWmfFileSave				= (PROCIKWMFFILESAVE)			GetProcAddress(hInstIKFILE, "IKWmfFileSave");
		lpFuncIKEmfFileLoad				= (PROCIKEMFFILELOAD)			GetProcAddress(hInstIKFILE, "IKEmfFileLoad");
		lpFuncIKEmfFileSave				= (PROCIKEMFFILESAVE)			GetProcAddress(hInstIKFILE, "IKEmfFileSave");
		lpFuncIKDxfFileLoad				= (PROCIKDXFFILELOAD)			GetProcAddress(hInstIKFILE, "IKDxfFileLoad");
		lpFuncIKDxfFileSave				= (PROCIKDXFFILESAVE)			GetProcAddress(hInstIKFILE, "IKDxfFileSave");
		lpFuncIKSvgFileLoad				= (PROCIKSVGFILELOAD)			GetProcAddress(hInstIKFILE, "IKSvgFileLoad");
		lpFuncIKSvgFileSave				= (PROCIKSVGFILESAVE)			GetProcAddress(hInstIKFILE, "IKSvgFileSave");
		lpFuncIKJ2kFileLoad				= (PROCIKJ2KFILELOAD)			GetProcAddress(hInstIKFILE, "IKJ2kFileLoad");
		lpFuncIKJ2kFileSave				= (PROCIKJ2KFILESAVE)			GetProcAddress(hInstIKFILE, "IKJ2kFileSave");
		lpFuncIKSxfP21FileLoad			= (PROCIKSXFP21FILELOAD)		GetProcAddress(hInstIKFILE, "IKSxfP21FileLoad");
		lpFuncIKSxfP21FileSave			= (PROCIKSXFP21FILESAVE)		GetProcAddress(hInstIKFILE, "IKSxfP21FileSave");
		lpFuncIKSxfSfcFileLoad			= (PROCIKSXFSFCFILELOAD)		GetProcAddress(hInstIKFILE, "IKSxfSfcFileLoad");
		lpFuncIKSxfSfcFileSave			= (PROCIKSXFSFCFILESAVE)		GetProcAddress(hInstIKFILE, "IKSxfSfcFileSave");
		lpFuncIKRGBBmpPlaneFileLoad		= (PROCIKRGBBMPPLANEFILELOAD)	GetProcAddress(hInstIKFILE, "IKRGBBmpPlaneFileLoad");
		lpFuncIKRGBBmpPlaneFileSave		= (PROCIKRGBBMPPLANEFILESAVE)	GetProcAddress(hInstIKFILE, "IKRGBBmpPlaneFileSave");
		lpFuncIKCMYKBmpPlaneFileLoad	= (PROCIKCMYKBMPPLANEFILELOAD)	GetProcAddress(hInstIKFILE, "IKCMYKBmpPlaneFileLoad");
		lpFuncIKCMYKBmpPlaneFileSave	= (PROCIKCMYKBMPPLANEFILESAVE)	GetProcAddress(hInstIKFILE, "IKCMYKBmpPlaneFileSave");
		lpFuncIKYCCBmpPlaneFileLoad		= (PROCIKYCCBMPPLANEFILELOAD)	GetProcAddress(hInstIKFILE, "IKYCCBmpPlaneFileLoad");
		lpFuncIKYCCBmpPlaneFileSave		= (PROCIKYCCBMPPLANEFILESAVE)	GetProcAddress(hInstIKFILE, "IKYCCBmpPlaneFileSave");

		lpFuncIKFileLoadMem				= (PROCIKFILELOADMEM)			GetProcAddress(hInstIKFILE, "IKFileLoadMem");
		lpFuncIKFileTypeMem				= (PROCIKFILETYPEMEM)			GetProcAddress(hInstIKFILE, "IKFileTypeMem");
		lpFuncIKBmpFileLoadMem			= (PROCIKBMPFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKBmpFileLoadMem");
		lpFuncIKBmpFileSaveMem			= (PROCIKBMPFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKBmpFileSaveMem");
		lpFuncIKJpegFileLoadMem			= (PROCIKJPEGFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKJpegFileLoadMem");
		lpFuncIKJpegFileSaveMem			= (PROCIKJPEGFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKJpegFileSaveMem");
		lpFuncIKJpegExifInfoMem			= (PROCIKJPEGEXIFINFOMEM)		GetProcAddress(hInstIKFILE, "IKJpegExifInfoMem");
		lpFuncIKGifFileLoadMem			= (PROCIKGIFFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKGifFileLoadMem");
		lpFuncIKGifFileSaveMem			= (PROCIKGIFFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKGifFileSaveMem");
		lpFuncIKTiffFileLoadMem			= (PROCIKTIFFFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKTiffFileLoadMem");
		lpFuncIKTiffFileSaveMem			= (PROCIKTIFFFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKTiffFileSaveMem");
		lpFuncIKPngFileLoadMem			= (PROCIKPNGFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKPngFileLoadMem");
		lpFuncIKPngFileLoadExMem		= (PROCIKPNGFILELOADEXMEM)		GetProcAddress(hInstIKFILE, "IKPngFileLoadExMem");
		lpFuncIKPngFileSaveMem			= (PROCIKPNGFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKPngFileSaveMem");
		lpFuncIKPngFileSaveExMem		= (PROCIKPNGFILESAVEEXMEM)		GetProcAddress(hInstIKFILE, "IKPngFileSaveExMem");
		lpFuncIKFpxFileLoadMem			= (PROCIKFPXFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKFpxFileLoadMem");
		lpFuncIKFpxFileSaveMem			= (PROCIKFPXFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKFpxFileSaveMem");
		lpFuncIKPcxFileLoadMem			= (PROCIKPCXFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKPcxFileLoadMem");
		lpFuncIKPcxFileSaveMem			= (PROCIKPCXFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKPcxFileSaveMem");
		lpFuncIKWmfFileLoadMem			= (PROCIKWMFFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKWmfFileLoadMem");
		lpFuncIKWmfFileSaveMem			= (PROCIKWMFFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKWmfFileSaveMem");
		lpFuncIKEmfFileLoadMem			= (PROCIKEMFFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKEmfFileLoadMem");
		lpFuncIKEmfFileSaveMem			= (PROCIKEMFFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKEmfFileSaveMem");
		lpFuncIKDxfFileLoadMem			= (PROCIKDXFFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKDxfFileLoadMem");
		lpFuncIKDxfFileSaveMem			= (PROCIKDXFFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKDxfFileSaveMem");
		lpFuncIKSvgFileLoadMem			= (PROCIKSVGFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKSvgFileLoadMem");
		lpFuncIKSvgFileSaveMem			= (PROCIKSVGFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKSvgFileSaveMem");
		lpFuncIKJ2kFileLoadMem			= (PROCIKJ2KFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKJ2kFileLoadMem");
		lpFuncIKJ2kFileSaveMem			= (PROCIKJ2KFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKJ2kFileSaveMem");
		lpFuncIKSxfP21FileLoadMem		= (PROCIKSXFP21FILELOADMEM)		GetProcAddress(hInstIKFILE, "IKSxfP21FileLoadMem");
		lpFuncIKSxfP21FileSaveMem		= (PROCIKSXFP21FILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKSxfP21FileSaveMem");
		lpFuncIKSxfSfcFileLoadMem		= (PROCIKSXFSFCFILELOADMEM)		GetProcAddress(hInstIKFILE, "IKSxfSfcFileLoadMem");
		lpFuncIKSxfSfcFileSaveMem		= (PROCIKSXFSFCFILESAVEMEM)		GetProcAddress(hInstIKFILE, "IKSxfSfcFileSaveMem");

		lpFuncIKFTPConnect				= (PROCIKFTPCONNECT)			GetProcAddress(hInstIKFILE, "IKFTPConnect");
		lpFuncIKFTPDisconnect			= (PROCIKFTPDISCONNECT)			GetProcAddress(hInstIKFILE, "IKFTPDisconnect");
		lpFuncIKFTPGetFileEx			= (PROCIKFTPGETFILEEX)			GetProcAddress(hInstIKFILE, "IKFTPGetFileEx");
		lpFuncIKFTPPutFileEx			= (PROCIKFTPPUTFILEEX)			GetProcAddress(hInstIKFILE, "IKFTPPutFileEx");
		lpFuncIKFTPDeleteFileEx			= (PROCIKFTPDELETEFILEEX)		GetProcAddress(hInstIKFILE, "IKFTPDeleteFileEx");
		lpFuncIKFTPRenameFileEx			= (PROCIKFTPRENAMEFILEEX)		GetProcAddress(hInstIKFILE, "IKFTPRenameFileEx");
		lpFuncIKHTTPConnect				= (PROCIKHTTPCONNECT)			GetProcAddress(hInstIKFILE, "IKHTTPConnect");
		lpFuncIKHTTPDisconnect			= (PROCIKHTTPDISCONNECT)		GetProcAddress(hInstIKFILE, "IKHTTPDisconnect");
		lpFuncIKHTTPGetFileEx			= (PROCIKHTTPGETFILEEX)			GetProcAddress(hInstIKFILE, "IKHTTPGetFileEx");
		lpFuncIKHTTPPutFileEx			= (PROCIKHTTPPUTFILEEX)			GetProcAddress(hInstIKFILE, "IKHTTPPutFileEx");
	}
#endif

	// IK7SCAN.DLL
#ifdef _IK7SCAN_
	lpFuncIKScanCloseDS				= NULL;
	lpFuncIKScanExec				= NULL;
	lpFuncIKScanGetBitDepth			= NULL;
	lpFuncIKScanGetCapEnum			= NULL;
	lpFuncIKScanGetCapRange			= NULL;
	lpFuncIKScanGetDSInfo			= NULL;
	lpFuncIKScanGetMinimumSize		= NULL;
	lpFuncIKScanGetPhysicalSize		= NULL;
	lpFuncIKScanGetPixelType		= NULL;
	lpFuncIKScanInitialize			= NULL;
	lpFuncIKScanIsCapSupported		= NULL;
	lpFuncIKScanList				= NULL;
	lpFuncIKScanListLen				= NULL;
	lpFuncIKScanOpenDS				= NULL;
	lpFuncIKScanSelect				= NULL;
	lpFuncIKScanTerminate			= NULL;

	if (lpDllPath) {
		lstrcpy(Path, lpDllPath);
		lstrcat(Path, IKSCAN_MODULE);
	} else {
		lstrcpy(Path, IKSCAN_MODULE);
	}
	hInstIKSCAN = LoadLibrary(Path);
	if (hInstIKSCAN) {
		lpFuncIKScanCloseDS			= (PROCIKSCANCLOSEDS)			GetProcAddress(hInstIKSCAN, "IKScanCloseDS");
		lpFuncIKScanExec			= (PROCIKSCANEXEC)				GetProcAddress(hInstIKSCAN, "IKScanExec");
		lpFuncIKScanGetBitDepth		= (PROCIKSCANGETBITDEPTH)		GetProcAddress(hInstIKSCAN, "IKScanGetBitDepth");
		lpFuncIKScanGetCapEnum		= (PROCIKSCANGETCAPENUM)		GetProcAddress(hInstIKSCAN, "IKScanGetCapEnum");
		lpFuncIKScanGetCapRange		= (PROCIKSCANGETCAPRANGE)		GetProcAddress(hInstIKSCAN, "IKScanGetCapRange");
		lpFuncIKScanGetDSInfo		= (PROCIKSCANGETDSINFO)			GetProcAddress(hInstIKSCAN, "IKScanGetDSInfo");
		lpFuncIKScanGetMinimumSize	= (PROCIKSCANGETMINIMUMSIZE)	GetProcAddress(hInstIKSCAN, "IKScanGetMinimumSize");
		lpFuncIKScanGetPhysicalSize	= (PROCIKSCANGETPHYSICALSIZE)	GetProcAddress(hInstIKSCAN, "IKScanGetPhysicalSize");
		lpFuncIKScanGetPixelType	= (PROCIKSCANGETPIXELTYPE)		GetProcAddress(hInstIKSCAN, "IKScanGetPixelType");
		lpFuncIKScanInitialize		= (PROCIKSCANINITIALIZE)		GetProcAddress(hInstIKSCAN, "IKScanInitialize");
		lpFuncIKScanIsCapSupported	= (PROCIKSCANISCAPSUPPORTED)	GetProcAddress(hInstIKSCAN, "IKScanIsCapSupported");
		lpFuncIKScanList			= (PROCIKSCANLIST)				GetProcAddress(hInstIKSCAN, "IKScanList");
		lpFuncIKScanListLen			= (PROCIKSCANLISTLEN)			GetProcAddress(hInstIKSCAN, "IKScanListLen");
		lpFuncIKScanOpenDS			= (PROCIKSCANOPENDS)			GetProcAddress(hInstIKSCAN, "IKScanOpenDS");
		lpFuncIKScanSelect			= (PROCIKSCANSELECT)			GetProcAddress(hInstIKSCAN, "IKScanSelect");
		lpFuncIKScanTerminate		= (PROCIKSCANTERMINATE)			GetProcAddress(hInstIKSCAN, "IKScanTerminate");
	}
#endif

	// IK7PRINT.DLL
#ifdef _IK7PRINT_
	lpFuncIKArc					= NULL;
	lpFuncIKChord				= NULL;
	lpFuncIKDrawText			= NULL;
	lpFuncIKEllipse				= NULL;
	lpFuncIKEnumPrinters		= NULL;
	lpFuncIKFillRect			= NULL;
	lpFuncIKGetDevModeHandle	= NULL;
	lpFuncIKGetDevModeInfo		= NULL;
	lpFuncIKGetImageFromHdc		= NULL;
	lpFuncIKGetPaperSize		= NULL;
	lpFuncIKGetPixel			= NULL;
	lpFuncIKGetPrinterPort		= NULL;
	lpFuncIKGetTextExtent		= NULL;
	lpFuncIKImageOut			= NULL;
	lpFuncIKImageOutToHwnd		= NULL;
	lpFuncIKLine				= NULL;
	lpFuncIKPaint				= NULL;
	lpFuncIKPie					= NULL;
	lpFuncIKPolygon				= NULL;
	lpFuncIKPolyline			= NULL;
	lpFuncIKPreviewInit			= NULL;
	lpFuncIKPrintAbortDoc		= NULL;
	lpFuncIKPrintCreateDC		= NULL;
	lpFuncIKPrintDeleteDC		= NULL;
	lpFuncIKPrintDlg			= NULL;
	lpFuncIKPrintEndDoc			= NULL;
	lpFuncIKPrintEndPage		= NULL;
	lpFuncIKPrintStartDoc		= NULL;
	lpFuncIKPrintStartPage		= NULL;
	lpFuncIKRectangle			= NULL;
	lpFuncIKReleaseDevModeHandle= NULL;
	lpFuncIKRoundRect			= NULL;
	lpFuncIKSetDevModeInfo		= NULL;
	lpFuncIKSetPixel			= NULL;
	lpFuncIKSetPrint			= NULL;
	lpFuncIKTextOut				= NULL;
	lpFuncIKEnumPorts			= NULL;
	lpFuncIKPolyBezier			= NULL;
	lpFuncIKFrameRect			= NULL;
	lpFuncIKDrawFocusRect		= NULL;
	lpFuncIKEnumPaperSizes		= NULL;
	lpFuncIKEnumPaperBins		= NULL;
	lpFuncIKEnumResolutions		= NULL;
	lpFuncIKPrintGetArrayNum	= NULL;
	lpFuncIKGetDefaultPrinter	= NULL;
	lpFuncIKSetDefaultPrinter	= NULL;
	lpFuncIKDrawString			= NULL;
	lpFuncIKMeasureString		= NULL;
	lpFuncIKSaveDevModeHandle	= NULL;
	lpFuncIKPrintGdipEnd		= NULL;
	lpFuncIKPrintGdipStart		= NULL;

	if (lpDllPath) {
		lstrcpy(Path, lpDllPath);
		lstrcat(Path, IKPRINT_MODULE);
	} else {
		lstrcpy(Path, IKPRINT_MODULE);
	}
	hInstIKPRINT = LoadLibrary(Path);
	if (hInstIKPRINT) {
		lpFuncIKArc					= (PROCIKARC)					GetProcAddress(hInstIKPRINT, "IKArc");
		lpFuncIKChord				= (PROCIKCHORD)					GetProcAddress(hInstIKPRINT, "IKChord");
		lpFuncIKDrawText			= (PROCIKDRAWTEXT)				GetProcAddress(hInstIKPRINT, "IKDrawText");
		lpFuncIKEllipse				= (PROCIKELLIPSE)				GetProcAddress(hInstIKPRINT, "IKEllipse");
		lpFuncIKEnumPrinters		= (PROCIKENUMPRINTERS)			GetProcAddress(hInstIKPRINT, "IKEnumPrinters");
		lpFuncIKFillRect			= (PROCIKFILLRECT)				GetProcAddress(hInstIKPRINT, "IKFillRect");
		lpFuncIKGetDevModeHandle	= (PROCIKGETDEVMODEHANDLE)		GetProcAddress(hInstIKPRINT, "IKGetDevModeHandle");
		lpFuncIKGetDevModeInfo		= (PROCIKGETDEVMODEINFO)		GetProcAddress(hInstIKPRINT, "IKGetDevModeInfo");
		lpFuncIKGetImageFromHdc		= (PROCIKGETIMAGEFROMHDC)		GetProcAddress(hInstIKPRINT, "IKGetImageFromHdc");
		lpFuncIKGetPaperSize		= (PROCIKGETPAPERSIZE)			GetProcAddress(hInstIKPRINT, "IKGetPaperSize");
		lpFuncIKGetPixel			= (PROCIKGETPIXEL)				GetProcAddress(hInstIKPRINT, "IKGetPixel");
		lpFuncIKGetPrinterPort		= (PROCIKGETPRINTERPORT)		GetProcAddress(hInstIKPRINT, "IKGetPrinterPort");
		lpFuncIKGetTextExtent		= (PROCIKGETTEXTEXTENT)			GetProcAddress(hInstIKPRINT, "IKGetTextExtent");
		lpFuncIKImageOut			= (PROCIKIMAGEOUT)				GetProcAddress(hInstIKPRINT, "IKImageOut");
		lpFuncIKImageOutToHwnd		= (PROCIKIMAGEOUTTOHWND)		GetProcAddress(hInstIKPRINT, "IKImageOutToHwnd");
		lpFuncIKLine				= (PROCIKLINE)					GetProcAddress(hInstIKPRINT, "IKLine");
		lpFuncIKPaint				= (PROCIKPAINT)					GetProcAddress(hInstIKPRINT, "IKPaint");
		lpFuncIKPie					= (PROCIKPIE)					GetProcAddress(hInstIKPRINT, "IKPie");
		lpFuncIKPolygon				= (PROCIKPOLYGON)				GetProcAddress(hInstIKPRINT, "IKPolygon");
		lpFuncIKPolyline			= (PROCIKPOLYLINE)				GetProcAddress(hInstIKPRINT, "IKPolyline");
		lpFuncIKPreviewInit			= (PROCIKPREVIEWINIT)			GetProcAddress(hInstIKPRINT, "IKPreviewInit");
		lpFuncIKPrintAbortDoc		= (PROCIKPRINTABORTDOC)			GetProcAddress(hInstIKPRINT, "IKPrintAbortDoc");
		lpFuncIKPrintCreateDC		= (PROCIKPRINTCREATEDC)			GetProcAddress(hInstIKPRINT, "IKPrintCreateDC");
		lpFuncIKPrintDeleteDC		= (PROCIKPRINTDELETEDC)			GetProcAddress(hInstIKPRINT, "IKPrintDeleteDC");
		lpFuncIKPrintDlg			= (PROCIKPRINTDLG)				GetProcAddress(hInstIKPRINT, "IKPrintDlg");
		lpFuncIKPrintEndDoc			= (PROCIKPRINTENDDOC)			GetProcAddress(hInstIKPRINT, "IKPrintEndDoc");
		lpFuncIKPrintEndPage		= (PROCIKPRINTENDPAGE)			GetProcAddress(hInstIKPRINT, "IKPrintEndPage");
		lpFuncIKPrintStartDoc		= (PROCIKPRINTSTARTDOC)			GetProcAddress(hInstIKPRINT, "IKPrintStartDoc");
		lpFuncIKPrintStartPage		= (PROCIKPRINTSTARTPAGE)		GetProcAddress(hInstIKPRINT, "IKPrintStartPage");
		lpFuncIKRectangle			= (PROCIKRECTANGLE)				GetProcAddress(hInstIKPRINT, "IKRectangle");
		lpFuncIKReleaseDevModeHandle= (PROCIKRELEASEDEVMODEHANDLE)	GetProcAddress(hInstIKPRINT, "IKReleaseDevModeHandle");
		lpFuncIKRoundRect			= (PROCIKROUNDRECT)				GetProcAddress(hInstIKPRINT, "IKRoundRect");
		lpFuncIKSetDevModeInfo		= (PROCIKSETDEVMODEINFO)		GetProcAddress(hInstIKPRINT, "IKSetDevModeInfo");
		lpFuncIKSetPixel			= (PROCIKSETPIXEL)				GetProcAddress(hInstIKPRINT, "IKSetPixel");
		lpFuncIKSetPrint			= (PROCIKSETPRINT)				GetProcAddress(hInstIKPRINT, "IKSetPrint");
		lpFuncIKTextOut				= (PROCIKTEXTOUT)				GetProcAddress(hInstIKPRINT, "IKTextOut");
		lpFuncIKEnumPorts			= (PROCIKENUMPORTS)				GetProcAddress(hInstIKPRINT, "IKEnumPorts");
		lpFuncIKPolyBezier			= (PROCIKPOLYBEZIER)			GetProcAddress(hInstIKPRINT, "IKPolyBezier");
		lpFuncIKFrameRect			= (PROCIKFRAMERECT)				GetProcAddress(hInstIKPRINT, "IKFrameRect");
		lpFuncIKDrawFocusRect		= (PROCIKDRAWFOCUSRECT)			GetProcAddress(hInstIKPRINT, "IKDrawFocusRect");
		lpFuncIKEnumPaperSizes		= (PROCIKENUMPAPERSIZES)		GetProcAddress(hInstIKPRINT, "IKEnumPaperSizes");
		lpFuncIKEnumPaperBins		= (PROCIKENUMPAPERBINS)			GetProcAddress(hInstIKPRINT, "IKEnumPaperBins");
		lpFuncIKEnumResolutions		= (PROCIKENUMRESOLUTIONS)		GetProcAddress(hInstIKPRINT, "IKEnumResolutions");
		lpFuncIKPrintGetArrayNum	= (PROCIKPRINTGETARRAYNUM)		GetProcAddress(hInstIKPRINT, "IKPrintGetArrayNum");
		lpFuncIKGetDefaultPrinter	= (PROCIKGETDEFAULTPRINTER)		GetProcAddress(hInstIKPRINT, "IKGetDefaultPrinter");
		lpFuncIKSetDefaultPrinter	= (PROCIKSETDEFAULTPRINTER)		GetProcAddress(hInstIKPRINT, "IKSetDefaultPrinter");
		lpFuncIKDrawString			= (PROCIKDRAWSTRING)			GetProcAddress(hInstIKPRINT, "IKDrawString");
		lpFuncIKMeasureString		= (PROCIKMEASURESTRING)			GetProcAddress(hInstIKPRINT, "IKMeasureString");
		lpFuncIKSaveDevModeHandle	= (PROCIKSAVEDEVMODEHANDLE)		GetProcAddress(hInstIKPRINT, "IKSaveDevModeHandle");
		lpFuncIKPrintGdipEnd		= (PROCIKPRINTGDIPEND)			GetProcAddress(hInstIKPRINT, "IKPrintGdipEnd");
		lpFuncIKPrintGdipStart		= (PROCIKPRINTGDIPSTART)		GetProcAddress(hInstIKPRINT, "IKPrintGdipStart");
	}
#endif

	// IK7Effect.DLL
#ifdef _IK7EFFECT_
	lpFuncIKAffine				= NULL;
	lpFuncIKAntiAlias			= NULL;
	lpFuncIKAutoSelectImageEx	= NULL;
	lpFuncIKBlur				= NULL;
	lpFuncIKCanvas				= NULL;
	lpFuncIKCheckSecretImage	= NULL;
	lpFuncIKChroma				= NULL;
	lpFuncIKConvertColor		= NULL;
	lpFuncIKCustomFilter		= NULL;
	lpFuncIKCutRectImage		= NULL;
	lpFuncIKEmboss				= NULL;
	lpFuncIKEndDibAccess		= NULL;
	lpFuncIKGetDibPixel			= NULL;
	lpFuncIKGlassTile			= NULL;
	lpFuncIKLayer				= NULL;
	lpFuncIKLayerEx				= NULL;
	lpFuncIKLens				= NULL;
	lpFuncIKMakeRGBAImage		= NULL;
	lpFuncIKMosaic				= NULL;
	lpFuncIKMotionBlur			= NULL;
	lpFuncIKOilPaint			= NULL;
	lpFuncIKOutline				= NULL;
	lpFuncIKPanorama			= NULL;
	lpFuncIKPasteImage			= NULL;
	lpFuncIKRemoveNoise			= NULL;
	lpFuncIKResizeEx			= NULL;
	lpFuncIKRGBGamma			= NULL;
	lpFuncIKRGBLevel			= NULL;
	lpFuncIKRGBRev				= NULL;
	lpFuncIKRGBSpline			= NULL;
	lpFuncIKRipple				= NULL;
	lpFuncIKRotationEx			= NULL;
	lpFuncIKSelectImageEx		= NULL;
	lpFuncIKSetDibPixel			= NULL;
	lpFuncIKSetGray				= NULL;
	lpFuncIKSetSecretImage		= NULL;
	lpFuncIKSharp				= NULL;
	lpFuncIKSplitRGBAImage		= NULL;
	lpFuncIKStartDibAccess		= NULL;
	lpFuncIKUnifyColor			= NULL;
	lpFuncIKWaves				= NULL;
	lpFuncIKWhirlPinch			= NULL;
	lpFuncIKYCCGamma			= NULL;
	lpFuncIKYCCLevel			= NULL;
	lpFuncIKYCCRev				= NULL;
	lpFuncIKYCCSpline			= NULL;
	lpFuncIKAutoSelectImage		= NULL;
	lpFuncIKResize				= NULL;
	lpFuncIKRotation			= NULL;
	lpFuncIKSelectImage			= NULL;
	lpFuncIKRedEyeRemoval		= NULL;

	if (lpDllPath) {
		lstrcpy(Path, lpDllPath);
		lstrcat(Path, IKEFFECT_MODULE);
	} else {
		lstrcpy(Path, IKEFFECT_MODULE);
	}
	hInstIKEFFECT = LoadLibrary(Path);
	if (hInstIKEFFECT) {
		lpFuncIKAffine				= (PROCIKAFFINE)			GetProcAddress(hInstIKEFFECT, "IKAffine");
		lpFuncIKAntiAlias			= (PROCIKANTIALIAS)			GetProcAddress(hInstIKEFFECT, "IKAntiAlias");
		lpFuncIKAutoSelectImageEx	= (PROCIKAUTOSELECTIMAGEEX)	GetProcAddress(hInstIKEFFECT, "IKAutoSelectImageEx");
		lpFuncIKBlur				= (PROCIKBLUR)				GetProcAddress(hInstIKEFFECT, "IKBlur");
		lpFuncIKCanvas				= (PROCIKCANVAS)			GetProcAddress(hInstIKEFFECT, "IKCanvas");
		lpFuncIKCheckSecretImage	= (PROCIKCHECKSECRETIMAGE)	GetProcAddress(hInstIKEFFECT, "IKCheckSecretImage");
		lpFuncIKChroma				= (PROCIKCHROMA)			GetProcAddress(hInstIKEFFECT, "IKChroma");
		lpFuncIKConvertColor		= (PROCIKCONVERTCOLOR)		GetProcAddress(hInstIKEFFECT, "IKConvertColor");
		lpFuncIKCustomFilter		= (PROCIKCUSTOMFILTER)		GetProcAddress(hInstIKEFFECT, "IKCustomFilter");
		lpFuncIKCutRectImage		= (PROCIKCUTRECTIMAGE)		GetProcAddress(hInstIKEFFECT, "IKCutRectImage");
		lpFuncIKEmboss				= (PROCIKEMBOSS)			GetProcAddress(hInstIKEFFECT, "IKEmboss");
		lpFuncIKEndDibAccess		= (PROCIKENDDIBACCESS)		GetProcAddress(hInstIKEFFECT, "IKEndDibAccess");
		lpFuncIKGetDibPixel			= (PROCIKGETDIBPIXEL)		GetProcAddress(hInstIKEFFECT, "IKGetDibPixel");
		lpFuncIKGlassTile			= (PROCIKGLASSTILE)			GetProcAddress(hInstIKEFFECT, "IKGlassTile");
		lpFuncIKLayer				= (PROCIKLAYER)				GetProcAddress(hInstIKEFFECT, "IKLayer");
		lpFuncIKLayerEx				= (PROCIKLAYEREX)			GetProcAddress(hInstIKEFFECT, "IKLayerEx");
		lpFuncIKLens				= (PROCIKLENS)				GetProcAddress(hInstIKEFFECT, "IKLens");
		lpFuncIKMakeRGBAImage		= (PROCIKMAKERGBAIMAGE)		GetProcAddress(hInstIKEFFECT, "IKMakeRGBAImage");
		lpFuncIKMosaic				= (PROCIKMOSAIC)			GetProcAddress(hInstIKEFFECT, "IKMosaic");
		lpFuncIKMotionBlur			= (PROCIKMOTIONBLUR)		GetProcAddress(hInstIKEFFECT, "IKMotionBlur");
		lpFuncIKOilPaint			= (PROCIKOILPAINT)			GetProcAddress(hInstIKEFFECT, "IKOilPaint");
		lpFuncIKOutline				= (PROCIKOUTLINE)			GetProcAddress(hInstIKEFFECT, "IKOutline");
		lpFuncIKPanorama			= (PROCIKPANORAMA)			GetProcAddress(hInstIKEFFECT, "IKPanorama");
		lpFuncIKPasteImage			= (PROCIKPASTEIMAGE)		GetProcAddress(hInstIKEFFECT, "IKPasteImage");
		lpFuncIKRemoveNoise			= (PROCIKREMOVENOISE)		GetProcAddress(hInstIKEFFECT, "IKRemoveNoise");
		lpFuncIKResizeEx			= (PROCIKRESIZEEX)			GetProcAddress(hInstIKEFFECT, "IKResizeEx");
		lpFuncIKRGBGamma			= (PROCIKRGBGAMMA)			GetProcAddress(hInstIKEFFECT, "IKRGBGamma");
		lpFuncIKRGBLevel			= (PROCIKRGBLEVEL)			GetProcAddress(hInstIKEFFECT, "IKRGBLevel");
		lpFuncIKRGBRev				= (PROCIKRGBREV)			GetProcAddress(hInstIKEFFECT, "IKRGBRev");
		lpFuncIKRGBSpline			= (PROCIKRGBSPLINE)			GetProcAddress(hInstIKEFFECT, "IKRGBSpline");
		lpFuncIKRipple				= (PROCIKRIPPLE)			GetProcAddress(hInstIKEFFECT, "IKRipple");
		lpFuncIKRotationEx			= (PROCIKROTATIONEX)		GetProcAddress(hInstIKEFFECT, "IKRotationEx");
		lpFuncIKSelectImageEx		= (PROCIKSELECTIMAGEEX)		GetProcAddress(hInstIKEFFECT, "IKSelectImageEx");
		lpFuncIKSetDibPixel			= (PROCIKSETDIBPIXEL)		GetProcAddress(hInstIKEFFECT, "IKSetDibPixel");
		lpFuncIKSetGray				= (PROCIKSETGRAY)			GetProcAddress(hInstIKEFFECT, "IKSetGray");
		lpFuncIKSetSecretImage		= (PROCIKSETSECRETIMAGE)	GetProcAddress(hInstIKEFFECT, "IKSetSecretImage");
		lpFuncIKSharp				= (PROCIKSHARP)				GetProcAddress(hInstIKEFFECT, "IKSharp");
		lpFuncIKSplitRGBAImage		= (PROCIKSPLITRGBAIMAGE)	GetProcAddress(hInstIKEFFECT, "IKSplitRGBAImage");
		lpFuncIKStartDibAccess		= (PROCIKSTARTDIBACCESS)	GetProcAddress(hInstIKEFFECT, "IKStartDibAccess");
		lpFuncIKUnifyColor			= (PROCIKUNIFYCOLOR)		GetProcAddress(hInstIKEFFECT, "IKUnifyColor");
		lpFuncIKWaves				= (PROCIKWAVES)				GetProcAddress(hInstIKEFFECT, "IKWaves");
		lpFuncIKWhirlPinch			= (PROCIKWHIRLPINCH)		GetProcAddress(hInstIKEFFECT, "IKWhirlPinch");
		lpFuncIKYCCGamma			= (PROCIKYCCGAMMA)			GetProcAddress(hInstIKEFFECT, "IKYCCGamma");
		lpFuncIKYCCLevel			= (PROCIKYCCLEVEL)			GetProcAddress(hInstIKEFFECT, "IKYCCLevel");
		lpFuncIKYCCRev				= (PROCIKYCCREV)			GetProcAddress(hInstIKEFFECT, "IKYCCRev");
		lpFuncIKYCCSpline			= (PROCIKYCCSPLINE)			GetProcAddress(hInstIKEFFECT, "IKYCCSpline");
		lpFuncIKAutoSelectImage		= (PROCIKAUTOSELECTIMAGE)	GetProcAddress(hInstIKEFFECT, "IKAutoSelectImage");
		lpFuncIKResize				= (PROCIKRESIZE)			GetProcAddress(hInstIKEFFECT, "IKResize");
		lpFuncIKRotation			= (PROCIKROTATION)			GetProcAddress(hInstIKEFFECT, "IKRotation");
		lpFuncIKSelectImage			= (PROCIKSELECTIMAGE)		GetProcAddress(hInstIKEFFECT, "IKSelectImage");
		lpFuncIKRedEyeRemoval		= (PROCIKREDEYEREMOVAL)		GetProcAddress(hInstIKEFFECT, "IKRedEyeRemoval");
	}
#endif

	// IK7VECTCOM.DLL
#ifdef _IK7VECT_
	lpFuncIKCreateVectImage		= NULL;
	lpFuncIKCreateVectImageEx	= NULL;
	lpFuncIKDrawVectObject		= NULL;
	lpFuncIKVectorToRaster		= NULL;
	lpFuncIKVectorGdipEnd		= NULL;
	lpFuncIKVectorGdipStart		= NULL;

	if (lpDllPath) {
		lstrcpy(Path, lpDllPath);
		lstrcat(Path, IKVECT_MODULE);
	} else {
		lstrcpy(Path, IKVECT_MODULE);
	}
	hInstIKVectCOM = LoadLibrary(Path);
	if (hInstIKVectCOM) {
		lpFuncIKCreateVectImage		= (PROCIKCREATEVECTIMAGE)	GetProcAddress(hInstIKVectCOM, "IKCreateVectImage");
		lpFuncIKCreateVectImageEx	= (PROCIKCREATEVECTIMAGEEX)	GetProcAddress(hInstIKVectCOM, "IKCreateVectImageEx");
		lpFuncIKDrawVectObject		= (PROCIKDRAWVECTOBJECT)	GetProcAddress(hInstIKVectCOM, "IKDrawVectObject");
		lpFuncIKVectorToRaster		= (PROCIKVECTORTORASTER)	GetProcAddress(hInstIKVectCOM, "IKVectorToRaster");
		lpFuncIKVectorGdipEnd		= (PROCIKVECTORGDIPEND)		GetProcAddress(hInstIKVectCOM, "IKVectorGdipEnd");
		lpFuncIKVectorGdipStart		= (PROCIKVECTORGDIPSTART)	GetProcAddress(hInstIKVectCOM, "IKVectorGdipStart");
	}
#endif

	// IK7RASTOVECT.DLL
#ifdef _IK7RAS2VECT_
	lpFuncIKRasterToVector = NULL;

	if (lpDllPath) {
		lstrcpy(Path, lpDllPath);
		lstrcat(Path, IKRAS2VECT_MODULE);
	} else {
		lstrcpy(Path, IKRAS2VECT_MODULE);
	}
	hInstIKRasterToVector = LoadLibrary(Path);
	if (hInstIKRasterToVector) {
		lpFuncIKRasterToVector = (PROCIKRASTERTOVECTOR)GetProcAddress(hInstIKRasterToVector, "IKRasterToVector");
	}
#endif
}

/****************************************************************
 * ＤＬＬのアンロード                                           *
 ****************************************************************/
void UnloadImageKitDll(void)
{
	if (hInstIKCOM)
		FreeLibrary(hInstIKCOM);

#ifdef _IK7FILE_
	if (hInstIKFILE)
		FreeLibrary(hInstIKFILE);
#endif

#ifdef _IK7SCAN_
	if (hInstIKSCAN)
		FreeLibrary(hInstIKSCAN);
#endif

#ifdef _IK7PRINT_
	if (hInstIKPRINT)
		FreeLibrary(hInstIKPRINT);
#endif

#ifdef _IK7EFFECT_
	if (hInstIKEFFECT)
		FreeLibrary(hInstIKEFFECT);
#endif

#ifdef _IK7VECT_
	if (hInstIKVectCOM)
		FreeLibrary(hInstIKVectCOM);
#endif

#ifdef _IK7RAS2VECT_
	if (hInstIKRasterToVector)
		FreeLibrary(hInstIKRasterToVector);
	lpFuncIKRasterToVector = NULL;
#endif
}
#endif			//#ifdef _MAIN_IK7_PROGRAM_

#ifdef __cplusplus
}
#endif

#endif			//IMAGEKIT7_DEF_INCLUDE
