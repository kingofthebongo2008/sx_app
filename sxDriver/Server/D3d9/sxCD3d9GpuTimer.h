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
// D3d9 Gpu timer. Measures the elapsed time on the Gpu between two Begin/End calls. It uses d3d9 queries to
// insert events in the d3d9 command buffer.
class sxCD3d9GpuTimer : public sxID3d9ResetListener
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCD3d9GpuTimer();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCD3d9GpuTimer();

	//-------------------------------------------------------------------------------------------------------
	// Begin timer. End() must be called before Begin could be called again.
	void Begin();
	
	//-------------------------------------------------------------------------------------------------------
	// End timer. Begin() must have been called before
	void End();

	//-------------------------------------------------------------------------------------------------------
	// Test if timer functionality is valid on the current gpu
	sxBool IsValid() const { return m_bValid; }

	//-------------------------------------------------------------------------------------------------------
	// Get the last measured elapsed time. Can only be called if timer is valid.
	sxFloat GetElapsedTime() const;

protected:

	//-------------------------------------------------------------------------------------------------------
	// Function called before a reset
	virtual void PreReset();

	//-------------------------------------------------------------------------------------------------------
	// Function called after a reset succeed
	virtual void PostResetSucceed();

private:

	// Timer begin query
	sxIDirect3DQuery9Ptr m_spTimeStampQueryBegin;

	// Timer end query
	sxIDirect3DQuery9Ptr m_spTimeStampQueryEnd;

	// Timer frequency query
	sxIDirect3DQuery9Ptr m_spTimeStampQueryFreq;

	// Last measured elapsed time
	sxFloat m_fElpasedTime;

	// Are queries issued
	sxBool m_bIssued;

	// Is timer valid
	sxBool m_bValid;

	// Has Begin() been called, and End() not yet
	sxBool m_bInsideBeginEnd;
};
