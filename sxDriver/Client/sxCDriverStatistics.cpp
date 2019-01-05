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
#include "sxCDriverStatistics.h"

//-----------------------------------------------------------------------------------------------------------
// Register sxCDriverStatistics object
sxRegisterSingleton(sxCDriverStatistics,	sxERegisterPrimaryPriority::eNormal,
											sxERegisterSecondaryPriority::eNormal);

//-----------------------------------------------------------------------------------------------------------
//
sxCDriverStatistics::sxCDriverStatistics() :
	m_iaClientFps(0),
	m_iServerFps(-1),
	m_iGpuFps(-1),
	m_iCommandBufferSize(0),
	m_iBuiltCommandsPerFrame(0),
	m_iDispatchedCommandsPerFrame(0),
	m_iD3d9CallsPerFrame(0)
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCDriverStatistics::Reset()
{
	m_iaClientFps = 0;
	m_iServerFps = -1;
	m_iGpuFps = -1;
	m_iCommandBufferSize = 0;
	m_iBuiltCommandsPerFrame = 0;
	m_iDispatchedCommandsPerFrame = 0;
	m_iD3d9CallsPerFrame = 0;
}

//-----------------------------------------------------------------------------------------------------------
//
sxCDriverStatistics::~sxCDriverStatistics()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCDriverStatistics::Format(ostringstream& a_rstrMessage)
{
	a_rstrMessage << "-----------------\n";
	a_rstrMessage << "Driver statistics\n";

	// Add Client fps
	a_rstrMessage << "-Client thread fps: " << m_iaClientFps.GetValue() << '\n';

	// Add Server fps
	a_rstrMessage << "-Server thread fps: ";
	if(m_iServerFps != -1)
	{
		a_rstrMessage << m_iServerFps;
	}
	else
	{
		a_rstrMessage << "No loop detected";
	}
	a_rstrMessage << '\n';

	// Add Gpu fps
	a_rstrMessage << "-Gpu fps: ";
	if(m_iGpuFps != -1)
	{
		a_rstrMessage << m_iGpuFps;
	}
	else
	{
		a_rstrMessage << "Unavailable";
	}
	a_rstrMessage << '\n';

	// Add built commands
	a_rstrMessage << "-Command buffer size: " << m_iCommandBufferSize << " Bytes" << '\n';

	// Add built commands
	a_rstrMessage << "-Built driver commands: " << m_iBuiltCommandsPerFrame << '\n';
	m_iBuiltCommandsPerFrame = 0;

	// Add dispatched commands
	a_rstrMessage << "-Dispatched driver commands: " << m_iDispatchedCommandsPerFrame << '\n';
	m_iDispatchedCommandsPerFrame = 0;

	// Add d3d9 calls and reset call count
	a_rstrMessage << "-D3d9 calls: " << m_iD3d9CallsPerFrame << '\n';
	m_iD3d9CallsPerFrame = 0;
}
