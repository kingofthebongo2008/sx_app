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

#include "sxDriver/Client/sxCCommandBuffer.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Manage application controls available to the user
class sxCControls : public sxITSingleton<sxCControls>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCControls();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCControls();

	//-------------------------------------------------------------------------------------------------------
	// Reset controls
	void Reset();

	//-------------------------------------------------------------------------------------------------------
	// Format control message
	void Format(ostringstream& a_rstrMessage);

    // Format control message
    void Format(wostringstream& a_rstrMessage);

	//-------------------------------------------------------------------------------------------------------
	// Modify control
	void Control(sxUInt a_uiKey, sxBool a_bKeyUp);

	//-------------------------------------------------------------------------------------------------------
	// Set/Get the light count
	void SetLightCount(sxUInt a_uiLightCount) {m_iaLightCount = a_uiLightCount;}
	sxUInt GetLightCount() const {return m_iaLightCount;}
	sxUInt GetMaxLightCount() const {return 75;}

	// EnableDisable dummy lights rendering
	void EnableDummyLightRendering(sxBool a_bValue) {m_iaRenderDummyLights = a_bValue;}
	sxBool IsDummyLightRenderingEnabled() const { return m_iaRenderDummyLights != 0;}

	//-------------------------------------------------------------------------------------------------------
	// Set/Get command buffer working mode
	void SetBufferingMode(sxEBufferingMode::sxEValue a_eValue);
	sxEBufferingMode::sxEValue GetBufferingMode() const;

	//-------------------------------------------------------------------------------------------------------
	// Increment/Decrement/Get main thread delay
	void IncMainThreadDelay();
	void DecMainThreadDelay();
	sxUInt GetMainThreadDelay() const {return m_iaMainThreadDelay;}

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Attributes are using atomic integers as they are read/writen from different threads simultaneously.
	// This is a hack in oder to simplify demo code. This isn't the right solution as this file should no be 
	// included in the driver. The right solution would rather be to have text rendering features available 
	// from the client side.

	// The number of lights
	sxCAtomicInt m_iaLightCount;

	// The command buffer working mode
	sxCAtomicInt m_iaBufferingMode;

	// Should render dummy lights
	sxCAtomicInt m_iaRenderDummyLights;

	// Main thread update loop delay
	sxCAtomicInt m_iaMainThreadDelay;
};