
// WallpaperChanger.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "res/Resource.h"		// main symbols


// CWallpaperChangerApp:
// See WallpaperChanger.cpp for the implementation of this class
//

class CWallpaperChangerApp : public CWinApp
{
public:
	CWallpaperChangerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWallpaperChangerApp theApp;
