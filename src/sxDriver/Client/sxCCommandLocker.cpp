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

#include "sxCCommandLocker.h"
#include "sxICommand.h"
#include "sxIState.h"

//-----------------------------------------------------------------------------------------------------------
//
sxICommandData const& _sxCCommandLocker::GetReadOnlyData(sxICommand const& a_rCommand) const
{
    return a_rCommand.GetReadOnlyData();
}

//-----------------------------------------------------------------------------------------------------------
//
sxICommandData& _sxCCommandLocker::GrabReadWriteData(sxICommand& a_rCommand, sxBool a_bDiscard) const
{
    return a_rCommand.GrabReadWriteData(a_bDiscard);
}

//-----------------------------------------------------------------------------------------------------------
//
sxICommandData const& _sxCCommandLocker::GetReadOnlyData(sxIState const& a_rState) const
{
    return a_rState.GetCommand().GetReadOnlyData();
}

//-----------------------------------------------------------------------------------------------------------
//
sxICommandData& _sxCCommandLocker::GrabReadWriteData(sxIState& a_rState, sxBool a_bDiscard) const
{
    return a_rState.GetCommand().GrabReadWriteData(a_bDiscard);
}

//-----------------------------------------------------------------------------------------------------------
//
void _sxCCommandLocker::AcquireReadOnlyAccess(sxICommandData const& a_rData) const
{
    a_rData.AcquireReadOnlyAccess();
}

//-----------------------------------------------------------------------------------------------------------
//
void _sxCCommandLocker::ReleaseReadOnlyAccess(sxICommandData const& a_rData) const
{
    a_rData.ReleaseReadOnlyAccess();
}

//-----------------------------------------------------------------------------------------------------------
//
void _sxCCommandLocker::AcquireReadWriteAccess(sxICommandData& a_rData) const
{
    a_rData.AcquireReadWriteAccess();
}

//-----------------------------------------------------------------------------------------------------------
//
void _sxCCommandLocker::ReleaseReadWriteAccess(sxICommandData& a_rData) const
{
    a_rData.ReleaseReadWriteAccess();
}

//-----------------------------------------------------------------------------------------------------------
//
sxIServerCommand& _sxCCommandLocker::GetServerCommand(sxICommand const& a_rCommand) const
{
    return a_rCommand.GetServerCommand();
}

//-----------------------------------------------------------------------------------------------------------
//
sxIServerCommand& _sxCCommandLocker::GetServerCommand(sxIState const& a_rState) const
{
    return a_rState.GetCommand().GetServerCommand();
}
