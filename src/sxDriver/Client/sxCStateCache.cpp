//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
//                                                                                                          |
//                              ShaderX7. Cross platform rendering thread. 2008.                            |
//              Guillaume Blanc. ELB (Etranges Libellules) http://www.elb-games.com/. Lyon, France.         |
//                                                                                                          |
//  This program is free software. It is distributed in the hope that it will be useful, but without any    |
//  warranty, without even the implied warranty of merchantability or fitness for a particular purpose.     |
//                                                                                                          |
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
#include "pch.h"
#include "sxCStateCache.h"
#include "sxCCommandBuffer.h"
#include "sxIState.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCStateCache

//-----------------------------------------------------------------------------------------------------------
// Declare and initialize ms_aDefaultStateConstructor entries to NULL
sxCStateCache::sxPFnStateDefaultConstrutor sxCStateCache::ms_aDefaultStateConstructor[sxEStateType::eValuesCount];

//-----------------------------------------------------------------------------------------------------------
//
sxCStateCache::sxCStateCache()
{
    // Allocate and initialize default state vector
    sxForEachEnum(eValue, sxEStateType)
    {
        // Get back function pointer
        sxPFnStateDefaultConstrutor pFnDefaultConstructor = ms_aDefaultStateConstructor[eValue];
        sxAssert(pFnDefaultConstructor != NULL, "No registered state for state type %d", eValue);

        // Calls state allocation
        sxIState& rState = (*pFnDefaultConstructor)();

        // Stores it in the default state collection
        m_vpDefaultStates.push_back(&rState);
        sxAssertNoText(m_vpDefaultStates[eValue] == &rState);

        // Also set the corresponding entry with the default value
        m_voEntries.push_back(sxCCacheEntry(rState));
        m_vuiFlushableEntries.push_back(eValue);
        m_vbOutdatedEntries.push_back(true);
    }
}

//-----------------------------------------------------------------------------------------------------------
//
sxCStateCache::~sxCStateCache()
{
    // Empties the state cache entries
    m_voEntries.clear();

    // Iterate and delete default states
    sxForEachEnum(eValue, sxEStateType)
    {
        // Fetch state object
        sxIState& rState = sxDereference(m_vpDefaultStates[eValue]);

        // Delete it
        sxDelete(rState);
        m_vpDefaultStates[eValue] = NULL;
    }
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCStateCache::Bind(sxIState& a_rState)
{
    // Fetch state type
    sxEStateType::sxEValue eValue = a_rState.GetStateType();

    // Fetch state entry
    sxCCacheEntry& rEntry = m_voEntries[eValue];

    // Compare the current entry with the new one
    if(rEntry != a_rState)
    {
        // Then update the entry
        SetEntry(eValue, a_rState);

        // Entry was modified
        return true;
    }

    // Entry wasn't modified
    return false;
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCStateCache::Reset(sxEStateType::sxEValue a_eStateType)
{
    // Fetch default state
    sxIState& rState = sxDereference(m_vpDefaultStates[a_eStateType]);

    // Bind it
    return Bind(rState);
}

//-----------------------------------------------------------------------------------------------------------
//
inline sxBool sxCStateCache::SetEntry(sxEStateType::sxEValue a_eValue, sxIState& a_rState)
{
    // Detect if entry was already out-dated
    sxBool bAlreadyOutdated = m_vbOutdatedEntries[a_eValue];

    // If entry isn't out-date yet, then do it
    if(bAlreadyOutdated == false)
    {
        // Flag entry as out-dated
        m_vbOutdatedEntries[a_eValue] = true;

        // Tells that this entry is now flushable
        m_vuiFlushableEntries.push_back(a_eValue);
    }

    // Finally stores entry value
    m_voEntries[a_eValue] = a_rState;

    return (bAlreadyOutdated == false);
}

//-------------------------------------------------------------------------------------------------------
//
sxUInt sxCStateCache::Flush(sxCCommandBuffer& a_rCommandBuffer)
{
    // All the flushable states will be queued
    sxUInt uiQueuedStates = m_vuiFlushableEntries.size();

    // Iterate through all out-dated states
    sxForEach(itFlushable, m_vuiFlushableEntries, sxCFlushableEntries)
    {
        // Fetch state index to flush
        sxUInt const uiFlushable = *itFlushable;

        // Reset state cache internal flags
        sxAssertNoText(m_vbOutdatedEntries[uiFlushable] == true);
        m_vbOutdatedEntries[uiFlushable] = false;

        // Fetch the state to flush
        sxCCacheEntry& rEntry = m_voEntries[uiFlushable];

        // Queue that entry to the command buffer
        rEntry.Queue(a_rCommandBuffer);
    }

    // No more state to flush
    m_vuiFlushableEntries.clear();

    return uiQueuedStates;
}


//-----------------------------------------------------------------------------------------------------------
//
sxUInt sxCStateCache::Reset()
{
    // Will count bound states for the returned value
    sxUInt uiBoundState = 0;

    // Iterate and bind default state
    sxForEachEnum(eValue, sxEStateType)
    {
        // Bind it as the current
        uiBoundState += Reset(eValue);
    }

    return uiBoundState;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCStateCache::RegisterDefaultState(   sxEStateType::sxEValue a_eStateType,
                                            sxPFnStateDefaultConstrutor a_pFnConstructor)
{
    sxAssertNoText(a_pFnConstructor != NULL);

    // Ensures it wasn't registered yet
    sxAssert(   ms_aDefaultStateConstructor[a_eStateType] == NULL,
                "State %d is already registered", a_eStateType);

    // Register it
    ms_aDefaultStateConstructor[a_eStateType] = a_pFnConstructor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCCacheEntry

//-----------------------------------------------------------------------------------------------------------
//
sxCCacheEntry::sxCCacheEntry(sxIState const& a_rState) :
    sxCReadOnlyLockerHolder(a_rState)
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCCacheEntry::sxCCacheEntry(sxCCacheEntry const& a_rEntry) :
    sxCReadOnlyLockerHolder(a_rEntry)
{
}

//-----------------------------------------------------------------------------------------------------------
//
inline sxBool sxCCacheEntry::operator != (sxIState const& a_rState)
{
    // Build a new entry in order to be able to compare
    sxCCacheEntry oEntry(a_rState);

    // Compare both the server command and data pointers in order to detect if the state is the same that the
    // one already bound to the cache.
    // - In asynchronous modes, as the state cache is reseted every frame, and as the data is duplicated if it
    // is modified, then strict pointer comparison is enough to detect that the state has a new value.
    // - In synchronous mode (no buffering), the data is duplicated if the state is already in the cache (as
    // its read counter is incremented in that case). This means that the server command pointer comparison 
    // is a valid test in this mode.
    return      (&GetServerCommand() != &oEntry.GetServerCommand())
            ||  (&GetData() != &oEntry.GetData());
}

//-----------------------------------------------------------------------------------------------------------
//
inline void sxCCacheEntry::Queue(sxCCommandBuffer& a_rCommandBuffer) const
{
    // Forward queuing to the command buffer
    a_rCommandBuffer.Queue(rThis);
}

//-----------------------------------------------------------------------------------------------------------
//
inline void sxCCacheEntry::operator = (sxIState const& a_rState)
{
    // Build a new entry from the state object  in order to be able to copy it to this
    sxCCacheEntry oEntry(a_rState);

    // Calls copy operator
    rThis = oEntry;
}

//-----------------------------------------------------------------------------------------------------------
//
inline void sxCCacheEntry::operator = (sxCCacheEntry const& a_rEntry)
{
    // Calls inherited operator
    sxCReadOnlyLockerHolder::operator = (a_rEntry);
}
