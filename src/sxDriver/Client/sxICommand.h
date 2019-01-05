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

#include "sxIObject.h"
#include "sxCCommandLocker.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver client command data object interface
class sxICommandData : public sxIIntrusiveCounter, private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxICommandData();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxICommandData();

	//-------------------------------------------------------------------------------------------------------
	// Pure virtual command data clear function
	virtual void Clear() = 0;

	//-------------------------------------------------------------------------------------------------------
	// Pure virtual command data copy function
	virtual void Copy(sxICommandData const& a_rData) = 0;

	//-------------------------------------------------------------------------------------------------------
	// Test if the data is acquired, either for read-only or read-write.
	sxBool IsAcquired()
	{
		// Counter can never be decremented twice has it can only be read-write acquired once.
		sxAssertNoText(m_iaAcquiredCounter >= -1);

		// Data is acquired if its counter has been incremented or decremented.
		return m_iaAcquiredCounter != 0;
	}

	//-------------------------------------------------------------------------------------------------------
	// Test if the data is acquired read-only.
	sxBool IsReadOnlyAcquired() const
	{
		// Counter can never be decremented twice has it can only be read-write acquired once.
		sxAssertNoText(m_iaAcquiredCounter >= -1);

		// Data is acquired for read-only if its counter has been incremented at least once.
		return m_iaAcquiredCounter > 0;
	}

	//-------------------------------------------------------------------------------------------------------
	// Test if the data is acquired for read-write.
	sxBool IsReadWriteAcquired() const
	{
		// Counter can never be decremented twice has it can only be read-write acquired once.
		sxAssertNoText(m_iaAcquiredCounter >= -1);

		// Data is acquired for read-write if its counter has been decremented once.
		return m_iaAcquiredCounter == -1;
	}

private:

	// _sxCCommandLocker is friend as it is the only class that can call acquisition functions
	friend class _sxCCommandLocker;

	//----------------------------------------------------------------------------------------------------------
	// Acquire the command data for read-only
	void AcquireReadOnlyAccess() const
	{
		sxAssertNoText(IsReadWriteAcquired() == false);
		m_iaAcquiredCounter++;
	}

	//----------------------------------------------------------------------------------------------------------
	// Release a command data previously acquired for read-only
	void ReleaseReadOnlyAccess() const
	{
		sxAssertNoText(IsReadOnlyAcquired() == true);
		m_iaAcquiredCounter--;
	}

	//----------------------------------------------------------------------------------------------------------
	// Acquire the command data for read-write
	void AcquireReadWriteAccess()
	{
		sxAssertNoText(IsReadOnlyAcquired() == false && IsReadWriteAcquired() == false);
		m_iaAcquiredCounter--;

		// Flag the data as modified as it has been locked with a write access
		m_bRebuildNeeded = true;
	}

	//----------------------------------------------------------------------------------------------------------
	// Release a command data previously acquired for read-write
	void ReleaseReadWriteAccess()
	{
		sxAssertNoText(IsReadWriteAcquired() == true);
		m_iaAcquiredCounter++;
	}

	// sxCPacket is friend as it tests and reset the building flag
	friend class sxCPacket;
	
	//----------------------------------------------------------------------------------------------------------
	// Test if a command data needs to be rebuilt. It is the case everytime it is modified, ie: acquired for
	// read-write. This flag is only tested from the server thread while the command data is acquired for 
	// read-only.
	sxBool IsRebuildNeeded() const
	{
		sxAssertNoText(IsReadOnlyAcquired());
		return m_bRebuildNeeded;
	}

	//----------------------------------------------------------------------------------------------------------
	// Reset rebuilt flag. Called from the server thread while the command data is acquired for read-only.
	void Rebuilt() const
	{
		sxAssertNoText(IsReadOnlyAcquired());
		sxAssertNoText(m_bRebuildNeeded == true);
		m_bRebuildNeeded = false;
	}

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Acuisition policy counter. It's mutable as read acquisitions are const but modifies this counter.
	mutable sxCAtomicInt m_iaAcquiredCounter;

	// Data modification flag. It's mutable as reset is called from the server on a const data.
	mutable sxBool m_bRebuildNeeded;
};

// Command data smart pointer type
typedef sxCTIntrusivePtr<sxICommandData>::sxCType sxICommandDataPtr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver client command object interface
class sxICommand : protected sxIObject
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxICommand(sxICommandData& a_rData, sxEFactoryObject::sxEValue a_eValue);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxICommand();

private:

	//-------------------------------------------------------------------------------------------------------
	// Allocate a data object. Delegated to derived classes as data type isn't known here
	virtual sxICommandData& AllocateData() = 0;

	// _sxCCommandLocker is friend in order to access the GetReadOnlyData and GrabReadWriteData functions
	friend class _sxCCommandLocker;
	
	//-------------------------------------------------------------------------------------------------------
	// Allow access to the server command in the private scope
	class sxIServerCommand& GetServerCommand() const { return (sxIServerCommand&)GetServerObject(); }

	//-------------------------------------------------------------------------------------------------------
	// Get a readable data of that command
	sxICommandData const& GetReadOnlyData() const;

	//-------------------------------------------------------------------------------------------------------
	// Get a writable data of that command. If a_bDiscard is true, then the data is initialized with default 
	// values, otherwise it maintains previous ones.
	sxICommandData& GrabReadWriteData(sxBool a_bDiscard);

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// List of the managed sxICommandData. Holds a reference via a smart pointer.
	typedef list<sxICommandDataPtr> sxCCommandDataList;
	sxCCommandDataList m_lCommandData;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Command wrapper object that implements sxICommand virtual and shared code through the use of the 
// t_CommandData template type.
template<typename t_CommandData, sxEFactoryObject::sxEValue t_eValue>
class sxCTCommandWrapper : public sxICommand
{
public:

	// Locker types definitions
	typedef sxCTReadOnlyLocker<t_CommandData> sxCReadOnlyLocker;
	typedef sxCTReadWriteLocker<t_CommandData> sxCReadWriteLocker;

	//-------------------------------------------------------------------------------------------------------
	// Constructor, Allocate the initial data to be passed to the command interface
	sxCTCommandWrapper() :
		sxICommand(AllocateData(), t_eValue)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Releases all command data references
	virtual ~sxCTCommandWrapper()
	{
	}

private:

	//-------------------------------------------------------------------------------------------------------
	// Implement data object allocation command virtual function.
	virtual sxICommandData& AllocateData() {return sxNew(t_CommandData);}
};
