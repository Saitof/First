/////////////////////////////////////////////////////////////////////////////
// JCAD3 ライセンス部
// LCheck.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include <direct.h>
#include "stdafx.h"
#include "resource.h"
#include "DataType.h"
#include "Jcad3Glb.h"
#include "InputsDlg.h"
#include "LEAPS.h"
#include "msi.h"				//msi.h のディレクトリの場所が include パスに含まれているか確認 

/**********************/
int LicenseCheck(void)													//<<< JCAD3ﾗｲｾﾝｽﾁｪｯｸ
/**********************/
{
	char    chUserName[256]={0}, chCmpName[256]={0}, chSrNum[256]={0}, chWork[256];
	DWORD   dwUserName=256, dwCmpName=256, dwSrNum=256;
	UINT    rc;
	int     stp, edp, ret=1, chk=1, iRet, iLen;
	CString str;
	BOOL    m_OfflineCertif=FALSE;
	CString m_ServerAddress="leaps.pegasys-inc.com";
	CString m_ServerPath="/cgi-bin/Lic.cgi";
	BOOL    m_UseProxy=FALSE;
	CString m_ProxyServerAddress=_T("");
	int     m_ProxyServerPort=8080;
	CString m_Maker_N="EADEF47B3CAEF4EDA9B1438DC46444E4EC22FA7D49065BE754F3B6B446D4C232F5E9D6A7748EE026C29F9E1ECC7CA0DC27C47A9427C38E1D7E56F6E71EC38C5BCDD66674C61F70171752CDAC5127CF99A7A05D3BD91C3D06E41E04C5CC04A1078668255A905F71F7B98D1E05D22F978E1EE3A3EA7638CC429DCCE186DC5A15B9";
	CString m_Maker_E="0EF242C206305EB6B008F4020A2B9DFBF3197FA63A2C9F72CB396663E894856055779B9F9A091792C1E9935F0D0346BEE1F2E737F938A9A021A3C9E4D115C0C936092EF7A17E9B5FC252F81D0F76FF098B4600E9A0E7FB603D50EF73FE48BC25D86B8B0ACB4800128FF7D706C11E9DA0524C833064B432A6562ED7F19CCA7BAF";
	int     m_MakerID=17;
	int     m_ProductID=1;
	CString m_ProductKey=_T("");
	CString m_Product_N="8AAF35E33B5CF41BD55FC843FA480CF4EB5D0DF43B2E8C8DA85730DB0EFA1DCD4C24AEF4F6AFF17BBE06514FA0B7C29103A0A4ECD5ED8DF75FBD73FFF8DCBEA6170D3377427553CFD7959D25742411E50B38300F91620EAF1C46F20F78EA2D84F4D81D697843DE8C64B45A80CBF1A3CD92172D9AEDA4744A88B14311FACF69A3";
	CString m_Product_E="1082929B0D298AC65C73058E3014C4970FD43884B1B64DAF57169201B28B7D73DE65E410F8CBCD828A6E7764E877666698D013A24A3AC1A9A3C750DB6CDD53A5E17B597DE6FDB694C1987E4B5E1B64E2F7C6FCED1FC115FF4AD35F81DBE65BEC516AB90BF8988E3E6EA8D00170AFDB94A90E712136B77BA6438E429BBB833779";
	CString m_LicenseFile=_T("");
	char    szPath[MAX_PATH];
	FILE*   fp;
	CString path;
	int     er=0, n1, n2, n3, n4, no;

	//## ignore license 
	return 0;

#if 0
	rc=::MsiGetUserInfo("{17DB9E3C-3AE4-423A-8CD4-E8F0C4C5E937}",
                        chUserName, &dwUserName, chCmpName, &dwCmpName,
                        chSrNum, &dwSrNum);								// ｲﾝｽﾄｰﾙ情報取得

	if(rc!=USERINFOSTATE_PRESENT) {										// <ｴﾗｰ発生の場合>
		memset(chUserName, NULL, sizeof(chUserName));					// ﾊﾞｯﾌｧ初期化
		memset(chCmpName, NULL, sizeof(chCmpName));
		memset(chSrNum, NULL, sizeof(chSrNum));
		dwUserName = 0, dwCmpName = 0, dwSrNum = 0;
	}
	if(dwUserName>0) {													// ﾕｰｻﾞ情報
		stp = (chUserName[0]==' ') ? 1 : 0;
		edp = (chUserName[dwUserName-1]==' ') ? dwUserName-1 : dwUserName;
		if(edp-stp>0) {
			memset(chWork, NULL, sizeof(chWork));
			memcpy(chWork, &chUserName[stp], edp-stp);
			memset(chUserName, NULL, sizeof(chUserName));
			memcpy(chUserName, chWork, edp-stp);
		}
	}
	if(dwCmpName>0) {													// 会社情報
		stp = (chCmpName[0]==' ') ? 1 : 0;
		edp = (chCmpName[dwCmpName-1]==' ') ? dwCmpName-1 : dwCmpName;
		if(edp-stp>0) {
			memset(chWork, NULL, sizeof(chWork));
			memcpy(chWork, &chCmpName[stp], edp-stp);
			memset(chCmpName, NULL, sizeof(chCmpName));
			memcpy(chCmpName, chWork, edp-stp);
		}
	}
	if(dwSrNum>0) {														// ｼﾘｱﾙNo情報
		stp = (chSrNum[0]==' ') ? 1 : 0;
		edp = (chSrNum[dwSrNum-1]==' ') ? dwSrNum-1 : dwSrNum;
		if(edp-stp>0) {
			memset(chWork, NULL, sizeof(chWork));
			memcpy(chWork, &chSrNum[stp], edp-stp);
			memset(chSrNum, NULL, sizeof(chSrNum));
			if(edp-stp>19) {
				memcpy(chSrNum, chWork, 19);
			} else {
				memcpy(chSrNum, chWork, edp-stp);
			}
		}
	}

	if(!memcmp(chSrNum, "0000-0000-0000-0000", 19)||
       rc!=USERINFOSTATE_PRESENT) {										// <<<体験版>>>
		AfxMessageBox(IDS_TRIAL_VERSION);								// 体験版ﾒｯｾｰｼﾞ
		ret = 1;
	} else {															// <<<製品版>>>
		n1 = (int)(chSrNum[18]-0x30);									// 数値部(一の桁)
		n2 = (int)(chSrNum[17]-0x30);									// 数値部(十の桁)
		n3 = (int)(chSrNum[16]-0x30);									// 数値部(百の桁)
		n4 = (int)(chSrNum[15]-0x30);									// 数値部(千の桁)
		no = (1000*n4)+(100*n3)+(10*n2)+n1;								// 数値部の数値
		if((no>=20&&no<=79)||
           (no>=510&&no<=529)||
           (no>=600&&no<=610)) {										// <対象外数値の場合>
			AfxMessageBox(IDS_TRIAL_VERSION);							// 体験版ﾒｯｾｰｼﾞ
			ret = 1;
		} else {														// <対象数値の場合>
			m_LicenseFile = GetAppPath(0)+LIC_NAME;						// ﾗｲｾﾝｽﾌｧｲﾙﾊﾟｽ
			m_ProductKey.Format("%s", chSrNum);							// ｼﾘｱﾙ番号
			// データの取得から開始……
			LEAPS_Initialize();
			// 1. ライセンスファイルのチェック
			LEAPS_SetKeyData(0, (const char*)m_Maker_N, (const char*)m_Maker_E);
			LEAPS_SetKeyData(1, (const char*)m_Product_N, (const char*)m_Product_E);
			LEAPS_SetLicenseFileName(m_LicenseFile);
			LEAPS_SetProductKey(m_ProductKey);
			LEAPS_SetMakerID(m_MakerID);
			LEAPS_SetProductID(m_ProductID);
			iRet = LEAPS_CheckLicense(FALSE);							// ﾗｲｾﾝｽﾁｪｯｸ
			if(iRet==0) {												// ﾗｲｾﾝｽ:OK
				if(LEAPS_GetProductID()==m_ProductID) {					// ﾌﾟﾛﾀﾞｸﾄID:OK
					memset(chWork, NULL, 256);
					LEAPS_GetProductKey(chWork, 256);					// ﾗｲｾﾝｽﾌｧｲﾙからﾌﾟﾛﾀﾞｸﾄｷｰ取得
					if(memcmp(chSrNum, chWork, 19)==0) {				// ﾌﾟﾛﾀﾞｸﾄｷｰ(ｼﾘｱﾙNo):OK
						chk = 0, ret = 0;								// 確認終了(chk=0),認証OK(ret=0)
					}
				}
			}
			// 2. ネット認証開始
			if(chk) {													// <ﾗｲｾﾝｽ確認>
				AfxMessageBox(IDS_LICENSE_CHECK);						// ﾈｯﾄ認証開始ﾒｯｾｰｼﾞ
				// 2.1 プロキシ情報入力（ダイアログ）
				CProxyDlg dlg;
				if(dlg.DoModal()==IDOK) {								// ﾌﾟﾛｷｼ情報ﾀﾞｲｱﾛｸﾞ表示
					dlg.GetValue(&m_OfflineCertif, &m_UseProxy,
                                 &m_ProxyServerAddress,
                                 &m_ProxyServerPort);					// ﾃﾞｰﾀ取得
				} else {												// <ﾀﾞｲｱﾛｸﾞでｷｬﾝｾﾙ>
					AfxMessageBox(IDS_TRIAL_VERSION);					// 体験版ﾒｯｾｰｼﾞ
					chk = 0, ret = 1;									// 確認終了(chk=0),認証NG(ret=1)
				}
			}
			if(chk) {													// <ﾗｲｾﾝｽ確認>
				// 2.2 ネット認証
				LEAPS_SetKeyData(0, (const char*)m_Maker_N, (const char*)m_Maker_E);
				LEAPS_SetKeyData(1, (const char*)m_Product_N, (const char*)m_Product_E);
				LEAPS_SetLicenseFileName(m_LicenseFile);
				LEAPS_SetProductKey(m_ProductKey);
				LEAPS_SetMakerID(m_MakerID);
				LEAPS_SetProductID(m_ProductID);
				LEAPS_SetFieldData(0, NULL);
				LEAPS_SetFieldData(1, NULL);
				LEAPS_SetFieldData(2, NULL);
				LEAPS_SetFieldData(3, NULL);
				LEAPS_SetProxyServer(m_ProxyServerAddress, m_ProxyServerPort);
				LEAPS_SetProxyUseFlag(m_UseProxy);
				LEAPS_SetServerCount(1);
				LEAPS_SetServer(0, m_ServerAddress, m_ServerPath, 80);
				LEAPS_SetFieldData(0, "JCAD3 GLB Japanese");
				if(m_OfflineCertif) {									// <<ｵﾌﾗｲﾝ認証>>
					char *pData = new char[LEAPS_GetPostDataSize()];
					LEAPS_GetPostData(pData);							// 送信ﾃﾞｰﾀ取得
					SHGetSpecialFolderPath(NULL, szPath,
                                       CSIDL_DESKTOPDIRECTORY, FALSE);	// ﾃﾞｽｸﾄｯﾌﾟﾊﾟｽ取得
					path.Format("%s", szPath);
					path = path+"\\"+OFL_NAME;							// ｵﾌﾗｲﾝ認証ﾌｧｲﾙ名
					if(fopen_s(&fp, path, "w")==0) {					// ﾌｧｲﾙOPEN
						if(fprintf(fp, "%s\n", pData)<0) {
							er = 1;
						}
						fclose(fp);										// ﾌｧｲﾙCLOSE
					} else {
						er = 1;
					}
					if(er) {
						AfxMessageBox(IDS_ERR_FILEWRITE);				// ｢ﾌｧｲﾙ保存NG｣
					}
					delete[] pData;
					ret = 1;											// 認証NG(ret=1)
				} else {												// <<ｵﾝﾗｲﾝ認証>>
					iRet = LEAPS_PostData(0);							// 認証
					if(iRet<0) {										// <ｴﾗｰ終了>
						if(iRet==-6) {
							iLen = LEAPS_GetErrorResponse(NULL, 0);		// ｴﾗｰ文字数取得
							iLen = (iLen>256) ? 256 : iLen;
							iLen = (iLen<0) ? 0 : iLen;
						} else {
							iLen = 16;
						}
						char *pData = new char[iLen+2];
						if(iRet==-6) {
							LEAPS_GetErrorResponse(pData, iLen+1);		// LEAPSｴﾗｰﾒｯｾｰｼﾞ取得
						} else {
							memset(pData, NULL, 16);
							sprintf_s(pData, 16, "return=%d", iRet);	// ret=-6以外はｴﾗｰNo
						}
						str.Format(IDS_LICENSE_ERR, pData);				// ｴﾗｰﾒｯｾｰｼﾞ
						AfxMessageBox(str);
						delete[] pData;
						ret = 1;										// 認証NG(ret=1)
					} else {											// <正常終了>
						// 2.3 取得成功、ライセンスファイルのチェック
						iRet = LEAPS_CheckLicense(FALSE);
						ret = 0;										// 認証OK(ret=0)
					}
				}
			}
			// 認証終了
			LEAPS_Finalize();
		}
	}
#endif
	return ret;															// 認証OK(ret=0)/認証NG(ret=1)

}
