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
#include "sxCCommandBuffer.h"
#include "sxCDevice.h"
#include "sxCDriverStatistics.h"
#include "sxDriver/Server/Interface/sxIServerCommand.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCCommandBuffer::sxCCommandBuffer(	sxCDevice& a_rDevice,
									sxEBufferingMode::sxEValue a_eMode) :
	m_rDevice(a_rDevice),
	m_eMode(a_eMode),
	m_bInsideBeginEnd(false),
	m_bExitServerThread(false),
	m_aBuffers(NULL),
	m_uiBufferCount(0),
	m_oClientFpsCounter(0.5f),
	m_oServerFpsCounter(0.5f)
{
	// Create server thread
	m_spServerThread = sxCThread::Create(	"Server",
											&sxCCommandBuffer::ServerEntryFunction,
											this);

	// Initialize buffers for the selected mode
	InitializeMode(m_eMode);
}

//-----------------------------------------------------------------------------------------------------------
//
sxCCommandBuffer::~sxCCommandBuffer()
{
	// Test entry condition
	sxAssert(m_bInsideBeginEnd == false, "Cannot not be called inside a being/end pair");

	// All the buffers must have been released to the client before going further
	WaitAllBuffersFlushed();

	// Create a scope in order to request server thread exit and release mutex before the Join call.
	{
		sxCMutexLocker oLocker(m_oQueuingMutex);

		// Flag that exit's requested
		m_bExitServerThread = true;

		// Notify server thread that exit is requested
		m_oQueuingCondition.NotifyOne();
	}

	// Wait for the server thread to exit
	m_spServerThread->Join();

	// Clean all buffers
	DeallocateBuffers();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::Queue(sxICommand const& a_rCommand)
{
	// Test entry condition
	sxAssert(m_bInsideBeginEnd == true, "Can only be called inside a begin/end pair");

	// Build the packet with the command to be queued, the data is obtained during the packet construction
	sxCPacket oPacket(a_rCommand);

	// Queue the packet
	_Queue(oPacket);
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::Queue(sxCReadOnlyLockerHolder const& a_rLockerHolder)
{
	// Test entry condition
	sxAssert(m_bInsideBeginEnd == true, "Can only be called inside a begin/end pair");

	// Build the packet with the server command object (matching a client state object) and its data
	sxCPacket oPacket(a_rLockerHolder);

	// Queue the packet
	_Queue(oPacket);
}

//-------------------------------------------------------------------------------------------------------
//
inline void sxCCommandBuffer::_Queue(sxCPacket& a_rPacket)
{
	// Immediately dispatch in synchronous mode, otherwise queue to the buffer
	if(m_eMode == sxEBufferingMode::eNoBuffering)
	{
		// Dispatch this packet to the device
		a_rPacket.Dispatch(m_rDevice);
	}
	else
	{
		// Fetch current buffer
		sxCBuffer& rBuffer = GetBufferToFill();

		// Queue the packet
		rBuffer.QueuePacket(a_rPacket);
	}
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::SetBufferingMode(sxEBufferingMode::sxEValue a_eMode)
{
	// Test entry condition
	sxAssert(m_bInsideBeginEnd == false, "Cannot be called inside a being/end pair");

	// Early out if mode are equals
	if(m_eMode == a_eMode)
	{
		return;
	}

	// All the buffers must have been released to the client before going further
	WaitAllBuffersFlushed();

	// Stores new mode
	m_eMode = a_eMode;

	// Initialize buffers for this new mode
	InitializeMode(m_eMode);
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCCommandBuffer::Begin()
{
	// Test entry condition
	sxAssert(m_bInsideBeginEnd == false, "Already inside a being/end pair");

	//-----------------------------------------------------------------
	// Handle device reset
	if(m_rDevice.IsResetNeeded())
	{
		// Reset is done synchronously so first all buffers are flushed in order to stop the flushing thread
		WaitAllBuffersFlushed();

		// Then reset is done
		sxBool bReseted = m_rDevice.Reset();

		// Return immediately if reset has failed
		if(bReseted == false)
		{
			return false;
		}

		// Otherwise, continue
	}

	// Internally flag as inside a begin/end pair
	m_bInsideBeginEnd = true;

	// Early out in synchronous mode
	if(m_eMode == sxEBufferingMode::eNoBuffering)
	{
		return true;
	}

	//-----------------------------------------------------------------
	// In any asynchronous mode, ensures a buffer is ready to be filled

	// Lock the mutex before accessing buffers
	sxCMutexLocker oLocker(m_oQueuingMutex);

	switch(m_eMode)
	{
		case sxEBufferingMode::eDoubleBuffer:
		{
			//-----------------------------------
			// Waits for a buffer to be available
			// Buffer will be released by the server and pushed into the client list
			while(m_lpClientBuffers.empty() == true)
			{
				m_oQueuingCondition.Wait(oLocker);
			}
		}break;
		case sxEBufferingMode::eTripleBuffer:
		{
			// Test if a buffer is available
			if(m_lpClientBuffers.empty() == true)
			{
				//----------------------------------------------
				// Discards the oldest buffer sent to the server

				// Get the last server buffer
				sxCBuffer& rBuffer = sxDereference(m_lpServerBuffers.back());

				// Remove it from the server list
				m_lpServerBuffers.pop_back();

				// Clear it
				rBuffer.Clear();

				// Release the buffer to the client
				m_lpClientBuffers.push_front(&rBuffer);
			}
		}break;
		default: sxAssertUnreachableCode(); break;
	}

	// At least one buffer is available in the client list
	sxAssertNoText(m_lpClientBuffers.empty() == false);

	//----------------------------------------------------------
	// Then use the front of the list as the buffer to be filled
	
	// Buffer to fill should be empty at this point
	sxAssertNoText(GetBufferToFill().IsEmpty());

	// Everything's fine
	return true;
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::End()
{
	// Test entry condition
	sxAssert(m_bInsideBeginEnd == true, "Not inside a being/end pair");

	// Internally flag as outside of a begin/end pair
	m_bInsideBeginEnd = false;

	//--------------------------------
	// Compute fps between two End() calls
	m_oClientFpsCounter.Update();

	// Update statistics
	sxCDriverStatistics::GetInstance().SetClientFps(m_oClientFpsCounter.GetFramePerSecond());

	// Early out in synchronous mode
	if(m_eMode == sxEBufferingMode::eNoBuffering)
	{
		return;
	}

	//---------------------------------------------------------------------------------
	// In any asynchronous mode, forwards the buffer that was just filled to the server
	{
		// Lock the mutex before accessing buffers
		sxCMutexLocker oLocker(m_oQueuingMutex);

		// Remove buffer from the client list. Last filled buffer was at the front of the list
		sxCBuffer& rBuffer = sxDereference(m_lpClientBuffers.front());
		m_lpClientBuffers.pop_front();

		// Give it to the server. Push it in the front as it is the newest one (the oldest is at the back).
		m_lpServerBuffers.push_front(&rBuffer);

		// Notify the server that it has a new buffer to flush
		m_oQueuingCondition.NotifyOne();
	}
}

//-------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::InitializeMode(sxEBufferingMode::sxEValue a_eMode)
{
	// Clean all buffers
	DeallocateBuffers();

	// Modify buffers count
	m_uiBufferCount = 0;
	switch(a_eMode)
	{
		case sxEBufferingMode::eNoBuffering: m_uiBufferCount = 0; break;
		case sxEBufferingMode::eDoubleBuffer: m_uiBufferCount = 2; break;
		case sxEBufferingMode::eTripleBuffer: m_uiBufferCount = 3; break;
		default: sxAssertUnreachableCode(); break;
	}

	// Allocate buffers
	m_aBuffers = sxNewArray(sxCBuffer, m_uiBufferCount);

	// Transfers all the buffers to the client, ready to be filled
	sxForEachUInt(uiBuffer, m_uiBufferCount)
	{
		// Fetch buffer
		sxCBuffer& rBuffer = m_aBuffers[uiBuffer];

		// Push it in the list
		m_lpClientBuffers.push_front(&rBuffer);
	}

	// Reset driver statistics
	sxCDriverStatistics::GetInstance().Reset();

}

//-----------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::DeallocateBuffers()
{
	// Test entry condition
	sxAssert(m_bInsideBeginEnd == false, "Cannot not be called inside a being/end pair");

	// Test destruction conditions
	sxAssert(		m_lpClientBuffers.size() == m_uiBufferCount
				&&	m_lpServerBuffers.size() == 0,
				"All buffers mus have been flushed prior to calling this function");

	// Empties client list such that no more buffer is referenced
	m_lpClientBuffers.clear();

	// Deallocate buffers
	sxDeleteArray(m_aBuffers);
}

//-------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::WaitAllBuffersFlushed()
{
	// Get inside the mutex sxope
	sxCMutexLocker oLocker(m_oQueuingMutex);

	// Loop until all buffers are flushed, ie: all in the client list
	while(m_lpClientBuffers.size() != m_uiBufferCount)
	{
		m_oQueuingCondition.Wait(oLocker);
	}
}

//-------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::ServerEntryFunction(void* a_pvArgument)
{
	// Fetch sxCCommandBuffer instance
	sxCCommandBuffer& rCommandBuffer = sxDereference((sxCCommandBuffer*)a_pvArgument);

	// Forward to non static function
	rCommandBuffer.ServerFlushFunction();
}

//-------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::ServerFlushFunction()
{
	// Fetch statistics object
	sxCDriverStatistics& rStatistics = sxCDriverStatistics::GetInstance();

	// Loop until exit is asked (exit is tested inside the mutex scope).
	while(true)
	{
		// Execute every tests inside the mutex
		sxCMutexLocker oLocker(m_oQueuingMutex);

		//---------------------------------
		// Loop while there's nothing to do
		while(		m_bExitServerThread == false
				&&	m_lpServerBuffers.empty() == true)
		{
			// Wait for a notification
			m_oQueuingCondition.Wait(oLocker);
		}

		// Is exit requested
		if(m_bExitServerThread == true)
		{
			// Then exit the continuous while loop
			break;
		}

		//---------------------------------------------------
		// Pick up the newest buffer in the list and flush it
		sxCBuffer& rBuffer = PickBufferToFlush(oLocker);

		// Unlock mutex during server execution of rBuffer.
		// Indeed, command buffer dispatching runs in parallel ie: outside of the mutex scope
		oLocker.Unlock();
			rBuffer.Dispatch(m_rDevice);
		oLocker.Lock();

		// Release the buffer to the client
		// Pushes rBuffer at the back of the list, as the front is used for the filled buffer
		m_lpClientBuffers.push_back(&rBuffer);

		// Notify a buffer was released to the client
		m_oQueuingCondition.NotifyOne();

		//--------------------------------
		// Compute fps between two Dispatch() calls
		m_oServerFpsCounter.Update();

		// Update statistics
		rStatistics.SetServerFps(m_oServerFpsCounter.GetFramePerSecond());
	}

	//----------------------------------------
	// Clear all server buffers before exiting

	// Execute discard inside the mutex
	sxCMutexLocker oLocker(m_oQueuingMutex);

	// Discard all buffers
	DiscardAllServerBuffers(oLocker);
}

//-------------------------------------------------------------------------------------------------------
//
sxCBuffer& sxCCommandBuffer::PickBufferToFlush(sxCMutexLocker& a_rLocker)
{
	// Test entry condition
	sxAssert(a_rLocker.IsLocked(), "Mutex should be locked");

	// As no buffer is picked yet, integrity can be checked
	sxAssertNoText(m_lpClientBuffers.size() + m_lpServerBuffers.size() == m_uiBufferCount);

	// Select buffer according to the mode
	sxCBuffer* pBuffer = NULL;
	switch(m_eMode)
	{
		case sxEBufferingMode::eDoubleBuffer:
		{
			//--------------------------------------------------------------
			// Picks up the buffer to flush which is at the back of the list
			// It's very unlikely that the other buffer is already ready to be flushed

			// Fetch the buffer
			pBuffer = m_lpServerBuffers.back();

			// No longer in the server list
			m_lpServerBuffers.pop_back();
		}break;
		case sxEBufferingMode::eTripleBuffer:
		{
			//-----------------------------------------------
			// Picks up the newest buffer and discard oldests
			// New buffers are pushed at front, so the oldest is at the back
			pBuffer = m_lpServerBuffers.front();

			// This one is no longer in the server list
			m_lpServerBuffers.pop_front();

			// Discard other buffers
			DiscardAllServerBuffers(a_rLocker);

		}break;
		default: sxAssertUnreachableCode(); break;
	}

	// A buffer should have been found
	sxAssertNoText(pBuffer != NULL);

	// Return selected buffer
	return *pBuffer;
}

//-------------------------------------------------------------------------------------------------------
//
void sxCCommandBuffer::DiscardAllServerBuffers(sxCMutexLocker& a_rLocker)
{
	// Test entry condition
	sxAssert(a_rLocker.IsLocked(), "Mutex should be locked");

	// Loop and empty server buffers
	while(m_lpServerBuffers.empty() == false)
	{
		// Fetch buffer
		sxCBuffer& rBuffer = sxDereference(m_lpServerBuffers.back());
		
		// Remove it from the server list
		m_lpServerBuffers.pop_back();

		// Clear the selected buffer outside mutex lock
		a_rLocker.Unlock();
			rBuffer.Clear();
		a_rLocker.Lock();

		//---------------------------------
		// Release the buffer to the client
		// Pushes at the back of the list as the front is used as the filled buffer
		m_lpClientBuffers.push_back(&rBuffer);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCPacket

//-------------------------------------------------------------------------------------------------------
//
inline void sxCPacket::Dispatch(sxCDevice& a_rDevice)
{
	// Fetch server device to use for command Build and Dispatch
	sxIServerDevice& rServerDevice = a_rDevice.GetServerDevice();

	// Fetch statistics object
	sxCDriverStatistics& rStatistics = sxCDriverStatistics::GetInstance();
	
	// Fetch server command to Build and Dispatch
	sxIServerCommand& rServerCommand = GetServerCommand();

	// Fetch Command Data to be used for Build and Dispatch
	sxICommandData const& rCommandData = GetData();

	// Build the server object if needed
	if(rCommandData.IsRebuildNeeded() == true)
	{
		// Build the command, passing the data as argument
		rServerCommand.Build(rCommandData, rServerDevice);

		// Notify the data that it was built
		rCommandData.Rebuilt();

		// Increment statistic counter
		rStatistics.IncBuiltCommandsPerFrame();
	}

	// And finally dispatch it, still passing the data as argument
	rServerCommand.Dispatch(rCommandData, rServerDevice);

	// Increment dispatched command count
	rStatistics.IncDispatchedCommandsPerFrame();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sxCBuffer

//-----------------------------------------------------------------------------------------------------------
//
sxCBuffer::sxCBuffer()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCBuffer::~sxCBuffer()
{
	sxAssertNoText(m_lPackets.empty());
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCBuffer::Dispatch(sxCDevice& a_rDevice)
{
	// Iterate through each packet and dispatch it
	sxForEach(oPacketIterator, m_lPackets, sxCPacketList)
	{
		// Fetch packet
		sxCPacket& rPacket = *oPacketIterator;

		// Forward the dispatch
		rPacket.Dispatch(a_rDevice);
	}

	// Fetch statistics object
	sxCDriverStatistics& rStatistics = sxCDriverStatistics::GetInstance();

	// Sets command buffer size
	rStatistics.SetCommandBufferSize(m_lPackets.size() * sizeof(sxCPacket));

	// Clear buffer once it has been dispatched
	Clear();
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCBuffer::Clear()
{
	m_lPackets.clear();
}

//-----------------------------------------------------------------------------------------------------------
//
sxBool sxCBuffer::IsEmpty()
{
	return m_lPackets.empty();
}
