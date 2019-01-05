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

#include "sxKernel/Thread/sxCMutex.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface of a all D3d9 objects that needs to warn for Reset function calls.
class sxID3d9ResetWarner : public sxITSingleton<sxID3d9ResetWarner>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor.
	sxID3d9ResetWarner();

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Checks that all resources are released on destruction
	virtual ~sxID3d9ResetWarner();

protected:

	//-------------------------------------------------------------------------------------------------------
	// Call PreReset on all sxID3d9ResetListener objects
	void SendPreReset();

	//-------------------------------------------------------------------------------------------------------
	// Call PostResetSucceed on all sxID3d9ResetListener objects
	void SendPostResetSucceed();

	//-------------------------------------------------------------------------------------------------------
	// Call PostResetFailed on all sxID3d9ResetListener objects
	void SendPostResetFailed();

private:

	//-------------------------------------------------------------------------------------------------------
	//
	inline void RegisterResetListener(class sxID3d9ResetListener& a_rResource)
	{
		sxCMutexLocker oLock(m_oListMutex);

		// Append the resource to the list
		m_lVideoMemoryResources.push_front(&a_rResource);
	}

	//-------------------------------------------------------------------------------------------------------
	//
	inline void UnregisterResetListener(class sxID3d9ResetListener& a_rResource)
	{
		sxCMutexLocker oLock(m_oListMutex);

		// Remove the resource 
		m_lVideoMemoryResources.remove(&a_rResource);
	}

private:

	// sxID3d9ResetListener is friend in order to call registration functions
	friend class sxID3d9ResetListener;

	// Type of video memory resource list
	typedef list<class sxID3d9ResetListener*> sxID3d9ResetAwareList;

	// Stores the list of all objects that handle video memory
	sxID3d9ResetAwareList m_lVideoMemoryResources;

	// Video resource list protection
	sxCMutex m_oListMutex;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface of a WinDd9 object that needs to listen for Reset function calls.
class sxID3d9ResetListener
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxID3d9ResetListener();

	//------------------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxID3d9ResetListener();

private:

	// sxID3d9ResetWarner is friend in order to call ***Reset functions
	friend class sxID3d9ResetWarner;

	//------------------------------------------------------------------------------------------------------------------
	// Function called before a reset
	virtual void PreReset() {}

	//------------------------------------------------------------------------------------------------------------------
	// Function called after a reset succeed
	virtual void PostResetSucceed() {}

	//------------------------------------------------------------------------------------------------------------------
	// Function called after a reset failed
	virtual void PostResetFailed() {}
};

