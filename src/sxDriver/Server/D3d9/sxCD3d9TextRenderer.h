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

#include "sxDriver/Server/Interface/sxIServerDevice.h"
#include "sxD3d9.h"
#include "sxID3d9ResetListener.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver D3d9 server text output object
class sxCD3d9TextRenderer : protected sxID3d9ResetListener, private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize the font
	sxCD3d9TextRenderer();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCD3d9TextRenderer();

	//-------------------------------------------------------------------------------------------------------
	// Render the specified text on the top left corner of the window
	void Dispatch(	class sxCD3d9Device& a_rDevice,
					sxWCharCPtr a_szText,
					sxCFloat3 const& a_rf3Color,
					sxBool a_bBottom = false,
					sxBool a_bCentered = false);

protected:

	//-------------------------------------------------------------------------------------------------------
	// Function called before a reset
	virtual void PreReset();

	//-------------------------------------------------------------------------------------------------------
	// Function called after a reset succeed
	virtual void PostResetSucceed();

private:

	// The D3d9x font object used for rendering
	sxID3DXFontPtr m_spFont;
};

