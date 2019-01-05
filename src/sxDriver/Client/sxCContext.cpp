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
#include "sxCContext.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCContext::sxCContext(sxCDevice& a_rDevice) :
    sxCCommandBuffer(a_rDevice),
    sxCStateCache()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCContext::~sxCContext()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCContext::Queue(sxICommand& a_rCommand)
{
    // First flush the state cache such that all out-dated cache entries will be queued before the command 
    // itself. This is important as the command relies on the state cache current state.
    Flush(rThis);

    // Forwards to the command buffer
    sxCCommandBuffer::Queue(a_rCommand);
}

//-------------------------------------------------------------------------------------------------------
//
void sxCContext::End()
{
    // Reset state chache to default. This clear the cache from any "user" state.
    Reset();

    // Forwards end to the command buffer
    sxCCommandBuffer::End();
}
