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

#include "sxDriver/Client/sxIState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render target state command data. Embeds render target sizes and clear color. This clear color information
// might not be a real-world case but rather demonstrates the one-to-many abstraction design.
class sxCRenderTargetStateData final : public sxIStateData
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxCRenderTargetStateData();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual clear function
	virtual void Clear();

	//-------------------------------------------------------------------------------------------------------
	// Implement virtual copy function
	virtual void Copy(sxICommandData const& a_rData);
	
	//-------------------------------------------------------------------------------------------------------
	// Width setter/getter
	void SetWidth(sxUInt a_uiValue) {m_uiWidth = a_uiValue;}
	sxUInt GetWidth() const {return m_uiWidth;}

	//-------------------------------------------------------------------------------------------------------
	// Height setter/getter
	void SetHeight(sxUInt a_uiValue) {m_uiHeight = a_uiValue;}
	sxUInt GetHeight() const {return m_uiHeight;}

	//-------------------------------------------------------------------------------------------------------
	// Clear color setter/getter
	void SetColor(sxCFloat4 const& a_f4Value) {m_f4Color = a_f4Value;}
	sxCFloat4 const& GetColor() const {return m_f4Color;}

private:

	// Render target width
	sxUInt m_uiWidth;

	// Render target height
	sxUInt m_uiHeight;

	// Render target clear color
	sxCFloat4 m_f4Color;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type of the sxCRenderTargetState object built base on the state wrapper helper
typedef sxCTStateWrapper<	sxCRenderTargetStateData,
							sxEStateType::eRenderTarget,
							sxEFactoryObject::eRenderTargetState> sxCRenderTargetState;
