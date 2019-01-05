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

#include "sxCCommandBuffer.h"
#include "sxCStateCache.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Driver rendering context object. Implement a rendering state machine context and its associated command
// buffer and server.
class sxCContext : public sxCCommandBuffer, public sxCStateCache
{
public:

	//-------------------------------------------------------------------------------------------------------
	// Constructor. Initialize the command buffer and the state cache 
	sxCContext(sxCDevice& a_rDevice);

	//-------------------------------------------------------------------------------------------------------
	// Destructor
	~sxCContext();

	//-------------------------------------------------------------------------------------------------------
	// Queue the command into the command buffer. State cache is internally flush before the command is 
	// effectively queued.
	void Queue(class sxICommand& a_rCommand);

	//-------------------------------------------------------------------------------------------------------
	// Notify context queuing/binding is ending. Queuing must be began before calling this function. This
	// overrides the command buffer function in order to reset the state cache.
	virtual void End();

private:

	//-------------------------------------------------------------------------------------------------------
	// Disable State object direct queuing at compilation time by declaring the function as private
	void Queue(class sxIState&);
};
