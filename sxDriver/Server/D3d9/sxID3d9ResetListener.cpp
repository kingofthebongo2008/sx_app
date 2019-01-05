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
#include "sxID3d9ResetListener.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxID3d9ResetWarner

//-----------------------------------------------------------------------------------------------------------
//
sxID3d9ResetWarner::sxID3d9ResetWarner()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxID3d9ResetWarner::~sxID3d9ResetWarner()
{
	// Ensure that no resource is still used by the application
	sxAssert(	m_lVideoMemoryResources.empty(),
				"%d video memory resources have not been destroyed", m_lVideoMemoryResources.size());
}

//-----------------------------------------------------------------------------------------------------------
//
void sxID3d9ResetWarner::SendPreReset()
{
	sxCMutexLocker oLock(m_oListMutex);

	// Iterate through all registered resource and call the release functions
	sxForEachCallFunctor(	m_lVideoMemoryResources,
							mem_fun(&sxID3d9ResetListener::PreReset));
}

//-----------------------------------------------------------------------------------------------------------
//
void sxID3d9ResetWarner::SendPostResetSucceed()
{
	sxCMutexLocker oLock(m_oListMutex);

	// Iterate through all registered resource and call the release functions
	sxForEachCallFunctor(	m_lVideoMemoryResources,
							mem_fun(&sxID3d9ResetListener::PostResetSucceed));
}

//-----------------------------------------------------------------------------------------------------------
//
void sxID3d9ResetWarner::SendPostResetFailed()
{
	sxCMutexLocker oLock(m_oListMutex);

	// Iterate through all registered resource and call the release functions
	sxForEachCallFunctor(	m_lVideoMemoryResources,
							mem_fun(&sxID3d9ResetListener::PostResetFailed));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxID3d9ResetListener 

//-----------------------------------------------------------------------------------------------------------
//
sxID3d9ResetListener::sxID3d9ResetListener() 
{ 
	sxID3d9ResetWarner::GetInstance().RegisterResetListener(rThis);
}

//-----------------------------------------------------------------------------------------------------------
//
sxID3d9ResetListener::~sxID3d9ResetListener() 
{ 
	sxID3d9ResetWarner::GetInstance().UnregisterResetListener(rThis);
}

