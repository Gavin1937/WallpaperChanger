// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "WallpaperChanger.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Message from the Systray Icon
#define MYWM_NOTIFYICON		(WM_USER+2)


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

// IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP

    ON_REGISTERED_MESSAGE(CakTrayIcon::WM_TRAYICONNOTIFY, OnTrayIconNotify)

    
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame(const UINT& trayIconID)
    : CFrameWnd(), m_TrayIconID(trayIconID)
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    
    m_TrayIcon.Create(this, m_TrayIconID);  //  1 is a tray icon identifier
    m_TrayIcon.SetIconAndTip(LoadIcon(AfxGetInstanceHandle(), 
                            MAKEINTRESOURCE(IDR_MAINFRAME)), 
                            _T("This is a tray icon"));
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Add Icon to Systray
	TrayMessage(NIM_ADD);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::TrayMessage( DWORD dwMessage)
{

	CString sTip(_T("BrigSoft Example"));	

	NOTIFYICONDATA tnd;

	tnd.cbSize		= sizeof(NOTIFYICONDATA);
	tnd.hWnd		= m_hWnd;
	tnd.uID			= IDR_TRAYICON;

	tnd.uFlags		= NIF_MESSAGE|NIF_ICON;

	tnd.uCallbackMessage	= MYWM_NOTIFYICON;

	
	VERIFY( tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE (IDR_TRAYICON)) );

	tnd.uFlags		= NIF_MESSAGE|NIF_ICON|NIF_TIP;

	lstrcpyn(tnd.szTip, (LPCTSTR)sTip, sizeof(tnd.szTip)/sizeof(tnd.szTip[0]) );
	
	return Shell_NotifyIcon(dwMessage, &tnd);

}



void CMainFrame::OnClose() 
{
	//Remove the Icon from the Systray
	TrayMessage( NIM_DELETE );	

	CFrameWnd::OnClose();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// Open window when double click to the Systray Icon
	if(message == MYWM_NOTIFYICON){
			
		switch (lParam){

			case WM_LBUTTONDBLCLK:
				switch (wParam)	{
					case IDR_TRAYICON:
						
						ShowWindow(SW_NORMAL);
						SetForegroundWindow();
						SetFocus();

						return TRUE;
						break;
					}
			break;
		}
	}	
	
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{	
	// To open the window we use the systray icon.
	// So we want to hide window instead minimize it.
	if(nType == SIZE_MINIMIZED){
		ShowWindow(SW_HIDE);
	}
	else{
		CFrameWnd::OnSize(nType, cx, cy);
	}	
}

LRESULT CMainFrame::OnTrayIconNotify(WPARAM, LPARAM)
{
    MessageBox(_T("Testing Text"));
}