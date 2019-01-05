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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver statistics holder
class sxCDriverStatistics : public sxITSingleton<sxCDriverStatistics>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCDriverStatistics();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCDriverStatistics();

	//-------------------------------------------------------------------------------------------------------
	// Format statistics for display
	void Format(ostringstream& a_rstrMessage);

	//-------------------------------------------------------------------------------------------------------
	// Reset counter
	void Reset();

	//-------------------------------------------------------------------------------------------------------
	// Set client fps
	void SetClientFps(sxInt a_iValue) {m_iaClientFps = a_iValue;}

	//-------------------------------------------------------------------------------------------------------
	// Set server fps
	void SetServerFps(sxInt a_iValue) {m_iServerFps = a_iValue;}

	//-------------------------------------------------------------------------------------------------------
	// Set Gpu maximal fps
	void SetGpuFps(sxInt a_iValue) {m_iGpuFps = a_iValue;}

	//-------------------------------------------------------------------------------------------------------
	// Set Command buffer size in bytes
	void SetCommandBufferSize(sxInt a_iValue) {m_iCommandBufferSize = a_iValue;}

	//-------------------------------------------------------------------------------------------------------
	// Increment built commands per frame count
	void IncBuiltCommandsPerFrame() {m_iBuiltCommandsPerFrame++;}

	//-------------------------------------------------------------------------------------------------------
	// Increment dispatched commands per frame count
	void IncDispatchedCommandsPerFrame() {m_iDispatchedCommandsPerFrame++;}

	//-------------------------------------------------------------------------------------------------------
	// Increment d3d9 calls per frame count
	void IncD3d9CallsPerFrame() {m_iD3d9CallsPerFrame++;}

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Client fps
	sxCAtomicInt m_iaClientFps;

	// Server fps
	sxInt m_iServerFps;

	// Gpu optimal fps
	sxInt m_iGpuFps;

	// Command buffer size in bytes
	sxInt m_iCommandBufferSize;

	// Built commands per frame
	sxInt m_iBuiltCommandsPerFrame;

	// Dispatched commands per frame
	sxInt m_iDispatchedCommandsPerFrame;

	// D3d9 calls per frame
	sxInt m_iD3d9CallsPerFrame;
};