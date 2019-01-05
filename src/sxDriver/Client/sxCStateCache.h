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

#include "sxEStateType.h"
#include "sxCCommandLocker.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement an entry in the state cache. Inherit sxCReadOnlyLockerHolder class in order to keep track of 
// server command references and acquiring command data.
class sxCCacheEntry : public sxCReadOnlyLockerHolder
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize the entry from a state object
	sxCCacheEntry(sxIState const& a_rState);

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor.
	sxCCacheEntry(sxCCacheEntry const& a_rEntry);

	//-------------------------------------------------------------------------------------------------------
	// Destructor.
	~sxCCacheEntry() {}

	//-------------------------------------------------------------------------------------------------------
	// Compare the entry to a_rState state object. This function test if the state object and data stored in
	// the entry is the same as the one passed as argument. It does not use a per member comparison but rather
	// does a very fast test based on command data management policy (See implementation for more details).
	sxBool operator != (sxIState const& a_rState);

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator from a state object.
	void operator = (sxIState const& a_rState);

	//-------------------------------------------------------------------------------------------------------
	// Assignment operator.
	void operator = (sxCCacheEntry const& a_rEntry);

	//-------------------------------------------------------------------------------------------------------
	// Queue an entry to the Command Buffer passed as argument
	void Queue(class sxCCommandBuffer& a_rCommandBuffer) const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver state cache
class sxCStateCache : private sxINonCopyable
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor
	sxCStateCache();

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	~sxCStateCache();

	//-------------------------------------------------------------------------------------------------------
	// Binds a_rState to the cache. Returns true if the cache entry was not out-dated yet
	sxBool Bind(sxIState& a_rState);

	//-------------------------------------------------------------------------------------------------------
	// Reset a_eStateType entry to its default state. Returns true if the cache entry was not out-dated yet.
	sxBool Reset(sxEStateType::sxEValue a_eStateType);
	
	//-------------------------------------------------------------------------------------------------------
	// Reset all entries to their default state. Returns the number of states that wasn't out-dated yet.
	sxUInt Reset();

protected:

	//-------------------------------------------------------------------------------------------------------
	// Flushes all out-dated states to the command buffer
	sxUInt Flush(class sxCCommandBuffer& a_rCommandBuffer);

private:

	//-------------------------------------------------------------------------------------------------------
	// Modify an entry in the cache. Maintains internal structures coherency.
	sxBool SetEntry(sxEStateType::sxEValue a_eValue, sxIState& a_rState);

	// _sxRegisterDefaultState is friend in order to use default state constructor registration
	template<typename, sxEStateType::sxEValue>
	friend void _sxRegisterDefaultState(sxERegisteredOperation::EValue);

	// Type of the default state constructor
	typedef sxIState& (*sxPFnStateDefaultConstrutor)();

	//-------------------------------------------------------------------------------------------------------
	// Default state constructor registration function
	static void RegisterDefaultState(	sxEStateType::sxEValue a_eStateType,
										sxPFnStateDefaultConstrutor a_pFnConstructor);

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// State cache entries
	typedef vector<sxCCacheEntry> sxCEntryVector;
	sxCEntryVector m_voEntries;

	// Outdated entries boolean flag
	typedef vector<sxBool> sxCOutdatedEntries;
	sxCOutdatedEntries m_vbOutdatedEntries;

	// Vector of entries to be flushed
	typedef vector<sxUInt> sxCFlushableEntries;
	sxCFlushableEntries m_vuiFlushableEntries;

	// Default states
	typedef vector<sxIState*> sxCDefaultVector;
	sxCDefaultVector m_vpDefaultStates;

	// Registered default state constructors
	static sxPFnStateDefaultConstrutor ms_aDefaultStateConstructor[sxEStateType::eValuesCount];
};
