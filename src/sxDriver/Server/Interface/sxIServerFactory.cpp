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
#include "sxIServerFactory.h"
#include "sxDriver/Client/sxCFactorySelector.h"

//-------------------------------------------------------------------------------------------------------
//
sxIServerFactory::sxIServerFactory(sxEGraphicApi::sxEValue a_eApi)
{
    // Get factory selector object that support registration functions
    sxCFactorySelector& rSelector = sxCFactorySelector::GetInstance();

    // Register this factory
    rSelector.RegisterFactory(a_eApi, rThis);
}

//-------------------------------------------------------------------------------------------------------
//
sxIServerFactory::~sxIServerFactory()
{
}
