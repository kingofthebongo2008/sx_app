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

//-----------------------------------------------------------------------------------------------------------
// Enable the core debug objects so that a mirrored object which contains all of the properties of the 
// object can be viewed in the watch window. This #define must be done before the D3d9.h file
#ifdef __sxBuildDebug
#	define D3D_DEBUG_INFO
#endif // __sxBuildDebug

//-----------------------------------------------------------------------------------------------------------
// Define extended function call error checking
#ifdef __sxBuildAssert
#	define __sxD3d9DriverAPIErrorCheck
#endif // __sxBuildAssert

//-----------------------------------------------------------------------------------------------------------
// Include D3d9 SDK files

// Include d3d9 base header
#include "d3d9.h"

// Include d3dx base header
#include "d3dx9.h"

#ifdef __sxD3d9DriverAPIErrorCheck
// Includes dx error management header file
#	include "dxerr.h"
#endif // __sxD3d9DriverAPIErrorCheck

// Driver statistics that count d3d9 calls
#include "sxDriver/Client/sxCDriverStatistics.h"

//-----------------------------------------------------------------------------------------------------------
// Define D3d9 SmartPtrs 
typedef sxCTComPtr<IUnknown>::sxCType sxIUnknownPtr;
typedef sxCTComPtr<IDirect3D9>::sxCType sxIDirect3D9Ptr;
typedef sxCTComPtr<IDirect3DDevice9>::sxCType sxIDirect3DDevice9Ptr;
typedef sxCTComPtr<IDirect3DSwapChain9>::sxCType sxIDirect3DSwapChain9Ptr;
typedef sxCTComPtr<IDirect3DVertexDeclaration9>::sxCType sxIDirect3DVertexDeclaration9Ptr;
typedef sxCTComPtr<IDirect3DSurface9>::sxCType sxIDirect3DSurface9Ptr;
typedef sxCTComPtr<IDirect3DBaseTexture9>::sxCType sxIDirect3DBaseTexture9Ptr;
typedef sxCTComPtr<IDirect3DTexture9>::sxCType sxIDirect3DTexture9Ptr;
typedef sxCTComPtr<IDirect3DCubeTexture9>::sxCType sxIDirect3DCubeTexture9Ptr;
typedef sxCTComPtr<IDirect3DVolumeTexture9>::sxCType sxIDirect3DVolumeTexture9Ptr;
typedef sxCTComPtr<IDirect3DIndexBuffer9>::sxCType sxIDirect3DIndexBuffer9Ptr;
typedef sxCTComPtr<IDirect3DVertexBuffer9>::sxCType sxIDirect3DVertexBuffer9Ptr;
typedef sxCTComPtr<IDirect3DVertexShader9>::sxCType sxIDirect3DVertexShader9Ptr;
typedef sxCTComPtr<IDirect3DPixelShader9>::sxCType sxIDirect3DPixelShader9Ptr;
typedef sxCTComPtr<IDirect3DQuery9>::sxCType sxIDirect3DQuery9Ptr;

//-----------------------------------------------------------------------------------------------------------
// Define D3dx SmartPtrs 
typedef sxCTComPtr<ID3DXBuffer>::sxCType sxID3DXBufferPtr;
typedef sxCTComPtr<ID3DXConstantTable>::sxCType sxID3DXConstantTablePtr;
typedef sxCTComPtr<ID3DXFont>::sxCType sxID3DXFontPtr;

//=========================================================
// Implement extended function call error checking
//=========================================================
#ifdef __sxD3d9DriverAPIErrorCheck

// Define a D3d9 error message format
#define sxD3d9GetErrorString(x_hRes,x_msg,x_count) DXGetErrorDescription(x_hRes,x_msg,x_count)

// Define a D3d9 function call macro that tests return error code
#define sxD3d9Call(x_function)\
{\
	sxCDriverStatistics::GetInstance().IncD3d9CallsPerFrame();\
	HRESULT _hRes = x_function;\
    TCHAR   _msg[1024];\
    sxD3d9GetErrorString(_hRes,_msg,1024);\
	sxAssert(SUCCEEDED(_hRes), "%s\n%S", #x_function,_msg);\
}__noop()

// Define a D3d9 function call macro that tests return error code. In a device lost case, it notifies the renderer
#define sxD3d9CallHandleLost(x_rDevice, x_function)\
{\
	sxCDriverStatistics::GetInstance().IncD3d9CallsPerFrame();\
	HRESULT _hRes = rDevice.GetD3dDevice().x_function;\
	if(_hRes == D3DERR_DEVICELOST)\
	{\
		rDevice.ResetNeeded();\
	}\
	else\
	{\
        TCHAR   _msg[1024];\
        sxD3d9GetErrorString(_hRes,_msg,1024);\
		sxAssert(SUCCEEDED(_hRes), "%s\n%S", #x_function, _msg);\
	}\
}__noop()

#else // __sxD3d9DriverAPIErrorCheck

// Define a D3d9 error message format
#define sxD3d9GetErrorString(x_hRes) "D3d9 Error"

// Define a D3d9 function call macro that only calls x_function (no error checking if
// __sxKglD3d9DriverFunctionCallCheck is not defined)
#define sxD3d9Call(x_function)\
{\
	sxCDriverStatistics::GetInstance().IncD3d9CallsPerFrame();\
	x_function;\
}__noop()

// Define a D3d9 function call macro that tests return error code. In a device lost case, it notifies the renderer
#define sxD3d9CallHandleLost(x_rDevice, x_function)\
{\
	sxCDriverStatistics::GetInstance().IncD3d9CallsPerFrame();\
	HRESULT _hRes = x_rDevice.GetD3dDevice().x_function;\
	if(_hRes == D3DERR_DEVICELOST) { rDevice.ResetNeeded(); }\
}__noop()

#endif // __sxD3d9DriverAPIErrorCheck

//----------------------------------------------------------------------------------------------------------------------
// sxFloat to a sxUInt hexadecimal value
inline sxUInt sxFloatToHex(sxFloat a_fVal)
{
	union
	{
		sxFloat m_fValue;
		sxUInt m_uiValue;
	} oFloatUInt;

	oFloatUInt.m_fValue = a_fVal;
	return oFloatUInt.m_uiValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Encapsulate D3DPRESENT_PARAMETERS initialization
class sxCD3d9PresentParameter
{
public:

	//-------------------------------------------------------------------------------------------------------
	//  Constructor from render target settings
	sxCD3d9PresentParameter(IDirect3D9& a_rD3d9Object,
							sxUInt a_uiWidth,
							sxUInt a_uiHeight,
							HWND a_hDisplayHandle);

	//-------------------------------------------------------------------------------------------------------
	//  D3DPRESENT_PARAMETERS* automatic cast operator
	operator D3DPRESENT_PARAMETERS* () {return &m_oD3d9PresentParameter;};

private:

	//  The present parameter managed by the class
	D3DPRESENT_PARAMETERS m_oD3d9PresentParameter;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a helper macro to initialize smart pointers when filled in a get function with &sxCPointee

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// _sxCD3d9SmartPointerProxy
template <typename t_SmartPointer>
class _sxCD3d9SmartPointerProxy : private sxINonCopyable
{
public:

	// Fetch the pointee object type through the smart pointer type
	typedef typename t_SmartPointer::sxCPointee sxCPointee;

	//-------------------------------------------------------------------------------------------------------
	// Construct the proxy object
	_sxCD3d9SmartPointerProxy(t_SmartPointer& a_rSmartPtr) :
		m_rSmartPtr(a_rSmartPtr),
		m_pPointee(NULL)
	{}

	//-------------------------------------------------------------------------------------------------------
	// Destructor, set the smart pointer to its new value
	~_sxCD3d9SmartPointerProxy()
	{
		// Set the smart pointer to m_pPointee 
		m_rSmartPtr = m_pPointee;

		// D3d9 automatically adds a reference to the pointer, so we need to release it
		if(m_pPointee != NULL)
		{
			m_pPointee->Release();
		}
	}

	//-------------------------------------------------------------------------------------------------------
	// Get access to the proxy object
	sxCPointee* GetProxyPointer() {return &m_pPointee;}

private:

	// The smart pointer the proxy is working on
	t_SmartPointer& m_rSmartPtr;

	// Temporary proxy value
	sxCPointee m_pPointee;
};

//-----------------------------------------------------------------------------------------------------------
//  Create a _sxCD3d9SmartPointerProxy object. Hides template parameter complexity for the constructor
template <typename t_SmartPointer>
inline _sxCD3d9SmartPointerProxy<t_SmartPointer> _sxCreateD3d9SmartPointerProxy(t_SmartPointer& a_rPointer)
{
	return _sxCD3d9SmartPointerProxy<t_SmartPointer>(a_rPointer);
}

//-----------------------------------------------------------------------------------------------------------
//  Allows smart pointer initialization in a get function (D3d like function)
#define sxGetD3d9SmartModifier(x_rspPointer) _sxCreateD3d9SmartPointerProxy(x_rspPointer).GetProxyPointer()
