//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//																											|
//								ShaderX7. Cross platform rendering thread. 2008.							|
//				Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.			|
//																											|
//	This program is free software. It is distributed in the hope that it will be useful, but without any	|
//	warranty, without even the implied warranty of merchantability or fitness for a particular purpose.		|
//																											|
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "sxKernel/sxPcHeader.h"
#include "sxCWindow.h"
#include "sxCControls.h"
#include "sxKernel/sxIMain.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCWindow::sxCWindow() :
	m_bActive(false) 
{
	// Logging
	sxLog("Creating application window");

	// Fetch application module instance
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Initialize WNDCLASS name.
	m_strRegisteredWndClass = L"sxCWindows Class Name";

	//-------------------------------------
	// Register window class

	// Fills WNDCLASSEX structure
	WNDCLASSEX oWndclass;
	sxMemZero(&oWndclass, sizeof(oWndclass));
	oWndclass.cbSize		= sizeof(WNDCLASSEX);
	oWndclass.style			= CS_CLASSDC;
	oWndclass.lpfnWndProc	= &sxCWindow::WindowProc;
	oWndclass.cbClsExtra	= 0;
	oWndclass.cbWndExtra	= 0;
 	oWndclass.hInstance	= hInstance;
 	oWndclass.hIcon	= NULL;
 	oWndclass.hIconSm		= NULL;
	oWndclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	oWndclass.hbrBackground	= NULL;
	oWndclass.lpszMenuName	= NULL;
	oWndclass.lpszClassName	= m_strRegisteredWndClass.c_str();
	
	// Try to register the class
	sxValidate(RegisterClassEx(&oWndclass) != 0, "Unable to register window class");

	//-------------------------------------
	// Create Window

	// Initialize application style in windowed mode. Full screen mode switch is done be the graphic module.
	sxUInt uiStyle = WS_POPUP | WS_OVERLAPPEDWINDOW;

	// Initialize default application size
	RECT oRect = {0, 0, 800, 600};

	// Calculates the required size of the window rectangle, based on the desired client-rectangle size
	sxValidate(AdjustWindowRect(&oRect, uiStyle, FALSE) != 0, "Unable to calculate window size");

	// Call windows creation function
	m_hDisplayHandle = CreateWindow(m_strRegisteredWndClass.c_str(),
									L"sxDeviceThread",
									uiStyle,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									oRect.right - oRect.left,
									oRect.bottom - oRect.top,
									GetDesktopWindow(),
									NULL,
									NULL,
									NULL);
	// Test for success
	sxValidate(m_hDisplayHandle != NULL, "Unable to create window");

	//-------------------------------------
	// Show window

	// Sets the visibility state of the window. SW_SHOWNORMAL activates and displays the window.
	ShowWindow(m_hDisplayHandle, SW_SHOWNORMAL);

	// Updates the client area of the specified window by sending a WM_PAINT message to the window 
	// if the window's update region is not empty. 
	sxValidate(UpdateWindow(m_hDisplayHandle) != 0, "Unable to update window region");
}

//-----------------------------------------------------------------------------------------------------------
//
sxCWindow::~sxCWindow()
{
	// Destroy the window
	sxValidate(DestroyWindow(m_hDisplayHandle) != 0, "Unable to destroy window");

	// Unregister the window class
	sxValidate(	UnregisterClass(m_strRegisteredWndClass.c_str(), 
								GetModuleHandle(NULL)) != 0,
				"Unable to unregister window class");
}

//-----------------------------------------------------------------------------------------------------------
//
LRESULT CALLBACK sxCWindow::WindowProc( HWND a_hWnd, UINT a_uiMsg, WPARAM a_wParam, LPARAM a_lParam )
{
	switch(a_uiMsg)
	{
		//-------------------------------------
		// Keyboard messages
		case WM_KEYDOWN:
		{
			// Forward control
			if(sxCControls::IsInstanceValid())
			{
				sxCControls::GetInstance().Control(a_wParam, false);
			}
			return 0L;
		}
		case WM_KEYUP:
		{
			if(a_wParam == VK_ESCAPE)
			{
				PostQuitMessage(EXIT_SUCCESS);
			}

			// Forward control
			if(sxCControls::IsInstanceValid())
			{
				sxCControls::GetInstance().Control(a_wParam, true);
			}
			
			return 0L;
		}

		case WM_ACTIVATEAPP:
		{
			sxCWindow::GetInstance().OnActive(LOWORD(a_wParam) != WA_INACTIVE);
			return 0L;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(EXIT_SUCCESS);
			return 0L;
		}

		case WM_SIZE:
		{
			sxIMain::GetInstance().Resize(LOWORD(a_lParam), HIWORD(a_lParam));
			return 0L;
		}
	}

	// Called to process messages not handled by the message map.
	return DefWindowProc(a_hWnd, a_uiMsg, a_wParam, a_lParam);
}
