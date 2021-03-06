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

#ifdef __sxBuildUTest

#include "sxKernel/sxIMain.h"
#include "sxCUTestDevice.h"

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestDevice::sxCUTestDevice()
{
}

//-----------------------------------------------------------------------------------------------------------
//
sxCUTestDevice::~sxCUTestDevice()
{
}

//-----------------------------------------------------------------------------------------------------------
//
void sxCUTestDevice::ForceResetNeeded()
{
    ResetNeeded();
}

#endif // __sxBuildUTest
