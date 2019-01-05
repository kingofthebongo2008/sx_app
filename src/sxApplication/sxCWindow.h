//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#pragma once

#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application main window.
class sxCWindow : public sxITSingleton<sxCWindow>
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCWindow();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCWindow();

	//-------------------------------------------------------------------------------------------------------
	// Test if window is active
	sxBool IsActive() const {return m_bActive;}

	//-------------------------------------------------------------------------------------------------------
	// Get window display handle
	HWND GetDisplayHandle() const {return m_hDisplayHandle;}

private:

	//-------------------------------------------------------------------------------------------------------
	// Windows message procedure
	static LRESULT CALLBACK WindowProc( HWND a_hWnd, UINT a_uiMsg, WPARAM a_wParam, LPARAM a_lParam );

	//-------------------------------------------------------------------------------------------------------
	//  Set application active flag
	void OnActive(sxBool a_bActive) {m_bActive = a_bActive;}

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Name of the registered WNDCLASS
	wstring m_strRegisteredWndClass;

	// Application handle
	HWND m_hDisplayHandle;

	// Stores if the application is active (has focus)
	sxBool m_bActive;
};
