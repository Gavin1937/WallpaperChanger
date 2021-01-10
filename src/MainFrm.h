// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__68498576_2038_4894_BD6F_320C9AD1BDD7__INCLUDED_)
#define AFX_MAINFRM_H__68498576_2038_4894_BD6F_320C9AD1BDD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "akTrayIcon.h"


class CMainFrame : public CFrameWnd
{
	
// protected: // create from serialization only
public:
	CMainFrame(const UINT& trayIconID);
	// DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    
    afx_msg LRESULT OnTrayIconNotify(WPARAM, LPARAM);
    
	DECLARE_MESSAGE_MAP()

private:
	BOOL TrayMessage( DWORD dwMessage);
    CakTrayIcon m_TrayIcon;
    UINT m_TrayIconID;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__68498576_2038_4894_BD6F_320C9AD1BDD7__INCLUDED_)
