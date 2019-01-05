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

#include "sxDriver/Server/Interface/sxIServerCommand.h"

class sxICommand;
class sxIState;
class sxICommandData;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement commands, states and command data access functions. This is the only friend class that can access to 
// commands, states and command data, so every locker object should inherit this class.
class _sxCCommandLocker : private sxINonCopyable
{
protected:

	//-------------------------------------------------------------------------------------------------------
	// Get the command data available for read only operations
	sxICommandData const& GetReadOnlyData(sxICommand const& a_rCommand) const;

	//-------------------------------------------------------------------------------------------------------
	// Get the command data available for read-write operations
	sxICommandData& GrabReadWriteData(sxICommand& a_rCommand, sxBool a_bDiscard) const;

	//-------------------------------------------------------------------------------------------------------
	// Get the command data (of a state) available for read only operations
	sxICommandData const& GetReadOnlyData(sxIState const& a_rState) const;

	//-------------------------------------------------------------------------------------------------------
	// Get the command data (of a state) available for read-write operations
	sxICommandData& GrabReadWriteData(sxIState& a_rState, sxBool a_bDiscard) const;

	//-------------------------------------------------------------------------------------------------------
	// Acquire command data read only access
	void AcquireReadOnlyAccess(sxICommandData const& a_rData) const;

	//-------------------------------------------------------------------------------------------------------
	// Release command data read only access
	void ReleaseReadOnlyAccess(sxICommandData const& a_rData) const;

	//-------------------------------------------------------------------------------------------------------
	// Acquire command data read-write access
	void AcquireReadWriteAccess(sxICommandData& a_rData) const;

	//-------------------------------------------------------------------------------------------------------
	// Release command data read-write access
	void ReleaseReadWriteAccess(sxICommandData& a_rData) const;

	//-------------------------------------------------------------------------------------------------------
	// Get the server command of a command object
	sxIServerCommand& GetServerCommand(sxICommand const& a_rCommand) const;

	//-------------------------------------------------------------------------------------------------------
	// Get the server command of a state object
	sxIServerCommand& GetServerCommand(sxIState const& a_rState) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a read only locker object. It provides a const access on the locked data in order to ensure at 
// compilation time that no "write" function is called. It is based on the raii pattern in order to ensure 
// locking-unlocking symmetry.
template <typename t_CommandData>
class sxCTReadOnlyLocker : protected _sxCCommandLocker
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a command
	sxCTReadOnlyLocker(sxICommand const& a_rCommand) :
		m_spCommandData(&GetReadOnlyData(a_rCommand))
	{
		AcquireReadOnlyAccess(m_spCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a state
	sxCTReadOnlyLocker(sxIState const& a_rState) :
		m_spCommandData(&GetReadOnlyData(a_rState))
	{
		AcquireReadOnlyAccess(m_spCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Releases the locked data
	~sxCTReadOnlyLocker()
	{
		ReleaseReadOnlyAccess(m_spCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator. Releases previous data and acquire the new one.
	void operator = (sxCTReadOnlyLocker<t_CommandData> const& a_rCommandLocker)
	{
		// Release the previously hold data
		ReleaseReadOnlyAccess(m_spCommandData);

		// Switch to the new one
		m_spCommandData = a_rCommandLocker.m_spCommandData;

		// Acquire it
		AcquireReadOnlyAccess(m_spCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Const command data getter
	t_CommandData const& GetData() const {return (t_CommandData&)*m_spCommandData;}

protected:

	//-------------------------------------------------------------------------------------------------------
	// Default constructor. Should not be used
	sxCTReadOnlyLocker() { sxAssertUnreachableCode(); }

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a command data. Used in a copy constructor in some cases
	sxCTReadOnlyLocker(sxICommandData const& a_rCommandData) :
		m_spCommandData(&a_rCommandData)
	{
		AcquireReadOnlyAccess(a_rCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor. Lock again the data of a sxCTReadOnlyLocker.
	sxCTReadOnlyLocker(sxCTReadOnlyLocker<t_CommandData> const& a_rCommandLocker) :
		m_spCommandData(a_rCommandLocker.m_spCommandData)
	{
		AcquireReadOnlyAccess(m_spCommandData);
	}

private:

	// Pointer to the const locked data. Uses a smart pointer as it holds a reference on the data when used inside
	// the driver.
	sxCTIntrusivePtr<sxICommandData const>::sxCType m_spCommandData;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a read-write locker object. It provides a non-const access on the locked data such that read
// and write calls can be done.It is based on the raii pattern in order to ensure locking-unlocking symmetry.
template <typename t_CommandData>
class sxCTReadWriteLocker : public _sxCCommandLocker
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a command. If a_bDiscard is true, then the locked command data is cleared.
	// Otherwise, previously written data value are maintained which can require a deep copy.
	sxCTReadWriteLocker(sxICommand& a_rCommand, sxBool a_bDiscard = true) :
		m_spCommandData(&GrabReadWriteData(a_rCommand, a_bDiscard))
	{
		AcquireReadWriteAccess(m_spCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a state. If a_bDiscard is true, then the locked state data is cleared.
	// Otherwise, previously written data value are maintained which can require a deep copy.
	sxCTReadWriteLocker(sxIState& a_rState, sxBool a_bDiscard = true) :
		m_spCommandData(&GrabReadWriteData(a_rState, a_bDiscard))
	{
		AcquireReadWriteAccess(m_spCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Releases the locked data
	~sxCTReadWriteLocker()
	{
		ReleaseReadWriteAccess(m_spCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Non-const command data getter
	t_CommandData& GetData() const {return (t_CommandData&)*m_spCommandData;}

protected:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a command data. Used in a copy constructor in some cases
	sxCTReadWriteLocker(sxICommandData& a_rCommandData) :
		m_spCommandData(a_rCommandData)
	{
		AcquireReadWriteAccess(a_rCommandData);
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor. Lock again the data of a sxCTReadOnlyLocker.
	sxCTReadWriteLocker(sxCTReadWriteLocker<t_CommandData> const& a_rCommandLocker) :
		m_spCommandData(&a_rCommandLocker.m_rCommandData)
	{
		AcquireReadWriteAccess(m_spCommandData);
	}

private:

	// Pointer to the locked data. Uses a smart pointer as it holds a reference on the data when used inside
	// the driver.
	sxCTIntrusivePtr<sxICommandData>::sxCType m_spCommandData;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a read-only locker object and holds the related server command. This class is used in the driver
// everywhere a reference to a command data and a server command is needed, ie: the state cache, the command 
// buffer...
class sxCReadOnlyLockerHolder : public sxCTReadOnlyLocker<sxICommandData>
{
protected:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a command and grab its server version.
	sxCReadOnlyLockerHolder(sxICommand const& a_rCommand) :
		sxCTReadOnlyLocker<sxICommandData>(a_rCommand),
		m_spServerCommand(sxCTReadOnlyLocker<sxICommandData>::GetServerCommand(a_rCommand))
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Lock a state and grab its server version.
	sxCReadOnlyLockerHolder(sxIState const& a_rState) :
		sxCTReadOnlyLocker<sxICommandData>(a_rState),
		m_spServerCommand(sxCTReadOnlyLocker<sxICommandData>::GetServerCommand(a_rState))
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize from a already acquired server command and data.
	sxCReadOnlyLockerHolder(	sxIServerCommand& a_rServerCommand,
								sxICommandData const& a_rCommandData) :
		sxCTReadOnlyLocker<sxICommandData>(a_rCommandData),
		m_spServerCommand(a_rServerCommand)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor.
	sxCReadOnlyLockerHolder(sxCReadOnlyLockerHolder const& a_rCommandLocker) :
		sxCTReadOnlyLocker<sxICommandData>(a_rCommandLocker),
		m_spServerCommand(a_rCommandLocker.m_spServerCommand)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Default constructor. Should not be used
	sxCReadOnlyLockerHolder()
	{
		sxAssertUnreachableCode();
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Releases the locked data
	~sxCReadOnlyLockerHolder()
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator
	void operator = (sxCReadOnlyLockerHolder const& a_rCommandLocker)
	{
		sxCTReadOnlyLocker<sxICommandData>::operator = (a_rCommandLocker);
		m_spServerCommand = a_rCommandLocker.m_spServerCommand;
	}

	//----------------------------------------------------------------------------------------------------------
	// Server command accessor
	sxIServerCommand& GetServerCommand() const {return m_spServerCommand;}

private:

	// Server command smart pointer. Maintains a reference count on the server object
	sxCTIntrusivePtr<sxIServerCommand>::sxCType m_spServerCommand;
};