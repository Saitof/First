/*
 *
 * L.E.A.P.System Client SDK Header for Version 1.1
 *         Copyright(c) 2003-2005 Pegasys Inc. All Rights Reserved.
 *
 * Versionは Major.Minor.Release.BuildNumber というルール付けで設定されています。
 * Release upは無償で行われます。
 * Minor upは無償で行われますが、構造が大幅に変わっていることがあります。
 * Major upの場合、基本的には有償です。構造が大幅に変わっていることがあります。
 *
 * Version History:
 *   2003/12/01 - 1.0.0.1
 *				First Release
 *   2004/03/01 - 1.0.0.2
 *				暗号化サポート関数を追加。アルゴリズムは1個
 *   2004/03/15 - 1.0.0.3
 *				暗号化サポート:アルゴリズム追加(1個:計2個)
 *   2004/03/16 - 1.0.0.4
 *				暗号化サポート:アルゴリズム追加(1個:計3個)
 *   2004/03/17 - 1.0.0.5
 *				暗号化サポート:アルゴリズム追加(2個:計5個)
 *   2004/03/18 - 1.0.0.6
 *				暗号化サポート:アルゴリズム追加(2個:計7個)
 *   2004/03/19 - 1.0.0.7
 *				暗号化サポート:アルゴリズム追加(3個:計10個)
 *   2004/03/22 - 1.0.0.8
 *				暗号化サポート:アルゴリズム追加(3個:計13個)
 *   2004/03/23 - 1.0.0.9
 *				暗号化サポート:アルゴリズム追加(3個:計16個)
 *   2004/03/24 - 1.0.0.10
 *				暗号化サポート:アルゴリズム追加(3個:計19個)
 *   2004/03/29 - 1.0.0.11
 *				圧縮サポート追加。アルゴリズムは7個
 *   2004/03/30 - 1.0.0.12
 *				暗号化サポート:アルゴリズム追加(9個:計28個)
 *   2004/03/31 - 1.0.0.13
 *				実装したが廃止されたクラスのリンクを排除
 *				圧縮サポート:アルゴリズム追加(5個:計12個)
 *   2004/04/01 - 1.0.0.14
 *				圧縮サポートでバッファオーバーランすることがあったのを修正
 *				圧縮サポートにコールバックインターフェイス追加
 *				圧縮サポート:アルゴリズム追加(36個:計48個)
 *				圧縮サポート:モード設定追加
 *					同一アルゴリズムで、速度重視、標準、メモリ重視のアプローチがある場合は切り替え可能
 *					ない場合は同じルーチンで処理を行う
 *   2004/04/02 - 1.0.0.15
 *				圧縮サポート:アルゴリズム追加(3個:計51個)
 *				圧縮サポート:エラー取得方法を追加  
 *   2004/04/19 - 1.0.0.16
 *				圧縮サポート:バッファオーバーランの可能性箇所を排除
 *				圧縮サポート:アルゴリズム追加(4個:計54個)
 *   2004/04/21 - 1.0.0.17
 *				ハッシュサポート追加
 *   2004/05/13 - 1.0.1.18
 *				プロキシサーバーの認証に対応
 *   2004/06/04 - 1.0.2.19
 *				RSA暗号高速化
 *				ライセンスファイルのプロダクトIDを取得する関数を追加
 *   2004/06/09 - 1.0.3.20
 *				RSA暗号高速化
 *				フリーフィールドのアドレス計算が間違っていたのを修正
 *   2004/06/10 - 1.0.4.21
 *				構造化例外ハンドラが環境によって落ちる可能性があったのを修正
 *   2004/06/15 - 1.0.5.22
 *				ライセンス対象データが小さく、暗号キーが長い場合に不正なブロックを生成していたのを修正
 *   2004/06/25 - 1.0.5.23
 *				将来の圧縮アルゴリズム拡張のためのクラス整理
 *				インラインで書かれていた複合クラスをインラインのまま分割した
 *   2004/06/29 - 1.0.5.24
 *				圧縮アルゴリズム0でのエスケープ処理を修正
 *   2004/07/22 - 1.0.5.25
 *				マシン識別子取得方法を修正
 *				前バージョンと互換モードを追加。デフォルトは新モード
 *   2004/07/23 - 1.0.6.26
 *				ファイアーウォール対応
 *   2004/07/27 - 1.0.7.27
 *				特定状況でのエラーコードが間違っていたのを修正
 *   2004/07/30 - 1.0.8.28
 *				VisualC++6.0対応
 *   2004/08/10 - 1.0.9.29
 *				マシン識別子取得方法を追加
 *				デフォルトはさらに追加されたモード
 *   2004/09/07 - 1.0.10.30
 *				プロキシサーバー対応強化
 *   2004/09/17 - 1.1.0.31
 *				通知機能追加
 *				メモリ上圧縮解凍がサイズによっては正しく動作していなかったのを修正
 *   2004/09/29 - 1.1.0.32
 *				プロキシサーバーのDigest認証に対応
 *   2004/10/04 - 1.1.0.33
 *				プロキシサーバーのNTLM認証に対応
 *   2004/11/19 - 1.1.0.34
 *              通知機能に日付管理を追加
 *   2004/12/27 - 1.1.0.35
 *				通知アーカイブ機能に日付管理を追加。
 *				ただし判定はアプリケーションに一任する。
 *   2005/01/05 - 1.1.0.36
 *				日付判定をアーカイブ内に内蔵
 *				画像圧縮サポート関数を追加。アルゴリズムは1個
 *   2005/01/11 - 1.1.0.37
 *				通知機能アーカイブフォーマット変更
 *   2005/01/18 - 1.1.0.38
 *				複数アプリケーションで通知機能アーカイブを共有する場合の通知システムを追加
 *   2005/01/24 - 1.1.0.39
 *				日付判定のテスト関数を追加
 *   2005/04/05 - 1.1.1.40
 *				マシン識別子取得方法を追加
 *				ライブラリ内部関数/クラスをnamespaceの中に入れ、リンク問題を発生しにくくするように修正
 *   2005/05/25 - 1.1.2.41
 *				SNTPサポートを追加
 *				マシン識別子判定を間違える条件があったのを修正
 *   2006/10/05 - 1.1.2.42
 *				マシン識別子取得方法を追加
 *   2006/11/07 - 1.1.2.43
 *				Vista対応
 *   2007/03/26 - 1.1.2.44
 *				Vista対応強化
 *   2007/04/03 - 1.1.3.45
 *				ライセンスファイルの発効日-ソフトリミット-ハードリミット間隔制限を撤廃
 *   2007/04/24 - 1.1.4.46
 *				マシン識別子取得方法追加
 *				デフォルトをWindows98/Meでも動作するモード(Type1)へ変更
 *   2008/05/15 - 1.1.4.47
 *				User Agentを追加
 *   2008/05/19 - 1.1.4.48
 *				1.1.3.45での変更の悪影響で、残り日数が取れなくなっていたのを修正
 *   2008/05/21 - 1.1.4.49
 *				内部の初期化タイミングを変更
 *   2008/10/02 - 1.1.4.50
 *				VisualStudio 2008 SP1対応
 *   2009/01/29 - 1.1.4.51
 *				FreeFieldに長さ0の文字列が登録された場合にリークしていたのを修正
 *				プロダクトキーがリークしていたのを修正
 *   2009/05/11 - 1.1.5.52
 *				VisualC++ 6.0サポート削除
 *				ドキュメント修正
 *   2009/10/27 - 1.1.6.53
 *				ドキュメント修正
 *				サンプルプログラムを追加
 *				ファイルの配置を変更
 ***************************************************************************************************/

#ifndef __LEAPS_H_DEFINED__
#define __LEAPS_H_DEFINED__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ライセンスファイル関係定義
 */
enum LEAPS_InternalMachineFunctionType {
	LEAPS_InternalMachineFunction_Type0,
	LEAPS_InternalMachineFunction_Type1,
	LEAPS_InternalMachineFunction_Type2,
	LEAPS_InternalMachineFunction_Type3,
	LEAPS_InternalMachineFunction_Type4,
	LEAPS_InternalMachineFunction_Type5,
	LEAPS_InternalMachineFunction_Type6,
	LEAPS_InternalMachineFunction_Type7,
	LEAPS_InternalMachineFunction_Type8,
	LEAPS_InternalMachineFunction_Type9,
	LEAPS_InternalMachineFunction_Type10,
	LEAPS_InternalMachineFunction_Type11,
	LEAPS_InternalMachineFunction_Type12,
	LEAPS_InternalMachineFunction_Type13,
	LEAPS_InternalMachineFunction_Type14
};

typedef int (__stdcall *LPLEAPSGetMachineID)(void* ptrIDBuffer, int iIDBufferSize);
typedef int (__stdcall *LPLEAPSCompareMachineID)(void* ptrIDBuffer, int iIDBufferSize);

int __stdcall LEAPS_Initialize(void);

int __stdcall LEAPS_SetKeyData(int iKeyID, const char* pszKey_N, const char* pszKey_E);
int __stdcall LEAPS_SetMachineFunctions(LPLEAPSGetMachineID lpGetMachineID, LPLEAPSCompareMachineID lpCompareMachineID);
int __stdcall LEAPS_SetMachineFunctionType(enum LEAPS_InternalMachineFunctionType type);
int __stdcall LEAPS_SetLicenseFileName(const char* pszLicenseFileName);
int __stdcall LEAPS_CheckProductKey(char* pszProductKey);
int __stdcall LEAPS_SetProductKey(const char* pszProductKey);
int __stdcall LEAPS_SetMakerID(int iMakerID);
int __stdcall LEAPS_SetProductID(int iProductID);

int __stdcall LEAPS_SetFieldData(int iFieldNumber, char* pszFieldData);
int __stdcall LEAPS_SetProxyServer(const char* pszProxyServerAddress, unsigned short sPort);
int __stdcall LEAPS_SetProxyUseFlag(BOOL bUse);
int __stdcall LEAPS_SetProxyAuth(const char* pszUserName, const char* pszPassword);
int __stdcall LEAPS_SetProxyAuthUseFlag(BOOL bUse);
int __stdcall LEAPS_SetServerCount(int iServerCount);
int __stdcall LEAPS_SetServer(int iServerNumber, const char* pszServerAddress, const char* pszCGIPath, unsigned short sPort);
int __stdcall LEAPS_GetPostDataSize(void);
int __stdcall LEAPS_GetPostData(char* pszDataArea);
int __stdcall LEAPS_SetPostData(const char* pszDataArea);
int __stdcall LEAPS_PostData(int iServerNumber);
int __stdcall LEAPS_GetHTTPResponseCode(void);
int __stdcall LEAPS_GetErrorResponse(char* pszTextArea, int iTextAreaLength);

int __stdcall LEAPS_CheckLicense(BOOL bCheckDate);
int __stdcall LEAPS_CheckLicense2(BOOL bDateCheck, int* iRemainDate);
int __stdcall LEAPS_GetProductID();
int __stdcall LEAPS_GetProductKey(char* pszProductKeyBuffer, int iProductKeyBufferSize);
int __stdcall LEAPS_GetFieldData(int iFieldNumber, char* pszFieldDataBuffer, int iFieldDataBufferSize);

/*
 * 通知機能関係
 */
// 自由データ領域として16バイト
#define LEAPS_INFO_FREE_ARRAY_SIZE 16
int __stdcall LEAPS_SetInfoBaseName(const char* pszMakerName, const char* pszProductName, int iLanguageCode);
int __stdcall LEAPS_SetMakerInfoServer(const char* pszServerAddress, const char* pszCGIPath, unsigned short sPort);
int __stdcall LEAPS_GetMakerInfoArchiveCount(void);
int __stdcall LEAPS_GetMakerInfoArchiveTitle(int iIndex, char* pszTitleBuffer, int iTitleBufferSize);
int __stdcall LEAPS_GetMakerInfoArchiveFreeDataArray(int iIndex, void* pBuffer);
int __stdcall LEAPS_SetMakerInfoArchiveFreeDataArray(int iIndex, void* pBuffer);
int __stdcall LEAPS_GetMakerInfoArchiveLineCount(int iIndex);
int __stdcall LEAPS_GetMakerInfoArchiveLine(int iIndex, int iLineNumber, char* pszLineBuffer, int iLineBufferSize);
int __stdcall LEAPS_ResetMakerInfoArchive(void);
int __stdcall LEAPS_SetMakerInfoArchiveChangeEventRecieveWindow(HWND hWnd, UINT Message, LPARAM lParam, WPARAM wParam);
int __stdcall LEAPS_CanGetMakerInfo(void);
int __stdcall LEAPS_GetMakerInfo(void);
int __stdcall LEAPS_GetMakerInfoHTTPResponseCode(void);
int __stdcall LEAPS_GetMakerInfoErrorResponse(char* pszTextArea, int iTextAreaLength);
int __stdcall LEAPS_GetMakerInfoCount(void);
int __stdcall LEAPS_GetProductInfoArchiveCount(void);
int __stdcall LEAPS_GetProductInfoArchiveTitle(int iIndex, char* pszTitleBuffer, int iTitleBufferSize);
int __stdcall LEAPS_GetProductInfoArchiveFreeDataArray(int iIndex, void* pBuffer);
int __stdcall LEAPS_SetProductInfoArchiveFreeDataArray(int iIndex, void* pBuffer);
int __stdcall LEAPS_GetProductInfoArchiveLineCount(int iIndex);
int __stdcall LEAPS_GetProductInfoArchiveLine(int iIndex, int iLineNumber, char* pszLineBuffer, int iLineBufferSize);
int __stdcall LEAPS_SetProductInfoArchiveChangeEventRecieveWindow(HWND hWnd, UINT Message, LPARAM lParam, WPARAM wParam);
int __stdcall LEAPS_ResetProductInfoArchive(void);
int __stdcall LEAPS_SetProductInfoServer(const char* pszServerAddress, const char* pszCGIPath, unsigned short sPort);
int __stdcall LEAPS_CanGetProductInfo(void);
int __stdcall LEAPS_GetProductInfo(void);
int __stdcall LEAPS_GetProductInfoHTTPResponseCode(void);
int __stdcall LEAPS_GetProductInfoErrorResponse(char* pszTextArea, int iTextAreaLength);
int __stdcall LEAPS_GetProductInfoCount(void);
int __stdcall LEAPS_GetProductVersionupInfoArchiveCount(void);
int __stdcall LEAPS_GetProductVersionupInfoArchiveTitle(int iIndex, char* pszTitleBuffer, int iTitleBufferSize);
int __stdcall LEAPS_GetProductVersionupInfoArchiveFreeDataArray(int iIndex, void* pBuffer);
int __stdcall LEAPS_SetProductVersionupInfoArchiveFreeDataArray(int iIndex, void* pBuffer);
int __stdcall LEAPS_GetProductVersionupInfoArchiveLineCount(int iIndex);
int __stdcall LEAPS_GetProductVersionupInfoArchiveLine(int iIndex, int iLineNumber, char* pszLineBuffer, int iLineBufferSize);
int __stdcall LEAPS_SetProductVersionupInfoArchiveChangeEventRecieveWindow(HWND hWnd, UINT Message, LPARAM lParam, WPARAM wParam);
int __stdcall LEAPS_ResetProductVersionupInfoArchive(void);
int __stdcall LEAPS_SetProductVersionupInfoServer(const char* pszServerAddress, const char* pszCGIPath, unsigned short sPort);
int __stdcall LEAPS_CanGetProductVersionupInfo(void);
int __stdcall LEAPS_GetProductVersionupInfo(void);
int __stdcall LEAPS_GetProductVersionupInfoHTTPResponseCode(void);
int __stdcall LEAPS_GetProductVersionupInfoErrorResponse(char* pszTextArea, int iTextAreaLength);
int __stdcall LEAPS_GetProductVersionupInfoCount(void);
int __stdcall LEAPS_FreeInfo(void);

int __stdcall LEAPS_Finalize(void);

/*
 * デバッガ検出関係定義
 */
int __stdcall LEAPS_StartAntiDebugger(int iCheckMilliSec);
int __stdcall LEAPS_StopAntiDebugger(void);

/*
 * 暗号関係定義
 */
int __stdcall LEAPS_CipherAlgorithmCount(void);
int __stdcall LEAPS_CipherMinPassPhraseLen(int iAlgoNum);
int __stdcall LEAPS_CipherMaxPassPhraseLen(int iAlgoNum);
int __stdcall LEAPS_CipherEncrypt(int iAlgoNum, void* pPassPhrase, int iPassPhraseLen, void* pPlain, int iPlainLen, void* pCrypt, int* piCryptLen);
int __stdcall LEAPS_CipherDecrypt(int iAlgoNum, void* pPassPhrase, int iPassPhraseLen, void* pCrypt, int iCryptLen, void* pPlain, int* piPlainLen);

/*
 * ファイル圧縮関係定義
 */
typedef int (__stdcall *LPLEAPSReadCallback)(void* pContext, DWORD dwReadBytesLow, DWORD dwReadBytesHigh);
typedef int (__stdcall *LPLEAPSWriteCallback)(void* pContext, DWORD dwWriteBytesLow, DWORD dwWriteBytesHigh);

enum LEAPS_CompressorMode {
	LEAPS_Compressor_Normal,
	LEAPS_Compressor_Speed,
	LEAPS_Compressor_Memory
};

int __stdcall LEAPS_CompressorAlgorithmCount(void);
enum LEAPS_CompressorMode __stdcall LEAPS_CompressorSetMode(enum LEAPS_CompressorMode mode);
int __stdcall LEAPS_CompressorGetLastError(void);
int __stdcall LEAPS_CompressorEncodeFileToFile(int iAlgoNum, char* pszInFile, char* pszOutFile, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeFileToHandle(int iAlgoNum, char* pszInFile, HANDLE hWrite, DWORD* pdwWriteBytes, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeFileToMemory(int iAlgoNum, char* pszInFile, void* pOut, DWORD* pdwOutSize, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeHandleToFile(int iAlgoNum, HANDLE hRead, DWORD dwReadSize, char* pszOutFile, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeHandleToHandle(int iAlgoNum, HANDLE hRead, DWORD dwReadSize, HANDLE hWrite, DWORD* pdwWriteBytes, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeHandleToMemory(int iAlgoNum, HANDLE hRead, DWORD dwReadSize, void* pOut, DWORD* pdwOutSize, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeMemoryToFile(int iAlgoNum, void* pIn, DWORD dwInLen, char* pszOutFile, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeMemoryToHandle(int iAlgoNum, void* pIn, DWORD dwInLen, HANDLE hWrite, DWORD* pdwWriteBytes, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorEncodeMemoryToMemory(int iAlgoNum, void* pIn, DWORD dwInLen, void* pOut, DWORD* pdwOutSize, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeFileToFile(int iAlgoNum, char* pszInFile, char* pszOutFile, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeFileToHandle(int iAlgoNum, char* pszInFile, HANDLE hWrite, DWORD* pdwWriteBytes, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeFileToMemory(int iAlgoNum, char* pszInFile, void* pOut, DWORD* pdwOutSize, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeHandleToFile(int iAlgoNum, HANDLE hRead, DWORD dwReadSize, char* pszOutFile, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeHandleToHandle(int iAlgoNum, HANDLE hRead, DWORD dwReadSize, HANDLE hWrite, DWORD* pdwWriteBytes, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeHandleToMemory(int iAlgoNum, HANDLE hRead, DWORD dwReadSize, void* pOut, DWORD* pdwOutSize, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeMemoryToFile(int iAlgoNum, void* pIn, DWORD dwInLen, char* pszOutFile, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeMemoryToHandle(int iAlgoNum, void* pIn, DWORD dwInLen, HANDLE hWrite, DWORD* pdwWriteBytes, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);
int __stdcall LEAPS_CompressorDecodeMemoryToMemory(int iAlgoNum, void* pIn, DWORD dwInLen, void* pOut, DWORD* pdwOutSize, LPLEAPSReadCallback pRead, void* pReadContext, LPLEAPSWriteCallback pWrite, void* pWriteContext);

/*
 * ハッシュ関係定義
 */
enum LEAPS_HashType {
	LEAPS_Hash_CRC16,
	LEAPS_Hash_CRC32,
	LEAPS_Hash_MD5,
	LEAPS_Hash_SHA1,
	LEAPS_Hash_SHA256,
	LEAPS_Hash_SHA384,
	LEAPS_Hash_SHA512,
	LEAPS_Hash_RIPEMD128,
	LEAPS_Hash_RIPEMD160,
	LEAPS_Hash_RIPEMD256,
	LEAPS_Hash_RIPEMD320
};

int __stdcall LEAPS_HashCreateHandle(enum LEAPS_HashType emHash);
int __stdcall LEAPS_HashCompute(int iHashHandle, void* pComputeArea, int iComputeAreaSize);
int __stdcall LEAPS_HashGetValue(int iHashHandle, void* pHashArea, int iHashAreaSize);
int __stdcall LEAPS_HashCloseHandle(int iHashHandle);

/*
 * 映像圧縮関係定義
 */
int __stdcall LEAPS_VideoCompressorInitialize(void);
int __stdcall LEAPS_VideoCompressorAlgorithmCount(void);
int __stdcall LEAPS_VideoCompressorEncodeToFile(int iAlgoNum, BITMAPINFO* pInfo, void* pBitmapData, char* pszOutFile);
int __stdcall LEAPS_VideoCompressorEncodeToHandle(int iAlgoNum, BITMAPINFO* pInfo, void* pBitmapData, HANDLE hWrite, DWORD* pdwWriteBytes);
int __stdcall LEAPS_VideoCompressorEncodeToMemory(int iAlgoNum,  BITMAPINFO* pInfo, void* pBitmapData, void* pOut, DWORD* pdwOutSize);
int __stdcall LEAPS_VideoCompressorGetBitmapInfoFromFile(char* pszInFile, BITMAPINFO* pInfo);
int __stdcall LEAPS_VideoCompressorGetBitmapInfoFromHandle(HANDLE hRead, BITMAPINFO* pInfo);
int __stdcall LEAPS_VideoCompressorGetBitmapInfoFromMemory(void* pIn, DWORD dwInLen, BITMAPINFO* pInfo);
int __stdcall LEAPS_VideoCompressorDecodeFromFile(int iAlgoNum, char* pszInFile, void* pBitmapData);
int __stdcall LEAPS_VideoCompressorDecodeFromHandle(int iAlgoNum, HANDLE hRead, void* pBitmapData);
int __stdcall LEAPS_VideoCompressorDecodeFromMemory(int iAlgoNum, void* pIN, DWORD dwInLen, void* pBitmapData);
int __stdcall LEAPS_VideoCompressorFinalize(void);

/*
 * SNTPサポート定義
 */
int __stdcall LEAPS_SNTP_SetServer(const char* pszServerAddress, unsigned short sPort);
int __stdcall LEAPS_SNTP_CheckCalendar(void);

#ifdef __cplusplus
};
#endif
#endif
