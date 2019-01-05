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
#include "sxCD3d9Device.h"
#include "sxDriver/Client/sxCDriverStatistics.h"
#include "sxKernel/sxIMain.h"
#include "sxApplication/sxCControls.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9Device::sxCD3d9Device() :
	m_hDeviceWindow(NULL),
	m_uiDeviceWidth(0),
	m_uiDeviceHeight(0),
	m_bInsideBeginEndScene(false),
	m_uiActiveLightCount(0),
	m_uiMaxActiveLights(0),
	m_uiMaxPrimitiveCount(0),
	m_uiMaxVertexIndex(0)
{
	// Logging
	sxLog("Instancing D3d9 driver server device");

	// Create direct 3D object. D3d automatically increment references, we must explicitly release it because
	// the smart pointer stores the real reference
	m_spD3dObject = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_spD3dObject == NULL)
	{
		sxLog("DirectX 9c Initialization failed");
		while(true);
	}
	m_spD3dObject->Release();

	//-------------------
	// Create D3d9 device

	// Fetch application handle
	m_hDeviceWindow = (HWND)sxIMain::GetInstance().GetApplicationHandle();
	sxAssert(m_hDeviceWindow, "Platform specific application handle must not be NULL");

	// Use the application handle real client size
	RECT oClientRect;
	sxValidate(GetClientRect(m_hDeviceWindow, &oClientRect) != 0, "GetClientRect failed");

	m_uiDeviceWidth = oClientRect.right - oClientRect.left;
	m_uiDeviceHeight = oClientRect.bottom - oClientRect.top;

	// Initialize present parameters
	sxCD3d9PresentParameter oPresent(m_spD3dObject, m_uiDeviceWidth, m_uiDeviceHeight, m_hDeviceWindow);

	// Assemble creation parameters
	sxUInt uiCreationFlags = 0;

	// Specifies hardware vertex processing.
	uiCreationFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// Indicates that the application requests Direct3D to be multi thread safe.
	//uiCreationFlags |= D3DCREATE_MULTITHREADED;

	// Create the device
	sxValidate(	SUCCEEDED(m_spD3dObject->CreateDevice(	D3DADAPTER_DEFAULT,
														D3DDEVTYPE_HAL,
														m_hDeviceWindow,
														uiCreationFlags,
														oPresent,
														sxGetD3d9SmartModifier(m_spD3dDevice))),
				"D3d9 Device creation failed");
	sxLog("D3d9 device initialized");

	//----------------------------------------------------------------------------
	// Call the reset function in order to always use the same initialization path
	ResetNeeded();

	//---------------------------------------
	// Initialize internal caps based on D3d9

	// Fetch caps
	D3DCAPS9 oCaps;
	sxD3d9Call(m_spD3dDevice->GetDeviceCaps(&oCaps));

	// Fills internal cpas
	m_uiMaxActiveLights = oCaps.MaxActiveLights;
	m_uiMaxPrimitiveCount = oCaps.MaxPrimitiveCount;
	m_uiMaxVertexIndex = oCaps.MaxVertexIndex;
}

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9Device::~sxCD3d9Device()
{
	sxLog("Releasing D3d9 server");

	// Release device first
	m_spD3dDevice = NULL;

	// Finally release d3d object
	m_spD3dObject = NULL;
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCD3d9Device::Reset()
{
	//------------------------------------------------------------------------
	// 1. Try to see if the device could be reseted using TestCooperativeLevel

	// Test if reset can be called
	HRESULT hTest = m_spD3dDevice->TestCooperativeLevel();

	// Test cooperative test result
	switch(hTest)
	{
		case D3D_OK:
		{
			// The device is operational and the calling application can continue. We're still going to do 
			// the reset cos we need to close this chapter by restoring video memory. 
		}
		break;
		case D3DERR_DEVICENOTRESET:
		{
			// If an application detects a lost device, it should pause and periodically call 
			// TestCooperativeLevel until it receives a return value of D3DERR_DEVICENOTRESET. Don't return 
			// but continue to the reset function
		}
		break;
		case D3DERR_DRIVERINTERNALERROR:
		case D3DERR_DEVICELOST:
		{
			// If the device is lost but cannot be restored at the current time, TestCooperativeLevel 
			// returns the D3DERR_DEVICELOST return code. 
			return false;
		}
		break;
		default:
		{
			sxAssert(false, sxD3d9GetErrorString(hTest));
			return false;
		}
		break;
	}

	//------------------------------
	// 2. Prepare present parameters
	sxCD3d9PresentParameter oPresent(m_spD3dObject, m_uiDeviceWidth, m_uiDeviceHeight, m_hDeviceWindow);

	//-------------------------------------------------
	// 3. PreReset : Release all video memory resources
	SendPreReset();

	//-----------------------
	// 4. Do the actual reset

	// Makes the reset call
	HRESULT hReset = m_spD3dDevice->Reset(oPresent);

	// Test reset result
	switch(hReset)
	{
		case D3D_OK:
		{
			//---------------------------------------------------------------
			// Reset Succeed : Restore video memory and re-init render states
			SendPostResetSucceed();

			//--------------------------
			// Initialize default states
			
			// Enable Z buffer
			sxD3d9Call(m_spD3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE));

			// Enable lighting
			sxD3d9Call(m_spD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE));

			// Reset current light count as the d3d9::reset has invalidated all states
			ResetActiveLightCount();

			// Internally flag as reseted
			Reseted();

			// Everything's fine
			return true;
		}
		break;
		default:
		{
			//-------------
			// Reset Failed
			SendPostResetFailed();

			// When IDirect3DDevice9::Reset fails, the only valid methods that can be called are
			// IDirect3DDevice9::Reset, IDirect3DDevice9::TestCooperativeLevel, and the various Release 
			// member functions. Calling any other method can result in an exception. 

			// Reset has failed, we can only exit and try again next frame. False is return in order to 
			// inform the application that no rendering can be done. 
			return false;
		}
		break;
	}
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9Device::SetDeviceWidth(sxUInt a_uiValue)
{
	// Detect a difference
	if(m_uiDeviceWidth != a_uiValue)
	{
		// Store the new value...
		m_uiDeviceWidth = a_uiValue;

		// ... and ask for the reset
		ResetNeeded();
	}
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9Device::SetDeviceHeight(sxUInt a_uiValue)
{
	// Detect a difference
	if(m_uiDeviceHeight != a_uiValue)
	{
		// Store the new value...
		m_uiDeviceHeight = a_uiValue;

		// ... and ask for the reset
		ResetNeeded();
	}
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9Device::BeginScene()
{
	// If not already inside the BeginScene-EndScene
	if(m_bInsideBeginEndScene == false)
	{
		// Flag as done
		m_bInsideBeginEndScene = true;

		// Begin timer as the d3d9 command buffer is starting
		m_oGpuTimer.Begin();
		
		// Notify device begin
		sxD3d9Call(m_spD3dDevice->BeginScene());
	}
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9Device::EndScene()
{
	// Test entry condition
	sxAssert(m_bInsideBeginEndScene == true, "Should not be called if BeginScene hasn't been called");

	// Display overlayed text
	OverlayText();

	//--------------
	// End Gpu timer
	m_oGpuTimer.End();

	//-------------------------
	// Now really end the frame

	// Reset flag
	m_bInsideBeginEndScene = false;
	
	// Release all references at the end of the frame as server objects and data might be destroyed
	m_oStateShadow.Release();

	// Notify device end
	sxD3d9Call(m_spD3dDevice->EndScene());
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9Device::ActivateLights(sxUInt a_uiLightCount)
{
	// Number of active lights
	sxUInt const uiActiveLightCount = GetActiveLightCount();

	// Fetch d3d9 device
	IDirect3DDevice9& rDevice = GetD3dDevice();

	// Enable required light count
	sxForEachUIntInRange(uiLight, uiActiveLightCount, a_uiLightCount)
	{
		sxD3d9Call(rDevice.LightEnable(uiLight, TRUE));
	}

	// Disable all other active lights
	sxForEachUIntInRange(uiLight, a_uiLightCount, uiActiveLightCount)
	{
		sxD3d9Call(rDevice.LightEnable(uiLight, FALSE));
	}

	// Update current active light count
	m_uiActiveLightCount = a_uiLightCount;
}

//-------------------------------------------------------------------------------------------------------
//
void sxCD3d9Device::OverlayText()
{
	// Get driver statistic object
	sxCDriverStatistics& rStatistics = sxCDriverStatistics::GetInstance();

	// Update gpu fps
	if(m_oGpuTimer.IsValid())
	{
		// Use the elapsed time computed by the gpu timer, whatever the measured frame was
		m_oGpuFpsCounter.Update(m_oGpuTimer.GetElapsedTime());

		// Set the statistics with the computed fps
		rStatistics.SetGpuFps(m_oGpuFpsCounter.GetFramePerSecond());
	}

	//---------------------------------
	// Overlay output text at frame end
	ostringstream strMessage;

	// Get a formated message from the current controls
	sxCControls::GetInstance().Format(strMessage);
	
	// Add a separator
	strMessage << "\n";

	// Get a formated message from the current statistics
	rStatistics.Format(strMessage);

	// Overlay output text at frame end
	m_oTextRenderer.Dispatch(	rThis,
								strMessage.str().c_str(),
								sxCFloat3(1,1,0));

	// Overlay golem text at frame end
	m_oTextRenderer.Dispatch(	rThis,
								"The Golem 3d model is available for download at www.elb-games.com",
								sxCFloat3(1,1,1),
								true,
								true);
}
