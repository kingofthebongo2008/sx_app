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

#include "sxKernel/Thread/sxCThread.h"
#include "sxKernel/Thread/sxCCondition.h"
#include "sxKernel/Thread/sxCMutex.h"
#include "sxDriver/Client/sxCCommandLocker.h"
#include "sxDriver/Client/sxICommand.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a command buffer packet, containing the data and the server object to dispatch. Implementation 
// is based on the sxCReadOnlyLockerHolder object.
class sxCPacket : protected sxCReadOnlyLockerHolder
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Construct a packet from a server command and a data already packed into a Lock holder object. It is 
	// used for state queuing as the state object is stored in this two object when it is bound to the cache.
	explicit sxCPacket(sxCReadOnlyLockerHolder const& a_rLockerHolder) :
		sxCReadOnlyLockerHolder(a_rLockerHolder)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Construct a packet from a command.
	explicit sxCPacket(sxICommand const& a_rCommand) :
		sxCReadOnlyLockerHolder(a_rCommand)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Copy constructor
	explicit sxCPacket(sxCPacket const& a_rPacket) :
		sxCReadOnlyLockerHolder(a_rPacket)
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Destructor. It releases every references and acquisitions over the server commands and data.
	~sxCPacket()
	{
	}

	//-------------------------------------------------------------------------------------------------------
	// Dispatch the packet on the server side
	void Dispatch(class sxCDevice& a_rDevice);

private:

	// Hides assignment operator in the private section in order to detect unexpected calls
	void operator = (sxCPacket const& a_rPacket);

	//-------------------------------------------------------------------------------------------------------
	// Default constructor. Should not be used
	sxCPacket() { sxAssertUnreachableCode(); }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement a buffer of packets
class sxCBuffer : sxINonCopyable
{
public:
	
	//-------------------------------------------------------------------------------------------------------
	// Constructs an empty buffer
	sxCBuffer();

	//-------------------------------------------------------------------------------------------------------
	// Destructor. Object must be empty before being destroyed
	~sxCBuffer();

	//-------------------------------------------------------------------------------------------------------
	// Queue a packet to the buffer
	void QueuePacket(sxCPacket const& a_rPacket)
	{
		m_lPackets.push_back(a_rPacket);
	}

	//-------------------------------------------------------------------------------------------------------
	// Dispatch all the packets to the server side
	void Dispatch(class sxCDevice& a_rDevice);

	//-------------------------------------------------------------------------------------------------------
	// Clear all the packets. It releases every references and acquisitions over the server commands and data
	void Clear();

	//-------------------------------------------------------------------------------------------------------
	// Test if buffer is empty
	sxBool IsEmpty();

private:

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// Collection of command buffer packets
	typedef list<sxCPacket> sxCPacketList;
	sxCPacketList m_lPackets;
};

//-------------------------------------------------------------------------------------------------------
// Declare buffering modes enum
sxEnumBegin(sxEBufferingMode)
	eNoBuffering,
	eDoubleBuffer,
	eTripleBuffer,
sxEnumEnd()
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver command buffer
class sxCCommandBuffer : public sxITSingleton<sxCCommandBuffer>
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Default buffering mode is triple buffer.
	sxCCommandBuffer(	class sxCDevice& a_rDevice,
						sxEBufferingMode::sxEValue a_eMode = sxEBufferingMode::eTripleBuffer);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	virtual ~sxCCommandBuffer();

	//-------------------------------------------------------------------------------------------------------
	// Notify command buffer queuing is beginning. Queuing must be ended before calling this function.
	// Return true if begin succeed, otherwise it fails and no queuing could be done. Also End') should not
	// be called if Begin has failed.
	virtual sxBool Begin();

	//-------------------------------------------------------------------------------------------------------
	// Notify command buffer queuing is ending. Queuing must be began before calling this function.
	virtual void End();

	//-------------------------------------------------------------------------------------------------------
	// Modify command buffer execution mode. Must be called outside of the begin end frame as it will wait 
	// for all frames to be flushed.
	void SetBufferingMode(sxEBufferingMode::sxEValue a_eMode);

	//-------------------------------------------------------------------------------------------------------
	// Get command buffer execution mode.
	sxEBufferingMode::sxEValue GetBufferingMode() const {return m_eMode;}

	//-------------------------------------------------------------------------------------------------------
	// Wait until all buffers are flushed and released to the client.
	void WaitAllBuffersFlushed();

protected:

	//-------------------------------------------------------------------------------------------------------
	// Queue the command into the command buffer. State cache is internally flush before the command is 
	// effectively queued.
	void Queue(sxICommand const& a_rCommand);

	// sxCStateCache is friend as it is the only object authorized to queue a state entry
	friend class sxCCacheEntry;
	
	//-------------------------------------------------------------------------------------------------------
	// Queue the state into the command buffer.
	void Queue(sxCReadOnlyLockerHolder const& a_rLockerHolder);

private:

	//-------------------------------------------------------------------------------------------------------
	// Internal function to queue the command into the command buffer.
	void _Queue(sxCPacket& a_rPacket);

	//-------------------------------------------------------------------------------------------------------
	// Internal function that release all buffers.
	void DeallocateBuffers();

	//-------------------------------------------------------------------------------------------------------
	// Internal function that initialize buffers for the specified mode. All buffers must have been flushed 
	// prior to calling this function.
	void InitializeMode(sxEBufferingMode::sxEValue a_eMode);

	//-------------------------------------------------------------------------------------------------------
	// Static server thread entry function, forwards call to ServerFlushFunction.
	static void ServerEntryFunction(void* a_pvArgument);

	//-------------------------------------------------------------------------------------------------------
	// In charge of going through the command buffer, building and dispatching server commands.
	void ServerFlushFunction();

	//-------------------------------------------------------------------------------------------------------
	// Get the buffer to be flushed from the server list. Should be called inside the m_oQueuingMutex lock.
	sxCBuffer& PickBufferToFlush(sxCMutexLocker& a_rLocker);

	//-------------------------------------------------------------------------------------------------------
	// Get the buffer to be filled.
	sxCBuffer& GetBufferToFill()
	{
		// Test entry condition
		sxAssert(m_bInsideBeginEnd == true, "Can only be called inside a begin/end pair");

		// The buffer to fill is always at the front of the client list
		return sxDereference(m_lpClientBuffers.front());
	}

	//-------------------------------------------------------------------------------------------------------
	// Rejects all buffer that are still referenced by the server. Should be called inside the
	// m_oQueuingMutex lock.
	void DiscardAllServerBuffers(sxCMutexLocker& a_rLocker);

	//-------------------------------------------------------------------------------------------------------
	// Attributes

	// The device the buffer in running on
	class sxCDevice& m_rDevice;

	// Command buffer mode, ie synchronous, buffered...
	sxEBufferingMode::sxEValue m_eMode;

	// Is inside begin/end pair
	sxBool m_bInsideBeginEnd;

	// Server thread object
	sxCThreadPtr m_spServerThread;

	// Mutex that rules buffer accesses
	sxCMutex m_oQueuingMutex;

	// Condition that rules buffer swaps
	sxCCondition m_oQueuingCondition;

	// Notifies server thread exit at the end of the application
	sxBool m_bExitServerThread;

	// Collection of command buffer buffers
	sxCBuffer* m_aBuffers;

	// Buffer count
	sxUInt m_uiBufferCount;

	// Collection of buffers to be flushed on the server side. The newest buffer is at the front and the 
	// oldest at the back.
	typedef list<sxCBuffer*> sxCServerBuffers;
	sxCServerBuffers m_lpServerBuffers;

	// Collection of buffers to be filled on the client side. The front of the list is the currently filled
	// buffer (when inside a begin/end pair)
	typedef list<sxCBuffer*> sxCClientBuffers;
	sxCClientBuffers m_lpClientBuffers;

	// Client fps counter
	sxCFpsAutoCounter m_oClientFpsCounter;

	// Server fps counter
	sxCFpsAutoCounter m_oServerFpsCounter;
};
