/*
 *
 * L.E.A.P.System Client SDK Header for Version 1.1
 *         Copyright(c) 2003-2005 Pegasys Inc. All Rights Reserved.
 *
 * Version�� Major.Minor.Release.BuildNumber �Ƃ������[���t���Őݒ肳��Ă��܂��B
 * Release up�͖����ōs���܂��B
 * Minor up�͖����ōs���܂����A�\�����啝�ɕς���Ă��邱�Ƃ�����܂��B
 * Major up�̏ꍇ�A��{�I�ɂ͗L���ł��B�\�����啝�ɕς���Ă��邱�Ƃ�����܂��B
 *
 * Version History:
 *   2003/12/01 - 1.0.0.1
 *				First Release
 *   2004/03/01 - 1.0.0.2
 *				�Í����T�|�[�g�֐���ǉ��B�A���S���Y����1��
 *   2004/03/15 - 1.0.0.3
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(1��:�v2��)
 *   2004/03/16 - 1.0.0.4
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(1��:�v3��)
 *   2004/03/17 - 1.0.0.5
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(2��:�v5��)
 *   2004/03/18 - 1.0.0.6
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(2��:�v7��)
 *   2004/03/19 - 1.0.0.7
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(3��:�v10��)
 *   2004/03/22 - 1.0.0.8
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(3��:�v13��)
 *   2004/03/23 - 1.0.0.9
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(3��:�v16��)
 *   2004/03/24 - 1.0.0.10
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(3��:�v19��)
 *   2004/03/29 - 1.0.0.11
 *				���k�T�|�[�g�ǉ��B�A���S���Y����7��
 *   2004/03/30 - 1.0.0.12
 *				�Í����T�|�[�g:�A���S���Y���ǉ�(9��:�v28��)
 *   2004/03/31 - 1.0.0.13
 *				�����������p�~���ꂽ�N���X�̃����N��r��
 *				���k�T�|�[�g:�A���S���Y���ǉ�(5��:�v12��)
 *   2004/04/01 - 1.0.0.14
 *				���k�T�|�[�g�Ńo�b�t�@�I�[�o�[�������邱�Ƃ��������̂��C��
 *				���k�T�|�[�g�ɃR�[���o�b�N�C���^�[�t�F�C�X�ǉ�
 *				���k�T�|�[�g:�A���S���Y���ǉ�(36��:�v48��)
 *				���k�T�|�[�g:���[�h�ݒ�ǉ�
 *					����A���S���Y���ŁA���x�d���A�W���A�������d���̃A�v���[�`������ꍇ�͐؂�ւ��\
 *					�Ȃ��ꍇ�͓������[�`���ŏ������s��
 *   2004/04/02 - 1.0.0.15
 *				���k�T�|�[�g:�A���S���Y���ǉ�(3��:�v51��)
 *				���k�T�|�[�g:�G���[�擾���@��ǉ�  
 *   2004/04/19 - 1.0.0.16
 *				���k�T�|�[�g:�o�b�t�@�I�[�o�[�����̉\���ӏ���r��
 *				���k�T�|�[�g:�A���S���Y���ǉ�(4��:�v54��)
 *   2004/04/21 - 1.0.0.17
 *				�n�b�V���T�|�[�g�ǉ�
 *   2004/05/13 - 1.0.1.18
 *				�v���L�V�T�[�o�[�̔F�؂ɑΉ�
 *   2004/06/04 - 1.0.2.19
 *				RSA�Í�������
 *				���C�Z���X�t�@�C���̃v���_�N�gID���擾����֐���ǉ�
 *   2004/06/09 - 1.0.3.20
 *				RSA�Í�������
 *				�t���[�t�B�[���h�̃A�h���X�v�Z���Ԉ���Ă����̂��C��
 *   2004/06/10 - 1.0.4.21
 *				�\������O�n���h�������ɂ���ė�����\�����������̂��C��
 *   2004/06/15 - 1.0.5.22
 *				���C�Z���X�Ώۃf�[�^���������A�Í��L�[�������ꍇ�ɕs���ȃu���b�N�𐶐����Ă����̂��C��
 *   2004/06/25 - 1.0.5.23
 *				�����̈��k�A���S���Y���g���̂��߂̃N���X����
 *				�C�����C���ŏ�����Ă��������N���X���C�����C���̂܂ܕ�������
 *   2004/06/29 - 1.0.5.24
 *				���k�A���S���Y��0�ł̃G�X�P�[�v�������C��
 *   2004/07/22 - 1.0.5.25
 *				�}�V�����ʎq�擾���@���C��
 *				�O�o�[�W�����ƌ݊����[�h��ǉ��B�f�t�H���g�͐V���[�h
 *   2004/07/23 - 1.0.6.26
 *				�t�@�C�A�[�E�H�[���Ή�
 *   2004/07/27 - 1.0.7.27
 *				����󋵂ł̃G���[�R�[�h���Ԉ���Ă����̂��C��
 *   2004/07/30 - 1.0.8.28
 *				VisualC++6.0�Ή�
 *   2004/08/10 - 1.0.9.29
 *				�}�V�����ʎq�擾���@��ǉ�
 *				�f�t�H���g�͂���ɒǉ����ꂽ���[�h
 *   2004/09/07 - 1.0.10.30
 *				�v���L�V�T�[�o�[�Ή�����
 *   2004/09/17 - 1.1.0.31
 *				�ʒm�@�\�ǉ�
 *				�������㈳�k�𓀂��T�C�Y�ɂ���Ă͐��������삵�Ă��Ȃ������̂��C��
 *   2004/09/29 - 1.1.0.32
 *				�v���L�V�T�[�o�[��Digest�F�؂ɑΉ�
 *   2004/10/04 - 1.1.0.33
 *				�v���L�V�T�[�o�[��NTLM�F�؂ɑΉ�
 *   2004/11/19 - 1.1.0.34
 *              �ʒm�@�\�ɓ��t�Ǘ���ǉ�
 *   2004/12/27 - 1.1.0.35
 *				�ʒm�A�[�J�C�u�@�\�ɓ��t�Ǘ���ǉ��B
 *				����������̓A�v���P�[�V�����Ɉ�C����B
 *   2005/01/05 - 1.1.0.36
 *				���t������A�[�J�C�u���ɓ���
 *				�摜���k�T�|�[�g�֐���ǉ��B�A���S���Y����1��
 *   2005/01/11 - 1.1.0.37
 *				�ʒm�@�\�A�[�J�C�u�t�H�[�}�b�g�ύX
 *   2005/01/18 - 1.1.0.38
 *				�����A�v���P�[�V�����Œʒm�@�\�A�[�J�C�u�����L����ꍇ�̒ʒm�V�X�e����ǉ�
 *   2005/01/24 - 1.1.0.39
 *				���t����̃e�X�g�֐���ǉ�
 *   2005/04/05 - 1.1.1.40
 *				�}�V�����ʎq�擾���@��ǉ�
 *				���C�u���������֐�/�N���X��namespace�̒��ɓ���A�����N���𔭐����ɂ�������悤�ɏC��
 *   2005/05/25 - 1.1.2.41
 *				SNTP�T�|�[�g��ǉ�
 *				�}�V�����ʎq������ԈႦ��������������̂��C��
 *   2006/10/05 - 1.1.2.42
 *				�}�V�����ʎq�擾���@��ǉ�
 *   2006/11/07 - 1.1.2.43
 *				Vista�Ή�
 *   2007/03/26 - 1.1.2.44
 *				Vista�Ή�����
 *   2007/04/03 - 1.1.3.45
 *				���C�Z���X�t�@�C���̔�����-�\�t�g���~�b�g-�n�[�h���~�b�g�Ԋu������P�p
 *   2007/04/24 - 1.1.4.46
 *				�}�V�����ʎq�擾���@�ǉ�
 *				�f�t�H���g��Windows98/Me�ł����삷�郂�[�h(Type1)�֕ύX
 *   2008/05/15 - 1.1.4.47
 *				User Agent��ǉ�
 *   2008/05/19 - 1.1.4.48
 *				1.1.3.45�ł̕ύX�̈��e���ŁA�c����������Ȃ��Ȃ��Ă����̂��C��
 *   2008/05/21 - 1.1.4.49
 *				�����̏������^�C�~���O��ύX
 *   2008/10/02 - 1.1.4.50
 *				VisualStudio 2008 SP1�Ή�
 *   2009/01/29 - 1.1.4.51
 *				FreeField�ɒ���0�̕����񂪓o�^���ꂽ�ꍇ�Ƀ��[�N���Ă����̂��C��
 *				�v���_�N�g�L�[�����[�N���Ă����̂��C��
 *   2009/05/11 - 1.1.5.52
 *				VisualC++ 6.0�T�|�[�g�폜
 *				�h�L�������g�C��
 *   2009/10/27 - 1.1.6.53
 *				�h�L�������g�C��
 *				�T���v���v���O������ǉ�
 *				�t�@�C���̔z�u��ύX
 ***************************************************************************************************/

#ifndef __LEAPS_H_DEFINED__
#define __LEAPS_H_DEFINED__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ���C�Z���X�t�@�C���֌W��`
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
 * �ʒm�@�\�֌W
 */
// ���R�f�[�^�̈�Ƃ���16�o�C�g
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
 * �f�o�b�K���o�֌W��`
 */
int __stdcall LEAPS_StartAntiDebugger(int iCheckMilliSec);
int __stdcall LEAPS_StopAntiDebugger(void);

/*
 * �Í��֌W��`
 */
int __stdcall LEAPS_CipherAlgorithmCount(void);
int __stdcall LEAPS_CipherMinPassPhraseLen(int iAlgoNum);
int __stdcall LEAPS_CipherMaxPassPhraseLen(int iAlgoNum);
int __stdcall LEAPS_CipherEncrypt(int iAlgoNum, void* pPassPhrase, int iPassPhraseLen, void* pPlain, int iPlainLen, void* pCrypt, int* piCryptLen);
int __stdcall LEAPS_CipherDecrypt(int iAlgoNum, void* pPassPhrase, int iPassPhraseLen, void* pCrypt, int iCryptLen, void* pPlain, int* piPlainLen);

/*
 * �t�@�C�����k�֌W��`
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
 * �n�b�V���֌W��`
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
 * �f�����k�֌W��`
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
 * SNTP�T�|�[�g��`
 */
int __stdcall LEAPS_SNTP_SetServer(const char* pszServerAddress, unsigned short sPort);
int __stdcall LEAPS_SNTP_CheckCalendar(void);

#ifdef __cplusplus
};
#endif
#endif
