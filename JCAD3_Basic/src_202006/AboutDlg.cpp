/////////////////////////////////////////////////////////////////////////////
// バージョン情報ダイアログクラス(CAboutDlg)定義
// AboutDlg.cpp
//---------------------------------------------------------------------------
// LastEdit : 2010/03/13 by M.Fukushima
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AboutDlg.h"
#include "DataAccess1.h"
#include "msi.h"				//msi.h のディレクトリの場所が include パスに含まれているか確認 

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

/************************************************/
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)						// ｸﾗｽの構築
/************************************************/
{
}

/**************************************************/
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
/**************************************************/
{
	CDialog::DoDataExchange(pDX);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg クラスのメッセージ ハンドラ

/**********************************/
BOOL CAboutDlg::OnInitDialog(void)
/**********************************/
{
	CString	str, strInfo;
	char    chUserName[256]={0}, chCmpName[256]={0}, chSrNum[256]={0};
	char    chWork[256];
	DWORD   dwUserName=256, dwCmpName=256, dwSrNum=256;
	UINT    ret;
	int     stp, edp;

	// CG: 次のコードは「システム情報」コンポーネントにより追加されています。
	CDialog::OnInitDialog();
	if(GetDemoFlg()!=0) {												// <体験版の場合>
		str.LoadString(IDS_STR35);										// ﾊﾞｰｼﾞｮﾝ取得
		GetDlgItem(IDC_STATIC1)->SetWindowText(str);					// ﾊﾞｰｼﾞｮﾝ表示
	}
	ret=::MsiGetUserInfo("{17DB9E3C-3AE4-423A-8CD4-E8F0C4C5E937}",
                         chUserName, &dwUserName, chCmpName, &dwCmpName,
                         chSrNum, &dwSrNum);							// ｲﾝｽﾄｰﾙ情報取得
	if(ret!=USERINFOSTATE_PRESENT) {									// <ｴﾗｰ発生の場合>
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
			strInfo.Format("%s", chWork);
		} else {
			strInfo = _T("");
		}
	} else {
		strInfo = _T("");
	}
	SetDlgItemText(IDC_EDIT1, strInfo);
	if(dwCmpName>0) {													// 会社情報
		stp = (chCmpName[0]==' ') ? 1 : 0;
		edp = (chCmpName[dwCmpName-1]==' ') ? dwCmpName-1 : dwCmpName;
		if(edp-stp>0) {
			memset(chWork, NULL, sizeof(chWork));
			memcpy(chWork, &chCmpName[stp], edp-stp);
			strInfo.Format("%s", chWork);
		} else {
			strInfo = _T("");
		}
	} else {
		strInfo = _T("");
	}
	SetDlgItemText(IDC_EDIT2, strInfo);
	if(dwSrNum>0) {														// ｼﾘｱﾙNo情報
		stp = (chSrNum[0]==' ') ? 1 : 0;
		edp = (chSrNum[dwSrNum-1]==' ') ? dwSrNum-1 : dwSrNum;
		if(edp-stp>0) {
			memset(chWork, NULL, sizeof(chWork));
			if(edp-stp>19) {
				if(chSrNum[stp+20]=='9') {								// 非表示の場合
					memcpy(chWork, "xxxx-xxxx-xxxx-xxxx", 19);
				} else {
					memcpy(chWork, &chSrNum[stp], 19);
				}
			} else {
				memcpy(chWork, &chSrNum[stp], edp-stp);
			}
			strInfo.Format("%s", chWork);
			str.LoadString(IDS_STR36);
			strInfo = str + " : " + strInfo;
		} else {
			strInfo = _T("");
		}
	} else {
		strInfo = _T("");
	}
	SetDlgItemText(IDC_EDIT3, strInfo);
	return TRUE;
}
