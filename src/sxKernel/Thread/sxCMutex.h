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

#include <windows.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutex object, encapsulate os specific mutex object. This mutex can only be locked using the sxCMutexLocker 
// raii tool class.
class sxCMutex : private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize un unlocked mutex
	sxCMutex();

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Delete un unlocked mutex
	~sxCMutex();

protected:

	// Raii scope lock object
	template <sxBool> friend class sxCTMutexLocker;

	//-------------------------------------------------------------------------------------------------------
	// Lock this mutex
	void Lock();

	//-------------------------------------------------------------------------------------------------------
	// Try to lock this mutex without waiting for its ownership. Return true if lock succeeded, false otherwise
	sxBool TryLock();

	//-------------------------------------------------------------------------------------------------------
	// Unlock this mutex
	void Unlock();

private:

	//! CRITICAL_SECTION object used as a mutex
	CRITICAL_SECTION m_oCriticalSection;

	//! Locked status used for assertions only
	sxIfBuildAssertion(sxBool m_bLocked);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutex raii locker object, used to manipulate mutex objects. The template t_bTry arguments specifies wheter
// the locker should only try to lock rahter than wait.
template <sxBool t_bTry>
class sxCTMutexLocker : private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Construct a mutex locker object. Lock a_rMutex according to a_bLockOnCreate value.
	sxCTMutexLocker(sxCMutex& a_rMutex, sxBool a_bLockOnCreate = true) :
		m_rMutex(a_rMutex),
		m_bLocked(a_bLockOnCreate)
	{
		// Early out if not lock required
		if(a_bLockOnCreate == false)
		{
			return;
		}

		// Lock or try lock according to template argument
		if(t_bTry == true)
		{
			m_bLocked = m_rMutex.TryLock();
		}
		else
		{
			m_rMutex.Lock();
		}
	}

	//-------------------------------------------------------------------------------------------------------
	// Destruct a locker object. Unlock the mutex if it is locked
	~sxCTMutexLocker()
	{
		// Only unlock locked mutex
		if(IsLocked() == true)
		{
			m_rMutex.Unlock();
		}
	}

	//-------------------------------------------------------------------------------------------------------
	// Lock this mutex
	void Lock()
	{
		sxAssert(m_bLocked == false, "Mutex scope is already locked");

		// Lock the mutex and modify internal status
		m_rMutex.Lock();
		m_bLocked = true;
	}

	//-------------------------------------------------------------------------------------------------------
	// Try to lock this mutex without waiting for its ownership. Return true if lock succeeded, false otherwise
	sxBool TryLock()
	{
		sxAssert(m_bLocked == false, "Mutex scope is already locked");

		if(m_rMutex.TryLock())
		{
			// Lock succeeded
			m_bLocked = true;
		}

		return m_bLocked;
	}

	//-------------------------------------------------------------------------------------------------------
	// Unlock this mutex
	void Unlock()
	{
		sxAssert(m_bLocked == true, "Mutex scope isn't locked");
		m_rMutex.Unlock();
		m_bLocked = false;
	}

	//-------------------------------------------------------------------------------------------------------
	// Get if mutex is locked by this locker
	sxBool IsLocked()
	{
		return m_bLocked;
	}

private:

	//! The mutex object to monitor
	sxCMutex& m_rMutex;

	//! Lock status
	sxBool m_bLocked;
};

// Type of the standard mutex locking concept
typedef sxCTMutexLocker<false> sxCMutexLocker;

// Type of the try lock concept
typedef sxCTMutexLocker<true> sxCMutexTryLocker;
