#ifndef CFOLDERDLG_H
#define CFOLDERDLG_H

#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE  0x0040
#endif

#include "stdafx.h"
using namespace std;

class CFolderDlg
{
public:
	static CString Show()
	{
		TCHAR            szFolderPath[MAX_PATH] = { 0 };
		CString            strFolderPath = TEXT("");

		BROWSEINFO        sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot = 0;
		sInfo.lpszTitle = _T("Select Folder£º");
		sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = NULL;

		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
		if (lpidlBrowse != NULL)
		{
			if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
			{
				strFolderPath = szFolderPath;
			}
		}
		if (lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}

		return strFolderPath;

	}

};

#endif

