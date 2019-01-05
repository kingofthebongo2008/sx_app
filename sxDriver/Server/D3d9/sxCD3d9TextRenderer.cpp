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
#include "sxCD3d9TextRenderer.h"
#include "sxCD3d9Device.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9TextRenderer::sxCD3d9TextRenderer()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCD3d9TextRenderer::~sxCD3d9TextRenderer()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCD3d9TextRenderer::Dispatch(	sxCD3d9Device& a_rDevice,
									sxCharCPtr a_szText,
									sxCFloat3 const& a_rf3Color,
									sxBool a_bBottom,
									sxBool a_bCentered)
{
	// Compute character count
	sxUInt const uiChracterCount = strlen(a_szText);

	// Early out  if the message is empty
	if(uiChracterCount == 0)
	{
		return;
	}

	// Build the font if it isn't done yet
	if(m_spFont == NULL)
	{
		// Create the d3dx font object
		sxD3d9Call(D3DXCreateFont(	&a_rDevice.GetD3dDevice(),
									18,
									0,
									FW_NORMAL,
									1,
									FALSE,
									DEFAULT_CHARSET,
									OUT_DEFAULT_PRECIS,
									DEFAULT_QUALITY,
									DEFAULT_PITCH | FF_DONTCARE,
									"courier new",
									sxGetD3d9SmartModifier(m_spFont)));

		// Prepare a string with all the characters that'll be used
		sxCharCPtr szCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 %-+/*=.,;:";
		
		// Prepare character fonts
		sxD3d9Call(m_spFont->PreloadText(szCharacters, strlen(szCharacters)));
	}

	// Create a rect of the size of the render target
	RECT oRect =
	{	
		0,
		0,
		a_rDevice.GetDeviceWidth(),
		a_rDevice.GetDeviceHeight()
	};

	// Build D3d color
	D3DCOLOR oColor = D3DCOLOR_COLORVALUE(a_rf3Color.m_fX, a_rf3Color.m_fY, a_rf3Color.m_fZ, 1.f);

	// Draw the text
	sxValidateNoText(	m_spFont->DrawText(	NULL,
											a_szText,
											uiChracterCount,
											&oRect,
												(a_bBottom ? DT_BOTTOM : DT_TOP)
											|	(a_bCentered ? DT_CENTER : 0)
											|	DT_LEFT,
											oColor)
						!= 0);
}

//------------------------------------------------------------------------------------------------------------------
//
void sxCD3d9TextRenderer::PreReset()
{
	if(m_spFont != NULL)
	{
		sxD3d9Call(m_spFont->OnLostDevice());
	}
}

//------------------------------------------------------------------------------------------------------------------
//
void sxCD3d9TextRenderer::PostResetSucceed()
{
	if(m_spFont != NULL)
	{
		sxD3d9Call(m_spFont->OnResetDevice());
	}
}
