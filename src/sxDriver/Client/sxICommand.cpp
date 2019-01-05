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
#include "sxICommand.h"
#include "sxCCommandBuffer.h"

#include "sxCCommandLocker.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxICommandData

//-------------------------------------------------------------------------------------------------------
//
sxICommandData::sxICommandData() :
	m_iaAcquiredCounter(0),
	m_bRebuildNeeded(true)
{
}

//-------------------------------------------------------------------------------------------------------
//
sxICommandData::~sxICommandData()
{
	sxAssert(IsAcquired() == false, "Deleting an acquired command data");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxICommand

//-------------------------------------------------------------------------------------------------------
//
sxICommand::sxICommand(sxICommandData& a_rData, sxEFactoryObject::sxEValue a_eValue) :
	sxIObject(a_eValue)
{
	// Initialize with initial data
	m_lCommandData.push_front(&a_rData);
}

//-------------------------------------------------------------------------------------------------------
//
sxICommand::~sxICommand()
{
	// Clear the list which automatically release smart pointers and destroy data
	m_lCommandData.clear();
}

//-----------------------------------------------------------------------------------------------------------
//
sxICommandData const& sxICommand::GetReadOnlyData() const
{
	// Test entry conditions
	sxAssertNoText(m_lCommandData.empty() == false);

	// The front of the list data is always the most recent
	sxICommandData const& rData = *m_lCommandData.front();
	sxAssert(	rData.IsReadWriteAcquired() == false,
				"This data is already locked for write on the client side");

	// Return the readable data
	return rData;
}

//-----------------------------------------------------------------------------------------------------------
//
sxICommandData& sxICommand::GrabReadWriteData(sxBool a_bDiscard)
{
	// Test entry conditions
	sxAssertNoText(m_lCommandData.empty() == false);

	//---------------------------------------------------------------------------------------
	// Stores the latest used data as it might be required to copy it to a newly allocated one.
	sxICommandData& rLastestData = sxDereference(m_lCommandData.front());

	// ----------------------------------------------------------------------------------
	// Iterate through all data in order to get one that isn't acquired for read purpose.
	// The first one would be great as it could avoid calling the copy.
	sxCCommandDataList::iterator oIterator = m_lCommandData.begin();
	for(; oIterator != m_lCommandData.end(); ++oIterator)
	{
		// Fetch iterated data
		sxICommandData& rData = sxDereference(*oIterator);

		// Test if the data will be acquire-able for writing, ie: not acquired for reading
		if(rData.IsReadOnlyAcquired() == false)
		{
			// No need to iterate further, got it...
			break;
		}
	}
	
	//------------------------------------------------------
	// Allocate a new data if none was available in the list

	// Will store the grabbed data
	sxICommandData* pGrabbedData = NULL;

	// Was a data found in the list
	if(oIterator == m_lCommandData.end())
	{
		// No data available so ask for a new data allocation
		pGrabbedData = &AllocateData();
	}
	else
	{
		// Use the one found in the list
		pGrabbedData = *oIterator;
	}

	// Dereference the found data in order to assert its validity
	sxICommandData& rGrabbedData = sxDereference(pGrabbedData);

	//-----------------------------------------------------------------------
	// Now update data list in order to maintain the latest data at the front

	// Skip this if the data is already at the front
	if(oIterator != m_lCommandData.begin())
	{
		// Push the new data at the front. This is done before removing it from the list because it avoids 
		// using a smart pointer for "pGrabbedData". Indeed its reference counter might falls down to 0 
		// when it is removed from the list.
		m_lCommandData.push_front(&rGrabbedData);

		// Remove it from the list, but only if it was found there
		if(oIterator != m_lCommandData.end())
		{
			m_lCommandData.erase(oIterator);
		}
	}

	//-----------------------------------
	// Now handles "discarding behavior".
	// Copy the data if discard isn't required, otherwise clear it
	if(a_bDiscard == false)
	{
		// Skip copy if source and destination data are the same. It can be the case if the data found in
		// the search loop was the front one. 
		if(&rGrabbedData != &rLastestData)
		{
			rGrabbedData.Copy(rLastestData);
		}
	}
	else
	{
		// Discard is accomplished by clearing the data
		rGrabbedData.Clear();
	}

	// No reason for the data to be acquired so assert it
	sxAssertNoText(rGrabbedData.IsAcquired() == false);

	// All done, return the data that is available for read-write
	return rGrabbedData;
}
